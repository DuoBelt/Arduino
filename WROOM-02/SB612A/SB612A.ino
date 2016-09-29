extern "C" {
#include "user_interface.h"
}

#define PIN_SB612A_TEST (14)
#define PIN_LED (13)

volatile unsigned int counter = 0;

void cyclePoint() {
  digitalWrite(PIN_LED, digitalRead(PIN_SB612A_TEST));
  counter++;
}

void setup() {
  Serial.begin(115200);

  wifi_set_sleep_type(LIGHT_SLEEP_T);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_SB612A_TEST, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(14), cyclePoint, CHANGE);
}

void loop() {
}
