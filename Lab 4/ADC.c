/******************************************************************************
 * Name:    ADC.c
 * Description: Analog Digital Converter
 * Version: V1.00
 * Authors: Quinn Bast
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 *
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *****************************************************************************/
 
 #include "stm32f10x.h"
 #include "I:\ENEL\ENEL-387-Lab\Lab 4\ADC.h"
  
 void ADCInit(void){
	 
	  //Enable peripheral clocks for ADC and Port A Pins
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN ;
	 
	 //Configure Port A1 and Port A2 as Analog Inputs	 
	 GPIOA->CRL &= ~GPIO_CRL_CNF1 &	//PA1 as analog input
								 ~GPIO_CRL_CNF2;	//PA2 as analog input
	 
	 //Enable and calirate the ADC
	 ADC1->CR2 |= (ADC_CR2_ADON) | (ADC_CR2_CAL);
	 
	 //Select the maximum sampling rate for PA1 and PA2
	 ADC1->SMPR2 |= (ADC_SMPR2_SMP1) | (ADC_SMPR2_SMP2);
 }
 
 uint32_t readADC(int channel){
	 
		//Set the channel
		ADC1->SQR3 = (ADC_SQR3_SQ1 & channel);
	 
		//Tell the ADC to take a reading
		ADC1->CR2 &= (ADC_CR2_ADON);
		
	 while(!(ADC1->SR & ADC_SR_EOC)) {
	 }
	 
	 return ADC1->DR & ADC_DR_DATA;
 }