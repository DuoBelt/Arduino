#include <RTCZero.h>

#define PIN_LED (13)

RTCZero rtc;
int AlarmTime;

void setup()
{
  rtc.begin();
}

void loop()
{

  // Simple indication of being awake
  digitalWrite(13, HIGH);   // turn the LED on 
  delay(100);              
  digitalWrite(13, LOW);    // turn the LED off
  delay(100);
  digitalWrite(13, HIGH);   // turn the LED on 
  delay(100);
  digitalWrite(13, LOW);    // turn the LED off
  

  AlarmTime += 1; // Adds 5 seconds to alarm time
  AlarmTime = AlarmTime % 60; // checks for roll over 60 seconds and corrects
  rtc.setAlarmSeconds(AlarmTime); // Wakes at next alarm time, i.e. every 5 secs
  
  rtc.enableAlarm(rtc.MATCH_SS); // Match seconds only
  rtc.attachInterrupt(alarmMatch); // Attach function to interupt
  rtc.standbyMode();    // Sleep until next alarm match
  
}

void alarmMatch() // Do something when interrupt called
{
  
}
