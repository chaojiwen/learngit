/*************************************************************************************************/
/*************************************     32Î»Æ½Ì¨²âÊÔ     **************************************/
/* ´´½¨ÈÕÆÚ£º2014/1/26																			 */
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

#include <time.h> //ÓÃµ½ÁËtimeº¯Êý
#include <stdlib.h>
#include <math.h>


#if (Gateway_NJSY_Enable == 0x01)
#include "App_Gateway_NJSY.h"
#endif

/************************************************************************************************/
#define RoundTo(Num,Dgt) 		(((float)((int)(Num*((int)pow(10,Dgt)))))/pow(10,Dgt))

/************************************************************************************************/
/* ´æ´¢ADCÊý¾Ý¼Ä´æÆ÷´óÐ¡ */
#define Sensor_Store_Data_DR_Size	(33u)
/* ´æ´¢ADCÊý¾Ý¼Ä´æÆ÷ */
static float Sensor_Store_Data_DR[Sensor_Store_Data_DR_Size];
/************************************************************************************************/
#if (Wireless_RF_Card_Config_Enable == 0x01)
/* ´æ´¢¿¨Æ¬µçÁ¿Êý¾Ý¼Ä´æÆ÷´óÐ¡ */
#define Sensor_Store_Power_DR_Size	(32u)
/* ´æ´¢¿¨Æ¬µçÁ¿Êý¾Ý¼Ä´æÆ÷ */
static u8 	Sensor_Store_Power_DR[Sensor_Store_Power_DR_Size];
#endif
/************************************************************************************************/



/*
*********************************************************************************************************
*	º¯ Êý Ãû: Sensor_State_Check_Alone
*	¹¦ÄÜËµÃ÷: ¸ù¾Ý¶ÔÓ¦Í¨µÀµÄÉÏÏÂÏÞºÍÊµÊ±Êý¾Ý,¶Ô ¶ÔÓ¦Í¨µÀ×´Ì¬½øÐÐÉèÖÃ.
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
static void Sensor_State_Check_Alone(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{

	
	/* ÉèÖÃÍ¨µÀÉÏÏÂÏÞ */
	switch(sensor_t->Type)
	{


		case Sensor_Tmp:
		{
			/* ÎÂ¶È´«¸ÐÆ÷ */
			break;
		}
	
		case Sensor_Hum:
		{
			/* Êª¶È´«¸ÐÆ÷ */
			break;
		}
	
		case Sensor_RF_Card_Single_Tmp:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* ¿¨Æ¬Êý¾Ý½ÓÊÕ³¬Ê±²»±¨¾¯ */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif			
			/* µ¥ÎÂ¿¨Æ¬ */
			break;
		}
		
		case Sensor_RF_Card_Humiture_Tmp:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* ¿¨Æ¬Êý¾Ý½ÓÊÕ³¬Ê±²»±¨¾¯ */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif		
			/* ÎÂÊª¿¨Æ¬->ÎÂ¶ÈÍ¨µÀ */
			break;
		}	
		
		case Sensor_RF_Card_Humiture_Hum:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* ¿¨Æ¬Êý¾Ý½ÓÊÕ³¬Ê±²»±¨¾¯ */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif		
			/* ÎÂÊª¿¨Æ¬->Êª¶ÈÍ¨µÀ */
			break;
		}

		case Sensor_PT1000:
		{
			/* ÎÂ¶È´«¸ÐÆ÷ */
			break;
		}
			case Sensor_GW_NJSY_Tmp:
		{
			/*ÄÏ¾©Ê¢ÒÚÎÂ¶È´«¸ÐÆ÷ */
			break;
		}
			case Sensor_GW_NJSY_Hum:
		{
			/*ÄÏ¾©Ê¢ÒÚÊª¶ÈÈ´«¸ÐÆ÷ */
			break;
		}
			case Sensor_CarbonDioxide:
		{
			/* ¶þÑõ»¯Ì¼´«¸ÐÆ÷ */
			break;
		}

		default:
		{
			/* ´«¸ÐÆ÷Í¨µÀÀàÐÍÎª²»¿ÉÉèÖÃ±¨¾¯¹¦ÄÜÀàÐÍ(±ÈÈç:¾­¶È¡¢Î³¶È¡¢ËÙ¶ÈµÈ)»òÕß·Ç·¨ÀàÐÍ:ÕâÐ©ÀàÐÍ²»²ÎÓÚ×´Ì¬ÉèÖÃ. */
			return;
		}
	}
	
	/* ÅÐ¶ÏÊÇ·ñ³¬±ê */
	/* ÉÏÏÞ ³¬³öÅÐ¶Ï */		
	if(sensor_f->Sensor_Real_Time_Data > sensor_t->SENSOR_Up_Limit)
	{
		/* ÉÏÏÞ×´Ì¬¼ì²â */
		if(sensor_t->SENSOR_Alarm_State & Sensor_Alarm_Up)
		{
			__nop();
		}else 
		{
			/* ÉÏÏÞ ×´Ì¬ ÉèÖÃ */
			Alarm_State_Up_Set(sensor_t,sensor_f);	
		}	
		/* ÏÂÏÞ ×´Ì¬ ÉèÖÃ */
		Alarm_State_Low_Reset(sensor_t,sensor_f);
	/* ÎÂ¶ÈÏÂÏÞ ³¬³öÅÐ¶Ï */		 
	}else if(sensor_f->Sensor_Real_Time_Data < sensor_t->SENSOR_Low_Limit)
	{
		if(sensor_t->SENSOR_Alarm_State & Sensor_Alarm_Low)
		{
			__nop();	
		}else if((sensor_f->Sensor_Real_Time_Data==-9990.0)||(sensor_f->Sensor_Real_Time_Data==-9990.4))//9990.04  zwc 2017.11.27
		{
				/* ÏÂÏÞ ×´Ì¬ ÉèÖÃ */
	   	Alarm_State_Low_Reset(sensor_t,sensor_f);
		}
		
		else
		{
			/* ÏÂÏÞ ×´Ì¬ ÉèÖÃ */
			Alarm_State_Low_Set(sensor_t,sensor_f);	
		}
	
		/* ÉÏÏÞ ×´Ì¬ ÉèÖÃ */
		Alarm_State_Up_Reset(sensor_t,sensor_f);
	}else
	{
		/* ÉÏÏÞ ×´Ì¬ ÉèÖÃ */
		Alarm_State_Up_Reset(sensor_t,sensor_f);
		/* ÏÂÏÞ ×´Ì¬ ÉèÖÃ */
		Alarm_State_Low_Reset(sensor_t,sensor_f);
	}

}

