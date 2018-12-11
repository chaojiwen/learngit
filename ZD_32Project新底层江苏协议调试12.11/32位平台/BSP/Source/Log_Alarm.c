/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/10/24																			 */
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
/* 函数名	: Alarm_State_Up_Reset														 	 			*/
/* 输入  	: sensor_t->SENSOR_Alarm_State:上限报警状态；sensor_f->Tmp_Up_Alarm_Cnt:报警状态延时时间。		*/
/* 输出		: 无 																						*/
/* 作用	 	: 设置上限报警状态							 												*/
/* 创建日期	: 2015/04/18																				*/
/********************************************************************************************************/
void Alarm_State_Up_Set(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* 累计警报时间 */
	sensor_f->Sensor_Up_Alarm_Cnt += Alarm_State_Check_Period;
	/* 警报时间 是否找过 设置的值 */
	if(sensor_f->Sensor_Up_Alarm_Cnt >= sensor_f->Sensor_Alarm_Delay)
	{
		/* 清除报警时间计数值 */
		sensor_f->Sensor_Up_Alarm_Cnt = 0;
		/* 设置 当前通道报警状态为:报警 */
		sensor_t->SENSOR_Alarm_State |= Alarm_Up;
	}	
}

/********************************************************************************************************/
/* 函数名	: Alarm_State_Low_Set														 	 			*/
/* 输入  	: sensor_t->SENSOR_Alarm_State:下限报警状态；sensor_f->Tmp_Up_Alarm_Cnt:报警状态延时时间。		*/
/* 输出		: 无 																						*/
/* 作用	 	: 设置下限报警状态							 												*/
/* 创建日期	: 2015/04/18																				*/
/********************************************************************************************************/
void Alarm_State_Low_Set(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	/* 累计警报时间 */
	sensor_f->Sensor_Low_Alarm_Cnt += Alarm_State_Check_Period;
	/* 警报时间 是否找过 设置的值 */
	if(sensor_f->Sensor_Low_Alarm_Cnt >= sensor_f->Sensor_Alarm_Delay)
	{
		/* 清除报警时间计数值 */
		sensor_f->Sensor_Low_Alarm_Cnt = 0;
		/* 设置 当前通道报警状态为:报警 */
		sensor_t->SENSOR_Alarm_State |= Alarm_Low;
	}	
}

/********************************************************************************************************/
/* 函数名	: Alarm_State_Tmp_Up_Reset														 	 		*/
/* 输入  	: sensor_t->SENSOR_Alarm_State:上限报警状态；sensor_f->Tmp_Up_Alarm_Cnt:报警状态延时时间。		*/
/* 输出		: 无 																						*/
/* 作用	 	: 清除上限报警状态							 												*/
/* 创建日期	: 2015/04/18																				*/
/********************************************************************************************************/
void Alarm_State_Up_Reset(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	
	/* 状态检查 */
	if(sensor_t->SENSOR_Alarm_State == Alarm_Up)
	{
		/* 判断 状态时间 累计 */
		if(sensor_f->Sensor_Up_Alarm_Cnt < Alarm_Reset_Delay)
		{
			/* 累计时间 */
			sensor_f->Sensor_Up_Alarm_Cnt += Alarm_State_Check_Period;	
		}else
		{	/* 清除累计时间 */
			sensor_f->Sensor_Up_Alarm_Cnt = 0;
			/* 取消 报警 状态 */
			sensor_t->SENSOR_Alarm_State &= ~Alarm_Up;
			/* 判断 报警取消标志 */
			if(sensor_f->Sensor_Suspend_Alarm_Flag == TRUE)
			{
				/* 清除 报警取消标志 */
				sensor_f->Sensor_Suspend_Alarm_Flag = FALSE;
			}
		}
	}else if(sensor_f->Sensor_Up_Alarm_Cnt)
	{
		/* 清除 累计时间 */
		sensor_f->Sensor_Up_Alarm_Cnt = 0;
	}
}

