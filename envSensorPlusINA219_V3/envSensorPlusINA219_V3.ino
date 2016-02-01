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

#define VS 10

String thisMAC = "";
static unsigned int upCount = 0L;

static float lastSendV = 0.0;

static int delayMS = 1000*10;

bool busyFlag = false;

void cyclePoint(){
  if(busyFlag == false){
    float t = 0;
    float p = 0;
    float h = 0;
    float v = 0;
    float loadvoltage = 0;
    float current_mA = 0;

    busyFlag = true;
    
    String info = "t=" + String(t) + "&h=" +  String(h) + "&p=" + String(p) + "&v=" + String(v,3) + "&mac=" + thisMAC + "&up=" + upCount++ + "&spv=" + String(loadvoltage,3) + "&spa=" + String(current_mA,3);
  
    Serial.println(info);
    const char host[] = "www.klabo.co.jp";
    WiFiClient client;
    int httpPort = 80;
    if (client.connect(host, httpPort)) {
      digitalWrite(LEDPIN, HIGH);
      String url = String("/tph.php?")  + info;
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  
      while (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      digitalWrite(LEDPIN, LOW);
    }
    else Serial.println("client???");
    busyFlag = false;
  }
  else{
    Serial.println("busy!");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Start collection");

  pinMode(LEDPIN, OUTPUT);

  const char ssid[] = "CP6F";
  const char password[] = "muraponn";

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

  ticker.attach(5.0,cyclePoint);
}

void loop() {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);

  float t = bme.readTemperature();
  float p = bme.readPressure() / 100.0F;
  float h = bme.readHumidity();

  int adcv =  system_adc_read(); // from TOUT (4.2V - 470K - 150K - GND)
  float v = (float)adcv / (1024 / 4.2);

  lastSendV = v;

    delay(1000*5);
}
