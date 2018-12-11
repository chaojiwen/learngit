/************************************************************************************************/
/** @file              : App_Gateway_NJSY.c														*/
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
#include "App_Gateway_NJSY.h"
#if (Gateway_NJSY_Enable == 0x01)

/********************************* 宏定义 *********************************/
/* 网关接收超时时间 */
#define AppGW_CmdOvertimeValue	(LogGW_ReceOvertimeValue*10+100)
/* 网关重启时间 */
#define AppGW_RestartTimeValue	(LogGW_RestartTimeValue*10)

/********************************* 南京盛亿网关 参数类型 *********************************/
#if (1)
/* 网关功能码类型 */
typedef enum 
{
	/* 初始化网关 */
	GWCMD_Init 			= (0u), 
	/* 重启网关功能 */
	GWCMD_Restart		= (1u),
	/* 配置网关功能 */
	GWCMD_ConfigFunc	= (2u),
	/* 读数据 */
	GWCMD_ReadData 		= (3u), 
	/* 读电量 */
	GWCMD_ReadLevel 	= (4u), 
	/* 更新网关数据 */
	GWCMD_UpdataProbeData = (5u), 
	/* 更新网关状态 */
	GWCMD_UpdataState   = (6u),
	/* 设置网关地址 */
	GWCMD_SetAdder 		= (7u),
	/* 设置网关信道(波段) */
	GWCMD_SetChannel 	= (8u),
	/* 设置网关数据有效时间 */
	GWCMD_SetActiveTime = (9u),
	/* 设置网关工作模式 */
	GWCMD_SetMode 		= (10u),
	/* 设置网关通信波特率 */
	GWCMD_SetBaudrate 	= (11u),
}GWCMDType;

typedef struct
{
	/* 命令类型 */
	GWCMDType Type;
	/* 命令参数 */
    void* Para;
}GWCMDStruct;

/* 初始化网关 消息队列 数据结构 */
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
}GWInitStruct;

/* 重启网关功能 消息队列 数据结构 */
typedef struct
{	
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
}GWRestartStruct;

/* 配置网关功能 消息队列 数据结构 */
typedef struct
{	
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
}GWConfigFuncStruct;

/* 读探头数据 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 数据指针 */
	float* Data;
	/* 网关探头类型 */
	GW_ProbeType Type;
	/* 探头编号 */
	uint8_t No;
}GWReadProbeStruct;

/* 读探头电量状态 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 数据指针 */
	GW_Level* Level;
	/* 探头编号 */
	uint8_t No;
}GWReadLevelStruct;

/* 更新网关状态 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 网关地址 */
	uint8_t* Adder;
}GWUpdataStateStruct;

/* 更新网关数据 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
}GWUpdataProbeDataStruct;

/* 设置网关地址 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 网关地址 */
	uint8_t Adder;
	
}GWSetAdderStruct;

/* 设置网关信道(波段) 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 网关通信信道 */
	uint8_t Channel;
	
}GWSetChannelStruct;

/* 设置网关数据有效时间 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 网关数据有效时间 */
	uint8_t ActiveTime;
	
}GWSetActiveTimeStruct;

/* 设置网关工作模式 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 网关工作模式 */
	GW_Mode Mode;
	
}GWSetModeStruct;

/* 设置网关通信波特率 消息队列 数据结构 */
typedef struct
{
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 网关通信波特率 */
	uint32_t Baudrate;
}GWSetBaudrateStruct;


#endif
/********************************* 南京盛亿网关 应用参数 *********************************/
/********************************* 创建任务参数 *********************************/
/* 任务堆栈 */
OS_STK AppTaskGatewayNJSYStk[AppTask_GatewayNJSY_StkSize];


/********************************* 消息队列 *********************************/
/* 消息队列 */
static  OS_EVENT* GW_NJSY_Q;
/* 消息队列长度 */
#define GW_NJSY_Q_Len		((u8)OS_MAX_QS)
/* 控制块数组 */
static void* GW_NJSY_Grp[GW_NJSY_Q_Len];

/********************************* 运行参数 *********************************/
static AppGWInitPara* AppGWRunPara = 
(AppGWInitPara*)&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Gateway;

/********************************* 应用接口定义 *********************************/
AppGatewayAPIStruct AppGW_API;

