#include "..\include\init_target.h"
/*******************************
**函数名:ChipHalInit()
**功能:片内硬件初始化
*******************************/

void ChipHalInit(void)
{
//    SysTick_init(180);
	//NVIC_Configuration();
	GPIO_Configuration();	     	//初始化GPIO
	BEEP_OFF
	SDRAM_Init();
	WS_YM_FUN();
//    U6_LY_Configuration();
    Dac1_Init();
//    TIM2_Configuration(); //采样定时器
}
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOB,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOC,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOD,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOE,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOF,ENABLE); 
	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOG,ENABLE); 
	
	/**********核心板*****************/
	
	//蜂鸣器													 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
/****************ADC*****************************************/
	 //ADC转化时钟														 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 //ADC复位												 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	//ADC忙检测
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	/*********开机检测***************/ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/**********60kv*****************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	      
	  /************采样定时器********************/             
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	  /************串口********************/      
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
*函数名: TIM_Configuration()
*功  能：定时器2中断函数初始化函数       
*输  入：无
*输  出：
*说  明：1、可以根据参数设置，20KHZ和10KHZ定时中断
*/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 9000 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1; 
		       
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM2, ENABLE);
}
//读取指定地址的半字(16位数据) 
//faddr:读地址 
//返回值:对应数据.
u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr; 
}  
//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}
//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写. 
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.) 
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)	
{ 
  FLASH_Status status = FLASH_COMPLETE;
	u32 addrx=0;
	u32 endaddr=0;	
  if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址
	FLASH_Unlock();									//解锁 
  FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
 		
	addrx=WriteAddr;				//写入的起始地址
	endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址
	if(addrx<0X1FFF0000)			//只有主存储区,才需要执行擦除操作!!
	{
		while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
		{
			if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
			{   
				status=FLASH_EraseSector(STMFLASH_GetFlashSector(addrx),VoltageRange_3);//VCC=2.7~3.6V之间!!
				if(status!=FLASH_COMPLETE)break;	//发生错误了
			}else addrx+=4;
		} 
	}
	if(status==FLASH_COMPLETE)
	{
		while(WriteAddr<endaddr)//写数据
		{
			if(FLASH_ProgramWord(WriteAddr,*pBuffer)!=FLASH_COMPLETE)//写入数据
			{ 
				break;	//写入异常
			}
			WriteAddr+=4;
			pBuffer++;
		} 
	}
  FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
} 

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(4位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)   	
{
	u32 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
		ReadAddr+=4;//偏移4个字节.	
	}
}

void Dac1_Init(void)
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
  DAC_Init(DAC_Channel_1,&DAC_InitType);	 //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC通道1
  
 	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
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
void tesr_dac_fun(void)
{
	float f_set_V_temp;
	
	unsigned int i;
	
	f_set_V_temp = 0.2;
	while(1)
	{
		DAC_OUT_Vol(f_set_V_temp);
		f_set_V_temp += 0.2;
		if(f_set_V_temp > 3.3)
		{
			f_set_V_temp = 0;
		}
		DelaymS(100);
	}
}
///*********************LY使用串口6，pC6,PC7*************************/
//void U6_LY_Configuration(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    USART_InitTypeDef USART_InitStructure;
//	USART_ClockInitTypeDef USART_ClockInitStructure;

