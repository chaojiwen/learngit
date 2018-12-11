/************************************************************************************************/
/** @file              : Log_Gateway_NJSY.c															*/
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
#include "Log_Gateway_NJSY.h"
#if (Gateway_NJSY_Enable == 0x01)
#include "Dev_Gateway_NJSY.h"
#include "Log_CRC16.h"


#include <ucos_ii.h>
#include <string.h>



/********************************* 宏定义 *********************************/
/* 网关接收数据 存储寄存器大小 */
#define GW_ReceDataBufSize		(264u)
/* 由于协议使用的是大端模式，STM32使用的是小端模式，因此这里需要转换为大端。 */
#define BigToLittle_Endian_HalfWord(data)	(((((uint16_t)data) & 0xff00) >> 8) |\
											 ((((uint16_t)data) & 0x00ff) << 8))
#define BigToLittle_Endian_Word(data)		(((((uint32_t)data) & 0xff000000) >> 24) |\
											 ((((uint32_t)data) & 0x00ff0000) >> 8)  |\
											 ((((uint32_t)data) & 0x0000ff00) << 8)  |\
											 ((((uint32_t)data) & 0x000000ff) << 24))
/* 网关接收数据超时数据 */
#define GW_ReceTimeoutPara		(-9990.0f)

/********************************* 网关运行参数类型 *********************************/
/* 网关接收应答状态 */
typedef enum {GW_ReceFail = (0u), GW_ReceSuccess = (1u)} GW_ReceState;
typedef enum {GW_Idle = (0u), GW_Busy = (1u)} GW_Run;

/* 网关功能码类型 */
typedef enum {
				/* 读数据 */
				GW_ReadData = (3u), 
				/* 配置参数 */
				GW_Config 	 = (6u),
				/* 读网关地址 */
				GW_ReadAdder = (83u)
			  }GWCMDType;
/* 网关配置参数地址 */
typedef enum {
				/* 设置地址 */
				GC_SetAdder 	= (0u), 
				/* 设置信道 */
				GC_SetChannel 	= (3u),
				/* 设置数据丢失时间 */
				GC_SetActiveTime = (4u),
				/* 设置通信波特率 */
				GC_SetBaudrate 	= (6u),
				/* 设置工作模式 */
				GC_SetMode 		= (7u),
/* 网关读数据地址（默认是从0x09地址开始，一个探头有2数据[1、温度，2、湿度]），
   每个数据有4个字节[float]，总共32个探头。因此对应通道地址依次往上递增，
   比如:探头2的地址为0x19[0x09+8*2]。	 */
				/* 读数据 */
				GC_ReadData		= (9u),
				/* 读网关地址命令 */
				GC_ReadAdder 	= (83u)
			 }GWConfigAdderType; 
/********************************* 网关运行参数结构 *********************************/
typedef struct
{
	/* 配置寄存器 */
	struct CR
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
	}CR;
	/* 状态寄存器 */
	struct SR
	{
		/* 网关工作状态 */
		GW_Result Work;
		/* 读探头数据状态 */
		GW_Result ReadData;
		/* 读网关地址状态 */
		GW_Result ReadAdder;
		/* 设置网关地址状态 */
		GW_Result SetAdder;
		/* 设置波特率 */
		GW_Result SetBaudrate;
		/* 设置信道状态 */
		GW_Result SetChannel;
		/* 设置数据丢失时间状态 */
		GW_Result SetInvalidTime;
		/* 设置网关工作模式状态 */
		GW_Result SetMode;
		/* 网关运行状态 */
		GW_Run	 Run;
	}SR;
	/* 数据寄存器 */
	struct DR
	{
		/* 探头通道数据 */
		float Data[32][2];
		/* 探头电池电量状态 */
		GW_Level Level[32];
		/* 读取的网关地址 */
		uint8_t Adder;
	}DR;
	
}GWRegStruct;

/********************************* 网关命令结构 *********************************/
typedef struct
{
	/* 网关地址 */
	uint8_t	GW_Adder;
	/* 功能码 */
	GWCMDType	Func;
	/* 预留码1(不知到这个什么用) */
	uint8_t	Reserve1;
	/* 访问地址 */
	GWConfigAdderType	Visit_Adder;
	/* 预留码2(不知到这个什么用) */
	uint8_t	Reserve2;
	/* 数据 */
	uint8_t	Data;
	/* 校验(Modbus RTU) */
	uint16_t Check;
}GWCMDStruct;

/********************************* 网关配置应答数据结构 *********************************/
typedef struct
{
	/* 网关地址 */
	uint8_t	GW_Adder;
	/* 功能码 */
	GWCMDType	Func;
	/* 预留码1(不知到这个什么用) */
	uint8_t	Reserve1;
	/* 访问地址 */
	GWConfigAdderType	Visit_Adder;
	/* 预留码2(不知到这个什么用) */
	uint8_t	Reserve2;
	/* 数据 */
	uint8_t* Data;
	/* 校验(Modbus RTU) */
	uint16_t* Check;
}GWConfigACKStruct;

/********************************* 网关读数据应答数据结构 *********************************/
typedef struct
{
	/* 网关地址 */
	uint8_t	GW_Adder;
	/* 功能码 */
	GWCMDType	Func;
	/* 数据长度 */
	uint8_t	Len;
	/* 数据 */
	uint8_t* Data;
	/* 校验(Modbus RTU) */
	uint16_t* Check;
}GWReadDateACKStruct;



