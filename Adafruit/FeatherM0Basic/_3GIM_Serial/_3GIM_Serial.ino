#define PIN_PON (13)

unsigned int counter = 0;
char *command[] = {
  (char *)"$YV",
  (char *)"$YS",
  (char *)"$YT",
  (char *)"$LG a 1",
  (char *)"$WG http://www.klabo.co.jp",
};

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  pinMode(PIN_PON, OUTPUT);
  digitalWrite(PIN_PON, HIGH);
  Serial.println("delay");
  delay(2000);
  digitalWrite(PIN_PON, LOW);

  Serial1.begin(115200);

  Serial.println("Start!!!");

  //  Serial1.println((char *)"$YV");
}

void loop() {
  int bytes;
  if ((bytes = Serial1.available()) > 0) {
    counter++;
    while (bytes--) {
      char c = Serial1.read();
      Serial.print(c);
    }
  }
  else {
    Serial1.println(command[counter%(sizeof(command)/sizeof(char *))]);
    Serial.println("Waiting " + String(counter));
    delay(1000);
  }
}
