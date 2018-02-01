/* 
ENEL 387 Lab 3
Author	-	Quinn Bast
Date 	-	January 31st, 2018
*/

#include "I:\ENEL-387-Lab\Libraries\GPIO_lib.h"
#include "LCD.h"

int main(){
  
	char topString[16] ="Quinn Bast";
	char lowString[16] = "200352973";
	char buttonStates[16] = "0000";
	
	clockInit();
	led_IO_init();
	portClockInit();
	portInit();
	initLCD();
	
	
	
	stringToLCD(topString, 0, 0);
	stringToLCD(lowString, 1, 0);
	
	while(1){
		
		int i;
		for(i=5; i<=8; i++){
			
			setLED(i-4, readSwitch(i));
			
			if(readSwitch(i) == 0){
				buttonStates[i-5] = '0';
			} else {
				buttonStates[i-5] = '1';
			}
		}
		scrollLCD(1, 1000*6000);
		stringToLCD(buttonStates, 0, 12);
		
	}
	
	return 0;
}