/********************************* 网关运行参数 *********************************/
static GWRegStruct GWReg;
/********************************* 网关命令参数 *********************************/
static GWCMDStruct GWCMD;

/********************************* 网关接收消息参数 *********************************/
/* 接收状态  消息邮箱指针 */
static OS_EVENT*  GW_ReceStateMboxp = (OS_EVENT *)0;
/* 接收数据 存储寄存器 */
static uint8_t GW_ReceDataBuf[GW_ReceDataBufSize];
/* 接收数据 存储寄存器位置指针 */
static uint16_t GW_ReceDataIndex = 0;
/********************************* 网关接信号量参数 *********************************/
static OS_EVENT* LogGW_Sem;
/********************************* 网关API *********************************/
LogGatewayAPIStruct LogGW_API;

/********************************* 网关接收数据函数定义 *********************************/
static void GWLog_ReceData_Enable(void);
static void GWLog_ReceData_Disable(void);

static GW_Result GWLog_ReadProbe(uint8_t probeNo,GW_ProbeType probetype,float* probe);
static GW_Result GWLog_ReadLevel(uint8_t probeNo,GW_Level* level);
static GW_Result GWLog_ReadAdder(uint8_t* adder);
static GW_Result GWLog_SetAdder(uint8_t adder);
static GW_Result GWLog_SetChannel(uint8_t channel);
static GW_Result GWLog_SetActiveTime(uint8_t time);
static GW_Result GWLog_SetMode(GW_Mode mode);
static GW_Result GWLog_SetBaudrate(uint32_t baudrate);
/***********************************************************************************/
#if (1)//数据接收超时软件定时器回调函数
/* 数据接收超时软件定时器 句柄指针 */
static OS_TMR* GWLog_Overtime_ptmr;

/************************************************************************************************/
/* 函数名	: GWLog_Overtime_CallBack										 	 					*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 数据接收超时软件定时器回调函数						 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void GWLog_Overtime_CallBack(OS_TMR *ptmr,void *p_arg)
{	
	/* 创建启动软件定时器信息 */
	INT8U MboxErr;
	(void) p_arg;
	
	
	/* 关闭接收过程 */
	GWLog_ReceData_Disable();
	
	/* 发送失败消息 */
	MboxErr = OSMboxPost (GW_ReceStateMboxp, (void*)0);
	/* 判断发送消息状态 */
	if(MboxErr == OS_ERR_NONE)
	{__nop();}
	
}

#endif

#if (1)//接收状态软件定时器回调函数
/* 数据接收超时软件定时器 句柄指针 */
static OS_TMR* GWLog_ReceData_ptmr;

