void checkNMEA(char *line) {
  char *p;
  char c;
  int es = 0;
  char *ppp = line;
  char *ep[0x100];

  for (p = line; c = *p;) {
    if (c == ',') {
      ep[es++] = ppp;
      *p++ = 0;
      ppp = p;
    }
    else {
      p++;
    }
  }
  ep[es++] = ppp;


  if (strcmp(ep[0], "GPRMC") == 0) {
    char *vp = ep[2];
    char *dp = ep[9];
    char *tp = ep[1];
    char *lonP = ep[3];
    char *latP = ep[5];
    char *mp = ep[12];

    float lon = atof(lonP);
    float lat = atof(latP);

    Serial1.println(String(vp) + " " + "20" + String(dp).substring(4, 6) + "-" + String(dp).substring(2, 4) + "-" + String(dp).substring(0, 2) + " " + String(tp).substring(0, 2) + ":" + String(tp).substring(2, 4) + ":" + String(tp).substring(4, 6));
    Serial1.println(String(mp) + " lon:lat = [" + String(lon, 3) + ":" + String(lat, 3) + "]");
  }
  //  Serial.println();
}

void GPSwriteln(char *p)
{
  char checkSum;
  char c;

  Serial2.print('$');

  for (checkSum = 0; c = *p++; ) {
    checkSum ^= c;
    Serial2.print(c);
  }

  Serial2.print('*');
  Serial2.println(checkSum, HEX);
}

int GPSreadln(char *buffer) {
  int length = 0;
  char checkSum = 0;
  bool loop = true;
  bool isChar = true;
  char sumb[3];
  char *cp = sumb;

//Serial1.println(String(__FILE__) + ":" + String(__LINE__));
  
  while (loop) {
    int bytes = Serial2.available();
//Serial1.println(String(__FILE__) + ":" + String(__LINE__));
    while (bytes--) {
      char c = Serial2.read();
      //      Serial.print(c);
      switch (c) {
        case '$':
          break;
        case '*':
          isChar = false;
          break;
        case 0x0d:
          break;
        case 0x0a:
          *cp = 0;
          *buffer = 0;
          loop = false;
          break;
        default:
          if (isChar) {
            checkSum ^= c;
            *buffer++ = c;
            length++;
          }
          else {
            *cp++ = c;
          }
          break;
      }
    }
  }

  char gotSum;
  sscanf(sumb, "%x", &gotSum);

  //  Serial.println(gotSum, HEX);
  //  Serial.println(checkSum, HEX);

  return (checkSum == gotSum ? length : 0);
}


