//Badge Mode
void badge() {
  Serial.println(F("Badge Mode Started"));
  drawBadge();
  while (!menuOn) { //The only way we exit Badge Mode is via the Menu.
    int i = getButton();
    int ob = bdg; //Old badge. Check it at end to see if we've changed.
    if (i != 0){
      Serial.print(F("Button Pressed: "));
      Serial.println(i);
    }
    if (i == 1) {
      //Button 1: Previous badge.
      bdg -= 1;
      if (bdg < 1){
        //Go to "Last" badge, #3
        bdg = 3;
      }
    }
    else if (i == 2) {
      //Button 2: Go to the Menu.
      menuOn = true;
    }
    else if (i == 3) {
      //Button 3: Next Badge
      bdg += 1;
      if (bdg > 3){
        //We only have 3 color badges
        bdg = 1;
      }
    }
    if (bdg != ob) {
      //We've changed badges. Draw the new one.
      drawBadge();
    }
    delay(125);
  }
}

void drawBadge() {
  //Draw the appropriate badge
  Serial.print(F("Drawing Badge "));
  Serial.println(bdg);
  int track = millis();
  tft.setTextSize(5);
  tft.setTextColor(ILI9340_BLACK);
  if (bdg == 3) {
    //Red badge
    tft.fillScreen(ILI9340_RED);
    tft.fillRect(97, 36, 126, 126, ILI9340_BLACK);
    tft.fillRect(102, 41, 116, 116, ILI9340_WHITE);
    tft.setCursor(115, 180);
    tft.print(F("RED"));
  }
  else if (bdg == 2) {
    //Green Badge
    tft.fillScreen(ILI9340_GREEN);
    tft.fillCircle(160, 90, 65, ILI9340_BLACK);
    tft.fillCircle(160, 90, 60, ILI9340_WHITE);
    tft.setCursor(95, 180);
    tft.print(F("GREEN"));
  }
  else if (bdg == 1) {
    //Yellow Badge
    tft.fillScreen(ILI9340_YELLOW);
    tft.fillRect(45, 50, 230, 50, ILI9340_BLACK);
    tft.fillRect(50, 55, 220, 40, ILI9340_WHITE);
    tft.setCursor(70, 180);
    tft.print(F("YELLOW"));
  }
  else {
    //Something went wrong. Draw an error screen.
    tft.fillRect(0, 0, 100, 240, ILI9340_GREEN);
    tft.fillRect(100, 0, 120, 240, ILI9340_YELLOW);
    tft.fillRect(220, 0, 100, 240, ILI9340_RED);
  }
  track = millis() - track;
  Serial.print(track);
  Serial.println(F(" ms to display"));
}

