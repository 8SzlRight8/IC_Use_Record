#ifndef Define_FILE
#define Define_FILE
 
#include "./usart/bsp_rs232_usart.h"

#define   DEBUG_MODLE   0   //1:use for debug version;   0: use for release version

//1、加上包宏，更改每包的大小。  2、然后再进行采样数据的处理
#define   PACK_ALL      2000
#define   PACK_ONE      PACK_ALL/2
#define   PACK_BT       PACK_ONE/2

//2、更改两个电压采集单元   （只用修改宏定义即可） 
#define   MX_NUM       100   //母线电压
#define   MX_OFF       20
#define   CV_NUM       500   //电容电压
#define   CV_OFF       20

//串口中断开启
#define	  d_LY_RX_IRQ_EN          USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE); 
#define	  d_LY_RX_IRQ_DIS         USART_ITConfig(RS232_USART, USART_IT_RXNE, DISABLE);

//AD采样
#define	  AD_SAMPLE_H          GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define	  AD_SAMPLE_L          GPIO_ResetBits(GPIOB, GPIO_Pin_0); 

//AD复位
#define	  AD_RESET_H          GPIO_SetBits(GPIOG, GPIO_Pin_3);   //高电平复位
#define	  AD_RESET_L          GPIO_ResetBits(GPIOG, GPIO_Pin_3);

//脉冲输出
#define   d_PULSE_OUT_EN      GPIO_SetBits(GPIOA, GPIO_Pin_12);
#define   d_PULSE_OUT_DIS     GPIO_ResetBits(GPIOA, GPIO_Pin_12);

#define   d_TIM_EN      GPIO_ResetBits(GPIOC, GPIO_Pin_9);
#define   d_TIM_DIS     GPIO_SetBits(GPIOC, GPIO_Pin_9);

#define   d_DCDC_DCAC_EN       GPIO_SetBits(GPIOB, GPIO_Pin_9);GPIO_SetBits(GPIOB, GPIO_Pin_7);
#define   d_DCDC_DCAC_DIS      GPIO_ResetBits(GPIOB, GPIO_Pin_9);GPIO_ResetBits(GPIOB, GPIO_Pin_7);

#define   d_DCDC_EN       		GPIO_SetBits(GPIOB, GPIO_Pin_7);
#define   d_DCDC_DIS      		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
#define   d_DCAC_EN       		GPIO_SetBits(GPIOB, GPIO_Pin_9);
#define   d_DCAC_DIS      		GPIO_ResetBits(GPIOB, GPIO_Pin_9);

/****************429核心板*************************/
#define   BEEP_ON		       GPIO_ResetBits(GPIOC, GPIO_Pin_1);
#define   BEEP_OFF	           GPIO_SetBits(GPIOC, GPIO_Pin_1); 


/**************************************************************************************************
说明：定义数据类型
*****************************************************************************************************/ 
#define I8    signed char
#define U8  unsigned char     
#define I16   signed short   
#define U16 unsigned short    
#define I32   signed long   
#define U32 unsigned long   
#define BOOLEAN U8

typedef unsigned char  uint8;                                           /*  无符号8位整型变量           */
typedef signed   char  int8;                                            /*  有符号8位整型变量           */
typedef unsigned short uint16;                                             /*  无符号16位整型变量          */
typedef signed   short int16;                                           /*  有符号16位整型变量          */
typedef unsigned int   uint32;                                          /*  无符号32位整型变量          */
typedef signed   int   int32;                                           /*  有符号32位整型变量          */
typedef float          fp32;                                            /*  单精度浮点数（32位长度）    */
typedef double         fp64;                                            /*  双精度浮点数（64位长度）    */

typedef unsigned char  INT8U;                                           /*  无符号8位整型变量           */
typedef signed   char  INT8S;                                           /*  有符号8位整型变量           */
typedef unsigned short INT16U;                                          /*  无符号16位整型变量          */
typedef signed   short INT16S;                                          /*  有符号16位整型变量          */
typedef unsigned int   INT32U;                                          /*  无符号32位整型变量          */
typedef signed   int   INT32S;                                          /*  有符号32位整型变量          */
typedef float          FP32;                                            /*  单精度浮点数（32位长度）    */
typedef double         FP64;                                            /*  双精度浮点数（64位长度）    */

#endif /*Define_FILE*/

