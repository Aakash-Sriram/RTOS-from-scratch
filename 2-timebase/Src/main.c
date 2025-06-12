#include "timebase.h"
#include "uart.h"
#include "led.h"

int main(void){
	led_init();
	uart_tx_init();
	timebase_init();
	while(1){
		delay(5);
		printf("this is printing every 5 seconds\n\r");
	}
	return 0;
}

