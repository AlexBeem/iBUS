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

bool iBus::is_alive()
{
	if(millis() - m_last_iBus_packet < m_timeout)
	{
		return true;
	}
	return false;
}

void iBus::handle()
{
	if(m_ser.available())
	{
		uint8_t c = m_ser.read();

		if(!m_in_packet && c == 0x55)
		{
			m_in_packet = true;
			m_packet_offset = 0;
		}

		if(m_packet_offset >= m_packet_size)
		{
			m_packet_offset = m_packet_size;
			m_in_packet = false;
			if(m_checksum_check(m_packet))
			{
				m_parse_channels(m_packet, m_channel);
				m_last_iBus_packet = millis();
			}
			m_packet[0] = 0x00; // Meant to signify error/no packet
		}
		else
		{
			m_packet[m_packet_offset++] = c; // Save byte and iterate offset
		}
	}
}

bool iBus::m_checksum_check(uint8_t packet[])
{
	int recieved_checksum = packet[30] << 8 | packet[29];
	int ch[14];
	m_parse_channels(packet, ch);

	int sum = 0;
	for(int i=0; i<m_channels_per_packet; i++)
	{
		sum += ch[i];
	}
	return recieved_checksum == sum;
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