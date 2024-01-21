
/* Includes */
#include "stm32f10x.h"
#include <i2c.h>
#include "timer.h"
#include "LCD.h"

void makeItGoFast(void);

int main(void)
{
	makeItGoFast();
	i2c_init();
	lcd_init();
	lcd_put_curs(0, 0);
	lcd_put_string("RemoteController");
	lcd_put_curs(6, 1);
	lcd_put_string("V1.0");
	delay_ms(3000);

	int i = 1;

	/* Infinite loop */
	while (1)
	{
		i++;
	}
}

void makeItGoFast(void)
{
	// Conf clock : 72MHz using HSE 8MHz crystal w/ PLL X 9 (8MHz x 9 = 72MHz)
	FLASH->ACR |= FLASH_ACR_LATENCY_2; // Two wait states, per datasheet
	RCC->CFGR |= RCC_CFGR_PPRE1_2;	   // prescale APB1 = HCLK/2
	RCC->CR |= RCC_CR_HSEON;		   // enable HSE clock
	while (!(RCC->CR & RCC_CR_HSERDY))
		; // wait for the HSEREADY flag

	RCC->CFGR |= RCC_CFGR_PLLSRC;	// set PLL source to HSE
	RCC->CFGR |= RCC_CFGR_PLLMULL9; // multiply by 9
	RCC->CR |= RCC_CR_PLLON;		// enable the PLL
	while (!(RCC->CR & RCC_CR_PLLRDY))
		; // wait for the PLLRDY flag

	RCC->CFGR |= RCC_CFGR_SW_PLL; // set clock source to pll

	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL))
		; // wait for PLL to be CLK

	SystemCoreClockUpdate(); // calculate the SYSCLOCK value
}
