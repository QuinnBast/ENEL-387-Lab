/******************************************************************************
 * Name:    lab1_lib.h
 * Description: STM32 peripherals initialization
 * Version: V1.00
 * Authors: Dave Duguid / Trevor Douglas
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 *
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *          V1.1 reformatted (kjn)
 *****************************************************************************/
#include <stdint.h>


//Bit masks for leds on STM32VLDiscovery board PC8 and PC9 using GPIOC_BSRR
// Green LED is on bit 9 of port C, Blue LED is on bit 8.
 #define GREEN_ON	0X00000200 //
 #define GREEN_OFF	0x02000000 //
 #define BLUE_ON	0x00000100 //
 #define BLUE_OFF	0x01000000 //


 //Functions available for public use

// Initialize the Cortex M3 clock using the RCC registers
  void clockInit(void);
// A general purpose countdown timer delay routine
  void delay(uint32_t delay);
// Initialize the clocks and IO pins for the LEDs and USER switch
  void led_IO_init (void);
// Flash some LEDs in a pattern determined by the state of the USER switch
  void led_wigwag(void);
