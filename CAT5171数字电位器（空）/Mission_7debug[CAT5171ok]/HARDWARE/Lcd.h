#ifndef _LCD_H_
#define _LCD_H_ 

#include "stm32f4xx.h"

extern volatile  unsigned char    *LCD_Add_PCmd;    //LCDÖ¸Áî±£´æµØÖ·
extern volatile  unsigned char    *LCD_Add_PDat;    //LCDÊý¾Ý±£´æµØÖ·

#define CH_CHAR_WIDTH						16
#define CH_CHAR_HEIGHT					16

#define EN_CHAR_WIDTH						8
#define EN_CHAR_HEIGHT					16

#define FontAddress             (u32)0x080C0000									//ºº×Ö×ÖÄ£´æ·ÅÎ»ÖÃ
#define AsciiFontAddress        (u32)0x080BF000									//asciiÂë×ÖÄ£´æ·ÅÎ»ÖÃ

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

//ÆÁÄ»ÏÔÊ¾·½Ïò£¨Ô­µãÔÚ×óÉÏ½Ç£© 0:¶¥µ½µ×/1:µ×µ½¶¥/2:ÓÒµ½×ó/3:×óµ½ÓÒ
#define SCREEN_DIR              3		//(23æ¨ªå±ã€01ç«–å±)               
  

/********************************* A23Òý½Å *************************************/
//A23(PE2)-------->¸ßµçÆ½ÓÐÐ§
#define LCD_A23_GPIO_PORT       GPIOE
#define LCD_A23_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define LCD_A23_GPIO_PIN        GPIO_Pin_2

#define LCD_DATA_ON							GPIO_SetBits(LCD_A23_GPIO_PORT,LCD_A23_GPIO_PIN)
#define LCD_CMD_ON						  GPIO_ResetBits(LCD_A23_GPIO_PORT,LCD_A23_GPIO_PIN)
/*************************************************************************************/

/********************************* LCD_RESET_PG7Òý½Å *************************************/
//LCD_RESET(PG7)-------->¸ßµçÆ½ÓÐÐ§
#define LCD_RES_GPIO_PORT      GPIOG
#define LCD_RES_GPIO_CLK       RCC_AHB1Periph_GPIOG
#define LCD_RES_GPIO_PIN       GPIO_Pin_7

#define LCD_RES_ON						 GPIO_SetBits(LCD_RES_GPIO_PORT,LCD_RES_GPIO_PIN)
#define LCD_RES_OFF						 GPIO_ResetBits(LCD_RES_GPIO_PORT,LCD_RES_GPIO_PIN)
/*************************************************************************************/


/*********************************** ³õÊ¼»¯ÅäÖÃ ***************************************/
void LCD_GPIO_Init(void);
void Lcd_Reset(void);                                                   //¸´Î»
void Lcd_Initial(void);                                                 //LCD³õÊ¼»¯ÃüÁî
void Lcd_CmdWrite(uint8_t cmd);                                         //Ð´ÈëLCDÃüÁî
void Lcd_DataWrite(uint8_t data);                                       //Ð´ÈëLCDÊý¾Ý
void Lcd_AddrRange_Set(u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_FillAll(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);

/*********************************** Àà ***************************************/
void LCD_DrawOneEN(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext);				//Ó¢ÎÄ×Ö·û
void LCD_DrawOneCH(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext);         //ÖÐÎÄ×Ö·û
void LCD_DrawMixText(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color,u16 bkcolor,u8 *Mixtext); //Ð´ÎÄ×Ö

/*********************************** »­Í¼ÐÎº¯Êý ***************************************/
void LCD_DrawLine(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);       //»­Ïß
//»­Ô²
void LCD_DrawPic(u16 xsta,u16 ysta,u16 xend,u16 yend,u8 *Pic);          //»­Í¼Æ¬
#endif 
/*********************************** END OF FILE ***************************************/
