#pragma once

#include <Arduino.h>
#include "ibus.h"

iBus ibus(Serial2);

void print_channels();