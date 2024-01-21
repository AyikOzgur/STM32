#ifndef TIMER
#define TIMER

#include "stm32f4xx.h"
extern volatile uint32_t ch2_rising, ch2_falling, diff, ch2;

void timer2_init(void);

#endif
