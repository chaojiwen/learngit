#include "Gsm_Api.h"
#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��
#include "GSM_esp8266.h" //���gsmģ��ײ�ͷ�ļ�(����SIM800C,SIM7020C��ģ��ĵײ���������)
#include "GSM_USART.h"
#include "ucos_ii.h"
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "String_Utility.h"
#include "GSM_app.h"
/****************************** �������� ******************************/
void GSM_ESP8266_DelayMS(uint32_t ms)
{
	     OSTimeDly(ms);//��ʱ10ms		
}
/**********************************************************************/


/************************** ��ģ����ƽӿ� **************************/
//GSMģ�����IO�ڳ�ʼ��	
void Gsm_Io_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//GSMģ���·���ؿ��ؿ��ƽ�
	RCC_APB2PeriphClockCmd(GSM_CIRCUIT_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin		= GSM_CIRCUIT_PIN;
	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GSM_CIRCUIT_PORT, &GPIO_InitStructure);
	GSM_Circuit_Power(OFF); //Ĭ�Ϲر�GSMģ���·���ؿ���
	
	//GSM��Դ���ƽ�
	RCC_APB2PeriphClockCmd(GSM_PWRCTRL_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin		= GSM_PWRCTRL_PIN;
	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GSM_PWRCTRL_PORT, &GPIO_InitStructure);
	GSM_Power(ON); //Ĭ�Ϲر�GSMģ���Դ
	
	//GSM�������ƽ�
	RCC_APB2PeriphClockCmd(GSM_ONOFF_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin	    = GSM_ONOFF_PIN;
	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GSM_ONOFF_PORT, &GPIO_InitStructure);
	GSM_OnOff(ON); //Ĭ��GSMģ�鴦�ڹػ�״̬
	
	//GSMģ�����߿��ƽ�
	RCC_APB2PeriphClockCmd(GSM_SLEEP_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin	    = GSM_SLEEP_PIN;
	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GSM_SLEEP_PORT, &GPIO_InitStructure);
	GSM_Sleep(ON); //Ĭ��GSMģ�鴦�ڷ�����״̬
}

////////////GSMģ�鿪���͹ػ�
//////////static void Gsm_Boot(void)
//////////{
//////////	GSM_OnOff(OFF);
//////////	GSM_ESP8266_DelayMS(3000);//��ʱ3s
//////////	GSM_OnOff(ON);
//////////}

//��GSMģ���Դ	
void Gsm_Power_Up(void)
{
	GSM_Circuit_Power(ON);
//	GSM_ESP8266_DelayMS(300); //��ʱ300ms�Ա�ϵͳ��Դ�ȶ�
//	Gsm_Boot(); //GSMģ�鿪���͹ػ�
}	

//�ر�GSMģ���Դ
void Gsm_Power_Down(void)
{
//	Gsm_Boot();//GSMģ�鿪���͹ػ�
//	GSM_ESP8266_DelayMS(300);//��ʱ300ms�Ա�ϵͳ��Դ�ȶ�
	GSM_Circuit_Power(OFF);
}

//����GSMģ��
bool Gsm_Power_Rest(void)
{
	bool ret;
	ret=true;
	Gsm_Power_Down();//�ر�GSMģ���Դ
	GSM_ESP8266_DelayMS(300);//��ʱ300ms
	Gsm_Power_Up();//��GSMģ���Դ
	return ret;
}

//GSMģ���������
void Gsm_Module_Sleep(void)
{
	GSM_Sleep(ON);
	GSM_ESP8266_DelayMS(60);
}

//GSMģ�黽��
void Gsm_Module_WakeUp(void)
{
	GSM_Sleep(OFF);
	GSM_ESP8266_DelayMS(60);
}
/**********************************************************************/


