#include "GSM_app.h"
#include "interface.h"
#include "Gsm_Interface.h"
#include "Gsm_Api.h"
#include "stdbool.h"
#include "ucos_ii.h"
#include "Mcu_Config.h"
#include "string.h"
#include "Log_GSM.h"

#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	
#include "Log_GSM_Config.h"
#endif

BOOL DEBUG_Flag = TRUE;//���������Ϣ������Ʊ�־λ  GSM_DEBUG_Flag = FALSE�����򿪵�����Ϣ���  GSM_DEBUG_Flag = TRUE����򿪵�����Ϣ���

extern  GSM_GPRS_SR_Struct GPRS_SR_Reg;



GSM_GPRS_Connection    GSM_Tcp_Conn[GSM_MAX_NUM_TCP_CONN];// �豸Ҫ���ʵ�tcp��������������



bool Sim800c_env_init(_SCSStruct * state)
{
#if (GSM_Module_Type==0x00) //SIM800Cģ��
	  u8 GsmCsq=0;//gsmģ���ź�ǿ��
		u8 times = 0;//ִ���������
	  bool  ret=false;
    GSM_Transmittion_Init(&gsm_handle);//GSMģ����ؽӿں���ע�� 
  	gsm_handle.pGSM_IO_Init();//GSMģ�����IO�ڳ�ʼ��	
    gsm_handle.pGSM_Usart_Init(115200);//GSMģ�鴮�ڳ�ʼ��	
		gsm_handle.pGSM_Power_Down();//��GSMģ����йػ����� 
    gsm_handle.pGSM_Power_Up();//��GSMģ����п�������      
	  OSTimeDly(5000);//��ʱ5s	
		 gsm_handle.pGSM_send_cmd("ATE0","OK",1000);//�رջ���
		 gsm_handle.pGSM_send_cmd("AT+CIPHEAD=0","OK",1000);//�رս�������ʱ��ʾ+IPDͷ
	//1�����SIM���Ƿ���λ
    if(gsm_handle.sim800c.pGSM_Check_Sim_Card())	
		{
		    Log_printf("��⵽SIM��������\n");	  
		}
		else
		{
			 Log_printf("δ��⵽SIM��������\n");	
			 /* ���ÿ�δ�����Ϣ */
			 *state	= _SIM_NotDetected;
			 goto dev_gsm_init_err;

		}
		//2�����ģ���ź�ǿ��
		 times = 0;//ִ���������
		 ret=false;
	   do{
			    OSTimeDly(3000);//��ʱ3s	
			   if(gsm_handle.sim800c.pGSM_CSQ(&GsmCsq)&&GsmCsq>0)
	       {
					  ret=true;
					  break;
	       }
				 else
				 {
				   ret=false;
				 }	
  				 
		   }while(times++<20);//���ɹ��������1����
			 if(!ret)//��ȡ�ź�ǿ��ʧ��
		   {
				  Log_printf("ģ�����źţ�����\n");
			   	/* �����������ź�״̬ */
			   *state	= _SIM_NoSignal;
				  goto dev_gsm_init_err;  	 
		   }
		   //3�����ģ���Ƿ��Ѿ�ע����������
		   times = 0;//ִ���������
		   ret=false;
		   do{
				    OSTimeDly(3000);//��ʱ3s	 
			      if(gsm_handle.sim800c.pGSM_CheckNetwork())
	          {
					     ret=true;
					     break;
	          }
				    else
				    {
				      ret=false;
				    }
		   }while(times++<100);//���ɹ��������5����
			 if(!ret)//��ȡ�ź�ǿ��ʧ��
		   {
				  Log_printf("ģ���޷�ע�������磡����\n");
			    /* �����������ź�״̬ */
		    	*state	= _SIM_NoSignal;	
				  goto dev_gsm_init_err;  	 
		   }
		   //4�����ģ���Ƿ��Ѿ�������������
		   times = 0;//ִ���������
		   ret=false;
		   do{
				    OSTimeDly(3000);//��ʱ3s	 
			      if(gsm_handle.sim800c.pGSM_CheckGPRSActive())
	          {
					     ret=true;
					     break;
	          }
				    else
				    {
				      ret=false;
				    }
		   }while(times++<100);//���ɹ��������5����
			 if(!ret)//��ȡ�ź�ǿ��ʧ��
		   {
				  Log_printf("ģ���޷����������磡����\n");
				 
		    	/* ���ÿ�Ƿ��ͣ��״̬ */
			    *state	= _SIM_ShutDown;
				  goto dev_gsm_init_err;  	 
		   }
			 
			 //5��ģ���ʼ���ɹ�
	     Log_printf(">>>��ʼ��GSMģ��ɹ�������\r\n");
		  /* ���ÿ�δ�����Ϣ */
	     *state	= _SIM_OK;
       return true;	
dev_gsm_init_err:
			   gsm_handle.pGSM_Power_Down();//��GSMģ����йػ�
         return false;		 
#endif		 
#if (GSM_Module_Type==0x03) //����MG2639ģ��
		u8 GsmCsq=0;//gsmģ���ź�ǿ��
		u8 times = 0;//ִ���������
	  int 	baud_rate;   //GSMģ�鲨����	 
	  bool  ret=false;
    GSM_Transmittion_Init(&gsm_handle);//GSMģ����ؽӿں���ע�� 
  	gsm_handle.pGSM_IO_Init();//GSMģ�����IO�ڳ�ʼ��	
    gsm_handle.pGSM_Usart_Init(115200);//GSMģ�鴮�ڳ�ʼ��	
		gsm_handle.pGSM_Power_Down();//��GSMģ����йػ�	 
    gsm_handle.pGSM_Power_Up();//��GSMģ����п�������      
	  OSTimeDly(5000);//��ʱ5s	
		//1�����ģ��Ĳ������Ƿ�Ϊ9600   ����������Ҫ���¶����������
		baud_rate=gsm_handle.mg2639.GSM_Adaptive_BaudRate();
    if(baud_rate!=115200)	
		{
			Log_printf("��ǰMG2639ģ�鲨����Ϊ:%d     ��Ҫ��������ģ��Ĳ�����\n",baud_rate);
		    if(!gsm_handle.mg2639.GSM_SetBaudRate(115200))
				{
					Log_printf("����MG2639ģ�鲨����ʧ�ܣ�����\n");
			      	 /* ���ÿ�δ�����Ϣ */
			       *state	= _SIM_NotDetected;
			       goto dev_gsm_init_err;
				}
		
		}	
		//2������ģ����Է�ʽ
		if(gsm_handle.mg2639.pGSM_send_cmd("ATE1","OK",1000))//�رջ���
		{
		  Log_printf("����MG2639ģ����Թ��ܳɹ�������\n");
		}			
	//3�����SIM���Ƿ���λ
    if(gsm_handle.mg2639.pGSM_Check_Sim_Card())	
		{
		    Log_printf("MG2639ģ���⵽SIM��������\n");	  
		}
		else
		{
			 Log_printf("MG2639ģ��δ��⵽SIM��������\n");	
			 /* ���ÿ�δ�����Ϣ */
			 *state	= _SIM_NotDetected;
			 goto dev_gsm_init_err;

		}
			//3�����ģ���ź�ǿ��
		 times = 0;//ִ���������
		 ret=false;
	   do{
			    OSTimeDly(3000);//��ʱ3s	
			   if(gsm_handle.mg2639.pGSM_CSQ(&GsmCsq)&&GsmCsq>0)
	       {
					 Log_printf("��ѯMG2639ģ���ź�ǿ�ȳɹ�������  ��ǰMG2639ģ����ź�ǿ��Ϊ:%d\n",GsmCsq);
					  ret=true;
					  break;
	       }
				 else
				 {
				   ret=false;
				 }	
  				 
		   }while(times++<20);//���ɹ��������1����
			 if(!ret)//��ȡ�ź�ǿ��ʧ��
		   {
				  Log_printf("MG2639ģ�����źţ�����\n");
			   	/* �����������ź�״̬ */
			   *state	= _SIM_NoSignal;
				  goto dev_gsm_init_err;  	 
		   }	
		//4�����ģ���Ƿ��Ѿ�ע����������
		   times = 0;//ִ���������
		   ret=false;
		   do{
				    OSTimeDly(3000);//��ʱ3s	 
			      if(gsm_handle.mg2639.pGSM_CheckNetwork())
	          {
							 Log_printf("MG2639ģ���Ѿ��ɹ�ע�������磡����\n");
					     ret=true;
					     break;
	          }
				    else
				    {
				      ret=false;
				    }
		   }while(times++<100);//���ɹ��������5����
			 if(!ret)//��ȡ�ź�ǿ��ʧ��
		   {
				  Log_printf("MG2639ģ���޷�ע�������磡����\n");
			    /* �����������ź�״̬ */
		    	*state	= _SIM_NoSignal;	
				  goto dev_gsm_init_err;  	 
		   }
		
	 //5�����ģ���Ƿ��Ѿ�������������
		   times = 0;//ִ���������
		   ret=false;
		   do{
				    OSTimeDly(3000);//��ʱ3s	 
			      if(gsm_handle.mg2639.pGSM_CheckGPRSActive())
	          {
							Log_printf("MG2639ģ���Ѿ��ɹ����������磡����\n");
					     ret=true;
					     break;
	          }
				    else
				    {
				      ret=false;
				    }
		   }while(times++<100);//���ɹ��������5����
			 if(!ret)//��ȡ�ź�ǿ��ʧ��
		   {
				  Log_printf("MG2639ģ���޷����������磡����\n");
				 
		    	/* ���ÿ�Ƿ��ͣ��״̬ */
			    *state	= _SIM_ShutDown;
				  goto dev_gsm_init_err;  	 
		   }		 
      //5��ģ���ʼ���ɹ�
	     Log_printf(">>>��ʼ��MG2639ģ��ɹ�������\r\n");
		  /* ����SIM�����Ϣ */
	     *state	= _SIM_OK;
       return true;			
dev_gsm_init_err:
	 /* �����������ź�״̬ */
	 *state	= _SIM_NoSignal;	
   gsm_handle.pGSM_Power_Down();//��GSMģ����йػ�
      return false;				 	 
#endif			 
#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	 
    bool Wifi_IP_Get_Way;//Wifiģ���ȡIP��ʽ
		u8 times = 0;//ִ���������
	  bool  ret=false;
    GSM_Transmittion_Init(&gsm_handle);//GSMģ����ؽӿں���ע�� 
  	gsm_handle.pGSM_IO_Init();//GSMģ�����IO�ڳ�ʼ��	
    gsm_handle.pGSM_Usart_Init(115200);//GSMģ�鴮�ڳ�ʼ��	
    gsm_handle.pGSM_Power_Up();//��GSMģ����п������� 
	  OSTimeDly(5000);//��ʱ3s	
		
	  //1�����ģ�鹤��״̬�Ƿ�����
   if(gsm_handle.esp8266.pGSM_Send_AT())
	 {
	   Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State=1; 
      Log_printf("1��WIFIģ�鹤��״̬����������\n");	 			 
	 }
	 else
	 {
		 Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State=0;
	   Log_printf("1��WIFIģ�鹤��״̬���ϣ�����\n");			
	 }
	  //2���ر�Wifiģ�����
	 if(gsm_handle.esp8266.pGSM_Echo(true))
	 {
	   Log_printf("2���ر�Wifiģ����Գɹ�������\n");	 	
	 }
	 else
	 {
		  Log_printf("2���ر�Wifiģ�����ʧ�ܣ�����\n");	
	 }
	 
/**************************************************************************/	 
	 //3����Wifiģ������ΪStationģʽ
	 if(gsm_handle.esp8266.pGSM_SetWifiMode())
	 {
	   Log_printf("3����Wifiģ������ΪStationģʽ�ɹ�������\n");	 	
	 }
	 else
	 {
		  Log_printf("3����Wifiģ������ΪStationģʽʧ�ܣ�����\n");	
	 }
	 	//4����ѯWifiģ���MAC��ַ
	 	 times = 0;//ִ���������
	   do{	
			     if(gsm_handle.esp8266.pGSM_AskMacAddress(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address))
	         {
						 ret=true;
				     break;
	         }
			 	   else
				   {
				     ret=false;
				   } 
           OSTimeDly(10);//��ʱ10ms					  
		   }while(times++<5);//���ɹ��������5��
		 
			 if(ret)//��ѯWifiģ���MAC��ַ
		   {
				   Log_printf("4��Wifiģ���MAC��ַΪ:%s������\n",Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address);	
		   }
			 else
			 {
			     Log_printf("4����ѯWifiģ���MAC��ַʧ�ܣ�����\n");	
			 }

//�ж��Ƿ���Ҫ����Wifiģ�����
	 if(GSM_Config_Reg.GSM_Config_State==TRUE)
	 {
		      SetWifiPara();
		 GSM_Config_Reg.GSM_Config_State=FALSE;
	 }			 
			 
			 
	 	//5����ѯWifiWlan����
	 	 times = 0;//ִ���������
	   do{	
			     if(gsm_handle.esp8266.pGSM_AskCipStatus(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP ,Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask))
	         {
						 ret=true;
				     break;
	         }
			 	   else
				   {
				     ret=false;
				   } 
           OSTimeDly(10);//��ʱ10ms					  
		   }while(times++<5);//���ɹ��������5��
		 
			 if(ret)//��ѯWifiWlan����
		   {
				   Log_printf("5����ѯWifiWlan�����ɹ�������\n");	
		   }
			 else
			 {
			     Log_printf("5����ѯWifiWlan����ʧ�ܣ�����\n");	
			 }	

	 	//5����ѯģ���ȡIP��ַ��ʽ
	 	 times = 0;//ִ���������
	   do{	
			     if(gsm_handle.esp8266.pGSM_AskDHCP(&Wifi_IP_Get_Way))
	         {
						 Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way=Wifi_IP_Get_Way;
						 
						 ret=true;
				     break;
	         }
			 	   else
				   {
				     ret=false;
				   } 
           OSTimeDly(10);//��ʱ10ms					  
		   }while(times++<5);//���ɹ��������5��
		 
			 if(ret)//��ѯWifiWlan����
		   {
				   Log_printf("6����ѯģ���ȡIP��ַ��ʽ�ɹ�������\n");	
		   }
			 else
			 {
			     Log_printf("6����ѯģ���ȡIP��ַ��ʽʧ�ܣ�����\n");	
			 }

		 ret=true;
		 Log_printf("WIFIģ���ʼ���ɹ�������\n");	
		  /* ���ÿ�δ�����Ϣ */
	   *state	= _SIM_OK;		
    return  ret;	
#endif
			 
}
#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	 
//����Wifiģ�����
bool    SetWifiPara(void)
{
int                          times=0;
INSTRU_WIFI                  Current_Wifi_Fix_Tab;//��ǰWifi����״̬��Wlan�Ȳ���
   //��ǰGSMģ����Ҫ���ò���
	    if(GSM_Config_Reg.WifiNamePWD_Config_State==TRUE)
			{//����Wifiģ������ּ�����
				gsm_handle.esp8266.pGSM_OpenConnectAP(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name,
									                            Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password);
				
			  GSM_Config_Reg.WifiNamePWD_Config_State=FALSE;
			}		
			if(GSM_Config_Reg.WifiWLAN_Config_State==TRUE)
			{//����Wifiģ���WLAN����
				times=0;
							do{
								 gsm_handle.esp8266.pGSM_AskCipStatus(Current_Wifi_Fix_Tab.Inf.Wifi_Local_IP,
							                                     Current_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,
							                                     Current_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask);
								if((strcmp(Current_Wifi_Fix_Tab.Inf.Wifi_Local_IP,Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP)!=0)||\
								 (strcmp(Current_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP)!=0)||\
							   (strcmp(Current_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask)!=0))
								{
								    gsm_handle.esp8266.pGSM_SetStaticIP(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,
									                                 Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP ,
							                                     Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask  
							                                     );
								
								}else
								{
								   break;
								
								}
							   OSTimeDly(100);//��ʱ100ms		
							}while(times++<3);	
				
			 GSM_Config_Reg.WifiWLAN_Config_State=FALSE;
			}
		
			 OSTimeDly(100);//��ʱ10ms			
		 if(GSM_Config_Reg.WifiWLAN_GET_IP_WAY_Config_State==TRUE)
			{//����Wifiģ���ȡIP��ַ��ʽ
								 times=0;
									do{
											  gsm_handle.esp8266.pGSM_SetDHCP(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way);
									
									  OSTimeDly(100);//��ʱ100ms		
									}while(times++<5);										
									if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==1)
									{
									   gsm_handle.esp8266.pGSM_Rest();//��Wifiģ����и�λ
									}	
			  GSM_Config_Reg.WifiWLAN_GET_IP_WAY_Config_State=FALSE;
			}	
				
		 if(GSM_Config_Reg.WifiRest_Config_State==TRUE)
			{//��λWifiģ��
			  	gsm_handle.esp8266.pGSM_Rest();//��λWifiģ��
				GSM_Config_Reg.WifiRest_Config_State=FALSE;
			}	
			if(GSM_Config_Reg.WifiNamePWD_Config_State==TRUE)
			{//Wifiģ��ָ���������
			 	gsm_handle.esp8266.pGSM_Restore();//��λWifiģ��	
				
				GSM_Config_Reg.WifiNamePWD_Config_State=FALSE;
			}
			return   true;
}

