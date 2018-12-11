/************************************************************************************************/
/** @file              : Log_Gateway_HZZH.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年12月14日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--杭州创辉网关逻辑层文件							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0      ZRDang      2016年12月14日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_Gateway_HZZH.h"
#if (Gateway_HZZH_Enable == 0x01)
#include "Dev_Gateway_HZZH.h"
#include <ucos_ii.h>


/********************************* 宏定义 *********************************/
/* 杭州创辉网关通信数据包头 */
#define LogGW_ComPackHeader			(0X9BB9)

/* 杭州创辉网关申请发送命令信号量等待时间 */
#define LogGW_SemOvertimeValue		(20u)

/* 杭州创辉网关等待命令响应的超时时间(50ms) */
#define LogGWHZZH_ReceCmdAckOvertimeValue	(50u)

/* 杭州创辉网关重新启动网关的时间 */
#define LogGW_RestartTimeValue		(100u)

/* 杭州创辉网关串口通信波特率 */
#define LogGW_Baudrate				(115200ul)

/* 杭州创辉网关接收数据帧状态(无完整的数据帧) */
#define LogGW_ReceDataNull			(0XFF)

/**************************************************************************/
/************************** 杭州创辉网关命令结构 ***************************/
/* 杭州创辉网关功能码类型 */
typedef struct
{
	/* 包头 */
	uint8_t	PackHeader[2];
	/* 长度 */
	uint8_t		Len;
	/* ID号 */
	uint8_t		ID[4];
	/* 命令类型 */
	GWHZZHCmdType	CmdType;
}LogGWCmdStruct;

/* 杭州创辉网关命令数据最大长度(根据“路由上报格式_基础版1014”可知最大长度为34字节) */
#define LogGWCmd_DataLenMax			(4u)
/* 杭州创辉网关命令数据结构 */
typedef struct
{
	uint8_t Buf[LogGWCmd_DataLenMax+10];
}LogGWCmdReceBufStruct;

/* 杭州创辉网关数据应答命令数据结构 */
typedef struct
{
	/* 固定的数据结构 */
	LogGWCmdStruct LogGWCmdFix;
	/* 数据 */
	uint8_t		Data[LogGWCmd_DataLenMax];
	/* 校验CKS */
	uint16_t	Check;
}LogGWCmdDataACKStruct;

/* 杭州创辉网关设置响应时间命令数据结构 */
typedef struct
{
	/* 固定的数据结构 */
	LogGWCmdStruct LogGWCmdFix;
	/* 响应时间 */
	uint8_t	WkupTime;
	/* 探头数量 */
	uint8_t	ProbeNum;
	/* 校验CKS */
	uint16_t	Check;
}LogGWCmdSetWkupTimeStruct;

/* 杭州创辉网关设置响应时间的应答命令数据结构 */
typedef struct
{
	/* 固定的数据结构 */
	LogGWCmdStruct LogGWCmdFix;
	/* 校验CKS */
	uint16_t	Check;
}LogGWCmdSetWkupTimeACKStruct;

/* 杭州创辉网关读响应时间命令数据结构 */
typedef struct
{
	/* 固定的数据结构 */
	LogGWCmdStruct LogGWCmdFix;
	/* 校验CKS */
	uint16_t	Check;
}LogGWCmdReadWkupTimeStruct;

/* 杭州创辉网关读响应时间的应答命令数据结构 */
typedef struct
{
	/* 固定的数据结构 */
	LogGWCmdStruct LogGWCmdFix;
	/* 响应时间 */
	uint8_t	WkupTime;
	/* 探头数量 */
	uint8_t	ProbeNum;
	/* 校验CKS */
	uint16_t	Check;
}LogGWCmdReadWkupTimeACKStruct;

/************************** 杭州创辉网关接收数据状态结构 ********************/
typedef struct
{
	
	/* 状态寄存器 */
	struct
	{
		/* 状态寄存器 */
		struct
		{
			/* 开始接收数据标志 */
			uint8_t StartFlag;
			/* 读网关响应时间和探头数量命令响应状态 */
			GWHZZH_Result CmdAckRWTPNState;
			/* 设置网关响应时间和探头数量命令响应状态 */
			GWHZZH_Result CmdAckSWTPNState;
		}SR;
		
		/* 数据寄存器 */
		struct
		{
			/* 接收命令/数据寄存器 */
			LogGWCmdReceBufStruct Reg[2];

		}DR;
		
		/* 接收数据指针 */
		uint8_t Index;	
		/* 接收数据寄存器编号 */
		uint8_t DRNo;
		/* 接收完成寄存器编号 */
		uint8_t FinishDRNo;
	}ReceReg;

}LogGWRunParaStruct;



