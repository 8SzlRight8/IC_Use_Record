/**
  ******************************************************************************
  * @file    bsp_general_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   通用定时器定时范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "bsp_general_tim.h"
#include "./usart/bsp_rs232_usart.h"
#include "bsp_dac.h"
#include "./led/bsp_led.h" 
#include "ExtVar.h" 

unsigned int i_timer_num = 0;
unsigned int bt_Timer_start_flg = 0;
 /**
  * @brief  通用定时器 TIMx,x[1,8]中断优先级配置
  * @param  无
  * @retval 无
  */
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// 开启TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到TIM_Period，即为TIM_Period次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Period = 10000-1;       
	
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=90MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 9-1;	
  // 采样时钟分频
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // 计数方式
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// 初始化定时器TIMx, x[1,8]
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	
	// 清除定时器更新中断标志位
	TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	
	// 开启定时器更新中断
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
	
	// 使能定时器
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

/**
  * @brief  初始化高级控制定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	
  
  TIM_Mode_Config();
}

void  GENERAL_TIM_IRQHandler (void)
{
	/*********** 串口超时处理 ****************/
	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
		if(bt_Rx_Timer_start_flg_LY == 1)
		{
		    i_Rx_TN1ms_LY++;
		    
		    if(i_Rx_TN1ms_LY > 500)//500ms
		    {
		        i_Rx_TN1ms_LY = 0;
		        bt_Rx_Timer_start_flg_LY = 0;
		        i_Rx_irq_num_LY = 0;
		    }
		}
		
		
//		LED4_TOGGLE;
		
		
		AD_SAMPLE_L
	/*********** DAC输出控制 ****************/
		if(bt_start_DA_out_flg == 1)
		{
//			f_DAC_OUT_V = 2;
//			DAC_OUT_Vol(f_DAC_OUT_V);
			if(bt_achieve_goal_v_flg == 0)
			{
				i_DAC_dly_num++;
				if((i_DAC_dly_num >= 4) ) //0.35S
				{
					i_DAC_dly_num = 0;
					DAC_OUT_Vol(f_DAC_OUT_V);
					f_DAC_OUT_V += 0.0001;		       
					if(f_DAC_OUT_V > Vol_RateMost)   
					{
#if(DEBUG_MODLE == 1)   //debug version(without Cap)
{
            //set DA value
						f_DAC_OUT_V = Vol_RateMost;  //store the value of DA
						bt_start_DA_out_flg = 0;     //close the DA adjustment master switch
						i_DAC_dly_num = 0;           //clear the DA count value
  
           	//set state value of AD sample
						b_sample_status = 0;
						
	          //set "achieve muxian vol" flag to "1".
//						bt_shengya_ok_flg = 1;       // it means muxian vol reach the max of this level
}
#elif(DEBUG_MODLE == 0)   //release version(with Cap)
{
	          //set DA value
						f_DAC_OUT_V = Vol_RateMost;     //store the value of DA             
						bt_start_DA_out_flg = 0;        //close the DA adjustment master switch
						i_DAC_dly_num = 0;              //clear the DA count value
	
						f_DAC_OUT_V = 0.0;              //clear DA Output value
						DAC_OUT_Vol(f_DAC_OUT_V);    

	          //close pulse output
            d_PULSE_OUT_DIS	                //prepare for stage 2 (ensure this is close!!)
	
            //set state value of AD sample	
						b_sample_status = 0;
						
	          //set "achieve muxian vol" flag to "1".
//						bt_shengya_ok_flg = 1;          // it means muxian vol reach the max of this level
}	
#endif
					}  
				}
			}	
			else if(bt_achieve_goal_v_flg == 1)
			{
				i_DAC_dly_num = 0;                 //clear the DA count value
			}
		}
		else
		{
			i_DAC_dly_num = 0;                   //clear the DA count value
		}
		
		//如果母线升压超时，超时3s，则关闭升压,防止没有升压成功，AD出现问题。
//		if(bt_MuXianSYCS_flg == 1)
//		{
//			bt_MuXianSYCS_Time++;
//			if(bt_MuXianSYCS_Time >= 500)
//			{
//				bt_MuXianSYCS_Time = 0;
//				bt_MuXianSYCS_flg = 0;
//				
//				d_PULSE_OUT_DIS                 //关闭脉冲
//			    d_DCDC_DCAC_DIS                 //关闭DADC/DCDC
//			    bt_start_DA_out_flg = 0;        //关闭调压
//			    i_DAC_dly_num = 0;              //清除DA计数
//			    f_DAC_OUT_V = 0.0;
//			    DAC_OUT_Vol(f_DAC_OUT_V);       //关闭DAC调压
//			}
//		}
//		
		
