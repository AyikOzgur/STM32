#ifndef System_clock
#define System_clock

#include "stm32f411xe.h"

#define PLL_M 25
#define PLL_N 400
#define PLL_P 1 // for pllp=4

void clock_init(void);
void systick_init(void);

#endif
