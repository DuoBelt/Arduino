#define DELAYMS (100)

unsigned short readD() {
  unsigned char Hbyte;
  unsigned char Lbyte;
  unsigned short dist;

  swsDS.write(0x55);
  delay(DELAYMS);

  Hbyte = swsDS.read();
  Lbyte = swsDS.read();

  dist = ((unsigned short)Hbyte * 0x100) + (unsigned short)Lbyte;

  return (dist);
}

unsigned char readT() {
  unsigned char temp;

  swsDS.write(0x50);
  delay(DELAYMS);
  temp = (unsigned char)swsDS.read();
  temp -= 45;

  return (temp);
}

