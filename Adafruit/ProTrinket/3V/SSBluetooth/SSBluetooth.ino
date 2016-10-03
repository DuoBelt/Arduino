#include <SoftwareSerial.h>

#define SS_RX (5)
#define SS_TX (6)

SoftwareSerial ss(SS_RX, SS_TX);

volatile int counter = 0;

void setup() {

  Serial.begin(115200);
  ss.begin(115200);

}

void loop() {
  ss.println(counter++);
  delay(1000);
}


