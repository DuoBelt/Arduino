#include "Ws2822s.h"

#define NUM_PIXELS 3
#define DAI_PIN 11
#define LED_PIN 13

Ws2822s LED(DAI_PIN, NUM_PIXELS);

unsigned int counter = 0;

void setup()
{
  pinMode(DAI_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  unsigned int target = (counter++) % NUM_PIXELS;

  for (unsigned int a = 0; a < NUM_PIXELS; a++) {
    if(a != target){
      LED.setColor(a, 0x00, 0x00, 0x00);
    }
  }
  LED.setColor(target, 0x00, 0x00, 0xFF);
  LED.send();

  delay(100);
}