#if (1)//定时更新网关探头数据回调函数
/* 定时更新网关探头数据软件定时器 句柄指针 */
static OS_TMR* AppGW_UpdataProbeData_ptmr;
	
/************************************************************************************************/
/* 函数名	: AppGW_UpdataProbeData_CallBack													*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 定时更新网关探头数据						 										*/
/* 创建日期	: 2016/11/21																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGW_UpdataProbeData_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* 更新网关探头数据 */
	AppGW_API.UpdataProbeData();
}

#endif

#if (1)//定时更新网关工作状态回调函数
/* 定时更新网关工作状态软件定时器 句柄指针 */
static OS_TMR* AppGW_WorkState_ptmr;
	
/************************************************************************************************/
/* 函数名	: AppGW_UpdataState_CallBack										 		 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 定时更新取网关工作状态						 										*/
/* 创建日期	: 2016/11/21																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGW_UpdataState_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* 更新网关探头数据 */
	AppGW_API.UpdataState();
}

#endif

#if (1)//定时配置网关功能回调函数
/* 定时配置网关功能软件定时器 句柄指针 */
static OS_TMR* AppGW_ReConfigFunc_ptmr;
	
/************************************************************************************************/
/* 函数名	: AppGW_ReConfigFunc_CallBack										 		 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 定时读取网关工作状态						 										*/
/* 创建日期	: 2016/11/21																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGW_ReConfigFunc_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* 启动网关，初始化配置 */
	AppGW_API.ConfigFunc();
}

#endif
/***********************************************************************************/
#if (1)//网关功能函数
/************************************************************************************************/
/* 函数名	: AppGW_Init										 	 							*/
/* 输入  	: initPara:初始化网关参数															*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化网关						 										*/
/* 创建日期	: 2016/11/24																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGW_Init(AppGWInitPara* initPara)
{	
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 读探头数据 消息队列 数据 */
	static GWInitStruct gInit;
	/* 网关命令结构 */
	static GWCMDStruct gCMD;
	
	
	/* 设置命令 */
	gCMD.Type = GWCMD_Init;
	gCMD.Para = &gInit;
	
	/* 网关地址 */
	gInit.Adder 		= initPara->Inf_State.Adder;
	/* 信道(波段) */
	gInit.Channel 		= initPara->Inf_State.Channel;
	/* 数据丢失时间 */
	gInit.ActiveTime 	= initPara->Inf_State.ActiveTime;
	/* 工作模式（正常/调试） */
	if(gInit.Mode == 0)
	{
		/* 正常模式 */
		initPara->Inf_State.Mode = GW_Release;
	}else
	{
		/* 调试模式 */
		initPara->Inf_State.Mode = GW_Debug;
	}
	/* 通信波特率 */
	gInit.Baudrate 		= initPara->Inf_State.Baudrate;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPostFront (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* 函数名	: AppGW_Restart										 	 							*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 重启网关						 													*/
/* 创建日期	: 2016/12/01																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_Restart(void)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 读探头数据 消息队列 数据 */
	static GWRestartStruct gRestartFunc;
	/* 网关命令结构 */
	static GWCMDStruct gCMD;
	
	
	/* 设置命令 */
	gCMD.Type = GWCMD_Restart;
	gCMD.Para = &gRestartFunc;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPostFront (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	return GW_Success;
}

/************************************************************************************************/
/* 函数名	: AppGW_ConfigFunc										 	 						*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置网关功能						 										*/
/* 创建日期	: 2016/11/28																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGW_ConfigFunc(void)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 读探头数据 消息队列 数据 */
	static GWConfigFuncStruct gConfigFunc;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 设置命令 */
	gCMD.Type = GWCMD_ConfigFunc;
	gCMD.Para = &gConfigFunc;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPostFront (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* 函数名	: AppGW_ReadProbe										 	 						*/
/* 输入  	: no:探头编号[1-32]，type:探头类型[Temp/Hum]，data:探头数据							*/
/* 输出		: 无 																				*/
/* 作用	 	: 读南京盛亿网关探头数据						 										*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_ReadProbe(uint8_t no,GW_ProbeType type,float* data)
{	
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 删除消息邮箱指针 */
	OS_EVENT* gDelMbox;
	/* 消息邮箱指针 */
	void* gMbox_p;
	/* 申请内存状态信息 */
	INT8U gMboxerr;
	/* 读探头数据 消息队列 数据 */
	static GWReadProbeStruct gReadProbePara;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GW_Fail;
	}
	
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* 内存申请成功 */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
	}
