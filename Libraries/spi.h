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
 **********
 *******************************************************************/
 #include "stm32f10x.h"
 #include <stdint.h>
 
 //initialize the SPI interface
 void spiInit(void);
 
 //send data over SPI 
 uint16_t spiTransfer(uint16_t);