#pragma once

#include <Arduino.h>
#include "ibus.h"

#define SERIAL_CLASS 

const int packet_size = 31;
const int channels_per_packet = 14;
uint8_t partial_packet[packet_size];
uint8_t full_packet[packet_size];
int channel[14];

bool in_packet = false;
int packet_offset = 0;

iBus ibus(Serial2);

void print_channels();