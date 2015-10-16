#include <Wire.h>
#include <ST7032.h>

ST7032 lcd;
static unsigned int counter = 0;

void setup() {
  Serial.begin(115200);

  Serial.println("setup");

  lcd.begin(16,2);
  lcd.setContrast(63);
}

void loop() {
  lcd.setCursor(0,0);
  lcd.print("0123456789012345");
  lcd.setCursor(0,1);
  lcd.print("abcdefghijklmnop");

  if(counter%2){
    lcd.noDisplay();
  }
  else{
    lcd.display();
  }
  counter++;
  delay(1000);
}
