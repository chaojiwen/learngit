/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2018 STMicroelectronics															*/
/**																								*/
/**�ļ���     : GSM_MG2639.c																*/
/**����       : ����GSMģ��ײ�����					*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1     ZWC         2018��12��06��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Gsm_Api.h"
#if (GSM_Module_Type==0x03) //����MG2639ģ��
#include "GSM_MG2639.h"
#include "GSM_USART.h"
#include "ucos_ii.h"
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include "string_utility.h"
/************************************************************************************************
*������   ��u8* GSM_MG2639_check_cmd(char *str,u8 *UsartBuf)															
*������� ��str :ҪѰ�ҵ�Ŀ���ַ���											
*������� ��	  																				
*����     �����������ַ��� 	  
*�������� �� 2018/12/06	
*˵��     �� //���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����  ����ֵ:0,û�еõ��ڴ���Ӧ���� ����,�ڴ�Ӧ������λ��(str��λ��)
************************************************************************************************/
bool   GSM_MG2639_check_cmd(char *str,u8 *UsartBuf)
{
	 int ret=0;
	ret=String_KMPMatch((char*)UsartBuf,str);
	if(ret==-1)
  {
		
		return false;
	}
	
	return true;
}
/************************************************************************************************
*������   ��u8 GSM_MG2639_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)											
*������� ��cmd:Ҫ���͵�ATָ���ַ���	ack��Ҫƥ��������ַ��� waittime:ƥ�������ַ�����Ҫ�ȴ���ʱ�� ��λ10ms										
*������� ������ִ�гɹ����� 1   ʧ�ܷ��� 0  																				
*����     ������ATָ�� 	  
*�������� �� 2018/12/06 	
*˵��     ��
************************************************************************************************/
bool GSM_MG2639_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
{
	 u8 res=1;
   Clear_GSM_RX_BUF(UsartBuf);//��ջ�������
	 SIMCOM_Printf("%s\r\n",cmd);//�����ַ�����
	 if(cmd==NULL)//���û�������ַ���ƥ��Ļ�����ֱ�ӷ��سɹ�����ִ���ַ���ƥ����
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
	    		 if(GSM_MG2639_check_cmd("ERROR",UsartBuf))
	    	   {
	    			  res=0;
             break;//�õ���Ч���� 			 
	    	   }
					 if(GSM_MG2639_check_cmd("CLOSED",UsartBuf))
	    	   {
	    			  res=0;
             break;//�õ���Ч���� 			 
	    	   }
	    	   if(GSM_MG2639_check_cmd("ALREADY CONNECT",UsartBuf))
	    	   {
             break;//�õ���Ч���� 			 
	    	   }					 
	    	   if(GSM_MG2639_check_cmd(ack,UsartBuf))
	    	   {
             break;//�õ���Ч���� 			 
	    	   }
           OSTimeDly(10);//��ʱ10ms					 
	    	}
	    	if(waittime==0)
	    	{
	    		res=0; 
	    	}
	    }
		}
	return res;
}

/************************************************************************************************
*������   �� GSM_MG2639_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
*������� ��cmd:Ҫ���͵�ATָ���ַ���	ack��Ҫƥ��������ַ��� waittime:ƥ�������ַ�����Ҫ�ȴ���ʱ�� ��λ10ms										
*������� ������ִ�гɹ����� 1   ʧ�ܷ��� 0  																				
*����     �����Ͳ����س�����ATָ�� 	  
*�������� �� 2018/12/06 	
*˵��     ��
************************************************************************************************/
bool GSM_MG2639_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
{
	 u8 res=1;
   Clear_GSM_RX_BUF(UsartBuf);//��ջ�������
	 SIMCOM_Printf("%s",cmd);//�����ַ�����
	 if(cmd==NULL)//���û�������ַ���ƥ��Ļ�����ֱ�ӷ��سɹ�����ִ���ַ���ƥ����
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
	    		 if(GSM_MG2639_check_cmd("ERROR",UsartBuf))
	    	   {
	    			  res=0;
             break;//�õ���Ч���� 			 
	    	   }
					 if(GSM_MG2639_check_cmd("CLOSED",UsartBuf))
	    	   {
	    			  res=0;
             break;//�õ���Ч���� 			 
	    	   }
	    	   if(GSM_MG2639_check_cmd("ALREADY CONNECT",UsartBuf))
	    	   {
             break;//�õ���Ч���� 			 
	    	   }					 
	    	   if(GSM_MG2639_check_cmd(ack,UsartBuf))
	    	   {
             break;//�õ���Ч���� 			 
	    	   }
           OSTimeDly(10);//��ʱ10ms					 
	    	}
	    	if(waittime==0)
	    	{
	    		res=0; 
	    	}
	    }
		}
	return res;
}

