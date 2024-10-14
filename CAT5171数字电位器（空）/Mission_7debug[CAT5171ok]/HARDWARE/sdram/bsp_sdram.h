#ifndef __SDRAM_H
#define	__SDRAM_H

#include "stm32f4xx.h"
#include <stdio.h>

#define W9825G6K 0x2000000  //32M�ֽ�

#define SDRAM_BANK_ADDR     ((uint32_t)0xC0000000)
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)      


/*��Ϣ���*/
#define SDRAM_DEBUG_ON        									 1

#define SDRAM_INFO(fmt,arg...)           printf("<<-SDRAM-INFO->> "fmt"\n",##arg)
#define SDRAM_ERROR(fmt,arg...)          printf("<<-SDRAM-ERROR->> "fmt"\n",##arg)
#define SDRAM_DEBUG(fmt,arg...)          do{\
                                          if(SDRAM_DEBUG_ON)\
                                          printf("<<-SDRAM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
/**
  * @}
  */  

/*�����Դ��������*/
#define LCD_LMT028_BLEN_PORT	  GPIOD
#define LCD_LMT028_BLEN_CLK 	  RCC_AHB1Periph_GPIOD
#define LCD_LMT028_BLEN_PIN 	  GPIO_Pin_2			

#define LCD_BLEN_ON             GPIO_SetBits(LCD_LMT028_BLEN_PORT,LCD_LMT028_BLEN_PIN)
#define LCD_BLEN_OFF            GPIO_ResetBits(LCD_LMT028_BLEN_PORT,LCD_LMT028_BLEN_PIN)																					

/*��Ļˢ������*/																	
#define LCD_LMT028_FMARK_PORT	  GPIOD
#define LCD_LMT028_FMARK_CLK 	  RCC_AHB1Periph_GPIOD
#define LCD_LMT028_FMARK_PIN 	  GPIO_Pin_13	
																					
#define LCD_FMARK_ON            GPIO_SetBits(LCD_LMT028_FMARK_PORT,LCD_LMT028_FMARK_PIN)
#define LCD_FMARK_OFF           GPIO_ResetBits(LCD_LMT028_FMARK_PORT,LCD_LMT028_FMARK_PIN)																			
																					
/*A���е�ַ�ź���*/    
#define FMC_A0_GPIO_PORT        GPIOF
#define FMC_A0_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A0_GPIO_PIN         GPIO_Pin_0
#define FMC_A0_PINSOURCE        GPIO_PinSource0
#define FMC_A0_AF               GPIO_AF_FMC

#define FMC_A1_GPIO_PORT        GPIOF
#define FMC_A1_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A1_GPIO_PIN         GPIO_Pin_1
#define FMC_A1_PINSOURCE        GPIO_PinSource1
#define FMC_A1_AF               GPIO_AF_FMC

#define FMC_A2_GPIO_PORT        GPIOF
#define FMC_A2_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A2_GPIO_PIN         GPIO_Pin_2
#define FMC_A2_PINSOURCE        GPIO_PinSource2
#define FMC_A2_AF               GPIO_AF_FMC

#define FMC_A3_GPIO_PORT        GPIOF
#define FMC_A3_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A3_GPIO_PIN         GPIO_Pin_3
#define FMC_A3_PINSOURCE        GPIO_PinSource3
#define FMC_A3_AF               GPIO_AF_FMC

#define FMC_A4_GPIO_PORT        GPIOF
#define FMC_A4_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A4_GPIO_PIN         GPIO_Pin_4
#define FMC_A4_PINSOURCE        GPIO_PinSource4
#define FMC_A4_AF               GPIO_AF_FMC

#define FMC_A5_GPIO_PORT        GPIOF
#define FMC_A5_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A5_GPIO_PIN         GPIO_Pin_5
#define FMC_A5_PINSOURCE        GPIO_PinSource5
#define FMC_A5_AF               GPIO_AF_FMC

#define FMC_A6_GPIO_PORT        GPIOF
#define FMC_A6_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A6_GPIO_PIN         GPIO_Pin_12
#define FMC_A6_PINSOURCE        GPIO_PinSource12
#define FMC_A6_AF               GPIO_AF_FMC

#define FMC_A7_GPIO_PORT        GPIOF
#define FMC_A7_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A7_GPIO_PIN         GPIO_Pin_13
#define FMC_A7_PINSOURCE        GPIO_PinSource13
#define FMC_A7_AF               GPIO_AF_FMC

#define FMC_A8_GPIO_PORT        GPIOF
#define FMC_A8_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A8_GPIO_PIN         GPIO_Pin_14
#define FMC_A8_PINSOURCE        GPIO_PinSource14
#define FMC_A8_AF               GPIO_AF_FMC

#define FMC_A9_GPIO_PORT        GPIOF
#define FMC_A9_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_A9_GPIO_PIN         GPIO_Pin_15
#define FMC_A9_PINSOURCE        GPIO_PinSource15
#define FMC_A9_AF               GPIO_AF_FMC


#define FMC_A10_GPIO_PORT        GPIOG
#define FMC_A10_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_A10_GPIO_PIN         GPIO_Pin_0
#define FMC_A10_PINSOURCE        GPIO_PinSource0
#define FMC_A10_AF               GPIO_AF_FMC


#define FMC_A11_GPIO_PORT        GPIOG
#define FMC_A11_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_A11_GPIO_PIN         GPIO_Pin_1
#define FMC_A11_PINSOURCE        GPIO_PinSource1
#define FMC_A11_AF               GPIO_AF_FMC

#define FMC_A12_GPIO_PORT        GPIOG
#define FMC_A12_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_A12_GPIO_PIN         GPIO_Pin_2
#define FMC_A12_PINSOURCE        GPIO_PinSource2
#define FMC_A12_AF               GPIO_AF_FMC
/*BA��ַ��*/
#define FMC_BA0_GPIO_PORT        GPIOG
#define FMC_BA0_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_BA0_GPIO_PIN         GPIO_Pin_4
#define FMC_BA0_PINSOURCE        GPIO_PinSource4
#define FMC_BA0_AF               GPIO_AF_FMC

#define FMC_BA1_GPIO_PORT        GPIOG
#define FMC_BA1_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_BA1_GPIO_PIN         GPIO_Pin_5
#define FMC_BA1_PINSOURCE        GPIO_PinSource5
#define FMC_BA1_AF               GPIO_AF_FMC

/*DQ�����ź���*/
#define FMC_D0_GPIO_PORT        GPIOD
#define FMC_D0_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_D0_GPIO_PIN         GPIO_Pin_14
#define FMC_D0_PINSOURCE        GPIO_PinSource14
#define FMC_D0_AF               GPIO_AF_FMC

#define FMC_D1_GPIO_PORT        GPIOD
#define FMC_D1_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_D1_GPIO_PIN         GPIO_Pin_15
#define FMC_D1_PINSOURCE        GPIO_PinSource15
#define FMC_D1_AF               GPIO_AF_FMC

#define FMC_D2_GPIO_PORT        GPIOD
#define FMC_D2_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_D2_GPIO_PIN         GPIO_Pin_0
#define FMC_D2_PINSOURCE        GPIO_PinSource0
#define FMC_D2_AF               GPIO_AF_FMC

#define FMC_D3_GPIO_PORT        GPIOD
#define FMC_D3_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_D3_GPIO_PIN         GPIO_Pin_1
#define FMC_D3_PINSOURCE        GPIO_PinSource1
#define FMC_D3_AF               GPIO_AF_FMC

#define FMC_D4_GPIO_PORT        GPIOE
#define FMC_D4_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D4_GPIO_PIN         GPIO_Pin_7
#define FMC_D4_PINSOURCE        GPIO_PinSource7
#define FMC_D4_AF               GPIO_AF_FMC

#define FMC_D5_GPIO_PORT        GPIOE
#define FMC_D5_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D5_GPIO_PIN         GPIO_Pin_8
#define FMC_D5_PINSOURCE        GPIO_PinSource8
#define FMC_D5_AF               GPIO_AF_FMC

#define FMC_D6_GPIO_PORT        GPIOE
#define FMC_D6_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D6_GPIO_PIN         GPIO_Pin_9
#define FMC_D6_PINSOURCE        GPIO_PinSource9
#define FMC_D6_AF               GPIO_AF_FMC

#define FMC_D7_GPIO_PORT        GPIOE
#define FMC_D7_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D7_GPIO_PIN         GPIO_Pin_10
#define FMC_D7_PINSOURCE        GPIO_PinSource10
#define FMC_D7_AF               GPIO_AF_FMC

#define FMC_D8_GPIO_PORT        GPIOE
#define FMC_D8_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D8_GPIO_PIN         GPIO_Pin_11
#define FMC_D8_PINSOURCE        GPIO_PinSource11
#define FMC_D8_AF               GPIO_AF_FMC

#define FMC_D9_GPIO_PORT        GPIOE
#define FMC_D9_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D9_GPIO_PIN         GPIO_Pin_12
#define FMC_D9_PINSOURCE        GPIO_PinSource12
#define FMC_D9_AF               GPIO_AF_FMC

#define FMC_D10_GPIO_PORT        GPIOE
#define FMC_D10_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D10_GPIO_PIN         GPIO_Pin_13
#define FMC_D10_PINSOURCE        GPIO_PinSource13
#define FMC_D10_AF               GPIO_AF_FMC

#define FMC_D11_GPIO_PORT        GPIOE
#define FMC_D11_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D11_GPIO_PIN         GPIO_Pin_14
#define FMC_D11_PINSOURCE        GPIO_PinSource14
#define FMC_D11_AF               GPIO_AF_FMC

#define FMC_D12_GPIO_PORT        GPIOE
#define FMC_D12_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_D12_GPIO_PIN         GPIO_Pin_15
#define FMC_D12_PINSOURCE        GPIO_PinSource15
#define FMC_D12_AF               GPIO_AF_FMC

#define FMC_D13_GPIO_PORT        GPIOD
#define FMC_D13_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_D13_GPIO_PIN         GPIO_Pin_8
#define FMC_D13_PINSOURCE        GPIO_PinSource8
#define FMC_D13_AF               GPIO_AF_FMC

#define FMC_D14_GPIO_PORT        GPIOD
#define FMC_D14_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_D14_GPIO_PIN         GPIO_Pin_9
#define FMC_D14_PINSOURCE        GPIO_PinSource9
#define FMC_D14_AF               GPIO_AF_FMC

#define FMC_D15_GPIO_PORT        GPIOD
#define FMC_D15_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_D15_GPIO_PIN         GPIO_Pin_10
#define FMC_D15_PINSOURCE        GPIO_PinSource10
#define FMC_D15_AF               GPIO_AF_FMC


/*�����ź���*/  
/*CSƬѡ*/
#define FMC_CS_GPIO_PORT        GPIOC
#define FMC_CS_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define FMC_CS_GPIO_PIN         GPIO_Pin_2
#define FMC_CS_PINSOURCE        GPIO_PinSource2
#define FMC_CS_AF               GPIO_AF_FMC

/*WEдʹ��*/
#define FMC_WE_GPIO_PORT        GPIOC
#define FMC_WE_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define FMC_WE_GPIO_PIN         GPIO_Pin_0
#define FMC_WE_PINSOURCE        GPIO_PinSource0
#define FMC_WE_AF               GPIO_AF_FMC
/*RAS��ѡͨ*/
#define FMC_RAS_GPIO_PORT        GPIOF
#define FMC_RAS_GPIO_CLK         RCC_AHB1Periph_GPIOF
#define FMC_RAS_GPIO_PIN         GPIO_Pin_11
#define FMC_RAS_PINSOURCE        GPIO_PinSource11
#define FMC_RAS_AF               GPIO_AF_FMC
/*CAS��ѡͨ*/
#define FMC_CAS_GPIO_PORT        GPIOG
#define FMC_CAS_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_CAS_GPIO_PIN         GPIO_Pin_15
#define FMC_CAS_PINSOURCE        GPIO_PinSource15
#define FMC_CAS_AF               GPIO_AF_FMC
/*CLKͬ��ʱ�ӣ��洢����2*/
#define FMC_CLK_GPIO_PORT        GPIOG
#define FMC_CLK_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_CLK_GPIO_PIN         GPIO_Pin_8
#define FMC_CLK_PINSOURCE        GPIO_PinSource8
#define FMC_CLK_AF               GPIO_AF_FMC
/*CKEʱ��ʹ�ܣ��洢����2*/
#define FMC_CKE_GPIO_PORT        GPIOC
#define FMC_CKE_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define FMC_CKE_GPIO_PIN         GPIO_Pin_3
#define FMC_CKE_PINSOURCE        GPIO_PinSource3
#define FMC_CKE_AF               GPIO_AF_FMC

/*DQM1��������*/
#define FMC_UDQM_GPIO_PORT        GPIOE
#define FMC_UDQM_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_UDQM_GPIO_PIN         GPIO_Pin_1
#define FMC_UDQM_PINSOURCE        GPIO_PinSource1
#define FMC_UDQM_AF               GPIO_AF_FMC
/*DQM0��������*/
#define FMC_LDQM_GPIO_PORT        GPIOE
#define FMC_LDQM_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_LDQM_GPIO_PIN         GPIO_Pin_0
#define FMC_LDQM_PINSOURCE        GPIO_PinSource0
#define FMC_LDQM_AF               GPIO_AF_FMC


/** @defgroup STM32429 SDRAM����
  * @{
  */ 
void  SDRAM_Init(void);
void  SDRAM_WriteBuffer(uint32_t* pBuffer, uint32_t uwWriteAddress, uint32_t uwBufferSize);
void  SDRAM_ReadBuffer(uint32_t* pBuffer, uint32_t uwReadAddress, uint32_t uwBufferSize);
uint8_t SDRAM_Test(void);

static void WSYM_GPIO_Config(void);
void WS_YM_FUN(void);


/*A���е�ַ�ź���*/    
#define FMC_A13_GPIO_PORT        GPIOG
#define FMC_A13_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_A13_GPIO_PIN         GPIO_Pin_3
#define FMC_A13_PINSOURCE        GPIO_PinSource3
#define FMC_A13_AF               GPIO_AF_FMC

#define FMC_A19_GPIO_PORT        GPIOE
#define FMC_A19_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_A19_GPIO_PIN         GPIO_Pin_3
#define FMC_A19_PINSOURCE        GPIO_PinSource3
#define FMC_A19_AF               GPIO_AF_FMC

#define FMC_A20_GPIO_PORT        GPIOE
#define FMC_A20_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_A20_GPIO_PIN         GPIO_Pin_4
#define FMC_A20_PINSOURCE        GPIO_PinSource4
#define FMC_A20_AF               GPIO_AF_FMC

#define FMC_A21_GPIO_PORT        GPIOE
#define FMC_A21_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_A21_GPIO_PIN         GPIO_Pin_5
#define FMC_A21_PINSOURCE        GPIO_PinSource5
#define FMC_A21_AF               GPIO_AF_FMC

#define FMC_A22_GPIO_PORT        GPIOE
#define FMC_A22_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define FMC_A22_GPIO_PIN         GPIO_Pin_6
#define FMC_A22_PINSOURCE        GPIO_PinSource6
#define FMC_A22_AF               GPIO_AF_FMC

//#define FMC_A23_GPIO_PORT        GPIOE
//#define FMC_A23_GPIO_CLK         RCC_AHB1Periph_GPIOE
//#define FMC_A23_GPIO_PIN         GPIO_Pin_2
//#define FMC_A23_PINSOURCE        GPIO_PinSource2
//#define FMC_A23_AF               GPIO_AF_FMC

/*�����ź���*/  
/*CSƬѡ*/
#define FMC_RAM_CS_GPIO_PORT        GPIOG
#define FMC_RAM_CS_GPIO_CLK         RCC_AHB1Periph_GPIOG
#define FMC_RAM_CS_GPIO_PIN         GPIO_Pin_9
#define FMC_RAM_CS_PINSOURCE        GPIO_PinSource9
#define FMC_RAM_CS_AF               GPIO_AF_FMC

/*WEдʹ��*/
#define FMC_RAM_WE_GPIO_PORT        GPIOD
#define FMC_RAM_WE_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_RAM_WE_GPIO_PIN         GPIO_Pin_5
#define FMC_RAM_WE_PINSOURCE        GPIO_PinSource5
#define FMC_RAM_WE_AF               GPIO_AF_FMC
/*��ʹ��*/
#define FMC_RAM_OE_GPIO_PORT        GPIOD
#define FMC_RAM_OE_GPIO_CLK         RCC_AHB1Periph_GPIOD
#define FMC_RAM_OE_GPIO_PIN         GPIO_Pin_4
#define FMC_RAM_OE_PINSOURCE        GPIO_PinSource4
#define FMC_RAM_OE_AF               GPIO_AF_FMC
#endif /* __SDRAM_H */
