#include "main.h"




#define LED_ON(n)		(PORTB |= (1<<n))
#define LED_OFF(n)		(PORTB &= ~(1<<n))






int main(void)
{
	CPU_PRESCALE(0); // use the 16 MHz clk
    InitUART();
	InitTimer0();
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
	uint16_t prevTime=0,currentTime;
	while (1)
	{
		currentTime = GetMillis();
		if ((currentTime - prevTime > 1000) || (currentTime < prevTime ))
		{
			prevTime = currentTime;
			PORTB =~PORTB;
		}

	}
}

