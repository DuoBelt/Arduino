//
//	I2C制御LCDドライバ (ACM1602専用) テスト
//		12/04/26 T.Nakao
//
//		Wireと併用
//

#include <Wire.h>             // I2C
#include <wI2cLcdACM1602.h>   // LCD


wI2cLcdACM1602 lcd;           //

char StrBuf[17];


void setup(void) {
  byte adrs, i;

  Wire.begin();       // I2C初期化
  lcd.begin();        // LCD初期化

  lcd.clear();        // 全クリア
  lcd.noBlink();      // カーソル点滅なし
  lcd.noCursor();     // カーソル表示なし

  delay(1000);

  byte num = 0;
	char ch;
  for(i = 0; i < 16; i++) {
		ch = 0x30 + num;
    lcd.print(ch);
    num = (num + 1) % 10;
  }

  lcd.setCursor(5, 1);          // カーソル位置
  for(i = 0; i < 10; i++) {
		ch = 0x30 + num;
    lcd.print(ch);
    num = (num + 1) % 10;
  }

  delay(1000);

  // 表示OFF/ON
  lcd.noDisplay();
  delay(1000);
  lcd.display();

  delay(1000);

  // 文字列表示
  lcd.clear();            // 全クリア
  lcd.print("test1");     // 文字列表示

  lcd.setCursor(5, 1);    // カーソル位置指定
  lcd.print("test2");     // 文字列表示

  delay(1000);

  // 数値文字列表示
  byte val = 0x56;
  sprintf(StrBuf, " 0x%02X", val);
  lcd.print(StrBuf);

  lcd.setCursor(6, 0);
  lcd.print("123 ");      // 文字列表示
  lcd.print(45);          // 数値表示
	lcd.print(' ');					// 文字表示
	lcd.print(5, 2);				// 2進数表示
}



void loop(void) {

}

