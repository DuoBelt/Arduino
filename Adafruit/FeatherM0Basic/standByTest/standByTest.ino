#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <RTCZero.h>

#define PIN_TSW (12)
#define PIN_LED (6)

RTCZero rtcZERO;
unsigned long aCounter = 0;
int alarmSS = 0;

Adafruit_7segment matrix = Adafruit_7segment();

void onTSW(){
  aCounter = 0;  
}

void setup()
{
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_TSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TSW), onTSW, FALLING);

  matrix.begin(0x70);

  rtcZERO.begin();
  alarmSS = rtcZERO.getSeconds();
  rtcZERO.attachInterrupt(alarmSR);
  rtcZERO.enableAlarm(rtcZERO.MATCH_SS);
}

void loop()
{
  matrix.print(aCounter, DEC);
  matrix.writeDisplay();
  rtcZERO.setAlarmSeconds(alarmSS = (alarmSS + 1) % 60);
  rtcZERO.standbyMode();
}

void alarmSR() // alarm Service Routine
{
  digitalWrite(PIN_LED, (aCounter++) % 2);
}
