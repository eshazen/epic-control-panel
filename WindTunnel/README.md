# Wind tunnel controller

This is a single (or possibly two) stepping motor drive control box for the wind tunnel.
Based on the SCOS control panel.

Some ideas for parts (requested Tasker to order, 7/30/24)

* Motor:  https://a.co/d/2aoKfqv
* Driver:  https://a.co/d/eidFa4V
* 12V brick:  https://a.co/d/8zxQC4y
* 5V regulator:  https://a.co/d/gvrwRQ0

## Box wiring 

### Power

Wire the 12V input jack as follows:

* Red
  * 5V module IN+
  * motor driver "VCC"
* Black
  * 5V module IN-
  * motor driver "GND"
  * control panel GND (J2 pin 6)
  
Wire the 5V regulator as follows:

* IN+ to red power jack wire
* IN- to black power jack wire
* OUT+ to control panel VCC (J2 pin 4)
* OUT- to control panel GND (J2 pin 6)

Using Eric's conrol panel... for now leave the UART port open,
so we need to use the SPI pins on the programming header (which is convenient
anyway)

Control panel wiring:

* Motor PUL+ - MOSI (J1 pin 4)
* Motor DIR+ - MISO (J1 pin 1)
* Limit SW - SCK (J1 pin 3)
* GND - J1 pin 6

So this would be a 6-pin ribbon cable, with stripped ends terminating
in screw terminals on the driver module.

The limit switch would use presumably the "NO" terminal to SCK
and the "C" (common) to GND.
