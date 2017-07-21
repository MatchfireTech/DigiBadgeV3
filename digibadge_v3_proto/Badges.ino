//Draw Badges
//Badges are based on the Autism Self-Advocacy Network's Color Communications Badges
//They're based off of the ones adapted for Bronycon: http://bronycon.org/about/policies/color-communication-cards/
//The current ones for the Autism Self-Advocacy Netowork are a little different: http://autisticadvocacy.org/wp-content/uploads/2014/02/ColorCommunicationBadges.pdf
//The DrawBadge function is designed for a bit of flexibility in design shape.
//My "Default" badges are designed for visibility, and are based on the BronyCon badges
//BronyCon-Accurate Badges are designed to match the BronyCon badges as much as possible.
//ASAN Badges use the ASAN symbols, but with the text on the bottom instead.
//To switch between is/will be an option in the menu.

#define badgetxt 6 //Badge text size factor.

void badgeLoop () {
  Serial.println(F("Badge Mode Started"));
  updateBadge(); //We JUST started Badge mode. We need to draw the badge.
  delay(250); //Short delay to prevent rapid flitting between menu and mode
  while (!menuOn) { //Only quit when we go to menu mode.
    i = pwr.getInfo();
    byte ob = badge;
    if (!bitRead(i, 1) and (bitRead(i, 3) and bitRead(i, 2))){
      if (badge > 0) {
        badge--;
      }
      else {
        //Loop around.
        badge = 2;
      }
    }
    else if (!bitRead(i, 3) and ((bitRead(i, 1) and bitRead(i, 2)))){
      badge++;
      if (badge > 2) {
        //Loop around.
        badge = 0;
      }
    }
    else if (!bitRead(i, 2) and ((bitRead(i, 1) and bitRead(i, 3)))){
      menuOn = true; //Exit to menu.
    }
    delay(15);
    i = clearButton();
    if (ob != badge) {
      updateBadge();
    }
    delay(upspd); //Wait a little before updating. Updating too fast will cause us to speed through things at alarming rates.
  }
}

void updateBadge() {
  if (badge == 0) {
    greenBadge();
  }
  else if (badge == 1) {
    yellowBadge();
  }
  else if (badge == 2) {
    redBadge();
  }
  else {
    //This should never happen.
    badge = 1;
    yellowBadge();
  }
}

void yellowBadge() {
  Serial.print(F("Displaying Yellow badge. Time: "));
  int t = millis();
  if (bstyle == 0) {
    //Badge Style 0 = Default.
    drawBadge(TFT_YELLOW, "R", "YELLOW", 230, 50);
  }
  else if (bstyle == 1) {
    //Badge Style 1 = BronyCon
    drawBadge(TFT_YELLOW, "R", "YELLOW", 230, 50, TFT_WHITE, TFT_BLACK, TFT_WHITE, 6, 2, 0);
  }
  else if (bstyle == 2) {
    //Badge Style 2 = ASAN
    drawBadge(TFT_YELLOW, "T", "YELLOW", 150, 130, TFT_BLACK, TFT_WHITE, TFT_BLACK, 2, 4);
  }
  else {
    //Badge Style is out of what it should be.
    //Reset to default.
    bstyle = 0;
    Serial.println(F("ERR"));
    yellowBadge();
  }
  t = millis() - t;
  Serial.print(t); Serial.println(F(" ms"));
}

void greenBadge() {
  Serial.print(F("Displaying Green badge. Time: "));
  int t = millis();
  if (bstyle == 0) {
    //Badge Style 0 = Default.
    drawBadge(TFT_GREEN, "C", "GREEN", 65);
  }
  else if (bstyle == 1) {
    //Badge Style 1 = BronyCon
    drawBadge(TFT_GREEN, "C", "GREEN", 65, 65, TFT_WHITE, TFT_WHITE, TFT_GREEN, 6, 2, 15);
  }
  else if (bstyle == 2) {
    //Badge Style 2 = ASAN
    drawBadge(TFT_GREEN, "C", "GREEN", 65, 65, TFT_BLACK, TFT_WHITE, TFT_BLACK, 2, 4);
  }
  else {
    //Badge Style is out of what it should be.
    //Reset to default.
    bstyle = 0;
    Serial.println(F("ERR"));
    greenBadge();
  }
  t = millis() - t;
  Serial.print(t); Serial.println(F(" ms"));
}

