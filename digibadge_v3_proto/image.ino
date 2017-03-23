//Image Mode
void image(){
  Serial.println(F("Image Mode Started"));
  //Draw the image.
  findBMP(img);
  while(!menuOn){
    int i = getButton();
    int oi = img;
    if (i != 0){
      Serial.print(F("Button Pressed: "));
      Serial.println(i);
    }
    if (i == 1) {
      //Button 1: Previous image
      img -= 1;
      if (img < 1){
        //Go to highest number for images we have
        img = imgnum;
      }
    }
    else if (i == 2) {
      //Button 2: Go to the Menu.
      menuOn = true;
    }
    else if (i == 3) {
      //Button 3: Next Badge
      img += 1;
      if (img > imgnum){
        //We ran out of images. Cycle back to the beginning.
        img = 1;
      }
    }
    if (img != oi) {
      //We've changed images. Draw the new one.
      findBMP(img);
    }
    delay(125);
  }
}

void slideshow() {
  //Slideshow mode
  Serial.println(F("Entering Slideshow Mode"));
  //Draw the current image
  findBMP(img);
  int t = 0; //We'll use this to change the slides.
  while(!menuOn){
    int i = getButton();
    int oi = img;
    if (i != 0){
      Serial.print(F("Button Pressed: "));
      Serial.println(i);
    }
    if (i == 1) {
      //Button 1: Previous image
      img -= 1;
      if (img < 1){
        //Go to highest number for images we have
        img = imgnum;
      }
    }
    else if (i == 2) {
      //Button 2: Go to the Menu.
      menuOn = true;
    }
    else if (i == 3) {
      //Button 3: Next Badge
      img += 1;
      if (img > imgnum){
        //We ran out of images. Cycle back to the beginning.
        img = 1;
      }
    }
    if (img != oi) {
      //We've changed images. Draw the new one.
      //Reset the timer counter as well.
      findBMP(img);
      t = 0; 
    }
    delay(125);
    //Now for slideshow stuff.
    t += 125;
    if (t >= slide){ //Time to change!
      img += 1; //Increase the image.
      if (img > imgnum){
        //Ran out of images. Start over.
        img = 1;
      }
      t = 0; //Reset the timer
      findBMP(img); //Display the image.
    }
  }
}

