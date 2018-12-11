/**
  ******************************************************************************
  * @file              : Log_SMS.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015年3月19日 15:09:47
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
  */
#include "System_Config.h"

#include "Log_SMS.h"

#include "Log_Alarm.h"
#include "Log_GSM.h"
#include "Log_RF_Card.h"
#include "Log_Sensor.h"


#include <string.h>
#include <stdio.h>

#include "Unicode.c"
#include "app.h"

/* 由于发送短信是用的Unicode码，而正常编译器使用的是GBK码，
   因此编译器无法Unicode码，会产生很多不必要的警告。
   添加 “#pragma diag_suppress 870”可以取消这些警告。   */
#pragma diag_suppress 870

///////////////* 短信报警时间 */
//////////////#define SMS_Alarm_Time_String			"报警时间:%04d年%02d月%02d日,%02d时%02d分%02d秒;\r\n"
///////////////* 仪器号 */
////////////////#define SMS_SN_NO_String				"仪器号:%X%X%X%X%05d;\r\n"
//////////////#define SMS_SN_NO_String				"仪器号:%04X%05d;\r\n"
///////////////* 温度传感器参数显示 */
//////////////#define SMS_Sensor_Temp_Para_String		"当前数据:%c%d.%01d℃(上限值:%c%d.%01d℃,下限值:%c%d.%01d℃);\r\n"
///////////////* 湿度传感器参数显示 */
//////////////#define SMS_Sensor_Humi_Para_String		"当前数据:%d.%01d%%RH(上限值:%d.%01d%%RH,下限值:%d.%01d%%RH);\r\n"
///////////////* 卡片温度传感器接收超时参数显示 */
//////////////#define SMS_RF_Card_Data_Rece_Timeout_Temp_Para_String	"当前数据:无数据(上限值:%c%d.%01d℃,下限值:%c%d.%01d℃);\r\n"
///////////////* 卡片湿度传感器接收超时参数显示 */
//////////////#define SMS_RF_Card_Data_Rece_Timeout_Humi_Para_String	"当前数据:无数据(上限值:%d.%01d%%RH,下限值:%d.%01d%%RH);\r\n"
///////////////* 传感器通道号 */
//////////////#define SMS_Sensor_CH_NO_String			"传感器编号:%01d;\r\n"
///////////////* 传感器通道类型 */
//////////////#define SMS_Sensor_CH_Type_String		"通道%01d类型:%s;\r\n"
///////////////* 温度传感器 */
//////////////#define SMS_Sensor_Temp_String			"温度传感器"
///////////////* 湿度传感器 */
//////////////#define SMS_Sensor_Humi_String			"湿度传感器"


///////////////* 南京盛亿传感器温度 */
//////////////#define SMS_ShengYi_Sensor_Temp_String			"盛亿温度传感器"

///////////////* 南京盛亿传感器湿度 */
//////////////#define SMS_ShengYi_Sensor_Humi_String			"盛亿湿度传感器"



///////////////* 单温卡片传感器 */
//////////////#define SMS_Sensor_RF_Card_Single_Temp_String	"单温卡片传感器"
///////////////* 温湿度卡片温度传感器 */
//////////////#define SMS_Sensor_RF_Card_Humiture_Temp_String	"温湿度卡片温度传感器"
///////////////* 温湿度卡片湿度传感器 */
//////////////#define SMS_Sensor_RF_Card_Humiture_Humi_String	"温湿度卡片湿度传感器"
///////////////* 传感器报警状态 */
//////////////#define SMS_Sensor_Alarm_State_String	"报警状态:%s;\r\n"
///////////////* 传感器报警 */
//////////////#define SMS_Sensor_Alarm_String			"报警"
///////////////* 传感器未报警 */
//////////////#define SMS_Sensor_Normal_String		"未报警"
///////////////* 传感器报警类型 */
//////////////#define SMS_Sensor_Alarm_Type_String	"通道%01d报警类型:%s;\r\n"
///////////////* 传感器上限报警 */
//////////////#define SMS_Sensor_Alarm_Up_String		"上限报警"
///////////////* 传感器下限报警 */
//////////////#define SMS_Sensor_Alarm_Down_String	"下限报警"
///////////////* 卡片传感器接收超时报警类型 */
//////////////#define SMS_RF_Card_Data_Rece_Timeout_Alarm_String	"报警类型:数据接收超时;\r\n"
///////////////* 掉电报警信息 */
//////////////#define SMS_Power_Alarm_Type_String		"报警事件:电源未接."
///////////////* 电池电量低报警信息 */
//////////////#define SMS_Battery_Alarm_Type_String	"报警事件:电池电量低."







/* 等待发送的短信内容数据 */
char SMS_UTF8[SMS_UTF8_Data_Buffer_Size];

