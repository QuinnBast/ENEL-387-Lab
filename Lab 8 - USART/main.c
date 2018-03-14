/*
Transmitter:
1. Enable the USART by writing the UE bit in USART_CR1 register to 1.
2. Program the M bit in USART_CR1 to define the word length.
3. Program the number of stop bits in USART_CR2.
4. Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
place. Configure the DMA register as explained in multibuffer communication.
5. Select the desired baud rate using the USART_BRR register.
6. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
7. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
for each data to be transmitted in case of single buffer.
8. After writing the last data into the USART_DR register, wait until TC=1. This indicates
that the transmission of the last frame is complete. This is required for instance when
the USART is disabled or enters the Halt mode to avoid corrupting the last
transmission.

The TXE bit is always cleared by a write to the data register.
The TXE bit is set by hardware and it indicates:
? The data has been moved from TDR to the shift register and the data transmission has
started.
? The TDR register is empty.
? The next data can be written in the USART_DR register without overwriting the
previous data.
This flag generates an interrupt if the TXEIE bit is set.

*/

 #include "I:\ENEL\ENEL-387-Lab\Libraries\GPIO_lib.h"
 #include "I:\ENEL\ENEL-387-Lab\Libraries\LCD.h"
 #include "I:\ENEL\ENEL-387-Lab\Lab 7 - SPI\spi.h"
 #include "I:\ENEL\ENEL-387-Lab\Lab 8 - USART\USART.h"
 #include "stm32f10x.h"
 //#include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\GPIO_lib.h"
 //#include "C:\Users\Quinn\Dropbox\College\Semester 11 2018 Winter\ENEL 387\ENEL-387-Lab\Libraries\LCD.h"
	#include <stdlib.h>
	#include <math.h>

int main(){
	
	uint8_t hex = 0x21;
	uint8_t read;
	char text[16] = "";

initUSART();
initLCD();
clockInit();
led_IO_init();
portClockInit();
portInit();
	
	while(1){
		
		sendUSART(hex);
		if(hex < 0x7E){
			hex = hex + 1;
		} else {
			hex = 0x21;
		}
		
		read = readUSART();
		
		if(read > 0x21 && read <= 0x30){
			setLED(3, 1);
			setLED(4, 0);
		}
		
		if(read > 0x30 && read <= 0x41){
			setLED(4, 1);
			setLED(3, 0);
		}
		
		
		text[0] = read;
		stringToLCD(text, 0, 0);
		hexToLCD(text[0], 1, 1);
		
		delay(6000000);
	}
	
}
	
