#include <avr/io.h>
#include <string.h>

#include "main.h"
#include "fc.h"
#include "uart.h"

uint8_t _isEngeinsOnFlag = 0;
uint8_t _control =  RF_CONTROL;


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
int16_t _constants[3][4] = {{10 ,0    ,0   ,0},   //factor
							{0    ,400 ,400 ,0},    //p
							{0    ,100 ,100 ,0}};  //d
							
void setConst(int16_t i,int16_t j,int16_t val){
	_constants[i][j] = val;
}


int16_t getConst(int16_t i,int16_t j){
	return _constants[i][j];
}

void setEngeinsOn(int8_t val){
	_isEngeinsOnFlag = val;
}	

int8_t getEngeinsOn(){
	return _isEngeinsOnFlag;
}	
////////////////////////////////////////////////////////////////////

           //   motors:        0         1          2         3
int16_t mixer[4][4] = {{MIXER_VAL ,MIXER_VAL ,MIXER_VAL ,MIXER_VAL  },     //u1 colective
                        {-MIXER_VAL,0         ,MIXER_VAL ,0         },     //u2 roll
						{ 0        ,MIXER_VAL, 0        ,-MIXER_VAL },     //u3 pitch
						{MIXER_VAL ,-MIXER_VAL,MIXER_VAL ,-MIXER_VAL}};    //u4 yaw

void setControl(uint8_t control){
	_control = control;
}

uint8_t getControl(){
	return _control;
}

void fc(int16_t*  IMUData,double*  angle,int16_t*  PPMIn,int16_t*  PPMOut){
	switch (_control){
		case RF_CONTROL: doFlightControl(IMUData,angle,PPMIn,PPMOut); break; 
		case MANUEL_CONTROL: break; 
		case UART_CONTROL: doFlightControl(IMUData,angle,getUartPPMIn(),PPMOut); break; 
	}
}



void doFlightControl(int16_t*  IMUData,double*  angle,int16_t*  PPMIn,int16_t*  PPMOut){
	updateYaw(PPMIn[3]*_constants[0][3]/15);
	if (_isEngeinsOnFlag){
		// lets define four control signals:
		// u0 to control the colective 
		// u1 to control the roll
		// u2 to control the pitch
		// u3 to conrol the yaw.
		int16_t u[4];
		u[0] = (PPMIn[2]+145)*_constants[0][0];  //colective
		u[1] = PPMIn[1]*_constants[0][1]+(_constants[1][1]*angle[0]+_constants[2][1]*angle[3]); //roll
		u[2] = PPMIn[0]*_constants[0][2]+(_constants[1][2]*angle[1]+_constants[2][2]*angle[4]); //pitch
		u[3] =                            (_constants[1][3]*angle[2]+_constants[2][3]*angle[5]); //yaw
			
			
			
		setOutputsData(u,20,23);
		////////////////////////////////////////////
		for (int i=0;i<4;i++){
			PPMOut[i] = 0;
			for (int j=0;j<4;j++){
				PPMOut[i] += mixer[j][i]*u[j];
			}
			PPMOut[i] = PPMOut[i]/10;
			SPEED_LIMITER(PPMOut[i]);
		}
	} else {
		for (int i=0;i<4;i++)
			PPMOut[i] = 0;
	}
	setOutputsData(PPMIn,12,15);
	setOutputData(_control,32);
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
		resetYaw();
	}
	
	if (dowenLeftCounter > TIME_OF_COMMAND ) {
		_isEngeinsOnFlag = 0;
	}
}