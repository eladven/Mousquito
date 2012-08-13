#include <avr/io.h>
#include <string.h>

#include "main.h"
#include "uart.h"
#include "timer0.h"
#include "fc.h"
#include "imu.h"
#include "memory.h"


// local variables for this modul:
uint8_t   Transmiting = FALSE;
uint8_t   ReciveBuff   [BUFF_LENGTH];
uint8_t   TransmitBuff [BUFF_LENGTH];
uint8_t   TransmitBuffIndex = 1;  
uint8_t   ReciveBuffIndex = 0;

//output data  
volatile int16_t _outputData[OUTPUT_BUFFER_SIZE];

// output flags.
uint8_t syncOutCoded = 0;
uint8_t syncOutUnCoded = 0;
uint8_t menualcontrol = 0;

char  _dataLabels[36][20] = {"acc X",  //0
						"acc_Y",
						"acc_Z",
						"gyro_X",
						"gyro_Y",
						"gyro_Z",
						"mag_X",
						"mag_Y",
						"mag_Z",
						"roll",
						"pitch", //10
						"yaw",
						"ppmIn_0",
						"ppmIn_1",
						"ppmIn_2",
						"ppmIn_3",
						"ppmOut_0",
						"ppmOut_1",
						"ppmOut_2",
						"ppmOut_3",
						"20", //20
						"21",
						"22",
						"23",
						"24",
						"25",
						"26",
						"27",
						"28",
						"29",
						"30", //30
						"31",
						"menual_Control",
						"engeins_On",
						"34",
						"35"
							};


// local functions  for this modul:
void HendelNewCommand(void);


void setOutputData(int16_t data,uint8_t index){
	_outputData[index] = data;
}

void setOutputsData(int16_t* data,uint8_t from,uint8_t to){
	uint8_t j=0;
	for(uint8_t i=from;i<=to;i++){
		setOutputData(data[j],i);
		j++;
	}	
}


