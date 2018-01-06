# MPLAB IDE generated this makefile for use with GNU make.
# Project: 3.mcp
# Date: Sat Sep 12 13:07:04 2015

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

3.cof : main.o 5110.o ds1302.o
	$(LD) /l"E:\360data\Microchip\mcc18\lib" "..\..\360data\Microchip\mcc18\lkr\18f45k20.lkr" "main.o" "5110.o" "ds1302.o" /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /o"3.cof" /M"3.map" /W

main.o : main.c ../../360data/Microchip/mcc18/h/delays.h ds1302.h 5110.h main.c ../../360data/Microchip/mcc18/h/p18f45k20.h ../../360data/Microchip/mcc18/h/p18cxxx.h
	$(CC) -p=18F45K20 "main.c" -fo="main.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

5110.o : 5110.c ../../360data/Microchip/mcc18/h/delays.h ds1302.h 5110.h english_6x8_pixel.h 5110.c ../../360data/Microchip/mcc18/h/p18f45k20.h ../../360data/Microchip/mcc18/h/p18cxxx.h
	$(CC) -p=18F45K20 "5110.c" -fo="5110.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

ds1302.o : ds1302.c ../../360data/Microchip/mcc18/h/delays.h ds1302.h ds1302.c ../../360data/Microchip/mcc18/h/p18f45k20.h ../../360data/Microchip/mcc18/h/p18cxxx.h
	$(CC) -p=18F45K20 "ds1302.c" -fo="ds1302.o" -D__DEBUG -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "5110.o" "ds1302.o" "3.cof" "3.hex"

