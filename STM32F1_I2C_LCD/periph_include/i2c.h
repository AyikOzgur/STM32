#ifndef i2c_h
#define i2c_h

#include "stm32f10x.h"
 
void i2c_init(void);
void i2c_start (void);
void i2c_write (uint8_t data);
void i2c_address (uint8_t Address);
void i2c_stop (void);
void i2c_write_multi (uint8_t *data, uint8_t size);
char i2c_read_ack(void);
char i2c_read_nack(void);
 
#endif