int16_t getOutputData(uint8_t index){
	return _outputData[index];
}

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
    if( (2+TransmitBuffIndex) >= (BUFF_LENGTH-1)) //if the buffer is  full return 0.
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
    if( (strlen(st)+TransmitBuffIndex) >= (BUFF_LENGTH-1)) //if the buffer is  full return 0.
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
    if( (2+TransmitBuffIndex) >= BUFF_LENGTH-1) //if the buffer is  full return 0.
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
    if( (strlen(st)+TransmitBuffIndex) >= BUFF_LENGTH-1) //if the buffer is  full return 0.
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
    if( (TransmitBuffIndex + 85) >= BUFF_LENGTH-1) //if the buffer is  full return 0.
    {
        return 0;
    }
    int i =0;
    for (i=0; TransmitBuff[i];i++); // find the buffers edge 
	TransmitBuff[i++] = 'O';
	TransmitBuff[i++] = 'U';
	TransmitBuff[i++] = 'T';
	for (int j=0;j<12;j++) //4x12 = 36 
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
	TransmitBuff[i++] = 'Z';
	TransmitBuff[i++] = 'Z';
	TransmitBuff[i++] = 'Z';
	TransmitBuff[i++] = '\n';
	TransmitBuff[i++] = 13;
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
	
	if (strcmp(operands[0],"syncout") ==0)
    {
        if (numOfOperands==1)
        {
            PrintString("SYNCOUT ") ;  
			PrintInt(syncOutUnCoded) ;     
            PrintEndl() ;      
        }    
        else
        {
            int tmp=-1;
            sscanf(operands[1],"%d",&tmp);   
            if (tmp == 0)
            {
                PrintString("SYNCOUT 0") ;    
                PrintEndl() ;
				syncOutUnCoded = 0;
            }  
			if (tmp == 1)
            {
                PrintString("SYNCOUT 1") ;    
                PrintEndl() ;
				syncOutUnCoded = 1;
            }  
        }        
    }
	if (strcmp(operands[0],"syncoutcoded") ==0)
    {
        if (numOfOperands==1)
        {
            PrintString("SYNCOUTCODED ") ;  
			PrintInt(syncOutCoded) ;     
            PrintEndl() ;      
        }    
        else
        {
            int tmp=-1;
            sscanf(operands[1],"%d",&tmp);   
            if (tmp == 0)
            {
                PrintString("SYNCOUTCODED 0") ;    
                PrintEndl() ;
				syncOutCoded = 0;
            }  
			if (tmp == 1)
            {
                PrintString("SYNCOUTCODED 1") ;    
                PrintEndl() ;
				syncOutCoded = 1;
            }  
        }        
    }
	
	if (strcmp(operands[0],"getdatalabel") ==0)
    {
		int tmp=-1;
        sscanf(operands[1],"%d",&tmp);  
        if ((tmp >= 0) && (tmp <= 35)){
            PrintString("GETDATALEBEL ") ;
			PrintInt(tmp);
			PrintString(" ");
			PrintString(_dataLabels[tmp]);
            PrintEndl() ;
           }  
    }
	
	if (strcmp(operands[0],"motor") ==0)
    {
		int index=0,value=0;
		if (menualcontrol){
			sscanf(operands[1],"%d",&index);  
			sscanf(operands[2],"%d",&value);  
			PrintString("MOTOR ") ;
			PrintInt(index);
			PrintString(" ");
			PrintInt(value);
			PrintEndl();
			setPPMOut(index,value);
		}else {
			PrintString("MOTOR - NOT MENUAL CONTROL") ;
			PrintEndl();
		}
    }
	
	if (strcmp(operands[0],"menualcontrol") ==0)
    {
        if (numOfOperands==1)
        {
            PrintString("MENUALCONTROL ") ;  
			PrintInt(menualcontrol) ;     
            PrintEndl() ;      
        }    
        else
        {
            int tmp=-1;
            sscanf(operands[1],"%d",&tmp);   
            if (tmp == 0)
            {
                PrintString("MENUALCONTROL 0") ;    
                PrintEndl() ;
				menualcontrol = 0;
				setMenualControl(0);
            }  
			if (tmp == 1)
            {
                PrintString("MENUALCONTROL 1") ;    
                PrintEndl() ;
				menualcontrol = 1;
				setMenualControl(1);
            }  
        }        
    }
	if (strcmp(operands[0],"setconst") ==0)
    {
        if (numOfOperands==4)
        {
			int i=-1,j=-1,val=-1;
            sscanf(operands[1],"%d",&i);   
			sscanf(operands[2],"%d",&j);   
			sscanf(operands[3],"%d",&val);   
            PrintString("SETCONST ") ;  
			PrintInt(i) ; 
			PrintString(" ") ;  
			PrintInt(j) ;    
			PrintString(" ") ;  
			PrintInt(val) ;            
            PrintEndl() ;      
			setConst(i,j,val);
        }    
    }
	if (strcmp(operands[0],"setimubias") ==0){
	    PrintString("SETIMUBIAS ") ;            
        PrintEndl() ;     
        SetIMUDataBias();
    }
	if (strcmp(operands[0],"saveimubias") ==0)
    {
        if (numOfOperands==2)
        {
			int i=-1;
            sscanf(operands[1],"%d",&i);   
			if (i>=0 && i < NUM_OFBIASES_SETS){
				PrintString("SAVEIMUBIAS ") ;  
				PrintInt(i) ;   
				PrintEndl() ;   
				writeToMemory(GetIMUBiases(),IMU_BIASES_ADRESS[i],6);
			}   
        }    
    }
	if (strcmp(operands[0],"loadimubias") ==0)
    {
        if (numOfOperands==2)
        {
			int i=-1;
            sscanf(operands[1],"%d",&i);   
			if (i>=0 && i < NUM_OFBIASES_SETS){
				PrintString("LOADIMUBIAS ") ;  
				PrintInt(i) ;   
				PrintEndl() ;   
				readFromMemory(GetIMUBiases(),IMU_BIASES_ADRESS[i],6);
		
			}   
        }    
    }
	if (strcmp(operands[0],"printimubias") ==0)
    {
		PrintString("PRINTIMUBIAS ") ;  
		PrintEndl() ;   
		PrintString("ACCX ACCY ACCZ GYROX GYROY GYROZ") ;  
		PrintEndl() ;
		for (uint8_t i=0;i<6;i++){
			PrintInt(GetIMUBiases()[i]) ;  
			PrintString(" "); 
		}
		PrintEndl() ;   
    }
	
	
}


//******************************************************************************************//
// this function is calld by the main periodicly. it perpuse is to handel all the data
// that the program shold plot out periodicly.
//******************************************************************************************//

void SyncOut()
{
	int16_t  dataOut[12]; //the data that will synchronisly print out
	
	int16_t millis = GetMillis();
	if ( IsNewOutputPeriod() )  {
		addIMUDataBias(); // add one bias sample if needed
		
		if (syncOutUnCoded) {
			PrintString("angles ");
			PrintInt(_outputData[12]);
			PrintString("  ");
			PrintInt(_outputData[13]);
			PrintString("  ");
			PrintInt(_outputData[14]);
			PrintString("  ");
			PrintInt(_outputData[15]);
			PrintString("  ");
			PrintEndl();	
		}
		if (syncOutCoded) {
			PrintArray(_outputData);
		}
	}
}

