#include <RTC8564.h>

#define PIN_RTC_INT (15)
#define INTERVAL_SECS (1)

RTC8564 rtc;
RTC_TIME thisTime;

volatile unsigned int counter = 0;

void setup() {

  Serial.begin(115200);

  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

  PCMSK0 |= (1 << PCINT1); // on D15
  PCICR  |= (1 << PCIE0); // enable PCI
}

ISR(PCINT0_vect) {
  if (digitalRead(PIN_RTC_INT) == LOW) {
    counter++;
  }
}

void loop() {
  Serial.println(counter);
  delay(1000);
}
