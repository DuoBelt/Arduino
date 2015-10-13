#include <Wire.h>
#include <PCA9685.h>

PCA9685 Led;

static int anodePin[3];

unsigned int counter;

void setup() {

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  anodePin[0] = 3;
  anodePin[1] = 4;
  anodePin[2] = 5;

  Serial.begin(115200);

  counter = 0;

  Wire.begin();
  Led.begin(0x40);

  if ( Led.init()) {
    Serial.println("Start 7-segmenit led test");
  }
  else {
    Serial.println("Failed ..");
  }

  allOFF();
  //  allON();
}

void loop() {
  unsigned int dst = counter % 4;

  if(dst == 3){
    allOFF();
  } 
  else{
   for (int a = 0; a < 3; a++) {
      digitalWrite(anodePin[a], a==dst);
    }
    outDigit(dst);
    
  }

  delay(1000 / 1);
  counter++;
}

void outDigit(int value) {
  static const unsigned char digitCodeMap[] = {
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
    B00000000, // BLANK
    B01000000
  }; // DASH

  //  Serial.println(value);
  unsigned char src = digitCodeMap[value];
  int a;
  int b;
  unsigned char bit = 1;
  for (a = 8, b = 0; a--; b++, bit <<= 1) {
    if (src & bit) {
      Led.setLEDOn(b);
    }
    else {
      Led.setLEDOff(b);
    }
  }
}

void allON() {
  Serial.println("ON!");
  for (int a = 0; a < 8; a++) {
    Led.setLEDOn(a);
    delay(1000 / 4);
  }
}

void allOFF() {
  Serial.println("----");
  for (int a = 0; a < 8; a++) {
    Led.setLEDOff(a);
  }
}

