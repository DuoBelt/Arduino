#include "avr/sleep.h"

#define PIN_PIR (3)

bool lampStat = false;
bool levelPIR = false;

void cyclePoint() {
  levelPIR = digitalRead(PIN_PIR);
}

float getLUX(){
  float lux = 0.0;
  return(lux);
}

void setup() {
  pinMode(PIN_PIR, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_PIR), cyclePoint, CHANGE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
  float lux = getLUX();
  if (levelPIR) {
    if(lampStat){
      ;
    }
    else{
      lampControl(HIGH);
      lampStat = HIGH;
    }
  }
  else{
    lampControl(LOW);
    lampStat = LOW;
  }
  sleep_mode();
}