/************************************************************************************************/
/* 函数名	: GWLog_ReceData_CallBack										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 接收状态软件定时器回调函数						 									*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void GWLog_ReceData_CallBack(OS_TMR *ptmr,void *p_arg)
{
	(void) p_arg;

	/* 判断当前命令的应答数据 */
	
	/* 判断当前命令 功能码 */
	if(GWCMD.Func == GW_ReadData)
	{
		/* 判断数据是否接收完整 */
		if((GWCMD.Data > (0u))&&
		   (GWCMD.Data <=(128u))&&
		   (GW_ReceDataIndex == (5u+((uint16_t)GWCMD.Data)*2)))
		{
			/* 数据接收成功 */
			/* 消息应答指针 */
			GWReadDateACKStruct* gACKp = 
			(GWReadDateACKStruct*)GW_ReceDataBuf;
			GWReadDateACKStruct gACK;
			/* 创建启动软件定时器信息 */
			INT8U MboxErr;
			
			
			/*  */
			gACK = *gACKp;
			/* 设置数据地址 */
			gACK.Data = &GW_ReceDataBuf[3];
			/* 判断数据信息 */
			if((gACK.GW_Adder 	== GWCMD.GW_Adder)&&
			   (gACK.Func 		== GWCMD.Func)&&
			   (gACK.Len		==((((uint16_t)GWCMD.Data)*2)%0X100)))
			{
				/* 校验数据 */
				uint16_t check;
					
				
				/* 设置数据地址 */
				gACK.Check = (uint16_t*)&GW_ReceDataBuf[3u+((uint16_t)GWCMD.Data)*2];
				/* 计算校验 */
				check = BigToLittle_Endian_HalfWord(CRC16_GetValue(GW_ReceDataBuf,(3u+((uint16_t)GWCMD.Data)*2)));
				/* 判断校验值是否正常 */
				if(*gACK.Check == check)
				{
					/* 计数器 */
					uint8_t num;
					/* 数据指针 */
					uint32_t* gProbeDatap;
					
					
					/* 校验正确，设置读网关地址数据 */
					for(num = 0;num < (GWCMD.Data/2);num++)
					{
						/* 设置数据指针 */
						gProbeDatap = (((uint32_t*)gACK.Data)+num);
						
						/* 数据大小端转换 */
						*gProbeDatap = BigToLittle_Endian_Word(*gProbeDatap);
						
						/* 根据数据值，判断探头电量。 */
						/* 当某个传感器出现电量不足时，该传感器发来的数据的第一个
						   参数会+9000，行成一个极大值。 */
						if((*((float*)gProbeDatap)) >= (9000.0f))
						{
							/* 当前探头电池电量不足。 */
							GWReg.DR.Level[(num/2)] = GW_Level_FALSE;
							/* 复制数据 */
							GWReg.DR.Data[num/2][num%2] = ((*((float*)gProbeDatap))-(9000.0f));
						}else
						{
							/* 当前探头电池电量正常。 */
							GWReg.DR.Level[(num/2)] = GW_Level_TRUE;
							/* 复制数据 */
							GWReg.DR.Data[num/2][num%2] = (*((float*)gProbeDatap));
						}
						
						/* 判断当前网关的工作模式 */
						if(GWReg.CR.Mode == GW_Debug)
						{
							/* 网关处于调试模式，探头湿度数据将是0值，
							   这样界面显示整体体验比较差，因此，这里
							   将信号强度数据拷贝一份到湿度寄存器中。 */
							if((num%2) == (1u))
							{
								/* 拷贝温度寄存器中的信号强度数据到湿度寄存器中 */
								GWReg.DR.Data[num/2][num%2] = GWReg.DR.Data[num/2][0];
							}
						}
					}
					
					/* 设置读网关地址失败状态 */
					GWReg.SR.ReadData = GW_Success;
				}else
				{
					/* 校验错误 */
					/* 设置读网关地址失败状态 */
					GWReg.SR.ReadData = GW_Fail;
				}
			}else
			{
				/* 关闭接收过程，发送接收失败消息 */
				/* 设置读网关地址失败状态 */
				GWReg.SR.ReadData = GW_Fail;
			}
			/* 关闭接收过程 */
			GWLog_ReceData_Disable();
			
			/* 发送消息 */
			MboxErr = OSMboxPost (GW_ReceStateMboxp, (void*)&GWReg.SR.ReadData);
			/* 判断发送消息状态 */
			if(MboxErr == OS_ERR_NONE)
			{__nop();}
		}else
		{
			/* 数据没有接收完整，继续接收 */
			__nop();
		}
	}
	else if(GWCMD.Func == GW_Config)
	{
		/* 判断数据是否接收完整 */
		if(GW_ReceDataIndex == (8u))
		{
			/* 数据接收成功 */
			/* 消息应答指针 */
			GWConfigACKStruct* gACKp = (GWConfigACKStruct*)GW_ReceDataBuf;
			/* 创建启动软件定时器信息 */
			INT8U MboxErr;
			/* 配置状态指针 */
			GW_Result* gStatep;
			
			
			/* 设置 配置状态指针 */
			switch(GWCMD.Visit_Adder)
			{
				/* 设置地址 */
				case GC_SetAdder:
				{
					gStatep = &GWReg.SR.SetAdder;
					break;
				}

				/* 设置信道 */
				case GC_SetChannel:
				{
					gStatep = &GWReg.SR.SetChannel;
					break;
				}
				
				/* 设置数据丢失时间 */
				case GC_SetActiveTime:
				{
					gStatep = &GWReg.SR.SetInvalidTime;
					break;
				}
				
				/* 设置通信波特率 */
				case GC_SetBaudrate:
				{
					gStatep = &GWReg.SR.SetBaudrate;
					break;
				}
				
				/* 设置工作模式 */
				case GC_SetMode:
				{
					gStatep = &GWReg.SR.SetMode;
					break;
				}
				
				default:
				{
					/* 未设定功能 不作处理 */
					/* 不返回接收数据状态，等待超时 */
					__nop();
					return;
				}
			}
			/* 设置数据地址 */
			gACKp->Data = &GW_ReceDataBuf[5];
			/* 判断数据信息 */
			if((gACKp->GW_Adder 	== (GWCMD.GW_Adder))&&
			   (gACKp->Func 			== (GWCMD.Func))&&
			   (gACKp->Reserve1			== (GWCMD.Reserve1))&&
			   (gACKp->Visit_Adder		== (GWCMD.Visit_Adder))&&
			   (gACKp->Reserve2			== (GWCMD.Reserve2)))
			{
				/* 校验数据 */
				uint16_t check;
			
				
				/* 设置数据地址 */
				gACKp->Check = (uint16_t*)&GW_ReceDataBuf[6];
				/* 计算校验 */
				check = BigToLittle_Endian_HalfWord(CRC16_GetValue(GW_ReceDataBuf,(6u)));
				/* 判断校验值是否正常 */
				if(*gACKp->Check == check)
				{
					/* 设置读网关地址失败状态 */
					*gStatep = GW_Success;
				}else
				{
					/* 校验错误 */
					/* 设置读网关地址失败状态 */
					*gStatep = GW_Fail;
				}
			}else
			{
				/* 关闭接收过程，发送接收失败消息 */
				/* 设置读网关地址失败状态 */
				*gStatep = GW_Fail;
			}
			/* 关闭接收过程 */
			GWLog_ReceData_Disable();
			
			/* 发送失败消息 */
			MboxErr = OSMboxPost (GW_ReceStateMboxp, (void*)gStatep);
			/* 判断发送消息状态 */
			if(MboxErr == OS_ERR_NONE)
			{__nop();}
		}else
		{
			/* 数据没有接收完整，继续接收 */
			__nop();
		}
	}
	else if(GWCMD.Func == GW_ReadAdder)
	{
		/* 判断数据是否接收完整 */
		if(GW_ReceDataIndex == (8u))
		{
			/* 数据接收成功 */
			/* 消息应答指针 */
			GWConfigACKStruct* gACKp = (GWConfigACKStruct*)GW_ReceDataBuf;
			/* 创建启动软件定时器信息 */
			INT8U MboxErr;
			
			
			/* 设置数据地址 */
			gACKp->Data = &GW_ReceDataBuf[5];
			/* 判断数据信息 */
			if((gACKp->GW_Adder 	== (0x53))&&
			   (gACKp->Func 			== (0x53))&&
			   (gACKp->Reserve1			== (0x53))&&
			   (gACKp->Visit_Adder		== (0x53))&&
			   (gACKp->Reserve2			== (0x53)))
			{
				/* 校验数据 */
				uint16_t check;
					
				
				/* 设置数据地址 */
				gACKp->Check = (uint16_t*)&GW_ReceDataBuf[6];
				/* 计算校验 */
				check = BigToLittle_Endian_HalfWord(CRC16_GetValue(GW_ReceDataBuf,(6u)));
				/* 判断校验值是否正常 */
				if((*gACKp->Check == check)||(*gACKp->Check == 0xEC15))
				{
					/* 校验正确，设置读网关地址数据 */
					GWReg.DR.Adder = *gACKp->Data;
					/* 设置读网关地址失败状态 */
					GWReg.SR.ReadAdder = GW_Success;
				}else
				{
					/* 校验错误 */
					/* 设置读网关地址失败状态 */
					GWReg.SR.ReadAdder = GW_Fail;
				}
			}
			else
			{
				/* 关闭接收过程，发送接收失败消息 */
				/* 设置读网关地址失败状态 */
				GWReg.SR.ReadAdder = GW_Fail;
			}
			/* 关闭接收过程 */
			GWLog_ReceData_Disable();
			
			/* 发送失败消息 */
			MboxErr = OSMboxPost (GW_ReceStateMboxp, (void*)&GWReg.SR.ReadAdder);
			/* 判断发送消息状态 */
			if(MboxErr == OS_ERR_NONE)
			{__nop();}
		}else
		{
			/* 数据没有接收完整，继续接收 */
			__nop();
		}
	}
}

