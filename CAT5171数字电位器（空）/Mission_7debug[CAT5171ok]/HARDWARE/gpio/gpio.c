#include "gpio.h"
#include "ExtVar.h"

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOG,ENABLE); 
	
	/**********���İ�*****************/
	
	//������													 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	BEEP_OFF
	
/****************ADC*****************************************/
	 //ADCת��ʱ��														 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 //ADC��λ												 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	//ADCæ���
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;    //����
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	/*********�������***************/ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/**********60kv�����忪��*****************/
	//�����Ƚ��丳ֵΪ���õĵ�ƽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//����˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//�͵�ƽ��Ч
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	d_TIM_EN 
//	d_TIM_DIS
}
