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
 
 //Write a decimal to the LCD screen
 void intToLCD(int, int, int);
 
  //Write a hex value to the LCD screen
 void hexToLCD(int, int, int);
 
 //Scroll the LCD screen by 1 box with a given delay
 void scrollLCD(int, int, int);
 
 //Check updates
 int checkDelay(int);
 
 //Update timer
 void updateTimer(void);