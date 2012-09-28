#include "main.h"
#include "uart.h"
#include "angleEstimation.h"
#include "timer0.h"
#include "math.h"
#include "imu.h"

#define pi 3.141592654 
#define twopi 6.283185307 
#define degToRad 0.01745329252
#define ANGLELIMITER(x)		if (x<-pi) x += twopi; if (x>pi) x -= twopi;
static double phi=0,teta=0,psi=0;

void resetYaw(){
	psi = 0;
}

void updateYaw(int16_t angle){
	psi += 0.001*angle;
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

	double p= degToRad*IMUData[3]/14.25;
	double q= degToRad*IMUData[4]/14.25;
	double r= degToRad*IMUData[5]/14.25;
	
	double sinphi  = sin(phi );
	double cosphi  = cos(phi );
	double sinteta = sin(teta);
	double costeta = cos(teta);
	
	double dphi  = (p+((q*sinphi)+(r*cosphi))*(sinteta/costeta) );
	double dteta = -((q*cosphi)-(r*sinphi));
	double dpsi  = -(q*sinphi+r*cosphi)/costeta;
	
	
	///////////////////////// testing //////////////////////////////////////////////
	double ophi= phi,oteta = teta,opsi= psi;
	
	phi  = (phi + dphi/SYNC_PERIOD)*0.95 + 0.05*accPhi;		// int type
	teta = (teta+dteta/SYNC_PERIOD)*0.95 + 0.05*accTeta;		// int type
	psi  = psi + dpsi/SYNC_PERIOD;								// fsample/2 int type
	
	angle[3] = (phi-ophi)*SYNC_PERIOD;
	angle[4] = (teta-oteta)*SYNC_PERIOD;
	angle[5] = (psi-opsi)*SYNC_PERIOD;
	
	ANGLELIMITER(phi);
	ANGLELIMITER(teta);
	ANGLELIMITER(psi );
	
	angle[0] = phi;
	angle[1] = teta;
	angle[2] = psi;
//	angle[3] = dphi;
//	angle[4] = dteta;
//	angle[5] = dpsi;

	for(uint8_t i=0;i<3;i++){
		//setOutputData((angle[i]*180)/3.14,i+9);
		setOutputData(angle[i]*1000,i+9);
	}
	
	setOutputData(angle[3]*1000,24);
	setOutputData(angle[4]*1000,25);
	setOutputData(angle[5]*1000,26);
	
}




