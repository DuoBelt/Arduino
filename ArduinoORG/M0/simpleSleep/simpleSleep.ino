#include <RTCZero.h>

RTCZero rtc;

int AlarmTime = 0;
int Counter = 0;

void setup()
{
  rtc.begin();
}

void loop()
{
  counter++;
  digitalWrite(13,counter%2);

  AlarmTime += 1;
  AlarmTime = AlarmTime % 60; // checks for roll over 60 seconds and corrects
  rtc.setAlarmSeconds(AlarmTime); // Wakes at next alarm time, i.e. every 5 secs

  rtc.enableAlarm(rtc.MATCH_SS); // Match seconds only
  rtc.attachInterrupt(alarmMatch); // Attach function to interupt
  rtc.standbyMode();    // Sleep until next alarm match
}

void alarmMatch() // Do something when interrupt called
{

}

