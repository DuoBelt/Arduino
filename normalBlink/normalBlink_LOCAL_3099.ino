/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 4. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */

#define PIN (13)
#define DMS (1000/4)

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 4 as an output.
  pinMode(PIN, OUTPUT);
  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Hi!");
  digitalWrite(PIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(DMS);              // wait for a second
  digitalWrite(PIN, LOW);    // turn the LED off by making the voltage LOW
  delay(DMS);              // wait for a second
}