/* 网关接收应答状态 */
/********************************* 杭州创辉网关接收应答状态 ****************/
typedef enum {GWHZZH_ReceCmdAckFail = (0u), 
			  GWHZZH_ReceCmdAckSuccess = (1u)} GWHZZH_ReceCmdAckState;

/**************************************************************************/

/********************************* 请求发送命令信号量 **********************/
static OS_EVENT* LogGW_Sem;

/********************************* 杭州创辉网关接收读响应时间和探头数量命令响应
								   消息邮箱指针 ****/
static OS_EVENT*  GWHZZH_RRWTPNCAMp = (OS_EVENT *)0;

/********************************* 杭州创辉网关接收设置响应时间和探头数量命令响
								   应消息邮箱指针 ****/
static OS_EVENT*  GWHZZH_RSWTPNCAMp = (OS_EVENT *)0;			  
			  

/********************************* 杭州创辉网关API ************************/
LogGatewayHZZHAPIStruct LogGWHZZHAPI;
/********************************* 杭州创辉网关逻辑层接收数据状态寄存器 ****/
static LogGWRunParaStruct LogGWRunPara = 
{	
	
	/* 开始接收数据标志 */
	0,
	/* 读网关响应时间和探头数量命令响应状态 */
	GWHZZH_Fail,
	/* 设置网关响应时间和探头数量命令响应状态 */
	GWHZZH_Fail,
	
	
	/* 数据寄存器 */
	0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,
	0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,
	
	
	/* 接收数据指针 */
	0,
	/* 接收数据寄存器编号 */
	0,
	/* 接收完成寄存器编号 */
	LogGW_ReceDataNull,
};

/*************************************************************************/

/********************************* 杭州创辉网关响应处理函数指针 ************/
/* 探头数据响应指针 */
void (*LogGW_ProbeDataACKp)(uint8_t* id,uint8_t* data);
/* 设置网关响应时间和探头数量指针 */
void (*LogGW_SetWkupTimeProbeNumACKp)(void);
/* 读网关响应时间和探头数量指针 */
void (*LogGW_ReadWkupTimeProbeNumACKp)(uint8_t wkupTime,uint8_t ProbeNum);

/*************************************************************************/

#if (1)//重启网关软件定时器回调函数
/* 重启网关软件定时器 句柄指针 */
static OS_TMR* LogGW_Restart_ptmr;
/************************************************************************************************/
/* 函数名	: LogGW_Restart_CallBack										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 重启网关软件定时器回调函数						 									*/
/* 创建日期	: 2016/12/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void LogGW_Restart_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* 创建启动软件定时器信息 */
	INT8U tmrErr;
	/* 创建启动软件定时器状态 */
	BOOLEAN tmrState;
	
	
	(void) p_arg;

	/* 启动网关电源 */
	DevGWHZZHAPI.OpenPower();
	
	/* 开闭重启定时器 */
	tmrState = OSTmrStop(ptmr,
                         OS_TMR_OPT_NONE,
                         (void*)0,
                         &tmrErr);
	/* 判断关闭软件定时器状态 */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* 关闭软件定时器失败 */
		__nop();
	}
}

#endif

#if (1)//杭州创辉网关逻辑层函数
/************************************************************************************************/
/* 函数名	: LogGW_GetCheck										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 校验结果结果																		*/
/* 作用	 	: 计算杭州创辉通信数据校验值					 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static uint16_t LogGW_GetCheck(LogGWCmdStruct* dataFrame)
{	
	uint16_t nResult = 0;
	uint8_t index;
	/* 和校验 */
	uint8_t gCheckSum;
	/* 异或校验 */
	uint8_t gCheckXor;
	
	
	gCheckSum = dataFrame->Len;
	
	gCheckXor = dataFrame->Len;
	
	for(index = 3;index < (dataFrame->Len+1);index ++)
	{
		gCheckSum += *((dataFrame->PackHeader)+index);
		
		gCheckXor ^= *((dataFrame->PackHeader)+index);
		
		gCheckXor ++ ;
	}
	
	/* 交换高低位 */
	nResult = gCheckSum+gCheckXor*256;
	
	return nResult;
}

