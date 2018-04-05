 #include "stm32f10x.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\LCD.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Mp3 Play Station\MP3Trigger.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Mp3 Play Station\PS2.h"
 #include <stdlib.h>
 #include <stdio.h>
 #include <math.h>

	int volume = 40;
	int prevState[4] = {0};
	int state[4] = {0};
	
	char * lcdState = "SongName";
	char* songNames[] = { 
	"Allstar - Smashmouth",  
	"Oh Lately It's So Quiet - Ok Go", 
	"I Like To Dance - Hot Chelle Rae",  
	"Big Bang Theory Theme", 
	"Crazy Train - Ozzy Osbourne", 
	"Killer Queen - Queen", 
	"10,000 Emerald Pools - Borns", 
	"Dancing on Quicksand - Bad Suns", 
	"Dangerous - Big Data", 
	"Cheap Sunglasses - RAC ft. Matthew Koma", 
	"Dazzle - Oh Wonder", 
	"heart - flor", 
	"Genghis Khan - Miike Snow", 
	"I Think I Like You - SIREN", 
	"Kaleidoscope - Fenech Soler", 
	"Banner - Lights", 
	"Love Never Felt So Good - Micheal Jackson, Justin Timberlake",
	"Night by Night - Chromeo",
	"A Million Miles Away - FM Attack",
	"16 Years - The Griswolds",
	"Always - Great Good Fine OK",
	"Arcadia - The Kite String Tangle",
	"Black River - Wolf Gang",
	"Gun - CVRCHES",
	"Sun Shy - Dresses",
	"ILYSB - LANY",
	"Heart is Full - Miike Snow",
	"One Minute More - Capital Cities",
	"My Type - Saint Motel",
	"Closer - The Chainsmokers ft. Halsey",
	"Your Body Is A Weapon - The Wombats",
	"Uma - Panama Wedding",
	"Vice - POP ETC",
	"Shut Up and Dance - Walk The Moon",
	"East of Eden - Zella Day"};
	int currentTextPosition = 0;

	void printSongInformation();
	void updateSongInformation();
	void updateVolumeInformation(int);

int main(){
	
	//Volume of 0x80(128) is minimum, 0x00(0) is maximum.
	char str[4] = "    ";
	int counter = 0;

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
	printSongInformation();
	
	int windowPosition = 0;
	int lastWindowPosition = 0;
	
	while(1){
		lastWindowPosition = windowPosition;
		windowPosition = getWindowPosition();
			
		lcdUpdateTimer();
		if((windowPosition == 1 || windowPosition == 20) && lastWindowPosition != windowPosition){
			//If half the screen has scrolled, update the song titles.
			updateSongInformation();
		}
			
		updateVolumeInformation(windowPosition);
		scrollLCD(1, 1, 600);
	}
}

//Interrupt handler for USART3 Rx line.
void USART3_IRQHandler(void)
{
	//Read the data to clear the interrupt.
	uint8_t data = USART3->DR;
	
	//Determine what to do with the data
	if(data == 0x58){
		//The currently playing track has finished. Start the next track.
			clearLCD();
			nextTrack();
			resetTimer();
			setScreenPosition(0);
			printSongInformation();
	} else if(data == 0x78){
		//The currently playing track was cancelled by a new command
		//Nothing required to be done.
	} else if(data == 0x45){
		//The requested track was not able to be found.
	}

	return;
}

void SysTick_Handler(){
	//This interrupt occurs once every 1/60th of a second.
	uint16_t * controllerData;
	float increaseRate = 0;
	
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
			clearLCD();
			nextTrack();
			resetTimer();
			setScreenPosition(0);
			printSongInformation();
			state[1] = 1;
		} else if ((controllerData[0] & PS2_RIGHT_BUTTON)){
			state[1] = 0;
		}
		
		if((controllerData[0] & PS2_LEFT_BUTTON) == 0 && prevState[2] == 0){
			clearLCD();
			prevTrack();
			resetTimer();
			setScreenPosition(0);
			printSongInformation();
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
			
			volume = volume - (int)(3*(increaseRate/abs(increaseRate)));
			
			//Set a boundary on the volume to prevent looping or exceeding the maximum
			if(volume > 255){
				volume = 255;
			}
			if(volume < 0){
				volume = 0;
			}
			setVolume(volume);
		}
}

void printSongInformation(){
	  stringToLCD("(", 0, 0);
		intToLCD(getTrackNumber(), 0, 1);
		if(getTrackNumber() < 10){
			stringToLCD(" ", 0, 2);
		}
		stringToLCD("/34) ", 0, 3);
		
		//Print the first 12 characters
		char *temp = (char*) malloc(13);
    strncpy(temp, songNames[getTrackNumber() - 1], 12);
		temp[13] = '\0';
		stringToLCD(temp, 0, 8);
		free(temp);
		currentTextPosition = 0;
}
void updateSongInformation(){
	//There are 40 characters per line
	//The LCD only shows 16 characters at a time.
	
	//This gives a 4 character buffer to overwrite 20 characters when they aren't shown.
	//Determine the length of the song title that we need to print to know if it has all been printed.
	int songLength = strlen(songNames[getTrackNumber() - 1]);
	
	currentTextPosition += 1;
	//Determine what the next set of 16 characters should print
	if(currentTextPosition * 20 <= (songLength + 8)){
		//Get the next 16 characters to print
		int startPos = 20 * currentTextPosition;
		char *temp = (char*) malloc(20);
    strncpy(temp, songNames[getTrackNumber() - 1]+startPos-8, 20);
		if(strlen(temp) < 20){
			    int len = strlen(temp);
					memset(temp+len, ' ', 20 - len);   
					temp[20] = '\0';
		}
		//Determine where the window is.
		if(getWindowPosition() == 1){
			//Print to the right side
			stringToLCD(temp, 0, 20);
		} else {
			//Print to the left side.
			stringToLCD(temp, 0, 0);
		}
		free(temp);
	} else if(getWindowPosition() != 1) {
		//Print the start of the song information
		printSongInformation();
	} else if (currentTextPosition > 2) {
		stringToLCD("                    ", 0, 20);
	}
}
void updateVolumeInformation(int windowPos){
	
		//Combine the string into one string.
	char* volumeTag = "Volume: ";
	char *volumeString = (char*) malloc(4);
	int volumeRatio = (int)(100 - (volume/2.55));
	sprintf(volumeString, "%i", volumeRatio);
	
	char *firstHalf = (char*) malloc(12);
	char *secondHalf = (char*) malloc(12);
	char *strings = (char*) malloc(13);
	strcat(strings, volumeTag);
	strcat(strings, volumeString);
	strcat(strings, "%");
	
	//Need to clear the window location as we move along.
	if(windowPos == 0){
	 stringToLCD(" ", 1, 38);
	} else {
	 stringToLCD(" ", 1, windowPos - 1);
	}
	
	if(windowPos > 27){
	 strncpy(secondHalf, (strings + 40 - (windowPos - 1) - 1), 12 - (40 - windowPos));
		
		stringToLCD(strings, 1, windowPos);
		stringToLCD(secondHalf, 1, 0);
		if(volumeRatio != 100){
		 stringToLCD(" ", 1, 12 - (40 - windowPos) - 1);
		}
	} else {
		stringToLCD(strings, 1, windowPos);
		if(volumeRatio != 100){
		 stringToLCD(" ", 1, windowPos + 11);
		}
	}
	
	//Determine where to write the string based on the current windowPos.
	//If windowPos > (40 - 12), the volume will need to be split up.
	
	free(volumeString);
	free(strings);
	free(firstHalf);
	free(secondHalf);
}