
#ifndef init_target_FILE
#define init_target_FILE 
  
#include "ExtVar.h" 
 
void  ChipHalInit(void);
void NVIC_Configuration(void);
void USART_Configuration(void);
void TIM2_Configuration(void);
void TIM2_IRQHandler(void);
void USART2_IRQHandler(void);
void GPIO_Configuration(void);
void SDRAM_Init(void);
void SDRAM_GPIOConfig(void);
void SDRAM_InitSequence(void);
static void delay(__IO uint32_t nCount);

void SDRAM_TEST_FUN(void);
void  Adc_Init(void);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 times);
extern void WS_YM_FUN(void);
void WSYM_TEST_FUN(void);

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
 

//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  

u32 STMFLASH_ReadWord(u32 faddr);		  	//读出字  
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
void TIM3_CODLE_FUN(void);
extern void AD_Sample_Fun(U32 s_temp);
void UART1_Rx_deal_Fun(unsigned char data);
extern void USART1_Putc(unsigned char c);

void U6_LY_Configuration(void);
void USART6_IRQHandler(void);
void UART6_Rx_deal_Fun_LY(unsigned char data);


void Dac1_Init(void);
void DAC_OUT_Vol(float f_temp);
void tesr_dac_fun(void);

#endif  
