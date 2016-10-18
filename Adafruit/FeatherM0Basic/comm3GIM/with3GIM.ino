class USE3GIM {
public:
  USE3GIM(void);
  float lon; // longitude
  float lat; // lattitude
  int sats; // count of satellite
private:
  char buffer[0x400];
  bool getRSSI(void);
  int readLINE(void);
  int readSIZE(int size);
};

USE3GIM::USE3GIM(void){
  lon = 0.0;
  lat = 0.0;
}

bool USE3GIM::getRSSI(void){
  static char comm[]="$YR";
  static int len = sizeof(com) - 1;

  Serial1.println(command);
  for(bool loop=true; bool; ){
    char *p = readLINE();
    if(strncmp(p,com,len) == 0){
      
    }
  }
}

int USE3GIM::readSIZE(int size){
  int length = 0;
  char *p;
  for(p=buffer; size--; length++){
    *p++ = Serial1.read();
  }

  *p = 0;
  return(length);
}

int USE3GIM::readLINE(void){
  int length = 0;
  for(bool loop=true; loop; ){
    switch(char c = Serial1.read()){
      case 0x0d: // CR
        buffer[length] = 0;
        break;
      case 0x0a: // LF
        loop = false;
        break;
      default:
        buffer[length++] = c;
        break;
    }
  }
  return(length);
}