static void SMS_Set_CH_Inf		(u8 CH_num,char* sms_string,INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f);
static void SMS_Set_Power_Inf	(char* sms_string,INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
static void SMS_Set_Battery_Inf	(char* sms_string,INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);
	
static void SMS_Set_Sensor_Alarm_Inf	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,u8 alarm_ch_no);
static void SMS_Set_Power_Alarm_Inf		(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
static void SMS_Set_Battery_Alarm_Inf	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);

/**********************************************************************************************************/

/* 短信重复报警时间间隔 数据指针 */
static u32 *SMS_Repeat_Alarm_Interval_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval;
/* 实时数据存储开关 数据大小 */
#define SMS_Repeat_Alarm_Interval_Data_Size (sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval))




/*
*********************************************************************************************************
*	函 数 名: SMS_Set_Power_Inf
*	功能说明: 根据电源的报警状态信息设置短信内容
*	形    参：UTF8_p:UTF-8编码存储空间指针；String_p:字符串数据指针；字符串长度
*	返 回 值: 无
*********************************************************************************************************
*/
static void SMS_Set_Power_Inf(char* sms_string,INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* 设置电源报警信息 */
	sprintf(&sms_string[strlen(sms_string)],SMS_Power_Alarm_Type_String);
}

/*
*********************************************************************************************************
*	函 数 名: SMS_Set_Battery_Inf
*	功能说明: 根据电池电量的报警状态信息设置短信内容
*	形    参：UTF8_p:UTF-8编码存储空间指针；String_p:字符串数据指针；字符串长度
*	返 回 值: 无
*********************************************************************************************************
*/
static void SMS_Set_Battery_Inf(char* sms_string,INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* 设置电源报警信息 */
	sprintf(&sms_string[strlen(sms_string)],SMS_Battery_Alarm_Type_String);
}

/************************************************************************************************/
/* 函数名	: SMS_Set_CH_Inf															 	 	*/
/* 输入  	: sms_string:字符信息；sensor_t:传感器配置表信息；sensor_f:传感器配置表信息				*/
/* 输出		: 无 																				*/
/* 作用	 	: 根据每个通道的配置信息个状态设置短信内容						 						*/
/* 创建日期	: 2015/04/20																		*/
/************************************************************************************************/
static void SMS_Set_CH_Inf(u8 CH_num,char* sms_string,INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* 判断传感器 通道一 是否配置过 */
	if(sensor_t->Type != 0)
	{
		/* 通道一 为传感器通道 */
		/* 设置通道类型信息 */
		/* 传感器通道号 *///这里通道编号 加1 的原因是通道1的值为0，保证输出的结果与设置的一致。
		sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_NO_String,(CH_num+1));
		/* 传感器通道类型 */
		if(sensor_t->Type == Sensor_Tmp)
		{
			/* 板载温度传感器 */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Temp_String);
		}else if(sensor_t->Type == Sensor_Hum)
		{
			/* 板载湿度传感器 */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Humi_String);
		}else if(sensor_t->Type == Sensor_RF_Card_Single_Tmp)
		{
			/* 单温卡片传感器 */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_RF_Card_Single_Temp_String);
		}else if(sensor_t->Type == Sensor_RF_Card_Humiture_Tmp)
		{
			/* 温湿度卡片温度传感器 */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_RF_Card_Humiture_Temp_String);		
		}else if(sensor_t->Type == Sensor_RF_Card_Humiture_Hum)
		{
			/* 温湿度卡片湿度传感器 */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_RF_Card_Humiture_Humi_String);	
		}else if(sensor_t->Type == Sensor_PT1000)
		{
			/* PT1000温度传感器 */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Temp_String);
		}else if(sensor_t->Type == Sensor_GW_NJSY_Tmp	)
		{
			/* 南京盛亿网关温度探头类型 */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Temp_String);//温度传感器
		}else if(sensor_t->Type == Sensor_GW_NJSY_Hum	)
		{
			/* 南京盛亿网关湿度探头类型 */
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_CH_Type_String,(CH_num+1),SMS_Sensor_Humi_String);//盛亿湿度传感器
		}
//		/* 传感器报警状态 */
//		sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Alarm_State_String,
//		(sensor_t->SENSOR_Alarm_State == Alarm_Null) ? SMS_Sensor_Normal_String : SMS_Sensor_Alarm_String);
		/* 传感器报警类型 */
		if(sensor_t->SENSOR_Alarm_State != Alarm_Null)
		{
			/* 设置通道报警信息 */
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x01)
			/* 启动 无线RF卡片数据接收超时报警 */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_RF_Card_Data_Rece_Timeout_Alarm_String);
			}else			
