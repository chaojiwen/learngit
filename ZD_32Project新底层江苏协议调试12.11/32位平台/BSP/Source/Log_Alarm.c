/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/24																			 */
/*************************************************************************************************/
#include "System_Config.h"

#include "Log_Alarm.h"

#include "Ks0108.h"	 

#include "Mcu_Beep.h"
#include "Mcu_led.h"

#include "Log_Battery.h"
#include "Log_GSM.h"
#include "Log_Power.h"
#include "Log_Sensor.h"
#include "Log_SMS.h"

#include "app.h"

/********************************************************************************************************/
/* ������	: Alarm_State_Up_Reset														 	 			*/
/* ����  	: sensor_t->SENSOR_Alarm_State:���ޱ���״̬��sensor_f->Tmp_Up_Alarm_Cnt:����״̬��ʱʱ�䡣		*/
/* ���		: �� 																						*/
/* ����	 	: �������ޱ���״̬							 												*/
/* ��������	: 2015/04/18																				*/
/********************************************************************************************************/
void Alarm_State_Up_Set(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* �ۼƾ���ʱ�� */
	sensor_f->Sensor_Up_Alarm_Cnt += Alarm_State_Check_Period;
	/* ����ʱ�� �Ƿ��ҹ� ���õ�ֵ */
	if(sensor_f->Sensor_Up_Alarm_Cnt >= sensor_f->Sensor_Alarm_Delay)
	{
		/* �������ʱ�����ֵ */
		sensor_f->Sensor_Up_Alarm_Cnt = 0;
		/* ���� ��ǰͨ������״̬Ϊ:���� */
		sensor_t->SENSOR_Alarm_State |= Alarm_Up;
	}	
}

/********************************************************************************************************/
/* ������	: Alarm_State_Low_Set														 	 			*/
/* ����  	: sensor_t->SENSOR_Alarm_State:���ޱ���״̬��sensor_f->Tmp_Up_Alarm_Cnt:����״̬��ʱʱ�䡣		*/
/* ���		: �� 																						*/
/* ����	 	: �������ޱ���״̬							 												*/
/* ��������	: 2015/04/18																				*/
/********************************************************************************************************/
void Alarm_State_Low_Set(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* �ۼƾ���ʱ�� */
	sensor_f->Sensor_Low_Alarm_Cnt += Alarm_State_Check_Period;
	/* ����ʱ�� �Ƿ��ҹ� ���õ�ֵ */
	if(sensor_f->Sensor_Low_Alarm_Cnt >= sensor_f->Sensor_Alarm_Delay)
	{
		/* �������ʱ�����ֵ */
		sensor_f->Sensor_Low_Alarm_Cnt = 0;
		/* ���� ��ǰͨ������״̬Ϊ:���� */
		sensor_t->SENSOR_Alarm_State |= Alarm_Low;
	}	
}

/********************************************************************************************************/
/* ������	: Alarm_State_Tmp_Up_Reset														 	 		*/
/* ����  	: sensor_t->SENSOR_Alarm_State:���ޱ���״̬��sensor_f->Tmp_Up_Alarm_Cnt:����״̬��ʱʱ�䡣		*/
/* ���		: �� 																						*/
/* ����	 	: ������ޱ���״̬							 												*/
/* ��������	: 2015/04/18																				*/
/********************************************************************************************************/
void Alarm_State_Up_Reset(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	
	/* ״̬��� */
	if(sensor_t->SENSOR_Alarm_State == Alarm_Up)
	{
		/* �ж� ״̬ʱ�� �ۼ� */
		if(sensor_f->Sensor_Up_Alarm_Cnt < Alarm_Reset_Delay)
		{
			/* �ۼ�ʱ�� */
			sensor_f->Sensor_Up_Alarm_Cnt += Alarm_State_Check_Period;	
		}else
		{	/* ����ۼ�ʱ�� */
			sensor_f->Sensor_Up_Alarm_Cnt = 0;
			/* ȡ�� ���� ״̬ */
			sensor_t->SENSOR_Alarm_State &= ~Alarm_Up;
			/* �ж� ����ȡ����־ */
			if(sensor_f->Sensor_Suspend_Alarm_Flag == TRUE)
			{
				/* ��� ����ȡ����־ */
				sensor_f->Sensor_Suspend_Alarm_Flag = FALSE;
			}
		}
	}else if(sensor_f->Sensor_Up_Alarm_Cnt)
	{
		/* ��� �ۼ�ʱ�� */
		sensor_f->Sensor_Up_Alarm_Cnt = 0;
	}
}

