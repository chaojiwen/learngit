/************************************************************************************************/
/** @file              : App_Gateway_HZZH.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年12月15日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--杭州创辉网关应用层文件							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2016年12月15日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __App_Gateway_HZZH_h
#define	__App_Gateway_HZZH_h

#include "System_Config.h"
#if (Gateway_HZZH_Enable == 0x01)
#include "Mcu_Config.h"
#include "Log_Gateway_HZZH.h"
#include "ucos_ii.h"

/********************************* 功能选项 *********************************/
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h> 杭州创辉网关功能设置.
//	<e0> 调试信息.
//	<i>  目前调试信息中的项目只有1个.
//  <o1.0..2> 判断调试信息
//	<i>  调试阶段需要判断条件
//  <0=> [关闭]调试信息.
//  <1=> [打开]调试信息.
#define AppGatewayHZZHDebugEnable		(0x01)
#define AppGatewayHZZHDebugInform		(1)
//  </e>


// </h>
//	<<< end of configuration section >>>

/********************************* 网关运行参数类型 *********************************/
/* 网关任务堆栈大小 */
#define AppTask_GatewayHZZH_StkSize 	(128u)

/********************************* 网关探头数据结构 *********************************/
typedef struct
{
	/* 电池电量整数 */
	u32 PowerInteger		:4;
	/* 电池电量小数 */
	u32 PowerDecimal		:4;
	/* 温度整数 */
	u32 TempInteger			:7;
	/* 温度正负标志 */
	u32 TempPM				:1;
	/* 湿度小数 */
	u32 HumiDecimal			:4;
	/* 温度小数 */
	u32 TempDecimal			:4;
	/* 湿度整数 */
	u32 HumiInteger			:8;
}AppGatewayHZZHProbeDataStruct;
/********************************* 网关应用接口结构 *********************************/
typedef struct
{	
	void 			(*Init)					(void);
	void 			(*Restart)				(void);
	GWHZZH_Result	(*UpdateState)			(void);
	GWHZZH_Result	(*GetState)				(void);
	GWHZZH_Result	(*ReadWkupTimeProbeNum)	(uint16_t* time,uint16_t* probeNum);	
	GWHZZH_Result	(*SetWkupTime)			(uint16_t time,uint16_t probeNum);
	GWHZZH_Result	(*SetProbeNum)			(uint16_t time,uint16_t probeNum);
	
	
}AppGatewayHZZHAPIStruct;
/********************************* 应用接口声明 *********************************/
extern AppGatewayHZZHAPIStruct AppGWHZZH_API;
extern OS_STK AppTaskGatewayHZZHStk[AppTask_GatewayHZZH_StkSize];

/* ******************* 杭州创辉网关操作函数声明 ******************* */
void AppTaskGatewayHZZH(void *p_arg);



#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
