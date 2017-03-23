# DigiBadgeV3
Code and files for Version 3 of the Matchfire DigiBadge.
Looking for previous versions?
[Version 2 can be found here](https://github.com/MatchfireTech/DigiBadgeV3)
[Version 1 can be found here](https://github.com/Andon-A/DigiBadge)

Currently in the prototype stage, so not all files are available, and no files should be taken as final.

To view and edit the PCB, the PCBWeb program is required. It is free and available at http://www.pcbweb.com

#Description
The Version 3 is a complete redesign of the DigiBadge, with a new microcontroller, new screen, new power system, and a host of other goodies:
*ESP8266 core, on a ESP-12E chip. The WiFi is disabled by default, but can easily be enabled and used by custom code!
*2.2" TFT Screen, with a visible display about the size of the V2! It also has a 320x280 resolution for higher quality pictures.
*3.3v Boost Regulator, to ensure you get the most out of your batteries
*A three-button front-side control setup, utilizing a menu system to simplify use. No more navigation switch!
*Native USB interface, with a built-in micro-usb port. Please note that this will **not** be sufficient to power the device alone!
*Completely re-written code, for easier understanding and modification. This code also includes a menu system for simplicity and ease of access.
*The Version 3 also includes several Pride Flags, as detailed below.

#More description
As I'm not done writing the code and designing the PCB yet, I'm also not done documenting thigns since they'll probably change.
More things will go here as they're relevant.
**Of Note**: Currently there are no datasheets included, as the parts may or may not change.
**Code Base**: The Code is currently a prototype. Everything works, so far, but there is a lot of pieces missing. The backlight is going to be run through an I2C potentiometer, for example. And there are currently no filesystem interactions, so nothing is saved.
