#include <SoftwareSerial.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BME280.h>

#include <Wire.h>
#include <avr/sleep.h>
#include <RTC8564.h>

#define I2C_ID_ADXL345 (0x3a)
#define I2C_ID_BME280 (0x76)
#define PIN_LED (4)

#define PIN_RTC_INT (2)
#define PIN_RTC_NUM (0)

#define INTERVAL_SECS (1*5)

#define SWSP_RX (8)
#define SWSP_TX (9)

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(I2C_ID_ADXL345);
Adafruit_BME280 bme;
RTC8564 rtc;
RTC_TIME thisTime;
SoftwareSerial swserial(SWSP_RX, SWSP_TX);

volatile unsigned long counter = 0;

/* ******************************************************
   Interrupt handler
****************************************************** */
void timerHandler()
{
  counter+=INTERVAL_SECS;
}

/* ******************************************************
   1st. setup
****************************************************** */
void setup() {
  // ++++++++++++ for Serial
  Serial.begin(115200);
  swserial.begin(115200);

  // ++++++++++++ for sleep
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  attachInterrupt(PIN_RTC_NUM, timerHandler, FALLING);

  // ++++++++++++ for ADXL345
  accel.begin();
  accel.setRange(ADXL345_RANGE_16_G);

  // ++++++++++++ for RTC
  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

  // ++++++++++++ for BME280
  bme.begin(I2C_ID_BME280);

  // ++++++++++++ for others
  pinMode(PIN_LED, OUTPUT);
  counter = 0;
}

/* ******************************************************
   2nd. loop
****************************************************** */
void loop() {
//  digitalWrite(PIN_LED, counter % 2);
  float t = bme.readTemperature();
  float p = bme.readPressure() / 100.0F;
  float h = bme.readHumidity();
  //
  rtc.now(&thisTime);
  String tph = String(t) + '/' + String(p) + '/' + String(h);
  swserial.println(tph);
  //
  sensors_event_t event;
  accel.getEvent(&event);

  swserial.print("X: "); swserial.print(event.acceleration.x); swserial.print("  ");
  swserial.print("Y: "); swserial.print(event.acceleration.y); swserial.print("  ");
  swserial.print("Z: "); swserial.print(event.acceleration.z); swserial.print("  "); swserial.println("m/s^2 ");
  //
  delay(10); // 次のスリープまでに処理を完了させるための時間稼ぎ
  sleep_mode(); // zzz ...
}
