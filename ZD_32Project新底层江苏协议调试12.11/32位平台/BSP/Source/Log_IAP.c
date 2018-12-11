/************************************************************************************************/
/** @file              : Log_IAP.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年08月24日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--升级固件应用层文件								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年08月24日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_IAP.h"
#include "Mcu_Config.h"
#include "Dev_Fram.h"
#include "stdio.h"
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)
#include "Mcu_Flash.h"
#include "app.h"
#include <string.h>
/* 固件升级参数源 */
ConfigService_Para_Update* IAP_ParaSrc;

/* 固件升级每次向升级服务器请求数据字节数 */
#define UploadUpdataBytes	       (0x800)     //每次升级请求2KBytes的数据

/* 升级状态信息信息*/
#define ConfigService_UploadUpdataStatus	       (0x08)
extern void ConfigService_Cmd(uint8_t item,uint8_t type);

/* 数据寄存器 */
/* 发送数据寄存器 数据区大小 */
#define IAP_TXDR_Size		(3000u)
/* 接收数据寄存器 数据区大小 */
#define IAP_RXDR_Size		(3000u)
/* 发送数据寄存器 */
static uint8_t IAP_TXDR[IAP_TXDR_Size];
/* 接收数据寄存器 */
static uint8_t IAP_RXDR[IAP_RXDR_Size];

/* 固件升级通信协议结构 */
typedef struct
{
	//道路运输车辆通讯协 协议采用大端模式(big-endian)的网络字节序来传递字和双字。
	//这个数据结构只能适应2通道的传感器GPRS数据发送，并且不能传输状态信息。
	/* 头 标识位 */ //(1 Byte)
	uint8_t			Start_Iden;
	/* 通信 消息头 */
	/* 消息 ID(命令) *///(2 Byte)
	uint8_t			ID_Type_HSB;
	uint8_t			ID_Type_LSB;
	/* 消息体 属性 *///(2 Byte)
	//消息体属性 消息体长度
	uint8_t	 		Attribute_HSB_Length:2;		
	//消息体属性 数据加密方式
	uint8_t	 		Attribute_HSB_Enc_Mode:3;
	//消息体属性 分包
	uint8_t	 		Attribute_HSB_Sub_Flag:1;		
	//消息体属性 保留
	uint8_t 		Attribute_HSB_Reserve:2;
	//消息体属性 消息体长度
	uint8_t	 		Attribute_LSB_Length:8;
	/* 终端序列号(SN号): *///(6 Byte)
	uint8_t			SN_NO[6];
	/* 消息 流水号 *///(2 Byte)
	uint8_t			Serial_NO_HSB;
	uint8_t			Serial_NO_LSB;

	/* 通信 消息体 */
	/* 消息类型:0XFA(固件升级) *///(1 Byte)
	uint8_t			Body_Type;
	/* 消息体数据 */
	uint8_t			Data;
}
IAPComStruct;

/* 固件升级通信协议默认配置 */
static const IAPComStruct  IAPComDefault = 
{
	/* 头 标识位 */
	0X7E,
/****************************************************/
	/* 通信 消息头 */
	/* 消息 ID(命令) */
	0X09,
	0X00,
	/* 消息体 属性 */
	/* 消息体属性 消息体长度 */
	0X00,	
	/* 消息体属性 数据加密方式 */
	0X00,
	/* 消息体属性 分包 */
	0X00,		
	/* 消息体属性 保留 */
	0X00,
	/* 消息体属性 消息体长度 */
	0X00,
	/* 终端序列号(SN号): */
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* 消息 流水号 */
	0X00,
	0X00,
/****************************************************/
	/* 通信 消息体 */
	/* 消息类型: */
	0XFA,

};
/****************************************************/
/* 固件升级上传数据结构 */
typedef struct
{
	/* 文件名称 */
	char DocumentName[32];
	/* 包序号 */
	uint32_t Packet;
	/* 包大小 */
	uint32_t Size;
	
}
IAP_UploadStruct;

/* 默认固件升级上传数据 */
static const IAP_UploadStruct IAP_UploadDefault = 
{
	/* 文件名称 */
	'\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0', 
	'\0','\0','\0','\0', '\0','\0','\0','\0', '\0','\0','\0','\0',
	'\0','\0','\0','\0', '\0','\0','\0','\0', 
	/* 包序号 */
	1,
	/* 包大小 */
	0x4B0,
};
/****************************************************/
/* 固件升级 文件参数结构 */
typedef struct
{
	/* 包总数 */
	uint32_t Total;
	/* 包序号 */
	uint32_t Num;
	/* 包大小 */
	uint32_t Size;
	/* 数据 */
	uint8_t	 Data;
}
IAP_UpdateFileParaStruct;
/* 固件升级文件参数 */
//static IAP_UpdateFileParaStruct IAP_UpdateFilePara;

