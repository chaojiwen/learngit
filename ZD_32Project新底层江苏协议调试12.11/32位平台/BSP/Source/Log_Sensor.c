/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/1/26																			 */
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

#include <time.h> //用到了time函数
#include <stdlib.h>
#include <math.h>


#if (Gateway_NJSY_Enable == 0x01)
#include "App_Gateway_NJSY.h"
#endif

/************************************************************************************************/
#define RoundTo(Num,Dgt) 		(((float)((int)(Num*((int)pow(10,Dgt)))))/pow(10,Dgt))

/************************************************************************************************/
/* 存储ADC数据寄存器大小 */
#define Sensor_Store_Data_DR_Size	(33u)
/* 存储ADC数据寄存器 */
static float Sensor_Store_Data_DR[Sensor_Store_Data_DR_Size];
/************************************************************************************************/
#if (Wireless_RF_Card_Config_Enable == 0x01)
/* 存储卡片电量数据寄存器大小 */
#define Sensor_Store_Power_DR_Size	(32u)
/* 存储卡片电量数据寄存器 */
static u8 	Sensor_Store_Power_DR[Sensor_Store_Power_DR_Size];
#endif
/************************************************************************************************/



/*
*********************************************************************************************************
*	函 数 名: Sensor_State_Check_Alone
*	功能说明: 根据对应通道的上下限和实时数据,对 对应通道状态进行设置.
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Sensor_State_Check_Alone(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{

	
	/* 设置通道上下限 */
	switch(sensor_t->Type)
	{


		case Sensor_Tmp:
		{
			/* 温度传感器 */
			break;
		}
	
		case Sensor_Hum:
		{
			/* 湿度传感器 */
			break;
		}
	
		case Sensor_RF_Card_Single_Tmp:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* 卡片数据接收超时不报警 */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif			
			/* 单温卡片 */
			break;
		}
		
		case Sensor_RF_Card_Humiture_Tmp:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* 卡片数据接收超时不报警 */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif		
			/* 温湿卡片->温度通道 */
			break;
		}	
		
		case Sensor_RF_Card_Humiture_Hum:
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x00)
			/* 卡片数据接收超时不报警 */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para){return;}
#endif		
			/* 温湿卡片->湿度通道 */
			break;
		}

		case Sensor_PT1000:
		{
			/* 温度传感器 */
			break;
		}
			case Sensor_GW_NJSY_Tmp:
		{
			/*南京盛亿温度传感器 */
			break;
		}
			case Sensor_GW_NJSY_Hum:
		{
			/*南京盛亿湿度却衅� */
			break;
		}
			case Sensor_CarbonDioxide:
		{
			/* 二氧化碳传感器 */
			break;
		}

		default:
		{
			/* 传感器通道类型为不可设置报警功能类型(比如:经度、纬度、速度等)或者非法类型:这些类型不参于状态设置. */
			return;
		}
	}
	
	/* 判断是否超标 */
	/* 上限 超出判断 */		
	if(sensor_f->Sensor_Real_Time_Data > sensor_t->SENSOR_Up_Limit)
	{
		/* 上限状态检测 */
		if(sensor_t->SENSOR_Alarm_State & Sensor_Alarm_Up)
		{
			__nop();
		}else 
		{
			/* 上限 状态 设置 */
			Alarm_State_Up_Set(sensor_t,sensor_f);	
		}	
		/* 下限 状态 设置 */
		Alarm_State_Low_Reset(sensor_t,sensor_f);
	/* 温度下限 超出判断 */		 
	}else if(sensor_f->Sensor_Real_Time_Data < sensor_t->SENSOR_Low_Limit)
	{
		if(sensor_t->SENSOR_Alarm_State & Sensor_Alarm_Low)
		{
			__nop();	
		}else if((sensor_f->Sensor_Real_Time_Data==-9990.0)||(sensor_f->Sensor_Real_Time_Data==-9990.4))//9990.04  zwc 2017.11.27
		{
				/* 下限 状态 设置 */
	   	Alarm_State_Low_Reset(sensor_t,sensor_f);
		}
		
		else
		{
			/* 下限 状态 设置 */
			Alarm_State_Low_Set(sensor_t,sensor_f);	
		}
	
		/* 上限 状态 设置 */
		Alarm_State_Up_Reset(sensor_t,sensor_f);
	}else
	{
		/* 上限 状态 设置 */
		Alarm_State_Up_Reset(sensor_t,sensor_f);
		/* 下限 状态 设置 */
		Alarm_State_Low_Reset(sensor_t,sensor_f);
	}

}

