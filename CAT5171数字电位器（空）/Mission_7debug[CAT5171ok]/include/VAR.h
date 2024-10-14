#ifndef VAR_FILE
#define VAR_FILE 
  
/****************** ADC数据采样 ******************/
volatile  unsigned char    b_sample_status = 9;               //开始就实时采集
volatile  signed short    *s_ADC_sample_ptr  = (volatile  signed short *)0x64000000; 


volatile  signed short     s_sample_temp1,s_sample_temp2,s_sample_temp3,s_sample_temp4;   //用来保存采集的临时数据
volatile  signed short     s_li_power_array[110];           //保存用到的li电池电压数据
volatile  signed short     s_muxian_v_array[110];           //保存用到的母线电压数据
volatile  float            f_LI_V;                          //单节锂电池电压
volatile  float            f_muxian_V;                      //母线电压
volatile  unsigned char    bt_status_sample_over_flg;       //采样完成标志
volatile  unsigned short   i_status_sample_ord;             //采样计数器
													       
volatile  unsigned char    bt_status_test_over_flg;         //一次采样测试完成标志
volatile  unsigned int     i_status_test_ord;               //测试计数器
volatile  unsigned char    muxian_achieveVol_flg;           //母线达到目标电压标志

volatile  unsigned char    bt_LYsample_time_over_flg;       //母线电压升压到之后采样了一定点之后，可以进行数据传输
volatile  unsigned char    bt_sample_over_flg;              //表示可以传输数据了

volatile  unsigned int    bt_is_convert_flg;                //传输数据总开关
volatile  unsigned int    bt_once_sampover_flg;             //每采集100ms数据，则加1。数据传输后减1

volatile  unsigned int     i_status_pulse_ord;               //脉冲计数器
volatile  unsigned char    ac_achieveVol_flg;                //输出电压达到目标电压,达到目标值为1

//volatile  unsigned short   bt_afterad_pluse_flg = 0;        //脉冲是否开始标志
//volatile  unsigned int     bt_afterad_pluse_num = 0;        //计时脉冲用的，最多120s，需要用int
											
unsigned char   *b_PC_V_JZ          =  (unsigned char *)(0xC0000000 + 0);    //保存用文件基址
unsigned char   *b_PC_I_JZ          =  (unsigned char *)(0xC0800000 + 0);    //保存用文件基址

volatile  signed short    *s_120KV_V_ptr = (signed short  *)(0xC0000000);      //电压地址
volatile  signed short    *s_120KV_I_ptr = (signed short  *)(0xC0800000);      //电流地址													
volatile  signed short    s_I_OUT_array[320];               //输出电流
volatile  unsigned short  s_H_V_OUT_array[320];             //高压输出
volatile  signed short    s_mid_power_array[320];           //中间电压
volatile  signed short    s_LI_V_array[320];                //锂电池电压

/*********************** 串口参数 ******************/
unsigned char  b_RX_Array_LY[3000];                         //蓝牙接收数组
unsigned char  b_TX_Array_LY[3000];                         //蓝牙发送数组

volatile unsigned int bt_Rx_Timer_start_flg_LY = 0,bt_Rx_Over_flg_LY = 0;		//接收超时 、 接收完成标志
volatile unsigned int i_Rx_irq_num_LY,i_Rx_TN1ms_LY,i_Rx_comm_len_LY;	//指令字节序号 、 是否超时计数 、 接收指令长度

volatile unsigned int i_Tx_irq_cnt_LY,i_Tx_irq_num_LY;					//发送回去的指令，暂时没用到
volatile unsigned int bt_Tx_over_flg_LY;								//

/*********************** 指令中用到的变量 ******************/
//20指令
volatile float Set_Vol;  //存放指令中的设置电压数
volatile  unsigned int i_Sample_all_Num;

//32命令中的分包变量
volatile  unsigned short   s_120kv_package_all_num;          //数据上传的总包数
volatile  unsigned short   s_120kv_package_ord = 0xffff;              //变量中的数据数量

/*********************** DA输出参数 ******************/
volatile unsigned char bt_start_DA_out_flg = 0;
volatile unsigned int  i_DAC_dly_num;
volatile float f_DAC_OUT_V = 0.0;            //用于控制DA逐步输出

/*********************** AD输出参数 ******************/
volatile float d_muxian_vol = 0;                //用来比较AD的值是否达到目标值

volatile unsigned char bt_ad_sample_over_flg;	 //采样一次完成后置1 （没用了）

volatile unsigned char bt_achieve_goal_v_flg = 0;   //升压到目标值后置1
volatile unsigned char 	bt_shengya_ok_flg;       //升压完成达到目标值

volatile unsigned char 	bt_CheckV_Time = 0;      //检测升压值是否达到35KV，如果达到35kv，则停止升压。
volatile unsigned char 	bt_CheckV_flg = 0;       //检测升压值是否达到35KV，达到目标值则置1

volatile float 	Vol_RateMost = 0;       		 //各级升压档位的最大值
volatile float 	f_AC_OutputVol_Max = 0.0;        //用来存放各档位的交流输出最大电压值
volatile unsigned char 	check_out_v_achieve_flg;    //达到目标之后给上位机发送的标志位

volatile unsigned char 	bt_MuXianSYCS_flg = 0;   //母线升压超时标志位
volatile unsigned int 	bt_MuXianSYCS_Time = 0;      //母线升压超时标志计时


//用于校验传输数据是否为对的
volatile unsigned char 	bt_Validation_resut = 0;      //校验参数

//用于检测当前采集到的包数
volatile unsigned int 	now_pack_num = 0;

/************************ 2023.11.13 新增刷新当前电压功能 ***************************/
//此采集阶段在传输数据完成后，采集的当前电压
volatile signed short    s_show_now_array[120];        // 保存当前电压的数据
volatile unsigned char 	 show_now_caiji_finish_flg;    // 0：表示没有采集完成、1表示采集完成
volatile float           show_now_vol;                 // 用来显示当前电压值
volatile unsigned char   show_now_vol_state;           // 0表示，计算失败，1表示 计算完成。

/************************ 判断如果出现异常，则先断电 ***************************/
volatile unsigned int    bit_over_vol_sum;
volatile unsigned char   bit_over_vol_flg;
volatile unsigned char   bit_convert_data_state;

#endif 
