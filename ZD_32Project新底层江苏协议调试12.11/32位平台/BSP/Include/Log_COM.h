#ifndef __Log_COM_H
#define __Log_COM_H
			  	 
#include "stm32f10x.h"	     
#include "Mcu_Config.h"

#include "Gsm_Api.h"

/************************************************************************************************/
/* ������У׼ͨ��Э�� ���ݴ�С */
#define Com_Sensor_Cal_Data_Sise					(60u)
/* ������У׼ͨ��Э�� ���ݽṹ */
typedef struct {
/* ��һ������ ͷ�� */
	u8	Head_Code;
/* �ڶ������� �豸��ַ(�Ҹо����������) */
	u8	Instru_NO;
/* ���������� �����루������� */	
	u8	Cmd_Code;
/* ���ĸ����� ͨ���� */
	u8	Chanel_Num;
/* ��������� ���ݳ��� */
	u8	Data_Len;
/* ���������� ���� */
	u8	Com_Data[Com_Sensor_Cal_Data_Sise];
/* ���߸����� ����0X66���������������֡�ã� */
	u8	Constant;
/* �ڰ˸����� У�� */
	u8	Check;
}COM_SENSOR_CAL_TYPEDEF;

/***********************����1����ֵ������У׼���ݵĽṹ�ͱ��� �����Ͷ���*******************************/
/* ������У׼ ���� ͷ�� */
#define Com_Sensor_Cal_Send_Head_Code				          (0X25)
/* ������У׼ ���� ͷ�� */
#define Com_Sensor_Cal_Rece_Head_Code				          (0X3c)
/* ������У׼ ���� */
#define Com_Sensor_Cal_Constant						            (0X66)
/* ������У׼ ��ʵʱ���� ��ͨ������ */
#define Com_Sensor_Cal_Read_Real_Data_CH_NO			      (0XFF)
/* ������У׼ ͨ�����ݻ����С */
#define Com_Sensor_Cal_Data_Buffer_Size				 (INSTRU_SENSOR_Data_Tab_Size*2*2)//8��У׼��*ÿ��У׼��2�ֽ�*ÿ��ͨ��2������
/* ������У׼ ����֡�̶����ݳ��� */
#define Com_Sensor_Cal_Fix_Data_Len					          (7u)
/* ������У׼ ͨ��������󳤶� */
#define Com_Sensor_Cal_Packet_Len					     (Com_Sensor_Cal_Data_Sise+Com_Sensor_Cal_Fix_Data_Len)
/* ������У׼ ��д���� ���ݳ��� */
#define Com_Sensor_Cal_Curve_Data_Len				   (INSTRU_SENSOR_Data_Tab_Size*2)
/* ������У׼ ͨ�� ��ʱʱ�� */
#define Com_Sensor_Cal_Timeout						            (600u)
/* ������У׼ �������� �ɹ� */
#define Com_Sensor_Cal_Set_Para_Succeed				        (0X55)
/* ������У׼ �������� ʧ�� */
#define Com_Sensor_Cal_Set_Para_Fail				          (0XAA)
/* ������У׼ ������ */


#if (GSM_Module_Type==2) //ESP8266Wifiģ��
/**************ESP8266Wifiģ����ز�����**********************/
/* ����дWifi���Ʋ����� ͨ������������һ���ֽ����������ֶ���д 0x55��ʾ��ȡ  0xAA��ʾд�� WSSSID*/
#define Com_Read_Write_Wifi_Name_Cmd_Code	    (0X20)
/* ����дWifi��WSKEY��ز���(���ܷ�ʽ������)������ ͨ������������һ���ֽ����������ֶ���д 0x55��ʾ��ȡ  0xAA��ʾд�� WSSSID*/
#define Com_Read_Write_Wifi_WSKEY_Cmd_Code    (0X21)
/* ����дWifi��WANN��������ز���(����IP��ַ���������롢����IP��ַ )ͨ������������һ���ֽ����������ֶ���д 0x55��ʾ��ȡ  0xAA��ʾд�� WSSSID*/
#define Com_Read_Write_Wifi_WALN_Cmd_Code	    (0X22)
/* ����дWifi��NETP��������ز���(Ҫ���ӵ����ݷ�������IP��ַ���˿ں�)ͨ������������һ���ֽ����������ֶ���д 0x55��ʾ��ȡ  0xAA��ʾд�� WSSSID*/
#define Com_Read_Write_Wifi_NETP_Cmd_Code	    (0X23)
/*��λWifiģ����߽�Wifiģ��ظ���������*/
#define Com_Set_Rest_Wifi_Cmd_Code	          (0X25)
/*����дWifi��ȡ��ʽ*/
#define Com_Read_Write_Wifi_DHCP_Cmd_Code	    (0X26)
/*����дWifiģ���MAC��ַ*/
#define Com_Read_Write_Wifi_MAC_Cmd_Code	    (0X27)


