#include "main.h"
#include "uart.h"
#include "timer0.h"
#include "angleEstimation.h"
#include "i2c.h"
#include "imu.h"


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
	sei(); 		//enable global interapt 
	
	PrintEndl() ;
	PrintEndl() ;
	PrintString("FLYING MOSQUITO IS ON.") ;    
    PrintEndl() ;
	PrintEndl() ;
	
	InitIMU(); // use i2c, thus must be after sei instruction.
	
	int16_t  IMUData[9];  //accX,accY,accZ,gyroX,gyroY,gyroZ,magX,magY,magZ
	double  angle[3];  //pitch,roll,yaw
	
	while (1)   // infinit loop 
	{
		int16_t t1,t2;
		
		if  (IsNewPeriod())  //if the phase have just changed
		{
		t1= GetMillis();
			LED_ON(4);
			GetIMUData(IMUData);  // 3 ms
			SpiritLevelEstimator2(IMUData,angle);
			SyncOut(IMUData,angle);		
			t2= GetMillis();
				PrintString("time  ");
		PrintInt(t1);
		PrintString("  ");
		PrintInt(t2);
		PrintEndl();
		_delay_ms(1000);
		}
		LED_OFF(4);	
	}
	return 1;
}

