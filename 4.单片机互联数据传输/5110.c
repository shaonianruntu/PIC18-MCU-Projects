#include "5110.h"
#include "num.h"
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
  
void delay_nms(unsigned int n)      
  {
   unsigned int i=0;
   for (i=0;i<n;i++)
   delay_1ms();
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
/*-----------------------------------------------------------------------
 * show               : 显示坐标轴，以及清除坐标区域
 * 输入参数            :无
 -----------------------------------------------------------------------*/
void show()
{
    unsigned char i,j;
    LCD_set_XY(0,0);
    LCD_write_char('u');                //显示变量u
    for(i=0;i<4;i++)
    {
        LCD_set_XY(8,i);                //i一直在加，从上往下刷屏
        LCD_write_byte(0xff,1);         //坐标轴y
        for(j=0;j<s;j++)               //按照波形宽度s总坐标轴y开始向右进行清空
        LCD_write_byte(0x00,1);
    }
    LCD_set_XY(8,4);                    //此行牵扯后面的，x坐标轴的显示。后面不用清空
    LCD_write_byte(0xff,1);
    for(i=0;i<70;i++)                   //显示x坐标轴
    {
        LCD_write_byte(0x80,1);
    }
    LCD_set_XY(0,5);                    //显示原点
    LCD_write_char('O');
    LCD_set_XY(70,5);                   //显示变量t
    LCD_write_char('t');
}
/*-----------------------------------------------------------------------
 * show_dat            : 将电压值对应到LCD显示出来
 * 输入参数number      :采样电压值的十倍
 *        x            :电压值的横坐标
 * 用if判断把屏幕简单的分成5块，然后通过横纵坐标控制电压值像素点的
 * 位置，并发送对应的值（换算成0~7）过去
 * （0~7）已经取好模，在num.h中的num[]数组上
 -----------------------------------------------------------------------*/
void show_dat(int number,int x)
{
        if(number<8)
        {
            LCD_set_XY(x+y+9,4);                        //x为电压值的横坐标，y为起始位置的偏移（对应坐标轴），9是应为坐标轴为8、
            LCD_write_byte((num[number]|0x80),1);       //或上0x80是因为x坐标与这个块公用。最高位是x坐标轴
        }
        else if(number<16)
        {
            LCD_set_XY(x+y+9,3);
            LCD_write_byte(num[number-8],1);
        }
        else if(number<24)
        {
            LCD_set_XY(x+y+9,2);
            LCD_write_byte(num[number-16],1);
        }
        else if(number<32)
        {
            LCD_set_XY(x+y+9,1);
            LCD_write_byte(num[number-24],1);
        }
        else
        {
            LCD_set_XY(x+y+9,0);
            LCD_write_byte(num[number-32],1);
        }
}
/*-----------------------------------------------------------------------
 * show_num            : 将电压值发送到LCD并生成可移动波形
 * 输入参数number      :采样电压值的十倍
 -----------------------------------------------------------------------*/
void show_num(int number)
{ 
    dat[x]=number;                  //存储每个像素点电压值
    if(x<s)                         //刚上电，小于波形宽度，从左网游依次显示
    {
        show_dat(dat[x],x);         //将电压值对应到LCD显示
    }
    else
    {
        show();                     //利用显示坐标函数，顺便清除坐标中间的上次像素点
        move();                     //电压波形左移
        x=s-1;                      //x回到波形宽度的极限
        show_dat(dat[x],x);         //将电压值对应到LCD显示（波形宽度的最后一个）
    }
    x++;                            //记录电压像素的横坐标
    delay(T);                   //延时，控制左移速度
}
/*-----------------------------------------------------------------------
 * move()             : 电压波形左移函数
 * 输入参数            :无
 -----------------------------------------------------------------------*/
void move()
{
    unsigned char i=0;
    for(i=0;i<s;i++)
    {
        dat[i]=dat[i+1];        //所有电压像素点的值左移
        show_dat(dat[i],i);     //刷新显示新波形像素点
    }
}
/*-----------------------------------------------------------------------
 * display()           : 电压显示函数
 * 输入参数voltage     :采样电压值的十倍
 -----------------------------------------------------------------------*/
void display(int voltage)
{
    LCD_set_XY(26,5);                   //确定电压值显示位置
    LCD_write_char(voltage/1000+48);      //加48是为了将数字转成字符+48
    LCD_write_char('.');
    LCD_write_char(voltage%1000/100+48);
    LCD_write_char(voltage%100/10+48);
    LCD_write_char(voltage%10+48);
    LCD_write_char('V');
}