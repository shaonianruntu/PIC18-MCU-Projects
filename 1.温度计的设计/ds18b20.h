#ifndef DS18B20_H
#define	DS18B20_H

#include "p18f45k20.h"
#include "delays.h"


#define Delay_1us {Nop();Nop();Nop();Nop();}
void delay_nms(unsigned int n);
void delay(int x);
void Delay1ms(unsigned int y);
unsigned char Ds18b20Init(void);
void  Ds18b20ChangTemp(void);
int Ds18b20ReadTemp(void);
unsigned char Ds18b20ReadByte(void);
void Ds18b20WriteByte(unsigned char dat);
void Ds18b20ReadTempCom(void);


#endif	/* DS18B20_H */