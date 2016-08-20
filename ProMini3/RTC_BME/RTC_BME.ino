#include <SoftwareSerial.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_BME280.h>

// ATMega328をRTCからの割り込みでスリープから起こす!

#include <Wire.h>
#include <avr/sleep.h>
#include <RTC8564.h>

#define I2C_ID_ADXL345 (0x3a)

#define PIN_LED (4)

#define PIN_RTC_INT (2)
#define PIN_RTC_NUM (0)

#define INTERVAL_SECS (1)

#define SWSP_RX (8)
#define SWSP_TX (9)

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(I2C_ID_ADXL345);
Adafruit_BME280 bme;
RTC8564 rtc;
RTC_TIME thisTime;

SoftwareSerial swserial(SWSP_RX, SWSP_TX);

volatile unsigned long counter = 0;

void timerHandler()
{
  counter++;
}

void setup() {
  pinMode(PIN_LED, OUTPUT);

  //  while (!Serial);
  Serial.begin(115200);

  accel.begin(); // ADXL345
  accel.setRange(ADXL345_RANGE_16_G);

  //  Serial.println("start RTC test");

  //    set_sleep_mode(SLEEP_MODE_IDLE);
  //    set_sleep_mode(SLEEP_MODE_ADC);
  //  set_sleep_mode(SLEEP_MODE_STANDBY);
  //  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  pinMode(PIN_RTC_INT, INPUT_PULLUP);
  attachInterrupt(PIN_RTC_NUM, timerHandler, FALLING);

  rtc.begin();
  rtc.setTimer( rtc.TIMING_1_SEC, INTERVAL_SECS, true, true );

  bme.begin(0x76);

  swserial.begin(115200);

  counter = 0;
}

void loop() {
  digitalWrite(PIN_LED, counter % 2);
  //  Serial.println(counter);
  float t = bme.readTemperature();
  float p = bme.readPressure() / 100.0F;
  float h = bme.readHumidity();
  //
  rtc.now(&thisTime);
  String tph = String(t) + '/' + String(p) + '/' + String(h);
  Serial.println(tph);
  //
  sensors_event_t event;
  accel.getEvent(&event);

  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m/s^2 ");
  //
  swserial.println(counter);
  //
  delay(10); // 次のスリープまでに処理を完了させるための時間稼ぎ
  sleep_mode(); // zzz ...
}
