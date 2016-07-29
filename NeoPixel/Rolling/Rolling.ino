#include <Adafruit_NeoPixel.h>

#define PIN 6
#define MANY 12

Adafruit_NeoPixel strip = Adafruit_NeoPixel(MANY, PIN, NEO_GRB + NEO_KHZ800);

#define DELAY_MIN 20
#define DELAY_MAX 75

unsigned int top = 0;
unsigned int msec = DELAY_MIN;
bool accel = true;

static unsigned long colors[MANY];

void setup() {
  Serial.begin(115200);


  strip.begin();

  colors[0] = strip.Color(255, 255, 255);
  colors[1] = strip.Color(63, 63, 63);
  colors[2] = strip.Color(31, 31, 31);
  colors[3] = strip.Color(0, 0, 0);
  colors[4] = strip.Color(0, 0, 0);
  colors[5] = strip.Color(0, 0, 0);
  colors[6] = strip.Color(0, 0, 0);
  colors[7] = strip.Color(0, 0, 0);
  colors[8] = strip.Color(0, 0, 0);
  colors[9] = strip.Color(0, 0, 0);
  colors[10] = strip.Color(0, 0, 0);
  colors[11] = strip.Color(0, 0, 0);

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

