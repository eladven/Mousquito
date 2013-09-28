#include "fixMath.h"
#include "main.h"
#include "math.h"


#define PI_DIV_2 1570
#define PI       3141

int16_t  _arcTan[1900];
uint16_t  _arcTanSlopesLimits[10];

void initFixMath(){
	//	between 0 - 1899 (/1000) use fix table
	for (int i=0;i<1900;i++){
		_arcTan[i] = atan(i/1000.0)*1000;
	}
	
	// for the others angles, use liner approximation in section where the 
	// vertical difference is 50 (/1000) [rad].
	
	//1000*atan(1899/1000) = 1086
	int16_t limit = 1100;
	int16_t index = 0;
	for (uint16_t i=1900;((i<50000)|| (index == 11));i++){
		if (atan(i/1000.0)*1000 > limit){
			limit = limit + 50;
			_arcTanSlopesLimits[index++] = i;
		}
		if (i > 5000)
			i += 4;
		else if (i > 10000)
			i += 24;
		else if (i > 15000)
			i += 99;
	}

	PrintEndl() ;
	PrintString("INIT FIX MATH") ;  
	PrintEndl() ;  
}

int32_t fixMult(int32_t arg1,int32_t arg2){
	return arg1 * arg2 / DENOMINATOR;
}

int16_t fixDiv(int16_t arg1,int16_t arg2){
	return ((arg1 * (int32_t)DENOMINATOR)/ arg2) ;
}


int16_t fixArcTanPositive(int16_t arg){
	if (arg < 1900)
		return _arcTan[arg];
	if (arg < _arcTanSlopesLimits[0])
		return (1086 + ((arg - 1900) * 50) / (_arcTanSlopesLimits[0] - 1900) );
	for (int i=0;i<10;i++){
		if (arg < _arcTanSlopesLimits[i])
			return (1100 + 50*(i-1) + ((arg - _arcTanSlopesLimits[i-1]) * (int32_t)50) / (_arcTanSlopesLimits[i] - _arcTanSlopesLimits[i-1]) );
	}
	return PI_DIV_2; 
}

int16_t fixArcTan(int16_t arg){
	if (arg < 0)
		return -fixArcTanPositive(-arg);
	return fixArcTanPositive(arg);
}

int16_t fixATan2(int16_t y,int16_t x){   
	if (x == 0){
		PrintString(" x = 0") ;      
        PrintEndl() ;    
		if (y > 0) 
			return PI_DIV_2;
		else if (y < 0)
			return -PI_DIV_2;
		else
			return 0;
	}
	if (x > 0)
		return fixArcTan(fixDiv(y,x));
	// x < 0
	if (y < 0)
		return fixArcTan(fixDiv(y,x)) - PI;
	// x < 0, y > 0
		return fixArcTan(fixDiv(y,x)) + PI;
}