/************************************************************************************************/
/* 函数名	: LogGW_ParseData										 	 						*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 初始化杭州创辉网关逻辑层运行参数					 								*/
/* 创建日期	: 2016/12/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void LogGW_ParseData(LogGWCmdStruct* dataFrame)
{	
	/* 数据帧校验 */
	uint16_t gCheck;
	/* 创建启动软件定时器信息 */
	INT8U MboxErr;
	

	/* 计算校验 */
	gCheck = LogGW_GetCheck(dataFrame);
	
	switch(dataFrame->CmdType)
	{
		/* 探头响应命令 */
 		case GWHZZHCmd_ProbeDataACK:
		{
			/* 判断校验值是否正确 */
			if(gCheck != ((LogGWCmdDataACKStruct*)dataFrame)->Check)
			{
				/* 校验不正确 */
				__nop();
				return;
			}else
			{
				/* 处理探头数据响应 */
				LogGW_ProbeDataACKp(((LogGWCmdDataACKStruct*)dataFrame)->LogGWCmdFix.ID,
									((LogGWCmdDataACKStruct*)dataFrame)->Data);
			}
			break;
		}
		
		/* 设置唤醒时间响应命令 */
 		case GWHZZHCmd_SetWkupTimeProbeNumACK:
		{
			/* 判断校验值是否正确 */
			if(gCheck != ((LogGWCmdSetWkupTimeACKStruct*)dataFrame)->Check)
			{
				/* 校验不正确 */
				__nop();
				return;
			}else
			{
				/* 处理设置响应时间和探头数量 */
				LogGW_SetWkupTimeProbeNumACKp();
				
				/* 设置网关响应时间和探头数量命令响应状态 */
				LogGWRunPara.ReceReg.SR.CmdAckSWTPNState = GWHZZH_Success;
				
				/* 设置响应时间和探头数量成功 */
				/* 发送消息 */
				MboxErr = OSMboxPost (GWHZZH_RSWTPNCAMp, 
									 (void*)&LogGWRunPara.ReceReg.SR.CmdAckSWTPNState);
				
				/* 判断接收数据状态消息创建状态 */
				if(MboxErr != OS_ERR_NONE)
				{__nop();/* 失败 */}
			}
			break;
		}
		
		/* 读唤醒时间响应命令 */
 		case GWHZZHCmd_ReadWkupTimeProbeNumACK:
		{
			/* 判断校验值是否正确 */
			if(gCheck != ((LogGWCmdReadWkupTimeACKStruct*)dataFrame)->Check)
			{
				/* 校验不正确 */
				__nop();
				return;
			}else
			{
				/* 处理读响应时间和探头数量 */
				LogGW_ReadWkupTimeProbeNumACKp(((LogGWCmdReadWkupTimeACKStruct*)dataFrame)->WkupTime,
											   ((LogGWCmdReadWkupTimeACKStruct*)dataFrame)->ProbeNum);
				
				/* 读网关响应时间和探头数量命令响应状态 */
				LogGWRunPara.ReceReg.SR.CmdAckRWTPNState = GWHZZH_Success;
				
				/* 设置响应时间和探头数量成功 */
				/* 发送消息 */
				MboxErr = OSMboxPost (GWHZZH_RRWTPNCAMp, 
									 (void*)&LogGWRunPara.ReceReg.SR.CmdAckRWTPNState);
				
				/* 判断接收数据状态消息创建状态 */
				if(MboxErr != OS_ERR_NONE)
				{__nop();/* 失败 */}
			}
			break;
		}
		
		default:
		{
			/* 未设定类型，不作处理。 */
			__nop();
			return;
		}
	}
	
}

