//
// single timer interrupt to count milliseconds
// simple interrupt-driven stepping motor code included
//

#define INTERRUPT_STEPPER

#ifdef INTERRUPT_STEPPER
#include "step.h"
volatile uint16_t steps_to_go = 0;
#endif

#include "timer.h"

volatile uint32_t millis_value = 0;

//------------------------------------------------------------------------------
void timer_setup(void){

  TCCR0A = (1 << WGM01);	/* set CTC mode so stop at OCR0A */
  
  // enable Timer0+1 overflow interrupt:
  TIMSK0 = (1 << OCIE0A);

  // Set CS00 + 01 bits so timer runs at clock speed/64:
  TCCR0B |= (1 << CS01) | (1 << CS00);
  // N.B. this is 8-bit value for timer 0
  OCR0A = 125;			/* 8MHz / 64 / 125 = 1kHz */
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
#endif

//------------------------------------------------------------------------------
ISR(TIMER0_COMPA_vect)
{
  millis_value++;
#ifdef INTERRUPT_STEPPER
  if( steps_to_go) {
    STEP_PORT |= _BV(STEP_STEP_BIT);
    _delay_us( 1);
    STEP_PORT &= ~_BV(STEP_STEP_BIT);
    --steps_to_go;
  }
#endif
}

