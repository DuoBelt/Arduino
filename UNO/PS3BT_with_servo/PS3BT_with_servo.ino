// only for DUALSHOCK3

#include <Servo.h>
#include <avr/sleep.h>

#include <PS3BT.h>
#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
PS3BT PS3(&Btd); // This will just create the instance
//PS3BT PS3(&Btd, 0x00, 0x15, 0x83, 0x3D, 0x0A, 0x57); // This will also store the bluetooth address - this can be obtained from the dongle when running the sketch

bool printTemperature, printAngle;

#define PIN_SERVOMOTOR (9)
typedef struct {
  int xRH;
} PREV_BUTTON;

PREV_BUTTON prevButton = {0};

Servo thisServo;


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

  set_sleep_mode(SLEEP_MODE_IDLE);
  thisServo.attach(PIN_SERVOMOTOR);
}
void loop() {
  sleep_mode();

  Usb.Task();

  if (PS3.PS3Connected) {
    int xRH = PS3.getAnalogHat(RightHatX);
    if ( xRH > 137 || xRH < 117) {
      if (xRH != prevButton.xRH) {
        prevButton.xRH = xRH;
        
        Serial.print(F("\tRightHatX: "));
        Serial.print(xRH);
        Serial.println("");
        //
        int value = xRH * 0.7;
        thisServo.write(value);
        //
      }
    }
  }
}