/************************************************************************************************/
/* 函数名  ：Sensor_State_Check_ALL														 		*/
/* 输入    ：(void *p_arg) 																		*/
/* 输出	   : 无 																				*/
/* 作用	   ：对所有传感器状态进行判断 									 		 					*/
/* 创建日期：2014/9/11																			*/
/************************************************************************************************/
void Sensor_State_Check_ALL(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* 所有传感器通道数据进行判断 */
	/* 根据对应通道的上下限和实时数据,对 对应通道状态进行设置. */

/* 判断 通道1 */
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
	
/* 判断 通道2 */
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

/* 判断 通道3 */
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

/* 判断 通道4 */
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

/* 判断 通道5 */
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

/* 判断 通道6 */
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

/* 判断 通道7 */
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
	
/* 判断 通道8 */
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

/* 判断 通道9 */
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

/* 判断 通道10 */
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

/* 判断 通道11 */
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

/* 判断 通道12 */
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

/* 判断 通道13 */
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

/* 判断 通道14 */
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

/* 判断 通道15 */
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

/* 判断 通道16 */
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

/* 判断 通道17 */
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

/* 判断 通道18 */
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

/* 判断 通道19 */
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

/* 判断 通道20 */
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

/* 判断 通道21 */
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

/* 判断 通道22 */
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

/* 判断 通道23 */
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

/* 判断 通道24 */
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

/* 判断 通道25 */
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

/* 判断 通道26 */
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

/* 判断 通道27 */
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

/* 判断 通道28 */
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

/* 判断 通道29 */
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

/* 判断 通道30 */
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

/* 判断 通道31 */
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

/* 判断 通道32 */
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
*	函 数 名: DispSensorNumConfig
*	功能说明: 传感器显示项目配置
*	形    参：无
*	返 回 值: 无
*******************************************************************************************
*/
void DispSensorNumConfig(u8 sensor_num)
{
	u8 num;

	/* 清除 显示 项目 */
	for(num = 0;num < Sensor_CH_Config_Num; num ++)
	{
		Menu_Cha_Tsd_CN[num] = (u8*)0;
		Menu_Cha_Tsd_EN[num] = (u8*)0;
	}
	
	/* 根据传感器 数量 来配显示内容 */
	if(sensor_num != 0)
	{	/* 添加 显示 项目 */
		for(num=0;num<sensor_num;num++)
		{
			/* 中文 显示 */
			Menu_Cha_Tsd_CN[num] = Cha_Tsd_CN_Tab[num];
			/* 英文 显示 */
			Menu_Cha_Tsd_EN[num] = Cha_Tsd_EN_Tab[num];
		}	
	}else
	{
			/* 中文 显示 */
			Menu_Cha_Tsd_CN[num] = Cha_NULL_CN_Tab;
			/* 英文 显示 */
			Menu_Cha_Tsd_EN[num] = Cha_NULL_EN_Tab;	
	}

} 

/*
*******************************************************************************************
*	函 数 名: Temp_Sensor_Failure
*	功能说明: 温度传感器 故障入口
*	形    参：无
*	返 回 值: 无
*******************************************************************************************
*/
void Temp_Sensor_Failure(void)
{
	//暂时还没有对故障的传感器进行处理，
	/* 后期会对传感器故障类型进行分类处理：
	   1、未接
	   2、对地短接
	   3、对VCC短接
	   4、其它 */
	__nop();
}

/*
*******************************************************************************************
*	函 数 名: Humi_Sensor_Failure
*	功能说明: 湿度传感器 故障入口
*	形    参：无
*	返 回 值: 无
*******************************************************************************************
*/
void Humi_Sensor_Failure(void)
{
	//暂时还没有对故障的传感器进行处理，
	/* 后期会对传感器故障类型进行分类处理：
	   1、未接
	   2、对地短接
	   3、对VCC短接
	   4、其它 */
	__nop();
}

/*
*******************************************************************************************
*	函 数 名: Sensor_Alarm_Check
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*******************************************************************************************
*/
u8 Sensor_Alarm_Check(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* 通道报警状态 */
	u8 ch_alarm_sr = 0;
	
	if(sensor_t->Type != Sensor_Null)
	{
		/* 判断 当前 通道的报警状态 */
		if(sensor_t->SENSOR_Alarm_State != Alarm_Null)
		{
			/* 当前通道的 为报警状态 */
//			/* 判断 当前 通道 报警总开关 */
//			if(sensor_t->SENSOR_Alarm_Switch == TRUE)
//			{
				/* 当前 通道 报警总开关:开 */
				/* 当前 通道 报警取消标志 */
				if(sensor_f->Sensor_Suspend_Alarm_Flag == FALSE)
				{
					/* 当前 通道 没有取消报警标志 */
					/* 获取单前通道的声光报警开关状态 */
					ch_alarm_sr |= ((sensor_t->SENSOR_Sound_Alarm_Switch == TRUE) ? 0X1 : 0X0);
					ch_alarm_sr |= ((sensor_t->SENSOR_Light_Alarm_Switch == TRUE) ? 0X2 : 0X0);
					
				}
				/* 获取单前通道的短信报警开关状态 */
				ch_alarm_sr |= ((sensor_t->SENSOR_SMS_Alarm_Switch == TRUE) ? 0X4 : 0X0);
//			}
		}	
	}
	
	return ch_alarm_sr;
}

/************************************************************************************************/
/* 函数名	: Sensor_Fraud_Data																	*/
/* 输入		: fad_sw:数据做假开关;s_data;真实数据;up_val:数据上限;low_val:数据下限.					*/
/* 输出		: 假数据																				*/
/* 作用		: 数据做假函数		 									 							*/
/* 创建日期	: 2015/11/18																		*/
/************************************************************************************************/
static float Sensor_Fraud_Data(float s_data,float fad_up_val,float fad_low_val,float sen_up_val,float sen_low_val)
{

	/* 判断参数的合法性 */
	if(((sen_low_val <= fad_up_val)&&(sen_low_val >= fad_low_val))&&
	   ((sen_up_val <= fad_up_val)&&(sen_up_val >= fad_low_val)))
	{
		/* 参数合法 */
		/* 判断参数的在那个范围内 */
		if((s_data > sen_up_val)&&(s_data <= fad_up_val))
		{
			/* 参数在上限作弊区域 */
			/* 根据随机函数造一个随机值 */
			return (sen_up_val - (rand() % 6)*0.1); //产生0-5的随机数
		}else if((s_data < sen_low_val)&&(s_data >= fad_low_val))
		{
			/* 参数在下限作弊区域 */
			/* 根据随机函数造一个随机值 */
			return (sen_low_val + (rand() % 6)*0.1); //产生0-5的随机数
		}else
		{
			/* 参数在其他区域 */
			/* 返回真实值 */
			return s_data;
		}
	}else
	{
		/* 参数不合法 */
		/* 返回真实值 */
		return s_data;
	}

}

#if (Gateway_NJSY_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: BCD_To_HEX																*/
/* 输入		: bcdCode:BCD码																					*/
/* 输出		: HEX码																				*/
/* 作用		: 更新传感器数据		 									 							*/
/* 创建日期	: 2016/11/30																		*/
/************************************************************************************************/
static uint32_t BCD_To_HEX(uint32_t bcdCode)
{
    /* HEX码寄存器 */
	uint32_t hexCode = 0;
	uint8_t hexCode_1;
	uint8_t hexCode_2;
	uint8_t hexCode_3;
	uint8_t hexCode_4;
	uint8_t sCode;
	
	
	/* 获取1字节编码 */
    sCode = bcdCode&0xFF;
	/* 转换第一字节编码 */
	hexCode_1 = (sCode/16*10 + sCode%16);
	/* 将转换好的编码写入寄存器 */
	hexCode += hexCode_1;
	
	/* 获取1字节编码 */
    sCode = (bcdCode>>8)&0xFF;
	/* 转换第2字节编码 */
	hexCode_2 = (sCode/16*10 + sCode%16);
	/* 将转换好的编码写入寄存器 */
	hexCode += hexCode_2*0x100;
	
	/* 获取1字节编码 */
    sCode = (bcdCode>>16)&0xFF;
	/* 转换第3字节编码 */
	hexCode_3 = (sCode/16*10 + sCode%16);
	/* 将转换好的编码写入寄存器 */
	hexCode += hexCode_3*0x10000;
	
	/* 获取1字节编码 */
    sCode = (bcdCode>>24)&0xFF;
	/* 转换第3字节编码 */
	hexCode_4 = (sCode/16*10 + sCode%16);
	/* 将转换好的编码写入寄存器 */
	hexCode += hexCode_4*0x1000000;
	
    return hexCode;  
}
#endif

/************************************************************************************************/
/* 函数名	: Sensor_Updata_Data																*/
/* 输入		: 																					*/
/* 输出		: 无																				*/
/* 作用		: 更新传感器数据		 									 							*/
/* 创建日期	: 2015/04/20																		*/
/************************************************************************************************/
void Sensor_Updata_Data	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
/* 电池  */
#define Battery    		 (32u)

	
	/* 获得ADC采集数据 */
	ADC_Trans_Data(Sensor_Store_Data_DR);
	
#if (Wireless_RF_Card_Config_Enable == 0x01)
	
	/* 获得RF卡片数据 */
	RF_Card_Trans_Sensor_Data(Sensor_Store_Data_DR);
	
	/* 获得RF卡片电量 */
	RF_Card_Trans_Power_Data(Sensor_Store_Power_DR);

#endif
	
	//4、根据传感器配置 设置通道值
/* 判断 传感器功能是否配置 */
#if (Sensor_Function_Config_Enable != 0x00)
	
/* 判断 通道1 */
#if		(Sensor_CH1_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

		
#elif 	(Sensor_CH1_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1]);
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1); 
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_1]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_1];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);
		
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH1_Config_Type == Sensor_PT1000)
		/* 获取中继电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_1]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
		
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
#endif
	
/* 判断 通道2 */
#if		(Sensor_CH2_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_2]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_2];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																			  
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
#elif 	((Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH2_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_2]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#elif	((Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data,1);																				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



