#ifndef lcd_h
#define lcd_h

#define SLAVE_ADDRESS_LCD 0x4E // change this according to ur setup

#define cursorright 20
#define cursorleft  16
#define clear        1
#define screenrigt  28
#define screenleft  24
#define gohome       2

void lcd_init(void);
void lcd_4bit(void);
void lcd_put_data(char data);
void lcd_put_string (char *str);
void lcd_put_cmd(char cmd);
void lcd_put_curs(int x, int y);

//char customChar[] = {
//  0x00,
//  0x00,
//  0x0A,
//  0x00,
//  0x11,
//  0x0E,
//  0x00,
//  0x00
//};

#endif

