//
// Color Picker
//
//  wiring ...
//  D3 <--- tactile switch (PULL-UP)
//  D5 <--- Bluetooth RX
//  D6 ---> Bluetooth TX
//  A4 ---> I2C - SDA
//  A5 ---> I2C - SDC

#include <Wire.h>
#include <avr/sleep.h>
#include <SoftwareSerial.h>

#define BLUETOOTH_RX 5
#define BLUETOOTH_TX 6

#define WHITELED_PIN 4
#define VIBRATOR_PIN 8

SoftwareSerial SWSerial(BLUETOOTH_RX,BLUETOOTH_TX);

void setup() {
  Serial.begin(115200);
  SWSerial.begin(9600);
  
  Wire.begin();
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

  pinMode(WHITELED_PIN,OUTPUT);
  pinMode(VIBRATOR_PIN,OUTPUT);
  
  set_sleep_mode(SLEEP_MODE_IDLE);
  sleep_enable();
}

static int pushCounter = 0;
void checkButton(){
  pushCounter++;  
}

void loop() {
  sleep_mode();
  if(pushCounter){
//    Serial.println(pushCounter);
    pushCounter = 0;
    sendColorData();
  }
}

void sendColorData() {
  Wire.beginTransmission(0x2A);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(0x2A, 8);
  if (Wire.available()) {
    static char *name[] = {"Red","Green","Blue","IR"};
    int rgbi[sizeof(name)/sizeof(char *)];
    int HByte;
    int LByte;
    int value;
    int a;
    int b;
    
    for(a=0,b=sizeof(name)/sizeof(char *); b--; a++){
      HByte = Wire.read();
      LByte = Wire.read();
      value = HByte << 8 | LByte;
      rgbi[a] = value;
      Serial.print(name[a]);
      Serial.print(" = ");
      Serial.print(value);
      Serial.print(" ");
    }
    Serial.println("");
    SWSerial.println("読み込み完了!");
  }
  Wire.endTransmission();
  
}