/* 判断 通道3 */
#if		(Sensor_CH3_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#elif		(Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		//printf("Sensor_Store_Data_DR[Sensors_CH_3]=%f\r\n",Sensor_Store_Data_DR[Sensors_CH_3]);
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 	= CarbonDioxide_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		//printf("Sensor_Sampl_Data=%f\r\n",fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);

//		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
//																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
//																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
//																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
//																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
//		//fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data=345;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											
#elif 	(Sensor_CH3_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						   config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						   config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_3]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_3];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
#elif 	((Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH3_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#elif	(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);	

//#elif	(Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
//		/* 获取卡片电量状态信息 */
//		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
//		/* 获取温度数据采样值 */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data 		= CarbonDioxide_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_3]);
//		/* 获取温度数据校准值 */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data,
//																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value,
//																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value);
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		/* 数据做假函数 */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,
//																							fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Up_Limit,
//																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Fraud_Low_Limit,
//																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
//																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit);		
//		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		/* 保留小数点后面1位数据. */
//		fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data,1);											

#endif


/* 判断 通道4 */
#if		(Sensor_CH4_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH4_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */	
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_4]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_4];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
#elif 	((Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH4_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_4]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道5 */
#if		(Sensor_CH5_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH5_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_5]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_5];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
#elif 	((Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH5_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_5]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道6 */
#if		(Sensor_CH6_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH6_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_6]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_6];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
#elif 	((Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH6_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_6]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data,1);	

