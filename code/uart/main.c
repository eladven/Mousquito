#include "main.h"
#include "uart.h"
#include "timer0.h"
#include "angleEstimation.h"
#include "i2c.h"



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
	
	TWI_Master_Initialise();

	sei(); 		//enable global interapt 
	
	InitIMU(); // use i2c, thus must be after sei instruction.
	
	
	volatile int16_t  IMUData[9];  //accX,accY,accZ,gyroX,gyroY,gyroZ,magX,magY,magZ
	
	uint8_t prevPhase=0;
	while (1)   // infinit loop 
	{
	    uint8_t currentPase = GetPhase();
		if  ( currentPase != prevPhase  )  //if the phase have just changed
		{
			prevPhase = currentPase;
		//	runEstimator();
			SyncOut();
			LED_ON(4);
			_delay_ms(200);
			
			/*
			/// code with Maor magno 
			TWI_targetSlaveAddress = TWI_magnetometerAdd;
	
			messageBuf[0] = (TWI_targetSlaveAddress<<1) | (TWI_WRITE); // The first byte must always consit of General Call code or the TWI slave address.
			messageBuf[1] = 0x00;
			TWI_Start_Read_Write( messageBuf, 2 );
	
			messageBuf[0] = (TWI_targetSlaveAddress<<TWI_ADR_BITS) | (TWI_READ); // The first byte must always consit of General Call code or the TWI slave address.
			TWI_Start_Read_Write( messageBuf, 1 );

			PrintString("messageBuf  ");
			PrintInt(messageBuf[0]);
			PrintEndl();
			TWI_Read_Data_From_Buffer(messageBuf, 4 );
			PrintString("messageBuf - after reading  ");
			PrintInt(messageBuf[0]);
			PrintString("  ");
			PrintInt(messageBuf[1]);
			PrintString("  ");
			PrintInt(messageBuf[2]);
			PrintString("  ");
			PrintInt(messageBuf[3]);
			PrintEndl();
			//printChar(messageBuf);
			*/
			/// code for accelometer
			
			
			GetIMUData(IMUData);
			
			PrintString("IMU DATA  ");
			PrintString("Acc  ");
			PrintInt(IMUData[0]);
 			PrintString("  ");
			PrintInt(IMUData[1]);
 			PrintString("  ");
			PrintInt(IMUData[2]);
			PrintString("  Gyro  ");
			PrintInt(IMUData[3]);
 			PrintString("  ");
			PrintInt(IMUData[4]);
 			PrintString("  ");
			PrintInt(IMUData[5]);
 			PrintEndl();
	
		}
		LED_OFF(4);	
	}
	return 1;
}

