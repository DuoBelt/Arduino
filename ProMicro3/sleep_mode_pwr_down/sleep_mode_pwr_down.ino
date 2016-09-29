#include <avr/sleep.h>

#define LEDPIN (17)
#define TSWPIN (7)

volatile unsigned long counter = 0;

void cyclePoint() {
  counter++;
}

void setup() {

  pinMode(LEDPIN, OUTPUT);
  pinMode(TSWPIN, INPUT_PULLUP);

  attachInterrupt(TSWPIN,cyclePoint,FALLING);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
  digitalWrite(LEDPIN, counter % 2);

  sleep_mode();
}
