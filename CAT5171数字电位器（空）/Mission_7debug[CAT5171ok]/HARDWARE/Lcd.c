#include "Lcd.h"
#include "bsp_sdram.h" 

volatile  unsigned char    *LCD_Add_PCmd = (unsigned char *)0x65200000;    //LCD指令保存地址 0x65200000
volatile  unsigned char    *LCD_Add_PDat = (unsigned char *)0x64200000;    //LCD数据保存地址 0x64200000

void dly(uint16_t i)
{
	for(;i>0;i--);
}

/****************************************************************************
* 名 称： LCD_GPIO_Init
* 功 能： 初始化GPIO端口
* 输 入： 无
* 输 出： 无
****************************************************************************/
void LCD_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (LCD_A23_GPIO_CLK,ENABLE); 
	RCC_AHB1PeriphClockCmd (LCD_RES_GPIO_CLK,ENABLE);

/************************ LCD引脚 ***************************/
	//LCD配置
	GPIO_InitStructure.GPIO_Pin = LCD_A23_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Init(LCD_A23_GPIO_PORT, &GPIO_InitStructure);
	LCD_CMD_ON;
/************************************************************/

/******************* LCD_RESET_PG7引脚 **********************/
	//LCD_RESET配置
	GPIO_InitStructure.GPIO_Pin = LCD_RES_GPIO_PIN;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_Init(LCD_RES_GPIO_PORT, &GPIO_InitStructure);
	LCD_RES_ON;
/************************************************************/
	
}

/****************************************************************************
* 名 称： Lcd_Reset
* 功 能： 重置LCD屏幕
* 输 入： 无
* 输 出： 无
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
* 名 称： Lcd_CmdWrite
* 功 能： 写入LCD指令
* 输 入： cmd
* 输 出： 无
****************************************************************************/
void Lcd_CmdWrite(uint8_t cmd)
{
	LCD_CMD_ON;
	dly(1);
	*LCD_Add_PCmd = cmd;
	dly(2);
}

/****************************************************************************
* 名 称： Lcd_DataWrite
* 功 能： 写入LCD数据
* 输 入： data
* 输 出： 无
****************************************************************************/
void Lcd_DataWrite(uint8_t data)
{
	LCD_DATA_ON;
	dly(1);
	*LCD_Add_PDat = data;              //写入数据或参数
  dly(2);
}

/****************************************************************************
* 名 称： Lcd_AddrRange_Set
* 功 能： 定位到所需的位置，并顺便发送写指令
* 输 入： 屏幕显示位置的横坐标x和纵坐标y
* 输 出： 无
****************************************************************************/

void Lcd_AddrRange_Set(u16 x1,u16 y1,u16 x2,u16 y2)
{
	uint16_t  addr; 
	
		if(SCREEN_DIR==0)
	{
		Lcd_CmdWrite(0x2a);//列地址设置
		addr = x1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = x2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		Lcd_CmdWrite(0x2c);//储存器写
		
		Lcd_CmdWrite(0x2b);//行地址设置
		addr = y1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = y2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		
		Lcd_CmdWrite(0x2c);//储存器写
	}
	else if(SCREEN_DIR==1)
	{
		Lcd_CmdWrite(0x2a);//列地址设置
		addr = x1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = x2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		Lcd_CmdWrite(0x2c);//储存器写
		
		Lcd_CmdWrite(0x2b);//行地址设置
		addr = y1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = y2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		
		Lcd_CmdWrite(0x2c);//储存器写
	}
	else if(SCREEN_DIR==2)
	{
		Lcd_CmdWrite(0x2a);//列地址设置
		addr = x1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = x2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		Lcd_CmdWrite(0x2c);//储存器写
		
		Lcd_CmdWrite(0x2b);//行地址设置
		addr = y1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = y2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		
		Lcd_CmdWrite(0x2c);//储存器写
	}
	else
	{
		Lcd_CmdWrite(0x2a);//列地址设置
		addr = x1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = x2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		Lcd_CmdWrite(0x2c);//储存器写
		
		Lcd_CmdWrite(0x2b);//行地址设置
		addr = y1;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		addr = y2;
		Lcd_DataWrite(addr>>8);
		Lcd_DataWrite(addr&0xFF);
		
		Lcd_CmdWrite(0x2c);//储存器写
	}
}

