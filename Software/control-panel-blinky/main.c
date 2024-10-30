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

#include "step.h"

static char buff[16];
static int16_t this_shaft;
static uint8_t key;

void delay_mul( int count) {
  if( count)
    for( int i=0; i<count; i++)
      _delay_ms( 25);
}

uint32_t ms;
int main (void)
{
  kb_init();
  lcd_init();
  led_init();
  step_init();

  _delay_ms(10);
  
  lcd_cls();
  set_leds( 0xf);

  lcd_puts("KB / LED test");

  _delay_ms(1000);
  lcd_cls();

  while( 1) {

    key = -1;

    lcd_cls();
    lcd_puts("RUN CCW");
    step_run(1, 800);
    lcd_cls();
    lcd_puts("RUN CW ");
    step_run( 0, 800);
    lcd_cls();

    for( int i=0; i<10; i++) {
      for( uint8_t b=1; b<0x10; b<<=1) {
	if( read_kb() != key) {
	  snprintf( buff, sizeof(buff), "Keys: %d ", read_kb());
	  key = read_kb();
	  lcd_addr(0);
	  lcd_puts( buff);
	}
	set_leds( b);
	_delay_ms(100);
      }
    }

  }
}
