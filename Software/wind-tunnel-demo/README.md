# Wind tunnel control code

## Debug commits

* eb7472d (HEAD) UP/DOWN for test; working on step_int
  <br>This one is working with two bottons for up/down

## Wiring

    MOSI to DIR
	SCK to PUL
	MISO to limit switch
	
	ENA tied on (maybe use a pin from serial connector for this?

## UI Specification

4 buttons:  UP, DOWN, SAVE, RECALL

Shaft knob cycles between 4 saved positions

