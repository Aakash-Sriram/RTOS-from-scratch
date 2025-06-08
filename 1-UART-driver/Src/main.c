#include"led.h"
int main(void){
	uart_tx_init();
	led_init();
	for(int i=0;i<100000;i++){
	printf("Hello from STM32");
	led_on();
	for(int i=0;i<10000;i++){}
	led_off();
	}
}
