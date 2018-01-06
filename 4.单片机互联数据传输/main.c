#include "p18f45k20.h"
#include "5110.h"
#include "adc.h"

int T=200;   //移动速度
int y=0;     //起始位置偏移
int x=0;     //记录波形横坐标点
int s=60;   //波形宽度
unsigned int sum;       //采样电压值*10
unsigned char dat[80];
void delay(int a)       //延时函数
{
    int b;
    for(;a>0;a--)
        for(b=0;b<123;b++);
}
void main()
{
    TRISD = 0x00;       //将PORTD设置为输出
    LCD_init();         //LCD初始化
    LCD_clear();        //LCD清屏
    show();             //显示坐标轴
    adc_init();         //ad初始化
    while(1)
    {
        ADCON0bits.GO_DONE = 1;         //启动一次AD转换
        show_num(sum/100);      //显示波形
        display(sum);       //显示电压值
    }
}
