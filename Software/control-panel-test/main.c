/*
 * demo code for SCOS control panel
 *
 * setup 1kHz timer interrupt
 * scan buttons every 8ms and queue hits
 * interrupt on shaft encoder rotation and update value
 * 
 * send to USART and 9600 baud:
 *   e <count>         on encoder change
 *   k <code>          on key press
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

static char buff[16];
static int16_t this_shaft;
static uint8_t esc;

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

  lcd_puts("SCOS Control V0.1");
  ms = get_millis();
  while( get_millis() - ms < 1000)
    ;
  lcd_cls();
  set_leds( 0);

  while( 1) {
    ms = get_millis();

#ifdef UART_LCD
    if( USART0CharacterAvailable()) {
      uint8_t ch = USART0ReceiveByte();
      if( esc) {
	set_leds( ch);
	esc = 0;
      } else {
	if( ch == '\001')
	  lcd_cls();
	else if( ch == '\002')
	  lcd_addr(40);
	else if( ch == '\033')
	  esc = 1;
	else if( ch == '\003')
	  lcd_addr(0);
	else if( isprint( ch))
	  lcd_putc( ch);
      }
    }
#endif    

    // poll KB and shaft encoder every 8 ms
    if( !(ms & 7)) {
      poll_kb();
      
      // check/update keyboard and shaft encoder every 64 ms
      if( !(ms & 63)) {

	int k = get_kb();
	if( k) {
	  snprintf( buff, sizeof(buff), "k %d\n", k);
#ifdef LOCAL_LCD
	  lcd_addr(0);
	  lcd_puts( buff);
#endif
	  USART0SendString( buff);
	}

	this_shaft = get_shaft_count();
	if( this_shaft) {
	  snprintf( buff, sizeof(buff), "e %d\n", this_shaft);
	  clear_shaft();
#ifdef LOCAL_LCD
	  lcd_addr(40);
	  lcd_puts( buff);
#endif
	  USART0SendString( buff);
	}

      }
    }
  }
}


