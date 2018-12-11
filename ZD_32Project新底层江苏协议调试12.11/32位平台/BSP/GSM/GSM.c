#include "Gsm_Api.h"
#if (GSM_Module_Type==0x00) //SIM800Cģ��
#include "GSM.h"
#include "ucos_ii.h"
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include "string_utility.h"

/************************************************************************************************
*������   ��u8* Gsm_check_cmd(char *str,u8 *UsartBuf)															
*������� ��str :ҪѰ�ҵ�Ŀ���ַ���											
*������� ��	  																				
*����     �����������ַ��� 	  
*�������� �� 2018/08/23	
*˵��     �� //���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����  ����ֵ:0,û�еõ��ڴ���Ӧ���� ����,�ڴ�Ӧ������λ��(str��λ��)
************************************************************************************************/
bool   Gsm_check_cmd(char *str,u8 *UsartBuf)
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
*������   ��u8 Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)											
*������� ��cmd:Ҫ���͵�ATָ���ַ���	ack��Ҫƥ��������ַ��� waittime:ƥ�������ַ�����Ҫ�ȴ���ʱ�� ��λ10ms										
*������� ������ִ�гɹ����� 1   ʧ�ܷ��� 0  																				
*����     ������ATָ�� 	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
bool Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
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
	    		 if(Gsm_check_cmd("ERROR",UsartBuf))
	    	   {
	    			  res=0;
             break;//�õ���Ч���� 			 
	    	   }
					 if(Gsm_check_cmd("CLOSED",UsartBuf))
	    	   {
	    			  res=0;
             break;//�õ���Ч���� 			 
	    	   }
	    	   if(Gsm_check_cmd("ALREADY CONNECT",UsartBuf))
	    	   {
             break;//�õ���Ч���� 			 
	    	   }					 
	    	   if(Gsm_check_cmd(ack,UsartBuf))
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
*������   �� Gsm_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
*������� ��cmd:Ҫ���͵�ATָ���ַ���	ack��Ҫƥ��������ַ��� waittime:ƥ�������ַ�����Ҫ�ȴ���ʱ�� ��λ10ms										
*������� ������ִ�гɹ����� 1   ʧ�ܷ��� 0  																				
*����     �����Ͳ����س�����ATָ�� 	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
bool Gsm_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
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
	    		 if(Gsm_check_cmd("ERROR",UsartBuf))
	    	   {
	    			  res=0;
             break;//�õ���Ч���� 			 
	    	   }
					 if(Gsm_check_cmd("CLOSED",UsartBuf))
	    	   {
	    			  res=0;
             break;//�õ���Ч���� 			 
	    	   }
	    	   if(Gsm_check_cmd("ALREADY CONNECT",UsartBuf))
	    	   {
             break;//�õ���Ч���� 			 
	    	   }					 
	    	   if(Gsm_check_cmd(ack,UsartBuf))
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

