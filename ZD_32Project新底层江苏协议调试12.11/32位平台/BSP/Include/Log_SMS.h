#ifndef __Log_SMS_H
#define __Log_SMS_H
			  	 
#include "stm32f10x.h"
#include "Mcu_Config.h"


/* 短信UTF-8的数据缓存缓存 */
#define SMS_UTF8_Data_Buffer_Size					(512u)
/* 等待发送的短信内容数据 */
extern char SMS_UTF8[SMS_UTF8_Data_Buffer_Size];

/* 函数声明 */
BOOL SMS_Send_Sensor_Inf	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,u8 alarm_ch_no);
BOOL SMS_Send_Power_Inf		(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
BOOL SMS_Send_Battery_Inf	(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
BOOL SMS_Again_Send_State_Check(u32 system_time,u32* send_time,u8 alarm_state);

#endif  
	 

