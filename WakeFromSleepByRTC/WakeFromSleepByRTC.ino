#include <Wire.h>
#include <avr/sleep.h>
#include <RTC8564.h>

RTC8564 rtc;

void timerHandler()
{
//  Serial.println("Hi!");
}

void setup()
{
  Serial.begin(115200);

  pinMode(13,OUTPUT);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, timerHandler, FALLING);

  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, 1, true, true );
//  rtc.setTimer( rtc.TIMING_15_625_MS, 1000, true, true );
}

void loop()
{
  RTC_TIME rtc_time;

  digitalWrite(13,HIGH);
  if (rtc.now(&rtc_time)) {
    const char* dayofweek[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    char buf[32];
    snprintf(buf, 32, "%04u-%02u-%02u(%s) %02u:%02u:%02u\n",
             rtc_time.year,
             rtc_time.month,
             rtc_time.day,
             dayofweek[rtc_time.wday],
             rtc_time.hour,
             rtc_time.min,
             rtc_time.sec   );

    Serial.print(buf);
    delay(10);
  }
  digitalWrite(13,LOW);
//  delay(1000);
  sleep_mode();
}
