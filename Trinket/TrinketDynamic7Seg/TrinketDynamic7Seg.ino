/*
   本スケッチはアノードコモンの3桁+n7セグLED(てかDPがあるので8セグだ)をPro Trinketでドライブするサンプルである。
   だがそもそもArduinoで7セグのダイナミック点灯をしようとすると、使うピンが多過ぎて(8+桁数)LEDコントローラみたいになってしまう。
   なのでこれはあくまで「arduinoでもできる」レベルなもんであって、実際にやるもんじゃない。
*/

static int aPin[] = {A0, A1, A2}; // コモン各桁(アノード)はA0から
static int cPin[] = {3, 4, 5, 6, 8, 9, 10, 11}; // 各セグメントの制御は余剰Dピンで

static unsigned int numA = sizeof(aPin) / sizeof(int);
unsigned int counter = 0;

void setup() {
  int *p;
  int b;

  Serial.begin(115200);

  for (p = aPin, b = numA; b--; ) {
    pinMode(*p++, OUTPUT);
  }

  for (p = cPin, b = 8; b--;) {
    pinMode(*p++, OUTPUT);
  }

  allOFF();
}

void loop() {
  int *p;
  int b;
  unsigned int focus = counter % numA;
  for (a = 0, p = aPin, b = numA; b--; p++, a++) {
    digitalWrite(*p, a == focus);
  }
  outDigit(focus);

  delay(1000 / 1000);
  counter++;
}

void outDigit(int value) {
  static const unsigned char bitPattern[] = {
    B00111111, // 0
    B00000110, // 1
    B01011011, // 2
    B01001111, // 3
    B01100110, // 4
    B01101101, // 5
    B01111101, // 6
    B00000111, // 7
    B01111111, // 8
    B01101111, // 9
  };

  unsigned char src = bitPattern[value];
  int a;
  int b;
  unsigned char bitPos = 1;
  for (a = 8, b = 0; a--; b++, bitPos <<= 1) {
    int tPin = cPin[b];
    digitalWrite(tPin, src & bitPos);
  }
}

void allOFF() {
  Serial.println("----");
  for (int a = 0; a < 8; a++) {
    int tPin = cPin[a];
    digitalWrite(tPin, LOW);
  }
}