/************************************************************************************************/
/* 函数名	: LogGW_ReceData										 	 						*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 初始化杭州创辉网关逻辑层运行参数					 								*/
/* 创建日期	: 2016/12/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void LogGW_ReceData(uint8_t data)
{
	/* 判断开始接收数据标志 */
	if(LogGWRunPara.ReceReg.SR.StartFlag == (1u))
	{
		/* 开始接收数据帧 */
		LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo].\
		Buf[LogGWRunPara.ReceReg.Index] = data;
		
		/* 设置接收数据寄存器指针 */
		LogGWRunPara.ReceReg.Index++;
		
		/* 判断数据是否接收完成 */
		if(((((LogGWCmdStruct*)(&LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo])))->Len +3) ==\
			LogGWRunPara.ReceReg.Index)
		{
			/* 数据接收完成，设置数据帧接收完成编号。 */
			LogGWRunPara.ReceReg.FinishDRNo = LogGWRunPara.ReceReg.DRNo;
			/* 修改接收数据寄存器编号 */
			if(LogGWRunPara.ReceReg.DRNo == (0u))
			{LogGWRunPara.ReceReg.DRNo = (1u);}
			else
			{LogGWRunPara.ReceReg.DRNo = (0u);}
			
			/* 清除接收数据寄存器指针 */
			LogGWRunPara.ReceReg.Index = (0u);
			/* 清除开始接收数据帧标志 */
			LogGWRunPara.ReceReg.SR.StartFlag = (0u);
		}else if((((LogGWCmdStruct*)(&LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo])+3))->Len ==\
				(LogGWCmd_DataLenMax+(10u)))
		{
			/* 接收的长度超出当前协议最大数据长度。 */
			/* 清除接收数据寄存器指针 */
			LogGWRunPara.ReceReg.Index = (0u);
			/* 清除开始接收数据标志 */
			LogGWRunPara.ReceReg.SR.StartFlag = (0u);
		}
	}else
	{
		/* 未开始接收，判断当前数据是否为开始接收标志数据 */
		if(((LogGW_ComPackHeader>>8) == data)&&((0u) == LogGWRunPara.ReceReg.Index))
		{
			/* 当前数据为开始接收标志的第一个参数 */
			/* 保存当前数据 */
			LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo].\
			Buf[LogGWRunPara.ReceReg.Index] = data;
			/* 设置接收数据寄存器指针 */
			LogGWRunPara.ReceReg.Index++;
			
		}else if(((LogGW_ComPackHeader&0xFF) == data)&&((1u) == LogGWRunPara.ReceReg.Index))
		{
			/* 当前数据为开始接收标志的第一个参数 */
			/* 保存当前数据 */
			LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.DRNo].\
			Buf[LogGWRunPara.ReceReg.Index] = data;
			/* 设置接收数据寄存器指针 */
			LogGWRunPara.ReceReg.Index++;
			/* 设置开始接收数据标志 */
			LogGWRunPara.ReceReg.SR.StartFlag = (1u);
		}else
		{
			/* 当前数据不是开始接收标志的第一个参数 */
			/* 清除接收数据寄存器指针 */
			LogGWRunPara.ReceReg.Index = (0u);
			/* 清除开始接收数据标志 */
			LogGWRunPara.ReceReg.SR.StartFlag = (0u);
		}
	}
	
	/* 判断数据帧接收完成状态 */
	if(LogGW_ReceDataNull != LogGWRunPara.ReceReg.FinishDRNo)
	{
		/* 数据帧就收完成，解析当前数据。 */
		LogGW_ParseData((void*)&LogGWRunPara.ReceReg.DR.Reg[LogGWRunPara.ReceReg.FinishDRNo]);
		/* 清除接收完成寄存器编号 */
		LogGWRunPara.ReceReg.FinishDRNo = LogGW_ReceDataNull;
	}
	
}

