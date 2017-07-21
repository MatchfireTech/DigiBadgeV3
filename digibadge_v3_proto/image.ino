//Image slideshow and static image display mode.
//If we're in Slideshow mode, automatically change every few seconds.
//Otherwise, just sit on the same image.
//We don't worry about if we don't have any images, since we shouldn't come here if that is the case.

void imageLoop() {
  Serial.println(F("Image Mode started"));
  drawSDJPEG(curimg, 0, 0);
  delay(250); //Short delay to prevent rapid flitting between menu and mode
  while (!menuOn) { //Only quit when we go to menu mode
    i = pwr.getInfo();
    int img = curimg;
    if (!bitRead(i, 1) and (bitRead(i, 3) and bitRead(i, 2))){
      if (curimg > 0) {
        curimg--;
      }
      else {
        //Loop around.
        curimg = imgnum;
      }
    }
    else if (!bitRead(i, 3) and ((bitRead(i, 1) and bitRead(i, 2)))){
      curimg++;
      if (curimg > imgnum) {
        //Loop around.
        curimg = 1;
      }
    }
    else if (!bitRead(i, 2) and ((bitRead(i, 1) and bitRead(i, 3)))){
      menuOn = true; //Exit to menu.
    }
    delay(15);
    i = clearButton();
    if (img != curimg) {
      drawSDJPEG(curimg, 0, 0);
    }
    delay(upspd); //Wait a little before updating. Updating too fast will cause us to speed through things at alarming rates.
  }
}

void slideshowLoop() {
  Serial.println(F("Slideshow mode started"));
  drawSDJPEG(curimg, 0, 0);
  delay(250); //Short delay to prevent rapid flitting between menu and mode
  int scount = 0;
  while (!menuOn) { //Only quit when we go to menu mode
    i = pwr.getInfo();
    int img = curimg;
    if (!bitRead(i, 1) and (bitRead(i, 3) and bitRead(i, 2))){
      if (curimg > 0) {
        curimg--;
      }
      else {
        //Loop around.
        curimg = imgnum;
      }
    }
    else if (!bitRead(i, 3) and ((bitRead(i, 1) and bitRead(i, 2)))){
      curimg++;
      if (curimg > imgnum) {
        //Loop around.
        curimg = 1;
      }
    }
    else if (!bitRead(i, 2) and ((bitRead(i, 1) and bitRead(i, 3)))){
      menuOn = true; //Exit to menu.
    }
    delay(15);
    i = clearButton();
    if (img != curimg) {
      drawSDJPEG(curimg, 0, 0);
    }
    delay(upspd); //Wait a little before updating. Updating too fast will cause us to speed through things at alarming rates.
    scount += upspd; //Increment the slideshow counter
    if (scount >= slide) {
      //We've waited for the alloted time. Next image!
      Serial.println(F("Slide updating."));
      curimg ++;
      if (curimg > imgnum) {
        curimg = 1;
      }
      drawSDJPEG(curimg, 0, 0);
      scount = 0;
    }
  }
}

