//
// very simple stepping motor library
//

#include "step.h"

void step_init() {
  STEP_DDR |= STEP_MASK;
}

// set direction
void step_dir( int dir) {
  if( dir)
    STEP_PORT &= ~_BV(STEP_DIR_BIT);
  else
    STEP_PORT |= _BV(STEP_DIR_BIT);
}

// step once with 1us pulse
void step_once(void) {
  STEP_PORT |= _BV(STEP_STEP_BIT);
  _delay_us( 1);
  STEP_PORT &= ~_BV(STEP_STEP_BIT);
}

// run for nsteps in dir   (non-interrupt)
void step_run( int dir, int nsteps) {
  step_dir( dir);
  while( nsteps--) {
    step_once();
    _delay_ms( STEP_DELAY);
  }
}
