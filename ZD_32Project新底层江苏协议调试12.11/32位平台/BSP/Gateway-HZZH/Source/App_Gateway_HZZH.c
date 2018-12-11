/************************************************************************************************/
/** @file              : App_Gateway_HZZH.c														*/
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
#include "App_Gateway_HZZH.h"
#if (Gateway_HZZH_Enable == 0x01)
#include "Log_Sensor.h"
/********************************* 宏定义 *********************************/
/* 杭州创辉网关重新启动网关的时间 */
#define AppGW_RestartTimeValue		(20u)

/* 杭州创辉网关操作指令超时时间 */
#define AppGW_CmdOvertimeValue		(25u)

/* 杭州创辉网关接收数据计时时间频率 */
#define AppGWHZZH_TimeOutValue		(100u)

/* 杭州创辉网关看门狗时间 */
#define AppGWHZZH_WatchdogTimeValue	(30000ul)

/* 杭州创辉网关正常状态查询时间 */
#define AppGWHZZH_NStateTimeValue	(60000ul)

/* 杭州创辉网关异常状态查询时间 */
#define AppGWHZZH_AStateTimeValue	(1000ul)

/***********************************************************************************/

/* 杭州创辉网关功能码类型 */
typedef enum 
{
	/* 初始化网关 */
	AppGWHZZHCmd_Init 					= (0u),
	/* 重启网关功能 */
	AppGWHZZHCmd_Restart				= (1u),
	/* 更新网关状态 */
	AppGWHZZHCmd_UpdateState			= (2u),	
	/* 读探头响应时间和探头数量数据 */
	AppGWHZZHCmd_ReadWkupTimeProbeNum 	= (3u),
	/* 设置探头响应时间 */
	AppGWHZZHCmd_SetWkupTime 			= (4u),
	/* 设置探头数量 */
	AppGWHZZHCmd_SetProbeNum 			= (5u),

}AppGWHZZHCmdType;

/* 杭州创辉网关功能码类型 */
typedef struct
{
	/* 命令类型 */
	AppGWHZZHCmdType Type;
	/* 命令参数 */
    void* Para;
}AppGWHZZHCmdStruct;

/* 初始化网关功能 消息邮箱 数据结构 */
typedef struct
{	
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
}AppGWHZZHInitMboxStruct;

/* 重启网关功能 消息邮箱 数据结构 */
typedef struct
{	
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
}AppGWHZZHRestartMboxStruct;

/* 读网关工作状态功能 消息邮箱 数据结构 */
typedef struct
{	
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
}AppGWHZZHStateMboxStruct;

/* 读网关响应时间和探头数量功能 消息邮箱 数据结构 */
typedef struct
{	
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
}AppGWHZZHRWTPNMboxStruct;

/* 设置网关响应时间 消息邮箱 数据结构 */
typedef struct
{	
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 响应时间 */
	uint8_t WkupTime;
	/* 探头数量 */
	uint8_t ProbeNum;
}AppGWHZZHSWTMboxStruct;

/* 设置网关探头数量功能 消息邮箱 数据结构 */
typedef struct
{	
	/* 消息邮箱指针 */
	OS_EVENT* MboxHandle;
	/* 响应时间 */
	uint8_t WkupTime;
	/* 探头数量 */
	uint8_t ProbeNum;
}AppGWHZZHSPNMboxStruct;
/* 杭州创辉网应用层运行数据结构 */
typedef struct
{
	

	/* 状态寄存器 */
	struct
	{
		/* 网关工作状态 */
		GWHZZH_Result WorkState;

	}SR;
	
	/* 数据寄存器 */
	struct
	{
		/* 探头类型数据指针 */
		uint8_t* ProbeTypep;
		/* 探头ID数据指针 */
		uint8_t* ProbeIDp;
		/* 探头数据指针 */
		float* ProbeDatap;
		/* 探头电量数据指针 */
		uint8_t* ProbeLevelp;
		
		/* 探头响应时间 */
		uint8_t WkupTime;
		/* 探头数量 */
		uint8_t ProbeNum;
		
		/* 接收数据时间 */
		uint32_t ReceTimeOut[32];
	}DR;

}AppGWHZZHRunParaStruct;

/***********************************************************************************/




/********************************* 杭州创辉网关 应用参数 *********************************/
/********************************* 创建任务参数 *********************************/
/* 任务堆栈 */
OS_STK AppTaskGatewayHZZHStk[AppTask_GatewayHZZH_StkSize];