/********************************************************************************************************/
/* ������	: Alarm_State_Tmp_Low_Reset														 	 		*/
/* ����  	: sensor_t->SENSOR_Alarm_State:���ޱ���״̬��sensor_f->Tmp_Up_Alarm_Cnt:����״̬��ʱʱ�䡣		*/
/* ���		: �� 																						*/
/* ����	 	: ������ޱ���״̬							 												*/
/* ��������	: 2015/04/18																				*/
/********************************************************************************************************/
void Alarm_State_Low_Reset(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	
	/* ״̬��� */
	if(sensor_t->SENSOR_Alarm_State == Alarm_Low)
	{
		/* �ж� ״̬ʱ�� �ۼ� */
		if(sensor_f->Sensor_Low_Alarm_Cnt < Alarm_Reset_Delay)
		{
			/* �ۼ�ʱ�� */
			sensor_f->Sensor_Low_Alarm_Cnt += Alarm_State_Check_Period;	
		}else
		{	/* ����ۼ�ʱ�� */
			sensor_f->Sensor_Low_Alarm_Cnt = 0;
			/* ȡ�� ���� ״̬ */
			sensor_t->SENSOR_Alarm_State &= ~Alarm_Low;
			/* �ж� ����ȡ����־ */
			if(sensor_f->Sensor_Suspend_Alarm_Flag == TRUE)
			{
				/* ��� ����ȡ����־ */
				sensor_f->Sensor_Suspend_Alarm_Flag = FALSE;
			}
		}
	}else if(sensor_f->Sensor_Low_Alarm_Cnt)
	{
		/* ��� �ۼ�ʱ�� */
		sensor_f->Sensor_Low_Alarm_Cnt = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Alarm_Reminding
*	����˵��: ƽ̨������ͨ�����ⱨ�������ű������ܵļ�⡣
*	��    �Σ�*config_tab:�������ò���ָ�룻fix_tab:�����̶���������ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Alarm_Reminding(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{

#if ((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��1����״̬ */
	u8 sensor_ch1_alarm_sr 	= 0;
	/* ����������ͨ��1����Ϣ����״̬ */
	static BOOL sensor_ch1_sms_send_state 	= FALSE;
	/* ����������ͨ��1�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch1_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��2����״̬ */
	u8 sensor_ch2_alarm_sr 	= 0;
	/* ����������ͨ��2����Ϣ����״̬ */
	static BOOL sensor_ch2_sms_send_state 	= FALSE;
	/* ����������ͨ��2�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch2_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��3����״̬ */
	u8 sensor_ch3_alarm_sr 	= 0;
	/* ����������ͨ��3����Ϣ����״̬ */
	static BOOL sensor_ch3_sms_send_state 	= FALSE;
	/* ����������ͨ��3�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch3_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��4����״̬ */
	u8 sensor_ch4_alarm_sr 	= 0;
	/* ����������ͨ��4����Ϣ����״̬ */
	static BOOL sensor_ch4_sms_send_state 	= FALSE;
	/* ����������ͨ��4�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch4_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��5����״̬ */
	u8 sensor_ch5_alarm_sr 	= 0;
	/* ����������ͨ��5����Ϣ����״̬ */
	static BOOL sensor_ch5_sms_send_state 	= FALSE;
	/* ����������ͨ��5�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch5_sms_again_send_time_reg = 0;
#endif
		
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��6����״̬ */
	u8 sensor_ch6_alarm_sr 	= 0;
	/* ����������ͨ��6����Ϣ����״̬ */
	static BOOL sensor_ch6_sms_send_state 	= FALSE;
	/* ����������ͨ��6�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch6_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_Hum) || \
	 (Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��7����״̬ */
	u8 sensor_ch7_alarm_sr 	= 0;
	/* ����������ͨ��7����Ϣ����״̬ */
	static BOOL sensor_ch7_sms_send_state 	= FALSE;
	/* ����������ͨ��7�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch7_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��8����״̬ */
	u8 sensor_ch8_alarm_sr 	= 0;
	/* ����������ͨ��8����Ϣ����״̬ */
	static BOOL sensor_ch8_sms_send_state 	= FALSE;
	/* ����������ͨ��8�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch8_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��9����״̬ */
	u8 sensor_ch9_alarm_sr 	= 0;
	/* ����������ͨ��9����Ϣ����״̬ */
	static BOOL sensor_ch9_sms_send_state 	= FALSE;
	/* ����������ͨ��9�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch9_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��10����״̬ */
	u8 sensor_ch10_alarm_sr = 0;
	/* ����������ͨ��10����Ϣ����״̬ */
	static BOOL sensor_ch10_sms_send_state 	= FALSE;
	/* ����������ͨ��10�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch10_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��11����״̬ */
	u8 sensor_ch11_alarm_sr = 0;
	/* ����������ͨ��11����Ϣ����״̬ */
	static BOOL sensor_ch11_sms_send_state 	= FALSE;
	/* ����������ͨ��11�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch11_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��12����״̬ */
	u8 sensor_ch12_alarm_sr = 0;
	/* ����������ͨ��12����Ϣ����״̬ */
	static BOOL sensor_ch12_sms_send_state 	= FALSE;
	/* ����������ͨ��12�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch12_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��13����״̬ */
	u8 sensor_ch13_alarm_sr = 0;
	/* ����������ͨ��13����Ϣ����״̬ */
	static BOOL sensor_ch13_sms_send_state 	= FALSE;
	/* ����������ͨ��13�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch13_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��14����״̬ */
	u8 sensor_ch14_alarm_sr = 0;
	/* ����������ͨ��14����Ϣ����״̬ */
	static BOOL sensor_ch14_sms_send_state 	= FALSE;
	/* ����������ͨ��14�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch14_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��15����״̬ */
	u8 sensor_ch15_alarm_sr = 0;
	/* ����������ͨ��15����Ϣ����״̬ */
	static BOOL sensor_ch15_sms_send_state 	= FALSE;
	/* ����������ͨ��15�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch15_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��16����״̬ */
	u8 sensor_ch16_alarm_sr = 0;
	/* ����������ͨ��16����Ϣ����״̬ */
	static BOOL sensor_ch16_sms_send_state 	= FALSE;
	/* ����������ͨ��16�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch16_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��17����״̬ */
	u8 sensor_ch17_alarm_sr = 0;
	/* ����������ͨ��17����Ϣ����״̬ */
	static BOOL sensor_ch17_sms_send_state 	= FALSE;
	/* ����������ͨ��17�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch17_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��18����״̬ */
	u8 sensor_ch18_alarm_sr = 0;
	/* ����������ͨ��18����Ϣ����״̬ */
	static BOOL sensor_ch18_sms_send_state 	= FALSE;
	/* ����������ͨ��18�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch18_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��19����״̬ */
	u8 sensor_ch19_alarm_sr = 0;
	/* ����������ͨ��19����Ϣ����״̬ */
	static BOOL sensor_ch19_sms_send_state 	= FALSE;
	/* ����������ͨ��19�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch19_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��20����״̬ */
	u8 sensor_ch20_alarm_sr = 0;
	/* ����������ͨ��20����Ϣ����״̬ */
	static BOOL sensor_ch20_sms_send_state 	= FALSE;
	/* ����������ͨ��20�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch20_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��21����״̬ */
	u8 sensor_ch21_alarm_sr = 0;
	/* ����������ͨ��21����Ϣ����״̬ */
	static BOOL sensor_ch21_sms_send_state 	= FALSE;
	/* ����������ͨ��21�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch21_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��22����״̬ */
	u8 sensor_ch22_alarm_sr = 0;
	/* ����������ͨ��22����Ϣ����״̬ */
	static BOOL sensor_ch22_sms_send_state 	= FALSE;
	/* ����������ͨ��22�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch22_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��23����״̬ */
	u8 sensor_ch23_alarm_sr = 0;
	/* ����������ͨ��23����Ϣ����״̬ */
	static BOOL sensor_ch23_sms_send_state 	= FALSE;
	/* ����������ͨ��23�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch23_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��24����״̬ */
	u8 sensor_ch24_alarm_sr = 0;
	/* ����������ͨ��24����Ϣ����״̬ */
	static BOOL sensor_ch24_sms_send_state 	= FALSE;
	/* ����������ͨ��24�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch24_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��25����״̬ */
	u8 sensor_ch25_alarm_sr = 0;
	/* ����������ͨ��25����Ϣ����״̬ */
	static BOOL sensor_ch25_sms_send_state 	= FALSE;
	/* ����������ͨ��25�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch25_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��26����״̬ */
	u8 sensor_ch26_alarm_sr = 0;
	/* ����������ͨ��26����Ϣ����״̬ */
	static BOOL sensor_ch26_sms_send_state 	= FALSE;
	/* ����������ͨ��26�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch26_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��27����״̬ */
	u8 sensor_ch27_alarm_sr = 0;
	/* ����������ͨ��27����Ϣ����״̬ */
	static BOOL sensor_ch27_sms_send_state 	= FALSE;
	/* ����������ͨ��27�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch27_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��28����״̬ */
	u8 sensor_ch28_alarm_sr = 0;
	/* ����������ͨ��28����Ϣ����״̬ */
	static BOOL sensor_ch28_sms_send_state 	= FALSE;
	/* ����������ͨ��28�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch28_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��29����״̬ */
	u8 sensor_ch29_alarm_sr = 0;
	/* ����������ͨ��29����Ϣ����״̬ */
	static BOOL sensor_ch29_sms_send_state 	= FALSE;
	/* ����������ͨ��29�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch29_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��30����״̬ */
	u8 sensor_ch30_alarm_sr = 0;
	/* ����������ͨ��30����Ϣ����״̬ */
	static BOOL sensor_ch30_sms_send_state 	= FALSE;
	/* ����������ͨ��30�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch30_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��31����״̬ */
	u8 sensor_ch31_alarm_sr = 0;
	/* ����������ͨ��31����Ϣ����״̬ */
	static BOOL sensor_ch31_sms_send_state 	= FALSE;
	/* ����������ͨ��31�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch31_sms_again_send_time_reg = 0;
#endif
	
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
	/* ������ͨ��32����״̬ */
	u8 sensor_ch32_alarm_sr = 0;
	/* ����������ͨ��32����Ϣ����״̬ */
	static BOOL sensor_ch32_sms_send_state 	= FALSE;
	/* ����������ͨ��32�ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 sensor_ch32_sms_again_send_time_reg = 0;
#endif
	
	/* ��Դ���� �ı���״̬ */
	u8 power_alarm_sr 	= 0;
	/* ��ص��� �ı���״̬ */
	u8 battery_alarm_sr = 0;
	/* ���籨�� ����Ϣ����״̬ */
	static BOOL power_sms_send_state 	= FALSE;
	/* ��ص����� ����Ϣ����״̬ */
	static BOOL battery_sms_send_state 	= FALSE;
	/* ���籨�� �ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 power_sms_again_send_time_reg = 0;
	/* ��ص����� �ظ�����Ϣ����ʱ��Ĵ��� */
	static u32 battery_sms_again_send_time_reg = 0;
	
	/* ϵͳ��ǰʱ�� */
	u32 cur_time;	
	
	
#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
	/* �ж� ʵʱ���ݴ洢����. */
	if(config_tab->Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch == FALSE)
	{
		return;
	}
#endif
	
	/* ��ȡϵͳʱ�� */
	cur_time = OSTimeGet();
	
	/* ��� ������ ���� */
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch1_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH1,&fix_tab->Inf_State.Instru_Sensor.Inf.CH1);
#endif
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch2_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH2,&fix_tab->Inf_State.Instru_Sensor.Inf.CH2);
#endif
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch3_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH3,&fix_tab->Inf_State.Instru_Sensor.Inf.CH3);
#endif
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch4_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH4,&fix_tab->Inf_State.Instru_Sensor.Inf.CH4);
#endif
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch5_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH5,&fix_tab->Inf_State.Instru_Sensor.Inf.CH5);
#endif
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch6_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH6,&fix_tab->Inf_State.Instru_Sensor.Inf.CH6);
#endif
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_Hum) || \
	 (Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch7_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH7,&fix_tab->Inf_State.Instru_Sensor.Inf.CH7);
