#include "5110.h"
#include "english_6x8_pixel.h"

void delay_1us(void)                
  {
   unsigned int i;
  for(i=0;i<1000;i++);
  }

  void delay_1ms(void)               
  {
   unsigned int i;
   for (i=0;i<1140;i++);
  }
  
/*-----------------------------------------------------------------------
 *LCD_init          : LCD初始化
 * 输入参数         ：无
-----------------------------------------------------------------------*/
void LCD_init(void)
  {
            
    LCD_RST = 0;
    delay_1us();

    LCD_RST = 1;
  		
    LCD_CE = 0;
    delay_1us();
		
    LCD_CE = 1;
    delay_1us();

    LCD_write_byte(0x21, 0);	//使用扩展命令
    LCD_write_byte(0xc8, 0);    //设置偏置电压
    LCD_write_byte(0x06, 0);	//温度校正
    LCD_write_byte(0x13, 0);	//1：48
    LCD_write_byte(0x20, 0);	//使用基本命令
    LCD_clear();	            //清屏
    LCD_write_byte(0x0c, 0);	//设定显示模式，正常显示
        
           //关闭LCD
    LCD_CE = 0;
	 
  }
/*-----------------------------------------------------------------------
 *LCD_clear        : 清屏函数
 * 输入参数         ：无
-----------------------------------------------------------------------*/
void LCD_clear(void)
  {
    unsigned int i;

    LCD_write_byte(0x0c, 0);			
    LCD_write_byte(0x80, 0);			

    for (i=0; i<504; i++)
      LCD_write_byte(0, 1);			
  }
/*-----------------------------------------------------------------------
 *LCD_set_XY        : 设置坐标函数
 *输入参数   X       ：0--83
 *          Y       ：0--5
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);		// 列
    LCD_write_byte(0x80 | X, 0);        // 行
  }

/*-----------------------------------------------------------------------
 *LCD_write_char    : 显示英文字符
 *输入参数 c        ：显示的字符
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;

    c -= 32;                                //由于english_6x8_pixel.h头文件取模问题，此处应该减去32

    for (line=0; line<6; line++)            //显示宽度为6的字符
      LCD_write_byte(font6x8[c][line], 1);
  }
/*-----------------------------------------------------------------------
 *LCD_write_english_String  : 英文字符串显示函数
 *输入参数*s      ：英文字符串指针
 *        X,Y    : 显示字符串的位置，X 0-83 ,Y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X,unsigned char Y,char *s)
  {
    LCD_set_XY(X,Y);
    while (*s) 
      {
	 LCD_write_char(*s);
	 s++;
      }
  }
/*-----------------------------------------------------------------------
 *LCD_write_chinese_string: 在LCD上显示汉字
 * 输入参数X,Y      :显示汉字起始坐标 X,Y 
 *        ch_with  ：汉字点阵的宽度
 *        num      ：显示汉字的个数
 *        line     ：汉字点阵组中的起始行数
 *        row      ：汉字显示的行间距
 -----------------------------------------------------------------------*/
void LCD_write_chinese_string(unsigned char X, unsigned char Y, 
                   unsigned char ch_with,unsigned char num,
                   unsigned char line,unsigned char row,unsigned char j[][26])
  {
    unsigned char i,n;
    
    LCD_set_XY(X,Y);                            
    
    for (i=0;i<num;)
      {
      	for (n=0; n<ch_with*2; n++)             
      	  { 
      	    if (n==ch_with)                    
      	      {
      	        if (i==0) LCD_set_XY(X,Y+1);
      	        else
      	           LCD_set_XY((X+(ch_with+row)*i),Y+1);
              }
      	    LCD_write_byte(j[line+i][n],1);
      	  }
      	i++;
      	LCD_set_XY((X+(ch_with+row)*i),Y);
      }
  }
  
/*-----------------------------------------------------------------------
 * LCD_draw_map       : 位图绘制函数
 * 输入参数X,Y         :显示绘制的起始坐标 X,Y 
 *         *map       ：位图点阵数据
 *         Pix_x      ：位图像素（长）
 *         Pix_y      ：位图像素（高）
 -----------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(unsigned char X,unsigned char Y,unsigned char *map,
                  unsigned char Pix_x,unsigned char Pix_y)
  {
    unsigned int i,n;
    unsigned char row;
    
    if (Pix_y%8==0) row=Pix_y/8;     
      else
        row=Pix_y/8+1;
    
    for (n=0;n<row;n++)
      {
      	LCD_set_XY(X,Y);
        for(i=0; i<Pix_x; i++)
          {
            LCD_write_byte(map[i+n*Pix_x], 1);
          }
        Y++;                       
      }      
  }
/*-----------------------------------------------------------------------
 *LCD_write_byte    : 像LCD发送字节
 * 输入参数 dat      ：需要发送的8位字节
 *          command  ：命令。为0发送命令，为1发送内容
-----------------------------------------------------------------------*/
void LCD_write_byte(unsigned char dat, unsigned char command)
  {
    unsigned char i;

    LCD_CE = 0;
    
    if (command == 0)

     LCD_DC = 0;
    else

     LCD_DC = 1;
		for(i=0;i<8;i++)
		{
			if(dat&0x80)
				SDIN = 1;
			else
				SDIN = 0;
			SCLK = 0;
			dat = dat << 1;
			SCLK = 1;
		}

     LCD_CE = 1;
  }