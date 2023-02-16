
/* Includes */
#include "stm32f4xx.h"
#include "System_clock.h"
#include "uart.h"
/* Private macro */
/* Private variables */
uint8_t sizeofbuffer = 25;
volatile uint8_t buf[25];
volatile uint16_t ch[16];  //16 chanels data
volatile char count_char;
/* Private function prototypes */
/* Private functions */

/*
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	clock_init();
	usart2_init();
	int i = 0;


  /* TODO - Add your application code here */
  /* Infinite loop */

	while (1)
	{
		i++;
	}
}
