void checkNMEA(char *line){
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

  for (int a = 0; a < es; a++) {
    Serial.print("[" + String(ep[a]) + "]");
  }
  Serial.println();
  
}

int GPSreadln(char *buffer) {
  int length = 0;
  bool loop = true;
  while (loop) {
    int bytes = Serial1.available();
    while (bytes--) {
      char c = Serial1.read();
      switch (c) {
        case 0x0d:
          break;
        case 0x0a:
          loop = false;
          break;
        default:
          *buffer++ = c;
          length++;
          break;
      }
    }
  }
  *buffer = 0;

  return(length);
}

