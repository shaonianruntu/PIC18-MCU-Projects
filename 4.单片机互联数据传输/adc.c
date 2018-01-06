#include "adc.h"

/*
 * 函数名:adc初始化函数
 * 输入  :无
 * 输出  :无
 */
void adc_init(void)
{
    TRISA = 0Xff;           //将PORTA设置为输出
    ADCON1 = 0X0E;          //将AN0设置为模拟通道
    ADCON0 = 0X00;          //选同AN0
    ADCON2 = 0XBA;          //右对齐,20TAD,32TOSC
    ADCON0bits.ADON = 1;               //打开AD模块电源
    PIR1bits.ADIF = 0;               //清零标志位
    PIE1bits.ADIE = 1;               //中断使能 
    INTCONbits.GIE = 1;                //开总中断
    INTCONbits.PEIE = 1;               //打开外设中断
    delay(5);               //延时
    ADCON0bits.GO_DONE = 1;             //启动一次AD转换
}
/*
 * 函数名:adc中断服务函数
 * 入口  :无
 * 出口  :无
 */ 
#pragma interruptlow pic18_int  //这里使用interruptlow这个关键词来声明InterruptHandlerLow这个函数是低优先级中断服务函数，用了关键词后，这个函数将会由编译器自动产生基本的现场保护，并且这个函数的返回将是使用RETFIE返回的。

 void pic18_int (void)
 {
    float nu;
    unsigned int high;												
    if(PIR1bits.ADIF == 1)                       //判断AD中断是否置位
    {
        PIR1bits.ADIF= 0; 
        high = ADRESH;                       //清零标志位
        sum = (high<<8) | ADRESL;       //将采样值换算为电压值SUM
        nu = sum;
        nu = nu * 3.3 / 1023 * 1000;
        sum = nu;
    }
 }
/*
 * 函数名:中断入口
 * 入口  :无
 * 出口  :无
 */
 #pragma code InterruptVectorLow = 0x18 //用#pragma伪指令定义一个名字叫InterruptVectorLow的段，并把这个段放到0x18地址起始的代码空间
 void InterruptVectorLow (void)  //低优先级中断向量函数
 {
	_asm
	goto pic18_int
	_endasm
 }
 #pragma code     //这里不是多余的，它是告诉连接器回到默认的代码段，如果不加的话，连接器就会傻傻地把后面的代码紧跟着上面的代码一直放下去。而LKR文件里定义了向量区最多到0x29地址，所以如果没加此行通常会报错
 
//void interrupt pic18_int()
//{
//    float num;
//    if(PIRbits.ADIF == 1)                       //判断AD中断是否置位
//    {
//        PIRbits.ADIF= 0;                        //清零标志位
//        sum = ADRESH<<8 | ADRESL;       //将采样值换算为电压值SUM
//        num = sum;
//        num = num * 3.3 / 1023 * 10;
//        sum = num;
//    }
//}
