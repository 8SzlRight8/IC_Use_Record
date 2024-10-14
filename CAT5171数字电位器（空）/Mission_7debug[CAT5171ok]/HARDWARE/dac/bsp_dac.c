#include "./dac/bsp_dac.h"

volatile uint16_t Vol_D = 0;

void DAC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE );	  //使能PORTA通道时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // 端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //模拟输入
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;	//GPIO_Speed_50MHz
	
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // 端口配置
//	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//配置DAC通道
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
	
	//初始化通道
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1
//	DAC_Init(DAC_Channel_2,&DAC_InitType);	 //初始化DAC通道2
	
	//使能
	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC1
//	DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC2
  
	//设置初始化
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
//	DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
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
	
	DAC_SetChannel1Data(DAC_Align_12b_R, s_temp);     //刷新DA值,数据右对齐 取值范围 0~ 4095 ,板子上接的参考电压时3.3v 所以4095时为3.3v     
    DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);   //软件触发,DA值更新  
}

