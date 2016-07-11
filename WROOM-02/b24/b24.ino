#include <dummy.h>

#include <Adafruit_INA219.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
extern "C" {
#include "user_interface.h"
}

Adafruit_BME280 bme; // I2C
Adafruit_INA219 ina219; // ina219
Ticker ticker;

#define LEDPIN (13)

String thisMAC = "";
static unsigned int upCount = 0L;

static float lastSendV = 0.0;
static float lastSendA = 0.0;

static int delayMS = 1000 * 10;

void setup() {
  Serial.begin(115200);
  Serial.println("Start collection");

  pinMode(LEDPIN, OUTPUT);

  const char ssid[] = "ms101";
  const char password[] = "sekitakovich";

  wifi_set_sleep_type(LIGHT_SLEEP_T);

  unsigned char MAC_STA[6];
  unsigned char *MAC  = WiFi.macAddress(MAC_STA);

  thisMAC =
    String(MAC[0], HEX) + ":" +
    String(MAC[1], HEX) + ":" +
    String(MAC[2], HEX) + ":" +
    String(MAC[3], HEX) + ":" +
    String(MAC[4], HEX) + ":" +
    String(MAC[5], HEX);
  Serial.println(thisMAC);

  digitalWrite(LEDPIN, HIGH);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000 * 1);
    Serial.print(".");
  }
  Serial.println("");
  digitalWrite(LEDPIN, LOW);

  bme.begin(0x76);
  Serial.println("Measuring T:P:H with BME280.");
  ina219.begin();
  Serial.println("Measuring voltage and current with INA219.");
}

