#ifndef uart
#define uart

extern volatile uint8_t buf[25];
extern volatile uint16_t ch[16];
extern uint8_t sizeofbuffer;
extern volatile char count_char;
void usart2_init(void);

#endif
