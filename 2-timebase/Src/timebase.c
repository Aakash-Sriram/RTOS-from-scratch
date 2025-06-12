#include"timebase.h"
#define ONE_SEC_LOAD 16000000//16mhz is the default for sysclock -> 16000000 cycle per sec
#define SysCTRL_ENABLE (1U<<0)
#define SysCTRL_TICKINT (1U<<1)
#define SysCTRL_CLCK_SOURCE (1U<<2)
#define SysCTRL_COUNTFLAG (1U<<16)

#define MAXX_DELAY 0xFFFFFFFFU
volatile uint8_t timer_variable =0;
volatile uint8_t freq = 2;
volatile uint8_t cur;
void increment(void){
	timer_variable+=freq;
}
uint8_t getTick(void){
	__disable_irq();
	cur = timer_variable;
	__enable_irq();
	return cur;
}
void delay(uint8_t delay){
	uint8_t tickstart = getTick();
	uint8_t wait = delay;
	if(wait<MAXX_DELAY){
		wait+=freq;
	}
	while((getTick()-tickstart)<wait){}
}
void SysTick_Handler(void){
	increment();
}
void timebase_stop(void){
	SysTick->CTRL &=~SysCTRL_ENABLE;
}

void timebase_init(void){
	/*
	 * reload timer with number of cycle per s
	*/
	SysTick->LOAD = ONE_SEC_LOAD -1;
	/*
	 * clear systick current value register
	 */
	SysTick->VAL =0;
	/* select internal clock source
	 */
	SysTick->CTRL = SysCTRL_CLCK_SOURCE;
	/* enable interupt
	 */
	SysTick->CTRL |= SysCTRL_TICKINT;
	/* enable systick*/
	SysTick->CTRL |= SysCTRL_ENABLE;
	/* enable global interupt
	 */

	__enable_irq();
}

