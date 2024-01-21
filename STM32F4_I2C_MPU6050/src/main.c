
/* Includes */
#include "stm32f4xx.h"
#include "System_clock.h"
#include "i2c.h"
#include "MPU6050.h"


float roll, pitch, yaw;
int iteration = 500;
mpu6050type mpu6050;

int main(void)
{
	clock_init();
	i2c_init();
	mpu6050.set_gyro_angles = 0;
	MPU6050_init();
	MPU6050_callibration(&mpu6050, iteration);
	systick_init();

	int i = 0;

	while (1)
	{
		i++;
	}
}
