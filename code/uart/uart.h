//******************************************************************************************
// 
// This modul handel all uart. the uart use for ground communication.
// this model sopplaied printing metods for the program 
// and also it sopplid directied commands services for the user (infront of hyper terminl)
// when the user type commands on his hyper terminal, the commands are executed at the 
// local function HendelNewCommand(). this function gets the user command , procces it, 
// and executed it immidiatly.
//
//******************************************************************************************
#ifndef UART_H
#define UART_H

#define BAUD_RATE 115200
#define BUFF_LENGTH 250


void InitUART(void); // method for initilazed the UART modul.
// method to print to the uart. If This methods work indpendently by interrupts. if thay can't print the specified data 
// thay will return zero value. else thay will return the value 1.
int  PrintChar(char c);
int  PrintString(char *st);
int  PrintEndl(void);
int  PrintInt(int  num);
int  PrintArray(int16_t *num); // this method get array of 12 integers and print the data in a way that 
// the data is printable 
void SyncOut(int16_t *IMUData,double *angle,int16_t *PPMIn); // plot data periodicly.



#endif 