#endif

#if (1)//重启网关软件定时器回调函数
/* 重启网关软件定时器 句柄指针 */
static OS_TMR* GWLog_Restart_ptmr;
/************************************************************************************************/
/* 函数名	: GWLog_Restart_CallBack										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 重启网关软件定时器回调函数						 									*/
/* 创建日期	: 2016/11/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void GWLog_Restart_CallBack(OS_TMR *ptmr,void *p_arg)
{
	/* 创建启动软件定时器信息 */
	INT8U tmrErr;
	/* 创建启动软件定时器状态 */
	BOOLEAN tmrState;
	
	
	(void) p_arg;

	/* 启动网关电源 */
	DevGW_OpenPower();
	
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

/************************************************************************************************/
/* 函数名	: GWLog_ReceData_Enable										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 启动接收网关应答数据、启动应答超时软件定时器、启动接收数据软件定时器					*/
/* 创建日期	: 2016/11/17																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void GWLog_ReceData_Enable(void)
{
	/* 创建启动软件定时器信息 */
	INT8U tmrErr;
	/* 创建启动软件定时器状态 */
	BOOLEAN tmrState;

	
	/* 使能接收数据状态 */
	GWReg.SR.Run = GW_Busy;
	
	/* 停止超时软件定时器 */
	tmrState = OSTmrStart(GWLog_Overtime_ptmr,&tmrErr);
	/* 判断关闭软件定时器状态 */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* 关闭软件定时器失败 */
		__nop();
	}
	
	/* 停止接收数据定时器 */
	tmrState = OSTmrStart(GWLog_ReceData_ptmr,&tmrErr);
	/* 判断关闭接收数据定时器状态 */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* 关闭软件定时器失败 */
		__nop();
	}

	/* 清除存储寄存器位置指针 */
	GW_ReceDataIndex = 0;
	
	/* 清除存储寄存器 */
	memset(GW_ReceDataBuf,0x00,GW_ReceDataBufSize);
}

/************************************************************************************************/
/* 函数名	: GWLog_ReceData_Disable										 	 			*/
/* 输入  	: ptmr:软件定时器句柄指针,p_arg:回调函数参数(暂时没有)			 						*/
/* 输出		: 无 																				*/
/* 作用	 	: 关闭接收网关应答数据、关闭应答超时软件定时器、关闭接收数据软件定时器					*/
/* 创建日期	: 2016/11/17																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void GWLog_ReceData_Disable(void)
{
	/* 创建启动软件定时器信息 */
	INT8U tmrErr;
	/* 创建启动软件定时器状态 */
	BOOLEAN tmrState;
	/* 创建网关信号量 */
	INT8U gSemErr;
	
	
	/* 结束接收数据 */
	/* 使能接收数据状态 */
	GWReg.SR.Run = GW_Idle;
	
	/* 停止超时软件定时器 */
	tmrState = OSTmrStop(GWLog_Overtime_ptmr,
						 OS_TMR_OPT_NONE,
						 (void*)0,
						 &tmrErr);
	/* 判断关闭软件定时器状态 */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* 关闭软件定时器失败 */
		__nop();
	}
	
	/* 停止接收数据定时器 */
	tmrState = OSTmrStop(GWLog_ReceData_ptmr,
						 OS_TMR_OPT_PERIODIC,
						 (void*)0,
						 &tmrErr);
	/* 判断关闭接收数据定时器状态 */
	if((tmrErr != OS_ERR_NONE)||(tmrState == OS_FALSE))
	{
		/* 关闭软件定时器失败 */
		__nop();
	}
	
	/* 清除存储寄存器位置指针 */
	GW_ReceDataIndex = 0;
	
	/* 清除存储寄存器 */
	memset(GW_ReceDataBuf,0x00,GW_ReceDataBufSize);
	
	/* 申请网关信号量 */
	gSemErr = OSSemPost (LogGW_Sem);
	if(gSemErr != OS_ERR_NONE)
	{__nop();}
}