/************************************************************************************************/
/* º¯ÊýÃû  £ºSensor_State_Check_ALL														 		*/
/* ÊäÈë    £º(void *p_arg) 																		*/
/* Êä³ö	   : ÎÞ 																				*/
/* ×÷ÓÃ	   £º¶ÔËùÓÐ´«¸ÐÆ÷×´Ì¬½øÐÐÅÐ¶Ï 									 		 					*/
/* ´´½¨ÈÕÆÚ£º2014/9/11																			*/
/************************************************************************************************/
void Sensor_State_Check_ALL(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* ËùÓÐ´«¸ÐÆ÷Í¨µÀÊý¾Ý½øÐÐÅÐ¶Ï */
	/* ¸ù¾Ý¶ÔÓ¦Í¨µÀµÄÉÏÏÂÏÞºÍÊµÊ±Êý¾Ý,¶Ô ¶ÔÓ¦Í¨µÀ×´Ì¬½øÐÐÉèÖÃ. */

/* ÅÐ¶Ï Í¨µÀ1 */
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
	
/* ÅÐ¶Ï Í¨µÀ2 */
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

/* ÅÐ¶Ï Í¨µÀ3 */
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

/* ÅÐ¶Ï Í¨µÀ4 */
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

/* ÅÐ¶Ï Í¨µÀ5 */
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

/* ÅÐ¶Ï Í¨µÀ6 */
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

/* ÅÐ¶Ï Í¨µÀ7 */
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
	
/* ÅÐ¶Ï Í¨µÀ8 */
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

/* ÅÐ¶Ï Í¨µÀ9 */
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

/* ÅÐ¶Ï Í¨µÀ10 */
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

/* ÅÐ¶Ï Í¨µÀ11 */
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

/* ÅÐ¶Ï Í¨µÀ12 */
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

/* ÅÐ¶Ï Í¨µÀ13 */
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

/* ÅÐ¶Ï Í¨µÀ14 */
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

/* ÅÐ¶Ï Í¨µÀ15 */
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

/* ÅÐ¶Ï Í¨µÀ16 */
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

/* ÅÐ¶Ï Í¨µÀ17 */
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

/* ÅÐ¶Ï Í¨µÀ18 */
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

/* ÅÐ¶Ï Í¨µÀ19 */
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

/* ÅÐ¶Ï Í¨µÀ20 */
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

/* ÅÐ¶Ï Í¨µÀ21 */
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

/* ÅÐ¶Ï Í¨µÀ22 */
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

/* ÅÐ¶Ï Í¨µÀ23 */
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

/* ÅÐ¶Ï Í¨µÀ24 */
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

/* ÅÐ¶Ï Í¨µÀ25 */
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

/* ÅÐ¶Ï Í¨µÀ26 */
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

/* ÅÐ¶Ï Í¨µÀ27 */
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

/* ÅÐ¶Ï Í¨µÀ28 */
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

/* ÅÐ¶Ï Í¨µÀ29 */
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

/* ÅÐ¶Ï Í¨µÀ30 */
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

/* ÅÐ¶Ï Í¨µÀ31 */
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

/* ÅÐ¶Ï Í¨µÀ32 */
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
*	º¯ Êý Ãû: DispSensorNumConfig
*	¹¦ÄÜËµÃ÷: ´«¸ÐÆ÷ÏÔÊ¾ÏîÄ¿ÅäÖÃ
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*******************************************************************************************
*/
void DispSensorNumConfig(u8 sensor_num)
{
	u8 num;

	/* Çå³ý ÏÔÊ¾ ÏîÄ¿ */
	for(num = 0;num < Sensor_CH_Config_Num; num ++)
	{
		Menu_Cha_Tsd_CN[num] = (u8*)0;
		Menu_Cha_Tsd_EN[num] = (u8*)0;
	}
	
	/* ¸ù¾Ý´«¸ÐÆ÷ ÊýÁ¿ À´ÅäÏÔÊ¾ÄÚÈÝ */
	if(sensor_num != 0)
	{	/* Ìí¼Ó ÏÔÊ¾ ÏîÄ¿ */
		for(num=0;num<sensor_num;num++)
		{
			/* ÖÐÎÄ ÏÔÊ¾ */
			Menu_Cha_Tsd_CN[num] = Cha_Tsd_CN_Tab[num];
			/* Ó¢ÎÄ ÏÔÊ¾ */
			Menu_Cha_Tsd_EN[num] = Cha_Tsd_EN_Tab[num];
		}	
	}else
	{
			/* ÖÐÎÄ ÏÔÊ¾ */
			Menu_Cha_Tsd_CN[num] = Cha_NULL_CN_Tab;
			/* Ó¢ÎÄ ÏÔÊ¾ */
			Menu_Cha_Tsd_EN[num] = Cha_NULL_EN_Tab;	
	}

} 

/*
*******************************************************************************************
*	º¯ Êý Ãû: Temp_Sensor_Failure
*	¹¦ÄÜËµÃ÷: ÎÂ¶È´«¸ÐÆ÷ ¹ÊÕÏÈë¿Ú
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*******************************************************************************************
*/
void Temp_Sensor_Failure(void)
{
	//ÔÝÊ±»¹Ã»ÓÐ¶Ô¹ÊÕÏµÄ´«¸ÐÆ÷½øÐÐ´¦Àí£¬
	/* ºóÆÚ»á¶Ô´«¸ÐÆ÷¹ÊÕÏÀàÐÍ½øÐÐ·ÖÀà´¦Àí£º
	   1¡¢Î´½Ó
	   2¡¢¶ÔµØ¶Ì½Ó
	   3¡¢¶ÔVCC¶Ì½Ó
	   4¡¢ÆäËü */
	__nop();
}

/*
*******************************************************************************************
*	º¯ Êý Ãû: Humi_Sensor_Failure
*	¹¦ÄÜËµÃ÷: Êª¶È´«¸ÐÆ÷ ¹ÊÕÏÈë¿Ú
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*******************************************************************************************
*/
void Humi_Sensor_Failure(void)
{
	//ÔÝÊ±»¹Ã»ÓÐ¶Ô¹ÊÕÏµÄ´«¸ÐÆ÷½øÐÐ´¦Àí£¬
	/* ºóÆÚ»á¶Ô´«¸ÐÆ÷¹ÊÕÏÀàÐÍ½øÐÐ·ÖÀà´¦Àí£º
	   1¡¢Î´½Ó
	   2¡¢¶ÔµØ¶Ì½Ó
	   3¡¢¶ÔVCC¶Ì½Ó
	   4¡¢ÆäËü */
	__nop();
}

/*
*******************************************************************************************
*	º¯ Êý Ãû: Sensor_Alarm_Check
*	¹¦ÄÜËµÃ÷: 
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*******************************************************************************************
*/
u8 Sensor_Alarm_Check(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* Í¨µÀ±¨¾¯×´Ì¬ */
	u8 ch_alarm_sr = 0;
	
	if(sensor_t->Type != Sensor_Null)
	{
		/* ÅÐ¶Ï µ±Ç° Í¨µÀµÄ±¨¾¯×´Ì¬ */
		if(sensor_t->SENSOR_Alarm_State != Alarm_Null)
		{
			/* µ±Ç°Í¨µÀµÄ Îª±¨¾¯×´Ì¬ */
//			/* ÅÐ¶Ï µ±Ç° Í¨µÀ ±¨¾¯×Ü¿ª¹Ø */
//			if(sensor_t->SENSOR_Alarm_Switch == TRUE)
//			{
				/* µ±Ç° Í¨µÀ ±¨¾¯×Ü¿ª¹Ø:¿ª */
				/* µ±Ç° Í¨µÀ ±¨¾¯È¡Ïû±êÖ¾ */
				if(sensor_f->Sensor_Suspend_Alarm_Flag == FALSE)
				{
					/* µ±Ç° Í¨µÀ Ã»ÓÐÈ¡Ïû±¨¾¯±êÖ¾ */
					/* »ñÈ¡µ¥Ç°Í¨µÀµÄÉù¹â±¨¾¯¿ª¹Ø×´Ì¬ */
					ch_alarm_sr |= ((sensor_t->SENSOR_Sound_Alarm_Switch == TRUE) ? 0X1 : 0X0);
					ch_alarm_sr |= ((sensor_t->SENSOR_Light_Alarm_Switch == TRUE) ? 0X2 : 0X0);
					
				}
				/* »ñÈ¡µ¥Ç°Í¨µÀµÄ¶ÌÐÅ±¨¾¯¿ª¹Ø×´Ì¬ */
				ch_alarm_sr |= ((sensor_t->SENSOR_SMS_Alarm_Switch == TRUE) ? 0X4 : 0X0);
//			}
		}	
	}
	
	return ch_alarm_sr;
}

/************************************************************************************************/
/* º¯ÊýÃû	: Sensor_Fraud_Data																	*/
/* ÊäÈë		: fad_sw:Êý¾Ý×ö¼Ù¿ª¹Ø;s_data;ÕæÊµÊý¾Ý;up_val:Êý¾ÝÉÏÏÞ;low_val:Êý¾ÝÏÂÏÞ.					*/
/* Êä³ö		: ¼ÙÊý¾Ý																				*/
/* ×÷ÓÃ		: Êý¾Ý×ö¼Ùº¯Êý		 									 							*/
/* ´´½¨ÈÕÆÚ	: 2015/11/18																		*/
/************************************************************************************************/
static float Sensor_Fraud_Data(float s_data,float fad_up_val,float fad_low_val,float sen_up_val,float sen_low_val)
{

	/* ÅÐ¶Ï²ÎÊýµÄºÏ·¨ÐÔ */
	if(((sen_low_val <= fad_up_val)&&(sen_low_val >= fad_low_val))&&
	   ((sen_up_val <= fad_up_val)&&(sen_up_val >= fad_low_val)))
	{
		/* ²ÎÊýºÏ·¨ */
		/* ÅÐ¶Ï²ÎÊýµÄÔÚÄÇ¸ö·¶Î§ÄÚ */
		if((s_data > sen_up_val)&&(s_data <= fad_up_val))
		{
			/* ²ÎÊýÔÚÉÏÏÞ×÷±×ÇøÓò */
			/* ¸ù¾ÝËæ»úº¯ÊýÔìÒ»¸öËæ»úÖµ */
			return (sen_up_val - (rand() % 6)*0.1); //²úÉú0-5µÄËæ»úÊý
		}else if((s_data < sen_low_val)&&(s_data >= fad_low_val))
		{
			/* ²ÎÊýÔÚÏÂÏÞ×÷±×ÇøÓò */
			/* ¸ù¾ÝËæ»úº¯ÊýÔìÒ»¸öËæ»úÖµ */
			return (sen_low_val + (rand() % 6)*0.1); //²úÉú0-5µÄËæ»úÊý
		}else
		{
			/* ²ÎÊýÔÚÆäËûÇøÓò */
			/* ·µ»ØÕæÊµÖµ */
			return s_data;
		}
	}else
	{
		/* ²ÎÊý²»ºÏ·¨ */
		/* ·µ»ØÕæÊµÖµ */
		return s_data;
	}

}

#if (Gateway_NJSY_Enable == 0x01)
/************************************************************************************************/
/* º¯ÊýÃû	: BCD_To_HEX																*/
/* ÊäÈë		: bcdCode:BCDÂë																					*/
/* Êä³ö		: HEXÂë																				*/
/* ×÷ÓÃ		: ¸üÐÂ´«¸ÐÆ÷Êý¾Ý		 									 							*/
/* ´´½¨ÈÕÆÚ	: 2016/11/30																		*/
/************************************************************************************************/
static uint32_t BCD_To_HEX(uint32_t bcdCode)
{
    /* HEXÂë¼Ä´æÆ÷ */
	uint32_t hexCode = 0;
	uint8_t hexCode_1;
	uint8_t hexCode_2;
	uint8_t hexCode_3;
	uint8_t hexCode_4;
	uint8_t sCode;
	
	
	/* »ñÈ¡1×Ö½Ú±àÂë */
    sCode = bcdCode&0xFF;
	/* ×ª»»µÚÒ»×Ö½Ú±àÂë */
	hexCode_1 = (sCode/16*10 + sCode%16);
	/* ½«×ª»»ºÃµÄ±àÂëÐ´Èë¼Ä´æÆ÷ */
	hexCode += hexCode_1;
	
	/* »ñÈ¡1×Ö½Ú±àÂë */
    sCode = (bcdCode>>8)&0xFF;
	/* ×ª»»µÚ2×Ö½Ú±àÂë */
	hexCode_2 = (sCode/16*10 + sCode%16);
	/* ½«×ª»»ºÃµÄ±àÂëÐ´Èë¼Ä´æÆ÷ */
	hexCode += hexCode_2*0x100;
	
	/* »ñÈ¡1×Ö½Ú±àÂë */
    sCode = (bcdCode>>16)&0xFF;
	/* ×ª»»µÚ3×Ö½Ú±àÂë */
	hexCode_3 = (sCode/16*10 + sCode%16);
	/* ½«×ª»»ºÃµÄ±àÂëÐ´Èë¼Ä´æÆ÷ */
	hexCode += hexCode_3*0x10000;
	
	/* »ñÈ¡1×Ö½Ú±àÂë */
    sCode = (bcdCode>>24)&0xFF;
	/* ×ª»»µÚ3×Ö½Ú±àÂë */
	hexCode_4 = (sCode/16*10 + sCode%16);
	/* ½«×ª»»ºÃµÄ±àÂëÐ´Èë¼Ä´æÆ÷ */
	hexCode += hexCode_4*0x1000000;
	
    return hexCode;  
}
#endif

/************************************************************************************************/
/* º¯ÊýÃû	: Sensor_Updata_Data																*/
/* ÊäÈë		: 																					*/
/* Êä³ö		: ÎÞ																				*/
/* ×÷ÓÃ		: ¸üÐÂ´«¸ÐÆ÷Êý¾Ý		 									 							*/
/* ´´½¨ÈÕÆÚ	: 2015/04/20																		*/
/************************************************************************************************/
void Sensor_Updata_Data	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
/* µç³Ø  */
#define Battery    		 (32u)

	
	/* »ñµÃADC²É¼¯Êý¾Ý */
	ADC_Trans_Data(Sensor_Store_Data_DR);
	
#if (Wireless_RF_Card_Config_Enable == 0x01)
	
	/* »ñµÃRF¿¨Æ¬Êý¾Ý */
	RF_Card_Trans_Sensor_Data(Sensor_Store_Data_DR);
	
	/* »ñµÃRF¿¨Æ¬µçÁ¿ */
	RF_Card_Trans_Power_Data(Sensor_Store_Power_DR);

#endif
	
	//4¡¢¸ù¾Ý´«¸ÐÆ÷ÅäÖÃ ÉèÖÃÍ¨µÀÖµ
/* ÅÐ¶Ï ´«¸ÐÆ÷¹¦ÄÜÊÇ·ñÅäÖÃ */
#if (Sensor_Function_Config_Enable != 0x00)
	
/* ÅÐ¶Ï Í¨µÀ1 */
#if		(Sensor_CH1_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

		
#elif 	(Sensor_CH1_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1); 
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH1_Config_Type == Sensor_PT1000)
		/* »ñÈ¡ÖÐ¼ÌµçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
		
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
#endif
	
/* ÅÐ¶Ï Í¨µÀ2 */
#if		(Sensor_CH2_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																			  
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH2_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#elif	((Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



