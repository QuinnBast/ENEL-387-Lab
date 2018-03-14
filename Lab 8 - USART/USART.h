/******************************************************************************
 * Name:    USART.c
 * Description: USART functions
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

#include "I:\ENEL\ENEL-387-Lab\Libraries\GPIO_lib.h"
 //#include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include <stdint.h>
 #include <math.h>

void initUSART(void);
void sendUSART(uint8_t);
uint8_t readUSART(void);
 