/********************************************************************************************************/
/* 函数名	: Alarm_State_Tmp_Low_Reset														 	 		*/
/* 输入  	: sensor_t->SENSOR_Alarm_State:下限报警状态；sensor_f->Tmp_Up_Alarm_Cnt:报警状态延时时间。		*/
/* 输出		: 无 																						*/
/* 作用	 	: 清除下限报警状态							 												*/
/* 创建日期	: 2015/04/18																				*/
/********************************************************************************************************/
void Alarm_State_Low_Reset(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f)
{
	
	/* 状态检查 */
	if(sensor_t->SENSOR_Alarm_State == Alarm_Low)
	{
		/* 判断 状态时间 累计 */
		if(sensor_f->Sensor_Low_Alarm_Cnt < Alarm_Reset_Delay)
		{
			/* 累计时间 */
			sensor_f->Sensor_Low_Alarm_Cnt += Alarm_State_Check_Period;	
		}else
		{	/* 清除累计时间 */
			sensor_f->Sensor_Low_Alarm_Cnt = 0;
			/* 取消 报警 状态 */
			sensor_t->SENSOR_Alarm_State &= ~Alarm_Low;
			/* 判断 报警取消标志 */
			if(sensor_f->Sensor_Suspend_Alarm_Flag == TRUE)
			{
				/* 清除 报警取消标志 */
				sensor_f->Sensor_Suspend_Alarm_Flag = FALSE;
			}
		}
	}else if(sensor_f->Sensor_Low_Alarm_Cnt)
	{
		/* 清除 累计时间 */
		sensor_f->Sensor_Low_Alarm_Cnt = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Alarm_Reminding
*	功能说明: 平台传感器通道声光报警，短信报警功能的检测。
*	形    参：*config_tab:仪器配置参数指针；fix_tab:仪器固定参数配置指针
*	返 回 值: 无
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
	/* 传感器通道1报警状态 */
	u8 sensor_ch1_alarm_sr 	= 0;
	/* 传感器报警通道1短信息发送状态 */
	static BOOL sensor_ch1_sms_send_state 	= FALSE;
	/* 传感器报警通道1重复短信息发送时间寄存器 */
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
	/* 传感器通道2报警状态 */
	u8 sensor_ch2_alarm_sr 	= 0;
	/* 传感器报警通道2短信息发送状态 */
	static BOOL sensor_ch2_sms_send_state 	= FALSE;
	/* 传感器报警通道2重复短信息发送时间寄存器 */
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
	/* 传感器通道3报警状态 */
	u8 sensor_ch3_alarm_sr 	= 0;
	/* 传感器报警通道3短信息发送状态 */
	static BOOL sensor_ch3_sms_send_state 	= FALSE;
	/* 传感器报警通道3重复短信息发送时间寄存器 */
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
	/* 传感器通道4报警状态 */
	u8 sensor_ch4_alarm_sr 	= 0;
	/* 传感器报警通道4短信息发送状态 */
	static BOOL sensor_ch4_sms_send_state 	= FALSE;
	/* 传感器报警通道4重复短信息发送时间寄存器 */
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
	/* 传感器通道5报警状态 */
	u8 sensor_ch5_alarm_sr 	= 0;
	/* 传感器报警通道5短信息发送状态 */
	static BOOL sensor_ch5_sms_send_state 	= FALSE;
	/* 传感器报警通道5重复短信息发送时间寄存器 */
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
	/* 传感器通道6报警状态 */
	u8 sensor_ch6_alarm_sr 	= 0;
	/* 传感器报警通道6短信息发送状态 */
	static BOOL sensor_ch6_sms_send_state 	= FALSE;
	/* 传感器报警通道6重复短信息发送时间寄存器 */
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
	/* 传感器通道7报警状态 */
	u8 sensor_ch7_alarm_sr 	= 0;
	/* 传感器报警通道7短信息发送状态 */
	static BOOL sensor_ch7_sms_send_state 	= FALSE;
	/* 传感器报警通道7重复短信息发送时间寄存器 */
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
	/* 传感器通道8报警状态 */
	u8 sensor_ch8_alarm_sr 	= 0;
	/* 传感器报警通道8短信息发送状态 */
	static BOOL sensor_ch8_sms_send_state 	= FALSE;
	/* 传感器报警通道8重复短信息发送时间寄存器 */
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
	/* 传感器通道9报警状态 */
	u8 sensor_ch9_alarm_sr 	= 0;
	/* 传感器报警通道9短信息发送状态 */
	static BOOL sensor_ch9_sms_send_state 	= FALSE;
	/* 传感器报警通道9重复短信息发送时间寄存器 */
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
	/* 传感器通道10报警状态 */
	u8 sensor_ch10_alarm_sr = 0;
	/* 传感器报警通道10短信息发送状态 */
	static BOOL sensor_ch10_sms_send_state 	= FALSE;
	/* 传感器报警通道10重复短信息发送时间寄存器 */
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
	/* 传感器通道11报警状态 */
	u8 sensor_ch11_alarm_sr = 0;
	/* 传感器报警通道11短信息发送状态 */
	static BOOL sensor_ch11_sms_send_state 	= FALSE;
	/* 传感器报警通道11重复短信息发送时间寄存器 */
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
	/* 传感器通道12报警状态 */
	u8 sensor_ch12_alarm_sr = 0;
	/* 传感器报警通道12短信息发送状态 */
	static BOOL sensor_ch12_sms_send_state 	= FALSE;
	/* 传感器报警通道12重复短信息发送时间寄存器 */
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
	/* 传感器通道13报警状态 */
	u8 sensor_ch13_alarm_sr = 0;
	/* 传感器报警通道13短信息发送状态 */
	static BOOL sensor_ch13_sms_send_state 	= FALSE;
	/* 传感器报警通道13重复短信息发送时间寄存器 */
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
	/* 传感器通道14报警状态 */
	u8 sensor_ch14_alarm_sr = 0;
	/* 传感器报警通道14短信息发送状态 */
	static BOOL sensor_ch14_sms_send_state 	= FALSE;
	/* 传感器报警通道14重复短信息发送时间寄存器 */
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
	/* 传感器通道15报警状态 */
	u8 sensor_ch15_alarm_sr = 0;
	/* 传感器报警通道15短信息发送状态 */
	static BOOL sensor_ch15_sms_send_state 	= FALSE;
	/* 传感器报警通道15重复短信息发送时间寄存器 */
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
	/* 传感器通道16报警状态 */
	u8 sensor_ch16_alarm_sr = 0;
	/* 传感器报警通道16短信息发送状态 */
	static BOOL sensor_ch16_sms_send_state 	= FALSE;
	/* 传感器报警通道16重复短信息发送时间寄存器 */
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
	/* 传感器通道17报警状态 */
	u8 sensor_ch17_alarm_sr = 0;
	/* 传感器报警通道17短信息发送状态 */
	static BOOL sensor_ch17_sms_send_state 	= FALSE;
	/* 传感器报警通道17重复短信息发送时间寄存器 */
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
	/* 传感器通道18报警状态 */
	u8 sensor_ch18_alarm_sr = 0;
	/* 传感器报警通道18短信息发送状态 */
	static BOOL sensor_ch18_sms_send_state 	= FALSE;
	/* 传感器报警通道18重复短信息发送时间寄存器 */
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
	/* 传感器通道19报警状态 */
	u8 sensor_ch19_alarm_sr = 0;
	/* 传感器报警通道19短信息发送状态 */
	static BOOL sensor_ch19_sms_send_state 	= FALSE;
	/* 传感器报警通道19重复短信息发送时间寄存器 */
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
	/* 传感器通道20报警状态 */
	u8 sensor_ch20_alarm_sr = 0;
	/* 传感器报警通道20短信息发送状态 */
	static BOOL sensor_ch20_sms_send_state 	= FALSE;
	/* 传感器报警通道20重复短信息发送时间寄存器 */
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
	/* 传感器通道21报警状态 */
	u8 sensor_ch21_alarm_sr = 0;
	/* 传感器报警通道21短信息发送状态 */
	static BOOL sensor_ch21_sms_send_state 	= FALSE;
	/* 传感器报警通道21重复短信息发送时间寄存器 */
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
	/* 传感器通道22报警状态 */
	u8 sensor_ch22_alarm_sr = 0;
	/* 传感器报警通道22短信息发送状态 */
	static BOOL sensor_ch22_sms_send_state 	= FALSE;
	/* 传感器报警通道22重复短信息发送时间寄存器 */
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
	/* 传感器通道23报警状态 */
	u8 sensor_ch23_alarm_sr = 0;
	/* 传感器报警通道23短信息发送状态 */
	static BOOL sensor_ch23_sms_send_state 	= FALSE;
	/* 传感器报警通道23重复短信息发送时间寄存器 */
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
	/* 传感器通道24报警状态 */
	u8 sensor_ch24_alarm_sr = 0;
	/* 传感器报警通道24短信息发送状态 */
	static BOOL sensor_ch24_sms_send_state 	= FALSE;
	/* 传感器报警通道24重复短信息发送时间寄存器 */
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
	/* 传感器通道25报警状态 */
	u8 sensor_ch25_alarm_sr = 0;
	/* 传感器报警通道25短信息发送状态 */
	static BOOL sensor_ch25_sms_send_state 	= FALSE;
	/* 传感器报警通道25重复短信息发送时间寄存器 */
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
	/* 传感器通道26报警状态 */
	u8 sensor_ch26_alarm_sr = 0;
	/* 传感器报警通道26短信息发送状态 */
	static BOOL sensor_ch26_sms_send_state 	= FALSE;
	/* 传感器报警通道26重复短信息发送时间寄存器 */
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
	/* 传感器通道27报警状态 */
	u8 sensor_ch27_alarm_sr = 0;
	/* 传感器报警通道27短信息发送状态 */
	static BOOL sensor_ch27_sms_send_state 	= FALSE;
	/* 传感器报警通道27重复短信息发送时间寄存器 */
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
	/* 传感器通道28报警状态 */
	u8 sensor_ch28_alarm_sr = 0;
	/* 传感器报警通道28短信息发送状态 */
	static BOOL sensor_ch28_sms_send_state 	= FALSE;
	/* 传感器报警通道28重复短信息发送时间寄存器 */
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
	/* 传感器通道29报警状态 */
	u8 sensor_ch29_alarm_sr = 0;
	/* 传感器报警通道29短信息发送状态 */
	static BOOL sensor_ch29_sms_send_state 	= FALSE;
	/* 传感器报警通道29重复短信息发送时间寄存器 */
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
	/* 传感器通道30报警状态 */
	u8 sensor_ch30_alarm_sr = 0;
	/* 传感器报警通道30短信息发送状态 */
	static BOOL sensor_ch30_sms_send_state 	= FALSE;
	/* 传感器报警通道30重复短信息发送时间寄存器 */
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
	/* 传感器通道31报警状态 */
	u8 sensor_ch31_alarm_sr = 0;
	/* 传感器报警通道31短信息发送状态 */
	static BOOL sensor_ch31_sms_send_state 	= FALSE;
	/* 传感器报警通道31重复短信息发送时间寄存器 */
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
	/* 传感器通道32报警状态 */
	u8 sensor_ch32_alarm_sr = 0;
	/* 传感器报警通道32短信息发送状态 */
	static BOOL sensor_ch32_sms_send_state 	= FALSE;
	/* 传感器报警通道32重复短信息发送时间寄存器 */
	static u32 sensor_ch32_sms_again_send_time_reg = 0;
#endif
	
	/* 电源连接 的报警状态 */
	u8 power_alarm_sr 	= 0;
	/* 电池电量 的报警状态 */
	u8 battery_alarm_sr = 0;
	/* 掉电报警 短信息发送状态 */
	static BOOL power_sms_send_state 	= FALSE;
	/* 电池电量低 短信息发送状态 */
	static BOOL battery_sms_send_state 	= FALSE;
	/* 掉电报警 重复短信息发送时间寄存器 */
	static u32 power_sms_again_send_time_reg = 0;
	/* 电池电量低 重复短信息发送时间寄存器 */
	static u32 battery_sms_again_send_time_reg = 0;
	
	/* 系统当前时间 */
	u32 cur_time;	
	
	
#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
	/* 判断 实时数据存储开关. */
	if(config_tab->Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch == FALSE)
	{
		return;
	}
#endif
	
	/* 获取系统时间 */
	cur_time = OSTimeGet();
	
	/* 检测 传感器 报警 */
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


	/* 检测 电源连接状态 */
	power_alarm_sr = Power_ET_Alarm_Check(&config_tab->Inf_State.Instru_Power,&fix_tab->Inf_State.Instru_Power);
	/* 检测 电池电量状态 */
	battery_alarm_sr = Power_BT_Alarm_Check(&config_tab->Inf_State.Instru_Power,&fix_tab->Inf_State.Instru_Power);
	
	/* 判断 声音 报警总开关 */
	if(config_tab->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch == TRUE)
	{
		/* 声音 报警总开关:开 */
		/* 判断 传感器 所有通道 报警状态 */
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
			/* 存在 传感器 声音报警 请求 */
			/* 发送 传感器 声音报警 */
			Sensor_Alarm_Sound();
		}/* 判断 电源连接状态 报警 */
		else if(power_alarm_sr & 0x01)
		{
			/* 存在 传感器 声音报警 请求 */
			/* 发送 传感器 声音报警 */
			Power_Alarm_Sound();
		}/* 判断 电池电量低 报警状态 */
		else if(battery_alarm_sr & 0x01)
		{
			/* 存在 电池电量低 声音报警 请求 */
			/* 发送 电池电量低 声音报警 */
			Battery_Alarm_Sound();
		}
	
	}
	
	/* 判断 光 报警总开关 */
	if(config_tab->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch == TRUE)
	{
		/* 光 报警总开关:开 */
		/* 判断 传感器 所有通道 报警状态 */
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
			/* 存在 传感器 光 报警 请求 */
			/* 发送 传感器 光 报警 */
			Sensor_Alarm_Light();
		}/* 判断 传感器 所有通道 报警状态 */
		else if(power_alarm_sr & 0x02)
		{
			/* 存在 电源连接 光 报警 请求 */
			/* 发送 电源连接 光 报警 */
			Power_Alarm_Light();
		}/* 判断 电池电量低 报警状态 */
		else if(battery_alarm_sr & 0x02)
		{
			/* 存在 电池电量低 光 报警 请求 */
			/* 发送 电池电量低 光 报警 */
			Battery_Alarm_Light();
		}
	}
	
	/* 判断 短信 报警总开关 */
	if(config_tab->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch == TRUE)
	{
		/* 光 报警总开关:开 */
		/* 判断 是否有短信报警正在发送。 */
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_State == FALSE)
		{
			/* 没有正在发送的短信或者发送已经结束 */
			/* 判断短信发送结果 */
#if (SMS_User_Number == 0x01)
			if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] == TRUE)
