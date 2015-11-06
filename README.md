Conclusion
==========

* 18650 Li-ion cell: approx 40 days per charge.
* 3xAA alkaline betteries almost 100 days.

After running five different setups, I think using a 18650 Li-ion cell
with a charger / protection circuit is the best way for me. The 4.2
volts from this setup is slightly out of spec for the esp8266 but in
practice it has worked very reliably. It is also very convenient to be
able to simply connect a charger when the batteries are empty.

3xAA batteries lasts significantly longer, and is a good option. But I
like the form factor and convenience of the 18650 cell better.

![voltage-plot](https://i.imgur.com/eIuInXo.png)

esp-battery-logger
==================
Simple project for the esp8266 using the power saving features of the
chip to test the possibility of battery powered sensors. While
generating the voltage logs, the esp8266 wakes up every 30 seconds
doing basically nothing. Every 10 minutes, a WIFI-connection is made
and the voltage level is uploaded to a MQTT server.

![photo of setup 5](https://pbs.twimg.com/media/CTJ5OL5UwAACq52.jpg:large)

Setups
======
So far I have run this experiment using four different setups:

1. 2xAA connected directly to the esp8266 as shown in the image above.
   A network / server miss-hap consumed lots of energy due to the
   firmware not properly entering deep sleep when it failed to connect
   to the server.  The module died after less than a week. When I
   measured the batteries 12 hours after the module stopped uploading,
   they were completely drained.  Even if everything had worked as I
   hoped, I estimate that the batteries would have lasted less than a
   month. [photo of setup 1](https://pbs.twimg.com/media/B_P5WMMUoAMHy9B.jpg:large)

2. 2xAA with a [step-up converter](http://www.ebay.com/itm/mini-DC-DC-0-8-3-3V-to-DC-3-3V-Step-UP-Boost-Power-Module-For-Breadboard-Arduino-/281556288481?hash=item418e1003e1)
   This worked for 33 days, and the battery voltage read using a
   multimeter just after the esp8266 stopped working was just below
   2.6 V.

3. 3xAA connected directly to the esp8266 in a similar way as test 1.
   Connecting the batteries this way is out of spec for the esp8266,
   but in my experience it works.  This worked for 95 days!

4. 1x18650 [cheap E-bay Li-ion](http://www.ebay.com/itm/2x-UltraFire-18650-6000mAh-Li-Ion-Rechargeable-3-7V-Battery-For-Torch-Flashlight-/381196017618?hash=item58c10d9fd2).
   I messed up this test by using a not fully charged cell. It still
   worked for 17 days, but this is not very useful data.

5. 1x18650 [cheap E-bay Li-ion](http://www.ebay.com/itm/2x-UltraFire-18650-6000mAh-Li-Ion-Rechargeable-3-7V-Battery-For-Torch-Flashlight-/381196017618?hash=item58c10d9fd2).
   With a [charger & protection circuit](http://www.ebay.com/itm/5V-Micro-USB-1A-18650-Lithium-Battery-Charging-Board-Charger-Module-Protection-/400665520672?hash=item5d4986aa20).
   This time I made sure the cell was fully charged... After running
   the cell for two cycles, this setup lasts for about 40 days per
   charge.

All AA batteries used are regular alkaline batteries. Most of them
from [Kjell & Company](http://www.kjell.com/sortiment/el/batterier/alkaliska/kjell-company-aa-batterier-%28lr6%29-20-pack-p32301)

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
For some reason I have been having problems getting esp_mqtt to work
with the newer SDKs. Please try SDK version 1.0 if you have strange
boot hangs or issues.

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
