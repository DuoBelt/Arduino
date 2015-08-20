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

#define SD_GATE_PIN (3)

void setup() {
  Serial.begin(115200);

  pinMode(SD_GATE_PIN,INPUT);
}

void loop() {
}
