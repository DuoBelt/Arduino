#include <Wire.h>
#include <avr/sleep.h>
#include <RTC8564.h>

#define INTERVAL_SECS (3)

RTC8564 rtc;

static unsigned int prev;
static unsigned int counter;

void timerHandler()
{
   counter++;
//  Serial.println("Hi!");   
}

void setup() {
  prev = 0;
  counter = 0;
  
  while(!Serial);
  Serial.begin(115200);
  Serial.println("start RTC test");

//  set_sleep_mode(SLEEP_MODE_IDLE);
  set_sleep_mode(SLEEP_MODE_ADC);
//  set_sleep_mode(SLEEP_MODE_STANDBY);
//  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  pinMode(7, INPUT_PULLUP);
  attachInterrupt(4, timerHandler, FALLING);

  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );
}

void loop() {
  sleep_mode();
  if(counter != prev){
    prev = counter;
    Serial.println(counter);
  }
}