/********************************* 消息队列 *********************************/
/* 消息队列 */
static  OS_EVENT* AppGWHZZH_Q;
/* 消息队列长度 */
#define AppGWHZZH_Q_Len		((u8)OS_MAX_QS)
/* 控制块数组 */
static void* AppGWHZZH_Grp[AppGWHZZH_Q_Len];

/********************************* 运行参数 *********************************/


/********************************* 杭州创辉网应用层运行数据定义 **************/
AppGWHZZHRunParaStruct	AppGWHZZHRunPara =
{	
	
	
	/* 网关工作状态 */
	GWHZZH_Fail,
	
	
	/* 数据寄存器 */
	/* 探头类型数据指针 */
	(uint8_t*)&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1.Type,
	/* 探头ID数据指针 */
	(uint8_t*)&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,
	/* 探头数据指针 */
	&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data,
	/* 探头电量数据指针 */
	&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State,
	
	/* 探头响应时间 */
	1u,
	/* 探头数量 */
	50u,
	
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	Gateway_HZZH_ReceTimeout,Gateway_HZZH_ReceTimeout,
	
};
	
/********************************* 应用接口定义 *****************************/
AppGatewayHZZHAPIStruct AppGWHZZH_API;

/***********************************************************************************/




#if (1)//网关看门狗软件定时器回调函数
/* 网关看门狗软件定时器 句柄指针 */
static OS_TMR* AppGWHZZH_Watchdog_ptmr;

/************************************************************************************************/
/* 函数名	: AppGWHZZH_Watchdog_CallBack										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 网关看门狗软件定时器回调函数						 								*/
/* 创建日期	: 2016/12/19																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_Watchdog_CallBack(OS_TMR *ptmr,void *p_arg)
{	
	/* 关闭软件定时器状态 */
	BOOLEAN tmrState;
	/* 创建软件定时器结果寄存器 */
	INT8U tmrErr;
	
	
	/* 重启网关 */
	AppGWHZZH_API.Restart();
	
	/* 启动网关看门狗软件定时器(延时5分钟后调用) */
	tmrState = OSTmrStart (AppGWHZZH_Watchdog_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_Restart_CallBack										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 网关重启软件定时器回调函数						 									*/
/* 创建日期	: 2016/12/19																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_FeedWatchdog(void)
{
	/* 关闭软件定时器状态 */
	BOOLEAN tmrState;
	/* 创建软件定时器结果寄存器 */
	INT8U tmrErr;
	
	
	/* 停止定时初始化网关定时器 */
	tmrState = OSTmrStop (AppGWHZZH_Watchdog_ptmr,
						  OS_TMR_OPT_NONE,
						  (OS_TMR_CALLBACK)AppGWHZZH_Watchdog_CallBack,
						  &tmrErr);
					
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((OS_ERR_NONE != tmrErr)||(OS_FALSE == tmrState))
	{
		/* 停止定时初始化网关定时器:失败 */
		__nop();
	}
#endif
	
	/* 删除软件定时器 */
	tmrState = OSTmrDel (AppGWHZZH_Watchdog_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* 创建 网关看门狗软件定时器 */
	AppGWHZZH_Watchdog_ptmr = 
	OSTmrCreate (
					AppGWHZZH_WatchdogTimeValue,	//软件定时器启动后延时多少时间调用回调函数
					AppGWHZZH_WatchdogTimeValue,	//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_ONE_SHOT,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)AppGWHZZH_Watchdog_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"网关看门狗定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);
	
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}	
	
	
	/* 启动定时初始化网关定时器 */
	/* 启动网关看门狗软件定时器(延时5分钟后调用) */
	tmrState = OSTmrStart (AppGWHZZH_Watchdog_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
}

#endif

#if (1)//网关状态软件定时器回调函数
/* 网关状态软件定时器 句柄指针 */
static OS_TMR* AppGWHZZH_State_ptmr;

/************************************************************************************************/
/* 函数名	: AppGWHZZH_State_CallBack										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 网关状态软件定时器回调函数						 								*/
/* 创建日期	: 2016/12/19																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_State_CallBack(OS_TMR *ptmr,void *p_arg)
{	
	
	/* 读网关当前状态 */
	AppGWHZZH_API.UpdateState();
}

#endif

#if (1)//接收数据计时软件定时器回调函数
/* 接收数据计时软件定时器 句柄指针 */
static OS_TMR* AppGWHZZH_TimeOut_ptmr;
/************************************************************************************************/
/* 函数名	: AppGWHZZH_TimeOut_CallBack										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 接收数据计时软件定时器回调函数						 									*/
/* 创建日期	: 2016/12/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_TimeOut_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* 计数器 */
	uint8_t num;
	
	/* 计时时间寄存器 */
	for(num=0;num<32;num++)
	{
		/* 判断当前的通道类型 */
		if((Sensor_GW_HZZH_Tmp != *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL)))&&
		   (Sensor_GW_HZZH_Hum != *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL))))
		{
			/* 非杭州创辉网关类型 */
			continue;
		}
		/* 判断时间寄存器数据是否超出 */
		if(Gateway_HZZH_ReceTimeout > AppGWHZZHRunPara.DR.ReceTimeOut[num])
		{
			/* 接收数据未超时，设置时间寄存器。 */
			AppGWHZZHRunPara.DR.ReceTimeOut[num]++;
		}else
		{
			/* 接收数据超时，设置数据寄存器值为超时值。 */
			*((float*)(((uint8_t*)AppGWHZZHRunPara.DR.ProbeDatap)+num*sizeof(INSTRU_SENSOR_CHANNEL_FIX))) = 
			Gateway_HZZH_TimeoutPara;
		}
	}
}

