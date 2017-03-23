//Pride Flag Mode
void flag() {
  Serial.println(F("Flag Mode Started"));
  drawFlag();
  while (!menuOn) { 
    //As long as we're not in the menu, display the flags.
    int i = getButton();
    int of = flg; //Old badge. Check it at end to see if we've changed.
    if (i != 0){
      Serial.print(F("Button Pressed: "));
      Serial.println(i);
    }
    if (i == 1) {
      //Button 1: Previous flag.
      flg -= 1;
      if (flg < 1){
        //Go to "Last" flag, #5
        flg = 5;
      }
    }
    else if (i == 2) {
      //Button 2: Go to the Menu.
      menuOn = true;
    }
    else if (i == 3) {
      //Button 3: Next Flag
      flg += 1;
      if (flg > 5){
        //We only have 5 different flags
        flg = 1;
      }
    }
    if (flg != of) {
      //We've changed flags. Draw the new one.
      drawFlag();
    }
    delay(125);
  }
}

void drawFlag() {
  Serial.print(F("Drawing Flag "));
  Serial.println(flg);
  int track = millis();
  if (flg == 1) {
    //LGBT Pride Flag
    tft.fillRect(0, 0, 320, 41, 0xD801); //Red
    tft.fillRect(0, 41, 320, 41, 0xFBC1); //Orange
    tft.fillRect(0, 81, 320, 41, 0xFF61); //Yellow
    tft.fillRect(0, 121, 320, 41, 0x0B83); //Green
    tft.fillRect(0, 161, 320, 41, 0x015F); //Royal Blue
    tft.fillRect(0, 201, 320, 40, 0x600E); //Violet
  }
  else if (flg == 2) {
    //Bi Flag
    tft.fillRect(0, 0, 320, 81, 0xD00F); //???
    tft.fillRect(0, 81, 320, 81, 0x7272); //???
    tft.fillRect(0, 161, 320, 80, 0x01D5); //???
  }
  else if (flg == 3) {
    //Trans Flag
    tft.fillRect(0, 0, 320, 49, 0x5E7F); //???
    tft.fillRect(0, 49, 320, 49, 0xF557); //???
    tft.fillRect(0, 97, 320, 49, ILI9340_WHITE); //White. Obviously.
    tft.fillRect(0, 145, 320, 49, 0xF557); //Another layer of ???
    tft.fillRect(0, 193, 320, 48, 0x537F); //Another layer of ???
  }
  else if (flg == 4) {
    //Pansexual Flag
    tft.fillRect(0, 0, 320, 81, 0x259F); //???
    tft.fillRect(0, 81, 320, 81, 0xFEC0); //???
    tft.fillRect(0, 161, 320, 81, 0xF911); //???
  }
  else if (flg == 5) {
    //Asexual Flag
    tft.fillRect(0, 0, 320, 61, ILI9340_BLACK); //Black.
    tft.fillRect(0, 61, 320, 61, 0xA514); //??
    tft.fillRect(0, 121, 320, 61, ILI9340_WHITE); //White
    tft.fillRect(0, 181, 320, 61, 0x8010); //??
  }
  else {
    //Something went wrong. Draw an error screen.
    //Badges have vertical lines. Let's make Flags have horizontal
    tft.fillRect(0, 0, 320, 81, ILI9340_GREEN);
    tft.fillRect(0, 81, 320, 81, ILI9340_YELLOW);
    tft.fillRect(0, 161, 320, 81, ILI9340_RED);
  }
  track = millis() - track;
  Serial.print(track);
  Serial.println(F(" ms to display"));
}