#elif (SMS_User_Number == 0x02)		
			if((GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] == TRUE)&&(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] == TRUE))
#elif (SMS_User_Number == 0x03)
			if((GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] == TRUE)&&(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] == TRUE)&&(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] == TRUE))
#else
#error "没有设置短信报警用户"
#endif		
			{
				/* 短信发送成功 */
				/* 判断其他报警短息需要发送。 */
			
///* 判断 传感器功能是否配置 */
//#if (Sensor_Config_Enable != 0x00)
//这里应该设置成短信报警配置功能
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
				/* 判断 传感器通道1 所有通道 报警状态 */
				else if((sensor_ch1_alarm_sr & 0x04)&&(sensor_ch1_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch1_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_1);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道2 所有通道 报警状态 */
				else if((sensor_ch2_alarm_sr & 0x04)&&(sensor_ch2_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch2_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_2);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道3 所有通道 报警状态 */
				else if((sensor_ch3_alarm_sr & 0x04)&&(sensor_ch3_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch3_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_3);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道4 所有通道 报警状态 */
				else if((sensor_ch4_alarm_sr & 0x04)&&(sensor_ch4_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch4_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_4);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道5 所有通道 报警状态 */
				else if((sensor_ch5_alarm_sr & 0x04)&&(sensor_ch5_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch5_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_5);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道6 所有通道 报警状态 */
				else if((sensor_ch6_alarm_sr & 0x04)&&(sensor_ch6_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch6_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_6);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道7 所有通道 报警状态 */
				else if((sensor_ch7_alarm_sr & 0x04)&&(sensor_ch7_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch7_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_7);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道8 所有通道 报警状态 */
				else if((sensor_ch8_alarm_sr & 0x04)&&(sensor_ch8_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch8_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_8);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道9 所有通道 报警状态 */
				else if((sensor_ch9_alarm_sr & 0x04)&&(sensor_ch9_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch9_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_9);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道8 所有通道 报警状态 */
				else if((sensor_ch10_alarm_sr & 0x04)&&(sensor_ch10_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch10_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_10);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道11 所有通道 报警状态 */
				else if((sensor_ch11_alarm_sr & 0x04)&&(sensor_ch11_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch11_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_11);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道12 所有通道 报警状态 */
				else if((sensor_ch12_alarm_sr & 0x04)&&(sensor_ch12_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch12_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_12);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道13 所有通道 报警状态 */
				else if((sensor_ch13_alarm_sr & 0x04)&&(sensor_ch13_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch13_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_13);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道14 所有通道 报警状态 */
				else if((sensor_ch14_alarm_sr & 0x04)&&(sensor_ch14_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch14_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_14);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道15 所有通道 报警状态 */
				else if((sensor_ch15_alarm_sr & 0x04)&&(sensor_ch15_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch15_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_15);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道16 所有通道 报警状态 */
				else if((sensor_ch16_alarm_sr & 0x04)&&(sensor_ch16_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch16_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_16);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道17 所有通道 报警状态 */
				else if((sensor_ch17_alarm_sr & 0x04)&&(sensor_ch17_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch17_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_17);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道18 所有通道 报警状态 */
				else if((sensor_ch18_alarm_sr & 0x04)&&(sensor_ch18_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch18_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_18);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道19 所有通道 报警状态 */
				else if((sensor_ch19_alarm_sr & 0x04)&&(sensor_ch19_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch19_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_19);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道20 所有通道 报警状态 */
				else if((sensor_ch20_alarm_sr & 0x04)&&(sensor_ch20_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch20_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_20);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道21 所有通道 报警状态 */
				else if((sensor_ch21_alarm_sr & 0x04)&&(sensor_ch21_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch21_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_21);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道22 所有通道 报警状态 */
				else if((sensor_ch22_alarm_sr & 0x04)&&(sensor_ch22_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch22_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_22);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道23 所有通道 报警状态 */
				else if((sensor_ch23_alarm_sr & 0x04)&&(sensor_ch23_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch23_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_23);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道24 所有通道 报警状态 */
				else if((sensor_ch24_alarm_sr & 0x04)&&(sensor_ch24_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch24_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_24);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道25 所有通道 报警状态 */
				else if((sensor_ch25_alarm_sr & 0x04)&&(sensor_ch25_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch25_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_25);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道26 所有通道 报警状态 */
				else if((sensor_ch26_alarm_sr & 0x04)&&(sensor_ch26_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch26_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_26);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道27 所有通道 报警状态 */
				else if((sensor_ch27_alarm_sr & 0x04)&&(sensor_ch27_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch27_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_27);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道28 所有通道 报警状态 */
				else if((sensor_ch28_alarm_sr & 0x04)&&(sensor_ch28_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch28_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_28);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道29 所有通道 报警状态 */
				else if((sensor_ch29_alarm_sr & 0x04)&&(sensor_ch29_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch29_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_29);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道30 所有通道 报警状态 */
				else if((sensor_ch30_alarm_sr & 0x04)&&(sensor_ch30_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch30_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_30);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道31 所有通道 报警状态 */
				else if((sensor_ch31_alarm_sr & 0x04)&&(sensor_ch31_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch31_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_31);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
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
				/* 判断 传感器通道32 所有通道 报警状态 */
				else if((sensor_ch32_alarm_sr & 0x04)&&(sensor_ch32_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					sensor_ch32_sms_send_state = SMS_Send_Sensor_Inf(config_tab,fix_tab,Sensors_CH_32);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
					sensor_ch32_sms_again_send_time_reg = cur_time;
				}
#endif	
//#endif			
		
				/* 判断 电源连接 所有通道 报警状态 */
				else if((power_alarm_sr & 0x04)&&(power_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					power_sms_send_state = SMS_Send_Power_Inf(config_tab,fix_tab);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
					power_sms_again_send_time_reg = cur_time;
				}
				/* 判断 电池电量低 报警状态 */
				else if((battery_alarm_sr & 0x04)&&(battery_sms_send_state == FALSE))
				{
					/* 设置 报警短信发送 */
					battery_sms_send_state = SMS_Send_Battery_Inf(config_tab,fix_tab);
					/* 将单前时间设置为短信发送时间，为后续的重复发送短息报警做基准时间。 */
					battery_sms_again_send_time_reg = cur_time;
				}
				
			}else
			{
				/* 当前短信发送失败，请求重新发送短信。 */
				GSM_SMS_SR_Reg.GSM_SMS_Send_State = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Send_SMS_EVENT);
			}
		}
	}
	
///* 判断 传感器功能是否配置 */
//#if (Sensor_Config_Enable != 0x00)
	
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) || \
	 (Sensor_CH1_Config_Type == Sensor_Hum) || \
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
	 (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	

	/* 判断 传感器 通道1 报警状态 */
	if((sensor_ch1_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道1 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch1_sms_send_state == TRUE)
		{
			/* 传感器 通道1 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch1_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道1 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch1_sms_send_state == TRUE)
		{
			/* 短信 发送成功 */
			/* 判断 短信重发 状态 */
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
		
	/* 判断 传感器 通道2 报警状态 */
	if((sensor_ch2_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道2 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch2_sms_send_state == TRUE)
		{
			/* 传感器 通道2 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch2_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道1 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch2_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
		
	/* 判断 传感器 通道3 报警状态 */
	if((sensor_ch3_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道3 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch3_sms_send_state == TRUE)
		{
			/* 传感器 通道3 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch3_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道3 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch3_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道4 报警状态 */
	if((sensor_ch4_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道4 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch4_sms_send_state == TRUE)
		{
			/* 传感器 通道4 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch4_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道4 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch4_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道5 报警状态 */
	if((sensor_ch5_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道5 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch5_sms_send_state == TRUE)
		{
			/* 传感器 通道5 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch5_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道5 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch5_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道6 报警状态 */
	if((sensor_ch6_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道6 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch6_sms_send_state == TRUE)
		{
			/* 传感器 通道6 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch6_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道6 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch6_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道7 报警状态 */
	if((sensor_ch7_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道7 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch7_sms_send_state == TRUE)
		{
			/* 传感器 通道7 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch7_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道7 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch7_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道8 报警状态 */
	if((sensor_ch8_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道8 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch8_sms_send_state == TRUE)
		{
			/* 传感器 通道8 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch8_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道8 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch8_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道9 报警状态 */
	if((sensor_ch9_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道9 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch9_sms_send_state == TRUE)
		{
			/* 传感器 通道9 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch9_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道9 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch9_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道10 报警状态 */
	if((sensor_ch10_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道10 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch10_sms_send_state == TRUE)
		{
			/* 传感器 通道10 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch10_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道10 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch10_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道11 报警状态 */
	if((sensor_ch11_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道11 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch11_sms_send_state == TRUE)
		{
			/* 传感器 通道11 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch11_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道11 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch11_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道12 报警状态 */
	if((sensor_ch12_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道12 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch12_sms_send_state == TRUE)
		{
			/* 传感器 通道12 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch12_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道12 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch12_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道13 报警状态 */
	if((sensor_ch13_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道13 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch13_sms_send_state == TRUE)
		{
			/* 传感器 通道13 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch13_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道13 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch13_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道14 报警状态 */
	if((sensor_ch14_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道14 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch14_sms_send_state == TRUE)
		{
			/* 传感器 通道14 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch14_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道14 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch14_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道15 报警状态 */
	if((sensor_ch15_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道15 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch15_sms_send_state == TRUE)
		{
			/* 传感器 通道15 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch15_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道15 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch15_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道16 报警状态 */
	if((sensor_ch16_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道16 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch16_sms_send_state == TRUE)
		{
			/* 传感器 通道16 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch16_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道16 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch16_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道17 报警状态 */
	if((sensor_ch17_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道17 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch17_sms_send_state == TRUE)
		{
			/* 传感器 通道17 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch17_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道17 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch17_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道18 报警状态 */
	if((sensor_ch18_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道18 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch18_sms_send_state == TRUE)
		{
			/* 传感器 通道18 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch18_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道18 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch18_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道19 报警状态 */
	if((sensor_ch19_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道19 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch19_sms_send_state == TRUE)
		{
			/* 传感器 通道19 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch19_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道19 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch19_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道20 报警状态 */
	if((sensor_ch20_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道20 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch20_sms_send_state == TRUE)
		{
			/* 传感器 通道20 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch20_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道20 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch20_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道21 报警状态 */
	if((sensor_ch21_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道21 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch21_sms_send_state == TRUE)
		{
			/* 传感器 通道21 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch21_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道21 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch21_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道22 报警状态 */
	if((sensor_ch22_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道22 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch22_sms_send_state == TRUE)
		{
			/* 传感器 通道22 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch22_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道22 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch22_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道23 报警状态 */
	if((sensor_ch23_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道23 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch23_sms_send_state == TRUE)
		{
			/* 传感器 通道23 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch23_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道23 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch23_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道24 报警状态 */
	if((sensor_ch24_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道24 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch24_sms_send_state == TRUE)
		{
			/* 传感器 通道24 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch24_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道24 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch24_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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
	
	/* 判断 传感器 通道25 报警状态 */
	if((sensor_ch25_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道25 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch25_sms_send_state == TRUE)
		{
			/* 传感器 通道25 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch25_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道25 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch25_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道26 报警状态 */
	if((sensor_ch26_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道26 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch26_sms_send_state == TRUE)
		{
			/* 传感器 通道26 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch26_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道26 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch26_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道27 报警状态 */
	if((sensor_ch27_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道27 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch27_sms_send_state == TRUE)
		{
			/* 传感器 通道27 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch27_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道27 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch27_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道28 报警状态 */
	if((sensor_ch28_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道28 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch28_sms_send_state == TRUE)
		{
			/* 传感器 通道28 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch28_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道28 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch28_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道29 报警状态 */
	if((sensor_ch29_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道29 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch29_sms_send_state == TRUE)
		{
			/* 传感器 通道29 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch29_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道29 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch29_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道30 报警状态 */
	if((sensor_ch30_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道30 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch30_sms_send_state == TRUE)
		{
			/* 传感器 通道30 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch30_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道30 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch30_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道31 报警状态 */
	if((sensor_ch31_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道31 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch31_sms_send_state == TRUE)
		{
			/* 传感器 通道31 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch31_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道31 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch31_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
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

	/* 判断 传感器 通道32 报警状态 */
	if((sensor_ch32_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 传感器 通道32 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(sensor_ch32_sms_send_state == TRUE)
		{
			/* 传感器 通道32 短信信息已经发送 */
			/* 清除 短信发送标志 */
			sensor_ch32_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 传感器 通道32 报警 */
		/* 判断 短信发送 状态 */
		if(sensor_ch32_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
			sensor_ch32_sms_send_state = SMS_Again_Send_State_Check(cur_time,&sensor_ch32_sms_again_send_time_reg,power_alarm_sr);
		}
	}
#endif	
//#endif

	/* 判断 断电报警 状态 */
	if((power_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 电源连接 正常 */
		/* 判断 掉电报警 短信息发送状态 */
		if(power_sms_send_state == TRUE)
		{
			/* 掉电报警 短信信息已经发送 */
			/* 清除 短信发送标志 */
			power_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 断电报警 */
		/* 判断 短信发送 状态 */
		if(power_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
			power_sms_send_state = SMS_Again_Send_State_Check(cur_time,&power_sms_again_send_time_reg,power_alarm_sr);
		}
	}

	/* 判断 电池电量低报警状态 */
	if((battery_alarm_sr & 0x04) == 0X00)
	{
		/* 当前 电池电量 正常 */
		/* 判断 电池电量低 短信息发送状态 */
		if(battery_sms_send_state == TRUE)
		{
			/* 电池电量低 短信信息已经发送 */
			/* 清除 短信发送标志 */
			battery_sms_send_state = FALSE;
		}
	}else
	{
		/* 当前 电池电量低报警 */
		/* 判断 短信发送 状态 */
		if(battery_sms_send_state == TRUE)
		{
			/* 判断 短信重发 状态 */
			battery_sms_send_state = SMS_Again_Send_State_Check(cur_time,&battery_sms_again_send_time_reg,battery_alarm_sr);
		}
	}
	
}

/************************************************************************************************/
/* 函数名	: Reset_Sensor_Suspend_Alarm_Flag_All_CH											*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 清除全部配置通道的报警取消标志														*/
/* 创建日期	: 2015/6/1																			*/
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
	/* 判断 传感器 通道1 报警取消标志状态。 */
	if(sensor_fix->Inf.CH1.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道2 报警取消标志状态。 */
	if(sensor_fix->Inf.CH2.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道3 报警取消标志状态。 */
	if(sensor_fix->Inf.CH3.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道4 报警取消标志状态。 */
	if(sensor_fix->Inf.CH4.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道5 报警取消标志状态。 */
	if(sensor_fix->Inf.CH5.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道6 报警取消标志状态。 */
	if(sensor_fix->Inf.CH6.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道7 报警取消标志状态。 */
	if(sensor_fix->Inf.CH7.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道8 报警取消标志状态。 */
	if(sensor_fix->Inf.CH8.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道9 报警取消标志状态。 */
	if(sensor_fix->Inf.CH9.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道10 报警取消标志状态。 */
	if(sensor_fix->Inf.CH10.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道11 报警取消标志状态。 */
	if(sensor_fix->Inf.CH11.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道12 报警取消标志状态。 */
	if(sensor_fix->Inf.CH12.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道13 报警取消标志状态。 */
	if(sensor_fix->Inf.CH13.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道14 报警取消标志状态。 */
	if(sensor_fix->Inf.CH14.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道15 报警取消标志状态。 */
	if(sensor_fix->Inf.CH15.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道16 报警取消标志状态。 */
	if(sensor_fix->Inf.CH16.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道17 报警取消标志状态。 */
	if(sensor_fix->Inf.CH17.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道18 报警取消标志状态。 */
	if(sensor_fix->Inf.CH18.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道19 报警取消标志状态。 */
	if(sensor_fix->Inf.CH19.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道20 报警取消标志状态。 */
	if(sensor_fix->Inf.CH20.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道21 报警取消标志状态。 */
	if(sensor_fix->Inf.CH21.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道22 报警取消标志状态。 */
	if(sensor_fix->Inf.CH22.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道23 报警取消标志状态。 */
	if(sensor_fix->Inf.CH23.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道24 报警取消标志状态。 */
	if(sensor_fix->Inf.CH24.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道25 报警取消标志状态。 */
	if(sensor_fix->Inf.CH25.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道26 报警取消标志状态。 */
	if(sensor_fix->Inf.CH26.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道27 报警取消标志状态。 */
	if(sensor_fix->Inf.CH27.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道28 报警取消标志状态。 */
	if(sensor_fix->Inf.CH28.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道29 报警取消标志状态。 */
	if(sensor_fix->Inf.CH29.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道30 报警取消标志状态。 */
	if(sensor_fix->Inf.CH30.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道31 报警取消标志状态。 */
	if(sensor_fix->Inf.CH31.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道32 报警取消标志状态。 */
	if(sensor_fix->Inf.CH32.Sensor_Suspend_Alarm_Flag == TRUE)
	{

		/* 清除报警取消标志 */
		sensor_fix->Inf.CH32.Sensor_Suspend_Alarm_Flag = FALSE;
	}
#endif

}

/************************************************************************************************/
/* 函数名	: Reset_Sensor_Alarm_State_All_CH													*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 清除全部配置通道的报警状态															*/
/* 创建日期	: 2015/8/25																			*/
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
	/* 判断 传感器 通道1 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH1.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道2 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH2.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道3 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH3.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道4 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH4.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道5 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH5.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道6 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH6.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道7 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH7.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道8 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH8.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道9 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH9.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道10 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH10.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道11 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH11.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道12 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH12.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道13 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH13.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道14 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH14.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道15 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH15.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道16 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH16.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道17 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH17.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道18 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH18.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道19 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH19.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道20 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH20.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道21 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH21.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道22 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH22.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道23 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH23.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道24 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH24.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道25 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH25.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道26 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH26.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道27 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH27.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道28 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH28.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道29 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH29.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道30 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH30.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道31 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH31.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
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
	/* 判断 传感器 通道32 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH32.SENSOR_Alarm_State == TRUE)
	{
		/* 清除报警取消标志 */
		sensor_c->Inf_State.CH32.SENSOR_Alarm_State = FALSE;
	}
#endif

}

/************************************************************************************************/
/* 函数名	: Set_Sensor_Suspend_Alarm_Flag_All_CH												*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 全部配置通道的报警取消标志															*/
/* 创建日期	: 2015/6/1																			*/
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
	/* 判断 传感器 通道1 的报警状态 */
	if(sensor_c->Inf_State.CH1.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道1报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道2 报警取消标志状态。 */
	if(sensor_c->Inf_State.CH2.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道2报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道3 的报警状态 */
	if(sensor_c->Inf_State.CH3.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道3报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道4 的报警状态 */
	if(sensor_c->Inf_State.CH4.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道4报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道5 的报警状态 */
	if(sensor_c->Inf_State.CH5.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道5报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道6 的报警状态 */
	if(sensor_c->Inf_State.CH6.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道6报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道7 的报警状态 */
	if(sensor_c->Inf_State.CH7.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道7报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道8 的报警状态 */
	if(sensor_c->Inf_State.CH8.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道8报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道9 的报警状态 */
	if(sensor_c->Inf_State.CH9.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道9报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道10 的报警状态 */
	if(sensor_c->Inf_State.CH10.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道10报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道11 的报警状态 */
	if(sensor_c->Inf_State.CH11.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道11报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道12 的报警状态 */
	if(sensor_c->Inf_State.CH12.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道12报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道13 的报警状态 */
	if(sensor_c->Inf_State.CH13.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道13报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道14 的报警状态 */
	if(sensor_c->Inf_State.CH14.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道14报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道15 的报警状态 */
	if(sensor_c->Inf_State.CH15.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道15报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道16 的报警状态 */
	if(sensor_c->Inf_State.CH16.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道16报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道17 的报警状态 */
	if(sensor_c->Inf_State.CH17.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道17报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道18 的报警状态 */
	if(sensor_c->Inf_State.CH18.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道18报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道19 的报警状态 */
	if(sensor_c->Inf_State.CH19.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道19报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道20 的报警状态 */
	if(sensor_c->Inf_State.CH20.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道20报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道21 的报警状态 */
	if(sensor_c->Inf_State.CH21.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道21报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道22 的报警状态 */
	if(sensor_c->Inf_State.CH22.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道22报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道23 的报警状态 */
	if(sensor_c->Inf_State.CH23.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道23报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道24 的报警状态 */
	if(sensor_c->Inf_State.CH24.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道24报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道25 的报警状态 */
	if(sensor_c->Inf_State.CH25.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道25报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道26 的报警状态 */
	if(sensor_c->Inf_State.CH26.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道26报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道27 的报警状态 */
	if(sensor_c->Inf_State.CH27.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道27报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道28 的报警状态 */
	if(sensor_c->Inf_State.CH28.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道28报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道29 的报警状态 */
	if(sensor_c->Inf_State.CH29.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道29报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道30 的报警状态 */
	if(sensor_c->Inf_State.CH30.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道30报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道31 的报警状态 */
	if(sensor_c->Inf_State.CH31.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道31报警 */
		/* 设置报警取消标志 */
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
	/* 判断 传感器 通道32 的报警状态 */
	if(sensor_c->Inf_State.CH32.SENSOR_Alarm_State != Alarm_Null)
	{
		/* 传感器通道32报警 */
		/* 设置报警取消标志 */
		sensor_f->Inf.CH32.Sensor_Suspend_Alarm_Flag = TRUE;
	}
#endif
	
}



/******************* (C) ZHOU *****END OF FILE****/


