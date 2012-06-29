//******************************************************************************************
// 

//******************************************************************************************
#ifndef FC_H
#define FC_H

#include "timer0.h"

#define STIK_LIMIT 90
#define TIME_OF_COMMAND 20000/SYNC_PERIOD 

void fc(int16_t*  IMUData,double*  angle,int16_t*  PPMIn,int16_t*  PPMOut);


#endif 
