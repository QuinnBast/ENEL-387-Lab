/******************************************************************************
 * Name:    MP3Trigger.h
 * Description: MP3Trigger functions
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
 #include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "stm32f10x.h"
 #include <stdint.h>
 #include <math.h>

//Initialize communication to the MP3 Trigger
void initMP3Trigger(void);

//Toggle between play and pause
void startStop();

//Skip to the next track
void nextTrack();

//Go to the previous track
void prevTrack();

//Play a specific track number
void playTrack(int trackNo);

//Set the volume output
//Maximum is 0x00; Over 0x40 (64) is inaudible
void setVolume(int volume);

//Get the number of tracks on the SD Card
int getNoTracks();

//Get the current track number
int getTrackNumber();