#include "main.h"

void setup() 
{
  Serial.begin(115200);
  while(!Serial)
  {;}

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  delay(100);

  Serial.println("start");

}

void loop() 
{
  ibus.handle();
  print_channels();
}


uint32_t last_print = 0;
void print_channels()
{
  if(millis() - last_print > 50)
  {
    last_print = millis();
    for(int i=0; i<6; i++)
    {
      Serial.print("ch "); Serial.print(i+1); Serial.print(": "); Serial.print(ibus.get_channel(i));
      if(ibus.is_alive())
      {
        Serial.println();
      }
      else
      {
        Serial.println(" !!LINK DEAD!!");
      }
    }
    Serial.print(ibus.time_since_last());
    Serial.println();
  }
}