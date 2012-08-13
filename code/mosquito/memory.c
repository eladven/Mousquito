#include "main.h"
#include "memory.h"
#include <avr/eeprom.h> 

extern const uint16_t IMU_BIASES_ADRESS[3] = {0,18,36};
   
void writeToMemory(int16_t* values,uint16_t address,uint16_t length){
	for (uint16_t i = 0;i<length;i ++){
		eeprom_write_word ((uint16_t*)(2*i + address), values[i]);
	}
} 

void readFromMemory(int16_t* values,uint16_t address,uint16_t length){
	for (uint16_t i = 0;i<length;i ++){
		values[i] = eeprom_read_word ((uint16_t*)(2*i + address));
	}
} 

