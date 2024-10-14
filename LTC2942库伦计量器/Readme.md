# LTC2942驱动使用记录

2024.10.13

---



## 前言

硬件平台：STM32F429ZGT6

软件平台：keil



参考博主文章：https://blog.csdn.net/qq_42837982/article/details/103627328



## 一、I2C 文件 和 LTC2942如何调用

### 1.1 修改步骤

~~~~c
/***************************** 使 用 说 明 *****************************
* ----------------------------------------------------------------------
* 
* 调 试 平 台 ：STM32F429ZET6
* I2C（软/硬）：软件模拟I2C。
* SDA、SCL端口配置：外部加上拉电阻，OD开漏输出
* 
* ----------------------------------------------------------------------
* 
* 使用步骤：
* 1、若I2C的GPIO口不对，则修改为对应的GPIO口宏定义 即可
*    I2C_SCL_PORT、I2C_SCL_CLK、I2C_SCL_PIN	以及
*    I2C_SDA_PORT、I2C_SDA_CLK、I2C_SDA_PIN	  
*    （初始化端口在第四步中调用）
* 
* 2、包含此头文件到对应的设备驱动文件，定义一个全局 的I2C结构体变量
* 	 I2C_Dev_t * i2c_LTC2942;   //i2c_LTC2942 为自己取的变量名
* 
* 3、在使用前 调用函数 i2c_LTC2942 = i2c_dev_create();即可。 
* 	 （此函数作用：创建 I2C结构体变量，并初始化）
* 
* 4、随后即可用指针的方式调用对应的函数（方法）
*    Eg: i2c_dev->i2c_checkDevice(0xC8)...//检查是否能正常通信
* 
* 注意：【I2C总线只挂一个设备不用看】
*    1）用指针操作方便了，但是没有函数传参说明，需要到头文件查看。
*       （不过有参数没写的话，编译器可检查，keil中F12可调转到申明）
* 
*    2）定义了一个 i2c_dev_del(I2C_Dev_t* dev) 函数，有点问题，如果
*       全程不需要删除i2c设备，可不用关心。需要使用内存池就能解决。
*       （自带malloc和free函数的问题，申请次数过多容易造成内存溢出）。
* 
* ----------------------------------------------------------------------
***********************************************************************/
~~~~



### 1.2 如何调用

~~~c
0、准备工作
    需在 dri_LTC2942.c 中添加 #include "i2c_protocol.h" 
    需在 main.c 中添加 #include "dri_LTC2942.h" 
        
1、在 dri_LTC2942.c 文件中 定义一个全局变量
I2C_Dev_t * LTC2942;

2、在main.c 调用 LTC2942_Init() , 初始化LTC2942。

3、根据需求 调用 获取库伦量 getbatteryCapacity() 以及 获取电量百分比 batteryWarn() 函数.
~~~





## 二、看LTC2942写驱动程序

### 2.1 写一个字节时序

![image-20241013190558728](C:\Users\20477\AppData\Roaming\Typora\typora-user-images\image-20241013190558728.png)

~~~c
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
~~~



### 2.2 写两个字节时序

![image-20241013191240289](C:\Users\20477\AppData\Roaming\Typora\typora-user-images\image-20241013191240289.png)

~~~~c
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
~~~~



### 2.3 读一个字节时序

![image-20241013191518122](C:\Users\20477\AppData\Roaming\Typora\typora-user-images\image-20241013191518122.png)

~~~~c
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
~~~~



### 2.4 读两个字节时序

![image-20241013191818898](C:\Users\20477\AppData\Roaming\Typora\typora-user-images\image-20241013191818898.png)

~~~~c
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

~~~~



### 2.5 初始化函数

~~~c
//初始化函数
void LTC2942_Init(void)
{
	unsigned char revByte = 0;
	
	//配置I2C的两个引脚
	LTC2942 = i2c_dev_create(); 
	LTC2942->i2c_checkDevice(ADDR_Device); //初始化端口 + 检查是否正常通讯
	
	//寄存器B 写入默认值，M为128.
	LTC2942_writeOneByte(ADDR_B,0x3C);
}
~~~



### 2.6 获取库伦量

~~~c
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
~~~



### 2.7 获取电量百分比

~~~c
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
~~~



### 2.8 报警函数

~~~c
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
~~~



## 三、LTC2942功能记录实现

~~~c
1、测得电池中的库仑量，对电池电量进行判断。不会出现电压测电量数据忽上忽下的情况。【已经完成】
    
2、几种报警方式【还未用】
~~~



## 四、硬件连接情况

LTC2942推荐电路

![image-20241013193945639](C:\Users\20477\AppData\Roaming\Typora\typora-user-images\image-20241013193945639.png)



硬件LTC2942部分原理图（底板）

![image-20241013193841243](C:\Users\20477\AppData\Roaming\Typora\typora-user-images\image-20241013193841243.png)



