/************************************************************************************************/
/** @file              : Log_ConfigService_Http.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年09月20日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--配置服务器(Http)应用层文件						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年09月20日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_ConfigService_Http.h"

#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x01)//Http协议


#include "Dev_Fram.h"
#include "Dev_RTC.h"
#include "Log_HTTP.h"
#include "Log_IAP.h"
#include "app.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>



/* 数据寄存器 */
/* 发送数据寄存器 */
static uint8_t ConfigService_TXDR[1536];
/* 接收数据寄存器 */
static uint8_t ConfigService_RXDR[1500];



#if ((AppGPRSComProtocolEnable == 0x01)&&(AppGPRSComProtocol == 0x01))
extern CreateHTTPParaStruct HTTPPara;
#endif	
/* 配置服务器应用参数 */
ConfigService_AppDef ConfigService_App = 
{
	FALSE,
	FALSE,
};

/************************************************************************************************/
/* 函数名	: ConfigService_StopRun												 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无		 																		*/
/* 作用	 	: 停止配置								 											*/
/* 创建日期 	: 2016/08/30																		*/
/* 修改日期 	: 2016/08/30																		*/
/*   版本 	: Ver:0																				*/
/************************************************************************************************/
void ConfigService_StopRun(void)
{
	/* 停止配置 */
	ConfigService_App.Run_State = FALSE;
}

/************************************************************************************************/
/* 函数名	: ConfigService_StartRun												 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无		 																		*/
/* 作用	 	: 启动配置								 											*/
/* 创建日期 	: 2016/08/30																		*/
/* 修改日期 	: 2016/08/30																		*/
/*   版本 	: Ver:0																				*/
/************************************************************************************************/
void ConfigService_StartRun(void)
{
	/* 停止配置 */
	ConfigService_App.Run_State = TRUE;
}


/************************************************************************************************/
/* 函数名	: ConfigService_TXLog															 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置服务器发送数据业务流程								 							*/
/* 创建日期 	: 2016/08/11																		*/
/* 修改日期 	: 2016/08/16																		*/
/*   版本 	: V0: 创建“数据业务”逻辑															*/
/*   版本 	: V1: 将原来的“数据业务”改为现在的“发送数据业务”									*/
/************************************************************************************************/
void ConfigService_TXLog(INSTRU_CONFIG_INF_TAB* scrPara)
{
	
	/* 临时时间缓存 */
	Times time;
	/* 网关编号指针 */
	SERIAL_NUMBER* gatewayIdNoP = &scrPara->Inf_State.Instru_System.Inf.Serial_Number;
	
	
	/* 判断是否正在进行配置服务器操作 */
	if(ConfigService_App.Run_State == TRUE)
	{
		/* 正在进行配置服务器操作，不再发送其它操作命令。 */
		return;
	}
	
	/* 设置配置服务器运行状态 */
	ConfigService_StartRun();
	
	/* 配置上传状态参数 */
	/* 清除数据接口参数 */
	memset((char*)&HTTPPara.ApiState,0x00,sizeof(HTTPPara.ApiState));
	
	/* 设置网关编号 */
	sprintf(HTTPPara.ApiState.GatewayIdNo,"%04X%05d",
	((((*(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0x00ff) << 8)),
	((((*(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0x00ff) << 8)));
	
	/* 设置上传时间戳 */
	time.Year 	= scrPara->Inf_State.Instru_RTC.Inf_State.Year;
	time.Month	= scrPara->Inf_State.Instru_RTC.Inf_State.Month;
	time.Day 	= scrPara->Inf_State.Instru_RTC.Inf_State.Day;
	time.Hour 	= scrPara->Inf_State.Instru_RTC.Inf_State.Hour;
	time.Min 	= scrPara->Inf_State.Instru_RTC.Inf_State.Min;
	time.Sec 	= scrPara->Inf_State.Instru_RTC.Inf_State.Sec;
	/* 将参数的采集时间转换成秒，将这个数据设置为参数签名的时间数据。 */
	HTTPPara.ApiState.TimeStamp = RTC_Date_Time_To_Second(&time);
	
	/* 清除参数签名数据缓存。 */
	memset(HTTPPara.ApiState.Sign,0x00,sizeof(HTTPPara.ApiState.Sign));
	
	/* 设置电源状态 */
	HTTPPara.ApiState.PowerState = scrPara->Inf_State.Instru_Power.Inf_State.ET_Link_State;
	
	/* 设置电池电量 */
	HTTPPara.ApiState.BatteryLevel = scrPara->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	
	/* 创建状态数据。 */
	HTTP_Create_State((char*)ConfigService_TXDR);
	
	/* 设置GPRS发送数据参数 */
	GPRS.ConfigService.Para.TXDR	= ConfigService_TXDR;
	GPRS.ConfigService.Para.TXLen	= strlen((char*)ConfigService_TXDR);
	GPRS.ConfigService.Para.RXDR	= ConfigService_RXDR;
	GPRS.ConfigService.Para.RXLen	= 1500;
	
	/* 发送队列(ConfigService) */
	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,ConfigService_EVENT);

	/* 设置配置服务器运行状态 */
	ConfigService_StartRun();
}

/************************************************************************************************/
/* 函数名	: ConfigService_ParseData															*/
/* 输入  	: 无								 												*/
/* 输出		: 无		 																		*/
/* 作用	 	: 配置服务器解析数据								 									*/
/* 创建日期 	: 2016/08/16																		*/
/* 修改日期 	: 2016/08/16																		*/
/*   版本 	: V0: 创建																			*/
/************************************************************************************************/
static BOOL ConfigService_ParseData(uint8_t* string)
{
	
	/* 数据指针 */
	char * datap;
	
	/* 查询HTTP传输成功字符串 */
	datap = strstr((char*)string,"HTTP/1.1 200 OK");
	/* 判断HTTP传输是否成功 */
	if(NULL == datap)
	{
		/* 传输失败 */
		return FALSE;
	}
	
	/* 查询数据平台成功接收字符串 */
	datap = strstr((char*)string,"\"code\":\"0000\"");
	/* 判断数据平台是否成功接收数据 */
	if(NULL == datap)
	{
		/* 传输失败 */
		return FALSE;
	}

	return TRUE;
}

/************************************************************************************************/
/* 函数名	: ConfigService_RXLog												 				*/
/* 输入  	: gprs:GPRS运行参数.			 														*/
/* 输出		: 无		 																		*/
/* 作用	 	: 配置服务器接收数据业务流程								 							*/
/* 创建日期 	: 2016/08/16																		*/
/* 修改日期 	: 2016/08/16																		*/
/*   版本 	: V0: 创建																			*/
/************************************************************************************************/
void ConfigService_RXLog(GPRS_Struct gprs,INSTRU_CONFIG_INF_TAB* scrPara)
{
	/* 解析状态 */
	BOOL parseState = FALSE;
	
	/* 判断接收数据状态 */
	if(gprs.ConfigService.State.Result == TRUE)
	{
		/* 接收数据成功 */
		
		/* 解析数据 */
		parseState = ConfigService_ParseData(gprs.ConfigService.Para.RXDR);
		
		/* 判断数据解析状态 */
		if(parseState == TRUE)
		{
			/* 数据解析成功 */
			__nop();
		}else
		{
			/* 数据解析失败 */
			__nop();
		}
	}else
	{
		/* 通信失败 */
		__nop();
	}

	/* 清除配置服务器正在运行状态标志 */
	ConfigService_StopRun();
}

#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