/****************************************************/
/* 固件升级参数寄存器 */
static IAP_UploadStruct IAP_UploadParaCR;

///* 固件升级应用参数结构 */
//typedef struct
//{
//	/* 操作状态 */
//	BOOL RunState;
//	/* 升级标志 */
//	uint8_t* UpdateFlag;
//	/* 升级状态 */
//	BOOL UpdateState;
//}
//IAP_AppStruct;

/* 固件升级应用参数 */
IAP_AppStruct IAP_App = 
{
	FALSE,
	(uint8_t*)0,
	FALSE,
     0,	
};

/****************************************************/


#define htonl(A)     ((((u32)(A) & 0xff000000) >> 24) | \
(((u32)(A) & 0x00ff0000) >> 8) | \
(((u32)(A) & 0x0000ff00) << 8) | \
(((u32)(A) & 0x000000ff) << 24))
/************************************************************************************************/
/* 函数名	: IAP_SetUploadPara																		*/
/* 输入  	: documentname:文件名字符串，packet:包序号，size:包大小			 					*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置上传数据参数								 									*/
/* 创建日期 	: 2016/08/25																		*/
/* 修改日期 	: 2016/08/25																		*/
/************************************************************************************************/
static void IAP_SetUploadPara(char* documentname,uint32_t packet,uint32_t size)
{
	/* 文件名长度 */
	uint32_t documentnamelen;

	
	/* 初始化固件升级寄存器 */
	IAP_UploadParaCR = IAP_UploadDefault;
	
	/* 计算文件名长度 */
	documentnamelen = strlen(documentname);
	
	if(documentnamelen > sizeof(IAP_UploadParaCR.DocumentName))
	{
		documentnamelen = sizeof(IAP_UploadParaCR.DocumentName);
	}
	
	/* 设置升级文件名 */
	memcpy(IAP_UploadParaCR.DocumentName,documentname,documentnamelen);
	
	/* 将小段格式转换为大端格式 */
	packet = htonl(packet);
	/* 包序号 */
	IAP_UploadParaCR.Packet = packet;
	/* 将小段格式转换为大端格式 */
	size = htonl(size);
	/* 包大小 */
	IAP_UploadParaCR.Size = size;

}

/************************************************************************************************/
/* 函数名	: IAP_ParseData																		*/
/* 输入  	: data:数据，len:数据长度			 												*/
/* 输出		: 数据解析状态 																		*/
/* 作用	 	: 解析接收到的数据								 									*/
/* 创建日期 	: 2016/08/25																		*/
/* 修改日期 	: 2016/08/25																		*/
/************************************************************************************************/
static BOOL IAP_ParseData(uint8_t* data,uint32_t* len)
{
	/* 消息头指针 */
	IAPComStruct * iapcomparap;
	
	/* 校验数据 */
	uint8_t  check = 0;
	/* 计数器 */
	uint32_t num;
	
	
	/* 先将发送数据区清空(节省数据空间) */
	memset(IAP_TXDR,0X00, sizeof(IAP_TXDR));
	
	/* 1、反向转义数据 */
	/* 将转义后的数据存到数据区 */
	*len = GPRS_Network_To_Host_Translator_Data(IAP_TXDR,data,*len);
	/* 清空接收数据区(节省数据空间) */
	memset(data,0X00,*len);
	/* 将发送区的数据转移到接收数据区 */
	memcpy(data,IAP_TXDR,*len);
	
	/* 2、判断数据合法性 */
	/* 设置数据指针 */
	iapcomparap = (IAPComStruct *)IAP_TXDR;
	
	/* a、比较SN号是否一致 */
	if(memcmp(iapcomparap->SN_NO,
			 (uint8_t*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),
			  6) != 0)
	{
		/* SN号不一致 */
		return FALSE;
	}
	
	/* b、计算校验 */
	for(num = 1 ; num < (*len - 2); num++)
	{
		/* 从 消息头 到 消息体 最后一个 数据 异或 */
		check ^= IAP_TXDR[num];
	}
	
	/* 判断校验是否相同 */
	if(check != IAP_TXDR[*len - 2])
	{
		/* 校验不相同 */
		return FALSE;
	}
	
	/* c、消息体类型 */
	if(iapcomparap->Body_Type != 0xFA)
	{
		/* 消息体类型不是配置服务器类型 */
		return FALSE;
	}

	/* 数据解析完成 */
	return TRUE;
}

