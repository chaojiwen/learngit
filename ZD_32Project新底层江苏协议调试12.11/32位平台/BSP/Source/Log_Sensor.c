/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/1/26																			 */
/*************************************************************************************************/
#include "System_Config.h"
#include "App_GPS.h"

#include "Log_Sensor.h"

#include "Log_Alarm.h"
#include "Log_Adc1.h"
#include "Log_Hum_Cal.h"
#include "Log_RF_Card.h"
#include "Log_Temp_Cal.h"
#include "Log_Temp_PT1000.h"

#include "Menu.H"
#include "Ks0108.h"

#include <time.h> //�õ���time����
#include <stdlib.h>
#include <math.h>


#if (Gateway_NJSY_Enable == 0x01)
#include "App_Gateway_NJSY.h"
#endif

/************************************************************************************************/
#define RoundTo(Num,Dgt) 		(((float)((int)(Num*((int)pow(10,Dgt)))))/pow(10,Dgt))

/************************************************************************************************/
/* �洢ADC���ݼĴ�����С */
#define Sensor_Store_Data_DR_Size	(33u)
/* �洢ADC���ݼĴ��� */
static float Sensor_Store_Data_DR[Sensor_Store_Data_DR_Size];
/************************************************************************************************/
#if (Wireless_RF_Card_Config_Enable == 0x01)
/* �洢��Ƭ�������ݼĴ�����С */
#define Sensor_Store_Power_DR_Size	(32u)
/* �洢��Ƭ�������ݼĴ��� */
static u8 	Sensor_Store_Power_DR[Sensor_Store_Power_DR_Size];
#endif
/************************************************************************************************/



/*
*********************************************************************************************************
*	�� �� ��: Sensor_State_Check_Alone
*	����˵��: ���ݶ�Ӧͨ���������޺�ʵʱ����,�� ��Ӧͨ��״̬��������.
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Sensor_State_Check_Alone(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{

	
	/* ����ͨ�������� */
	switch(sensor_t->Type)
	{


		case Sensor_Tmp:
		{
			/* �¶ȴ����� */
			break;
		}
	
		case Sensor_Hum:
		{
			/* ʪ�ȴ����� */
			break;
		}
	
		case Sensor_RF_Card_Single_Tmp:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* ��Ƭ���ݽ��ճ�ʱ������ */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif			
			/* ���¿�Ƭ */
			break;
		}
		
		case Sensor_RF_Card_Humiture_Tmp:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* ��Ƭ���ݽ��ճ�ʱ������ */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif		
			/* ��ʪ��Ƭ->�¶�ͨ�� */
			break;
		}	
		
		case Sensor_RF_Card_Humiture_Hum:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* ��Ƭ���ݽ��ճ�ʱ������ */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif		
			/* ��ʪ��Ƭ->ʪ��ͨ�� */
			break;
		}

		case Sensor_PT1000:
		{
			/* �¶ȴ����� */
			break;
		}
			case Sensor_GW_NJSY_Tmp:
		{
			/*�Ͼ�ʢ���¶ȴ����� */
			break;
		}
			case Sensor_GW_NJSY_Hum:
		{
			/*�Ͼ�ʢ��ʪ��ȴ����� */
			break;
		}
			case Sensor_CarbonDioxide:
		{
			/* ������̼������ */
			break;
		}

		default:
		{
			/* ������ͨ������Ϊ�������ñ�����������(����:���ȡ�γ�ȡ��ٶȵ�)���߷Ƿ�����:��Щ���Ͳ�����״̬����. */
			return;
		}
	}
	
	/* �ж��Ƿ񳬱� */
	/* ���� �����ж� */		
	if(sensor_f->Sensor_Real_Time_Data > sensor_t->SENSOR_Up_Limit)
	{
		/* ����״̬��� */
		if(sensor_t->SENSOR_Alarm_State & Sensor_Alarm_Up)
		{
			__nop();
		}else 
		{
			/* ���� ״̬ ���� */
			Alarm_State_Up_Set(sensor_t,sensor_f);	
		}	
		/* ���� ״̬ ���� */
		Alarm_State_Low_Reset(sensor_t,sensor_f);
	/* �¶����� �����ж� */		 
	}else if(sensor_f->Sensor_Real_Time_Data < sensor_t->SENSOR_Low_Limit)
	{
		if(sensor_t->SENSOR_Alarm_State & Sensor_Alarm_Low)
		{
			__nop();	
		}else if((sensor_f->Sensor_Real_Time_Data==-9990.0)||(sensor_f->Sensor_Real_Time_Data==-9990.4))//9990.04  zwc 2017.11.27
		{
				/* ���� ״̬ ���� */
	   	Alarm_State_Low_Reset(sensor_t,sensor_f);
		}
		
		else
		{
			/* ���� ״̬ ���� */
			Alarm_State_Low_Set(sensor_t,sensor_f);	
		}
	
		/* ���� ״̬ ���� */
		Alarm_State_Up_Reset(sensor_t,sensor_f);
	}else
	{
		/* ���� ״̬ ���� */
		Alarm_State_Up_Reset(sensor_t,sensor_f);
		/* ���� ״̬ ���� */
		Alarm_State_Low_Reset(sensor_t,sensor_f);
	}

}

