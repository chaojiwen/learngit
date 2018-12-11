#include "Log_GSM_Config.h"
#include <stdio.h>
#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��

//GSMģ���������ݼĴ���
GSM_Config_Struct    GSM_Config_Reg =
{
   FALSE,//Ĭ��GSMû���ڽ�������
	 FALSE,//Ĭ�ϲ�����Wifiģ������ּ�����
	 FALSE,//Ĭ�ϲ�����Wifiģ���Wlan����
	 FALSE,//Ĭ�ϲ�����Wifiģ���ȡIP��ַ��ʽ
	 FALSE,//Ĭ�ϲ���Wifiģ����и�λ����
	 FALSE,//Ĭ�ϲ���Wifiģ����лָ���������
};




/* ����Wifi���ֺ�����*/
static GSM_Config_QUE    GSM_Wifi_QUE_SetWifiNamePWD	                 = {WifiNamePWD_EVENT,	             ((u32*)0X0),TRUE};
/* ����WifiWlan����*/
static GSM_Config_QUE    GSM_Wifi_QUE_SetWifiWLAN                      = {WifiWLAN_EVENT,	                 ((u32*)0X0),TRUE};
/* ����Wifiģ���ȡIP��ַ��ʽ   STATIC����DHCPģʽ����*/
static GSM_Config_QUE    GSM_Wifi_QUE_GET_IP_WAY                       = {WifiWLAN_GET_IP_WAY_EVENT,	      ((u32*)0X0),TRUE};
/*��λWifiģ��*/
static GSM_Config_QUE    GSM_Wifi_QUE_Reset                            = {WifiRest_EVENT,	                  ((u32*)0X0),TRUE};
/* Wifiģ��ָ���������*/
static GSM_Config_QUE    GSM_Wifi_QUE_Factory_Data_Reset               = {Factory_Data_Wifi_EVENT,	        ((u32*)0X0),TRUE};
#endif	
/************************************************************************************************/
/* ������	��GSM_Config_Function																		*/
/* ����		��gsm_que_inf:��ǰ����Ϣ����															*/
/* ���		���� 																				*/
/* ����		��ִ����Ϣ���� ���е� ��Ϣ����			 								 		 		*/
/* ��������	��2018/11/21																		*/
/************************************************************************************************/
void GSM_Config_Function(GSM_Config_QUE*  gsm_config_que_inf,INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	
      switch(gsm_config_que_inf->EVENT_Cmd)
			{
			
			  case  WifiNamePWD_EVENT :/* ����Wifi���ֺ����� �¼� */
				{
				    printf("���յ�����Wifi���ֺ����� �¼�\n");
				
				     break;
				}
			  case  WifiWLAN_EVENT :/* ����WLAN����(����IP���������롢���ص�ַ) �¼� */
				{
				    printf("���յ�����WLAN����(����IP���������롢���ص�ַ) �¼�\n");
				     break;
				}
			 case  WifiWLAN_GET_IP_WAY_EVENT :/* ����Wifi IP��ַ��ȡ��ʽ STATIC����DHCPģʽ�¼� */
				{
				    printf("���յ�����WifiIP��ַ��ȡ��ʽ STATIC����DHCPģʽ �¼�\n");
				     break;
				}
			 case  WifiRest_EVENT :	/* ��λWifiģ���¼� */
				{
				    printf("���յ���λWifiģ���¼�\n");
				     break;
				}
				case  Factory_Data_Wifi_EVENT :	/* Wifiģ��ָ����������¼� */
				{
				    printf("���յ�����Wifiģ��ָ����������¼�\n");
				     break;
				}
				default:
				{
				
				  break;
				}
			
			}
				/* ��Ϣִ����� */
	gsm_config_que_inf->EVENT_Getarg = TRUE;
}

