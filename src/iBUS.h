#pragma once

#include <Arduino.h>
#include <HardwareSerial.h>
#include <SoftwareSerial.h>

class iBus
{
public:
    iBus(HardwareSerial& serial);
    iBus(SoftwareSerial& serial);
    
    int get_channel(int ch);
    bool is_alive();
    void handle();

    uint32_t time_since_last();

private:

    Stream& m_ser;

    const static int m_packet_size = 31;
    const static int m_channels_per_packet = 14;

    const static int m_timeout = 25; // If no iBUS packet has been recieved in this time, consider the TX off

    uint32_t m_last_iBus_packet = 0;

    uint8_t m_packet[m_packet_size];
    int m_channel[m_channels_per_packet];

    bool m_in_packet = false;
    int m_packet_offset = 0;

    bool m_checksum_check(uint8_t packet[]);
    void m_parse_channels(uint8_t packet[], int ch[]);
    void m_copy_array(uint8_t src[], uint8_t dst[], int dst_size = 0);
};