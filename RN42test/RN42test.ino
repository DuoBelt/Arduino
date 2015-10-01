#include <SoftwareSerial.h>

#define BT_RX 5
#define BT_TX 6

SoftwareSerial portBT(BT_RX,BT_TX);

int counter = 0;

void setup() {
  Serial.begin(115200);
  portBT.begin(115200);
}

void loop() {
  Serial.println(counter);
  portBT.println(counter);
  delay(1000*5);
  counter++;
}
