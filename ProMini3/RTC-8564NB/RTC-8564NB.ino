// ATMega328をRTCからの割り込みでスリープから起こす!

#include <Wire.h>
#include <avr/sleep.h>
#include <RTC8564.h>

#define PIN_LED (4)

#define PIN_RTC_INT (2)
#define PIN_RTC_NUM (0)

#define INTERVAL_SECS (1)

RTC8564 rtc;
RTC_TIME thisTime;

volatile unsigned long counter = 0;

void timerHandler()
{
  counter++;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);

//  while (!Serial);
  Serial.begin(115200);

  //  Serial.println("start RTC test");

  //    set_sleep_mode(SLEEP_MODE_IDLE);
  //    set_sleep_mode(SLEEP_MODE_ADC);
  //  set_sleep_mode(SLEEP_MODE_STANDBY);
  //  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  attachInterrupt(PIN_RTC_NUM, timerHandler, FALLING);

  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

  counter = 0;
}

void loop() {
  digitalWrite(PIN_LED, counter % 2);
//  Serial.println(counter);
//
  rtc.now(&thisTime);
  Serial.println(thisTime.sec);
//
  delay(10); // 次のスリープまでに処理を完了させるための時間稼ぎ
  sleep_mode(); // zzz ...
}