/* ÅÐ¶Ï Í¨µÀ3 */
#if		(Sensor_CH3_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#elif		(Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		//printf("Sensor_Store_Data_DR[Sensors_CH_3]=%f\r\n",Sensor_Store_Data_DR[Sensors_CH_3]);
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 	= CarbonDioxide_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		//printf("Sensor_Sampl_Data=%f\r\n",fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);

//		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
//																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
//																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
//																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
//																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
//		//fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data=345;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											
#elif 	(Sensor_CH3_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						   config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						   config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH3_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#elif	(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);	

//#elif	(Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
//		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
//		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
//		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= CarbonDioxide_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
//		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
//																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
//																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		/* Êý¾Ý×ö¼Ùº¯Êý */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
//																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
//																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
//																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
//																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#endif


/* ÅÐ¶Ï Í¨µÀ4 */
#if		(Sensor_CH4_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH4_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH4_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ5 */
#if		(Sensor_CH5_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH5_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH5_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ6 */
#if		(Sensor_CH6_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH6_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH6_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);	

#endif	

/* ÅÐ¶Ï Í¨µÀ7 */
#if		(Sensor_CH7_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH7_Config_Type == Sensor_DoorContact)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= DoorContact_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;

#elif 	(Sensor_CH7_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH7_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);	

#endif

	
/* ÅÐ¶Ï Í¨µÀ8 */
#if		(Sensor_CH8_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH8_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH8_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ9 */
#if		(Sensor_CH9_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH9_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH9_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ10 */
#if		(Sensor_CH10_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH10_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);															  
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);																				  
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH10_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ11 */
#if		(Sensor_CH11_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH11_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH11_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ12 */
#if		(Sensor_CH12_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH12_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH12_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);	
	 