/****************************************************************************
* 名 称： LCD_FillAll
* 功 能： 将整个屏幕清空或写满
* 输 入： 0 -- 清空，1 -- 写满
* 输 出： 无
* 描 述：从(xsta+1,ysta+1)画到(xend,yend)
****************************************************************************/
void LCD_FillAll(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 addr,i,j; 
	Lcd_AddrRange_Set(xsta,ysta,xend,yend);//设置显示范围
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
* 名 称： Lcd_Initial
* 功 能： LCD的初始化函数
* 输 入： 无
* 输 出： 无
****************************************************************************/
void Lcd_Initial(void)
{
	LCD_GPIO_Init();              //初始化FMC之外的LCD引脚（BLEN 和 D/C引脚）
	
	Lcd_Reset();
	
	Lcd_CmdWrite(0x11);				//唤醒
	DelaymS(50);
	
	Lcd_CmdWrite(0x35);				//TE ON
	Lcd_DataWrite(0x00);
	
	//显示和颜色模式设置
	Lcd_CmdWrite(0x36);			//上下左右扫描 以及 设置是RGB或者BGR模式
			
	if(SCREEN_DIR==0)Lcd_DataWrite(0x00);
	else if(SCREEN_DIR==1)Lcd_DataWrite(0xc0);
	else if(SCREEN_DIR==2)Lcd_DataWrite(0x70);
	else Lcd_DataWrite(0xA0);
	
	Lcd_CmdWrite(0x3A);				//set ARM data access
	Lcd_DataWrite(0x05);
	
	//帧率设置
	Lcd_CmdWrite(0xB2);
	Lcd_DataWrite(0x0C);
	Lcd_DataWrite(0x0C);
	Lcd_DataWrite(0x00);
	Lcd_DataWrite(0x33);
	Lcd_DataWrite(0x33);
	
	Lcd_CmdWrite(0xb7);				//vcom setting
	Lcd_DataWrite(0x24);
	
	//ST7789 电源设置
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
	
	//ST7789 gamma设置
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

	//可以做个判断,不同方向可以不同画底板
	LCD_FillAll(0,0,320,240,WHITE);
	
	LCD_BLEN_OFF;
  Lcd_CmdWrite(0x21);
	Lcd_CmdWrite(0x29);             //Display on
	DelaymS(10);
	
//	LCD_FillAll(0,0,120,240,YELLOW);
//	LCD_FillAll(5,10,20,20,GRAY);
}

/****************************************************************************
* 名 称： LCD_DrawLine
* 功 能： 画一条直线
* 输 入： 起始位置和结束位置以及颜色
* 输 出： 无
* 描 述：从(xsta+1,ysta+1)画到(xend,yend)，比较特殊，画线也可由填充函数画出
****************************************************************************/
void LCD_DrawLine(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 addr,i,j; 
	//横线——X可以相差不止1，但是Y只能相差1（竖线同理）
	if(((xsta<xend)&&(ysta+1==yend)) || ((ysta<yend)&&(xsta+1==xend)))
	{
		Lcd_AddrRange_Set(xsta,ysta,xend,yend);//设置显示范围
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
* 名 称： LCD_DrawOneCH
* 功 能： 在规定的位置上输出固定的字符
* 输 入： 起始位置和结束位置以及颜色
* 输 出： 无
* 描 述：从(xsta+1,ysta+1)到多一个长宽为字模大小的范围内
****************************************************************************/
void LCD_DrawOneCH(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext)
{          
	uint8_t ucBuffer[32] = {0};							//定义一个存放一个汉字的字模的缓存
	uint8_t ucTemp = *Mixtext;							//用来保存其编码
	uint32_t OffsetAddress;									//保存其的字模偏移地址
	uint8_t* TempP;													//用来暂存读取的地址，太长
	
	u16 addr,bk_color,i,j;	//addr为保存的颜色,bk_color为字体的背景颜色，i，j为循环变量
	u16 cur_x,cur_y; 				//中间变量用来存放循环条件，多汉字字节+16即可
	cur_x = xsta;
	cur_y = ysta;
	
	addr = color;
	bk_color = bkcolor;
	Lcd_AddrRange_Set(cur_x,cur_y,cur_x+15,cur_y+15);//设置显示范围
	
	if(ucTemp >= 0xA1)												  //ASCII码中间的值		
	{
		//1、先计算该字符的偏移
		ucTemp = *Mixtext;

		OffsetAddress = ((*Mixtext - 0xA1) * 94 + (*(Mixtext+1) - 0xA1)) *32;
		TempP = (uint8_t*)(FontAddress + (u32)OffsetAddress);
		
		//2、再通过该字符的偏移找到其对应的字模
		for(i=cur_y;i<cur_y+16;i++)
		{													   	 	
			for(j=cur_x;j<cur_x+16;j++)
			{
				if((TempP[(i-cur_y)*2+((j-cur_x)/8)] << ((j-cur_x)%8)) & 0x80)
				{
					Lcd_DataWrite(addr>>8);
					Lcd_DataWrite(addr&0xFF);
				}
				else					//空白区域的颜色数据/选择画或者跳过（跳过暂时不清楚,后续琢磨）
				{
					Lcd_DataWrite(bk_color>>8);
					Lcd_DataWrite(bk_color&0xFF);
				}
			}
		}
	} 
}

/****************************************************************************
* 名 称： LCD_DrawOneCH
* 功 能： 在规定的位置上输出固定的字符
* 输 入： 起始位置和结束位置以及颜色
* 输 出： 无
* 描 述：从(xsta+1,ysta+1)到多一个长宽为字模大小的范围内
****************************************************************************/
void LCD_DrawOneEN(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext)
{          
	uint8_t ucTemp = *Mixtext;								           //用来保存其编码
	uint32_t OffsetAddress;									//保存其的字模偏移地址
	uint8_t* TempP;													//用来暂存读取的地址，太长
	
	u16 addr,bk_color,i,j;	//addr为保存的颜色,bk_color为字体的背景颜色，i，j为循环变量
	u16 cur_x,cur_y; 				//中间变量用来存放循环条件，多汉字字节+16即可
	cur_x = xsta;
	cur_y = ysta;
	
	addr = color;
	bk_color = bkcolor;
	
	Lcd_AddrRange_Set(cur_x,cur_y,cur_x+7,cur_y+15);//设置显示范围  
	
	if(ucTemp < 0xA1)												  //ASCII码中间的值		
	{
		//1、先计算该字符的偏移
		ucTemp = *Mixtext;

		OffsetAddress = ucTemp << 4;
		TempP = (uint8_t*)(AsciiFontAddress + (u32)OffsetAddress);
		
		//2、再通过该字符的偏移找到其对应的字模
		for(i=cur_y;i<cur_y+16;i++)
		{													   	 	
			for(j=cur_x;j<cur_x+8;j++)
			{
				if((TempP[(i-cur_y)+((j-cur_x)/8)] << ((j-cur_x)%8)) & 0x80)
				{
					Lcd_DataWrite(addr>>8);
					Lcd_DataWrite(addr&0xFF);
				}
				else					//空白区域的颜色数据/选择画或者跳过（跳过暂时不清楚,后续琢磨）
				{
					Lcd_DataWrite(bk_color>>8);
					Lcd_DataWrite(bk_color&0xFF);
				}
			}
		}
	} 
}

/****************************************************************************
* 名 称： LCD_DrawMixText
* 功 能： 在规定的位置上输出固定的字符
* 输 入： 起始位置和结束位置以及颜色
* 输 出： 无
* 描 述：从(xsta+1,ysta+1)到(xend,yend)的范围内，先尝试16x16的文字显示
****************************************************************************/
void LCD_DrawMixText(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color,u16 bkcolor,u8 *Mixtext)
{          
	u16 addr,bk_color,i,j;	//addr为保存的颜色,bk_color为字体的背景颜色，i，j为循环变量
	u16 cur_x,cur_y; 				//中间变量用来存放循环条件，多汉字字节+16即可
	cur_x = xsta;
	cur_y = ysta;

	while(*Mixtext != '\0')                              
	{
		if(*Mixtext < 0xA1)				         //英文
		{
			if(cur_x + EN_CHAR_WIDTH > xend)				       //如果超过X范围
			{
				cur_x = xsta;
				cur_y += EN_CHAR_HEIGHT;									
			}	
			if(cur_y + EN_CHAR_HEIGHT > yend)             //超过超过Y范围
			{

			}
			
			LCD_DrawOneEN(cur_x,cur_y,color,bkcolor,Mixtext);
			cur_x += EN_CHAR_WIDTH;
			
			Mixtext++;
		}
		else                           //中文
		{
			if(cur_x + CH_CHAR_WIDTH > xend)				       //如果超过屏幕宽度
			{
				cur_x = xsta;
				cur_y += CH_CHAR_HEIGHT;									
			}	
			if(cur_y + CH_CHAR_HEIGHT > yend)             //超过屏幕高度
			{
				
			}
			
			LCD_DrawOneCH(cur_x,cur_y,color,bkcolor,Mixtext);
			cur_x += CH_CHAR_WIDTH;							         //每输出一个字符就将其加上一个字符的宽度
			
			Mixtext += 2;
		}
  }	
}

/****************************************************************************
* 名 称： LCD_DrawPic
* 功 能： 在规定的位置上输出图片
* 输 入： 起始位置和结束位置
* 输 出： 无
* 描 述：将图片转换成二进制文件后读取，对于图片文件的构成需要有一定的理解
****************************************************************************/
void LCD_DrawPic(u16 xsta,u16 ysta,u16 xend,u16 yend,u8 *Pic)
{          
	u16 addr,i,j;	//addr为保存的颜色,i，j为循环变量
	
	Lcd_AddrRange_Set(xsta,ysta,xend-1,yend-1);          //设置显示范围

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
