/******************************************************************************
 * Name:    LCD.h
 * Description: LCD display functions
 * Version: V1.00
 * Authors: Quinn Bast
 *
 *
 *
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *****************************************************************************/
  
 
 //Initialize the LCD function for writing
 void initLCD(void);
 
 //Send a command to the LCD
 void sendCommand(int);
 
 //Write a data command to the LCD
 void writeLCD(int);
 
 //Write a string to the LCD screen
 void stringToLCD(char*, int, int);
 
 //Scroll the LCD screen by 1 box with a given delay
 void scrollLCD(int, int, int);
 
 // A delay function that skips over the function if not enough time has passed so as to not freeze the whole system while waiting for a delay.
	int checkDelay(int);
	
	//Read the timer's value
	int readTimer(void);