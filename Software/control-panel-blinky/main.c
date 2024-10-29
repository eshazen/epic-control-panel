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
  led_init();
  
  lcd_cls();
  set_leds( 0xf);
  lcd_puts("HELLO!");
  _delay_ms(1000);

  while( 1) {
    set_leds( esc);
    esc++;
    _delay_ms(500);
  }
}
