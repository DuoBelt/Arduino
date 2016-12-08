#include <Adafruit_NeoPixel.h>
#include <Qduino.h>

/*
   IRremote: IRrecvDemo - demonstrates receiving IR codes with IRrecv
   An IR detector/demodulator must be connected to the input RECV_PIN.
   Version 0.1 July, 2009
   Copyright 2009 Ken Shirriff
   http://arcfn.com
*/

#include <IRremote.h>

int RECV_PIN = 12;

IRrecv irrecv(RECV_PIN);
decode_results result;

qduino thisQ;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 6, NEO_GRB + NEO_KHZ800);

void setup()
{
  while (true) {
    if (Serial) {
      break;
    }
  }
  Serial.begin(115200);
  Serial.println("Ready go!");

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  thisQ.setup();

  int a;
  for (a = 0; a < 6; a++) {
    if (a % 2) {
      thisQ.ledOff();
    }
    else {
      thisQ.setRGB("green");
    }
    delay(1000);
  }

  irrecv.enableIRIn(); // Start the receiver
}

bool play = false;

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void controlThis() {
  Serial.println(result.value, HEX);
  switch (result.value) {
    case 0xFF30CF: // '1'
      colorWipe(strip.Color(255, 0, 0), 50); // Red
      break;
    case 0xFF18E7: // '2'
      colorWipe(strip.Color(0, 255, 0), 50); // Green
      break;
    case 0xFF7A85: // '3'
      colorWipe(strip.Color(0, 0, 255), 50); // Blue
      break;
    case 0xFFC23D: // Play/Pause
      if (play) {
        play = false;
        colorWipe(strip.Color(0, 0, 0), 50); // Black
      }
      else {
        play = true;
        colorWipe(strip.Color(255, 255, 255), 50); // White
      }
      break;
    default:
      break;
  }
}

void dumpThis() {
  if (result.decode_type == NEC) {
    Serial.println(result.value, HEX);
  }
}

void loop() {
  if (irrecv.decode(&result)) {
    //    Serial.println(result.decode_type);
    //    Serial.println(result.value, HEX);
    //    dumpThis();
    controlThis();
    irrecv.resume(); // Receive the next value
  }
  else {
    delay(100);
  }
}
