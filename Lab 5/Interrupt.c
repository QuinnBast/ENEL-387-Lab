/******************************************************************************
 * Name:    Interrupt.c
 * Description: Interrupt initilization and setup
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
 
  #include "stm32f10x.h"
	 
 void initSysTick(){

	 SysTick->CTRL =  0x00;				//Turn systick Off
	 SysTick->VAL = 0x00;					//Reset the timer
	 SysTick->LOAD = 12000000;	  //Set the interrupt to occur evert 1 second.
	 SysTick->CTRL = 0x07;				//Enable SysTick, Enable Interrupts, Use 24MHz clock.
	 
 }
 
 void initExternalInterrupt(int portNumber, int risingEdge){
	 
	 //Enable clock to the multiplexer
	 RCC->APB2ENR |=  RCC_APB2ENR_AFIOEN;
	 
	 //Enable a specific port.
	 //EXTICR[0] handles 0-3
	 //EXTICR[1] handles 4-7
	 //EXTICR[2] handles 8-11
	 //EXTICR[3] handles 12-15
	 
	 //Enable the specific port.
	 if(portNumber >=0 && portNumber <= 3){
			AFIO->EXTICR[0] &= ~AFIO_EXTICR1_EXTI0 << (portNumber * 4);
	 } else if(portNumber >=4 && portNumber <= 7){
		  AFIO->EXTICR[1] &= ~AFIO_EXTICR1_EXTI0 << ((portNumber - 4) * 4);
	 } else if (portNumber >=8 && portNumber <= 11){
		 AFIO->EXTICR[1] &= ~AFIO_EXTICR1_EXTI0 << ((portNumber - 8) * 4);
	 } else if (portNumber >=12 && portNumber <= 15){
		 AFIO->EXTICR[1] &= ~AFIO_EXTICR1_EXTI0 << ((portNumber - 12) * 4); 
	 }
	 
	 //Unmask the port that is required
	 EXTI->IMR |= EXTI_IMR_MR0 << portNumber;
	 
	 //Enable or disable falling edge detection
	 if(risingEdge){
		 EXTI->RTSR |= EXTI_RTSR_TR0 << portNumber;
	 } else {
		 EXTI->FTSR |= EXTI_FTSR_TR0 << portNumber;
	 }
	 
	 //Unmask the interrupt in the NVIC
	 switch(portNumber){
		 case 0:
				NVIC->ISER[0] |= NVIC_ISER_SETENA_6;
				break;
		 case 1:
			  NVIC->ISER[0] |= NVIC_ISER_SETENA_7;
				break;
		 case 2:
			  NVIC->ISER[0] |= NVIC_ISER_SETENA_8;
				break;
		 case 3:
			  NVIC->ISER[0] |= NVIC_ISER_SETENA_9;
				break;
		 case 4:
			  NVIC->ISER[0] |= NVIC_ISER_SETENA_10;
				break;
		 case 5:
		 case 6:
		 case 7:
		 case 8:
		 case 9:
			  NVIC->ISER[0] |= NVIC_ISER_SETENA_23;
				break;
		 case 10:
		 case 11:
		 case 12:
		 case 13:
		 case 14:
		 case 15:
			  NVIC->ISER[1] |= NVIC_ISER_SETENA_8;
				break;
	 }
 }
	 