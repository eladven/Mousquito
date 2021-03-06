#include "main.h"
#include "timer0.h"
#include "math.h"

uint8_t counter=0;  // time [100 usec]
uint16_t millis = 0;
uint16_t sec    = 0;
static uint8_t phase = 0;
static uint8_t outputPhase = 0;

void InitTimer0(void)
{

	//enable compare A interrupt. disable compare B and overflow interrupts
    TIMSK0 = (0<<OCIE0B)|(1<<OCIE0A)|(0<<TOIE0);
    //Normal poert operation OC0A disconnect. CTC mode of operation
	TCCR0A = (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)|(1<<WGM01)|(0<<WGM00);
    // clk/8 prescaler (16 MHz/8 = 2MHz)
    TCCR0B = (0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00);
	// get compare interpat evry 2MHz / 200 = 10kHz. 
	OCR0A = 200;
	//reset counter
	TCNT0 = 0;
}

uint16_t GetMicros(void) //returns time [100 usec]
{
	return counter; 
}

uint16_t GetMillis(void) 
{
	return millis; 
}

uint16_t GetSecs(void)  
{
	return sec; 
}




uint8_t IsNewPeriod(void)
{
	static uint8_t internalPhase = 0;
	if (phase != internalPhase)
	{
		internalPhase = phase;
		return TRUE;
	}
	return FALSE;
}

uint8_t IsNewOutputPeriod(void)
{
	static uint8_t internalPhase = 0;
	if (outputPhase != internalPhase)
	{
		internalPhase = outputPhase;
		return TRUE;
	}
	return FALSE;
}

ISR(TIMER0_COMPA_vect)  //evry 10 kHz
{ 
	static uint16_t cnt = 0;
	static uint16_t outputCnt = 0;


	cnt++;
	counter ++;
	outputCnt ++;
	if (counter == 10)
	{
		counter = 0;
		millis++;
	}
	if (millis == 1000)
	{
		millis = 0;
		sec++;
	}
	if (cnt == SYNC_PERIOD)
	{
		cnt = 0;
		if (phase)   //tugel the phase every SYNC_PERIOD
			phase = 0;
		else
		    phase = 1;
	}
	if (outputCnt == OUTPUT_PERIOD)
	{
		outputCnt = 0;
		if (outputPhase)   //tugel the phase every SYNC_PERIOD
			outputPhase = 0;
		else
		    outputPhase = 1;
	}
}

