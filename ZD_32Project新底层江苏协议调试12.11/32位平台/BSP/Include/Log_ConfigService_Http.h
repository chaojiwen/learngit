/************************************************************************************************/
/** @file              : Log_ConfigService_Http.H													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��09��20��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���÷�����(Http)Ӧ�ò��ļ�							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2016 (C) HZ Zeda Team																*/
/**																								*/
/**�ļ���     : Log_ConfigService_Http.H																*/
/**����       : ���÷�����Ӧ�ù���																*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��09��20��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_GSM.h"
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x01)

#ifndef __Log_ConfigService_Http_H
#define __Log_ConfigService_Http_H

#include "Mcu_Config.h"



/****************************************������**************************************************/
/************************************************************************************************/ 


/* ���÷�����Ӧ�ò��� */
typedef struct
{
	/* ͬ��״̬ */
	BOOL SYNC_State;
	/* ����״̬ */
	BOOL Run_State;
}ConfigService_AppDef;


/*********************************  ��������  ********************************/
void ConfigService_TXLog(INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_RXLog(GPRS_Struct gprs,INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_StopRun(void);
void ConfigService_StartRun(void);
#endif
#endif
#endif
/******************* (C) HZ Zeda Team *****END OF FILE****/
