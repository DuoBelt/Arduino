#include <SparkFun_ADXL345.h>
#include <avr/sleep.h>

ADXL345 adxl = ADXL345();

int interruptPin = 3; // only INT1:pin3 Pro Trinket
volatile bool busyINT = false;
bool prevActivity = false;
unsigned int counter = 0;

#define PIN_LED (13)
#define ACT_TH (60)

void setup() {

  Serial.begin(115200);
  Serial.println("check the ACTIVITY start!");

  adxl.powerOn();
  adxl.setRangeSetting(16);

  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(ACT_TH);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

  adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(ACT_TH);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(1);         // How many seconds of no activity is inactive?

  adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);"
  // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
  // This library may have a problem using INT2 pin. Default to INT1 pin.

  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(0);
  adxl.doubleTapINT(0);
  adxl.singleTapINT(0);

  byte interrupts = adxl.getInterruptSource(); // Why ????????????????????????????????
  attachInterrupt(digitalPinToInterrupt(interruptPin), thisISR, RISING);   // Attach Interrupt

  pinMode(PIN_LED, OUTPUT);

  //  set_sleep_mode(SLEEP_MODE_IDLE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void thisISR() {
  busyINT = true;
}

void loop() {
  if (busyINT) {
    busyINT = false;
    digitalWrite(PIN_LED, HIGH);
    //
    byte interrupts = adxl.getInterruptSource();
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