#endif	

/* 判断 通道7 */
#if		(Sensor_CH7_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH7_Config_Type == Sensor_DoorContact)
		/* 获取卡片电量状态信息 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= DoorContact_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;

#elif 	(Sensor_CH7_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_7]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_7];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
#elif 	((Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH7_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_7]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data,1);	

#endif

	
/* 判断 通道8 */
#if		(Sensor_CH8_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH8_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_8]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_8];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
#elif 	((Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH8_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_8]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道9 */
#if		(Sensor_CH9_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH9_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_9]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_9];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
#elif 	((Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH9_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_9]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道10 */
#if		(Sensor_CH10_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH10_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_10]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_10];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);															  
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);																				  
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
#elif 	((Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH10_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_10]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道11 */
#if		(Sensor_CH11_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH11_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_11]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_11];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
#elif 	((Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH11_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_11]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道12 */
#if		(Sensor_CH12_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH12_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_12]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_12];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
#elif 	((Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH12_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_12]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data,1);	
	 
#endif

/* 判断 通道13 */
#if		(Sensor_CH13_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH13_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_13]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_13];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
#elif 	((Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH13_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_13]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道14 */
#if		(Sensor_CH14_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH14_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_14]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_14];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
#elif 	((Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH14_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_14]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data,1);	
	 
#endif


/* 判断 通道15 */
#if		(Sensor_CH15_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH15_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_15]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_15];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
#elif 	((Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH15_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_15]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data,1);	
 
