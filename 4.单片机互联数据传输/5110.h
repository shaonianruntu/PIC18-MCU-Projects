#ifndef __5110_H
#define __5110_H


#include "p18f45k20.h"


#define SCLK LATDbits.LATD3
#define SDIN LATDbits.LATD4
#define LCD_DC LATDbits.LATD5
#define LCD_CE LATDbits.LATD6
#define LCD_RST  LATDbits.LATD7


extern int x,y,s,T;
extern unsigned char dat[80];

void LCD_init(void);
void LCD_clear(void);
void LCD_move_chinese_string(unsigned char X, unsigned char Y, unsigned char T); 
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s);
void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                   unsigned char ch_with,unsigned char num,
                   unsigned char line,unsigned char row,unsigned char j[][26]);
void chinese_string(unsigned char X, unsigned char Y, unsigned char T);                 
void LCD_write_char(unsigned char c);
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,
                  unsigned char Pix_x,unsigned char Pix_y);
void LCD_write_byte(unsigned char dat, unsigned char dc);
void LCD_set_XY(unsigned char X, unsigned char Y);
void delay_1us(void);         
void show(void);
void show_dat(int number,int x);
void show_num(int number);
void move(void);
void delay(int a);
void display(int dat);
 

#endif

