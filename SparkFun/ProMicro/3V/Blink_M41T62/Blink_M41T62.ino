#include <Wire.h>
#include <M41T62.h>

#include <avr/sleep.h>

#define LEDPIN (17)
#define IRQPIN (0)

RTC_M41T62 rtc;
volatile unsigned long counter = 0;

void cyclePoint(){
  counter++;  
}

void setup() {
  while(true){
    if(Serial) break;
  }
  Serial.begin(115200);

  pinMode(LEDPIN, OUTPUT);

  Wire.begin();
  rtc.begin();
  rtc.checkFlags();
  rtc.alarmRepeat(1);

  pinMode(IRQPIN, INPUT);
//  attachInterrupt(digitalPinToInterrupt(IRQPIN), cyclePoint, FALLING);
  attachInterrupt(digitalPinToInterrupt(IRQPIN), cyclePoint, CHANGE);

//  set_sleep_mode(SLEEP_MODE_IDLE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
//  Serial.println(counter);

  digitalWrite(LEDPIN, counter%2);
  
  sleep_mode();
}
