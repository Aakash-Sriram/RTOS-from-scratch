#include "timebase.h"
#include<stdio.h>

int main(void){

	printf("%d\n",timer_variable);
	timebase_init();
	int l = 10;
	while(l>0){
		printf("%d\n",timer_variable);
		l--;
	}
}

