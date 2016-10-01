#include <Arduino.h>

int main(void)
{
    init();

#if defined(USBCON)
    USBDevice.attach();
#endif

  Serial.begin(115200);

  Serial.println("main");

  while(true){
    ;
  }
    return 0;
}

