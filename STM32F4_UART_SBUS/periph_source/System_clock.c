
#include "stm32f4xx.h"
#include "System_clock.h"
extern volatile int millis;

void clock_init(void)
{

	RCC->CR |= RCC_CR_HSEON;
	while (!(RCC->CR & RCC_CR_HSERDY))
		;

	FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_3WS;

	RCC->CFGR |= RCC_CFGR_HPRE_DIV1; //

	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // M:25  N:400 P:4

	RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P << 16) | (RCC_PLLCFGR_PLLSRC_HSE);

	RCC->CR |= RCC_CR_PLLON;
	while (!(RCC->CR & RCC_CR_PLLRDY))
		;

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL))
		;

	SystemCoreClockUpdate();
}

void systick_init(void)
{

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	SysTick->LOAD = 62500; // 5ms
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL &= ~(1 << 2); // Systick clock source = AHB/8
	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority(SysTick_IRQn, 0x01); //(1UL << __NVIC_PRIO_BITS) - 1UL
}

void SysTick_Handler(void)
{
}
