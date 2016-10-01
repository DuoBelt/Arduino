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

#define INTERVAL_SECS (1*1)

#define SWSBT_RX (8)
#define SWSBT_TX (9)

#define SWSDS_RX (10)
#define SWSDS_TX (11)

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(I2C_ID_ADXL345);
Adafruit_BME280 bme;
RTC8564 rtc;
RTC_TIME thisTime;
SoftwareSerial swsBT(SWSBT_RX, SWSBT_TX);
SoftwareSerial swsDS(SWSDS_RX, SWSDS_TX);

volatile unsigned long counter = 0;

/* ******************************************************
   Interrupt handler
****************************************************** */
void timerHandler()
{
  counter += INTERVAL_SECS;
}

/* ******************************************************
   1st. setup
****************************************************** */
void setup() {
  // ++++++++++++ for Serial
  Serial.begin(115200);
  swsBT.begin(115200);
  swsDS.begin(9600);

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

  Serial.println("Start!");
}

/* ******************************************************
   2nd. loop
****************************************************** */
void loop() {
  Serial.println(counter);
  if ((counter % 5) == 0) {
    digitalWrite(PIN_LED, HIGH);
    float t = bme.readTemperature();
    float p = bme.readPressure() / 100.0F;
    float h = bme.readHumidity();
    //
    rtc.now(&thisTime);
    String tph = String(t) + '/' + String(p) + '/' + String(h);
    swsBT.println(tph);
    //
    sensors_event_t event;
    accel.getEvent(&event);

    swsBT.print("X: "); swsBT.print(event.acceleration.x); swsBT.print("  ");
    swsBT.print("Y: "); swsBT.print(event.acceleration.y); swsBT.print("  ");
    swsBT.print("Z: "); swsBT.print(event.acceleration.z); swsBT.print("  "); swsBT.println("m/s^2 ");
    //
    unsigned short dist;
    unsigned char temp;
    String info;

    dist = readD();
    temp = readT();

    info = "Distance = " + String(dist) + " Temp = " + temp;
    swsBT.println(info);
    //
  }
  else {
    digitalWrite(PIN_LED, LOW);
  }
  delay(10); // 次のスリープまでに処理を完了させるための時間稼ぎ
  sleep_mode(); // zzz ...
}
