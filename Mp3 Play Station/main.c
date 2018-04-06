 #include "stm32f10x.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\LCD.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Mp3 Play Station\MP3Trigger.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Mp3 Play Station\PS2.h"
 #include <stdlib.h>
 #include <stdio.h>
 #include <math.h>
 #include <time.h>

	int volume = 40;
	int prevState[5] = {0};
	int state[5] = {0};
	int shuffle = 0;
	int repeat = 0;
	int play = 1;
	
	char * lcdState = "SongName";
	
	//I feel awful doing this but there was no way to get the track name from the MP3 Trigger.
	//If there was a way to get the track names from the MP3 Trigger I would have 100% used it.
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
	
	//Variable so that the LCD knows how much of the track has been printed to the screen.
	int currentTextPosition = 0;

	//Function declarations
	void printSongInformation();
	void updateSongInformation();
	void updateVolumeInformation(int);

int main(){
	
	//Volume of 0x80(128) is minimum, 0x00(0) is maximum.
	char str[4] = "    ";
	int counter = 0;

	//Initialization functions
	initMP3Trigger();	//Initialize the MP3 trigger over USART
	initLCD();				//initialize the LCD
	clockInit();			//Start GPIO clocks
	//led_IO_init();		//Start LED IOs
	portClockInit();	//Start the port clocks
	portInit();				//Initialize ports
	PS2Init();				//Enable SPI for the PS2 controller and wake up controller.
	
	srand(1234);   //Seed a random number
	
	//Set some defaults for the Mp3 trigger.
	playTrack(1);		//Start by playing the first track on the trigger
	delay(600);			
	setVolume(volume);	//Set the volume to a comfortable level.
	printSongInformation();	//Print the song information for the first song.
	
	//Variables that keep track of the LCD's current and previous location.
	int windowPosition = 0;
	int lastWindowPosition = 0;
	
	while(1){
		//Update the LCD's window position.
		lastWindowPosition = windowPosition;
		windowPosition = getWindowPosition();
			
		//Update the LCD timer.
		lcdUpdateTimer();
		
		//If the window has moved,
		//If the window is moving to the 1st index or the 20th index
		//(To the first box on each half of the screen)
		if((windowPosition == 1 || windowPosition == 20) && lastWindowPosition != windowPosition){
			//If half the screen has scrolled, update the song titles.
			updateSongInformation();
		}
			
		//Update the volume information.
		updateVolumeInformation(windowPosition);
		//Scroll the LCD
		scrollLCD(1, 1, 600);
	}
}

//Interrupt handler for USART3 Rx line. (MP3 Triggger)
void USART3_IRQHandler(void)
{
	//Read the data to clear the interrupt.
	uint8_t data = USART3->DR;
	
	//Determine what to do with the data.
	if(data == 0x58){
		//The currently playing track has finished. Start the next track.
		
		//If shuffle is on, play a random track.
		if(shuffle){
			clearLCD();
			playTrack(rand() % 34 + 1);
			resetTimer();
			setScreenPosition(0);
			printSongInformation();
		} else if(repeat){
			//If repeat is on, play the same track again.
			clearLCD();
			playTrack(getTrackNumber());
			resetTimer();
			setScreenPosition(0);
			printSongInformation();
		} else {
			//Otherwise, go to the next track.
			clearLCD();
			nextTrack();
			resetTimer();
			setScreenPosition(0);
			printSongInformation();
		}
	} else if(data == 0x78){
		//The currently playing track was cancelled by a new command
		//Nothing required to be done here, just a status update from the trigger.
	} else if(data == 0x45){
		//The requested track was not able to be found.
		//Nothing to do here, my code is correct so this will never be the case :)
	}

	return;
}

