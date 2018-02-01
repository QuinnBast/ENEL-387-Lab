/******************************************************************************
 * Name:    GPIO_lib.c
 * Description: GPIO Port functions
 * Version: V1.00
 * Author: Quinn Bast / Dave Duguid / Trevor Douglas
 *
 * This software is supplied "AS IS" without warranties of any kind.
 * Created using functions from lab1_lib as provided from ENSE 387 Lab 1
 *
 *
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *****************************************************************************/
 #include "stm32f10x.h"
 #include "GPIO_lib.h"

/*
* Name:         void clockInit()
* Paramaters:   none
* Description: This function will initialize the device internal
               clock to 24 Mhz
*/
void clockInit(void)
{
    uint32_t temp = 0x000;
    //If you hover over the RCC you can go to the definition and then
    //see it is a structure of all the RCC registers.  Then you can
    //simply assign a value.
    RCC->CFGR = 0x00050002;     // PLLMUL X3, PREDIV1 is PLL input

    RCC->CR =  0x01010081;      // Turn on PLL, HSE, HSI

    while (temp != 0x02000000)  // Wait for the PLL to stabilize
    {
        temp = RCC->CR & 0x02000000; //Check to see if the PLL lock bit is set
    }
}

/*
* Name:         void delay()
* Paramaters:   6000 = 1ms
* Description:  This function creates a delay
*/
void delay(uint32_t count)
{
	
    int i=0;
    for(i=0; i< count; ++i)
    {
    }
}

// SWITCH AND LED I/O FUNCTIONS

void led_IO_init (void)
{
    //Enable peripheral clocks for various ports and subsystems
    //Bit 4: Port C Bit3: Port B Bit 2: Port A
    RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPBEN
        | RCC_APB2ENR_IOPAEN ;

    //Set the config and mode bits for Port C bit 9 and 8 so they will
    // be push-pull outputs (up to 50 MHz)
    GPIOC->CRH |= GPIO_CRH_MODE9 | GPIO_CRH_MODE8 ;
    GPIOC->CRH &= ~GPIO_CRH_CNF9 & ~GPIO_CRH_CNF8 ;
}

/*************************************************
* This function will alternate the LEDs on the VLDiscovery board
**************************************************/
void led_wigwag (void)
{
    int i = 1200000;

    // Read the USER button and select the LED pattern according to its state
    if ((GPIOA->IDR & GPIO_IDR_IDR0) == 0)
    {
        GPIOC->BSRR = BLUE_ON | GREEN_OFF;
        delay(i);
        GPIOC->BSRR = GREEN_ON | BLUE_OFF;
        delay(i);
    }
    else
    {
        GPIOC->BSRR = BLUE_ON | GREEN_ON;
        delay(i);
        GPIOC->BSRR = GREEN_OFF | BLUE_OFF;
        delay(i);
    }
}

void portClockInit(){
		RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;	//Enable the clock for Port A IO
		RCC->APB2ENR |=  RCC_APB2ENR_IOPBEN;	//Enable the clock for Port B IO
		RCC->APB2ENR |=  RCC_APB2ENR_IOPCEN;	//Enable the clock for Port C IO
}
	
void portInit(){
	//Pins are default configured as general inputs.
	//No need to configure inputs.
	
	//Configure Port A Outputs
	GPIOA->CRH = GPIO_CRH_MODE9  | 	//Configure LEDs on A9
							 GPIO_CRH_MODE10 | 	//Configure LEDs on A10
							 GPIO_CRH_MODE11 | 	//Configure LEDs on A11
							 GPIO_CRH_MODE12 ;	//Configure LEDs on A12
}

int readSwitch(int sw){
	//sw indicates the value of the switch to read
	int value = 1;
	
	switch(sw){
		case 1:
			value = GPIOA->IDR & GPIO_IDR_IDR6;	//Return the value of SW1.1 (On PA6)
			break;
		case 2:
			value = GPIOA->IDR & GPIO_IDR_IDR7;	//Return the value of SW1.2 (on PA7)
			break;
		case 3:
			value = GPIOC->IDR & GPIO_IDR_IDR10;	//Return the value of SW1.3 (on PC10)
			break;
		case 4:
			value = GPIOC->IDR & GPIO_IDR_IDR11;	//Return the value of SW1.4 (on PC11)
			break;
		case 5:
			value = GPIOB->IDR & GPIO_IDR_IDR8;	//Return the value of SW2 (on PB8)
			break;
		case 6:
			value = GPIOB->IDR & GPIO_IDR_IDR9;	//Return the value of SW3 (on PB9)
			break;
		case 7:
			value = GPIOC->IDR & GPIO_IDR_IDR12;	//Return the value of SW4 (on PC12)
			break;
		case 8:
			value = GPIOA->IDR & GPIO_IDR_IDR5;	//Return the value of SW5 (on PA5)
			break;
		default:
			value = 1;	//By default say nothing is pressed.
			break;
	}
	//If the switch is on return 1, if the switch is off return 0
	//'1' = off by default
	return value >= 1 ? 0 : 1;
}

void setLED(int led, int value){
	//led is an interger of the switch we want to set
	//value is a 1 or 0 -- 1 for on, 0 for false
	
	switch(led){
		case 1:
			GPIOA->BSRR = value == 1 ? GPIO_BSRR_BR9 : GPIO_BSRR_BS9;	//Toggle PA9
			return;
		case 2:
			GPIOA->BSRR = value == 1 ? GPIO_BSRR_BR10 : GPIO_BSRR_BS10;	//Toggle PA10
			return;
		case 3:
			GPIOA->BSRR = value == 1 ? GPIO_BSRR_BR11 : GPIO_BSRR_BS11;	//Toggle PA11
			return;
		case 4:
			GPIOA->BSRR = value == 1 ? GPIO_BSRR_BR12 : GPIO_BSRR_BS12;	//Toggle PA12
			return;
	}
}
