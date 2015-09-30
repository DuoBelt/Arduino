#include <SoftwareSerial.h>
#include<avr/sleep.h>
#include <avr/wdt.h>

#include <Wire.h>

#define SAMPLES 60

#define PIN_RX 5
#define PIN_TX 6
#define BAUD 9600
SoftwareSerial mySerial(PIN_RX, PIN_TX);

// -------------------------------------------------------------------------------------------
class BME280 {
  public:
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    BME280(int address) { // Constructor
      Serial.println("at contructor");
      int a;
      i2cAddress = address;
      pos = 0;
      for(a=0;a<SAMPLES; a++){
        dT[a] = dP[a] = dH[a] = 0.0;
      }
      counter = 0L;
    }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    String toText(){
      return text;      
    }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void writeReg(uint8_t reg_address, uint8_t data)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(reg_address);
  Wire.write(data);
  Wire.endTransmission();
}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    void start(){
      uint8_t osrs_t = 1;             //Temperature oversampling x 1
      uint8_t osrs_p = 1;             //Pressure oversampling x 1
      uint8_t osrs_h = 1;             //Humidity oversampling x 1
      uint8_t mode = 3;               //Normal mode
      uint8_t t_sb = 5;               //Tstandby 1000ms
      uint8_t filter = 0;             //Filter off
      uint8_t spi3w_en = 0;           //3-wire SPI Disable
    
      uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
      uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;
      uint8_t ctrl_hum_reg  = osrs_h;
    
      writeReg(0xF2, ctrl_hum_reg);
      writeReg(0xF4, ctrl_meas_reg);
      writeReg(0xF5, config_reg);
    }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    void update(){
      int a;
      int ss;
      
      counter++;
      ss = counter<SAMPLES? (int)counter:SAMPLES;
      
      readTPH();
      unsigned int offset = (pos++)%SAMPLES;
      dT[offset] = cT;
      dP[offset] = cP;
      dH[offset] = cH;

      aT = aP = aH = 0.0;
      for(a=0; a<ss; a++){
        aT += dT[a];
        aP += dP[a];
        aH += dH[a];
      }
      aT /= ss;
      aP /= ss;
      aH /= ss;

      text = String("T="+String(aT)+","+"P="+String(aP)+","+"H="+String(aH));
    }
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  private:
    unsigned long counter;
    int i2cAddress;
    unsigned int pos;
    double dT[SAMPLES];
    double dP[SAMPLES];
    double dH[SAMPLES];
    double aT;
    double aP;
    double aH;
    double cT;
    double cP;
    double cH;
    String text;
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    void readTPH(){
      cT = 27.5;
      cP = 1005.0;
      cH = 56.7;
    }
};
// -------------------------------------------------------------------------------------------

BME280 bme280 = BME280(0x76);

void setup() {
  Serial.begin(115200);
  mySerial.begin(BAUD);
  Wire.begin();
  Serial.println("at setup");
}

void loop() {
  int a;
  bme280.update();
  String text = bme280.toText();
  Serial.println(text);
  mySerial.println(text);
  delay(10);
  for(a=0; a<60; a++){
    delayWDT(6);
  }
}

// ---------------------------------------------------------------------
void delayWDT(unsigned long t) {        // パワーダウンモードでdelayを実行
  delayWDT_setup(t);                    // ウォッチドッグタイマー割り込み条件設定
  ADCSRA &= ~(1 << ADEN);               // ADENビットをクリアしてADCを停止（120μA節約）
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);  // パワーダウンモード
  sleep_enable();

  sleep_mode();                         // ここでスリープに入る

  sleep_disable();                      // WDTがタイムアップでここから動作再開
  ADCSRA |= (1 << ADEN);                // ADCの電源をON (|=が!=になっていたバグを修正2014/11/17)
}
// ---------------------------------------------------------------------
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
// ---------------------------------------------------------------------
ISR(WDT_vect) {                         // WDTがタイムアップした時に実行される処理
  //  wdt_cycle++;                        // 必要ならコメントアウトを外す
}

