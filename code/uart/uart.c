#include <avr/io.h>

#include "main.h"
#include "uart.h"

void InitUART(void)
{
	// Enable receive and transmitte
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR1C = (0<<USBS1)|(1<<UCSZ11)|(1<<UCSZ10);
	// Set baud rate.
	UBRR1H = 0;
	UBRR1L = (SYSCLK / (BAUD_RATE * 8L) - 1);
	//duoble the speed
	UCSR1A |= (1 << U2X1); 
	// enable RX-Interrupt  enable TX-Interrupt
	UCSR1B |= (1 << RXCIE1);
	UCSR1B |= (1 << TXCIE1);
	// set TX port as output, RX port as input
	DDRD |= (1<<3);
	DDRD |= (0<<2);
}

// hendel recive interrupt
ISR(USART1_RX_vect)
{
	char c = UDR1;
    while (! (UCSR1A & (1<<UDRE1)));
	UDR1 = c+1;
}

// hendel transmit interrupt
ISR(USART1_TX_vect)
{
int i=19;
}




