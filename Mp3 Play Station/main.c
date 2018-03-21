 #include "stm32f10x.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\LCD.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Mp3 Play Station\MP3Trigger.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Mp3 Play Station\PS2.h"
 #include <stdlib.h>
 #include <math.h>

int main(){
	
	//Volume of 0x80(128) is minimum, 0x00(0) is maximum.
	int volume = 40;
	uint16_t * controllerData;

	initMP3Trigger();
	initLCD();
	clockInit();
	led_IO_init();
	portClockInit();
	portInit();
	PS2Init();
	
	//Set some defaults for the Mp3 trigger.
	playTrack(3);
	delay(600);
	setVolume(volume);
	
	while(1){
		
		controllerData = pollController();
	
		if((controllerData[0] & PS2_X_BUTTON) == 0){
			startStop();
		}
		
		if((controllerData[0] & PS2_RIGHT_BUTTON) == 0){
			nextTrack();
		}
		
		if((controllerData[0] & PS2_LEFT_BUTTON) == 0){
			prevTrack();
		}
			
	hexToLCD((controllerData[0]), 0, 0);
	hexToLCD((controllerData[1] << 16) | (controllerData[2]), 1, 0);
	delay(6000);
	}
}
	
