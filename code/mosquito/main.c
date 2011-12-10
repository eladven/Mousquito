#include "main.h"
#include "uart.h"
#include "timer0.h"
#include "angleEstimation.h"
#include "i2c.h"
#include "imu.h"
#include "ppmIn.h"
#include "ppmOut.h"

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
	// disable watchdog
    MCUSR &=~(1<<WDRF);
    WDTCSR |= (1<<WDCE)|(1<<WDE);
    WDTCSR = 0;
    InitUART();
	InitTimer0();
	InitLeds();
	TWI_Master_Initialise();
	InitppmIn();
	InitppmOut();

	sei(); 		//enable global interapt 
	//////////////////////////////////////////////////////////////
	PORTA |= (1<<0); //set channel 0 for ppm out sequnce
	////////////////////////////////////////////////////////////////
	
	PrintEndl() ;
	PrintEndl() ;
	PrintString("FLYING MOSQUITO IS ON.") ;    
    PrintEndl() ;
	PrintEndl() ;
	InitIMU(); // use i2c, thus must be after sei instruction.
	int16_t  IMUData[9];  //accX,accY,accZ,gyroX,gyroY,gyroZ,magX,magY,magZ
	double  angle[3];  //pitch,roll,yaw
	int16_t  PPMIn[4];  //data from RC modol
	int16_t  PPMOut[8] = {0,0,0,0,0,0,0,0};  //data to motors
	
	
	updateSpeed(250,3);
	updateSpeed(250,2);
	while (1)   // infinit loop 
	{
		int16_t t1,t2;
		
		if  (IsNewPeriod())  //if the phase have just changed
		{
		    t1= GetMillis();
			GetIMUData(IMUData);  // 3 ms
			Estimator(IMUData,angle);
			uint8_t rcStatus = GetPPMIn(PPMIn);
			// pass the data to the uart module.
			for(uint8_t i=0;i<9;i++){
				setOutputData(IMUData[i],i);
			}
			for(uint8_t i=0;i<3;i++){
				setOutputData((angle[i]*180)/3.14,i+9);
			}
			for(uint8_t i=0;i<4;i++){
				setOutputData(PPMIn[i],i+12);
			}
			for(uint8_t i=0;i<4;i++){
				setOutputData(PPMOut[i],i+16);
			}
			if (rcStatus == PPM_IN_OK )
			{
				LED_ON(4);
				LED_OFF(5);
				LED_OFF(6);
			}
			else if (rcStatus == No_RF_SIGNAL )
			{
				LED_ON(5);
				LED_OFF(4);
				LED_OFF(6);
			}
			else if(rcStatus == NO_RECIVER_CONECCTION )
			{
				LED_ON(6);
				LED_OFF(5);
				LED_OFF(4);
			}
			SyncOut();		
		/*	t2= GetMillis(); 
			PrintString("time  ");
			PrintInt(t1);
			PrintString("  ");
			PrintInt(t2);
			PrintEndl();
			_delay_ms(1000); */
		}
	}
	return 1;
}

