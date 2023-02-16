#include "stm32f10x.h"
#include "i2c.h"
void i2c_init(void)
{
	RCC->APB1ENR |= (1<<21);   //i2c1 hattininin clock unu aktif et
	RCC->APB2ENR |= (1<<0);    // Alternate function clock enable
	RCC->APB2ENR |= (1<<3);    // gpiob clock enable
	                            
	GPIOB->CRL |= (255<<24U);      //PB6 and PB7 open drain output
	
	I2C1->CR1 |= (1<<15);      // I2C is in under reset
	I2C1->CR1 &= ~(1<<15);     // I2C is not in under reset


	// Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	I2C1->CR2 |= (36<<0);  // PCLK1 FREQUENCY in MHz
	
	
	// Configure the clock control registers
	I2C1->CCR |= (180<<0);  // check calculation in PDF
	// Configure the rise time register
	I2C1->TRISE = 37;  // check PDF again

	// Program the I2C_CR1 register to enable the peripheral
	I2C1->CR1 |= (1<<10);  // Enable the ACK
	I2C1->CR1 |= (1<<0);  // Enable I2C

}

void i2c_start (void)
{
	I2C1->CR1 |= (1<<10);  			// Enable the ACK
	I2C1->CR1 |= (1<<8);  			// Generate START
	while (!(I2C1->SR1 & (1<<0)));  // Wait for SB bit to set
}

void i2c_write (uint8_t data)
{	
	while (!(I2C1->SR1 & (1<<7)));  // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & (1<<2)));  // wait for BTF bit to set
}

void i2c_address (uint8_t Address)
{
	I2C1->DR = Address;  					// send the address

	while (!(I2C1->SR1 & (1<<1)));  		// wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2;  	// read SR1 and SR2 to clear the ADDR bit
}

void i2c_stop (void)
{
	I2C1->CR1 |= (1<<9);  // Stop I2C
}

void i2c_write_multi (uint8_t *data, uint8_t size)
{	
	while (!(I2C1->SR1 & (1<<7)));  				// wait for TXE bit to set
	while (size)
	{
		while (!(I2C1->SR1 & (1<<7)));  			// wait for TXE bit to set
		I2C1->DR = (volatile uint32_t )*data++;  	// send data
		size--;
	}
	
	while (!(I2C1->SR1 & (1<<2)));  				// wait for BTF to set
}

char i2c_read_ack(void)
{
	I2C1->CR1 |= (1<<10); // READ ACK
	while (!(I2C1->SR1 & 0x00000040));
	while (!(I2C1->SR1 & 0x00000004));
	char data = I2C1->DR;
	return data;
}

char i2c_read_nack(void)
{
	char data = I2C1->DR;
	I2C1->CR1 &= ~(1<<10); // READ NACK
	while (!(I2C1->SR1 & 0x00000040));
	i2c_stop();
	return data;
}