u8 Gsm_send_data_Status(char *wbuf, int wlen,char *ack,u16 waittime,u8 *UsartBuf)
{
  		u8 res=1;
      Clear_GSM_RX_BUF(UsartBuf);//��ջ�������
		  Usart_SendStr_length(USART3,wbuf,wlen);//������������
	    waittime=waittime*10;
	   if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	   {
	   	 while(--waittime)	//�ȴ�����ʱ
	   	 { 	
	   	    if(Gsm_check_cmd(ack,UsartBuf))
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




/************************************************************************************************
*������   ��bool GSM_AskCSQ(uint8_t *csq, uint8_t *UartBuf)														
*������� ��uint8_t *csq  	,	�ź�ǿ��uint8_t��ָ��										
*������� ��	��ȡ�ź�ǿ���Ƿ�ɹ�  																				
*����     ��	 	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
bool Gsm_AskCSQ(uint8_t *csq, uint8_t *UartBuf)
{
	char *p;
	bool ret = false;

	ret = Gsm_send_cmd("AT+CSQ", "+CSQ", Ordinary_AT_WaitTime_n10ms, UartBuf);
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





u8 Gsm_send_data(char *wbuf, int wlen,u16 waittime,uint8_t *UartBuf)//��������
{
	u8 res=1;
  char  cmd[32]={0};	
	sprintf(cmd,"AT+CIPSEND=%d\r\n",wlen);
	if(Gsm_send_cmd(cmd,">",Ordinary_AT_WaitTime_n10ms,UartBuf))
	{
		 if(Gsm_send_data_Status(wbuf,wlen,"SEND OK",waittime,UartBuf))
		 {
//			 	 Log_printf("ģ��ɹ��ϴ����ݣ�����\n");	
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

static bool  Gsm_check_Rev_Data(int *Location1_0x7E, int *Location2_0x7E,int rlen,u8 *UsartBuf)//�����յ��������Ƿ�Ϸ�
{
	   bool  ret=false; 
	   int i=0;
//Location1_0x7E:0x7E��һ�γ��ֵ�λ��;Location2_0x7E:0x7E�ڶ��γ��ֵ�λ��;
		for(i=0;i<rlen;i++)//���ҵ�һ��0x7E���ֵ�λ��
		{
		  if(UsartBuf[i]==0x7E)
			{
			  *Location1_0x7E=i;
				 ret=true;
				  break;
			}
			else
			{
			   ret=false;
			}
		}
		
		if(ret)
		{
		   for(i=*Location1_0x7E;i<rlen;i++)//���ҵڶ��δ�0x7E���ֵ�λ��
		   {
		    if(UsartBuf[i+1]==0x7E)
		   	 {
		   	   *Location2_0x7E=i+1;
				 	  ret=true;
		   	 	  break;
		   	 }
			  else
			   {
			      ret=false;
			   }
				
		   }
		}
		else
		{
		  ret=false;
		}
	

	 return ret;
}

u8 Gsm_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf)//��������
{
      u8 res=1;
	int Location1=0,Location2=0;//Location1:0x7E��һ�γ��ֵ�λ��;Location2:0x7E�ڶ��γ��ֵ�λ��;
//      Clear_GSM_RX_BUF(UartBuf);//��ջ�������
	 //1����0x7E��
	   if(waittime)		//��Ҫ�ȴ�Ӧ��
	   {
	   	 while(--waittime)	//�ȴ�����ʱ
	   	 { 
				 /*�����յ��������Ƿ�Ϸ�*/
	   	    if(Gsm_check_Rev_Data(&Location1,&Location2,GSM_RX_BUF_LEN,UartBuf))
	   	    {
						*rlen=(Location2-Location1)+1;
						memcpy(rbuf,&UartBuf[Location1],*rlen);//��������
             break;//�õ���Ч���� 			 
	   	    }
					OSTimeDly(10);//��ʱ10ms
	   	 }
	   	 if(waittime==0)
	   	 {
	   	 	res=0; 
	   	 }
	   }
	   return res;
} 



