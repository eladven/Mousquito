//******************************************************************************************
// 
// This modul read the signal from the RC reciver.
// This modul use timer/counter1 and external interrupt int 7  - PE7.
//
//******************************************************************************************

#ifndef PPMIN_H
#define PPMIN_H

#define PPM_IN_OK 0
#define No_RF_SIGNAL 1
#define NO_RECIVER_CONECCTION 2

void  InitppmIn(void);
uint8_t  GetPPMIn(uint16_t * chanel); // get for RC chanels. any chanel domain is -128..128 +-5 lsbs. 
                                      // the value that returns idicate the RC status:PPM_IN_OK/No_RF_SIGNAL/NO_RECIVER_CONECTION


#endif 