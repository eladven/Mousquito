//******************************************************************************************
// 
// This modul send ppm signals to the ESC.
// This modul use timer/counter3 and Ports  - PE7.
//
//******************************************************************************************

#ifndef PPMOUT_H
#define PPMOUT_H


#define NUM_OF_PPM_CHANNELS 4
#define MIN_SPEED_GAP 8;

void  InitppmOut(void);
// update the speed for one the four motors.
// speed can be [0-249].
void updateSpeed(uint8_t _speed,uint8_t _channel);

#endif 