#endif
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Alarm_Type_String,(CH_num+1),
				(sensor_t->SENSOR_Alarm_State == Alarm_Up) ? SMS_Sensor_Alarm_Up_String : SMS_Sensor_Alarm_Down_String);
			}

		}
		/* 温度传感器参数显示 */
		if(sensor_t->Type == Sensor_Tmp)
		{
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
			((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
			(((u16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
			((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Up_Limit),
			((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
			((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Low_Limit),
			((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
		}else if(sensor_t->Type == Sensor_Hum)/* 湿度传感器参数显示 */
		{
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Humi_Para_String,
			(((u16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
			((u16) sensor_t->SENSOR_Up_Limit),
			((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
			((u16) sensor_t->SENSOR_Low_Limit),
			((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
		}
		else if(sensor_t->Type == Sensor_RF_Card_Single_Tmp)/* 单温卡片参数显示 */
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x01)
			/* 启动 无线RF卡片数据接收超时报警 */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_RF_Card_Data_Rece_Timeout_Temp_Para_String,
				((sensor_f->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((sensor_f->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));	
			}else			
#endif
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
				((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
				(((u16)sensor_f->Sensor_Real_Time_Data)),
				((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
				((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
			}
		}
		else if(sensor_t->Type == Sensor_RF_Card_Humiture_Tmp)/* 温湿卡片 的温度数据 参数显示 */
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x01)
			/* 启动 无线RF卡片数据接收超时报警 */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_RF_Card_Data_Rece_Timeout_Temp_Para_String,
				((sensor_f->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((sensor_f->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));	
			}else			
#endif
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
				((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
				(((u16)sensor_f->Sensor_Real_Time_Data)),
				((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
				((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
				((u16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));		
			}
			
		}
		else if(sensor_t->Type == Sensor_RF_Card_Humiture_Hum)/* 温湿卡片 的湿度数据 参数显示 */
		{
#if	(Wireless_RF_Card_Data_Rece_Timeout_Alarm_Set == 0x01)
			/* 启动 无线RF卡片数据接收超时报警 */
			if(sensor_f->Sensor_Real_Time_Data == Wireless_RF_Card_Data_Rece_Timeout_Para)
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_RF_Card_Data_Rece_Timeout_Humi_Para_String,
				((s16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((s16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));	
			}else			
#endif
			{
				sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Humi_Para_String,
			(((s16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
				((s16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((s16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));		
			}
		}else if(sensor_t->Type == Sensor_PT1000)/* PT1000温度传感器参数显示 */
		{
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
			((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
			(((u16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
			((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Up_Limit),
			((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
			((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Low_Limit),
			((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
			
			
			
		}else if(sensor_t->Type == Sensor_GW_NJSY_Tmp)/* 南京盛亿网关温度探头类型 */
		{	
			
			
		
			
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Temp_Para_String,
			((sensor_f->Sensor_Real_Time_Data >= 0.0) ? '+':'-'),
			(((u16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
			((sensor_t->SENSOR_Up_Limit >= 0.0) ? '+':'-'),
			((u16) sensor_t->SENSOR_Up_Limit),
			((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
			((sensor_t->SENSOR_Low_Limit >= 0.0) ? '+':'-'),
			((u16)sensor_t->SENSOR_Low_Limit),
			((u16)(sensor_t->SENSOR_Low_Limit*10)%10));
			
			
		
			

			
		}else if(sensor_t->Type == Sensor_GW_NJSY_Hum)/* 南京盛亿网关湿度探头类型 */
		{
			sprintf(&sms_string[strlen(sms_string)],SMS_Sensor_Humi_Para_String,
			(((s16)sensor_f->Sensor_Real_Time_Data)),
			((u16)(sensor_f->Sensor_Real_Time_Data*10)%10),
				((s16) sensor_t->SENSOR_Up_Limit),
				((u16)(sensor_t->SENSOR_Up_Limit*10)%10),
				((s16) sensor_t->SENSOR_Low_Limit),
				((u16)(sensor_t->SENSOR_Low_Limit*10)%10));		
		}

	}
	
}

/*
*********************************************************************************************************
*	函 数 名: SMS_Set_Sensor_Alarm_Inf
*	功能说明: 设置传感器报警短信内容
*	形    参：config_tab:配置信息表;fix_tab:仪器变量表;
*	返 回 值: 无
*********************************************************************************************************
*/
static void SMS_Set_Sensor_Alarm_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,u8 alarm_ch_no)
{
	
	/* 清除寄存器内容 */
	memset(SMS_UTF8,0X00,sizeof(SMS_UTF8));
	
	/* 设置发送短信内容 */
	/* 获取当前报警时间点的日期时间数据 */
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_Alarm_Time_String,
	config_tab->Inf_State.Instru_RTC.Inf_State.Year,
	config_tab->Inf_State.Instru_RTC.Inf_State.Month,
	config_tab->Inf_State.Instru_RTC.Inf_State.Day,
	config_tab->Inf_State.Instru_RTC.Inf_State.Hour,
	config_tab->Inf_State.Instru_RTC.Inf_State.Min,
	config_tab->Inf_State.Instru_RTC.Inf_State.Sec
	);
	
	switch(alarm_ch_no)
	{	
#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_Hum)||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000))
		case Sensors_CH_1:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_1,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH1,&fix_tab->Inf_State.Instru_Sensor.Inf.CH1);	
			break;
		}
#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
		case Sensors_CH_1:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_1,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH1,&fix_tab->Inf_State.Instru_Sensor.Inf.CH1);	
			break;
		}
		
#endif
		
#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum)||(Sensor_CH2_Config_Type == Sensor_PT1000))
		case Sensors_CH_2:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_2,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH2,&fix_tab->Inf_State.Instru_Sensor.Inf.CH2);	
			break;
		}
#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
		case Sensors_CH_2:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_2,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH2,&fix_tab->Inf_State.Instru_Sensor.Inf.CH2);
			break;
		}
#endif	
		
#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum)||(Sensor_CH3_Config_Type == Sensor_PT1000))
		case Sensors_CH_3:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_3,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH3,&fix_tab->Inf_State.Instru_Sensor.Inf.CH3);	
			break;
		}
#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_3:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_3,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH3,&fix_tab->Inf_State.Instru_Sensor.Inf.CH3);
			break;
		}
#endif			
		
#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||(Sensor_CH4_Config_Type == Sensor_Hum)||(Sensor_CH4_Config_Type == Sensor_PT1000))
		case Sensors_CH_4:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_4,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH4,&fix_tab->Inf_State.Instru_Sensor.Inf.CH4);	
			break;
		}
#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
				
		case Sensors_CH_4:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_4,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH4,&fix_tab->Inf_State.Instru_Sensor.Inf.CH4);
			break;
		}
#endif		
		
#if ((Sensor_CH5_Config_Type == Sensor_Tmp)||(Sensor_CH5_Config_Type == Sensor_Hum)||(Sensor_CH5_Config_Type == Sensor_PT1000))
		case Sensors_CH_5:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_5,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH5,&fix_tab->Inf_State.Instru_Sensor.Inf.CH5);	
			break;
		}
#elif ((Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_5:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_5,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH5,&fix_tab->Inf_State.Instru_Sensor.Inf.CH5);
			break;
		}
#endif	
		
#if ((Sensor_CH6_Config_Type == Sensor_Tmp)||(Sensor_CH6_Config_Type == Sensor_Hum)||(Sensor_CH6_Config_Type == Sensor_PT1000))
		case Sensors_CH_6:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_6,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH6,&fix_tab->Inf_State.Instru_Sensor.Inf.CH6);	
			break;
		}
#elif ((Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_6:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_6,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH6,&fix_tab->Inf_State.Instru_Sensor.Inf.CH6);
			break;
		}
#endif	
		
#if ((Sensor_CH7_Config_Type == Sensor_Tmp)||(Sensor_CH7_Config_Type == Sensor_Hum)||(Sensor_CH7_Config_Type == Sensor_PT1000)|| (Sensor_CH7_Config_Type == Sensor_DoorContact))
		case Sensors_CH_7:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_7,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH7,&fix_tab->Inf_State.Instru_Sensor.Inf.CH7);	
			break;
		}
#elif ((Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
				
		case Sensors_CH_7:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_7,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH7,&fix_tab->Inf_State.Instru_Sensor.Inf.CH7);	
			break;
		}
#endif	
		
#if ((Sensor_CH8_Config_Type == Sensor_Tmp)||(Sensor_CH8_Config_Type == Sensor_Hum)||(Sensor_CH8_Config_Type == Sensor_PT1000))
		case Sensors_CH_8:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_8,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH8,&fix_tab->Inf_State.Instru_Sensor.Inf.CH8);	
			break;
		}
#elif ((Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_8:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_8,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH8,&fix_tab->Inf_State.Instru_Sensor.Inf.CH8);
			break;
		}
#endif	

#if ((Sensor_CH9_Config_Type == Sensor_Tmp)||(Sensor_CH9_Config_Type == Sensor_Hum)||(Sensor_CH9_Config_Type == Sensor_PT1000))
		case Sensors_CH_9:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_9,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH9,&fix_tab->Inf_State.Instru_Sensor.Inf.CH9);	
			break;
		}
#elif ((Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_9:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_9,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH9,&fix_tab->Inf_State.Instru_Sensor.Inf.CH9);
			break;
		}
#endif		
		
#if ((Sensor_CH10_Config_Type == Sensor_Tmp)||(Sensor_CH10_Config_Type == Sensor_Hum)||(Sensor_CH10_Config_Type == Sensor_PT1000))
		case Sensors_CH_10:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_10,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH10,&fix_tab->Inf_State.Instru_Sensor.Inf.CH10);	
			break;
		}
#elif ((Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_10:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_10,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH10,&fix_tab->Inf_State.Instru_Sensor.Inf.CH10);
			break;
		}
#endif	
		
#if ((Sensor_CH11_Config_Type == Sensor_Tmp)||(Sensor_CH11_Config_Type == Sensor_Hum)||(Sensor_CH11_Config_Type == Sensor_PT1000))
		case Sensors_CH_11:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_11,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH11,&fix_tab->Inf_State.Instru_Sensor.Inf.CH11);	
			break;
		}
#elif ((Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_11:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_11,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH11,&fix_tab->Inf_State.Instru_Sensor.Inf.CH11);
			break;
		}
#endif	
		
#if ((Sensor_CH12_Config_Type == Sensor_Tmp)||(Sensor_CH12_Config_Type == Sensor_Hum)||(Sensor_CH12_Config_Type == Sensor_PT1000))
		case Sensors_CH_12:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_12,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH12,&fix_tab->Inf_State.Instru_Sensor.Inf.CH12);	
			break;
		}
#elif ((Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_12:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_12,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH12,&fix_tab->Inf_State.Instru_Sensor.Inf.CH12);
			break;
		}
#endif		
		
#if ((Sensor_CH13_Config_Type == Sensor_Tmp)||(Sensor_CH13_Config_Type == Sensor_Hum)||(Sensor_CH13_Config_Type == Sensor_PT1000))
		case Sensors_CH_13:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_13,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH13,&fix_tab->Inf_State.Instru_Sensor.Inf.CH13);	
			break;
		}
#elif ((Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_13:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_13,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH13,&fix_tab->Inf_State.Instru_Sensor.Inf.CH13);
			break;
		}
#endif		
		
#if ((Sensor_CH14_Config_Type == Sensor_Tmp)||(Sensor_CH14_Config_Type == Sensor_Hum)||(Sensor_CH14_Config_Type == Sensor_PT1000))
		case Sensors_CH_14:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_14,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH14,&fix_tab->Inf_State.Instru_Sensor.Inf.CH14);	
			break;
		}
