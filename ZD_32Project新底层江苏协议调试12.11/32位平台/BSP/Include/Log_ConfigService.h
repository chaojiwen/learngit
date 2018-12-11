/************************************************************************************************/
/** @file              : Log_ConfigService.H													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年08月02日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--配置服务器应用层文件							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2016 (C) HZ Zeda Team																*/
/**																								*/
/**文件名     : Log_ConfigService.H																*/
/**作用       : 配置服务器应用功能																*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年08月02日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_GSM.h"
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)//部标协议


#ifndef __Log_ConfigService_H
#define __Log_ConfigService_H

#include "Mcu_Config.h"



/****************************************参数表**************************************************/
/************************************************************************************************/ 

/* 系统信息 */
typedef struct
{
	/* 序列号 */
	SERIAL_NUMBER SerialNumber;
	/* 软件版本号 */
	VERSION SoftwareVersion;
	/* 硬件版本号 */
	VERSION HardwareVersion;
}
ConfigService_Para_System;

/* 电源信息 */
typedef struct
{
	/* 电源状态 */
	BOOL PowerState;
	/* 外接电源 报警开关 */
	uint8_t PowerAlarmSwitch;
	/* 外接电源 声光报警开关 */
	BOOL PowerSLAlarmSwitch;
	/* 外接电源 短信报警开关 */
	BOOL PowerSMSAlarmSwitch;

	/* 电量状态 */
	uint8_t PowerQuantityState;
	/* 低电量 报警开关 */
	BOOL BTAlarmSwitch;
	/* 低电量 声光报警开关 */
	BOOL BTSLAlarmSwitch;
	/* 低电量 短信报警开关 */
	BOOL BTSMSAlarmSwitch;

}
ConfigService_Para_Power;

/* 探头类型 */
typedef enum
{
	/* 未定义 */
	CSPS_Null = 0x00,
	/* 温度探头 */
	CSPS_Temp = 0x01,
	/* 湿度探头 */
	CSPS_Humi = 0x02,
	/* 单温卡片探头 */
	CSPS_RF_Temp = 0x03,
	/* 温湿卡片探头 */
	CSPS_RF_T_H_Temp = 0x04,
	/* 单温卡片探头 */
	CSPS_RF_T_H_Humi = 0x05,
	/* GPS经度探头 */
	CSPS_GPS_Lon = 0x06,
	/* GPS纬度探头 */
	CSPS_GPS_Lat = 0x07,
	/*  GPS速度探头 */
	CSPS_GPS_Spd = 0x08,
	/* 单温卡片探头 */
	CSPS_PT1000 = 0x09,
}
ConfigService_Para_SensorType;

/* 传感器信息 */
typedef struct
{
	/* 探头类型 */
	ConfigService_Para_SensorType SensorType;
	/* 探头报警开关 */
	BOOL SensorAlarmSwitch;
	/* 探头声光音报警开关 */
	BOOL SLAlarmSwitch;
	/* 短信报警开关 */
	BOOL SMSAlarmSwitch;
	
	/* 传感器 SN号 */
	uint8_t SN[4];
	/* RF卡片 号 */
	uint32_t ID;
	/* 报警上限 */
	float UpLimit;
	/* 报警下限 */
	float LowLimit;
}
ConfigService_Para_Sensor;

/* 菜单信息 */
typedef struct
{
	/* 声光 报警开关 */
	BOOL	SLAlarmSwitch;
	/* 短信新 报警开关 */
	BOOL	SMSAlarmSwitch;
	/* 保留 */
	uint8_t	Reserve_0;
	/* 保留 */
	uint8_t	Reserve_1;
	
	/* 重复短信报警时间 */
	uint32_t SMSAlarm_RepeatTime;
	/* 报警数据记录间隔 */
	uint32_t SaveInterval_Alarm;
	/* 正常数据记录间隔 */
	uint32_t SaveInterval_Normal;
	
}
ConfigService_Para_Menu;

/* 升级信息 */
typedef struct
{
	/* 升级标志 */
	uint8_t	Flag;
	/* 升级文件名 */
	char DocumentName[31];
	/* 升级地址（IP/域名） */
	char Network[32];	
	/* 升级端口 */
	uint8_t Port_HSB;
	uint8_t Port_LSB;
	/* 升级文件校验位*/
	uint32_t FileCheckBit;
	
}
ConfigService_Para_Update;

/* GSM模块信息 */
typedef struct
{
  char DataDomainName[32];/* 数据服务器地址（域名） */
	u8   DataNetworkReserve1[4];//数据服务器预留4个字节 
	SERVER_IP_ADDRESS    Data_IP_Address;/* 数据服务器地址（IP） */
	u16 DataNetworkPort	;/*数据服务器端口*/	
	u16 DataNetworkReserve;/*数据服务器保留 */	
	
  char ConfigDomainName[32];/* 配置服务器地址（域名） */
	u8   ConfigNetworkReserve1[4];//配置服务器预留4个字节
	SERVER_IP_ADDRESS  Config_IP_Address;/* 配置服务器地址（IP） */
	u16 ConfigNetworkPort	;/* 配置服务器端口*/
	u16 ConfigNetworkReserve;/*数据服务器保留 */
	
	char UpdateDomainName[32];/* 升级服务器地址（域名） */
	u8   UpdateNetworkReserve1[4];//升级服务器预留4个字节
	SERVER_IP_ADDRESS   Update_IP_Address;/* 升级服务器地址（IP） */
	u16  UpdateNetworkPort	;/* 升级服务器端口*/
	u16  UpdateNetworkReserve;/*升级服务器保留 */	
	
	/*报警短信号码配置 目前GSM模块只支持前三个报警号码发送短信 4、5为预留号码以便后期加入*/
	MOBILE_NO 	Alarm_SMS_NO1;/* GSM模块 报警短信号码_1 */		
	MOBILE_NO	  Alarm_SMS_NO2;/* GSM模块 报警短信号码_2 */	
	MOBILE_NO 	Alarm_SMS_NO3;/* GSM模块 报警短信号码_3 */	
	MOBILE_NO 	Alarm_SMS_NO4;/* GSM模块 报警短信号码_4 */		
	MOBILE_NO 	Alarm_SMS_NO5;/* GSM模块 报警短信号码_5 */	
  u8             Reserve[4];//升级服务器预留4个字节	
}
ConfigService_Para_GSM;

/* 配置服务器参数表 */
typedef struct
{
	/* 系统信息  */
	ConfigService_Para_System	System;
	/* 电源信息 */
	ConfigService_Para_Power	Power;
	/* 探头信息 */
	ConfigService_Para_Sensor	Sensor[32];
	/* 菜单信息 */
	ConfigService_Para_Menu		Menu;
	/* 升级信息 */
	ConfigService_Para_Update	Update;
	/* GSM模块信息 */
	ConfigService_Para_GSM    Gsm;
}ConfigService_ParaTabDef;


/* 配置服务器应用参数 */
typedef struct
{
	/* 同步状态 */
	BOOL SYNC_State;
	/* 操作状态 */
	BOOL Run_State;

	
}ConfigService_AppDef;

extern ConfigService_AppDef ConfigService_App;

void ConfigService_Cmd(uint8_t item,uint8_t type);
/*********************************  函数声明  ********************************/
void ConfigService_TXLog(INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_RXLog(GPRS_Struct gprs,INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_InitPara(INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_StopRun(void);
void ConfigService_StartRun(void);

#endif

#endif
#endif
/******************* (C) HZ Zeda Team *****END OF FILE****/