//	/************** 脉冲时长控制 ***************/
//		if(bt_afterad_pluse_flg == 1)
//		{
//			bt_afterad_pluse_num++; 			//脉冲时长加1，知道目标时长
//			if(bt_afterad_pluse_num > i_Sample_all_Num)
//			{
//				bt_afterad_pluse_flg = 0;       //大于目标时长就不计时了
//				bt_afterad_pluse_num = 0;       //计数值清零
//				d_PULSE_OUT_DIS                 //关闭脉冲
//				
//				d_DCDC_DCAC_DIS                 //关闭DADC/DCDC
//				bt_start_DA_out_flg = 0;        //关闭调压
//				i_DAC_dly_num = 0;          //清除DA计数
//				f_DAC_OUT_V = 0.0;
//				DAC_OUT_Vol(f_DAC_OUT_V);         //关闭DAC调压
//			}
//		}
		
	/************ ADC采样 ***************/
        switch(b_sample_status)
        {
			case 0:
				    s_sample_temp1 = *s_ADC_sample_ptr;
			        s_sample_temp2 = *s_ADC_sample_ptr;
        		    s_muxian_v_array[i_status_sample_ord] = *s_ADC_sample_ptr;
					s_li_power_array[i_status_sample_ord] = *s_ADC_sample_ptr;
					
					i_status_sample_ord++;
					if(i_status_sample_ord >= 100)
					{
						i_status_sample_ord = 0;
						bt_status_sample_over_flg = 1;
						b_sample_status = 3;
					}
        		   break;
      case 1:
				   s_sample_temp1 = *s_ADC_sample_ptr;;             
				   s_sample_temp2 = *s_ADC_sample_ptr;;  
				   s_mid_power_array[i_status_test_ord] = *s_ADC_sample_ptr;;        
				   s_LI_V_array[i_status_test_ord] = *s_ADC_sample_ptr;;
                   i_status_test_ord++;
        		   if(i_status_test_ord >= MX_NUM)		//每次采100个点  !!!
        		   {
        		       i_status_test_ord = 0;
        		       bt_status_test_over_flg = 1;
//        		       b_sample_status = 2;                //经过判断母线电压之后再赋值为2
        		   }
        		break;
			case 2:      //升到目标值之后就开始采样
				   s_120KV_I_ptr[i_status_pulse_ord] = *s_ADC_sample_ptr;;             
				   s_120KV_V_ptr[i_status_pulse_ord] = *s_ADC_sample_ptr;; 		//从0开始采样
				   
				   i_status_pulse_ord++; 			//脉冲时长加1，知道目标时长
			
					//每CV_NUM个点检测电容电压一次
				   if((i_status_pulse_ord % CV_NUM) == 0) //改
				   {
				      bt_CheckV_flg = 1;
							now_pack_num++;
				   }
				   
					 //可以开始传输数据
					 if(i_status_pulse_ord > 1)
					 {
						 bt_LYsample_time_over_flg = 1;       //可以开始进行数据传输  //改
					 }
					
				   if(i_status_pulse_ord == (i_Sample_all_Num))	//采集完成之后继续采样，其它正常关闭
				   {
					   d_DCDC_DIS                 //关闭DADC/DCDC
				     f_DAC_OUT_V = 0.0;
				     DAC_OUT_Vol(f_DAC_OUT_V);         //关闭DAC调压
					   
					   ac_achieveVol_flg = 0;
					   
					   d_DCAC_DIS
					   i_status_pulse_ord = 0;          //计数值清零
					   b_sample_status = 3;            //结束当前传输数据的采集，进入刷新当前电压的采集
						 
						 //add
#if(DEBUG_MODLE == 1)   //debug version(without Cap)
{
					bt_CheckV_flg = 0;              
					bt_CheckV_Time = 0;  
					d_PULSE_OUT_DIS
}
#elif(DEBUG_MODLE == 0)   //release version(with Cap)
{

}	
#endif
				   }
				break;
			case 3:
				   s_sample_temp1 = *s_ADC_sample_ptr;;             
				   s_show_now_array[i_status_pulse_ord] = *s_ADC_sample_ptr;; 		//从0开始采样
           i_status_pulse_ord++;
        	 if(i_status_pulse_ord > 100)		//每次采100个点，置一个flag，进行计算
        	 {
        	   i_status_pulse_ord = 0;
        	   show_now_caiji_finish_flg = 1;
        	 }
        break;
			default:
				break;

        }
		AD_SAMPLE_H
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_IT_Update);  		 
	}		 	
}

/*********************************************END OF FILE**********************/