/************************************************************************************************/
/* 函数名	: GWLog_ReceData										 	 					*/
/* 输入  	: data:数据			 																*/
/* 输出		: 无 																				*/
/* 作用	 	: 接收数据功能						 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void GWLog_ReceData(uint8_t data)
{
	
	/* 判断接收数据状态 */	
	if(GWReg.SR.Run == GW_Idle)
	{
		/* 网关处于空闲状态，不需要接收数据 */
		if(GW_ReceDataIndex)
		{
			/* 清除存储寄存器位置指针 */
			GW_ReceDataIndex = 0;
		}
		return;
	}
	
	/* 将数据写入接收数据存储寄存器 */
	GW_ReceDataBuf[GW_ReceDataIndex] = data;
	/* 存储寄存器位置指针 指向下一个位置 */
	GW_ReceDataIndex = (GW_ReceDataIndex+1)%GW_ReceDataBufSize;
}

/************************************************************************************************/
/* 函数名	: GWLog_Restart										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 重启网关功能					 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_Restart(void)
{
	/* 创建启动软件定时器信息 */
	INT8U tmrErr;
	/* 创建启动软件定时器状态 */
	BOOLEAN tmrState;

	
	/* 关闭网关电源 */
	DevGW_ClosePower();
	
	/* 启动重启定时器（延时1S调用 网关重启回调函数） */
	tmrState = OSTmrStart (GWLog_Restart_ptmr,&tmrErr);
	if((tmrErr != OS_ERR_NONE)||(tmrState != OS_FALSE))
	{__nop();}
	
	return GW_Success;
}

/************************************************************************************************/
/* 函数名	: GWLog_ReadProbe										 	 					*/
/* 输入  	: probeNo:探头编号，probe:探头数据			 										*/
/* 输出		: 无 																				*/
/* 作用	 	: 读取探头数据					 													*/
/* 创建日期	: 2016/11/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_ReadProbe(uint8_t probeNo,GW_ProbeType probetype,float* probe)
{	
	
	/* 判断探头编号是否超出选择范围 */
	if(probeNo == 0)
	{
		/* 未配置的探头ID，返回无数据更新数据。 */
		*probe = GW_ReceTimeoutPara;
		return GW_Fail;
	}
	if(probeNo >= 33)
	{
		/* 超出范围 */
		*probe = GW_ReceTimeoutPara;
		return GW_Fail;
	}
	
	/* 设置获取的数据 */
	*probe = GWReg.DR.Data[(probeNo-1)][probetype];
	return GW_Success;
}

/************************************************************************************************/
/* 函数名	: GWLog_ReadLevel										 	 						*/
/* 输入  	: probeNo:探头编号，probe:探头数据			 										*/
/* 输出		: 无 																				*/
/* 作用	 	: 读取探头数据					 													*/
/* 创建日期	: 2016/11/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_ReadLevel(uint8_t probeNo,GW_Level* level)
{
	/* 判断探头编号是否超出选择范围 */
	if(probeNo == 0)
	{
		/* 未配置的探头ID，返回无数据更新数据。 */
		*level = GW_Level_FALSE;
		return GW_Fail;
	}
	if(probeNo >= 33)
	{
		/* 超出范围 */
		*level = GW_Level_FALSE;
		return GW_Fail;
	}
	
	/* 设置获取的数据 */
	*level = GWReg.DR.Level[(probeNo-1)];
	return GW_Success;
}

/************************************************************************************************/
/* 函数名	: GWLog_UpdataProbeData										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 更新网关数据功能					 													*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/*   注释	: 目前1次读取数据，将32个探头的数据全部读取下来。										*/
/************************************************************************************************/
static GW_Result GWLog_UpdataProbeData(void)
{	
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* 判断信号量状态 */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GW_Fail;
	}
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* 这个判断没什么必要 */
//	/* 判断消息状态 */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* 启动网关接收数据 */
	GWLog_ReceData_Enable();
	
	/* 设置命令参数 */
	GWCMD.GW_Adder 		= (GWReg.CR.Adder);
	GWCMD.Func 		 	= (GW_ReadData);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder 	= (GC_ReadData);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (0x80);//读取32个通道数据
	
	/* 计算校验 */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* 发送读地址命令 */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));

	(void) mbox_Mgs;
	/* 网关应答成功 */
	return GW_Success;
}

