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

#define LEDPIN (4)

volatile unsigned long counter = 0;

void cyclePoint(){
  counter++;  
}

void setup() {
  // initialize digital pin LEDPIN as an output.
  pinMode(LEDPIN, OUTPUT);

  Serial.begin(115200);

  FlexiTimer2::set(1000,cyclePoint);
  FlexiTimer2::start();

  set_sleep_mode(SLEEP_MODE_IDLE);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println(counter);
  Serial.println("High");
  digitalWrite(LEDPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  Serial.println("Low");
  digitalWrite(LEDPIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second

  sleep_mode();
}
