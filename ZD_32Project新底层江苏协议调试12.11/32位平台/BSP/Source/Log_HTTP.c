/************************************************************************************************/
/** @file              : Log_HTTP.c																*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��06��03��															*/
/** @brief             : c file																	*/
/** @description       : �����������--������--�߼���--HTTP Ӧ���ļ�								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**�ļ���     : Log_HTTP.c																		*/
/**����       : ����ʪ��������ת����HTTP��ʽ�ַ�����												*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��06��03��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_HTTP.h"

/************************************************************************************************/
#include <string.h>
#include <stdio.h>


/************************************************************************************************/
#include "BSP.h"

#include "Log_MD5.h"
#include "Log_Time.h"

#include "Mcu_Config.h"

#include "System_Config.h"

/* HTTP���� */
CreateHTTPParaStruct HTTPPara;


/************************************************************************************************/
/* ������	��HTTPSet_Dody																				*/
/* ����		��destbuf:���ݻ���ָ��																				*/
/* ���		��������Ϣ���� 																				*/
/* ����		������������Ϣ					 									 		 		*/
/* ��������	��2015/06/21																		*/
/* ��������	��2015/06/21																		*/
/************************************************************************************************/
static u32 HTTPSet_Dody(char * destbuf)
{
	/* ���� */
	char buf[256];
	/* ̽ͷ��� */
	unsigned char probeNum;
	/* ������ */
	unsigned char num;

	
	
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����Body��ʼ��ǩ */
	sprintf(buf,HTTPBody_Start);
	/* ������������ */
	strcpy(destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* �������ر�� */
	sprintf(buf,	HTTPBody_ProbeIdNo,HTTPPara.ApiPara.GatewayIdNo);
	/* ������������ */
	strcat(destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����ʱ��� */
	sprintf(buf,HTTPBody_Timestamp,HTTPPara.ApiPara.TimeStamp);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	
	/* ���ò���ǩ�� */
	sprintf(buf,"gatewayIdNo=%stimestamp=%dapiKey=%s",
			HTTPPara.ApiPara.GatewayIdNo,
			HTTPPara.ApiPara.TimeStamp,
			HTTP_USER_KEY);
	/* ����MD5���� */
	App_MD5(HTTPPara.ApiPara.Sign,buf);
	/* �������ǩ������ */
	memset(buf,	 0x00, sizeof(buf));
	/* �������ǩ�� */
	sprintf(buf,HTTPBody_Sign,HTTPPara.ApiPara.Sign);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	
	/* ����GPS���� */
	/* �ж�GPS�豸״̬ */
	if((HTTPPara.ApiPara.GPS_State == TRUE)&&
	   (HTTPPara.ApiPara.Lng != GPS_Data_Rece_Timeout_Value)&&
	   (HTTPPara.ApiPara.Lat != GPS_Data_Rece_Timeout_Value))
	{
		/* ����(UCOS-IIʹ��sprintf�����ֽڶ�������) */
		char lngdataStr[16];
		char latdataStr[16];
		
		
		/* ������� */
		memset(lngdataStr,0x00, sizeof(lngdataStr));
		memset(latdataStr,0x00, sizeof(latdataStr));
		
		/* ������ת�����ַ��� */
		float_to_string(HTTPPara.ApiPara.Lng,lngdataStr);
		/* ����GPS�豸�����þ��Ȳ��� */
		sprintf(buf,HTTPBody_Lng,lngdataStr);
		/* ������������ */
		strcat (destbuf,buf);
		/* ������� */
		memset(buf,0x00, sizeof(buf));
		
		/* ������ת�����ַ��� */
		float_to_string(HTTPPara.ApiPara.Lat,latdataStr);
		/* ����GPS�豸������γ�Ȳ��� */
		sprintf(buf,HTTPBody_Lat,latdataStr);
		/* ������������ */
		strcat (destbuf,buf);
		/* ������� */
		memset(buf,0x00, sizeof(buf));
	}
	
	/* ���òɼ�ʱ�� */
	sprintf(buf,HTTPBody_CollectTime,HTTPPara.ApiPara.CollectTime);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����logs��ʼ��ǩ */
	sprintf(buf,HTTPBody_Logs_Start);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����Logs���� */
	for(num = 0,probeNum = 0;(probeNum < HTTPPara.ApiPara.ProbeNum)&&(num < 32) ;num++)
	{
		/* �жϵ�ǰ̽ͷ���� */
		if(Sensor_Tmp == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* ����(UCOS-IIʹ��sprintf�����ֽڶ�������) */
			char tempdataStr[16];
			
			
			/* ������� */
			memset(tempdataStr,0x00, sizeof(tempdataStr));
			/* ���¶�����ת�����ַ��� */
			float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
			/* ����logs */
			sprintf(buf,HTTPBody_Logs_Temp,
			HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
			tempdataStr);

		}else if(Sensor_RF_Card_Single_Tmp == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* ����(UCOS-IIʹ��sprintf�����ֽڶ�������) */
			char tempdataStr[16];
				
			/* ������� */
			memset(tempdataStr,0x00, sizeof(tempdataStr));
			/* �ж��Ƿ���յ����� */
			if(Wireless_RF_Card_Data_Rece_Timeout_Para != HTTPPara.ApiPara.Logs[probeNum].Temp)
			{
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* ����logs */
				sprintf(buf,HTTPBody_Logs_Temp,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr);
			}else
			{
				/* δ���յ����ݣ�ֻ����̽ͷ��ţ����������ݡ� */
				/* ����logs */
				continue;
			}
		}else if(Sensor_Hum == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* ����(UCOS-IIʹ��sprintf�����ֽڶ�������) */
			char humidataStr[16];
			
			
			/* ������� */
			memset(humidataStr,0x00, sizeof(humidataStr));
			/* ���¶�����ת�����ַ��� */
			float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
			/* ����logs */
			sprintf(buf,HTTPBody_Logs_Humi,
			HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
			humidataStr);
		}else if(Sensor_RF_Card_Humiture_Tmp == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* ����(UCOS-IIʹ��sprintf�����ֽڶ�������) */
			char tempdataStr[16];
			char humidataStr[16];
			
			
			/* ������� */
			memset(tempdataStr,0x00, sizeof(tempdataStr));
			memset(humidataStr,0x00, sizeof(humidataStr));
			/* �ж��Ƿ���յ����� */
			if((Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* δ���յ����ݣ�ֻ����̽ͷ��ţ����������ݡ� */
				/* ����logs */
				continue;
//				sprintf(buf,HTTPBody_Logs_Null,HTTPPara.ApiPara.Logs[probeNum].probeIdNo);
			}else if((Wireless_RF_Card_Data_Rece_Timeout_Para != HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* ֻ���¶����� */
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* ����logs */
				sprintf(buf,HTTPBody_Logs_Temp,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr);
			}else if((Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para != HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* ֻ��ʪ�ȶ����� */
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
				/* ����logs */
				sprintf(buf,HTTPBody_Logs_Humi,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				humidataStr);
			}else
			{
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
				/* ����logs */
				sprintf(buf,HTTPBody_Logs_TempHumi,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr,
				humidataStr);
			}
			
		}else if(Sensor_GW_NJSY_Tmp == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* ����(UCOS-IIʹ��sprintf�����ֽڶ�������) */
			char tempdataStr[16];
			char humidataStr[16];
			
			
			/* ������� */
			memset(tempdataStr,0x00, sizeof(tempdataStr));
			memset(humidataStr,0x00, sizeof(humidataStr));
			/* �ж��Ƿ���յ����� */
			if((Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* δ���յ����ݣ�ֻ����̽ͷ��ţ����������ݡ� */
				/* ����logs */
				continue;
//				sprintf(buf,HTTPBody_Logs_Null,HTTPPara.ApiPara.Logs[probeNum].probeIdNo);
			}else if((Gateway_NJSY_TimeoutPara != HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* ֻ���¶����� */
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* ����logs */
				sprintf(buf,HTTPBody_Logs_Temp,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr);
			}else if((Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara != HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* ֻ��ʪ�ȶ����� */
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
				/* ����logs */
				sprintf(buf,HTTPBody_Logs_Humi,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				humidataStr);
			}else
			{
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* ���¶�����ת�����ַ��� */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
				/* ����logs */
				sprintf(buf,HTTPBody_Logs_TempHumi,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr,
				humidataStr);
			}
		}else
		{
			continue;
		}
		/* ������������ */
		strcat (destbuf,buf);
		/* ������� */
		memset(buf,0x00, sizeof(buf));
		
		/* ����̽ͷ��� */
		probeNum++;
		/* ����Json�������� */
		if(probeNum < HTTPPara.ApiPara.ProbeNum)
		{
			/* ������������ */
			strcat (destbuf,",");
		}
	}
	
	/* ����logs������ǩ */
	sprintf(buf,HTTPBody_Logs_End);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����Body������ǩ */
	sprintf(buf,HTTPBody_End);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	return strlen(destbuf);
}

/************************************************************************************************/
/* ������	��HTTPSetState_Dody																				*/
/* ����		��destbuf:���ݻ���ָ��																				*/
/* ���		��������Ϣ���� 																				*/
/* ����		������������Ϣ					 									 		 		*/
/* ��������	��2015/09/20																		*/
/* ��������	��2015/09/20																		*/
/************************************************************************************************/
static u32 HTTPSetState_Dody(char * destbuf)
{
	/* ���� */
	char buf[256];
//	/* ̽ͷ��� */
//	unsigned char probeNum;
//	/* ������ */
//	unsigned char num;


	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����Body��ʼ��ǩ */
	sprintf(buf,HTTPBody_Start);
	/* ������������ */
	strcpy(destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* �������ر�� */
	sprintf(buf,	HTTPBody_ProbeIdNo,HTTPPara.ApiState.GatewayIdNo);
	/* ������������ */
	strcat(destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����ʱ��� */
	sprintf(buf,HTTPBody_Timestamp,HTTPPara.ApiState.TimeStamp);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ���ò���ǩ�� */
	sprintf(buf,"gatewayIdNo=%stimestamp=%dapiKey=%s",
			HTTPPara.ApiState.GatewayIdNo,
			HTTPPara.ApiState.TimeStamp,
			HTTP_USER_KEY);
	/* ����MD5���� */
	App_MD5(HTTPPara.ApiState.Sign,buf);
	/* �������ǩ������ */
	memset(buf,	 0x00, sizeof(buf));
	/* �������ǩ�� */
	sprintf(buf,HTTPBody_Sign,HTTPPara.ApiState.Sign);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����"��Դ״̬"���� */
	sprintf(buf,HTTPBody_PowerState,(HTTPPara.ApiState.PowerState==1)?1:2);
	/* ������������ */
	strcat(destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����"���ص���"���� */
	sprintf(buf,HTTPBody_BatteryLevel,HTTPPara.ApiState.BatteryLevel);
	/* ������������ */
	strcat(destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	/* ����Body������ǩ */
	sprintf(buf,HTTPBody_End);
	/* ������������ */
	strcat (destbuf,buf);
	/* ������� */
	memset(buf,0x00, sizeof(buf));
	
	return strlen(destbuf);
}


/************************************************************************************************/
/* ������	��HTTPSet_Header																	*/
/* ����		��destbuf:���ݻ���ָ��																*/
/* ���		��������Ϣ���� 																		*/
/* ����		������������Ϣ					 									 		 		*/
/* ��������	��2015/06/23																		*/
/* ��������	��2015/06/23																		*/
/************************************************************************************************/
static void HTTPSet_Header(char * destbuf,u32 ContentLen)
{	
	/* ���� */
	char contentStr[256];
	
	/* ������� */
	memset(contentStr,0x00, sizeof(contentStr));
	
	/* ����Body������Ϣ */
	sprintf(contentStr,HTTPHeader_ContentLength,ContentLen);

	/* �����ײ��ֶ����� */
	sprintf(destbuf,"%s%s%s%s%s",
			HTTPHeader_Host,
			contentStr,
			HTTPHeader_ContentType,
			HTTPHeader_Connection,
			HTTPHeader_End);
}

/************************************************************************************************/
/* ������	��HTTP_Create_Data																	*/
/* ����		����																					*/
/* ���		���� 																				*/
/* ����		������HTTP����					 									 		 		*/
/* ��������	��2015/06/03																		*/
/* ��������	��2015/06/03																		*/
/************************************************************************************************/
void HTTP_Create_Data(char * destbuf)
{
	/* ��ʼ�� */
	char requestLine[128];
	/* �ײ��ֶ� */
	char headers[128];
	/* ���� */
	char bodyJson[1280];
	/* ������Ϣ���� */
	u32  bodyLen = 0;
	
	
	/* ������� */
	memset(requestLine,0x00, sizeof(requestLine));
	memset(headers,0x00, sizeof(headers));
	memset(bodyJson,0x00, sizeof(bodyJson));
	
	/* ������ʼ������ */
	sprintf(requestLine,"%s %s %s\r\n",
			HTTPRequestLine_Method,
			HTTPRequestLine_Path,
			HTTPRequestLine_Verison);
	
	/* ������������ */
	bodyLen = HTTPSet_Dody(bodyJson);
	
	/* �����ײ��ֶ����� */
	HTTPSet_Header(headers,bodyLen);
	
	/* HTTP ���� */
	sprintf(destbuf,"%s%s%s",
			requestLine,
			headers,
			bodyJson);
}


/************************************************************************************************/
/* ������	��HTTP_Create_State																	*/
/* ����		��destbuf:���ݻ���																					*/
/* ���		���� 																				*/
/* ����		������HTTP����					 									 		 		*/
/* ��������	��2015/09/20																		*/
/* ��������	��2015/09/20																		*/
/************************************************************************************************/
void HTTP_Create_State(char * destbuf)
{
	/* ��ʼ�� */
	char requestLine[128];
	/* �ײ��ֶ� */
	char headers[128];
	/* ���� */
	char bodyJson[512];
	/* ������Ϣ���� */
	u32  bodyLen = 0;
	
	
	/* ������� */
	memset(requestLine,0x00, sizeof(requestLine));
	memset(headers,0x00, sizeof(headers));
	memset(bodyJson,0x00, sizeof(bodyJson));
	
	/* ������ʼ������ */
	sprintf(requestLine,"%s %s %s\r\n",
			HTTPRequestLine_Method,
			HTTPRequestLine_Path_State,
			HTTPRequestLine_Verison);
			
	/* ������������ */
	bodyLen = HTTPSetState_Dody(bodyJson);
	
	/* �����ײ��ֶ����� */
	HTTPSet_Header(headers,bodyLen);
	
	/* HTTP ���� */
	sprintf(destbuf,"%s%s%s",
			requestLine,
			headers,
			bodyJson);

}


/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
