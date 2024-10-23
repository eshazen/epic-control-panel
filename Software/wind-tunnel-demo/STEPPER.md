# Interrupt-driver stepper library

Minimal functions:

    step_home()       // run until limit sw hit or timeout
	step_pos()        // current position in steps
	step_pos_mm()     // current position in mm
	step_go( pos)     // go to position
	step_stop()       // stop immediately at current position

Implementation notes:

Keep track of position in steps as `uint32`.

Accelerate and decelerate per preset constants.

