#include <avr/io.h>
#include <string.h>

#include "main.h"
#include "uart.h"
#include "timer0.h"



// local variables for this modul:
uint8_t   Transmiting = FALSE;
uint8_t   ReciveBuff   [BUFF_LENGTH];
uint8_t   TransmitBuff [BUFF_LENGTH];
uint8_t   TransmitBuffIndex = 1;  
uint8_t   ReciveBuffIndex = 0;  


int16_t intsqt = -1;  //this variables use for transmit data  between SyncOut and HndelData. their value isthe 
float sqt = -1;       // incoming argument, -1 means that it is not in use


// local functions  for this modul:
void HendelNewCommand(void);

void InitUART(void)
{
	// Enable receive and transmitte
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR1C = (0<<USBS1)|(1<<UCSZ11)|(1<<UCSZ10);
	// Set baud rate.
	UBRR1H = 0;
	UBRR1L = (SYSCLK / (BAUD_RATE * 8L) - 1);
	//duoble the speed
	UCSR1A |= (1 << U2X1); 
	// enable RX-Interrupt  enable TX-Interrupt
	UCSR1B |= (1 << RXCIE1);
	UCSR1B |= (1 << TXCIE1);
	// set TX port as output, RX port as input
	DDRD |= (1<<3);
	DDRD |= (0<<2);
}
//****************************************************************/
// hendel recive interrupt.
// gets only a-z , ' ' ,0-9 and '.' chars. when get them 
// print then on the user screen and save them in ReciveBuff
// when get a Enter char, procces the data at HendelNewCommand();
//***************************************************************/

ISR(USART1_RX_vect)
{
	char c = UDR1;  //get data
    //static char st[2]="a";
    if (c!=13)   //if its not Enter signal
    {
		if ( (  (c >= 'a' )  && (c<='z') ) || ( (c>='0') && (c<='9') ) || (c==' ')  || (c=='.')) //if we accept this char
		{  
		    ReciveBuff[ReciveBuffIndex++]=c;  //push yhe new data to buffer
            PrintChar(c) ;   
        }
    }
    else  // get Enter
    {
		ReciveBuff[ReciveBuffIndex]=0;  //mark the buffer
		ReciveBuffIndex = 0 ;    //reset the index
		PrintEndl();     //print /n
		HendelNewCommand();
    }
}

//***************************************************************************/
// hendel transmit interrupt
// transmit the TransmitBuff char by char (index by TransmitBuffIndex)
// after finsh transmit new char the interrupt USART1_TX_vect
// will do it for the new char. it will continue till the end
// of the buffer or '/0' char. when finish change Transmiting flg to false.
// to start transmiting sequence just set Transmiting flg to true
// and set UDR1 to TransmitBuff[0].
//**************************************************************************/
ISR(USART1_TX_vect)
{
	uint8_t c;
	if(Transmiting) 
	{       
		c = TransmitBuff[TransmitBuffIndex++];
		// when get terminating char or when the buffer is full.
		if((c == 0) || (TransmitBuffIndex >= BUFF_LENGTH-1))
		{
			TransmitBuffIndex = 1; // reset index
			Transmiting = FALSE; // stop next move 
			TransmitBuff[0]=0;
		}
		UDR1 = c; // transmit the next char, when this transmit will finish, it will simuli TX interrupt, so it will continue.
	}
	// finish transmission 
	else TransmitBuffIndex = 1;
}

//**************************************************************
//  this function use fot print chars
//**************************************************************
int  PrintChar(char c)
{            
    if( (2+TransmitBuffIndex) > (BUFF_LENGTH)) //if the buffer is  full return 0.
    {
        return 0;
    }
    int i;
    for (i=0; TransmitBuff[i];i++); // find the buffers edge 
	TransmitBuff[i] = c;
	TransmitBuff[i+1] = 0;   
	if(!Transmiting)    // if we are transmiting , the transmiting will continue by interrupts till the hole 
	                     //  buffer will transmit . if we are not transmiting , start the transmiting.
	{
		Transmiting = TRUE;
        UDR1 = TransmitBuff[0]; 
	}  
	return 1;
}


