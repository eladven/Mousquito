#include <avr/io.h>
#include <string.h>

#include "main.h"
#include "fc.h"
#include "uart.h"

uint8_t _isEngeinsOnFlag = 0;
uint8_t _isMenualControl = 0;

//////    structure:   
//                
//                0
//                ^
//                | x
//                |    y
//       1<------- ------->3
//                |
//                |
//                v
//                2
//
//////////////////////////////////////////////////////

           //   motors:  0   1   2   3
int16_t mixer[4][4] = {{10 ,10 ,10 ,10 },     //u1 colective
                        {0  ,-10,0  ,10 },     //u2 roll
						{10 ,0  ,10 ,0  },     //u3 pitch
						{10 ,-10,10 ,-10}};    //u4 yaw

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
			u[0] = PPMIn[2]*COLECTIVE_FACTOR;
			u[1] = PPMIn[1]*ROLL_FACTOR+P_ROLL*angle[0]+D_ROLL*angle[3];
			u[2] = PPMIn[0]*PITCH_FACTOR+P_PITCH*angle[1]+D_PITCH*angle[4];
			u[3] = PPMIn[3]*YAW_FACTOR+P_YAW*angle[2]+D_YAW*angle[5];
			
			for (int i=0;i<4;i++){
				PPMOut[i] = 0;
				for (int j=0;j<4;j++)
					PPMOut[i] = mixer[i][j]*u[j];
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