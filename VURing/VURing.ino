#include <MCP3208.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_VALUE_MAX 0xFF

#define NP_DIN_PIN 4
#define NP_LED_COUNT 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NP_LED_COUNT, NP_DIN_PIN, NEO_GRB + NEO_KHZ800);

MCP3208 adc(10);

//
// VURing --- VU + Spectrum analyzer on Neopixel
//
// Sound Detector::Gate -> Trinket::D3(INT)
// Sound Detector::Envelope -> Trinket::A0
// Sound Detector::Audio -> MSGEQ7::Audio
// MSGEQ7::Strobe <- Trinket::D5
// MSGEQ7::Reset <- Trinket::D6
// Neopixel::DATA <- Trinket::D4

#define SD_GATE_PIN 3 // Notice! INT-1
#define SD_GATE_IRQ 1
#define SD_ENV_PIN A0

#define EQ_OUT_PIN A1
#define EQ_RESET_PIN 5
#define EQ_STROBE_PIN 6

#define NP_DIN_PIN 4

#define ENV_COLLECTION_WIDTH 5

static int isGateOpen = 0;
static int lastVU = 0;
static int envCollection[ENV_COLLECTION_WIDTH];
static int *epC;
static int *epS;
static int *epE;

static unsigned char RGB[] = {0, 0, 0};
static int offValue;

void checkTheGate() {
  int value = digitalRead(SD_GATE_PIN);
  isGateOpen = value;
  Serial.println(value);
  return;
}

void setup() {
  Serial.begin(115200);

  memset(envCollection, 0, sizeof(envCollection));
  epS = &envCollection[0];
  epE = &envCollection[ENV_COLLECTION_WIDTH];
  epC = epS;

  pinMode(SD_GATE_PIN, INPUT);
  pinMode(EQ_RESET_PIN, OUTPUT);
  pinMode(EQ_STROBE_PIN, OUTPUT);
  pinMode(NP_DIN_PIN, OUTPUT);

  attachInterrupt(SD_GATE_IRQ, checkTheGate, CHANGE);

  strip.begin();
  strip.setBrightness(0x3F);
//  strip.setBrightness(0x7F);
  //  strip.setBrightness(0xFF);
  offValue = strip.Color(0, 0, 0);

  adc.begin();

  return;
}

void updateRGB() {
  int a;
  int bandValue[6];

  for(a=0; a<6; a++){
    bandValue[a] = adc.analogRead(a);
  }

  RGB[0] = (unsigned char)(( bandValue[0]) / (1 * 1));

  int ooo = 0;
  for(a=1; a<5; a++){
    int ppp;
    if((ppp = bandValue[a])>ooo){
      ooo = ppp;    
    }
  }
  RGB[1] = ooo;
 
  RGB[2] = (unsigned char)(( bandValue[5]) / (1 * 1));
//
//  RGB[0] = (unsigned char)((bandValue[0]>bandValue[1]? bandValue[0]:bandValue[1]) / (1 * 1));
//  RGB[1] = (unsigned char)((bandValue[2]>bandValue[3]? bandValue[2]:bandValue[3]) / (1 * 1));
//  RGB[2] = (unsigned char)((bandValue[4]>bandValue[5]? bandValue[4]:bandValue[5]) / (1 * 1));
//
//  RGB[0] = (unsigned char)((bandValue[0]) / (1 * 1));
//  RGB[1] = (unsigned char)((bandValue[1] + bandValue[2] + bandValue[3] + bandValue[4]) / (1 * 4));
//  RGB[2] = (unsigned char)(( bandValue[5]) / (1 * 1));

//  RGB[0] = (unsigned char)(bandValue[0] / (1 * 1));
//  RGB[1] = (unsigned char)((bandValue[1] + bandValue[2] + bandValue[3] + bandValue[4]) / (1 * 4));
//  RGB[2] = (unsigned char)(bandValue[5] / (1 * 1));

//  RGB[0] = (unsigned char)((bandValue[0] + bandValue[1] + bandValue[2]) / (1 * 3));
//  RGB[1] = (unsigned char)((bandValue[3] + bandValue[4]) / (1 * 2));
//  RGB[2] = (unsigned char)(bandValue[5] / (1 * 1));

  //  if(pos){
  //    strip.setBrightness(0xFF/pos);
  //  }
  //

  //  String info = "R: "+String(R)+" "+"G: "+String(G)+" "+"B: "+String(B);
  //  Serial.println(info);
  //  delay(5);
}

void updatePOS(int pos) {
  int a;
  int onValue = strip.Color(RGB[0], RGB[1], RGB[2]);

  for (a = 0; a < pos; a++) {
    strip.setPixelColor(a, onValue);
  }
  for ( ; a < NP_LED_COUNT; a++) {
    strip.setPixelColor(a, offValue);
  }

  strip.show();
  return;
}

int checkEnvelope(int env) {
  int avg;
  int *p;

  *epC++ = env;
  if (epC == epE) {
    epC = epS;
  }

  for (avg = 0, p = epS; p < epE; ) {
    avg += *p++;
  }
  avg /= ENV_COLLECTION_WIDTH;

  return (avg / NP_LED_COUNT);
}

void loop() {
  if (isGateOpen) {
    int env = analogRead(SD_ENV_PIN); // read envelope value
    int thisVU = checkEnvelope(env);
    if (thisVU != lastVU) {
      updateRGB();
      updatePOS(thisVU);


      lastVU = thisVU;
    }
  }
  else {
    updatePOS(0);
  }
//  delay(1000 / 100);
  delayWDT(0);
}
// ---------------------------------------------------------------------
#include<avr/sleep.h>
#include <avr/wdt.h>

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

