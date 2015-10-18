#include <MsTimer2.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 4

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

unsigned long int counter = 0;
int pos = 0;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code


  strip.begin();
  strip.setBrightness(0x40);
  strip.show(); // Initialize all pixels to 'off'

//  int a;
//
//  for (a = 0; a < 12; a++) {
//    strip.setPixelColor(a, strip.Color(0x00, 0x00, 0x00));
//  }
//  strip.show();
//  delay(1000);
//  for (a = 0; a < 12; a++) {
//    strip.setPixelColor(a, strip.Color(0xFF, 0xFF, 0xFF));
//  }
//  strip.show();

  MsTimer2::set(1000/100,cyclePoint);
  MsTimer2::start();
}

void cyclePoint(){
  int a;
  int ooo = (int)counter%4;

  for(a=0; a<12; a++){
    strip.setPixelColor(a,(a%ooo == 0)? strip.Color(0xFF, 0xFF, 0xFF):strip.Color(0x00, 0x00, 0x00));
  }
  strip.show();
    counter++;  
}

void loop() {
  delay(500);
}
