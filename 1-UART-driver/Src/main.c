#include<stdio.h>
#include"uart.h"
int main(void){
	uart_tx_init();
	for(int i=0;i<100000;i++){
	printf("Hello from STM32\n");
	for(int i=0;i<10000;i++){}
	}
}
