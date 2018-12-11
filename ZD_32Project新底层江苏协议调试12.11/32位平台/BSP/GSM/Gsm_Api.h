#ifndef _GSM_API_H   
#define _GSM_API_H	
#include <stdbool.h>
#include "stm32f10x.h"
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>

//	<o> ��Ӧ��ͨ��ģ��ѡ��
//  <0=> [SIM800C]ģ��.
//  <1=> [SIM7020C]ģ��.
//  <2=> [ESP8266Wifi]ģ��.
//  <3=> [����MG2639]ģ��.
//  <4=> [����1]ģ��.
//  <5=> [����2]ģ��.
#define  GSM_Module_Type			       (0x03)
//	<<< end of configuration section >>>	

//GSMͨ��ģ����
typedef struct
{
		//��ģ����ƽӿ�
	 void  (*pGSM_IO_Init)           (void);//GSMģ��IO��ʼ��
	 void  (*pGSM_Power_Up)          (void);//��GSMģ���Դ
	 void  (*pGSM_Power_Down)        (void);//�ر�GSMģ���Դ	
	 void  (*pGSM_Usart_Init)        (u32 bound);//GSMģ�鴮�ڳ�ʼ��
	 void  (*pGSM_Sleep)             (void); //GSMģ�����˯��
	 void  (*pGSM_WakeUp)            (void); //GSMģ�黽��
	 bool  (*pGSM_Power_Rest)        (void);//����GSMģ��

	//��ģ�������ӿ�
	 struct sim800c	 
	 {
	  bool  (*pGSM_Echo)             (bool flag);//����GSMģ���Ƿ����
	  bool  (*pGSM_Send_AT)          (void);//��ģ�鷢��"AT"
	  bool  (*pGSM_CSQ)              (uint8_t *csq); //��ȡGSMģ����ź�ǿ��
	  bool  (*pGSM_CheckNetwork)     (void);//���SIM���Ƿ�ע����������
	  bool  (*pGSM_CheckGPRSActive)  (void);//���SIM���Ƿ�������GPRS����
	  bool  (*pGSM_Check_Sim_Card)   (void);//���SIM���Ƿ���λ
		bool  (*pGSM_Send_Sms)(char *num,char *smstext);//���Ͷ�����������
	 }sim800c;
	 	//��ģ�������ӿ�
	 struct mg2639	 
	 {
	  bool  (*pGSM_Echo)             (bool flag);//����GSMģ���Ƿ����
	  bool  (*pGSM_Send_AT)          (void);//��ģ�鷢��"AT"
	  bool  (*pGSM_send_cmd)          (char *cmd,char *ack,u16 waittime);//��GSMģ�鷢��ATָ��
	  bool  (*pGSM_CSQ)              (uint8_t *csq); //��ȡGSMģ����ź�ǿ��
		int	  (*GSM_Adaptive_BaudRate) (void);  /* ����Ӧ������ */
		bool	   (*GSM_SetBaudRate)       (int br);//����ģ�鲨����
	  bool  (*pGSM_CheckNetwork)     (void);//���SIM���Ƿ�ע����������
	  bool  (*pGSM_CheckGPRSActive)  (void);//���SIM���Ƿ�������GPRS����
	  bool  (*pGSM_Check_Sim_Card)   (void);//���SIM���Ƿ���λ
	  bool  (*pGSM_Open_Connect)      (uint8_t ServerID,char *ServerName,char *ServerPort);//��TCP����������
		bool  (*pGSM_SendData)          (uint8_t ServerID,char *wbuf, int wlen);//���������������
		bool  (*pGSM_Send_Sms)(char *num,char *smstext);//���Ͷ�����������	 
	 }mg2639;
	 	//��ģ�������ӿ�
	struct esp8266
	{
		bool  (*pGSM_Echo)(bool flag); //����GSMģ���Ƿ����
		bool  (*pGSM_Send_AT)(void); //��ģ�鷢��"AT"
		bool  (*pGSM_CSQ)(uint8_t *csq); //��ȡGSMģ����ź�ǿ��
		bool  (*pGSM_CheckNetwork)(void); //���SIM���Ƿ�ע����������
		bool  (*pGSM_SetWifiMode)(void); //����WIFIģʽ
		bool  (*pGSM_OpenConnectAP)(char *SSID, char *password); //����AP����
		bool  (*pGSM_CloseConnectAP)(void); //�Ͽ�AP����
		bool  (*pGSM_CheckNetStatus)(uint8_t *Status); //��ѯ����������Ϣ
		bool  (*pGSM_SetDHCP)(bool flag); //����IP��ַ�Ƿ�̬��ȡ
		bool  (*pGSM_SetStaticIP)(char *ip, char *gateway, char* netmask); //���þ�̬IP
		bool  (*pGSM_SetSingleConnect)(void); //����������
		bool  (*pGSM_SetTransmittionMode)(void); //���ô���ģʽ��ĿǰΪ��͸����
		bool  (*pGSM_AskLocalIPAddress)(char *station_ip); //��ѯ����IP��ַ(Stationģʽ��)
		bool  (*pGSM_AskCipStatus)(char *station_ip, char *gateway, char *netmask); //��ѯ�Ѿ�����stationģʽ�µľ�̬IP��ַ�Ȳ���
		bool  (*pGSM_AskMacAddress)(char *MacAddress); //��ѯMAC��ַ
		bool  (*pGSM_Restore)(void); //�ָ���������
		bool  (*pGSM_LightSleep)(bool flag); //����ǳ˯��ģʽ
		bool  (*pGSM_SetLightSleepWakeUpPin)(uint8_t pin, uint8_t level); //���û���ģ��ǳ˯�ߵ�����
		bool  (*pGSM_AskDHCP)(bool *flag); //��ѯDHCP״̬(Stationģʽ��)
	  bool  (*pGSM_Rest)(void); //����Wifiģ��
	}esp8266; 
	//��ͨ�Žӿ�
   bool  (*pGSM_send_cmd)          (char *cmd,char *ack,u16 waittime);//��GSMģ�鷢��ATָ��
	 bool  (*pGSM_Open_Connect)      (char *ServerName,char *ServerPort);//��TCP����������
	 bool  (*pGSM_CloseConnect)      (void);//�ر�TCP����
	 bool  (*pGSM_SendData)          (char *wbuf, int wlen);//���������������
	 bool  (*pGSM_RevData)           (char *rbuf, int *rlen);//��������������� 
}GSM_HANDLE;


bool   GSM_Transmittion_Init(GSM_HANDLE *pHandle);//GSMģ��Ӧ�ò���ײ�ӿڳ�ʼ������
	
extern  GSM_HANDLE   gsm_handle; //����GSMͨѶ���

#endif /*_GSMAPI_H_*/