//��������Ƿ��ͳɹ�
u8 GSM_MG2639_send_data_Status(char *wbuf, int wlen,char *ack,u16 waittime,u8 *UsartBuf)
{
  		u8 res=1;
      Clear_GSM_RX_BUF(UsartBuf);//��ջ�������
		  Usart_SendStr_length(USART3,wbuf,wlen);//������������
	    waittime=waittime*10;
	   if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	   {
	   	 while(--waittime)	//�ȴ�����ʱ
	   	 { 	
	   	    if(GSM_MG2639_check_cmd(ack,UsartBuf))
	   	    {
             break;//�õ���Ч���� 			 
	   	    }
					OSTimeDly(1);//��ʱ1ms
	   	 }
	   	 if(waittime==0)
	   	 {
	   	 	res=0; 
	   	 }
	   
	   }
	   return res;
}

//ģ�����������������
u8   GSM_MG2639_send_data(uint8_t ServerID,char *wbuf, int wlen,u16 waittime,uint8_t *UartBuf)//��������
{
	u8 res=1;
  char  cmd[32]={0};	
	sprintf(cmd,"AT+ZIPSEND=%d,%d\r\n",ServerID,wlen);
	if(GSM_MG2639_send_cmd(cmd,">",Ordinary_AT_WaitTime_n10ms,UartBuf))
	{
		 if(GSM_MG2639_send_data_Status(wbuf,wlen,"+ZIPSEND:OK",waittime,UartBuf))
		 {
			 	 printf("ģ��ɹ��ϴ����ݣ�����\n");	
                res=1;
		 }
		 else
		 {
			          res=0;
		 }
	}
	else          res=0;

	return res;
}

