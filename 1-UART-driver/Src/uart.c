#include"uart.h"
static void  uart_config_baudrate(uint32_t peripheral_clock,uint32_t baud);
static void  uart_set_baudrate(uint32_t peripheral_clock,uint32_t baud);
#define GPIOAEN  (1U<<0)
#define UAR2EN    (1U<<17)
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

	RCC->APB1ENR |= UAR2EN;
}
static void  uart_set_baudrate(uint32_t peripheral_clock,uint32_t baud){
	USART2_>BRR = uart_config_baudrate(peripheral_clock,baud);
}
static void  uart_config_baudrate(uint32_t peripheral_clock,uint32_t baud){
	(peripheral_clock + ( (baud/2U) )/baud);

}
