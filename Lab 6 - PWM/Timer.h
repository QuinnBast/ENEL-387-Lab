/******************************************************************************
 * Name:    Timer.h
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
 
  //Initialize a timer with a given count value.
 void initTimer(int, float);
 
 //Read the value of the timer.
 int readTimer(void);
 
 //Set the duty cycle
 void setDuty(float);
 
 //Set the period
 void setPeriod(int);