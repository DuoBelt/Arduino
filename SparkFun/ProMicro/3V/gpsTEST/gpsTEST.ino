unsigned int counter = 0;

void setup() {
  while (true) {
    if (Serial) break;
  }
  Serial.begin(115200);
}

void loop() {
  static char *nmea[] = {
    "$GPGGA,024959.084,,,,,0,0,,,M,,M,,*47",
    "$GPGLL,,,,,024959.084,V,N*75",
    "$GPGSA,A,1,,,,,,,,,,,,,,,*1E",
    "$GPGSV,1,1,01,06,,,18*77",
    "$GPRMC,024959.084,V,,,,,0.00,0.00,260916,,,N*48",
    "$GPVTG,0.00,T,,M,0.00,N,0.00,K,N*32"
  };

  char line[0x100];
  strcpy(line, nmea[counter % (sizeof(nmea) / sizeof(char *))]);

  Serial.println(line);

  char *p;
  char c;
  char csum = 0;
  int es = 0;
  char *ppp;
  bool loop = true;
  char *ep[0x100];

  for (p = line; c = *p; ) {
    switch (c) {
      case ',':
        csum ^= c;
        ep[es++] = ppp;
        *p++ = 0;
        ppp = p;
        break;
      case '$':
        p++;
        ppp = p;
        break;
      case '*':
        *p++ = 0;
        ep[es++] = ppp;
        loop = false;
        break;
      default:
        csum ^= c;
        p++;
        //        Serial.print(c);
        break;
    }
    if (loop == false) {
      char ooo;
      sscanf(p,"%x",&ooo);
      if(ooo != csum){
//        Serial.println("Checksum NG : " + String(ooo,HEX) + ":" + String(csum,HEX));
      }
      break;
    }
  }

//    Serial.print(" csum = ");
//    Serial.print(csum, HEX);
//    Serial.print(" es = ");
//    Serial.print(es);
//    Serial.println();

  ///*
  for (int a = 0; a < es; a++) {
    Serial.print("[" + String(ep[a]) + "]");
  }
  Serial.println();
  //  */
  delay(1000);
  counter++;
}

