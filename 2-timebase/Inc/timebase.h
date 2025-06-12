#ifndef __TIMEBASE_H__
#define __TIMEBASE_H__
#include"stm32f4xx.h"
#include<stdint.h>
#endif
extern volatile uint8_t timer_variable;
uint8_t getTick(void);
void delay(uint8_t delay);
void timebase_stop(void);
void timebase_init(void);
