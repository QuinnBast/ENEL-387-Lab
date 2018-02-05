/* 
ENEL 387 Lab 3
Author	-	Quinn Bast
Date 	-	January 31st, 2018
*/

 //#include "I:\ENEL-387-Lab\Libraries\GPIO_lib.h"
 //#include "LCD.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
  #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\LCD.h"
	#include <stdlib.h>

int main(){
  
	char topString[16] ="Quinn Bast";
	char lowString[16] = "200352973";
	char side2[16] = "Second Side!";
	char buttonStates[16] = "0000";
	int counter = 0;
	char timer[16] = "";
	
	clockInit();
	led_IO_init();
	portClockInit();
	portInit();
	initLCD();
	
	
	
	stringToLCD(topString, 0, 0);
	stringToLCD(lowString, 1, 0);
	stringToLCD(side2, 9, 20);
	
	while(1){
		int i;
		
		//Update the timer's values each loop.
		updateTimer();
		for(i=5; i<=8; i++){
			
			setLED(i-4, readSwitch(i));
			
			if(readSwitch(i) == 0){
				buttonStates[i-5] = '0';
			} else {
				buttonStates[i-5] = '1';
			}
		}
		
		if(checkDelay(1000) == 1){
			counter = counter + 1;
		}
		timer[4] = (counter % 10) + 48;
		timer[3] = ((counter % 60) / 10) + 48;
		timer[2] = ':';
		timer[1] = (counter / 60) + 48;
		timer[0] = ((counter /60) / 100) + 48;
		
		stringToLCD(timer, 1, 20);
		scrollLCD(1, 20, 2500);
		stringToLCD(buttonStates, 0, 12);
	}
	
	return 0;
}