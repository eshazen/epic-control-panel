/*
 * Wind tunnel control code
 *
 *
 *  Motor STEP on MOSI / PB3
 *  Motor DIR on MISO / PB4
 *
 * Button 1 (upper left)  - UP
 * Button 2 (upper right) - DOWN
 * Button 3 (lower left)  - SAVE
 * Button 4 (lower right) - RECALL
 *
 * setup 1kHz timer interrupt
 * scan buttons every 8ms and queue hits
 * interrupt on shaft encoder rotation and update value
 * 
 */

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <ctype.h>

#include "lcd.h"
#include "shaft.h"
#include "timer.h"
#include "kb.h"
#include "uart.h"
#include "led.h"
#include "step.h"

static char buff[16];
static int16_t this_shaft;
static uint8_t esc;
static uint16_t revs = 1;

void delay_mul( int count) {
  if( count)
    for( int i=0; i<count; i++)
      _delay_ms( 25);
}

static uint16_t ms;
static uint16_t ms_last;

int main (void)
{
  uint8_t kb_code;

  lcd_init();
  shaft_init();
  timer_setup();
  kb_init();
  USART0Init();
  led_init();
  
  sei();

  lcd_cls();
  set_leds( 0xf);

  ENC_LED_DDR |= ENC_LED_MASK;
  ENC_LED_PORT |= ENC_LED_MASK;

  lcd_puts("Wind Tunnel 0.1");
  ms = get_millis_u16();
  while( get_millis_u16() - ms < 1000) /* delay 1s */
    ;
  lcd_cls();
  set_leds( 0);
  step_init();

  // infinite event loop
  while( 1) {

    // wait for 1ms
    ms = get_millis_u16();
    if( ms != ms_last) {
      ms_last = ms;

      // check/update running motor
      if( kb_code == 1) {
	step_dir(1);
	step_once();
      }

      if( kb_code == 2) {
	step_dir(0);
	step_once();
      }

      // poll KB and shaft encoder every 8 ms
      if( !(ms & 7)) {

	kb_code = read_kb_code();

	// check/update keyboard and shaft encoder every 64 ms
	if( !(ms & 63)) {

	  this_shaft = get_shaft_count();
	  if( this_shaft) {
	    // process knob rotation
	  }

	  // update display every 1024 ms
	  if( !(ms & 1023)) {
	    lcd_addr( 0);
	    snprintf( buff, sizeof(buff), "kb: %d  ", kb_code);
	    lcd_puts( buff);
	    lcd_addr( 40);
	    snprintf( buff, sizeof(buff), "ms: %d  ", ms);
	    lcd_puts( buff);
	  }
	}
      }
    }
  }
}


