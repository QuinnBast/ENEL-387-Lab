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
 *****************************************************************************/
 
 #include "I:\ENEL 387\Libraries\GPIO_lib.h"
 #include "stm32f10x.h"
 
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
	 
	 delay(2.5*6000);
 }
 
  //Initialize the LCD function for writing
 void initLCD(){
	 
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
	 sendCommand(0x38);	//Wake
	 delay(5*6000);
	 sendCommand(0x38); //Wake
	 sendCommand(0x38); //Wake
	 sendCommand(0x38); //Enable 8bit
	 sendCommand(0x0F); //Display on
	 sendCommand(0x01); //Clear display
	 sendCommand(0x06); //Entry Mode Set
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
	 int topLine = 0xC0 + offset;
	 int bottomLine = 0x80 + offset;

	 
	 if(line == 1){
			sendCommand(topLine);
	 } else {
		 sendCommand(bottomLine);
	 }
	 
	 do{
		 writeLCD(deref);
		 ptr += 1;
		 deref = *ptr;
	 }while(deref != '\0');
 }