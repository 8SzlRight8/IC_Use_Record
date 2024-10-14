#include "stm32f4xx.h"
#include "VAR.h"
#include "ExtVar.h"
#include "bsp_rs232_usart.h"
#include "bsp_dac.h"
#include "bsp_general_tim.h"
#include "./led/bsp_led.h" 
#include "bsp_advance_tim.h"
#include "digital_i2c.h"

void  DelaymS(unsigned int  dly)
{  
	unsigned int  i;
  
	for(; dly>0; dly--) 
		for(i=0; i<37000; i++);
}


//���ĸ�ߵ�ѹ
unsigned char check_muxian_v_fun(void) 
{
	float f_sum = 0;
	unsigned  int i;
	for(i = MX_OFF; i < MX_NUM-MX_OFF; i++)
	{
		f_sum += ((float)s_mid_power_array[i]/3267.8)*(100.0);		//AD�ɼ���ƽ��
	}
	f_sum /= (MX_NUM-2*MX_OFF);
	
	if(f_sum >= 0){
	}
	else{
		f_sum = -f_sum;
	}
	
	if(f_sum >= 0)  		//�������ù̶���λĸ�ߵ�ѹֵ��ر�  d_muxian_vol
	{
		muxian_achieveVol_flg = 1;
		return 1;
	}
	else
	{
		return 0;
	}
}

//��������ѹ
unsigned char check_out_v_fun(void)
{
	volatile double f_sum = 0;
	volatile unsigned  int i;
	for(i = (CV_OFF + (CV_NUM * bt_CheckV_Time)); i < (CV_NUM-CV_OFF  + (CV_NUM *bt_CheckV_Time)); i++)
	{
		f_sum += ((double)s_120KV_V_ptr[i]/3267.8)*(10000.0);		//AD�ɼ���ƽ��    //��
	}
	f_sum /= (CV_NUM-2*CV_OFF);
	
	
	if(f_sum >= 0){
	}
	else{
		f_sum = -f_sum; 
	}
	
//	f_sum = 0.7 * f_sum - 5000.0;         //��
	
	bt_CheckV_Time++;		//������ɣ�������1
	
	//������ʩ�������β�����С��5KV����ô���ر���ѹ����һ��������ѹΪ0
	if((f_sum < 1900) && (bit_over_vol_sum <= 5) && (bit_over_vol_flg == 0))
	{
		bit_over_vol_sum++;
		bit_over_vol_flg = 0;
	}else if(bit_over_vol_sum > 5)
	{
		bit_over_vol_flg = 1;
		bit_over_vol_sum = 0;
	}
	
	if(f_sum >= f_AC_OutputVol_Max)  				//ʵ�����Ǵ���35KV ����ѹ��10000:1
	{
		ac_achieveVol_flg = 1;                      //����ﵽĿ��ֵ
		return 1;
	}
	else
	{
		return 0;
	}
}

//11������� ﮵�� �� ĸ�ߵ�ѹ�ɼ� ����
void LI_V_OUT_Fun(void)
{
	volatile float f_sum = 0,temp_v = 0;
	volatile unsigned  int i;
	
    for(i = 10; i < 100; i++)
    {
		temp_v = ((float)s_li_power_array[i])/3276.8;
		temp_v = (float)((temp_v/6.0) + 5.1)*4 + 0.3;
		f_sum += temp_v;		//AD�ɼ���ƽ��
    }

    f_LI_V = f_sum / 90;

}
void muxian_V_OUT_Fun(void)
{
	unsigned int i;
    unsigned short max,min;
    float f_sum = 0;
    float f_k,f_b;
    
    f_k = 100.0;
    f_b = 0;
    
    max = s_muxian_v_array[10];
    min = s_muxian_v_array[10];
    for(i = 10; i < 100; i++)		//ȥ�������ͣ���ƽ��
    {
    	f_sum += (float)s_muxian_v_array[i];
    	if(max < s_muxian_v_array[i])
    	{
    		max = s_muxian_v_array[i];
    	}
    	if(min > s_muxian_v_array[i])
    	{
    		min = s_muxian_v_array[i];
    	}
    }
    f_sum -= min;
    f_sum -= max;
    f_sum /= 88;
    f_sum /= 3276.7;

    f_muxian_V = f_sum * f_k + f_b;     //ĸ�ߵ�ѹ�ı������㹫ʽ

}

