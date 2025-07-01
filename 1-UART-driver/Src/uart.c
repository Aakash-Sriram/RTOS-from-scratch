#include"uart.h"
static uint32_t uart_config_baudrate(uint32_t peripheral_clock, uint32_t baud);
static void  uart_set_baudrate(uint32_t peripheral_clock,uint32_t baud);
static void uart_write(int ch);
//default 16 megahertz for peripheral clock
#define SYS_FREQ 16000000U
#define APB1_CLOCK  SYS_FREQ
#define UART_BAUDRATE  115200
#define GPIOAEN  (1U<<0)
#define SR_RXNE (1U<<5)
#define SR_TXE (1U<<7)
#define UAR2EN    (1U<<17)
#define CR1_TE  (1U<<3)
#define CR1_UE (1U<<13)
#define USART_DATAREG_READY (1U<<5)
#define UART_RECIEVER_ENABLE (1U<<2)
#define UART_RECIEVER_WAKE (1U<<1)


int __io_putchar(int ch){
	uart_write(ch);
	return ch;
}
int __io_getchar(void){
	int ch = uart_read();
	return ch;
}
static int uart_read(void){
	while( !(USART2->SR & SR_RXNE) ){}
	return (USART2->DR & 0xFF);
}
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
	uart_set_baudrate(APB1_CLOCK,UART_BAUDRATE);

	USART2->CR1 = CR1_TE;//transmitter enable and set everything else to zero
	USART2->CR1 |=  CR1_UE;//enable USART

}

//static int uart_recieve(void){
//	USART2->CR1 |= UART_RECIEVER_WAKE;
//	USART2->CR1 |= UART_RECIEVER_ENABLE;
//	while( !(USART2->SR & SR_RXNE) ){}
//	return (USART2->DR & 0xFF);
//
//}


static void uart_write(int ch){
	/*
	 * make sure transmit data reg is empty
	 * write  transmit data registeer
	 * */
	while( !(USART2->SR & SR_TXE) ){}
	USART2->DR = (ch&0XFF);

}
static uint32_t uart_config_baudrate(uint32_t peripheral_clock, uint32_t baud) {
    return ( (peripheral_clock + (baud / 2U)) / baud );
}

static void uart_set_baudrate(uint32_t peripheral_clock, uint32_t baud) {
    USART2->BRR = uart_config_baudrate(peripheral_clock, baud);
}

