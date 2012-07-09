
 #ifndef IMU_H
 #define IMU_H
 
 // Slaves Sensors Address
#define TWI_magnetometerAdd 0x0C
#define TWI_accelerometerAdd 0x38
#define TWI_gyroscopeAdd 0x68


void InitIMU(void);
void GetIMUData(int16_t *IMUData);
void GetIMUBiases(int16_t *biases);



#endif
