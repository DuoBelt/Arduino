void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  Serial1.begin(115200);
}

void loop() {
  while (Serial1.available()) {
    Serial.print((char)Serial1.read());
  }
  delay(1000);
  Serial.println("Hi");
}
