#include "stm32f4xx.h"
#include "System_clock.h"
#include "timer.h"

volatile uint32_t ch2_rising, ch2_falling, diff, ch2;

int main(void)
{
	clock_init();
	timer2_init();
	int i = 0;

	while (1)
	{
		i++;
	}
}
