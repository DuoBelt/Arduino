#include <SoftwareSerial.h>

#define SS_RX (5)
#define SS_TX (6)

#include <SparkFun_ADXL345.h>
#include <avr/sleep.h>

#define USE_AVOLUME false

ADXL345 adxl = ADXL345();

SoftwareSerial ss(SS_RX, SS_TX);

int interruptPin = 3; // only INT1:pin3 Pro Trinket
volatile bool busyINT = false;
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

  //  pinMode(SS_RX,INPUT);
  //  pinMode(SS_TX,OUTPUT);
  ss.begin(115200);

  adxl.powerOn();
  adxl.setRangeSetting(16);

  adxl.setActivityXYZ(1, 1, 1);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setActivityThreshold(volumeA);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

  adxl.setInactivityXYZ(1, 1, 1);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
  adxl.setInactivityThreshold(volumeI);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
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

  //  byte interrupts = adxl.getInterruptSource(); // Why ????????????????????????????????
  //  attachInterrupt(digitalPinToInterrupt(interruptPin), thisISR, RISING);   // Attach Interrupt

  pinMode(PIN_LED, OUTPUT);

  set_sleep_mode(SLEEP_MODE_IDLE);
  //  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

//  ss.println("Ready go!");
}

void thisISR() {
  busyINT = true;
}

void loop() {
#if USE_VOLUME
  volumeA = analogRead(A0) / 4;
  volumeI = analogRead(A1) / 4;
  adxl.setActivityThreshold(volumeA);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
  adxl.setInactivityThreshold(volumeI);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
#endif
  if (true) {
    busyINT = false;
    //
    byte interrupts = adxl.getInterruptSource();
    bool thisActivity = false;

    if (adxl.triggered(interrupts, ADXL345_ACTIVITY)) {
      thisActivity = true;
    }
    if (thisActivity != prevActivity) {
      prevActivity = thisActivity;
      if (thisActivity) {
        digitalWrite(PIN_LED, HIGH);
        counter++;
        Serial.println("Activity " + String(counter));
        ss.println(counter);
        delay(10);
      }
      digitalWrite(PIN_LED, LOW);
    }
    //
  }
  else {
    sleep_mode();
  }
}


