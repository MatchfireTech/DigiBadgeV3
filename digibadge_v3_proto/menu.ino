
#define MENU_BGND TFT_BLACK
#define MENU_TXT TFT_GREEN
#define MENU_CUR TFT_YELLOW
#define MENU_TITLE TFT_RED
#define MENU_INFO TFT_WHITE
#define MENU_WARN TFT_RED

void menu() {
  Serial.println(F("Entering Main Menu"));
  byte items = 1; //Number of Menu items.
  int cur = 0; //Current menu. If this is -1, we exit.
  byte sel = 1; //Current selection. Valid entries are 1 to items
  tft.fillScreen(MENU_BGND);
  items = newMenu(0);
  drawCursor(1, sel);
  delay(500); //Short delay to prevent rapid flitting between menu and mode
    while (cur >= 0) {
    i = pwr.getInfo();
    byte os = sel; //Old selection. We'll compare this at the end.
    byte oc = cur; //OC = old menu
    if (!bitRead(i, 1) and (bitRead(i, 3) and bitRead(i, 2))) {
      //Button 1 pressed. Go "Up" if possible. If not, loop around.
      if (sel > 1) {
        sel --;
      }
      else {
        sel = items;
      }
    }
    else if (!bitRead(i, 3) and ((bitRead(i, 1) and bitRead(i, 2)))){
      //Button 3 pressed.
      if (sel < items) {
        sel ++;
      }
      else {
        sel = 1;
      }
    }
    else if (!bitRead(i, 2) and ((bitRead(i, 1) and bitRead(i, 3)))){
      //Button 2 pressed, something is selected.
      cur = select(cur,sel); //Pawn it off on another function.
    }
    delay(15);
    i = clearButton();
    if (sel != os) {
      //Selection has changed
      drawCursor(os, sel);
    }
    if (cur != oc) {
      sel = 1;
      tft.fillScreen(MENU_BGND); //Empty the screen.
      items = newMenu(cur);
      drawCursor(1, sel);
    }
    delay(menuspd);
  }
  saveSettings();
}

void drawCursor(int os, int s) {
  //Draws the cursor on a given line.
  tft.setTextSize(menutxt);
  //First, erase the old cursor.
  tft.setCursor(0,(8*os*menutxt));
  tft.setTextColor(MENU_BGND);
  tft.print(F(">"));
  //Now, draw the new one.
  tft.setCursor(0,(8*s*menutxt));
  tft.setTextColor(MENU_CUR);
  tft.print(F(">"));  
}

int newMenu(int c) {
  //Draws a given menu
  if (c == 0){
    //Main Menu
    drawMenu("DigiBadge V3 Main Menu", "  Badge Mode", "  Image Mode", "  Slideshow Mode", "  Flag Mode", "  Badge Settings",  "  Brightness", "  Slide Timer","  Device Info", "  Power Off", "  Exit Menu");
    return 10;
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
  else if (c == 4){
    //Badge Style selection
    drawMenu("Badge Style", "  Default", "  BronyCon", "  ASAN");
    return 3;
  }
}

void drawSDInfo() {
  //Display various info about the machine.
  i = pwr.getInfo();
  tft.setCursor(0, 33);
  tft.setTextColor(MENU_INFO);
  tft.println(F("DigiBadge Version 3"));
  tft.println(F("phoenixborntech.com"));
  tft.print(F("Battery Voltage: "));
  int v = pwr.getBatt();
  Serial.println(v);
  float volt = v / 1000.00;
  Serial.println(volt);
  if (volt < 1.5) {
    tft.setTextColor(MENU_WARN);
  }
  tft.print(volt);
  tft.setTextColor(MENU_INFO);
  tft.println(F("v"));
  if (!bitRead(i,0)) {
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
    if (bitRead(i,0)) {
      tft.println(F("No SD Card to mount"));
      tft.println("");
    }
    else {
      tft.println(F("SD Card Mounting Failed"));
      tft.println("");
    }
  }
  tft.println("");
  tft.println(F("Code v0.4, 7/9/2017"));
  tft.println(F("For updates, see:"));
  tft.println(F("phoenixborntech.com/dbv3"));
}

void drawMenu(char title[], char i1[], char i2[], char i3[],
              char i4[], char i5[], char i6[], char i7[],
              char i8[], char i9[], char i10[], char i11[],
              char i12[], char i13[], char i14[], char i15[]){
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.setTextColor(MENU_TITLE);
  tft.println(title);
  tft.setTextColor(MENU_TXT);
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
    else if (s == 5) {
      //Enter Badge Settings menu
      Serial.println(F("Entering Badge Menu"));
      return 4;
    }
    else if (s == 6){
      //Brightness Settings
      Serial.println(F("Entering Brightness Menu"));
      return 1;
    }
    else if (s == 7){
      //Slideshow Timer
      Serial.println(F("Entering Slideshow Timer Menu"));
      return 2;
    }
    else if (s == 8){
      //Device Information
      Serial.println(F("Entering Device Information"));
      return 3;
    }
    else if (s == 9){
      //Shut down the system
      Serial.println(F("Shutting down the system."));
      saveSettings();//Save settings to text file in SPIFFS.
      pwr.powerOff();
      //Put a delay in here to make sure we don't get caught in things.
      delay(2000);
      return 0;
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
      bklt += 5;
      if (bklt < 10) {
        bklt = 255; //We accidentally rolled around. Fix it.
      }
      pwr.setBacklight(bklt);
      return 1;//Return to the same menu
    }
    else if (s == 2){
      //Brightness down
      Serial.println(F("Decrease Brightness"));
      bklt -= 5;
      if (bklt < 10){
        bklt = 10;
      }
      pwr.setBacklight(bklt);
      return 1; //Return to the same menu
    }
    else if (s == 3){
      //Max Brightness
      Serial.println(F("Maximum Brightness"));
      bklt = 255;
      pwr.setBacklight(bklt);
      return 1; //Return to same menu
    }
    else if (s == 4) {
      //Min Brightness
      Serial.println(F("Minimum Brightness"));
      bklt = 10;
      pwr.setBacklight(bklt);
      return 1;
    }
    else if (s == 5){
      //Default brightness
      Serial.println(F("Default Brightness"));
      bklt = 175;
      pwr.setBacklight(bklt);
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
  else if (c == 4){
    if (s == 1) {
      Serial.println(F("Default Badge Style"));
      bstyle = 0;
      return 0;
    }
    else if (s == 2) {
      Serial.println(F("BronyCon Badge Style"));
      bstyle = 1;
      return 0;
    }
    else if (s == 3) {
      Serial.println(F("ASAN Badge Style"));
      bstyle = 2;
      return 0;
    }
  }
}