#elif ((Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_14:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_14,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH14,&fix_tab->Inf_State.Instru_Sensor.Inf.CH14);
			break;
		}
#endif	
		
#if ((Sensor_CH15_Config_Type == Sensor_Tmp)||(Sensor_CH15_Config_Type == Sensor_Hum)||(Sensor_CH15_Config_Type == Sensor_PT1000))
		case Sensors_CH_15:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_15,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH15,&fix_tab->Inf_State.Instru_Sensor.Inf.CH15);	
			break;
		}
#elif ((Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_15:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_15,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH15,&fix_tab->Inf_State.Instru_Sensor.Inf.CH15);
			break;
		}
#endif	
		
#if ((Sensor_CH16_Config_Type == Sensor_Tmp)||(Sensor_CH16_Config_Type == Sensor_Hum)||(Sensor_CH16_Config_Type == Sensor_PT1000))
		case Sensors_CH_16:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_16,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH16,&fix_tab->Inf_State.Instru_Sensor.Inf.CH16);	
			break;
		}
#elif ((Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_16:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_16,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH16,&fix_tab->Inf_State.Instru_Sensor.Inf.CH16);
			break;
		}
#endif
		
#if ((Sensor_CH17_Config_Type == Sensor_Tmp)||(Sensor_CH17_Config_Type == Sensor_Hum)||(Sensor_CH17_Config_Type == Sensor_PT1000))
		case Sensors_CH_17:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_17,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH17,&fix_tab->Inf_State.Instru_Sensor.Inf.CH17);	
			break;
		}
