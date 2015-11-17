void setup() {
  Serial.begin(115200);
}

void loop() {
  float val = 123.456789;
  int a;
  for (a = 0; a < 5; a++){
    String ooo = String(val, a);
    Serial.println(ooo);
  }
  delay(1000 * 1);
}
