#include "p18f45k20.h"
#include "5110.h"
#include "adc.h"

int T=200;   //�ƶ��ٶ�
int y=0;     //��ʼλ��ƫ��
int x=0;     //��¼���κ������
int s=60;   //���ο��
unsigned int sum;       //������ѹֵ*10
unsigned char dat[80];
void delay(int a)       //��ʱ����
{
    int b;
    for(;a>0;a--)
        for(b=0;b<123;b++);
}
void main()
{
    TRISD = 0x00;       //��PORTD����Ϊ���
    LCD_init();         //LCD��ʼ��
    LCD_clear();        //LCD����
    show();             //��ʾ������
    adc_init();         //ad��ʼ��
    while(1)
    {
        ADCON0bits.GO_DONE = 1;         //����һ��ADת��
        show_num(sum/100);      //��ʾ����
        display(sum);       //��ʾ��ѹֵ
    }
}
