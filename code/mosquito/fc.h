//******************************************************************************************
// 

//******************************************************************************************
#ifndef FC_H
#define FC_H

#include "timer0.h"

#define STIK_LIMIT 85
#define TIME_OF_COMMAND 8000/SYNC_PERIOD 
#define MAX_SPEED_VAL 250
#define MIN_SPEED_VAL 65
#define MIXER_VAL 1

#define COLECTIVE_FACTOR 1
#define ROLL_FACTOR 1
#define P_ROLL 1
#define D_ROLL 1
#define PITCH_FACTOR 1
#define P_PITCH 1
#define D_PITCH 1
#define YAW_FACTOR 1
#define P_YAW 1
#define D_YAW 1

void fc(int16_t*  IMUData,double*  angle,int16_t*  PPMIn,int16_t*  PPMOut);


#endif 