//**************************************************************
//  this function use fot print strings.
//**************************************************************
int  PrintString(char *st)
{            
    if( (strlen(st)+TransmitBuffIndex) > (BUFF_LENGTH-1)) //if the buffer is  full return 0.
    {
        return 0;
    }
    int i,j;
    for (i=0; TransmitBuff[i];i++); // find the buffers edge 
    for (j=0;st[j];j++)
         TransmitBuff[i+j] = st[j];   
   TransmitBuff[i+j] = 0;   
   if(!Transmiting)     // if we are transmiting , the transmiting will continue by interrupts till the hole 
	                     //  buffer will transmit . if we are not transmiting , start the transmiting.
   {
        Transmiting = TRUE;
        UDR1 = TransmitBuff[0]; 
   }  
   return 1;
}

//**************************************************************
//  this function use print end line char. 
//**************************************************************
int  PrintEndl(void)
{            
    if( (2+TransmitBuffIndex) > BUFF_LENGTH) //if the buffer is  full return 0.
    {
        return 0;
    }
    int i;
    for (i=0; TransmitBuff[i];i++); // find the buffers edge 
     TransmitBuff[i] ='\n';       // write into bufer /n sequence 
     TransmitBuff[i+1] =13; 
     TransmitBuff[i+2] = 0;   
     if(!Transmiting)   // if we are transmiting , the transmiting will continue by interrupts till the hole 
	                     //  buffer will transmit . if we are not transmiting , start the transmiting.
     {
          Transmiting = TRUE;
          UDR1 = TransmitBuff[0]; 
    }  
    return 1;
}

//**************************************************************
//  this function use fot print integers.
//**************************************************************
int  PrintInt(int  num)
{       
    char st[25];
    sprintf(st,"%d",num);  
    if( (strlen(st)+TransmitBuffIndex) > BUFF_LENGTH) //if the buffer is  full return 0.
    {
        return 0;
    }
    int i,j;
    for (i=0; TransmitBuff[i];i++); // find the buffers edge 
    for (j=0;st[j];j++)             //copy the new int to the buffer
         TransmitBuff[i+j] = st[j];   
   TransmitBuff[i+j] = 0;   
   if(!Transmiting)          // if we are transmiting , the transmiting will continue by interrupts till the hole 
                             //  buffer will transmit . if we are not transmiting , start the transmiting.
   {
        Transmiting = TRUE;
        UDR1 = TransmitBuff[0]; 
   }  
   return 1;
}


//**************************************************************
//  this function use to print arry of 12 integers. it take group of
//  three integer (2*3 = 6 bytes =48 bits) , and partition it to eight segments of  bits 
//  to each segment  , it add the number 'A' (65) and print it ascii value.
//  at this way, this function print 12 integers by print 8*4 = 32 bytes, which all of them
//  are PRINTBLE ,so the application that handle that data , can use textual solutions.
//**************************************************************
int  PrintArray(int16_t *num)
{    
    if( (TransmitBuffIndex + 32) > BUFF_LENGTH) //if the buffer is  full return 0.
    {
        return 0;
    }
    int i =0;
    for (i=0; TransmitBuff[i];i++); // find the buffers edge 
	for (int j=0;j<4;j++) //4x3 = 12 
	{  // make any 3 int (3*2 = 6 bytes) into a 8 byte , by concat any 6 bit one after one
		TransmitBuff[i++] = ((uint16_t)num[j*3]>>10) + 'A';
		TransmitBuff[i++] = (((uint16_t)num[j*3]>>4) & 0x003F) + 'A';
		TransmitBuff[i++] = (((num[j*3]<<2) | ((uint16_t)num[j*3 + 1]>>14) ) & 0x003F )+ 'A';
		TransmitBuff[i++] = (((uint16_t)num[j*3 +1]>>8) & 0x003F )+ 'A';
		TransmitBuff[i++] = (((uint16_t)num[j*3+1]>>2) & 0x003F )+ 'A';
		TransmitBuff[i++] = (((num[j*3+1]<<4) | ((uint16_t)num[j*3+2]>>12)) & 0x003F ) + 'A';
		TransmitBuff[i++] = (((uint16_t)num[j*3+2]>>6) & 0x003F)+ 'A';
		TransmitBuff[i++] = (num[j*3+2] & 0x003F) + 'A';
	}
    TransmitBuff[i] = 0;  
   if(!Transmiting)          // if we are transmiting , the transmiting will continue by interrupts till the hole 
                             //  buffer will transmit . if we are not transmiting , start the transmiting.
   {
        Transmiting = TRUE;
        UDR1 = TransmitBuff[0]; 
   }  
   return 1;
}



