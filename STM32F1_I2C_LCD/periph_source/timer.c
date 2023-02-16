/*
 * timer.c
 *
 *  Created on: Feb 16, 2023
 *      Author: ozgur
 */
#include "stm32f10x.h"
#include "timer.h"


void timer_init(void)
{
	RCC->APB1ENR |= (1<<0);  // Enable the timer2 clock
	// 2. Set the prescalar and the ARR
	TIM2->PSC = 72-1;  // 72MHz/72 = 1 MHz ~~ 1 uS delay
	TIM2->ARR = 0xffff;  // MAX ARR value

	// 3. Enable the Timer, and wait for the update Flag to set
	TIM2->CR1 |= (1<<0); // Enable the Counter
	while (!(TIM2->SR & (1<<0)));
}


void delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		delay_us (1000); // delay of 1 ms
	}
}


void delay_us (uint16_t us)
{
	TIM2->CNT = 0;
	while (TIM2->CNT < us);
}
