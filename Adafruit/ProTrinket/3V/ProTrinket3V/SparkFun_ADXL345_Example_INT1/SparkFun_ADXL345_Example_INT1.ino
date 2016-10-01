#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library
#include <avr/sleep.h>

ADXL345 adxl = ADXL345();             // USE FOR I2C COMMUNICATION

int interruptPin = 3; // only INT1:3 Pro Trinket
volatile bool busyINT = false;
bool prevActivity = false;
unsigned int counter = 0;

#define PIN_LED (13)
#define ACT_TH (48)

void setup() {

  Serial.begin(115200);                 // Start the serial terminal
  Serial.println("check the ACTIVITY start!");

  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(16);           // Give the range settings
  //  adxl.setRangeSetting(2);           // Give the range settings

  adxl.setSpiBit(0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1

  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(ACT_TH);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

  //  adxl.setInactivityXYZ(1, 0, 0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(ACT_TH);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  adxl.setTimeInactivity(5);         // How many seconds of no activity is inactive?

  //  adxl.setTapDetectionOnXYZ(0, 0, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setTapDetectionOnXYZ(1, 1, 1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

  // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
  adxl.setTapThreshold(50);           // 62.5 mg per increment
  adxl.setTapDuration(15);            // 625 μs per increment
  adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
  adxl.setDoubleTapWindow(200);       // 1.25 ms per increment

  // Set values for what is considered FREE FALL (0-255)
  adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
  adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment

  // Setting all interupts to take place on INT1 pin
  adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);"
  // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
  // This library may have a problem using INT2 pin. Default to INT1 pin.

  // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
  adxl.InactivityINT(1);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(0);
  adxl.doubleTapINT(0);
  adxl.singleTapINT(0);

  byte interrupts = adxl.getInterruptSource(); // Why ???
  attachInterrupt(digitalPinToInterrupt(interruptPin), thisISR, RISING);   // Attach Interrupt

  pinMode(PIN_LED, OUTPUT);

  //  set_sleep_mode(SLEEP_MODE_IDLE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void thisISR() {
  busyINT = true;
  digitalWrite(PIN_LED, HIGH);
}

void loop() {
  //    int x, y, z;
  //    adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
  if (busyINT) {
    busyINT = false;
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


