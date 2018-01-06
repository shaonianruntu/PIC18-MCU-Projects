#ifndef DS1302_H
#define	DS1302_H

#include  "p18f45k20.h"
#include  "delays.h"

#define uchar unsigned char
#define uint  unsigned int 

void delay_nms(unsigned int n);
void delay(int j);
void ds1302write(uchar addr,uchar dat);
uchar ds1302read(char addr);
void ds1302init(void);
void ds1302readtime(void);

#endif	/* DS1302_H */