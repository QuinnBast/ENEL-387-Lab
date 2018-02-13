/******************************************************************************
 * Name:    ADC.h
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
 #include <stdint.h>
 
 void ADCInit(void);
 
 uint32_t readADC(int);