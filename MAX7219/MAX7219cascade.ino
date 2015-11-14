/*---------------------- 2012-01-06  written by iizuka -----*/
/* MAX7219 TEST     CASCADE                                 */
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

  max7219write( 0x0F, 0x00, 0x0F, 0x00 ); // display test , normal operation
  max7219write( 0x09, 0xFF, 0x09, 0xFF ); // decode mode , decode digit 0 to 7
  max7219write( 0x0A, 0x06, 0x0A, 0x06 ); // intensity , 0x01=dark .... 0x0F=bright  
  max7219write( 0x0B, 0x07, 0x0B, 0x02 ); // scan limit , main -> display 0 to 7   sub -> 0 to 2  
  max7219write( 0x0C, 0x01, 0x0C, 0x01 ); // shutdown mode register , normal operation
}

void loop()                     
{
  // display  "01234567" "89-"
  max7219write( 1, 0, 1, 8  );
  max7219write( 2, 1, 2, 9  );
  max7219write( 3, 2, 3, 10 );
  max7219write( 4, 3, 0, 0  );
  max7219write( 5, 4, 0, 0  );
  max7219write( 6, 5, 0, 0  );
  max7219write( 7, 6, 0, 0  );
  max7219write( 8, 7, 0, 0  );
  delay(2000);

  // display  "-9876543" "210"
  max7219write( 1, 10, 1, 2 );
  max7219write( 2, 9,  2, 1 );
  max7219write( 3, 8,  3, 0 );
  max7219write( 4, 7,  0, 0 );
  max7219write( 5, 6,  0, 0 );
  max7219write( 6, 5,  0, 0 );
  max7219write( 7, 4,  0, 0 );
  max7219write( 8, 3,  0, 0 );
  delay(2000);
}


void max7219write( byte addrmain, byte datamain, byte addrsub, byte datasub )
{
  digitalWrite( LOADpin, LOW );
  shiftOut( DINpin, CLKpin, MSBFIRST, addrsub );
  shiftOut( DINpin, CLKpin, MSBFIRST, datasub );
  shiftOut( DINpin, CLKpin, MSBFIRST, addrmain );
  shiftOut( DINpin, CLKpin, MSBFIRST, datamain );
  digitalWrite( LOADpin, HIGH );
  delayMicroseconds(1); 
}  




