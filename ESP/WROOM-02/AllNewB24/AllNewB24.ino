#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
extern "C" {
#include <user_interface.h>
}

String thisMacAddress = "";

#include <Adafruit_BME280.h>
#include <Adafruit_INA219.h>

#define LEDPIN (13)

class B24 {
  public:
    B24(char *ssid, char *password) {
      ssid = ssid;
      password = password;
    }
  private:
    char *ssid;
    char *passsord;
    Adafruit_INA219 ina;
    Adafruit_BME280 bme;
}

B24 b24 = B24("ms101","sekitaokvich");

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);

  wifi_set_sleep_type(LIGHT_SLEEP_T);

}

