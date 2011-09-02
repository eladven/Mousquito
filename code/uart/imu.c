
#include "main.h"
#include "imu.h"
#include "i2c.h"
void InitIMU(void)
{ 
	unsigned char messageBuf[8];
	//Accelormeter:
	// set the range to +-2g (reg14h[4:3] = 0b00, and set the digital filter to 100 Hz bandwith (reg14h[2:0] = 0b010)
	// WITHOUT CHANGE reg14h[7:5]
	// default value of reg14h is 0x2E
	messageBuf[0] = (TWI_accelerometerAdd<<1) | (TWI_WRITE); // read original reg14h data.
	messageBuf[1] = 0x14;  
	TWI_Start_Read_Write( messageBuf, 2 );	
	messageBuf[0] = (TWI_accelerometerAdd<<TWI_ADR_BITS) | (TWI_READ); 
	TWI_Start_Read_Write( messageBuf,2); // one for sla+rw + 2 x 3 regs	
	TWI_Read_Data_From_Buffer(messageBuf, 2 );
	
	messageBuf[1] = (messageBuf[1] & 0xE0) | 0x02;

	messageBuf[0] = (TWI_accelerometerAdd<<1) | (TWI_WRITE); // read original reg14h data.
	messageBuf[2] = messageBuf[1]; 
	messageBuf[1] = 0x14;  
	TWI_Start_Read_Write( messageBuf, 3 );		
	
	// Gyro:
	// set the range to +-20000 [deg/sc] (reg16h[4:3] = 0b11, and set the digital filter to 100 Hz bandwith (reg14h[2:0] = 0b000)
	// default value of reg16h is 0x00
	messageBuf[0] = (TWI_gyroscopeAdd<<1) | (TWI_WRITE); 
	messageBuf[1] = 0x16; 
	messageBuf[2] = 0x18;  
	TWI_Start_Read_Write( messageBuf, 3 );	
}

void GetIMUData(int16_t *IMUData)
{
	unsigned char messageBuf[8];
	// read from acceolerometr.
	messageBuf[0] = (TWI_accelerometerAdd<<1) | (TWI_WRITE); // The first byte must always consit of General Call code or the TWI slave address.
	messageBuf[1] = 0x02;  //start read data form sh address - the x acc lsb.
	TWI_Start_Read_Write( messageBuf, 2 );
	
	messageBuf[0] = (TWI_accelerometerAdd<<TWI_ADR_BITS) | (TWI_READ); // The first byte must always consit of General Call code or the TWI slave address.
	TWI_Start_Read_Write( messageBuf,7); // one for sla+rw + 2 x 3 regs
	TWI_Read_Data_From_Buffer(messageBuf, 7 );
	
	int8_t hx = messageBuf[2],hy = messageBuf[4],hz = messageBuf[6];
	IMUData[0] =   ((hx << 8 ) >> 6) | (messageBuf[1] >> 6); // two's compliment of accX[msb](hx[7:0]) # accX[lsb](hx[7:6])
	IMUData[1] =   ((hy << 8 ) >> 6) | (messageBuf[3] >> 6);
	IMUData[2] =   ((hz << 8 ) >> 6) | (messageBuf[5] >> 6);
	
	// read from gyro.
	
	
	messageBuf[0] = (TWI_gyroscopeAdd<<1) | (TWI_WRITE); // The first byte must always consit of General Call code or the TWI slave address.
	messageBuf[1] = 0x1D;  //start read data form 0x1D address - the x gyro msb.
	//TWI_Start_Random_Read(TWI_Start_Random_Read,5, 2);
	
	TWI_Start_Read_Write( messageBuf,2); // one for sla+rw + 2 x 3 regs
	
	messageBuf[0] = (TWI_gyroscopeAdd<<TWI_ADR_BITS) | (TWI_READ); // The first byte must always consit of General Call code or the TWI slave address.
	TWI_Start_Read_Write( messageBuf,7); // one for sla+rw + 2 x 3 regs
	
	TWI_Read_Data_From_Buffer(messageBuf, 7 );
	IMUData[3] =   (messageBuf[1] << 8 ) | messageBuf[2] ; // two's compliment of gyroX[msb](hx[7:0]) # accX[lsb](hx[7:6])
	IMUData[4] =   (messageBuf[3] << 8 ) | messageBuf[4] ; 
	IMUData[5] =   (messageBuf[5] << 8 ) | messageBuf[6] ; 
	
}






