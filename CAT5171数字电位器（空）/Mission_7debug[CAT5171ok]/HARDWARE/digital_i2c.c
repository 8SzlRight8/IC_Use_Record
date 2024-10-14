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

	/*��
		�����߼������ǲ���I2CͨѶʱ��Ƶ��
    ����������CPU��Ƶ180MHz ��MDK���뻷����1���Ż�
  
		�����ԣ�ѭ������Ϊ20~250ʱ����ͨѶ����

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
	
//	GPIO_ResetBits(GPIOC, GPIO_Pin_11);   //�����ַΪ0
	GPIO_SetBits(GPIOC, GPIO_Pin_11);   //�����ַΪ1


		i2c_Config();
		
	//��� i2c �ļ�������Ƿ�ͨ
//		i2c_CheckDevice(0x5a);
	
//	#else
//	
//	i2c_CfgGpio();
//	
//	#endif
}
//#define	SDA_IN()	{GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=GPIO_Mode_IN<<14;}//sda����
//#define	SDA_OUT()	{GPIOB->MODER&=0XFFFF3FFF;GPIOB->MODER|=GPIO_Mode_OUT<<14;}//sda���

void SDA_OUT(void)
{
//	GPIO_InitTypeDef	GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//SDA,
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
//	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	GPIOH->MODER  &= ~( 0x03<< (2*10));	   //���
	GPIOH->MODER |= (GPIO_Mode_OUT<<2*10); //�������ģʽ
}
void SDA_IN(void)
{
//	GPIO_InitTypeDef	GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
////	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIOH->MODER  &= ~( 0x03<< (2*10));	   //���
	GPIOH->MODER |= (GPIO_Mode_IN<<2*10); //��������ģʽ
	
}


//-----------------------------
// CAT5171 ��ʼ��־
//-----------------------------
void CAT5171_Start(void)
{
//	SDA_OUT(); //SDA�����
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
// CAT5171 ������־
//-----------------------------
void CAT5171_Stop(void)
{
//	SDA_OUT(); //SDA�����
  SDA_L();
  SCL_H();
  i2c_Delay(); //Tsu:sta
  SDA_H();
	
}

//-----------------------------
// Master �ȴ� CAT5171 ��Ӧ
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
// Master ��Ӧ CAT5171
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
// Master û����Ӧ CAT5171
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
	
  SDA_IN();//SDA��Ϊ����
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

//CAT5117��ȡ����
void CAT5171_Write_Data(u8 byte)
{
	//1��ȷ����ַ
	i2c_Start();
	i2c_SendByte(Com_Write);//�����أ�д����
	i2c_WaitAck();
	
	//2�����͵�λ������
	i2c_SendByte(Com_Wiper);    //
	i2c_WaitAck();         //�ȴ��ظ�
	
	//3����λ���Ĵ�������
	i2c_SendByte(byte);    //���õ������ֵ
	i2c_WaitAck();         //�ȴ��ظ�
	i2c_Stop();            //����
	
}

//CAT5117��ȡ����
u8 CAT5171_Read_Data(void)
{
	u8 ReadByte;
	
//	//1��ȷ����ַ
//	CAT5171_Start();
//	CAT5171_WriteByte(Com_Read);    //�����أ�������
//	CAT5171_WaitAck();               //�ȴ��ظ�
//	
//	//2����������
//	ReadByte = CAT5171_ReadByte();
//	CAT5171_NoAck();    //�޻ظ�
//	CAT5171_Stop();     //����
	
	i2c_Start();
	i2c_SendByte(Com_Read);
	i2c_WaitAck();
	
	ReadByte = i2c_ReadByte();
	i2c_NAck();
	i2c_Stop();
	
	
	return ReadByte;
}

//��⺯��
uint8_t CAT5171_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck = 0;
	
	CAT5171_Start();		/* ���������ź� */

	/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
//	CAT5171_WriteByte(_Address | EEPROM_I2C_WR);
	CAT5171_WriteByte(0x58);
	CAT5171_WaitAck();	/* ����豸��ACKӦ�� */
//	CAT5171_Ack();
//	CAT5171_NoAck();
	
	CAT5171_Stop();			/* ����ֹͣ�ź� */

	return ucAck;
}