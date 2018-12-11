#ifndef __Log_GSM_Config_H
#define __Log_GSM_Config_H
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "Mcu_Config.h"
#include "Gsm_Api.h"
/* GSM�� ��Ϣ��� ��Ϣ��������(1 Byte) */
typedef enum {
	
	/* ����Wifi���ֺ����� �¼� */
	WifiNamePWD_EVENT		                 = 0X00,
	/* ����WLAN����(����IP���������롢���ص�ַ) �¼� */
	WifiWLAN_EVENT		                   = 0X01,
	/* ����Wifi IP��ַ��ȡ��ʽ STATIC����DHCPģʽ�¼� */
	WifiWLAN_GET_IP_WAY_EVENT		         = 0X02,
	/* ��λWifiģ���¼� */
	WifiRest_EVENT		                   = 0X03,
	/* Wifiģ���¼� */
	Factory_Data_Wifi_EVENT		           = 0X04,
}GSM_Config_TYPE;

typedef struct
{
	GSM_Config_TYPE	           EVENT_Cmd;		// �¼� ����
	u32*			                 EVENT_Str;		// �¼� ���� ָ��
	BOOL			                 EVENT_Getarg;	// �¼� ִ�н��(FALSE:�ź�����ִ�У�TRUE:�ͺ�ִ�н���)
}GSM_Config_QUE;//GSMģ����Ϣ���нṹ������


typedef struct
{
   BOOL   GSM_Config_State;//GSM_Config_State=FALSE������Ҫ����GSMģ�����     GSM_Config_State=TRUE������Ҫ����GSMģ�����
   BOOL   WifiNamePWD_Config_State;//����Wifiģ������Ƽ�����״̬��־
	 BOOL   WifiWLAN_Config_State;//����Wifiģ���WLAN����״̬��־
	 BOOL   WifiWLAN_GET_IP_WAY_Config_State;//����Wifiģ���ȡIP��ַ��ʽ״̬��־
	 BOOL   WifiRest_Config_State;//��λWifiģ��״̬��־
   BOOL   WifiFactory_Data_Config_State;//Wifiģ��ָ���������״̬��־
}GSM_Config_Struct;


extern  GSM_Config_Struct    GSM_Config_Reg;//GSMģ���������ݼĴ���



void    GSM_Config_Function(GSM_Config_QUE*  gsm_config_que_inf,INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);

BOOL    GSM_Config_Send_Q(OS_EVENT* pevent,GSM_Config_TYPE EVENT_Type);

#endif

