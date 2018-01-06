#ifndef BEEF_H
#define	BEEF_H

#include "p18f45k20.h"

#define  speaker  PORTBbits.RB4

void delay1(unsigned char t);
void song(void);
void beefring(void);

#endif	/* BEEF_H */