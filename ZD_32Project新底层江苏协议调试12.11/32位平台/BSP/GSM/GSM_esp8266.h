#ifndef _GSM_ESP8266_H   
#define _GSM_ESP8266_H	

#include <stdbool.h>
#include "GSM_USART.h"
#include "stm32f10x.h"

#define   ON  1  //��
#define   OFF 0  //��

//GSMģ���·���ؿ��ؿ��ƽ�
#define   GSM_CIRCUIT_CLK			RCC_APB2Periph_GPIOB
#define   GSM_CIRCUIT_PORT			GPIOB
#define   GSM_CIRCUIT_PIN			GPIO_Pin_1 

//GSMģ���Դ���ƽ�
#define   GSM_PWRCTRL_CLK           RCC_APB2Periph_GPIOC
#define   GSM_PWRCTRL_PORT          GPIOC
#define   GSM_PWRCTRL_PIN           GPIO_Pin_15

//GSMģ�鿪�����ƽ� (PWRKEY)
#define   GSM_ONOFF_CLK             RCC_APB2Periph_GPIOB
#define   GSM_ONOFF_PORT            GPIOB
#define   GSM_ONOFF_PIN             GPIO_Pin_2 
 
//GSMģ�����߿��ƽ�
#define   GSM_SLEEP_CLK             RCC_APB2Periph_GPIOD
#define   GSM_SLEEP_PORT            GPIOD
#define   GSM_SLEEP_PIN             GPIO_Pin_9 

//GSMģ���·���ؿ��ؿ��ƽ�������ƺ���	
#define   GSM_Circuit_Power(a)		if (a)	\
					GPIO_SetBits(GSM_CIRCUIT_PORT,GSM_CIRCUIT_PIN); \
					else		\
					GPIO_ResetBits(GSM_CIRCUIT_PORT,GSM_CIRCUIT_PIN) 
					
					
//GSMģ���Դ���ƽ�������ƺ���	
#define   GSM_Power(a)		if (a)	\
					GPIO_SetBits(GSM_PWRCTRL_PORT, GSM_PWRCTRL_PIN); \
					else		\
					GPIO_ResetBits(GSM_PWRCTRL_PORT, GSM_PWRCTRL_PIN) 
					
					
//GSMģ�鿪�����ƽ�������ƺ���		
#define   GSM_OnOff(a)		if (a)	\
					GPIO_SetBits(GSM_ONOFF_PORT, GSM_ONOFF_PIN); \
					else		\
					GPIO_ResetBits(GSM_ONOFF_PORT, GSM_ONOFF_PIN) 

//GSMģ�����߿��ƽ�������ƺ���	
#define   GSM_Sleep(a)		if (a)	\
					GPIO_SetBits(GSM_SLEEP_PORT, GSM_SLEEP_PIN); \
					else		\
					GPIO_ResetBits(GSM_SLEEP_PORT, GSM_SLEEP_PIN) 
					
					
//ģ����Ӧ��ָͨ��ȴ���ʱ�䵥λΪ:10ms
#define   Ordinary_AT_WaitTime_n10ms		    100	

//ָ��ȴ���ʱ�䵥λΪ:10ms ����AP���� 
#define   Open_AP_AT_WaitTime_n10ms				2000	//��ȴ�20��			

//ָ��ȴ���ʱ�䵥λΪ:10ms �Ͽ�AP���� 
#define   Close_AP_AT_WaitTime_n10ms			400		//��ȴ�4��	
					
//ָ��ȴ���ʱ�䵥λΪ:10ms ��TCP����������
#define   Open_Server_AT_WaitTime_n10ms		    600		//��ȴ�6��

//ָ��ȴ���ʱ�䵥λΪ:10ms �ر�TCP����������
#define   Close_Server_AT_WaitTime_n10ms	    600		//��ȴ�6��
					
//ָ��ȴ���ʱ�䵥λΪ:10ms ���ͷ���������
#define   SendData_Server_AT_WaitTime_n10ms		600					


/****************************** �������� ******************************/
void GSM_ESP8266_DelayMS(uint32_t ms);

/************************** ��ģ����ƽӿ� **************************/
void Gsm_Io_Init(void); //GSMģ�����IO�ڳ�ʼ��
void Gsm_Power_Up(void); //��GSMģ���Դ	
void Gsm_Power_Down(void); //�ر�GSMģ���Դ
bool Gsm_Power_Rest(void); //����GSMģ��
void Gsm_Module_Sleep(void); //GSMģ���������
void Gsm_Module_WakeUp(void); //GSMģ�黽��

/************************** ��ģ�������ӿ� ***************************/
bool Gsm_SetWifiMode_Station(uint8_t *UartBuf); //����WIFIģʽΪStationģʽ
bool Gsm_OpenConnectAP(char *SSID, char *password, uint8_t *UartBuf); //����AP����
bool Gsm_CloseConnectAP(uint8_t *UartBuf); //�Ͽ�AP����
bool Gsm_CheckStatus(uint8_t *Status, uint8_t *UartBuf); //��ѯ����������Ϣ
bool Gsm_StationMode_SetDHCP(bool flag, uint8_t *UartBuf); //Stationģʽ������DHCP�Ƿ�ʹ��
bool Gsm_StationMode_SetStaticIP(char *ip, char *gateway, char* netmask, uint8_t *UartBuf); //����stationģʽ��̬IP�Ȳ���
bool Gsm_CheckCipStatus(char *station_ip, char *gateway, char *netmask, uint8_t *UartBuf); //��ѯ�Ѿ�����stationģʽ�µľ�̬IP��ַ�Ȳ���
bool Gsm_SetSingleConnect(uint8_t *UartBuf); //����������
bool Gsm_SetTransmittionMode(uint8_t *UartBuf); //������ͨ����ģʽ����͸����
bool Gsm_CheckLocalIPAddress(char *station_ip, uint8_t *UartBuf); //��ѯ����IP��ַ(Stationģʽ��)
bool Gsm_AskRssi(int *rssi, uint8_t *UartBuf); //��ѯ�ź�ǿ��RSSI
bool Gsm_AskStationMacAddress(char *MacAddress, uint8_t *UartBuf); //��ѯMAC��ַ
bool Gsm_Echo(bool flag, u8 *UsartBuf); //�Ƿ�����GSM����
bool Gsm_Send_AT(u8 *UsartBuf); //��ģ�鷢��"AT"
bool Gsm_Restore(u8 *UsartBuf);
bool Gsm_Into_LightSleep(bool flag, u8 *UsartBuf);
bool Gsm_Set_LightSleepWakeUpPin(uint8_t pin, uint8_t level, u8 *UsartBuf);
bool Gsm_AskDHCP(bool *flag, u8 *UsartBuf);
bool Gsm_Rest(u8 *UsartBuf);//����Wifiģ��



/**************************** ��ͨ�Žӿ� *****************************/
bool Gsm_ClientConnectTCP(char *server, char *port, uint8_t *UartBuf); //��ΪClient����TCP����
bool Gsm_ClientCloseTCP(uint8_t *UartBuf); //�Ͽ�TCP����
bool  Gsm_check_cmd(char *str,u8 *UsartBuf); //���������ַ��� 
u8 Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf); //����ATָ��
u8 Gsm_send_data_Status(char *wbuf, int wlen, char *ack, u16 waittime, u8 *UsartBuf); 
u8 Gsm_send_data(char *wbuf, int wlen, u16 waittime, uint8_t *UartBuf); //��������
u8 Gsm_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf); //��������

#endif  


























