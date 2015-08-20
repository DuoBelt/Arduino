#include <Adafruit_NeoPixel.h>
#include <MsTimer2.h>

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
static int envCollection[ENV_COLLECTION_WIDTH];
static int *epC;
static int *epS;
static int *epE;

void checkTheGate(){
  int value = analogRead(SD_GATE_PIN);
  isGateOpen = value;
  return;
}

void setup() {
  Serial.begin(115200);

  isGateOpen = 0;
  memset(envCollection,0,sizeof(envCollection));
  epS = &envCollection[0];
  epE = &envCollection[ENV_COLLECTION_WIDTH];
  epC = epS;
  
  pinMode(SD_GATE_PIN,INPUT);
  pinMode(EQ_RESET_PIN,OUTPUT);
  pinMode(EQ_STROBE_PIN,OUTPUT);
  pinMode(NP_DIN_PIN,OUTPUT);

  attachInterrupt(SD_GATE_IRQ,checkTheGate,CHANGE);
}

void refreshMeter(intenv){
  *epC = env;  
}

void loop() {
  if(isGateOpen){
    int env = analogRead(SD_ENV_PIN); // read envelope value
    refreshMeter(env);
  }
  else{
    
  }
  delay(1000/100);
}