/************************************************************************************************/
/* ������  ��Sensor_State_Check_ALL														 		*/
/* ����    ��(void *p_arg) 																		*/
/* ���	   : �� 																				*/
/* ����	   �������д�����״̬�����ж� 									 		 					*/
/* �������ڣ�2014/9/11																			*/
/************************************************************************************************/
void Sensor_State_Check_ALL(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* ���д�����ͨ�����ݽ����ж� */
	/* ���ݶ�Ӧͨ���������޺�ʵʱ����,�� ��Ӧͨ��״̬��������. */

/* �ж� ͨ��1 */
#if	((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH1_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH1,&fix_tab->Inf_State.Instru_Sensor.Inf.CH1);
#endif		
	
/* �ж� ͨ��2 */
#if	((Sensor_CH2_Config_Type == Sensor_Tmp) ||\
	(Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH2_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH2,&fix_tab->Inf_State.Instru_Sensor.Inf.CH2);
#endif	

/* �ж� ͨ��3 */
#if	((Sensor_CH3_Config_Type == Sensor_Tmp) ||\
(Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH3_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH3,&fix_tab->Inf_State.Instru_Sensor.Inf.CH3);
#endif	

/* �ж� ͨ��4 */
#if	((Sensor_CH4_Config_Type == Sensor_Tmp) ||\
(Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH4_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH4,&fix_tab->Inf_State.Instru_Sensor.Inf.CH4);
#endif	

/* �ж� ͨ��5 */
#if	((Sensor_CH5_Config_Type == Sensor_Tmp) ||\
(Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH5_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH5,&fix_tab->Inf_State.Instru_Sensor.Inf.CH5);
#endif	

/* �ж� ͨ��6 */
#if	((Sensor_CH6_Config_Type == Sensor_Tmp) ||\
(Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH6_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH6,&fix_tab->Inf_State.Instru_Sensor.Inf.CH6);
#endif

/* �ж� ͨ��7 */
#if	((Sensor_CH7_Config_Type == Sensor_Tmp) ||\
		(Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
		(Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
	 (Sensor_CH7_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH7,&fix_tab->Inf_State.Instru_Sensor.Inf.CH7);
#endif
	
/* �ж� ͨ��8 */
#if	((Sensor_CH8_Config_Type == Sensor_Tmp) ||\
(Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH8_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH8,&fix_tab->Inf_State.Instru_Sensor.Inf.CH8);
#endif

/* �ж� ͨ��9 */
#if	((Sensor_CH9_Config_Type == Sensor_Tmp) ||\
(Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH9_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH9,&fix_tab->Inf_State.Instru_Sensor.Inf.CH9);
#endif

/* �ж� ͨ��10 */
#if	((Sensor_CH10_Config_Type == Sensor_Tmp) ||\
(Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH10_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH10,&fix_tab->Inf_State.Instru_Sensor.Inf.CH10);
#endif

/* �ж� ͨ��11 */
#if	((Sensor_CH11_Config_Type == Sensor_Tmp) ||\
(Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH11_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH11,&fix_tab->Inf_State.Instru_Sensor.Inf.CH11);
#endif

/* �ж� ͨ��12 */
#if	((Sensor_CH12_Config_Type == Sensor_Tmp) ||\
(Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH12_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH12,&fix_tab->Inf_State.Instru_Sensor.Inf.CH12);
#endif

/* �ж� ͨ��13 */
#if	((Sensor_CH13_Config_Type == Sensor_Tmp) ||\
(Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH13_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH13,&fix_tab->Inf_State.Instru_Sensor.Inf.CH13);
#endif

/* �ж� ͨ��14 */
#if	((Sensor_CH14_Config_Type == Sensor_Tmp) ||\
(Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH14_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH14,&fix_tab->Inf_State.Instru_Sensor.Inf.CH14);
#endif

/* �ж� ͨ��15 */
#if	((Sensor_CH15_Config_Type == Sensor_Tmp) ||\
(Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH15_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH15,&fix_tab->Inf_State.Instru_Sensor.Inf.CH15);
#endif

/* �ж� ͨ��16 */
#if	((Sensor_CH16_Config_Type == Sensor_Tmp) ||\
(Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH16_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH16,&fix_tab->Inf_State.Instru_Sensor.Inf.CH16);
#endif

/* �ж� ͨ��17 */
#if	((Sensor_CH17_Config_Type == Sensor_Tmp) ||\
(Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH17_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH17,&fix_tab->Inf_State.Instru_Sensor.Inf.CH17);
#endif

/* �ж� ͨ��18 */
#if	((Sensor_CH18_Config_Type == Sensor_Tmp) ||\
(Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH18_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH18,&fix_tab->Inf_State.Instru_Sensor.Inf.CH18);
#endif

/* �ж� ͨ��19 */
#if	((Sensor_CH19_Config_Type == Sensor_Tmp) ||\
(Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH19_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH19,&fix_tab->Inf_State.Instru_Sensor.Inf.CH19);
#endif

/* �ж� ͨ��20 */
#if	((Sensor_CH20_Config_Type == Sensor_Tmp) ||\
(Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH20_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH20,&fix_tab->Inf_State.Instru_Sensor.Inf.CH20);
#endif

/* �ж� ͨ��21 */
#if	((Sensor_CH21_Config_Type == Sensor_Tmp) ||\
(Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH21_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH21,&fix_tab->Inf_State.Instru_Sensor.Inf.CH21);
#endif

/* �ж� ͨ��22 */
#if	((Sensor_CH22_Config_Type == Sensor_Tmp) ||\
(Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH22_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH22,&fix_tab->Inf_State.Instru_Sensor.Inf.CH22);
#endif

/* �ж� ͨ��23 */
#if	((Sensor_CH23_Config_Type == Sensor_Tmp) ||\
(Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH23_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH23,&fix_tab->Inf_State.Instru_Sensor.Inf.CH23);
#endif

/* �ж� ͨ��24 */
#if	((Sensor_CH24_Config_Type == Sensor_Tmp) ||\
(Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH24_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH24,&fix_tab->Inf_State.Instru_Sensor.Inf.CH24);
#endif

/* �ж� ͨ��25 */
#if	((Sensor_CH25_Config_Type == Sensor_Tmp) ||\
(Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH25_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH25,&fix_tab->Inf_State.Instru_Sensor.Inf.CH25);
#endif

/* �ж� ͨ��26 */
#if	((Sensor_CH26_Config_Type == Sensor_Tmp) ||\
(Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH26_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH26,&fix_tab->Inf_State.Instru_Sensor.Inf.CH26);
#endif

/* �ж� ͨ��27 */
#if	((Sensor_CH27_Config_Type == Sensor_Tmp) ||\
(Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH27_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH27,&fix_tab->Inf_State.Instru_Sensor.Inf.CH27);
#endif

/* �ж� ͨ��28 */
#if	((Sensor_CH28_Config_Type == Sensor_Tmp) ||\
(Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH28_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH28,&fix_tab->Inf_State.Instru_Sensor.Inf.CH28);
#endif

/* �ж� ͨ��29 */
#if	((Sensor_CH29_Config_Type == Sensor_Tmp) ||\
(Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH29_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH29,&fix_tab->Inf_State.Instru_Sensor.Inf.CH29);
#endif

/* �ж� ͨ��30 */
#if	((Sensor_CH30_Config_Type == Sensor_Tmp) ||\
(Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH30_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH30,&fix_tab->Inf_State.Instru_Sensor.Inf.CH30);
#endif

/* �ж� ͨ��31 */
#if	((Sensor_CH31_Config_Type == Sensor_Tmp) ||\
(Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH31_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH31,&fix_tab->Inf_State.Instru_Sensor.Inf.CH31);
#endif

/* �ж� ͨ��32 */
#if	((Sensor_CH32_Config_Type == Sensor_Tmp) ||\
(Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
	 (Sensor_CH32_Config_Type == Sensor_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp) ||\
	 (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
	Sensor_State_Check_Alone(&config_tab->Inf_State.Instru_Sensor.Inf_State.CH32,&fix_tab->Inf_State.Instru_Sensor.Inf.CH32);
#endif
	
}


/*
*******************************************************************************************
*	�� �� ��: DispSensorNumConfig
*	����˵��: ��������ʾ��Ŀ����
*	��    �Σ���
*	�� �� ֵ: ��
*******************************************************************************************
*/
void DispSensorNumConfig(u8 sensor_num)
{
	u8 num;

	/* ��� ��ʾ ��Ŀ */
	for(num = 0;num < Sensor_CH_Config_Num; num ++)
	{
		Menu_Cha_Tsd_CN[num] = (u8*)0;
		Menu_Cha_Tsd_EN[num] = (u8*)0;
	}
	
	/* ���ݴ����� ���� ������ʾ���� */
	if(sensor_num != 0)
	{	/* ��� ��ʾ ��Ŀ */
		for(num=0;num<sensor_num;num++)
		{
			/* ���� ��ʾ */
			Menu_Cha_Tsd_CN[num] = Cha_Tsd_CN_Tab[num];
			/* Ӣ�� ��ʾ */
			Menu_Cha_Tsd_EN[num] = Cha_Tsd_EN_Tab[num];
		}	
	}else
	{
			/* ���� ��ʾ */
			Menu_Cha_Tsd_CN[num] = Cha_NULL_CN_Tab;
			/* Ӣ�� ��ʾ */
			Menu_Cha_Tsd_EN[num] = Cha_NULL_EN_Tab;	
	}

} 

/*
*******************************************************************************************
*	�� �� ��: Temp_Sensor_Failure
*	����˵��: �¶ȴ����� �������
*	��    �Σ���
*	�� �� ֵ: ��
*******************************************************************************************
*/
void Temp_Sensor_Failure(void)
{
	//��ʱ��û�жԹ��ϵĴ��������д���
	/* ���ڻ�Դ������������ͽ��з��ദ��
	   1��δ��
	   2���Եض̽�
	   3����VCC�̽�
	   4������ */
	__nop();
}

/*
*******************************************************************************************
*	�� �� ��: Humi_Sensor_Failure
*	����˵��: ʪ�ȴ����� �������
*	��    �Σ���
*	�� �� ֵ: ��
*******************************************************************************************
*/
void Humi_Sensor_Failure(void)
{
	//��ʱ��û�жԹ��ϵĴ��������д���
	/* ���ڻ�Դ������������ͽ��з��ദ��
	   1��δ��
	   2���Եض̽�
	   3����VCC�̽�
	   4������ */
	__nop();
}

/*
*******************************************************************************************
*	�� �� ��: Sensor_Alarm_Check
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*******************************************************************************************
*/
u8 Sensor_Alarm_Check(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* ͨ������״̬ */
	u8 ch_alarm_sr = 0;
	
	if(sensor_t->Type != Sensor_Null)
	{
		/* �ж� ��ǰ ͨ���ı���״̬ */
		if(sensor_t->SENSOR_Alarm_State != Alarm_Null)
		{
			/* ��ǰͨ���� Ϊ����״̬ */
//			/* �ж� ��ǰ ͨ�� �����ܿ��� */
//			if(sensor_t->SENSOR_Alarm_Switch == TRUE)
//			{
				/* ��ǰ ͨ�� �����ܿ���:�� */
				/* ��ǰ ͨ�� ����ȡ����־ */
				if(sensor_f->Sensor_Suspend_Alarm_Flag == FALSE)
				{
					/* ��ǰ ͨ�� û��ȡ��������־ */
					/* ��ȡ��ǰͨ�������ⱨ������״̬ */
					ch_alarm_sr |= ((sensor_t->SENSOR_Sound_Alarm_Switch == TRUE) ? 0X1 : 0X0);
					ch_alarm_sr |= ((sensor_t->SENSOR_Light_Alarm_Switch == TRUE) ? 0X2 : 0X0);
					
				}
				/* ��ȡ��ǰͨ���Ķ��ű�������״̬ */
				ch_alarm_sr |= ((sensor_t->SENSOR_SMS_Alarm_Switch == TRUE) ? 0X4 : 0X0);
//			}
		}	
	}
	
	return ch_alarm_sr;
}

/************************************************************************************************/
/* ������	: Sensor_Fraud_Data																	*/
/* ����		: fad_sw:�������ٿ���;s_data;��ʵ����;up_val:��������;low_val:��������.					*/
/* ���		: ������																				*/
/* ����		: �������ٺ���		 									 							*/
/* ��������	: 2015/11/18																		*/
/************************************************************************************************/
static float Sensor_Fraud_Data(float s_data,float fad_up_val,float fad_low_val,float sen_up_val,float sen_low_val)
{

	/* �жϲ����ĺϷ��� */
	if(((sen_low_val <= fad_up_val)&&(sen_low_val >= fad_low_val))&&
	   ((sen_up_val <= fad_up_val)&&(sen_up_val >= fad_low_val)))
	{
		/* �����Ϸ� */
		/* �жϲ��������Ǹ���Χ�� */
		if((s_data > sen_up_val)&&(s_data <= fad_up_val))
		{
			/* ������������������ */
			/* �������������һ�����ֵ */
			return (sen_up_val - (rand() % 6)*0.1); //����0-5�������
		}else if((s_data < sen_low_val)&&(s_data >= fad_low_val))
		{
			/* ������������������ */
			/* �������������һ�����ֵ */
			return (sen_low_val + (rand() % 6)*0.1); //����0-5�������
		}else
		{
			/* �������������� */
			/* ������ʵֵ */
			return s_data;
		}
	}else
	{
		/* �������Ϸ� */
		/* ������ʵֵ */
		return s_data;
	}

}

#if (Gateway_NJSY_Enable == 0x01)
/************************************************************************************************/
/* ������	: BCD_To_HEX																*/
/* ����		: bcdCode:BCD��																					*/
/* ���		: HEX��																				*/
/* ����		: ���´���������		 									 							*/
/* ��������	: 2016/11/30																		*/
/************************************************************************************************/
static uint32_t BCD_To_HEX(uint32_t bcdCode)
{
    /* HEX��Ĵ��� */
	uint32_t hexCode = 0;
	uint8_t hexCode_1;
	uint8_t hexCode_2;
	uint8_t hexCode_3;
	uint8_t hexCode_4;
	uint8_t sCode;
	
	
	/* ��ȡ1�ֽڱ��� */
    sCode = bcdCode&0xFF;
	/* ת����һ�ֽڱ��� */
	hexCode_1 = (sCode/16*10 + sCode%16);
	/* ��ת���õı���д��Ĵ��� */
	hexCode += hexCode_1;
	
	/* ��ȡ1�ֽڱ��� */
    sCode = (bcdCode>>8)&0xFF;
	/* ת����2�ֽڱ��� */
	hexCode_2 = (sCode/16*10 + sCode%16);
	/* ��ת���õı���д��Ĵ��� */
	hexCode += hexCode_2*0x100;
	
	/* ��ȡ1�ֽڱ��� */
    sCode = (bcdCode>>16)&0xFF;
	/* ת����3�ֽڱ��� */
	hexCode_3 = (sCode/16*10 + sCode%16);
	/* ��ת���õı���д��Ĵ��� */
	hexCode += hexCode_3*0x10000;
	
	/* ��ȡ1�ֽڱ��� */
    sCode = (bcdCode>>24)&0xFF;
	/* ת����3�ֽڱ��� */
	hexCode_4 = (sCode/16*10 + sCode%16);
	/* ��ת���õı���д��Ĵ��� */
	hexCode += hexCode_4*0x1000000;
	
    return hexCode;  
}
#endif

/************************************************************************************************/
/* ������	: Sensor_Updata_Data																*/
/* ����		: 																					*/
/* ���		: ��																				*/
/* ����		: ���´���������		 									 							*/
/* ��������	: 2015/04/20																		*/
/************************************************************************************************/
void Sensor_Updata_Data	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
/* ���  */
#define Battery    		 (32u)

	
	/* ���ADC�ɼ����� */
	ADC_Trans_Data(Sensor_Store_Data_DR);
	
#if (Wireless_RF_Card_Config_Enable == 0x01)
	
	/* ���RF��Ƭ���� */
	RF_Card_Trans_Sensor_Data(Sensor_Store_Data_DR);
	
	/* ���RF��Ƭ���� */
	RF_Card_Trans_Power_Data(Sensor_Store_Power_DR);

#endif
	
	//4�����ݴ��������� ����ͨ��ֵ
/* �ж� �����������Ƿ����� */
#if (Sensor_Function_Config_Enable != 0x00)
	
/* �ж� ͨ��1 */
#if		(Sensor_CH1_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

		
#elif 	(Sensor_CH1_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1]);
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1); 
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH1_Config_Type == Sensor_PT1000)
		/* ��ȡ�м̵���״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
		
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
#endif
	
/* �ж� ͨ��2 */
#if		(Sensor_CH2_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																			  
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH2_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#elif	((Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



/* �ж� ͨ��3 */
#if		(Sensor_CH3_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#elif		(Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		//printf("Sensor_Store_Data_DR[Sensors_CH_3]=%f\r\n",Sensor_Store_Data_DR[Sensors_CH_3]);
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 	= CarbonDioxide_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		//printf("Sensor_Sampl_Data=%f\r\n",fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);

//		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
//																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
//																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
//																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
//																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
//		//fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data=345;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											
#elif 	(Sensor_CH3_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						   config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						   config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH3_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#elif	(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);	

//#elif	(Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
//		/* ��ȡ��Ƭ����״̬��Ϣ */
//		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
//		/* ��ȡ�¶����ݲ���ֵ */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= CarbonDioxide_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
//		/* ��ȡ�¶�����У׼ֵ */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
//																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
//																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		/* �������ٺ��� */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
//																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
//																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
//																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
//																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		/* ����С�������1λ����. */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#endif


/* �ж� ͨ��4 */
#if		(Sensor_CH4_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH4_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH4_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��5 */
#if		(Sensor_CH5_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH5_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH5_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��6 */
#if		(Sensor_CH6_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH6_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH6_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);	

#endif	

/* �ж� ͨ��7 */
#if		(Sensor_CH7_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH7_Config_Type == Sensor_DoorContact)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= DoorContact_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;

#elif 	(Sensor_CH7_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH7_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);	

#endif

	
/* �ж� ͨ��8 */
#if		(Sensor_CH8_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH8_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH8_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��9 */
#if		(Sensor_CH9_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH9_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH9_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��10 */
#if		(Sensor_CH10_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH10_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);															  
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);																				  
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH10_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��11 */
#if		(Sensor_CH11_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH11_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH11_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��12 */
#if		(Sensor_CH12_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH12_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH12_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);	
	 
#endif

/* �ж� ͨ��13 */
#if		(Sensor_CH13_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH13_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH13_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��14 */
#if		(Sensor_CH14_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH14_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH14_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);	
	 
#endif


/* �ж� ͨ��15 */
#if		(Sensor_CH15_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH15_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH15_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);	
 
#endif


/* �ж� ͨ��16 */
#if		(Sensor_CH16_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH16_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH16_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��17 */
#if		(Sensor_CH17_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH17_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH17_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��18 */
#if		(Sensor_CH18_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH18_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH18_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��19 */
#if		(Sensor_CH19_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH19_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH19_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��20 */
#if		(Sensor_CH20_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH20_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH20_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��21 */
#if		(Sensor_CH21_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH21_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH21_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��22 */
#if		(Sensor_CH22_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH22_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH22_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��23 */
#if		(Sensor_CH23_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH23_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH23_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	((Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#endif


/* �ж� ͨ��24 */
#if		(Sensor_CH24_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH24_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH24_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��25 */
#if		(Sensor_CH25_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH25_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH25_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��26 */
#if		(Sensor_CH26_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH26_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH26_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��27 */
#if		(Sensor_CH27_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH27_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH27_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��28 */
#if		(Sensor_CH28_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH28_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;		
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH28_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��29 */
#if		(Sensor_CH29_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH29_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																								config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH29_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��30 */
#if		(Sensor_CH30_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH30_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH30_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��31 */
#if		(Sensor_CH31_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH31_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31])
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																		  
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH31_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);	

