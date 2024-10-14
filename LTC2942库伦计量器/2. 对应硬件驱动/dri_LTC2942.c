#include "dri_LTC2942.h"

I2C_Dev_t * LTC2942;

////-------------------------------------------------------

//�ԼĴ���B����д0xFC������
//������ѹ:3v(11)
//Ԥ��Ƶ:128(111)
//������ģʽ(10)��
//�ص�ģ�⣻(0)
//��Ĵ�����д��һ���ֽ�
void LTC2942_writeOneByte(u8 reg, u8 data)
{
	//1����ʼ
	LTC2942->i2c_start();
	
	//2�������豸д��ַ���ȴ���Ӧ
	LTC2942->i2c_writeByte(ADDR_DeviceWrite);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//3��д��01�Ĵ������ȴ���Ӧ
	LTC2942->i2c_writeByte(reg);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//4��д�����ݵȴ���Ӧ
	LTC2942->i2c_writeByte(data);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//5�����Ͷ�ȡ����ʱ��
	LTC2942->i2c_stop();
}

////�ԼĴ���CD EF GF ����д������
////CDĬ��:7FFF; �����ɺ�:FFFF
////EF:�������������:4200mv
////GH:��ص͵�������:3600mv
void LTC2942_writeTwoByte(u8 reg, u8 hbyte, u8 lbyte)
{
	//1�����Ϳ�ʼ�ź�
	LTC2942->i2c_start();
	
	//2�������豸д��ַ�����ȴ���Ӧ
	LTC2942->i2c_writeByte(ADDR_DeviceWrite);//11001000
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}
	
	//3�����ͼĴ�����ַ�����ȴ���Ӧ
	LTC2942->i2c_writeByte(reg);//o2h / 04h / 06h
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}
	
	//4�����͵�һ���ֽڵ����ݣ����ȴ���Ӧ
	LTC2942->i2c_writeByte(hbyte);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//5���ȴ��ڶ����ֽڵ����ݣ����ȴ���Ӧ
	LTC2942->i2c_writeByte(lbyte);
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return;
	}

	//6������ֹͣ�ź�
	LTC2942->i2c_stop();
}

////A[7]:оƬʶ��; 1 : LTC2942;     0 : LTC2942;
////A[5]:ָʾ�ۻ���ɵ�ֵ�ﵽ������ײ���
////A[3]:ָʾ�ۻ����ֵ�����˵����ֵ���ޡ�
////A[2]:ָʾ�ۻ����ֵ�½���������ֵ�������¡�
////A[1]:VBAT����ָʾ��ص�ѹ��SENSE-���½�����ѡ����VBAT��ֵ��
////A[0]:
u8 LTC2942_readOneByte(u8 reg)
{
	u8 receByte = 0;
	
	//1�����Ϳ�ʼ�ź�
	LTC2942->i2c_start();
	
	//2�������豸д��ַ�����ȴ���Ӧ
	LTC2942->i2c_writeByte(ADDR_DeviceWrite); //1100 1000
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}
	
	//3�����ͼĴ����ĵ�ַ�����ȴ���Ӧ
	LTC2942->i2c_writeByte(reg);//00h
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}

	//4�����·��Ϳ�ʼָ����ڶ�ȡ����
	LTC2942->i2c_start();
	
	//5�����Ͷ����ݵ�ַ�����ȴ���Ӧ
	LTC2942->i2c_writeByte(ADDR_DeviceRead);//11001001
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}

	//6����ȡ���ݣ���֪ͨ�ӻ�����ȡ��ϡ�
	receByte = LTC2942->i2c_readByte();
	LTC2942->i2c_nack();
	
	//7������ֹͣ�ź�
	LTC2942->i2c_stop();

	return receByte;	
}


u16 LTC2942_readTwoByte(u8 reg)
{
	u16 receTwoByte = 0;
	u16 hbyte = 0;
	u8 lbyte = 0;
	
	//1�����Ϳ�ʼ�ź�
	LTC2942->i2c_start();
	
	//2�������豸д��ַ�����ȴ���Ӧ
	LTC2942->i2c_writeByte(ADDR_DeviceWrite); //1100 1000
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}
	
	//3�����ͼĴ�����ַ�����ȴ���Ӧ
	LTC2942->i2c_writeByte(reg);//o2h / 04h / 06h
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}

	//4�����·��Ϳ�ʼָ����ڶ�����
	LTC2942->i2c_start();
	
	//5�������豸�Ķ���ַ�����ȴ���Ӧ
	LTC2942->i2c_writeByte(ADDR_DeviceRead);//11001001
	if(LTC2942->i2c_waitAck() != 0)
	{
		LTC2942->i2c_stop();
		return 0;
	}
	
	//6����ȡ���ݸ��ֽڣ����ظ��ӻ�
	hbyte = LTC2942->i2c_readByte();
	LTC2942->i2c_ack();
	
	//7����ȡ���ݵ��ֽڣ����ظ��ӻ�
	lbyte= LTC2942->i2c_readByte();
	LTC2942->i2c_nack();
	
	//8��ֹͣ����ͨ��
	LTC2942->i2c_stop();

	receTwoByte = (hbyte<<8) | lbyte;
	return receTwoByte;
}

//��ʼ������
void LTC2942_Init(void)
{
	unsigned char revByte = 0;
	
	//����I2C����������
	LTC2942 = i2c_dev_create(); 
	LTC2942->i2c_checkDevice(ADDR_Device); //��ʼ���˿� + ����Ƿ�����ͨѶ
	
	//�Ĵ���B д��Ĭ��ֵ��MΪ128.
	LTC2942_writeOneByte(ADDR_B,0x3C); //д��
	LTC2942_readOneByte(ADDR_B);       //�����鿴�Ƿ�����ȷ��
}


/*****************************
* 
* ���ܺ���
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
	//�õ�������󣬽��е�����ʾ
}


//��ذٷֱ�:	��ǰ����ֵ�����ܶ�ȵ���ֵ��
u8 getbatteryPercent(void)
{
	u16 currentBAT = 0;//��ǰ����ֵ
	u16 rateBAT = 0;//�����ֵ
	static u8 bat_per = 0;  //�ٷֱȵ���

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
		//�͵�������
	}

	if(batteryHigh)
	{
		//�������״̬
	}

	if(batteryVeryLow)
	{
		//����ָʾ��ص�ѹ��SENSE-���½�����ѡ����VBAT��ֵ��
	}
}


