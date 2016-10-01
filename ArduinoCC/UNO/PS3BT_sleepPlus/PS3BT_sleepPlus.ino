#include <avr/sleep.h>

#define LEDPIN (8)

/*
  Example sketch for the PS3 Bluetooth library - developed by Kristian Lauszus
  For more information visit my blog: http://blog.tkjelectronics.dk/ or
  send me an e-mail:  kristianl@tkjelectronics.com
*/

#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

typedef struct {
  int LHX;
  int LHY;
  int RHX;
  int RHY;
  int LAB;
  int RAB;
} ANALOG_PREV;

ANALOG_PREV prev = {0};

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printTemperature, printAngle;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  pinMode(LEDPIN, OUTPUT);
  set_sleep_mode(SLEEP_MODE_IDLE);
  //  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  //  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  //  set_sleep_mode(SLEEP_MODE_STANDBY);
}
void loop() {
  sleep_mode();
  Usb.Task();

  if (PS3.PS3Connected || PS3.PS3NavigationConnected) {
    int LHX = PS3.getAnalogHat(LeftHatX);
    int LHY = PS3.getAnalogHat(LeftHatY);
    int RHX = PS3.getAnalogHat(RightHatX);
    int RHY = PS3.getAnalogHat(RightHatY);
    if (LHX > 137 || LHX < 117 || LHY > 137 || LHY < 117 || RHX > 137 || RHX < 117 || RHY > 137 || RHY < 117) {
      if (LHX != prev.LHX || LHY != prev.LHY || RHX != prev.RHX || RHY != prev.RHY) {
        prev.LHX = LHX;
        prev.LHY = LHY;
        prev.RHX = RHX;
        prev.RHY = RHY;
        Serial.print(F("\r\nLeftHatX: "));
        Serial.print(LHX);
        Serial.print(F("\tLeftHatY: "));
        Serial.print(LHY);
        Serial.print(F("\tRightHatX: "));
        Serial.print(RHX);
        Serial.print(F("\tRightHatY: "));
        Serial.print(RHY);
      }
    }

    // Analog button values can be read from almost all buttons
    int LAB = PS3.getAnalogButton(L2);
    int RAB = PS3.getAnalogButton(R2);
    if ( LAB || RAB) {
      if (LAB != prev.LAB || RAB != prev.RAB) {
        prev.LAB = LAB;
        prev.RAB = RAB;
        Serial.print(F("\r\nL2: "));
        Serial.print(LAB);
        Serial.print(F("\tR2: "));
        Serial.print(RAB);
      }
    }

    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }
    else {
      if (PS3.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTraingle"));
        PS3.setRumbleOn(RumbleLow);
      }
      if (PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        PS3.setRumbleOn(RumbleHigh);
      }
      if (PS3.getButtonClick(CROSS))
        Serial.print(F("\r\nCross"));
      if (PS3.getButtonClick(SQUARE))
        Serial.print(F("\r\nSquare"));

      if (PS3.getButtonClick(UP)) {
        Serial.print(F("\r\nUp"));
        PS3.setLedOff();
        PS3.setLedOn(LED4);
      }
      if (PS3.getButtonClick(RIGHT)) {
        Serial.print(F("\r\nRight"));
        PS3.setLedOff();
        PS3.setLedOn(LED1);
      }
      if (PS3.getButtonClick(DOWN)) {
        Serial.print(F("\r\nDown"));
        PS3.setLedOff();
        PS3.setLedOn(LED2);
      }
      if (PS3.getButtonClick(LEFT)) {
        Serial.print(F("\r\nLeft"));
        PS3.setLedOff();
        PS3.setLedOn(LED3);
      }

      if (PS3.getButtonClick(L1)) {
        Serial.print(F("\r\nL1"));
        digitalWrite(LEDPIN, LOW);
      }
      if (PS3.getButtonClick(L3))
        Serial.print(F("\r\nL3"));
      if (PS3.getButtonClick(R1)) {
        Serial.print(F("\r\nR1"));
        digitalWrite(LEDPIN, HIGH);
      }
      if (PS3.getButtonClick(R3))
        Serial.print(F("\r\nR3"));

      if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect - "));
        PS3.printStatusString();
      }
      if (PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));
        printAngle = !printAngle;
      }
    }
#if 0 // Set this to 1 in order to see the angle of the controller
    if (printAngle) {
      Serial.print(F("\r\nPitch: "));
      Serial.print(PS3.getAngle(Pitch));
      Serial.print(F("\tRoll: "));
      Serial.print(PS3.getAngle(Roll));
    }
#endif
  }
#if 0 // Set this to 1 in order to enable support for the Playstation Move controller
  else if (PS3.PS3MoveConnected) {
    if (PS3.getAnalogButton(T)) {
      Serial.print(F("\r\nT: "));
      Serial.print(PS3.getAnalogButton(T));
    }
    if (PS3.getButtonClick(PS)) {
      Serial.print(F("\r\nPS"));
      PS3.disconnect();
    }
    else {
      if (PS3.getButtonClick(SELECT)) {
        Serial.print(F("\r\nSelect"));
        printTemperature = !printTemperature;
      }
      if (PS3.getButtonClick(START)) {
        Serial.print(F("\r\nStart"));
        printAngle = !printAngle;
      }
      if (PS3.getButtonClick(TRIANGLE)) {
        Serial.print(F("\r\nTriangle"));
        PS3.moveSetBulb(Red);
      }
      if (PS3.getButtonClick(CIRCLE)) {
        Serial.print(F("\r\nCircle"));
        PS3.moveSetBulb(Green);
      }
      if (PS3.getButtonClick(SQUARE)) {
        Serial.print(F("\r\nSquare"));
        PS3.moveSetBulb(Blue);
      }
      if (PS3.getButtonClick(CROSS)) {
        Serial.print(F("\r\nCross"));
        PS3.moveSetBulb(Yellow);
      }
      if (PS3.getButtonClick(MOVE)) {
        PS3.moveSetBulb(Off);
        Serial.print(F("\r\nMove"));
        Serial.print(F(" - "));
        PS3.printStatusString();
      }
    }
    if (printAngle) {
      Serial.print(F("\r\nPitch: "));
      Serial.print(PS3.getAngle(Pitch));
      Serial.print(F("\tRoll: "));
      Serial.print(PS3.getAngle(Roll));
    }
    else if (printTemperature) {
      Serial.print(F("\r\nTemperature: "));
      Serial.print(PS3.getTemperature());
    }
  }
#endif
}