#elif ((Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_17:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_17,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH17,&fix_tab->Inf_State.Instru_Sensor.Inf.CH17);
			break;
		}
#endif		
		
#if ((Sensor_CH18_Config_Type == Sensor_Tmp)||(Sensor_CH18_Config_Type == Sensor_Hum)||(Sensor_CH18_Config_Type == Sensor_PT1000))
		case Sensors_CH_18:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_18,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH18,&fix_tab->Inf_State.Instru_Sensor.Inf.CH18);	
			break;
		}
#elif ((Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_18:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_18,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH18,&fix_tab->Inf_State.Instru_Sensor.Inf.CH18);
			break;
		}
#endif	
		
#if ((Sensor_CH19_Config_Type == Sensor_Tmp)||(Sensor_CH19_Config_Type == Sensor_Hum)||(Sensor_CH19_Config_Type == Sensor_PT1000))
		case Sensors_CH_19:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_19,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH19,&fix_tab->Inf_State.Instru_Sensor.Inf.CH19);	
			break;
		}
#elif ((Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
		 	
		case Sensors_CH_19:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_19,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH19,&fix_tab->Inf_State.Instru_Sensor.Inf.CH19);
			break;
		}
#endif	
		
#if ((Sensor_CH20_Config_Type == Sensor_Tmp)||(Sensor_CH20_Config_Type == Sensor_Hum)||(Sensor_CH20_Config_Type == Sensor_PT1000))
		case Sensors_CH_20:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_20,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH20,&fix_tab->Inf_State.Instru_Sensor.Inf.CH20);	
			break;
		}
