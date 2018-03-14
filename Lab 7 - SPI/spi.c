/******************************************************************************
 * Name:    spi.c
 * Description: SPI functions
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
 
 void spiInit(){
	 
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;		//Enable the clock for the SPI interface
	 
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;		//Enable the clock for Port B IO
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;		//Enable alternate function clock
	 
	  //Set PB15 and PB 13 to alternate function outputs.
    GPIOB->CRH |= GPIO_CRH_CNF15_1 | GPIO_CRH_CNF13_1 | GPIO_CRH_MODE13  |GPIO_CRH_MODE15 ;
    GPIOB->CRH &= ~GPIO_CRH_CNF15_0 & ~GPIO_CRH_CNF13_0 ;
	 
		//Set PB14 to floating input (default state, no change required)
		//Set PB12 to GPIO out
		GPIOB->CRH |= GPIO_CRH_MODE12;
	 
		//Make PB12 high so no data is being sent over SPI on init.
		GPIOB->ODR = GPIO_ODR_ODR12;
	 
	 
		//Init SPI.
		SPI2->CR1 &= ~SPI_CR1_DFF;	//Set (0=)8 bit or (1=)16 bit. (PS2 controller uses 8 bit)
		SPI2->CR1 |= SPI_CR1_SSM;	  //Disable setting the NSS pin
		SPI2->CR1 |= SPI_CR1_SSI;		//Disable setting the NSS pin
		SPI2->CR2 &= ~SPI_CR2_SSOE;	//Disable setting the NSS pin
		SPI2->CR1 |= SPI_CR1_MSTR;	//This device is the master
		
		//Device specific configurations.
		SPI2->CR1 &= ~SPI_CR1_CPHA;	//Second clock edge is the data transfer.
		SPI2->CR1 &= ~SPI_CR1_CPOL;	//Clock low when idle
		SPI2->CR1 &= ~SPI_CR1_LSBFIRST;	//MSB sent in first.
		SPI2->CR1 |= SPI_CR1_BR_1 | SPI_CR1_BR_0;	//Set the baud rate to 5MHz (24MHz/4.8 = 5) best power of 2 to 4.8 is 8 = 3
		
		//Enable the SPI interface
		SPI2->CR1 |= SPI_CR1_SPE;
 }
 
  //send data over SPI 
 uint16_t spiTransfer(uint16_t data){
	 
	  uint16_t ret = 0x0000;
	 
		GPIOB->BSRR = GPIO_BSRR_BR12;	//Toggle PB12
	 
		//Wait until data can be sent.
		while((SPI2->SR & SPI_SR_TXE) == 0){
		}
		
		//If data can be transmitted, send data
		if((SPI2->CR1 & SPI_CR1_LSBFIRST) > 0){
			//If the data is being sent LSB First,
			//put the lower 8 bits on the line.
			SPI2->DR = data;
		} else {
			//If data is being sent MSB first,
			//put upper 8 bits on the line.
			SPI2->DR = data >> 8;
		}
		
		//Is transmitted?
		while((SPI2->SR & SPI_SR_RXNE) == 0){
		}
		
		//If data can be transmitted, send data
		if((SPI2->CR1 & SPI_CR1_LSBFIRST) > 0){
			//If the data is being sent LSB First,
			//read the data into the first 8 bits of the return
			ret |= SPI2->DR;
		} else {
			//If data is being sent MSB first,
			//read the data to the upper 8 bits of the return
			ret |= SPI2->DR << 8;
		}
		
		//Wait until data can be sent.
		while((SPI2->SR & SPI_SR_TXE) == 0){
		}
		
		//If data can be transmitted, send data
		if((SPI2->CR1 & SPI_CR1_LSBFIRST) > 0){
			//If the data is being sent LSB First,
			//put the upper 8 bits on the line.
			SPI2->DR = data >> 8;
		} else {
			//If data is being sent MSB first,
			//put lower 8 bits on the line.
			SPI2->DR = data;
		}
		
		//Is transmitted?
		while((SPI2->SR & SPI_SR_RXNE) == 0){
		}
		
				//If data can be transmitted, send data
		if((SPI2->CR1 & SPI_CR1_LSBFIRST) > 0){
			//If the data is being sent LSB First,
			//read the data into the first 8 bits of the return
			ret |= SPI2->DR << 8;
		} else {
			//If data is being sent MSB first,
			//read the data to the upper 8 bits of the return
			ret |= SPI2->DR;
		}
		
		GPIOB->BSRR = GPIO_BSRR_BS12;	//Toggle PB12
		
		return ret;
 }