#include "./dac/bsp_dac.h"

volatile uint16_t Vol_D = 0;

void DAC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE );	  //ʹ��PORTAͨ��ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;	//GPIO_Speed_50MHz
	
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // �˿�����
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//����DACͨ��
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//��ʹ�ô������� TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1�������ر� BOFF1=1
	
	//��ʼ��ͨ��
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //��ʼ��DACͨ��1
//	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //��ʼ��DACͨ��2
	
	//ʹ��
	DAC_Cmd(DAC_Channel_1, ENABLE);  //ʹ��DAC1
//	DAC_Cmd(DAC_Channel_2, ENABLE);  //ʹ��DAC2
  
	//���ó�ʼ��
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
//	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ
}

void DAC_OUT_Vol(float f_temp)
{
	short s_temp;
	float f_1;
	if(f_temp > 3.2)
	{
		f_temp = 3.2;
	}
	else if(f_temp < 0.1)
	{
		f_temp = 0.1;
	}
	f_1 = f_temp * 4095 / 3.3;
	s_temp = (short)f_1;
	
	DAC_SetChannel1Data(DAC_Align_12b_R, s_temp);     //ˢ��DAֵ,�����Ҷ��� ȡֵ��Χ 0~ 4095 ,�����ϽӵĲο���ѹʱ3.3v ����4095ʱΪ3.3v     
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);   //�������,DAֵ����  
}

