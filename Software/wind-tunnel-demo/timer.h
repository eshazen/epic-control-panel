#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void timer_setup(void);
uint32_t get_millis(void);
uint16_t get_millis_u16(void);
uint8_t get_millis_u8(void);
void set_steps( uint16_t s);
void set_speed( uint8_t s);

#endif