/*************
** �������ã�ִ��ָ��11\12�Ĳ��������ز��Բ���������ʼ���ԣ�
** ��   ������
**
**
*************/
void P_120KV_deal_11_FUN(void)
{
	unsigned short s_sum_temp;   //�������ݺ�
	unsigned short i;             //������������ֽ����ݵ�ѭ������
	
//1����ȡĿ¼�е�һ��Ŀ¼���Լ�Ŀ¼���ƣ����������������ȥ
	i_Tx_irq_num_LY = 3 + 1;		//��������LEN������ֽ���
	b_TX_Array_LY[0] = 0x68;
	b_TX_Array_LY[1] = 0x86;
	b_TX_Array_LY[2] = i_Tx_irq_num_LY;
	b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
	b_TX_Array_LY[4] = b_RX_Array_LY[4];
	b_TX_Array_LY[5] = 1;
	i_Tx_irq_num_LY += 4;		//���������ܳ���
	
	s_sum_temp = 0;
	for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
	{
			s_sum_temp += b_TX_Array_LY[i];
	}
	b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
	b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
	
	
//2������Ž������е����ݷ��͸���λ��
	BEEP_ON
	usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
	BEEP_OFF
	
	TIM_Add_Pulse();
//3���������ж�
	d_LY_RX_IRQ_EN;
}


void P_120KV_deal_12_FUN(void)
{
	unsigned short s_sum_temp;   //�������ݺ�
	unsigned short i;             //������������ֽ����ݵ�ѭ������
	
//1����ȡĿ¼�е�һ��Ŀ¼���Լ�Ŀ¼���ƣ����������������ȥ
	i_Tx_irq_num_LY = 3 + 1;		//��������LEN������ֽ���
	b_TX_Array_LY[0] = 0x68;
	b_TX_Array_LY[1] = 0x86;
	b_TX_Array_LY[2] = i_Tx_irq_num_LY;
	b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
	b_TX_Array_LY[4] = b_RX_Array_LY[4];
	b_TX_Array_LY[5] = 1;
	i_Tx_irq_num_LY += 4;		//���������ܳ���
	
	s_sum_temp = 0;
	for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
	{
			s_sum_temp += b_TX_Array_LY[i];
	}
	b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
	b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
	
	
//2������Ž������е����ݷ��͸���λ��
	BEEP_ON
	usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
	BEEP_OFF
	
	TIM_Reduce_Pulse();
//3���������ж�
	d_LY_RX_IRQ_EN;
}
/*************
** �������ã�ִ��ָ��20�Ĳ��������ز��Բ���������ʼ���ԣ�
** ��   ������
**
**
*************/
void P_120KV_deal_20_FUN(void)
{
	unsigned char i;	      //�����е�ѭ������
	unsigned char setvol[4] = {0};  //��������float������ĵ��ֽ�����
	unsigned short s_sum_temp;      //������¼�ַ�����һ���Ƕ��٣�16λ
	unsigned char com20_return_code = 1;
	unsigned int i_package_temp;
	
	
//׼���������������ɼ��е�����
	memset(s_mid_power_array,0,320);      //Ŀǰ�������Ǻ��б�Ҫ
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
//1���жϽ������ݵĵ�ѹ�ȼ� �Լ� ����ʱ��
	//��ѹ�ȼ�
	for( i = 0 ; i < 4 ; i++ )
	{
		setvol[i] = b_RX_Array_LY[5+i];
	}
	Set_Vol = *(float*)setvol;		//����ֱ����memcpyֱ�Ӹ������е�����
	
	
	//����ʱ��
	switch(b_RX_Array_LY[9])	//9
	{
		case 0:
			i_Sample_all_Num = 15 * 1000;
			break;
		case 1:
			i_Sample_all_Num = 30 * 1000;
			break;
		case 2:
			i_Sample_all_Num = 60 * 1000;
			break;
		case 3:
			i_Sample_all_Num = 120 * 1000;
			break;
		default:
			com20_return_code = 0;
			break;
	}
	
	i_package_temp = i_Sample_all_Num * 2 * 2;
	i_package_temp /= PACK_ALL;                                 //����һ���м���
	i_Sample_all_Num = i_Sample_all_Num;				//��ɼ�1.5s��Ŀǰ��1.5s�����ݲ�û������
	s_120kv_package_all_num = (unsigned short)i_package_temp;//���ܰ�����ֵ������
	
	memset(s_120KV_V_ptr,0,(i_Sample_all_Num * 2));
	memset(s_120KV_I_ptr,0,(i_Sample_all_Num * 2));
	
//4�������Լ����ʹ��ڷ�������
	i_Tx_irq_num_LY = 1 + 3;
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x20;
    b_TX_Array_LY[5] = com20_return_code;
    i_Tx_irq_num_LY += 4;  //�ܷ����ֽ���
    
    s_sum_temp = 0;
    for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
    {
        s_sum_temp += b_TX_Array_LY[i];
    }
    b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
    b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
	
			BEEP_ON
			usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
			BEEP_OFF
	
//5����ս��ջ�����
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//6���������ж�
	d_LY_RX_IRQ_EN   		//�����������ж�
	
}

