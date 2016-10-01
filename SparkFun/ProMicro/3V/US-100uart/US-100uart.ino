#include <SoftwareSerial.h>

#define PIN_TX (8)
#define PIN_RX (9)

#define DELAYMS (100)

SoftwareSerial ss(PIN_RX, PIN_TX);

void setup() {
  while (true) {
    if (Serial) {
      break;
    }
    else {
      delay(1000);
    }
  }
  Serial.begin(115200);
  Serial.println("Start");

  ss.begin(9600);

  pinMode(PIN_TX, OUTPUT);
  pinMode(PIN_RX, INPUT);
}

unsigned short readD() {
  unsigned char Hbyte;
  unsigned char Lbyte;
  unsigned short dist;

  ss.write(0x55);
  delay(DELAYMS);

  Hbyte = ss.read();
  Lbyte = ss.read();

  dist = ((unsigned short)Hbyte * 0x100) + (unsigned short)Lbyte;

  return (dist);
}

unsigned char readT() {
  unsigned char temp;

  ss.write(0x50);
  delay(DELAYMS);
  temp = (unsigned char)ss.read();
  temp -= 45;

  return (temp);
}

void loop() {
  unsigned short dist;
  unsigned char temp;
  String info;

  dist = readD();
  temp = readT();

  info = "Distance = " + String(dist) + " Temp = " + temp;
  Serial.println(info);

  delay(1000);

}
