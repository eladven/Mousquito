#include "main.h"
#include "uart.h"
#include "angleEstimation.h"
#include "timer0.h"
#include "math.h"

void SpiritLevelEstimator(int16_t *IMUData,double *angle)
{
	double c0=IMUData[0],c1=IMUData[1],c2=IMUData[2];
	angle[1] = atan2(c1,c2);
	angle[0] = atan2(c0,sqrt(c1*c1+c2*c2));
		PrintString("sfsdfsdf  ");
		//PrintInt(IMUData[1]);
		//PrintString("  ");
		PrintInt((angle[0]*180)/3.14);
		PrintString("  ");
		PrintInt((angle[1]*180)/3.14);
		PrintEndl();
}


void SpiritLevelEstimator2(int16_t *IMUData,double *angle)
{
	double c0=IMUData[0],c1=IMUData[1],c2=IMUData[2];
	angle[1] = c_atan2(c1=IMUData[1], c1=IMUData[2]);
	angle[0] = c_atan2(c0,sqrt(c1*c1+c2*c2));
		PrintString("sfsdfsdf  ");
		PrintInt(angle[0]);
		PrintString("  ");
		PrintInt(angle[1]);
		PrintEndl();
}

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