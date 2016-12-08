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

void setup()
{
  while (true) {
    if (Serial) {
      break;
    }
  }
  Serial.begin(115200);
  Serial.println("Ready go!");

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

void controlThis() {
  Serial.println(result.value, HEX);
  switch (result.value) {
    case 0xFFC23D: // Play/Pause
      if (play) {
        play = false;
        thisQ.setRGB("yellow");
      }
      else {
        play = true;
        thisQ.ledOff();
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
