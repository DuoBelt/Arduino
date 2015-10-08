#include <Wire.h>

#include <MsTimer2.h>

#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

#define SAMPLES 100
#define INTERVAL (1000/100)

bool readyGo;
unsigned int counter;
float current[SAMPLES];

void setup(void)
{
  uint32_t currentFrequency;

  Serial.begin(115200);
  Serial.println("Start INA219 sampling");

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  ina219.begin();
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //ina219.setCalibration_16V_400mA();

  readyGo = false;
  counter = 0;
  MsTimer2::set(INTERVAL, cyclePoint);
  MsTimer2::start();
}

void cyclePoint() {
  unsigned int offset = counter % SAMPLES;
  current[offset] = ina219.getCurrent_mA();
  counter++;
  if (readyGo == false && counter >= SAMPLES) {
    readyGo = true;
  }
}

void loop(void)
{
  if (readyGo) {
    int a;
    float cSum = 0.0;
    for (a = 0; a < SAMPLES; a++) {
      cSum += current[a];
    }
    cSum /= SAMPLES;
    Serial.print("Current Average = ");
    Serial.println(cSum);
  }
  delay(1000 / 1);
}
