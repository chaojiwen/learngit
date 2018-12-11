/************************************************************************************************/
/** @file              : Log_ConfigService_Http.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��09��20��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���÷�����(Http)Ӧ�ò��ļ�						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��09��20��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_ConfigService_Http.h"

#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x01)//HttpЭ��


#include "Dev_Fram.h"
#include "Dev_RTC.h"
#include "Log_HTTP.h"
#include "Log_IAP.h"
#include "app.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>



/* ���ݼĴ��� */
/* �������ݼĴ��� */
static uint8_t ConfigService_TXDR[1536];
/* �������ݼĴ��� */
static uint8_t ConfigService_RXDR[1500];



#if ((AppGPRSComProtocolEnable == 0x01)&&(AppGPRSComProtocol == 0x01))
extern CreateHTTPParaStruct HTTPPara;
#endif	
/* ���÷�����Ӧ�ò��� */
ConfigService_AppDef ConfigService_App = 
{
	FALSE,
	FALSE,
};

/************************************************************************************************/
/* ������	: ConfigService_StopRun												 				*/
/* ����  	: ��			 																	*/
/* ���		: ��		 																		*/
/* ����	 	: ֹͣ����								 											*/
/* �������� 	: 2016/08/30																		*/
/* �޸����� 	: 2016/08/30																		*/
/*   �汾 	: Ver:0																				*/
/************************************************************************************************/
void ConfigService_StopRun(void)
{
	/* ֹͣ���� */
	ConfigService_App.Run_State = FALSE;
}

/************************************************************************************************/
/* ������	: ConfigService_StartRun												 				*/
/* ����  	: ��			 																	*/
/* ���		: ��		 																		*/
/* ����	 	: ��������								 											*/
/* �������� 	: 2016/08/30																		*/
/* �޸����� 	: 2016/08/30																		*/
/*   �汾 	: Ver:0																				*/
/************************************************************************************************/
void ConfigService_StartRun(void)
{
	/* ֹͣ���� */
	ConfigService_App.Run_State = TRUE;
}


