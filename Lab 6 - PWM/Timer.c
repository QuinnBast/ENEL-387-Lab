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
	
	int dutyCompare = time * duty;
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;	//ENABLE the timer clock
	TIM1->CR1 |= TIM_CR1_CEN;						//ENABLE the timer
	TIM1->PSC = time;										//Set the prescaler to indicate the number of miliseconds.
	
	TIM1->CCER = TIM_CCER_CC1E;					//Allow capture compare register to capture the timer
	TIM1->CCMR1 = dutyCompare;					//Set the compare flag to go high after the duty cycle has completed.
	TIM1->CNT = 0;											//Set the value of the timer to 0
}

int readTimer(void){
	return TIM1->CCR1;
}