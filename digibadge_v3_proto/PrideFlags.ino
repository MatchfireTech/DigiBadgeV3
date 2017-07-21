/*
 * Pride Flags!
 * We can easily create 5 of them: LGBT, Bisexual, Transgender, Pansexual, and Asexual.
 * These flags are all fairly simple to make, as they consist of horizontal bars.
 * Thus, the drawFlag function can draw a number of flags that consist of horizontal colors.
 */

void flagLoop () {
  Serial.println(F("Pride Flag Mode Started"));
  updateFlag(); //We JUST started Badge mode. We need to draw the badge.
  delay(250); //Short delay to prevent rapid flitting between menu and mode
  while (!menuOn) { //Only quit when we go to menu mode.
    i = pwr.getInfo();
    byte of = flag;
    if (!bitRead(i, 1) and (bitRead(i, 3) and bitRead(i, 2))){
      if (flag > 0) {
        flag--;
      }
      else {
        //Loop around.
        flag = 4;
      }
    }
    else if (!bitRead(i, 3) and (bitRead(i, 1) and bitRead(i, 2))){
      flag++;
      if (flag > 4) {
        //Loop around.
        flag = 0;
      }
    }
    else if (!bitRead(i, 2) and (bitRead(i, 1) and bitRead(i, 3))){
      menuOn = true; //Exit to menu.
    }
    delay(15);
    i = clearButton();
    if (of != flag) {
      updateFlag();
    }
    delay(upspd); //Wait a little before updating. Updating too fast will cause us to speed through things at alarming rates.
  }
}

void updateFlag() {
  if (flag == 0) {
    drawLGBT();
  }
  else if (flag == 1) {
    drawBi();
  }
  else if (flag == 2) {
    drawTrans();
  }
  else if (flag == 3) {
    drawPan();
  }
  else if (flag == 4) {
    drawAsex();
  }
  else {
    //Shouldn't happen!
    flag = 0;
    drawLGBT();
  }
}

void drawLGBT() {
  //Draws the LGBT flag.
  //6 bars of equal size: Red, Orange, Yellow, Green, Royal Blue, and Violet.
  Serial.print(F("Displaying LGBT flag. Time: "));
  int t = millis();
  drawFlag(17, 0xD801, 17, 0xFBC1, 17, 0xFF61, 17, 0x0B83, 17, 0x015F, 17, 0x600E);
  t = millis() - t;
  Serial.print(t); Serial.println(F(" ms"));
}

void drawBi(){
  //Draws the Bisexual flag
  //Three bands, middle being narrower. Pinkishpurple, purple, and a dark blue.
  //Forgive me, I'm terrible with colors.
  Serial.print(F("Displaying Bisexual flag. Time: "));
  int t = millis();
  drawFlag(40, 0xD00F, 20, 0x7272, 40, 0x01D5);
  t = millis() - t;
  Serial.print(t); Serial.println(F(" ms"));
}

void drawTrans(){
  //Draws the Transgender flag.
  //Five equal bars of Light Blue, Pink, White, Pink, Light Blue.
  Serial.print(F("Displaying Transgender flag. Time: "));
  int t = millis();
  drawFlag(20, 0x5E7F, 20, 0xF557, 20, TFT_WHITE, 20, 0xF557, 20, 0x5E7F);
  t = millis() - t;
  Serial.print(t); Serial.println(F(" ms"));
}

void drawPan(){
  //Draws the Pansexual flag
  //Pink, Yellow, and Blue in even bands.
  Serial.print(F("Displaying Pansexual flag. Time: "));
  int t = millis();
  drawFlag(33,  0xF911, 34, 0xFEC0, 33, 0x259F);  
  t = millis() - t;
  Serial.print(t); Serial.println(F(" ms"));
}

void drawAsex(){
  //Draws the Asexual flag.
  //Four equal bands: Black, Grey, White, Purple.
  Serial.print(F("Displaying Asexual flag. Time: "));
  int t = millis();
  drawFlag(25, TFT_BLACK, 25, 0xA514, 25, TFT_WHITE, 25, 0x8010);
  t = millis() - t;
  Serial.print(t); Serial.println(F(" ms"));
}

void drawFlag(int h1, uint32_t c1, int h2, uint32_t c2, int h3, uint32_t c3, int h4, uint32_t c4, int h5, uint32_t c5, int h6, uint32_t c6){
  //Flag Drawing Function.
  //Draws 1-6 bars in designated heights and colors.
  //Height is given in percentage of screen height.
  int w = tft.width();
  //Convert numbers to screen size.
  h1 = int((h1/100.00) * tft.height());
  h2 = int((h2/100.00) * tft.height()) + h1;
  h3 = int((h3/100.00) * tft.height()) + h2;
  h4 = int((h4/100.00) * tft.height()) + h3;
  h5 = int((h5/100.00) * tft.height()) + h4;
  h6 = int((h6/100.00) * tft.height()) + h5;
  //Draw the rectangles. Sometimes they don't line up perfectly,
  //So make the height a little bigger so we don't get gaps.
  tft.fillRect(0, 0, w, h1+1, c1);
  tft.fillRect(0, h1+1, w, h2+1, c2);
  tft.fillRect(0, h2+1, w, h3+1, c3);
  tft.fillRect(0, h3+1, w, h4+1, c4);
  tft.fillRect(0, h4+1, w, h5+1, c5);
  tft.fillRect(0, h5+1, w, h6, c6);
}
