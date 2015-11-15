extern "C" {
#include "user_interface.h"
}

#define PIN 0
#define LED 13

void intercept() {
  bool pin = digitalRead(PIN);
  digitalWrite(LED, !pin);
}

void setup() {
  Serial.begin(115200);

  wifi_set_sleep_type(LIGHT_SLEEP_T);

  pinMode(PIN, INPUT);
  pinMode(LED, OUTPUT);

  attachInterrupt(PIN, intercept, CHANGE);
}

void loop() {
  Serial.println("loop");
  delay(1000);
}
