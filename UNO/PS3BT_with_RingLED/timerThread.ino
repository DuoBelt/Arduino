void setClut() {
  clut[0] = strip.Color(0, 0, 0); // Red
  clut[1] = strip.Color(255, 0, 0); // Red
  clut[2] = strip.Color(0, 255, 0); // Green
  clut[3] = strip.Color(0, 0, 255); // Blue
  clut[4] = strip.Color(255, 255, 0); // R+G
  clut[5] = strip.Color(255, 0, 255); // R+B
  clut[6] = strip.Color(0, 255, 255); // G+B
  clut[7] = strip.Color(255, 255, 255); // White
}

void cyclePoint() {
  if (useRING) {
    unsigned int index = counter;
    unsigned int a;
    for(a=0; a<12; a++){
      strip.setPixelColor(a, a == index? clut[thisClut]:clut[0]);
    }
    if(cwRING){
      counter++;
      if(counter == 12){
        counter = 0;
      }
    }
    else{
      if(counter){
        counter--;
      }
      else counter = 11;
    }
  }
  else {
  }
  strip.show();
}


