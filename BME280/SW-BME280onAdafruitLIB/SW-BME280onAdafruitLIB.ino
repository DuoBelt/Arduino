#include <SoftwareSerial.h>
#include <Wire.h>
#include <SPI.h>
#include <avr/sleep.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// スイッチサイエンスのそれをAdafruitのライブラリで動かすシリーズ(笑)

// Vio - NC
// Vcore - Vcc
// GND - GND
// CS8(?) - Vcc
// SDI - A4
// SCK - A5
// SDO - GND

Adafruit_BME280 bme; // I2C

#define SAMPLES 10

#define BT_RX 5
#define BT_TX 6

SoftwareSerial BTSerial(BT_RX, BT_TX);

class TPH {
  public:
    void init() {
      isAvailable = false;
      counter = 0;
    }
    void updateTPH() {
      unsigned int offset = counter % SAMPLES;
      unsigned int samples = (counter < SAMPLES) ? counter : SAMPLES;

      _T[offset] = bme.readTemperature();
      _P[offset] = bme.readPressure() / 100.0F;
      _H[offset] = bme.readHumidity();

      if (samples) {
        unsigned int a;
        unsigned int b;
        float *pT = _T;
        float *pP = _P;
        float *pH = _H;
        T = P = H = 0.0;
        for (a = 0, b = samples; b--; a++) {
          T += *pT++;
          P += *pP++;
          H += *pH++;
        }
        T /= samples;
        P /= samples;
        H /= samples;
        isAvailable = true;
      }
      counter++;
    }
    unsigned int counter;
    bool isAvailable;
    float T, P, H;
  private:
    float _T[SAMPLES];
    float _P[SAMPLES];
    float _H[SAMPLES];
};

TPH tph;

void setup() {
  Serial.begin(115200);
  Serial.println("start BME280 Logger");

  BTSerial.begin(115200);

  tph.init();
  bme.begin(0x76); // Adafruit:0x77 SwitchScience:0x76
}

void loop() {
  tph.updateTPH();

  if (tph.isAvailable && ((tph.counter % 1) == 0)) {
    String info = String("T=") + tph.T + String(",P=") + tph.P + String(",H=") + tph.H;
    Serial.println(info);
    BTSerial.println(info);
    delay(10);
  }

  for(int a=0; a<15; a++){
    delayWDT(6);
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