#endif

#if (1)//杭州创辉网关应用函数
/************************************************************************************************/
/* 函数名	: AppGW_GetProbeDataTemp										 	 				*/
/* 输入  	: data:探头数据							 											*/
/* 输出		: 探头温度数据																		*/
/* 作用	 	: 将探头数据中的温度数据解析出来						 								*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static float AppGW_GetProbeDataTemp(AppGatewayHZZHProbeDataStruct* data)
{

	/* 将数据中的探头温度数据解析出来 */
	return ((data->TempPM ? -1 : 1)*((float)data->TempInteger+((float)data->TempDecimal)*0.1));
}

/************************************************************************************************/
/* 函数名	: AppGW_GetProbeDataHum										 	 					*/
/* 输入  	: data:探头数据							 											*/
/* 输出		: 探头湿度数据																		*/
/* 作用	 	: 将探头数据中的湿度数据解析出来						 								*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static float AppGW_GetProbeDataHum(AppGatewayHZZHProbeDataStruct* data)
{
	/* 将数据中的探头温度数据解析出来 */
	return ((float)data->HumiInteger+((float)data->HumiDecimal)*0.1);
}

/************************************************************************************************/
/* 函数名	: AppGW_GetProbeLevel										 	 					*/
/* 输入  	: data:探头数据							 											*/
/* 输出		: 探头电量数据																		*/
/* 作用	 	: 将探头数据中的电量数据解析出来						 								*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static uint8_t AppGW_GetProbeLevel(AppGatewayHZZHProbeDataStruct* data)
{
	/* 卡片电池电压值 */
	float gVoltage;
	/* 电量百分比 */
	uint8_t gLevel;
	
	
	/* 将数据中的探头温度数据解析出来 */
	gVoltage =  ((float)data->PowerInteger+((float)data->PowerDecimal)*0.1);
	
	/* 计算电量百分比 */
	if((3.0) < gVoltage)
	{
		/* 电量为100% */
		gLevel = 100;
	}else if(((3.0) >= gVoltage)&&((2.6) < gVoltage))
	{
		/* 计算电量百分比 */
		gLevel = ((gVoltage-2.6)*100*5)/2;
	}else if((2.6) >= gVoltage)
	{
		/* 计算电量百分比 */
		gLevel = 0;
	}
	
	return gLevel;
}

#endif

