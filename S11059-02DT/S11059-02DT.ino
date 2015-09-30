#include <Wire.h>
#include <avr/sleep.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);

  Wire.beginTransmission(0x2A);
  Wire.write(0x0);
  Wire.write(0x89);
  Wire.endTransmission();
  Wire.beginTransmission(0x2A);
  Wire.write(0x0);
  Wire.write(0x09);
  Wire.endTransmission();

  pinMode(3,INPUT_PULLUP);
  attachInterrupt(1,checkButton,FALLING);
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
}

static int count = 0;
void checkButton(){
  count++;  
}

void loop() {
  sleep_mode();
  if(count){
    count = 0;
    colorread();
  }
}

void colorread() {
  Wire.beginTransmission(0x2A);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(0x2A, 8);
  if (Wire.available()) {
    static char *name[] = {"R","G","B","I"};
    int HByte;
    int LByte;
    int value;
    int a;
    int b;
    
    for(a=0,b=sizeof(name)/sizeof(char *); b--; a++){
      HByte = Wire.read();
      LByte = Wire.read();
      value = HByte << 8 | LByte;
      Serial.print(name[a]);
      Serial.print(" = ");
      Serial.print(value);
      Serial.print(" ");
    }
    Serial.println("");
  }
  Wire.endTransmission();
}

