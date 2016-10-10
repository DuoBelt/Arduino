#define PIN_PON (A0)

unsigned int counter = 0;
unsigned int prev = 0;

char *command[] = {
  (char *)"$YV",
  (char *)"$YS",
    (char *)"$YT",
  //  (char *)"$LG a 1",
  //  (char *)"$WG http://www.klabo.co.jp"
};

char buffer[0x400];
int offset = 0;

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
  if (getResponse()) {
      counter++;
    Serial.print(buffer);
    if (buffer[0] == '$') {
    }
  }
  else if (counter > prev) {
    prev = counter;
    Serial1.println(command[counter % (sizeof(command) / sizeof(char *))]);
    Serial.println("Waiting " + String(counter));
    delay(1000);
  }
}

bool getResponse() {
  bool completed = false;
  if (Serial1.available()) {
    char c = Serial1.read();
    buffer[offset++] = c;
    if (c == 0x0a) {
      buffer[offset] = 0;
      offset = 0;
      completed = true;
    }
  }
  return (completed);
}

