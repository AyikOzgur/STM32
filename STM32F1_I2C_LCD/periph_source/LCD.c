#include <i2c.h>
#include "LCD.h"
#include "timer.h"
void lcd_put_data(char data)
{

	char datanew[2];
	uint8_t son[6];
	// D7-D4 0 ENABLE RW RS
	datanew[0] = (data & 0xf0);		   // low bits
	datanew[0] |= (9 << 0);			   // rs and open 1
	datanew[1] = ((data << 4) & 0xf0); // high bits
	datanew[1] |= (9 << 0);			   // rs and open 1
	son[0] = datanew[0];
	son[1] = datanew[0] | (1 << 2); // enable
	son[2] = datanew[0];
	son[3] = datanew[1];
	son[4] = datanew[1] | (1 << 2); // enable
	son[5] = datanew[1];

	i2c_start();
	i2c_address(SLAVE_ADDRESS_LCD);
	i2c_write_multi(son, 6);
	i2c_stop();
}

void lcd_put_string(char *str)
{
	while (*str)
		lcd_put_data(*str++);
}

void lcd_put_cmd(char cmdl)
{
	char datanew[2];
	uint8_t son[6];
	// D7-D4 0 ENABLE RW RS
	datanew[0] = (cmdl & 0xf0);		   // low bits
	datanew[0] |= (1 << 3);			   // rs and open 1
	datanew[1] = ((cmdl << 4) & 0xf0); // high bits
	datanew[1] |= (1 << 3);			   // rs and open 1
	son[0] = datanew[0];
	son[1] = datanew[0] | (1 << 2); // enable
	son[2] = datanew[0];
	son[3] = datanew[1];
	son[4] = datanew[1] | (1 << 2); // enable
	son[5] = datanew[1];

	i2c_start();
	i2c_address(SLAVE_ADDRESS_LCD);
	i2c_write_multi(son, 6);
	i2c_stop();
	delay_ms(2);
}

void lcd_put_curs(int x, int y)
{
	uint8_t location = 0x80 + (x + (0x40 * y));
	lcd_put_cmd(location);
}

void lcd_init(void)
{

	uint8_t data = 0b00100000;
	data |= (1 << 3);
	i2c_start();
	i2c_address(SLAVE_ADDRESS_LCD);
	i2c_write(data);
	i2c_stop();

	data |= (1 << 2); // enable 1
	i2c_start();
	i2c_address(SLAVE_ADDRESS_LCD);
	i2c_write(data);
	i2c_stop();
	delay_ms(100);

	data &= ~(1 << 2); //  enable 0
	i2c_start();
	i2c_address(SLAVE_ADDRESS_LCD);
	i2c_write(data);
	i2c_stop();
	delay_ms(100);

	lcd_put_cmd(40); // 4 bit and 2 line
	delay_ms(500);

	lcd_put_cmd(clear); // clear display
	delay_ms(500);
	lcd_put_cmd(gohome); // go home
	delay_ms(500);
	lcd_put_cmd(15); // display, cursor and blink on
	delay_ms(500);

	lcd_put_cmd(clear);
}
