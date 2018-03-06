/* 
ENEL 387 Lab 3
Author	-	Quinn Bast
Date 	-	January 31st, 2018
*/

 #include "I:\ENEL\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "I:\ENEL\ENEL-387-Lab\Libraries\LCD.h"
 #include "I:\ENEL\ENEL-387-Lab\Libraries\ADC.h"
 #include "I:\ENEL\ENEL-387-Lab\Lab 6 - PWM\Timer.h"
 #include "stm32f10x.h"
 //#include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 //#include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\LCD.h"
	#include <stdlib.h>
	#include <math.h>

int main(){
	
	clockInit();
	led_IO_init();
	portClockInit();
	portInit();
	initLCD();
	initTimer(24000, 0.5);
	
	while(1){
		int i;
		
		for(i=5; i<=8; i++){
			setLED(i-4, readSwitch(i));
		}  
		
		hexToLCD(readTimer(), 1, 0);
		
		//scrollLCD(1, 20, 2500);
	}
	
	return 0;
}