void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  Serial1.begin(115200);

  Serial.println("Start!!!");

  //  Serial1.println((char *)"$YV");
}

void loop() {
  int bytes;
  if ((bytes = Serial1.available()) > 0) {
    while (bytes--) {
      char c = Serial1.read();
      Serial.print(c);
    }
  }
  else {
    Serial1.println((char *)"$YT");
    Serial.println("Waiting");
    delay(1000);
  }
}
