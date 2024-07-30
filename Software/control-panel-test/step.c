//
// very simple stepping motor library
//

#include "step.h"

void step_init() {
  STEP_DDR |= STEP_MASK;
}

void step_run( int dir, int nsteps) {
  if( dir)
    STEP_PORT &= ~_BV(STEP_DIR_BIT);
  else
    STEP_PORT |= _BV(STEP_DIR_BIT);
  while( nsteps--) {
    STEP_PORT |= _BV(STEP_STEP_BIT);
    _delay_ms( STEP_DELAY);
    STEP_PORT &= ~_BV(STEP_STEP_BIT);
  }
}
