#include <Adafruit_TCS34725.h>

//
// Color Picker 2015
//
//  Pro Trinket Wiring ...
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

#define TACTILE_SW_PIN 3
#define WHITELED_PIN 4
#define VIBRATOR_PIN 8

SoftwareSerial SWSerial(BLUETOOTH_RX,BLUETOOTH_TX);

//
#define TCS34725_R_Coef 0.136 
#define TCS34725_G_Coef 1.000
#define TCS34725_B_Coef -0.444
#define TCS34725_GA 1.0
#define TCS34725_DF 310.0
#define TCS34725_CT_Coef 3810.0
#define TCS34725_CT_Offset 1391.0

// Autorange class for TCS34725
class tcs34725 {
public:
  tcs34725(void);

  boolean begin(void);
  void getData(void);  

  boolean isAvailable, isSaturated;
  uint16_t againx, atime, atime_ms;
  uint16_t r, g, b, c;
  uint16_t ir; 
  uint16_t r_comp, g_comp, b_comp, c_comp;
  uint16_t saturation, saturation75;
  float cratio, cpl, ct, lux, maxlux;
  
private:
  struct tcs_agc {
    tcs34725Gain_t ag;
    tcs34725IntegrationTime_t at;
    uint16_t mincnt;
    uint16_t maxcnt;
  };
  static const tcs_agc agc_lst[];
  uint16_t agc_cur;

  void setGainTime(void);  
  Adafruit_TCS34725 tcs;    
};
//
// Gain/time combinations to use and the min/max limits for hysteresis 
// that avoid saturation. They should be in order from dim to bright. 
//
// Also set the first min count and the last max count to 0 to indicate 
// the start and end of the list. 
//
const tcs34725::tcs_agc tcs34725::agc_lst[] = {
  { TCS34725_GAIN_60X, TCS34725_INTEGRATIONTIME_700MS,     0, 47566 },
  { TCS34725_GAIN_16X, TCS34725_INTEGRATIONTIME_154MS,  3171, 63422 },
  { TCS34725_GAIN_4X,  TCS34725_INTEGRATIONTIME_154MS, 15855, 63422 },
  { TCS34725_GAIN_1X,  TCS34725_INTEGRATIONTIME_2_4MS,   248,     0 }
};
tcs34725::tcs34725() : agc_cur(0), isAvailable(0), isSaturated(0) {
}

// initialize the sensor
boolean tcs34725::begin(void) {
  tcs = Adafruit_TCS34725(agc_lst[agc_cur].at, agc_lst[agc_cur].ag);
  if ((isAvailable = tcs.begin())) 
    setGainTime();
  return(isAvailable);
}

// Set the gain and integration time
void tcs34725::setGainTime(void) {
  tcs.setGain(agc_lst[agc_cur].ag);
  tcs.setIntegrationTime(agc_lst[agc_cur].at);
  atime = int(agc_lst[agc_cur].at);
  atime_ms = ((256 - atime) * 2.4);  
  switch(agc_lst[agc_cur].ag) {
  case TCS34725_GAIN_1X: 
    againx = 1; 
    break;
  case TCS34725_GAIN_4X: 
    againx = 4; 
    break;
  case TCS34725_GAIN_16X: 
    againx = 16; 
    break;
  case TCS34725_GAIN_60X: 
    againx = 60; 
    break;
  }        
}