#elif ((Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_20:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_20,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH20,&fix_tab->Inf_State.Instru_Sensor.Inf.CH20);
			break;
		}
#endif		
		
#if ((Sensor_CH21_Config_Type == Sensor_Tmp)||(Sensor_CH21_Config_Type == Sensor_Hum)||(Sensor_CH21_Config_Type == Sensor_PT1000))
		case Sensors_CH_21:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_21,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH21,&fix_tab->Inf_State.Instru_Sensor.Inf.CH21);	
			break;
		}
#elif ((Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_21:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_21,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH21,&fix_tab->Inf_State.Instru_Sensor.Inf.CH21);
			break;
		}
#endif		
		
#if ((Sensor_CH22_Config_Type == Sensor_Tmp)||(Sensor_CH22_Config_Type == Sensor_Hum)||(Sensor_CH22_Config_Type == Sensor_PT1000))
		case Sensors_CH_22:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_22,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH22,&fix_tab->Inf_State.Instru_Sensor.Inf.CH22);	
			break;
		}
#elif ((Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_22:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_22,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH22,&fix_tab->Inf_State.Instru_Sensor.Inf.CH22);
			break;
		}
#endif	
		
#if ((Sensor_CH23_Config_Type == Sensor_Tmp)||(Sensor_CH23_Config_Type == Sensor_Hum)||(Sensor_CH23_Config_Type == Sensor_PT1000))
		case Sensors_CH_23:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_23,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH23,&fix_tab->Inf_State.Instru_Sensor.Inf.CH23);	
			break;
		}
#elif ((Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_23:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_23,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH23,&fix_tab->Inf_State.Instru_Sensor.Inf.CH23);
			break;
		}
#endif	
		
#if ((Sensor_CH24_Config_Type == Sensor_Tmp)||(Sensor_CH24_Config_Type == Sensor_Hum)||(Sensor_CH24_Config_Type == Sensor_PT1000))
		case Sensors_CH_24:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_24,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH24,&fix_tab->Inf_State.Instru_Sensor.Inf.CH24);	
			break;
		}
#elif ((Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_24:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_24,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH24,&fix_tab->Inf_State.Instru_Sensor.Inf.CH24);
			break;
		}
#endif		
		
#if ((Sensor_CH25_Config_Type == Sensor_Tmp)||(Sensor_CH25_Config_Type == Sensor_Hum)||(Sensor_CH25_Config_Type == Sensor_PT1000))
		case Sensors_CH_25:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_25,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH25,&fix_tab->Inf_State.Instru_Sensor.Inf.CH25);	
			break;
		}
#elif ((Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_25:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_25,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH25,&fix_tab->Inf_State.Instru_Sensor.Inf.CH25);
			break;
		}
#endif		
		
#if ((Sensor_CH26_Config_Type == Sensor_Tmp)||(Sensor_CH26_Config_Type == Sensor_Hum)||(Sensor_CH26_Config_Type == Sensor_PT1000))
		case Sensors_CH_26:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_26,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH26,&fix_tab->Inf_State.Instru_Sensor.Inf.CH26);	
			break;
		}
#elif ((Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_26:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_26,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH26,&fix_tab->Inf_State.Instru_Sensor.Inf.CH26);
			break;
		}
#endif	
		
#if ((Sensor_CH27_Config_Type == Sensor_Tmp)||(Sensor_CH27_Config_Type == Sensor_Hum)||(Sensor_CH27_Config_Type == Sensor_PT1000))
		case Sensors_CH_27:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_27,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH27,&fix_tab->Inf_State.Instru_Sensor.Inf.CH27);	
			break;
		}
#elif ((Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_27:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_27,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH27,&fix_tab->Inf_State.Instru_Sensor.Inf.CH27);
			break;
		}
#endif	
		
#if ((Sensor_CH28_Config_Type == Sensor_Tmp)||(Sensor_CH28_Config_Type == Sensor_Hum)||(Sensor_CH28_Config_Type == Sensor_PT1000))
		case Sensors_CH_28:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_28,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH28,&fix_tab->Inf_State.Instru_Sensor.Inf.CH28);	
			break;
		}
#elif ((Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_28:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_28,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH28,&fix_tab->Inf_State.Instru_Sensor.Inf.CH28);
			break;
		}
#endif		
		
#if ((Sensor_CH29_Config_Type == Sensor_Tmp)||(Sensor_CH29_Config_Type == Sensor_Hum)||(Sensor_CH29_Config_Type == Sensor_PT1000))
		case Sensors_CH_29:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_29,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH29,&fix_tab->Inf_State.Instru_Sensor.Inf.CH29);	
			break;
		}
#elif ((Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_29:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_29,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH29,&fix_tab->Inf_State.Instru_Sensor.Inf.CH29);
			break;
		}
#endif		
		
