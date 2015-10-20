#include <stdio.h>

static int waitMS = 1000/4;

void setup()
{
  Serial.begin(115200); //This pipes to the serial monitor
}

void loop()
{
  Serial.println("Hi!");
  RXLED1;
  TXLED0;
  delay(waitMS);
  RXLED0;
  TXLED1;
  delay(waitMS);
}