// Retrieve data from the sensor and do the calculations
void tcs34725::getData(void) {
  // read the sensor and autorange if necessary
  tcs.getRawData(&r, &g, &b, &c);
  while(1) {
    if (agc_lst[agc_cur].maxcnt && c > agc_lst[agc_cur].maxcnt) 
      agc_cur++;
    else if (agc_lst[agc_cur].mincnt && c < agc_lst[agc_cur].mincnt)
      agc_cur--;
    else break;

    setGainTime(); 
    delay((256 - atime) * 2.4 * 2); // shock absorber
    tcs.getRawData(&r, &g, &b, &c);
    break;    
  }

  // DN40 calculations
  ir = (r + g + b > c) ? (r + g + b - c) / 2 : 0;
  r_comp = r - ir;
  g_comp = g - ir;
  b_comp = b - ir;
  c_comp = c - ir;   
  cratio = float(ir) / float(c);

  saturation = ((256 - atime) > 63) ? 65535 : 1024 * (256 - atime);
  saturation75 = (atime_ms < 150) ? (saturation - saturation / 4) : saturation;
  isSaturated = (atime_ms < 150 && c > saturation75) ? 1 : 0;
  cpl = (atime_ms * againx) / (TCS34725_GA * TCS34725_DF); 
  maxlux = 65535 / (cpl * 3);

  lux = (TCS34725_R_Coef * float(r_comp) + TCS34725_G_Coef * float(g_comp) + TCS34725_B_Coef * float(b_comp)) / cpl;
  ct = TCS34725_CT_Coef * float(b_comp) / float(r_comp) + TCS34725_CT_Offset;
}

tcs34725 rgb_sensor;
//

void setup() {
  Serial.begin(115200);
  SWSerial.begin(115200);
  
  pinMode(TACTILE_SW_PIN,INPUT_PULLUP);
  attachInterrupt(1,checkButton,CHANGE);

  pinMode(WHITELED_PIN,OUTPUT);
  pinMode(VIBRATOR_PIN,OUTPUT);

  digitalWrite(WHITELED_PIN, LOW);
  rgb_sensor.begin();
}

static int pushCounter = 0;
static int readCounter = 0;
void checkButton(){
  if(digitalRead(TACTILE_SW_PIN) == LOW){
//    digitalWrite(WHITELED_PIN,HIGH);
    pushCounter++;
  }
  else{
//    digitalWrite(WHITELED_PIN,LOW);
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
  digitalWrite(WHITELED_PIN,HIGH);
  rgb_sensor.getData();
  digitalWrite(WHITELED_PIN,LOW);
  SWSerial.println(readCounter);
//
  Serial.print(F("Gain:")); 
  Serial.print(rgb_sensor.againx); 
  Serial.print(F("x "));
  Serial.print(F("Time:")); 
  Serial.print(rgb_sensor.atime_ms); 
  Serial.print(F("ms (0x")); 
  Serial.print(rgb_sensor.atime, HEX);   
  Serial.println(F(")"));
  
  Serial.print(F("Raw R:")); 
  Serial.print(rgb_sensor.r); 
  Serial.print(F(" G:")); 
  Serial.print(rgb_sensor.g); 
  Serial.print(F(" B:")); 
  Serial.print(rgb_sensor.b); 
  Serial.print(F(" C:")); 
  Serial.println(rgb_sensor.c); 

  Serial.print(F("IR:")); 
  Serial.print(rgb_sensor.ir); 
  Serial.print(F(" CRATIO:"));
  Serial.print(rgb_sensor.cratio); 
  Serial.print(F(" Sat:"));
  Serial.print(rgb_sensor.saturation); 
  Serial.print(F(" Sat75:"));
  Serial.print(rgb_sensor.saturation75); 
  Serial.print(F(" "));
  Serial.println(rgb_sensor.isSaturated ? "*SATURATED*" : "");

  Serial.print(F("CPL:")); 
  Serial.print(rgb_sensor.cpl);
  Serial.print(F(" Max lux:")); 
  Serial.println(rgb_sensor.maxlux);

  Serial.print(F("Compensated R:")); 
  Serial.print(rgb_sensor.r_comp); 
  Serial.print(F(" G:")); 
  Serial.print(rgb_sensor.g_comp); 
  Serial.print(F(" B:")); 
  Serial.print(rgb_sensor.b_comp); 
  Serial.print(F(" C:")); 
  Serial.println(rgb_sensor.c_comp); 
  
  Serial.print(F("Lux:")); 
  Serial.print(rgb_sensor.lux);
  Serial.print(F(" CT:")); 
  Serial.print(rgb_sensor.ct);
  Serial.println(F("K")); 
  
  Serial.println();
//
  delay(10);
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