/************************************************************/
/* Wifi��WLAN�������ݽṹ */
typedef struct 
{
	/* ����IP��ַ */
	SERVER_IP_ADDRESS LOCALIP;
		/* ��������IP��ַ */
	SERVER_IP_ADDRESS MASK;
		/* ����IP��ַ */
	SERVER_IP_ADDRESS GATEWAY;

}WifiWlanConfigStruct;

#endif


/* ��ʵʱ���� */
#define Com_Sensor_Cal_Cmd_Code_Read_Real_Data		    (0X01)
/* д���� */                                      
#define Com_Sensor_Cal_Cmd_Code_Write_Curve_Data	    (0X03)
/* ������ */                                      
#define Com_Sensor_Cal_Cmd_Code_Read_Curve_Data		    (0X07)
/* ��ԭʼֵ���� */                                
#define Com_Sensor_Cal_Cmd_Code_Read_Orig_Data		    (0X30)
/* д����SN�� */                                  
#define Com_Config_SN_Cmd_Code_Write				          (0X31)
/* ������SN�� */                                  
#define Com_Config_SN_Cmd_Code_Read					          (0X32)
/* д���ô�����ͨ������ */                        
#define Com_Config_Sensor_Cmd_Code_Write			        (0X33)
/* �����ô�����ͨ������ */                        
#define Com_Config_Sensor_Cmd_Code_Read				        (0X34)
                                                  
/* �����ô�ӡ���ƺ� */                            
#define Com_Config_Card_NO_Cmd_Code_Read			        (0x35)
/* д���ô�ӡ���ƺ� */                            
#define Com_Config_Card_NO_Cmd_Code_Write			        (0x36)
                                                  
/* �����ô�ӡ��˾���� */                          
#define Com_Config_Company_Name_Cmd_Code_Read		      (0x37)
/* д���ô�ӡ��˾���� */                          
#define Com_Config_Company_Name_Cmd_Code_Write		    (0x38)
                                                  
/* ���������ٹ��� */                              
#define Com_Config_Fad_Data_Cmd_Code_Read			        (0x39)
/* д�������ٹ��� */                                  
#define Com_Config_Fad_Data_Cmd_Code_Write			      (0x3A)
                                                      
/* д���ݷ�����������IP���˿� */                      
#define ComConfigDataNetworkWrite					            (0x3B)
/* �����ݷ�����������IP���˿� */                      
#define ComConfigDataNetworkRead					            (0x3C)
                                                      
                                                  
/* д���÷�����������IP���˿� */                  
#define ComConfigNetworkWrite					                (0x56)
/* �����÷�����������IP���˿� */                      
#define ComConfigNetworkRead					                (0x57)
/* д���÷�����������IP���˿� */                      
#define ComConfigUpgradeNetworkWrite	                (0x58)
/* �����÷�����������IP���˿� */                      
#define ComConfigUpgradeNetworkRead		                (0x59)

