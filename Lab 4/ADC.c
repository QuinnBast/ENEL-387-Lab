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
 #include "I:\ENSE\ENSE374Labs\ENEL-387-Lab\Lab 4\ADC.h"
 
 struct AdcReader{
	 int queued;
	 uint32_t lastValue;
 } ar;
 
 void ADCInit(void){
		ar.lastValue = 0;
	  ar.queued = 0;
	 
	  //Enable peripheral clocks for ADC Port A Pins
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPAEN ;
	 
	 //Configure Port A1 and Port A2 as Analog Inputs
	 GPIOA->CRL &= ~GPIO_CRL_CNF1 &	//PA1 as analog input
								 ~GPIO_CRL_CNF2;	//PA2 as analog input
	 
	 //Enable and calirate the ADC
	 ADC1->CR2 |= (ADC_CR2_ADON) | (ADC_CR2_CAL);
	 
	 //Select the maximum sampling rate for PA1 and PA2
	 ADC1->SMPR2 |= (ADC_SMPR2_SMP1 & 0x7) | (ADC_SMPR2_SMP2 & 0x7);
	 
	 //Set the sequence of channels to sample channel one
	 ADC1->SQR3 = (ADC_SQR3_SQ1 & 0x1);
 }
 
 void readADC(){
	 //If there is not a reading queued, queue a reading
	 if(ar.queued == 0){
		 	//Tell the ADC to take a reading
			ADC1->CR2 |= (ADC_CR2_ADON);
			ar.queued = 1;
	 } else if((ADC1->SR & ADC_SR_EOC) > 0) {
			//If a reading has come back, return the reading
		 ar.lastValue = ADC1->DR;
		 ar.queued = 0;
	 }
 }
 
 uint32_t readADCtoHex(){
	 readADC();
	 return ar.lastValue;
 }
 
 char* readADCtoString(){
	 char reading[16] = "0x00000";
	 char *pointer = reading;
	 uint32_t mask = 0x00000F00;
	 int value = 0;
	 int i=7;
	 
	 readADC();
	 
	 for(i=7; i<10 ; i++){
		 value = mask & ar.lastValue;
		 value = (value >> (9 - i)*4);
		 if(value >= 0 && value <= 9){
			 reading[i] = value + 0x30;
		 } else {
			 reading[i] = value + 0x37;
		 }
		 mask = mask >> 4;
	 }
	 return pointer;
 }