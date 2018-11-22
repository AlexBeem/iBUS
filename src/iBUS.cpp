#include <iBUS.h>

iBus::iBus(HardwareSerial& serial):
m_ser(serial)
{
	serial.begin(115200);
	this->m_ser = serial;
}

iBus::iBus(SoftwareSerial& serial):
m_ser(serial)
{
	serial.begin(115200);
	this->m_ser = serial;
}

int iBus::get_channel(int ch)
{
	return m_channel[ch];
}

void iBus::set_channel(int ch, int val)
{
	m_channel_out[ch] = val;
}

bool iBus::is_alive()
{
	if(millis() - m_last_iBus_packet < m_timeout)
	{
		return true;
	}
	return false;
}

void iBus::handle(unsigned int timeout)
{
	uint32_t start = millis();
	while(m_ser.available() && millis()-start < timeout)
	{
		uint8_t c = m_ser.read();

		if(!m_in_packet && c == 0x55)
		{
			m_in_packet = true;
			m_packet_offset = 0;
		}

		if(m_packet_offset >= m_packet_size-1 && m_in_packet)
		{
			m_in_packet = false;
			m_packet[m_packet_size-1] = c;
		
			if(m_checksum_check(m_packet))
			{
				m_parse_channels(m_packet, m_channel);
				m_last_iBus_packet = millis();
			}
					
			m_packet[0] = 0x00; // Meant to signify error/no packet
		}
		
		if(m_in_packet)
		{
			m_packet[m_packet_offset++] = c; // Save byte and iterate offset
		}
	}

	if(is_alive() && millis() - m_last_packet_sent > m_minimum_packet_spacing)
	{
		m_last_packet_sent = millis();
		m_send_packet(m_channel_out);
	}
}

void iBus::m_send_packet(int ch[])
{
	uint8_t buff[1+ m_channels_per_packet*2 +2];
	buff[0] = 0x55;

	for(int i=0; i<m_channels_per_packet; i++)
	{
		buff[i*2+1] = (ch[i] & 0x00FF);
		buff[i*2+2] = (ch[i] >> 8);
	}
	int checksum = m_get_checksum(ch);

	buff[29] = (checksum & 0x00FF);
	buff[30] = (checksum >> 8);

	m_ser.write(buff, 1+ m_channels_per_packet*2 +2);
}

int iBus::m_get_checksum(int ch[])
{
	int sum = 0;
	for(int i=0; i<m_channels_per_packet; i++)
	{
		sum += ch[i];
	}
	return sum;
}

bool iBus::m_checksum_check(uint8_t packet[])
{
	int recieved_checksum = packet[30] << 8 | packet[29];
	int ch[m_channels_per_packet];
	m_parse_channels(packet, ch);

	return (recieved_checksum == m_get_checksum(ch));
}

void iBus::m_parse_channels(uint8_t packet[], int ch[])
{
	for(int i=0; i<m_channels_per_packet; i++)
	{
		ch[i] = packet[i*2+2] << 8 | packet[i*2+1]; // Store get channel values
	}
}

uint32_t iBus::time_since_last()
{
	return millis() - m_last_iBus_packet;
}