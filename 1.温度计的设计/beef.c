#include"beef.h"

unsigned char timer0h,timer0l,time;
        //���ո�
 unsigned char sszymmh[]={5,1,1, 5,1,1, 6,1,2, 5,1,2, 1,2,2, 7,1,4,
         5,1,1, 5,1,1, 6,1,2, 5,1,2, 2,2,2, 1,2,4,
         5,1,1, 5,1,1, 5,2,2, 3,2,2, 1,2,2, 7,1,2, 6,1,2,
         4,2,1, 4,2,1, 3,2,2, 1,2,2, 2,2,2, 1,2,4};
       // ����Ƶ�ʱ� �߰�λ
 unsigned char FREQH[]={
                                0xF2,0xF3,0xF5,0xF5,0xF6,0xF7,0xF8, 
                                0xF9,0xF9,0xFA,0xFA,0xFB,0xFB,0xFC,0xFC, //1,2,3,4,5,6,7,8,i
                                0xFC,0xFD,0xFD,0xFD,0xFD,0xFE,
                                0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFF,
                               } ;
         // ����Ƶ�ʱ� �Ͱ�λ
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
 * ��ʱ��0�жϷ�����
 * ******************************************
 */
 #pragma interruptlow timer0  //����ʹ��interruptlow����ؼ���������InterruptHandlerLow��������ǵ����ȼ��жϷ����������˹ؼ��ʺ�������������ɱ������Զ������������ֳ�������������������ķ��ؽ���ʹ��RETFIE���صġ�

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
 * ��ʱ��0�ж����
 * ******************************************
 */
 #pragma code InterruptVectorLow = 0x18 //��#pragmaαָ���һ�����ֽ�InterruptVectorLow�ĶΣ���������ηŵ�0x18��ַ��ʼ�Ĵ���ռ�
 void InterruptVectorLow (void)  //�����ȼ��ж���������
 {
	_asm
	goto timer0
	_endasm
 }
 #pragma code     //���ﲻ�Ƕ���ģ����Ǹ����������ص�Ĭ�ϵĴ���Σ�������ӵĻ����������ͻ�ɵɵ�ذѺ���Ĵ������������Ĵ���һֱ����ȥ����LKR�ļ��ﶨ������������ൽ0x29��ַ���������û�Ӵ���ͨ���ᱨ��
/*
 * *****************************************
 * �質����
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
 * ���ֺ���
 * ******************************************
 */
 void beefring(void)
 {
 unsigned char k,i;
 TRISBbits.TRISB4=0;
 PORTBbits.RB4=0;
 INTCONbits.GIE=1;
 INTCONbits.PEIE=1;
 INTCONbits.TMR0IE=1;     // TMOD=1; //��CT0��ʱ������ʽ1
               //EA=1;
              // ET0=1;//IE=0x82 //CPU���ж�,CT0���ж� 
 INTCONbits.TMR0IF=0;
 T0CONbits.TMR0ON=1;
 T0CON=0X08;
  i=0;  
  while(i<75){         //�������鳤�� �������ͷ����        
  k=sszymmh[i]+7*sszymmh[i+1]-1;
  timer0h=FREQH[k];
  timer0l=FREQL[k];
  time=sszymmh[i+2];
  i=i+3;
  song();
            }
  //TMR0ON=0;
 }