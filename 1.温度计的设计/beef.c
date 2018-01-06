#include"beef.h"

unsigned char timer0h,timer0l,time;
        //生日歌
 unsigned char sszymmh[]={5,1,1, 5,1,1, 6,1,2, 5,1,2, 1,2,2, 7,1,4,
         5,1,1, 5,1,1, 6,1,2, 5,1,2, 2,2,2, 1,2,4,
         5,1,1, 5,1,1, 5,2,2, 3,2,2, 1,2,2, 7,1,2, 6,1,2,
         4,2,1, 4,2,1, 3,2,2, 1,2,2, 2,2,2, 1,2,4};
       // 音阶频率表 高八位
 unsigned char FREQH[]={
                                0xF2,0xF3,0xF5,0xF5,0xF6,0xF7,0xF8, 
                                0xF9,0xF9,0xFA,0xFA,0xFB,0xFB,0xFC,0xFC, //1,2,3,4,5,6,7,8,i
                                0xFC,0xFD,0xFD,0xFD,0xFD,0xFE,
                                0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,
                               } ;
         // 音阶频率表 低八位
 unsigned char FREQL[]={
                                 0x42,0xC1,0x17,0xB6,0xD0,0xD1,0xB6,
                                 0x21,0xE1,0x8C,0xD8,0x68,0xE9,0x5B,0x8F, //1,2,3,4,5,6,7,8,i
                                 0xEE,0x44, 0x6B,0xB4,0xF4,0x2D, 
                                 0x47,0x77,0xA2,0xB6,0xDA,0xFA,0x16,
                                };
void delay1(unsigned char t)
   {
   unsigned char t1;
   unsigned long t2;
   for(t1=0;t1<18;t1++)
   {
    for(t2=0;t2<8000;t2++)
       {
        ;
        }
   }
   T0CONbits.TMR0ON=0;
   }
/*
 * *****************************************
 * 定时器0中断服务函数
 * ******************************************
 */
 #pragma interruptlow timer0  //这里使用interruptlow这个关键词来声明InterruptHandlerLow这个函数是低优先级中断服务函数，用了关键词后，这个函数将会由编译器自动产生基本的现场保护，并且这个函数的返回将是使用RETFIE返回的。

 void timer0 (void)
 {
     INTCONbits.TMR0IF=0;
	 T0CONbits.TMR0ON=0;
	 speaker=!speaker;
	 TMR0H=timer0h;
	 TMR0L=timer0l;
	 T0CONbits.TMR0ON=1;
 }
/*
 * *****************************************
 * 定时器0中断入口
 * ******************************************
 */
 #pragma code InterruptVectorLow = 0x18 //用#pragma伪指令定义一个名字叫InterruptVectorLow的段，并把这个段放到0x18地址起始的代码空间
 void InterruptVectorLow (void)  //低优先级中断向量函数
 {
	_asm
	goto timer0
	_endasm
 }
 #pragma code     //这里不是多余的，它是告诉连接器回到默认的代码段，如果不加的话，连接器就会傻傻地把后面的代码紧跟着上面的代码一直放下去。而LKR文件里定义了向量区最多到0x29地址，所以如果没加此行通常会报错
/*
 * *****************************************
 * 歌唱函数
 * ******************************************
 */
void song()
{
 TMR0H=timer0h;
 TMR0L=timer0l;
 T0CONbits.TMR0ON=1;
 delay1(time);                       
 }
/*
 * *****************************************
 * 音乐函数
 * ******************************************
 */
 void beefring(void)
 {
 unsigned char k,i;
 TRISBbits.TRISB4=0;
 PORTBbits.RB4=0;
 INTCONbits.GIE=1;
 INTCONbits.PEIE=1;
 INTCONbits.TMR0IE=1;     // TMOD=1; //置CT0定时工作方式1
               //EA=1;
              // ET0=1;//IE=0x82 //CPU开中断,CT0开中断 
 INTCONbits.TMR0IF=0;
 T0CONbits.TMR0ON=1;
 T0CON=0X08;
  i=0;  
  while(i<75){         //音乐数组长度 ，唱完从头再来        
  k=sszymmh[i]+7*sszymmh[i+1]-1;
  timer0h=FREQH[k];
  timer0l=FREQL[k];
  time=sszymmh[i+2];
  i=i+3;
  song();
            }
  //TMR0ON=0;
 }