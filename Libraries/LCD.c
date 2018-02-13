/******************************************************************************
 * Name:    LCD.c
 * Description: LCD functions
 * Version: V1.00
 * Authors: Quinn Bast
 *
 *
 *
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 **********
 *******************************************************************/
  #include "I:\ENEL\ENEL-387-Lab\Libraries\GPIO_lib.h"
 //#include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "stm32f10x.h"
 #include <stdint.h>
 #include <math.h>
 
  //Global timer
 
struct Time{
 uint64_t lastTime;
 uint64_t thisTime;
} t;
  
 
 //LCD Initialize Commands
 
 //Enable 2 line LCD
 //Command 1 initialize
 #define LCD_INIT_8BIT 0x38 
 
 //LCD Display Init commands
 //Command 2 Initilize Options
 #define LCD_CMD_DISPLAY_ON_CURSOR_ON_BLINK_ON 0x0F 
 #define LCD_CMD_DISPLAY_ON_CURSOR_ON_BLINK_OFF 0x0E
 #define LCD_CMD_DISPLAY_ON_CURSOR_OFF 0x0C
 #define LCD_CMD_DISPLAY_OFF 0x08
 
 //Command 3 requirement
 #define LCD_CMD_DISPLAY_CLEAR 0x01
 
 //Command 4 options
 #define LCD_CMD_DISPLAY_INCREMENT_RIGHT_WINDOW_SHIFT 0x06
 #define LCD_CMD_DISPLAY_INCREMENT_RIGHT_NO_SHIFT 0x05
 #define LCD_CMD_DISPLAY_INCREMENT_LEFT_WINDOW_SHIFT 0x04
 #define LCD_CMD_DISPLAY_INCREMENT_LEFT_NO_SHIFT 0x03
 
 
 //LCD Scroll Window commands
 #define LCD_CMD_DISPLAY_SCROLL_WINDOW_RIGHT 0x1B
 #define LCD_CMD_DISPLAY_SCROLL_WINDOW_LEFT 0x18
 
  void sendCommand(int data){
	 //8 bits
	 //DataFormat: DB7-DB0
	 
	 // RS	-		B0
	 // RW	-		B5
	 // E		- 	B1
	 
	 //RS and RW will always be 0, want E high
	 GPIOB->BSRR = 0x00210002;	//Set E high
	 
	 //Load data to the output
	 GPIOC->ODR &= 0xFF00;	//Reset lower bits.
	 GPIOC->ODR |= data;		//Set data on output.
	 
	 delay(8000);
	 
	 //set E low
	 GPIOB->BSRR = 0x00230000;	//SET E low
	 
	 delay(1.5*6000);
 }
 
  //Initialize the LCD function for writing
 void initLCD(){
	 
	 initTimer();
	 
	 //Enable clocks for ports B anc C
	 RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;	//Enable the clock for Port B IO
	 RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;	//Enable the clock for Port C IO
	 
	 //Configure port B pins as outputs
	 GPIOB->CRL |= GPIO_CRL_MODE0 | 
								 GPIO_CRL_MODE1 |
								 GPIO_CRL_MODE5;
	 
	 GPIOB->CRL &= ~GPIO_CRL_CNF0 &
								 ~GPIO_CRL_CNF1 &
								 ~GPIO_CRL_CNF5;
	 	 
	 //Configure Port C pins as outputs
	 GPIOC->CRL |= GPIO_CRL_MODE0 | 
								 GPIO_CRL_MODE1 |
								 GPIO_CRL_MODE2 |
								 GPIO_CRL_MODE3 |
								 GPIO_CRL_MODE4 |
								 GPIO_CRL_MODE5 |
								 GPIO_CRL_MODE6 |
								 GPIO_CRL_MODE7;
								 
	 GPIOC->CRL &= ~GPIO_CRL_CNF0 &
								 ~GPIO_CRL_CNF1 &
								 ~GPIO_CRL_CNF2 &
								 ~GPIO_CRL_CNF3 &
								 ~GPIO_CRL_CNF4 &
								 ~GPIO_CRL_CNF5 &
								 ~GPIO_CRL_CNF6 &
								 ~GPIO_CRL_CNF7;
	 
	 //Wait 15ms for warmup
	 delay(20*6000);
	 
	 //Send commands to wake up and configure
	 sendCommand(LCD_INIT_8BIT);	//Wake
	 delay(5*6000);
	 sendCommand(LCD_INIT_8BIT); //Wake
	 sendCommand(LCD_INIT_8BIT); //Wake
	 sendCommand(LCD_INIT_8BIT); //Enable 8bit
	 sendCommand(LCD_CMD_DISPLAY_ON_CURSOR_OFF); //Display on
	 sendCommand(LCD_CMD_DISPLAY_CLEAR); //Clear display
	 sendCommand(LCD_CMD_DISPLAY_INCREMENT_RIGHT_WINDOW_SHIFT); //Entry Mode Set
 }
 
 //Write a data command to the LCD
 void writeLCD(int data){
	 	 //8 bits
	 //DataFormat: DB7-DB0
	 
	 // RS	-		B0
	 // RW	-		B5
	 // E		- 	B1
	 
	 //RS and RW will always be 0, want E high
	 GPIOB->BSRR = 0x00200003;	//Set E high
	 
	 //Load data to the output
	 GPIOC->ODR &= 0xFF00;	//Reset lower bits.
	 GPIOC->ODR |= data;		//Set data on output.
	 
	 delay(8000);
	 
	 //set E low
	 GPIOB->BSRR = 0x00220001;	//SET E low
	 
	 delay(2.5*6000);
 }
 
 //Write a string to the LCD screen
 void stringToLCD(char* string, int line, int offset){
	 
	 //Set cursor to LCD position
	 char* ptr = string;
	 int deref = *ptr;
	 int position = (line == 1) ? 0xC0 + offset : 0x80 + offset;

	 
	 sendCommand(position);
	 
	 do{
		 writeLCD(deref);
		 ptr += 1;
		 deref = *ptr;
	 }while(deref != '\0');
 }
 
 void hexToLCD(uint32_t hexValue, int line, int offset){
	 int i = 0;
	 uint32_t mask = 0x0000000F;
	 uint32_t tempHex;
	 int position = (line == 1) ? 0xC0 + offset : 0x80 + offset;
	 
	 sendCommand(position);
	 //Write 0x in front of value
	 writeLCD(0x30);	
	 writeLCD(0x78);
	 
	  for(i=0; i<8 ; i++){			
			//Shift hex value all the way to the right.
		 tempHex = (hexValue >> (7 - i)*4) & mask;
		 if(tempHex >= 0 && tempHex <= 9){
			 tempHex = tempHex + 0x30;
		 } else {
			 tempHex = tempHex + 0x37;
		 }
		 writeLCD(tempHex);
	 }
 }
 
  void intToLCD(int intValue, int line, int offset){
	 int i = 0;
	 int digit;
	 int divisor = 10000;
	 int position = (line == 1) ? 0xC0 + offset : 0x80 + offset;
	 
	  for(i=0; i<4 ; i++){			
			//Get digits in order and print.
		 digit = (intValue / divisor) % 10;
		 intValue = intValue - (digit * divisor);
		 digit = digit + 0x30;
		 writeLCD(digit);
		 divisor = divisor / 10;
	 }
 }
	
 int checkDelay(int delay){
	//Delay in milliseconds
	
	int difference;
	int rollover;	
	
	if(t.lastTime > t.thisTime){
		//The timer rolled over, we need to calculate the difference
		rollover = 0x0000FFFF - t.lastTime;
		difference = rollover + t.thisTime;
	} else {
		difference = t.thisTime - t.lastTime;
	}
	
	//if the delay is going to roll over next cycle, return true.
	if(((t.thisTime % delay) > delay * 0.75) && ((t.thisTime + difference) % delay) < delay * 0.25){
		return 1;
	} else {
		return 0;
	}
}
 
 
 //Scroll the LCD by one box
 int scrollLCD(int direction, int boxes, int delay){
	 int scrolls = 0;
	 
	 if(checkDelay(delay) == 1){
		 if(direction == 1){
			 while(scrolls < boxes){
				sendCommand(LCD_CMD_DISPLAY_SCROLL_WINDOW_RIGHT);
				 scrolls = scrolls + 1;
			 }
		 } else {
			 while(scrolls < boxes){
				sendCommand(LCD_CMD_DISPLAY_SCROLL_WINDOW_LEFT);
				 scrolls = scrolls + 1;
			 }
		 }
		 return 1;
	 } else {
		 return 0;
	 }
 }
 
 void initTimer(void){
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//ENABLE the timer clock
	TIM1->CR1 |= 0x1;										//ENABLE the timer
	TIM1->PSC = 0x00005DC0;							//Set the prescaler to be 24,000 resulting in one timer tick per millisecond.
	TIM1->CNT = 0;											//Set the value of the timer to 0
}

int readTimer(void){
	return TIM1->CNT;
}

void updateTimer(){
	t.lastTime = t.thisTime;
	t.thisTime = readTimer();
}