#endif
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch8_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH8,&fix_tab->Inf_State.Instru_Sensor.Inf.CH8);
#endif
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch9_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH9,&fix_tab->Inf_State.Instru_Sensor.Inf.CH9);
#endif
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch10_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH10,&fix_tab->Inf_State.Instru_Sensor.Inf.CH10);
#endif
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch11_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH11,&fix_tab->Inf_State.Instru_Sensor.Inf.CH11);
#endif
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch12_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH12,&fix_tab->Inf_State.Instru_Sensor.Inf.CH12);
#endif
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch13_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH13,&fix_tab->Inf_State.Instru_Sensor.Inf.CH13);
#endif
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch14_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH14,&fix_tab->Inf_State.Instru_Sensor.Inf.CH14);
#endif
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch15_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH15,&fix_tab->Inf_State.Instru_Sensor.Inf.CH15);
#endif
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch16_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH16,&fix_tab->Inf_State.Instru_Sensor.Inf.CH16);
#endif
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch17_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH17,&fix_tab->Inf_State.Instru_Sensor.Inf.CH17);
#endif
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch18_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH18,&fix_tab->Inf_State.Instru_Sensor.Inf.CH18);
#endif
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch19_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH19,&fix_tab->Inf_State.Instru_Sensor.Inf.CH19);
#endif
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch20_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH20,&fix_tab->Inf_State.Instru_Sensor.Inf.CH20);
#endif
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch21_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH21,&fix_tab->Inf_State.Instru_Sensor.Inf.CH21);
#endif
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch22_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH22,&fix_tab->Inf_State.Instru_Sensor.Inf.CH22);
#endif
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch23_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH23,&fix_tab->Inf_State.Instru_Sensor.Inf.CH23);
#endif
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch24_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH24,&fix_tab->Inf_State.Instru_Sensor.Inf.CH24);
#endif
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch25_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH25,&fix_tab->Inf_State.Instru_Sensor.Inf.CH25);
#endif
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch26_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH26,&fix_tab->Inf_State.Instru_Sensor.Inf.CH26);
#endif
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch27_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH27,&fix_tab->Inf_State.Instru_Sensor.Inf.CH27);
#endif
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch28_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH28,&fix_tab->Inf_State.Instru_Sensor.Inf.CH28);
#endif
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch29_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH29,&fix_tab->Inf_State.Instru_Sensor.Inf.CH29);
#endif
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch30_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH30,&fix_tab->Inf_State.Instru_Sensor.Inf.CH30);
#endif
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch31_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH31,&fix_tab->Inf_State.Instru_Sensor.Inf.CH31);
#endif
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
	sensor_ch32_alarm_sr = Sensor_Alarm_Check(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH32,&fix_tab->Inf_State.Instru_Sensor.Inf.CH32);
