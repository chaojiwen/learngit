/************************************************************************************************/
/** @file              : Log_ConfigService.H													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��08��02��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���÷�����Ӧ�ò��ļ�							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2016 (C) HZ Zeda Team																*/
/**																								*/
/**�ļ���     : Log_ConfigService.H																*/
/**����       : ���÷�����Ӧ�ù���																*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��08��02��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_GSM.h"
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)//����Э��


#ifndef __Log_ConfigService_H
#define __Log_ConfigService_H

#include "Mcu_Config.h"



/****************************************������**************************************************/
/************************************************************************************************/ 

/* ϵͳ��Ϣ */
typedef struct
{
	/* ���к� */
	SERIAL_NUMBER SerialNumber;
	/* ����汾�� */
	VERSION SoftwareVersion;
	/* Ӳ���汾�� */
	VERSION HardwareVersion;
}
ConfigService_Para_System;

/* ��Դ��Ϣ */
typedef struct
{
	/* ��Դ״̬ */
	BOOL PowerState;
	/* ��ӵ�Դ �������� */
	uint8_t PowerAlarmSwitch;
	/* ��ӵ�Դ ���ⱨ������ */
	BOOL PowerSLAlarmSwitch;
	/* ��ӵ�Դ ���ű������� */
	BOOL PowerSMSAlarmSwitch;

	/* ����״̬ */
	uint8_t PowerQuantityState;
	/* �͵��� �������� */
	BOOL BTAlarmSwitch;
	/* �͵��� ���ⱨ������ */
	BOOL BTSLAlarmSwitch;
	/* �͵��� ���ű������� */
	BOOL BTSMSAlarmSwitch;

}
ConfigService_Para_Power;

/* ̽ͷ���� */
typedef enum
{
	/* δ���� */
	CSPS_Null = 0x00,
	/* �¶�̽ͷ */
	CSPS_Temp = 0x01,
	/* ʪ��̽ͷ */
	CSPS_Humi = 0x02,
	/* ���¿�Ƭ̽ͷ */
	CSPS_RF_Temp = 0x03,
	/* ��ʪ��Ƭ̽ͷ */
	CSPS_RF_T_H_Temp = 0x04,
	/* ���¿�Ƭ̽ͷ */
	CSPS_RF_T_H_Humi = 0x05,
	/* GPS����̽ͷ */
	CSPS_GPS_Lon = 0x06,
	/* GPSγ��̽ͷ */
	CSPS_GPS_Lat = 0x07,
	/*  GPS�ٶ�̽ͷ */
	CSPS_GPS_Spd = 0x08,
	/* ���¿�Ƭ̽ͷ */
	CSPS_PT1000 = 0x09,
}
ConfigService_Para_SensorType;

/* ��������Ϣ */
typedef struct
{
	/* ̽ͷ���� */
	ConfigService_Para_SensorType SensorType;
	/* ̽ͷ�������� */
	BOOL SensorAlarmSwitch;
	/* ̽ͷ�������������� */
	BOOL SLAlarmSwitch;
	/* ���ű������� */
	BOOL SMSAlarmSwitch;
	
	/* ������ SN�� */
	uint8_t SN[4];
	/* RF��Ƭ �� */
	uint32_t ID;
	/* �������� */
	float UpLimit;
	/* �������� */
	float LowLimit;
}
ConfigService_Para_Sensor;

/* �˵���Ϣ */
typedef struct
{
	/* ���� �������� */
	BOOL	SLAlarmSwitch;
	/* ������ �������� */
	BOOL	SMSAlarmSwitch;
	/* ���� */
	uint8_t	Reserve_0;
	/* ���� */
	uint8_t	Reserve_1;
	
	/* �ظ����ű���ʱ�� */
	uint32_t SMSAlarm_RepeatTime;
	/* �������ݼ�¼��� */
	uint32_t SaveInterval_Alarm;
	/* �������ݼ�¼��� */
	uint32_t SaveInterval_Normal;
	
}
ConfigService_Para_Menu;

