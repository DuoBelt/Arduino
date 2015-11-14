/*---------------------- 2012-01-06  written by iizuka -----*/
/* MAX7219 TEST                                             */
/*----------------------------------------------------------*/
#include <SPI.h>

#define LOADpin 10
// DIN = Digital 11 pin
// CLK = Digital 13 pin

static int counter = 0;

void setup()   {
  pinMode(LOADpin,OUTPUT);
  digitalWrite(LOADpin,HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST); 
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);

  delay(500);  
  max7219write( 0x0F, 0x00 ); // display test , normal operation
  max7219write( 0x0A, 0x08 ); // intensity , 0x01=dark .... 0x0F=bright  
  max7219write( 0x0B, 0x07 ); // scan limit , display 0 to 7  
  max7219write( 0x0C, 0x01 ); // shutdown mode register , normal operation

  max7219write( 0x09, 0x00 ); // non decode mode
}

void loop()                     
{
  String volt = String(3.78);
  String amp = String(23.4);

  
  max7219write( 8, B01110111);
  max7219write( 7, 2 );
  max7219write( 6, 3 );
  max7219write( 5, 4 );
  max7219write( 4, 5 );
  max7219write( 3, 6 );
  max7219write( 2, 7 );
  max7219write( 1, 8 );

  delay(1000/2);
  counter++;
}


void max7219write( byte high_byte, byte low_byte )
{
  digitalWrite( LOADpin, LOW );
  SPI.transfer( high_byte );
  SPI.transfer( low_byte  );
  digitalWrite( LOADpin, HIGH );
}  



