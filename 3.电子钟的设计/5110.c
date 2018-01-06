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
 *LCD_init          : LCD��ʼ��
 * �������         ����
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

    LCD_write_byte(0x21, 0);	//ʹ����չ����
    LCD_write_byte(0xc8, 0);    //����ƫ�õ�ѹ
    LCD_write_byte(0x06, 0);	//�¶�У��
    LCD_write_byte(0x13, 0);	//1��48
    LCD_write_byte(0x20, 0);	//ʹ�û�������
    LCD_clear();	            //����
    LCD_write_byte(0x0c, 0);	//�趨��ʾģʽ��������ʾ
        
           //�ر�LCD
    LCD_CE = 0;
	 
  }
/*-----------------------------------------------------------------------
 *LCD_clear        : ��������
 * �������         ����
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
 *LCD_set_XY        : �������꺯��
 *�������   X       ��0--83
 *          Y       ��0--5
-----------------------------------------------------------------------*/
void LCD_set_XY(unsigned char X, unsigned char Y)
  {
    LCD_write_byte(0x40 | Y, 0);		// ��
    LCD_write_byte(0x80 | X, 0);        // ��
  }

/*-----------------------------------------------------------------------
 *LCD_write_char    : ��ʾӢ���ַ�
 *������� c        ����ʾ���ַ�
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
  {
    unsigned char line;

    c -= 32;                                //����english_6x8_pixel.hͷ�ļ�ȡģ���⣬�˴�Ӧ�ü�ȥ32

    for (line=0; line<6; line++)            //��ʾ���Ϊ6���ַ�
      LCD_write_byte(font6x8[c][line], 1);
  }
/*-----------------------------------------------------------------------
 *LCD_write_english_String  : Ӣ���ַ�����ʾ����
 *�������*s      ��Ӣ���ַ���ָ��
 *        X,Y    : ��ʾ�ַ�����λ�ã�X 0-83 ,Y 0-5
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
 *LCD_write_chinese_string: ��LCD����ʾ����
 * �������X,Y      :��ʾ������ʼ���� X,Y 
 *        ch_with  �����ֵ���Ŀ��
 *        num      ����ʾ���ֵĸ���
 *        line     �����ֵ������е���ʼ����
 *        row      ��������ʾ���м��
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
 * LCD_draw_map       : λͼ���ƺ���
 * �������X,Y         :��ʾ���Ƶ���ʼ���� X,Y 
 *         *map       ��λͼ��������
 *         Pix_x      ��λͼ���أ�����
 *         Pix_y      ��λͼ���أ��ߣ�
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
 *LCD_write_byte    : ��LCD�����ֽ�
 * ������� dat      ����Ҫ���͵�8λ�ֽ�
 *          command  �����Ϊ0�������Ϊ1��������
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