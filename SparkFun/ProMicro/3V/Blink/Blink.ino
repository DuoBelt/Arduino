#include <avr/sleep.h>
#include <FlexiTimer2.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin LEDPIN. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */


// the setup function runs once when you press reset or power the board

#define LEDPIN (17)

volatile unsigned long counter = 0;

void cyclePoint(){
  counter++;  
}

void setup() {
  while(true){
    if(Serial) break;
  }
  Serial.begin(115200);
  Serial.println("Start");

  pinMode(LEDPIN, OUTPUT);

  FlexiTimer2::set(1000,cyclePoint);
  FlexiTimer2::start();

  set_sleep_mode(SLEEP_MODE_IDLE);
}

void loop() {
  digitalWrite(LEDPIN, counter%2);
  Serial.println(counter);

  sleep_mode();
}
