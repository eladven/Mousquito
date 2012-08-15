#include <avr/io.h>
#include <string.h>

#include "main.h"
#include "fc.h"
#include "uart.h"

uint8_t _isEngeinsOnFlag = 0;
uint8_t _isMenualControl = 0;

#define SPEED_LIMITER(x) if (x < MIN_SPEED_VAL) x = MIN_SPEED_VAL; if ( x > MAX_SPEED_VAL) x = MAX_SPEED_VAL;

//////    structure:   
//                
//                3 ccw
//                ^
//                | x
//                |    y
//   cw  0<------- ------->2 cw
//                |
//                |
//                v
//                1 ccw
//
//////////////////////////////////////////////////////


// This part is used only for configurate the controller constants
////////////////////////////////////////////////////////////////////

//                         //colec,roll,pitch,yaw
int16_t _constants[3][4] = {{103,0,0,0},   //factor
							{0  ,0,0,0},    //p
							{0  ,0,0,0}};  //d
							
void setConst(int16_t i,int16_t j,int16_t val){
	_constants[i][j] = val;
}

////////////////////////////////////////////////////////////////////

           //   motors:        0         1          2         3
int16_t mixer[4][4] = {{MIXER_VAL ,MIXER_VAL ,MIXER_VAL ,MIXER_VAL  },     //u1 colective
                        {MIXER_VAL ,0         ,-MIXER_VAL,0         },     //u2 roll
						{ 0        ,MIXER_VAL, 0        ,-MIXER_VAL },     //u3 pitch
						{MIXER_VAL ,-MIXER_VAL,MIXER_VAL ,-MIXER_VAL}};    //u4 yaw

void setMenualControl(uint8_t isMenualControl){
	_isMenualControl = isMenualControl;
}

void fc(int16_t*  IMUData,double*  angle,int16_t*  PPMIn,int16_t*  PPMOut){
	getRCCommands(PPMIn);
	if (!_isMenualControl){
		if (_isEngeinsOnFlag){
			// lets define four control signals:
			// u0 to control the colective 
			// u1 to control the roll
			// u2 to control the pitch
			// u3 to conrol the yaw.
			int16_t u[4];
			/*
			u[0] = PPMIn[2]*COLECTIVE_FACTOR;
			u[1] = PPMIn[1]*ROLL_FACTOR+P_ROLL*angle[0]+D_ROLL*angle[3];
			u[2] = PPMIn[0]*PITCH_FACTOR+P_PITCH*angle[1]+D_PITCH*angle[4];
			u[3] = PPMIn[3]*YAW_FACTOR+P_YAW*angle[2]+D_YAW*angle[5];
			*/
			////////////////////////////
			u[0] = (PPMIn[2]+145)*_constants[0][0];  //colective
			u[1] = PPMIn[1]*_constants[0][1]+_constants[1][1]*angle[0]+_constants[2][1]*angle[3]; //roll
			u[2] = PPMIn[0]*_constants[0][2]+_constants[1][2]*angle[1]+_constants[2][2]*angle[4];
			u[3] = PPMIn[3]*_constants[0][3]+_constants[1][3]*angle[2]+_constants[2][3]*angle[5];
			
			setOutputsData(u,20,23);
			////////////////////////////////////////////
			for (int i=0;i<4;i++){
				PPMOut[i] = 0;
				for (int j=0;j<4;j++){
					PPMOut[i] += mixer[j][i]*u[j];
				}
				PPMOut[i] = PPMOut[i]/100;
				SPEED_LIMITER(PPMOut[i]);
				
			}
		} else {
			for (int i=0;i<4;i++)
				PPMOut[i] = 0;
		}
	}
	
	setOutputData(_isMenualControl,32);
	setOutputData(_isEngeinsOnFlag,33);
}


void getRCCommands(int16_t*  PPMIn){
	static uint8_t dowenRightCounter = 0,dowenLeftCounter = 0;
	if (PPMIn[2] < -STIK_LIMIT){
		if (PPMIn[3] < -STIK_LIMIT){ 
			dowenRightCounter++;
		} else {
			dowenRightCounter = 0;
		}
		if (PPMIn[3] > STIK_LIMIT){ 
			dowenLeftCounter++;
		} else {
			dowenLeftCounter = 0;
		}
	} else {
		dowenRightCounter = 0;
		dowenLeftCounter = 0;
	}
	
	if (dowenRightCounter > TIME_OF_COMMAND ) {
		_isEngeinsOnFlag = 1;
	}
	
	if (dowenLeftCounter > TIME_OF_COMMAND ) {
		_isEngeinsOnFlag = 0;
	}
}