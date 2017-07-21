//SD Card functions.

void startSD() {
  //Check to see if we have an SD card
  i = pwr.getInfo();
  if (bitRead(i,0) == 0) {
    //We have an SD card.
    byte sd_tries = 0;
    while (!sd_load and sd_tries < 5) {
      Serial.print(F("Attempting SD load..."));
      //Try 5 times to load the SD card.
      sd_load = SD.begin(sd_cs);
      if (sd_load) {
        Serial.println(F("Sucess!"));
      }
      else {
        Serial.print(F("Load failed."));
        sd_tries ++;
        if (sd_tries < 5) {
          Serial.println(F(" Retrying."));
        }
        else {
          Serial.println(F("\nSD Load failure."));
        }
      }
    }    
  }
  else {
    Serial.println(F("No SD card found."));
  }
}

void forceStartSD() {
  byte sd_tries = 0;
  while (!sd_load and sd_tries < 5) {
    Serial.print(F("Attempting SD load..."));
    //Try 5 times to load the SD card.
    sd_load = SD.begin(sd_cs);
    if (sd_load) {
      Serial.println(F("Sucess!"));
    }
    else {
      Serial.print(F("Load failed."));
      sd_tries ++;
      if (sd_tries < 5) {
        Serial.println(F(" Retrying."));
      }
      else {
        Serial.println(F("\nSD Load failure."));
      }
    }
  }
}

void countJPEG() {
  //Counts the number of appropriate JPEG files found.
  Serial.println(F("Counting JPEG images."));
  unsigned int count = 0;
  File dir = SD.open("/"); //Open the base directory.
  dir.rewindDirectory(); //Just in case someone hasn't been kind.
  File fi = dir.openNextFile(); //Open first file.
  while (fi) {
    //While we have the file open.
    if (! fi.isDirectory()) {
      //A file. Check if it is a jpeg.
      if ((String(fi.name()).endsWith(".jpg")) or (String(fi.name()).endsWith(".JPG"))){
        //It does. Count it.
        Serial.print(fi.name());
        Serial.println(F(" found."));
        count ++;
      }
    }
    fi.close(); //Close the file
    delay(5); //Delay a little bit to be safe.
    fi = dir.openNextFile(); //Open the next file.
  }
  dir.rewindDirectory();
  dir.close(); //Close the directory
  imgnum = count;
  Serial.print(count);
  Serial.println(" JPEG files found.");
}

void drawSDJPEG(unsigned int fnum, int xpos, int ypos) {
  //Functions much like countJPEG, except when it finds the image it's looking for it stops and displays it.
  unsigned int count = 0;
  Serial.print(F("Displaying Image Number "));
  Serial.println(fnum);
  File dir = SD.open("/");
  dir.rewindDirectory();
  File fi = dir.openNextFile();
  while (fi) {
    if (! fi.isDirectory()) {
      if ((String(fi.name()).endsWith(".jpg")) or (String(fi.name()).endsWith(".JPG"))){
        count ++;
        if (count == fnum) {
          //We've found the image we're looking for. Display it.
          Serial.print(F("Image "));
          Serial.print(fi.name());
          Serial.print(F(" Found at position "));
          Serial.println(count);
          fi.close();
          loadJPEG(fi.name(), xpos, ypos);
          break;
        }
      }
    }
    fi.close();
    delay(5);
    fi = dir.openNextFile();
  }
  dir.rewindDirectory();
  dir.close();
}