//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART1时钟
//	
//  //串口1引脚复用映射
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOA9复用为USART1
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOA10复用为USART1
//	
//	//USART1    
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //GPIOA2与GPIOA3
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
//	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10
//	
//	USART_InitStructure.USART_BaudRate = 115200;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
//  USART_ClockInit(USART6, &USART_ClockInitStructure);
//  USART_Init(USART6, &USART_InitStructure);
//  USART_Cmd(USART6, ENABLE);
//    
//	//串口6使用接收中断
//	USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
//}
//void USART6_IRQHandler(void)
//{
//	//接收中断
//	if(USART_GetITStatus(USART6,USART_IT_RXNE)==SET)
//	{
//		USART_ClearITPendingBit(USART6,USART_IT_RXNE);
//		UART6_Rx_deal_Fun_LY(USART_ReceiveData(USART6)); //接收读卡器送上来的信息
//	}
//	//溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
//	if(USART_GetFlagStatus(USART6,USART_FLAG_ORE)==SET)
//	{
//		USART_ClearFlag(USART6,USART_FLAG_ORE);	//读SR
//		USART_ReceiveData(USART6);				//读DR
//	}
//}
//void UART6_Rx_deal_Fun_LY(unsigned char data)
//{  
//	switch (i_Rx_irq_num_LY)
//	{
//		case 0:
//			if(data == 0x68)
//			{
//				b_RX_Array_LY[i_Rx_irq_num_LY] = data;
//				i_Rx_irq_num_LY++;	
//				i_Rx_TN1ms_LY = 0;
//    			bt_Rx_Timer_start_flg_LY = 1;
//    			
//    			
//			}
//			else
//			{
//    			i_Rx_TN1ms_LY = 0;
//    			i_Rx_irq_num_LY = 0; 
//    			bt_Rx_Timer_start_flg_LY = 0;
//    			
//			}
//		break;
//		case 1:
//			if(data == 0x86)
//			{
//				b_RX_Array_LY[i_Rx_irq_num_LY] = data; 
//				i_Rx_irq_num_LY++;
//					
//			}
//			else
//			{
//				i_Rx_TN1ms_LY = 0;
//    			i_Rx_irq_num_LY = 0; 
//    			bt_Rx_Timer_start_flg_LY = 0;
//    				
//			}
//		break;
//		case 2:
//			b_RX_Array_LY[i_Rx_irq_num_LY] = data; 
//			i_Rx_irq_num_LY++;
//		break;
//		case 3:
//			b_RX_Array_LY[i_Rx_irq_num_LY] = data; 
//			i_Rx_irq_num_LY++;
//			i_Rx_comm_len_LY = b_RX_Array_LY[3];
//			i_Rx_comm_len_LY <<= 8;
//			i_Rx_comm_len_LY += b_RX_Array_LY[2];
//		break;
//		
//		default:
//			b_RX_Array_LY[i_Rx_irq_num_LY] = data;
//			i_Rx_irq_num_LY++;
//			if (i_Rx_irq_num_LY  == i_Rx_comm_len_LY + 4)//接收完全部数据
//			{
//				i_Rx_TN1ms_LY = 0;
//    			i_Rx_irq_num_LY = 0; 
//    			bt_Rx_Timer_start_flg_LY = 0;
//				bt_Rx_Over_flg_LY = 1;
//				d_LY_RX_IRQ_DIS
//			}	
//		break;
//	}			            
//}
//void TIM2_IRQHandler(void)
//{ 
//    if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
//    {
//    	AD_SAMPLE_L
//    	/************接收超时定时器****************/
//		if(bt_Rx_Timer_start_flg_LY == 1)
//		{
//		    i_Rx_TN1ms_LY++;
//		    
//		    if(i_Rx_TN1ms_LY > 5000)//500ms
//		    {
//		        i_Rx_TN1ms_LY = 0;
//		        bt_Rx_Timer_start_flg_LY = 0;
//		        i_Rx_irq_num_LY = 0;
//		    }
//		}
//		/***********DAC输出允许函数****************/
//		if(bt_start_DA_out_flg == 1)
//		{
//		    i_DAC_dly_num++;
//		    if(i_DAC_dly_num >= 1000) //0.2S
//		    {
//		    	i_DAC_dly_num = 0;
//		    	DAC_OUT_Vol(f_DAC_OUT_V);
//		    	f_DAC_OUT_V += 0.2;
//		    	if(f_DAC_OUT_V > 2.0)   //2V对应100V电压输出
//		    	{
//		    	   bt_shengya_ok_flg = 1;
//		    	   bt_start_DA_out_flg = 0;
//		    	}  
//		    }
//		}
//		/************数据采集***************/
//        switch(b_sample_status)
//        {
//        	case 1:
//        		       s_I_OUT_array[s_status_sample_ord] = *s_ADC_sample_ptr;;             //输出电流
//                   s_H_V_OUT_array[s_status_sample_ord] = *s_ADC_sample_ptr;;          //高压输出
//                   s_mid_power_array[s_status_sample_ord] = *s_ADC_sample_ptr;;        //中间电压
//                   s_LI_V_array[s_status_sample_ord] = *s_ADC_sample_ptr;;             //锂电池电压
//                   
//                   s_status_sample_ord++;
//        		   if(s_status_sample_ord >= 400)
//        		   {
//        		       s_status_sample_ord = 0;
//        		       bt_status_sample_over_flg = 1;
//        		       b_sample_status = 0;
//        		   }
//        		   break;
//        	case 2:
//        		   s_I_OUT_array[s_V_check_sample_ord] = *s_ADC_sample_ptr;;             //输出电流
//                   s_H_V_OUT_array[s_V_check_sample_ord] = *s_ADC_sample_ptr;;          //高压输出
//                   s_mid_power_array[s_V_check_sample_ord] = *s_ADC_sample_ptr;;        //中间电压
//                   s_LI_V_array[s_V_check_sample_ord] = *s_ADC_sample_ptr;;             //锂电池电压
//                   
//                   s_V_check_sample_ord++;
//        		   if(s_V_check_sample_ord >= 400)
//        		   {
//        		       s_V_check_sample_ord = 0;
//        		       bt_V_check_sample_over_flg = 1;
//        		       b_sample_status = 0;
//        		   }
//        		   break;
//        	case 3:
//						    s_60KV_V_ptr[s_test_sample_ord] = *s_ADC_sample_ptr;;          //高压输出
//        		   s_60KV_I_ptr[s_test_sample_ord] = *s_ADC_sample_ptr;;             //输出电流
//               
//        		   s_test_sample_ord++;
//        		   i_Sample_Ord_Num++;
//		    	   if(i_Sample_Ord_Num >= i_Sample_all_Num)
//		    	   {
//		    	       i_Sample_Ord_Num = 0;
//		    	       bt_test_sample_over_flg = 1;
//		    	       b_sample_status = 0;
//		    	       d_DCDC_DCAC_DIS
//		    	       d_PULSE_DIS
//		    	   }
//        		   break;	   
//        	
//        }
//		AD_SAMPLE_H
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    }
//} 