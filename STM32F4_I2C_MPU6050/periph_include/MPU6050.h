#ifndef MPU6050
#define MPU6050

#include "stm32f411xe.h"  
 
#define MPU6050_ADDR 0xD0
#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define FILTER_CONFIG_REG 0x1A
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75



typedef struct{
 volatile float roll;
 volatile float pitch;
 volatile float yaw;
 volatile int16_t Ax, Ay, Az, Gx, Gy, Gz, Temp_Raw;
 volatile float cAx, cAy, cAz, cGx, cGy, cGz;


 volatile float  acc_roll, acc_pitch, acc_total_vector, angle_roll, angle_pitch;
 volatile double gyro_roll, gyro_pitch;
 volatile double gyro_yaw;
 volatile double anglepitchout, anglerollout;
 volatile char   set_gyro_angles;

}mpu6050type;


void MPU_read(uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size);
void MPU_write(uint8_t Address, uint8_t Reg, uint8_t Data);
void MPU6050_init (void);
void MPU6050_callibration(mpu6050type *mpu, int iteration);
void MPU_read_eulers(mpu6050type *mpu,float *roll, float *pitch, float *yaw);

 
#endif
