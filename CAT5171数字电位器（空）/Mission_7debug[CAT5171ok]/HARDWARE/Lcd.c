#include "Lcd.h"
#include "bsp_sdram.h" 

volatile  unsigned char    *LCD_Add_PCmd = (unsigned char *)0x65200000;    //LCDָ����ַ 0x65200000
volatile  unsigned char    *LCD_Add_PDat = (unsigned char *)0x64200000;    //LCD���ݱ����ַ 0x64200000

void dly(uint16_t i)
{
	for(;i>0;i--);
}

/****************************************************************************
* �� �ƣ� LCD_GPIO_Init
* �� �ܣ� ��ʼ��GPIO�˿�
* �� �룺 ��
* �� ���� ��
****************************************************************************/
void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (LCD_A23_GPIO_CLK,ENABLE); 
	RCC_AHB1PeriphClockCmd (LCD_RES_GPIO_CLK,ENABLE);

/************************ LCD���� ***************************/
	//LCD����
	GPIO_InitStructure.GPIO_Pin = LCD_A23_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Init(LCD_A23_GPIO_PORT, &GPIO_InitStructure);
	LCD_CMD_ON;
/************************************************************/

/******************* LCD_RESET_PG7���� **********************/
	//LCD_RESET����
	GPIO_InitStructure.GPIO_Pin = LCD_RES_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Init(LCD_RES_GPIO_PORT, &GPIO_InitStructure);
	LCD_RES_ON;
/************************************************************/
	
}

/****************************************************************************
* �� �ƣ� Lcd_Reset
* �� �ܣ� ����LCD��Ļ
* �� �룺 ��
* �� ���� ��
****************************************************************************/
void Lcd_Reset(void)
{
	 LCD_RES_OFF;
	 dly(10);
	 LCD_RES_ON;
	 dly(10);
	 LCD_RES_OFF;
	 dly(10);
	 LCD_RES_ON;
	 DelaymS(170);
}

/****************************************************************************
* �� �ƣ� Lcd_CmdWrite
* �� �ܣ� д��LCDָ��
* �� �룺 cmd
* �� ���� ��
****************************************************************************/
void Lcd_CmdWrite(uint8_t cmd)
{
	LCD_CMD_ON;
	dly(1);
	*LCD_Add_PCmd = cmd;
	dly(2);
}

/****************************************************************************
* �� �ƣ� Lcd_DataWrite
* �� �ܣ� д��LCD����
* �� �룺 data
* �� ���� ��
****************************************************************************/
void Lcd_DataWrite(uint8_t data)
{
	LCD_DATA_ON;
	dly(1);
	*LCD_Add_PDat = data;              //д�����ݻ����
  dly(2);
}

/****************************************************************************
* �� �ƣ� Lcd_AddrRange_Set
* �� �ܣ� ��λ�������λ�ã���˳�㷢��дָ��
* �� �룺 ��Ļ��ʾλ�õĺ�����x��������y
* �� ���� ��
****************************************************************************/

void Lcd_AddrRange_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	uint16_t  addr; 
	
		if(SCREEN_DIR==0)
	{
		Lcd_CmdWrite(0x2a);//�е�ַ����
		addr = x1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = x2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		Lcd_CmdWrite(0x2c);//������д
		
		Lcd_CmdWrite(0x2b);//�е�ַ����
		addr = y1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = y2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		
		Lcd_CmdWrite(0x2c);//������д
	}
	else if(SCREEN_DIR==1)
	{
		Lcd_CmdWrite(0x2a);//�е�ַ����
		addr = x1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = x2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		Lcd_CmdWrite(0x2c);//������д
		
		Lcd_CmdWrite(0x2b);//�е�ַ����
		addr = y1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = y2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		
		Lcd_CmdWrite(0x2c);//������д
	}
	else if(SCREEN_DIR==2)
	{
		Lcd_CmdWrite(0x2a);//�е�ַ����
		addr = x1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = x2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		Lcd_CmdWrite(0x2c);//������д
		
		Lcd_CmdWrite(0x2b);//�е�ַ����
		addr = y1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = y2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		
		Lcd_CmdWrite(0x2c);//������д
	}
	else
	{
		Lcd_CmdWrite(0x2a);//�е�ַ����
		addr = x1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = x2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		Lcd_CmdWrite(0x2c);//������д
		
		Lcd_CmdWrite(0x2b);//�е�ַ����
		addr = y1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = y2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		
		Lcd_CmdWrite(0x2c);//������д
	}
}

