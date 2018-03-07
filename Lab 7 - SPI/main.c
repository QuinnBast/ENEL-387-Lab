/* 
ENEL 387 Lab 3
Author	-	Quinn Bast
Date 	-	January 31st, 2018
*/

 #include "I:\ENEL\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "I:\ENEL\ENEL-387-Lab\Libraries\LCD.h"
 #include "I:\ENEL\ENEL-387-Lab\Lab 7 - SPI\spi.h"
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
	spiInit();
	
	while(1){
		
		if(spiTransfer(0xF1D2) == 0xF1D2){
			setLED(1, 1);
			setLED(2, 0);
			setLED(3, 1);
			setLED(4, 0);
		} else {
			setLED(1, 0);
			setLED(2, 0);
			setLED(3, 0);
			setLED(4, 0);
		}
		delay(6000);
		//hexToLCD(readSpiData(), 1, 0);
				
	}
	
	return 0;
}