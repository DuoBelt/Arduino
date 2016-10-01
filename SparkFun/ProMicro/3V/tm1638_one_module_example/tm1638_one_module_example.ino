#include <FlexiTimer2.h>

/*
  Library examples for TM1638.

  Copyright (C) 2011 Ricardo Batista <rjbatista at gmail dot com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the version 3 GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/sleep.h>
#include <TM1638.h>

#define PIN_DIO (8)
#define PIN_CLK (9)
#define PIN_STB (7)

// define a module on data pin 8, clock pin 9 and strobe pin 7
TM1638 module(PIN_DIO, PIN_CLK, PIN_STB);

volatile unsigned int counter = 0;
unsigned int prevC = 0;

void cyclePoint() {
  counter++;
}

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);
  Serial.println("Start!");

  FlexiTimer2::set(1000, cyclePoint);
  FlexiTimer2::start();

  set_sleep_mode(SLEEP_MODE_IDLE);

  module.clearDisplay();
  module.setLEDs(0);
  module.setupDisplay(true, 7);
}

void loop() {
  byte keys = module.getButtons();

  //  Serial.println(counter);
  //  module.setLEDs(((keys & 0xF0) << 8) | (keys & 0xF));
  module.setLEDs(((keys & 0xF0) << 8) | (keys & 0xF));

  if (counter > prevC) {
    Serial.println(counter);
    module.setDisplayToHexNumber(counter, 0xFF);
    prevC = counter;
  }
  sleep_mode();
}
