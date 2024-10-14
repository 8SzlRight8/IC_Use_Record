/**
  ******************************************************************************
  * @file    bsp_general_tim.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ͨ�ö�ʱ����ʱ����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F429 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
  * @brief  ͨ�ö�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; 	
		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	  // ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	// ����TIMx_CLK,x[6,7] 
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  /* �ۼ� TIM_Period�������һ�����»����ж�*/		
  //����ʱ����0������TIM_Period����ΪTIM_Period�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Period = 10000-1;       
	
	// ͨ�ÿ��ƶ�ʱ��ʱ��ԴTIMxCLK = HCLK/2=90MHz 
	// �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 9-1;	
  // ����ʱ�ӷ�Ƶ
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // ������ʽ
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// ��ʼ����ʱ��TIMx, x[1,8]
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	
	// �����ʱ�������жϱ�־λ
	TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	
	// ������ʱ�������ж�
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
	
	// ʹ�ܶ�ʱ��
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	
  
  TIM_Mode_Config();
}

void  GENERAL_TIM_IRQHandler (void)
{
	/*********** ���ڳ�ʱ���� ****************/
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
	/*********** DAC������� ****************/
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
		
		//���ĸ����ѹ��ʱ����ʱ3s����ر���ѹ,��ֹû����ѹ�ɹ���AD�������⡣
//		if(bt_MuXianSYCS_flg == 1)
//		{
//			bt_MuXianSYCS_Time++;
//			if(bt_MuXianSYCS_Time >= 500)
//			{
//				bt_MuXianSYCS_Time = 0;
//				bt_MuXianSYCS_flg = 0;
//				
//				d_PULSE_OUT_DIS                 //�ر�����
//			    d_DCDC_DCAC_DIS                 //�ر�DADC/DCDC
//			    bt_start_DA_out_flg = 0;        //�رյ�ѹ
//			    i_DAC_dly_num = 0;              //���DA����
//			    f_DAC_OUT_V = 0.0;
//			    DAC_OUT_Vol(f_DAC_OUT_V);       //�ر�DAC��ѹ
//			}
//		}
//		
		
//	/************** ����ʱ������ ***************/
//		if(bt_afterad_pluse_flg == 1)
//		{
//			bt_afterad_pluse_num++; 			//����ʱ����1��֪��Ŀ��ʱ��
//			if(bt_afterad_pluse_num > i_Sample_all_Num)
//			{
//				bt_afterad_pluse_flg = 0;       //����Ŀ��ʱ���Ͳ���ʱ��
//				bt_afterad_pluse_num = 0;       //����ֵ����
//				d_PULSE_OUT_DIS                 //�ر�����
//				
//				d_DCDC_DCAC_DIS                 //�ر�DADC/DCDC
//				bt_start_DA_out_flg = 0;        //�رյ�ѹ
//				i_DAC_dly_num = 0;          //���DA����
//				f_DAC_OUT_V = 0.0;
//				DAC_OUT_Vol(f_DAC_OUT_V);         //�ر�DAC��ѹ
//			}
//		}
		
	/************ ADC���� ***************/
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
        		   if(i_status_test_ord >= MX_NUM)		//ÿ�β�100����  !!!
        		   {
        		       i_status_test_ord = 0;
        		       bt_status_test_over_flg = 1;
//        		       b_sample_status = 2;                //�����ж�ĸ�ߵ�ѹ֮���ٸ�ֵΪ2
        		   }
        		break;
			case 2:      //����Ŀ��ֵ֮��Ϳ�ʼ����
				   s_120KV_I_ptr[i_status_pulse_ord] = *s_ADC_sample_ptr;;             
				   s_120KV_V_ptr[i_status_pulse_ord] = *s_ADC_sample_ptr;; 		//��0��ʼ����
				   
				   i_status_pulse_ord++; 			//����ʱ����1��֪��Ŀ��ʱ��
			
					//ÿCV_NUM��������ݵ�ѹһ��
				   if((i_status_pulse_ord % CV_NUM) == 0) //��
				   {
				      bt_CheckV_flg = 1;
							now_pack_num++;
				   }
				   
					 //���Կ�ʼ��������
					 if(i_status_pulse_ord > 1)
					 {
						 bt_LYsample_time_over_flg = 1;       //���Կ�ʼ�������ݴ���  //��
					 }
					
				   if(i_status_pulse_ord == (i_Sample_all_Num))	//�ɼ����֮��������������������ر�
				   {
					   d_DCDC_DIS                 //�ر�DADC/DCDC
				     f_DAC_OUT_V = 0.0;
				     DAC_OUT_Vol(f_DAC_OUT_V);         //�ر�DAC��ѹ
					   
					   ac_achieveVol_flg = 0;
					   
					   d_DCAC_DIS
					   i_status_pulse_ord = 0;          //����ֵ����
					   b_sample_status = 3;            //������ǰ�������ݵĲɼ�������ˢ�µ�ǰ��ѹ�Ĳɼ�
						 
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
				   s_show_now_array[i_status_pulse_ord] = *s_ADC_sample_ptr;; 		//��0��ʼ����
           i_status_pulse_ord++;
        	 if(i_status_pulse_ord > 100)		//ÿ�β�100���㣬��һ��flag�����м���
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
