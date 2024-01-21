#ifndef i2c
#define i2c

#include "stm32f411xe.h"

void i2c_init(void);
void i2c_start(void);
void i2c_write(uint8_t data);
void i2c_address(uint8_t Address);
void i2c_stop(void);
void i2c_write_multi(uint8_t *data, uint8_t size);
char i2c_read_ack(void);
char i2c_read_nack(void);
void i2c_read(uint8_t Address, uint8_t *buffer, uint8_t size);

#endif
