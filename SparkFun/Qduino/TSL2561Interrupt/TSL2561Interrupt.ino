#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <avr/sleep.h>

#define PIN_INT (15)

SFE_TSL2561 light;

boolean gain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int ms;  // Integration ("shutter") time in milliseconds

void setup()
{
  while(true){
    if(Serial) break;
  }
  
  Serial.begin(115200);
  Serial.println("TSL2561 example sketch");

  light.begin();

  unsigned char ID;

  if (light.getID(ID)){
    Serial.print("Got factory ID: 0X");
    Serial.print(ID, HEX);
    Serial.println(", should be 0X5X");
  }
  else  {
    byte error = light.getError();
    printError(error);
  }

  gain = 0;
  unsigned char time = 2;

  Serial.println("Set timing...");
  light.setTiming(gain, time, ms);

// ----------------------------------------------------
  pinMode(PIN_INT, INPUT_PULLUP);

  PCMSK0 |= (1 << PCINT1); // D15
  PCICR |= (1 << PCIE0); // enable PCI

  light.setInterruptThreshold(200, 250);
  light.setInterruptControl(1, 1);

  set_sleep_mode(SLEEP_MODE_IDLE);
// ----------------------------------------------------
  Serial.println("Powerup...");
  light.setPowerUp();

}

bool gotInterrupt = true;

ISR(PCINT0_vect) {
  bool value = digitalRead(PIN_INT);
  light.clearInterrupt();
  gotInterrupt = true;
}

void loop()
{
  if (gotInterrupt) {
    gotInterrupt = false;
    unsigned int data0, data1;

    if (light.getData(data0, data1))  {
      Serial.print("data0: ");
      Serial.print(data0);
      Serial.print(" data1: ");
      Serial.print(data1);

      double lux;    // Resulting lux value
      boolean good;  // True if neither sensor is saturated

      good = light.getLux(gain, ms, data0, data1, lux);

      // Print out the results:

      Serial.print(" lux: ");
      Serial.print(lux);
      if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
    }
    else    {
      // getData() returned false because of an I2C error, inform the user.

      byte error = light.getError();
      printError(error);
    }
  }
  else {
    sleep_mode();
  }
}

void printError(byte error)
// If there's an I2C error, this function will
// print out an explanation.
{
  Serial.print("I2C error: ");
  Serial.print(error, DEC);
  Serial.print(", ");

  switch (error)
  {
    case 0:
      Serial.println("success");
      break;
    case 1:
      Serial.println("data too long for transmit buffer");
      break;
    case 2:
      Serial.println("received NACK on address (disconnected?)");
      break;
    case 3:
      Serial.println("received NACK on data");
      break;
    case 4:
      Serial.println("other error");
      break;
    default:
      Serial.println("unknown error");
  }
}

