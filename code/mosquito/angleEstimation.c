#include "main.h"
#include "uart.h"
#include "angleEstimation.h"
#include "timer0.h"
#include "math.h"
#include "imu.h"
#include "fixMath.h"

#define pi 3.141592654 
#define twopi 6.283185307 
#define degToRad 0.01745329252
#define ANGLELIMITER(x)		if (x<-pi) x += twopi; if (x>pi) x -= twopi;
const static double  GYRO_FACTOR = 0.0001*degToRad/14.25;
static double phi=0,teta=0,psi=0;

void resetYaw(){
	psi = 0;
}

void updateYaw(int16_t angle){
	psi += 0.001*angle;
}

void Estimator2(int16_t *IMUDataInt,double *angle)
{

	int16_t * imuBiases;
	int16_t IMUData[6];
	imuBiases = GetIMUBiases();
	// subtract the bias from acc and gyro:
	for (int j=0; j<6; j++)
		IMUData[j]=IMUDataInt[j] - imuBiases[j]/20.0;
		
	IMUData[2] +=256; //need to feal the gravity
	
	int16_t accPhi = fixATan2(IMUData[1],IMUData[2]);
	int16_t accTeta = fixATan2(IMUData[0],intSqrt((int32_t)IMUData[1]*IMUData[1]+(int32_t)IMUData[2]*IMUData[2]));

	
	angle[0] = accPhi;
	angle[1] = accTeta;

	for(uint8_t i=0;i<2;i++){
		setOutputData(angle[i]*180 / PI,i+9);
	}
}


void Estimator(int16_t *IMUDataInt,double *angle)
{

	int16_t * imuBiases;
	double IMUData[6];
	imuBiases = GetIMUBiases();
	// subtract the bias from acc and gyro:
	for (int j=0; j<6; j++)
		IMUData[j]=IMUDataInt[j] - imuBiases[j]/20.0;
		
	IMUData[2] +=256; //need to feal the gravity
	
	double accPhi = atan2(IMUData[1],IMUData[2]);
	double accTeta = atan2(IMUData[0],sqrt(IMUData[1]*IMUData[1]+IMUData[2]*IMUData[2]));
	double magPsi = atan2(IMUDataInt[7],-IMUDataInt[6]);

	double p= IMUData[3]*GYRO_FACTOR;
	double q= IMUData[4]*GYRO_FACTOR;
	double r= IMUData[5]*GYRO_FACTOR;
	
	double sinphi  = sin(phi );
	double cosphi  = cos(phi );
	double sinteta = sin(teta);
	double costeta = cos(teta);
	
	double dphi  = (p+((q*sinphi)+(r*cosphi))*(sinteta/costeta) );
	double dteta = -((q*cosphi)-(r*sinphi));
	double dpsi  = -(q*sinphi+r*cosphi)/costeta;
	
	double AccNorm = IMUData[0]*IMUData[0] + IMUData[1]*IMUData[1]+IMUData[2]*IMUData[2];
	double AccFactor = sqrt( (62500.0 - AccNorm)*(62500.0 - AccNorm));
	AccFactor = (AccFactor > 1500) ? 0 : (1500 - AccFactor)/1500;
	
	phi  = (phi + dphi*SYNC_PERIOD)* (1 - AccFactor*0.1) + AccFactor*0.1*accPhi;		// int type
	teta = (teta +dteta*SYNC_PERIOD)*(1 - AccFactor*0.1) + AccFactor*0.1*accTeta;		// int type
	psi  = (psi + dpsi*SYNC_PERIOD)*0.9999 + 0.0001*magPsi; //psi ;								
	
	ANGLELIMITER(phi);
	ANGLELIMITER(teta);
	ANGLELIMITER(psi );
	
	angle[0] = phi;
	angle[1] = teta;
	angle[2] = psi;
	angle[3] = dphi;
	angle[4] = dteta;
	angle[5] = dpsi;

	for(uint8_t i=0;i<3;i++){
		setOutputData((angle[i]*180)/3.14,i+9);
	}
}





