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

void loop() {

  float shuntvoltage = ina219.getShuntVoltage_mV();
  float busvoltage = ina219.getBusVoltage_V();
  float current_mA = ina219.getCurrent_mA();
  float loadvoltage = busvoltage + (shuntvoltage / 1000);

  float t = bme.readTemperature();
  float p = bme.readPressure() / 100.0F;
  float h = bme.readHumidity();

  static char host[] = "www.klabo.co.jp";
  char thisURL[0x100];

  delayMS = 1000 * ((loadvoltage > lastSendV) ? 5 : 60);


  String getOption = "t=" + String(t) + "&h=" +  String(h) + "&p=" + String(p) + "&mac=" + thisMAC + "&up=" + upCount++ + "&spv=" + String(loadvoltage, 3) + "&spa=" + String(current_mA, 3);
  String getString = String("/tph.php?")  + getOption;

  getString.toCharArray(thisURL, sizeof(thisURL));

  uploadThis(host, thisURL, 80);

  delay(delayMS);
  lastSendV = loadvoltage;
}