void SysTick_Handler(){
	//This interrupt occurs once every 1/60th of a second.
	//Use this interrupt to poll data from the PS2 controller.
	
	uint16_t * controllerData;
	float increaseRate = 0;
	
	//Update the button states
	for(int i=0; i<5; i++){
			prevState[i] = state[i];
		}
	
	//Get the controller data.
	controllerData = pollController();

	//If the falling edge of the X button is detected,
	if((controllerData[0] & PS2_X_BUTTON) == 0 && prevState[0] == 0){
		startStop();	//Start or stop the music.
		play = !play;	//Toggle the play status
		state[0] = 1;	//Debounce
	} else if((controllerData[0] & PS2_X_BUTTON)){
		state[0] = 0;	//Reset debounce
	}
	
	//If the R1 Buton is pressed,
	if((controllerData[0] & PS2_R1_BUTTON) == 0 && prevState[1] == 0){
		//Clear the LCD
		clearLCD();
		//If shuffle is enabled, go to a random song. Otherwise, play the next track.
		if(shuffle){
			playTrack(rand() % 34 + 1);
		} else {
		nextTrack();
		}
		
		resetTimer();	//Reset the LCD scroll timer
		setScreenPosition(0);	//SET the LCD to the left most position
		printSongInformation();	//Print the new song information
		state[1] = 1;
	} else if ((controllerData[0] & PS2_R1_BUTTON)){
		state[1] = 0;
	}
	
	//If L1 is pressed
	if((controllerData[0] & PS2_L1_BUTTON) == 0 && prevState[2] == 0){
		clearLCD();	//clear the LCD
		prevTrack(); //Play the previous track
		resetTimer(); //Reset the LCD scroll timer
		setScreenPosition(0);	//SET the LCD to the very left position
		printSongInformation();	//print the new song information
		state[2] = 1;
	} else if ((controllerData[0] & PS2_L1_BUTTON)){
		state[2] = 0;
	}
	
	//If the square button is pressed
	if((controllerData[0] & PS2_SQUARE_BUTTON) == 0 && prevState[3] == 0){
		shuffle = !shuffle; //Toggle the shuffle feature.
		repeat = 0;	//DISABLE the repeat feature.
		state[3] = 1;
	} else if((controllerData[0] & PS2_SQUARE_BUTTON)){
		state[3] = 0;
	}
	
	//If the circle button is pressed,
	if((controllerData[0] & PS2_CIRCLE_BUTTON) == 0 && prevState[4] == 0){
		repeat = !repeat;	//Toggle repeat feature
		shuffle = 0;	//Disable the shuffle feature
		state[4] = 1;
	} else if((controllerData[0] & PS2_CIRCLE_BUTTON)){
		state[4] = 0;
	}
	
	//Handle Ananlog volume control.
	if((controllerData[2] & PS2_LEFT_ANALOG_VERTICAL) != PS2_LEFT_ANALOG_VERTICAL_DEFAULT){
		
		//If the analog value is not hoizontally centered, determine where the analog stick is.
		//Range from 0 to 255.
		
		uint8_t leftVertical = (controllerData[2] & PS2_LEFT_ANALOG_VERTICAL) >> PS2_LEFT_ANALOG_VERTICAL_SHIFT;
		
		//Create a range from -1 to 1 based on the input.
		increaseRate = 2 * (0.5 - (255/leftVertical));
		
		//Adjust the volume using the ratio of the analog stick.
		volume = volume - (int)(1*(increaseRate/abs(increaseRate)));
		
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
	   //Format: (n/34) SongTitle - Artist ft. features
	  stringToLCD("(", 0, 0);	//Print the first (
		intToLCD(getTrackNumber(), 0, 1);	//Print the track number
		if(getTrackNumber() < 10){
			stringToLCD(" ", 0, 2);	//print a space if the track number is single digit.
		}
		stringToLCD("/34) ", 0, 3);	//Print total number of tracks.
		
		//Print the first 12 characters of the song title
		char *temp = (char*) malloc(13);
    strncpy(temp, songNames[getTrackNumber() - 1], 12);
		temp[13] = '\0';	//Terminate the string
		stringToLCD(temp, 0, 8);
		free(temp);	//free the variable that was allocated to store string information.
		currentTextPosition = 0;
}
void updateSongInformation(){
	//There are 40 characters on the LCD
	//Some song titles take more than 40 characters to display all the information
	//This makes displaying song information complicated.....
	//Commence the adventure:
	
	//LCD only shows 16 characters at a time but allows 40 characters.	
	//This gives a 4 character buffer to overwrite 20 characters (half the screen)
	//when they aren't shown.
	
	//Determine the length of the song title that we need to print to know if it has all been printed.
	int songLength = strlen(songNames[getTrackNumber() - 1]);
	
	//We are displaying the next portion of the song's title, increase counter.
	currentTextPosition += 1;
	
	//Determine if the whole song title has been printed.
	if(currentTextPosition * 20 <= (songLength + 8)){
		//If there is more information to print
		//Get the next 20 characters to print
		
		int startPos = 20 * currentTextPosition;	//Determine the portion of the string to start at
		char *temp = (char*) malloc(20);	//Allocate a string
    strncpy(temp, songNames[getTrackNumber() - 1]+startPos-8, 20);	//Copy 20 characters from the song title to the string.
		if(strlen(temp) < 20){
					//If the song title was less than 20 characters, fill the buffer with spaces.
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
		free(temp);	//free the memory used by the string!
		
	} else if(getWindowPosition() != 1) {
		//If we have printed all of the song information and the window is not currently in 0-20 index
		//Print the start of the song information
		printSongInformation();
	} else if (currentTextPosition > 2) {
		//If the window has printed all of the information, clear the remaining information while the screen loops
		stringToLCD("                    ", 0, 20);
	}
}

void updateVolumeInformation(int windowPos){
	//Print the status line
	//Format: Volume: nnn% S P
	
	//Because this information is persistant,
	//This information should not scroll.
	//This information should stay on the screen in the same location at all times.
	//However, the screen scrolls....
	//This makes things EVEN MORE DIFFICULT.
	//Commence the next adventure....
	
	//Setup the format for the string we want to display.
	char* volumeTag = "Volume: ";
	char *volumeString = (char*) malloc(4);
	int volumeRatio = (int)(100 - (volume/2.55));
	sprintf(volumeString, "%i", volumeRatio);	//Convert the volume integer to a string
	
	//Allocate space for the combined string.
	char *strings = (char*) malloc(16);
	//Concatenate all the information requried for the string
	strcat(strings, volumeTag);	//"Volume: "
	strcat(strings, volumeString);	// "nnn"
	strcat(strings, "%  ");	//"%  "
		
	if(shuffle){
		strcat(strings, "S");	//If shuffle is enabled, write an "S"
	} else if(repeat) {
		strcat(strings, "R");	//If repeat is enabled, write an "R"
	} else {
		strcat(strings, " ");	//Otherwise write nothing.
	}
	
	if(play){
		strcat(strings, "  ");	//If songs are playing, write nothing
	} else {
		strcat(strings, " P");	//If songs are paused, write a "P"
	}
	
	//Need to clear the window location as we move along.
	if(windowPos == 0){
	 stringToLCD("    ", 1, 37);
	} else {
	 stringToLCD(" ", 1, windowPos - 1);
	}
	
	
	//So.... if the window is scrolling along, there will be a point where the
	//window wraps around and starts to show some of the text from the beginning.
	
	//Let's check this condition.
	
	if(windowPos > 22){
		
	 //If the screen split is somewhere on the screen, we can't simply write all of the information
	 //on one line. Thus, we will need to split the string.
	 //It is still possible to write all of the first half on the left side
	 //But once the screen starts to loop at 0 we need to determine what part of the string
	 //Needs to be continued at that location.
	 //After 6 hours of debugging, this is the solution:
		
	 
	 char *secondHalf = (char*) malloc(16);	//Allocate memory for the second half of the string
	 //Copy the required characters to the second half of the string.
		
		//Start from (40 - windowPos)
		//End at (16) - (40 - windowPos)
	 strncpy(secondHalf, (strings + 40 - windowPos), 16 - (40 - windowPos));
		
		stringToLCD(strings, 1, windowPos); //Write the first string to the screen at the windows location
		stringToLCD(secondHalf, 1, 0);	//Write the second string at position 0
		free(secondHalf);	//free the memory!
	} else {
		//If the window isn't split, just write the string we need at the window location
		stringToLCD(strings, 1, windowPos);
	}
	
	//Free the memory used!
	free(volumeString);
	free(strings);
}