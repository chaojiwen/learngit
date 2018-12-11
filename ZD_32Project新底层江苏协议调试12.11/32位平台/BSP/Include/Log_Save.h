#ifndef __Log_Save_H
#define __Log_Save_H

#include "System_Config.h"
#include "stm32f10x.h"	     


#include "Log_Sensor.h"
#include "Log_Time.h"


/* ���������ݵ� ��� ���� */
#define Save_Sensor_Data_Buffer_Size		(sizeof(DATETIME_Storage_Format)/sizeof(u32)+Sensor_CH_Config_Num)//�32��ͨ��(������Դͨ��)
/* �洢ʱ������ݽṹ��С */
#define DATETIME_Buffer_Size	(4u)

//ʱ��������flash�д洢��ѹ����ʽ
typedef union 
{
	struct 
	{
		u32 SECOND_BITS	:6;
		u32 MINUTE_BITS	:6;
		u32 HOUR_BITS	:5;
		u32 DAY_BITS	:5;
		u32 MONTH_BITS	:4;
		u32 YEAR_BITS	:6;
	}DATETIME_Bit;
	u8 DATETIME_Buffer[DATETIME_Buffer_Size];
	u32 DATETIME_Bit_ALL;
}DATETIME_Storage_Format;

//�洢/��ȡ ������ ���ݽṹ
typedef union
{
	struct
	{	
		/* ʱ��� */
		DATETIME_Storage_Format Timestamp;
		/* 32��ͨ���Ĵ��������� */
		float Sensor_Data[Sensor_CH_Config_Num];
	}GPRS_IR;
	/* ���ݻ��� */
	u32 Save_Sensor_Data_Buffer[Save_Sensor_Data_Buffer_Size];
}Save_Sensor_Data_Struct;

#if ((Sensor_Function_Config_Enable != 0x00) && (Sensor_CH_Config_Num > 0))
#define Save_Sensor_Data_Len	(sizeof(DATETIME_Storage_Format)+Sensor_CH_Config_Num*sizeof(float))
#else
#define Save_Sensor_Data_Len	(0)
#endif


/* �������� */
void Log_Save_Flag_Check(INSTRU_MENU *save_inf,Times *system_time);
void Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
void Save_Flag_Post(INSTRU_MENU *menu_t,Times *system_time);
void Save_Get_Timestamp(DATETIME_Storage_Format* Timestamp);

float Save_Tmp_To_Float(float tmp);
float Save_Hum_To_Float(float hum);

#if ((Gateway_NJSY_Enable == 0x01)||(Gateway_HZZH_Enable == 0x01))
float SaveGw_TimeoutParaCheck(float data);
#endif
	 
#endif  
	 

