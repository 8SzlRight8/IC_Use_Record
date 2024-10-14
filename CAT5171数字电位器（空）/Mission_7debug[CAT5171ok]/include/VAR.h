#ifndef VAR_FILE
#define VAR_FILE 
  
/****************** ADC���ݲ��� ******************/
volatile  unsigned char    b_sample_status = 9;               //��ʼ��ʵʱ�ɼ�
volatile  signed short    *s_ADC_sample_ptr  = (volatile  signed short *)0x64000000; 


volatile  signed short     s_sample_temp1,s_sample_temp2,s_sample_temp3,s_sample_temp4;   //��������ɼ�����ʱ����
volatile  signed short     s_li_power_array[110];           //�����õ���li��ص�ѹ����
volatile  signed short     s_muxian_v_array[110];           //�����õ���ĸ�ߵ�ѹ����
volatile  float            f_LI_V;                          //����﮵�ص�ѹ
volatile  float            f_muxian_V;                      //ĸ�ߵ�ѹ
volatile  unsigned char    bt_status_sample_over_flg;       //������ɱ�־
volatile  unsigned short   i_status_sample_ord;             //����������
													       
volatile  unsigned char    bt_status_test_over_flg;         //һ�β���������ɱ�־
volatile  unsigned int     i_status_test_ord;               //���Լ�����
volatile  unsigned char    muxian_achieveVol_flg;           //ĸ�ߴﵽĿ���ѹ��־

volatile  unsigned char    bt_LYsample_time_over_flg;       //ĸ�ߵ�ѹ��ѹ��֮�������һ����֮�󣬿��Խ������ݴ���
volatile  unsigned char    bt_sample_over_flg;              //��ʾ���Դ���������

volatile  unsigned int    bt_is_convert_flg;                //���������ܿ���
volatile  unsigned int    bt_once_sampover_flg;             //ÿ�ɼ�100ms���ݣ����1�����ݴ�����1

volatile  unsigned int     i_status_pulse_ord;               //���������
volatile  unsigned char    ac_achieveVol_flg;                //�����ѹ�ﵽĿ���ѹ,�ﵽĿ��ֵΪ1

//volatile  unsigned short   bt_afterad_pluse_flg = 0;        //�����Ƿ�ʼ��־
//volatile  unsigned int     bt_afterad_pluse_num = 0;        //��ʱ�����õģ����120s����Ҫ��int
											
unsigned char   *b_PC_V_JZ          =  (unsigned char *)(0xC0000000 + 0);    //�������ļ���ַ
unsigned char   *b_PC_I_JZ          =  (unsigned char *)(0xC0800000 + 0);    //�������ļ���ַ

volatile  signed short    *s_120KV_V_ptr = (signed short  *)(0xC0000000);      //��ѹ��ַ
volatile  signed short    *s_120KV_I_ptr = (signed short  *)(0xC0800000);      //������ַ													
volatile  signed short    s_I_OUT_array[320];               //�������
volatile  unsigned short  s_H_V_OUT_array[320];             //��ѹ���
volatile  signed short    s_mid_power_array[320];           //�м��ѹ
volatile  signed short    s_LI_V_array[320];                //﮵�ص�ѹ

/*********************** ���ڲ��� ******************/
unsigned char  b_RX_Array_LY[3000];                         //������������
unsigned char  b_TX_Array_LY[3000];                         //������������

volatile unsigned int bt_Rx_Timer_start_flg_LY = 0,bt_Rx_Over_flg_LY = 0;		//���ճ�ʱ �� ������ɱ�־
volatile unsigned int i_Rx_irq_num_LY,i_Rx_TN1ms_LY,i_Rx_comm_len_LY;	//ָ���ֽ���� �� �Ƿ�ʱ���� �� ����ָ���

volatile unsigned int i_Tx_irq_cnt_LY,i_Tx_irq_num_LY;					//���ͻ�ȥ��ָ���ʱû�õ�
volatile unsigned int bt_Tx_over_flg_LY;								//

/*********************** ָ�����õ��ı��� ******************/
//20ָ��
volatile float Set_Vol;  //���ָ���е����õ�ѹ��
volatile  unsigned int i_Sample_all_Num;

//32�����еķְ�����
volatile  unsigned short   s_120kv_package_all_num;          //�����ϴ����ܰ���
volatile  unsigned short   s_120kv_package_ord = 0xffff;              //�����е���������

/*********************** DA������� ******************/
volatile unsigned char bt_start_DA_out_flg = 0;
volatile unsigned int  i_DAC_dly_num;
volatile float f_DAC_OUT_V = 0.0;            //���ڿ���DA�����

/*********************** AD������� ******************/
volatile float d_muxian_vol = 0;                //�����Ƚ�AD��ֵ�Ƿ�ﵽĿ��ֵ

volatile unsigned char bt_ad_sample_over_flg;	 //����һ����ɺ���1 ��û���ˣ�

volatile unsigned char bt_achieve_goal_v_flg = 0;   //��ѹ��Ŀ��ֵ����1
volatile unsigned char 	bt_shengya_ok_flg;       //��ѹ��ɴﵽĿ��ֵ

volatile unsigned char 	bt_CheckV_Time = 0;      //�����ѹֵ�Ƿ�ﵽ35KV������ﵽ35kv����ֹͣ��ѹ��
volatile unsigned char 	bt_CheckV_flg = 0;       //�����ѹֵ�Ƿ�ﵽ35KV���ﵽĿ��ֵ����1

volatile float 	Vol_RateMost = 0;       		 //������ѹ��λ�����ֵ
volatile float 	f_AC_OutputVol_Max = 0.0;        //������Ÿ���λ�Ľ����������ѹֵ
volatile unsigned char 	check_out_v_achieve_flg;    //�ﵽĿ��֮�����λ�����͵ı�־λ

volatile unsigned char 	bt_MuXianSYCS_flg = 0;   //ĸ����ѹ��ʱ��־λ
volatile unsigned int 	bt_MuXianSYCS_Time = 0;      //ĸ����ѹ��ʱ��־��ʱ


//����У�鴫�������Ƿ�Ϊ�Ե�
volatile unsigned char 	bt_Validation_resut = 0;      //У�����

//���ڼ�⵱ǰ�ɼ����İ���
volatile unsigned int 	now_pack_num = 0;

/************************ 2023.11.13 ����ˢ�µ�ǰ��ѹ���� ***************************/
//�˲ɼ��׶��ڴ���������ɺ󣬲ɼ��ĵ�ǰ��ѹ
volatile signed short    s_show_now_array[120];        // ���浱ǰ��ѹ������
volatile unsigned char 	 show_now_caiji_finish_flg;    // 0����ʾû�вɼ���ɡ�1��ʾ�ɼ����
volatile float           show_now_vol;                 // ������ʾ��ǰ��ѹֵ
volatile unsigned char   show_now_vol_state;           // 0��ʾ������ʧ�ܣ�1��ʾ ������ɡ�

/************************ �ж���������쳣�����ȶϵ� ***************************/
volatile unsigned int    bit_over_vol_sum;
volatile unsigned char   bit_over_vol_flg;
volatile unsigned char   bit_convert_data_state;

#endif 
