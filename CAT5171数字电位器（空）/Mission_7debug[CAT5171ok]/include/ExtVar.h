#ifndef EXTVAR_FILE
#define EXTVAR_FILE 

#include "define.h"

/****************** ADC���ݲ��� ******************/
extern volatile  unsigned char    b_sample_status;               //״̬������  0�����У�1��״̬��⣻2�����ݲɼ�
extern volatile  signed short     *s_ADC_sample_ptr; 

extern volatile  signed short     s_sample_temp1,s_sample_temp2,s_sample_temp3,s_sample_temp4;   //��������ɼ�����ʱ����
extern volatile  signed short     s_li_power_array[110];
extern volatile  signed short     s_muxian_v_array[110];
extern volatile  float            f_LI_V;                        //����﮵�ص�ѹ
extern volatile  float            f_muxian_V;                    //ĸ�ߵ�ѹ
extern volatile  unsigned char    bt_status_sample_over_flg;     //������ɱ�־
extern volatile  unsigned short   i_status_sample_ord;           //����������

extern volatile  unsigned char    bt_status_test_over_flg;       //һ�β���������ɱ�־
extern volatile  unsigned int     i_status_test_ord;             //���Լ�����
extern volatile  unsigned char    muxian_achieveVol_flg;           //ĸ�ߴﵽĿ���ѹ��־

extern volatile  unsigned char   bt_LYsample_time_over_flg;       //������ɱ�־
extern volatile  unsigned char    bt_sample_over_flg;              //��ʾ���Դ���������

extern volatile  unsigned int     bt_is_convert_flg;               //���������ܿ���
extern volatile  unsigned int     bt_once_sampover_flg;            //ÿ�ɼ�100ms���ݣ����1�����ݴ�����1

extern volatile  unsigned int     i_status_pulse_ord;            //���������
extern volatile  unsigned char    ac_achieveVol_flg;                //�����ѹ�ﵽĿ���ѹ

//extern volatile  unsigned short   bt_afterad_pluse_flg;          //�����Ƿ�ʼ��־
//extern volatile  unsigned int     bt_afterad_pluse_num;          //��ʱ�����õģ����120s����Ҫ��int

extern unsigned char   *b_PC_V_JZ;    //�������ļ���ַ
extern unsigned char   *b_PC_I_JZ;    //�������ļ���ַ

extern volatile  signed short     *s_120KV_V_ptr;      //��ѹ��ַ
extern volatile  signed short     *s_120KV_I_ptr;      //������ַ		
extern volatile  signed short     s_I_OUT_array[320];            //�������
extern volatile  unsigned short   s_H_V_OUT_array[320];          //��ѹ���
extern volatile  signed short     s_mid_power_array[320];        //�м��ѹ
extern volatile  signed short     s_LI_V_array[320];             //﮵�ص�ѹ

/*********************** ���ڲ��� ******************/
extern  unsigned char  b_RX_Array_LY[3000];
extern  unsigned char  b_TX_Array_LY[3000];

extern  volatile  unsigned int bt_Rx_Timer_start_flg_LY,bt_Rx_Over_flg_LY;     //���ճ�ʱ �� ������ɱ�־
extern  volatile  unsigned int i_Rx_irq_num_LY,i_Rx_TN1ms_LY,i_Rx_comm_len_LY; //ָ���ֽ���� �� �Ƿ�ʱ���� �� ����ָ���

extern  volatile  unsigned int i_Tx_irq_cnt_LY,i_Tx_irq_num_LY;
extern  volatile  unsigned int bt_Tx_over_flg_LY;	

/*********************** ָ������ȡ���� ******************/
//20ָ��
extern volatile float Set_Vol;  //���ָ���е����õ�ѹ��
extern volatile  unsigned int i_Sample_all_Num;

//32�����еķְ�����
extern volatile  unsigned short   s_120kv_package_all_num;          //�����ϴ����ܰ���
extern volatile  unsigned short   s_120kv_package_ord;              //�����е���������

/*********************** DA��� ******************/
extern volatile unsigned char bt_start_DA_out_flg;
extern volatile unsigned int  i_DAC_dly_num;
extern volatile float f_DAC_OUT_V;            //���ڿ���DA�����

/*********************** AD������� ******************/

extern volatile float d_muxian_vol;                //�����Ƚ�AD��ֵ�Ƿ�ﵽĿ��ֵ

extern volatile unsigned char bt_ad_sample_over_flg;	//����һ����ɺ���1

extern volatile unsigned char bt_achieve_goal_v_flg;  //��ѹ��Ŀ��ֵ����1
extern volatile unsigned char 	bt_shengya_ok_flg;      //��ѹ��ɴﵽĿ��ֵ

extern volatile unsigned char 	bt_CheckV_Time;      //�����ѹֵ�Ƿ�ﵽ35KV������ﵽ35kv����ֹͣ��ѹ
extern volatile unsigned char 	bt_CheckV_flg;       //�����ѹֵ�Ƿ�ﵽ35KV���ﵽĿ��ֵ����1

extern volatile float 	Vol_RateMost;       		 //������ѹ��λ�����ֵ
extern volatile float 	f_AC_OutputVol_Max;        //������Ÿ���λ�Ľ����������ѹֵ

extern volatile unsigned char 	check_out_v_achieve_flg;    //�ﵽĿ��֮�����λ�����͵ı�־λ

extern volatile unsigned char 	bt_MuXianSYCS_flg;   //ĸ����ѹ��ʱ��־λ
extern volatile unsigned int 	bt_MuXianSYCS_Time;      //ĸ����ѹ��ʱ��־��ʱ

//����У�鴫�������Ƿ�Ϊ�Ե�
extern volatile unsigned char 	bt_Validation_resut;      //У�����

//��⵱ǰ�İ���
extern volatile unsigned int 	now_pack_num;  

/************************ 2023.11.13 ����ˢ�µ�ǰ��ѹ���� ***************************/
//�˲ɼ��׶��ڴ���������ɺ󣬲ɼ��ĵ�ǰ��ѹ
extern volatile signed short    s_show_now_array[120];        // ���浱ǰ��ѹ������
extern volatile unsigned char 	show_now_caiji_finish_flg;    // 0����ʾû�вɼ���ɡ�1��ʾ�ɼ����
extern volatile float           show_now_vol;                 // ������ʾ��ǰ��ѹֵ
extern volatile unsigned char   show_now_vol_state;           // 0��ʾ������ʧ�ܣ�1��ʾ ������ɡ�

#endif 
