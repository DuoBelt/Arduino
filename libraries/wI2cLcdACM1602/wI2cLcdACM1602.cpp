//
//	I2C制御LCDドライバ (ACM1602専用)
//		12/04/26 T.Nakao
//
//		Wireと併用
//

#include "wI2cLcdACM1602.h"
#include "Wire.h"
#include "Arduino.h"


// コンストラクタ
wI2cLcdACM1602::wI2cLcdACM1602() {

}


void wI2cLcdACM1602::init(void) {
	begin();
}


void wI2cLcdACM1602::begin(void) {
  // LCD初期化
	delayMicroseconds(15000);
	writeCmd(0x01);
	delayMicroseconds(5000);
	writeCmd(0x38);
	delayMicroseconds(5000);
	writeCmd(0x0f);
	delayMicroseconds(5000);
	writeCmd(0x06);
	delayMicroseconds(5000);

	DisplayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
}


void wI2cLcdACM1602::clear(void)
{
	writeCmd(LCD_CLEARDISPLAY);
	delayMicroseconds(2000);
}

void wI2cLcdACM1602::home(void)
{
	writeCmd(LCD_RETURNHOME);
	delayMicroseconds(2000);
}


void wI2cLcdACM1602::setCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = { 0x00, 0x40 };
	if ( row >= NUM_LINES ) {
		row = NUM_LINES - 1;
	}

	writeCmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}


void wI2cLcdACM1602::noDisplay() {
	DisplayControl &= ~LCD_DISPLAYON;
	writeCmd(LCD_DISPLAYCONTROL | DisplayControl);
}
void wI2cLcdACM1602::display() {
	DisplayControl |= LCD_DISPLAYON;
	writeCmd(LCD_DISPLAYCONTROL | DisplayControl);
}


void wI2cLcdACM1602::noCursor() {
	DisplayControl &= ~LCD_CURSORON;
	writeCmd(LCD_DISPLAYCONTROL | DisplayControl);
}
void wI2cLcdACM1602::cursor() {
	DisplayControl |= LCD_CURSORON;
	writeCmd(LCD_DISPLAYCONTROL | DisplayControl);
}


void wI2cLcdACM1602::noBlink() {
	DisplayControl &= ~LCD_BLINKON;
	writeCmd(LCD_DISPLAYCONTROL | DisplayControl);
}
void wI2cLcdACM1602::blink() {
	DisplayControl |= LCD_BLINKON;
	writeCmd(LCD_DISPLAYCONTROL | DisplayControl);
}


void wI2cLcdACM1602::scrollDisplayLeft(void) {
	writeCmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void wI2cLcdACM1602::scrollDisplayRight(void) {
	writeCmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}



void wI2cLcdACM1602::leftToRight(void) {
	DisplayMode |= LCD_ENTRYLEFT;
	writeCmd(LCD_ENTRYMODESET | DisplayMode);
}



void wI2cLcdACM1602::rightToLeft(void) {
	DisplayMode &= ~LCD_ENTRYLEFT;
	writeCmd(LCD_ENTRYMODESET | DisplayMode);
}


void wI2cLcdACM1602::autoscroll(void) {
	DisplayMode |= LCD_ENTRYSHIFTINCREMENT;
	writeCmd(LCD_ENTRYMODESET | DisplayMode);
}


void wI2cLcdACM1602::noAutoscroll(void) {
	DisplayMode &= ~LCD_ENTRYSHIFTINCREMENT;
	writeCmd(LCD_ENTRYMODESET | DisplayMode);
}



void wI2cLcdACM1602::createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x07; // we only have 8 locations 0-7

	uint8_t i;
	writeCmd(LCD_SETCGRAMADDR | (location << 3));
	for (i = 0; i < 8; i++) {
		writeData(charmap[i]);
	}
}


void wI2cLcdACM1602::writeCmd(uint8_t cmd) {
	uint8_t rs_flg;

	Wire.beginTransmission(0x50);
	rs_flg = 0x00;
	Wire.write(rs_flg);
	Wire.write(cmd);
	Wire.endTransmission();
}


void wI2cLcdACM1602::writeData(uint8_t dat) {
	Wire.beginTransmission(0x50);
	Wire.write(0x80);
	Wire.write(dat);
	Wire.endTransmission();
}


inline size_t wI2cLcdACM1602::write(uint8_t value) {
	writeData(value);
	return 1;
}

