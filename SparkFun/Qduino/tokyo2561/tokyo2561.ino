#include "avr/sleep.h"
#include "tsl2561.h"

#define PIN_TSL_INT (15)

tsl2561 tsl;

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  set_sleep_mode(SLEEP_MODE_IDLE);

  pinMode(PIN_TSL_INT, INPUT_PULLUP);
  PCMSK0 |= (1 << PCINT1);
  PCICR  |= (1 << PCIE0);

  bool stat = false;
  
  stat = tsl.init();

Serial.println(stat);
  
  tsl.start(true, false);
}

volatile bool ooo = false;

ISR(PCINT0_vect) {
  if(digitalRead(PIN_TSL_INT) == LOW){
    ooo = true;
  }
//  tsl.clear_interrupt();
}

void loop() {
  if (ooo) {
    ooo = false;
    float lux = tsl.get_data();
    Serial.println(String(lux) + " lux");
    delay(10);
  }
  else {
    //    Serial.println("zzz ...");
    //    delay(1000);
    sleep_mode();
  }
}