/*************
** �������ã�ִ��ָ��22�Ĳ�����ֹͣ���ԣ�
** ��   ������
**
**
*************/
void P_120KV_deal_22_FUN(void)
{
	unsigned int i;            
	unsigned short s_sum_temp;  
	
//1���رղ��Կ���
	
//2�������Լ����ʹ��ڷ�������
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	i_Tx_irq_num_LY = 1 + 3;
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x22;
    b_TX_Array_LY[5] = 1;
    i_Tx_irq_num_LY += 4;  //�ܷ����ֽ���	
    
    s_sum_temp = 0;
    for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
    {
        s_sum_temp += b_TX_Array_LY[i];
    }
    b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
    b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
	
			BEEP_ON
			usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
			BEEP_OFF
	
//3��ֹͣ�������
	bt_start_DA_out_flg = 0;    //�ر�DA
	i_DAC_dly_num = 0;          //���DA����
	f_DAC_OUT_V = 0.0;
	DAC_OUT_Vol(f_DAC_OUT_V);         //�ر�DAC��ѹ
	
	//�������Լ���������ʾλ����
//	b_sample_status = 9;         //Ϊ�˱���ʵʱ�ɼ���2023.11.13��
	
	b_sample_status = 3;           //״̬��Ϊʵʱ�ɼ���2023.11.13
		
	i_status_test_ord = 0;			//ĸ����ѹ�׶�
	bt_status_test_over_flg = 0;
	
	i_status_pulse_ord = 0;         //������ѹ�׶�
	
	d_DCDC_DIS              //�ر�DCDC����
	d_DCAC_DIS              //�ر�DCAC����

//	d_PULSE_OUT_DIS         //�ر�����
	
//4����ս��ջ�����
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//5���������ж�
	d_LY_RX_IRQ_EN   		//�����������ж� 
}

