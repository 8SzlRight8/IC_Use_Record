#ifndef _LCD_H_
#define _LCD_H_ 

#include "stm32f4xx.h"

extern volatile  unsigned char    *LCD_Add_PCmd;    //LCD指令保存地址
extern volatile  unsigned char    *LCD_Add_PDat;    //LCD数据保存地址

#define CH_CHAR_WIDTH						16
#define CH_CHAR_HEIGHT					16

#define EN_CHAR_WIDTH						8
#define EN_CHAR_HEIGHT					16

#define FontAddress             (u32)0x080C0000									//汉字字模存放位置
#define AsciiFontAddress        (u32)0x080BF000									//ascii码字模存放位置

//RGB 565
#define RED                     0xf800
#define GREEN                   0x07e0
#define BLUE                    0x001f
#define BLACK                   0x0000 
#define WHITE                   0xffff
#define YELLOW  			          0xffe0
#define CYAN                    0x07ff
#define MAGENTA                 0xf81f
#define GRAY                    0x8410

//屏幕显示方向（原点在左上角） 0:顶到底/1:底到顶/2:右到左/3:左到右
#define SCREEN_DIR              3		//(23妯睆銆�01绔栧睆)               
  

/********************************* A23引脚 *************************************/
//A23(PE2)-------->高电平有效
#define LCD_A23_GPIO_PORT       GPIOE
#define LCD_A23_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define LCD_A23_GPIO_PIN        GPIO_Pin_2

#define LCD_DATA_ON							GPIO_SetBits(LCD_A23_GPIO_PORT,LCD_A23_GPIO_PIN)
#define LCD_CMD_ON						  GPIO_ResetBits(LCD_A23_GPIO_PORT,LCD_A23_GPIO_PIN)
/*************************************************************************************/

/********************************* LCD_RESET_PG7引脚 *************************************/
//LCD_RESET(PG7)-------->高电平有效
#define LCD_RES_GPIO_PORT      GPIOG
#define LCD_RES_GPIO_CLK       RCC_AHB1Periph_GPIOG
#define LCD_RES_GPIO_PIN       GPIO_Pin_7

#define LCD_RES_ON						 GPIO_SetBits(LCD_RES_GPIO_PORT,LCD_RES_GPIO_PIN)
#define LCD_RES_OFF						 GPIO_ResetBits(LCD_RES_GPIO_PORT,LCD_RES_GPIO_PIN)
/*************************************************************************************/


/*********************************** 初始化配置 ***************************************/
void LCD_GPIO_Init(void);
void Lcd_Reset(void);                                                   //复位
void Lcd_Initial(void);                                                 //LCD初始化命令
void Lcd_CmdWrite(uint8_t cmd);                                         //写入LCD命令
void Lcd_DataWrite(uint8_t data);                                       //写入LCD数据
void Lcd_AddrRange_Set(u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_FillAll(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);

/*********************************** 类 ***************************************/
void LCD_DrawOneEN(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext);				//英文字符
void LCD_DrawOneCH(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext);         //中文字符
void LCD_DrawMixText(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color,u16 bkcolor,u8 *Mixtext); //写文字

/*********************************** 画图形函数 ***************************************/
void LCD_DrawLine(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);       //画线
//画圆
void LCD_DrawPic(u16 xsta,u16 ysta,u16 xend,u16 yend,u8 *Pic);          //画图片
#endif 
/*********************************** END OF FILE ***************************************/
