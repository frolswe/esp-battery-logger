esp-battery-logger
==================
Simple project for the esp8266 using the power saving features of the
chip to test the possibility of battery powered sensors.

![esp-photo](https://pbs.twimg.com/media/B_P5WMMUoAMHy9B.jpg:large)

Requirements
------------
### [XPD_DCDC connected to the reset pin](http://hackaday.com/2015/02/08/hack-allows-esp-01-to-go-to-deep-sleep/)
The XPD_DCDC pin on the esp8266 chip must be connected to the reset
pin. Some esp modules (esp-01) requires soldering this connection
directly on the chip.

### [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk)
This SDK and compiler. Clone and build this repository and edit the
Makefile with the correct path.

### [esp_mqtt](https://github.com/tuanpmt/esp_mqtt.git)
Cloned using git submodules. Used for sending battery status to an
MQTT server. Also the Makefile and uart driver from esp_mqtt is used
in this project.

### [esptool](https://github.com/themadinventor/esptool.git)
Tool used for the final parts of the firmware build process and to
flash it to the chip. Cloned using git submodules.

Build process
-------------
1. Clone and build esp-open-sdk.  
   `$ cd esp-open-sdk && make`
2. Checkout the git submodules.  
   `$ git submodule init`  
   `$ git submodule update`
3. Make sure PATH includes the newly built toolchain.  
   `$ export PATH=$PATH:/path/to/esp-open-sdk/xtensa-lx106-elf/bin`
4. Change the `SDK_BASE` in the Makefile and the `WIFI_PASS` etc in
   user_main.c for your environment.
5. Build and flash.  
   `$ make test`

Donations
=========
Please send me an encouraging email, or tips on
https://www.changetip.com/tipsters/FrolHuppSe

License
=======
The MIT License (MIT)

Copyright (c) 2015 Fredrik Olofsson <frol@hupp.se>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
