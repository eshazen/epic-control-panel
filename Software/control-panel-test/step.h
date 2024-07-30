#ifndef _STEP_H_INCLUDED
#define _STEP_H_INCLUDED

#include <avr/io.h>
#include <util/delay.h>


//
// stepping motor library
//
void step_init();
void step_run( int dir, int steps);

#define STEP_PORT PORTB
#define STEP_DDR DDRB
#define STEP_DIR_BIT 4
#define STEP_STEP_BIT 3

#define STEP_MASK (_BV(STEP_DIR_BIT)|_BV(STEP_STEP_BIT))

#define STEP_DELAY 1

#define MICRO_STEPS 16

#endif
