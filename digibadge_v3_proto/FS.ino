
bool saveSettings() {
  //Saves all current settings, for re-loading when the device is started again.
  Serial.println(F("Saving Settings..."));
  fs::File settings = SPIFFS.open("/settings.txt", "w");
  if (!settings) {
    Serial.println(F("FAILED"));
    return false;
  }
  else {
    settings.print("bklt=");
    settings.println(bklt);
    settings.print("mode=");
    settings.println(mde);
    settings.print("badge=");
    settings.println(badge);
    settings.print("style=");
    settings.println(bstyle);
    settings.print("flag=");
    settings.println(flag);
    settings.print("image=");
    settings.println(curimg);
    settings.print("slide=");
    settings.println(slide);
    settings.close();
    Serial.println("Settings Saved");
    return true;
  }
}

void loadSettings() {
  Serial.println(F("Loading settings from file"));
  fs::File settings = SPIFFS.open("/settings.txt", "r");
  while (settings.available()) {
    String line = settings.readStringUntil('=');
    String var = settings.readStringUntil('\n');
    if (line == "bklt") {
      bklt = (byte)var.toInt(); 
    }
    else if (line == "mode") {
      mde = (byte)var.toInt(); 
    }
    else if (line == "badge") {
      badge = (byte)var.toInt(); 
    }
    else if (line == "style") {
      bstyle = (byte)var.toInt(); 
    }
    else if (line == "flag") {
      flag = (byte)var.toInt(); 
    }
    else if (line == "image") {
      //Only load current image if we have an SD card loaded with images.
      if (imgnum >= var.toInt()){
        curimg = var.toInt();
      }
    }
    else if (line == "slide") {
      slide = var.toInt();
    }
  }
  Serial.println("Finished.");
}

