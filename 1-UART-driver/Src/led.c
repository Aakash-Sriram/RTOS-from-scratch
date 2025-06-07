#include "led.h"

#define GPIOAEN        (1U<<0)// 0b 0000 0000 0000 0000 0000 0000 0000 0000 -> 0001
//sets all bits except bit no.1 to zero so if there are
//any other GPIO ports we depend on the state of them will be altered
///soooooo , use OR operator so the other ports in use stays on
void led_init(void){
	//Enable clock access to led port (port A)

	//RCC->AHB1ENR = GPIOAEN; bad
	RCC->AHB1ENR |=GPIOAEN; // Noice



	//Set led pin as output pin
	//Why ?
//	gpio has atleast 2 register -> direction reg and data reg
//	direction reg -> should the pin be input(accept v) or vice versa
//	data reg -> should the pin recieve data or vice versa
//nowadays there are 10+ reg like pull down , speed and other params
	GPIOA->MODER |= (1U<<10); //set bit 10 to 1
	GPIOA->MODER &=~(1U<<11);//set bit 11 to 0

}
void led_on(void){
//set led pin HIGH

	GPIOA->ODR|=(1U<<5);
}
void led_off(void){
//set led pin LOW
	GPIOA->ODR&=~(1U<<5);
}