#if ((Sensor_CH30_Config_Type == Sensor_Tmp)||(Sensor_CH30_Config_Type == Sensor_Hum)||(Sensor_CH30_Config_Type == Sensor_PT1000))
		case Sensors_CH_30:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_30,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH30,&fix_tab->Inf_State.Instru_Sensor.Inf.CH30);	
			break;
		}
#elif ((Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_30:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_30,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH30,&fix_tab->Inf_State.Instru_Sensor.Inf.CH30);
			break;
		}
#endif	
		
#if ((Sensor_CH31_Config_Type == Sensor_Tmp)||(Sensor_CH31_Config_Type == Sensor_Hum)||(Sensor_CH31_Config_Type == Sensor_PT1000))
		case Sensors_CH_31:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_31,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH31,&fix_tab->Inf_State.Instru_Sensor.Inf.CH31);	
			break;
		}
#elif ((Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
		
		case Sensors_CH_31:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_31,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH31,&fix_tab->Inf_State.Instru_Sensor.Inf.CH31);
			break;
		}
#endif	
		
#if ((Sensor_CH32_Config_Type == Sensor_Tmp)||(Sensor_CH32_Config_Type == Sensor_Hum)||(Sensor_CH32_Config_Type == Sensor_PT1000))
		case Sensors_CH_32:
		{	
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
			((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
			((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_32,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH32,&fix_tab->Inf_State.Instru_Sensor.Inf.CH32);	
			break;
		}
#elif ((Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
			
		case Sensors_CH_32:
		{
			/* 获取获取仪器号 */
			sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[0]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[1]),
			((u16)(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[2]<<8)+
			 (u16)config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[3]));
			/* 设置短信报警内容 */
			SMS_Set_CH_Inf(Sensors_CH_32,SMS_UTF8,&config_tab->Inf_State.Instru_Sensor.Inf_State.CH32,&fix_tab->Inf_State.Instru_Sensor.Inf.CH32);
			break;
		}
#endif		
		
		
		default:
		{

			break;
		}
	}
	
}

/*
*********************************************************************************************************
*	函 数 名: SMS_Send_Sensor_Inf
*	功能说明: 发送传感器报警短信报警
*	形    参：State；
*	返 回 值: 无
*********************************************************************************************************
*/
BOOL SMS_Send_Sensor_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,u8 alarm_ch_no)
{
	/* 短信消息发送状态 */
	BOOL sms_state;
	
	
	/* 发送短信 */
	sms_state = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Send_SMS_EVENT);
	/* 判断短信发送队列状态 *///注意：这里的状态任务的优先级要比GSM任务高，否则发送完成队列后就执行GSM任务了，短信内容没有被设置。
	if(sms_state == TRUE)
	{
		/* 发送成功 */
		/* 设置发送短信内容 */
		SMS_Set_Sensor_Alarm_Inf(config_tab,fix_tab,alarm_ch_no);
		/* 设置短信发送状态 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_State = TRUE;
	}
	/* 返回 短信信息 发送情况 */
	return sms_state;
}

/*
*********************************************************************************************************
*	函 数 名: SMS_Set_Power_Alarm_Inf
*	功能说明: 设置掉电报警短信内容
*	形    参：config_tab:配置信息表;fix_tab:仪器变量表;
*	返 回 值: 无
*********************************************************************************************************
*/
static void SMS_Set_Power_Alarm_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	
	/* 清除寄存器内容 */
	memset(SMS_UTF8,0X00,sizeof(SMS_UTF8));
	
	/* 设置发送短信内容 */
	/* 获取当前报警时间点的日期时间数据 */
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_Alarm_Time_String,
	config_tab->Inf_State.Instru_RTC.Inf_State.Year,
	config_tab->Inf_State.Instru_RTC.Inf_State.Month,
	config_tab->Inf_State.Instru_RTC.Inf_State.Day,
	config_tab->Inf_State.Instru_RTC.Inf_State.Hour,
	config_tab->Inf_State.Instru_RTC.Inf_State.Min,
	config_tab->Inf_State.Instru_RTC.Inf_State.Sec
	);

	/* 获取获取仪器号 *///"仪器号:%X%X%X%X%05d;\r\n"
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) | \
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) | \
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));

	/* 根据电源的报警状态信息设置短信内容 */
	SMS_Set_Power_Inf(&SMS_UTF8[strlen(SMS_UTF8)],&config_tab->Inf_State.Instru_Power,&fix_tab->Inf_State.Instru_Power);
	

}

