// スイッチサイエンスのそれをAdafruitのライブラリで動かすシリーズ(笑)

// Vio - NC
// Vcore - Vcc
// GND - GND
// CS8(?) - Vcc
// SDI - A4
// SCK - A5
// SDO - GND

#include <Wire.h>
#include <SPI.h>
#include <avr/sleep.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme; // I2C

#define SAMPLES 10

class TPH{
public:
  void init(){
    int a;
    isAvailable = false;
    counter = 0;
//    T = P = H = 0.0;
    for(a=0; a<SAMPLES; a++){
      _T[a] = 0.0;
      _P[a] = 0.0;
      _H[a] = 0.0;
    }
  }
  void saveThis(float t,float p,float h){
    unsigned int a;
    unsigned int b;
    unsigned int offset = counter%SAMPLES;
    unsigned int many = (counter<SAMPLES)? counter:SAMPLES;

    _T[offset] = t;
    _P[offset] = p;
    _H[offset] = h;

    T = P = H = 0.0;
    if(many){
      for(a=0,b=many; b--; a++){
        T += _T[a];
        P += _P[a];
        H += _H[a];
      }
      T /= many;
      P /= many;
      H /= many;
      isAvailable = true;
    }
    counter++;
//    String info = String("T=") + T + String(",P=") + P + String(",H=") + H;
//    Serial.println(info);
  }
  bool isAvailable;
  float T,P,H;
private:
  unsigned int counter;
  float _T[SAMPLES];
  float _P[SAMPLES];
  float _H[SAMPLES];
};

TPH tph;

void setup() {
  Serial.begin(115200);
  Serial.println("start BME280");

  tph.init();
  bme.begin(0x76); // Adafruit:0x77 SwitchScience:0x76
}

void loop() {
  float T = bme.readTemperature();
  float P = bme.readPressure() / 100.0F;
  float H = bme.readHumidity();

  tph.saveThis(T,P,H);

  if(tph.isAvailable){
    String info = String("T=") + tph.T + String(",P=") + tph.P + String(",H=") + tph.H;
    Serial.println(info);
    delay(10);
  
    delayWDT(8);
  }
}
//
void delayWDT(unsigned long t) {        // パワーダウンモードでdelayを実行
  delayWDT_setup(t);                    // ウォッチドッグタイマー割り込み条件設定
  ADCSRA &= ~(1 << ADEN);               // ADENビットをクリアしてADCを停止（120μA節約）
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
  if (ii > 9 ){                         // 変な値を排除
    ii = 9;
  }
  bb =ii & 7;                           // 下位3ビットをbbに
  if (ii > 7){                          // 7以上（7.8,9）なら
    bb |= (1 << 5);                     // bbの5ビット目(WDP3)を1にする
  }
  bb |= ( 1 << WDCE );

  MCUSR &= ~(1 << WDRF);                // MCU Status Reg. Watchdog Reset Flag ->0
  // start timed sequence
  WDTCSR |= (1 << WDCE) | (1<<WDE);     // ウォッチドッグ変更許可（WDCEは4サイクルで自動リセット）
  // set new watchdog timeout value
  WDTCSR = bb;                          // 制御レジスタを設定
  WDTCSR |= _BV(WDIE);
} 

ISR(WDT_vect) {                         // WDTがタイムアップした時に実行される処理
  //  wdt_cycle++;                        // 必要ならコメントアウトを外す
}

