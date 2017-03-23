bool startSD() {
  //Starts up the SD card.
  Serial.println(F("Attempting to start SD card"));
  bool det = checkSD(); //Prototype isn't set up for this yet, so this will still return true.
  if (det) { //We have an SD card.
    if (! SD.begin(sd_cs)) {
      //SD card failed to load.
      Serial.println(F("SD Card load failed"));
    }
    else {
      //SD Card loaded. Count the files.
      Serial.println(F("SD Card loaded. Counting files."));
      imgnum = 0;
      findBMP(-1);
      Serial.print(F("Loaded "));
      Serial.print(imgnum);
      Serial.println(F(" files"));
      sd_load = true; //Let the rest of the program know we have successfully loaded the file.
    }
  }
  else { //No SD card found.
    Serial.println(F("No SD card found"));
  }
}

bool checkSD(){
  //Eventually, we'll have to go and get an analog voltage 
  return true;
}

void findBMP(int img, int x, int y, bool fill) {
  Serial.println(F("Finding Image"));
  //Either finds a file at located position
  //OR simply counts them, if img is -1
  int count = 0;
  File dir = SD.open("/"); //Open Directory
  dir.rewindDirectory(); //Rewind everything, because sometimes it gets funky.
  File fi = dir.openNextFile(); //Open the first file.
  while (fi) {
    //As long as we have a file open...
    if (! fi.isDirectory()) {
      //A file. Check if it is a BMP.
      if (String(fi.name()).endsWith(".BMP")) {
        //It's a BMP image
        count++; //Count it
        if ((count == img) && (img >= 1)) {
          //This is the file we're looking for.
          fi.close(); //Close it.
          //Serial.print(F("Loaded File: "));
          //Serial.println(fi.name());
          bmpDraw(fi.name(), x, y, fill); //Display it.
          break; //No need to go searching anymore.
        }
      }
    }
    fi.close(); //Close the file, open the next.
    delay(10); //Short delay, just to be safe.
    fi = dir.openNextFile();
  }
  dir.close(); //Close the directory
  if (img < 1) {
    //We were counting files.
    //Serial.print(F("Updating filecount to "));
    imgnum = count;
    //Serial.println(filecount);
  }
}

#define BUFFPIXEL 20 //Pixel buffer. I haven't seen any performance gains from increasing this, and 80+ causes issues
#define backfill 0x0000 //Color of backfill. Makes for easy change if that's what we want.

void bmpDraw(char *filename, int x, int y, bool fill) {
  //This is adapted from the various Adafruit BMP loading examples.
  //Some things have been stripped out for ease of use.
  //Also added an auto-centering and auto-backfilling option.

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3 * BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if ((x >= tft.width()) || (y >= tft.height())){
    Serial.println("Abort");
    Serial.println(x);
    Serial.println(y);
    Serial.println(tft.width());
    Serial.println(tft.height());
    return;
  }

  Serial.print(F("Loading image '"));
  Serial.print(filename);
  Serial.print(F("', image #"));
  Serial.print(img);
  Serial.print(F(" of "));
  Serial.println(imgnum);

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print(F("File not found"));
    return;
  }

  // Parse BMP header
  if (read16(bmpFile) == 0x4D42) { // BMP signature
    read32(bmpFile); //File size. Don't need it.
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image dat
    read32(bmpFile); //Header size
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);

    //If we have the "Default" values of -1, -1, then we want to center the image.
    //We can center only horizontally, or only vertically.
    if (x = -1) {
      if (bmpWidth < tft.width()) {
        //Center the image.
        x = (tft.width() - bmpWidth)/2; //Difference, divided by 2. All-int process removes fractionals.
      }
      else {
        x = 0; //Default to 0 otherwise.
      }
    }
    if (y = -1) {
      if (bmpHeight < tft.height()) {
        y = (tft.height() - bmpHeight)/2; //Same deal with Y axis.
      }
      else {
        y = 0; //Again, default to 0.
      }
    }
    if ((fill) && ((bmpHeight < tft.height()) || (bmpWidth < tft.width()))) {
      //If the BMP height/width is less than the screen size AND fill is true, fill it with the background color.
      tft.fillScreen(backfill);
    }


    
    if (read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      //Serial.print("Bit Depth: "); Serial.println(bmpDepth);
      if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print(F("Image size: "));
        Serial.print(bmpWidth);
        Serial.print(F("x"));
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if (bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if ((x + w - 1) >= tft.width())  w = tft.width()  - x;
        if ((y + h - 1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x + w - 1, y + h - 1);

        for (row = 0; row < h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if (flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
            pos = bmpImageoffset + row * rowSize;
          if (bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col = 0; col < w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.Color565(r, g, b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
        
      } // end goodBmp
    }
  }
  //If in Slideshow mode, add a "Play" indicator to the top/right
  if (mde == 2) {
    tft.fillTriangle(5, 2, 16, 13, 5, 24, ILI9340_BLACK);
    tft.fillTriangle(6, 5, 14, 13, 6, 21, ILI9340_WHITE);
  }
  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}

// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) {
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) {
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}
