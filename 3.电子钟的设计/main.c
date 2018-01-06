#include "p18f45k20.h"
#include "5110.h"
#include "ds1302.h"

#define K4 PORTBbits.RB3
#define K3 PORTBbits.RB2
#define K2 PORTBbits.RB1
#define K1 PORTBbits.RB0

unsigned char flag=0;        //标志位
unsigned char num;
int miao,shi,fen;
extern uchar TIME[7];
extern uchar READ_RTC_ADDR[7];
extern uchar WRITE_RTC_ADDR[7];
void keyscan(void);
/** ******************************
 在5110上显示每一时刻的时间，显示函数
 * *******************************/
void display(void)
{
    LCD_set_XY(12,3);
    LCD_write_char(TIME[2]/16+48);   //时
    LCD_write_char((TIME[2]&0x0f)+48);      //加48是为了将数字转成字符+48
    LCD_write_char(':');
    LCD_write_char(TIME[1]/16+48);   //分
    LCD_write_char((TIME[1]&0x0f)+48);      //除以16，与上0x0f应为读出数据是BCD码，需要这样转换
    LCD_write_char(':');
    LCD_write_char(TIME[0]/16+48);   //秒
    LCD_write_char((TIME[0]&0x0f)+48);  
 }

void main()
{
   TRISD =0x00;
   TRISC=0XFF;
   PORTC=0XFF;
   LCD_clear();//lcd清屏
   LCD_init();//lcd初始化
   ds1302init();//ds1302初始化
	while(1)
	{
      keyscan();  //键盘扫描函数
    }
}
/** ******************************************************************************
 *键盘扫描函数，调整 时分秒
 * k1键按下后，时钟停止走时，再按一下退出调时，1302继续走时；
 * k2键：按下第一次调整秒，第二次调整分，第三次调整小时；第四次是空，为了使计数num变为0；
 * k3键：加键；
 * k4键：减键
 * ********************************************************************************/
void keyscan()
{
   if(K1==0)   //k1键按下
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
        if(K2==0)          //k2键按下
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
         if(K3==0)//加键
        {
            delay_nms(5);
            if(K3==0)
            {
             while(!K3);
              switch(num)  
               {
                    case 1:TIME[0]++;
									    if((TIME[0]&0x0f)>9)    //如果个位大于9
											{
											     TIME[0]+=0x10;       //十位加1，个位变为0
													 TIME[0]=TIME[0]&0xf0;
													   if((TIME[0]/16)>5)   //如果十位大于5，十位变为0
														      TIME[0]=TIME[0]&0x0f;
											 }
                                    
					                 LCD_set_XY(18,3);
									 LCD_write_char((TIME[0]/16)+48);				//秒
									 LCD_write_char((TIME[0]&0x0f)+48);
									 break;
                    case 2:TIME[1]++;
					              if((TIME[1]&0x0f)>9)       //如果个位大于9
											{
											     TIME[1]+=0x10;  //十位加1，个位变为0
													 TIME[1]=TIME[1]&0xf0;
													   if((TIME[1]/16)>5)   //如果十位大于5，十位变为0
														      TIME[1]=TIME[1]&0x0f;
											 }
                                  
									LCD_set_XY(16,3);
							     	LCD_write_char('0'+TIME[1]/16);				//分
									LCD_write_char('0'+(TIME[1]&0x0f));
									break;
									
                    case 3:TIME[2]++;
					               if((TIME[2]&0x0f)>9)      //如果个位大于9
										    {
											     TIME[2]+=0x10;    //十位加1，个位变为0
													 TIME[2]=TIME[2]&0xf0;
										    }
                                    if(((TIME[2]/16)==2)&&((TIME[2]&0x0f)==4))  //如果加到24，小时要变为00
														      TIME[2]=0;
                                   
					                LCD_set_XY(12,3);
									LCD_write_char('0'+TIME[2]/16);				//时
									LCD_write_char('0'+(TIME[2]&0x0f));
									break;
                                    
			    }						
		    }						
        }
       
         if(K4==0)//  减键
        {
              delay_nms(5);
            if(K4==0)
            {
               while(!K4);
               switch(num)
               {
                    case 1: miao = (TIME[0]>>4)*10+(TIME[0]&0x0f);//bcd码转换成十进制数
                            miao--;
                             if(miao<0)
                             {
                               miao=59;
                             }
                            TIME[0]=((miao/10)<<4)|((miao%10)&0x0f);    //十进制转换成bcd码  
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
                            LCD_write_char((TIME[0]&0x0f)+48);//秒
									
									 break; 
                    case 2: fen = (TIME[1]>>4)*10+(TIME[1]&0x0f);   //bcd码转换成十进制数
                            fen--;
                             if(fen<0)
                             {
                               fen=59;
                             }
                             TIME[1]=((fen/10)<<4)|((fen%10)&0x0f);         //十进制转换成bcd码  
                        /*TIME[1]--;
					             if(((TIME[1]+1)&0x0f)==0)
											{
													   if((TIME[1]+1)==0)
														      TIME[1]=0x59;
														 else
														 TIME[1]=TIME[1]&0xf9;
											 }*/
                                    
						    LCD_set_XY(16,3);
						    LCD_write_char('0'+TIME[1]/16);				//分
						    LCD_write_char('0'+(TIME[1]&0x0f));
									
									break;                   
					case 3: shi = (TIME[2]>>4)*10+(TIME[2]&0x0f);  //bcd码转换成十进制数
                            shi--;
                             if(shi<0)
                             {
                               shi=23;
                             }
                             TIME[2]=((shi/10)<<4)|((shi%10)&0x0f);   //十进制转换成bcd码  
                        /*TIME[2]--;
					               if(((TIME[2]+1)&0x0f)==0)
											{
													   if((TIME[2]+1)==0)
														      TIME[2]=0x23;
														 else
														 TIME[2]=TIME[2]&0xf9;
											 }*/
                                    
					        LCD_set_XY(12,3);
							LCD_write_char((TIME[2]/16)+48);				//时
						    LCD_write_char(((TIME[2]&0x0f))+48);
									
									break;
                                    
               
                }                   
            }
        }
                            ds1302write(0x8e,0x00);      //关闭写保护
                            ds1302write(WRITE_RTC_ADDR[0],TIME[0]);   //将调整好的秒写进秒寄存器
                            ds1302write(WRITE_RTC_ADDR[1],TIME[1]);   //将调整好的分写进分寄存器
                            ds1302write(WRITE_RTC_ADDR[2],TIME[2]);    //将调整好的时写进小时寄存器
                            ds1302write(0x8e,0x80);    //打开写保护
    }
   
    display();  //显示函数
   
}
