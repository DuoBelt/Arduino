/*
EIC  ZERO pins
0   11
1   13
2   10 a0 a5
3   12
4   6 a3
5   7 a4
6   8  sda(20)
7   9  scl(21)
8   a1
9   3 a2
10  1 MOSI(23)
11  0  SCK(24)
12  MISO(22)
13
14  2
15  5
*/

#define PIN_LED 13
#define PIN_TSW A5

volatile int counter = 0;

void cyclePoint() {
  digitalWrite(PIN_LED, counter % 2);
  counter++;
}

void setup() {
  while (true) {
    if (SerialUSB) {
      break;
    }
  }
  SerialUSB.begin(115200);
  SerialUSB.println("Start");

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TSW, INPUT_PULLUP);

  //  attachInterrupt(digitalPinToInterrupt(PIN_TSW), cyclePoint, FALLING);
  attachInterrupt(PIN_TSW, cyclePoint, FALLING);
}

void loop() {
  SerialUSB.println(counter);
  delay(1000);
}
