/************************************************************************************************/
/** @file              : ComProtocol_ZDE.h														*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017年02月16日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--变送器--通信协议--“泽大通用协议”应用文件					*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : ComProtocol_ZDE.h																*/
/**作用       : 根据“泽大通用协议”提供的通信协议格式将探头数据上传到云平台							*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2017年02月16日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#ifndef __ComProtocol_ZDE_h
#define	__ComProtocol_ZDE_h
#include "Log_GSM.h"
#if (ComProtocol_ZDE_Enable == 0x01)
#include "App_Save.h"

/********************************* 宏定义变量 **************/
/* “泽大通用协议”发送数据寄存器大小 */
#define ComProtocolZDE_TXSize		(1024ul)
/* “泽大通用协议”接收数据寄存器大小 */
#define ComProtocolZDE_RXSize		(128ul)

/********************************* “泽大通用协议”应用数据结构 **************/
typedef struct
{
	/* 配置寄存器 */
	struct
	{
		uint8_t null;
		
	}CR;
	
	/* 状态寄存器 */
	struct
	{
		uint8_t null;
	
	}SR;
	
	/* 数据寄存器 */
	struct
	{
		/* “泽大通用协议”数据寄存器 */
		uint8_t TXDR[ComProtocolZDE_TXSize];
		uint8_t RXDR[ComProtocolZDE_RXSize];
		/* 发送数据长度 */
		uint16_t TXLen;
	}DR;
	
}
ComProtocolZDEAppStruct;

/********************************* 全局变量声明 *****************************/
/* “泽大通用协议”数据寄存器 */
extern ComProtocolZDEAppStruct		ComProtocolZDEApp;

/********************************* 函数声明 *********************************/
BOOL ComProtocolZDE_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,
							   uint8_t* destBuf,int* dataLen);
BOOL ComProtZDE_ParseData(uint8_t* destBuf,int* dataLen);

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

