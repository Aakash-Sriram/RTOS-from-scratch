#ifndef __UART_H_
#define __UART_H_
#include"stm32f4xx.h"
#include<stdio.h>
void uart_tx_init(void);
static  int uart_read(void);
//static int uart_recieve(void);
static void uart_write(int ch);
static uint32_t uart_config_baudrate(uint32_t peripheral_clock, uint32_t baud);
static void uart_set_baudrate(uint32_t peripheral_clock, uint32_t baud);
#endif