/* ������Ϣ */
typedef struct
{
	/* ������־ */
	uint8_t	Flag;
	/* �����ļ��� */
	char DocumentName[31];
	/* ������ַ��IP/������ */
	char Network[32];	
	/* �����˿� */
	uint8_t Port_HSB;
	uint8_t Port_LSB;
	/* �����ļ�У��λ*/
	uint32_t FileCheckBit;
	
}
ConfigService_Para_Update;

/* GSMģ����Ϣ */
typedef struct
{
  char DataDomainName[32];/* ���ݷ�������ַ�������� */
	u8   DataNetworkReserve1[4];//���ݷ�����Ԥ��4���ֽ� 
	SERVER_IP_ADDRESS    Data_IP_Address;/* ���ݷ�������ַ��IP�� */
	u16 DataNetworkPort	;/*���ݷ������˿�*/	
	u16 DataNetworkReserve;/*���ݷ��������� */	
	
  char ConfigDomainName[32];/* ���÷�������ַ�������� */
	u8   ConfigNetworkReserve1[4];//���÷�����Ԥ��4���ֽ�
	SERVER_IP_ADDRESS  Config_IP_Address;/* ���÷�������ַ��IP�� */
	u16 ConfigNetworkPort	;/* ���÷������˿�*/
	u16 ConfigNetworkReserve;/*���ݷ��������� */
	
	char UpdateDomainName[32];/* ������������ַ�������� */
	u8   UpdateNetworkReserve1[4];//����������Ԥ��4���ֽ�
	SERVER_IP_ADDRESS   Update_IP_Address;/* ������������ַ��IP�� */
	u16  UpdateNetworkPort	;/* �����������˿�*/
	u16  UpdateNetworkReserve;/*�������������� */	
	
	/*�������ź������� ĿǰGSMģ��ֻ֧��ǰ�����������뷢�Ͷ��� 4��5ΪԤ�������Ա���ڼ���*/
	MOBILE_NO 	Alarm_SMS_NO1;/* GSMģ�� �������ź���_1 */		
	MOBILE_NO	  Alarm_SMS_NO2;/* GSMģ�� �������ź���_2 */	
	MOBILE_NO 	Alarm_SMS_NO3;/* GSMģ�� �������ź���_3 */	
	MOBILE_NO 	Alarm_SMS_NO4;/* GSMģ�� �������ź���_4 */		
	MOBILE_NO 	Alarm_SMS_NO5;/* GSMģ�� �������ź���_5 */	
  u8             Reserve[4];//����������Ԥ��4���ֽ�	
}
ConfigService_Para_GSM;

/* ���÷����������� */
typedef struct
{
	/* ϵͳ��Ϣ  */
	ConfigService_Para_System	System;
	/* ��Դ��Ϣ */
	ConfigService_Para_Power	Power;
	/* ̽ͷ��Ϣ */
	ConfigService_Para_Sensor	Sensor[32];
	/* �˵���Ϣ */
	ConfigService_Para_Menu		Menu;
	/* ������Ϣ */
	ConfigService_Para_Update	Update;
	/* GSMģ����Ϣ */
	ConfigService_Para_GSM    Gsm;
}ConfigService_ParaTabDef;


/* ���÷�����Ӧ�ò��� */
typedef struct
{
	/* ͬ��״̬ */
	BOOL SYNC_State;
	/* ����״̬ */
	BOOL Run_State;

	
}ConfigService_AppDef;

extern ConfigService_AppDef ConfigService_App;

void ConfigService_Cmd(uint8_t item,uint8_t type);
/*********************************  ��������  ********************************/
void ConfigService_TXLog(INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_RXLog(GPRS_Struct gprs,INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_InitPara(INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_StopRun(void);
void ConfigService_StartRun(void);

#endif

#endif
#endif
/******************* (C) HZ Zeda Team *****END OF FILE****/
