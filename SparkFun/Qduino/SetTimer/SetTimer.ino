#include <Wire.h>
#include <RTC8564.h>

RTC8564 RTC;

volatile bool gotInt = false;
volatile unsigned int counter = 0;

#define PIN_RTC_INT (1)
#define INTERVAL_SECS (1)

void intercept(){
  gotInt = true;
  counter++;
}

void setup()
{
  gotInt = false;

  Serial.begin(115200);
  RTC.begin();

  RTC.setTimer( RTC.TIMING_1_SEC, INTERVAL_SECS, true, true );
  pinMode(PIN_RTC_INT, INPUT_PULLUP);

  attachInterrupt(PIN_RTC_INT, intercept, FALLING);
}

void loop(){
  RTC_TIME thisTime;
  RTC.now(&thisTime);
  Serial.println(thisTime.sec);
  if (gotInt) {
    gotInt = false;
    Serial.println(counter);

//    RTC.stopTimer();
  }
//  else {
    delay(500);
//  }
}
