/************************************************************************************************/
/** @file              : Log_Gateway_HZZH.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月15日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--杭州创辉网关逻辑层文件									*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2016年11月15日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __Log_Gateway_HZZH_h
#define	__Log_Gateway_HZZH_h
#include "System_Config.h"

#if (Gateway_HZZH_Enable == 0x01)

/********************************* 宏定义 *********************************/



/********************************* 网关运行参数类型 *********************************/
/* 杭州创辉网关运行结果 */
typedef enum {GWHZZH_Fail = (0u), GWHZZH_Success = (1u)}  GWHZZH_Result;
/* 杭州创辉网关命令类型 */
typedef enum {
				/* 无 */
				GWHZZHCmd_Null = (0X01),	
				/* 探头响应 */
				GWHZZHCmd_ProbeDataACK = (0X01),	
				/* 设置唤醒时间 */
				GWHZZHCmd_SetWkupTimeProbeNum = (0X81),
				/* 设置唤醒时间响应 */
				GWHZZHCmd_SetWkupTimeProbeNumACK = (0X82),
				/* 读唤醒时间 */
			    GWHZZHCmd_ReadWkupTimeProbeNum = (0X83),
				/* 读唤醒时间响应 */
				GWHZZHCmd_ReadWkupTimeProbeNumACK = (0X84),
			 }  GWHZZHCmdType;



/********************************* 网关API结构 *********************************/


typedef struct
{	
	void	 		 (*Init)		(void);
	void	 		 (*Restart)		(void);
	GWHZZH_Result	 (*State)		(void);
	GWHZZH_Result	 (*ReadWkupTimeProbeNum)	(void);
	GWHZZH_Result	 (*SetWkupTime)	(uint8_t time,uint8_t probeNum);
	GWHZZH_Result	 (*SetProbeNum)	(uint8_t time,uint8_t probeNum);
	
}LogGatewayHZZHAPIStruct;

/********************************* 接口声明 *********************************/
extern LogGatewayHZZHAPIStruct LogGWHZZHAPI;

/* ******************* Gateway_HZZH 函数声明 ******************* */
void LogGW_InitAPI(void (*probeDataACKp)(uint8_t* id,uint8_t* data),
				   void (*setWkupTimeProbeNumACKp)(void),
				   void (*readWkupTimeProbeNumACKp)(uint8_t wkupTime,uint8_t ProbeNum));


#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
