#include "stm32f4xx.h"
#include "i2c.h"
#include "MPU6050.h"
#include "math.h"


void MPU_write(uint8_t Address, uint8_t Reg, uint8_t Data)
{
	i2c_start ();
	i2c_address (Address);
	i2c_write (Reg);
	i2c_write (Data);
	i2c_stop ();
}

void MPU_read(uint8_t Address, uint8_t Reg, uint8_t *buffer, uint8_t size)
{
	i2c_start ();
	i2c_address (Address);
	i2c_write (Reg);
	i2c_start ();  			// repeated start
	i2c_read (Address+0x01, buffer, size);
	i2c_stop ();
}



void MPU6050_init (void)
{
	uint8_t check;
	uint8_t Data;

	// check device ID WHO_AM_I

	MPU_read(MPU6050_ADDR,WHO_AM_I_REG, &check, 1);

	if (check == 104)  // 0x68 will be returned by the sensor if everything goes well
	{
		// power management register 0X6B we should write all 0's to wake the sensor up
		Data = 0;
		MPU_write (MPU6050_ADDR, PWR_MGMT_1_REG, Data);

		// Set accelerometer configuration in ACCEL_CONFIG Register
		// FS_SEL=2 -> ? 8g
		Data = 0x10;
		MPU_write(MPU6050_ADDR, ACCEL_CONFIG_REG, Data);

		// Set Gyroscopic configuration in GYRO_CONFIG Register
		// FS_SEL=1 -> ? 500 ?/s
		Data = 0x08;
		MPU_write(MPU6050_ADDR, GYRO_CONFIG_REG, Data);
		
		Data = 0x01;
		MPU_write(MPU6050_ADDR,FILTER_CONFIG_REG, Data);
	}
}

void MPU_read_eulers(mpu6050type *mpu,float *roll, float *pitch, float *yaw)
{

	uint8_t Rx_data[14];
	
	MPU_read (MPU6050_ADDR, ACCEL_XOUT_H_REG, Rx_data, 14);
	
	mpu->Ax = (int16_t)(Rx_data[0] << 8 | Rx_data [1]);
	mpu->Ay = (int16_t)(Rx_data[2] << 8 | Rx_data [3]);
	mpu->Az = (int16_t)(Rx_data[4] << 8 | Rx_data [5]);
	mpu->Temp_Raw  = (int16_t)(Rx_data[6] << 8 | Rx_data [7]);
	mpu->Gx  = (int16_t)(Rx_data[8] << 8 | Rx_data [9]);
	mpu->Gy  = (int16_t)(Rx_data[10] << 8 | Rx_data [11]);
	mpu->Gz  = (int16_t)(Rx_data[12] << 8 | Rx_data [13]);
	
	if(mpu->Gz<75 && mpu->Gz>-75)
	{
		mpu->Gz=mpu->cGz;
	}
	
	
	//obtaining gyro angles
	
	mpu->gyro_roll += (mpu->Gy-mpu->cGy)*0.0000763; 			//5000 us == 0.005 second --> degree/second
	mpu->gyro_pitch += (mpu->Gx-mpu->cGx)*0.0000763;
	mpu->gyro_yaw += (mpu->Gz-mpu->cGz)*0.0000763;
	
	
	mpu->gyro_pitch += mpu->gyro_roll * sin((mpu->Gz-mpu->cGz) * 0.000001331);               //If the IMU has yawed transfer the roll angle to the pitch angel
	mpu->gyro_roll -= mpu->gyro_pitch * sin((mpu->Gz-mpu->cGz) * 0.000001331);               //If the IMU has yawed transfer the pitch angle to the roll angel
	

	//obtaining acc angles
	mpu->acc_total_vector = sqrtf(((mpu->Ax)*(mpu->Ax)) + ((mpu->Ay)*(mpu->Ay)) + ((mpu->Az)*(mpu->Az)));
	//57.296 = 1 / (3.142 / 180)
	mpu->acc_pitch = asinf((float)mpu->Ay / mpu->acc_total_vector) * 57.296;
	mpu->acc_roll = asinf((float)mpu->Ax / mpu->acc_total_vector) * -57.296;


	

	if(mpu->set_gyro_angles)
	{                                                                             //If the IMU is already started
		mpu->gyro_pitch = mpu->gyro_pitch * 0.9995 + mpu->acc_pitch * 0.0005;     //Correct the drift of the gyro pitch angle with the accelerometer pitch angle
		mpu->gyro_roll = mpu->gyro_roll * 0.9995 + mpu->acc_roll * 0.0005;        //Correct the drift of the gyro roll angle with the accelerometer roll angle
	}
	else
	{                                                                         //At first start
		mpu->gyro_pitch = mpu->acc_pitch;                                     //Set the gyro pitch angle equal to the accelerometer pitch angle
		mpu->gyro_roll = mpu->acc_roll;                                       //Set the gyro roll angle equal to the accelerometer roll angle
		mpu->set_gyro_angles = 1;                                             //Set the IMU started flag
	}
	
	mpu->anglepitchout = mpu->anglepitchout * 0.9 + mpu->gyro_pitch * 0.1;   //Basic filtering
	mpu->anglerollout = mpu->anglerollout * 0.9 + mpu->gyro_roll * 0.1;
	
	*roll = mpu->anglerollout;
	*pitch = mpu->anglepitchout;
	*yaw = mpu->gyro_yaw;
		
}


void MPU6050_callibration(mpu6050type *mpu, int iteration)
{
	uint8_t Rx_data[14];


	for(int i =0 ; i<iteration ; i++)
	{
		MPU_read(MPU6050_ADDR, ACCEL_XOUT_H_REG, Rx_data, 14);

		mpu->Ax = (int16_t)(Rx_data[0] << 8 | Rx_data [1]);
		mpu->Ay = (int16_t)(Rx_data[2] << 8 | Rx_data [3]);
		mpu->Az = (int16_t)(Rx_data[4] << 8 | Rx_data [5]);
		mpu->Temp_Raw  = (int16_t)(Rx_data[6] << 8 | Rx_data [7]);
		mpu->Gx  = (int16_t)(Rx_data[8] << 8 | Rx_data [9]);
		mpu->Gy  = (int16_t)(Rx_data[10] << 8 | Rx_data [11]);
		mpu->Gz  = (int16_t)(Rx_data[12] << 8 | Rx_data [13]);

		mpu->cAx += mpu->Ax;
		mpu->cAy += mpu->Ay;
		mpu->cAz += mpu->Az;
		mpu->cGx += mpu->Gx;
		mpu->cGy += mpu->Gy;
		mpu->cGz += mpu->Gz;
	}

	mpu->cAx/= (iteration);
	mpu->cAy/= (iteration);
	mpu->cAz/= (iteration);
	mpu->cGx/= (iteration);
	mpu->cGy/= (iteration);
	mpu->cGz/= (iteration);

}

