#include <SoftwareSerial.h>

#include <avr/sleep.h>

#define PIN_SB612A_TEST (3)
#define PIN_LED (13)

#define SS_RX (4)
#define SS_TX (5)

volatile unsigned int counter = 0;
volatile bool lastStatus = false;
volatile bool thisStatus = false;

SoftwareSerial ss(SS_RX, SS_TX);

void cyclePoint() {
  bool status = digitalRead(PIN_SB612A_TEST);
  digitalWrite(PIN_LED, status);
  thisStatus = status;
  counter++;
}

void setup() {
  Serial.begin(115200);
  ss.begin(115200);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_SB612A_TEST, INPUT);

  attachInterrupt(digitalPinToInterrupt(PIN_SB612A_TEST), cyclePoint, CHANGE);
}

void loop() {
  if (thisStatus != lastStatus) {
    Serial.println(thisStatus ? "Catch!" : "Lost ...");
//    ss.println(thisStatus ? "なにものじゃ" : "にどとくるな");
    lastStatus = thisStatus;
    delay(100);
  }
  else {
    sleep_mode();
  }
}
