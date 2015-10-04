//
// I2C制御 ACM1602 共通関数テスト用I2Cホスト
//    wire使用バージョン
//

#include <Wire.h>				// I2C

#include "wAcm1602Func.h"		// LCD共通関数

wLcdFunc lcd;						// LCD共通関数

char StrBuf[17];


void setup(void) {
	byte adrs, i;

	Wire.begin();				// I2C初期化

	lcd.init();  				// LCD初期化

	lcd.clear();				// 全クリア
	lcd.noBlink();			// カーソル点滅なし
	lcd.noCursor();			// カーソル表示なし

	delay(1000);

	byte num = 0;
	for(i = 0; i < 16; i++) {
		lcd.writeData(0x30 + num);	// ダイレクト LCDデータライト
		num = (num + 1) % 10;
	}

	lcd.setCursor(5, 1);					// カーソル位置
	for(i = 0; i < 10; i++) {
		lcd.writeData(0x30 + num);
		num = (num + 1) % 10;
	}

	delay(1000);

	// 表示OFF/ON
	lcd.noDisplay();
	delay(1000);
	lcd.display();

	delay(1000);

	// 文字列表示
	lcd.clear();						// 全クリア
	lcd.string("test1");		// 文字列表示

	lcd.setCursor(5, 1);		// カーソル位置指定
	lcd.string("test2");		// 文字列表示

	delay(1000);

	// 数値文字列表示
	byte val = 0x12;
	sprintf(StrBuf, " 0x%02X", val);
	lcd.string(StrBuf);
}



void loop(void) {

}