//
void P_120KV_deal_30_FUN(void)
{
	unsigned char i;	      //�����е�ѭ������
	unsigned short s_sum_temp;      //������¼�ַ�����һ���Ƕ��٣�16λ
	unsigned char com30_return_code = 1;
	unsigned int i_package_temp;

//ÿ�ο�ʼ�͸�λAD7656
		AD_RESET_L
		DelaymS(5);
		AD_RESET_H
		DelaymS(5);
		AD_RESET_L
	 
//׼���������������ɼ��е�����
	memset(s_mid_power_array,0,320);      //Ŀǰ�������Ǻ��б�Ҫ
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	i_status_test_ord = 0;				//��ʼֵ��ֵΪ0
	bt_status_test_over_flg = 0;
	muxian_achieveVol_flg = 0;          //Ĭ�ϴﵽĸ�ߵ�ѹ��־Ϊ0
	i_status_pulse_ord = 0;				//��ʼֵ��ֵΪ0
	
	bt_shengya_ok_flg = 0;              //ĸ����ѹ��ɱ�־
	
	bt_LYsample_time_over_flg = 0;      //�������ݲ���3s����ɱ�־
	bt_sample_over_flg = 0;             //���ݿ�ʼ�����־
	
	bit_over_vol_sum = 0;               //��ѹ������������Σ���������
	
	check_out_v_achieve_flg = 0;    //�ó�ʼֵ0.��δ����Ŀ���ѹ����
	
	memset(s_120KV_V_ptr,0,(i_Sample_all_Num * 2));
	memset(s_120KV_I_ptr,0,(i_Sample_all_Num * 2));
	
	
//2������ָ���еĵ�ѹ�ȼ��������ʵ�DAֵ
	//������óɹ�����ʼ����
	if(com30_return_code == 1)
	{
		d_DCDC_EN                    //��DCDC���ܿ���
	
		b_sample_status = 1;         //AD���Կ�ʼ����
		bt_start_DA_out_flg = 1;	 //���Խ���DA��ֵ��
		bt_achieve_goal_v_flg = 0;   //�ﵽĿ��ֵ֮����1
	}
	
//3������AD������ĸ�ߵ�ѹ��׼
	
	if((Set_Vol > 0) && (Set_Vol < 30))						//(Set_Vol > 15) && 
	{
		d_muxian_vol = 75 - 0.5;	         				//1.24 => 75
	
		f_DAC_OUT_V = 1.24 - 0.08;					//���ʱ�䳤��ֻӰ��ĸ�ߵ�ѹ��ѹʱ�䣬��DCDC
		DAC_OUT_Vol(f_DAC_OUT_V);

		Vol_RateMost = 1.36;						//��Ӧ���ĸ�ߵ�ѹֻ�ܵ�90
		f_AC_OutputVol_Max = Set_Vol*1000.0;
	}
	else if(Set_Vol >= 30)
	{
		d_muxian_vol = 125 - 1.5;                       	//1.54 => 125
  
		f_DAC_OUT_V = 1.54 - 0.08;
		DAC_OUT_Vol(f_DAC_OUT_V);

//		Vol_RateMost = 1.66;						//��Ӧ���ĸ�ߵ�ѹֻ�ܵ�140��>1.65
		Vol_RateMost = 1.9;						//��Ӧ���ĸ�ߵ�ѹֻ�ܵ�140��>1.65
		f_AC_OutputVol_Max = Set_Vol*1000.0;
	}		
	
//4�������Լ����ʹ��ڷ�������
	
	i_Tx_irq_num_LY = 1 + 3;
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x30;
    b_TX_Array_LY[5] = com30_return_code;
    i_Tx_irq_num_LY += 4;  //�ܷ����ֽ���
    
    s_sum_temp = 0;
    for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
    {
        s_sum_temp += b_TX_Array_LY[i];
    }
    b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
    b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
	
	
	BEEP_ON
    usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
    BEEP_OFF
	
//5����ս��ջ�����
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//6���������ж�
	d_LY_RX_IRQ_EN   		//�����������ж�
	
}

/*************
** �������ã�ִ��ָ��31�Ĳ�������ѯ�Ƿ������ϣ�
** ��   ������
**
** ע�⣺�Ƿ�������
*************/
void P_120KV_deal_31_FUN(void)
{
	unsigned int i;            
	unsigned short s_sum_temp;
	
//1�������Լ����ʹ��ڷ�������
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	i_Tx_irq_num_LY = 1 + 3;
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x31;
    b_TX_Array_LY[5] = bt_sample_over_flg;
    i_Tx_irq_num_LY += 4;  //�ܷ����ֽ���
    
    s_sum_temp = 0;
    for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
    {
        s_sum_temp += b_TX_Array_LY[i];
    }
    b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
    b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
    
			BEEP_ON
			usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
			BEEP_OFF

//2����ս��ջ�����
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//3���������ж�
	d_LY_RX_IRQ_EN   		//�����������ж� 
}


//����32�����е����ݴ���
void xiancan_zhongweizhipj_lvbo_Fun(volatile  I16  *array)
{
	I16 temp_array[6];
	int i,j,sum,max,min;
	
	array[0] = array[1];
	for(i = 0; i < 500 - 4; i++)
	{
		max = array[i];
		min = array[i];
		for(j = i; j < (i + 4); j++)
		{
			temp_array[j - i] = array[j];
			if( max < array[j])
			{
				max = array[j];
			}
			
			if(min > array[j])
			{
				min = array[j];
			}
		}
		
		sum = 0;
		for(j = 0; j < 4; j++)
		{
			sum += temp_array[j];
		}
		
		sum -= min;
		sum -= max;
		sum >>= 1;
		array[i] = (I16)sum;
	}
}

//ȥ���ɵ�
void xiancan_chufeidian_lvbo_Fun(volatile  I16  *array)
{
	I16 temp_array[6],i_temp;
	int i,j,sum,max,min;
	unsigned char  bt_chufa_flg,b_js_ord;
	
	array[0] = array[1];
	
	bt_chufa_flg = 0;
	b_js_ord  = 0;
	for(i = 20; i < PACK_BT - 4; i++)
	{
		if(bt_chufa_flg == 0)
		{
			if(array[i] < -3276) 
			{
				b_js_ord++;
				if(b_js_ord >= 10)
				{
					bt_chufa_flg = 1;
				}
			}
			else
			{
				b_js_ord = 0;
			}
		}
		else
		{
			if(array[i] > -1638)
			{
				array[i] = array[i - 1];
			}
		}
	}
	array[PACK_BT - 4] = array[PACK_BT - 5];
	array[PACK_BT - 3] = array[PACK_BT - 5];
	array[PACK_BT - 2] = array[PACK_BT - 5];
	array[PACK_BT - 1] = array[PACK_BT - 5];
}

//�Ŵ����С����
void change_multiple_Fun(volatile  I16  *array)
{
	I16 temp_array[6];
	int i;
	for(i = 0;i < PACK_BT; i++)
	{
		array[i] /= 6;
	}
}

/*************
** �������ã�ִ��ָ��32�Ĳ�������ȡ�����������
** ��   ������
**
**
*************/
void P_120KV_deal_32_FUN(void)
{
	unsigned int i;
	unsigned short s_sum_temp;
	unsigned char next_pack_ok = 0;
//�ظ�������Ϣ68 86 04 00 32 01 25 01
//1�������Լ����ʹ��ڷ�������
	memcpy((unsigned char *)&s_120kv_package_ord,(unsigned char *)&b_RX_Array_LY[5],2);
	memcpy(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	//�жϱ��������Ƿ�ɼ����
	if(now_pack_num > 0)
	{
		next_pack_ok = 1;
	}
	
	if(next_pack_ok == 0)   //
	{
		i_Tx_irq_num_LY = 0 + 3 + 1;
		b_TX_Array_LY[0] = 0x68;
		b_TX_Array_LY[1] = 0x86;
		b_TX_Array_LY[2] = i_Tx_irq_num_LY;
		b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
		b_TX_Array_LY[4] = 0x32;
		b_TX_Array_LY[5] = next_pack_ok;
		i_Tx_irq_num_LY += 4;  //�ܷ����ֽ���
		
		//����ﵽ֮�󣬴����־Ϊ2��˵��������
		if(bit_convert_data_state == 1)
		{
			bit_convert_data_state = 0;
			b_TX_Array_LY[5] = 2;
		}
	}
	else
	{
	
		if(s_120kv_package_ord >= s_120kv_package_all_num) //����Ŵ���
		{
			i_Tx_irq_num_LY = 0 + 3+1;
			b_TX_Array_LY[0] = 0x68;
			b_TX_Array_LY[1] = 0x86;
			b_TX_Array_LY[2] = i_Tx_irq_num_LY;
			b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
			b_TX_Array_LY[4] = 0x32;
			b_TX_Array_LY[5] = 0;
			i_Tx_irq_num_LY += 4;  //�ܷ����ֽ���
			
			//����ﵽ֮�󣬴����־Ϊ2��˵��������
			if(bit_convert_data_state == 1)
			{
				bit_convert_data_state = 0;
				b_TX_Array_LY[5] = 2;
			}
		}
		else
		{

			xiancan_chufeidian_lvbo_Fun((volatile  signed short  *)&s_120KV_V_ptr[s_120kv_package_ord * PACK_BT]);
			xiancan_chufeidian_lvbo_Fun((volatile  signed short  *)&s_120KV_I_ptr[s_120kv_package_ord * PACK_BT]);
			xiancan_zhongweizhipj_lvbo_Fun((volatile  signed short  *)&s_120KV_V_ptr[s_120kv_package_ord * PACK_BT]);
			xiancan_zhongweizhipj_lvbo_Fun((volatile  signed short  *)&s_120KV_I_ptr[s_120kv_package_ord * PACK_BT]);
			xiancan_zhongweizhipj_lvbo_Fun((volatile  signed short  *)&s_120KV_V_ptr[s_120kv_package_ord * PACK_BT]);
			xiancan_zhongweizhipj_lvbo_Fun((volatile  signed short  *)&s_120KV_I_ptr[s_120kv_package_ord * PACK_BT]);
//			change_multiple_Fun((volatile  signed short  *)&s_120KV_I_ptr[s_120kv_package_ord * PACK_BT]);           //����6�������
			
			i_Tx_irq_num_LY = PACK_ALL + 8 + 1;
	    b_TX_Array_LY[0] = 0x68;
	    b_TX_Array_LY[1] = 0x86;
	    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
	    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
	    b_TX_Array_LY[4] = 0x32;
			b_TX_Array_LY[5] = next_pack_ok;
	    memcpy((unsigned char *)&b_TX_Array_LY[6],(unsigned char *)&s_120kv_package_ord,2);
	    memcpy((unsigned char *)&b_TX_Array_LY[8],(unsigned char *)&s_120kv_package_all_num,2);
	    memcpy((unsigned char *)&b_TX_Array_LY[10],(unsigned char *)&b_PC_V_JZ[s_120kv_package_ord * PACK_ONE],PACK_ONE);
			memcpy((unsigned char *)&b_TX_Array_LY[11],(unsigned char *)&check_out_v_achieve_flg,1);
	    memcpy((unsigned char *)&b_TX_Array_LY[11 + PACK_ONE],(unsigned char *)&b_PC_I_JZ[s_120kv_package_ord * PACK_ONE],PACK_ONE);
	    
	    i_Tx_irq_num_LY += 4;  //�ܷ����ֽ���
			now_pack_num--;
			
			//����ﵽ֮�󣬴����־Ϊ2��˵��������
			if(bit_convert_data_state == 1)
			{
				bit_convert_data_state = 0;
				b_TX_Array_LY[5] = 2;
			}
		}
	}
    
    s_sum_temp = 0;
    for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
    {
        s_sum_temp += b_TX_Array_LY[i];
    }
    b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
    b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
    
    //***У������***// 
			BEEP_ON
			usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
			BEEP_OFF

	
//2����ս��ջ�����
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));

//3���������ж�
	d_LY_RX_IRQ_EN   		//�����������ж� 
}

/********************************
//
// �������ܣ���ȡ��ǰ��ѹ����
//
*********************************/
void P_120KV_deal_33_FUN(void)
{
	unsigned int i;            
	unsigned short s_sum_temp;
	float now_vol = show_now_vol;    // show_now_vol
	
//1�������Լ����ʹ��ڷ�������
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	i_Tx_irq_num_LY = (1 + 2 + 1 +4); //������ + �ܳ��� + float �����ݳ���
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x33;
		b_TX_Array_LY[5] = show_now_vol_state;
		memcpy((unsigned char *)&b_TX_Array_LY[6],(unsigned char *)&now_vol,4);

    i_Tx_irq_num_LY += 4;  //�ܷ����ֽ���
    
    s_sum_temp = 0;
    for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
    {
        s_sum_temp += b_TX_Array_LY[i];
    }
    b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
    b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
    
		BEEP_ON
		usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
		BEEP_OFF

//2����ս��ջ�����
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//3���������ж�
	d_LY_RX_IRQ_EN   		//�����������ж� 
}