//******************************************************************
//  this function is called automticly when the user press Enter
//  first it saprate it to oprands (words that have ' ' (space char)
//  beetwin. then it handel each comand according to it operands.
//******************************************************************
void HendelNewCommand(void)
{
    char operands [6][20];  // evrey word between spaces is operand. max 5 operands with 20 charecters each.
    int j=0,numOfOperands = 0;
    for (int i=0;ReciveBuff[i] && i < BUFF_LENGTH;i++)
    {
        if (ReciveBuff[i]==' ')
        {
            operands [numOfOperands][j]=0;
            if (numOfOperands <5)
				numOfOperands++;
                j=0;
        }
        else 
        {
            operands [numOfOperands][j] = ReciveBuff[i];
            if (j<20)
            j++;
        }
    }
    operands[numOfOperands][j]=0;  //for the last operand (before the ENTER)
    if (numOfOperands <5)
    numOfOperands++;
	
	//*******************************************
	//  start react to commands
	//*******************************************
                   
    if (strcmp(operands[0],"whoareyou") ==0)
    {
        if (numOfOperands==1)
        {
            PrintString("I AM FLYING MOSQUITO") ;    
            PrintEndl() ;      
        }    
        else
        {
            int tmp=-1;
            sscanf(operands[1],"%d",&tmp);   
            if (tmp == 1)
            {
                PrintString("CREATED BY ELAD VENEZIAN") ;    
                PrintEndl() ;
            }  
			if (tmp == 2)
            {
                PrintString("CREATED BY MAOR MEVORACH") ;    
                PrintEndl() ;
            }  
        }        
    }
	
	
}


//******************************************************************************************//
// this function is calld by the main periodicly. it perpuse is to handel all the data
// that the program shold plot out periodicly.
//******************************************************************************************//

void SyncOut(int16_t *IMUData,double *angle,int16_t *PPMIn)
{
	int16_t  dataOut[12]; //the data that will synchronisly print out
	static int16_t prevMillis = 0;
	int16_t millis = GetMillis();
	if ( (millis - prevMillis) < 0)
		prevMillis = prevMillis - 1000;
	if ( (millis - prevMillis) > 100)
	{
		prevMillis = millis;
		dataOut[0] = (angle[0]*180)/3.14;
		dataOut[1] = (angle[1]*180)/3.14;
		dataOut[2] = (angle[2]*180)/3.14;
		/*
		PrintString("SYNCOUT");
		PrintArray(dataOut);	
		PrintString("ENDL");
		*/
		
		
		PrintString("radio  ");
		for(uint8_t i=0;i<4;i++)
		{	
			PrintInt(PPMIn[i]);
			PrintString("  ");
			
		}
		PrintString("angles ");
		PrintInt((angle[0]*180)/3.14);
		PrintString("  ");
		PrintInt((angle[1]*180)/3.14);
		PrintString("  ");
		PrintInt((angle[2]*180)/3.14);
		PrintString("  ");

		PrintEndl();
		
	}
}

