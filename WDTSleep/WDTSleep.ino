#define USE_MSTIMER2 false

#if USE_MSTIMER2
#include <MsTimer2.h>
#endif

#include <SoftwareSerial.h>

#include <avr/sleep.h>
#include <avr/wdt.h>

#define RLED 13

SoftwareSerial mySerial(5,6);

static boolean ledStatus = LOW;
static int counter = 0;

#if USE_MSTIMER2
void cyclePoint(){
  counter++;
}
#endif

void setup() {
  pinMode(RLED, OUTPUT);
  Serial.begin(115200);
  mySerial.begin(9600);
 
#if USE_MSTIMER2
  MsTimer2::set(1000,cyclePoint);
  MsTimer2::start();
#endif
}

// the loop function runs over and over again forever
void loop() {
  Serial.println(counter);
  mySerial.println(counter);
  ledStatus = !ledStatus;
  digitalWrite(RLED, ledStatus);
#if USE_MSTIMER2
#else
  counter++;
#endif
  delay(1);
//  delayWDT(6);
  delayWDT(7);
//  delayWDT(8);
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

