#include <SparkFun_ADXL345.h>
#include <avr/sleep.h>

#define USE_AVOLUME false

ADXL345 adxl = ADXL345();

bool prevActivity = false;
unsigned int counter = 0;

#if USE_VOLUME
byte volumeA = 0;
byte volumeI = 0;
#else
#define ACT_TH (60)
byte volumeA = ACT_TH;
byte volumeI = ACT_TH;
#endif

#define PIN_LED (13)

void setup() {

  Serial.begin(115200);
  Serial.println("check the ACTIVITY start!");

  adxl.powerOn();
  adxl.setRangeSetting(16);

  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(volumeA);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

  adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(volumeI);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(1);         // How many seconds of no activity is inactive?

  pinMode(PIN_LED, OUTPUT);

  //  set_sleep_mode(SLEEP_MODE_IDLE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void loop() {
#if USE_VOLUME
  volumeA = analogRead(A0) / 4;
  volumeI = analogRead(A1) / 4;
  adxl.setActivityThreshold(volumeA);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
  adxl.setInactivityThreshold(volumeI);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
#endif
  digitalWrite(PIN_LED, HIGH);
  //
  bool thisActivity = false;

  if (adxl.triggered(interrupts, ADXL345_ACTIVITY)) {
    thisActivity = true;
  }
  /*
      else if (adxl.triggered(interrupts, ADXL345_INACTIVITY)) {
        thisActivity = false;
      }
  */
  if (thisActivity != prevActivity) {
    prevActivity = thisActivity;
    if (thisActivity) {
      counter++;
      Serial.println("Activity " + String(counter));
      delay(10);
    }
  }
  //
  digitalWrite(PIN_LED, LOW);
}
else {
  sleep_mode();
}
}


