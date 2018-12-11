/************************************************************************************************/
/** @file              : Log_Gateway_NJSY.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年11月15日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--网关逻辑层文件									*/
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
#ifndef __Log_Gateway_NJSY_h
#define	__Log_Gateway_NJSY_h
#include "System_Config.h"

#if (Gateway_NJSY_Enable == 0x01)

/********************************* 宏定义 *********************************/
/* 网关接收超时时间 */
#define LogGW_ReceOvertimeValue	(20u)     // 
/* 网关重启时间 */
#define LogGW_RestartTimeValue	(50u)



/********************************* 网关运行参数类型 *********************************/
/* 探头类型 */
typedef enum {GWProbe_Temp = (0u), GWProbe_Hum = (1u)} GW_ProbeType;
/* 网关运行结果 */
typedef enum {GW_Fail = (0u), GW_Success = (1u)}  GW_Result;
/* 网关工作模式 */
typedef enum {GW_Release = (0u), GW_Debug = (1u)} GW_Mode;
/* 网关电池电量状态 */
typedef enum {GW_Level_FALSE = (0u), GW_Level_TRUE = (1u)} GW_Level;

/********************************* 网关API结构 *********************************/
typedef struct
{	
	/* 网关地址 */
	uint8_t Adder;
	/* 信道(波段) */
	uint8_t Channel;
	/* 数据有效时间 */
	uint8_t ActiveTime;
	/* 工作模式（正常/调试） */
	GW_Mode Mode;
	/* 通信波特率 */
	uint32_t Baudrate;
}LogGatewayInitParaStruct;

typedef struct
{	
	void	  (*Init)			(LogGatewayInitParaStruct initPara);
	GW_Result (*Restart)		(void);
	GW_Result (*ConfigFunc)		(void);
	GW_Result (*ReadProbe)		(uint8_t probeNo,GW_ProbeType probetype,float* probe);
	GW_Result (*ReadLevel)		(uint8_t probeNo,GW_Level* level);
	GW_Result (*UpdataProbeData)(void);
	GW_Result (*UpdataState)	(void);
	GW_Result (*ReadAdder)		(uint8_t* adder);	
	GW_Result (*SetAdder)		(uint8_t adder);
	GW_Result (*SetChannel)		(uint8_t channel);
	GW_Result (*SetActiveTime)	(uint8_t time);
	GW_Result (*SetMode)		(GW_Mode mode);
	GW_Result (*SetBaudrate)	(uint32_t baudrate);
	
}LogGatewayAPIStruct;

/********************************* 接口声明 *********************************/
extern LogGatewayAPIStruct LogGW_API;

/* ******************* Gateway_NJSY 函数声明 ******************* */
void BSP_LogGW_API_Init(void);


#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