/****************************************************************************
* �� �ƣ� LCD_FillAll
* �� �ܣ� ��������Ļ��ջ�д��
* �� �룺 0 -- ��գ�1 -- д��
* �� ���� ��
* �� ������(xsta+1,ysta+1)����(xend,yend)
****************************************************************************/
void LCD_FillAll(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 addr,i,j; 
	Lcd_AddrRange_Set(xsta,ysta,xend,yend);//������ʾ��Χ
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)
		{
			addr = color;
			Lcd_DataWrite(addr>>8);
			Lcd_DataWrite(addr&0xFF);
		}
	} 					  	    
}

/****************************************************************************
* �� �ƣ� Lcd_Initial
* �� �ܣ� LCD�ĳ�ʼ������
* �� �룺 ��
* �� ���� ��
****************************************************************************/
void Lcd_Initial(void)
{
	LCD_GPIO_Init();              //��ʼ��FMC֮���LCD���ţ�BLEN �� D/C���ţ�
	
	Lcd_Reset();
	
	Lcd_CmdWrite(0x11);				//����
	DelaymS(50);
	
	Lcd_CmdWrite(0x35);				//TE ON
	Lcd_DataWrite(0x00);
	
	//��ʾ����ɫģʽ����
	Lcd_CmdWrite(0x36);			//��������ɨ�� �Լ� ������RGB����BGRģʽ
			
	if(SCREEN_DIR==0)Lcd_DataWrite(0x00);
	else if(SCREEN_DIR==1)Lcd_DataWrite(0xc0);
	else if(SCREEN_DIR==2)Lcd_DataWrite(0x70);
	else Lcd_DataWrite(0xA0);
	
	Lcd_CmdWrite(0x3A);				//set ARM data access
	Lcd_DataWrite(0x05);
	
	//֡������
	Lcd_CmdWrite(0xB2);
	Lcd_DataWrite(0x0C);
	Lcd_DataWrite(0x0C);
	Lcd_DataWrite(0x00);
	Lcd_DataWrite(0x33);
	Lcd_DataWrite(0x33);
	
	Lcd_CmdWrite(0xb7);				//vcom setting
	Lcd_DataWrite(0x24);
	
	//ST7789 ��Դ����
	Lcd_CmdWrite(0xBB);				//vcom setting
	Lcd_DataWrite(0x1C);
	
	Lcd_CmdWrite(0xC0);				//LCM control
	Lcd_DataWrite(0x2C);
	
	Lcd_CmdWrite(0xc2);  	    //VDV and VRH Command Enable	
	Lcd_DataWrite(0x01);

	Lcd_CmdWrite(0xc3); 		  //VRH Set, VAP(GVDD) & VAN(GVCL)
	Lcd_DataWrite(0x13); 

	Lcd_CmdWrite(0xc4);       //VDV Set, 
	Lcd_DataWrite(0x20);      //VDV=0V

	Lcd_CmdWrite(0xc6);       //Frame rate control in normal mode
	Lcd_DataWrite(0x0f);      //dot inversion & 60Hz

	Lcd_CmdWrite(0xd0);       //Power control 1
	Lcd_DataWrite(0xa4);
	Lcd_DataWrite(0xb1);       //AVDD=6.8V; AVCL=-4.8V; VDDS=2.3V
	
	//ST7789 gamma����
	Lcd_CmdWrite(0xe0);
	Lcd_DataWrite(0xd0);
	Lcd_DataWrite(0x0a);
	Lcd_DataWrite(0x12);
	Lcd_DataWrite(0x0a);
	Lcd_DataWrite(0x09);
	Lcd_DataWrite(0x07);
	Lcd_DataWrite(0x3a);
	Lcd_DataWrite(0x44);
	Lcd_DataWrite(0x50);
	Lcd_DataWrite(0x3a);
	Lcd_DataWrite(0x13);
	Lcd_DataWrite(0x12);
	Lcd_DataWrite(0x2e);
	Lcd_DataWrite(0x30);

	Lcd_CmdWrite(0xe1);
	Lcd_DataWrite(0xd0);
	Lcd_DataWrite(0x0e);
	Lcd_DataWrite(0x13);
	Lcd_DataWrite(0x0b);
	Lcd_DataWrite(0x0b);
	Lcd_DataWrite(0x25);
	Lcd_DataWrite(0x3a);
	Lcd_DataWrite(0x43);
	Lcd_DataWrite(0x4f);
	Lcd_DataWrite(0x38);
	Lcd_DataWrite(0x15);
	Lcd_DataWrite(0x14);
	Lcd_DataWrite(0x2c);
	Lcd_DataWrite(0x32);

	//���������ж�,��ͬ������Բ�ͬ���װ�
	LCD_FillAll(0,0,320,240,WHITE);
	
	LCD_BLEN_OFF;
  Lcd_CmdWrite(0x21);
	Lcd_CmdWrite(0x29);             //Display on
	DelaymS(10);
	
//	LCD_FillAll(0,0,120,240,YELLOW);
//	LCD_FillAll(5,10,20,20,GRAY);
}

