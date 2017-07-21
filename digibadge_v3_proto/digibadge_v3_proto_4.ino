#include <SPI.h>
#include <Wire.h>
#include <TFT_eSPI.h> //Controlls the TFT. https://github.com/Bodmer/TFT_eSPI
#include <v3_Power.h> //Controlls the ATTiny that controlls the power.
#include <JPEGDecoder.h> //For decoding JPEGs. https://github.com/Bodmer/JPEGDecoder
#include <ESP8266WiFi.h> // ESP8266 Wifi specific library

#define sd_cs 16
#define upspd 100
#define menuspd 100 //Menu tends to get a bit twitchy at lower numbers
#define menutxt 2

TFT_eSPI tft = TFT_eSPI();
v3_Power pwr = v3_Power();

byte i = 0; //Used for buttons and other info.
byte bklt = 175; //Backlight brightness. 0-255
bool sd_load = false; //If we have a SD card loaded or not.
bool menuOn = false; //If we're in the menu or not.
byte badge = 1; //0 is green, 1 is Yellow, 2 is Red
byte mde = 1; //0 is Badge, 1 is Image, 2 is Slideshow, 3 is Flags
byte bstyle = 0; //Badge Style. 0 is Default. 1 is BronyCon, 2 is ASAN.
byte flag = 0; //0 is LGBT, 1 is Bi, 2 is Trans, 3 is Pan, 4 is Asexual.
unsigned int imgnum = 0; //Number of images found.
unsigned int curimg = 1; //Which image we are on.
unsigned int slide = 2500; //5 seconds between slides.

//Prototypes
void drawBadge(uint32_t color, const char *shape, const char *text, int swidth, int sheight = 0, uint32_t textcolor = TFT_BLACK, uint32_t bordercolor = TFT_BLACK, uint32_t shapecolor = TFT_WHITE, byte textloc = 6, byte symloc = 2, byte border = 5);
void drawFlag(int h1, uint32_t c1, int h2 = 0, uint32_t c2 = TFT_BLACK, int h3 = 0, uint32_t c3 = TFT_BLACK, int h4 = 0, uint32_t c4 = TFT_BLACK,
              int h5 = 0, uint32_t c5 = TFT_BLACK, int h6 = 0, uint32_t c6 = TFT_BLACK);
void drawMenu(char title[], char i1[], char i2[] = " ", char i3[] = " ",
              char i4[] = " ", char i5[] = " ", char i6[] = " ", char i7[] = " ",
              char i8[] = " ", char i9[] = " ", char i10[] = " ", char i11[] = " ",
              char i12[] = " ", char i13[] = " ", char i14[] = " ", char i15[] = " ");

ADC_MODE(ADC_VCC);

void setup() {
  //Start I2C, TFT, SPIFFS and Serial.
  Wire.begin();
  Serial.begin(74880);
  Serial.println(F("\nDigiBadge Starting"));
  i = pwr.getInfo();
  if (bitRead(i, 4) == 0){
    //We're not supposed to be on. Enter deep sleep.
    //We set this to 0, as we wait on the ATTiny to wake us.
    Serial.println(F("Power should be off. Sleeping."));
    //ESP.deepSleep(0); 
    //Not actually hooked up yet.
  }
  //Turn off the WiFi.
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
  Serial.print(F("Starting Spiffs..."));
  if (SPIFFS.begin()){
    Serial.println(F("Success."));
  }
  else {
    Serial.println(F("Failed!"));
  }
  tft.begin();
  //Serial.print(F("TFT ID: "));
  //Serial.println(tft.readID());
  tft.setRotation(1);
  tft.fillScreen(TFT_BLUE);
  pwr.setBacklight(bklt);
  forceStartSD(); //When a prototype with detector exists, or with final version, replace with StartSD.
  if (sd_load) {
    //We have an SD card. Count the number of usable JPEGs.
    countJPEG();
  }
  loadSettings();
  Serial.println(pwr.getVCC());
  Serial.println(pwr.getBatt());
  Serial.println("VCC'd");
}

void loop() {
  //i = clearButton();
  if (menuOn) {
    menu();
  }
  else if (mde == 0){
    badgeLoop();
  }
  else if (mde == 1){
    imageLoop();
  }
  else if (mde == 2){
    slideshowLoop();
  }
  else if (mde == 3){
    flagLoop();
  }
}

byte clearButton() {
  //Sets the buttons to be "Off"
  byte bt = pwr.getInfo();
  //Set bits 1, 2, and 3 to high
  bitSet(bt,1);
  bitSet(bt,2);
  bitSet(bt,3);
  return bt;
}

