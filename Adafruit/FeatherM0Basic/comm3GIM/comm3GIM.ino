#include <RTCZero.h>

#define V42port (A5)
#define PIN_PON (A0)
#define PIN_LED (13)
#define VBATPIN A7

RTCZero rtc;

bool opening = true;
unsigned int counter = 0;

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);

  rtc.begin();
  pinMode(V42port, INPUT);

  pinMode(PIN_PON, OUTPUT);
  digitalWrite(PIN_PON, HIGH);
  Serial.println("Power on!");
  delay(1000);
  digitalWrite(PIN_PON, LOW);

  pinMode(PIN_LED, OUTPUT);

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
    digitalWrite(PIN_LED, HIGH);

    float subV = ((analogRead(V42port) * 2) * 3.3) / 0x400;
    Serial.println("subV = " + String(subV, 2));

    float lipo = analogRead(VBATPIN);
    lipo *= 2;    // we divided by 2, so multiply back
    lipo *= 3.3;  // Multiply by 3.3V, our reference voltage
    lipo /= 1024; // convert to voltage
    Serial.println("lipo = " + String(lipo, 2));

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
          Serial.println("[YR]");
          Serial1.println("$YR");
          p = readLN();
          Serial.println(p);
          //
          if ((counter % 12) == 0) {
            Serial.println("$WG");
            Serial1.println("$WG http://www.klabo.co.jp/tph.php?t=0.00&h=0.00&p=0.00&mac=FF:FF:FF:FF:FF:FF&up=0&spv=" + String(subV, 2) +  "&spa=" + String(lipo, 2));
            p = readLN();
            Serial.println(p);
            p = readLN();
            Serial.println(p);
          }
          counter++;
          Serial.println("----------------------------------------------------------");
          //
        }
      }
    }
    digitalWrite(PIN_LED, LOW);
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