/************************** ��ģ�������ӿ� ***************************/
//����WIFIģʽΪStationģʽ
bool Gsm_SetWifiMode_Station(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CWMODE_DEF=1", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//����AP���� AT+CWJAP="SSID","password"
bool Gsm_OpenConnectAP(char *SSID, char *password, uint8_t *UartBuf)
{
	bool ret = false;
	char ap_data[64] = {0};
	
	//����������״̬��ȥ������������ 
//	if(!Gsm_send_cmd("AT+CIPSTATUS", "STATUS:2", Ordinary_AT_WaitTime_n10ms, UartBuf))
//	{	
		sprintf(ap_data, "AT+CWJAP_DEF=\"%s\",\"%s\"", SSID, password); //��װAP�����˺ź�����
		if(Gsm_send_cmd(ap_data, "WIFI CONNECTED", Open_AP_AT_WaitTime_n10ms, UartBuf))
			ret = true;
//	}
//	else
//		ret = true;
	return ret;
}

//�Ͽ�AP����
bool Gsm_CloseConnectAP(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CWQAP", "OK", Close_AP_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//��ѯ����������Ϣ
//Status = 2 (ESP8266 station ������AP�����IP��ַ)
//Status = 3 (ESP8266 station �ѽ���TCP��UDP����)
//Status = 4 (ESP8266 station �Ͽ�TCP��UDP��������)
//Status = 5 (ESP8266 station δ����IP)
bool Gsm_CheckStatus(uint8_t *Status, uint8_t *UartBuf)
{
	char *p;
	bool ret = false;

	ret = Gsm_send_cmd("AT+CIPSTATUS", "STATUS", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //û��ʱ��������Ϊ��ѯ����ʧ��
	{
		*Status = 0; 
		return false;
	}
	p = strstr((const char*)UartBuf, "STATUS:"); //������ȷ�򷵻غ�����ǰ���״γ��ֵ��׵�ַ�����򷵻�NULL
	if(p != NULL)
	{
		ret = true;
		*Status = *(p+7)-0x30; //���ַ�����Ӧ����λתΪ����
	}
	return ret;
}

//DHCPʹ���£�IP��ַ�Ƕ�̬��ȡ��
//Stationģʽ������DHCP�Ƿ�ʹ��
bool Gsm_StationMode_SetDHCP(bool flag, uint8_t *UartBuf)
{
	bool ret = false; 
	if(flag == true)
	{
		if(Gsm_send_cmd("AT+CWDHCP_DEF=1,1", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
			ret = true;
	}
	else
	{
		if(Gsm_send_cmd("AT+CWDHCP_DEF=1,0", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
			ret = true;
	}
	return ret;
}

//!!! ʹ��ǰ��Ҫ�ȹر�Stationģʽ��DHCPʹ��
//AT+ CIPSTA_DEF �C ����ESP8266 station�ľ�̬IP��ַ�������浽flash
//AT+CIPSTA_DEF="192.168.6.100","192.168.6.1","255.255.255.0"
//<ip> ESP8266 station��IP��ַ
//<gateway> ����
//<netmask> ��������
bool Gsm_StationMode_SetStaticIP(char *ip, char *gateway, char* netmask, uint8_t *UartBuf)
{
	bool ret = false;
	char ip_data[64] = {0};
	
	sprintf(ip_data, "AT+CIPSTA_DEF=\"%s\",\"%s\",\"%s\"", ip, gateway, netmask); //��װAP�����˺ź�����
	if(Gsm_send_cmd(ip_data, "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//����������
bool Gsm_SetSingleConnect(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CIPMUX=0", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//������ͨ����ģʽ����͸����
bool Gsm_SetTransmittionMode(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CIPMODE=0", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//��ѯ����IP��ַ(Stationģʽ��)
bool Gsm_CheckLocalIPAddress(char *station_ip, uint8_t *UartBuf)
{
	bool ret = false;
	uint16_t index_start = 0, index_end = 0;
	
	ret = Gsm_send_cmd("AT+CIFSR", "+CIFSR:", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //û��ʱ��������Ϊ��ѯ����ʧ��
	{
		return false;
	}
	
	index_start = String_KMPMatch((char*)UartBuf, "\""); //��һ�����ŵ�λ��
	index_end = String_KMPMatch(((char*)UartBuf+index_start+1), "\"") + index_start + 1; //���һ�����ŵ�λ��
	memcpy(station_ip, (const char*)(UartBuf+index_start+1), index_end-index_start-1); //ȡ��IP�ַ���

	return ret;
}

//��ѯ�Ѿ�����stationģʽ�µľ�̬IP��ַ�Ȳ���
bool Gsm_CheckCipStatus(char *station_ip, char *gateway, char *netmask, uint8_t *UartBuf)
{
	bool ret = false;
	uint16_t index_start_ip = 0, index_end_ip = 0;
	uint16_t index_start_gateway = 0, index_end_gateway = 0;
	uint16_t index_start_netmask = 0, index_end_netmask = 0;
	
	ret = Gsm_send_cmd("AT+CIPSTA_DEF?", "+CIPSTA_DEF:", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //û��ʱ��������Ϊ��ѯ����ʧ��
	{ 
		return false;
	}
	
	index_start_ip = String_KMPMatch((char*)UartBuf, "\""); //��һ�����ŵ�λ��
	index_end_ip = String_KMPMatch(((char*)UartBuf+index_start_ip+1), "\"") + index_start_ip + 1; //�ڶ������ŵ�λ��(��1����Ϊ������ʼ��ַ����1)
	memcpy(station_ip, (const char*)(UartBuf+index_start_ip+1), index_end_ip-index_start_ip-1); //ȡ��IP�ַ���
	
	index_start_gateway = String_KMPMatch(((char*)UartBuf+index_end_ip+1), "\"") + index_end_ip + 1; //���������ŵ�λ��(��1����Ϊ������ʼ��ַ����1)
	index_end_gateway = String_KMPMatch(((char*)UartBuf+index_start_gateway+1), "\"") + index_start_gateway + 1; //���ĸ����ŵ�λ��(��1����Ϊ������ʼ��ַ����1)
	memcpy(gateway, (const char*)(UartBuf+index_start_gateway+1), index_end_gateway-index_start_gateway-1); //ȡ�������ַ���
	
	index_start_netmask = String_KMPMatch(((char*)UartBuf+index_end_gateway+1), "\"") + index_end_gateway + 1; //��������ŵ�λ��(��1����Ϊ������ʼ��ַ����1)
	index_end_netmask = String_KMPMatch(((char*)UartBuf+index_start_netmask+1), "\"") + index_start_netmask + 1; //���������ŵ�λ��(��1����Ϊ������ʼ��ַ����1)
	memcpy(netmask, (const char*)(UartBuf+index_start_netmask+1), index_end_netmask-index_start_netmask-1); //ȡ�����������ַ���
	
	return ret;
}

bool Gsm_AskRssi(int *rssi, uint8_t *UartBuf)
{
	bool ret = false;
	uint16_t index_start = 0, index_end = 0;
	char str_rssi[6] = "";
	
	ret = Gsm_send_cmd("AT+CWJAP?", "+CWJAP:", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //û��ʱ��������Ϊ��ѯ����ʧ��
	{ 
		return false;
	}
	index_start = String_KMPMatch((char*)UartBuf, ","); //��һ�����ŵ�λ��
	
	index_start = String_KMPMatch(((char*)UartBuf+index_start+1), ",")+index_start+1; //�ڶ������ŵ�λ��(��1����Ϊ������ʼ��ַ����1)
	
	index_start = String_KMPMatch(((char*)UartBuf+index_start+1), ",")+index_start+1; //���������ŵ�λ��(��1����Ϊ������ʼ��ַ����1)
	
	index_end = String_KMPMatch(((char*)UartBuf+index_start+1), "\r\n")+index_start+1; //�����ŵĻس����н���
	
	//ȡ��RSSI�ַ���
	memcpy(str_rssi, (const char*)(UartBuf+index_start+1), index_end-index_start-1); 
	*rssi = atoi(str_rssi); //תΪ����
	

	return ret;
}

//��ѯMAC��ַ
bool Gsm_AskStationMacAddress(char *MacAddress, uint8_t *UartBuf)
{
	bool ret = false;
	uint16_t index_start = 0, index_end = 0;
	
	ret = Gsm_send_cmd("AT+CIPSTAMAC?", "+CIPSTAMAC:", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //û��ʱ��������Ϊ��ѯ����ʧ��
	{ 
		return false;
	}
	index_start = String_KMPMatch((char*)UartBuf, "\""); //��һ�����ŵ�λ��
	
	index_end = String_KMPMatch(((char*)UartBuf+index_start+1), "\"")+index_start+1; //�ڶ������ŵ�λ��(��1����Ϊ������ʼ��ַ����1)
	
	//ȡ��Mac��ַ
	memcpy(MacAddress, (const char*)(UartBuf+index_start+1), index_end-index_start-1); 
	
	return ret;
}



//�Ƿ�����GSM���� flag:true������� flag:false��������
bool Gsm_Echo(bool flag, u8 *UsartBuf)
{
	bool  ret=false; 
	if(flag == true)
	{
		if( Gsm_send_cmd("ATE1", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
			ret = true;   
	}
	else
	{
		if( Gsm_send_cmd("ATE0", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
			ret = true;    
	}
	return ret;
}

//��ģ�鷢��"AT"
bool Gsm_Send_AT(u8 *UsartBuf)
{
	bool ret = false; 

	if( Gsm_send_cmd("AT", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	return ret;
}

//�ָ���������
bool Gsm_Restore(u8 *UsartBuf)
{
	bool ret = false; 

	if( Gsm_send_cmd("AT+RESTORE", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	return ret;
}

//����ǳ˯��ģʽ
//bool flag : �Ƿ����ǳ˯��ģʽ�ı�־
bool Gsm_Into_LightSleep(bool flag, u8 *UsartBuf)
{
	bool ret = false; 

	if(flag == true)
	{
		//light sleepģʽ
		if( Gsm_send_cmd("AT+SLEEP=1", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
			ret=true;   
	}
	else
	{
		//modem sleepģʽ ��ƽʱʹ�õ�
		if( Gsm_send_cmd("AT+SLEEP=2", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
			ret=true;
	}	
	return ret;
}

//���û���ģ��ǳ˯�ߵ�����
bool Gsm_Set_LightSleepWakeUpPin(uint8_t pin, uint8_t level, u8 *UsartBuf)
{
	bool ret = false; 
	char cmd[20] = {0};
	
	sprintf(cmd, "AT+WAKEUPGPIO=1,%d,%d", pin, level);
	if( Gsm_send_cmd(cmd, "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	return ret;
}

//��ѯDHCP״̬(stationģʽ��)
bool Gsm_AskDHCP(bool *flag, u8 *UsartBuf)
{
	//���� +CWDHCP_DEF:3 
	//bit0=0ΪAPģʽDHCP�ر�,bit0=1ΪAPģʽDHCP����
	//bit1=0ΪStationģʽDHCP�ر�,bit1=1ΪStationģʽDHCP����
	bool ret = false; 
	char cmd[20] = "AT+CWDHCP_DEF?";
	uint16_t index_start = 0;
	uint8_t DHCP_Status = 0;
	
	if( Gsm_send_cmd(cmd, "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	index_start = String_KMPMatch((char*)UsartBuf, ":");
	DHCP_Status = UsartBuf[index_start + 1] - 0x30;
	if((DHCP_Status&(1<<1)) == 0x02) //�ж�Stationλ�ı�־
		*flag = true;
	else if((DHCP_Status&(1<<1)) == 0x00)
		*flag = false;
	
	return ret;
}

//����Wifiģ��
bool Gsm_Rest(u8 *UsartBuf)
{
	bool ret = false; 

	if( Gsm_send_cmd("AT+RST", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	return ret;
}




/**********************************************************************/


/**************************** ��ͨ�Žӿ� *****************************/
//��ΪClient����TCP���� AT+CIPSTART="TCP","192.168.3.116",8080
bool Gsm_ClientConnectTCP(char *server, char *port, uint8_t *UartBuf)
{
	bool ret = false;
	char ap_data[64] = {0};
	
	//����������״̬��ȥ������������
	if(!Gsm_send_cmd("AT+CIPSTATUS", "STATUS:3", Ordinary_AT_WaitTime_n10ms, UartBuf))
	{  
		sprintf(ap_data, "AT+CIPSTART=\"TCP\",\"%s\",%s", server, port); //��װAP�����˺ź�����
		
		if(Gsm_send_cmd(ap_data, "CONNECT", Open_Server_AT_WaitTime_n10ms, UartBuf))
			ret = true;
	}
	else
		ret = true;
	return ret;
}

//�Ͽ�TCP����
bool Gsm_ClientCloseTCP(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CIPCLOSE", "OK", Close_Server_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}	

/************************************************************************************************
*������   ��u8* Gsm_check_cmd(char *str,u8 *UsartBuf)															
*������� ��str :ҪѰ�ҵ�Ŀ���ַ���											
*������� ��	  																				
*����     �����������ַ��� 	  
*�������� �� 2018/08/23	
*˵��     �� //���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����  ����ֵ:0,û�еõ��ڴ���Ӧ���� ����,�ڴ�Ӧ������λ��(str��λ��)
************************************************************************************************/
bool  Gsm_check_cmd(char *str,u8 *UsartBuf)
{
	int ret=0;
	
	ret = String_KMPMatch((char*)UsartBuf, str);
	if(ret==-1)
	{
		return false;
	}
	return true;
}

/************************************************************************************************
*������   ��u8 Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)											
*������� ��cmd:Ҫ���͵�ATָ���ַ���	ack��Ҫƥ��������ַ��� waittime:ƥ�������ַ�����Ҫ�ȴ���ʱ�� ��λ10ms										
*������� ������ִ�гɹ����� 1   ʧ�ܷ��� 0  																				
*����     ������ATָ�� 	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
u8 Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
{
	u8 res=1;
	Clear_GSM_RX_BUF(UsartBuf);//��ջ�������
	SIMCOM_Printf("%s\r\n", cmd);//�����ַ�����
	if(cmd == NULL)//���û�������ַ���ƥ��Ļ�����ֱ�ӷ��سɹ�����ִ���ַ���ƥ����
	{
		return res=0;
	}
	else //���ַ�����Ҫƥ����
	{ 
		if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
		{
			while(--waittime)	//�ȴ�����ʱ
			{ 
				//�����Ƿ����ERROR��������ַ���,����еĻ�ֱ���˳��ȴ����
				if(Gsm_check_cmd("ERROR",UsartBuf)) //TCP����ʧ�ܻ᷵�ظ�ָ��
				{
					res=0;
					break;//�õ���Ч���� 			 
				}
//////				if(Gsm_check_cmd("CLOSED",UsartBuf)) //TCP����ʧ�ܻ᷵�ظ�ָ��
//////				{
//////					res=0;
//////					break;//�õ���Ч���� 			 
//////				}
				if(Gsm_check_cmd("FAIL",UsartBuf)) //����AP��·ʧ�ܻ᷵�ظ�ָ��
				{
					res=0;
					break;//�õ���Ч���� 			 
				}
////////				if(Gsm_check_cmd("WIFI DISCONNECT",UsartBuf)) //����AP��·ʧ�ܻ᷵�ظ�ָ��
//////// 				{
////////					res=0;
////////					break;//�õ���Ч���� 			 
////////				}	
				if(Gsm_check_cmd("No AP",UsartBuf)) //δ����AP����᷵�ظ�ָ��
 				{
					res=0;
					break;//�õ���Ч���� 			 
				}
				if(Gsm_check_cmd(ack, UsartBuf))
				{
					break;//�õ���Ч���� 			 
				}
				GSM_ESP8266_DelayMS(10); //��ʱ10ms					 
			}
			if(waittime==0)
			{
				res=0; 
			}
		}
	}
	return res;
}

u8 Gsm_send_data_Status(char *wbuf, int wlen, char *ack, u16 waittime, u8 *UsartBuf)
{
	u8 res=1;
	Clear_GSM_RX_BUF(UsartBuf);//��ջ�������
	Usart_SendStr_length(USART3,wbuf,wlen);//������������
	waittime=waittime*10;
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{ 	
			if(Gsm_check_cmd(ack, UsartBuf))
			{
				break;//�õ���Ч���� 			 
			}
			GSM_ESP8266_DelayMS(1); //��ʱ1ms
		}
		if(waittime==0)
		{
			res=0; 
		}
	}
	return res;
}

//��������
u8 Gsm_send_data(char *wbuf, int wlen, u16 waittime, uint8_t *UartBuf)
{
	u8 res=1;
	char  cmd[32]={0};	
	sprintf(cmd,"AT+CIPSEND=%d",wlen);
	if(Gsm_send_cmd(cmd, ">", Ordinary_AT_WaitTime_n10ms, UartBuf)) //0.5s
	{
		if(Gsm_send_data_Status(wbuf, wlen, "SEND OK", waittime, UartBuf))
		{
			res=1;
		}
		else
		{
			res=0;
		}
	}
	else
		res=0;
	return res;
}

//////////�����յ��������Ƿ�Ϸ�
//////////Location1_0x7E:0x7E��һ�γ��ֵ�λ��;Location2_0x7E:0x7E�ڶ��γ��ֵ�λ��;
////////static bool  Gsm_check_Rev_Data(int *Location1_0x7E, int *Location2_0x7E,int rlen,u8 *UsartBuf)
////////{
////////	bool  ret=false; 
////////	int i=0;

////////	for(i=0;i<rlen;i++)//���ҵ�һ��0x7E���ֵ�λ��
////////	{
////////		if(UsartBuf[i]==0x7E)
////////		{
////////			*Location1_0x7E=i;
////////			ret=true;
////////			break;
////////		}
////////		else
////////		{
////////			ret=false;
////////		}
////////	}

////////	if(ret)
////////	{
////////		for(i=*Location1_0x7E;i<rlen;i++)//���ҵڶ��δ�0x7E���ֵ�λ��
////////		{
////////			if(UsartBuf[i+1]==0x7E)
////////			{
////////				*Location2_0x7E=i+1;
////////				ret=true;
////////				break;
////////			}
////////			else
////////			{
////////				ret=false;
////////			}
////////		}
////////	}
////////	else
////////	{
////////		ret=false;
////////	}
////////	return ret;
////////}

//////////��������
////////u8 Gsm_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf)
////////{
////////	u8 res=1;
////////	int Location1=0,Location2=0;//Location1:0x7E��һ�γ��ֵ�λ��;Location2:0x7E�ڶ��γ��ֵ�λ��;
////////	
////////	if(waittime)		//��Ҫ�ȴ�Ӧ��
////////	{
////////		while(--waittime)	//�ȴ�����ʱ
////////		{ 
////////			/*�����յ��������Ƿ�Ϸ�*/
////////			if(Gsm_check_Rev_Data(&Location1,&Location2,GSM_RX_BUF_LEN,UartBuf))
////////			{
////////				*rlen=(Location2-Location1)+1;
////////				memcpy(rbuf,&UartBuf[Location1],*rlen);//��������
////////				break;//�õ���Ч���� 			 
////////			}
////////			GSM_ESP8266_DelayMS(10); //��ʱ10ms
////////		}
////////		if(waittime==0)
////////		{
////////			res=0; 
////////		}
////////	}
////////	return res;
////////} 
static bool Gsm_RevData_Check(char *rbuf, uint16_t rlen)
{
	if((rbuf[0] == 0x7e) && (rbuf[rlen-1] == 0x7e))
	{
		return true;
	}
	else
		return false;
}

//bool Gsm_ReceiveData(char *rbuf, int *rlen, uint16_t UartRevLength, uint8_t *UartBuf)
//{
//	//���в�����ְ�������ͬ��Ϊ�˺ͷְ����ճ��򱣳�һ��
//	int Mark_Buf[3] = {0}; //��¼�����ַ���"+IPD,"ÿ�������ڻ������е�����λ��
//	int Mark_Cnt = 0; //��¼�����ַ���"+IPD,"���ִ���
//	int Mark_Colon_Buf[3] = {0}; //��¼�����ַ���":"ÿ�������ڻ������е�����λ��
//	char SubPackageLength[3][6] = { {0}, {0}, {0}}; //��¼ÿ���ְ������ݳ��� (�ַ�����ʽ)
//	int SubLength[3] = {0}; //��¼ÿ���ְ������ݳ���(������ʽ)
//	int length_1=0;
//////////////	int length_1=0, length_2=0, length_3=0;
//	
//	//Ѱ�ҽ��ձ�־��
//	Mark_Cnt = Find_SubStringEx((char*)UartBuf, "+IPD,", UartRevLength, Mark_Buf);
//	if(Mark_Cnt == 0) //ע�⣬����bin�ļ���Ậ��+IPD���ʲ��ܲ��÷ְ���ʽ�б�+IPD
//	{

//		return false;
//	}
//	
//	//Ѱ�����ݳ��Ƚ�����
//	if(Find_SubStringEx((char*)UartBuf+Mark_Buf[0], ":", 5, Mark_Colon_Buf) == 1)
//		length_1 = Mark_Colon_Buf[0] - 1;
//	
//	//ת���õ��������ݵĳ���
//	memcpy(&(SubPackageLength[0]), (char*)UartBuf+Mark_Buf[0], length_1);
//	SubLength[0] = atoi(SubPackageLength[0]);
//	*rlen = SubLength[0];

//	
//	//�����ڻ��������ݷŵ���������
//	memcpy(rbuf, (char*)UartBuf+Mark_Buf[0]+length_1+1, SubLength[0]);
//	
//	return true;
//}
//����һ������GSMģ��ӷ�������ȡ�������Ӻ���
////bool Gsm_ReceiveDataChild(char *rbuf, int *rlen, uint16_t UartRevLength, uint8_t *UartBuf)
bool Gsm_ReceiveData(char *rbuf, int *rlen, uint16_t UartRevLength, uint8_t *UartBuf)

{
	//���в�����ְ�������ͬ��Ϊ�˺ͷְ����ճ��򱣳�һ��
	int Mark_Buf[10] = {0}; //��¼�����ַ���"+IPD,"ÿ�������ڻ������е�����λ��
	int Mark_Cnt = 0; //��¼�����ַ���"+IPD,"���ִ���
	int Mark_Colon_Buf[10] = {0}; //��¼�����ַ���":"ÿ�������ڻ������е�����λ��
	char SubPackageLength[10][6] = { {0}, {0}, {0}}; //��¼ÿ���ְ������ݳ��� (�ַ�����ʽ)
	int SubLength[10] = {0}; //��¼ÿ���ְ������ݳ���(������ʽ)
	int length_1[10]={0},i=0;
////////////	int length_1=0, length_2=0, length_3=0;
	  *rlen=0;
	//Ѱ�ҽ��ձ�־��
	Mark_Cnt = Find_SubStringEx((char*)UartBuf, "+IPD,", UartRevLength, Mark_Buf);
	if(Mark_Cnt == 0) //ע�⣬����bin�ļ���Ậ��+IPD���ʲ��ܲ��÷ְ���ʽ�б�+IPD
	{
		return false;
	}
	
	for(i=0;i<Mark_Cnt;i++)
	{
	  	//Ѱ�����ݳ��Ƚ�����
	  if(Find_SubStringEx((char*)UartBuf+Mark_Buf[i], ":", 5, Mark_Colon_Buf) == 1)
		{
		   length_1[i]= Mark_Colon_Buf[0] - 1;
				  //ת���õ��������ݵĳ���
	     memcpy(&(SubPackageLength[i]), (char*)UartBuf+Mark_Buf[i], length_1[i]);
	     SubLength[i] = atoi(SubPackageLength[i]);
	     *rlen += SubLength[i];

			if(i==0)
			{
			  //�����ڻ��������ݷŵ���������
	     memcpy(rbuf, (char*)UartBuf+Mark_Buf[i]+Mark_Colon_Buf[0], SubLength[i]);
			}
			else
			{
			  //�����ڻ��������ݷŵ���������
	     memcpy((char*)rbuf+SubLength[0], (char*)UartBuf+Mark_Buf[i]+Mark_Colon_Buf[0], SubLength[i]);
			
			}
			
		}
	  	
	}
	return true;
}



//��������
u8 Gsm_Rev_data(char *rbuf, int *rlen, u16 waittime, uint8_t *UartBuf)
{
	u8 res=1;
	
	if(waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			/*�����յ��������Ƿ�Ϸ�*/
			if(Gsm_ReceiveData(rbuf, rlen, GSM_RX_BUF_LEN, UartBuf))
			{
				if(Gsm_RevData_Check(rbuf, *rlen))
				{
					break;
				}
			}
			GSM_ESP8266_DelayMS(10); //��ʱ10ms
		}
		if(waittime==0)
		{
			res=0; 
		}
	}
	return res;
} 






#endif
/**********************************************************************/