#endif


/* 判断 通道16 */
#if		(Sensor_CH16_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH16_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_16]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_16];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
#elif 	((Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH16_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_16]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道17 */
#if		(Sensor_CH17_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH17_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_17]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_17];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
#elif 	((Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH17_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_17]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道18 */
#if		(Sensor_CH18_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH18_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_18]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_18];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
#elif 	((Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH18_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_18]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道19 */
#if		(Sensor_CH19_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH19_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_19]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_19];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
#elif 	((Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH19_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_19]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道20 */
#if		(Sensor_CH20_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH20_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_20]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_20];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
#elif 	((Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH20_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_20]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道21 */
#if		(Sensor_CH21_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH21_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_21]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_21];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
#elif 	((Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH21_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_21]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道22 */
#if		(Sensor_CH22_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH22_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_22]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_22];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
#elif 	((Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH22_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_22]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道23 */
#if		(Sensor_CH23_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH23_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_23]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_23];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
#elif 	((Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH23_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_23]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#elif	((Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data,1);

#endif


/* 判断 通道24 */
#if		(Sensor_CH24_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH24_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_24]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_24];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
#elif 	((Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH24_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_24]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道25 */
#if		(Sensor_CH25_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH25_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_25]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_25];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
#elif 	((Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH25_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_25]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道26 */
#if		(Sensor_CH26_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH26_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_26]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_26];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
#elif 	((Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH26_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_26]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道27 */
#if		(Sensor_CH27_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH27_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_27]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_27];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
#elif 	((Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH27_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_27]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道28 */
#if		(Sensor_CH28_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH28_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;		
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_28]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_28];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
#elif 	((Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH28_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_28]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道29 */
#if		(Sensor_CH29_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH29_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																								config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_29]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_29];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
#elif 	((Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH29_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_29]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道30 */
#if		(Sensor_CH30_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH30_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30]);
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_30]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_30];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
#elif 	((Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH30_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_30]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道31 */
#if		(Sensor_CH31_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH31_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31])
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																		  
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_31]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_31];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
#elif 	((Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH31_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_31]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data,1);	

#endif


/* 判断 通道32 */
#if		(Sensor_CH32_Config_Type == Sensor_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Temp_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif 	(Sensor_CH32_Config_Type == Sensor_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Humi_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* 获取湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* 获取单温卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* 获取单温卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* 获取温湿度卡片温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* 获取温湿度卡片温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= RF_Card_Get_Power_State(Sensor_Store_Power_DR[Sensors_CH_32]);
		/* 获取温湿度卡片湿度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Sensor_Store_Data_DR[Sensors_CH_32];
		/* 获取温湿度卡片湿度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Humi_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 经度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		= GPS_App_Send_Lon_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 纬度 定位信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		=  GPS_App_Send_Lat_Data();
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
#elif 	((Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd)&&(GPS_Config_Enable == 0x01))
		/* 获取仪器电量状态信息. */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State 	= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取GPS 速度 信息. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data		=  GPS_App_Send_Spd_Data();
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,1);
		/* 如果是GPS信息现在暂时无法做校准，因此直接等于采样数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data	= fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data = RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH32_Config_Type == Sensor_PT1000)
		/* 获取卡片电量状态信息 */
		//		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= (config_tab->Inf_State.Instru_Power.Inf_State.BT_Charge_State == TRUE)?0X00:0X01;
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State		= config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		/* 获取温度数据采样值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data 		= Temp_PT1000_Get_Sampl_Value(Sensor_Store_Data_DR[Sensors_CH_32]);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);

#elif	(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
							GWProbe_Temp,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);																								
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif	(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
		/* 获取探头电量状态信息 */
		AppGW_API.ReadLevel(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
						   (GW_Level*)&fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State);
		/* 获取探头温度数据采样值 */
		AppGW_API.ReadProbe(BCD_To_HEX(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO),
							GWProbe_Hum,
						   &fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data);
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);	

