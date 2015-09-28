#define DEBUG_THIS true

#include <SoftwareSerial.h>
#include <avr/sleep.h>

//
// D3 <--- タクトスイッチ (active HIGH)
// D5 ---> Bluetooth RX
// D6 ---> Bluetooth TX
// D13 ---> (LED)
// A4 <--> I2C:SDA 
// A5 <--> I2C:SCL
//

#define BLUETOOTH_RX PD5
#define BLUETOOTH_TX PD6
#define ONBOARD_LED PB5

SoftwareSerial SWSerial(BLUETOOTH_RX,BLUETOOTH_TX);

#define BUTTON PD3

void dummyRoutine(){
  return; 
}

void readAndSendTheValue(){
#if DEBUG_THIS
  Serial.println("Read the RGB and send it!");
#endif
//
//
//
  return;
}

void setup() {
  Serial.begin(115200);
  SWSerial.begin(9600);

  pinMode(ONBOARD_LED,OUTPUT);

  pinMode(BUTTON,INPUT_PULLUP);
  attachInterrupt(INT1,dummyRoutine,FALLING);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
}

void loop() {
#if DEBUG_THIS
  Serial.println("Enter ZZZ ...");
#endif
  sleep_mode();
#if DEBUG_THIS
  Serial.println("Good morningt!");
#endif
  readAndSendTheValue();
}
