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

#include "stm32f10x.h"

int main(){
	//ALTERNATE FUNCTION CONFIGURATION
	
	//The USART3 Alternate function Is default mapped to PB10 (Tx) and PB11 (Rx). No need to remap.
	//No configuration of the alternate functions
	
	
	//PIN CONFIGURATION
	
	//For USART to be enabled:
	//Tx Full Duplex : 							GPIO_CNF = Alternate function push-pull
	//Tx Half Duplex Synchronous : 	GPIO_CNF = Alternate function push-pull
	
	//Rx Full Duplex : 							GPIO_CNF = Floating Input / Input pull-up
	//Rx Half Duplex Synchronous : 	GPIO_CNF = Not used.
	
	//PB10
	GPIOB->CRH |= GPIO_CRH_MODE10; 	//Set PB10(Tx) to output mode (11)
	GPIOB->CRH &= (GPIO_CRH_CNF10_1 & ~GPIO_CRH_CNF10_0); 	//Set PB10(Tx) alternate function push-pull (10)
	
	//PB11
	GPIOB->CRH &= ~GPIO_CRH_MODE11_1 & GPIO_CRH_MODE11_0; 	//Set PB11(Rx) to be a floating input (01)
	
	
	//USART CONFIGURATION
	//PB10 and 11 use USART3 registers
	USART3->CR1 |= USART_CR1_UE;					//Enable USART
	USART3->CR2 &= USART_CR2_STOP & 0x01;	// Configure the number of stop bits
																				// 00 = 1 stop bit
																				// 01 = 0.5 stop bits
																				// 10 = 2 stop bits
																				// 11 = 1.5 Stop Bits
											
	// Set the buad rate. The LSB is the DIV_Fraction, upper 3 bytes are DIV_Mantissa
	// Baud = Fclk/(16*USARTDIV)
	// USARTDIV = USART_Mantissa + (USART_Fraction/16)
	
	//If clock is 24KHz
	// 38.4 KHz = 24kHz/(16*USARTDIV); USARTDIV = 25.6
	// 25 = 0x19
	// 0.6 = x/16; x = 9.6 = 10 = 0xA
	USART3->BRR &= 0x0000019A;
	
	//If clock is 8KHz
	// 38.4 KHz = 8kHz/(16*USARTDIV); USARTDIV = 76.8
	// 76 = 0x4C
	// 0.8 = x/16; x = 12.8 = 13 = 0xD
	//USART3->BRR &= 0x00004CD;
	
	USART3->CR1 &= USART_CR1_TE;				//Send an idle first frame transmission


/*
Reciever:
Start bit detection
In the USART, the start bit is detected when a specific sequence of samples is recognized.
This sequence is: 1 1 1 0 X 0 X 0 X 0 0 0 0.
//Thus the first 3 bytes being reieved are only to tell the program a message is being recieved.

During a USART reception, data shifts in least significant bit first through the RX pin.

Procedure:
1. Enable the USART by writing the UE bit in USART_CR1 register to 1.
2. Program the M bit in USART_CR1 to define the word length.
3. Program the number of stop bits in USART_CR2.
4. Select DMA enable (DMAR) in USART_CR3 if multibuffer communication is to take
place. Configure the DMA register as explained in multibuffer communication. STEP 3
5. Select the desired baud rate using the baud rate register USART_BRR
6. Set the RE bit USART_CR1. This enables the receiver which begins searching for a
start bit.

When a character is received
? The RXNE bit is set. It indicates that the content of the shift register is transferred to the
RDR. In other words, data has been received and can be read (as well as its
associated error flags).
? An interrupt is generated if the RXNEIE bit is set.
? The error flags can be set if a frame error, noise or an overrun error has been detected
during reception.
? In multibuffer, RXNE is set after every byte received and is cleared by the DMA read to
the Data Register.
? In single buffer mode, clearing the RXNE bit is performed by a software read to the
USART_DR register. The RXNE flag can also be cleared by writing a zero to it. The
RXNE bit must be cleared before the end of the reception of the next character to avoid
an overrun error.

The RXNE flag is set after every byte received
*/

	//USART is already enabled from enabling the Tx wire
	//Word length is already set through Tx.
	
}
	