#endif

/* ÅÐ¶Ï Í¨µÀ13 */
#if		(Sensor_CH13_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH13_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH13_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ14 */
#if		(Sensor_CH14_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH14_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH14_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);	
	 
#endif


/* ÅÐ¶Ï Í¨µÀ15 */
#if		(Sensor_CH15_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH15_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH15_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);	
 
#endif


/* ÅÐ¶Ï Í¨µÀ16 */
#if		(Sensor_CH16_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH16_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH16_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ17 */
#if		(Sensor_CH17_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH17_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH17_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ18 */
#if		(Sensor_CH18_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH18_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH18_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ19 */
#if		(Sensor_CH19_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH19_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH19_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ20 */
#if		(Sensor_CH20_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH20_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH20_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ21 */
#if		(Sensor_CH21_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH21_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH21_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ22 */
#if		(Sensor_CH22_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH22_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH22_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ23 */
#if		(Sensor_CH23_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH23_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH23_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	((Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#endif


/* ÅÐ¶Ï Í¨µÀ24 */
#if		(Sensor_CH24_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH24_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH24_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ25 */
#if		(Sensor_CH25_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH25_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH25_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ26 */
#if		(Sensor_CH26_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH26_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH26_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ27 */
#if		(Sensor_CH27_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH27_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH27_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ28 */
#if		(Sensor_CH28_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH28_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;		
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH28_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ29 */
#if		(Sensor_CH29_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH29_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																								config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH29_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ30 */
#if		(Sensor_CH30_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH30_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH30_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ31 */
#if		(Sensor_CH31_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH31_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31])
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																		  
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH31_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);	