void redBadge() {
  Serial.print(F("Displaying Red badge. Time: "));
  int t = millis();
  if (bstyle == 0) {
    //Badge Style 0 = Default.
    drawBadge(TFT_RED, "R", "RED", 126);
  }
  else if (bstyle == 1) {
    //Badge Style 1 = BronyCon
    drawBadge(TFT_RED, "R", "RED", 126, 126, TFT_WHITE, TFT_WHITE, TFT_RED, 6, 2, 15);
  }
  else if (bstyle == 2) {
    //Badge Style 2 = ASAN
    drawBadge(TFT_RED, "O", "RED", 126, 126, TFT_BLACK, TFT_WHITE, TFT_BLACK, 2, 4);    
  }
  else {
    //Badge Style is out of what it should be.
    //Reset to default.
    bstyle = 0;
    Serial.println(F("ERR"));
    redBadge();
  }
  t = millis() - t;
  Serial.print(t); Serial.println(F(" ms"));
}

void drawBadge(uint32_t color, const char *shape, const char *text, int swidth, int sheight, uint32_t textcolor, uint32_t bordercolor, uint32_t shapecolor, byte textloc, byte symloc, byte border) {
  //Color = Background Color of badge.
  //Shape = Shape used for symbol. RECT, CIRCLE, ROUNDRECT, TRIANGLE, INVTRIANGLE, and OCTAGON are options.
  //These can be shortened to R, C, RR, T, IT, and O.
  //Text = Text appearing below symbol.
  //SWidth = Symbol Width. INCLUDES border.
  //SHeight = Symbol Height. Default is 0, which is interpereted as being same as width. INCLUDES border.
  //Textcolor = Self-descriptive. Default white
  //Bordercolor = Self-descriptive. Default Black
  //Shapecolor = Self-descriptive. Default White.
  //TextLoc = Text Location, in 8ths, from top of screen.
  //symloc = Symbol Location, in 6th, from top of screen.
  //Border = How many pixels wide the symbol border is. Default 5
  tft.setRotation(1); //Badges are in landscape.
  tft.fillScreen(color); //Fill the background.
  //Text operations. Centers it left/right and 3/4 down the bottom.
  tft.setTextSize(badgetxt);
  tft.setTextColor(textcolor);
  //Center the text left/right
  int x = strlen(text); //Get the length of the text.
  x = x * 6 * badgetxt; //6 pixels wide per character, multiplied by the size factor for badges. Total width of text.
  x = x / 2; //Divide it in half.
  x = (tft.width() / 2) - x; //Get the midpoint of the TFT screen, subtract the half-size of the text.
  //Center the text up/down over the 3/4 mark down the screen.
  int y = 8 * badgetxt; //8 pixels high per character, multiplied by the size factor for badges.
  y = y / 2; //Divide it in half.
  y = int(tft.height() * (textloc / 8.0)) - y; //Get the text location, center text on top of it.
  //Set the cursor and print the text.
  tft.setCursor(x, y);
  tft.print(text);
  //Symbol operations.
  if (sheight == 0) {
    //If we have a default height, set it to width.
    sheight = swidth;
  }
  if (shape == "RECT" or shape == "ROUNDRECT" or shape == "R" or shape == "RR") {
    //Shape starts at top/left.
    //Centered Left/Right
    x = (tft.width() / 2) - (swidth / 2);
    //1/3 of the way top/bottom
    y = int(tft.height() * (symloc / 6.0)) - (sheight / 2);
    if (shape == "RECT" or shape == "R") {
      tft.fillRect(x, y, swidth, sheight, bordercolor);
      tft.fillRect(x+border, y+border, swidth - (border*2), sheight - (border*2), shapecolor);
    }
    else if (shape == "ROUNDRECT" or shape == "RR") {
      //For a nice rounded rect, set the radius as 1/4 of the width, whichever is shorter.
      int r;
      if (swidth < sheight) {
        r = swidth / 4;
      }
      else {
        r = sheight / 4;
      }
      tft.fillRoundRect(x, y, swidth, sheight, r, bordercolor);
      tft.fillRoundRect(x+border, y+border, swidth - (border*2), sheight - (border*2), r, shapecolor);
    }
  }
  else if (shape == "CIRCLE" or shape == "C") {
    //Shape starts at center. Makes math easy.
    //Centered Left/Right. 
    //1/3 of the way top/bottom.
    //Ignores sheight, as... Well, circle.
    x = tft.width() / 2;
    y = int(tft.height() * (symloc / 6.0));
    //Circles are really easy.
    tft.fillCircle(x, y, swidth, bordercolor);
    tft.fillCircle(x, y, swidth - border, shapecolor);
  }
  else if (shape == "TRIANGLE" or shape == "T") {
    //Shape starts at bottom/left.
    //While we CAN make almost any shape triangle, we keep it simple.
    //The bottom is flat, and top point is centered.
    x = (tft.width() / 2) - (swidth / 2);
    y = int(tft.height() * (symloc / 6.0)) + (sheight / 2);
    int x2 = x + swidth;
    int x3 = x + (swidth / 2);
    int y3 = y - sheight;
    int b1 = border * 2;
    tft.fillTriangle(x, y, x2, y, x3, y3, bordercolor);
    tft.fillTriangle(x+b1, y-border, x2-b1, y-border, x3, y3+b1, shapecolor);
  }
  else if (shape == "INVTRIANGLE" or shape == "IT") {
    //Inverted Triangle. Shape starts at top/left.
    //Top is flat, bottom point is centered.
    x = (tft.width() / 2) - (swidth / 2);
    y = int(tft.height() * (symloc / 6.0)) - (sheight / 2);
    int x2 = x + swidth;
    int x3 = x + (swidth / 2);
    int y3 = y + sheight;
    int b1 = border * 2;
    tft.fillTriangle(x, y, x2, y, x3, y3, bordercolor);
    tft.fillTriangle(x+b1, y+border, x2-b1, y+border, x3, y3-b1, shapecolor);
  }
  else if (shape == "OCTAGON" or shape == "O") {
    //Oh boy oh boy!
    //Octagon is a bit more complicated.
    //First, find the edge length.
    int e1 = swidth / (1 + sqrt(2)); //Used for top/bottom edges
    int e2 = sheight / (1 + sqrt(2)); //Used for left/right edges
    //Now we can find the first rect start coordinates.
    x = (tft.width() / 2) - (swidth / 2); //Centered, of course.
    y = int(tft.height() * (symloc / 6.0)) - (e2 / 2); //Centered 1/3 of the way down.
    //First rect.
    tft.fillRect(x, y, swidth + 1, e2, bordercolor);
    tft.fillRect(x+border, y, swidth - (border * 2) + 1, e2, shapecolor);
    //And the second rect's start coordinates.
    int x2 = (tft.width() / 2) - (e1 / 2);
    int y2 = int(tft.height() * (symloc / 6.0)) - (sheight / 2);
    //Fill the rect.
    tft.fillRect(x2, y2, e1, sheight+1, bordercolor);
    tft.fillRect(x2, y2 + border, e1, sheight - (border * 2) + 1, shapecolor);
    //Now! For the angles.
    //Angle 1 is easy. Top/Left, we can use the existing coordinates.
    tft.fillTriangle(x, y, x2, y2, x2, y, bordercolor);
    tft.fillTriangle(x+border, y, x2, y2+border, x2, y, shapecolor);
    //Angle 2. Top/Right. Y-coordinates are valid. X are not.
    tft.fillTriangle(x+swidth, y, x2+e1, y2, x2+e1, y, bordercolor);
    tft.fillTriangle(x+swidth-border, y, x2+e1, y2+border, x2+e1, y, shapecolor);
    //Angle 3. Bottom/Left. X-coordinates are valid. Y are not.
    tft.fillTriangle(x, y+e2, x2, y2+sheight, x2, y+e2, bordercolor);
    tft.fillTriangle(x+border, y+e2, x2, y2+sheight-border, x2, y+e2, shapecolor);
    //Angle 4. Bottom/Right. X and Y are invalid, so we need to use all calced ones.
    tft.fillTriangle(x+swidth, y+e2, x2+e1, y2+sheight, x2+e1, y+e2, bordercolor);
    tft.fillTriangle(x+swidth-border, y+e2, x2+e1, y2+sheight-border, x2+e1, y+e2, shapecolor);
   
  }
}