/****************************************************************************
* �� �ƣ� LCD_DrawLine
* �� �ܣ� ��һ��ֱ��
* �� �룺 ��ʼλ�úͽ���λ���Լ���ɫ
* �� ���� ��
* �� ������(xsta+1,ysta+1)����(xend,yend)���Ƚ����⣬����Ҳ������亯������
****************************************************************************/
void LCD_DrawLine(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 addr,i,j; 
	//���ߡ���X������ֹ1������Yֻ�����1������ͬ��
	if(((xsta<xend)&&(ysta+1==yend)) || ((ysta<yend)&&(xsta+1==xend)))
	{
		Lcd_AddrRange_Set(xsta,ysta,xend,yend);//������ʾ��Χ
		for(i=ysta;i<=yend;i++)
		{													   	 	
			for(j=xsta;j<=xend;j++)
			{
				addr = color;
				Lcd_DataWrite(addr>>8);
				Lcd_DataWrite(addr&0xFF);
			}
		} 
	}
}

/****************************************************************************
* �� �ƣ� LCD_DrawOneCH
* �� �ܣ� �ڹ涨��λ��������̶����ַ�
* �� �룺 ��ʼλ�úͽ���λ���Լ���ɫ
* �� ���� ��
* �� ������(xsta+1,ysta+1)����һ������Ϊ��ģ��С�ķ�Χ��
****************************************************************************/
void LCD_DrawOneCH(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext)
{          
	uint8_t ucBuffer[32] = {0};							//����һ�����һ�����ֵ���ģ�Ļ���
	uint8_t ucTemp = *Mixtext;							//�������������
	uint32_t OffsetAddress;									//���������ģƫ�Ƶ�ַ
	uint8_t* TempP;													//�����ݴ��ȡ�ĵ�ַ��̫��
	
	u16 addr,bk_color,i,j;	//addrΪ�������ɫ,bk_colorΪ����ı�����ɫ��i��jΪѭ������
	u16 cur_x,cur_y; 				//�м�����������ѭ���������຺���ֽ�+16����
	cur_x = xsta;
	cur_y = ysta;
	
	addr = color;
	bk_color = bkcolor;
	Lcd_AddrRange_Set(cur_x,cur_y,cur_x+15,cur_y+15);//������ʾ��Χ
	
	if(ucTemp >= 0xA1)												  //ASCII���м��ֵ		
	{
		//1���ȼ�����ַ���ƫ��
		ucTemp = *Mixtext;

		OffsetAddress = ((*Mixtext - 0xA1) * 94 + (*(Mixtext+1) - 0xA1)) *32;
		TempP = (uint8_t*)(FontAddress + (u32)OffsetAddress);
		
		//2����ͨ�����ַ���ƫ���ҵ����Ӧ����ģ
		for(i=cur_y;i<cur_y+16;i++)
		{													   	 	
			for(j=cur_x;j<cur_x+16;j++)
			{
				if((TempP[(i-cur_y)*2+((j-cur_x)/8)] << ((j-cur_x)%8)) & 0x80)
				{
					Lcd_DataWrite(addr>>8);
					Lcd_DataWrite(addr&0xFF);
				}
				else					//�հ��������ɫ����/ѡ�񻭻���������������ʱ�����,������ĥ��
				{
					Lcd_DataWrite(bk_color>>8);
					Lcd_DataWrite(bk_color&0xFF);
				}
			}
		}
	} 
}

