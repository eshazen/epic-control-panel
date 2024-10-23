// step_int.h - interrupt-driven stepper library
#ifndef _STEP_INT_H_INCLUDED
#define _STEP_INT_H_INCLUDED

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>


//-------------------- User configuration --------------------
// steps per revolution [must match driver switches!]
#define STEPS_PER_REV 800
// threads per inch [configure by user!]
#define STEPS_TPI 28
// maximum mm to move before timeout [configure by user!]
#define STEPS_MAX_MM 1000

// direction to move probe up (away from limit switch)
#define STEPS_DIR_UP 0


//-------------------- calculated values --------------------
// mm per rev
#define STEPS_MM_PER_REV (25.4/STEPS_TPI)
// steps per mm
#define STEPS_PER_MM (STEPS_PER_REV*STEPS_MM_PER_REV)
// maximum steps before limit
#define STEPS_TO_LIMIT (STEPS_PER_MM*STEPS_MAX_MM)

//-------------------- variables --------------------
static uint32_t motor_posn;	/* current position in steps */

void step_setup(void);		/* setup interrupts */

int step_home();		/* move to limit */
uint32_t step_pos();		/* get position in steps */
uint16_t step_pos_mm();		/* get position in mm*10 */
int step_go( uint32_t pos);    	/* go to absolute position in steps */
void step_stop();		/* stop now */

#endif
