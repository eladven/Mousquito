#include "main.h"
#include "uart.h"
#include "angleEstimation.h"
#include "timer0.h"
#include "math.h"
#include "imu.h"

#define pi 3.141592654 
#define twopi 6.283185307 
#define ANGLELIMITER(x)		if (x<-pi) x += twopi; if (x>pi) x -= twopi;

void Estimator(int16_t *IMUDataInt,double *angle)
{

	int16_t imuBiases[6];
	double IMUData[6];
	GetIMUBiases(imuBiases);
	// subtract the bias from acc and gyro:
	for (int j=0; j<6; j++)
		IMUData[j]=IMUDataInt[j] - imuBiases[j]/20.0;
		
	IMUData[2] +=256; //need to feal the gravity
	
	double accPhi = atan2(IMUData[1],IMUData[2]);
	double accTeta = atan2(IMUData[0],sqrt(IMUData[1]*IMUData[1]+IMUData[2]*IMUData[2]));
	
# define degToRad 0.01745329252
	static double phi=0,teta=0,psi=0;
	
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
	
	
	phi  = (phi + dphi/SYNC_PERIOD)*0.95 + 0.05*accPhi;		// int type
	teta = (teta+dteta/SYNC_PERIOD)*0.95 + 0.05*accTeta;		// int type
	psi  = psi + dpsi/SYNC_PERIOD;								// fsample/2 int type
	
	ANGLELIMITER(phi);
	ANGLELIMITER(teta);
	ANGLELIMITER(psi );
	
	angle[0] = phi;
	angle[1] = teta;
	angle[2] = psi;
}




//*******************************************************************************************
//**************************** Experimental  Estimators *************************************
//*******************************************************************************************


void SpiritLevelEstimator(int16_t *IMUData,double *angle)
{
	double c1=IMUData[1],c2=IMUData[2];
	angle[0] = atan2(IMUData[1],IMUData[2]);
	angle[1] = atan2(IMUData[0],sqrt(c1*c1+c2*c2));
	angle[2] = 0;
}

void GyroEstimator(int16_t *IMUData,double *angle)
{
# define degToRad 0.01745329252
	static double phi=0,teta=0,psi=0;
	
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
	
	
	phi  = phi + dphi/SYNC_PERIOD;		
	teta = teta+dteta/SYNC_PERIOD;		
	psi  = psi + dpsi/SYNC_PERIOD;	

	ANGLELIMITER(phi);
	ANGLELIMITER(teta);
	ANGLELIMITER(psi);
	
	angle[0] = phi;
	angle[1] = teta;
	angle[2] = psi;
}

/***********************************************************************************************************************/
	

#define dt 1/SYNC_PERIOD
#define x 0
#define y 1
#define z 2

double R[3][3];  //the DCM 

void runDCMEstimator(double *w)
{
//                                              [ 1       -wz*dt    wy*dt ] 
//	first , applay the kinmatic: R[k+1] = R[k]* [ wz*dt     1      -wx*dt ]
//                                              [ -wy*dt   wx*dt       1  ]  

	double Rtemp[3][3];            
	double kinematics[3][3] = {1 ,      -w[z]*dt  ,  w[y]*dt,  
	                             w[z]*dt,     1 ,     -w[x]*dt,
								-w[y]*dt,   w[x]*dt,       1 } ;
	for (int i=0;i<3;i++) // we dont use z , we calculate it from X and Y.
	{
		for (int j=0;j<3;j++)
		{
			Rtemp[i][j] = 0;
			for (int k=0;k<3;k++)
			{
				Rtemp[i][j] += R[i][k]*kinematics[k][j];
			}
		}
	}

//                                                               [X]	
//   secend step is normalization. we will use the fact that R = [Y] such that X ,Y , Z are orthoganl in pairs
//                                                               [Z]
//   define error = XºY ,and then Xortogonal = X - 0.5*error*Y , Yortognal = Y - 0.5*error*X 
//   and Zortognal = Xortognal x Yortognal.
//   Now , we will use the fact that ||X|| = ||Y|| = ||Z||. 
//   we will define: Xnorm = 0.5(3-Xortognal*Xortognal) *Xortognal
//   we will define: Ynorm = 0.5(3-Yortognal*Yortognal) *Yortognal
//   we will define: Znorm = 0.5(3-Zortognal*Zortognal) *Zortognal

	double error = 0;
	for (int i=0;i<3;i++)
		error += Rtemp[x][i]*Rtemp[y][i];
		   	
	for (int i=0;i<3;i++)
	{
		R[x][i] = Rtemp[x][i]-0.5*error*Rtemp[y][i];
		R[y][i] = Rtemp[y][i]-0.5*error*Rtemp[x][i];
	}
		
	R[z][x] = R[x][y]*R[y][z]-R[y][y]*R[x][z];
	R[z][y] = R[x][x]*R[y][z]-R[y][x]*R[x][z];
	R[z][z] = R[x][x]*R[y][y]-R[y][x]*R[x][y];

	for (int i=x; i <=z ;i++)
	{
		double temp = 0;
		for (int j=x;j <= z ; j++)
			temp += R[i][j]*R[i][j];
		double k = 0.5*(3-temp);
		for  (int j=x;j <= z ; j++)
			R[i][j] = R[i][j]*k;
	}
}