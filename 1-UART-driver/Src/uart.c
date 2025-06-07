#include"uart.h"

#define GPIOAEN        (1U<<0)

void uart_tx_init(void){
	//USART - universal synchronus asycnronus reciever transmitter but only async is only mostly used
	//so hence UART

	/*
	 * Enable clock access to GPIOA
	 * Set PA2 mode to alternate function mode
	 * Set alternate function type to AF7
	 */

	RCC->AHB1ENR |= GPIOAEN;

	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &=~ (1U<<4);


	GPIOA->AFR[0] &=~(1U<<11);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<8);


	/*
	 * Enable clock access to USART2
	 * configure baud rate
	 * configure transfer direction
	 *Enable the module
	 */
}