#endif


/**
  * @brief  Description "���GSM���ź�ǿ��"
  * @param  None
  * @retval int			GSM���ź�ǿ��(5~32Ϊ������ֵ)
  */
int Gsm_get_signal(void)
{
    u8 rssi;
    bool ret;
	#if (GSM_Module_Type==0x00) //SIM800Cģ��
    ret = gsm_handle.sim800c.pGSM_CSQ(&rssi);
	#endif
	#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��
	 ret = gsm_handle.esp8266.pGSM_CSQ(&rssi);
	#endif
	#if (GSM_Module_Type==0x03) //����MG2639ģ��
   ret = gsm_handle.mg2639.pGSM_CSQ(&rssi);
	#endif
    if (ret)
    {
        return rssi;
    }
    return -1;
}

/**
  * @brief  Description "���GSM��GPRS�ĸ���״̬"
  * @param  None
  * @retval bool		GPRSͨѶ�Ƿ�����
  */
int Gsm_get_gprs_state(void)
{
    bool ret;
    ret = gsm_handle.sim800c.pGSM_CheckGPRSActive();
    return ret;
}
/**
  * @brief  �������շ�����
  * @param  wbuf		���ͻ�����
  * @param  wlen		���ͻ������ֽ�����С
  * @param  rbuf		���ջ�����
  * @param  rlen		���ջ������ֽ�����С
* @param  Sever_Id �������ĸ��������������� 0:�������ݷ�����; 1:�������÷�����; 2:��������������
  * @retval bool		GPRSͨѶ�Ƿ�ɹ�
  */
