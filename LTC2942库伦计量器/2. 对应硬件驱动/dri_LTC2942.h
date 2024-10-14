#ifndef _DRI_LTC2942_H
#define _DRI_LTC2942_H

#include "stm32f4xx.h"
#include "i2c_protocol.h"

/*********************
* ������
**********************/
//����д���ݵ�ַ
#define ADDR_Device       0xC8

#define ADDR_DeviceWrite 	ADDR_Device|I2C_WR    //1100 1000
#define ADDR_DeviceRead	  ADDR_Device|I2C_RD    //1100 1001

//A:״̬�Ĵ���
#define ADDR_A		0x00
//B:���ƼĴ���
#define ADDR_B		0x01
//CD:�ۻ���ɼĴ���
#define ADDR_C		0x02
#define ADDR_D		0x03
//EF:��緧ֵ��
#define ADDR_E		0x04
#define ADDR_F		0x05
//GH:��緧ֵ��
#define ADDR_G		0x06
#define ADDR_H	  0x07

/*********************
* ��������
**********************/

void LTC2942_Init(void); //CAT5171��ʼ��

void LTC2942_writeOneByte(u8 reg, u8 data);
void LTC2942_writeTwoByte(u8 reg, u8 hbyte, u8 lbyte);
u8 LTC2942_readOneByte(u8 reg);
u16 LTC2942_readTwoByte(u8 reg);

//���ܺ���
void getbatteryCapacity(void);
unsigned char getbatteryPercent(void);
void batteryWarn(void);

#endif /*_DRI_LTC2942_H*/

