#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void setup() {
  Serial.begin(115200);
  Serial.println("PCA9865 PWM test!");

  pwm.begin();
  pwm.setPWMFreq(60);  // This is the maximum PWM frequency
    
  uint8_t twbrbackup = TWBR;
  TWBR = 12; // upgrade to 400KHz!
//
  randomSeed(analogRead(0));
//
}

void loop() {
  int a;
  String text = "RGB = [";
  Serial.println("Hey!");
  for(a=0; a<3; a++){
    int value = random(4096);
    pwm.setPWM(a,0,value);
    text += String(value);
    text += ",";
    delay(1000/50);
  }
  text += "]";
  Serial.println(text);
//  for (uint16_t i=0; i<4096; i +=8) {
//    for (uint8_t pwmnum=0; pwmnum < 1; pwmnum++) {
//      int value = (i + (4096/16)*pwmnum) % 4096 ;
//      pwm.setPWM(pwmnum, 0, value);
//  String info = String("Valuer = ")+String(value);
//  Serial.println(info);
//  delay(1000/100);
//    }
//  }
  
}
