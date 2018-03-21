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
 
 #define PS2_L2_BUTTON 0x0001
 #define PS2_R2_BUTTON 0x0002
 #define PS2_L1_BUTTON 0x0004
 #define PS2_R1_BUTTON 0x0008
 #define PS2_TRIANGLE_BUTTON 0x0010
 #define PS2_CIRCLE_BUTTON 0x0020
 #define PS2_X_BUTTON 0x0040
 #define PS2_SQUARE_BUTTON 0x0080
 #define PS2_SELECT_BUTTON 0x0100
 #define PS2_L3_BUTTON 0x0200
 #define PS2_R3_BUTTON 0x0400
 #define PS2_START_BUTTON 0x0800
 #define PS2_UP_BUTTON 0x1000
 #define PS2_RIGHT_BUTTON 0x2000
 #define PS2_DOWN_BUTTON 0x4000
 #define PS2_LEFT_BUTTON 0x8000
 
 //initialize the SPI interface
 void PS2Init(void);
 
 //send data over SPI 
 uint16_t* pollController();
 
 //Send and read data from the controller
 uint8_t readWrite(uint8_t);
 
 //Send data to the controller.
 void writeNoRead(uint8_t);