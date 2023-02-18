/*
 * timer.c
 *
 *  Created on: Feb 18, 2023
 *      Author: ozgur
 */

#include "timer.h"


void timer2_init(void)
{
	// 1 - Turn on timer2 clock
	RCC->APB1ENR |= (1<<0);

	// 2 - Turn on GPIOA clock, input pin is connected to PA1
	RCC->AHB1ENR |= (1<<0);

	// 3 - PA1 GPIO setup
	GPIOA->MODER |= (1<<3);
	GPIOA->MODER &= ~(1<<2); // Set as alternate func

	GPIOA->AFR[0] = 0b000000000010001; // AF1

	// 4 - TIMER2 setup
	TIM2->PSC = 99;
	TIM2->ARR =4000000;
	TIM2->CR1 |= (1<<0);   		// Counter is active
	TIM2->CCMR1 |= (1<<8); 		// CC2 channel is configured as input, IC2 is mapped on TI2.
	TIM2->CCER |= (1<<4);  		// Capture is active
	TIM2->DIER |= (1<<0);  		// update interrupt active
	TIM2->DIER |= (1<<2); 		// ch2 interrupt active
	TIM2->CCER &= ~TIM_CCER_CC2P; // polarity mode is rising edge

	// 5 - TIMER2 NVIC setup
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 0x03);

}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_CC2IF)					//check if correspoing interrupt has came, this IRQHandler is common for all chanels
	{
		if ((TIM2->CCER & TIM_CCER_CC2P) == 0)
		{
			ch2_rising = TIM2->CCR2;      		// save CCR2 value at rising edge
			TIM2->CCER |= TIM_CCER_CC2P; 		// change polarity to falling edge
		}

		else
		{
			ch2_falling = TIM2->CCR2;				//save falling edge value
			diff = ch2_falling - ch2_rising;
			if (diff < 0)
			diff += 0xFFFF;    					// if result is negative, trasfer it to positive region
			if (diff < 2010 && diff > 990)
			ch2 = diff;
			TIM2->CCER &= ~TIM_CCER_CC2P; 		// change polarity to rising
		}

		TIM2->SR &= ~TIM_SR_CC2IF; 				// clear interrupt flag
	}
}
