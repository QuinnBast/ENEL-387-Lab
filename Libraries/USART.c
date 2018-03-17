/******************************************************************************
 * Name:    USART.c
 * Description: USART functions
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

void initUSART(){
	
	//ALTERNATE FUNCTION CONFIGURATION
	
	//The USART3 Alternate function Is default mapped to PB10 (Tx) and PB11 (Rx). No need to remap.
	//No configuration of the alternate functions
	
	
	//PIN CONFIGURATION
	
	//For USART to be enabled:
	//Tx Full Duplex : 							GPIO_CNF = Alternate function push-pull
	//Tx Half Duplex Synchronous : 	GPIO_CNF = Alternate function push-pull
	
	//Rx Full Duplex : 							GPIO_CNF = Floating Input / Input pull-up
	//Rx Half Duplex Synchronous : 	GPIO_CNF = Not used.
	
	//Enable USART3 Clock
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
	
	//Enable GPIOB Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	//PB10
	GPIOB->CRH |= GPIO_CRH_MODE10; 	//Set PB10(Tx) to output mode (11)
	GPIOB->CRH |= GPIO_CRH_CNF10_1;	//Set PB10(Tx) alternate function push-pull (10)
	GPIOB->CRH &= ~GPIO_CRH_CNF10_0; 	
	
	//PB11
	GPIOB->CRH |= GPIO_CRH_MODE11_0;	//Set PB11(Rx) to be a floating input (01)
	GPIOB->CRH &= ~GPIO_CRH_MODE11_1; 
	
	//USART CONFIGURATION
	//PB10 and 11 use USART3 registers
	USART3->CR2 &= ~USART_CR2_STOP;				// Configure the number of stop bits
																				// 00 = 1 stop bit
																				// 01 = 0.5 stop bits
																				// 10 = 2 stop bits
																				// 11 = 1.5 Stop Bits
																				
	USART3->CR1 &= ~USART_CR1_PCE;				//Disable parity
	USART3->CR1 &= ~USART_CR1_M;					//1 start bit, 8 data bits, n stop bits
	
											
	// Set the buad rate. The LSB is the DIV_Fraction, upper 3 bytes are DIV_Mantissa
	// Baud = Fclk/(16*USARTDIV)
	// USARTDIV = USART_Mantissa + (USART_Fraction/16)
	
	//If clock is 24KHz
	// 9600 = 24kHz/(16*USARTDIV); USARTDIV = 156.25
	// 156 = 0x9C
	// 0.25 = x/16; x = 4 = 4 = 0x4
	USART3->BRR = 0x9C4;
	USART3->CR1 |= USART_CR1_TE;	//Enable transmitter.
	USART3->CR1 |= USART_CR1_RE;	//Enable reciever.
	
	USART3->CR1 |= USART_CR1_UE;	//Enable USART
}

void sendUSART(uint8_t data){
	
 	while((USART3->SR & USART_SR_TXE) != USART_SR_TXE){
	}
	
	USART3->DR = data;
	
	//Wait until TC (Transmission complete)!
	while((USART3->SR & USART_SR_TC) != USART_SR_TC){
	}
}

uint8_t readUSART(){
	uint8_t data;
	if((USART3->SR & USART_SR_RXNE) == USART_SR_RXNE){
		data = USART3->DR;
		return data;
	} else return 0;
}