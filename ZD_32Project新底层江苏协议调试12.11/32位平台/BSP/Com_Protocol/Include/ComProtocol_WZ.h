/************************************************************************************************/
/** @file              : ComProtocol_WZ.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017年02月13日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--变送器--通信协议--万智String协议 应用文件					*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2017年02月13日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __ComProtocol_WZ_h
#define	__ComProtocol_WZ_h
#include "Log_GSM.h"
#if (ComProtocol_WZ_Enable == 0x01)
#include "App_Save.h"
#include "interface.h"

/********************************* API结构 ******************************/


/********************************* 万智String协议通信协议应用数据结构 **************/
typedef struct
{
	/* 配置寄存器 */
	struct
	{
		/* 万智String协议网络接口 */
		const GPRS_Connection NetworkConnection;
		
		
	}CR;
	
	/* 状态寄存器 */
	struct
	{
		/* 创建数据标志 */
		BOOL CreateDataFlag;
	
	}SR;
	
	/* 数据寄存器 */
	struct
	{
		/* 万智String协议数据寄存器 */
		uint8_t TXDR[1024ul];
		uint8_t RXDR[128ul];
	}DR;
	
}ComProtocolWZDRStruct;

/********************************* 全局变量声明 *****************************/
/* 万智String协议数据寄存器大小 */
#define ProbeDataRegSize			(1024ul)
/* 万智String协议数据寄存器 */
extern ComProtocolWZDRStruct		ComProtocolWZDR;

/********************************* 函数声明 *********************************/
BOOL ComProtWZ_CreateData(Save_Sensor_Data_Struct* cSaveFrameReg);
BOOL ComProtWZ_ParseData(uint8_t* receBuf);


#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
