/**
  ******************************************************************************
  * @file              : Unicode.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015年3月20日 15:09:47
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

/* 短信报警时间 */
#define SMS_Alarm_Time_String			"报警时间:%04d年%02d月%02d日,%02d时%02d分%02d秒;\r\n"
/* 仪器号 */
//#define SMS_SN_NO_String				"仪器号:%X%X%X%X%05d;\r\n"
#define SMS_SN_NO_String				"仪器号:%04X%05d;\r\n"
/* 温度传感器参数显示 */
#define SMS_Sensor_Temp_Para_String		"当前数据:%c%d.%01d℃(上限值:%c%d.%01d℃,下限值:%c%d.%01d℃);\r"
/* 湿度传感器参数显示 */
#define SMS_Sensor_Humi_Para_String		"当前数据:%d.%01d%%RH(上限值:%d.%01d%%RH,下限值:%d.%01d%%RH);\r"
/* 卡片温度传感器接收超时参数显示 */
#define SMS_RF_Card_Data_Rece_Timeout_Temp_Para_String	"当前数据:无数据(上限值:%c%d.%01d℃,下限值:%c%d.%01d℃);\r"
/* 卡片湿度传感器接收超时参数显示 */
#define SMS_RF_Card_Data_Rece_Timeout_Humi_Para_String	"当前数据:无数据(上限值:%d.%01d%%RH,下限值:%d.%01d%%RH);\r"
/* 传感器通道号 */
#define SMS_Sensor_CH_NO_String			"传感器编号:%01d;\r\n"
/* 传感器通道类型 */
#define SMS_Sensor_CH_Type_String		"通道%01d类型:%s;\r\n"
/* 温度传感器 */
#define SMS_Sensor_Temp_String			"温度传感器"
/* 湿度传感器 */
#define SMS_Sensor_Humi_String			"湿度传感器"


/* 南京盛亿传感器温度 */
#define SMS_ShengYi_Sensor_Temp_String			"盛亿温度传感器"

/* 南京盛亿传感器湿度 */
#define SMS_ShengYi_Sensor_Humi_String			"盛亿湿度传感器"



/* 单温卡片传感器 */
#define SMS_Sensor_RF_Card_Single_Temp_String	"单温卡片传感器"
/* 温湿度卡片温度传感器 */
#define SMS_Sensor_RF_Card_Humiture_Temp_String	"温湿度卡片温度传感器"
/* 温湿度卡片湿度传感器 */
#define SMS_Sensor_RF_Card_Humiture_Humi_String	"温湿度卡片湿度传感器"
/* 传感器报警状态 */
#define SMS_Sensor_Alarm_State_String	"报警状态:%s;\r\n"
/* 传感器报警 */
#define SMS_Sensor_Alarm_String			"报警"
/* 传感器未报警 */
#define SMS_Sensor_Normal_String		"未报警"
/* 传感器报警类型 */
#define SMS_Sensor_Alarm_Type_String	"通道%01d报警类型:%s;\r\n"
/* 传感器上限报警 */
#define SMS_Sensor_Alarm_Up_String		"上限报警"
/* 传感器下限报警 */
#define SMS_Sensor_Alarm_Down_String	"下限报警"
/* 卡片传感器接收超时报警类型 */
#define SMS_RF_Card_Data_Rece_Timeout_Alarm_String	"报警类型:数据接收超时;\r\n"
/* 掉电报警信息 */
#define SMS_Power_Alarm_Type_String		"报警事件:电源未接."
/* 电池电量低报警信息 */
#define SMS_Battery_Alarm_Type_String	"报警事件:电池电量低."

/******************* (C) ZHOU *****END OF FILE****/







/******************* (C) ZHOU *****END OF FILE****/
