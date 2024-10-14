#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"
#include "bsp_i2c_gpio.h"

/*******************************************************************/
//IO��������
//extern void DelayuS(unsigned int dly);

//ʱ�Ӹߵ�ƽ��ȡ����

#define SCL_H()	GPIO_SetBits(GPIOC, GPIO_Pin_12)
#define SCL_L() GPIO_ResetBits(GPIOC, GPIO_Pin_12)

#define SDA_H()	GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define SDA_L() GPIO_ResetBits(GPIOC, GPIO_Pin_10)

//����0����1
#define Read_SDA() GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)
/*******************************************************************/
#define Com_Write  0x5a
#define Com_Read   0x59

#define Com_Wiper  0x00

#define Digital_MAX 0xFF
#define Digital_MIN 0xFF

#define MY_I2C   0

void delay_us(unsigned int dly);

void	CAT5171_Init(void);

uint8_t CAT5171_CheckDevice(uint8_t _Address);

void CAT5171_Write_Data(unsigned char _val);
unsigned char CAT5171_Read_Data(void);


#endif

