/************************************************************************************************/
/** @file              : App_Gateway_NJSY.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月18日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--南京盛亿网关应用层文件							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2016年11月18日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __App_Gateway_NJSY_h
#define	__App_Gateway_NJSY_h

#include "System_Config.h"
#if (Gateway_NJSY_Enable == 0x01)
#include "Mcu_Config.h"
#include "Log_Gateway_NJSY.h"
#include "ucos_ii.h"

/********************************* 功能选项 *********************************/
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h> 南京盛亿网关功能设置.
//	<e0> 调试信息.
//	<i>  目前调试信息中的项目只有1个.
//  <o1.0..2> 判断调试信息
//	<i>  调试阶段需要判断条件
//  <0=> [关闭]调试信息.
//  <1=> [打开]调试信息.
#define AppGatewayDebugEnable			(0x01)
#define AppGatewayDebugInform			(1)
//  </e>


// </h>
//	<<< end of configuration section >>>

/********************************* 网关运行参数类型 *********************************/
/* 网关任务堆栈大小 */
#define AppTask_GatewayNJSY_StkSize 	(128u)
/* 网关初始化参数 */
typedef INSTRU_Gateway_FIX  AppGWInitPara; 


/********************************* 网关应用接口结构 *********************************/
typedef struct
{	
	void 	  (*Init)			(AppGWInitPara* initPara);
	GW_Result (*Restart)		(void);		
	void 	  (*ConfigFunc)		(void);	
	GW_Result (*ReadProbe)		(uint8_t probeNo,GW_ProbeType probetype,float* probe);
	GW_Result (*ReadLevel)		(uint8_t probeNo,GW_Level* level);
	GW_Result (*UpdataProbeData)(void);	
	void      (*UpdataState)	(void);
	GW_Result (*SetAdder)		(uint8_t adder);
	GW_Result (*SetChannel)		(uint8_t channel);
	GW_Result (*SetActiveTime)	(uint8_t time);
	GW_Result (*SetMode)		(GW_Mode mode);
	GW_Result (*SetBaudrate)	(uint32_t baudrate);
	
}AppGatewayAPIStruct;
/********************************* 应用接口声明 *********************************/
extern AppGatewayAPIStruct AppGW_API;
extern OS_STK AppTaskGatewayNJSYStk[AppTask_GatewayNJSY_StkSize];

/* ******************* 南京盛亿网关操作函数声明 ******************* */
void AppTaskGatewayNJSY(void *p_arg);
void BSP_AppGateway_Init(void);


#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
