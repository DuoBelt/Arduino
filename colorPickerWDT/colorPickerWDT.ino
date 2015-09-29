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

#define WHITELED_PIN 13
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
  attachInterrupt(1,checkButton,CHANGE);

  pinMode(WHITELED_PIN,OUTPUT);
  pinMode(VIBRATOR_PIN,OUTPUT);
}

static int pushCounter = 0;
static int readCounter = 0;
void checkButton(){
  if(digitalRead(3) == LOW){
    digitalWrite(WHITELED_PIN,HIGH);
    pushCounter++;
  }
  else{
    digitalWrite(WHITELED_PIN,LOW);
  }
}

void loop() {
  delayWDT(6);
  if(pushCounter){
//    Serial.println(pushCounter);
    pushCounter = 0;
    sendColorData();
  }
  else{
  }
}

void sendColorData() {
  readCounter++;
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
    delay(10);
    SWSerial.println(readCounter);
  }
  Wire.endTransmission();
}

// ------------------------------------------------------
// ここから下を全て使う

void delayWDT(unsigned long t) {        // パワーダウンモードでdelayを実行
  delayWDT_setup(t);                    // ウォッチドッグタイマー割り込み条件設定
  ADCSRA &= ~(1 << ADEN);               // ADENビットをクリアしてADCを停止（120μA節約）
//  set_sleep_mode(SLEEP_MODE_STANDBY);  // Standby mode
//  set_sleep_mode(SLEEP_MODE_PWR_SAVE);  // PowerSave mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // パワーダウンモード
  sleep_enable();
  sleep_mode();                         // ここでスリープに入る
  sleep_disable();                      // WDTがタイムアップでここから動作再開
  ADCSRA |= (1 << ADEN);                // ADCの電源をON (|=が!=になっていたバグを修正2014/11/17)

}

void delayWDT_setup(unsigned int ii) {  // ウォッチドッグタイマーをセット。
  // 引数はWDTCSRにセットするWDP0-WDP3の値。設定値と動作時間は概略下記
  // 0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
  // 6=1sec, 7=2sec, 8=4sec, 9=8sec
  byte bb;
  if (ii > 9 ) {                        // 変な値を排除
    ii = 9;
  }
  bb = ii & 7;                          // 下位3ビットをbbに
  if (ii > 7) {                         // 7以上（7.8,9）なら
    bb |= (1 << 5);                     // bbの5ビット目(WDP3)を1にする
  }
  bb |= ( 1 << WDCE );

  MCUSR &= ~(1 << WDRF);                // MCU Status Reg. Watchdog Reset Flag ->0
  // start timed sequence
  WDTCSR |= (1 << WDCE) | (1 << WDE);   // ウォッチドッグ変更許可（WDCEは4サイクルで自動リセット）
  // set new watchdog timeout value
  WDTCSR = bb;                          // 制御レジスタを設定
  WDTCSR |= _BV(WDIE);
}

ISR(WDT_vect) {                         // WDTがタイムアップした時に実行される処理
  //  wdt_cycle++;                        // 必要ならコメントアウトを外す
}



