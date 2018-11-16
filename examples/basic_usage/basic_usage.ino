#include "iBUS.h"
#include <SoftwareSerial.h>

// Rx on pin 10. The library currently doesn't support TX, but SoftwareSerial needs a tx pin
SoftwareSerial sw_ser = SoftwareSerial(10, 11); 
iBus ibus(sw_ser);

// If you are on a board with multiple hardware serial, you can use that instead
// iBus ibus(Serial2);

void setup() 
{
  Serial.begin(115200);
  while(!Serial) // On USB CDC serial ports, wait for the serial connection.
  {;}

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("iBus test");
}

void loop() 
{
  ibus.handle(); // Run this often
  print_channels();
}


uint32_t last_print = 0;
const int PERIOD = 50;
const int channels = 6;
void print_channels()
{
  // Only print every PERIOD ms
  if(millis() - last_print > PERIOD)
  {
    last_print = millis();
    for(int i=0; i<channels; i++)
    {
      Serial.print("ch "); Serial.print(i+1); Serial.print(": "); Serial.print(ibus.get_channel(i));
      if(ibus.is_alive())
      {
        Serial.println();
        digitalWrite(LED_BUILTIN, LOW);
      }
      else
      {
        Serial.println(" !!LINK DEAD!!");
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
    Serial.print("Last valid packet was recieved "); Serial.print(ibus.time_since_last()); Serial.println(" ms ago");
  }
}