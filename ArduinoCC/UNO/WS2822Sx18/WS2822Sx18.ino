//*****************************************************************
// WS2822S
// 2015/01/29
//
// 色情報の設定　setColor(LEDの番号、赤、緑、青)
// 色情報の送信  send()
//
//******************************************************************

#include "Ws2822s.h"
#define NUM_PIXELS 18 // 使用するWS2822Sの数
#define LED_PIN 13 // WS2822SのDAIピンにつなげるArduinoのピン番号

Ws2822s LED(LED_PIN, NUM_PIXELS);

void setup()
{
}

void loop()
{
  for (int i = 0; i < NUM_PIXELS; ++i) {
    LED.setColor(i, 0xFF, 0xFF, 0xFF);
    LED.send();
    delay(1000/1);
    LED.setColor(i, 0x00, 0x00, 0x0);
    LED.send();
    delay(1000/2);
  }
}