#endif


/* �ж� ͨ��32 */
#if		(Sensor_CH32_Config_Type == Sensor_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH32_Config_Type == Sensor_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* ��ȡʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* ��ȡ���¿�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* ��ȡ���¿�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* ��ȡ��ʪ�ȿ�Ƭ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* ��ȡ��ʪ�ȿ�Ƭ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* ��ȡ��ʪ�ȿ�Ƭʪ�����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* ��ȡ��ʪ�ȿ�Ƭʪ������У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS ���� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS γ�� ��λ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* ��ȡ��������״̬��Ϣ. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡGPS �ٶ� ��Ϣ. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,1);
		/* �����GPS��Ϣ������ʱ�޷���У׼�����ֱ�ӵ��ڲ�������. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH32_Config_Type == Sensor_PT1000)
		/* ��ȡ��Ƭ����״̬��Ϣ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* ��ȡ�¶����ݲ���ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
		/* ��ȡ̽ͷ����״̬��Ϣ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State);
		/* ��ȡ̽ͷ�¶����ݲ���ֵ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data);
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
		/* ��ȡ�¶�����У׼ֵ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* �������ٺ��� */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ����С�������1λ����. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);	

#endif

	
#endif

		/* ��õ�ص���ͨ������ */
		fix_tab->Inf_State.Instru_Power.Inf.BT_Quantity_Sampl_Value 		= Sensor_Store_Data_DR[Battery];
	