static  bool Server_Send_Rev_Data(char *wbuf, int wlen, char *rbuf, int* rlen,u8 Server_Id)
{
	#if ((GSM_Module_Type==0) ||(GSM_Module_Type==2))//SIM800Cģ��
	  bool ret=false;	
	   //���ӷ�����
 if((__strlen(GSM_Tcp_Conn[Server_Id].ip) <= 0)&&((__strlen(GSM_Tcp_Conn[Server_Id].dn) > 0)))
	{
	   if(gsm_handle.pGSM_Open_Connect(GSM_Tcp_Conn[Server_Id].dn,GSM_Tcp_Conn[Server_Id].port))//��������zwc
		 {
		     ret=true;	//���������ӳɹ� 	
			 
			   GPRS_SR_Reg.GPRS_Work_State=TRUE;//���ӷ������ɹ�
		 }else
		 {
		        GPRS_SR_Reg.GPRS_Work_State=FALSE;//���ӷ�����ʧ��
			      GPRS_SR_Reg.SimState=SIM_NoSignal;//ͼ����ʾΪ��GPRS�ź�
		 }
	}
	else
	{
	   if(gsm_handle.pGSM_Open_Connect(GSM_Tcp_Conn[Server_Id].ip,GSM_Tcp_Conn[Server_Id].port))//IP����
		 {
		     ret=true;	//���������ӳɹ� 
			 
			  GPRS_SR_Reg.GPRS_Work_State=TRUE;//���ӷ������ɹ�
		 }
		 else
		 {
		        GPRS_SR_Reg.GPRS_Work_State=FALSE;//���ӷ�����ʧ��
			      GPRS_SR_Reg.SimState=SIM_NoSignal;//ͼ����ʾΪGPRS�ź�
		 }
	}
	
 //��������
	 if(ret)//���������ӳɹ� 
	 {		
	    if(gsm_handle.pGSM_SendData(wbuf,wlen))//�ж����������Ƿ�ɹ�
			{
				//���ݷ��ͳɹ���ʼ��������
				if(gsm_handle.pGSM_RevData(rbuf,rlen))
				{
				  ret=true;	//�������������ݳɹ� 
				}
				else
				{
				    goto gsm_rev_data_err;//��������������ʧ��
				}
			} 
	 }
	 
//�ر������������ 
  gsm_handle.pGSM_CloseConnect();
 return ret;
 
gsm_rev_data_err:
 gsm_handle.pGSM_CloseConnect();//�ر������������
 return false;
	#endif 
	 
	#if (GSM_Module_Type==3)//����MG2639ģ��
	  bool ret=false;	
	   //���ӷ�����
 if((__strlen(GSM_Tcp_Conn[Server_Id].ip) <= 0)&&((__strlen(GSM_Tcp_Conn[Server_Id].dn) > 0)))
	{
	   if(gsm_handle.mg2639.pGSM_Open_Connect(Server_Id,GSM_Tcp_Conn[Server_Id].dn,GSM_Tcp_Conn[Server_Id].port))//��������zwc
		 {
		     ret=true;	//���������ӳɹ� 		 
			   GPRS_SR_Reg.GPRS_Work_State=TRUE;//���ӷ������ɹ�
		 }else
		 {
		        GPRS_SR_Reg.GPRS_Work_State=FALSE;//���ӷ�����ʧ��
			      GPRS_SR_Reg.SimState=SIM_NoSignal;//ͼ����ʾΪ��GPRS�ź�
		 }
	}
	else
	{
	   if(gsm_handle.mg2639.pGSM_Open_Connect(Server_Id,GSM_Tcp_Conn[Server_Id].ip,GSM_Tcp_Conn[Server_Id].port))//IP����
		 {
		     ret=true;	//���������ӳɹ� 
			 
			  GPRS_SR_Reg.GPRS_Work_State=TRUE;//���ӷ������ɹ�
		 }
		 else
		 {
		        GPRS_SR_Reg.GPRS_Work_State=FALSE;//���ӷ�����ʧ��
			      GPRS_SR_Reg.SimState=SIM_NoSignal;//ͼ����ʾΪGPRS�ź�
		 }
	}
	
 //��������
	 if(ret)//���������ӳɹ� 
	 {		
	    if(gsm_handle.mg2639.pGSM_SendData(Server_Id,wbuf,wlen))//�ж����������Ƿ�ɹ�
			{
				//���ݷ��ͳɹ���ʼ��������
				if(gsm_handle.pGSM_RevData(rbuf,rlen))
				{
				  ret=true;	//�������������ݳɹ� 
				}
				else
				{
				    goto gsm_rev_data_err;//��������������ʧ��
				}
			} 
	 }
	 
//�ر������������ 
  gsm_handle.pGSM_CloseConnect();
 return ret;
 
gsm_rev_data_err:
 gsm_handle.pGSM_CloseConnect();//�ر������������
 return false;
	#endif	 
	 
	 
	 
	 
 
}
/**
  * @brief  �������������
  * @param  wbuf		���ͻ�����
  * @param  wlen		���ͻ������ֽ�����С
* @param  Sever_Id �������ĸ��������������� 0:�������ݷ�����; 1:�������÷�����; 2:��������������
  * @retval bool		GPRSͨѶ�Ƿ�ɹ�
  */
