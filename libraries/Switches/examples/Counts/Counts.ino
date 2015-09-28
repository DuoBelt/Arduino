/*

  Count.ino

  This sketch is an example to use Switches library.
  It counts how many times a switch connected to pin 2 is pushed and
  sends to Serial.

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

int count = 0;

void setup() {
  while (!Serial) {}
  Serial.begin(9600);
}

void loop() {
  if (sw1()) {
    count ++;
    Serial.println(count);
  }
}
