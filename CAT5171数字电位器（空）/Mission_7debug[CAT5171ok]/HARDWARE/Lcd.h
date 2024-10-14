#ifndef _LCD_H_
#define _LCD_H_ 

#include "stm32f4xx.h"

extern volatile  unsigned char    *LCD_Add_PCmd;    //LCDָ����ַ
extern volatile  unsigned char    *LCD_Add_PDat;    //LCD���ݱ����ַ

#define CH_CHAR_WIDTH						16
#define CH_CHAR_HEIGHT					16

#define EN_CHAR_WIDTH						8
#define EN_CHAR_HEIGHT					16

#define FontAddress             (u32)0x080C0000									//������ģ���λ��
#define AsciiFontAddress        (u32)0x080BF000									//ascii����ģ���λ��

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

//��Ļ��ʾ����ԭ�������Ͻǣ� 0:������/1:�׵���/2:�ҵ���/3:����
#define SCREEN_DIR              3		//(23横屏、01竖屏)               
  

/********************************* A23���� *************************************/
//A23(PE2)-------->�ߵ�ƽ��Ч
#define LCD_A23_GPIO_PORT       GPIOE
#define LCD_A23_GPIO_CLK        RCC_AHB1Periph_GPIOE
#define LCD_A23_GPIO_PIN        GPIO_Pin_2

#define LCD_DATA_ON							GPIO_SetBits(LCD_A23_GPIO_PORT,LCD_A23_GPIO_PIN)
#define LCD_CMD_ON						  GPIO_ResetBits(LCD_A23_GPIO_PORT,LCD_A23_GPIO_PIN)
/*************************************************************************************/

/********************************* LCD_RESET_PG7���� *************************************/
//LCD_RESET(PG7)-------->�ߵ�ƽ��Ч
#define LCD_RES_GPIO_PORT      GPIOG
#define LCD_RES_GPIO_CLK       RCC_AHB1Periph_GPIOG
#define LCD_RES_GPIO_PIN       GPIO_Pin_7

#define LCD_RES_ON						 GPIO_SetBits(LCD_RES_GPIO_PORT,LCD_RES_GPIO_PIN)
#define LCD_RES_OFF						 GPIO_ResetBits(LCD_RES_GPIO_PORT,LCD_RES_GPIO_PIN)
/*************************************************************************************/


/*********************************** ��ʼ������ ***************************************/
void LCD_GPIO_Init(void);
void Lcd_Reset(void);                                                   //��λ
void Lcd_Initial(void);                                                 //LCD��ʼ������
void Lcd_CmdWrite(uint8_t cmd);                                         //д��LCD����
void Lcd_DataWrite(uint8_t data);                                       //д��LCD����
void Lcd_AddrRange_Set(u16 x1,u16 y1,u16 x2,u16 y2);
void LCD_FillAll(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);

/*********************************** �� ***************************************/
void LCD_DrawOneEN(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext);				//Ӣ���ַ�
void LCD_DrawOneCH(u16 xsta,u16 ysta,u16 color,u16 bkcolor,u8 *Mixtext);         //�����ַ�
void LCD_DrawMixText(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color,u16 bkcolor,u8 *Mixtext); //д����

/*********************************** ��ͼ�κ��� ***************************************/
void LCD_DrawLine(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);       //����
//��Բ
void LCD_DrawPic(u16 xsta,u16 ysta,u16 xend,u16 yend,u8 *Pic);          //��ͼƬ
#endif 
/*********************************** END OF FILE ***************************************/
