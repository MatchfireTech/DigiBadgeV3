#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9340.h> // Hardware-specific library
#include <ESP8266WiFi.h> // ESP8266 Wifi specific library
#define FS_NO_GLOBALS //To make Filesystem work.
#include <FS.h> // ESP8266 Filesystem library
#include <SPI.h> //So we can talk to things
#include <SD.h> //So we can pull information from the SD card

//Defines for pins.
#define sd_cs 16
#define tft_cs 0
#define tft_dc 15
#define bklt   5 //Will be an I2C device!
#define txtsz 2 //Menu Text size

Adafruit_ILI9340 tft = Adafruit_ILI9340(tft_cs,  tft_dc, -1);

//Globals
int mde = 0; //0 is Badge, 1 is Image, 2 is Slideshow, 3 is Flags
int bright = 700; //Default Brightness. About 3/4 bright.
int slide = 5000;
int bdg = 1; //Default to yellow badge
int flg = 1; //Default to LGBT Flag
int img = 1; //Default to the first image
int imgnum = 0; //Number of images loaded.
bool sd_load = false; //If we've loaded an SD card or not.
bool menuOn = false; //We want to remember where we entered the menu from, if we want to exit where we were.

//Function prototypes
int getButton(int);
void menu(void);
void drawCursor(int os, int s);
int newMenu(int c);
int select(int c = 0, int sel = 1);
void drawMenu(char title[], char i1[], char i2[] = " ", char i3[] = " ",
              char i4[] = " ", char i5[] = " ", char i6[] = " ", char i7[] = " ",
              char i8[] = " ", char i9[] = " ", char i10[] = " ", char i11[] = " ",
              char i12[] = " ", char i13[] = " ", char i14[] = " ", char i15[] = " ");
void findBMP(int img, int x = -1, int y = -1, bool = true);

void setup() {
  //Start up the device.
  //pinMode(PWR, OUTPUT); //This pin will enable the power regulator while on.
  //digitalWrite(PWR, HIGH);
  //Turn off the WiFi.
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
  pinMode(bklt, OUTPUT); //Temporary Brightness
  analogWrite(bklt, 0); //Turn off screen until we're ready
  //Begin Serial communications for debugging.
  //74880 is used by the ESP8266 for its own startup, so may as well use it.  
  Serial.begin(74880);
  delay(50);
  Serial.println(F("")); //Empty line, so we start after the ESP8266 debug stuff.
  Serial.println(F("DigiBadge V3 Starting"));
  //Set up the screen.
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9340_BLACK);
  //Grab our stored data
  getSettings();
  //Set up the backlight.
  setBright(); 
  bootScreen();
  startSD();
}

void loop() {
  //Loop is a simple handler that tosses between the various modes.
  Serial.println(F("Main Loop Entered"));
  if (menuOn) {
    //If we want to go to the menu, go to the menu.
    menu();
  }
  //If we're here, then we're not going to the menu.
  else if (mde == 0) {
    //Badge mode.
    badge();
  }
  else if (mde == 1) {
    //Image Mode.
    image();
  }
  else if (mde == 2) {
    //Slideshow Mode
    slideshow();
  }
  else if (mde == 3) {
    //Flag mode.
    flag();
  }
  else {
    //We shouldn't get here. We should go to the menu, just to be safe.
    tft.fillScreen(ILI9340_BLUE);
    menuOn = true;
  }
}

float getVoltage() {
  //Eventually will display battery voltage.
  //For now, just return a number.
  return 1.20;
}

void getSettings() {
  //Grabs the settings from the Flash chip.
  //Or, well, it will.
  mde = 0; //Grab Mode information.
  bright = 700; //Grab Brightness information
  slide = 5000; //Grab Slideshow delay information
  //img = 0; //Grab which Image we're on.
  bdg = 1; //Grab which badge we're on.
  flg = 1; //Grab which flag we're on.
}

void setBright() {
  //Honestly, really simple.
  analogWrite(bklt, bright);
}

int getButton() {
  //Returns 0 for no buttons,
  //1,2,3 for buttons 1,2,3
  //4 for no buttons and SD card present.
  //Should read 82 for SD card
  //808-811 for Button 1
  //673-682 for Button 2
  //490-513 for Button 3
  //Second number is for button pressed with SD card
  //I COULD code that in, but... Honestly, not worth it.
  //Read the pin first
  int x = analogRead(0);
  //Start at the lowest
  if (x < 75){
    //No buttons.
    return 0;
  }
  else if (x < 450){
    //SD Present, no buttons pressed
    return 4;
  }
  else if (x < 650){
    //Button 3
    return 3;
  }
  else if (x < 750){
    //Button 2
    return 2;
  }
  else if (x < 1000){
    //Button 1
    return 1;
  }
  else {
    //Something weird.
    return x;
  }
}
