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

## Software

Wired per above to stepping motor and controller.  Wrote a simple test
program, which runs fine.  Also found that there is an easy patch to
Arduino to run on a "breadboard" 8MHz ATMega328, so in principle the
Arduino IDE could be used.

### UI option 1

In absence of any input from the customer, we propose the following
feature list, which would be implemented as a menu selected using the
knob.

* HOME - run motor until limit switch triggers.  Stop after timeout to
  prevent runaways.
* GO - go to displayed position
* SAVE - save displayed position in a memory
* RECALL - restore displayed position from a memory

The total run is less than 1m so a display in mm formatted as ddd.d
should be fine.  The display could show something like this on two lines:

    C 999.9 N 999.9
    HOME GO SAV RCL
1
"C" is current position.  "N" is new position.
<br>The knob would cycle between the four bottom menu items and the
"new" position.  Click the knob to select.  When the position is
selected, adjust the position with the knob.

A label next to the display can explain.

### UI option 2

Two buttons would be dedicated to UP and DOWN and would always move
the motor.  Speed would gradually increase as the button is held.
Taps of the button would move by one unit (unit t.b.d.).

Two other buttons would implement SAVE and RECALL.
Memory positions (e.g. 1..4) would be selected using the knob.

If home position is not known, the display would show e.g.

    PRESS RECALL TO
    CALIBRATE HOME
	
Otherwise normally the display would show:

    ****************
    Cur pos: 999.9 
      Pos n: 999.9
    ****************

Where the "n" would change as the knob is rotated.
If a stored position is unknown, "???.?" would display.
