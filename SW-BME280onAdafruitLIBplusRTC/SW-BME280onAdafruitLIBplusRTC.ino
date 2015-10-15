#define INTERVAL_SECS (60)
#define USE_LED true
#define USE_RTC false
#define USE_BT true

#if USE_BT
#include <SoftwareSerial.h>
#endif
#include <Wire.h>
#include <SPI.h>
#include <avr/sleep.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#if USE_RTC
#include <RTC8564.h>
#endif

// スイッチサイエンスのそれをAdafruitのライブラリで動かすシリーズ(笑)

// Vio - NC
// Vcore - Vcc
// GND - GND
// CS8(?) - Vcc
// SDI - A4
// SCK - A5
// SDO - GND

// d3 - RTC8564::INT

Adafruit_BME280 bme; // I2C

#if USE_BT
#define BT_RX 5
#define BT_TX 6
#endif

#if USE_LED
#define LEDPIN 13
#endif

#if USE_BT
SoftwareSerial BTSerial(BT_RX, BT_TX);
#endif

#if USE_RTC
RTC8564 rtc;
#endif

#if USE_RTC
void timerHandler()
{
}
#endif

void setup() {
  Serial.begin(115200);
#if USE_RTC
  Serial.println("start BME280 Logger with RTC");
#else
  Serial.println("start BME280 Logger with delayWDT");
#endif

#if USE_BT
  BTSerial.begin(115200);
#endif

  bme.begin(0x76); // Adafruit:0x77 SwitchScience:0x76

#if USE_RTC
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(1, timerHandler, FALLING);

  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );
#endif

#if USE_LED
  pinMode(LEDPIN, OUTPUT);
#endif
}

void loop() {
#if USE_LED
  digitalWrite(LEDPIN, HIGH);
#endif
  float T = bme.readTemperature();
  float P = bme.readPressure() / 100.0F;
  float H = bme.readHumidity();
  String info = String("T=") + T + String(" P=") + P + String(" H=") + H;
  Serial.println(info);
#if USE_BT
  BTSerial.println(info);
#endif
  delay(10);
#if USE_LED
  digitalWrite(LEDPIN, LOW);
#endif
#if USE_RTC
  sleep_mode();
#else
  for (int a = 0; a < INTERVAL_SECS; a++) {
    delayWDT(6);
  }
#endif
}

#if USE_RTC
#else
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
#endif
