#include "step_int.h"
//
// interrupt-driver stepping motor code
// N.B. configured for 8MHz system clock
//

// for now, interrupt and step at 1kHz

volatile uint16_t steps_to_go = 0;
volatile uint8_t ticks_per_step = 8;
volatile uint8_t ticks_this_step = 0;

volatile uint8_t ticks = 0;
volatile uint32_t millis_value = 0;

//------------------------------------------------------------------------------
// setup timer 0 to interrupt at about 8kHz
void step_setup(void){

  TCCR0A = (1 << WGM01);	/* set CTC mode so stop at OCR0A */
  
  // enable Timer0+1 overflow interrupt:
  TIMSK0 = (1 << OCIE0A);

  // Set CS00 + 01 bits so timer runs at clock speed/64:
  TCCR0B |= (1 << CS01) | (1 << CS00);
  // N.B. this is 8-bit value for timer 0
  OCR0A = 125;		/* 8MHz / 64 / 125 = 1kHz */
  // OCR0A = 14;			/* 8MHz / 64 / 15 = 8kHz */
}

//------------------------------------------------------------------------------
uint8_t get_millis_u8(void) {
  return millis_value;
}
			     
uint16_t get_millis_u16(void) {
  return millis_value;
}

//------------------------------------------------------------------------------
uint32_t get_millis(void){
  return millis_value;
}

#ifdef INTERRUPT_STEPPER
void set_steps( uint16_t s) {
  steps_to_go = s;
}

void set_speed( uint8_t s) {
  ticks_per_step = s;
}
#endif

//------------------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
  ticks++;
  if( (ticks & 7) == 0)
    millis_value++;
#ifdef INTERRUPT_STEPPER
  if( steps_to_go) {
    if( ticks_this_step)
      --ticks_this_step;
    else {
      ticks_this_step = ticks_per_step;
      STEP_PORT |= _BV(STEP_STEP_BIT);
      _delay_us( 1);
      STEP_PORT &= ~_BV(STEP_STEP_BIT);
      --steps_to_go;
    }
  }
#endif
}
