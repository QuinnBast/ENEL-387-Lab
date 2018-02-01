/* 
ENEL 387 Lab 1
Author	-	Quinn Bast
Date 	-	January 17th, 2018
*/

#include "lab1_lib.h"

int main(){

	clockInit();
	led_IO_init();
	while(1){
		led_wigwag();
	}
	return 0;
}