/************************************************************************************************/
/* 函数名	: IAP_Cmd																		 	*/
/* 输入  	: para:固件升级参数			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 发送心跳状态指令								 									*/
/* 创建日期 	: 2016/08/24																		*/
/* 修改日期 	: 2016/08/24																		*/
/************************************************************************************************/
static void IAP_Cmd(IAP_UploadStruct* para)
{
	/* 通信协议指针 */
	IAPComStruct* iapcomp = (IAPComStruct*)IAP_TXDR;
	/* 消息体指针 */
	IAP_UploadStruct * iapcombodyp;
	/* 数据长度 */
	uint32_t len;
	/* 校验位指针 */
	uint8_t * check;
	/* 尾 标识位 */
	uint8_t * end_Iden;
	/* 计数器 */
	uint32_t num;
	
	
	/* 清除发送数据区 */
	memset(IAP_TXDR,0x00,sizeof(IAP_TXDR));
	
	/* 设置通信消息头 */
	*iapcomp = IAPComDefault;
	
	/* 设置SN号 */
	memcpy(iapcomp->SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);
	
	/* 设置消息体指针 */
	iapcombodyp = ((IAP_UploadStruct *)&iapcomp->Data);
	
	/* 设置消息体内容 */
	*iapcombodyp = *para;
	
	/* 设置数据结束地址 */
	end_Iden = (((uint8_t*)(&iapcombodyp->Size+1)));
	
	/* 计算数据长度 */
	len = ((uint32_t)(end_Iden - ((uint8_t*)&iapcomp->Body_Type)));
	
	/* 计算消息体长度 */
	iapcomp->Attribute_HSB_Length = (len>>8);
	iapcomp->Attribute_LSB_Length = (len%256);
	
	/* 设置校验位指针 */
	check = end_Iden;
	
	/* 设置数据长度 */
	len = len + 13 - 1 ;//(14是通信协议固定数据长度-1头标识位)
	
	/* 计算校验码 */
	for(num = 1 ; num <= len ; num++)
	{
		/* 从 消息头 到 消息体 最后一个 数据 异或 */
		*check ^= IAP_TXDR[num];
	}
	
	/* 设置数据长度 */
	len = len + 1;//(+校验)
	
	//1、转义 数据
	/* 转义 *//* 获得GPRS发送数据长度 */
	num = GPRS_Host_To_Network_Translator_Data(
		 &IAP_RXDR[1],&IAP_TXDR[1],len);
	
	/* 拷贝转移完成数据 */
	memcpy(&IAP_TXDR[1],&IAP_RXDR[1],num);
	
	/* 清除接收数据寄存器 */
	memset(IAP_RXDR,0x00,sizeof(IAP_RXDR));
	
	/* 设置数据长度 */
	len = num+1;//+1头标识位
	
	/* 设置尾标识位指针 */
	end_Iden = &IAP_TXDR[len];
	/* 设置尾标识 */
	*end_Iden = 0X7E;

	/* 设置GPRS发送数据参数 */
	GPRS.IAP.Para.TXDR	= IAP_TXDR;
	GPRS.IAP.Para.TXLen	= len+1;//(加上1字节结束符长度)
	GPRS.IAP.Para.RXDR	= IAP_RXDR;
	GPRS.IAP.Para.RXLen	= IAP_RXDR_Size;

	/* 发送升级事件 */	
	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,IAP_EVENT);
	
	
}
/************************************************************************************************/
/* 函数名	: IAP_UpdateProcess																	*/
/* 输入  	: data:数据，len:数据长度			 												*/
/* 输出		: 无 																				*/
/* 作用	 	: 升级程序流程								 										*/
/* 创建日期 	: 2016/08/25																		*/
/* 修改日期 	: 2016/08/25																		*/
/************************************************************************************************/
static void IAP_UpdateProcess(uint8_t* data,uint32_t len)
{
	/* 消息头指针 */
	IAPComStruct * iapcomparap;
	/* 消息体内容 */
	IAP_UpdateFileParaStruct* iapcomfilep;
	IAP_UpdateFileParaStruct  iapcomfile;
	/* 数据指针 */
	uint8_t* updatefiledata;
	/*记录获取升级内容失败的次数;当获取失败后会发起重新获取命令，最多连接3次*/
static 	uint8_t  GetUpdateContentErrors=0;
	/* 写固件文件数据状态 */
	BOOL download;
	/* 解析数据 */ 
	if(IAP_ParseData(data,&len)==FALSE)
	{

		if(GetUpdateContentErrors<=20)
		{
			GetUpdateContentErrors++;
		/*获取升级内容失败 设置重新获取 升级文件信息 参数 */
			IAP_Cmd(&IAP_UploadParaCR);
		}else//当超过20次解析升级服务器内容失败,将升级服务器的运行状态设置为未在执行升级任务状态,若配置服务器那边未进行取消升级操作的话，设备会在此进行升级操作
		{
			IAP_App.RunState   = FALSE;
			GetUpdateContentErrors=0;
		}
		return;
	}

	/* 设置消息头指针 */
	iapcomparap = (IAPComStruct *)data;
	
	/* 设置操作内容指针 */
	iapcomfilep = (IAP_UpdateFileParaStruct*)(&iapcomparap->Data);
	
	/* 调整升级文件参数大小端 */
	iapcomfile.Total = htonl(iapcomfilep->Total);
	iapcomfile.Num 	 = htonl(iapcomfilep->Num);
	iapcomfile.Size  = htonl(iapcomfilep->Size);
	
	
	/*计算升级进度百分比*/
	IAP_App.UpdatePercent=(u8)((((float)iapcomfile.Num)/(iapcomfile.Total))*100);
	
	
	/* 设置数据指针 */
	updatefiledata = &iapcomfilep->Data;

	/* 判断包序号 */
	if(iapcomfilep->Num > 0)
	{
		/* 将数据写入Mcu中(Flash) */
		download = Write_Data((iapcomfile.Num-1)*UploadUpdataBytes,updatefiledata,iapcomfile.Size);
	}else
	{
		download = FALSE;
	}
	
	/* 判断数据写入状态 */
	if(TRUE == download)
	{
		/* 数据写入成功 */
		/* 判断当前数据包序号 */
		if(iapcomfile.Num == iapcomfile.Total)
		{
			/* 固件升级标志 */
			uint8_t updateflag = TRUE;		
			/* 当前为最后一包数据，设置固件升级成功状态. */
			IAP_App.UpdateState = TRUE;
			/* 设置程序更新成功标志(提供给Bootloader识别). */
			Write_Para(ENV_APP_UPDATED_START,&updateflag,ENV_APP_UPDATED_SIZE);
			/* 删除运行状态 */
			IAP_App.RunState = FALSE;
			/* 删除升级标志 */
			*IAP_App.UpdateFlag = FALSE;
			
	     OSTimeDlyHMSM(0,0,2,0);//延时2秒钟告诉平台程序升级成功
			ConfigService_Cmd(ConfigService_UploadUpdataStatus,0x01);			/* 告诉配置平台升级完成*/

			printf("升级数据接收完毕......\r\n发送告诉配置服务器设备已经完成升级数据的接收消息......\r\n");

				
		}else
		{
		  printf("已获取到第%d包升级数据\n",iapcomfile.Num);	

			/* 未完成固件升级，继续请求下一包数据 */
			IAP_SetUploadPara(IAP_ParaSrc->DocumentName,(iapcomfile.Num+1),UploadUpdataBytes);
			/* 设置 获取 升级文件信息 参数 */
			IAP_Cmd(&IAP_UploadParaCR);
		}
	}else//若当前数据包写入失败,则进行在此请求该包数据
	{
		/* 未完成固件升级，继续请求下一包数据 */
			IAP_SetUploadPara(IAP_ParaSrc->DocumentName,(iapcomfile.Num),UploadUpdataBytes);
			/* 设置 获取 升级文件信息 参数 */
			IAP_Cmd(&IAP_UploadParaCR);
		//到末前为止尚未发现写数据失败的情况。
		/* 结束升级 */
	}
	
}

