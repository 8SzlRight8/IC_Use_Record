#ifndef Define_FILE
#define Define_FILE
 
#include "./usart/bsp_rs232_usart.h"

#define   DEBUG_MODLE   0   //1:use for debug version;   0: use for release version

//1�����ϰ��꣬����ÿ���Ĵ�С��  2��Ȼ���ٽ��в������ݵĴ���
#define   PACK_ALL      2000
#define   PACK_ONE      PACK_ALL/2
#define   PACK_BT       PACK_ONE/2

//2������������ѹ�ɼ���Ԫ   ��ֻ���޸ĺ궨�弴�ɣ� 
#define   MX_NUM       100   //ĸ�ߵ�ѹ
#define   MX_OFF       20
#define   CV_NUM       500   //���ݵ�ѹ
#define   CV_OFF       20

//�����жϿ���
#define	  d_LY_RX_IRQ_EN          USART_ITConfig(RS232_USART, USART_IT_RXNE, ENABLE); 
#define	  d_LY_RX_IRQ_DIS         USART_ITConfig(RS232_USART, USART_IT_RXNE, DISABLE);

//AD����
#define	  AD_SAMPLE_H          GPIO_SetBits(GPIOB, GPIO_Pin_0);
#define	  AD_SAMPLE_L          GPIO_ResetBits(GPIOB, GPIO_Pin_0); 

//AD��λ
#define	  AD_RESET_H          GPIO_SetBits(GPIOG, GPIO_Pin_3);   //�ߵ�ƽ��λ
#define	  AD_RESET_L          GPIO_ResetBits(GPIOG, GPIO_Pin_3);

//�������
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

/****************429���İ�*************************/
#define   BEEP_ON		       GPIO_ResetBits(GPIOC, GPIO_Pin_1);
#define   BEEP_OFF	           GPIO_SetBits(GPIOC, GPIO_Pin_1); 


/**************************************************************************************************
˵����������������
*****************************************************************************************************/ 
#define I8    signed char
#define U8  unsigned char     
#define I16   signed short   
#define U16 unsigned short    
#define I32   signed long   
#define U32 unsigned long   
#define BOOLEAN U8

typedef unsigned char  uint8;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  int8;                                            /*  �з���8λ���ͱ���           */
typedef unsigned short uint16;                                             /*  �޷���16λ���ͱ���          */
typedef signed   short int16;                                           /*  �з���16λ���ͱ���          */
typedef unsigned int   uint32;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   int32;                                           /*  �з���32λ���ͱ���          */
typedef float          fp32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         fp64;                                            /*  ˫���ȸ�������64λ���ȣ�    */

typedef unsigned char  INT8U;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  INT8S;                                           /*  �з���8λ���ͱ���           */
typedef unsigned short INT16U;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short INT16S;                                          /*  �з���16λ���ͱ���          */
typedef unsigned int   INT32U;                                          /*  �޷���32λ���ͱ���          */
typedef signed   int   INT32S;                                          /*  �з���32λ���ͱ���          */
typedef float          FP32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         FP64;                                            /*  ˫���ȸ�������64λ���ȣ�    */

#endif /*Define_FILE*/

