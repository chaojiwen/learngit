/************************************************************************************************/
/** @file              : Log_IAP.H																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年08月24日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--固件升级应用层文件								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2016 (C) HZ Zeda Team																*/
/**																								*/
/**文件名     : Log_IAP.H																		*/
/**作用       : 固件升级应用功能																	*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年08月24日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_GSM.h"

#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)
#ifndef __Log_IAP_H
#define __Log_IAP_H

#include "stm32f10x.h"
#include "Log_ConfigService.h"

/****************************************参数表**************************************************/
/************************************************************************************************/ 
/* 固件升级应用参数结构 */
typedef struct
{
	/* 操作状态 */
	BOOL RunState;
	/* 升级标志 */
	uint8_t* UpdateFlag;
	/* 升级状态 */
	BOOL UpdateState;
	/* 升级进度百分比 */
	uint8_t UpdatePercent;
}
IAP_AppStruct;


/* 固件升级应用参数 */
extern IAP_AppStruct IAP_App;



/*********************************  函数声明  ********************************/
void IAP_UploadLog(GPRS_Struct* gprs);
void IAP_CheckFlag(void);
void IAP_ParaInit(ConfigService_Para_Update* updateparap,uint8_t* updateflag);
BOOL IAP_UpdateSatte(void);
void IAP_StartRun(void);
void IAP_StopRun(void);

void mcu_reset(void);//重启设备  复位单片机
#endif
#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
