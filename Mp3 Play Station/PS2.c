/******************************************************************************
 * Name:    spi.c
 * Description: SPI functions
 * Version: V1.00
 * Authors: Quinn Bast
 *
 *	Notes:
 *	Brown = Data (to Micro) = To PB14 and pull-up resistor from 5V
 *	Red = Power = 5V
 *	Orange = Command (from micro) To PB15
 *	Yellow = Chip Select = PB12
 *	Green = Attention = Resistor to 5V
 *	Blue	=	Clock = PB13
 *	Black = GND
 *
 *
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 **********
 *******************************************************************/
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\LCD.h"
 #include "stm32f10x.h"
 #include <stdint.h>
 #include <math.h>
 #include <assert.h>


  uint8_t readWrite(uint8_t data){
	 
	 uint8_t ret = 0x00;
	 
		//Wait until data can be sent.
		while((SPI2->SR & SPI_SR_TXE) == 0){
		}
		
		SPI2->DR = data;
		
		//Can read?
		while((SPI2->SR & SPI_SR_RXNE) == 0){
		}
		
		ret = SPI2->DR;
		return ret;
 }
 
 void PS2Init(){
	 	 uint8_t ret = 0x00;
	 
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;		//Enable the clock for the SPI interface
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;		//Enable the clock for Port B IO
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;		//Enable alternate function clock
	 
	  //Set PB15 and PB 13 to alternate function outputs.
    GPIOB->CRH |= GPIO_CRH_CNF15_1 | GPIO_CRH_CNF13_1 | GPIO_CRH_MODE13 | GPIO_CRH_MODE15 ;
    GPIOB->CRH &= ~GPIO_CRH_CNF15_0 & ~GPIO_CRH_CNF13_0 ;
	 
		//Set PB14 to floating input (default state, no change required)
	  GPIOB->CRH &= ~GPIO_CRH_MODE14 & ~GPIO_CRH_CNF14_0;
	  GPIOB->CRH |= GPIO_CRH_CNF14_1;
	 
		//Set PB12 to GPIO out
		GPIOB->CRH |= GPIO_CRH_MODE12;
	  GPIOB->CRH &= ~GPIO_CRH_CNF12;
	 
		//Make PB12 high so no data is being sent over SPI on init.
		GPIOB->ODR |= GPIO_ODR_ODR12;
	 
	 
		//Init SPI.
		SPI2->CR1 &= ~SPI_CR1_DFF;	//Set (0=)8 bit or (1=)16 bit. (PS2 controller uses 8 bit)
		SPI2->CR1 |= SPI_CR1_SSM;	  //Disable setting the NSS pin
		SPI2->CR1 |= SPI_CR1_SSI;		//Disable setting the NSS pin
		SPI2->CR2 &= ~SPI_CR2_SSOE;	//Disable setting the NSS pin
		SPI2->CR1 |= SPI_CR1_MSTR;	//This device is the master
		
		//Device specific configurations.
		SPI2->CR1 |= SPI_CR1_CPHA;	//Second clock edge (high->low) is the data transfer.
		SPI2->CR1 |= SPI_CR1_CPOL;	//Clock high when idle
		SPI2->CR1 |= SPI_CR1_LSBFIRST;	//LSB sent in first.
		SPI2->CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0;	//Set the baud rate to 125KHz (24MHz/x = 125KHz) x = 192. Next power of 2 is 256 = 0x110
		
		//Enable the SPI interface
		SPI2->CR1 |= SPI_CR1_SPE;
		
		delay(600*15);
		
	
		//Wake the device by sending 3 short polls.
	for(int i=0; i<3; i++)
	{
		GPIOB->BSRR = GPIO_BSRR_BR12;	//Attention LOW
		readWrite(0x01);
		readWrite(0x42);
		readWrite(0x00);
		readWrite(0x00);
		readWrite(0x00);
		GPIOB->BSRR = GPIO_BSRR_BS12;	//Attention HIGH
	}
	
	//Enter Config Mode
	GPIOB->BSRR = GPIO_BSRR_BR12;	//Attention LOW
	readWrite(0x01);
	readWrite(0x43);
	readWrite(0x00);
	readWrite(0x01);
	readWrite(0x00);
	GPIOB->BSRR = GPIO_BSRR_BS12;	//Attention HIGH
	
	
	//Enable Analog Controls
	GPIOB->BSRR = GPIO_BSRR_BR12;	//Attention LOW
	readWrite(0x01);
	readWrite(0x44);
	readWrite(0x00);
	readWrite(0x01);
	readWrite(0x03);
	readWrite(0x00);
	readWrite(0x00);
	readWrite(0x00);
	readWrite(0x00);
	GPIOB->BSRR = GPIO_BSRR_BS12;	//Attention HIGH
	
	
	//Exit Config Mode
	GPIOB->BSRR = GPIO_BSRR_BR12;	//Attention LOW
	readWrite(0x01);
	readWrite(0x43);
	readWrite(0x00);
	readWrite(0x00);
	readWrite(0x5A);
	readWrite(0x5A);
	readWrite(0x5A);
	readWrite(0x5A);
	readWrite(0x5A);
	GPIOB->BSRR = GPIO_BSRR_BS12;	//Attention HIGH
	
}
 
 uint16_t* pollController(){
	 static uint16_t ret[3] = {0};
	 ret[0] = 0x0000;
	 ret[1] = 0x0000;
	 ret[2] = 0x0000;
	 uint8_t command[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	 
	 GPIOB->BSRR = GPIO_BSRR_BR12;	//Attention LOW
	 readWrite(command[0]);
	 readWrite(command[1]);
	 readWrite(command[2]);
	 ret[0] |= readWrite(command[3]) << 8;
	 ret[0] |= readWrite(command[4]);
	 ret[1] |= readWrite(command[5]) << 8;
	 ret[1] |= readWrite(command[6]);
	 ret[2] |= readWrite(command[7]) << 8;
	 ret[2] |= readWrite(command[8]);
	 
	 GPIOB->BSRR = GPIO_BSRR_BS12;	//Attention HIGH
	 return ret;
 }