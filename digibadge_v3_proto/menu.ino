//Menu "Mode"
void menu() {
  //Enters main menu
  Serial.println(F("Starting Main Menu"));
  tft.fillScreen(ILI9340_BLACK); //Empty the screen.
  int m = 1; //Number of Menu items
  int c = 0; //Current Menu. If this is set to -1, we exit the menu system.
  int sel = 1; //Selection in current menu. Valid entries are 1 to m
  m = newMenu(0); //Bring up the Main Menu.
  drawCursor(1, sel);
  while (c >= 0) {
    int i = getButton();
    int os = sel; //os = Old Selection. We'll compare this at the end.
    int oc = c; //oc = Old Menu.
    if (i != 0){
      Serial.print(F("Button Pressed: "));
      Serial.println(i);
    }
    if (i == 1){
      //Button 1: Go up one item.
      sel -= 1;
      if (sel < 1){
        //Wrap around to the bottom.
        sel = m;
      }
    }
    else if (i == 3){
      //Button 2: Go down one item.
      sel += 1;
      if (sel > m) {
        //Wrap around to the top.
        sel = 1;
      }
    }
    else if (i == 2){
      //Ooooooh, we've selected something!
      c = select(c,sel);//Pawn it off on another function.
    }
    if ((sel != os) or (c != oc)) { //Selection or Menu has changed.
      if (c != oc) {
        //New Menu. Set selection to 1.
        sel = 1;
        tft.fillScreen(ILI9340_BLACK);// Empty the screen
        m = newMenu(c); //Draw the menu
      }
      if (c >= 0){
        drawCursor(os, sel); //Draw the updated cursor.
      }
    }
  delay(125);
  }
}

void drawCursor(int os, int s) {
  //Draws the cursor on a given line.
  tft.setTextSize(txtsz);
  //First, erase the old cursor.
  tft.setCursor(0,(8*os*txtsz));
  tft.setTextColor(ILI9340_BLACK);
  tft.print(F(">"));
  //Now, draw the new one.
  tft.setCursor(0,(8*s*txtsz));
  tft.setTextColor(ILI9340_YELLOW);
  tft.print(F(">"));  
}

int select(int c, int s){
  //Does actions depending on what's selected.
  if (c == 0){
    //Main Menu actions.
    if (s == 1){
      //Set Badge Mode
      Serial.println(F("Entering Badge Mode"));
      mde = 0;
      menuOn = false;
      return -1;
    }
    else if (s == 2){
      //set Slideshow mode
      Serial.println(F("Entering Slideshow Mode"));
      //TODO: Check SD card before attempting? Or handle that in Image mode?
      menuOn = false;
      mde = 1;
      return -1;
    }
    else if (s == 3){
      //set Image mode
      Serial.println(F("Entering Image Mode"));
      //TODO: Check SD card before attempting? Or handle that in Image mode?
      menuOn = false;
      mde = 2;
      return -1;
    }
    else if (s == 4){
      //set Flag mode
      Serial.println(F("Entering Flag Mode"));
      menuOn = false;
      mde = 3;
      return -1;
    }
    else if (s == 5){
      //Brightness Settings
      Serial.println(F("Entering Brightness Menu"));
      return 1;
    }
    else if (s == 6){
      //Slideshow Timer
      Serial.println(F("Entering Slideshow Timer Menu"));
      return 2;
    }
    else if (s == 7){
      //Device Information
      Serial.println(F("Entering Device Information"));
      return 3;
    }
    else if (s == 8){
      //Shut down the system
      Serial.println(F("Shutting down the system."));
      //shdn();
      return -1;
    }
    else{
      //Exit the menu with no changes
      Serial.println(F("Exiting Main Menu"));
      menuOn = false;
      return -1;
    }
  }
  else if (c == 1){
    //Brightness Menu
    if (s == 1){
      //Brightness Up
      Serial.println(F("Increase Brightness"));
      bright += 15;
      if (bright > 1023){
        bright = 1023;
      }
      setBright();
      return 1;//Return to the same menu
    }
    else if (s == 2){
      //Brightness down
      Serial.println(F("Decrease Brightness"));
      bright -= 15;
      if (bright < 128){
        bright = 128;
      }
      setBright();
      return 1; //Return to the same menu
    }
    else if (s == 3){
      //Max Brightness
      Serial.println(F("Maximum Brightness"));
      bright = 1023;
      setBright();
      return 1; //Return to same menu
    }
    else if (s == 4) {
      //Min Brightness
      Serial.println(F("Minimum Brightness"));
      bright = 128;
      setBright();
      return 1;
    }
    else if (s == 5){
      //Default brightness
      Serial.println(F("Default Brightness"));
      bright = 700;
      setBright();
      return 1; //Return to same menu
    }
    else if (s == 6){
      Serial.println(F("Returning to Main Menu"));
      return 0; //Return to Main Menu
    }
  }
  else if (c == 2){
    //Slideshow timer menu
    if (s == 1){
      //2.5 Seconds
      Serial.println(F("Slideshow: 2.5 Seconds"));
      slide = 2500;
      return 0;
    }
    else if (s == 2){
      //5 Seconds (Default)
      Serial.println(F("Slideshow: 5 Seconds"));
      slide = 5000;
      return 0;
    }
    else if (s == 3){
      //10 Seconds
      Serial.println(F("Slideshow: 10 Seconds"));
      slide = 10000;
      return 0;
    }
    else if (s == 4){
      //15 Seconds
      Serial.println(F("Slideshow: 15 Seconds"));
      slide = 15000;
      return 0;
    }
    else if (s == 5){
      //30 Seconds
      Serial.println(F("Slideshow: 30 Seconds"));
      slide = 30000;
      return 0;
    }
    else if (s == 6){
      //45 Seconds
      Serial.println(F("Slideshow: 45 Seconds"));
      slide = 45000;
      return 0;
    }
    else if (s == 7){
      //1 Minute/60 Seconds
      Serial.println(F("Slideshow: 1 Minute"));
      slide = 60000;
      return 0;
    }
    else if (s == 8){
      //1.5 Minutes/90 Seconds
      Serial.println(F("Slideshow: 1.5 Minutes"));
      slide = 90000;
      return 0;
    }
    else if (s == 9){
      //2 minutes/120 Seconds
      Serial.println(F("Slideshow: 2 Minutes"));
      slide = 120000;
      return 0;
    }
    else if (s == 10){
      //2.5 Minutes/150 Seconds
      Serial.println(F("Slideshow: 2.5 Minutes"));
      slide = 150000;
      return 0;
    }
    else if (s == 11){
      //3 Minutes/180 Seconds
      Serial.println(F("Slideshow: 3 Minutes"));
      slide = 180000;
      return 0;
    }
    else if (s == 12){
      //4 Minutes/240 Seconds
      Serial.println(F("Slideshow: 4 Minutes"));
      slide = 240000;
      return 0;
    }
    else if (s == 13){
      //5 Minutes/300 Seconds
      Serial.println(F("Slideshow: 5 Minutes"));
      slide = 300000;
      return 0;
    }
    else {
      //Return to main menu
      Serial.println(F("Returning to Main Menu"));
      return 0;
    }
  }
  else if (c == 3){
    Serial.println(F("Returning to Main Menu"));
    return 0;
  }
}

