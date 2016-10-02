#define USE_USB false

#include <Wire.h>
#include <avr/sleep.h>
#include <RTC8564.h>

#define LEDPIN (4)

#define PIN_RTC_INT (7)
#define PIN_RTC_NUM (4)

#define INTERVAL_SECS (1)

RTC8564 rtc;

volatile unsigned long counter = 0;

void timerHandler()
{
  counter++;
}

void setup() {
#if USE_USB
  while (!Serial);

  Serial.begin(115200);
  Serial.println("start RTC test");
#endif
  //  set_sleep_mode(SLEEP_MODE_IDLE);
    set_sleep_mode(SLEEP_MODE_ADC);
//  set_sleep_mode(SLEEP_MODE_STANDBY);
  //  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  //  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  attachInterrupt(PIN_RTC_NUM, timerHandler, FALLING);

  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  sleep_mode();
  digitalWrite(LEDPIN, counter % 2);
#if USE_USB
  Serial.println(counter);
#endif
}
