/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/ 

#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
//#include <TinyWireM.h> // Enable this line if using Adafruit Trinket, Gemma, etc.

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 
int pos = 0;    // variable to store the servo position 

Adafruit_7segment matrix = Adafruit_7segment();

void setup() 
{ 
  Serial.begin(115200);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
  matrix.begin(0x70);
} 
 
void loop() 
{ 
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    Serial.println(pos);
    matrix.println(pos);
    matrix.writeDisplay();
    delay(10*3);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    Serial.println(pos);
    matrix.println(pos);
    matrix.writeDisplay();
    delay(10*3);                       // waits 15ms for the servo to reach the position 
  } 

  myservo.write(90);
  matrix.println(90);
  matrix.writeDisplay();
  delay(1000*3);
} 