#endif
	
	/* 设置命令 */
	gCMD.Type = GWCMD_ReadData;
	gCMD.Para = &gReadProbePara;
	/* 设置命令参数 */
	gReadProbePara.MboxHandle 	= gMbox_Handle;
	gReadProbePara.Data 		= data;
	gReadProbePara.Type 		= type;
	gReadProbePara.No 			= no;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGW_ReadLevel										 	 						*/
/* 输入  	: no:探头编号[1-32]，level:探头电量状态												*/
/* 输出		: 无 																				*/
/* 作用	 	: 读南京盛亿网关电量状态						 										*/
/* 创建日期	: 2016/11/21																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_ReadLevel(uint8_t no,GW_Level* level)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 删除消息邮箱指针 */
	OS_EVENT* gDelMbox;
	/* 消息邮箱指针 */
	void* gMbox_p;
	/* 申请内存状态信息 */
	INT8U gMboxerr;
	/* 读探头数据 消息队列 数据 */
	static GWReadLevelStruct gReadLevel;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GW_Fail;
	}
	
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* 内存申请成功 */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
	}
#endif
	
	/* 设置命令 */
	gCMD.Type = GWCMD_ReadLevel;
	gCMD.Para = &gReadLevel;
	/* 设置命令参数 */
	gReadLevel.MboxHandle 	= gMbox_Handle;
	gReadLevel.Level 		= level;
	gReadLevel.No 			= no;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGW_UpdataProbeData										 	 					*/
/* 输入  	: 无																				*/
/* 输出		: 返回操作结果 																		*/
/* 作用	 	: 更新网关探头数据						 															*/
/* 创建日期	: 2016/11/21																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_UpdataProbeData(void)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 删除消息邮箱指针 */
	OS_EVENT* gDelMbox;
	/* 消息邮箱指针 */
	void* gMbox_p;
	/* 申请内存状态信息 */
	INT8U gMboxerr;
	/* 读探头数据 消息队列 数据 */
	static GWUpdataProbeDataStruct gUpdataProbeData;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GW_Fail;
	}
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* 内存申请成功 */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
	}
#endif
	
	/* 设置命令 */
	gCMD.Type = GWCMD_UpdataProbeData;
	gCMD.Para = &gUpdataProbeData;
	/* 设置命令参数 */
	gUpdataProbeData.MboxHandle 	= gMbox_Handle;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GW_Result*)gMbox_p;

}

/************************************************************************************************/
/* 函数名	: AppGW_UpdataState										 	 						*/
/* 输入  	: 无							 										*/
/* 输出		: 无 																			*/
/* 作用	 	: 设置南京盛亿网关地址						 										*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGW_UpdataState(void)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 读探头数据 消息队列 数据 */
	static GWUpdataStateStruct gUpdataState;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return;
	}
	
	/* 设置命令 */
	gCMD.Type = GWCMD_UpdataState;
	gCMD.Para = &gUpdataState;
	/* 设置命令参数 */
	gUpdataState.MboxHandle 	= gMbox_Handle;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* 函数名	: AppGW_SetAdder										 	 						*/
/* 输入  	: adder:地址[00-255]							 										*/
/* 输出		: 设置结果 																			*/
/* 作用	 	: 设置南京盛亿网关地址						 										*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetAdder(uint8_t adder)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 删除消息邮箱指针 */
	OS_EVENT* gDelMbox;
	/* 消息邮箱指针 */
	void* gMbox_p;
	/* 申请内存状态信息 */
	INT8U gMboxerr;
	/* 读探头数据 消息队列 数据 */
	static GWSetAdderStruct gSetAdder;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GW_Fail;
	}
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* 内存申请成功 */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
	}
#endif
	
	/* 设置命令 */
	gCMD.Type = GWCMD_SetAdder;
	gCMD.Para = &gSetAdder;
	/* 设置命令参数 */
	gSetAdder.MboxHandle 	= gMbox_Handle;
	gSetAdder.Adder 		= adder;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGW_SetChannel										 	 						*/
