#include <avr/io.h>
#include <string.h>

#include "main.h"
#include "fc.h"
#include "uart.h"

uint8_t _isEngeinsOnFlag = 0;
uint8_t _isMenualControl = 0;

void setMenualControl(uint8_t isMenualControl){
	_isMenualControl = isMenualControl;
}

void fc(int16_t*  IMUData,double*  angle,int16_t*  PPMIn,int16_t*  PPMOut){
	getRCCommands(PPMIn);
	if (!_isMenualControl){
		if (_isEngeinsOnFlag){
			for (int i=0;i<4;i++)
			PPMOut[i] = 70;
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