//******************************************************************************************
// 
// This modul pepuse is to timing the system
//
//******************************************************************************************
#ifndef TIMER0_H
#define TIMER0_H


#define SYNC_PERIOD  100 // [100 usec]  10000/40 = 250 Hz

void InitTimer0(void);
uint16_t GetMicros(void); //returns time [100 usec]
uint16_t GetMillis(void); //returns time [ msec]
uint16_t GetSecs(void); //returns time [sec]
uint8_t IsNewPeriod(void); //  every SYNC_PERIOD  , this function returns 1 only for one time.


#endif 