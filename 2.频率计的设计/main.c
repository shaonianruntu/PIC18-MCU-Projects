#include "p18f45k20.h"
#include "5110.h"

void CatchInit(void);
void display(void);

unsigned int n=0,i=0,FREQL,FREQH,FREQ1,FREQ2,FREQ,count=0,flag=1;
long F,sum=0;

void CatchInit()
{
    //RB3ΪCCP2����ڣ���������λ�޸ĸ��ö˿ڡ���9�С�PORTC�ĳ�PORTBE
    TRISB = 0x04;     //RB3����Ϊ����
    ANSELH = 0x00;      //��������
    CCP2CON = 0x05;     //��׽ģʽ��ÿ��������
    T3CON = 0xB9;       //16λ���ڲ�ʱ�ӣ���Ƶ 1��8��ʹ��,CPP1ʱ��ΪTimer1��CPP2ʱ��ΪTimer3
    
    INTCONbits.GIE = 1;        //����ȫ���ж�
    INTCONbits.PEIE = 1;       //���������ж�
 
    PIE2bits.CCP2IE = 1;     //����׽�ж�
    PIR2bits.CCP2IF = 0;     //�����׽�жϱ�־λ
    
    PIE2bits.TMR3IE = 1;     //ʹ��Timer����ж�
    PIR2bits.TMR3IF = 0;     //����жα�־λ

}

void main()
{
    TRISD = 0x00;
    LCD_init();         //LCD��ʼ��
    LCD_clear();        //LCD����
    display();          //����ʾһ��
    CatchInit();        //��׽ģʽ��ʼ��
    while(1)
    {   
        display();      //һֱѭ����ʾ
    }
}

void display()
{
    if(flag)                //�жϸߵ�Ƶ��־λ��flag=0Ϊ��Ƶģʽ����׽ÿ�������ء�flag=1Ϊ��Ƶģʽ����׽ÿ16��������
    {
        CCP2CON = 0x05;     //��׽ģʽ��ÿ��������
        F=4000000/FREQ/8;   //����Ƶ�ʡ�����16M����Ƭ���ڲ�4��Ƶ������Ϊ4000000��FREQΪ���β�׽�ն�֮�䶨ʱ����ֵ��8ΪTimer3Ϊ1��8��Ƶģʽ
        if(F>1000)          //����Ƶ�ʴ���1kHz���л�ģʽ��
        {
            flag=0;
        }
    }
    else
    {
        CCP2CON = 0x07;     //��׽ģʽ��ÿ16��������
        F=64000000/FREQ/8;  //����Ƶ�ʡ�����16M����Ƭ���ڲ�4��Ƶ��Ϊ4000000������ÿ16�������ز�׽�Σ�Ϊ16*4000000=64000000��FREQΪ���β�׽�ն�֮�䶨ʱ����ֵ��8ΪTimer3Ϊ1��8��Ƶģʽ
     if(n<5)                //ȡ�����ƽ��ֵ��
    {
        sum+=FREQ;
        n++;
    }
    else
    {
        sum/=5;
        F=4000000/sum/8;    //�������ƽ��ֵ
        n=0;
        sum=0;
    }
        if(F<=1000)     //����Ƶ��С��1kHz���л�����ģʽ
        {
            flag=1;
            sum=0;      //��ƽ��ֵ���ñ������㡣
            n=0;
        }
    }    
    LCD_set_XY(24,2);       //������ʾλ��
    LCD_write_char(F/1000000+48);           //��Ƶ��F������ʾ
    LCD_write_char(F%1000000/100000+48);
    LCD_write_char(F%100000/10000+48);
    LCD_write_char(F%10000/1000+48);
    LCD_write_char(F%1000/100+48);
    LCD_write_char(F%100/10+48);
    LCD_write_char(F%10+48);
    LCD_write_char('H');
    LCD_write_char('z');
}



 #pragma interruptlow catch  //����ʹ��interruptlow����ؼ���������InterruptHandlerLow��������ǵ����ȼ��жϷ����������˹ؼ��ʺ�������������ɱ������Զ������������ֳ�������������������ķ��ؽ���ʹ��RETFIE���صġ�

 void catch(void)
 {	 	
    if(PIR2bits.CCP2IF==1)       //��׽�ж�
    {
        PIR2bits.CCP2IF = 0;     //�����׽�жϱ�־λ
        i++;            //��¼������ÿ����һ��
        if((i%2)==1)
        {
            FREQL = CCPR2L;
            FREQH = CCPR2H;
            FREQ1 = (FREQH<<8)|FREQL;       //ȡ����һ��ֵ
        }
        if((i%2)==0)
        {
            i=0;
            FREQL = CCPR2L;
            FREQH = CCPR2H;
            FREQ2 = (FREQH<<8)|FREQL;       //ȥ���ڶ���ֵ
            FREQ = FREQ2-FREQ1;             //������ȡ��ֵ������õ���
        }
    }
    if(PIR2bits.TMR3IF==1)       //Timer3��ʱ������ж�
    {
        PIR2bits.TMR3IF = 0;     //���Timer3��ʱ������ж�
    } 
 }


 #pragma code InterruptVectorLow = 0x18 //��#pragmaαָ���һ�����ֽ�InterruptVectorLow�ĶΣ���������ηŵ�0x18��ַ��ʼ�Ĵ���ռ�
 void InterruptVectorLow (void)  //�����ȼ��ж���������
 {
	_asm
	goto catch
	_endasm
 }
 #pragma code     //���ﲻ�Ƕ���ģ����Ǹ����������ص�Ĭ�ϵĴ���Σ�������ӵĻ����������ͻ�ɵɵ�ذѺ���Ĵ������������Ĵ���һֱ����ȥ����LKR�ļ��ﶨ������������ൽ0x29��ַ���������û�Ӵ���ͨ���ᱨ��


