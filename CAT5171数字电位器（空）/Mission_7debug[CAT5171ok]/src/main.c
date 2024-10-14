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


//检查母线电压
unsigned char check_muxian_v_fun(void) 
{
	float f_sum = 0;
	unsigned  int i;
	for(i = MX_OFF; i < MX_NUM-MX_OFF; i++)
	{
		f_sum += ((float)s_mid_power_array[i]/3267.8)*(100.0);		//AD采集求平均
	}
	f_sum /= (MX_NUM-2*MX_OFF);
	
	if(f_sum >= 0){
	}
	else{
		f_sum = -f_sum;
	}
	
	if(f_sum >= 0)  		//大于设置固定档位母线电压值则关闭  d_muxian_vol
	{
		muxian_achieveVol_flg = 1;
		return 1;
	}
	else
	{
		return 0;
	}
}

//检查输出电压
unsigned char check_out_v_fun(void)
{
	volatile double f_sum = 0;
	volatile unsigned  int i;
	for(i = (CV_OFF + (CV_NUM * bt_CheckV_Time)); i < (CV_NUM-CV_OFF  + (CV_NUM *bt_CheckV_Time)); i++)
	{
		f_sum += ((double)s_120KV_V_ptr[i]/3267.8)*(10000.0);		//AD采集求平均    //改
	}
	f_sum /= (CV_NUM-2*CV_OFF);
	
	
	if(f_sum >= 0){
	}
	else{
		f_sum = -f_sum; 
	}
	
//	f_sum = 0.7 * f_sum - 5000.0;         //改
	
	bt_CheckV_Time++;		//计算完成，次数加1
	
	//保护措施，如果五次测量都小于5KV，那么将关闭升压。下一包数据升压为0
	if((f_sum < 1900) && (bit_over_vol_sum <= 5) && (bit_over_vol_flg == 0))
	{
		bit_over_vol_sum++;
		bit_over_vol_flg = 0;
	}else if(bit_over_vol_sum > 5)
	{
		bit_over_vol_flg = 1;
		bit_over_vol_sum = 0;
	}
	
	if(f_sum >= f_AC_OutputVol_Max)  				//实际上是大于35KV ，分压比10000:1
	{
		ac_achieveVol_flg = 1;                      //输出达到目标值
		return 1;
	}
	else
	{
		return 0;
	}
}

//11命令进行 锂电池 和 母线电压采集 工作
void LI_V_OUT_Fun(void)
{
	volatile float f_sum = 0,temp_v = 0;
	volatile unsigned  int i;
	
    for(i = 10; i < 100; i++)
    {
		temp_v = ((float)s_li_power_array[i])/3276.8;
		temp_v = (float)((temp_v/6.0) + 5.1)*4 + 0.3;
		f_sum += temp_v;		//AD采集求平均
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
    for(i = 10; i < 100; i++)		//去掉最高最低，求平均
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

    f_muxian_V = f_sum * f_k + f_b;     //母线电压的倍数计算公式

}

/*************
** 函数作用：执行指令11\12的操作（下载测试参数，并开始测试）
** 参   数：无
**
**
*************/
void P_120KV_deal_11_FUN(void)
{
	unsigned short s_sum_temp;   //保存数据和
	unsigned short i;             //计算最后两个字节数据的循环变量
	
//1、读取目录中的一级目录数以及目录名称，存放至发送数据中去
	i_Tx_irq_num_LY = 3 + 1;		//用来计算LEN后面的字节数
	b_TX_Array_LY[0] = 0x68;
	b_TX_Array_LY[1] = 0x86;
	b_TX_Array_LY[2] = i_Tx_irq_num_LY;
	b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
	b_TX_Array_LY[4] = b_RX_Array_LY[4];
	b_TX_Array_LY[5] = 1;
	i_Tx_irq_num_LY += 4;		//计算命令总长度
	
	s_sum_temp = 0;
	for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
	{
			s_sum_temp += b_TX_Array_LY[i];
	}
	b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
	b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
	
	
//2、将存放进数组中的数据发送给上位机
	BEEP_ON
	usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
	BEEP_OFF
	
	TIM_Add_Pulse();
//3、打开蓝牙中断
	d_LY_RX_IRQ_EN;
}


void P_120KV_deal_12_FUN(void)
{
	unsigned short s_sum_temp;   //保存数据和
	unsigned short i;             //计算最后两个字节数据的循环变量
	
//1、读取目录中的一级目录数以及目录名称，存放至发送数据中去
	i_Tx_irq_num_LY = 3 + 1;		//用来计算LEN后面的字节数
	b_TX_Array_LY[0] = 0x68;
	b_TX_Array_LY[1] = 0x86;
	b_TX_Array_LY[2] = i_Tx_irq_num_LY;
	b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
	b_TX_Array_LY[4] = b_RX_Array_LY[4];
	b_TX_Array_LY[5] = 1;
	i_Tx_irq_num_LY += 4;		//计算命令总长度
	
	s_sum_temp = 0;
	for(i = 0; i < i_Tx_irq_num_LY - 2; i++)
	{
			s_sum_temp += b_TX_Array_LY[i];
	}
	b_TX_Array_LY[i_Tx_irq_num_LY - 2] = s_sum_temp;
	b_TX_Array_LY[i_Tx_irq_num_LY - 1] = s_sum_temp >> 8;
	
	
//2、将存放进数组中的数据发送给上位机
	BEEP_ON
	usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
	BEEP_OFF
	
	TIM_Reduce_Pulse();
//3、打开蓝牙中断
	d_LY_RX_IRQ_EN;
}
/*************
** 函数作用：执行指令20的操作（下载测试参数，并开始测试）
** 参   数：无
**
**
*************/
void P_120KV_deal_20_FUN(void)
{
	unsigned char i;	      //函数中的循环变量
	unsigned char setvol[4] = {0};  //用来保存float型数组的单字节数据
	unsigned short s_sum_temp;      //用来记录字符总数一共是多少，16位
	unsigned char com20_return_code = 1;
	unsigned int i_package_temp;
	
	
//准备工作，清空数组采集中的数据
	memset(s_mid_power_array,0,320);      //目前看来不是很有必要
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
//1、判断接收数据的电压等级 以及 脉冲时长
	//电压等级
	for( i = 0 ; i < 4 ; i++ )
	{
		setvol[i] = b_RX_Array_LY[5+i];
	}
	Set_Vol = *(float*)setvol;		//或者直接用memcpy直接复制其中的数据
	
	
	//脉冲时长
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
	i_package_temp /= PACK_ALL;                                 //计算一共有几包
	i_Sample_all_Num = i_Sample_all_Num;				//多采集1.5s，目前这1.5s的数据并没有用上
	s_120kv_package_all_num = (unsigned short)i_package_temp;//将总包数赋值给变量
	
	memset(s_120KV_V_ptr,0,(i_Sample_all_Num * 2));
	memset(s_120KV_I_ptr,0,(i_Sample_all_Num * 2));
	
//4、配置以及发送串口发送内容
	i_Tx_irq_num_LY = 1 + 3;
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x20;
    b_TX_Array_LY[5] = com20_return_code;
    i_Tx_irq_num_LY += 4;  //总发送字节数
    
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
	
//5、清空接收缓冲区
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//6、打开蓝牙中断
	d_LY_RX_IRQ_EN   		//打开蓝牙接收中断
	
}

/*************
** 函数作用：执行指令22的操作（停止测试）
** 参   数：无
**
**
*************/
void P_120KV_deal_22_FUN(void)
{
	unsigned int i;            
	unsigned short s_sum_temp;  
	
//1、关闭测试开关
	
//2、配置以及发送串口发送内容
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	i_Tx_irq_num_LY = 1 + 3;
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x22;
    b_TX_Array_LY[5] = 1;
    i_Tx_irq_num_LY += 4;  //总发送字节数	
    
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
	
//3、停止脉冲操作
	bt_start_DA_out_flg = 0;    //关闭DA
	i_DAC_dly_num = 0;          //清除DA计数
	f_DAC_OUT_V = 0.0;
	DAC_OUT_Vol(f_DAC_OUT_V);         //关闭DAC调压
	
	//计数器以及计数器表示位清零
//	b_sample_status = 9;         //为了保持实时采集（2023.11.13）
	
	b_sample_status = 3;           //状态变为实时采集的2023.11.13
		
	i_status_test_ord = 0;			//母线升压阶段
	bt_status_test_over_flg = 0;
	
	i_status_pulse_ord = 0;         //电容升压阶段
	
	d_DCDC_DIS              //关闭DCDC开关
	d_DCAC_DIS              //关闭DCAC开关

//	d_PULSE_OUT_DIS         //关闭脉冲
	
//4、清空接收缓冲区
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//5、打开蓝牙中断
	d_LY_RX_IRQ_EN   		//打开蓝牙接收中断 
}

//
void P_120KV_deal_30_FUN(void)
{
	unsigned char i;	      //函数中的循环变量
	unsigned short s_sum_temp;      //用来记录字符总数一共是多少，16位
	unsigned char com30_return_code = 1;
	unsigned int i_package_temp;

//每次开始就复位AD7656
		AD_RESET_L
		DelaymS(5);
		AD_RESET_H
		DelaymS(5);
		AD_RESET_L
	 
//准备工作，清空数组采集中的数据
	memset(s_mid_power_array,0,320);      //目前看来不是很有必要
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	i_status_test_ord = 0;				//初始值赋值为0
	bt_status_test_over_flg = 0;
	muxian_achieveVol_flg = 0;          //默认达到母线电压标志为0
	i_status_pulse_ord = 0;				//初始值赋值为0
	
	bt_shengya_ok_flg = 0;              //母线升压完成标志
	
	bt_LYsample_time_over_flg = 0;      //测量数据测量3s后完成标志
	bt_sample_over_flg = 0;             //数据开始传输标志
	
	bit_over_vol_sum = 0;               //升压过程中连续五次，计数清零
	
	check_out_v_achieve_flg = 0;    //置初始值0.（未升到目标电压。）
	
	memset(s_120KV_V_ptr,0,(i_Sample_all_Num * 2));
	memset(s_120KV_I_ptr,0,(i_Sample_all_Num * 2));
	
	
//2、根据指令中的电压等级给出合适的DA值
	//如果设置成功，则开始测量
	if(com30_return_code == 1)
	{
		d_DCDC_EN                    //打开DCDC的总开关
	
		b_sample_status = 1;         //AD可以开始采样
		bt_start_DA_out_flg = 1;	 //可以进入DA调值了
		bt_achieve_goal_v_flg = 0;   //达到目标值之后置1
	}
	
//3、设置AD采样的母线电压标准
	
	if((Set_Vol > 0) && (Set_Vol < 30))						//(Set_Vol > 15) && 
	{
		d_muxian_vol = 75 - 0.5;	         				//1.24 => 75
	
		f_DAC_OUT_V = 1.24 - 0.08;					//这个时间长短只影响母线电压升压时间，即DCDC
		DAC_OUT_Vol(f_DAC_OUT_V);

		Vol_RateMost = 1.36;						//对应最大母线电压只能到90
		f_AC_OutputVol_Max = Set_Vol*1000.0;
	}
	else if(Set_Vol >= 30)
	{
		d_muxian_vol = 125 - 1.5;                       	//1.54 => 125
  
		f_DAC_OUT_V = 1.54 - 0.08;
		DAC_OUT_Vol(f_DAC_OUT_V);

//		Vol_RateMost = 1.66;						//对应最大母线电压只能到140—>1.65
		Vol_RateMost = 1.9;						//对应最大母线电压只能到140—>1.65
		f_AC_OutputVol_Max = Set_Vol*1000.0;
	}		
	
//4、配置以及发送串口发送内容
	
	i_Tx_irq_num_LY = 1 + 3;
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x30;
    b_TX_Array_LY[5] = com30_return_code;
    i_Tx_irq_num_LY += 4;  //总发送字节数
    
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
	
//5、清空接收缓冲区
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//6、打开蓝牙中断
	d_LY_RX_IRQ_EN   		//打开蓝牙接收中断
	
}

/*************
** 函数作用：执行指令31的操作（查询是否测试完毕）
** 参   数：无
**
** 注意：是否测试完毕
*************/
void P_120KV_deal_31_FUN(void)
{
	unsigned int i;            
	unsigned short s_sum_temp;
	
//1、配置以及发送串口发送内容
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	i_Tx_irq_num_LY = 1 + 3;
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x31;
    b_TX_Array_LY[5] = bt_sample_over_flg;
    i_Tx_irq_num_LY += 4;  //总发送字节数
    
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

//2、清空接收缓冲区
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//3、打开蓝牙中断
	d_LY_RX_IRQ_EN   		//打开蓝牙接收中断 
}


//用于32命令中的数据处理
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

//去除飞点
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

//放大或缩小倍数
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
** 函数作用：执行指令32的操作（读取测量数据命令）
** 参   数：无
**
**
*************/
void P_120KV_deal_32_FUN(void)
{
	unsigned int i;
	unsigned short s_sum_temp;
	unsigned char next_pack_ok = 0;
//回复蓝牙信息68 86 04 00 32 01 25 01
//1、配置以及发送串口发送内容
	memcpy((unsigned char *)&s_120kv_package_ord,(unsigned char *)&b_RX_Array_LY[5],2);
	memcpy(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	//判断本包数据是否采集完成
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
		i_Tx_irq_num_LY += 4;  //总发送字节数
		
		//如果达到之后，传输标志为2，说明出错了
		if(bit_convert_data_state == 1)
		{
			bit_convert_data_state = 0;
			b_TX_Array_LY[5] = 2;
		}
	}
	else
	{
	
		if(s_120kv_package_ord >= s_120kv_package_all_num) //包序号错误
		{
			i_Tx_irq_num_LY = 0 + 3+1;
			b_TX_Array_LY[0] = 0x68;
			b_TX_Array_LY[1] = 0x86;
			b_TX_Array_LY[2] = i_Tx_irq_num_LY;
			b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
			b_TX_Array_LY[4] = 0x32;
			b_TX_Array_LY[5] = 0;
			i_Tx_irq_num_LY += 4;  //总发送字节数
			
			//如果达到之后，传输标志为2，说明出错了
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
//			change_multiple_Fun((volatile  signed short  *)&s_120KV_I_ptr[s_120kv_package_ord * PACK_BT]);           //电流6倍的情况
			
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
	    
	    i_Tx_irq_num_LY += 4;  //总发送字节数
			now_pack_num--;
			
			//如果达到之后，传输标志为2，说明出错了
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
    
    //***校验数据***// 
			BEEP_ON
			usart_Send_Data(b_TX_Array_LY,i_Tx_irq_num_LY);
			BEEP_OFF

	
//2、清空接收缓冲区
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));

//3、打开蓝牙中断
	d_LY_RX_IRQ_EN   		//打开蓝牙接收中断 
}

/********************************
//
// 函数功能：读取当前电压数据
//
*********************************/
void P_120KV_deal_33_FUN(void)
{
	unsigned int i;            
	unsigned short s_sum_temp;
	float now_vol = show_now_vol;    // show_now_vol
	
//1、配置以及发送串口发送内容
	memset(b_TX_Array_LY,0,sizeof(b_TX_Array_LY));
	
	i_Tx_irq_num_LY = (1 + 2 + 1 +4); //命令码 + 总长度 + float 型数据长度
	
    b_TX_Array_LY[0] = 0x68;
    b_TX_Array_LY[1] = 0x86;
    b_TX_Array_LY[2] = i_Tx_irq_num_LY;
    b_TX_Array_LY[3] = i_Tx_irq_num_LY >> 8;
    b_TX_Array_LY[4] = 0x33;
		b_TX_Array_LY[5] = show_now_vol_state;
		memcpy((unsigned char *)&b_TX_Array_LY[6],(unsigned char *)&now_vol,4);

    i_Tx_irq_num_LY += 4;  //总发送字节数
    
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

//2、清空接收缓冲区
	memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));	
	
//3、打开蓝牙中断
	d_LY_RX_IRQ_EN   		//打开蓝牙接收中断 
}

/*******************
** 
** 函数作用：用来执行串口命令前刷新的操作
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
			f_sum += ((double)s_show_now_array[i]/3267.8)*(10000.0);		//AD采集求平均    //改
		}
		f_sum /= 80;
		
		
		if(f_sum <= 0){
		}
		else{
			f_sum = -f_sum; 
		}
		
		show_now_vol = (float)(f_sum/1000.0);
		show_now_vol_state = 1;            //标志为置1，表示完成
		memset(s_show_now_array,0,sizeof(s_show_now_array));	//数组清零
	}
	
	//如果五次都等于1,则
	if(bit_over_vol_flg == 1)
	{
		bit_over_vol_flg = 0;
		bit_convert_data_state = 1;
		//关闭所有的开关，并且初始化AD
		//1、关闭所有开关
		d_DCDC_DIS                 //关闭DADC/DCDC
		 f_DAC_OUT_V = 0.0;
		 DAC_OUT_Vol(f_DAC_OUT_V);         //关闭DAC调压
		 
		 ac_achieveVol_flg = 0;
		 
		 d_DCAC_DIS
		 i_status_pulse_ord = 0;          //计数值清零
		 b_sample_status = 3;            //结束当前传输数据的采集，进入刷新当前电压的采集
		
		d_PULSE_OUT_DIS
						 
		
		//2、复位DA7656
		AD_RESET_L
		DelaymS(5);
		AD_RESET_H
		DelaymS(5);
		AD_RESET_L
		
	}
	
	
}

void Close_Show_Now_Vol_flg(void)
{
	b_sample_status = 3;      //恢复到初始状态
	memset(s_show_now_array,0,sizeof(s_show_now_array));	//数组清零
	show_now_caiji_finish_flg = 0;              //采集完成标志为清零
	show_now_vol = 0.0;             //电压清零
	show_now_vol_state = 0;         //电压计算状态清零
}

/*******************
** 函数作用：用来记录函数进度
** 
** 
** 
********************/
void Task_Progress(void)
{
    //AD采样完成，检验是否达到目标电压
	if(muxian_achieveVol_flg == 0)
	{
		if(bt_status_test_over_flg == 1)
		{
			bt_status_test_over_flg = 0;
			i_status_test_ord = 0;
		
			if(check_muxian_v_fun() == 1)       //升压成功
			{
				bt_shengya_ok_flg = 1;          //升压完成标志
			}
			else
			{
				b_sample_status = 1;            //继续检测
				bt_shengya_ok_flg = 0;
			}
		}
	}
	
	//升压完成后
	if(bt_shengya_ok_flg == 1)
	{
		bt_shengya_ok_flg = 0;
		
		//关闭上一采集标记
		bt_start_DA_out_flg = 0;        //升DA标志置0
		bt_achieve_goal_v_flg = 1;
		i_DAC_dly_num = 0;              //DA调值计数器置0
		
		i_status_test_ord = 0;			    //采样的两值
//		bt_status_test_over_flg = 0;
		
		check_out_v_achieve_flg = 0;    //置初始值0.（未升到目标电压。）
		
		memset(s_mid_power_array,0,sizeof(s_mid_power_array));
		
		//开启下一采集标记
		i_status_pulse_ord = 0;
		bt_CheckV_flg = 0;              //采样输出电压是否达到目标值
		bt_CheckV_Time = 0;
		
		ac_achieveVol_flg = 0;          //默认是没有达到目标电压
		
		//可以进行蓝牙标志传输
		bt_LYsample_time_over_flg = 0;
		bt_sample_over_flg = 0;
		
		//可以发送数据包数清零
		now_pack_num = 0;
		
		//打开标记		
		b_sample_status = 2;            //进入到数据采样中，并且进行脉冲计时
		d_DCAC_EN                       //打开DCAC的总开关
		d_PULSE_OUT_EN                  //开启脉冲
	}
	
	//如果在采集输出电压升到目标值后，进行采集判断，如果大于35KV，就将其关闭
	if(b_sample_status == 2)
	{
		if(ac_achieveVol_flg == 0)	//没有达到目标值则判断
		{
			if(bt_CheckV_flg == 1)
			{
				bt_CheckV_flg = 0;
				//计算电压,如果大于??kv之后，就将电压关闭
				if(check_out_v_fun() == 1)
				{
#if(DEBUG_MODLE == 1)   //debug version
{

}
#elif(DEBUG_MODLE == 0)   //release version
{
					//关闭条件(ac_achieveVol_flg 不用清零，测试时间完后清零)
					d_DCDC_DCAC_DIS                 //关闭DADC/DCDC
					d_PULSE_OUT_DIS                 //关闭脉冲
					
					f_DAC_OUT_V = 0.0;
					DAC_OUT_Vol(f_DAC_OUT_V);       //关闭DAC调压
					
//					i_status_pulse_ord = 0;         //采样序号清零，导致又重新开始计数（达到目标之后数据为0的bug）
					bt_CheckV_flg = 0;              
					bt_CheckV_Time = 0;  
	
					//2024.02.03新增一个变量进行计算
					check_out_v_achieve_flg = 1;
}	
#endif
				}
			}	
		}
	}
	
	//如果可以则进行数据传输
	if(bt_LYsample_time_over_flg == 1)
	{
		bt_LYsample_time_over_flg = 0;
		bt_sample_over_flg = 1;        //表示可以进行数据传输
	}
}

void main(void)
{
	static u8 temp_val;
	
	/*初始化USART 配置模式为 115200 8-N-1，中断接收*/
	GPIO_Configuration();
  Debug_USART_Config();
	TIMx_Configuration();
	TIM_Advance_Configuration();
	DAC1_Init();
	SDRAM_Init();                 
	WS_YM_FUN();
	CAT5171_Init();
	
	//开始复位AD7656
	AD_RESET_L
	DelaymS(5);
	AD_RESET_H
	DelaymS(5);
	AD_RESET_L
	
//	BEEP_ON
//	DelaymS(500);
//	BEEP_OFF
	
	b_sample_status = 3;      //程序开机就实时采集
	
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
		
		/************联机通讯****************/
		if((bt_Rx_Over_flg_LY  == 1)) 
		{
      bt_Rx_Over_flg_LY = 0;			

			switch(b_RX_Array_LY[4])//命令码
			{
				case 0x11:       //获取设定电压
					     P_120KV_deal_11_FUN();
							
					     break;
				case 0x12:       //获取设定电压
					     P_120KV_deal_12_FUN();
					     break;
				case 0x20:       //获取设定电压
					     P_120KV_deal_20_FUN();
					     break;
				case 0x22:       //停止升压
							 bt_is_convert_flg = 0;
//							 Close_Show_Now_Vol_flg();
					     P_120KV_deal_22_FUN();
					     break;
				case 0x30:       //开始测试命令
							Close_Show_Now_Vol_flg(); 
							P_120KV_deal_30_FUN();
					     break;
				case 0x31:       //查询是否测试完毕
					     P_120KV_deal_31_FUN();
					     break;	
				case 0x32:       //读取测量数据
							 P_120KV_deal_32_FUN();
					     break;
				case 0x33:       //读当前电压数据
							 P_120KV_deal_33_FUN();
					     break;
				default:
						//清空接收缓冲区
						memset(b_RX_Array_LY,0,sizeof(b_RX_Array_LY));
						
						d_LY_RX_IRQ_EN
				        break;
			  }
		}
		
		Task_Progress();
	}	
}

