#ifndef _DRI_LTC2942_H
#define _DRI_LTC2942_H

#include "stm32f4xx.h"
#include "i2c_protocol.h"

/*********************
* 命令码
**********************/
//读、写数据地址
#define ADDR_Device       0xC8

#define ADDR_DeviceWrite 	ADDR_Device|I2C_WR    //1100 1000
#define ADDR_DeviceRead	  ADDR_Device|I2C_RD    //1100 1001

//A:状态寄存器
#define ADDR_A		0x00
//B:控制寄存器
#define ADDR_B		0x01
//CD:累积电荷寄存器
#define ADDR_C		0x02
#define ADDR_D		0x03
//EF:充电阀值高
#define ADDR_E		0x04
#define ADDR_F		0x05
//GH:充电阀值低
#define ADDR_G		0x06
#define ADDR_H	  0x07

/*********************
* 函数声明
**********************/

void LTC2942_Init(void); //CAT5171初始化

void LTC2942_writeOneByte(u8 reg, u8 data);
void LTC2942_writeTwoByte(u8 reg, u8 hbyte, u8 lbyte);
u8 LTC2942_readOneByte(u8 reg);
u16 LTC2942_readTwoByte(u8 reg);

//功能函数
void getbatteryCapacity(void);
unsigned char getbatteryPercent(void);
void batteryWarn(void);

#endif /*_DRI_LTC2942_H*/