/* 输入  	: channel:信道							 											*/
/* 输出		: 设置结果 																			*/
/* 作用	 	: 设置南京盛亿网关信道（波段）						 								*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetChannel(uint8_t channel)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 删除消息邮箱指针 */
	OS_EVENT* gDelMbox;
	/* 消息邮箱指针 */
	void* gMbox_p;
	/* 申请内存状态信息 */
	INT8U gMboxerr;
	/* 读探头数据 消息队列 数据 */
	static GWSetChannelStruct gSetChannel;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GW_Fail;
	}
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* 内存申请成功 */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
	}
#endif
	
	/* 设置命令 */
	gCMD.Type = GWCMD_SetChannel;
	gCMD.Para = &gSetChannel;
	/* 设置命令参数 */
	gSetChannel.MboxHandle 	= gMbox_Handle;
	gSetChannel.Channel 	= channel;

	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGW_SetActiveTime										 	 					*/
/* 输入  	: activeTime:有效时间							 									*/
/* 输出		: 设置结果 																			*/
/* 作用	 	: 设置南京盛亿网关数据有效时间						 								*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetActiveTime(uint8_t activeTime)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 删除消息邮箱指针 */
	OS_EVENT* gDelMbox;
	/* 消息邮箱指针 */
	void* gMbox_p;
	/* 申请内存状态信息 */
	INT8U gMboxerr;
	/* 读探头数据 消息队列 数据 */
	static GWSetActiveTimeStruct gSetActiveTime;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GW_Fail;
	}
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* 内存申请成功 */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
	}
#endif
	
	/* 设置命令 */
	gCMD.Type = GWCMD_SetActiveTime;
	gCMD.Para = &gSetActiveTime;
	/* 设置命令参数 */
	gSetActiveTime.MboxHandle 	= gMbox_Handle;
	gSetActiveTime.ActiveTime 	= activeTime;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGW_SetMode										 	 						*/
/* 输入  	: mode:工作模式							 											*/
/* 输出		: 设置结果 																			*/
/* 作用	 	: 设置南京盛亿网关工作模式						 									*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetMode(GW_Mode mode)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 删除消息邮箱指针 */
	OS_EVENT* gDelMbox;
	/* 消息邮箱指针 */
	void* gMbox_p;
	/* 申请内存状态信息 */
	INT8U gMboxerr;
	/* 读探头数据 消息队列 数据 */
	static GWSetModeStruct gSetMode;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GW_Fail;
	}
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* 内存申请成功 */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
	}
#endif
	
	/* 设置命令 */
	gCMD.Type = GWCMD_SetMode;
	gCMD.Para = &gSetMode;
	/* 设置命令参数 */
	gSetMode.MboxHandle 	= gMbox_Handle;
	gSetMode.Mode 			= mode;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGW_SetBaudrate										 	 							*/
/* 输入  	: baudrate:波特率							 													*/
/* 输出		: 设置结果																				*/
/* 作用	 	: 设置南京盛亿网关通信波特率						 									*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result AppGW_SetBaudrate(uint32_t baudrate)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 消息邮箱句柄 */
	OS_EVENT* gMbox_Handle;
	/* 删除消息邮箱指针 */
	OS_EVENT* gDelMbox;
	/* 消息邮箱指针 */
	void* gMbox_p;
	/* 申请内存状态信息 */
	INT8U gMboxerr;
	/* 读探头数据 消息队列 数据 */
	static GWSetBaudrateStruct gSetBaudrate;
	/* 网关命令结构 */ 
	static GWCMDStruct gCMD;
	
	
	/* 判断网关工作状态 */
	if(GW_Fail == AppGWRunPara->Inf_State.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GW_Fail;
	}
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	/* 内存申请成功 */
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
	}
#endif
	
	/* 设置命令 */
	gCMD.Type = GWCMD_SetBaudrate;
	gCMD.Para = &gSetBaudrate;
	/* 设置命令参数 */
	gSetBaudrate.MboxHandle 	= gMbox_Handle;
	gSetBaudrate.Baudrate 		= baudrate;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (GW_NJSY_Q,(void*)&gCMD);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GW_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: GW_NJSY_Handle										 	 						*/
