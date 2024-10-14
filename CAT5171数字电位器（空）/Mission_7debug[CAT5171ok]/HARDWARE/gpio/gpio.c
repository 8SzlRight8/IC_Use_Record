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
	
	/**********核心板*****************/
	
	//蜂鸣器													 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	BEEP_OFF
	
/****************ADC*****************************************/
	 //ADC转化时钟														 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 //ADC复位												 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	//ADC忙检测
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;    //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	/*********开机检测***************/ 
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
	
	/**********60kv，脉冲开关*****************/
	//可以先将其赋值为不用的电平
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
	
	//脉冲端口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	//低电平有效
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	d_TIM_EN 
//	d_TIM_DIS
}
