#include <FlexiTimer2.h>

#include <Adafruit_NeoPixel.h>
#include <Servo.h>

// only for DUALSHOCK3

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
Servo thisServo;

#define PIN_NEOPIXEL (8)
typedef struct {
  int xRH;
  int yLH;
  int aR;
} PREVSTAT;

PREVSTAT prevStat = {0};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

unsigned int counter = 0;
bool useRING = true;
bool cwRING = true;
int thisClut = 1;

unsigned long clut[8] = {0};

void setup() {
  Serial.begin(115200);
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nPS3 Bluetooth Library Started"));

  setClut();
  strip.begin();
  strip.setBrightness(0x80);
  strip.show(); // Initialize all pixels to 'off'

  thisServo.attach(PIN_SERVOMOTOR);

  FlexiTimer2::set(50, cyclePoint);
  FlexiTimer2::start();

  set_sleep_mode(SLEEP_MODE_IDLE);
}
void loop() {
  sleep_mode();

  Usb.Task();

  if (PS3.PS3Connected) {
    //
    int xRH = PS3.getAnalogHat(RightHatX);
    if ( xRH > 137 || xRH < 117) {
      if (xRH != prevStat.xRH) {
        prevStat.xRH = xRH;

        Serial.print(F("\tRightHatX: "));
        Serial.print(xRH);
        Serial.println("");
        //
        int value = xRH * 0.7;
        thisServo.write(value);
        //
      }
    }
    //
    int yLH = PS3.getAnalogHat(LeftHatY);
    if ( yLH > 137 || yLH < 117) {
      if (yLH != prevStat.yLH) {
        prevStat.yLH = yLH;

        Serial.print(F("\tLeftHatY: "));
        Serial.print(yLH);
        Serial.println("");
        //
        int value = 0xFF - yLH;
        strip.setBrightness(value);
        //
      }
    }
    //
    int aR = PS3.getAnalogButton(R2);
    if (aR != prevStat.aR) {
      prevStat.aR = aR;
      strip.setBrightness(aR);
      Serial.println(aR);
    }
    //
    if (PS3.getButtonClick(R1)) {
      cwRING = true;
    }
    else if (PS3.getButtonClick(L1)) {
      cwRING = false;
    }
    //
    if (PS3.getButtonClick(TRIANGLE)) {
      thisClut++;
      if (thisClut == 8) {
        thisClut = 1;
      }
    }
    //
    if (PS3.getButtonClick(CROSS)) {
      if (thisClut > 1) {
        thisClut--;
      }
      else {
        thisClut = 7;
      }
    }
    //
    if (PS3.getButtonClick(START)) {
      useRING = !useRING;
    }
    //
  }
}

