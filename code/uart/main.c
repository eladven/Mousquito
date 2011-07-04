#include "main.h"




#define LED_ON(n)		(PORTB |= (1<<n))
#define LED_OFF(n)		(PORTB &= ~(1<<n))






int main(void)
{
	CPU_PRESCALE(0); // use the 16 MHz clk
    InitUART();
	sei(); 		//enable global interapt 
	
	LED_OFF(6);
	LED_OFF(5);
	LED_OFF(4);
    DDRB |= (1<<6);
	DDRB |= (1<<5);
	DDRB |= (1<<4);
	while (1)
	{
	}
	/*{
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
*/
}

