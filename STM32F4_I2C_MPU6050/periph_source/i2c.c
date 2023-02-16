#include "stm32f4xx.h"
#include "i2c.h"

void i2c_init(void)
{

	RCC->APB1ENR |= (1<<21);   //turn on I2C bus clock
	RCC->AHB1ENR |= (1<<1);    //turn on I2C pin's GPIO bus clock
	
	GPIOB->MODER |= (2<<12) | (2<<14);    // Set PB6 and PB7 as alternate function
	GPIOB->OTYPER |= (1<<6) | (1<<7);     // set PB6 and PB7 as open drain
	GPIOB->OSPEEDR |= (3<<12) |(3<<14);   // PB6 VE PB7 output freq is HIGH
	GPIOB->PUPDR |= (1<<12) | (1<<14);    // Enable pullup
	GPIOB->AFR[0] |= (4<<24) | (4<<28);   // Set afr
	
	I2C1->CR1 |=  (1<<15);  //reset i2c
	I2C1->CR1 &= ~(1<<15);  // disable reset
	
	// Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	I2C1->CR2 |= (50<<0);  // PCLK1 FREQUENCY in MHz
	
	// Configure the clock control registers   //Please check reference manuel for calculation of these parameters
	I2C1->CCR = 250<<0;
	// Configure the rise time register
	I2C1->TRISE = 51;

	
	// Configure the clock control registers
	I2C1->CCR |= (1<<15); // fast mode
	I2C1->CCR &= ~(1<<14); // duty 1

	
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

void i2c_read (uint8_t Address, uint8_t *buffer, uint8_t size)
{
		
	int remaining = size;
	
	if (size == 1)
	{
		I2C1->DR = Address;  //  send the address
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		
		I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit.... EV6 condition
		I2C1->CR1 |= (1<<9);  // Stop I2C

		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		
		buffer[size-remaining] = I2C1->DR;  // Read the data from the DATA REGISTER
		
	}

	else 
	{
		I2C1->DR = Address;  //  send the address
		while (!(I2C1->SR1 & (1<<1)));  // wait for ADDR bit to set
		
		uint8_t temp = I2C1->SR1 | I2C1->SR2;  // read SR1 and SR2 to clear the ADDR bit
		
		while (remaining>2)
		{
			while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
			
			buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer			
			
			I2C1->CR1 |= 1<<10;  // Set the ACK bit to Acknowledge the data received
			
			remaining--;
		}
		
		// Read the SECOND LAST BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;
		
		I2C1->CR1 &= ~(1<<10);  // clear the ACK bit 
		
		I2C1->CR1 |= (1<<9);  // Stop I2C
		
		remaining--;
		
		// Read the Last BYTE
		while (!(I2C1->SR1 & (1<<6)));  // wait for RxNE to set
		buffer[size-remaining] = I2C1->DR;  // copy the data into the buffer
	}	
	
}

