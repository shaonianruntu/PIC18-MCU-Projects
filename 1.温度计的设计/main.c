#include "p18f45k20.h"
#include "ds18b20.h"
#include "beef.h"

//����ܵ�λѡ�ӵ���RC4~RC7
unsigned char table1[]={0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0xfd};//��������С����,���һ���ǡ���ܡ���ʾ���¶�
					// 	  0    1	2	 3	  4	   5	6	 7	  8    9    -
unsigned char table2[]={0x02,0x9e,0x24,0x0c,0x98,0x48,0xc0,0x1e,0x00,0x08};//������С����
					// 	  0    1	2	 3	  4	   5	6	 7	  8    9 

void display(int temp);
void init_timer1();

/*******************************************************************************
* ������         : main()
* ��������		 : ������
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void main(void)
{ 
    TRISD=0x00;	//output  
    TRISC=0X00;	//output   
	TRISB=0x1f;       //������
    PORTBbits.RB4=1; //�������ر�
  	PORTBbits.RB5=0;    //�¶ȴ�
    while(1)
    {  
    	display(Ds18b20ReadTemp());  
    }
}
/*******************************************************************************
* ������         :display(int temp)
* ��������		 : �������ʾ�¶�
* ����           : temp
* ���         	 : ��
*******************************************************************************/
void display(int temp)
{  
   unsigned char ds[5]={0,0,0,0,0};  
   float fp;
   if(temp>0)
 	{
     fp=temp;
     temp=fp*0.0625*100+0.5;   //�¶�ת��
     ds[0]=temp%10000/1000;
     ds[1]=temp%1000/100;  
     ds[2]=temp%100/10;
     ds[3]=temp%10;
   
     PORTD=table1[ds[0]];
     PORTC=~0X7F;
     delay_nms(1);
     PORTD=0XFF;  //����
     PORTC=~0XFF;
     delay_nms(1);
     
     PORTD=table2[ds[1]];
     PORTC=~0XBF;
     delay_nms(1);
     PORTD=0XFF;  //����
     PORTC=~0XFF;
     delay_nms(1);
     
     PORTD=table1[ds[2]];
     PORTC=~0XDF;
     delay_nms(1);
     PORTD=0XFF;    //����
     PORTC=~0XFF;
     delay_nms(1);
     PORTD=table1[ds[3]];
     PORTC=~0XEF;
     delay_nms(1);
     PORTD=0XFF;     //����
     PORTC=~0XFF;
     delay_nms(1);
    
    if((temp>3200)&&(temp<8000))               //�¶ȱ������.����35��С��80�ȣ�ds18b20��ʼ��ʱ��ʾ85�ȡ���ֹ�����ǳ�ʼ���󱨾���
     {
         beefring();                          //���ֱ�������
         TRISBbits.TRISB4=1;
         PORTBbits.RB4=1;  
         T0CONbits.TMR0ON=0;
       }
      else
      {
         TRISBbits.TRISB4=1;
         PORTBbits.RB4=1; 
       } 
   
  }     
    else
    {
         temp=temp-1;
         temp=~temp;
         fp=temp;
         temp=fp*0.0625*100+0.5;  
         ds[0]=temp%10000/1000;
         ds[1]=temp%1000/100;  
         ds[2]=temp%100/10;
         PORTD=table1[ds[10]];
         PORTC=0X7F;
         delay_nms(1);
         PORTD=0XFF;     //����
         PORTC=0XFF;
         delay_nms(1);
         PORTD=table1[ds[0]];
         PORTC=0XBF;
         delay_nms(1);
         PORTD=0XFF;        //����
         PORTC=0XFF;
         delay_nms(1);

         PORTD=table2[ds[1]];
         PORTC=0XDF;
         delay_nms(1);
         PORTD=0XFF;     //����
         PORTC=0XFF;
         delay_nms(1);

        PORTD=table1[ds[2]];
        PORTC=0XEF;
        delay_nms(1);
        PORTD=0XFF;      //����
        PORTC=0XFF;
        delay_nms(1);
     }
}
