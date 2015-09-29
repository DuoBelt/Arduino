//
// 釧路高専：カラー読み取り機
//

#define USE_S11059 true
#define DEBUG_THIS true

#include <Wire.h>
#include <SoftwareSerial.h>
#include <avr/sleep.h>

//
// D3 <--- タクトスイッチ (active HIGH)
// D5 ---> Bluetooth RX
// D6 ---> Bluetooth TX
// D13 ---> (LED)
// A4 <--> I2C:SDA 
// A5 <--> I2C:SCL
//

#define BLUETOOTH_RX 5
#define BLUETOOTH_TX 6
#define ONBOARD_LED 13

SoftwareSerial SWSerial(BLUETOOTH_RX,BLUETOOTH_TX);

#define BUTTON 3

static int pushCounter = 0;
void checkTheButton(){
  pushCounter++;
  return; 
}

void readAndSendTheValue(){
#if DEBUG_THIS
  Serial.println("Read the RGB and send it!");
#endif

#if USE_S11059
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
#else
  SWSerial.println("なんでやねん");
#endif
  return;
}

void setup() {
  Serial.begin(115200);
  SWSerial.begin(9600);

  pinMode(ONBOARD_LED,OUTPUT);

#if USE_S11059
  Wire.beginTransmission(0x2A);
  Wire.write(0x0);
  Wire.write(0x89);
  Wire.endTransmission();
  Wire.beginTransmission(0x2A);
  Wire.write(0x0);
  Wire.write(0x09);
  Wire.endTransmission();
#endif

  pinMode(BUTTON,INPUT_PULLUP);
  attachInterrupt(INT1,checkTheButton,FALLING);

  set_sleep_mode(SLEEP_MODE_IDLE);
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  sleep_enable();
}

void loop() {
#if DEBUG_THIS
  digitalWrite(ONBOARD_LED,LOW);
  Serial.println("Enter ZZZ ...");
  delay(10*5);
#endif
  sleep_mode();
#if DEBUG_THIS
  digitalWrite(ONBOARD_LED,HIGH);
//  Serial.println("Good morningt!");
//  delay(1000);
#endif
  if(pushCounter){
    pushCounter = 0;
    readAndSendTheValue();
  }
}
