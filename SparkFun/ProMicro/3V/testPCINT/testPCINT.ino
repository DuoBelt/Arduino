#include <avr/sleep.h>

#define PIN_TSW (9)

//#include <SoftwareSerial.h>

//SoftwareSerial ss (4,5);

void setup() {
  Serial.begin(115200);
  pinMode(PIN_TSW, INPUT_PULLUP);

  //  ss.begin(115200);

  set_sleep_mode(SLEEP_MODE_IDLE);

  PCMSK0 |= (1 << PCINT5);
  PCICR |= (1 << PCIE0);
}

volatile int counter = 0;
volatile bool tako = false;

ISR(PCINT0_vect) {
  if (digitalRead(PIN_TSW) == LOW) {
    counter++;
    tako = true;
  }
}

void loop() {
  if (tako) {
    tako = false;
    Serial.println(counter);
  }
  else {
    //    delay(1000);
    sleep_mode();
  }
}
