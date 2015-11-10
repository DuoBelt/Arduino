#include <stdio.h>

static int waitMS = 1000/1;
static int RXLED = 17;

void setup()
{
  while(!Serial);
  
  Serial.begin(115200);
  pinMode(RXLED,OUTPUT);
}

void loop()
{
  Serial.println("Hi!");
  digitalWrite(RXLED,LOW);
  TXLED0;
  delay(waitMS);
  digitalWrite(RXLED,HIGH);
  TXLED1;
  delay(waitMS);
}