/************************************************************************************************/
/* 函数名	: LogGWCmd_Load										 	 							*/
/* 输入  	: 无			 																	*/
/* 输出		: 校验结果结果																		*/
/* 作用	 	: 计算杭州创辉通信数据校验值					 										*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void LogGWCmd_Load(uint8_t* dest,LogGWCmdStruct* src)
{
	/* 计数器 */
	uint8_t num;
	/* 数据指针 */
	uint8_t* gData;
	/* 校验指针 */
	uint16_t* gCheck;
	
	/* 判断数据长度 */
	if(src->Len < 7)
	{
		/* 数据长度小于通信数据最小长度，说明不正确。 */
		return;
	}
		
	/* 获取包头 */
	dest[0] = src->PackHeader[0];
	dest[1] = src->PackHeader[1];
	
	/* 获取数据长度 */
	dest[2] = src->Len;
	
	/* 获取ID */
	dest[3] = src->ID[0];
	dest[4] = src->ID[1];
	dest[5] = src->ID[2];
	dest[6] = src->ID[3];
	
	/* 获取命令 */
	dest[7] = src->CmdType;
	
	switch(src->CmdType)
	{
		/* 设置唤醒时间命令 */
 		case GWHZZHCmd_SetWkupTimeProbeNum:
		{
			/* 设置数据指针 */
			gData = &((LogGWCmdSetWkupTimeStruct*)src)->WkupTime;
			/* 设置校验指针 */
			gCheck = &((LogGWCmdSetWkupTimeStruct*)src)->Check;
			break;
		}
		
		/* 读唤醒时间命令 */
 		case GWHZZHCmd_ReadWkupTimeProbeNum:
		{
			/* 设置数据指针 */
			gData = (uint8_t*)src;
			/* 设置校验指针 */
			gCheck = &((LogGWCmdReadWkupTimeStruct*)src)->Check;
			break;
		}

		default:
		{
			/* 未设定类型，不作处理。 */
			__nop();
			return;
		}
	}
	
	for(num = 0;num < (src->Len-7);num++)
	{
		/* 获取数据 */
		dest[num+8] = gData[num];
	}
	
	/* 获取校验值 */
	dest[num+8] = *gCheck;
	/* 获取校验值 */
	dest[num+9] = ((*gCheck)>>8);
	
}

/************************************************************************************************/
/* 函数名	: LogGWCmd_InitPara										 	 						*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 初始化杭州创辉网关逻辑层运行参数					 								*/
/* 创建日期	: 2016/12/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void LogGWCmd_InitPara(void)
{
	/* 创建软件定时器结果寄存器 */
	INT8U tmrErr;
	
	
	/* 创建重启网关软件定时器 */
	LogGW_Restart_ptmr = 
	OSTmrCreate (
					LogGW_RestartTimeValue,			//软件定时器启动后延时多少时间调用回调函数
					LogGW_RestartTimeValue,			//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_ONE_SHOT,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)LogGW_Restart_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"网关重启定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);					
							
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
	
					
	/* 创建 接收读网关响应时间和探头数量 消息邮箱 */
	GWHZZH_RRWTPNCAMp = OSMboxCreate ((void*)0);
	/* 判断接收数据状态消息创建状态 */
	if(GWHZZH_RRWTPNCAMp == ((OS_EVENT *)0))
	{__nop();/* 失败 */}
	
	/* 创建 接收读网关响应时间和探头数量 消息邮箱 */
	GWHZZH_RSWTPNCAMp = OSMboxCreate ((void*)0);
	/* 判断接收数据状态消息创建状态 */
	if(GWHZZH_RSWTPNCAMp == ((OS_EVENT *)0))
	{__nop();/* 失败 */}
	
}

#endif

#if (1)//杭州创辉网关功能接口函数
/************************************************************************************************/
/* 函数名	: LogGW_Restart										 	 							*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 重启网关					 														*/
/* 创建日期	: 2016/12/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void LogGW_Restart(void)
{
	/* 创建启动软件定时器信息 */
	INT8U tmrErr;
	/* 创建启动软件定时器状态 */
	BOOLEAN tmrState;
	
	
	/* 关闭网关电源 */
	DevGWHZZHAPI.ClosePower();
	
	/* 启动重启定时器（延时1S调用 网关重启回调函数） */
	tmrState = OSTmrStart (LogGW_Restart_ptmr,&tmrErr);
	if((tmrErr != OS_ERR_NONE)||(tmrState != OS_FALSE))
	{__nop();}
}

