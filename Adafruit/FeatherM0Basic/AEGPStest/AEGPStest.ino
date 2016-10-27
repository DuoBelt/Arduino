#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

#define PIN_TSW (5)
#define PIN_LED (6)

#define PIN_S2_RX (11)
#define PIN_S2_TX (10)

Uart Serial2 (&sercom1, PIN_S2_RX, PIN_S2_TX, SERCOM_RX_PAD_0, UART_TX_PAD_2);

volatile unsigned long aCounter = 0;

void SERCOM1_Handler() {
  Serial2.IrqHandler();
}

void onTSW() {
  aCounter = 0;
}

int offset = 0;
char buffer[0x400];

void setup() {
  Serial1.begin(115200);
  Serial2.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_TSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TSW), onTSW, FALLING);

  pinPeripheral(PIN_S2_RX, PIO_SERCOM);
  pinPeripheral(PIN_S2_TX, PIO_SERCOM);

  delay(100);

  Serial1.println((char *)"PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); // only RMC
  Serial1.println((char *)"PMTK220,5000"); // sec interval
  Serial1.println((char *)"PMTK250,1,0,115200"); // RTMC
  Serial1.println((char *)"PMTK301,1"); // RTMC
  Serial1.println((char *)"PMTK330,1"); // Tokyo - JAPAN

  Serial2.println("Version " + String(__TIMESTAMP__) + " start");
}

void loop() {
  int bytes;
  if ((bytes = Serial1.available()) > 0) {
    while (bytes--) {
      char c = Serial1.read();
      buffer[offset++] = c;
      if(c == 0x0a){
        buffer[offset] = 0;
        Serial2.print(buffer);
        offset = 0;
      }
    }
  }
  else {
    //    Serial2.println("zzz ...");
    delay(1000);
  }
}