/*******************
** 
** �������ã�����ִ�д�������ǰˢ�µĲ���
** 
** 
********************/
void Show_Now_Vol(void)
{
	volatile double f_sum = 0;
	volatile unsigned  int i;
	
	if(show_now_caiji_finish_flg == 1)
	{
		show_now_caiji_finish_flg = 0;
		
		for(i = 10; i < 90; i++)
		{
			f_sum += ((double)s_show_now_array[i]/3267.8)*(10000.0);		//AD�ɼ���ƽ��    //��
		}
		f_sum /= 80;
		
		
		if(f_sum <= 0){
		}
		else{
			f_sum = -f_sum; 
		}
		
		show_now_vol = (float)(f_sum/1000.0);
		show_now_vol_state = 1;            //��־Ϊ��1����ʾ���
		memset(s_show_now_array,0,sizeof(s_show_now_array));	//��������
	}
	
	//�����ζ�����1,��
	if(bit_over_vol_flg == 1)
	{
		bit_over_vol_flg = 0;
		bit_convert_data_state = 1;
		//�ر����еĿ��أ����ҳ�ʼ��AD
		//1���ر����п���
		d_DCDC_DIS                 //�ر�DADC/DCDC
		 f_DAC_OUT_V = 0.0;
		 DAC_OUT_Vol(f_DAC_OUT_V);         //�ر�DAC��ѹ
		 
		 ac_achieveVol_flg = 0;
		 
		 d_DCAC_DIS
		 i_status_pulse_ord = 0;          //����ֵ����
		 b_sample_status = 3;            //������ǰ�������ݵĲɼ�������ˢ�µ�ǰ��ѹ�Ĳɼ�
		
		d_PULSE_OUT_DIS
						 
		
		//2����λDA7656
		AD_RESET_L
		DelaymS(5);
		AD_RESET_H
		DelaymS(5);
		AD_RESET_L
		
	}
	
	
}

void Close_Show_Now_Vol_flg(void)
{
	b_sample_status = 3;      //�ָ�����ʼ״̬
	memset(s_show_now_array,0,sizeof(s_show_now_array));	//��������
	show_now_caiji_finish_flg = 0;              //�ɼ���ɱ�־Ϊ����
	show_now_vol = 0.0;             //��ѹ����
	show_now_vol_state = 0;         //��ѹ����״̬����
}

/*******************
** �������ã�������¼��������
** 
** 
** 
********************/
void Task_Progress(void)
{
    //AD������ɣ������Ƿ�ﵽĿ���ѹ
	if(muxian_achieveVol_flg == 0)
	{
		if(bt_status_test_over_flg == 1)
		{
			bt_status_test_over_flg = 0;
			i_status_test_ord = 0;
		
			if(check_muxian_v_fun() == 1)       //��ѹ�ɹ�
			{
				bt_shengya_ok_flg = 1;          //��ѹ��ɱ�־
			}
			else
			{
				b_sample_status = 1;            //�������
				bt_shengya_ok_flg = 0;
			}
		}
	}
	
	//��ѹ��ɺ�
	if(bt_shengya_ok_flg == 1)
	{
		bt_shengya_ok_flg = 0;
		
		//�ر���һ�ɼ����
		bt_start_DA_out_flg = 0;        //��DA��־��0
		bt_achieve_goal_v_flg = 1;
		i_DAC_dly_num = 0;              //DA��ֵ��������0
		
		i_status_test_ord = 0;			    //��������ֵ
//		bt_status_test_over_flg = 0;
		
		check_out_v_achieve_flg = 0;    //�ó�ʼֵ0.��δ����Ŀ���ѹ����
		
		memset(s_mid_power_array,0,sizeof(s_mid_power_array));
		
		//������һ�ɼ����
		i_status_pulse_ord = 0;
		bt_CheckV_flg = 0;              //���������ѹ�Ƿ�ﵽĿ��ֵ
		bt_CheckV_Time = 0;
		
		ac_achieveVol_flg = 0;          //Ĭ����û�дﵽĿ���ѹ
		
		//���Խ���������־����
		bt_LYsample_time_over_flg = 0;
		bt_sample_over_flg = 0;
		
		//���Է������ݰ�������
		now_pack_num = 0;
		
		//�򿪱��		
		b_sample_status = 2;            //���뵽���ݲ����У����ҽ��������ʱ
		d_DCAC_EN                       //��DCAC���ܿ���
		d_PULSE_OUT_EN                  //��������
	}
	
	//����ڲɼ������ѹ����Ŀ��ֵ�󣬽��вɼ��жϣ��������35KV���ͽ���ر�
	if(b_sample_status == 2)
	{
		if(ac_achieveVol_flg == 0)	//û�дﵽĿ��ֵ���ж�
		{
			if(bt_CheckV_flg == 1)
			{
				bt_CheckV_flg = 0;
				//�����ѹ,�������??kv֮�󣬾ͽ���ѹ�ر�
				if(check_out_v_fun() == 1)
				{
#if(DEBUG_MODLE == 1)   //debug version
{

}
#elif(DEBUG_MODLE == 0)   //release version
{
					//�ر�����(ac_achieveVol_flg �������㣬����ʱ���������)
					d_DCDC_DCAC_DIS                 //�ر�DADC/DCDC
					d_PULSE_OUT_DIS                 //�ر�����
					
					f_DAC_OUT_V = 0.0;
					DAC_OUT_Vol(f_DAC_OUT_V);       //�ر�DAC��ѹ
					
//					i_status_pulse_ord = 0;         //����������㣬���������¿�ʼ�������ﵽĿ��֮������Ϊ0��bug��
					bt_CheckV_flg = 0;              
					bt_CheckV_Time = 0;  
	
					//2024.02.03����һ���������м���
					check_out_v_achieve_flg = 1;
}	
#endif
				}
			}	
		}
	}
	
	//���������������ݴ���
	if(bt_LYsample_time_over_flg == 1)
	{
		bt_LYsample_time_over_flg = 0;
		bt_sample_over_flg = 1;        //��ʾ���Խ������ݴ���
	}
}

