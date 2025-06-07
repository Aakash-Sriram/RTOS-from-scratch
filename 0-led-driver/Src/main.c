#include"led.h"
int main(void){
	led_init();
	for(int i=0;i<100000;i++){
		led_on();
		for(int j=0;j<1000000;j++){}
		lef_off();
		for(int j=0;j<1000000;j++){}

	}
}