#endif


	/* ��� ��Դ����״̬ */
	power_alarm_sr = Power_ET_Alarm_Check(&config_tab->Inf_State.Instru_Power,&fix_tab->Inf_State.Instru_Power);
	/* ��� ��ص���״̬ */
	battery_alarm_sr = Power_BT_Alarm_Check(&config_tab->Inf_State.Instru_Power,&fix_tab->Inf_State.Instru_Power);
	
	/* �ж� ���� �����ܿ��� */
	if(config_tab->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch == TRUE)
	{
		/* ���� �����ܿ���:�� */
		/* �ж� ������ ����ͨ�� ����״̬ */
		if(FALSE
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))			
		||(sensor_ch1_alarm_sr & 0x01)
#endif
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))		
		||(sensor_ch2_alarm_sr & 0x01)
#endif
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))		
		||(sensor_ch3_alarm_sr & 0x01) 
#endif
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))		
		||(sensor_ch4_alarm_sr & 0x01)
#endif
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch5_alarm_sr & 0x01)
#endif
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch6_alarm_sr & 0x01) 
#endif
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_Hum) || \
	 (Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch7_alarm_sr & 0x01) 
#endif
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch8_alarm_sr & 0x01)
#endif
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch9_alarm_sr & 0x01) 
#endif
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch10_alarm_sr & 0x01)
#endif
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch11_alarm_sr & 0x01)
#endif
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch12_alarm_sr & 0x01)
#endif
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch13_alarm_sr & 0x01)
#endif
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch14_alarm_sr & 0x01)
#endif
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch15_alarm_sr & 0x01)
#endif
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch16_alarm_sr & 0x01)
#endif
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch17_alarm_sr & 0x01)
#endif
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch18_alarm_sr & 0x01)
#endif
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch19_alarm_sr & 0x01)
#endif
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch20_alarm_sr & 0x01)
#endif
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch21_alarm_sr & 0x01)
#endif
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch22_alarm_sr & 0x01)
#endif
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch23_alarm_sr & 0x01)
#endif
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch24_alarm_sr & 0x01)
#endif
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch25_alarm_sr & 0x01)
#endif
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch26_alarm_sr & 0x01)
#endif
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch27_alarm_sr & 0x01)
#endif
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch28_alarm_sr & 0x01)
#endif
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch29_alarm_sr & 0x01)
#endif
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch30_alarm_sr & 0x01)
#endif
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch31_alarm_sr & 0x01)
#endif
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch32_alarm_sr & 0x01)
#endif
		)
		{
			/* ���� ������ �������� ���� */
			/* ���� ������ �������� */
			Sensor_Alarm_Sound();
		}/* �ж� ��Դ����״̬ ���� */
		else if(power_alarm_sr & 0x01)
		{
			/* ���� ������ �������� ���� */
			/* ���� ������ �������� */
			Power_Alarm_Sound();
		}/* �ж� ��ص����� ����״̬ */
		else if(battery_alarm_sr & 0x01)
		{
			/* ���� ��ص����� �������� ���� */
			/* ���� ��ص����� �������� */
			Battery_Alarm_Sound();
		}
	
	}
	
	/* �ж� �� �����ܿ��� */
	if(config_tab->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch == TRUE)
	{
		/* �� �����ܿ���:�� */
		/* �ж� ������ ����ͨ�� ����״̬ */
		if(FALSE
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))			
		||(sensor_ch1_alarm_sr & 0x02)
#endif
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))		
		||(sensor_ch2_alarm_sr & 0x02)
#endif
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))		
		||(sensor_ch3_alarm_sr & 0x02) 
#endif
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))		
		||(sensor_ch4_alarm_sr & 0x02)
#endif
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch5_alarm_sr & 0x02)
#endif
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch6_alarm_sr & 0x02) 
#endif
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_Hum) || \
	 (Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch7_alarm_sr & 0x02) 
#endif
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch8_alarm_sr & 0x02)
#endif
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch9_alarm_sr & 0x02) 
#endif
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch10_alarm_sr & 0x02)
#endif
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch11_alarm_sr & 0x02)
#endif
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch12_alarm_sr & 0x02)
#endif
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch13_alarm_sr & 0x02)
#endif
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch14_alarm_sr & 0x02)
#endif
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch15_alarm_sr & 0x02)
#endif
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch16_alarm_sr & 0x02)
#endif
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch17_alarm_sr & 0x02)
#endif
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch18_alarm_sr & 0x02)
#endif
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch19_alarm_sr & 0x02)
#endif
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch20_alarm_sr & 0x02)
#endif
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch21_alarm_sr & 0x02)
#endif
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch22_alarm_sr & 0x02)
#endif
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch23_alarm_sr & 0x02)
#endif
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch24_alarm_sr & 0x02)
#endif
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch25_alarm_sr & 0x02)
#endif
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch26_alarm_sr & 0x02)
#endif
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch27_alarm_sr & 0x02)
#endif
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch28_alarm_sr & 0x02)
#endif
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch29_alarm_sr & 0x02)
#endif
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch30_alarm_sr & 0x02)
#endif
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch31_alarm_sr & 0x02)
#endif
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
		||(sensor_ch32_alarm_sr & 0x02)
#endif
		)
		{
			/* ���� ������ �� ���� ���� */
			/* ���� ������ �� ���� */
			Sensor_Alarm_Light();
		}/* �ж� ������ ����ͨ�� ����״̬ */
		else if(power_alarm_sr & 0x02)
		{
			/* ���� ��Դ���� �� ���� ���� */
			/* ���� ��Դ���� �� ���� */
			Power_Alarm_Light();
		}/* �ж� ��ص����� ����״̬ */
		else if(battery_alarm_sr & 0x02)
		{
			/* ���� ��ص����� �� ���� ���� */
			/* ���� ��ص����� �� ���� */
			Battery_Alarm_Light();
		}
	}
	
	/* �ж� ���� �����ܿ��� */
	if(config_tab->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch == TRUE)
	{
		/* �� �����ܿ���:�� */
		/* �ж� �Ƿ��ж��ű������ڷ��͡� */
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_State == FALSE)
		{
			/* û�����ڷ��͵Ķ��Ż��߷����Ѿ����� */
			/* �ж϶��ŷ��ͽ�� */
#if (SMS_User_Number == 0x01)
			if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] == TRUE)
#elif (SMS_User_Number == 0x02)		
			if((GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] == TRUE)&&(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] == TRUE))
#elif (SMS_User_Number == 0x03)
			if((GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] == TRUE)&&(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] == TRUE)&&(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] == TRUE))
