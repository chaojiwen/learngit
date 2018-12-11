/************************************************************************************************/
/** @file              : Log_ConfigService_Http.H													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年09月20日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--配置服务器(Http)应用层文件							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2016 (C) HZ Zeda Team																*/
/**																								*/
/**文件名     : Log_ConfigService_Http.H																*/
/**作用       : 配置服务器应用功能																*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年09月20日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_GSM.h"
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x01)

#ifndef __Log_ConfigService_Http_H
#define __Log_ConfigService_Http_H

#include "Mcu_Config.h"



/****************************************参数表**************************************************/
/************************************************************************************************/ 


/* 配置服务器应用参数 */
typedef struct
{
	/* 同步状态 */
	BOOL SYNC_State;
	/* 操作状态 */
	BOOL Run_State;
}ConfigService_AppDef;


/*********************************  函数声明  ********************************/
void ConfigService_TXLog(INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_RXLog(GPRS_Struct gprs,INSTRU_CONFIG_INF_TAB* scrPara);
void ConfigService_StopRun(void);
void ConfigService_StartRun(void);
#endif
#endif
#endif
/******************* (C) HZ Zeda Team *****END OF FILE****/
