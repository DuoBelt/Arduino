#include <TimerOne.h>

#define DELAYSECS (1000*1)

static unsigned int counter = 0;

void cyclePoint(){
  counter++;  
}

void setup() {
  counter = 0;

  Serial.begin(115200);
  Serial.println("Start TimerOne basictest");

//  なぜかこの書式だと周期がメチャクチャ
//  Timer1.initialize(500*1000);
//  Timer1.attachInterrupt(cyclePoint);

//  この様に引数なしでinitializeし、attachInterruptの2番めの引数に整数で500000と書くと500ms間隔になる
  Timer1.initialize();
  Timer1.attachInterrupt(cyclePoint,500000);

//  だがこの様に引数に演算子を含むと500ms間隔にならない ... なんでやねん!
//  Timer1.attachInterrupt(cyclePoint,500*1000);

//  これもダメ ... なんなんだー!!!
//  unsigned int microSecs = 500*1000;
//  Timer1.attachInterrupt(cyclePoint,microSecs);
}

void loop() {
  Serial.println(counter);
  delay(DELAYSECS);
}
