#define V42port (A5)
#define PIN_PON (A0)
#define VBATPIN A7

bool opening = true;

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  pinMode(V42port, INPUT);

  pinMode(PIN_PON, OUTPUT);
  digitalWrite(PIN_PON, HIGH);
  Serial.println("Power on!");
  delay(1000);
  digitalWrite(PIN_PON, LOW);

  Serial1.begin(115200);

  Serial.println("Start!!!");
}

void loop() {
  char *p;
  if (opening) {
    p = readLN(); Serial.println(p);
    p = readLN(); Serial.println(p);
    opening = false;
  }
  else {
    float V = ((analogRead(V42port)*2)*3.3)/0x400;
    Serial.println("V = " + String(V,1));

    Serial.println("[YR]");
    Serial1.println("$YR");
    p = readLN();
    Serial.println(p);
    if (strncmp(p, "$YR=OK", 5) == 0) {
      Serial.println("[YS]");
      Serial1.println("$YS");
      p = readLN();
      Serial.println(p);
      if (strcmp(p, "$YS=OK 1") == 0) {
        Serial.println("[YT]");
        Serial1.println("$YT");
        p = readLN();
        Serial.println(p);
        if (strncmp(p, "$YT=OK", 5) == 0) {
          Serial.println("[LG]");
          Serial1.println("$LG MSBASED 1");
          p = readLN();
          if (strncmp(p, "$LG=OK", 5) == 0) {
            Serial.println(p);
            Serial.println("-----------------------------------------------------------------------");
          }
        }
      }
    }
    delay(5000);
  }
}

char *readLN() {
  static char buffer[0x400];
  static int offset = 0;

  bool loop = true;
  while (loop) {
    if (Serial1.available()) {
      char c = Serial1.read();
      switch (c) {
        case 0x0d:
          buffer[offset++] = 0;
          break;
        case 0x0a:
          buffer[offset++] = 0;
          loop = false;
          offset = 0;
          break;
        default:
          buffer[offset++] = c;
          break;
      }
    }
  }
  return (buffer);
}


