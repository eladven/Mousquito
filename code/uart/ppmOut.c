#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#include "main.h"
#include "ppmOut.h"

volatile static uint16_t speed[NUM_OF_PPM_CHANNELS]; //speed goes from 0 (min) to 249 (max)
volatile static uint8_t ppmChannel=0;

 
void InitppmOut(void)
{	
    //normal mode of operation
	TCCR3A |= (0<<COM3A1)|(0<<COM3A0)|(0<<COM3B1)|(0<<COM3B0)|(0<<WGM31)|(0<<WGM30);
   
	//prescaler=8 --> Fosc=2Mhz,Tosc=0.5us,22ms/4ch=5.5ms 5.5ms/0.5us=11,000, 1ms/0.5us=2,000, minSpeedGap=2000/250= 8)
	//WGM32=1 --> Clear Timer on Compare match mode (when tcnt3=top=OCR3A).
    TCCR3B |=  (0<<ICNC1)|(0<<WGM33)|(1<<WGM32)|(0<<CS32)|(1<<CS31)|(0<<CS30);	
	
	//Output Compare A&B Match Interrupt Enable
	TIMSK3|=(1<<OCIE3B)|(1<<OCIE3A);
	
	//reset counter 3
	TCNT3 = 0;

	//set Output Compare 3A to 5.5ms
	OCR3A=11000;
	
	//set Output Compare 3B to min speed (= 1ms)
	OCR3B=2000;
	
	//set start speed value to min speed
	for(int j=0;j<NUM_OF_PPM_CHANNELS;j++){
		speed[j]=2000;
	}	
	speed[2]= (8*250) + 2000;
}


void updateSpeed(uint8_t _speed,uint8_t _channel)
{
	//update the speed of channel number [_channel]
	speed[_channel]= (8 * _speed) + 2000;
}



//Do this every 5.5ms:
ISR(TIMER3_COMPA_vect) 
{
		OCR3B=speed[ppmChannel];
		//TCNT3 = 0;
		PORTA |= (1<<ppmChannel); 	
}



//Do this after 1-2ms from tcnt3 set
ISR(TIMER3_COMPB_vect) 
{
	PORTA &= ~(1<<ppmChannel);
	
	if (ppmChannel<3){
		ppmChannel++;
	}
	else if (ppmChannel==3){
		ppmChannel = 0;
	}	
		
}