#if (1)//杭州创辉网关响应功能函数
/************************************************************************************************/
/* 函数名	: AppGWHZZH_ProbeDataACKHandle										 	 			*/
/* 输入  	: id:探头ID，data:探头数据							 								*/
/* 输出		: 无																				*/
/* 作用	 	: 处理探头数据响应。						 											*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_ProbeDataACKHandle(uint8_t* id,uint8_t* data)
{	
	/* 计数器 */
	uint8_t num;
	
	
	/* 查找配置的探头ID号 */
	for(num = 0;num < Sensor_CH_Config_Num;num++)
	{
		/* 判断当前通道类型 */
		if((Sensor_GW_HZZH_Tmp != *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL)))&&
		   (Sensor_GW_HZZH_Hum != *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL))))
		{
			/* 探头类型不是创辉探头，继续查找下一个探头。 */
			continue;
		}
		
		/* 判断当前通道的ID号是否与接收到探头ID是否一致。 */
		if(((AppGWHZZHRunPara.DR.ProbeIDp+num*sizeof(INSTRU_SENSOR_CHANNEL))[0] != id[3])||
		   ((AppGWHZZHRunPara.DR.ProbeIDp+num*sizeof(INSTRU_SENSOR_CHANNEL))[1] != id[2])||
		   ((AppGWHZZHRunPara.DR.ProbeIDp+num*sizeof(INSTRU_SENSOR_CHANNEL))[2] != id[1])||
		   ((AppGWHZZHRunPara.DR.ProbeIDp+num*sizeof(INSTRU_SENSOR_CHANNEL))[3] != id[0]))
		{
			/* 当前探头ID号与接收的探头ID不相同，继续查找下一个探头ID。 */
			continue;
		}
		
		/* 当前探头ID号与接收的探头ID相同，设置相应通道的数据和电量。 */
		/* 判断当前通道类型(温度/湿度) */
		if(Sensor_GW_HZZH_Tmp == *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL)))
		{
			/* 温度探头，将数据当中的温度数据写入应用寄存器 */
			*((float*)(((uint8_t*)AppGWHZZHRunPara.DR.ProbeDatap)+num*sizeof(INSTRU_SENSOR_CHANNEL_FIX))) = 
			AppGW_GetProbeDataTemp((AppGatewayHZZHProbeDataStruct*)data);
		}else if(Sensor_GW_HZZH_Hum == *(AppGWHZZHRunPara.DR.ProbeTypep+num*sizeof(INSTRU_SENSOR_CHANNEL)))
		{
			/* 湿度探头，将数据当中的温度数据写入应用寄存器 */
			*((float*)(((uint8_t*)AppGWHZZHRunPara.DR.ProbeDatap)+num*sizeof(INSTRU_SENSOR_CHANNEL_FIX))) = 
			AppGW_GetProbeDataHum((AppGatewayHZZHProbeDataStruct*)data);
		}
		/* 将数据当中的电量数据写入应用寄存器 */
		*(AppGWHZZHRunPara.DR.ProbeLevelp+num*sizeof(INSTRU_SENSOR_CHANNEL_FIX)) = 
		AppGW_GetProbeLevel((AppGatewayHZZHProbeDataStruct*)data);
		
		/* 清除当前探头接收数据超时时间 */
		AppGWHZZHRunPara.DR.ReceTimeOut[num] = 0;
	}
	
	/* 成功接收到一帧数据，清除看门狗定时器值(喂狗) */
	AppGWHZZH_FeedWatchdog();
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_SetWkupTimeProbeNumACKHandle											*/
/* 输入  	: 无														 						*/
/* 输出		: 无																				*/
/* 作用	 	: 处理设置探头响应时间和探头数量的响应。						 						*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_SetWkupTimeProbeNumACKHandle(void)
{	
	/* 目前没有对应设置 */
	__nop();
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_ReadWkupTimeProbeNumACKHandle											*/
/* 输入  	: wkupTime:响应时间，data:探头数量							 						*/
/* 输出		: 无																				*/
/* 作用	 	: 处理读探头响应时间和探头数量的响应。						 							*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_ReadWkupTimeProbeNumACKHandle(uint8_t wkupTime,uint8_t probeNum)
{	
	
	/* 设置探头响应时间和探头数量 */
	AppGWHZZHRunPara.DR.WkupTime = wkupTime;
	/* 设置探头数量 */
	AppGWHZZHRunPara.DR.ProbeNum = probeNum;
}

#endif

#if (1)//杭州创辉网关接口函数
/************************************************************************************************/
/* 函数名	: AppGWHZZH_Restart										 	 						*/
/* 输入  	: 无							 													*/
/* 输出		: 无																				*/
/* 作用	 	: 重启网关									 										*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void	AppGWHZZH_Restart(void)
{
	/* 发送消息队列状态信息 */
	INT8U gQerr;
	/* 读探头数据 消息邮箱 数据 */
	static AppGWHZZHRestartMboxStruct gRestartMbox;
	/* 网关命令结构 */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* 设置命令 */
	gGWHZZHCmd.Type = AppGWHZZHCmd_Restart;
	/* 设置参数指针 */
	gGWHZZHCmd.Para = &gRestartMbox;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPostFront (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();

	}
#endif
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_UpdateState										 	 						*/
/* 输入  	: 无							 													*/
/* 输出		: 无																				*/
/* 作用	 	: 读网关当前工作状态									 								*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_UpdateState(void)
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
	/* 读探头数据 消息邮箱 数据 */
	static AppGWHZZHStateMboxStruct gStateMbox;
	/* 网关命令结构 */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	
	/* 内存申请成功 */
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 设置命令 */
	gGWHZZHCmd.Type = AppGWHZZHCmd_UpdateState;
	/* 设置参数指针 */
	gGWHZZHCmd.Para = &gStateMbox;
	/* 设置命令参数 */
	gStateMbox.MboxHandle = gMbox_Handle;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GWHZZH_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_GetState										 	 					*/
