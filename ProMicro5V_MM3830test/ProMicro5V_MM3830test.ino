#define USE_CONSOLE false

#include <MsTimer2.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 10

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

static int colorValue = 0;
static int bandPins[] = {A7, A9, A6, A8, A0, A1};

void cyclePoint() {
  int a;
  int level[6];

  for (a = 0; a < 6; a++) {
    int val = analogRead(bandPins[a]);
#if USE_CONSOLE
    Serial.print(val);
    Serial.print(" ");
#endif
    level[a] = val;
  }
#if USE_CONSOLE
  Serial.println("");
#endif
  unsigned char R = (level[0] + level[1]) / (4 * 2);
  unsigned char G = (level[2] + level[3]) / (4 * 2);
  unsigned char B = (level[4] + level[5]) / (4 * 2);

  colorValue = strip.Color(R, G, B);
}

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  strip.begin();
  strip.setBrightness(0xFF);
  strip.show(); // Initialize all pixels to 'off'

  while (!Serial);
  Serial.begin(115200);

  for (int a = 0; a < 6; a++) {
    pinMode(bandPins[a], INPUT);
  }

  MsTimer2::set(1000 / 10, cyclePoint);
  MsTimer2::start();
}

void loop() {
  int a;
  for (a = 0; a < 8; a++) {
    strip.setPixelColor(a, colorValue);
  }
  strip.show();
  delay(1000/10);
}