/*
*********************************************************************************************************
*	�� �� ��: GSM_Send_Q
*	����˵��: ���� GSM ��Ϣ����
*	��    �Σ�pevent:��Ϣ���е�ָ�룬GSM_Config_TYPE:��Ϣ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
BOOL GSM_Config_Send_Q(OS_EVENT* pevent,GSM_Config_TYPE EVENT_Type)
{
	INT8U     	            err;
	OS_Q_DATA	           Q_data;
	GSM_Config_QUE*	    EVENT_p;	
	u16 		                num;
	u32 		                Q_p;
#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	
	if(EVENT_Type == WifiNamePWD_EVENT)
	{
		/* ����Wifi���ֺ����� */		
		EVENT_p = &GSM_Wifi_QUE_SetWifiNamePWD;		
	}
	else if(EVENT_Type == WifiWLAN_EVENT)
	{
		/* ����WifiWlan���� */		
		EVENT_p = &GSM_Wifi_QUE_SetWifiWLAN;		
	}
	else if(EVENT_Type == WifiWLAN_GET_IP_WAY_EVENT)
	{
		/* ����Wifiģ��IP��ȡ��ʽ���� */		
		EVENT_p = &GSM_Wifi_QUE_GET_IP_WAY;		
	}
	else if(EVENT_Type == WifiRest_EVENT)
	{
		/*��λWifiģ��*/		
		EVENT_p = &GSM_Wifi_QUE_Reset;		
	}	
	else if(EVENT_Type == Factory_Data_Wifi_EVENT)
	{
		/* Wifiģ��ָ��������� */		
		EVENT_p = &GSM_Wifi_QUE_Factory_Data_Reset;		
	}		

#endif	
	
	/* ���򱨵����� ˵�������в����� �ϴ�������Ϣ */
	/* �ж� �ϴ����� ִ�н�� */
	if(EVENT_p->EVENT_Getarg == FALSE)
	{
		/*������Ϣ ���� ��ִ�У��ȴ�ִ�н��� */
		return FALSE;
	}
	
	//���� �ڴ�ռ����� �������Ϣֻ��һ�����ݿռ䣬��ˣ��ڷ�����Ϣǰ��Ҫȷ���ϴε���Ϣ�Ѿ���ִ���ˡ�
	//����ͬһ����Ϣ��ռ��2����Ϣ�ռ䣬����������Ϣ�޷����롣
	/* �����Ϣ�������Ƿ���� ���� ����״̬ ��Ϣ�� */
	err = OSQQuery(pevent,&Q_data);
	
	/* �жϲ�ѯ��� */
	if(err == OS_ERR_NONE)
	{
		/* ��ѯ�ɹ� */
		
		/* ���� �������Ƿ��Ѿ����� ����״̬ ��Ϣ�� */
		for(num = 0,Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQOut);
			num < Q_data.OSNMsgs;
			num ++)
		{
			/* �жϵ�ǰ����Ϣָ���뷢��ָ���Ƿ���ͬ */
			if((*(u32*)Q_p) == (u32)EVENT_p)
			{
				/* ��ȣ������д��� ����״̬ ��Ϣ,��ô�Ͳ��ڷ��� ����״̬ ��Ϣ */
				return FALSE;
			}
			/* ����ָ�뵽��һ��λ�� */
			Q_p += sizeof(GSM_Config_QUE);
			/* �ж�ָ���Ƿ�Խ�� */
			if(Q_p == (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQEnd))
			{
				/* Խ��ͽ� ָ�� ָ��ͷλ�� */
				Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQStart);
			}
		}
	}else
	{
		/* ��ѯʧ�� */
		return FALSE;
	}

	/* ��� ��Ϣ ִ�н�� */
	EVENT_p->EVENT_Getarg = FALSE;
	/* ���� ��Ϣ *///FIFO	  �Ƚ��ȳ�
	err = OSQPost(pevent, EVENT_p);  //������Ϣֵ
	/* �ж� ��Ϣ ���� ״̬ */
	if(err == OS_ERR_Q_FULL)
	{
		/* ��Ϣ��������,�����ٽ�������Ϣ */
		/* ������� ������ ���� */
		return FALSE;
	}
	/* �ж� ��Ϣ ���� �ɹ� */
	return TRUE;
}
