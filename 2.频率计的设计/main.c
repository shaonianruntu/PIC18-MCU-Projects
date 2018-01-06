#include "p18f45k20.h"
#include "5110.h"

void CatchInit(void);
void display(void);

unsigned int n=0,i=0,FREQL,FREQH,FREQ1,FREQ2,FREQ,count=0,flag=1;
long F,sum=0;

void CatchInit()
{
    //RB3为CCP2输入口，，，配置位修改复用端口。第9行。PORTC改成PORTBE
    TRISB = 0x04;     //RB3设置为输入
    ANSELH = 0x00;      //数字输入
    CCP2CON = 0x05;     //捕捉模式，每个上升沿
    T3CON = 0xB9;       //16位，内部时钟，分频 1：8，使能,CPP1时钟为Timer1，CPP2时钟为Timer3
    
    INTCONbits.GIE = 1;        //允许全局中段
    INTCONbits.PEIE = 1;       //允许外设中段
 
    PIE2bits.CCP2IE = 1;     //允许捕捉中断
    PIR2bits.CCP2IF = 0;     //清除捕捉中断标志位
    
    PIE2bits.TMR3IE = 1;     //使能Timer溢出中断
    PIR2bits.TMR3IF = 0;     //清除中段标志位

}

void main()
{
    TRISD = 0x00;
    LCD_init();         //LCD初始化
    LCD_clear();        //LCD清屏
    display();          //先显示一下
    CatchInit();        //捕捉模式初始化
    while(1)
    {   
        display();      //一直循环显示
    }
}

void display()
{
    if(flag)                //判断高低频标志位，flag=0为低频模式，捕捉每个上升沿。flag=1为高频模式，捕捉每16个上升沿
    {
        CCP2CON = 0x05;     //捕捉模式，每个上升沿
        F=4000000/FREQ/8;   //计算频率。晶振16M，单片机内部4分频，所以为4000000，FREQ为两次捕捉终端之间定时器差值，8为Timer3为1：8分频模式
        if(F>1000)          //测量频率大于1kHz，切换模式。
        {
            flag=0;
        }
    }
    else
    {
        CCP2CON = 0x07;     //捕捉模式，每16个上升沿
        F=64000000/FREQ/8;  //计算频率。晶振16M，单片机内部4分频，为4000000，又有每16个上升沿捕捉次，为16*4000000=64000000，FREQ为两次捕捉终端之间定时器差值，8为Timer3为1：8分频模式
     if(n<5)                //取五次求平均值。
    {
        sum+=FREQ;
        n++;
    }
    else
    {
        sum/=5;
        F=4000000/sum/8;    //求出最终平均值
        n=0;
        sum=0;
    }
        if(F<=1000)     //测量频率小于1kHz，切换测量模式
        {
            flag=1;
            sum=0;      //求平均值所用变量清零。
            n=0;
        }
    }    
    LCD_set_XY(24,2);       //设置显示位置
    LCD_write_char(F/1000000+48);           //对频率F分离显示
    LCD_write_char(F%1000000/100000+48);
    LCD_write_char(F%100000/10000+48);
    LCD_write_char(F%10000/1000+48);
    LCD_write_char(F%1000/100+48);
    LCD_write_char(F%100/10+48);
    LCD_write_char(F%10+48);
    LCD_write_char('H');
    LCD_write_char('z');
}



 #pragma interruptlow catch  //这里使用interruptlow这个关键词来声明InterruptHandlerLow这个函数是低优先级中断服务函数，用了关键词后，这个函数将会由编译器自动产生基本的现场保护，并且这个函数的返回将是使用RETFIE返回的。

 void catch(void)
 {	 	
    if(PIR2bits.CCP2IF==1)       //捕捉中断
    {
        PIR2bits.CCP2IF = 0;     //清除捕捉中断标志位
        i++;            //记录次数，每两个一组
        if((i%2)==1)
        {
            FREQL = CCPR2L;
            FREQH = CCPR2H;
            FREQ1 = (FREQH<<8)|FREQL;       //取出第一个值
        }
        if((i%2)==0)
        {
            i=0;
            FREQL = CCPR2L;
            FREQH = CCPR2H;
            FREQ2 = (FREQH<<8)|FREQL;       //去除第二个值
            FREQ = FREQ2-FREQ1;             //将两次取得值相减，得到差
        }
    }
    if(PIR2bits.TMR3IF==1)       //Timer3定时器溢出中断
    {
        PIR2bits.TMR3IF = 0;     //清除Timer3定时器溢出中断
    } 
 }


 #pragma code InterruptVectorLow = 0x18 //用#pragma伪指令定义一个名字叫InterruptVectorLow的段，并把这个段放到0x18地址起始的代码空间
 void InterruptVectorLow (void)  //低优先级中断向量函数
 {
	_asm
	goto catch
	_endasm
 }
 #pragma code     //这里不是多余的，它是告诉连接器回到默认的代码段，如果不加的话，连接器就会傻傻地把后面的代码紧跟着上面的代码一直放下去。而LKR文件里定义了向量区最多到0x29地址，所以如果没加此行通常会报错