#else
#error "û�����ö��ű����û�"
#endif		
			{
				/* ���ŷ��ͳɹ� */
				/* �ж�����������Ϣ��Ҫ���͡� */
			
///* �ж� �����������Ƿ����� */
//#if (Sensor_Config_Enable != 0x00)
//����Ӧ�����óɶ��ű������ù���
				if(0)
				{
					__nop();
				}
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��1 ����ͨ�� ����״̬ */
				else if((sensor_ch1_alarm_sr & 0x04)&&(sensor_ch1_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch1_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_1);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch1_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��2 ����ͨ�� ����״̬ */
				else if((sensor_ch2_alarm_sr & 0x04)&&(sensor_ch2_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch2_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_2);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch2_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��3 ����ͨ�� ����״̬ */
				else if((sensor_ch3_alarm_sr & 0x04)&&(sensor_ch3_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch3_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_3);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch3_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��4 ����ͨ�� ����״̬ */
				else if((sensor_ch4_alarm_sr & 0x04)&&(sensor_ch4_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch4_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_4);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch4_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))	
				/* �ж� ������ͨ��5 ����ͨ�� ����״̬ */
				else if((sensor_ch5_alarm_sr & 0x04)&&(sensor_ch5_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch5_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_5);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch5_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��6 ����ͨ�� ����״̬ */
				else if((sensor_ch6_alarm_sr & 0x04)&&(sensor_ch6_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch6_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_6);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch6_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_Hum) || \
				(Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��7 ����ͨ�� ����״̬ */
				else if((sensor_ch7_alarm_sr & 0x04)&&(sensor_ch7_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch7_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_7);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch7_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��8 ����ͨ�� ����״̬ */
				else if((sensor_ch8_alarm_sr & 0x04)&&(sensor_ch8_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch8_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_8);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch8_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��9 ����ͨ�� ����״̬ */
				else if((sensor_ch9_alarm_sr & 0x04)&&(sensor_ch9_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch9_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_9);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch9_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��8 ����ͨ�� ����״̬ */
				else if((sensor_ch10_alarm_sr & 0x04)&&(sensor_ch10_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch10_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_10);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch10_sms_again_send_time_reg = cur_time;
				}
#endif		
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��11 ����ͨ�� ����״̬ */
				else if((sensor_ch11_alarm_sr & 0x04)&&(sensor_ch11_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch11_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_11);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch11_sms_again_send_time_reg = cur_time;
				}
#endif		
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��12 ����ͨ�� ����״̬ */
				else if((sensor_ch12_alarm_sr & 0x04)&&(sensor_ch12_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch12_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_12);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch12_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��13 ����ͨ�� ����״̬ */
				else if((sensor_ch13_alarm_sr & 0x04)&&(sensor_ch13_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch13_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_13);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch13_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��14 ����ͨ�� ����״̬ */
				else if((sensor_ch14_alarm_sr & 0x04)&&(sensor_ch14_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch14_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_14);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch14_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��15 ����ͨ�� ����״̬ */
				else if((sensor_ch15_alarm_sr & 0x04)&&(sensor_ch15_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch15_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_15);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch15_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��16 ����ͨ�� ����״̬ */
				else if((sensor_ch16_alarm_sr & 0x04)&&(sensor_ch16_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch16_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_16);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch16_sms_again_send_time_reg = cur_time;
				}
#endif		
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��17 ����ͨ�� ����״̬ */
				else if((sensor_ch17_alarm_sr & 0x04)&&(sensor_ch17_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch17_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_17);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch17_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��18 ����ͨ�� ����״̬ */
				else if((sensor_ch18_alarm_sr & 0x04)&&(sensor_ch18_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch18_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_18);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch18_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��19 ����ͨ�� ����״̬ */
				else if((sensor_ch19_alarm_sr & 0x04)&&(sensor_ch19_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch19_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_19);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch19_sms_again_send_time_reg = cur_time;
				}
#endif		
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��20 ����ͨ�� ����״̬ */
				else if((sensor_ch20_alarm_sr & 0x04)&&(sensor_ch20_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch20_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_20);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch20_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��21 ����ͨ�� ����״̬ */
				else if((sensor_ch21_alarm_sr & 0x04)&&(sensor_ch21_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch21_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_21);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch21_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��22 ����ͨ�� ����״̬ */
				else if((sensor_ch22_alarm_sr & 0x04)&&(sensor_ch22_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch22_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_22);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch22_sms_again_send_time_reg = cur_time;
				}
#endif		
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH223_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH223_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��23 ����ͨ�� ����״̬ */
				else if((sensor_ch23_alarm_sr & 0x04)&&(sensor_ch23_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch23_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_23);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch23_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH224_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH224_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��24 ����ͨ�� ����״̬ */
				else if((sensor_ch24_alarm_sr & 0x04)&&(sensor_ch24_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch24_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_24);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch24_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��25 ����ͨ�� ����״̬ */
				else if((sensor_ch25_alarm_sr & 0x04)&&(sensor_ch25_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch25_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_25);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch25_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��26 ����ͨ�� ����״̬ */
				else if((sensor_ch26_alarm_sr & 0x04)&&(sensor_ch26_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch26_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_26);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch26_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��27 ����ͨ�� ����״̬ */
				else if((sensor_ch27_alarm_sr & 0x04)&&(sensor_ch27_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch27_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_27);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch27_sms_again_send_time_reg = cur_time;
				}
#endif		
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��28 ����ͨ�� ����״̬ */
				else if((sensor_ch28_alarm_sr & 0x04)&&(sensor_ch28_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch28_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_28);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch28_sms_again_send_time_reg = cur_time;
				}
#endif		
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��29 ����ͨ�� ����״̬ */
				else if((sensor_ch29_alarm_sr & 0x04)&&(sensor_ch29_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch29_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_29);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch29_sms_again_send_time_reg = cur_time;
				}
#endif		
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��30 ����ͨ�� ����״̬ */
				else if((sensor_ch30_alarm_sr & 0x04)&&(sensor_ch30_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch30_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_30);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch30_sms_again_send_time_reg = cur_time;
				}
#endif	
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��31 ����ͨ�� ����״̬ */
				else if((sensor_ch31_alarm_sr & 0x04)&&(sensor_ch31_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch31_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_31);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch31_sms_again_send_time_reg = cur_time;
				}
#endif			
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
				/* �ж� ������ͨ��32 ����ͨ�� ����״̬ */
				else if((sensor_ch32_alarm_sr & 0x04)&&(sensor_ch32_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					sensor_ch32_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_32);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					sensor_ch32_sms_again_send_time_reg = cur_time;
				}
#endif	
//#endif			
		
				/* �ж� ��Դ���� ����ͨ�� ����״̬ */
				else if((power_alarm_sr & 0x04)&&(power_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					power_sms_send_state = SMS_Send_Power_Inf(config_tab,fix_tab);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					power_sms_again_send_time_reg = cur_time;
				}
				/* �ж� ��ص����� ����״̬ */
				else if((battery_alarm_sr & 0x04)&&(battery_sms_send_state == FALSE))
				{
					/* ���� �������ŷ��� */
					battery_sms_send_state = SMS_Send_Battery_Inf(config_tab,fix_tab);
					/* ����ǰʱ������Ϊ���ŷ���ʱ�䣬Ϊ�������ظ����Ͷ�Ϣ��������׼ʱ�䡣 */
					battery_sms_again_send_time_reg = cur_time;
				}
				
			}else
			{
				/* ��ǰ���ŷ���ʧ�ܣ��������·��Ͷ��š� */
				GSM_SMS_SR_Reg.GSM_SMS_Send_State = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Send_SMS_EVENT);
			}
		}
	}
	
///* �ж� �����������Ƿ����� */
//#if (Sensor_Config_Enable != 0x00)
	
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	

	/* �ж� ������ ͨ��1 ����״̬ */
	if((sensor_ch1_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��1 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch1_sms_send_state == TRUE)
		{
			/* ������ ͨ��1 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch1_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��1 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch1_sms_send_state == TRUE)
		{
			/* ���� ���ͳɹ� */
			/* �ж� �����ط� ״̬ */
			sensor_ch1_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch1_sms_again_send_time_reg,sensor_ch1_alarm_sr);	
		}
	}
#endif	
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		
	/* �ж� ������ ͨ��2 ����״̬ */
	if((sensor_ch2_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��2 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch2_sms_send_state == TRUE)
		{
			/* ������ ͨ��2 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch2_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��1 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch2_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch2_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch2_sms_again_send_time_reg,sensor_ch2_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		
	/* �ж� ������ ͨ��3 ����״̬ */
	if((sensor_ch3_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��3 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch3_sms_send_state == TRUE)
		{
			/* ������ ͨ��3 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch3_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��3 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch3_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch3_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch3_sms_again_send_time_reg,sensor_ch3_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��4 ����״̬ */
	if((sensor_ch4_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��4 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch4_sms_send_state == TRUE)
		{
			/* ������ ͨ��4 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch4_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��4 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch4_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch4_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch4_sms_again_send_time_reg,sensor_ch4_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��5 ����״̬ */
	if((sensor_ch5_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��5 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch5_sms_send_state == TRUE)
		{
			/* ������ ͨ��5 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch5_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��5 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch5_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch5_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch5_sms_again_send_time_reg,sensor_ch5_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��6 ����״̬ */
	if((sensor_ch6_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��6 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch6_sms_send_state == TRUE)
		{
			/* ������ ͨ��6 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch6_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��6 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch6_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch6_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch6_sms_again_send_time_reg,sensor_ch6_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_Hum) || \
	(Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��7 ����״̬ */
	if((sensor_ch7_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��7 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch7_sms_send_state == TRUE)
		{
			/* ������ ͨ��7 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch7_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��7 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch7_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch7_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch7_sms_again_send_time_reg,sensor_ch7_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��8 ����״̬ */
	if((sensor_ch8_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��8 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch8_sms_send_state == TRUE)
		{
			/* ������ ͨ��8 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch8_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��8 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch8_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch8_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch8_sms_again_send_time_reg,sensor_ch8_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��9 ����״̬ */
	if((sensor_ch9_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��9 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch9_sms_send_state == TRUE)
		{
			/* ������ ͨ��9 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch9_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��9 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch9_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch9_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch9_sms_again_send_time_reg,sensor_ch9_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��10 ����״̬ */
	if((sensor_ch10_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��10 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch10_sms_send_state == TRUE)
		{
			/* ������ ͨ��10 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch10_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��10 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch10_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch10_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch10_sms_again_send_time_reg,sensor_ch10_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��11 ����״̬ */
	if((sensor_ch11_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��11 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch11_sms_send_state == TRUE)
		{
			/* ������ ͨ��11 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch11_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��11 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch11_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch11_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch11_sms_again_send_time_reg,sensor_ch11_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��12 ����״̬ */
	if((sensor_ch12_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��12 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch12_sms_send_state == TRUE)
		{
			/* ������ ͨ��12 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch12_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��12 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch12_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch12_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch12_sms_again_send_time_reg,sensor_ch12_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��13 ����״̬ */
	if((sensor_ch13_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��13 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch13_sms_send_state == TRUE)
		{
			/* ������ ͨ��13 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch13_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��13 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch13_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch13_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch13_sms_again_send_time_reg,sensor_ch13_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��14 ����״̬ */
	if((sensor_ch14_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��14 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch14_sms_send_state == TRUE)
		{
			/* ������ ͨ��14 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch14_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��14 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch14_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch14_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch14_sms_again_send_time_reg,sensor_ch14_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��15 ����״̬ */
	if((sensor_ch15_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��15 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch15_sms_send_state == TRUE)
		{
			/* ������ ͨ��15 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch15_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��15 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch15_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch15_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch15_sms_again_send_time_reg,sensor_ch15_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��16 ����״̬ */
	if((sensor_ch16_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��16 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch16_sms_send_state == TRUE)
		{
			/* ������ ͨ��16 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch16_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��16 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch16_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch16_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch16_sms_again_send_time_reg,sensor_ch16_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��17 ����״̬ */
	if((sensor_ch17_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��17 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch17_sms_send_state == TRUE)
		{
			/* ������ ͨ��17 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch17_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��17 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch17_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch17_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch17_sms_again_send_time_reg,sensor_ch17_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��18 ����״̬ */
	if((sensor_ch18_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��18 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch18_sms_send_state == TRUE)
		{
			/* ������ ͨ��18 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch18_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��18 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch18_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch18_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch18_sms_again_send_time_reg,sensor_ch18_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��19 ����״̬ */
	if((sensor_ch19_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��19 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch19_sms_send_state == TRUE)
		{
			/* ������ ͨ��19 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch19_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��19 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch19_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch19_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch19_sms_again_send_time_reg,sensor_ch19_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��20 ����״̬ */
	if((sensor_ch20_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��20 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch20_sms_send_state == TRUE)
		{
			/* ������ ͨ��20 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch20_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��20 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch20_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch20_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch20_sms_again_send_time_reg,sensor_ch20_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��21 ����״̬ */
	if((sensor_ch21_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��21 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch21_sms_send_state == TRUE)
		{
			/* ������ ͨ��21 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch21_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��21 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch21_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch21_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch21_sms_again_send_time_reg,sensor_ch21_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��22 ����״̬ */
	if((sensor_ch22_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��22 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch22_sms_send_state == TRUE)
		{
			/* ������ ͨ��22 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch22_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��22 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch22_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch22_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch22_sms_again_send_time_reg,sensor_ch22_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��23 ����״̬ */
	if((sensor_ch23_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��23 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch23_sms_send_state == TRUE)
		{
			/* ������ ͨ��23 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch23_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��23 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch23_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch23_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch23_sms_again_send_time_reg,sensor_ch23_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))	

	/* �ж� ������ ͨ��24 ����״̬ */
	if((sensor_ch24_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��24 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch24_sms_send_state == TRUE)
		{
			/* ������ ͨ��24 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch24_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��24 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch24_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch24_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch24_sms_again_send_time_reg,sensor_ch24_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
	
	/* �ж� ������ ͨ��25 ����״̬ */
	if((sensor_ch25_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��25 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch25_sms_send_state == TRUE)
		{
			/* ������ ͨ��25 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch25_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��25 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch25_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch25_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch25_sms_again_send_time_reg,sensor_ch25_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��26 ����״̬ */
	if((sensor_ch26_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��26 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch26_sms_send_state == TRUE)
		{
			/* ������ ͨ��26 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch26_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��26 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch26_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch26_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch26_sms_again_send_time_reg,sensor_ch26_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��27 ����״̬ */
	if((sensor_ch27_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��27 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch27_sms_send_state == TRUE)
		{
			/* ������ ͨ��27 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch27_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��27 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch27_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch27_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch27_sms_again_send_time_reg,sensor_ch27_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��28 ����״̬ */
	if((sensor_ch28_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��28 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch28_sms_send_state == TRUE)
		{
			/* ������ ͨ��28 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch28_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��28 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch28_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch28_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch28_sms_again_send_time_reg,sensor_ch28_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��29 ����״̬ */
	if((sensor_ch29_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��29 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch29_sms_send_state == TRUE)
		{
			/* ������ ͨ��29 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch29_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��29 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch29_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch29_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch29_sms_again_send_time_reg,sensor_ch29_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��30 ����״̬ */
	if((sensor_ch30_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��30 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch30_sms_send_state == TRUE)
		{
			/* ������ ͨ��30 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch30_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��30 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch30_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch30_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch30_sms_again_send_time_reg,sensor_ch30_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��31 ����״̬ */
	if((sensor_ch31_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��31 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch31_sms_send_state == TRUE)
		{
			/* ������ ͨ��31 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch31_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��31 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch31_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch31_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch31_sms_again_send_time_reg,sensor_ch31_alarm_sr);
		}
	}
#endif	
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))

	/* �ж� ������ ͨ��32 ����״̬ */
	if((sensor_ch32_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ������ ͨ��32 ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(sensor_ch32_sms_send_state == TRUE)
		{
			/* ������ ͨ��32 ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			sensor_ch32_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ������ ͨ��32 ���� */
		/* �ж� ���ŷ��� ״̬ */
		if(sensor_ch32_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			sensor_ch32_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch32_sms_again_send_time_reg,power_alarm_sr);
		}
	}
#endif	
//#endif

	/* �ж� �ϵ籨�� ״̬ */
	if((power_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ��Դ���� ���� */
		/* �ж� ���籨�� ����Ϣ����״̬ */
		if(power_sms_send_state == TRUE)
		{
			/* ���籨�� ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			power_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ �ϵ籨�� */
		/* �ж� ���ŷ��� ״̬ */
		if(power_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			power_sms_send_state = SMS_Again_Send_State_Check(cur_time,&power_sms_again_send_time_reg,power_alarm_sr);
		}
	}

	/* �ж� ��ص����ͱ���״̬ */
	if((battery_alarm_sr & 0x04) == 0X00)
	{
		/* ��ǰ ��ص��� ���� */
		/* �ж� ��ص����� ����Ϣ����״̬ */
		if(battery_sms_send_state == TRUE)
		{
			/* ��ص����� ������Ϣ�Ѿ����� */
			/* ��� ���ŷ��ͱ�־ */
			battery_sms_send_state = FALSE;
		}
	}else
	{
		/* ��ǰ ��ص����ͱ��� */
		/* �ж� ���ŷ��� ״̬ */
		if(battery_sms_send_state == TRUE)
		{
			/* �ж� �����ط� ״̬ */
			battery_sms_send_state = SMS_Again_Send_State_Check(cur_time,&battery_sms_again_send_time_reg,battery_alarm_sr);
		}
	}
	
}

/************************************************************************************************/
/* ������	: Reset_Sensor_Suspend_Alarm_Flag_All_CH											*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ���ȫ������ͨ���ı���ȡ����־														*/
/* ��������	: 2015/6/1																			*/
/************************************************************************************************/
void Reset_Sensor_Suspend_Alarm_Flag_All_CH(INSTRU_SENSOR_FIX* sensor_fix)
{
	
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��1 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH1.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH1.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��2 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH2.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH2.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��3 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH3.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH3.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��4 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH4.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH4.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��5 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH5.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH5.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��6 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH6.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH6.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_Hum) || \
	(Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��7 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH7.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH7.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��8 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH8.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH8.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��9 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH9.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH9.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��10 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH10.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH10.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��11 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH11.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH11.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��12 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH12.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH12.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��13 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH13.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH13.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��14 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH14.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH14.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��15 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH15.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH15.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��16 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH16.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH16.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��17 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH17.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH17.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��18 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH18.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH18.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��19 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH19.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH19.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��20 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH20.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH20.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��21 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH21.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH21.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��22 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH22.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH22.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��23 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH23.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH23.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��24 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH24.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH24.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��25 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH25.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH25.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��26 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH26.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH26.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��27 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH27.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH27.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��28 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH28.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH28.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��29 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH29.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH29.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��30 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH30.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH30.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��31 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH31.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH31.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif	
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��32 ����ȡ����־״̬�� */
	if(sensor_fix->Inf.CH32.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* �������ȡ����־ */
		sensor_fix->Inf.CH32.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif

}

/************************************************************************************************/
/* ������	: Reset_Sensor_Alarm_State_All_CH													*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ���ȫ������ͨ���ı���״̬															*/
/* ��������	: 2015/8/25																			*/
/************************************************************************************************/
void Reset_Sensor_Alarm_State_All_CH(INSTRU_SENSOR* sensor_c)
{
	
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��1 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH1.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH1.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��2 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH2.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH2.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��3 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH3.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH3.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��4 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH4.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH4.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��5 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH5.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH5.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��6 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH6.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH6.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_Hum) || \
	(Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��7 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH7.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH7.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��8 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH8.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH8.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��9 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH9.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH9.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��10 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH10.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH10.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��11 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH11.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH11.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��12 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH12.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH12.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��13 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH13.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH13.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��14 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH14.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH14.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��15 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH15.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH15.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��16 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH16.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH16.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��17 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH17.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH17.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��18 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH18.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH18.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��19 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH19.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH19.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��20 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH20.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH20.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��21 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH21.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH21.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��22 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH22.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH22.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��23 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH23.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH23.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��24 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH24.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH24.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��25 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH25.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH25.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��26 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH26.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH26.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��27 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH27.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH27.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��28 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH28.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH28.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��29 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH29.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH29.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��30 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH30.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH30.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��31 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH31.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH31.SENSOR_Alarm_State = FALSE;
	}
#endif	
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� ������ ͨ��32 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH32.SENSOR_Alarm_State == TRUE)
	{
		/* �������ȡ����־ */
		sensor_c->Inf_State.CH32.SENSOR_Alarm_State = FALSE;
	}
#endif

}

/************************************************************************************************/
/* ������	: Set_Sensor_Suspend_Alarm_Flag_All_CH												*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ȫ������ͨ���ı���ȡ����־															*/
/* ��������	: 2015/6/1																			*/
/************************************************************************************************/
void Set_Sensor_Suspend_Alarm_Flag_All_CH(INSTRU_SENSOR* sensor_c,INSTRU_SENSOR_FIX* sensor_f)
{
	
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH1_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��1 �ı���״̬ */
	if(sensor_c->Inf_State.CH1.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��1���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH1.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH2_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_Hum) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH2_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��2 ����ȡ����־״̬�� */
	if(sensor_c->Inf_State.CH2.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��2���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH2.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH3_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_Hum) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH3_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��3 �ı���״̬ */
	if(sensor_c->Inf_State.CH3.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��3���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH3.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH4_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_Hum) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH4_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��4 �ı���״̬ */
	if(sensor_c->Inf_State.CH4.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��4���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH4.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH5_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) || \
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) || \
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum) ||(Sensor_CH5_Config_Type == Sensor_PT1000) )
	/* �ж� ������ ͨ��5 �ı���״̬ */
	if(sensor_c->Inf_State.CH5.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��5���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH5.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH6_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_Hum) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH6_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��6 �ı���״̬ */
	if(sensor_c->Inf_State.CH6.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��6���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH6.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH7_Config_Type == Sensor_Tmp) || \
		(Sensor_CH7_Config_Type == Sensor_Hum) || \
	 (Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH7_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��7 �ı���״̬ */
	if(sensor_c->Inf_State.CH7.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��7���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH7.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH8_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_Hum) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH8_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��8 �ı���״̬ */
	if(sensor_c->Inf_State.CH8.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��8���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH8.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH9_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_Hum) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH9_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��9 �ı���״̬ */
	if(sensor_c->Inf_State.CH9.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��9���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH9.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH10_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_Hum) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH10_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��10 �ı���״̬ */
	if(sensor_c->Inf_State.CH10.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��10���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH10.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH11_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_Hum) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH11_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��11 �ı���״̬ */
	if(sensor_c->Inf_State.CH11.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��11���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH11.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH12_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_Hum) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH12_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��12 �ı���״̬ */
	if(sensor_c->Inf_State.CH12.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��12���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH12.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH13_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_Hum) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH13_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��13 �ı���״̬ */
	if(sensor_c->Inf_State.CH13.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��13���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH13.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH14_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_Hum) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH14_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��14 �ı���״̬ */
	if(sensor_c->Inf_State.CH14.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��14���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH14.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH15_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_Hum) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH15_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��15 �ı���״̬ */
	if(sensor_c->Inf_State.CH15.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��15���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH15.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH16_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_Hum) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH16_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��16 �ı���״̬ */
	if(sensor_c->Inf_State.CH16.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��16���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH16.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH17_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_Hum) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH17_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��17 �ı���״̬ */
	if(sensor_c->Inf_State.CH17.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��17���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH17.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH18_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_Hum) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH18_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��18 �ı���״̬ */
	if(sensor_c->Inf_State.CH18.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��18���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH18.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH19_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_Hum) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH19_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��19 �ı���״̬ */
	if(sensor_c->Inf_State.CH19.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��19���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH19.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH20_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_Hum) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH20_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��20 �ı���״̬ */
	if(sensor_c->Inf_State.CH20.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��20���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH20.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH21_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_Hum) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH21_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��21 �ı���״̬ */
	if(sensor_c->Inf_State.CH21.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��21���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH21.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH22_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_Hum) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH22_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��22 �ı���״̬ */
	if(sensor_c->Inf_State.CH22.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��22���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH22.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH23_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_Hum) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH23_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��23 �ı���״̬ */
	if(sensor_c->Inf_State.CH23.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��23���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH23.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH24_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_Hum) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH24_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��24 �ı���״̬ */
	if(sensor_c->Inf_State.CH24.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��24���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH24.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH25_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_Hum) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH25_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��25 �ı���״̬ */
	if(sensor_c->Inf_State.CH25.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��25���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH25.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH26_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_Hum) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH26_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��26 �ı���״̬ */
	if(sensor_c->Inf_State.CH26.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��26���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH26.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH27_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_Hum) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH27_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��27 �ı���״̬ */
	if(sensor_c->Inf_State.CH27.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��27���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH27.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH28_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_Hum) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH28_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��28 �ı���״̬ */
	if(sensor_c->Inf_State.CH28.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��28���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH28.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH29_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_Hum) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH29_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��29 �ı���״̬ */
	if(sensor_c->Inf_State.CH29.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��29���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH29.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH30_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_Hum) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH30_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��30 �ı���״̬ */
	if(sensor_c->Inf_State.CH30.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��30���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH30.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH31_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_Hum) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH31_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��31 �ı���״̬ */
	if(sensor_c->Inf_State.CH31.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��31���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH31.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif	
#if ((Sensor_CH32_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_Hum) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) || \
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)||(Sensor_CH32_Config_Type == Sensor_PT1000))
	/* �ж� ������ ͨ��32 �ı���״̬ */
	if(sensor_c->Inf_State.CH32.SENSOR_Alarm_State != Alarm_Null)
	{
		/* ������ͨ��32���� */
		/* ���ñ���ȡ����־ */
		sensor_f->Inf.CH32.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif
	
}



/******************* (C) ZHOU *****END OF FILE****/


