#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "ppmIn.h"

static uint16_t _chanel[4] = {0,0,0,0};
static uint8_t  _flag = PPM_IN_OK;


void  InitppmIn(void)
{
	// enabled external interrupt int4
	EIMSK = (1<<INT7);
	// at raising edge 
	EICRB = (1<<ISC70) | (1<<ISC71);
	
	DDRE &= (0<<7);
	
	//enable compare A interrupt. disable compare B and overflow interrupts
  //  TIMSK1 = (0<<OCIE1B)|(1<<OCIE1A)|(0<<TOIE1);
    //Normal port operation OC0A disconnect. ctc mode of operation
	TCCR1A = (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10);
    // (16 MHz/64 ) 
    TCCR1B =  (0<<ICNC1)|(1<<WGM12)|(0<<WGM13)|(0<<CS12)|(1<<CS11)|(1<<CS10);	//reset counter
	OCR1A = 10000; // after 10,000 cycle , if there is no external interrupt , generate timer interrupt to indecate connection problem
	TCNT1 = 0;	
	
}

uint8_t  GetPPMIn(uint16_t * chanel)
{	
	for(uint8_t i=0;i<4;i++)
		chanel[i] = _chanel[i] - 380;
	return _flag;
}

ISR(INT7_vect)
{ 
    static uint8_t index=0;
	static uint8_t noReciverConnectionCounter = 0;
	
	uint16_t time= TCNT1; //read the duration of the current signal (from raising edge to raising edge).
	TCNT1 = 0;
	
	
	if (time > 1000)
		index = 0;
	else if (index < 4)
		_chanel[index++] = time;
  /*  PrintString("i ");	
    PrintInt(index);
    PrintString("  ");	
	PrintInt(time);
    PrintEndl();*/
	/*
	if (time > 1000)
	{
		index = 0;
		if (noReciverConnectionCounter > 0)
			noReciverConnectionCounter--;
		else
			_flag = PPM_IN_OK;
	}
	else if ( (index == 4) && (time < 1000) )
	{
		index = 3;
		noReciverConnectionCounter = 100;
		_flag = NO_RECIVER_CONECCTION;
	}
	else
		_chanel[index++] = time; //update chanel;	
		*/
}