/*******************д�����ֻ�������Ϣ*************************/
/* ���ֻ�������Ϣ*/                                   
#define  Com_Config_SMSAamPoeNO_Cmd_Code_Read			    (0x41)
#define  Check_SMSAamPhone_Len_Read                   (0x0F)//�����ж� ����  �ֻ��������ݳ��ȵ�λ��,������15λ
/*д�����ֻ�������Ϣ*/                                
#define  Com_Config_SMSAamPoeNO_Cmd_Code_Write			  (0x42)
/* д�����ֻ����� ����֡�̶����ݳ��� */
#define  Com_Config_SMSAamPoeNO_Fix_Data_Len			    (7u)
#define  Check_SMSAamPhone_Len                        (0x0B)//�����ж��ֻ����볤�ȵ�λ��,������11λ
/*****************����ʱ�������ϴ�ʱ��������ʷ��¼ʱ��������ӡʱ������ ������¼�����***************************/
/*******��ʱ����*******/
#define  Com_Config_Time_Interval_Cmd_Code_Read       (0x47)//��ʱ����������
/*******дʱ����*******/
#define  Com_Config_Time_Interval_Cmd_Code_Write      (0x48)//����ʱ����������
#define  Com_Config_Time_Interval_Set_Max_Second_Num  (0xFFFF)//����ʱ���� �����ֵ65535
/***********************���ñ�����ʱ (����Ϊ��λ) *********************************/
#define  Com_Config_Alarm_time_delay_SetMaxSecond     3600u//��󱨾���ʱ���ʱ��Ϊ1��Сʱ 
/************��������ʱ���***************/
#define  Com_Config_Alarm_time_delay_Cmd_Code_Read    (0x49)//��������ʱ������
/************д������ʱ���***************/
#define  Com_Config_Alarm_time_delay_Cmd_Code_Write   (0x4A)//д������ʱ������
/************�����ⱨ���ܿ���***************/
#define  Com_ConfigTotal_audible_light_alarm_Cmd_Code_Read    (0x4B)//�����ⱨ��������
/********  д���ⱨ���ܿ���***************/
#define  Com_ConfigTotal_audible_light_alarm_Cmd_Code_Write    (0x4C)//д���ⱨ��������
/************�����ű����ܿ���***************/
#define  Com_Config_SMS_alarm_total_switch_Cmd_Code_Read     (0x4D)//�����ű���������
#define  Com_Config_SMS_alarm_total_switch_Cmd_Code_Write    (0x4E)//д���ű���������

       
#define  Com_Config_GSM_Signal_Strength_Cmd_Code_Read    (0x80)//��GSMģ���ź�ǿ��

#define  Cancel_Sound_Light_Alarm_Cmd_Code            (0x81)//ȡ�����ⱨ�����ù����봦�ڴ������水�����ء�����������ͬ��

#define  Clear_Historical_Data_Cmd_Code               (0x09)//�����ʷ����
#define  Open_GSM_Debug_Information_Cmd_Code          (0x83)//��GSMģ�������Ϣ
#define  Check_PowerLink_And_Battery_Voltage_Cmd_Code (0x84)//�鿴��ӵ�״̬�͵�ص�ѹ״̬��Ϣ

#define  Open_DataSave_Cmd_Code                       (0x85)//�������ݼ�¼
//#define  Close_DataSave_Cmd_Code           (0x57)//�ر����ݼ�¼
#define  Check_DataSave_Status_Cmd_Code               (0x86)//��ѯ���ݼ�¼��״̬


#define  Com_Config_SystemTime_Cmd_Code_Read_Write    (0x08)//��ȡ������ϵͳʱ��


#define  Com_Config_Software_Version_NO_Major_Cmd_Code_Read   (0x0C)//��ȡ32λƽ̨����汾�ź�Ӳ���汾��

#define  Com_Config_System_Factory_Setting            (0x87)//�ָ���������


#define  Switch_ON                                    (0x01)//���ؿ�
#define  Switch_OFF                                   (0x00)//���ع�
 
/************************************************************************************************/

/* �������� */
void Com_Sensor_Cal_Send_Q			         (COM_SENSOR_CAL_TYPEDEF* Cal_Inf);
void COM_Sensor_Cal_Deal			           (COM_SENSOR_CAL_TYPEDEF* Cal_Inf);
u8 	 Com_Sensor_Cal_Get_Check		         (COM_SENSOR_CAL_TYPEDEF* Cal_Inf);
void COM_Sensor_Cal_Timeout_Check	       (INSTRU_FIX_INF_TAB* fix_tab,u16 check_time);
#endif  
