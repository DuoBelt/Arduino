#include <Wire.h>
#include <avr/sleep.h>
#include <RTC8564.h>
#include "SFE_ISL29125.h"

#define INTERVAL_SECS (3)

RTC8564 rtc;

static unsigned int prev;
static unsigned int counter;

SFE_ISL29125 RGB_sensor;

void timerHandler()
{
  counter++;
  //  Serial.println("Hi!");
}

void setup() {
  prev = 0;
  counter = 0;

  while (!Serial);
  Serial.begin(115200);
  Serial.println("start RTC test");

  if (RGB_sensor.init()) {
    Serial.println("Sensor Initialization Successful\n\r");
  }
  set_sleep_mode(SLEEP_MODE_IDLE);
  //  set_sleep_mode(SLEEP_MODE_ADC);
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
  if (counter != prev) {
    prev = counter;
    unsigned int red = RGB_sensor.readRed();
    unsigned int green = RGB_sensor.readGreen();
    unsigned int blue = RGB_sensor.readBlue();

    String info = "R:G:B = " + String(red, HEX) + ":" + String(green, HEX) + ":" + String(blue, HEX);
    Serial.println(info);
    // Print out readings, change HEX to DEC if you prefer decimal output
    //    Serial.print("Red: "); Serial.println(red, HEX);
    //    Serial.print("Green: "); Serial.println(green, HEX);
    //    Serial.print("Blue: "); Serial.println(blue, HEX);
    //    Serial.println();
  }
}
