/*
 * demo code for stepping motor control on Eric's control board
 *
 * wiring:
 *  Motor STEP on MOSI / PB3
 *  Motor DIR on MISO / PB4
 *
 * Button 1 runs motor <revs> CW
 * Button 2 runs motor <revs> CCW
 * Button 3 increments <revs>
 * Button 4 decrements <revs>
 *
 * setup 1kHz timer interrupt
 * scan buttons every 8ms and queue hits
 * interrupt on shaft encoder rotation and update value
 * 
 */

// local display of data sent
// #define LOCAL_LCD

// display received UART data
#define UART_LCD

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

uint32_t ms;

int main (void)
{
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
  ms = get_millis();
  while( get_millis() - ms < 1000) /* delay 1s */
    ;
  lcd_cls();
  set_leds( 0);
  step_init();

  step_run( 0, MICRO_STEPS*50);
  step_run( 1, MICRO_STEPS*50);

  while( 1) {
    ms = get_millis();

    // poll KB and shaft encoder every 8 ms
    if( !(ms & 7)) {
      poll_kb();
      
      // check/update keyboard and shaft encoder every 64 ms
      if( !(ms & 63)) {

	int k = get_kb();
	if( k) {
	  // process button press
	  switch( k) {
	  case 1:
	    lcd_addr( 40);
	    lcd_puts( "Run CW ");
	    step_run( 0, MICRO_STEPS*200*revs);
	    lcd_cls();
	    break;
	  case 2:
	    lcd_addr( 40);
	    lcd_puts( "Run CCW");
	    step_run( 1, MICRO_STEPS*200*revs);
	    lcd_cls();
	    break;
	  case 3:
	    revs++;
	    lcd_addr( 40);
	    snprintf( buff, sizeof(buff), "%d Revs ", revs);
	    lcd_puts( buff);
	    break;
	  case 4:
	    if( revs > 1)
	      --revs;
	    lcd_addr( 40);
	    snprintf( buff, sizeof(buff), "%d Revs ", revs);
	    lcd_puts( buff);
	    break;
	  }
	}

	this_shaft = get_shaft_count();
	if( this_shaft) {
	  // process knob rotation
	}

      }
    }
  }
}