/****************************************************************************
* �� �ƣ� LCD_DrawOneCH
* �� �ܣ� �ڹ涨��λ��������̶����ַ�
* �� �룺 ��ʼλ�úͽ���λ���Լ���ɫ
* �� ���� ��
* �� ������(xsta+1,ysta+1)����һ������Ϊ��ģ��С�ķ�Χ��
****************************************************************************/
void LCD_DrawOneEN(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext)
{          
	uint8_t ucTemp = *Mixtext;								           //�������������
	uint32_t OffsetAddress;									//���������ģƫ�Ƶ�ַ
	uint8_t* TempP;													//�����ݴ��ȡ�ĵ�ַ��̫��
	
	u16 addr,bk_color,i,j;	//addrΪ�������ɫ,bk_colorΪ����ı�����ɫ��i��jΪѭ������
	u16 cur_x,cur_y; 				//�м�����������ѭ���������຺���ֽ�+16����
	cur_x = xsta;
	cur_y = ysta;
	
	addr = color;
	bk_color = bkcolor;
	
	Lcd_AddrRange_Set(cur_x,cur_y,cur_x+7,cur_y+15);//������ʾ��Χ  
	
	if(ucTemp < 0xA1)												  //ASCII���м��ֵ		
	{
		//1���ȼ�����ַ���ƫ��
		ucTemp = *Mixtext;

		OffsetAddress = ucTemp << 4;
		TempP = (uint8_t*)(AsciiFontAddress + (u32)OffsetAddress);
		
		//2����ͨ�����ַ���ƫ���ҵ����Ӧ����ģ
		for(i=cur_y;i<cur_y+16;i++)
		{													   	 	
			for(j=cur_x;j<cur_x+8;j++)
			{
				if((TempP[(i-cur_y)+((j-cur_x)/8)] << ((j-cur_x)%8)) & 0x80)
				{
					Lcd_DataWrite(addr>>8);
					Lcd_DataWrite(addr&0xFF);
				}
				else					//�հ��������ɫ����/ѡ�񻭻���������������ʱ�����,������ĥ��
				{
					Lcd_DataWrite(bk_color>>8);
					Lcd_DataWrite(bk_color&0xFF);
				}
			}
		}
	} 
}

/****************************************************************************
* �� �ƣ� LCD_DrawMixText
* �� �ܣ� �ڹ涨��λ��������̶����ַ�
* �� �룺 ��ʼλ�úͽ���λ���Լ���ɫ
* �� ���� ��
* �� ������(xsta+1,ysta+1)��(xend,yend)�ķ�Χ�ڣ��ȳ���16x16��������ʾ
****************************************************************************/
void LCD_DrawMixText(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color,u16 bkcolor,u8 *Mixtext)
{          
	u16 addr,bk_color,i,j;	//addrΪ�������ɫ,bk_colorΪ����ı�����ɫ��i��jΪѭ������
	u16 cur_x,cur_y; 				//�м�����������ѭ���������຺���ֽ�+16����
	cur_x = xsta;
	cur_y = ysta;

	while(*Mixtext != '\0')                              
	{
		if(*Mixtext < 0xA1)				         //Ӣ��
		{
			if(cur_x + EN_CHAR_WIDTH > xend)				       //�������X��Χ
			{
				cur_x = xsta;
				cur_y += EN_CHAR_HEIGHT;									
			}	
			if(cur_y + EN_CHAR_HEIGHT > yend)             //��������Y��Χ
			{

			}
			
			LCD_DrawOneEN(cur_x,cur_y,color,bkcolor,Mixtext);
			cur_x += EN_CHAR_WIDTH;
			
			Mixtext++;
		}
		else                           //����
		{
			if(cur_x + CH_CHAR_WIDTH > xend)				       //���������Ļ���
			{
				cur_x = xsta;
				cur_y += CH_CHAR_HEIGHT;									
			}	
			if(cur_y + CH_CHAR_HEIGHT > yend)             //������Ļ�߶�
			{
				
			}
			
			LCD_DrawOneCH(cur_x,cur_y,color,bkcolor,Mixtext);
			cur_x += CH_CHAR_WIDTH;							         //ÿ���һ���ַ��ͽ������һ���ַ��Ŀ��
			
			Mixtext += 2;
		}
  }	
}

/****************************************************************************
* �� �ƣ� LCD_DrawPic
* �� �ܣ� �ڹ涨��λ�������ͼƬ
* �� �룺 ��ʼλ�úͽ���λ��
* �� ���� ��
* �� ������ͼƬת���ɶ������ļ����ȡ������ͼƬ�ļ��Ĺ�����Ҫ��һ�������
****************************************************************************/
void LCD_DrawPic(u16 xsta,u16 ysta,u16 xend,u16 yend,u8 *Pic)
{          
	u16 addr,i,j;	//addrΪ�������ɫ,i��jΪѭ������
	
	Lcd_AddrRange_Set(xsta,ysta,xend-1,yend-1);          //������ʾ��Χ

	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend+1;j++)
		{
			Lcd_DataWrite(Pic[i*(yend-ysta)*2 + j*2]);
			Lcd_DataWrite(Pic[i*(yend-ysta)*2 + j*2 + 1]);
		}
	} 
}

/*********************************** END OF FILE ***************************************/
