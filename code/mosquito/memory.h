
 #ifndef MEMORY_H
 #define MEMORY_H
 
void writeToMemory(int16_t* values,uint16_t address,uint16_t length);
void readFromMemory(int16_t* values,uint16_t address,uint16_t length);

#define NUM_OFBIASES_SETS 3
extern const uint16_t IMU_BIASES_ADRESS[3];
#endif
