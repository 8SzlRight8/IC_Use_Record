#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

#define RHEOSTAT_NOFCHANEL      2

/*=====================通道1 IO======================*/
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT1    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN1     GPIO_Pin_1
#define RHEOSTAT_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOA
#define RHEOSTAT_ADC_CHANNEL1      ADC_Channel_1
/*=====================通道2 IO ======================*/
// ADC IO宏定义	//PB0 channel 为8
#define RHEOSTAT_ADC_GPIO_PORT2    GPIOA
#define RHEOSTAT_ADC_GPIO_PIN2     GPIO_Pin_0
#define RHEOSTAT_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOA
#define RHEOSTAT_ADC_CHANNEL2      ADC_Channel_0
//// ADC IO宏定义	//PB0 channel 为8
//#define RHEOSTAT_ADC_GPIO_PORT2    GPIOB
//#define RHEOSTAT_ADC_GPIO_PIN2     GPIO_Pin_0
//#define RHEOSTAT_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOB
//#define RHEOSTAT_ADC_CHANNEL2      ADC_Channel_8

// ADC 序号宏定义
#define RHEOSTAT_ADC              ADC1
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC1
// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define RHEOSTAT_ADC_DR_ADDR    ((u32)ADC1+0x4c)


// ADC DMA 通道宏定义，这里我们使用DMA传输
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0


void ADC1_Doub_Init(void);

#endif /* __BSP_ADC_H */