/************************************************************************************************/
/* ������	: ConfigService_TXLog															 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���÷�������������ҵ������								 							*/
/* �������� 	: 2016/08/11																		*/
/* �޸����� 	: 2016/08/16																		*/
/*   �汾 	: V0: ����������ҵ���߼�															*/
/*   �汾 	: V1: ��ԭ���ġ�����ҵ�񡱸�Ϊ���ڵġ���������ҵ��									*/
/************************************************************************************************/
void ConfigService_TXLog(INSTRU_CONFIG_INF_TAB* scrPara)
{
	
	/* ��ʱʱ�仺�� */
	Times time;
	/* ���ر��ָ�� */
	SERIAL_NUMBER* gatewayIdNoP = &scrPara->Inf_State.Instru_System.Inf.Serial_Number;
	
	
	/* �ж��Ƿ����ڽ������÷��������� */
	if(ConfigService_App.Run_State == TRUE)
	{
		/* ���ڽ������÷��������������ٷ�������������� */
		return;
	}
	
	/* �������÷���������״̬ */
	ConfigService_StartRun();
	
	/* �����ϴ�״̬���� */
	/* ������ݽӿڲ��� */
	memset((char*)&HTTPPara.ApiState,0x00,sizeof(HTTPPara.ApiState));
	
	/* �������ر�� */
	sprintf(HTTPPara.ApiState.GatewayIdNo,"%04X%05d",
	((((*(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0x00ff) << 8)),
	((((*(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0x00ff) << 8)));
	
	/* �����ϴ�ʱ��� */
	time.Year 	= scrPara->Inf_State.Instru_RTC.Inf_State.Year;
	time.Month	= scrPara->Inf_State.Instru_RTC.Inf_State.Month;
	time.Day 	= scrPara->Inf_State.Instru_RTC.Inf_State.Day;
	time.Hour 	= scrPara->Inf_State.Instru_RTC.Inf_State.Hour;
	time.Min 	= scrPara->Inf_State.Instru_RTC.Inf_State.Min;
	time.Sec 	= scrPara->Inf_State.Instru_RTC.Inf_State.Sec;
	/* �������Ĳɼ�ʱ��ת�����룬�������������Ϊ����ǩ����ʱ�����ݡ� */
	HTTPPara.ApiState.TimeStamp = RTC_Date_Time_To_Second(&time);
	
	/* �������ǩ�����ݻ��档 */
	memset(HTTPPara.ApiState.Sign,0x00,sizeof(HTTPPara.ApiState.Sign));
	
	/* ���õ�Դ״̬ */
	HTTPPara.ApiState.PowerState = scrPara->Inf_State.Instru_Power.Inf_State.ET_Link_State;
	
	/* ���õ�ص��� */
	HTTPPara.ApiState.BatteryLevel = scrPara->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	
	/* ����״̬���ݡ� */
	HTTP_Create_State((char*)ConfigService_TXDR);
	
	/* ����GPRS�������ݲ��� */
	GPRS.ConfigService.Para.TXDR	= ConfigService_TXDR;
	GPRS.ConfigService.Para.TXLen	= strlen((char*)ConfigService_TXDR);
	GPRS.ConfigService.Para.RXDR	= ConfigService_RXDR;
	GPRS.ConfigService.Para.RXLen	= 1500;
	
	/* ���Ͷ���(ConfigService) */
	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,ConfigService_EVENT);

	/* �������÷���������״̬ */
	ConfigService_StartRun();
}

/************************************************************************************************/
/* ������	: ConfigService_ParseData															*/
/* ����  	: ��								 												*/
/* ���		: ��		 																		*/
/* ����	 	: ���÷�������������								 									*/
/* �������� 	: 2016/08/16																		*/
/* �޸����� 	: 2016/08/16																		*/
/*   �汾 	: V0: ����																			*/
/************************************************************************************************/
static BOOL ConfigService_ParseData(uint8_t* string)
{
	
	/* ����ָ�� */
	char * datap;
	
	/* ��ѯHTTP����ɹ��ַ��� */
	datap = strstr((char*)string,"HTTP/1.1 200 OK");
	/* �ж�HTTP�����Ƿ�ɹ� */
	if(NULL == datap)
	{
		/* ����ʧ�� */
		return FALSE;
	}
	
	/* ��ѯ����ƽ̨�ɹ������ַ��� */
	datap = strstr((char*)string,"\"code\":\"0000\"");
	/* �ж�����ƽ̨�Ƿ�ɹ��������� */
	if(NULL == datap)
	{
		/* ����ʧ�� */
		return FALSE;
	}

	return TRUE;
}

/************************************************************************************************/
/* ������	: ConfigService_RXLog												 				*/
/* ����  	: gprs:GPRS���в���.			 														*/
/* ���		: ��		 																		*/
/* ����	 	: ���÷�������������ҵ������								 							*/
/* �������� 	: 2016/08/16																		*/
/* �޸����� 	: 2016/08/16																		*/
/*   �汾 	: V0: ����																			*/
/************************************************************************************************/
void ConfigService_RXLog(GPRS_Struct gprs,INSTRU_CONFIG_INF_TAB* scrPara)
{
	/* ����״̬ */
	BOOL parseState = FALSE;
	
	/* �жϽ�������״̬ */
	if(gprs.ConfigService.State.Result == TRUE)
	{
		/* �������ݳɹ� */
		
		/* �������� */
		parseState = ConfigService_ParseData(gprs.ConfigService.Para.RXDR);
		
		/* �ж����ݽ���״̬ */
		if(parseState == TRUE)
		{
			/* ���ݽ����ɹ� */
			__nop();
		}else
		{
			/* ���ݽ���ʧ�� */
			__nop();
		}
	}else
	{
		/* ͨ��ʧ�� */
		__nop();
	}

	/* ������÷�������������״̬��־ */
	ConfigService_StopRun();
}

#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