/************************************************************************************************/
/* 函数名	: IAP_UploadLog																		*/
/* 输入  	: documentname:文件名字符串，packet:包序号，size:包大小			 					*/
/* 输出		: 无 																				*/
/* 作用	 	: 设置上传数据参数								 									*/
/* 创建日期 	: 2016/08/25																		*/
/* 修改日期 	: 2016/08/25																		*/
/************************************************************************************************/
void IAP_UploadLog(GPRS_Struct* gprs)
{

//	/* 判断升级标志 */
	if(FALSE == *IAP_App.UpdateFlag)
	{
		/* 停止升级 */
		IAP_App.RunState = FALSE;
		return;
	}	
	/* 判断通信状态 */
	if(TRUE == gprs->IAP.State.Result)
	{
		/* 通信成功 */
		/* 已经设置升级文件参数，开始升级 */
		IAP_UpdateProcess(gprs->IAP.Para.RXDR,gprs->IAP.Para.RXLen);
	}else
	{
		/* 通信失败，重新获取当前数据包。 */
		IAP_Cmd(&IAP_UploadParaCR);
	}

}

/************************************************************************************************/
/* 函数名	: IAP_StopRun																	*/
/* 输入  	: 无		 																		*/
/* 输出		: 无 																				*/
/* 作用	 	: 开始升级								 									*/
/* 创建日期 	: 2016/08/30																		*/
/* 修改日期 	: 2016/08/30																		*/
/************************************************************************************************/
void IAP_StartRun(void)
{
	/* 设置正在升级状态 */
	IAP_App.RunState = TRUE;
}