/*
*********************************************************************************************************
*	函 数 名: SMS_Set_Battery_Alarm_Inf
*	功能说明: 设置电池电量低报警短信内容
*	形    参：config_tab:配置信息表;fix_tab:仪器变量表;
*	返 回 值: 无
*********************************************************************************************************
*/
static void SMS_Set_Battery_Alarm_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{

	
	/* 清除寄存器内容 */
	memset(SMS_UTF8,0X00,sizeof(SMS_UTF8));
	
	/* 设置发送短信内容 */
	/* 获取当前报警时间点的日期时间数据 */
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_Alarm_Time_String,
	config_tab->Inf_State.Instru_RTC.Inf_State.Year,
	config_tab->Inf_State.Instru_RTC.Inf_State.Month,
	config_tab->Inf_State.Instru_RTC.Inf_State.Day,
	config_tab->Inf_State.Instru_RTC.Inf_State.Hour,
	config_tab->Inf_State.Instru_RTC.Inf_State.Min,
	config_tab->Inf_State.Instru_RTC.Inf_State.Sec
	);
   
	/* 获取获取仪器号 *///"仪器号:%X%X%X%X%05d;\r\n"
	sprintf(&SMS_UTF8[strlen(SMS_UTF8)],SMS_SN_NO_String,
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) | \
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) | \
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));

	/* 根据电源的报警状态信息设置短信内容 */
	SMS_Set_Battery_Inf(&SMS_UTF8[strlen(SMS_UTF8)],&config_tab->Inf_State.Instru_Power,&fix_tab->Inf_State.Instru_Power);

}

/*
*********************************************************************************************************
*	函 数 名: SMS_Send_Power_Inf
*	功能说明: 发送掉电短信报警
*	形    参：State；
*	返 回 值: 无
*********************************************************************************************************
*/
BOOL SMS_Send_Power_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* 短信消息发送状态 */
	BOOL sms_state;
	
	/* 发送短信 */
	sms_state = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Send_SMS_EVENT);
	/* 判断短信发送队列状态 *///注意：这里的状态任务的优先级要比GSM任务高，否则发送完成队列后就执行GSM任务了，短信内容没有被设置。
	if(sms_state == TRUE)
	{
		/* 发送成功 */
		/* 设置发送短信内容 */
		SMS_Set_Power_Alarm_Inf(config_tab,fix_tab);
		/* 设置短信发送状态 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_State = TRUE;
	}
	/* 返回 短信信息 发送情况 */
	return sms_state;
}

/*
*********************************************************************************************************
*	函 数 名: SMS_Send_Power_Inf
*	功能说明: 发送掉电短信报警
*	形    参：State；
*	返 回 值: 无
*********************************************************************************************************
*/
BOOL SMS_Send_Battery_Inf(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* 短信消息发送状态 */
	BOOL sms_state;
	

	/* 发送短信 */
	sms_state = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Send_SMS_EVENT);
	/* 判断短信发送队列状态 *///注意：这里的状态任务的优先级要比GSM任务高，否则发送完成队列后就执行GSM任务了，短信内容没有被设置。
	if(sms_state == TRUE)
	{
		/* 发送成功 */
		/* 设置发送短信内容 */
		SMS_Set_Battery_Alarm_Inf(config_tab,fix_tab);
		/* 设置短信发送状态 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_State = TRUE;
	}
	/* 返回 短信信息 发送情况 */
	return sms_state;
}

/************************************************************************************************/
/* 函数名	: SMS_Again_Send_State_Check														*/
/* 输入		: send_time:前次发送短信记录的系统时间时间;	alarm_state:报警状态							*/
/* 输出		: 无																				*/
/* 作用		: 更新传感器数据		 									 							*/
/* 创建日期	: 2015/04/20																		*/
/* 修改日期	: 2015/08/28																		*/
/************************************************************************************************/
BOOL SMS_Again_Send_State_Check(u32 system_time,u32* send_time,u8 alarm_state)
{


	/* 判断报警状态 */
	if((alarm_state & 0x04) == 0X00)
	{
		/* 没有短信报警,记录当前时间. */
		*send_time = system_time;
		/* 不重新发送短信。 */
		return TRUE;
	}else
	{
		/* 短信报警 */

		if(*SMS_Repeat_Alarm_Interval_P != 0)
		{
			/* 判断前次发送的报警时间到现在是否超过重发时间 */
			//由于 短信重复报警时间间隔 寄存器 存的是秒数据，而系统时间用的是毫秒单位
			//因此这里的把SMS_Repeat_Alarm_Interval_P*1000转换成毫秒。
			if((system_time - *send_time) > (*SMS_Repeat_Alarm_Interval_P*1000))
			{
				/* 大于短信重发时间。 */
				/* 记录当前时间为发送时间 */
				*send_time = system_time;
				/* 重新发送短信。 */
				return FALSE;
			}else
			{
				/* 小于短信重发时间。 */
				/* 不重新发送短信。 */
				return TRUE;
			}
		}else
		{
			/* 短信重复报警时间间隔等于0表示不发送重复短信。 */
			/* 记录当前时间. */
			*send_time = system_time;
			/* 不重新发送短信。 */
			return TRUE;
		}

	}

}

/******************* (C) ZHOU *****END OF FILE****/


