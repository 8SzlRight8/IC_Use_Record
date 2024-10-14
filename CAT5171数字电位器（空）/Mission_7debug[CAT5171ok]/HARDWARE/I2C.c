#include "I2C.h"

void delay_us(unsigned int dly)
{
	unsigned int i;
	while (dly != 0)
	{
		dly--;
		for (i = 0; i < 37; i++);
	}
}

void	IIC_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;//SDA,SCL
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;//SDA,SCL
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	SDA_H();
	SCL_H();
}
//#define	SDA_IN()	{GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=GPIO_Mode_IN<<14;}//sda����
//#define	SDA_OUT()	{GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=GPIO_Mode_OUT<<14;}//sda���

void SDA_OUT(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;//SDA,SCL
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}
void SDA_IN(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
	
void IIC_Start(void)
{
	SDA_OUT(); //SDA�����
    SDA_H();
    SCL_H();
    delay_us(40);
    
    SDA_L();
    delay_us(40);
    SDA_L();
    delay_us(40);
       
}
void IIC_Stop(void)
{
    SDA_OUT();
    SCL_L();		//1
    delay_us(40);	// 2
    SDA_L();		// 3. 1,2,3�����в���ȱ��
    delay_us(40);
    SCL_H();
    delay_us(40);
    SDA_H();
    delay_us(40);

}
void IIC_WaitAck(void)
{
	unsigned int k;
  SDA_IN();
	SCL_L();
	SDA_H();
	delay_us(40);

	SCL_H();
	k = 0;
	while((Read_SDA()!= 0) && (k < 120000))k++;
	delay_us(40);
	SCL_L();
	delay_us(40);	
}

void IIC_WriteByte(u8 byte)
{
	u8 i = 0;
	SDA_OUT();
	delay_us(20);
	for(i = 0; i < 8; i++)
	{
		 SCL_L();
		 delay_us(40);
		 if(byte & 0x80)
		 {
		 	SDA_H();
		 }
		 else
		 {
		 	SDA_L();
		 }
		 delay_us(40);
		 SCL_H();
		 delay_us(40);
		 byte<<=1;
	}
	SCL_L();
	delay_us(40);
}

u8 IIC_ReadByte(void)
{
	u8 i,ReadByte;
	
  SDA_IN();//SDA��Ϊ����
	SDA_H();
	delay_us(20);
	for(i = 0; i < 8; i++)
	{	
		ReadByte <<= 1;
		SCL_L();
		delay_us(40);
		SCL_H();
		delay_us(40);
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
void I2C_Ack(void)
{        
    SCL_L();
    delay_us(40);
    SDA_OUT();
	
    delay_us(40);
    SDA_L();
    delay_us(40);
    SCL_H();
    delay_us(40);
    SCL_L();
    delay_us(20);
}

void I2C_NoAck(void)
{        
        SCL_L();
        delay_us(40);
        SDA_OUT();
	
        delay_us(40);
        SDA_H();
        delay_us(40);
        SCL_H();
        delay_us(40);
        SCL_L();
        delay_us(40);
}

u8 PCF8563_ReaDAdress(u8 Adress)
{
	u8 ReadData;

	IIC_Start();
	IIC_WriteByte(0xa2);  
	IIC_WaitAck();
	IIC_WriteByte(Adress);
	IIC_WaitAck();
	IIC_Start();
	IIC_WriteByte(0xa3);  
	IIC_WaitAck();
	ReadData = IIC_ReadByte();
	IIC_Stop();
	return ReadData;
}

void PCF8563_WriteAdress(u8 Adress,u8 DataTX)
{
	IIC_Start();
	IIC_WriteByte(0xa2);
	IIC_WaitAck();
	IIC_WriteByte(Adress);
	IIC_WaitAck();
	IIC_WriteByte(DataTX);
	IIC_WaitAck();
	IIC_Stop();
}

//��CLKOUT�϶�ʱ1S���һ���½������壬������֤����������STM32��GPIO�������룬���ó��½����жϣ���Ƭ��ÿ��1S����һ���ж�
void PCF8563_CLKOUT_1s(void)
{
	PCF8563_WriteAdress(0x01, 0);	//��ֹ��ʱ��������������
	PCF8563_WriteAdress(0x0e, 0);	//�رն�ʱ���ȵ�
	PCF8563_WriteAdress(0x0d, 0x83);  //���������
}

unsigned char DECToBCD(unsigned char dec)
{
	return ((0xff & ((dec / 10) << 4)) | (0x0f & (dec % 10)));
}

unsigned char BCDToDEC(unsigned char bcd)
{
	return ((0xff & (bcd >> 4)) * 10 + (bcd & 0x0f));
}

//ȡ�ذ˸��ֽڵ�����:�룬�֣�ʱ���죬���ڣ��·ݣ���ݡ�

RTCTime PCF8563_ReadTime(void)
{
	RTCTime rtctime;
	u8 PCF8563_Time1[7]={0};
	IIC_Start();
	IIC_WriteByte(0xa2);
	IIC_WaitAck();
	IIC_WriteByte(0x02);
	IIC_WaitAck();
	IIC_Start();
	IIC_WriteByte(0xa3);
	IIC_WaitAck();
	PCF8563_Time1[0] = IIC_ReadByte()&0x7f;
	I2C_Ack();
	
	PCF8563_Time1[1] = IIC_ReadByte()&0x7f;
	I2C_Ack();

	PCF8563_Time1[2] = IIC_ReadByte()&0x3f;
	I2C_Ack();

	PCF8563_Time1[3] = IIC_ReadByte()&0x3f;
	I2C_Ack();
	
	PCF8563_Time1[4] = IIC_ReadByte()&0x07;
	I2C_Ack();
	
	PCF8563_Time1[5] = IIC_ReadByte()&0x1f;
	I2C_Ack();
	
	PCF8563_Time1[6] = IIC_ReadByte();
	I2C_NoAck();
	IIC_Stop();

	rtctime.Second  = ((PCF8563_Time1[0]&0xf0)>>4)*10 + (PCF8563_Time1[0]&0x0f);
	rtctime.Minute = ((PCF8563_Time1[1]&0xf0)>>4)*10 + (PCF8563_Time1[1]&0x0f);
	rtctime.Hour = ((PCF8563_Time1[2]&0xf0)>>4)*10 + (PCF8563_Time1[2]&0x0f);
	rtctime.Day = ((PCF8563_Time1[3]&0xf0)>>4)*10 + (PCF8563_Time1[3]&0x0f);
	rtctime.Week = ((PCF8563_Time1[4]&0xf0)>>4)*10 + (PCF8563_Time1[4]&0x0f);
	rtctime.Month = ((PCF8563_Time1[5]&0xf0)>>4)*10 + (PCF8563_Time1[5]&0x0f);
	rtctime.Year = ((PCF8563_Time1[6]&0xf0)>>4)*10 + (PCF8563_Time1[6]&0x0f);
    rtctime.Year+=2000;
	return rtctime;
}
void PCF8563_Init(RTCTime rtctime)
{
	PCF8563_WriteAdress(0x02, DECToBCD(rtctime.Second));
	PCF8563_WriteAdress(0x03, DECToBCD(rtctime.Minute));
	PCF8563_WriteAdress(0x04, DECToBCD(rtctime.Hour));
	PCF8563_WriteAdress(0x05, DECToBCD(rtctime.Day));
	PCF8563_WriteAdress(0x06, DECToBCD(rtctime.Week));
	PCF8563_WriteAdress(0x07, DECToBCD(rtctime.Month & 0x7f));
	PCF8563_WriteAdress(0x08, DECToBCD((unsigned char)(rtctime.Year % 2000)));
}

void RTC_initial(void)
{
	//PCF8563_WriteAdress(0xa,0x00);
  RTCTime TimeSet = {2022, 11, 23, 3, 14, 40, 50};// �����趨RTC�ĳ�ʼʱ��
   
	if((PCF8563_ReaDAdress(0xa) & 0x3f) != 0x6)                          //����Ƿ��һ�������������ʼ��ʱ��
	{
		PCF8563_Init(TimeSet);
		PCF8563_WriteAdress(0x0,0x00);
		PCF8563_WriteAdress(0xa,0x6);                                //8:00������0X0a��Сʱ��ʱ��
		PCF8563_WriteAdress(0x1,0x12);                              //������Ч
		PCF8563_WriteAdress(0xd,0xf0);	              //0X0d��CLKOUT�����������Ƶ������Ϊ32.768 kHZ
	}
}

/*
******************************************************************
*
*  �޸�RTCʱ�䲽�裺
*
*  1���޸�Var.h�еĺ궨��
*  2���޸� RTC_initial_chgtime() ������  
*	 "	RTCTime TimeSet = {2022, 11, 23, 3, 14, 40, 50};  "
*    ��ʱ�伴�ɡ�
*
******************************************************************
*/
void RTC_initial_chgtime(u16 Year,u8 Month,u8 Day,u8 Week,u8 Hour,u8 Minute,u8 Second)
{
	
/*********************** �޸ĵ�ǰʱ�� ********************/
	
  RTCTime TimeSet = {Year, Month, Day, Week, Hour, Minute, Second};	   //˳����������ʱ���루24Сʱ��ʱ����
  
/********************************************************/
  
	if((PCF8563_ReaDAdress(0xa) & 0x3f) == 0x6)                 //����Ƿ��һ�������������ʼ��ʱ��
	{
		PCF8563_Init(TimeSet);
		PCF8563_WriteAdress(0x0,0x00);
		PCF8563_WriteAdress(0xa,0x6);                                
		PCF8563_WriteAdress(0x1,0x12);                              
		PCF8563_WriteAdress(0xd,0xf0);	              			
	}
}