/************************************************************************************************/
/* 函数名	: LogGW_ReadWkupTime_ProbeNum										 	 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 读唤醒时间和标签数量					 											*/
/* 创建日期	: 2016/12/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result LogGW_ReadWkupTimeProbeNum(void)
{
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	/* 读唤醒时间和标签数量寄存器 */
	LogGWCmdReadWkupTimeStruct LogGWCmdReadWkupTime = {0};
	/* 发送数据寄存器 */
	static uint8_t gSendReg[10];
	
	
	/* 申请网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_SemOvertimeValue*10),
			  &mSemErr);
	
	/* 判断信号量状态 */
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GWHZZH_Fail;
	}

	/* 设置命令参数 */
	LogGWCmdReadWkupTime.LogGWCmdFix.PackHeader[0]	= (LogGW_ComPackHeader>>8);
	LogGWCmdReadWkupTime.LogGWCmdFix.PackHeader[1]	= (LogGW_ComPackHeader&0xFF);
	LogGWCmdReadWkupTime.LogGWCmdFix.Len 		= (7u);
	LogGWCmdReadWkupTime.LogGWCmdFix.ID[0] 	 	= (0x0);
	LogGWCmdReadWkupTime.LogGWCmdFix.ID[1] 	 	= (0x0);
	LogGWCmdReadWkupTime.LogGWCmdFix.ID[2]	 	= (0x0);
	LogGWCmdReadWkupTime.LogGWCmdFix.ID[3]	 	= (0x0);
	LogGWCmdReadWkupTime.LogGWCmdFix.CmdType	= (GWHZZHCmd_ReadWkupTimeProbeNum);
	
	/* 计算校验 */
	LogGWCmdReadWkupTime.Check = LogGW_GetCheck(&LogGWCmdReadWkupTime.LogGWCmdFix);
	
	/* 将设置好的参数转移到发送寄存器中 */
	LogGWCmd_Load(gSendReg,&LogGWCmdReadWkupTime.LogGWCmdFix);
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GWHZZH_RRWTPNCAMp);
	
	/* 发送读地址命令 */
	DevGWHZZHAPI.SendData(gSendReg,sizeof(gSendReg));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GWHZZH_RRWTPNCAMp),
						   (LogGWHZZH_ReceCmdAckOvertimeValue),
						   (&mBoxErr));
	
	/* 申请网关信号量 */
	mSemErr = OSSemPost (LogGW_Sem);
	if(mSemErr != OS_ERR_NONE)
	{__nop();}
	
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GWHZZH_ReceCmdAckState*)mbox_Mgs) == GWHZZH_ReceCmdAckSuccess))
	{
		/* 网关应答成功 */
		return GWHZZH_Success;
	}
	
	/* 返回操作结果 */
	return GWHZZH_Fail;
}

/************************************************************************************************/
/* 函数名	: LogGW_State										 	 							*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 读网关工作状态					 													*/
/* 创建日期	: 2016/12/19																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result LogGW_State(void)
{
	/* 发送读响应时间和探头数量 */
	return LogGW_ReadWkupTimeProbeNum();
}

/************************************************************************************************/
/* 函数名	: LogGW_SetWkupTime										 	 						*/
/* 输入  	: time:唤醒时间数据,probeNum:探头数量			 										*/
/* 输出		: 操作结果																			*/
/* 作用	 	: 设置杭州创辉网关唤醒时间功能					 									*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result LogGW_SetWkupTime(uint8_t time,uint8_t probeNum)
{
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	/* 读唤醒时间和标签数量寄存器 */
	LogGWCmdSetWkupTimeStruct LogGWCmdSetWkupTime = {0};
	/* 发送数据寄存器 */
	static uint8_t gSendReg[12];
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_SemOvertimeValue*10),
			  &mSemErr);
	
	/* 判断信号量状态 */
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GWHZZH_Fail;
	}
	
	/* 设置命令参数 */
	LogGWCmdSetWkupTime.LogGWCmdFix.PackHeader[0]	= (LogGW_ComPackHeader>>8);
	LogGWCmdSetWkupTime.LogGWCmdFix.PackHeader[1]	= (LogGW_ComPackHeader&0xFF);
	LogGWCmdSetWkupTime.LogGWCmdFix.Len 		= (9u);
	LogGWCmdSetWkupTime.LogGWCmdFix.ID[0] 	 	= (0x0);
	LogGWCmdSetWkupTime.LogGWCmdFix.ID[1] 	 	= (0x0);
	LogGWCmdSetWkupTime.LogGWCmdFix.ID[2]	 	= (0x0);
	LogGWCmdSetWkupTime.LogGWCmdFix.ID[3]	 	= (0x0);
	LogGWCmdSetWkupTime.LogGWCmdFix.CmdType		= (GWHZZHCmd_SetWkupTimeProbeNum);
	LogGWCmdSetWkupTime.WkupTime    			= (time);
	LogGWCmdSetWkupTime.ProbeNum 				= (probeNum);
	
	/* 计算校验 */
	LogGWCmdSetWkupTime.Check 					= LogGW_GetCheck(&LogGWCmdSetWkupTime.LogGWCmdFix);
	
	/* 将设置好的参数转移到发送寄存器中 */
	LogGWCmd_Load(gSendReg,&LogGWCmdSetWkupTime.LogGWCmdFix);
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GWHZZH_RSWTPNCAMp);
	
	/* 发送读地址命令 */
	DevGWHZZHAPI.SendData(gSendReg,sizeof(gSendReg));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GWHZZH_RSWTPNCAMp),
						   (LogGWHZZH_ReceCmdAckOvertimeValue),
						   (&mBoxErr));
	
	/* 申请网关信号量 */
	mSemErr = OSSemPost (LogGW_Sem);
	if(mSemErr != OS_ERR_NONE)
	{__nop();}
	
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GWHZZH_ReceCmdAckState*)mbox_Mgs) == GWHZZH_ReceCmdAckSuccess))
	{
		/* 网关应答成功 */
		return GWHZZH_Success;
	}
	
	/* 返回操作结果 */
	return GWHZZH_Fail;
}

