
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>



#define LED_ON(n)		(PORTB |= (1<<n))
#define LED_OFF(n)		(PORTB &= ~(1<<n))


#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#define SYSCLK  16000000
#define BAUD_RATE 9600


int main(void)
{

// Enable receive and transmitte
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR1C = (0<<USBS1)|(1<<UCSZ11)|(1<<UCSZ10);
	// Set baud rate 9600 for 16 mHz clk
	UBRR1H = 0;
	UBRR1L = (SYSCLK / (BAUD_RATE * 8L) - 1);
	UCSR1A |= (1 << U2X1); 
	//duoble the speed
	// enable RX-Interrupt  enable TX-Interrupt
	UCSR1B |= (1 << RXCIE1);
	UCSR1B |= (1 << TXCIE1);
	DDRD |= (1<<3);

	// set for 16 MHz clock, and make sure the LED is off
	CPU_PRESCALE(0);

	LED_OFF(6);
	LED_OFF(5);
	LED_OFF(4);
    DDRB |= (1<<6);
	DDRB |= (1<<5);
	DDRB |= (1<<4);
	while (1)
	{
	//LED_ON(6);
	while (! (UCSR1A & (1<<UDRE1)));
	UDR1 = 'E';
	while (! (UCSR1A & (1<<UDRE1)));
	UDR1 = 'L';
	while (! (UCSR1A & (1<<UDRE1)));
	UDR1 = 'A';
	while (! (UCSR1A & (1<<UDRE1)));
	UDR1 = 'D';
	while (! (UCSR1A & (1<<UDRE1)));
	UDR1 = ' ';
	char c;
	while (! (UCSR1A & (1<<RXC1)));
	c = UDR1;
	while (! (UCSR1A & (1<<UDRE1)));
	UDR1 = c+1;
	}

}

