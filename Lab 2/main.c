/* 
ENEL 387 Lab 2
Author	-	Quinn Bast
Date 	-	January 24th, 2018
*/

#include "GPIO_lib.h"

int main(){

	clockInit();
	led_IO_init();
	portClockInit();
	portInit();
	
	while(1){
		int i;
		for(i=5; i<=8; i++){
				setLED(i-4, readSwitch(i));
		}
	}
	
	return 0;
}