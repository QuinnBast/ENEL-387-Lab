/* 
ENEL 387 Lab 3
Author	-	Quinn Bast
Date 	-	January 31st, 2018
*/

 #include "I:\ENEL\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "I:\ENEL\ENEL-387-Lab\Libraries\LCD.h"
 #include "I:\ENEL\ENEL-387-Lab\Lab 4\ADC.h"
 //#include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 //#include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\LCD.h"
	#include <stdlib.h>
	#include <math.h>

int main(){
  
	uint32_t analogVoltageValue = 0;
	uint32_t analogTemperatureValue = 0;
	
	clockInit();
	led_IO_init();
	portClockInit();
	portInit();
	initLCD();
	ADCInit();
	
	while(1){
		int i;
		//Update the timer's values each loop.
		lcdUpdateTimer();
		
		for(i=5; i<=8; i++){
			setLED(i-4, readSwitch(i));
		}
		
			analogVoltageValue = readADC(2);
			hexToLCD(analogVoltageValue, 0, 0);
		
			decimalToLCD(analogVoltageValue * 8, 1, 0, 2);
		  writeLCD(0x56);
		
			analogTemperatureValue = readADC(1);
			hexToLCD(analogTemperatureValue, 0, 20);
		
			decimalToLCD(analogTemperatureValue * 8, 1, 20, 2);
		  writeLCD(216+7);
			writeLCD(0x43);
		  
		
			scrollLCD(1, 20, 2500);
}
	
	return 0;
}