#include<Wire.h>
//r:赤 g:緑 b:青 a:赤外
int r, g, b, a;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  //初期化
  Wire.beginTransmission(0x2A);
  Wire.write(0x0);
  Wire.write(0x89);
  Wire.endTransmission();
  Wire.beginTransmission(0x2A);
  Wire.write(0x0);
  Wire.write(0x09);
  Wire.endTransmission();

}
void loop() {
  delay(50);
  colorread();
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


