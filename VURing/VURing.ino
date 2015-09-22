#include <Adafruit_NeoPixel.h>

#include <MsTimer2.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define NP_DIN_PIN 4
#define NP_LED_COUNT 12
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NP_LED_COUNT, NP_DIN_PIN, NEO_GRB + NEO_KHZ800);

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

#define ENV_COLLECTION_WIDTH 10

static int isGateOpen;
static int lastVU;
static int envCollection[ENV_COLLECTION_WIDTH];
static int *epC;
static int *epS;
static int *epE;

static unsigned char valueRGB[3];
#define OffsetR 0
#define OffsetG 1
#define OffsetB 2

void checkTheGate() {
  int value = analogRead(SD_GATE_PIN);
  isGateOpen = value;
  return;
}

void setup() {
  Serial.begin(115200);

  isGateOpen = 0;
  lastVU = 0;
  memset(envCollection, 0, sizeof(envCollection));
  epS = &envCollection[0];
  epE = &envCollection[ENV_COLLECTION_WIDTH];
  epC = epS;

  memset(valueRGB,0,sizeof(valueRGB));

  pinMode(SD_GATE_PIN, INPUT);
  pinMode(EQ_RESET_PIN, OUTPUT);
  pinMode(EQ_STROBE_PIN, OUTPUT);
  pinMode(NP_DIN_PIN, OUTPUT);

  attachInterrupt(SD_GATE_IRQ, checkTheGate, CHANGE);

  strip.begin();
  strip.show();

  return;
}

void updateNP(int pos) {
  int a;
  for (a=0; a<pos; a++) {
    strip.Color(a, 0xFF, 0xFF, 0xFF);
  }
  for( ; a<NP_LED_COUNT; a++){
    strip.Color(a, 0x00, 0x00, 0x00);
  }
  
  strip.show();
  return;
}

void checkEnvelope(int env) {
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

  // refresh strip
  int thisVU = avg / NP_LED_COUNT;
  if (thisVU != lastVU) {
    updateNP(thisVU);


    lastVU = thisVU;
  }
  return;
}

void loop() {
  if (isGateOpen) {
    int env = analogRead(SD_ENV_PIN); // read envelope value
    checkEnvelope(env);
  }
  else {

  }
  delay(1000 / 100);
}
