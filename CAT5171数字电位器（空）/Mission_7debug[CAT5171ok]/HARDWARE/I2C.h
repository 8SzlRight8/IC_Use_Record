#ifndef I2C_H
#define I2C_H

#include "stm32f4xx.h"
/*******************************************************************/
//IO��������
//extern void DelayuS(unsigned int dly);

typedef struct 
{
    unsigned short Year;                                                        //  ��ǰ��                     
    unsigned char  Month;                                                       //  ��ǰ��                     
    unsigned char  Day;                                                         //  �������漸��               
    unsigned char  Week;                                                        //  ���ڼ���0-6��              
    unsigned char  Hour;                                                        //  Сʱ                        
    unsigned char  Minute;                                                      //  ����                      
    unsigned char  Second;                                                      //  ��                          
}RTCTime;

#define SCL_H()	GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define SCL_L() GPIO_ResetBits(GPIOC, GPIO_Pin_4)

#define SDA_H()	GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define SDA_L() GPIO_ResetBits(GPIOC, GPIO_Pin_5)

//����0����1
#define Read_SDA() GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
/*******************************************************************/
#define ReadCode 0xa3
#define WriteCode 0xa2

void delay_us(unsigned int dly);

u8 PCF8563_ReaDAdress(u8 Adress);
void PCF8563_WriteAdress(u8 Adress, u8 DataTX);
void PCF8563_CLKOUT_1s(void);
RTCTime PCF8563_ReadTime(void);
void PCF8563_Init(RTCTime rtctime);
void RTC_initial(void);
void IIC_Init(void);

void RTC_initial_chgtime(u16 Year,u8 Month,u8 Day,u8 Week,u8 Hour,u8 Minute,u8 Second);
#endif