#elif	((Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
		/* 获取温度数据校准值 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Temp_Get_Cal_Value(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value,
																						  config_tab->Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 数据做假函数 */
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= Sensor_Fraud_Data(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,
																							fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Up_Limit,
																						    fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Fraud_Low_Limit,
																							config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
																						    config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit);		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/* 保留小数点后面1位数据. */																		  
		fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data 	= RoundTo(fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data,1);	

#endif

	
#endif

		/* 获得电池电量通道数据 */
		fix_tab->Inf_State.Instru_Power.Inf.BT_Quantity_Sampl_Value 		= Sensor_Store_Data_DR[Battery];
	
#undef Battery
}

/************************************************************************************************/
/* 函数名	: Log_Sensor_Pgm_Ude_CH_Cfg															*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 系统服务后检测配置中传感器通道类型是否与默认系统配置相同:								*/
/*			: 相同通道信息保留，不同通道信息将被设置为默认系统信息.									*/
/* 创建日期	: 2015/10/21																		*/
/************************************************************************************************/
void Log_Sensor_Pgm_Ude_CH_Cfg(void)
{
/* 传感器结构信息指针. */
#define Sen_Cha_Srt(ch) 	((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))
/* 系统默认的传感器通道配置 */
#define Sen_Cha_Srt_Dft(ch) ((INSTRU_SENSOR_CHANNEL*)(&Default_Instru_Config_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))
	

	/* 判断配置表中通道1传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_1)->Type != Sensor_CH1_Config_Type)
	{
		/* 不同，将配置表中的传感器通道1的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_1) = *Sen_Cha_Srt_Dft(Sensors_CH_1);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_1),sizeof(*Sen_Cha_Srt(Sensors_CH_1)));
	}

	/* 判断配置表中通道2传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_2)->Type != Sensor_CH2_Config_Type)
	{
		/* 不同，将配置表中的传感器通道2的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_2) = *Sen_Cha_Srt_Dft(Sensors_CH_2);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_2),sizeof(*Sen_Cha_Srt(Sensors_CH_2)));
	}

	/* 判断配置表中通道3传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_3)->Type != Sensor_CH3_Config_Type)
	{
		/* 不同，将配置表中的传感器通道3的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_3) = *Sen_Cha_Srt_Dft(Sensors_CH_3);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_3),sizeof(*Sen_Cha_Srt(Sensors_CH_3)));
	}

	/* 判断配置表中通道4传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_4)->Type != Sensor_CH4_Config_Type)
	{
		/* 不同，将配置表中的传感器通道4的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_4) = *Sen_Cha_Srt_Dft(Sensors_CH_4);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_4),sizeof(*Sen_Cha_Srt(Sensors_CH_4)));
	}

	/* 判断配置表中通道5传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_5)->Type != Sensor_CH5_Config_Type)
	{
		/* 不同，将配置表中的传感器通道5的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_5) = *Sen_Cha_Srt_Dft(Sensors_CH_5);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_5),sizeof(*Sen_Cha_Srt(Sensors_CH_5)));
	}

	/* 判断配置表中通道6传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_6)->Type != Sensor_CH6_Config_Type)
	{
		/* 不同，将配置表中的传感器通道6的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_6) = *Sen_Cha_Srt_Dft(Sensors_CH_6);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_6),sizeof(*Sen_Cha_Srt(Sensors_CH_6)));
	}

	/* 判断配置表中通道7传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_7)->Type != Sensor_CH7_Config_Type)
	{
		/* 不同，将配置表中的传感器通道7的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_7) = *Sen_Cha_Srt_Dft(Sensors_CH_7);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_7),sizeof(*Sen_Cha_Srt(Sensors_CH_7)));
	}

	/* 判断配置表中通道8传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_8)->Type != Sensor_CH8_Config_Type)
	{
		/* 不同，将配置表中的传感器通道8的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_8) = *Sen_Cha_Srt_Dft(Sensors_CH_8);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_8),sizeof(*Sen_Cha_Srt(Sensors_CH_8)));
	}

	/* 判断配置表中通道9传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_9)->Type != Sensor_CH9_Config_Type)
	{
		/* 不同，将配置表中的传感器通道9的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_9) = *Sen_Cha_Srt_Dft(Sensors_CH_9);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_9),sizeof(*Sen_Cha_Srt(Sensors_CH_9)));
	}

	/* 判断配置表中通道10传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_10)->Type != Sensor_CH10_Config_Type)
	{
		/* 不同，将配置表中的传感器通道10的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_10) = *Sen_Cha_Srt_Dft(Sensors_CH_10);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_10),sizeof(*Sen_Cha_Srt(Sensors_CH_10)));
	}

	/* 判断配置表中通道11传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_11)->Type != Sensor_CH11_Config_Type)
	{
		/* 不同，将配置表中的传感器通道11的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_11) = *Sen_Cha_Srt_Dft(Sensors_CH_11);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_11),sizeof(*Sen_Cha_Srt(Sensors_CH_11)));
	}

	/* 判断配置表中通道12传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_12)->Type != Sensor_CH12_Config_Type)
	{
		/* 不同，将配置表中的传感器通道12的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_12) = *Sen_Cha_Srt_Dft(Sensors_CH_12);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_12),sizeof(*Sen_Cha_Srt(Sensors_CH_12)));
	}

	/* 判断配置表中通道13传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_13)->Type != Sensor_CH13_Config_Type)
	{
		/* 不同，将配置表中的传感器通道13的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_13) = *Sen_Cha_Srt_Dft(Sensors_CH_13);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_13),sizeof(*Sen_Cha_Srt(Sensors_CH_13)));
	}

	/* 判断配置表中通道13传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_14)->Type != Sensor_CH14_Config_Type)
	{
		/* 不同，将配置表中的传感器通道14的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_14) = *Sen_Cha_Srt_Dft(Sensors_CH_14);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_14),sizeof(*Sen_Cha_Srt(Sensors_CH_14)));
	}

	/* 判断配置表中通道15传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_15)->Type != Sensor_CH15_Config_Type)
	{
		/* 不同，将配置表中的传感器通道15的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_15) = *Sen_Cha_Srt_Dft(Sensors_CH_15);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_15),sizeof(*Sen_Cha_Srt(Sensors_CH_15)));
	}

	/* 判断配置表中通道16传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_16)->Type != Sensor_CH16_Config_Type)
	{
		/* 不同，将配置表中的传感器通道16的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_16) = *Sen_Cha_Srt_Dft(Sensors_CH_16);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_16),sizeof(*Sen_Cha_Srt(Sensors_CH_16)));
	}

	/* 判断配置表中通道17传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_17)->Type != Sensor_CH17_Config_Type)
	{
		/* 不同，将配置表中的传感器通道17的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_17) = *Sen_Cha_Srt_Dft(Sensors_CH_17);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_17),sizeof(*Sen_Cha_Srt(Sensors_CH_17)));
	}

	/* 判断配置表中通道18传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_18)->Type != Sensor_CH18_Config_Type)
	{
		/* 不同，将配置表中的传感器通道18的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_18) = *Sen_Cha_Srt_Dft(Sensors_CH_18);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_18),sizeof(*Sen_Cha_Srt(Sensors_CH_18)));
	}

	/* 判断配置表中通道19传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_19)->Type != Sensor_CH19_Config_Type)
	{
		/* 不同，将配置表中的传感器通道19的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_19) = *Sen_Cha_Srt_Dft(Sensors_CH_19);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_19),sizeof(*Sen_Cha_Srt(Sensors_CH_19)));
	}

	/* 判断配置表中通道20传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_20)->Type != Sensor_CH20_Config_Type)
	{
		/* 不同，将配置表中的传感器通道20的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_20) = *Sen_Cha_Srt_Dft(Sensors_CH_20);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_20),sizeof(*Sen_Cha_Srt(Sensors_CH_20)));
	}

	/* 判断配置表中通道21传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_21)->Type != Sensor_CH21_Config_Type)
	{
		/* 不同，将配置表中的传感器通道21的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_21) = *Sen_Cha_Srt_Dft(Sensors_CH_21);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_21),sizeof(*Sen_Cha_Srt(Sensors_CH_21)));
	}

	/* 判断配置表中通道22传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_22)->Type != Sensor_CH22_Config_Type)
	{
		/* 不同，将配置表中的传感器通道22的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_22) = *Sen_Cha_Srt_Dft(Sensors_CH_22);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_22),sizeof(*Sen_Cha_Srt(Sensors_CH_22)));
	}

	/* 判断配置表中通道23传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_23)->Type != Sensor_CH23_Config_Type)
	{
		/* 不同，将配置表中的传感器通道23的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_23) = *Sen_Cha_Srt_Dft(Sensors_CH_23);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_23),sizeof(*Sen_Cha_Srt(Sensors_CH_23)));
	}

	/* 判断配置表中通道24传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_24)->Type != Sensor_CH24_Config_Type)
	{
		/* 不同，将配置表中的传感器通道24的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_24) = *Sen_Cha_Srt_Dft(Sensors_CH_24);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_24),sizeof(*Sen_Cha_Srt(Sensors_CH_24)));
	}

	/* 判断配置表中通道25传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_25)->Type != Sensor_CH25_Config_Type)
	{
		/* 不同，将配置表中的传感器通道25的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_25) = *Sen_Cha_Srt_Dft(Sensors_CH_25);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_25),sizeof(*Sen_Cha_Srt(Sensors_CH_25)));
	}

	/* 判断配置表中通道26传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_26)->Type != Sensor_CH26_Config_Type)
	{
		/* 不同，将配置表中的传感器通道26的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_26) = *Sen_Cha_Srt_Dft(Sensors_CH_26);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_26),sizeof(*Sen_Cha_Srt(Sensors_CH_26)));
	}

	/* 判断配置表中通道27传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_27)->Type != Sensor_CH27_Config_Type)
	{
		/* 不同，将配置表中的传感器通道27的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_27) = *Sen_Cha_Srt_Dft(Sensors_CH_27);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_27),sizeof(*Sen_Cha_Srt(Sensors_CH_27)));
	}

	/* 判断配置表中通道28传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_28)->Type != Sensor_CH28_Config_Type)
	{
		/* 不同，将配置表中的传感器通道28的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_28) = *Sen_Cha_Srt_Dft(Sensors_CH_28);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_28),sizeof(*Sen_Cha_Srt(Sensors_CH_28)));
	}

	/* 判断配置表中通道29传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_29)->Type != Sensor_CH29_Config_Type)
	{
		/* 不同，将配置表中的传感器通道29的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_29) = *Sen_Cha_Srt_Dft(Sensors_CH_29);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_29),sizeof(*Sen_Cha_Srt(Sensors_CH_29)));
	}

	/* 判断配置表中通道30传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_30)->Type != Sensor_CH30_Config_Type)
	{
		/* 不同，将配置表中的传感器通道30的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_30) = *Sen_Cha_Srt_Dft(Sensors_CH_30);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_30),sizeof(*Sen_Cha_Srt(Sensors_CH_30)));
	}	

	/* 判断配置表中通道31传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_31)->Type != Sensor_CH31_Config_Type)
	{
		/* 不同，将配置表中的传感器通道31的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_31) = *Sen_Cha_Srt_Dft(Sensors_CH_31);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_31),sizeof(*Sen_Cha_Srt(Sensors_CH_31)));
	}
	
	/* 判断配置表中通道32传感器类型与系统配置是否相同 */
	if(Sen_Cha_Srt(Sensors_CH_32)->Type != Sensor_CH32_Config_Type)
	{
		/* 不同，将配置表中的传感器通道32的配置信息设置为默认配置. */
		*Sen_Cha_Srt(Sensors_CH_32) = *Sen_Cha_Srt_Dft(Sensors_CH_32);
		/* 保存配置 */
		Write_Config_Tab((u8*)Sen_Cha_Srt(Sensors_CH_32),sizeof(*Sen_Cha_Srt(Sensors_CH_32)));
	}



#undef Sen_Cha_Srt 
#undef Sen_Cha_Srt_Dft
}	


/******************* (C) ZHOU *****END OF FILE****/


