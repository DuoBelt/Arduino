#include <avr/sleep.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

/* Assign a unique ID to this sensor at the same time */
#define I2C_ID_ADXL345 (0x3a)

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(I2C_ID_ADXL345);

static unsigned long lastMillis = 0;
static unsigned int counter = 0;

static int prevXYZ[3] = {0};
static int voidRange = 1500;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("Start!");

  set_sleep_mode(SLEEP_MODE_IDLE);

  accel.begin();
  accel.setRange(ADXL345_RANGE_2_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);

}
void checkThis() {
  sensors_event_t event;
  accel.getEvent(&event);

  int xyz[3];
  int a;

  xyz[0] = (int)(event.acceleration.x * 100);
  xyz[1] = (int)(event.acceleration.y * 100);
  xyz[2] = (int)(event.acceleration.z * 100);

  bool action = false;
  int delta;
  for (a = 0; a < 3; a++) {
    delta = xyz[a] - prevXYZ[a];
    if (abs(delta) > voidRange) {
      action = true;
      counter++;
      lastMillis = millis();
      Serial.println("Delta " + String(delta) + " at " + String(a));
      break;
    }
    prevXYZ[a] = xyz[a];
  }

  if (action) {
    //    Serial.println(String(x) + " " + String(y) + " " + String(z));
    //    Serial.println("Action! at " + String(passedSecs));
  }

//  delay(10);
}

void loop(void)
{
  unsigned long time = millis();
//  Serial.println(time);
  if((time - lastMillis) > 1000){
    checkThis();
  }
    delay(100);
//  sleep_mode();
}