/************************************************************************************************
*������   ��u8 GSM_Open_Connect_Srever(char *ServerName,char *ServerPort,u16 waittime)															
*������� ��											
*������� ��	  																				
*����     �������������TCP���� 	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
u8 Gsm_Open_Connect_Srever(char *ServerName,char *ServerPort,u16 waittime,uint8_t *UartBuf)
{
	    u8  ret=0;
	   char	server[64];
	 if(Gsm_CheckNetwork(UartBuf))//�ȼ��GSMģ���Ƿ��Ѿ�ע����������
	 {
		if(!Gsm_send_cmd("AT+CIPSTATUS","STATE: CONNECT OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))
		{
			sprintf(server,"AT+CIPSTART=\"%s\",\"%s\",\"%s\"\r\n","TCP",ServerName,ServerPort);//IP����
	    if(Gsm_send_cmd(server,"CONNECT OK",waittime,(u8*)UartBuf))        ret=1;	 				
			else                                                               ret=0;
		}                                                                    
		else                                                                 ret=1;	
	 
	 }
	 else                                                                  ret=0;
 return ret;                                                             
}
/************************************************************************************************
*������   ��bool Gsm_Close_Connect_Srever(uint8_t *UartBuf)													
*������� ��											
*������� ��	  																				
*����     ���ر����������TCP���� 	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
bool Gsm_Close_Connect_Srever(uint8_t *UartBuf)
{
	    bool  ret=false;
	   //�ر�TCP���� 
      if(Gsm_send_cmd("AT+CIPCLOSE=1\r\n","CLOSE OK",Close_Server_AT_WaitTime_n10ms,(u8*)UartBuf)) ret=true;	 
			else                                                              ret=false;
      
 return ret;
}
/************************************************************************************************
*������   ��bool Gsm_Check_SIm_Card(uint8_t *UartBuf)												
*������� ��											
*������� ��	  																				
*����     �����SIM���Ƿ���λ;   true:������λ  fasle:������λ 	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
bool Gsm_Check_SIm_Card(uint8_t *UartBuf)
{
	    bool  ret=false;
	   //���SIM���Ƿ���λ
      if(Gsm_send_cmd("AT+CPIN?","+CPIN: READY",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else                                                                                  ret=false;
      
 return ret;
}
/************************************************************************************************
*������   ��bool Gsm_CheckNetwork(uint8_t *UartBuf)												
*������� ��											
*������� ��	  																				
*����     �����GSMģ���Ƿ��Ѿ�ע����������;   true:����ע������  fasle:����δע����	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
bool Gsm_CheckNetwork(uint8_t *UartBuf)
{
      bool  ret=false;
	
	   Gsm_send_cmd("AT+CREG=1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf);//���ü��ע������״̬ʱ���ظ�ʽ
	
	
	   //���GSMģ���Ƿ��Ѿ�ע����������
      if(Gsm_send_cmd("AT+CREG?","+CREG: 1,1",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else
			{
		  	if(Gsm_send_cmd("AT+CREG?","+CREG: 1,5",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf)) ret=true;
			}				

 return ret;
}
/************************************************************************************************
*������   ��bool Gsm_CheckGPRSActive(uint8_t *UartBuf)												
*������� ��											
*������� ��	  																				
*����     �����GSMģ���Ƿ��Ѿ���������GPRS����;   true:����ע������  fasle:����δ������GPRS����	  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
bool Gsm_CheckGPRSActive(uint8_t *UartBuf)
{
      bool  ret=false;
	    Gsm_send_cmd("AT+CGATT=1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf);//����GPRSҵ��
	   //���GSMģ���Ƿ��Ѿ���������GPRS����
      if(Gsm_send_cmd("AT+CGATT?","+CGATT: 1",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else                                                                                ret=false;
						

    return ret;
}

/************************************************************************************************
*������   ��bool Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf)											
*������� ��											
*������� ��	  																				
*����     ��SIM800C���Ͷ���
*�������� �� 2018/11/26 	
*˵��     ��
************************************************************************************************/
bool     Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf)//���Ͷ�������
{
	        // ���ض������Ķ�������(���Ĳ���UCS2���롢Ӣ�Ĳ���7bit����)
        unsigned char	    encoded[512]={0};//��Ŷ������ݾ������������	
				unsigned char	    Pn_encoded[64]={0};//�������뾭�����������
				int				        SmsContentLen = 0;//�������ݱ����unicode���ĳ���
        char              converbuff[512]={0};//�����512���ֽ�����
	      char              cmdbuff[64]={0};//�����������
	      char              end= 0x1A;//��������
	     uint8_t            testSend = 0;
	//1�������ź���Ͷ�Ϣ�����Ƿ�Ϊ��
	  if((Utils_strlen(num)<=0)||(Utils_strlen(smstext)<=0)){
		    goto sms_failure;
		}
	
	
    if(IsASSIC(smstext)==true)
    {
			//Ӣ��
			//"GSM"�ַ���
			if(!Gsm_send_no_entercmd("AT+CSCS=\"GSM\"\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

			//�ı�ģʽ
			if(!Gsm_send_no_entercmd("AT+CMGF=1\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

			//���ɵ绰�������� 
			sprintf(cmdbuff,"AT+CMGS=\"%s\"\r",num);
			//���ƶ�������
			strcpy(converbuff,smstext);

    }
    else
    {
			//����
			//"UCS2"�ַ���
			if(!Gsm_send_no_entercmd("AT+CSCS=\"UCS2\"\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;
			//�ı�ģʽ
			if(!Gsm_send_no_entercmd("AT+CMGF=1\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

			if(!Gsm_send_no_entercmd("AT+CSMP=17,167,0,8\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf) ) goto sms_failure;

			 Utils_pdu_encode16bit(Pn_encoded, num, strlen(num));//�Զ��ź������unicode����	
			 Utils_Hex2Str((char*)Pn_encoded,converbuff,(strlen(num)*2));//��Unicode��ת��Ϊ�ַ���(�����ֻ������ Unicode����ַ���)	
			
			 sprintf(cmdbuff,"AT+CMGS=\"%s\"\r",converbuff);  //��װ�绰����Unicode�����ַ���
			
			 SmsContentLen=Utils_pdu_encode16bit(encoded, smstext,Utils_strlen(smstext));//���������ݽ���unicode����
			 Utils_Hex2Str((char*)encoded,converbuff,SmsContentLen);//��Unicode��ת��Ϊ�ַ���(�������ݵ� Unicode����ַ���)	

    }
		
	//���͵绰����	
	if(Gsm_send_cmd(cmdbuff,">",Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf))
	{
		SIMCOM_Printf("%s",converbuff);//���Ͷ�������
		
		Gsm_send_cmd(&end,0,Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf);		//0x1A������
		
		//����Ƿ������
		while(Gsm_check_cmd("OK",(u8*)UartBuf) != true )
		{		
			if(++testSend >100)//��ȴ�10��  (100��  ÿ��100ms)
			{
				goto sms_failure;
			}
			OSTimeDly(Send_Sms_AT_WaitTime_n10ms);//��ʱ100ms				
		}	
		return true;
	}
	else
	{				
sms_failure:		
		end = 0x1B;
		Gsm_send_cmd(&end,0,0,(u8*)UartBuf);	//ESC,ȡ������ 
		return false;		
	}		
}
///////************************************************************************************************
//////*������   ��bool Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf)											
//////*������� ��											
//////*������� ��	  																				
//////*����     ��SIM800C���Ͷ���
//////*�������� �� 2018/11/26 	
//////*˵��     ��
//////************************************************************************************************/
//////bool     Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf)//���Ͷ�������
//////{
////////////	  bool  ret=false;
//////    char converbuff[512];//�����512���ֽ�����
//////	  char cmdbuff[80];//�����������
//////	  char end= 0x1A;//��������
//////	  uint8_t testSend = 0;
//////	
//////    if(IsASSIC(smstext)==true)
//////    {
//////			//Ӣ��
//////			//"GSM"�ַ���
//////			if(!Gsm_send_no_entercmd("AT+CSCS=\"GSM\"\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

//////			//�ı�ģʽ
//////			if(!Gsm_send_no_entercmd("AT+CMGF=1\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

//////			//���ɵ绰�������� 
//////			sprintf(cmdbuff,"AT+CMGS=\"%s\"\r",num);
//////			//���ƶ�������
//////			strcpy(converbuff,smstext);

//////    }
//////    else
//////    {
//////			//����
//////			//"UCS2"�ַ���
//////			if(!Gsm_send_no_entercmd("AT+CSCS=\"UCS2\"\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;
//////			//�ı�ģʽ
//////			if(!Gsm_send_no_entercmd("AT+CMGF=1\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

//////			if(!Gsm_send_no_entercmd("AT+CSMP=17,167,0,8\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf) ) goto sms_failure;

//////			//���ɵ绰�������UCS2�ĵ绰����(��Ҫת�� ucs2��)
//////			gsm_gbk2ucs2hex(converbuff,num);//ת��ΪUCS2
//////			sprintf(cmdbuff,"AT+CMGS=\"%s\"\r",converbuff);  
//////			
//////			//ת����������
//////			gsm_gbk2ucs2hex(converbuff,smstext);
//////    }
//////		
//////	//���͵绰����	
//////	if(Gsm_send_cmd(cmdbuff,">",Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf))
//////	{
//////		SIMCOM_Printf("%s",converbuff);//���Ͷ�������
//////		
//////		Gsm_send_cmd(&end,0,Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf);		//0x1A������
//////		
//////		//����Ƿ������
//////		while(Gsm_check_cmd("OK",(u8*)UartBuf) != true )
//////		{		
//////			if(++testSend >100)//��ȴ�10��  (100��  ÿ��100ms)
//////			{
//////				goto sms_failure;
//////			}
//////			OSTimeDly(Send_Sms_AT_WaitTime_n10ms);//��ʱ100ms				
//////		}	
//////		return true;
//////	}
//////	else
//////	{				
//////sms_failure:		
//////		end = 0x1B;
//////		Gsm_send_cmd(&end,0,0,(u8*)UartBuf);	//ESC,ȡ������ 
//////		return false;		
//////	}		
//////}




/************************************************************************************************
*������   ��void GSM_Init(void)														
*������� ��											
*������� ��	  																				
*����     ��GSM(sim800C)Ӳ����ʼ��  
*�������� �� 2018/08/23 	
*˵��     ��
************************************************************************************************/
//void Gsm_Init(void)
//{
// 	Gsm_Power_Down();//�ر�GSMģ���Դ
//  Gsm_Power_Up();//��GSMģ���Դ	
//	Gsm_Boot();//GSMģ�鿪��
//}

/***************************GSM��Դ��������***************************/
void Gsm_Io_Init(void)//GSMģ�����IO�ڳ�ʼ��	
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
		//GSM��Դ���ƽ�
	  RCC_APB2PeriphClockCmd(GSM_PWRCTRL_CLK, ENABLE);
	  GPIO_InitStructure.GPIO_Pin		  = GSM_PWRCTRL_PIN;
	  GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  GPIO_Init(GSM_PWRCTRL_PORT, &GPIO_InitStructure);
	  GSM_Power(OFF);//Ĭ�Ϲر�GSMģ���Դ
			//GSM�������ƽ�
	  RCC_APB2PeriphClockCmd(GSM_ONOFF_CLK, ENABLE);
	  GPIO_InitStructure.GPIO_Pin		  = GSM_ONOFF_PIN;
	  GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  GPIO_Init(GSM_ONOFF_PORT, &GPIO_InitStructure);
		GSM_OnOff(ON);//Ĭ��GSMģ�鴦�ڹػ�״̬
}

static void Gsm_Boot(void)//GSMģ�鿪���͹ػ�
{
	GSM_OnOff(ON);
	OSTimeDly(3000);//��ʱ3s
	GSM_OnOff(OFF);
}
void Gsm_Power_Up(void)//��GSMģ���Դ	
{
   GSM_Power(ON);
	 OSTimeDly(300);//��ʱ300ms�Ա�ϵͳ��Դ�ȶ�
	 Gsm_Boot();//GSMģ�鿪���͹ػ�
}	
void Gsm_Power_Down(void)//�ر�GSMģ���Դ
{
	 Gsm_Boot();//GSMģ�鿪���͹ػ�
	 OSTimeDly(300);//��ʱ300ms�Ա�ϵͳ��Դ�ȶ�
   GSM_Power(OFF);

}
bool Gsm_Power_Rest(void)//����GSMģ��
{
	 bool ret;
	  ret=true;
  Gsm_Power_Down();//�ر�GSMģ���Դ
	OSTimeDly(300);//��ʱ300ms
  Gsm_Power_Up();//��GSMģ���Դ
	
	 return ret;
}

bool Gsm_Echo(bool flag,u8 *UsartBuf)//�Ƿ�����GSM���� flag:true������� flag:false��������
{
	 bool  ret=false; 
	if(flag==true)
	{
		  if( Gsm_send_cmd("ATE1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))      ret=true;   
	  
	}
	else
	{
	    if( Gsm_send_cmd("ATE0","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))       ret=true;    
	
	}
	return ret;
}
bool    Gsm_Send_AT(u8 *UsartBuf)//��ģ�鷢��"AT"
{
    bool  ret=false; 
	
	 if( Gsm_send_cmd("AT","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))            ret=true;   
	
	  return ret;
}


#endif



