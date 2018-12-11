#ifndef __Log_Alarm_H
#define __Log_Alarm_H
			  	 
#include "stm32f10x.h"	     
#include "Mcu_Config.h"


/************************************************************************************************/
/* 传感器报警类型 */
#define	Alarm_Null				(0x00)	//无报警
#define	Alarm_Up				(0x01)	//上限报警
#define	Alarm_Low  				(0x02)	//下限报警
//#define	Alarm_Tmp_Up		(0x01)	//温度上限报警
//#define	Alarm_Tmp_Low  		(0x02)	//温度下限报警
//#define	Alarm_Hum_Up		(0x04)	//湿度上限报警
//#define	Alarm_Hum_Low		(0x08)	//湿度下限报警
/************************************************************************************************/
void Alarm_State_Up_Set		(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f);
void Alarm_State_Low_Set	(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f);
void Alarm_State_Up_Reset	(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f);
void Alarm_State_Low_Reset	(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f);





void Alarm_Reminding(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);



void Reset_Sensor_Suspend_Alarm_Flag_All_CH(INSTRU_SENSOR_FIX* sensor_fix);
void Reset_Sensor_Alarm_State_All_CH(INSTRU_SENSOR* sensor_c);
void Set_Sensor_Suspend_Alarm_Flag_All_CH(INSTRU_SENSOR* sensor_c,INSTRU_SENSOR_FIX* sensor_f);
#endif  
	 

