#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

static unsigned int counter = 0;
static int segments = 10;

void setup() {
  while(!Serial);
  Serial.begin(115200);
  Serial.println("PCA9865 PWM test!");

  pwm.begin();
//  pwm.setPWMFreq(60);  // This is the maximum PWM frequency
    
//  uint8_t twbrbackup = TWBR;
//  TWBR = 12; // upgrade to 400KHz!
}

void loop() {
  Serial.println(counter%segments);
  int focus = counter%segments;
  int a;
  for(a=0; a<segments; a++){
    if(a == focus){
      pwm.setPWM(a,0,4095);
    }
    else{
      pwm.setPWM(a,4096,0);
    }
  }
  counter++;
  delay(1000/1);
}
