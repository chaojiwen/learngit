#include "Gsm_Api.h"
GSM_HANDLE  gsm_handle; //����GSMͨѶ���
#if (GSM_Module_Type==0) //SIM800Cģ��
#include "GSM.h" //���gsmģ��ײ�ͷ�ļ�(����SIM800C,SIM7020C��ģ��ĵײ���������)
static bool GSM_AskCSQ(uint8_t *csq)//��ȡ�ź�ǿ��
{
	 bool  ret=false;
	if(Gsm_AskCSQ(csq,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
  return ret;
}
static bool GSM_Open_Connect(char *ServerName,char *ServerPort)//��TCP����������
{
    bool  ret=false;   

  if(Gsm_Open_Connect_Srever(ServerName,ServerPort,Open_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;
	 
}
static bool GSM_Close_Connect(void)//�ر�TCP����������
{
    bool  ret=false;   

  if(Gsm_Close_Connect_Srever((u8*)GSM_RX_BUF))
	{
	     ret=true;
	 }
    return ret;
}
static bool GSM_Send_Data(char *wbuf, int wlen)//��������
{
   bool  ret=false;    

  if(Gsm_send_data(wbuf,wlen,SendData_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	 }
    return ret;

}
static bool GSM_Rev_Data(char *rbuf, int *rlen)//��������
{
   bool  ret=false;    
	
  if(Gsm_Rev_data(rbuf,rlen,SendData_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	 }
    return ret;

}
//���Ͷ��ţ�֧����Ӣ��,����ΪGBK�룩
/**
 * @brief  gsm_sms ���Ͷ��ţ�֧����Ӣ��,����ΪGBK�룩
 * @param  num: �绰���룬smstext����������
 * @retval ��
 */
static bool GSM_Send_Sms(char *num,char *smstext)//���Ͷ�����������
{
    bool  ret=false;
	if(Gsm_Send_Sms(num,smstext,(u8*)GSM_RX_BUF))
	{
	    ret=true;
	}

return   ret;


}
static void GSM_IO_Init(void)//GSMģ��IO��ʼ��
{
     Gsm_Io_Init();
}
static void GSM_Power_Down(void)//�ر�GSMģ���Դ
{
     Gsm_Power_Down();//�ر�GSMģ���Դ
    
}
static void GSM_Power_Up(void)//��GSMģ���Դ	
{
     Gsm_Power_Up();//��GSMģ���Դ	  
}
//GSMģ���������
static void GSM_Module_Sleep(void)
{
//	Gsm_Module_Sleep();
}

//GSMģ�黽��
static void GSM_Module_WakeUp(void)
{
//	Gsm_Module_WakeUp();
}


static void GSM_Usart_Init(u32 bound)//GSMģ�鴮�ڳ�ʼ��
{
	   GSM_USART_Init(bound);//GSMģ�鴮�ڳ�ʼ��
}
static bool GSM_Check_Sim_Card(void)//���SIM���Ƿ���λ
{
	  bool  ret=false;    
  if(Gsm_Check_SIm_Card((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;
}

static bool GSM_CheckNetwork(void)//���SIM���Ƿ��Ѿ�ע����������
{
	 bool  ret=false;    
  if(Gsm_CheckNetwork((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;

}
static bool GSM_CheckGPRSActive(void)//���SIM���Ƿ��Ѿ���������GPRS����
{
	 bool  ret=false;    
  if(Gsm_CheckGPRSActive((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;

}
static bool GSM_Power_Rest(void)//����GSMģ��
{	 
	  bool  ret; 
	  ret=true; 
    Gsm_Power_Rest();//����GSMģ��  //���ڿ��Ը��ݼ��GSMģ��Ŀ��������ж�����ģ���Ƿ�ɹ�
	 return ret;
}
static bool GSM_send_cmd(char *cmd,char *ack,u16 waittime)
{
    bool  ret=false; 
   if(Gsm_send_cmd(cmd,ack,waittime,(u8*)GSM_RX_BUF))
	 {
	     ret=true;
	 }
    return ret;


}

static bool GSM_Echo(bool flag)//�Ƿ�����GSM���� flag:true������� flag:false��������
{	 
	  bool  ret=false; 
	if(flag==true)
	{
		 if(Gsm_Echo(true,(u8*)GSM_RX_BUF))      ret=true;   
	  
	}
	else
	{
	    if(Gsm_Echo(false,(u8*)GSM_RX_BUF))     ret=true;   
	
	}
	 return ret;
}

static bool  GSM_Send_AT(void)//��ģ�鷢��"AT"
{
  bool  ret=false;
	
	if(Gsm_Send_AT((u8*)GSM_RX_BUF))           ret=true;  
	
	 return ret;
}

#endif
#if (GSM_Module_Type==1) //SIM7020Cģ��

//����й�SIM7020Cģ��Ľӿڴ���

#endif

#if (GSM_Module_Type==2) //ESP8266Wifiģ��
#include "GSM_esp8266.h" //���gsmģ��ײ�ͷ�ļ�(����SIM800C,SIM7020C��ģ��ĵײ���������)
/************************** ��ģ����ƽӿ� **************************/
//GSMģ��IO��ʼ��
static void GSM_IO_Init(void)
{
	Gsm_Io_Init();
}

//�ر�GSMģ��
static void GSM_Power_Down(void)
{
	Gsm_Power_Down();
}

//��GSMģ��	
static void GSM_Power_Up(void)
{
	Gsm_Power_Up();
}

//GSMģ���������
static void GSM_Module_Sleep(void)
{
	Gsm_Module_Sleep();
}

//GSMģ�黽��
static void GSM_Module_WakeUp(void)
{
	Gsm_Module_WakeUp();
}

//GSMģ�鴮�ڳ�ʼ��
static void GSM_Usart_Init(u32 bound)
{
	GSM_USART_Init(bound);
}

//����GSMģ��
static bool GSM_Power_Rest(void)
{	 
	bool ret; 
	ret = true; 
	Gsm_Power_Rest(); //���ڿ��Ը��ݼ��GSMģ��Ŀ��������ж�����ģ���Ƿ�ɹ�
	return ret;
}
/**********************************************************************/


/************************** ��ģ�������ӿ� **************************/
//�����ϵ緢��AT
static bool GSM_Send_AT(void)//��ģ�鷢��"AT"
{
	bool ret = false;

	if(Gsm_Send_AT((u8*)GSM_RX_BUF))
		ret=true;  

	return ret;
}

//ȡ������
static bool GSM_Echo(bool flag) //�Ƿ�����GSM���� flag:true������� flag:false��������
{	 
	bool ret = false; 
	
	if(flag==true)
	{
		 if(Gsm_Echo(true,(u8*)GSM_RX_BUF))
			 ret=true;   
	}
	else
	{
	    if(Gsm_Echo(false,(u8*)GSM_RX_BUF))
			ret=true;   
	}
	
	 return ret;
}

//���;����ATָ��
static bool GSM_send_cmd(char *cmd, char *ack, u16 waittime)
{
	bool ret = false; 
	
	if(Gsm_send_cmd(cmd, ack, waittime, (u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	
	return ret;
}

//��ȡ�ź�ǿ��     //��ѯ�ź�ǿ��RSSI    �źŷ�Χ0~31      0�����ź����        31�����ź���ǿ   
static bool GSM_AskCSQ(uint8_t *csq)
{
	bool ret = false;
	int rssi;
	
	if(Gsm_AskRssi(&rssi, (u8*)GSM_RX_BUF))
	{
		rssi = (rssi+113)/2; //��RSSIת����CSQ
		*csq = rssi;
/**********WIFIģ���ź��޸�**************/		
   	if(*csq>=31){
   			*csq=31;
   	}
   	if(*csq<1){
   			*csq=1;
   	}
/***************************************/		
		ret = true;
	}
	
	return ret;
}

//����Ƿ��Ѿ�ע����������
static bool GSM_CheckNetwork(void)
{
	bool ret = false;  
	int rssi;
	
	if(Gsm_AskRssi(&rssi, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//����WIFIģʽ
static bool GSM_SetWifiMode(void)
{
	bool ret = false;  
	
	//Ŀǰ����ΪStationģʽ
	if(Gsm_SetWifiMode_Station((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//����AP����
static bool GSM_OpenConnectAP(char *SSID, char *password)
{
	bool ret = false;  
	
	if(GSM_SetWifiMode())//����Wifiģ�鴦��Stationģʽ
	{
	  	//ĿǰΪStationģʽ�¼���AP����
	   if(Gsm_OpenConnectAP(SSID, password, (u8*)GSM_RX_BUF))
	   {
	   	ret = true;
	   }
	}

	return ret;
}

//�Ͽ�AP����
static bool GSM_CloseConnectAP(void)
{
	bool ret = false;  
	
	//ĿǰΪStationģʽ�¶Ͽ�AP����
	if(Gsm_CloseConnectAP((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//��ѯ����������Ϣ
static bool GSM_CheckNetStatus(uint8_t *Status)
{
	bool ret = false;  
	
	if(Gsm_CheckStatus(Status, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//����IP��ַ�Ƿ�̬��ȡ
static bool GSM_SetDHCP(bool flag)
{
	bool ret = false;  
	
	if(Gsm_StationMode_SetDHCP(flag, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//���þ�̬IP
static bool GSM_SetStaticIP(char *ip, char *gateway, char* netmask)
{
	bool ret = false;  
	
	if(Gsm_StationMode_SetStaticIP(ip, gateway, netmask, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//��������ģʽ
static bool GSM_SetSingleConnect(void)
{
	bool ret = false;  
	
	//Ŀǰ����Ϊ������
	if(Gsm_SetSingleConnect((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//���ô���ģʽ
static bool GSM_SetTransmittionMode(void)
{
	bool ret = false;  
	
	//Ŀǰ����Ϊ��͸��ģʽ
	if(Gsm_SetTransmittionMode((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//��ѯ����IP��ַ
static bool GSM_AskLocalIPAddress(char *station_ip)
{
	bool ret = false;  
	
	//Ŀǰ��Stationģʽ�²��ұ���IP
	if(Gsm_CheckLocalIPAddress(station_ip, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//��ѯ��̬IP��ַ�Ȳ���
static bool GSM_AskCipStatus(char *station_ip, char *gateway, char *netmask)
{
	bool ret = false;  
	
	//Ŀǰ���ҵĴ���Stationģʽ�µ�
	if(Gsm_CheckCipStatus(station_ip, gateway, netmask, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//��ѯMAC��ַ
static bool GSM_AskMacAddress(char *MacAddress)
{
	bool ret = false;  
	
	if(Gsm_AskStationMacAddress(MacAddress, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//�ָ���������
static bool GSM_Restore(void)
{
	bool ret = false;  
	
	if(Gsm_Restore((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//����ǳ˯��ģʽ
//���������ǳ˯�ߣ����ѽ�Ϊ�͵�ƽ���ѣ���ô������ѽŴ��ڸߵ�ƽ����ģ���������˯�ߣ�ֱ�����ѽ�Ϊ�͵�ƽ
//ע�⣬ģ��տ���һ��ʱ��12s���Ҳ��ܽ���͹��ģ�
static bool GSM_LightSleep(bool flag)
{
	bool ret = false;  
	
//	if(Gsm_Into_LightSleep(flag, (u8*)GSM_RX_BUF))
//	{
//		ret = true;
//	}
	
	return ret;
}

//���û���ģ��ǳ˯�ߵ�����
static bool GSM_SetLightSleepWakeUpPin(uint8_t pin, uint8_t level)
{
	bool ret = false;  
	
//	//���ѽ�Ϊģ���GPIO5���͵�ƽ����
//	if(Gsm_Set_LightSleepWakeUpPin(5, 0,(u8*)GSM_RX_BUF))
//	{
//		ret = true;
//	}
	
	return ret;
}

//��ȡģ���ȡIP��ַ��ʽ
static bool GSM_AskDHCP(bool *flag)
{
	bool ret = false;  
	
	if(Gsm_AskDHCP(flag, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}
//����WIFIģ��
static bool GSM_Rest(void)
{
	bool ret = false;  
	
	if(Gsm_Rest((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

/**********************************************************************/


/**************************** ��ͨ�Žӿ� ****************************/
//��TCP����������
static bool GSM_Open_Connect(char *ServerName, char *ServerPort)
{
	bool ret = false;   
	
	if(Gsm_ClientConnectTCP(ServerName, ServerPort, (u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	
	return ret;
}

//�ر�TCP����������
static bool GSM_Close_Connect(void)
{
	bool ret = false;   

	if(Gsm_ClientCloseTCP((u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	return ret;
}

//��������
static bool GSM_Send_Data(char *wbuf, int wlen)
{
	bool ret = false;    

	if(Gsm_send_data(wbuf, wlen, SendData_Server_AT_WaitTime_n10ms, (u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	
	return ret;
}

//��������
static bool GSM_Rev_Data(char *rbuf, int *rlen)
{

	bool ret = false;    
	
	if(Gsm_Rev_data(rbuf, rlen, SendData_Server_AT_WaitTime_n10ms, (u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	return ret;
}
#endif
#if (GSM_Module_Type==3)//����MG2639ģ�� 
#include "GSM_MG2639.h"
static bool GSM_AskCSQ(uint8_t *csq)//��ȡ�ź�ǿ��
{
	 bool  ret=false;
	if(GSM_MG2639_AskCSQ(csq,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
  return ret;
}
static bool GSM_Echo(bool flag)//�Ƿ�����GSM���� flag:true������� flag:false��������
{	 
	  bool  ret=false; 
	if(flag==true)
	{
		 if(GSM_MG2639_Echo(true,(u8*)GSM_RX_BUF))      ret=true;   
	  
	}
	else
	{
	   if(GSM_MG2639_Echo(false,(u8*)GSM_RX_BUF))     ret=true;   
	
	}
	 return ret;
}

static void GSM_IO_Init(void)//GSMģ��IO��ʼ��
{
      GSM_MG2639_Io_Init();
}
static void GSM_Power_Down(void)//�ر�GSMģ���Դ
{
      GSM_MG2639_Power_Down();//�ر�GSMģ���Դ
    
}
static void GSM_Power_Up(void)//��GSMģ���Դ	
{
      GSM_MG2639_Power_Up();//��GSMģ���Դ	  
}
static void GSM_Usart_Init(u32 bound)//GSMģ�鴮�ڳ�ʼ��
{
	   GSM_USART_Init(bound);//GSMģ�鴮�ڳ�ʼ��
}
static bool GSM_Power_Rest(void)//����GSMģ��
{	 
	  bool  ret; 
	  ret=true; 
    GSM_MG2639_Power_Rest();//����GSMģ��  //���ڿ��Ը��ݼ��GSMģ��Ŀ��������ж�����ģ���Ƿ�ɹ�
	 return ret;
}
//GSMģ���������
static void GSM_Module_Sleep(void)
{
//	Gsm_Module_Sleep();
}

//GSMģ�黽��
static void GSM_Module_WakeUp(void)
{
//	Gsm_Module_WakeUp();
}
static bool GSM_send_cmd(char *cmd,char *ack,u16 waittime)
{
    bool  ret=false; 
   if(GSM_MG2639_send_cmd(cmd,ack,waittime,(u8*)GSM_RX_BUF))
	 {
	     ret=true;
	 }
    return ret;


}
static bool GSM_Open_Connect(uint8_t ServerID,char *ServerName,char *ServerPort)//��TCP����������
{
    bool  ret=false;   

  if(GSM_MG2639_Open_Connect_Srever(ServerID,ServerName,ServerPort,Open_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;
	 
}
static bool GSM_Close_Connect(void)//�ر�TCP����������
{
    bool  ret=false;   

////  if(Gsm_Close_Connect_Srever((u8*)GSM_RX_BUF))
////	{
////	     ret=true;
////	 }
    return ret;
}
static bool GSM_Send_Data(uint8_t ServerID,char *wbuf, int wlen)//��������
{
   bool  ret=false;    

  if(GSM_MG2639_send_data(ServerID,wbuf,wlen,SendData_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	 }
    return ret;

}
static bool GSM_Rev_Data(char *rbuf, int *rlen)//��������
{
   bool  ret=false;    
	
////  if(Gsm_Rev_data(rbuf,rlen,SendData_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
////	{
////	     ret=true;
////	 }
    return ret;

}
static bool GSM_Check_Sim_Card(void)//���SIM���Ƿ���λ
{
	  bool  ret=false;    
  if(GSM_MG2639_Check_SIm_Card((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;
}

static bool GSM_CheckNetwork(void)//���SIM���Ƿ��Ѿ�ע����������
{
	 bool  ret=false;    
  if(GSM_MG2639_CheckNetwork((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;

}
static bool GSM_CheckGPRSActive(void)//���SIM���Ƿ��Ѿ���������GPRS����
{
	 bool  ret=false;    
  if(GSM_MG2639_CheckGPRSActive((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;

}
static bool  GSM_Send_AT(void)//��ģ�鷢��"AT"
{
  bool  ret=false;
	
	if(GSM_MG2639_Send_AT((u8*)GSM_RX_BUF))           ret=true;  
	
	 return ret;
}
static int	 GSM_Adaptive_BaudRate(void)  /* ����Ӧ������ */
{
   int  BaudRate;
	BaudRate=GSM_MG2639_Adaptive_BaudRate((u8*)GSM_RX_BUF);
    return   BaudRate;
}
static bool	GSM_SetBaudRate(int br)//����ģ�鲨����
{
  bool  ret=false;
	 if(GSM_MG2639_SetBaudRate(br,(u8*)GSM_RX_BUF))//����ģ�鲨����
	 {
	     ret=true;
	 }
  return   ret;
}


//���Ͷ��ţ�֧����Ӣ��,����ΪGBK�룩
/**
 * @brief  gsm_sms ���Ͷ��ţ�֧����Ӣ��,����ΪGBK�룩
 * @param  num: �绰���룬smstext����������
 * @retval ��
 */
static bool GSM_Send_Sms(char *num,char *smstext)//���Ͷ�����������
{
    bool  ret=false;
////	if(Gsm_Send_Sms(num,smstext,(u8*)GSM_RX_BUF))
////	{
////	    ret=true;
////	}

return   ret;


}

//����й�����MG2639ģ��Ľӿڴ���

#endif
#if (GSM_Module_Type==4) //����1ģ��

//����йش���1ģ��Ľӿڴ���

#endif
#if (GSM_Module_Type==5) //����2ģ��

//����йش���2ģ��Ľӿڴ���

#endif

	//GSMͨ�Žӿڳ�ʼ��
bool GSM_Transmittion_Init(GSM_HANDLE *pHandle)
{
		//��ģ����ƽӿ�
		pHandle->pGSM_IO_Init=GSM_IO_Init;//GSMģ��IO��ʼ��
		pHandle->pGSM_Usart_Init=GSM_Usart_Init;//GSMģ�鴮�ڳ�ʼ��
		pHandle->pGSM_Power_Rest=GSM_Power_Rest;//����GSMģ��
		pHandle->pGSM_Power_Down=GSM_Power_Down;//�ر�GSMģ���Դ
	  pHandle->pGSM_Power_Up=GSM_Power_Up;//��GSMģ���Դ
		pHandle->pGSM_Sleep = GSM_Module_Sleep; //GSMģ�����˯��
	  pHandle->pGSM_WakeUp = GSM_Module_WakeUp; //GSMģ�黽��
	
		//��ģ�������ӿ�
    #if (GSM_Module_Type==0) //SIM800Cģ��
	  //ע���й�SIM800Cģ��Ľӿڴ���
		pHandle->sim800c.pGSM_Echo=GSM_Echo;//����ģ���Ƿ�رջ��Թ���
		pHandle->sim800c.pGSM_Send_AT=GSM_Send_AT;//�򴮿ڷ���"AT"
		pHandle->sim800c.pGSM_CSQ = GSM_AskCSQ;//��ȡ�ź�ǿ��
		pHandle->sim800c.pGSM_CheckNetwork=GSM_CheckNetwork;//���SIM���Ƿ�ע����������
		pHandle->sim800c.pGSM_CheckGPRSActive=GSM_CheckGPRSActive;//���SIM���ĸ���̬
		pHandle->sim800c.pGSM_Check_Sim_Card=GSM_Check_Sim_Card;//���SIM���Ƿ���λ
	  pHandle->sim800c.pGSM_Send_Sms=GSM_Send_Sms;//���Ͷ�������
	
		 	//��ͨ�Žӿ�
		pHandle->pGSM_send_cmd=GSM_send_cmd;//�򴮿ڷ���ATָ��
	  pHandle->pGSM_Open_Connect=GSM_Open_Connect;//��TCP����
	  pHandle->pGSM_CloseConnect=GSM_Close_Connect;//�ر�TCP����
	  pHandle->pGSM_SendData=GSM_Send_Data;//���������������
		pHandle->pGSM_RevData=GSM_Rev_Data;//���������������
	  #endif
	  #if (GSM_Module_Type==1) //SIM7020Cģ��
   //ע���й�SIM7020Cģ��Ľӿڴ���
    #endif
    #if (GSM_Module_Type==2) //ESP8266Wifiģ��
	  //��ģ�������ӿ�
	  pHandle->esp8266.pGSM_Echo = GSM_Echo; //���û���ģʽ ��true�����ԣ�false�ػ��ԣ�
	  pHandle->esp8266.pGSM_Send_AT = GSM_Send_AT; //�����ϵ緢��AT 
	  pHandle->esp8266.pGSM_CSQ = GSM_AskCSQ; //��ȡ�ź�ǿ��
	  pHandle->esp8266.pGSM_CheckNetwork = GSM_CheckNetwork; //���SIM���Ƿ�ע����������
	  
	  pHandle->esp8266.pGSM_SetWifiMode = GSM_SetWifiMode; //����WIFIģʽ
	  pHandle->esp8266.pGSM_OpenConnectAP = GSM_OpenConnectAP; //����AP����
	  pHandle->esp8266.pGSM_CloseConnectAP = GSM_CloseConnectAP; //�Ͽ�AP����
	  pHandle->esp8266.pGSM_CheckNetStatus = GSM_CheckNetStatus; //��ѯ����������Ϣ
	  pHandle->esp8266.pGSM_SetDHCP = GSM_SetDHCP; //����IP��ַ�Ƿ�̬��ȡ
	  pHandle->esp8266.pGSM_SetStaticIP = GSM_SetStaticIP; //���þ�̬IP
	  pHandle->esp8266.pGSM_SetSingleConnect = GSM_SetSingleConnect; //����������
	  pHandle->esp8266.pGSM_SetTransmittionMode = GSM_SetTransmittionMode; //���ô���ģʽ��ĿǰΪ��͸����
	  pHandle->esp8266.pGSM_AskLocalIPAddress = GSM_AskLocalIPAddress; //��ѯ����IP��ַ(Stationģʽ��)
	  pHandle->esp8266.pGSM_AskCipStatus = GSM_AskCipStatus; //��ѯ�Ѿ�����stationģʽ�µľ�̬IP��ַ�Ȳ���
	  pHandle->esp8266.pGSM_AskMacAddress = GSM_AskMacAddress; //��ѯMAC��ַ 
		pHandle->esp8266.pGSM_Restore = GSM_Restore; //�ָ���������
	  pHandle->esp8266.pGSM_LightSleep = GSM_LightSleep; //����ǳ˯��ģʽ
	  pHandle->esp8266.pGSM_SetLightSleepWakeUpPin = GSM_SetLightSleepWakeUpPin; //���û���ģ��ǳ˯�ߵ�����
		pHandle->esp8266.pGSM_AskDHCP = GSM_AskDHCP; //��ѯDHCP״̬(Stationģʽ��)
		pHandle->esp8266.pGSM_Rest=GSM_Rest;//����Wifiģ��
			 	//��ͨ�Žӿ�
		pHandle->pGSM_send_cmd=GSM_send_cmd;//�򴮿ڷ���ATָ��
	  pHandle->pGSM_Open_Connect=GSM_Open_Connect;//��TCP����
	  pHandle->pGSM_CloseConnect=GSM_Close_Connect;//�ر�TCP����
	  pHandle->pGSM_SendData=GSM_Send_Data;//���������������
		pHandle->pGSM_RevData=GSM_Rev_Data;//���������������

    #endif
    #if (GSM_Module_Type==3)//����MG2639ģ�� 
    pHandle->mg2639.pGSM_Echo=GSM_Echo;//����ģ���Ƿ�رջ��Թ���
		pHandle->mg2639.pGSM_Send_AT=GSM_Send_AT;//�򴮿ڷ���"AT"ָ��
		pHandle->mg2639.pGSM_send_cmd=GSM_send_cmd;//��ģ�鷢��ATָ��
		pHandle->mg2639.pGSM_CSQ=GSM_AskCSQ;//��ȡ�ź�ǿ��
		pHandle->mg2639.GSM_Adaptive_BaudRate=GSM_Adaptive_BaudRate;/* ����Ӧ������ */
		pHandle->mg2639.GSM_SetBaudRate=GSM_SetBaudRate;//����ģ��Ĳ�����
		pHandle->mg2639.pGSM_CheckNetwork=GSM_CheckNetwork;//���SIM���Ƿ�ע����������
		pHandle->mg2639.pGSM_CheckGPRSActive=GSM_CheckGPRSActive;//���SIM���ĸ���̬
		pHandle->mg2639.pGSM_Check_Sim_Card=GSM_Check_Sim_Card;//���SIM���Ƿ���λ
	  pHandle->mg2639.pGSM_Send_Sms=GSM_Send_Sms;//���Ͷ�������
		
		//��ͨ�Žӿ�
	  pHandle->mg2639.pGSM_Open_Connect=GSM_Open_Connect;//��TCP����
	  pHandle->pGSM_CloseConnect=GSM_Close_Connect;//�ر�TCP����
	  pHandle->mg2639.pGSM_SendData=GSM_Send_Data;//���������������
		pHandle->pGSM_RevData=GSM_Rev_Data;//���������������
    //ע���й�����MG2639ģ��Ľӿڴ���
    #endif
    #if (GSM_Module_Type==4) //����1ģ��
    
    //ע���йش���1ģ��Ľӿڴ��� 
    #endif
    #if (GSM_Module_Type==5) //����2ģ��
    //ע���йش���2ģ��Ľӿڴ��� 
    #endif
	  return true;
}   