void main(void)
{
	static u8 temp_val;
	
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	GPIO_Configuration();
  Debug_USART_Config();
	TIMx_Configuration();
	TIM_Advance_Configuration();
	DAC1_Init();
	SDRAM_Init();                 
	WS_YM_FUN();
	CAT5171_Init();
	
	//��ʼ��λAD7656
	AD_RESET_L
	DelaymS(5);
	AD_RESET_H
	DelaymS(5);
	AD_RESET_L
	
//	BEEP_ON
//	DelaymS(500);
//	BEEP_OFF
	
	b_sample_status = 3;      //���򿪻���ʵʱ�ɼ�
	
//	i2c_CheckDevice(0x58);
	CAT5171_Write_Data(0x7f);
	
//	while(1)
//	{
////		i2c_CheckDevice(0x5a);
////		CAT5171_CheckDevice(0x58);
//		delay_us(30);
////		CAT5171_Write_Data(0x7f);
//	}
	
  while(1)
	{
		Show_Now_Vol();
		
//		i2c_CheckDevice(0x58);
		
		/************����ͨѶ****************/
		if((bt_Rx_Over_flg_LY  == 1)) 
		{
      bt_Rx_Over_flg_LY = 0;			

			switch(b_RX_Array_LY[4])//������
			{
				case 0x11:       //��ȡ�趨��ѹ
					     P_120KV_deal_11_FUN();
							
					     break;
				case 0x12:       //��ȡ�趨��ѹ
					     P_120KV_deal_12_FUN();
					     break;
				case 0x20:       //��ȡ�趨��ѹ
					     P_120KV_deal_20_FUN();
					     break;
				case 0x22:       //ֹͣ��ѹ
							 bt_is_convert_flg = 0;
//							 Close_Show_Now_Vol_flg();
					     P_120KV_deal_22_FUN();
					     break;
				case 0x30:       //��ʼ��������
							Close_Show_Now_Vol_flg(); 
							P_120KV_deal_30_FUN();
					     break;
				case 0x31:       //��ѯ�Ƿ�������
					     P_120KV_deal_31_FUN();
					     break;	
				case 0x32:       //��ȡ��������
							 P_120KV_deal_32_FUN();
					     break;
				case 0x33:       //����ǰ��ѹ����
							 P_120KV_deal_33_FUN();
					     break;
				default:
						//��ս��ջ�����
						memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));
						
						d_LY_RX_IRQ_EN
				        break;
			  }
		}
		
		Task_Progress();
	}	
}