/************************************************************************************************/
/* 函数名	: LogGW_SetProbeNum										 	 						*/
/* 输入  	: time:唤醒时间数据,probeNum:探头数量			 										*/
/* 输出		: 操作结果																			*/
/* 作用	 	: 设置杭州创辉网关标签总数功能					 									*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GWHZZH_Result LogGW_SetProbeNum(uint8_t time,uint8_t probeNum)
{	
	
	/* 返回操作结果 */
	return LogGW_SetWkupTime(time,probeNum);
}

/************************************************************************************************/
/* 函数名	: LogGW_Init										 	 							*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化网关功能					 													*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void LogGW_Init(void)
{
	
	/* 初始化驱动层接口 */
	DevGW_InitAPI();
	
	/* 初始化网关 */
	DevGWHZZHAPI.Init(LogGW_Baudrate,LogGW_ReceData);
	
	/* 创建网关信号量 */
	LogGW_Sem = OSSemCreate (1u);
	
	/* 等待网关启动（电源稳定） */
	OSTimeDly(10);
	
}

#endif

/************************************************************************************************/
/* 函数名	: LogGW_InitAPI										 	 							*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化逻辑层功能接口				 												*/
/* 创建日期	: 2016/12/14																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void LogGW_InitAPI(void (*probeDataACKp)(uint8_t* id,uint8_t* data),
				   void (*setWkupTimeProbeNumACKp)(void),
				   void (*readWkupTimeProbeNumACKp)(uint8_t wkupTime,uint8_t ProbeNum))
{	
	
	
	/* 探头数据响应指针 */
	LogGW_ProbeDataACKp = probeDataACKp;
	/* 设置网关响应时间和探头数量指针 */
	LogGW_SetWkupTimeProbeNumACKp = setWkupTimeProbeNumACKp;
	/* 读网关响应时间和探头数量指针 */
	LogGW_ReadWkupTimeProbeNumACKp = readWkupTimeProbeNumACKp;
	
	/* 初始化杭州创辉网关逻辑层运行参数 */
	LogGWCmd_InitPara();
	
	/* 设置系统接口函数 */
	/* 初始化接口 */
	LogGWHZZHAPI.Init 			= LogGW_Init;
	
	/* 重新启动网关接口 */
	LogGWHZZHAPI.Restart		= LogGW_Restart;
	
	/* 重新启动网关接口 */
	LogGWHZZHAPI.State			= LogGW_State;
	
	/* 设置网关唤醒时间 */
	LogGWHZZHAPI.SetWkupTime	= LogGW_SetWkupTime;
	
	/* 设置网关标签数量 */
	LogGWHZZHAPI.SetProbeNum	= LogGW_SetProbeNum;
	
	/* 读网关唤醒时间和标签数量 */
	LogGWHZZHAPI.ReadWkupTimeProbeNum	= LogGW_ReadWkupTimeProbeNum;
	
}




#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