#endif


/* ÅÐ¶Ï Í¨µÀ32 */
#if		(Sensor_CH32_Config_Type == Sensor_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH32_Config_Type == Sensor_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* »ñÈ¡Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* »ñÈ¡µ¥ÎÂ¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* »ñÈ¡ÎÂÊª¶È¿¨Æ¬Êª¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ¾­¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS Î³¶È ¶¨Î»ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* »ñÈ¡ÒÇÆ÷µçÁ¿×´Ì¬ÐÅÏ¢. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡GPS ËÙ¶È ÐÅÏ¢. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,1);
		/* Èç¹ûÊÇGPSÐÅÏ¢ÏÖÔÚÔÝÊ±ÎÞ·¨×öÐ£×¼£¬Òò´ËÖ±½ÓµÈÓÚ²ÉÑùÊý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH32_Config_Type == Sensor_PT1000)
		/* »ñÈ¡¿¨Æ¬µçÁ¿×´Ì¬ÐÅÏ¢ */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* »ñÈ¡ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
		/* »ñÈ¡Ì½Í·µçÁ¿×´Ì¬ÐÅÏ¢ */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State);
		/* »ñÈ¡Ì½Í·ÎÂ¶ÈÊý¾Ý²ÉÑùÖµ */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data);
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
		/* »ñÈ¡ÎÂ¶ÈÊý¾ÝÐ£×¼Öµ */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* Êý¾Ý×ö¼Ùº¯Êý */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* ±£ÁôÐ¡ÊýµãºóÃæ1Î»Êý¾Ý. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);	