/************************************************************************************************/
/* 函数名	: GWLog_UpdataState										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 更新网关状态					 												*/
/* 创建日期	: 2016/11/28																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_UpdataState(void)
{
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* 判断信号量状态 */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GW_Fail;
	}
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* 这个判断没什么必要 */
//	/* 判断消息状态 */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* 启动网关接收数据 */
	GWLog_ReceData_Enable();
	
	/* 设置命令参数 */
	GWCMD.GW_Adder 		= (GWReg.CR.Adder);
	GWCMD.Func 		 	= (GW_ReadData);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder 	= (GC_ReadData);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (0x02);
	
	/* 计算校验 */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* 发送读地址命令 */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* 网关应答成功 */
		return GW_Success;
	}
	
	/* 返回操作结果 */
	return GW_Fail;
}

/************************************************************************************************/
/* 函数名	: GWLog_ReadAdder										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 读网关地址功能					 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_ReadAdder(uint8_t* adder)
{	
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* 判断信号量状态 */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GW_Fail;
	}
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* 这个判断没什么必要 */
//	/* 判断消息状态 */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* 启动网关接收数据 */
	GWLog_ReceData_Enable();
	
	/* 设置命令参数 */
	GWCMD.GW_Adder 		= 0x53;
	GWCMD.Func 		 	= GW_ReadAdder;
	GWCMD.Reserve1 	 	= 0x53;
	GWCMD.Visit_Adder 	= (GC_ReadAdder);
	GWCMD.Reserve2 	 	= 0x53;
	GWCMD.Data 	 	 	= 0x53;
	
	/* 由于这里的值全部为常数，因此校验也是常数，不用计算。 */
	GWCMD.Check 	 	= BigToLittle_Endian_HalfWord(0x15EC);
	
	/* 发送读地址命令 */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* 设置地址数据 */
		*adder = GWReg.DR.Adder;
		/* 网关应答成功 */
		return GW_Success;
	}
	
	/* 返回操作结果 */
	return GW_Fail;
}

/************************************************************************************************/
/* 函数名	: GWLog_SetAdder										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置网关地址功能				 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetAdder(uint8_t adder)
{
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* 判断信号量状态 */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GW_Fail;
	}
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* 这个判断没什么必要 */
//	/* 判断消息状态 */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* 启动网关接收数据 */
	GWLog_ReceData_Enable();
	
	/* 设置命令参数 */
	GWCMD.GW_Adder 	 	= (GWReg.DR.Adder);
	GWCMD.Func 		 	= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetAdder);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (adder);
	
	/* 计算校验 */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* 发送读地址命令 */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* 网关应答成功 */
		/* 设置网关地址为设置地址 */
		GWReg.CR.Adder = GWReg.DR.Adder = adder;
		return GW_Success;
	}
	
	/* 返回操作结果 */
	return GW_Fail;
}

/************************************************************************************************/
/* 函数名	: GWLog_SetChannel										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置信道功能					 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetChannel(uint8_t channel)
{
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* 判断信号量状态 */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GW_Fail;
	}
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* 这个判断没什么必要 */
//	/* 判断消息状态 */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* 启动网关接收数据 */
	GWLog_ReceData_Enable();
	
	/* 设置命令参数 */
	GWCMD.GW_Adder		= (GWReg.CR.Adder);
	GWCMD.Func 		 	= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetChannel);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (channel);
	
	/* 计算校验 */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* 发送读地址命令 */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* 网关应答成功 */
		/* 设置信道数据 */
		GWReg.CR.Channel = channel;
		/* 返回操作成功 */
		return GW_Success;
	}
	
	/* 返回操作结果 */
	return GW_Fail;
}

/************************************************************************************************/
/* 函数名	: GWLog_SetActiveTime										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置数据丢失时间功能					 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetActiveTime(uint8_t time)
{
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* 判断信号量状态 */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GW_Fail;
	}
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* 这个判断没什么必要 */
//	/* 判断消息状态 */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* 启动网关接收数据 */
	GWLog_ReceData_Enable();
	
	/* 设置命令参数 */
	GWCMD.GW_Adder		= (GWReg.CR.Adder);
	GWCMD.Func 		 	= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetActiveTime);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= (time);
	
	/* 计算校验 */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* 发送读地址命令 */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* 网关应答成功 */
		/* 设置数据有效时间 */
		GWReg.CR.ActiveTime = time;
		/* 返回操作成功 */
		return GW_Success;
	}
	
	/* 返回操作结果 */
	return GW_Fail;
}

/************************************************************************************************/
/* 函数名	: GWLog_SetMode										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置模式功能					 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetMode(GW_Mode mode)
{
	/* 消息邮箱  消息指针 */
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	/* 消息邮箱内容的指针 */
	void* mbox_Mgs;
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* 判断信号量状态 */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GW_Fail;
	}
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* 这个判断没什么必要 */
//	/* 判断消息状态 */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* 启动网关接收数据 */
	GWLog_ReceData_Enable();
	
	/* 设置命令参数 */
	GWCMD.GW_Adder		= (GWReg.CR.Adder);
	GWCMD.Func			= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetMode);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	 	= ((uint8_t)mode);
	
	/* 计算校验 */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* 发送读地址命令 */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* 网关应答成功 */
		/* 设置网关模式数据 */
		GWReg.CR.Mode = mode;
		/* 返回操作成功 */
		return GW_Success;
	}
	
	/* 返回操作结果 */
	return GW_Fail;
}

