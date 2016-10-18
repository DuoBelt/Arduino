#include <RTCZero.h>

#define PIN_LED (13)

RTCZero rtc;
int AlarmTime = 0;
int counter = 0;

void setup()
{
  pinMode(PIN_LED, OUTPUT);

  rtc.begin();
  rtc.enableAlarm(rtc.MATCH_SS);
  rtc.attachInterrupt(alarmMatch);
}

void loop()
{
  AlarmTime = (rtc.getSeconds() + 1) % 60;
  rtc.setAlarmSeconds(AlarmTime);
 
  rtc.standbyMode();
}

void alarmMatch() // Do something when interrupt called
{
  counter++;
  digitalWrite(PIN_LED, counter % 2);
}
