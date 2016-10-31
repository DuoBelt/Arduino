#include <Ticker.h>
#include <Adafruit_NeoPixel.h>

extern "C" {
#include "user_interface.h"
}

#define LED 13

#define RGBLEDPIN 14
#define RGBLEDCNT 8

Ticker ticker;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(RGBLEDCNT, RGBLEDPIN, NEO_GRB + NEO_KHZ800);
static unsigned int counter = 0;

void showNP(int offset, unsigned char r, unsigned char g, unsigned char b) {
  pixels.setPixelColor(offset, r, g, b);
  pixels.show();
}

void tPoint() {
  unsigned int a;
  unsigned int focus = (counter%RGBLEDCNT);
  for (a = 0; a < RGBLEDCNT; a++) {
    unsigned char value = (a == focus) ? 0xFF : 0x00;
    showNP(a, value, value, value);
  }
  counter++;
}

void intercept() {
  bool pin = digitalRead(PIN);
  digitalWrite(LED, !pin);
}

void setup() {
  Serial.begin(115200);

  pixels.begin();

  ticker.attach(0.5, tPoint);

  wifi_set_sleep_type(LIGHT_SLEEP_T);

  pinMode(PIN, INPUT);
  pinMode(LED, OUTPUT);

  attachInterrupt(PIN, intercept, CHANGE);
}

void loop() {
}
