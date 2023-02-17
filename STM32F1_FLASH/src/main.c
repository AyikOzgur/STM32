
/* Includes */
#include "stm32f10x.h"

#define Set_Bit(x, pos) (x |= (1U << pos))
#define Clear_Bit(x, pos) (x &= (~(1U<< pos)))
#define Set_Pin(x, pos) (x |= (1U << pos))
#define Clear_Pin(x, pos) (x &= (~(1U<< pos)<<16))
#define Toggle_Bit(x, pos) x ^= (1U<< pos)
#define Check_Bit(x, pos) (x & (1UL << pos))

void Flash_Write(uint32_t StartPageAddress, void *Data, uint16_t sizeofdata);
void Flash_Read(uint32_t adress, void *data , int size);

char data_write[] = "Hello World";
char data_read[sizeof(data_write)];

int size_r;
int size_w;
int page_adress=0x800fc00;


void makeItGoFast(void);


int main(void)
{
	makeItGoFast();
	Flash_Write(page_adress, &data_write, sizeof(data_write));
	Flash_Read(page_adress, &data_read, sizeof(data_read));

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
    FLASH->ACR      |= FLASH_ACR_LATENCY_2; // Two wait states, per datasheet
    RCC->CFGR       |= RCC_CFGR_PPRE1_2;    // prescale APB1 = HCLK/2
    RCC->CR         |= RCC_CR_HSEON;        // enable HSE clock
    while( !(RCC->CR & RCC_CR_HSERDY) );    // wait for the HSEREADY flag

    RCC->CFGR       |= RCC_CFGR_PLLSRC;     // set PLL source to HSE
    RCC->CFGR       |= RCC_CFGR_PLLMULL9;   // multiply by 9
    RCC->CR         |= RCC_CR_PLLON;        // enable the PLL
    while( !(RCC->CR & RCC_CR_PLLRDY) );    // wait for the PLLRDY flag

    RCC->CFGR       |= RCC_CFGR_SW_PLL;     // set clock source to pll

    while( !(RCC->CFGR & RCC_CFGR_SWS_PLL) );    // wait for PLL to be CLK

    SystemCoreClockUpdate();                // calculate the SYSCLOCK value
}

void Flash_Write(uint32_t StartPageAddress, void *Data, uint16_t sizeofdata)
{
	int index=0;
	// Unlock flash registers
	uint16_t *adres;
	uint16_t *data;
	adres=( uint16_t *)StartPageAddress;
	data= ( uint16_t *)Data;
	sizeofdata/=2;

	if(!(Check_Bit(FLASH->SR,FLASH_SR_BSY)))      		//check if bsy flag is zero
	{
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
		FLASH->CR &= ~(1<<0);
		FLASH->CR |= (1<<1);           					// FLASH PAGE ERASE ENABLE
		FLASH->AR = StartPageAddress;  					// page adress will be erased
		FLASH->CR |= (1<<6);           					// Start selected operation here erasing
		while(Check_Bit(FLASH->SR,FLASH_SR_BSY));	  	//wait till busy flag is zero

	}
	if(!(Check_Bit(FLASH->SR,FLASH_SR_BSY)))   	  		//check if bsy flag is zero
	{
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
		FLASH->CR &= ~(1<<1); 							//deselect erase mode
		FLASH->CR |= (1<<0);            				// FLASH PAGE PROGRAMME ENABLE
		while(sizeofdata>index)
		{
		*adres=*data;
		while((Check_Bit(FLASH->SR,FLASH_SR_BSY))); 	//check if bsy flag is zero
		adres++;
		data++;
		index++;
		}
	}
}

void Flash_Read(uint32_t adress, void *data , int size)
{
	size/=2;
    uint16_t *AddressPtr;
    uint16_t *valuePtr;
    AddressPtr = (uint16_t *)adress;
    valuePtr=(uint16_t *)data;
    for(int i=0 ; i<size; i++)
    {
        *((uint16_t *)valuePtr)=*((uint16_t *)AddressPtr);
        valuePtr++;
        AddressPtr++;
    }
}




