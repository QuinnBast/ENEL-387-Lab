/******************************************************************************
 * Name:    Timer.c
 * Description: Timer functions
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
 
void initTimer(int time, float duty){
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//ENABLE the timer clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	//Enable A IO Ports
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	//Enable AFIO
	
	//Set PA8 to be alternate function output to read the TIM1_CH1.
	GPIOA->CRH &= GPIO_CRH_CNF8_1 & ~GPIO_CRH_CNF8_0 & GPIO_CRH_MODE8 ;
	
	TIM1->CR1 |= TIM_CR1_CEN;						//ENABLE the timer
	TIM1->CR2 |= TIM_CR2_OIS1;					//Set idle states 'high'
	TIM1->EGR |= TIM_EGR_UG;						//Reset the counter when it has completed counting
	
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_2;	//Set the compare register to be in PWM mode.
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE | TIM_CCMR1_OC1FE;		//Preload each update. Compare each update.
	
	TIM1->CCER |= TIM_CCER_CC1E;				//Output signal to the cooresponding pin
	
	TIM1->PSC = time - 1;								//Set the prescaler
	
																			//Defines the period of the waveform.
																			//Setting this to 1000 to further scale down the prescaler and 
				
	TIM1->ARR = 1000;										//Set the auto-reload resigter if a countdown is required.
	TIM1->CCR1 = 1000 * duty;					  //Set the compare flag to go high after the duty cycle has completed.																		//Set the compare value to the duty cycle.
	
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSI;
																			//Become Australian.
																			//Enable the outputs, force to their idle state on enable.
																			
	TIM1->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
																			//Tell the timer the auto-reloader should be used, enable the timer.
	
	TIM1->CNT = 0;											//Set the value of the timer to 0
}

int readTimer(void){
	return TIM1->CNT;
}