/************************************************************************************************/
/* 函数名	: GWLog_SetBaudrate										 	 				*/
/* 输入  	: baudrate:网关通信波特率			 												*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置波特率功能					 													*/
/* 创建日期	: 2016/11/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_SetBaudrate(uint32_t baudrate)
{
	/* 消息邮箱  消息指针 */
	void* mbox_Mgs;
	/* 信号量错误信息 */
	INT8U mSemErr;
	/* 邮箱错误信息 */
	INT8U mBoxErr;
	
	
	/* 查询网关信号量 */
	OSSemPend (LogGW_Sem,
			  (LogGW_ReceOvertimeValue*10+50),
			  &mSemErr);
	
	/* 判断信号量状态 */	
	if(mSemErr != OS_ERR_NONE)
	{
		/* 没有信号量，返回操作失败。 */
		return GW_Fail;
	}
	
	/* 清除接收数据状态消息（可能之前收到的消息为处理） */
	/* 这里申请一次消息，假如有消息就可以将这个消息申请掉（清除掉）。 */
	mbox_Mgs = OSMboxAccept (GW_ReceStateMboxp);
	
	/* 这个判断没什么必要 */
//	/* 判断消息状态 */
//	if(mbox_Mgs == (void *)0)
//	{__nop();}
	
	/* 启动网关接收数据 */
	GWLog_ReceData_Enable();
	
	/* 设置命令参数 */
	GWCMD.GW_Adder		= (GWReg.CR.Adder);
	GWCMD.Func 		   	= (GW_Config);
	GWCMD.Reserve1 	 	= (0x0);
	GWCMD.Visit_Adder	= (GC_SetBaudrate);
	GWCMD.Reserve2 	 	= (0x0);
	GWCMD.Data 	 	   	= ((baudrate==19200)?1:0);
	
	/* 计算校验 */
	GWCMD.Check = BigToLittle_Endian_HalfWord(CRC16_GetValue((uint8_t*)&GWCMD,(6u)));
	
	/* 发送读地址命令 */
	DevGW_SendData((uint8_t*)&GWCMD,sizeof(GWCMDStruct));
	
	/* 等待接收数据消息 */
	mbox_Mgs = OSMboxPend ((GW_ReceStateMboxp),
						   (LogGW_ReceOvertimeValue*10+50),
						   (&mBoxErr));
	/* 读取消息 */
	if((mBoxErr == OS_ERR_NONE)&&
	   (mbox_Mgs != ((void *)0))&&
	   (*((GW_ReceState*)mbox_Mgs) == GW_ReceSuccess))
	{
		/* 网关应答成功 */
		/* 设置网关通信波特率数据 */
		GWReg.CR.Baudrate = baudrate;
		/* 返回操作成功 */
		return GW_Success;
	}
	
	/* 返回操作结果 */
	return GW_Fail;
}

/************************************************************************************************/
/* 函数名	: GWLog_Init										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化网关功能					 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void GWLog_Init(LogGatewayInitParaStruct initPara)
{
	/* 创建软件定时器结果寄存器 */
	INT8U tmrErr;
	
	
	/* 初始化运行参数 */
	GWReg.CR.Adder 			= initPara.Adder;
	GWReg.CR.Channel 		= initPara.Channel;
	GWReg.CR.ActiveTime 	= initPara.ActiveTime;
	GWReg.CR.Mode 			= initPara.Mode;
	GWReg.CR.Baudrate 		= initPara.Baudrate;
	
	//软件定时器的时钟节拍设置成1ms
	/* 创建接收数据超时软件定时器 */
	GWLog_Overtime_ptmr = 
	OSTmrCreate (
					LogGW_ReceOvertimeValue,		//软件定时器启动后延时多少时间调用回调函数
					LogGW_ReceOvertimeValue,		//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_ONE_SHOT,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)GWLog_Overtime_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"网关接收超时定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);
							
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}	
							
	/* 创建接收状态软件定时器 */
	GWLog_ReceData_ptmr = 
	OSTmrCreate (
					1,								//软件定时器启动后延时多少时间调用回调函数
					1,								//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_PERIODIC,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)GWLog_ReceData_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"网关接收状态定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);					
							
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
	
	/* 创建重启网关软件定时器 */
	GWLog_Restart_ptmr = 
	OSTmrCreate (
					LogGW_RestartTimeValue,			//软件定时器启动后延时多少时间调用回调函数
					LogGW_RestartTimeValue,			//在周期定时模式下，调用回调函数的周期
					OS_TMR_OPT_ONE_SHOT,			//软件定时器模式选择:OS_TMR_OPT_ONE_SHOT->单次启动、OS_TMR_OPT_PERIODIC->周期启动
					(OS_TMR_CALLBACK)GWLog_Restart_CallBack,	//回调函数	格式:void (*OS_TMR_CALLBACK)(void *ptmr, void *parg)
					(void*)0,						//回调函数参数
					"网关重启定时器",				//回调函数名称
					&tmrErr							//创建软件定时器函数执行结果寄存器
				);					
							
	/* 判断软件定时器创建状态 */
	if(tmrErr != OS_ERR_NONE)
	{__nop();/* 失败 */}
	
	
	/* 创建接收数据状态消息 */
	GW_ReceStateMboxp = OSMboxCreate ((void*)0);
	/* 判断接收数据状态消息创建状态 */
	if(GW_ReceStateMboxp == ((OS_EVENT *)0))
	{__nop();/* 失败 */}
	
	/* 创建网关信号量 */
	LogGW_Sem = OSSemCreate (1u);
	
	/* 初始化网关 */
	DevGW_Init(GWReg.CR.Baudrate,GWLog_ReceData);
	
	/* 等待网关启动（电源稳定） */
	OSTimeDly(10);
}

