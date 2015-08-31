#include "MsTimer2.h"
#include "avr/sleep.h"

#define USE_SS true

//
// use 'SoftwareSerial'
//

#if USE_SS
#include "SoftwareSerial.h"
#define PIN_RX 5
#define PIN_TX 6
SoftwareSerial mySerial(PIN_RX,PIN_TX);
#else
#define PIN_RX 0
#define PIN_TX 1
#endif

#define BAUD 9600

static int counter = 0;

void loudSpeaker(){
#if USE_SS
  mySerial.println(counter++);
#else
  Serial.println(counter++);
#endif
  digitalWrite(13,(counter%2)? HIGH:LOW);
}

void setup() {
#if USE_SS
  mySerial.begin(BAUD);
#else
  Serial.begin(BAUD);
#endif

  MsTimer2::set(1000*3,loudSpeaker);
  MsTimer2::start();

  set_sleep_mode(SLEEP_MODE_IDLE);
//  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  pinMode(13,OUTPUT);
}

void loop() {
 sleep_mode(); 
}

