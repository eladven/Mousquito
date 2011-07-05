#include "main.h"




#define LED_ON(n)		(PORTB |= (1<<n))
#define LED_OFF(n)		(PORTB &= ~(1<<n))






int main(void)
{
	CPU_PRESCALE(0); // use the 16 MHz clk
    InitUART();
	sei(); 		//enable global interapt 
	
	PrintEndl() ;
	PrintEndl() ;
	PrintEndl() ;
	PrintString("FLYING MOSQUITO IS ON.") ;    
    PrintEndl() ;
	PrintEndl() ;
			
	LED_OFF(6);
	LED_OFF(5);
	LED_OFF(4);
    DDRB |= (1<<6);
	DDRB |= (1<<5);
	DDRB |= (1<<4);
	while (1)
	{
		LED_ON(6);
		_delay_ms(400);
		LED_OFF(6);
		
		LED_ON(5);
		_delay_ms(400);
		LED_OFF(5);
		
		LED_ON(4);
		_delay_ms(400);
		LED_OFF(4);
		
	}
}

