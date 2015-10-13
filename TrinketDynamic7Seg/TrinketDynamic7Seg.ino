static int anodePin[3];
static int cathodePin[8];
unsigned int counter;

void setup() {

  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  anodePin[0] = 3;
  anodePin[1] = 4;
  anodePin[2] = 5;

  pinMode(6,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(A0,OUTPUT);

  cathodePin[0] = 6;
  cathodePin[1] = 8;
  cathodePin[2] = 9;
  cathodePin[3] = 10;
  cathodePin[4] = 11;
  cathodePin[5] = 12;
  cathodePin[6] = 13;
  cathodePin[7] = A0;
  
  Serial.begin(115200);

  counter = 0;

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

  delay(1000 / 1000);
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
    int cPin = cathodePin[b];
    digitalWrite(cPin,src&bit);
  }
}

void allON() {
  Serial.println("ON!");
  for (int a = 0; a < 8; a++) {
    int cPin = cathodePin[a];
    digitalWrite(cPin,HIGH);
  }
}

void allOFF() {
  Serial.println("----");
  for (int a = 0; a < 8; a++) {
    int cPin = cathodePin[a];
    digitalWrite(cPin,LOW);
  }
}

