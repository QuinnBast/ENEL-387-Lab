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
	int prevState[4] = {0};
	int state[4] = {0};
	char str[4] = "    ";
	uint16_t * controllerData;
	int counter = 0;
	float increaseRate = 0;

	initMP3Trigger();
	initLCD();
	clockInit();
	led_IO_init();
	portClockInit();
	portInit();
	PS2Init();
	
	//Set some defaults for the Mp3 trigger.
	playTrack(1);
	delay(600);
	setVolume(volume);
	
	while(1){
		
		for(int i=0; i<4; i++){
			prevState[i] = state[i];
		}
		controllerData = pollController();
	
		if((controllerData[0] & PS2_X_BUTTON) == 0 && prevState[0] == 0){
			startStop();
			state[0] = 1;
		} else if((controllerData[0] & PS2_X_BUTTON)){
			state[0] = 0;
		}
		
		if((controllerData[0] & PS2_RIGHT_BUTTON) == 0 && prevState[1] == 0){
			nextTrack();
			state[1] = 1;
		} else if ((controllerData[0] & PS2_RIGHT_BUTTON)){
			state[1] = 0;
		}
		
		if((controllerData[0] & PS2_LEFT_BUTTON) == 0 && prevState[2] == 0){
			prevTrack();
			state[2] = 1;
		} else if ((controllerData[0] & PS2_LEFT_BUTTON)){
			state[2] = 0;
		}
		
		//Handle Ananlog volume control.
		if((controllerData[2] & PS2_LEFT_ANALOG_VERTICAL) != PS2_LEFT_ANALOG_VERTICAL_DEFAULT){
			//If the analog value is not hoizontally centered, determine where the analog stick is.
			//Range from 0 to 255.
			uint8_t leftVertical = (controllerData[2] & PS2_LEFT_ANALOG_VERTICAL) >> PS2_LEFT_ANALOG_VERTICAL_SHIFT;
			
			//Create a range from -1 to 1 based on the input.
			increaseRate = 2 * (0.5 - (255/leftVertical));
			
			volume = volume - 3*(increaseRate/abs(increaseRate));
			
			//Set a boundary on the volume to prevent looping or exceeding the maximum
			if(volume > 255){
				volume = 255;
			}
			if(volume < 0){
				volume = 0;
			}
			
			setVolume(volume);
		}
			
	intToLCD(getTrackNumber(), 0, 0);
	stringToLCD(str, 0, 11);
	delay(6000);
	}
}

//Interrupt handler for USART3 Rx line.
void USART3_IRQHandler(void)
{
	//Read the data to clear the interrupt.
	uint8_t data = USART3->DR;
	char* str = "";
	
	//Determine what to do with the data
	if(data == 0x58){
		//The currently playing track has finished. Start the next track.
		nextTrack();
		str = "Stop";
	}
	
	if(data == 0x78){
		//The currently playing track was cancelled by a new command
		//Nothing required to be done.
		str = "Canc";
	}
	
	if(data == 0x45){
		//The requested track was not able to be found.
		str = "Err";
	}
	
	//For debugging purposes, display what data was sent.
	stringToLCD(str, 0, 11);
	return;
}
	
