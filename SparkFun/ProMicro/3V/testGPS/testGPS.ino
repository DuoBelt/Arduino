#include <SoftwareSerial.h>

SoftwareSerial ss(8, 9);

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  ss.begin(115200);
}

void loop() {
  while (ss.available()) {
    Serial.print((char)ss.read());
  }
  delay(1000);
  Serial.println("Hi");
}