/************************************************************************************************/
/* 函数名	: IAP_StopRun																	*/
/* 输入  	: 无		 																		*/
/* 输出		: 无 																				*/
/* 作用	 	: 停止升级								 									*/
/* 创建日期 	: 2016/08/30																		*/
/* 修改日期 	: 2016/08/30																		*/
/************************************************************************************************/
void IAP_StopRun(void)
{
	/* 设置正在升级状态 */
	IAP_App.RunState = FALSE;
}

/************************************************************************************************/
/* 函数名	: IAP_CheckFlag																		 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 固件升级标志查询								 									*/
/* 创建日期 	: 2016/08/24																		*/
/* 修改日期 	: 2016/08/24																		*/
/************************************************************************************************/
extern ConfigService_AppDef ConfigService_App;
void IAP_CheckFlag(void)
{
	/* 查询固件升级标志 */
	if((*IAP_App.UpdateFlag == TRUE)&&(IAP_App.RunState == FALSE))
	{	
		   OSTaskSuspend(APP_TASK_Save_PRIO);//关闭存储任务
		   OSTaskSuspend(APP_TASK_Key_PRIO);//关闭按键任务AppTaskADC
			 OSTaskSuspend(APP_TASK_ADC_PRIO);//关闭ADC任务采样
		   OSTaskSuspend(APP_TASK_COM_PRIO);//关闭配置工具任务               
		   OSTaskSuspend(APP_TASK_Beep_PRIO);//关闭蜂鸣器任务                 
		   OSTaskSuspend(APP_TASK_Light_PRIO);//关闭LED报警灯任务                  
	                     
		
			 printf("已经将升级文件的信息复制到升级信息数据区\n开始请求升级数据包\n");

		
		
		
		
		
		
		
		/* 开始升级 */
		IAP_StartRun();
		/* 清除固件升级备份数据 */
		Erase_Para();
		/* 设置固件升级参数 */
		IAP_SetUploadPara(IAP_ParaSrc->DocumentName,1,UploadUpdataBytes);
		/* 设置 获取 升级文件信息 参数 */
		IAP_Cmd(&IAP_UploadParaCR);
	}
}

/************************************************************************************************/
/* 函数名	: IAP_ParaInit																		*/
/* 输入  	: updateparap:固件升级参数指针		 												*/
/* 输出		: 无 																				*/
/* 作用	 	: 固件升级参数初始化								 									*/
/* 创建日期 	: 2016/08/28																		*/
/* 修改日期 	: 2016/08/28																		*/
/************************************************************************************************/
void IAP_ParaInit(ConfigService_Para_Update* updateparap,uint8_t* updateflag)
{
	
	/* 初始化固件升级参数指针 */
	IAP_ParaSrc = updateparap;
	/* 初始化固件升级标志指针 */
	IAP_App.UpdateFlag = updateflag;
}

/************************************************************************************************/
/* 函数名	: IAP_UpdateSatte																	*/
/* 输入  	: 无					 															*/
/* 输出		: 固件升级状态 																		*/
/* 作用	 	: 获取固件升级状态								 									*/
/* 创建日期 	: 2016/08/28																		*/
/* 修改日期 	: 2016/08/28																		*/
/************************************************************************************************/
BOOL IAP_UpdateSatte(void)
{
	/* 返回当前固件升级状态 */
	return IAP_App.UpdateState;
}
/************************************************************************************************/
/**	函 数 名:mcu_reset															*/
/**	功能说明: 重启设备  复位单片机																	*/
/**	形    参：无																	*/
/**	返 回 值: 无																					*/
/************************************************************************************************/	
void mcu_reset(void)//重启设备  复位单片机
{
	  __set_FAULTMASK(1);//关闭总中断
    NVIC_SystemReset();//请求单片机重启
}
#endif
#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