int newMenu(int c) {
  //Draws a given menu
  if (c == 0){
    //Main Menu
    drawMenu("DigiBadge V3 Main Menu", "  Badge Mode", "  Image Mode", "  Slideshow Mode", "  Flag Mode", "  Brightness", "  Slide Timer", "  Device Info", "  Power Off", "  Exit Menu");
    return 9;
  }
  else if (c == 1){
    //Brightness menu
    drawMenu("Brightness Settings", "  Increase Brightness", "  Decrease Brightness", "  Max Brightness", "  Min Brightness", "  Reset to Default", "  Exit to Main Menu");
    return 6;
  }
  else if (c == 2){
    //Slideshow Speed Menu
    drawMenu("Slideshow Speed Settings", "  2.5 Seconds", "  5 Seconds (Default)", "  10 Seconds", "  15 Seconds", "  30 Seconds", "  45 Seconds", "  1 Minute", "  1.5 Minutes", "  2 Minutes", "  2.5 Minutes", "  3 Minutes", "  4 Minutes", "  5 Minutes", "  Exit to Main Menu");
    return 14;
  }
  else if (c == 3){
    //Device Information
    drawMenu("Device Information", "  Return to Main Menu");
    drawSDInfo();
    return 1;
  }
}

void drawSDInfo() {
  //Display various info about the machine.
  tft.setCursor(0, 33);
  tft.setTextColor(ILI9340_WHITE);
  tft.println(F("DigiBadge Version 3"));
  tft.println(F("http://www.matchfire.net"));
  tft.print(F("Battery Voltage: "));
  if (getVoltage() < 2.0) {
    tft.setTextColor(ILI9340_RED);
  }
  tft.print(getVoltage());
  tft.setTextColor(ILI9340_WHITE);
  tft.println(F("v"));
  if (checkSD()) {
    tft.println(F("SD Card Detected"));
  }
  else {
    tft.println(F("No SD Card Detected"));
  }
  if (sd_load) {
    tft.println(F("SD Card Mounted"));
    tft.print(imgnum);
    tft.println(F(" images found"));
  }
  else {
    if (!checkSD()) {
      tft.println(F("No SD Card to mount"));
      tft.println("");
    }
    else {
      tft.println(F("SD Card Mounting Failed"));
      tft.println("");
    }
  }
  tft.println("");
  tft.println(F("Code v0.1, 3/19/2017"));
  tft.println(F("For updates, see:"));
  tft.println(F("matchfire.net/dbv3"));
}

void drawMenu(char title[], char i1[], char i2[], char i3[],
              char i4[], char i5[], char i6[], char i7[],
              char i8[], char i9[], char i10[], char i11[],
              char i12[], char i13[], char i14[], char i15[]){
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9340_RED);
  tft.println(title);
  tft.setTextColor(ILI9340_GREEN);
  tft.println(i1);
  tft.println(i2);
  tft.println(i3);
  tft.println(i4);
  tft.println(i5);
  tft.println(i6);
  tft.println(i7);
  tft.println(i8);
  tft.println(i9);
  tft.println(i10);
  tft.println(i11);
  tft.println(i12);
  tft.println(i13);
  tft.println(i14);
  tft.println(i15);
}

