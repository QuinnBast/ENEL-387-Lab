/******************************************************************************
 * Name:    PS2.h
 * Description: PS2 Controller functions
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
 void PS2Init(void);
 
 //send data over SPI 
 uint8_t* pollController();
 
 //Send and read data from the controller
 uint8_t readWrite(uint8_t);
 
 //Send data to the controller.
 void writeNoRead(uint8_t);