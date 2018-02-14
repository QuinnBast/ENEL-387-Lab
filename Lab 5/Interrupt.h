/******************************************************************************
 * Name:    Interrupt.h
 * Description: Interrupt initilization and setup
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
 
 //Function to initialize the interrupt system
 void initSysTick(void);
 
 //Initialize External Interrupts
 void initExternalInterrupt(int, int);