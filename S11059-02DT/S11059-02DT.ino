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
  int h, l;
  Wire.beginTransmission(0x2A);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(0x2A, 8);
  if (Wire.available()) {
    //赤
    h = Wire.read();
    l = Wire.read();
    r = h << 8 | l;
    Serial.print("r:");
    Serial.print(r);
    //緑
    h = Wire.read();
    l = Wire.read();
    g = h << 8 | l;
    Serial.print(" g:");
    Serial.print(g);
    //青
    h = Wire.read();
    b = Wire.read();
    r = h << 8 | l;
    Serial.print(" b:");
    Serial.print(b);
    //赤外
    h = Wire.read();
    l = Wire.read();
    a = h << 8 | l;
    Serial.print(" a:");
    Serial.print(a);
    Serial.println("");
  }
  Wire.endTransmission();
}