#undef Battery
}

/************************************************************************************************/
/* ������	: Log_Sensor_Pgm_Ude_CH_Cfg															*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ϵͳ������������д�����ͨ�������Ƿ���Ĭ��ϵͳ������ͬ:								*/
/*			: ��ͬͨ����Ϣ��������ͬͨ����Ϣ��������ΪĬ��ϵͳ��Ϣ.									*/
/* ��������	: 2015/10/21																		*/
/************************************************************************************************/
void Log_Sensor_Pgm_Ude_CH_Cfg(void)
{
/* �������ṹ��Ϣָ��. */
#define Sen_Cha_Srt(ch) 	((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))
/* ϵͳĬ�ϵĴ�����ͨ������ */
#define Sen_Cha_Srt_Dft(ch) ((INSTRU_SENSOR_CHANNEL*)(&Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))
	

	/* �ж����ñ���ͨ��1������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_1)->Type != Sensor_CH1_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��1��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_1) = *Sen_Cha_Srt_Dft(Sensors_CH_1);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_1),sizeof(*Sen_Cha_Srt(Sensors_CH_1)));
	}

	/* �ж����ñ���ͨ��2������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_2)->Type != Sensor_CH2_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��2��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_2) = *Sen_Cha_Srt_Dft(Sensors_CH_2);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_2),sizeof(*Sen_Cha_Srt(Sensors_CH_2)));
	}

	/* �ж����ñ���ͨ��3������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_3)->Type != Sensor_CH3_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��3��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_3) = *Sen_Cha_Srt_Dft(Sensors_CH_3);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_3),sizeof(*Sen_Cha_Srt(Sensors_CH_3)));
	}

	/* �ж����ñ���ͨ��4������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_4)->Type != Sensor_CH4_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��4��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_4) = *Sen_Cha_Srt_Dft(Sensors_CH_4);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_4),sizeof(*Sen_Cha_Srt(Sensors_CH_4)));
	}

	/* �ж����ñ���ͨ��5������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_5)->Type != Sensor_CH5_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��5��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_5) = *Sen_Cha_Srt_Dft(Sensors_CH_5);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_5),sizeof(*Sen_Cha_Srt(Sensors_CH_5)));
	}

	/* �ж����ñ���ͨ��6������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_6)->Type != Sensor_CH6_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��6��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_6) = *Sen_Cha_Srt_Dft(Sensors_CH_6);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_6),sizeof(*Sen_Cha_Srt(Sensors_CH_6)));
	}

	/* �ж����ñ���ͨ��7������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_7)->Type != Sensor_CH7_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��7��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_7) = *Sen_Cha_Srt_Dft(Sensors_CH_7);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_7),sizeof(*Sen_Cha_Srt(Sensors_CH_7)));
	}

	/* �ж����ñ���ͨ��8������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_8)->Type != Sensor_CH8_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��8��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_8) = *Sen_Cha_Srt_Dft(Sensors_CH_8);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_8),sizeof(*Sen_Cha_Srt(Sensors_CH_8)));
	}

	/* �ж����ñ���ͨ��9������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_9)->Type != Sensor_CH9_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��9��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_9) = *Sen_Cha_Srt_Dft(Sensors_CH_9);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_9),sizeof(*Sen_Cha_Srt(Sensors_CH_9)));
	}

	/* �ж����ñ���ͨ��10������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_10)->Type != Sensor_CH10_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��10��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_10) = *Sen_Cha_Srt_Dft(Sensors_CH_10);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_10),sizeof(*Sen_Cha_Srt(Sensors_CH_10)));
	}

	/* �ж����ñ���ͨ��11������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_11)->Type != Sensor_CH11_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��11��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_11) = *Sen_Cha_Srt_Dft(Sensors_CH_11);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_11),sizeof(*Sen_Cha_Srt(Sensors_CH_11)));
	}

	/* �ж����ñ���ͨ��12������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_12)->Type != Sensor_CH12_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��12��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_12) = *Sen_Cha_Srt_Dft(Sensors_CH_12);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_12),sizeof(*Sen_Cha_Srt(Sensors_CH_12)));
	}

	/* �ж����ñ���ͨ��13������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_13)->Type != Sensor_CH13_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��13��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_13) = *Sen_Cha_Srt_Dft(Sensors_CH_13);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_13),sizeof(*Sen_Cha_Srt(Sensors_CH_13)));
	}

	/* �ж����ñ���ͨ��13������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_14)->Type != Sensor_CH14_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��14��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_14) = *Sen_Cha_Srt_Dft(Sensors_CH_14);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_14),sizeof(*Sen_Cha_Srt(Sensors_CH_14)));
	}

	/* �ж����ñ���ͨ��15������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_15)->Type != Sensor_CH15_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��15��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_15) = *Sen_Cha_Srt_Dft(Sensors_CH_15);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_15),sizeof(*Sen_Cha_Srt(Sensors_CH_15)));
	}

	/* �ж����ñ���ͨ��16������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_16)->Type != Sensor_CH16_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��16��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_16) = *Sen_Cha_Srt_Dft(Sensors_CH_16);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_16),sizeof(*Sen_Cha_Srt(Sensors_CH_16)));
	}

	/* �ж����ñ���ͨ��17������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_17)->Type != Sensor_CH17_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��17��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_17) = *Sen_Cha_Srt_Dft(Sensors_CH_17);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_17),sizeof(*Sen_Cha_Srt(Sensors_CH_17)));
	}

	/* �ж����ñ���ͨ��18������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_18)->Type != Sensor_CH18_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��18��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_18) = *Sen_Cha_Srt_Dft(Sensors_CH_18);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_18),sizeof(*Sen_Cha_Srt(Sensors_CH_18)));
	}

	/* �ж����ñ���ͨ��19������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_19)->Type != Sensor_CH19_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��19��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_19) = *Sen_Cha_Srt_Dft(Sensors_CH_19);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_19),sizeof(*Sen_Cha_Srt(Sensors_CH_19)));
	}

	/* �ж����ñ���ͨ��20������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_20)->Type != Sensor_CH20_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��20��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_20) = *Sen_Cha_Srt_Dft(Sensors_CH_20);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_20),sizeof(*Sen_Cha_Srt(Sensors_CH_20)));
	}

	/* �ж����ñ���ͨ��21������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_21)->Type != Sensor_CH21_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��21��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_21) = *Sen_Cha_Srt_Dft(Sensors_CH_21);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_21),sizeof(*Sen_Cha_Srt(Sensors_CH_21)));
	}

	/* �ж����ñ���ͨ��22������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_22)->Type != Sensor_CH22_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��22��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_22) = *Sen_Cha_Srt_Dft(Sensors_CH_22);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_22),sizeof(*Sen_Cha_Srt(Sensors_CH_22)));
	}

	/* �ж����ñ���ͨ��23������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_23)->Type != Sensor_CH23_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��23��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_23) = *Sen_Cha_Srt_Dft(Sensors_CH_23);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_23),sizeof(*Sen_Cha_Srt(Sensors_CH_23)));
	}

	/* �ж����ñ���ͨ��24������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_24)->Type != Sensor_CH24_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��24��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_24) = *Sen_Cha_Srt_Dft(Sensors_CH_24);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_24),sizeof(*Sen_Cha_Srt(Sensors_CH_24)));
	}

	/* �ж����ñ���ͨ��25������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_25)->Type != Sensor_CH25_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��25��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_25) = *Sen_Cha_Srt_Dft(Sensors_CH_25);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_25),sizeof(*Sen_Cha_Srt(Sensors_CH_25)));
	}

	/* �ж����ñ���ͨ��26������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_26)->Type != Sensor_CH26_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��26��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_26) = *Sen_Cha_Srt_Dft(Sensors_CH_26);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_26),sizeof(*Sen_Cha_Srt(Sensors_CH_26)));
	}

	/* �ж����ñ���ͨ��27������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_27)->Type != Sensor_CH27_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��27��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_27) = *Sen_Cha_Srt_Dft(Sensors_CH_27);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_27),sizeof(*Sen_Cha_Srt(Sensors_CH_27)));
	}

	/* �ж����ñ���ͨ��28������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_28)->Type != Sensor_CH28_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��28��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_28) = *Sen_Cha_Srt_Dft(Sensors_CH_28);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_28),sizeof(*Sen_Cha_Srt(Sensors_CH_28)));
	}

	/* �ж����ñ���ͨ��29������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_29)->Type != Sensor_CH29_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��29��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_29) = *Sen_Cha_Srt_Dft(Sensors_CH_29);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_29),sizeof(*Sen_Cha_Srt(Sensors_CH_29)));
	}

	/* �ж����ñ���ͨ��30������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_30)->Type != Sensor_CH30_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��30��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_30) = *Sen_Cha_Srt_Dft(Sensors_CH_30);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_30),sizeof(*Sen_Cha_Srt(Sensors_CH_30)));
	}	

	/* �ж����ñ���ͨ��31������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_31)->Type != Sensor_CH31_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��31��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_31) = *Sen_Cha_Srt_Dft(Sensors_CH_31);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_31),sizeof(*Sen_Cha_Srt(Sensors_CH_31)));
	}
	
	/* �ж����ñ���ͨ��32������������ϵͳ�����Ƿ���ͬ */
	if(Sen_Cha_Srt(Sensors_CH_32)->Type != Sensor_CH32_Config_Type)
	{
		/* ��ͬ�������ñ��еĴ�����ͨ��32��������Ϣ����ΪĬ������. */
		*Sen_Cha_Srt(Sensors_CH_32) = *Sen_Cha_Srt_Dft(Sensors_CH_32);
		/* �������� */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_32),sizeof(*Sen_Cha_Srt(Sensors_CH_32)));
	}



#undef Sen_Cha_Srt 
#undef Sen_Cha_Srt_Dft
}	


/******************* (C) ZHOU *****END OF FILE****/


