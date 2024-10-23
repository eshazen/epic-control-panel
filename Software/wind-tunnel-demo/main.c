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

#define NUM_MEMORY 4
static uint8_t memory_no ;
static uint32_t memory_pos[NUM_MEMORY];

void delay_mul( int count) {
  if( count)
    for( int i=0; i<count; i++)
      _delay_ms( 25);
}

static uint16_t ms;
static uint16_t ms_last;

int main (void)
{
  static uint8_t kb_code;
  static uint8_t kb_last;
  static uint8_t kb_time;
  static uint8_t kb_speed;

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

  set_speed( 20);
  set_steps( 0);
  step_dir( 0);

  // infinite event loop
  while( 1) {

    // wait for 1ms
    ms = get_millis_u16();
    if( ms != ms_last) {
      ms_last = ms;

      // poll KB and shaft encoder every 8 ms
      if( !(ms & 7)) {

	kb_code = read_kb_code();

	if( kb_code) {
	  if( kb_code == kb_last) {
	    if( kb_time  < 255) {
	      ++kb_time;
	      kb_speed = 4+ 63-(kb_time >> 2);
	      set_speed( kb_speed);
	    }
	  } else {
	    kb_time = 0;
	  }
	} else {
	  kb_time = 0;
	}

	kb_last = kb_code;

	// check for some buttons
	if( kb_code == 1) {     // run CW, speed up as pressed
	  step_dir(0);
	  set_steps(50);
	}

	if( kb_code == 2) {
	  step_dir(1);
	  set_steps(50);
	}

	// check/update keyboard and shaft encoder every 64 ms

	if( !(ms & 63)) {

	  this_shaft = get_shaft_count();
	  memory_no = this_shaft % NUM_MEMORY;

	  // update display every 1024 ms
	  if( !(ms & 1023) && !kb_code) {
	    lcd_addr( 0);
	    snprintf( buff, sizeof(buff), "k: %d ", this_shaft);
	    lcd_puts( buff);
	    lcd_addr( 40);
	    snprintf( buff, sizeof(buff), "ms: %u  ", ms);
	    lcd_puts( buff);
	  }
	}
      }
    }
  }
}


