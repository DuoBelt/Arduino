#include <Adafruit_NeoPixel.h>

#define PIN 6
#define MANY 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(MANY, PIN, NEO_GRB + NEO_KHZ800);

#define DELAY_MIN 20
#define DELAY_MAX 80

unsigned int top = 0;
unsigned int msec = DELAY_MIN;
bool accel = true;

static unsigned long colors[MANY] = {0};

#define RBIT 0x01
#define GBIT 0x02
#define BBIT 0x04

static int cPattern[] = {RBIT | GBIT | BBIT, RBIT, GBIT, BBIT, RBIT | GBIT, RBIT | BBIT, GBIT | BBIT};
static int cIndex = 0;

void setupCLT() {
  int rgb = cPattern[cIndex++];

  colors[0] = strip.Color(rgb & RBIT ? 255 : 0, rgb & GBIT ? 255 : 0, rgb & BBIT ? 255 : 0);
  colors[1] = strip.Color(rgb & RBIT ? 64 : 0, rgb & GBIT ? 64 : 0, rgb & BBIT ? 64 : 0);
  colors[2] = strip.Color(rgb & RBIT ? 32 : 0, rgb & GBIT ? 32 : 0, rgb & BBIT ? 32 : 0);
//  colors[3] = strip.Color(rgb & RBIT ? 16 : 0, rgb & GBIT ? 16 : 0, rgb & BBIT ? 16 : 0);

  if (cIndex == sizeof(cPattern) / sizeof(int)) {
    cIndex = 0;
  }
}

void setup() {
  Serial.begin(115200);


  strip.begin();
/*
  colors[3] = strip.Color(0, 0, 0);
  colors[4] = strip.Color(0, 0, 0);
  colors[5] = strip.Color(0, 0, 0);
  colors[6] = strip.Color(0, 0, 0);
  colors[7] = strip.Color(0, 0, 0);
  colors[8] = strip.Color(0, 0, 0);
  colors[9] = strip.Color(0, 0, 0);
  colors[10] = strip.Color(0, 0, 0);
  colors[11] = strip.Color(0, 0, 0);
*/
  //  strip.show();
}

void loop() {
  unsigned int a;
  unsigned int b;

  //  Serial.println(top);

  for (a = 0, b = MANY; b--; a++) {
    unsigned int pos = (top + a) % MANY;
    strip.setPixelColor(a, colors[pos]);
  }
  strip.show();
  delay(msec);

  if (++top == MANY) {
    top = 0;
    setupCLT();

  }

  if (accel) {
    if (++msec == DELAY_MAX) {
      accel = false;
    }
  }
  else {
    if (--msec == DELAY_MIN) {
      accel = true;
    }
  }
}

