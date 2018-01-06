#include "p18f45k20.h"
#include "5110.h"
#include "ds1302.h"

#define K4 PORTBbits.RB3
#define K3 PORTBbits.RB2
#define K2 PORTBbits.RB1
#define K1 PORTBbits.RB0

unsigned char flag=0;        //��־λ
unsigned char num;
int miao,shi,fen;
extern uchar TIME[7];
extern uchar READ_RTC_ADDR[7];
extern uchar WRITE_RTC_ADDR[7];
void keyscan(void);
/** ******************************
 ��5110����ʾÿһʱ�̵�ʱ�䣬��ʾ����
 * *******************************/
void display(void)
{
    LCD_set_XY(12,3);
    LCD_write_char(TIME[2]/16+48);   //ʱ
    LCD_write_char((TIME[2]&0x0f)+48);      //��48��Ϊ�˽�����ת���ַ�+48
    LCD_write_char(':');
    LCD_write_char(TIME[1]/16+48);   //��
    LCD_write_char((TIME[1]&0x0f)+48);      //����16������0x0fӦΪ����������BCD�룬��Ҫ����ת��
    LCD_write_char(':');
    LCD_write_char(TIME[0]/16+48);   //��
    LCD_write_char((TIME[0]&0x0f)+48);  
 }

void main()
{
   TRISD =0x00;
   TRISC=0XFF;
   PORTC=0XFF;
   LCD_clear();//lcd����
   LCD_init();//lcd��ʼ��
   ds1302init();//ds1302��ʼ��
	while(1)
	{
      keyscan();  //����ɨ�躯��
    }
}
/** ******************************************************************************
 *����ɨ�躯�������� ʱ����
 * k1�����º�ʱ��ֹͣ��ʱ���ٰ�һ���˳���ʱ��1302������ʱ��
 * k2�������µ�һ�ε����룬�ڶ��ε����֣������ε���Сʱ�����Ĵ��ǿգ�Ϊ��ʹ����num��Ϊ0��
 * k3�����Ӽ���
 * k4��������
 * ********************************************************************************/
void keyscan()
{
   if(K1==0)   //k1������
   {
       delay_nms(5);
          if(K1==0)
           {
            flag=~flag;
            while(!K1);
            }
    }
   if(flag==0)
   ds1302readtime();
   else
    {
        if(K2==0)          //k2������
        {
            delay_nms(5);
            if(K2==0)
            {
                num++;
                if(num>3)
                    num=0;
                while(!K2);
            }
        }
         if(K3==0)//�Ӽ�
        {
            delay_nms(5);
            if(K3==0)
            {
             while(!K3);
              switch(num)  
               {
                    case 1:TIME[0]++;
									    if((TIME[0]&0x0f)>9)    //�����λ����9
											{
											     TIME[0]+=0x10;       //ʮλ��1����λ��Ϊ0
													 TIME[0]=TIME[0]&0xf0;
													   if((TIME[0]/16)>5)   //���ʮλ����5��ʮλ��Ϊ0
														      TIME[0]=TIME[0]&0x0f;
											 }
                                    
					                 LCD_set_XY(18,3);
									 LCD_write_char((TIME[0]/16)+48);				//��
									 LCD_write_char((TIME[0]&0x0f)+48);
									 break;
                    case 2:TIME[1]++;
					              if((TIME[1]&0x0f)>9)       //�����λ����9
											{
											     TIME[1]+=0x10;  //ʮλ��1����λ��Ϊ0
													 TIME[1]=TIME[1]&0xf0;
													   if((TIME[1]/16)>5)   //���ʮλ����5��ʮλ��Ϊ0
														      TIME[1]=TIME[1]&0x0f;
											 }
                                  
									LCD_set_XY(16,3);
							     	LCD_write_char('0'+TIME[1]/16);				//��
									LCD_write_char('0'+(TIME[1]&0x0f));
									break;
									
                    case 3:TIME[2]++;
					               if((TIME[2]&0x0f)>9)      //�����λ����9
										    {
											     TIME[2]+=0x10;    //ʮλ��1����λ��Ϊ0
													 TIME[2]=TIME[2]&0xf0;
										    }
                                    if(((TIME[2]/16)==2)&&((TIME[2]&0x0f)==4))  //����ӵ�24��СʱҪ��Ϊ00
														      TIME[2]=0;
                                   
					                LCD_set_XY(12,3);
									LCD_write_char('0'+TIME[2]/16);				//ʱ
									LCD_write_char('0'+(TIME[2]&0x0f));
									break;
                                    
			    }						
		    }						
        }
       
         if(K4==0)//  ����
        {
              delay_nms(5);
            if(K4==0)
            {
               while(!K4);
               switch(num)
               {
                    case 1: miao = (TIME[0]>>4)*10+(TIME[0]&0x0f);//bcd��ת����ʮ������
                            miao--;
                             if(miao<0)
                             {
                               miao=59;
                             }
                            TIME[0]=((miao/10)<<4)|((miao%10)&0x0f);    //ʮ����ת����bcd��  
                        /*TIME[0]--;
									    if(((TIME[0]+1)&0x0f)==0)
											{
													   if((TIME[0]+1)==0)
														      TIME[0]=0x59;
														 else
														 TIME[0]=TIME[0]&0xf9;
											 }*/
                                    
					       
                            LCD_set_XY(18,3);        
					        LCD_write_char((TIME[0]/16)+48);
                            LCD_write_char((TIME[0]&0x0f)+48);//��
									
									 break; 
                    case 2: fen = (TIME[1]>>4)*10+(TIME[1]&0x0f);   //bcd��ת����ʮ������
                            fen--;
                             if(fen<0)
                             {
                               fen=59;
                             }
                             TIME[1]=((fen/10)<<4)|((fen%10)&0x0f);         //ʮ����ת����bcd��  
                        /*TIME[1]--;
					             if(((TIME[1]+1)&0x0f)==0)
											{
													   if((TIME[1]+1)==0)
														      TIME[1]=0x59;
														 else
														 TIME[1]=TIME[1]&0xf9;
											 }*/
                                    
						    LCD_set_XY(16,3);
						    LCD_write_char('0'+TIME[1]/16);				//��
						    LCD_write_char('0'+(TIME[1]&0x0f));
									
									break;                   
					case 3: shi = (TIME[2]>>4)*10+(TIME[2]&0x0f);  //bcd��ת����ʮ������
                            shi--;
                             if(shi<0)
                             {
                               shi=23;
                             }
                             TIME[2]=((shi/10)<<4)|((shi%10)&0x0f);   //ʮ����ת����bcd��  
                        /*TIME[2]--;
					               if(((TIME[2]+1)&0x0f)==0)
											{
													   if((TIME[2]+1)==0)
														      TIME[2]=0x23;
														 else
														 TIME[2]=TIME[2]&0xf9;
											 }*/
                                    
					        LCD_set_XY(12,3);
							LCD_write_char((TIME[2]/16)+48);				//ʱ
						    LCD_write_char(((TIME[2]&0x0f))+48);
									
									break;
                                    
               
                }                   
            }
        }
                            ds1302write(0x8e,0x00);      //�ر�д����
                            ds1302write(WRITE_RTC_ADDR[0],TIME[0]);   //�������õ���д����Ĵ���
                            ds1302write(WRITE_RTC_ADDR[1],TIME[1]);   //�������õķ�д���ּĴ���
                            ds1302write(WRITE_RTC_ADDR[2],TIME[2]);    //�������õ�ʱд��Сʱ�Ĵ���
                            ds1302write(0x8e,0x80);    //��д����
    }
   
    display();  //��ʾ����
   
}
