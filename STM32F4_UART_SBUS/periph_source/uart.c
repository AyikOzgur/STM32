/*
 * uart.c
 *
 *  Created on: Feb 16, 2023
 *      Author: ozgur
 */
#include "stm32f411xe.h"
#include "uart.h"

void usart2_init(void)
{

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // Enable GPIOA CLOCK
	RCC->APB1ENR |= (1<<17);              // Enable USART2



	GPIOA->MODER &= ~(0xFU << 4); // Reset bits 4:5 for PA2 and 6:7 for PA3
	GPIOA->MODER |=  (0xAU << 4); // Set   bits 4:5 for PA2 and 6:7 for PA3 to alternate mode (10)
	GPIOA->OSPEEDR |= 0x000000A0; // Set pin 2/3 to high speed mode (0b10)
	GPIOA->AFR[0] |= (0x7 << 8);  // for pin A2
	GPIOA->AFR[0] |= (0x7 << 12); // for pin A3


	uint16_t uartdiv = 50000000 / 100000;

	USART2->CR1 |= ( USART_CR1_RE | USART_CR1_TE | USART_CR1_UE | USART_CR1_RXNEIE );
	USART2->CR1 &= ~(1<<12); // 8 bit data length
	USART2->CR1 |= (1<<10); // parity bit enable
	USART2->CR1 &= ~(1<<9); // even parity
	USART2->CR2 |= (1<<13); // 2 stop bit


	USART2->BRR = ( ( ( uartdiv / 16 ) << 4 ) | ( ( uartdiv % 16 ) << 0 ) );

	NVIC_EnableIRQ(USART2_IRQn);
	NVIC_SetPriority(USART2_IRQn, 0x03);
}

void USART2_IRQHandler(void)
{

	while (!(USART2->SR & USART_SR_RXNE)){}

	if (count_char == sizeofbuffer)
	{
		count_char = 0;

		ch[0] = ((uint16_t)buf[ 1] >> 0 | ((int16_t)buf[ 2] << 8 )) & 0x07FF;
		ch[1] = ((uint16_t)buf[ 2] >> 3 | ((int16_t)buf[ 3] << 5 )) & 0x07FF;
		ch[2] = ((uint16_t)buf[ 3] >> 6 | ((int16_t)buf[ 4] << 2 )  | (int16_t)buf[ 5] << 10 ) & 0x07FF;
		ch[3] = ((uint16_t)buf[ 5] >> 1 | ((int16_t)buf[ 6] << 7 )) & 0x07FF;
		ch[4] = ((uint16_t)buf[ 6] >> 4 | ((int16_t)buf[ 7] << 4 )) & 0x07FF;
		ch[5] = ((uint16_t)buf[ 7] >> 7 | ((int16_t)buf[ 8] << 1 )  | (int16_t)buf[9] <<  9 ) & 0x07FF;
		ch[6] = ((uint16_t)buf[ 9] >> 2 | ((int16_t)buf[10] << 6 )) & 0x07FF;
		ch[7] = ((uint16_t)buf[10] >> 5 | ((int16_t)buf[11] << 3 )) & 0x07FF;
		ch[8] = ((uint16_t)buf[12] << 0 | ((int16_t)buf[13] << 8 )) & 0x07FF;
		ch[9] = ((uint16_t)buf[13] >> 3 | ((int16_t)buf[14] << 5 )) & 0x07FF;
		ch[10] = ((uint16_t)buf[14] >> 6 | ((int16_t)buf[15] << 2 )  | (int16_t)buf[16] << 10 ) & 0x07FF;
		ch[11] = ((uint16_t)buf[16] >> 1 | ((int16_t)buf[17] << 7 )) & 0x07FF;
		ch[12] = ((uint16_t)buf[17] >> 4 | ((int16_t)buf[18] << 4 )) & 0x07FF;
		ch[13] = ((uint16_t)buf[18] >> 7 | ((int16_t)buf[19] << 1 )  | (int16_t)buf[20] <<  9 ) & 0x07FF;
		ch[14] = ((uint16_t)buf[20] >> 2 | ((int16_t)buf[21] << 6 )) & 0x07FF;
		ch[15] = ((uint16_t)buf[21] >> 5 | ((int16_t)buf[22] << 3 )) & 0x07FF;
	}

	else
	{
		buf[count_char]=USART2->DR;
		count_char++;
		USART2->SR &= ~USART_SR_RXNE;
	}
}
