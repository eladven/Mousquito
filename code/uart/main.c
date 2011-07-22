#include "main.h"
#include "uart.h"
#include "timer0.h"
#include "angleEstimation.h"


void InitLeds(void)
{
	DDRB |= (1<<6);
	DDRB |= (1<<5);
	DDRB |= (1<<4);
	PORTB = 0;
}

int main(void)
{
	CPU_PRESCALE(0); // use the 16 MHz clk
    InitUART();
	InitTimer0();
	InitLeds();
	
	// disable watchdog
    MCUSR &=~(1<<WDRF);
    WDTCSR |= (1<<WDCE)|(1<<WDE);
    WDTCSR = 0;
	sei(); 		//enable global interapt 
	
	PrintEndl() ;
	PrintEndl() ;
	PrintEndl() ;
	PrintString("FLYING MOSQUITO IS ON.") ;    
    PrintEndl() ;
	PrintEndl() ;
	

	uint8_t prevPhase=0;
	while (1)   // infinit loop 
	{
	    uint8_t currentPase = GetPhase();
		if  ( currentPase != prevPhase  )  //if the phase have just changed
		{
			prevPhase = currentPase;
			runEstimator();
			SyncOut();
		}
	}
	return 1;
}