bool Server_Send_Data(char *wbuf, int wlen,u8 Server_Id)
{
	  bool ret=false;	
	   //���ӷ�����
 if((__strlen(GSM_Tcp_Conn[Server_Id].ip) <= 0)&&((__strlen(GSM_Tcp_Conn[Server_Id].dn) > 0)))
	{
	   if(gsm_handle.pGSM_Open_Connect(GSM_Tcp_Conn[Server_Id].dn,GSM_Tcp_Conn[Server_Id].port))//��������zwc
		 {
		     ret=true;	//���������ӳɹ� 
		 }
	}
	else
	{
	   if(gsm_handle.pGSM_Open_Connect(GSM_Tcp_Conn[Server_Id].ip,GSM_Tcp_Conn[Server_Id].port))//IP����
		 {
		 
		     ret=true;	//���������ӳɹ� 
		 }
	}
 //��������
	 if(ret)//���������ӳɹ� 
	 {
	    if(gsm_handle.mg2639.pGSM_SendData(Server_Id,wbuf,wlen))//�ж����������Ƿ�ɹ�
			{		
				  ret=true;	//��������������ݳɹ�
	
			} 
	 }
//�رշ��������ӳɹ�  	 
 if(ret)
 {
    if(gsm_handle.pGSM_CloseConnect())
		{
		  ret=true;	//���������ӳɹ� 
		}
		else
		{
		 ret=false;	
		}
 }
 
 return ret;
}