/************************************************************************************************/
/* 函数名	: GWLog_ConfigFunc										 	 							*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置网关功能					 													*/
/* 创建日期	: 2016/11/28																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static GW_Result GWLog_ConfigFunc(void)
{

	/* 功能操作状态 */
	GW_Result gState;
	/* 读网关地址 */
	uint8_t gAdder;
	/* 通信波特率设置状态 */
	GW_Result gSetBaudrateState = GW_Fail;
	
	
	/* 读网关状态 */
	GWReg.SR.Work = GWLog_ReadAdder(&gAdder);
	/* 判断网关状态是否正常 */
	if(GWReg.SR.Work == GW_Fail)
	{
		/* 判断当前的波特率 */
		if(GWReg.CR.Baudrate == (115200ul))
		{
			/* 初始化网关 */
			DevGW_Init((19200ul),GWLog_ReceData);
		}else if(GWReg.CR.Baudrate == (19200ul))
		{
			/* 初始化网关 */
			DevGW_Init((115200ul),GWLog_ReceData);
		}else
		{return GW_Fail;}
		
		/* 等待网关启动（电源稳定） */
		OSTimeDly(10);
		
		/* 读网关状态 */
		GWReg.SR.Work = GWLog_ReadAdder(&gAdder);
		
		/* 判断网关状态是否正常 */
		if(GWReg.SR.Work == GW_Fail)
		{
			/* 模块通信失败(应该是没有连接网关) */
			/* 初始化32位变送器通信波特率 */
			DevGW_Init(GWReg.CR.Baudrate,GWLog_ReceData);
			/* 重启网关 */
			GWLog_Restart();
			return GW_Fail;
		}
	}else
	{
		/* 设置通信波特率状态 */
		gSetBaudrateState = GW_Success;
	}
	
	/* 判断读取的网关地址与默认的地址是否相同 */
	if(gAdder != GWReg.CR.Adder)
	{
		/* 不相同，设置默认的地址为当前网关地址 */
		gState = GWLog_SetAdder(GWReg.CR.Adder);
		/* 判断网关状态是否正常 */
		if(gState == GW_Fail)
		{__nop();}
	}
	
	/* 设置信道(波段) */
	gState = GWLog_SetChannel(GWReg.CR.Channel);
	/* 判断网关状态是否正常 */
	if(gState == GW_Fail)
	{__nop();}
	
	/* 设置数据丢失时间 */
	gState = GWLog_SetActiveTime(GWReg.CR.ActiveTime);
	/* 判断网关状态是否正常 */
	if(gState == GW_Fail)
	{__nop();}
	
	/* 设置工作模式 */
	gState = GWLog_SetMode(GWReg.CR.Mode);
	/* 判断网关状态是否正常 */
	if(gState == GW_Fail)
	{__nop();}
	
	/* 判断通信波特率是否需要需改 */
	if(gSetBaudrateState == GW_Fail)
	{
		/* 模块通信成功(模块的初始波特率与设置的波特率不相同) */
		/* 设置模块通信波特率 */
		GWLog_SetBaudrate(GWReg.CR.Baudrate);
		/* 初始化32位变送器通信波特率 */
		DevGW_Init(GWReg.CR.Baudrate,GWLog_ReceData);
	}
	
	/* 重启网关 */
	GWLog_Restart();

	return GW_Success;
}

/************************************************************************************************/
/* 函数名	: BSP_LogGW_API_Init										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化逻辑层API函数				 												*/
/* 创建日期	: 2016/11/15																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void BSP_LogGW_API_Init(void)
{
	/* 设置系统接口函数 */
	
	/* 初始化接口 */
	LogGW_API.Init 			= GWLog_Init;
	/* 重启网关接口 */
	LogGW_API.Restart 		= GWLog_Restart;
	/* 测试网关功能接口 */
	LogGW_API.ConfigFunc 	= GWLog_ConfigFunc;
	/* 读探头数据接口 */
	LogGW_API.ReadProbe 	= GWLog_ReadProbe;
	/* 读探头数据接口 */
	LogGW_API.ReadLevel 	= GWLog_ReadLevel;
	/* 更新网关数据 */
	LogGW_API.UpdataProbeData 	= GWLog_UpdataProbeData;
	/* 读网关地址 */
	LogGW_API.ReadAdder 	= GWLog_ReadAdder;
	/* 读网关状态 */
	LogGW_API.UpdataState 	= GWLog_UpdataState;
	/* 设置网关接口 */
	LogGW_API.SetAdder	 	= GWLog_SetAdder;
	/* 设置网关信道(波段) */
	LogGW_API.SetChannel 	= GWLog_SetChannel;
	/* 设置网关数据有效时间 */
	LogGW_API.SetActiveTime = GWLog_SetActiveTime;
	/* 设置网关数据有效时间 */
	LogGW_API.SetMode 		= GWLog_SetMode;
	/* 设置网关数据有效时间 */
	LogGW_API.SetBaudrate 	= GWLog_SetBaudrate;
}




#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