/* 输入  	: 无							 													*/
/* 输出		: 当前杭州创辉网关状态																*/
/* 作用	 	: 读杭州创辉网关状态																	*/
/* 创建日期	: 2017/01/11																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_GetState(void)
{

	/* 返回网关状态 */
	return AppGWHZZHRunPara.SR.WorkState;
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_ReadWkupTimeProbeNum										 	 		*/
/* 输入  	: 无							 													*/
/* 输出		: 无																				*/
/* 作用	 	: 读杭州创辉网关探头响应时间和探头数量													*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_ReadWkupTimeProbeNum(uint16_t* time,uint16_t* probeNum)
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
	/* 读探头数据 消息邮箱 数据 */
	static AppGWHZZHRWTPNMboxStruct gRWTPNMbox;
	/* 网关命令结构 */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* 判断网关工作状态 */
	if(GWHZZH_Fail == AppGWHZZHRunPara.SR.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
	
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	
	/* 内存申请成功 */
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 设置命令 */
	gGWHZZHCmd.Type = AppGWHZZHCmd_ReadWkupTimeProbeNum;
	/* 设置参数指针 */
	gGWHZZHCmd.Para = &gRWTPNMbox;
	/* 设置命令参数 */
	gRWTPNMbox.MboxHandle 	= gMbox_Handle;
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 判断获取信息状态	*/
	if (GWHZZH_Success == (*(GWHZZH_Result*)gMbox_p))
	{
		/* 设置唤醒时间和标签数量 */
		*time = AppGWHZZHRunPara.DR.WkupTime*15;
		*probeNum = AppGWHZZHRunPara.DR.ProbeNum*10;
	}else
	{
		/* 设置唤醒时间和标签数量为0 */
		*time = 0;
		*probeNum = 0; 
	}

	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GWHZZH_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_SetWkupTime										 	 					*/
/* 输入  	: time:							 													*/
/* 输出		: 无																				*/
/* 作用	 	: 设置杭州创辉网关探头响应时间														*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_SetWkupTime(uint16_t time,uint16_t probeNum)
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
	/* 设置探头响应时间数据 消息邮箱 数据 */
	static AppGWHZZHSWTMboxStruct gSWTMbox;
	/* 网关命令结构 */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* 判断网关工作状态 */
	if(GWHZZH_Fail == AppGWHZZHRunPara.SR.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
	
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	
	/* 内存申请成功 */
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 设置命令 */
	gGWHZZHCmd.Type = AppGWHZZHCmd_SetWkupTime;
	/* 设置参数指针 */
	gGWHZZHCmd.Para = &gSWTMbox;
	
	/* 设置命令参数 */
	gSWTMbox.MboxHandle = gMbox_Handle;
	/* 设置响应时间参数 */
	gSWTMbox.WkupTime = (time/15);
	/* 设置探头数量参数 */
	gSWTMbox.ProbeNum = (probeNum/10);
	
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GWHZZH_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_SetProbeNum										 	 					*/
/* 输入  	: time:							 													*/
/* 输出		: 无																				*/
/* 作用	 	: 设置杭州创辉网关探头数量															*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result AppGWHZZH_SetProbeNum(uint16_t time,uint16_t probeNum)
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
	/* 设置探头数量数据 消息邮箱 数据 */
	static AppGWHZZHSPNMboxStruct gSPNMbox;
	/* 网关命令结构 */
	static AppGWHZZHCmdStruct gGWHZZHCmd;
	
	
	/* 判断网关工作状态 */
	if(GWHZZH_Fail == AppGWHZZHRunPara.SR.WorkState)
	{
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
	
	/* 创建一个消息邮箱 */
	gMbox_Handle = OSMboxCreate ((void*)0);
	
	/* 内存申请成功 */
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断创建消息邮箱状态 */
	if(gMbox_Handle == (OS_EVENT *)0)
	{
		/* 创建消息邮箱失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 设置命令 */
	gGWHZZHCmd.Type = AppGWHZZHCmd_SetWkupTime;
	/* 设置参数指针 */
	gGWHZZHCmd.Para = &gSPNMbox;
	
	/* 设置命令参数 */
	gSPNMbox.MboxHandle = gMbox_Handle;
	/* 设置响应时间参数 */
	gSPNMbox.WkupTime = time/15;
	/* 设置探头数量参数 */
	gSPNMbox.ProbeNum = probeNum/10;
	
	
	/* 发送需要操作的网关命令 */
	gQerr = OSQPost (AppGWHZZH_Q,(void*)&gGWHZZHCmd);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gQerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	/* 等待操作完成返回的消息。 */
	gMbox_p = OSMboxPend (gMbox_Handle, AppGW_CmdOvertimeValue, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gMbox_p == ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 删除消息邮箱 */
	gDelMbox = OSMboxDel (gMbox_Handle, OS_DEL_ALWAYS, &gMboxerr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if((gMboxerr != OS_ERR_NONE)||(gDelMbox != ((void *)0)))
	{
		/* 发送命令失败 */
		__nop();
		/* 返回命令执行失败结果 */
		return GWHZZH_Fail;
	}
#endif
	
	/* 根据返回的消息，设置返回命令操作结果。 */
	return *(GWHZZH_Result*)gMbox_p;
}

/************************************************************************************************/
/* 函数名	: AppGW_Init										 	 							*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化杭州创辉网关功能					 											*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_Init(void)
{
	/* 创建软件定时器结果寄存器 */
	INT8U tmrErr;
	/* 创建启动软件定时器状态 */
	BOOLEAN tmrState;
	
	
	/* 初始化杭州创辉网关逻辑层接口 */
	LogGW_InitAPI(AppGWHZZH_ProbeDataACKHandle,
				  AppGWHZZH_SetWkupTimeProbeNumACKHandle,
				  AppGWHZZH_ReadWkupTimeProbeNumACKHandle);

	/* 启动网关看门狗软件定时器(延时5分钟后调用) */
	tmrState = OSTmrStart (AppGWHZZH_Watchdog_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* 启动接收数据计时软件定时器 */
	tmrState = OSTmrStart (AppGWHZZH_TimeOut_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	/* 启动网关状态软件定时器 */
	tmrState = OSTmrStart (AppGWHZZH_State_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{__nop();}
#endif
	
	
	
	LogGWHZZHAPI.Init();
	
}


#endif

/************************************************************************************************/
/* 函数名	: AppGW_Init										 	 							*/
/* 输入  	: 无							 													*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化应用层网关相关信息						 									*/
/* 创建日期	: 2016/12/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_EnvironInit(void)
{
	/* 创建软件定时器结果寄存器 */
	INT8U tmrErr;
	
	
	/* 先初始化 系统先关的参数 */
	/* 创建 南京盛亿网关消息队列 */
	AppGWHZZH_Q = OSQCreate(AppGWHZZH_Grp,AppGWHZZH_Q_Len);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
	if(AppGWHZZH_Q == (OS_EVENT *)0)
	{
		/* 创建队列失败 */
		__nop();
	}
#endif
	
	
	/* 创建 网关看门狗软件定时器 */
	AppGWHZZH_Watchdog_ptmr = 
	OSTmrCreate (
					AppGWHZZH_WatchdogTimeValue,	//软件定时器启动后延时多少时间调用回调函数
					AppGWHZZH_WatchdogTimeValue,	//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_ONE_SHOT,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)AppGWHZZH_Watchdog_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"网关看门狗定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);
	
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
	
	/* 创建接收数据计时软件定时器 */
	AppGWHZZH_TimeOut_ptmr = 
	OSTmrCreate (
					AppGWHZZH_TimeOutValue,			//软件定时器启动后延时多少时间调用回调函数
					AppGWHZZH_TimeOutValue,			//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_PERIODIC,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)AppGWHZZH_TimeOut_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"接收数据计时定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);
	
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
	
	/* 创建 网关状态软件定时器 */
	AppGWHZZH_State_ptmr = 
	OSTmrCreate (
					(10u),							//软件定时器启动后延时多少时间调用回调函数
					AppGWHZZH_AStateTimeValue,		//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_PERIODIC,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)AppGWHZZH_State_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"网关状态定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);
							
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
	
	
	
}

/************************************************************************************************/
/* 函数名	: AppGWHZZH_InitAPI										 	 							*/
/* 输入  	: initPara:初始化网关参数															*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化网关						 													*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void AppGWHZZH_InitAPI(void)
{

	
	/* 初始化网关任务参数 */
	AppGWHZZH_EnvironInit();
	
	/* 初始化接口 */
	AppGWHZZH_API.Init		 = AppGWHZZH_Init;
	
	/* 重启接口 */
	AppGWHZZH_API.Restart	 = AppGWHZZH_Restart;
	
	/* 更新网关当前工作状态 */
	AppGWHZZH_API.UpdateState= AppGWHZZH_UpdateState;
	
	/* 获取网关当前工作状态 */
	AppGWHZZH_API.GetState	 = AppGWHZZH_GetState;
	
	/* 读响应时间和探头数量接口 */
	AppGWHZZH_API.ReadWkupTimeProbeNum = AppGWHZZH_ReadWkupTimeProbeNum;
	
	/* 设置响应时间 */
	AppGWHZZH_API.SetWkupTime = AppGWHZZH_SetWkupTime;
	
	/* 设置探头数量 */
	AppGWHZZH_API.SetProbeNum = AppGWHZZH_SetProbeNum;
	
}

#if (1)//杭州创辉网关任务处理
/************************************************************************************************/
/* 函数名	: AppGWHZZH_CmdHandle																*/
/* 输入  	: cmd:应用命令 																		*/
/* 输出		: 无 																				*/
/* 作用	 	: 南京盛亿网关处理任务																*/
/* 创建日期 	: 2016/12/16																		*/
/************************************************************************************************/
static void AppGWHZZH_CmdHandle(AppGWHZZHCmdStruct* cmd)
{
	/* 操作状态寄存器 */
	const static GWHZZH_Result gSuccess = GWHZZH_Success;
	const static GWHZZH_Result gFail 	= GWHZZH_Fail;
				 GWHZZH_Result gState;
	/* 消息邮箱状态信息 */
	INT8U gMboxerr;
	/* 邮箱句柄指针 */
	OS_EVENT* gMboxp;
	
	
	/* 判断命令类型 */
	switch(cmd->Type)
	{
		/* 初始化杭州创辉网关 */
 		case AppGWHZZHCmd_Init:
 		{
			/* 初始化杭州创辉网关逻辑层 */
			LogGWHZZHAPI.Init();
			return;
 		}
		
		/* 重启杭州创辉网关 */
 		case AppGWHZZHCmd_Restart:
 		{
			/* 重启杭州创辉网关逻辑层 */
			LogGWHZZHAPI.Restart();
			/* 等待系统运行稳定 */
			OSTimeDly(AppGW_RestartTimeValue);
			return;
 		}
		
		/* 读杭州创辉网关工作状态 */
 		case AppGWHZZHCmd_UpdateState:
 		{
			/* 读杭州创辉网关数据指针 */
			AppGWHZZHStateMboxStruct* gStatep = cmd->Para;
			
			
			/* 设置消息邮箱句柄 */
			gMboxp = gStatep->MboxHandle;
			/* 重启杭州创辉网关逻辑层 */
			gState = LogGWHZZHAPI.State();
			
			/* 判断工作状态是否变化 */
			if(AppGWHZZHRunPara.SR.WorkState != gState)
			{
				/* 停止状态软件定时器 */
				/* 关闭软件定时器状态 */
				BOOLEAN tmrState;
				/* 创建软件定时器结果寄存器 */
				INT8U tmrErr;
				
				
				/* 停止定时初始化网关定时器 */
				tmrState = OSTmrStop (AppGWHZZH_State_ptmr,
									  OS_TMR_OPT_NONE,
									  (OS_TMR_CALLBACK)AppGWHZZH_State_CallBack,
									  &tmrErr);
				
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
				if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
				{__nop();}
#endif
				
				/* 删除软件定时器 */
				tmrState = OSTmrDel (AppGWHZZH_State_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
				if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
				{__nop();}
#endif
				
				/* 工作状态发什么变化，判断网关的工作状态。 */
				if(GWHZZH_Success == gState)
				{
					/* 网关状态正常，设置正常的查选时间 */
					/* 创建 网关状态软件定时器 */
					AppGWHZZH_State_ptmr = 
					OSTmrCreate (
									AppGWHZZH_NStateTimeValue,		//软件定时器启动后延时多少时间调用回调函数
									AppGWHZZH_NStateTimeValue,		//在周期定时模式下，调用回调函数的周期
									OS_TMR_OPT_PERIODIC,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
									(OS_TMR_CALLBACK)AppGWHZZH_State_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
									(void*)0,						//回调函数参数
									"网关状态定时器",				//回调函数名称
									&tmrErr							//创建软件定时器函数执行结果寄存器
								);
											
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))					
					/* 判断软件定时器创建状态 */
					if(tmrErr != OS_ERR_NONE)
					{__nop();/* 失败 */}
#endif
					
				}else
				{
					/* 网关状态正常，设置正常的查选时间 */
					/* 创建 网关状态软件定时器 */
					AppGWHZZH_State_ptmr = 
					OSTmrCreate (
									AppGWHZZH_NStateTimeValue,		//软件定时器启动后延时多少时间调用回调函数
									AppGWHZZH_AStateTimeValue,		//在周期定时模式下，调用回调函数的周期
									OS_TMR_OPT_PERIODIC,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
									(OS_TMR_CALLBACK)AppGWHZZH_State_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
									(void*)0,						//回调函数参数
									"网关状态定时器",				//回调函数名称
									&tmrErr							//创建软件定时器函数执行结果寄存器
								);
					
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))					
					/* 判断软件定时器创建状态 */
					if(tmrErr != OS_ERR_NONE)
					{__nop();/* 失败 */}
#endif
				}
				
				/* 设置网关工作状态 */
				AppGWHZZHRunPara.SR.WorkState = gState;
				/* 启动定时初始化网关定时器 */
				tmrState = OSTmrStart (AppGWHZZH_State_ptmr,&tmrErr);
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == 1))
				if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
				{__nop();}
#endif
			}
			break;
 		}
		
		/* 读杭州创辉网关响应时间和探头数量 */
 		case AppGWHZZHCmd_ReadWkupTimeProbeNum:
 		{
			/* 读杭州创辉网关数据指针 */
			AppGWHZZHRWTPNMboxStruct* gRWTPNp = cmd->Para;

			
			/* 设置消息邮箱句柄 */
			gMboxp = gRWTPNp->MboxHandle;
			/* 读杭州创辉网关响应时间和探头数量 */
			gState = LogGWHZZHAPI.ReadWkupTimeProbeNum();
			break;
 		}
		
		/* 设置杭州创辉网关响应时间 */
 		case AppGWHZZHCmd_SetWkupTime:
 		{
			/* 读杭州创辉网关数据指针 */
			AppGWHZZHSWTMboxStruct* gSWTp = cmd->Para;
			
			
			/* 设置消息邮箱句柄 */
			gMboxp = gSWTp->MboxHandle;
			/* 读杭州创辉网关响应时间和探头数量 */
			gState = LogGWHZZHAPI.SetWkupTime(gSWTp->WkupTime,gSWTp->ProbeNum);
			break;
 		}
		
		/* 设置杭州创辉网关探头数量 */
 		case AppGWHZZHCmd_SetProbeNum:
 		{	
			/* 读杭州创辉网关数据指针 */
			AppGWHZZHSPNMboxStruct* gSPNp = cmd->Para;
			
			
			/* 设置消息邮箱句柄 */
			gMboxp = gSPNp->MboxHandle;
			/* 读杭州创辉网关响应时间和探头数量 */
			gState = LogGWHZZHAPI.SetWkupTime(gSPNp->WkupTime,gSPNp->ProbeNum);
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
	if(gState == GWHZZH_Success)
	{
		/* 命令执行成功，发送消息邮箱。 */
		gMboxerr = OSMboxPost(gMboxp,((void*)&gSuccess));
	}else
	{
		/* 命令执行失败 */
		gMboxerr = OSMboxPost(gMboxp,((void*)&gFail));
	}
#if  ((AppGatewayHZZHDebugEnable == 0x01)&&(AppGatewayHZZHDebugInform == (1u)))
	/* 判断发送命令状态 */
	if(gMboxerr != OS_ERR_NONE)
	{
		/* 发送命令失败 */
		__nop();
	}
#endif
}

/************************************************************************************************/
/* 函数名	: AppTaskGatewayHZZH																*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 南京盛亿网关处理任务																*/
/* 创建日期 	: 2016/12/16																		*/
/************************************************************************************************/
void AppTaskGatewayHZZH(void *p_arg)
{
	/* 网关命令结构 */
	AppGWHZZHCmdStruct* gAppGWHZZHCmdDatap;
	/* 申请队列状态信息 */
	INT8U gQerr;
	(void)p_arg;
	
	
	/* 初始化应用层网关API函数 */
	AppGWHZZH_InitAPI();
	
	/* 启动网关，初始化配置 */
	AppGWHZZH_API.Init();
	
	while (1)
	{
		/* 等待 队列 消息 */
		gAppGWHZZHCmdDatap = (AppGWHZZHCmdStruct*)OSQPend(AppGWHZZH_Q,0,&gQerr);
		
		/* 接收消息 状态 判断 */
		if(gQerr == OS_ERR_NONE)
		{
			/* 处理网关操作命令 */
			AppGWHZZH_CmdHandle(gAppGWHZZHCmdDatap);
		}
	}

}
#endif
/***********************************************************************************/



#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