/**
  * @brief  Description "GPRSͨѶ"
  * @param  wbuf		���ͻ�����
  * @param  wlen		���ͻ������ֽ�����С
  * @param  rbuf		���ջ�����
  * @param  rlen		���ջ������ֽ�����С
* @param  Sever_Id �������ĸ��������������� 0:�������ݷ�����; 1:�������÷�����; 2:��������������
  * @retval bool		GPRSͨѶ�Ƿ�ɹ�
  */
bool Gsm_gprs_comm(char *wbuf, int wlen, char *rbuf, int* rlen,u8 Server_Id)
{
  bool ret=false;	
	
	switch(Server_Id)
	{
		case GSM_GPRS_Data:  //���ݷ�����
			if(Server_Send_Rev_Data(wbuf,wlen,rbuf,rlen,GSM_GPRS_Data))     ret=true;	
			break;
	  case GSM_GPRS_Config: //���÷�����
			if(Server_Send_Rev_Data(wbuf,wlen,rbuf,rlen,GSM_GPRS_Config))   ret=true;	
			break;
	  case GSM_GPRS_Update://����������
			if(Server_Send_Rev_Data(wbuf,wlen,rbuf,rlen,GSM_GPRS_Update))   ret=true;	
			break;
	  default:
			break;
	}
  return ret;
}







	






