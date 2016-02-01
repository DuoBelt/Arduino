#include <Adafruit_TCS34725.h> // RGB Senser
#include <Wire.h> // I2C Trans
#include <avr/sleep.h>
#include <SoftwareSerial.h> // BT

//
// Color Picker 2015.12.01 a
//
//  Pro Trinket Wiring ...
//  D3 <--- tactile switch (PULL-UP)
//  D4 <--- LED ON
//  D5 <--- Bluetooth RX
//  D6 ---> Bluetooth TX
//  A4 ---> I2C - SDA
//  A5 ---> I2C - SDC

#define TACTILE_SW_PIN 3 //タクトSW接続ピン(Pull-UP)
#define LEDPIN 4 //LED PIN番号
#define PIN_RX 5 // ソフトウエアシリアルRXピン番号
#define PIN_TX 6 // ソフトウエアシリアルTXピン番号
#define BAUD 115200 // ソフトウエアシリアルBaud
#define addr 0x29 // TCS34725 I2C Addr

SoftwareSerial mySerial(PIN_RX,PIN_TX);

float red,green,blue;
int val=100; //測光時間
int rgbmax;
int r,g,b;//RGB三色の変数を用意
//int x,y,d,e,h,i;
//int z;
int hmax,hmin,hh,hss; 
float hhf,hssf;
int COL=0;
// int speakerPin = 12;
int c_data,r_data,g_data,b_data;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

static unsigned int pushCounter = 0; // カウンター(デバッグの際に必要なら使いましょう)

void onSwitched(){
  pushCounter++; // 上記カウンターをインクリメントするだけです
}

void setup(){
  pinMode(TACTILE_SW_PIN,INPUT_PULLUP);
// 以下2行追加
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // スリープの際のモードをこれに初期設定しておきます
  attachInterrupt(1,onSwitched,FALLING); // GPIO3がHIGH->LOWとなった時に割り込み1が発生、sleepから目覚めるのと同時に上記onSwtihedが実行されます
// 以上2行追加
  pinMode(LEDPIN,OUTPUT);
  pinMode(PIN_RX,INPUT);
  pinMode(PIN_TX,OUTPUT);
  //シリアル通信設定
//  Serial.begin(9600);
  mySerial.begin(BAUD);
  Wire.begin();

 if (tcs.begin()) {
    mySerial.println("Found sensor");
  } else {
    mySerial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }

  digitalWrite(LEDPIN, LOW);

}

void loop(){
  uint16_t tc_clear, tc_red, tc_green, tc_blue;  

  sleep_mode(); // ここで予め設定されたモードでsleep状態に遷移、外部割り込み(今回は1)で抜けてきます
  
  int sw = digitalRead(TACTILE_SW_PIN);
//  if (sw == LOW){ // このif文は不要です

    digitalWrite(LEDPIN,HIGH);
    delay(300);
  
    tcs.getRawData(&tc_red, &tc_green, &tc_blue, &tc_clear); //***************
    delay(500);

    digitalWrite(LEDPIN,LOW);

/*    mySerial.print("R:"); mySerial.print(tc_red);
    mySerial.print(" G:"); mySerial.print(tc_green);
    mySerial.print(" B:"); mySerial.print(tc_blue);
    mySerial.print(" C:"); mySerial.print(tc_clear);
    mySerial.println(); */
  
    red = 255 * float(tc_red) / float(tc_clear);
    green = 255 * float(tc_green) / float(tc_clear);
    blue = 255 * float(tc_blue) / float(tc_clear);

    COL=0;
    r = int(red*6/10) ;
    g = int(green*7/10) ;
    b = int(blue*9/10) ;
  
/*  // RGB間の補正
  r=r*5/10;
  g=g*7/10;
  b=b*10/10; */
  
/*    mySerial.print("red="); mySerial.print(red,2);
    mySerial.print(" green="); mySerial.print(green,2);
    mySerial.print(" blue="); mySerial.print(blue,2);
    mySerial.print("r="); mySerial.print(r,DEC);
    mySerial.print(" g="); mySerial.print(g,DEC);
    mySerial.print(" b="); mySerial.print(b,DEC);
    mySerial.println(); */
    
    hmax = max(r,g);
    hmax = max(hmax,b);
    hmin = min(r,g);
    hmin = min(hmin,b);
    if (hmax == hmin){
      hh = 560;
    } else if (hmax == r){
      hhf = 60*float(g-b)/float(hmax-hmin);
      hh = int(hhf);
    } else if (hmax == g){
      hhf = 60*float(b-r)/float(hmax-hmin)+120;
      hh = int(hhf);
    } else if (hmax == b){
      hhf = 60*float(r-g)/float(hmax-hmin)+240;
      hh = int(hhf);
    } else {
      hh = 560;
    }

    if (hh < 0){
      hh = hh + 360;
    }
  
    hssf = 255 * (hmax - hmin) / hmax;
    hss = int(hssf);
     
//    mySerial.print("H=");
//    mySerial.print(hh);  
//    mySerial.print(" hhf="); mySerial.print(hhf,2); mySerial.print("  ");
//    mySerial.print(" S="); mySerial.print(hss,DEC);
//    mySerial.print(" V="); mySerial.print(hmax,DEC);
//    mySerial.println(); 

    if(hh<39) {
      COL=1;
      mySerial.println("橙");
    } else if (hh<89) {
      COL=2;
      mySerial.println("黄色");
    } else if (hh<129) {
      COL=3;
      mySerial.println("黄緑");
    } else if (hh<150) {
      COL=4;
      mySerial.println("緑");
    } else if (hh<169) {
      COL=5;
      mySerial.println("青緑");
    } else if (hh<209) {
      COL=6;
      mySerial.println("水色");
    } else if (hh<231) {
      COL=7;
      mySerial.println("青");
    } else if (hh<275) {
      COL=8;
      mySerial.println("紫");
    } else if (hh<325) {
      COL=9;
      mySerial.println("ピンク");
    } else if (hh<360) {
      COL=10;
      mySerial.println("赤");
    } else if (hh<361) {
      COL=10;
      mySerial.println("赤");
    } else {
      COL=11;
    }
  
//    delay(500); // このdelayは不要です

//  }
}