#endif

	
#endif

		/* »ñµÃµç³ØµçÁ¿Í¨µÀÊý¾Ý */
		fix_tab->Inf_State.Instru_Power.Inf.BT_Quantity_Sampl_Value 		= Sensor_Store_Data_DR[Battery];
	
#undef Battery
}

/************************************************************************************************/
/* º¯ÊýÃû	: Log_Sensor_Pgm_Ude_CH_Cfg															*/
/* ÊäÈë		: ÎÞ																				*/
/* Êä³ö		: ÎÞ 																				*/
/* ×÷ÓÃ		: ÏµÍ³·þÎñºó¼ì²âÅäÖÃÖÐ´«¸ÐÆ÷Í¨µÀÀàÐÍÊÇ·ñÓëÄ¬ÈÏÏµÍ³ÅäÖÃÏàÍ¬:								*/
/*			: ÏàÍ¬Í¨µÀÐÅÏ¢±£Áô£¬²»Í¬Í¨µÀÐÅÏ¢½«±»ÉèÖÃÎªÄ¬ÈÏÏµÍ³ÐÅÏ¢.									*/
/* ´´½¨ÈÕÆÚ	: 2015/10/21																		*/
/************************************************************************************************/
void Log_Sensor_Pgm_Ude_CH_Cfg(void)
{
/* ´«¸ÐÆ÷½á¹¹ÐÅÏ¢Ö¸Õë. */
#define Sen_Cha_Srt(ch) 	((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))
/* ÏµÍ³Ä¬ÈÏµÄ´«¸ÐÆ÷Í¨µÀÅäÖÃ */
#define Sen_Cha_Srt_Dft(ch) ((INSTRU_SENSOR_CHANNEL*)(&Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))
	

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ1´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_1)->Type != Sensor_CH1_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ1µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_1) = *Sen_Cha_Srt_Dft(Sensors_CH_1);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_1),sizeof(*Sen_Cha_Srt(Sensors_CH_1)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ2´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_2)->Type != Sensor_CH2_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ2µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_2) = *Sen_Cha_Srt_Dft(Sensors_CH_2);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_2),sizeof(*Sen_Cha_Srt(Sensors_CH_2)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ3´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_3)->Type != Sensor_CH3_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ3µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_3) = *Sen_Cha_Srt_Dft(Sensors_CH_3);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_3),sizeof(*Sen_Cha_Srt(Sensors_CH_3)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ4´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_4)->Type != Sensor_CH4_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ4µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_4) = *Sen_Cha_Srt_Dft(Sensors_CH_4);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_4),sizeof(*Sen_Cha_Srt(Sensors_CH_4)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ5´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_5)->Type != Sensor_CH5_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ5µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_5) = *Sen_Cha_Srt_Dft(Sensors_CH_5);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_5),sizeof(*Sen_Cha_Srt(Sensors_CH_5)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ6´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_6)->Type != Sensor_CH6_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ6µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_6) = *Sen_Cha_Srt_Dft(Sensors_CH_6);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_6),sizeof(*Sen_Cha_Srt(Sensors_CH_6)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ7´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_7)->Type != Sensor_CH7_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ7µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_7) = *Sen_Cha_Srt_Dft(Sensors_CH_7);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_7),sizeof(*Sen_Cha_Srt(Sensors_CH_7)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ8´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_8)->Type != Sensor_CH8_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ8µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_8) = *Sen_Cha_Srt_Dft(Sensors_CH_8);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_8),sizeof(*Sen_Cha_Srt(Sensors_CH_8)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ9´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_9)->Type != Sensor_CH9_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ9µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_9) = *Sen_Cha_Srt_Dft(Sensors_CH_9);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_9),sizeof(*Sen_Cha_Srt(Sensors_CH_9)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ10´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_10)->Type != Sensor_CH10_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ10µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_10) = *Sen_Cha_Srt_Dft(Sensors_CH_10);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_10),sizeof(*Sen_Cha_Srt(Sensors_CH_10)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ11´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_11)->Type != Sensor_CH11_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ11µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_11) = *Sen_Cha_Srt_Dft(Sensors_CH_11);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_11),sizeof(*Sen_Cha_Srt(Sensors_CH_11)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ12´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_12)->Type != Sensor_CH12_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ12µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_12) = *Sen_Cha_Srt_Dft(Sensors_CH_12);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_12),sizeof(*Sen_Cha_Srt(Sensors_CH_12)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ13´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_13)->Type != Sensor_CH13_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ13µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_13) = *Sen_Cha_Srt_Dft(Sensors_CH_13);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_13),sizeof(*Sen_Cha_Srt(Sensors_CH_13)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ13´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_14)->Type != Sensor_CH14_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ14µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_14) = *Sen_Cha_Srt_Dft(Sensors_CH_14);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_14),sizeof(*Sen_Cha_Srt(Sensors_CH_14)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ15´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_15)->Type != Sensor_CH15_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ15µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_15) = *Sen_Cha_Srt_Dft(Sensors_CH_15);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_15),sizeof(*Sen_Cha_Srt(Sensors_CH_15)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ16´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_16)->Type != Sensor_CH16_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ16µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_16) = *Sen_Cha_Srt_Dft(Sensors_CH_16);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_16),sizeof(*Sen_Cha_Srt(Sensors_CH_16)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ17´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_17)->Type != Sensor_CH17_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ17µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_17) = *Sen_Cha_Srt_Dft(Sensors_CH_17);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_17),sizeof(*Sen_Cha_Srt(Sensors_CH_17)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ18´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_18)->Type != Sensor_CH18_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ18µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_18) = *Sen_Cha_Srt_Dft(Sensors_CH_18);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_18),sizeof(*Sen_Cha_Srt(Sensors_CH_18)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ19´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_19)->Type != Sensor_CH19_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ19µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_19) = *Sen_Cha_Srt_Dft(Sensors_CH_19);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_19),sizeof(*Sen_Cha_Srt(Sensors_CH_19)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ20´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_20)->Type != Sensor_CH20_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ20µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_20) = *Sen_Cha_Srt_Dft(Sensors_CH_20);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_20),sizeof(*Sen_Cha_Srt(Sensors_CH_20)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ21´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_21)->Type != Sensor_CH21_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ21µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_21) = *Sen_Cha_Srt_Dft(Sensors_CH_21);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_21),sizeof(*Sen_Cha_Srt(Sensors_CH_21)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ22´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_22)->Type != Sensor_CH22_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ22µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_22) = *Sen_Cha_Srt_Dft(Sensors_CH_22);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_22),sizeof(*Sen_Cha_Srt(Sensors_CH_22)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ23´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_23)->Type != Sensor_CH23_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ23µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_23) = *Sen_Cha_Srt_Dft(Sensors_CH_23);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_23),sizeof(*Sen_Cha_Srt(Sensors_CH_23)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ24´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_24)->Type != Sensor_CH24_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ24µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_24) = *Sen_Cha_Srt_Dft(Sensors_CH_24);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_24),sizeof(*Sen_Cha_Srt(Sensors_CH_24)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ25´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_25)->Type != Sensor_CH25_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ25µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_25) = *Sen_Cha_Srt_Dft(Sensors_CH_25);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_25),sizeof(*Sen_Cha_Srt(Sensors_CH_25)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ26´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_26)->Type != Sensor_CH26_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ26µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_26) = *Sen_Cha_Srt_Dft(Sensors_CH_26);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_26),sizeof(*Sen_Cha_Srt(Sensors_CH_26)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ27´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_27)->Type != Sensor_CH27_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ27µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_27) = *Sen_Cha_Srt_Dft(Sensors_CH_27);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_27),sizeof(*Sen_Cha_Srt(Sensors_CH_27)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ28´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_28)->Type != Sensor_CH28_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ28µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_28) = *Sen_Cha_Srt_Dft(Sensors_CH_28);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_28),sizeof(*Sen_Cha_Srt(Sensors_CH_28)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ29´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_29)->Type != Sensor_CH29_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ29µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_29) = *Sen_Cha_Srt_Dft(Sensors_CH_29);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_29),sizeof(*Sen_Cha_Srt(Sensors_CH_29)));
	}

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ30´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_30)->Type != Sensor_CH30_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ30µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_30) = *Sen_Cha_Srt_Dft(Sensors_CH_30);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_30),sizeof(*Sen_Cha_Srt(Sensors_CH_30)));
	}	

	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ31´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_31)->Type != Sensor_CH31_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ31µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_31) = *Sen_Cha_Srt_Dft(Sensors_CH_31);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_31),sizeof(*Sen_Cha_Srt(Sensors_CH_31)));
	}
	
	/* ÅÐ¶ÏÅäÖÃ±íÖÐÍ¨µÀ32´«¸ÐÆ÷ÀàÐÍÓëÏµÍ³ÅäÖÃÊÇ·ñÏàÍ¬ */
	if(Sen_Cha_Srt(Sensors_CH_32)->Type != Sensor_CH32_Config_Type)
	{
		/* ²»Í¬£¬½«ÅäÖÃ±íÖÐµÄ´«¸ÐÆ÷Í¨µÀ32µÄÅäÖÃÐÅÏ¢ÉèÖÃÎªÄ¬ÈÏÅäÖÃ. */
		*Sen_Cha_Srt(Sensors_CH_32) = *Sen_Cha_Srt_Dft(Sensors_CH_32);
		/* ±£´æÅäÖÃ */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_32),sizeof(*Sen_Cha_Srt(Sensors_CH_32)));
	}



#undef Sen_Cha_Srt 
#undef Sen_Cha_Srt_Dft
}	


/******************* (C) ZHOU *****END OF FILE****/


