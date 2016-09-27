#include <SoftwareSerial.h>

#define PIN_B (4)
#define PIN_Y (5)
#define PIN_R (6)

#define PIN_SS_RX (8) 
#define PIN_SS_TX (9) 

SoftwareSerial ss(PIN_SS_RX,PIN_SS_TX);

void setup() {
  Serial.begin(115200);

  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_Y, OUTPUT);
  pinMode(PIN_R, OUTPUT);

  
}

void loop() {

}
