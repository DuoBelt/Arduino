/*---------------------- 2012-01-06  written by iizuka -----*/
/* MAX7219 TEST                                             */
/*----------------------------------------------------------*/
#define LOADpin 10
#define DINpin  11
#define CLKpin  13

void setup()   {
  pinMode(LOADpin,OUTPUT);
  pinMode(DINpin, OUTPUT);
  pinMode(CLKpin, OUTPUT);
  digitalWrite(LOADpin,HIGH);
  digitalWrite(DINpin, LOW );
  digitalWrite(CLKpin, LOW );
  delay(1);

  max7219write( 0x0F, 0x00 ); // display test , normal operation
  max7219write( 0x09, 0xFF ); // decode mode , decode digit 0 to 7
  max7219write( 0x0A, 0x08 ); // intensity , 0x01=dark .... 0x0F=bright  
  max7219write( 0x0B, 0x07 ); // scan limit , display 0 to 7  
  max7219write( 0x0C, 0x01 ); // shutdown mode register , normal operation
}

void loop()                     
{
  max7219write( 0x09, 0xFF );   // DIGIT 0-7 : Code
  // display  "12345678"
  max7219write( 1, 1 );
  max7219write( 2, 2 );
  max7219write( 3, 3 );
  max7219write( 4, 4 );
  max7219write( 5, 5 );
  max7219write( 6, 6 );
  max7219write( 7, 7 );
  max7219write( 8, 8 );
  delay(2000);
  
  // display "9-EHLP  "
  max7219write( 1, 9  );
  max7219write( 2, 10 );
  max7219write( 3, 11 );
  max7219write( 4, 12 );
  max7219write( 5, 13 );
  max7219write( 6, 14 );
  max7219write( 7, 15 );
  max7219write( 8, 15 );
  delay(2000);

  // display "1234Abcd"
  max7219write( 0x09, 0x0F );   // DIGIT 0-3 : Code   DIGIT 4-7 : No decode
  max7219write( 1, 1 );
  max7219write( 2, 2 );
  max7219write( 3, 3 );
  max7219write( 4, 4 );
  max7219write( 5, B01110111 );
  max7219write( 6, B00011111 );
  max7219write( 7, B00001101 );
  max7219write( 8, B00111101 );
  delay(2000);
}


void max7219write( byte high_byte, byte low_byte )
{
  digitalWrite( LOADpin, LOW );
  shiftOut( DINpin, CLKpin, MSBFIRST, high_byte );
  shiftOut( DINpin, CLKpin, MSBFIRST, low_byte  );
  digitalWrite( LOADpin, HIGH );
  delayMicroseconds(1); 
}  




