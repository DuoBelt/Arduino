//
// VURing --- VU + Spectrum analyzer on Neopixel
//
// Sound Detector::Gate -> Trinket::D3(INT)
// Sound Detector::Envelope -> Trinket::A0
// Sound Detector::Audio -> MM-3830:: LINE(L)
// Sound Detector::R7 <- PotentioMeter(Gain)
// MM-3830::F01 -> MCP3208::CH0
// MM-3830::F02 -> MCP3208::CH1
// MM-3830::F03 -> MCP3208::CH2
// MM-3830::F04 -> MCP3208::CH3
// MM-3830::F05 -> MCP3208::CH4
// MM-3830::F06 -> MCP3208::CH5
// MCP3208::CS <- Trinket::D10 (SPI)
// MCP3208::Din <- Trinket::D11 (SPI)
// MCP3208::Dout -> Trinket::D12 (SPI)
// MCP3208::CLK <- Trinket::D13 (SPI)
// Neopixel::DATA <- Trinket::D4

#include <MCP3208.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_VALUE_MAX 0xFF

#define NP_DIN_PIN 4
#define NP_LED_COUNT 12

#define SD_GATE_PIN 3 // Notice! INT-1
#define SD_GATE_IRQ 1
#define SD_ENV_PIN A0
#define NP_DIN_PIN 4
#define SPI_PIN 10

#define ENV_COLLECTION_WIDTH 5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NP_LED_COUNT, NP_DIN_PIN, NEO_GRB + NEO_KHZ800);
MCP3208 mcp3208(SPI_PIN);

static int isGateOpen = 0;
static int envCollection[ENV_COLLECTION_WIDTH];
static int *epC;
static int *epS;
static int *epE;

static unsigned char RGB[] = {0, 0, 0};
static int offValue;

void checkTheGate() {
  isGateOpen = digitalRead(SD_GATE_PIN);
  return;
}

void setup() {
  Serial.begin(115200);

  memset(envCollection, 0, sizeof(envCollection));
  epS = &envCollection[0];
  epE = &envCollection[ENV_COLLECTION_WIDTH];
  epC = epS;

  pinMode(SD_GATE_PIN, INPUT);
  pinMode(NP_DIN_PIN, OUTPUT);
  pinMode(SPI_PIN, OUTPUT);

  attachInterrupt(SD_GATE_IRQ, checkTheGate, CHANGE);

  strip.begin();
  strip.setBrightness(0x3F);
  //  strip.setBrightness(0x7F);
  //  strip.setBrightness(0xFF);
  offValue = strip.Color(0, 0, 0);

  mcp3208.begin();

  return;
}

void updateRGB() {
  int a;
  int b;
  int *p;
  int bandValue[6];

  for (a = 0, b = 6, p = bandValue; b--; a++) {
    *p++ = mcp3208.analogRead(a); // analogRead() returns unsigned 16bit integer
  }

  RGB[0] = (unsigned char)(( bandValue[0]) / (4 * 1));

  int ooo = 0;
  for (a = 1; a < 5; a++) {
    int ppp;
    if ((ppp = bandValue[a]) > ooo) {
      ooo = ppp;
    }
  }
  RGB[1] = ooo / (4 * 1);

  RGB[2] = (unsigned char)(( bandValue[5]) / (4 * 1));
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
  int b;
  int onValue = strip.Color(RGB[0], RGB[1], RGB[2]);

  for (a = 0, b = NP_LED_COUNT; b--; a++) {
    strip.setPixelColor(a, a < pos ? onValue : offValue);
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
  int waitParam;
  if (isGateOpen) {
    int thisVU = checkEnvelope(analogRead(SD_ENV_PIN)); // read envelope value
    updateRGB();
    updatePOS(thisVU);

    waitParam = 0;
  }
  else {
    waitParam = 9;
    updatePOS(0);
  }
  delayWDT(waitParam);
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

