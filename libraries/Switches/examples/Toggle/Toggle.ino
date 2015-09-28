/*

  Toggle.ino

  This sketch is an example to use Switches library.
  It toggles LED on Arduino when you push a switch connected to pin 2.

  To run the skecth you need to connect a pin of the switch to pin 2 
  and another pin to GND. You don't need pull up regitster since 
  the internal pullup register is enabled.

*/

#include <Switches.h>

Switches sw1(2, true, SWITCH_HtoL);
//           ^   ^         ^
//           |   |         |
//           |   |         +- SWITCH_LEVEL:  returns true if the pin is HIGH
//           |   |            SWITCH_NLEVEL: returns true if the pin is LOW
//           |   |            SWITCH_HtoL:   returns true if the pin level
//           |   |                           changes from HIGH to LOW
//           |   |            SWITCH_LtoH:   returns true if the pin level
//           |   |                           changes from LOW to HIGH
//           |   +- true: enable pullup register, false: disable
//           |
//           +--------- pin number

const int ledPin = 13;
bool val = LOW;

void setup() {
  pinMode(ledPin, HIGH);
  digitalWrite(ledPin, val);
}

void loop() {
  if (sw1()) {
    val = !val;
    digitalWrite(ledPin, val);
  }
}