/* 输入  	: 无							 													*/
/* 输出		: 无																				*/
/* 作用	 	: 南京盛亿网关处理命令函数						 									*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void GW_NJSY_Handle(GWCMDStruct* cmd)
{	
	/* 操作状态寄存器 */
	const static GW_Result gSuccess = GW_Success;
	const static GW_Result gFail 	= GW_Fail;
				 GW_Result gState;
	/* 消息邮箱状态信息 */
	INT8U gMboxerr;
	/* 邮箱句柄指针 */
	OS_EVENT* gMboxp;
	
	
	/* 判断命令类型 */
	switch(cmd->Type)
	{
		/* 初始化网关 */
 		case GWCMD_Init:
 		{
			/* 初始化网关数据指针 */
			GWInitStruct* gInitp = cmd->Para;
			/* 初始化网关参数 */
			LogGatewayInitParaStruct logGWInitPara;
			
			
			/* 设置网关初始化参数 */
			logGWInitPara.Adder 		= gInitp->Adder;
			logGWInitPara.Channel 		= gInitp->Channel;
			logGWInitPara.ActiveTime 	= gInitp->ActiveTime;
			logGWInitPara.Mode 			= gInitp->Mode;
			logGWInitPara.Baudrate 		= gInitp->Baudrate;
			
			/* 初始化网关 */
			LogGW_API.Init(logGWInitPara);
			
			return;
 		}
		
		/* 重启网关功能 */
		case GWCMD_Restart:
		{
//			/* 初始化网关数据指针 */
//			GWRestartStruct* gRestartp = cmd->Para;
//			
//			
//			/* 设置消息邮箱句柄 */
//			gMboxp = gRestartp->MboxHandle;
			/* 设置当前网关配置状态 */
			gState = LogGW_API.Restart();
			/* 等待系统运行稳定 */
			OSTimeDly(AppGW_RestartTimeValue+1000);
			return;
		}
		
		/* 配置网关功能 */
		case GWCMD_ConfigFunc:
		{
			/* 初始化网关数据指针 */
			GWConfigFuncStruct* gConfigFuncp = cmd->Para;
			
			
			/* 设置消息邮箱句柄 */
			gMboxp = gConfigFuncp->MboxHandle;
			/* 设置当前网关配置状态 */
			gState = LogGW_API.ConfigFunc();
			/* 设置操作状态 */
			AppGWRunPara->Inf_State.WorkState = gState;
			/* 判断配置网关功能状态 */
			if(AppGWRunPara->Inf_State.WorkState == GW_Success)
			{
				/* 创建软件定时器结果寄存器 */
				INT8U tmrErr;
				/* 关闭软件定时器状态 */
				BOOLEAN gTmrStopState;
				
				
				/* 停止定时初始化网关定时器 */
				gTmrStopState = OSTmrStop (AppGW_ReConfigFunc_ptmr,
										   OS_TMR_OPT_NONE,
										   (OS_TMR_CALLBACK)AppGW_ReConfigFunc_CallBack,
										   &tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
				if((OS_ERR_NONE != tmrErr)||(OS_FALSE == gTmrStopState))
				{
					/* 停止定时初始化网关定时器:失败 */
					__nop();
				}
#endif
				(void) gTmrStopState;
			}
			break;
		}
		
		/* 读数据 */
 		case GWCMD_ReadData:
 		{
			/* 初始化网关数据指针 */
			GWReadProbeStruct* gReadProbep = cmd->Para;

			
			/* 设置消息邮箱句柄 */
			gMboxp = gReadProbep->MboxHandle;
			/* 读探头数据 */
			gState = LogGW_API.ReadProbe(gReadProbep->No,
											 gReadProbep->Type,
											 gReadProbep->Data);
			break;
 		}
		
		/* 读电量 */
 		case GWCMD_ReadLevel:
 		{
			/* 初始化网关数据指针 */
			GWReadLevelStruct* gReadLevelp = cmd->Para;

			
			/* 设置消息邮箱句柄 */
			gMboxp = gReadLevelp->MboxHandle;
			/* 读探头电量 */
			gState = LogGW_API.ReadLevel(gReadLevelp->No,gReadLevelp->Level);
			break;
 		}
		
		/* 更新网关数据 */
 		case GWCMD_UpdataProbeData:
 		{
			/* 初始化网关数据指针 */
			GWUpdataProbeDataStruct* gUpdataProbeDatap = cmd->Para;

			
			/* 设置消息邮箱句柄 */
			gMboxp = gUpdataProbeDatap->MboxHandle;
			/* 更新网关数据 */
			gState = LogGW_API.UpdataProbeData();
			break;
 		}
		/* 更新网关状态 */
		case GWCMD_UpdataState:
		{
			/* 初始化网关数据指针 */
			GWUpdataStateStruct* gUpdataStatep = cmd->Para;
			
			
			/* 设置消息邮箱句柄 */
			gMboxp = gUpdataStatep->MboxHandle;
			/* 更新网关数据 */
			gState = LogGW_API.UpdataState();
			/* 判断网关工作状态 */
			if(GW_Fail == gState)
			{
				/* 创建软件定时器结果寄存器 */
				INT8U tmrErr;
				/* 创建启动软件定时器状态 */
				BOOLEAN tmrState;
				
				
				/* 网关工作异常，设置标志 */
				AppGWRunPara->Inf_State.WorkState = GW_Fail;
				
				/* 启动初始化程序 */
				tmrState = OSTmrStart (AppGW_ReConfigFunc_ptmr,&tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
				if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
				{__nop();}
#endif
				(void)tmrErr;(void)tmrState;
			}
			
			return;
		}
		
		/* 设置网关地址 */
 		case GWCMD_SetAdder:
 		{
			/* 初始化网关数据指针 */
			GWSetAdderStruct* gSetAdderp = cmd->Para;

			
			/* 设置消息邮箱句柄 */
			gMboxp = gSetAdderp->MboxHandle;
			/* 更新网关数据 */
			gState = LogGW_API.SetAdder(gSetAdderp->Adder);
			break;
 		}
		
		/* 设置网关信道(波段) */
 		case GWCMD_SetChannel:
 		{
			/* 初始化网关数据指针 */
			GWSetChannelStruct* gSetChannelp = cmd->Para;

			
			/* 设置消息邮箱句柄 */
			gMboxp = gSetChannelp->MboxHandle;
			/* 更新网关数据 */
			gState = LogGW_API.SetChannel(gSetChannelp->Channel);
			break;
 		}
		
		/* 设置网关数据有效时间 */
 		case GWCMD_SetActiveTime:
 		{
			/* 初始化网关数据指针 */
			GWSetActiveTimeStruct* gSetActiveTimep = cmd->Para;

			
			/* 设置消息邮箱句柄 */
			gMboxp = gSetActiveTimep->MboxHandle;
			/* 更新网关数据 */
			gState = LogGW_API.SetActiveTime(gSetActiveTimep->ActiveTime);
			break;
 		}
		
		/* 设置网关工作模式 */
 		case GWCMD_SetMode:
 		{
			/* 初始化网关数据指针 */
			GWSetModeStruct* gSetModep = cmd->Para;
			
			
			/* 设置消息邮箱句柄 */
			gMboxp = gSetModep->MboxHandle;
			/* 更新网关数据 */
			gState = LogGW_API.SetMode(gSetModep->Mode);
			break;
 		}
		
		/* 设置网关工作模式 */
 		case GWCMD_SetBaudrate:
 		{	
			/* 初始化网关数据指针 */
			GWSetBaudrateStruct* gSetBaudratep = cmd->Para;
			
			
			/* 设置消息邮箱句柄 */
			gMboxp = gSetBaudratep->MboxHandle;
			/* 更新网关数据 */
			gState = LogGW_API.SetBaudrate(gSetBaudratep->Baudrate);
			break;
 		}
		
		
		default:
		{
			/* 未设定功能 不作处理 */
			__nop();
			return;
		}
	}
	
	/* 判断命令执行状态 */
	if(gState == GW_Success)
	{
		/* 命令执行成功，发送消息邮箱。 */
		gMboxerr = OSMboxPost(gMboxp,((void*)&gSuccess));
	}else
	{
		/* 命令执行失败 */
		gMboxerr = OSMboxPost(gMboxp,((void*)&gFail));
	}
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gMboxerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* 函数名	: AppGW_Init										 	 							*/
/* 输入  	: 无							 													*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化应用层网关相关信息						 									*/
/* 创建日期	: 2016/11/18																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGW_EnvironInit(void)
{
	/* 创建软件定时器结果寄存器 */
	INT8U tmrErr;
	/* 创建启动软件定时器状态 */
	BOOLEAN tmrState;
	
	
	/* 初始化网关工作状态 */
	AppGWRunPara->Inf_State.WorkState = GW_Fail;
	
	/* 先初始化 系统先关的参数 */
	/* 创建 南京盛亿网关消息队列 */
	GW_NJSY_Q = OSQCreate(GW_NJSY_Grp,GW_NJSY_Q_Len);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	if(GW_NJSY_Q == (OS_EVENT *)0)
	{
		/* 创建队列失败 */
		__nop();
	}
#endif
	
	/* 创建定时更新网关探头数据软件定时器 */
	AppGW_UpdataProbeData_ptmr = 
	OSTmrCreate (
					300,							//软件定时器启动后延时多少时间调用回调函数
					500,							//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_PERIODIC,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)AppGW_UpdataProbeData_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"读网关探头数据定时器",			//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);					

#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
#endif				
	
	/* 创建定时读取网关工作状态软件定时器 */
	AppGW_WorkState_ptmr = 
	OSTmrCreate (
					1000,							//软件定时器启动后延时多少时间调用回调函数
					1000,							//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_PERIODIC,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)AppGW_UpdataState_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"读网关工作状态定时器",			//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);					
				
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
#endif
	
	/* 创建配置网关功能软件定时器 */
	AppGW_ReConfigFunc_ptmr = 
	OSTmrCreate (
					10,								//软件定时器启动后延时多少时间调用回调函数
					1000,							//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_PERIODIC,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)AppGW_ReConfigFunc_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"配置网关功能定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);		
	
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
#endif				
	
	/* 启动定时更新网关探头数据定时器（延时5S调用 网关重启回调函数） */
	tmrState = OSTmrStart (AppGW_UpdataProbeData_ptmr,&tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* 启动定时读取网关工作状态定时器（延时60S调用 网关重启回调函数） */
	tmrState = OSTmrStart (AppGW_WorkState_ptmr,&tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* 启动定时初始化网关定时器（延时60S调用 网关重启回调函数） */
	tmrState = OSTmrStart (AppGW_ReConfigFunc_ptmr,&tmrErr);
#if  ((AppGatewayDebugEnable == 0x01)&&(AppGatewayDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* 设置接口函数 */
	/* 初始化接口 */
	AppGW_API.Init 			= AppGW_Init;
	/* 重启网关接口 */
	AppGW_API.Restart		= AppGW_Restart;
	/* 配置网关功能 */
	AppGW_API.ConfigFunc 	= AppGW_ConfigFunc;
	/* 读探头数据接口 */
	AppGW_API.ReadProbe 	= AppGW_ReadProbe;
	/* 读探头电量状态 */
	AppGW_API.ReadLevel		= AppGW_ReadLevel;
	/* 更新网关探头数据 */
	AppGW_API.UpdataProbeData	= AppGW_UpdataProbeData;
	/* 更新网关状态 */
	AppGW_API.UpdataState	= AppGW_UpdataState;
	/* 设置网关接口 */
	AppGW_API.SetAdder	 	= AppGW_SetAdder;
	/* 设置网关信道(波段) */
	AppGW_API.SetChannel 	= AppGW_SetChannel;
	/* 设置网关数据有效时间 */
	AppGW_API.SetActiveTime = AppGW_SetActiveTime;
	/* 设置网关数据有效时间 */
	AppGW_API.SetMode 		= AppGW_SetMode;
	/* 设置网关数据有效时间 */
	AppGW_API.SetBaudrate 	= AppGW_SetBaudrate;
}

#endif
/***********************************************************************************/


/************************************************************************************************/
/* 函数名	: AppTaskGatewayNJSY																*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 南京盛亿网关处理任务																*/
/* 创建日期 	: 2016/11/18																		*/
/************************************************************************************************/
void AppTaskGatewayNJSY(void *p_arg)
{
	/* 申请队列状态信息 */
	INT8U gQerr;
	/* 网关命令结构 */
	GWCMDStruct* gCMDDatap;

	(void)p_arg;
	
	
	/* 初始化网关任务参数 */
	AppGW_EnvironInit();
	
	/* 初始化逻辑层网关API函数 */
	BSP_LogGW_API_Init();
	
	/* 启动网关，初始化配置 */
	AppGW_API.Init(AppGWRunPara);

	while (1)
	{
		/* 等待 队列 消息 */
		gCMDDatap = (GWCMDStruct*)OSQPend(GW_NJSY_Q,0,&gQerr);
		
		/* 接收消息 状态 判断 */
		if(gQerr == OS_ERR_NONE)
		{
			/* 处理网关操作命令 */
			GW_NJSY_Handle(gCMDDatap);
		}
	}
}



#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
