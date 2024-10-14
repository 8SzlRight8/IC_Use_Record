#include "dri_LTC2942.h"

I2C_Dev_t * LTC2942;

////-------------------------------------------------------

//对寄存器B进行写0xFC操作；
//警报电压:3v(11)
//预分频:128(111)
//充电完成模式(10)；
//关掉模拟；(0)
//向寄存器中写入一个字节
void LTC2942_writeOneByte(u8 reg, u8 data)
{
	//1、开始
	LTC2942->i2c_start();
	
	//2、发送设备写地址，等待回应
	LTC2942->i2c_writeByte(ADDR_DeviceWrite);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//3、写入01寄存器，等待回应
	LTC2942->i2c_writeByte(reg);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//4、写入数据等待回应
	LTC2942->i2c_writeByte(data);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//5、发送读取数据时序
	LTC2942->i2c_stop();
}

////对寄存器CD EF GF 进行写操作；
////CD默认:7FFF; 充电完成后:FFFF
////EF:电池满电量提醒:4200mv
////GH:电池低电量提醒:3600mv
void LTC2942_writeTwoByte(u8 reg, u8 hbyte, u8 lbyte)
{
	//1、发送开始信号
	LTC2942->i2c_start();
	
	//2、发送设备写地址，并等待回应
	LTC2942->i2c_writeByte(ADDR_DeviceWrite);//11001000
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}
	
	//3、发送寄存器地址，并等待回应
	LTC2942->i2c_writeByte(reg);//o2h / 04h / 06h
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}
	
	//4、发送第一个字节的数据，并等待回应
	LTC2942->i2c_writeByte(hbyte);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//5、等待第二个字节的数据，并等待回应
	LTC2942->i2c_writeByte(lbyte);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//6、发送停止信号
	LTC2942->i2c_stop();
}

////A[7]:芯片识别; 1 : LTC2942;     0 : LTC2942;
////A[5]:指示累积电荷的值达到顶部或底部。
////A[3]:指示累积电荷值超过了电荷阈值上限。
////A[2]:指示累积电荷值下降到电量阈值下限以下。
////A[1]:VBAT警报指示电池电压（SENSE-）下降低于选定的VBAT阈值。
////A[0]:
u8 LTC2942_readOneByte(u8 reg)
{
	u8 receByte = 0;
	
	//1、发送开始信号
	LTC2942->i2c_start();
	
	//2、发送设备写地址，并等待回应
	LTC2942->i2c_writeByte(ADDR_DeviceWrite); //1100 1000
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}
	
	//3、发送寄存器的地址，并等待回应
	LTC2942->i2c_writeByte(reg);//00h
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}

	//4、重新发送开始指令，用于读取数据
	LTC2942->i2c_start();
	
	//5、发送读数据地址，并等待回应
	LTC2942->i2c_writeByte(ADDR_DeviceRead);//11001001
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}

	//6、读取数据，并通知从机，读取完毕。
	receByte = LTC2942->i2c_readByte();
	LTC2942->i2c_nack();
	
	//7、发送停止信号
	LTC2942->i2c_stop();

	return receByte;	
}


u16 LTC2942_readTwoByte(u8 reg)
{
	u16 receTwoByte = 0;
	u16 hbyte = 0;
	u8 lbyte = 0;
	
	//1、发送开始信号
	LTC2942->i2c_start();
	
	//2、发送设备写地址，并等待回应
	LTC2942->i2c_writeByte(ADDR_DeviceWrite); //1100 1000
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}
	
	//3、发送寄存器地址，并等待回应
	LTC2942->i2c_writeByte(reg);//o2h / 04h / 06h
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}

	//4、重新发送开始指令，用于读操作
	LTC2942->i2c_start();
	
	//5、发送设备的读地址，并等待回应
	LTC2942->i2c_writeByte(ADDR_DeviceRead);//11001001
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}
	
	//6、读取数据高字节，并回复从机
	hbyte = LTC2942->i2c_readByte();
	LTC2942->i2c_ack();
	
	//7、读取数据低字节，并回复从机
	lbyte= LTC2942->i2c_readByte();
	LTC2942->i2c_nack();
	
	//8、停止数据通信
	LTC2942->i2c_stop();

	receTwoByte = (hbyte<<8) | lbyte;
	return receTwoByte;
}

//初始化函数
void LTC2942_Init(void)
{
	unsigned char revByte = 0;
	
	//配置I2C的两个引脚
	LTC2942 = i2c_dev_create(); 
	LTC2942->i2c_checkDevice(ADDR_Device); //初始化端口 + 检查是否正常通讯
	
	//寄存器B 写入默认值，M为128.
	LTC2942_writeOneByte(ADDR_B,0x3C); //写入
	LTC2942_readOneByte(ADDR_B);       //读出查看是否是正确的
}


/*****************************
* 
* 功能函数
*
*****************************/
float Qlsb = 0;
float QBAT = 0;
float Rsense = 100;
float M = 128;
void getbatteryCapacity(void)
{
	u16 recvData = 0;
	
	u8 C = 0;
	u8 D = 0;
	recvData = LTC2942_readTwoByte(ADDR_C);
	C = recvData >> 8;
	D = recvData&0x00ff;

	Qlsb = 0.085*(50/Rsense)*(M/128);
	QBAT = Qlsb*recvData;	
	//得到电荷量后，进行电量显示
}


//电池百分比:	当前电量值除以总额度电量值；
u8 getbatteryPercent(void)
{
	u16 currentBAT = 0;//当前电量值
	u16 rateBAT = 0;//额定电量值
	static u8 bat_per = 0;  //百分比电量

	currentBAT = LTC2942_readTwoByte(ADDR_C);
	rateBAT = LTC2942_readTwoByte(ADDR_E);
	
	bat_per = (u8)(100.0 * ((float)currentBAT/(float)rateBAT));
	
	return bat_per;	
}


void batteryWarn(void)
{
	u8 receA = 0;
	u8 batteryHigh;
	u8 batteryLow;
	u8 batteryVeryLow;
	receA = LTC2942_readOneByte(ADDR_A);
	batteryVeryLow = (receA&0x02);
	batteryLow = (receA&0x04);
	batteryHigh = (receA&0x08);

	if(batteryLow)
	{
		//低电量提醒
	}

	if(batteryHigh)
	{
		//电池满电状态
	}

	if(batteryVeryLow)
	{
		//警报指示电池电压（SENSE-）下降低于选定的VBAT阈值。
	}
}


