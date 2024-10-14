#include "digital_i2c.h"

void CAT5171_Stop(void);

void delay_us(unsigned int dly)
{
	unsigned int i;
	while (dly != 0)
	{
		dly--;
		for (i = 0; i < 37; i++);
	}
}

static void i2c_Delay(void)
{
	uint8_t i;

	/*　
		可用逻辑分析仪测量I2C通讯时的频率
    工作条件：CPU主频180MHz ，MDK编译环境，1级优化
  
		经测试，循环次数为20~250时都能通讯正常

	*/
	for (i = 0; i < 40; i++);
	
//	delay_us(5);
}

void CAT5171_Init(void)
{
//#if (MY_I2C == 1)
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;//SCL
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//SDA
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//SDA
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
//	GPIO_ResetBits(GPIOC, GPIO_Pin_11);   //赋予地址为0
	GPIO_SetBits(GPIOC, GPIO_Pin_11);   //赋予地址为1


		i2c_Config();
		
	//检查 i2c 的检查数据是否通
//		i2c_CheckDevice(0x5a);
	
//	#else
//	
//	i2c_CfgGpio();
//	
//	#endif
}
//#define	SDA_IN()	{GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=GPIO_Mode_IN<<14;}//sda输入
//#define	SDA_OUT()	{GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=GPIO_Mode_OUT<<14;}//sda输出

void SDA_OUT(void)
{
//	GPIO_InitTypeDef	GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//SDA,
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	GPIOH->MODER  &= ~( 0x03<< (2*10));	   //清空
	GPIOH->MODER |= (GPIO_Mode_OUT<<2*10); //设置输出模式
}
void SDA_IN(void)
{
//	GPIO_InitTypeDef	GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIOH->MODER  &= ~( 0x03<< (2*10));	   //清空
	GPIOH->MODER |= (GPIO_Mode_IN<<2*10); //设置输入模式
	
}


//-----------------------------
// CAT5171 开始标志
//-----------------------------
void CAT5171_Start(void)
{
//	SDA_OUT(); //SDA线输出
//	i2c_Delay();
  SDA_H();
  SCL_H();
  i2c_Delay(); //Tsu:sta
  
  SDA_L();
  i2c_Delay();       
	SCL_L();
  i2c_Delay();     
}

//-----------------------------
// CAT5171 结束标志
//-----------------------------
void CAT5171_Stop(void)
{
//	SDA_OUT(); //SDA线输出
  SDA_L();
  SCL_H();
  i2c_Delay(); //Tsu:sta
  SDA_H();
	
}

//-----------------------------
// Master 等待 CAT5171 响应
//-----------------------------
u8 CAT5171_WaitAck(void)
{
	unsigned int k;
	SCL_L();
	SDA_H();
	i2c_Delay();

	SCL_H();
	k = 0;
	
//	SDA_IN();
	while(Read_SDA())
	{
		k++;
		if(k > 200)
		{
//			CAT5171_Stop();
//			return 0;
			break;
		}
	}
//	SDA_L();     /////////////
//	SDA_IN();
	
//	i2c_Delay();   /////////////
//	i2c_Delay();
//	i2c_Delay();
	
	SCL_L();
	i2c_Delay();
	
	
	return 1;
}

//-----------------------------
// Master 响应 CAT5171
//-----------------------------
void CAT5171_Ack(void)
{        
	SDA_OUT();
	
	SDA_L();
	i2c_Delay();
	
	SCL_H();
	i2c_Delay();
	
	SCL_L();
	i2c_Delay();
	
	SDA_H();
}

//-----------------------------
// Master 没有响应 CAT5171
//-----------------------------
void CAT5171_NoAck(void)
{        
	SDA_OUT();

	SDA_H();
	i2c_Delay();
	SCL_H();
	i2c_Delay();
	SCL_L();
	i2c_Delay();
}

//-----------------------------
// Master ==> CAT5171
//-----------------------------
void CAT5171_WriteByte(unsigned char _val)
{
	u8 i = 0,temp_val = _val;
//	SDA_OUT();
	for(i = 0; i < 8; i++)
	{
		 SCL_L();
		 i2c_Delay();
		 if(temp_val & 0x80)
		 {
		 	SDA_H();
		 }
		 else
		 {
		 	SDA_L();
		 }
		 i2c_Delay();
		 SCL_H();
		 i2c_Delay();
		 temp_val<<=1;
	}
	SCL_L();
	i2c_Delay();
}

//-----------------------------
// CAT5171 ==> Master
//-----------------------------
unsigned char CAT5171_ReadByte(void)
{
	u8 i,ReadByte = 0x00;
	
  SDA_IN();//SDA设为输入
	SDA_H();
	delay_us(20);
	for(i = 0; i < 8; i++)
	{	
		ReadByte <<= 1;
		SCL_L();
		i2c_Delay();
		SCL_H();
		i2c_Delay();
		if(Read_SDA())
		{
			ReadByte |= 0x01;
		}
		else
		{
			ReadByte &= ~(0x01);	
		}
				
	}
	return ReadByte;
}


//-------------------------------------------------------

//CAT5117读取数据
void CAT5171_Write_Data(u8 byte)
{
	//1、确定地址
	i2c_Start();
	i2c_SendByte(Com_Write);//单挂载，写数据
	i2c_WaitAck();
	
	//2、发送电位器设置
	i2c_SendByte(Com_Wiper);    //
	i2c_WaitAck();         //等待回复
	
	//3、电位器寄存器设置
	i2c_SendByte(byte);    //设置电阻最大值
	i2c_WaitAck();         //等待回复
	i2c_Stop();            //结束
	
}

//CAT5117读取数据
u8 CAT5171_Read_Data(void)
{
	u8 ReadByte;
	
//	//1、确定地址
//	CAT5171_Start();
//	CAT5171_WriteByte(Com_Read);    //单挂载，读数据
//	CAT5171_WaitAck();               //等待回复
//	
//	//2、读出数据
//	ReadByte = CAT5171_ReadByte();
//	CAT5171_NoAck();    //无回复
//	CAT5171_Stop();     //结束
	
	i2c_Start();
	i2c_SendByte(Com_Read);
	i2c_WaitAck();
	
	ReadByte = i2c_ReadByte();
	i2c_NAck();
	i2c_Stop();
	
	
	return ReadByte;
}

//检测函数
uint8_t CAT5171_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck = 0;
	
	CAT5171_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
//	CAT5171_WriteByte(_Address | EEPROM_I2C_WR);
	CAT5171_WriteByte(0x58);
	CAT5171_WaitAck();	/* 检测设备的ACK应答 */
//	CAT5171_Ack();
//	CAT5171_NoAck();
	
	CAT5171_Stop();			/* 发送停止信号 */

	return ucAck;
}