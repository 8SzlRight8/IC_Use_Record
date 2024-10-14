#ifndef _BSP_ADVANCE_TIM_H_
#define	_BSP_ADVANCE_TIM_H_

#include "stm32f4xx.h"

/* ��ʱ�� */
#define ADVANCE_TIM           		    TIM1
#define ADVANCE_TIM_CLK       		    RCC_APB2Periph_TIM1

/* TIMͨ��1������� */
#define ADVANCE_OCPWM_PIN             GPIO_Pin_8
#define ADVANCE_OCPWM_GPIO_PORT       GPIOA
#define ADVANCE_OCPWM_GPIO_CLK        RCC_AHB1Periph_GPIOA
#define ADVANCE_OCPWM_PINSOURCE				GPIO_PinSource8
#define ADVANCE_OCPWM_AF							GPIO_AF_TIM1

/* TIMͨ��1����������� */
#define ADVANCE_OCNPWM_PIN            GPIO_Pin_7
#define ADVANCE_OCNPWM_GPIO_PORT      GPIOA
#define ADVANCE_OCNPWM_GPIO_CLK       RCC_AHB1Periph_GPIOA
#define ADVANCE_OCNPWM_PINSOURCE			GPIO_PinSource7
#define ADVANCE_OCNPWM_AF							GPIO_AF_TIM1

/* TIM��·�������� */
#define ADVANCE_BKIN_PIN              GPIO_Pin_6              
#define ADVANCE_BKIN_GPIO_PORT        GPIOA                      
#define ADVANCE_BKIN_GPIO_CLK         RCC_AHB1Periph_GPIOA
#define ADVANCE_BKIN_PINSOURCE	  		GPIO_PinSource6
#define ADVANCE_BKIN_AF						  	GPIO_AF_TIM1

void TIM_Advance_Configuration(void);

void TIM_Add_Pulse(void);
void TIM_Reduce_Pulse(void);

#endif /* _BSP_ADVANCE_TIM_H_ */