/************************************************************************************************
*������   ��bool GSM_MG2639_AskCSQ(uint8_t *csq, uint8_t *UartBuf)														
*������� ��uint8_t *csq  	,	�ź�ǿ��uint8_t��ָ��										
*������� ��	��ȡ�ź�ǿ���Ƿ�ɹ�  																				
*����     ��	 	  
*�������� �� 2018/12/06 	
*˵��     ��
************************************************************************************************/
bool GSM_MG2639_AskCSQ(uint8_t *csq, uint8_t *UartBuf)
{
	char *p;
	bool ret = false;

	ret = GSM_MG2639_send_cmd("AT+CSQ", "+CSQ", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //û��ʱ��������Ϊ��ѯ����ʧ��
	{
		*csq = 0;
		return false;
	}
	p = strstr((const char*)UartBuf, "+CSQ:"); //������ȷ�򷵻غ�����ǰ���״γ��ֵ��׵�ַ�����򷵻�NULL
	if(p != NULL)
	{
		ret = true;
		if((*(p+7)) == ',') //���ֻ��1λ���ȵ��ź�ǿ��
		{
			*csq = (*(p+6)-0x30);
		}
		else if((*(p+8)) == ',')//2λ���ȵ��ź�ǿ��
		{
			*csq = (*(p+6)-0x30)*10 + (*(p+7)-0x30); //���ַ�����Ӧ����λתΪ����
		}
		else //������ʽ���б�Ϊ����ʧ��
		{
			*csq = 0;
			ret = false;
		}
	}
	return ret;
}

bool GSM_MG2639_Echo(bool flag,u8 *UsartBuf)//�Ƿ�����GSM���� flag:true������� flag:false��������
{
	 bool  ret=false; 
	if(flag==true)
	{
		  if(GSM_MG2639_send_cmd("ATE1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))      ret=true;   
	  
	}
	else
	{
	    if(GSM_MG2639_send_cmd("ATE0","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))       ret=true;    
	
	}
	return ret;
}
/************************************************************************************************
*������   ��bool GSM_MG2639_CheckNetwork(uint8_t *UartBuf)												
*������� ��											
*������� ��	  																				
*����     �����GSMģ���Ƿ��Ѿ�ע����������;   true:����ע������  fasle:����δע����	  
*�������� �� 2018/12/10 	
*˵��     ��
************************************************************************************************/
bool GSM_MG2639_CheckNetwork(uint8_t *UartBuf)
{
      bool  ret=false;
	
	   GSM_MG2639_send_cmd("AT+CREG=1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf);//���ü��ע������״̬ʱ���ظ�ʽ
	   //���GSMģ���Ƿ��Ѿ�ע����������
      if(GSM_MG2639_send_cmd("AT+CREG?","+CREG: 1, 1",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else
			{
		  	if(GSM_MG2639_send_cmd("AT+CREG?","+CREG: 1, 5",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf)) ret=true;
			}				

 return ret;
}
/************************************************************************************************
*������   ��bool GSM_MG2639_CheckGPRSActive(uint8_t *UartBuf)												
*������� ��											
*������� ��	  																				
*����     �����GSMģ���Ƿ��Ѿ���������GPRS����;   true:����ע������  fasle:����δ������GPRS����	  
*�������� �� 2018/12/10 	
*˵��     ��
************************************************************************************************/
bool GSM_MG2639_CheckGPRSActive(uint8_t *UartBuf)
{
      bool  ret=false;
	    GSM_MG2639_send_cmd("AT+CGATT=1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf);//����GPRSҵ��
	   //���GSMģ���Ƿ��Ѿ���������GPRS����
      if(GSM_MG2639_send_cmd("AT+CGATT?","+CGATT: 1",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else                                                                                ret=false;
						

    return ret;
}
/************************************************************************************************
*������   ��u8 GSM_MG2639_Open_Connect_Srever(char *ServerName,char *ServerPort,u16 waittime)															
*������� ��											
*������� ��	  ��ģ��֧�ֶ���·ģʽ  ������ͬʱ��5����������������   ServerID:��ʾ������ID��   ID��:1~5																				
*����     �������������TCP���� 	  
*�������� �� 2018/12/10 	
*˵��     ��
************************************************************************************************/
u8 GSM_MG2639_Open_Connect_Srever(uint8_t ServerID,char *ServerName,char *ServerPort,u16 waittime,uint8_t *UartBuf)
{
	    u8  ret=0;
	   char	server[64];
	   char at[32];
	 if(GSM_MG2639_CheckNetwork(UartBuf))//�ȼ��GSMģ���Ƿ��Ѿ�ע����������
	 {
		 //1����GPRS������·
		 if(GSM_MG2639_send_cmd("AT+ZPPPOPEN","+ZPPPOPEN:CONNECTED",Mg2639_Open_GPRSdatalink_n10ms,(u8*)UartBuf))
		 {
			 //2����ѯ������·����״̬
			 		  sprintf(at,"AT+ZIPSTATUS=%d",ServerID);
	     	if(!GSM_MG2639_send_cmd(at,"+ZIPSTATUS: ESTABLISHED",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))
	     	{
	     		sprintf(server,"AT+ZIPSETUP=%d,%s,%s",ServerID,ServerName,ServerPort);//IP����
	         if(GSM_MG2639_send_cmd(server,"+ZIPSETUP:CONNECTED",waittime,(u8*)UartBuf))        ret=1;	 				
	     		else                                                                                ret=0;
	     	}                                                                    
	     	else                                                                                  ret=1;	
	      
	   }
	    else                                                                                    ret=0;
	      
       return ret;
	}
	else
	 {
	    ret=0;
	 }
		 return ret;                                                            
}


/***************************GSM��Դ��������***************************/
void GSM_MG2639_Io_Init(void)//GSMģ�����IO�ڳ�ʼ��	
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
		//GSM��Դ���ƽ�
	  RCC_APB2PeriphClockCmd(GSM_MG2639_PWRCTRL_CLK, ENABLE);
	  GPIO_InitStructure.GPIO_Pin		  = GSM_MG2639_PWRCTRL_PIN;
	  GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  GPIO_Init(GSM_MG2639_PWRCTRL_PORT, &GPIO_InitStructure);
	  GSM_MG2639_Power(OFF);//Ĭ�Ϲر�GSMģ���Դ
			//GSM�������ƽ�
	  RCC_APB2PeriphClockCmd(GSM_MG2639_ONOFF_CLK, ENABLE);
	  GPIO_InitStructure.GPIO_Pin		  = GSM_MG2639_ONOFF_PIN;
	  GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  GPIO_Init(GSM_MG2639_ONOFF_PORT, &GPIO_InitStructure);
		GSM_MG2639_OnOff(ON);//Ĭ��GSMģ�鴦�ڹػ�״̬
}

static void MG2639_Boot(void)//GSMģ�鿪���͹ػ�
{
	GSM_MG2639_OnOff(ON);
	OSTimeDly(3000);//��ʱ3s
	GSM_MG2639_OnOff(OFF);
}
void GSM_MG2639_Power_Up(void)//��GSMģ���Դ	
{
   GSM_MG2639_Power(ON);
	 OSTimeDly(300);//��ʱ300ms�Ա�ϵͳ��Դ�ȶ�
	 MG2639_Boot();//GSMģ�鿪���͹ػ�
}	
void GSM_MG2639_Power_Down(void)//�ر�GSMģ���Դ
{
	 MG2639_Boot();//GSMģ�鿪���͹ػ�
	 OSTimeDly(300);//��ʱ300ms�Ա�ϵͳ��Դ�ȶ�
   GSM_MG2639_Power(OFF);

}
bool GSM_MG2639_Power_Rest(void)//����GSMģ��
{
	 bool ret;
	  ret=true;
  GSM_MG2639_Power_Down();//�ر�GSMģ���Դ
	OSTimeDly(300);//��ʱ300ms
  GSM_MG2639_Power_Up();//��GSMģ���Դ
	 return ret;
}

/************************************************************************************************
*������   ��bool Gsm_Check_SIm_Card(uint8_t *UartBuf)												
*������� ��											
*������� ��	  																				
*����     �����SIM���Ƿ���λ;   true:������λ  fasle:������λ 	  
*�������� �� 2018/12/06 	
*˵��     ��
************************************************************************************************/
bool GSM_MG2639_Check_SIm_Card(uint8_t *UartBuf)
{
	    bool  ret=false;
	   //���SIM���Ƿ���λ
      if(GSM_MG2639_send_cmd("AT+CPIN?","+CPIN: READY",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else                                                                                         ret=false;
      
 return ret;
}
bool    GSM_MG2639_Send_AT(u8 *UsartBuf)//��ģ�鷢��"AT"
{
    bool  ret=false; 
	
	 if(GSM_MG2639_send_cmd("AT","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))            ret=true;   
	
	  return ret;
}
  /* ����Ӧ������ */
	int	GSM_MG2639_Adaptive_BaudRate(u8 *UartBuf)
	{
		/* ��������� */
		int baudratetab[7] = {9600,38400,115200,19200,14400,56000,57600};
		/* ������ */
		int num0;
		
		/* ѭ����ѯ */
		for(num0 = 0;num0 < 7;num0++)
		{
			/* ���ò����� */
		 GSM_USART_Init(baudratetab[num0]);//GSMģ�鴮�ڳ�ʼ��  ���ò�����Ϊ9600
			/* ����ATָ�� */
			if(GSM_MG2639_send_cmd("AT","OK",Ordinary_AT_WaitTime_n10ms,UartBuf) == true)
			{
				/* ��ǰ������Ϊģ��ͨ�Ų����� */
				return baudratetab[num0];
			}
		}
		
		return 0;
	}	
 bool	GSM_MG2639_SetBaudRate(int br,u8 *UartBuf)//����ģ�鲨����
    {
        char	at[32];
        sprintf(at, "AT+IPR=%d\r\n", br);
        if(GSM_MG2639_send_cmd(at, "OK", Ordinary_AT_WaitTime_n10ms,UartBuf) == false)
        {
            return false;
        }
        return true;
    }
#endif
