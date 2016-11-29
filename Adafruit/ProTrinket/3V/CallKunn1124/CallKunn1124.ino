#include <SoftwareSerial.h>

#define SS_RX (4)
#define SS_TX (5)

#define PIN_ADXL345_INT1 (3)

#include <SparkFun_ADXL345.h>
#include <avr/sleep.h>

#define USE_VOLUME true

ADXL345 adxl = ADXL345();

SoftwareSerial ss(SS_RX, SS_TX);

int interruptPin = PIN_ADXL345_INT1; // only INT1:pin3 Pro Trinket
volatile bool busyINT = false;
bool prevActivity = false;
unsigned int counter = 0;

#define ACT_TH (255)
#define PIN_VOLUME A0

#if USE_VOLUME
byte volumeA = 0;
byte volumeI = 0;
#else
byte volumeA = ACT_TH;
byte volumeI = ACT_TH;
#endif

#define PIN_LED (13)

void setup() {

  Serial.begin(115200);
  Serial.println("check the ACTIVITY start!");

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

  byte interrupts = adxl.getInterruptSource(); // Why ????????????????????????????????
  attachInterrupt(digitalPinToInterrupt(interruptPin), thisISR, RISING);   // Attach Interrupt

  //
  pinMode(PIN_LED, OUTPUT);
  int a;
  for (a = 0; a < 6; a++) {
    digitalWrite(PIN_LED, a % 2);
    delay(100);
  }
  //
  ss.println("Ready go");
#if USE_VOLUME
  byte v = analogRead(PIN_VOLUME) / 4;
  ss.println("Volume " + String(v));
#endif
//    set_sleep_mode(SLEEP_MODE_IDLE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
}

void thisISR() {
  busyINT = true;
}

void loop() {
#if USE_VOLUME
  byte v = analogRead(PIN_VOLUME) / 4;
  if (v != volumeA) {
    volumeA = v;
    volumeI = v;
    adxl.setActivityThreshold(volumeA);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)
    adxl.setInactivityThreshold(volumeI);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
  }
#else
  byte v = ACT_TH;
#endif
  if (busyINT) {
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
        ss.println(counter);
//        ss.println(v);
        delay(10);
        digitalWrite(PIN_LED, LOW);
      }
    }
  }
  else {
    digitalWrite(PIN_LED, LOW);
    sleep_mode();
  }
}


