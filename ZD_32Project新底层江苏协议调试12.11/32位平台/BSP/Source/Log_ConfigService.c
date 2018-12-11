/************************************************************************************************/
/** @file              : Log_ConfigService.c													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年01月07日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--配置服务器应用层文件							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年08月02日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_ConfigService.h"
/****************新增头文件************************/
#include "GSM_app.h"
#include "Gsm_Interface.h"
/************************************************************************************************/
/*	这里n表示network，h表示host，																	*/
/*	htons( ) htonl( )用于本地字节向网络字节转换的场合，												*/
/*	s表示short，即对2字节操作，																	*/
/*	l表示long,  即对4字节操作。																	*/
/*	同样ntohs( )ntohl( )用于网络字节向本地格式转换的场合。 											*/
/************************************************************************************************/
#define htons(A)     ((((u16)(A) & 0xff00) >> 8) | \
(((u16)(A) & 0x00ff) << 8))
#define htonl(A)     ((((u32)(A) & 0xff000000) >> 24) | \
(((u32)(A) & 0x00ff0000) >> 8) | \
(((u32)(A) & 0x0000ff00) << 8) | \
(((u32)(A) & 0x000000ff) << 24))

#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)//部标协议
#include "Dev_Fram.h"
#include "Dev_RTC.h"
#include "Log_HTTP.h"
#include "Log_IAP.h"
#include "app.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* 配置服务器通信协议结构 */
typedef struct
{
	//道路运输车辆通讯协 协议采用大端模式(big-endian)的网络字节序来传递字和双字。
	//这个数据结构只能适应2通道的传感器GPRS数据发送，并且不能传输状态信息。
	/* 头 标识位 */ //(1 Byte)
	uint8_t			Start_Iden;
	/* 通信 消息头 *///(16 Byte)
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
	/* 消息类型:0XFB(配置服务器信息) *///(1 Byte)
	uint8_t			Body_Type;
	/* 消息操作类型:00->同步状态;01->修改状态;02->回复 *///(1 Byte)
	uint8_t			Cmd_Type;
	/* 操作个数 *///(1 Byte)
	uint8_t			Num;

}ConfigServiceComStruct;


/* 探头类型 */
typedef enum
{
	/* 空数据包 */
	CSCC_Null    = 0x00,
	/* 系统信息 */
	CSCC_System  = 0x01,
	/* 电源信息 */
	CSCC_Power   = 0x02,
	/* 系统时钟信息 */
	CSCC_Clock   = 0x03,
	/* GSM模块信息 */
	CSCC_GSM     = 0x04,
	/* 传感器信息 */
	CSCC_Sensor  = 0x05,
	/* 菜单信息 */
	CSCC_Menu    = 0x06,
	/* 升级信息 */
	CSCC_Update  = 0x08,
}
ConfigService_ComContentType;

/* 配置服务器通信协议操作结构 */
typedef struct
{
	/* 类型 */
	ConfigService_ComContentType	Type;
	/* 地址 */
	uint8_t		Adder_H;	
	uint8_t		Adder_L;
	
	/* 长度 */
	uint8_t		Len_H;	
	uint8_t		Len_L;

	/* 数据 */
	uint8_t		Data[640];	

}ConfigServiceComContentStruct;


/* 配置服务器发送数据信息固定配置 */
static const ConfigServiceComStruct  ConfigServiceComFix = 
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
	0XFB,
	/* 消息操作类型 */
	0X00,
	/* 操作个数 */
	0X00,

/****************************************************/
};

/* 配置服务器参数 */
ConfigService_ParaTabDef ConfigService = 
{

	/* 系统信息 */
	/* 序列号(6字节) *///注意：这里的高低为数据
	0X0,0X0,0X0,0X0, 0X0,0X0,0X0,0X0, 0X0,0X0,0X0,0X0,
	
	/* 软件版本(3 字节)  */
	0, 0, 1,
	
	/* 硬件版本(3 字节)  */
	0, 0, 1,
	
	
	
	/* 电源状态 */
	TRUE,
	/* 外接电源 报警开关 */
	FALSE,
	/* 外接电源 声光报警开关 */
	FALSE,
	/* 外接电源 短信报警开关 */
	FALSE,

	/* 电量状态 */
	100,
	/* 低电量 报警开关 */
	FALSE,
	/* 低电量 声光报警开关 */
	FALSE,
	/* 低电量 短信报警开关 */
	FALSE,
	
	/* 探头信息 */
#if (1)	
	/* 探头 1 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,
	
	/* 探头 2 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,
	
	/* 探头 3 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 4 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 5 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 6 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 7 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 8 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 9 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 10 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 11 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 12 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 13 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 14 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 15 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 16 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 17 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 18 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 19 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 20 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 21 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 22 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 23 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 24 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 25 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 26 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 27 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 28 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 29 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,
	
	/* 探头 30 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,
	
	/* 探头 31 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,

	/* 探头 32 类型 */
	CSPS_Null,
	/* 探头报警开关 */
	FALSE,
	/* 声光音报警开关 */
	FALSE,
	/* 短信报警开关 */
	FALSE,

	/* 探头SN号 */
	0X00,0X00,0X00,0X00,
	/* 卡片ID号 */
	0X00000000, 
	/* 报警上限 */
	0.0,
	/* 报警下限 */
	0.0,
#endif

	/* 菜单信息 */
	/* 声光 报警开关 */
	FALSE,
	/* 短信 报警开关 */
	FALSE,
	/* 保留 */
	0,
	/* 保留 */
	0,
	
	/* 重复短信报警时间 */
	600,
	/* 报警数据记录间隔 */
	120,
	/* 正常数据记录间隔 */
	300,
	
	/* 升级信息 */
	0,
	/* 升级文件名 */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,
	/* 升级服务器地址（IP/域名） */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,	
	/* 升级服务器端口 */
	0X00,
	0X00,
/* 升级文件crc32校验位 */
	0,
 /* 数据服务器地址（域名） */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, /* 数据服务器预留字节共4个字节*/
	121,42,49,249, /* 数据服务器地址（IP） */
	/*数据服务器端口 */
	5902,
	0,
 /* 配置服务器地址（域名） */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, /* 配置服务器预留字节共4个字节*/
	115,28,105,232, /* 配置服务器地址（IP） */
	/*配置服务器端口 */
	12346,
	0,
	 /* 升级服务器地址（域名） */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, /* 升级服务器预留字节共4个字节*/
	0,0,0,0, /* 升级服务器地址（IP） */
	/*升级服务器端口 */
	0,
	0,
	/* GSM模块 报警短信号码_1(6字节)  */		//15067448280
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0,		 0X0,0X0,0X0,
	/* GSM模块 报警短信号码_2(6字节)  */
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0,		 0X0,0X0,0X0,
	/* GSM模块 报警短信号码_3(6字节)  */
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0,		 0X0,0X0,0X0,
	/* GSM模块 报警短信 备用4(6字节)  */
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0, 		 0X0,0X0,0X0,
	/* GSM模块 报警短信 备用5(6字节)  */
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0, 		 0X0,0X0,0X0,
	0X00,0X00,0X00,0X00, /* 预留字节共4个字节*/
};


/* 数据寄存器 */
/* 发送数据寄存器 */
static uint8_t ConfigService_TXDR[1536];
/* 接收数据寄存器 */
static uint8_t ConfigService_RXDR[1500];

/* 配置服务器应用参数 */
ConfigService_AppDef ConfigService_App = 
{
	FALSE,
	FALSE,
};



/************************************************************************************************/
/* 函数名	: ConfigService_SetTXContentData												 	*/
/* 输入  	: dest:目标地址，src:源地址			 												*/
/* 输出		: 结束地址 																				*/
/* 作用	 	: 设置发送操作内容的数据								 									*/
/* 创建日期 	: 2016/08/15																		*/
/************************************************************************************************/
static uint8_t* ConfigService_SetTXContentData(uint8_t* dest,ConfigServiceComContentStruct* src)
{
	/* 数据长度 */
	uint32_t len = 0;
	
	/* 计算数据长度 */
	len = 5 + (src->Len_H * 256) + src->Len_L;
	
	/* 设置SN号 */
	memcpy(dest,(uint8_t*)src,len);


	/* 返回结束地址 */
	return (dest+len);
}

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

/* 设置上传配置信息项目 */
/* 系统信息  */
#define ConfigService_UploadSystem	       (0x01)
/* 电源信息 */
#define ConfigService_UploadPower	         (0x02)
/* 系统时钟信息 */
#define ConfigService_Clock	               (0x03)
/* GSM模块信息 */
#define ConfigService_Gsm	                 (0x04)
/* 传感器探头信息 */
#define ConfigService_UploadSensor	       (0x05)
/* 菜单信息 */
#define ConfigService_UploadMenu	         (0x06)
/* 升级信息 */
#define ConfigService_UploadUpdata	       (0x08)

/* 空包信息 */
#define ConfigService_UploadNull	(0x00)
/* 全部信息 */
#define ConfigService_UploadAll		(0xFF)
/************************************************************************************************/
/* 函数名	: ConfigService_Cmd																 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 发送配置信息内容指令								 									*/
/* 创建日期 	: 2016/08/11																		*/
/* 修改日期 	: 2016/08/11																		*/
/************************************************************************************************/
void ConfigService_Cmd(uint8_t item,uint8_t type)
{
	ConfigServiceComStruct             *ConfigService_parap;/* 消息头指针 */
	ConfigServiceComContentStruct   configservicecomcontent;/* 操作内容寄存器 */
	uint8_t                                      *end_Adder;/* 数据结束地址 */
	uint8_t                                          *check;/* 校验位指针 */
	uint8_t                                       *end_Iden;/* 尾 标识位 */
	uint32_t                                            len;/* 数据长度 */
	uint32_t                                            num;/* 计数器 */  
	if((type != 0x00)&&(type != 0x01)&&(type != 0x02))	/* 判断操作类型是否合法 */
	{
		return;
	}
	
	/* 清除发送数据寄存器 */
	memset(ConfigService_TXDR,0X00, sizeof(ConfigService_TXDR));
	
	/* 设置默认值 */
	memcpy(ConfigService_TXDR,((uint8_t*)(&ConfigServiceComFix)),sizeof(ConfigServiceComFix));

	/* 设置“消息头”指针 */
	ConfigService_parap = (ConfigServiceComStruct *)ConfigService_TXDR;
	
	/* 设置SN号 */
	memcpy(ConfigService_parap->SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);
//	/* 设置消息体类型(默认) */
//	ConfigService_parap->Body_Type = 0xFB;
	/* 设置操作数类型 */
	ConfigService_parap->Cmd_Type = type;
	
	/* 设置操作数 */
	ConfigService_parap->Num = 0;
	
	/* 设置操作数据地址 */
	end_Adder = ((&ConfigService_parap->Num)+1);
	
	/* 判断是否需要设置 系统参数 */
	if(ConfigService_UploadSystem & item)
	{
		/* 设置 系统参数 */
		configservicecomcontent.Type 		          = CSCC_System;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H          	= 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.System)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.System)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.System),sizeof(ConfigService.System));
		
		/* 设置操作内容 */
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);
		
		/* 设置操作数 */
		ConfigService_parap->Num++;
	}

	/* 判断是否需要设置 电源信息 */
	if(ConfigService_UploadPower & item)
	{
		/* 设置 电源信息 */
		configservicecomcontent.Type 		          = CSCC_Power;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H 	        = 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.Power)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.Power)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.Power),sizeof(ConfigService.Power));
		/* 设置操作内容 */
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);
		/* 设置操作数 */
		ConfigService_parap->Num++;
	}

	/* 清除数据长度 */
	len = 0;
	/* 判断是否需要设置 传感器信息 */
	if(ConfigService_UploadSensor & item)
	{
		/* 设置 传感器信息 */
		configservicecomcontent.Type 		= CSCC_Sensor;
		configservicecomcontent.Adder_L 	= 0;
		configservicecomcontent.Adder_H 	= 0;
		for(num = 0 ; num < 32 ; num++)
		{
			if(ConfigService.Sensor[num].SensorType != CSPS_Null)	/* 判断当前的探头类型是否配置过 */
			{
				/* 当前探头已经配置 */
				memcpy(&configservicecomcontent.Data[sizeof(ConfigService_Para_Sensor)*num],
					  (uint8_t*)(&ConfigService.Sensor[num]),
					  sizeof(ConfigService_Para_Sensor));
				len += sizeof(ConfigService_Para_Sensor);/* 计算数据长度 */ 
			}
		}
		configservicecomcontent.Len_L		          = len%256;
		configservicecomcontent.Len_H		          = len/256;	
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);			/* 设置操作内容 */	
		ConfigService_parap->Num++;		/* 设置操作数 */
	}
	
	/* 判断是否需要设置 菜单信息 */
	if(ConfigService_UploadMenu & item)
	{
		/* 设置 菜单信息 */
		configservicecomcontent.Type 		          = CSCC_Menu;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H 	        = 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.Menu)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.Menu)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.Menu),sizeof(ConfigService.Menu));	
		/* 设置操作内容 */
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);
		/* 设置操作数 */
		ConfigService_parap->Num++;
	}
	
	/* 判断是否需要设置 远程升级信息 */
	if(ConfigService_UploadUpdata & item)
	{
		ConfigService.Update.Flag=0;
		/* 设置 远程升级信息 */
		configservicecomcontent.Type 		          = CSCC_Update;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H 	        = 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.Update.Flag)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.Update.Flag)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.Update.Flag),sizeof(ConfigService.Update.Flag));
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);	/* 设置操作内容 */
		ConfigService_parap->Num++;/* 设置操作数 */
	}
 /* 判断是否需要设置 GSM模块信息 */	
	if(ConfigService_Gsm & item)
	{
		/* 设置 GSM模块信息 */
		configservicecomcontent.Type 		          = CSCC_GSM;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H 	        = 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.Gsm)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.Gsm)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.Gsm),sizeof(ConfigService.Gsm));
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);	/* 设置操作内容 */
		ConfigService_parap->Num++;/* 设置操作数 */
	}
	if(ConfigService_UploadNull == item)/* 判断是否为 空包信息 */
	{
		/* 设置 空包信息 */
		configservicecomcontent.Type 		           = CSCC_Null;
		configservicecomcontent.Adder_L 	         = 0;
		configservicecomcontent.Adder_H 	         = 0;
		configservicecomcontent.Len_L		           = 0;
		configservicecomcontent.Len_H		           = 0;
		memset(configservicecomcontent.Data,0x00,sizeof(configservicecomcontent.Data));
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);/* 设置操作内容 */
		ConfigService_parap->Num++;/* 设置操作数 */
	}
	len = ((uint32_t)(end_Adder - &ConfigService_parap->Body_Type));	/* 计算数据长度 */
	/* 计算消息体长度 */
	ConfigService_parap->Attribute_HSB_Length = (len>>8);
	ConfigService_parap->Attribute_LSB_Length = (len%256);	
	check = end_Adder;/* 设置校验位指针 */
	/* 设置数据长度 */
	len = len + 13;//(13是通信协议固定数据长度)
	/* 计算校验码 */
	for(num = 1 ; num < len ; num++)
	{
		/* 从 消息头 到 消息体 最后一个 数据 异或 */
		*check ^= ConfigService_TXDR[num];
	}
	/* 设置数据长度 */
	len = len + 1;//(+校验)
	//1、转义 数据/* 转义 *//* 获得GPRS发送数据长度 */
	num = GPRS_Host_To_Network_Translator_Data(&ConfigService_RXDR[1],
	                                           &ConfigService_TXDR[1],len);	
	memcpy(&ConfigService_TXDR[1],&ConfigService_RXDR[1],num);/* 拷贝转移完成数据 */
	memset(ConfigService_RXDR,0x00,sizeof(ConfigService_RXDR));/* 清除接收数据寄存器 */
	len = num;/* 设置数据长度 */
	end_Iden  = &ConfigService_TXDR[len];/* 设置尾标识位指针 */
  *end_Iden = 0X7E;/* 设置尾标识 */
	/* 设置GPRS发送数据参数 */
	GPRS.ConfigService.Para.TXDR	= ConfigService_TXDR;
	GPRS.ConfigService.Para.TXLen	= len+1;//(加上1字节结束符长度)
	GPRS.ConfigService.Para.RXDR	= ConfigService_RXDR;
	GPRS.ConfigService.Para.RXLen	= 1500;
	/* 发送队列(ConfigService) */
	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,ConfigService_EVENT);
	/* 设置配置服务器运行状态 */
	ConfigService_StartRun();
}


/************************************************************************************************/
/* 函数名	: ConfigService_SYNC															 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 发送同步状态指令								 									*/
/* 创建日期 	: 2016/08/11																		*/
/************************************************************************************************/
//static void ConfigService_SYNC(void)
//{

//	/* 发送同步状态指令 */
//	ConfigService_Cmd(ConfigService_UploadNull,0x00);
//	
//}

/************************************************************************************************/
/* 函数名	: ConfigService_Query															 	*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 发送查询状态指令								 									*/
/* 创建日期 	: 2016/08/11																		*/
/************************************************************************************************/
static void ConfigService_Query(void)
{
	
	/* 发送修改状态指令 */
	ConfigService_Cmd(ConfigService_UploadNull,0x01);
	
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
extern  BOOL     GSM_DEBUG_Flag; //zwc
void ConfigService_TXLog(INSTRU_CONFIG_INF_TAB* scrPara)
{
	(void)scrPara;
	
	/* 判断是否正在进行配置服务器操作 */
	if(ConfigService_App.Run_State == TRUE)
	{
		/* 正在进行配置服务器操作，不再发送其它操作命令。 */
		return;
	}
/* 1、判断仪器是否同步完成 系统每次复位都会进行一次将系统上所有参数同步到配置服务器上*/
	if(ConfigService_App.SYNC_State == FALSE)	/* 仪器未进行同步或则同步失败 */
	{
			/* 仪器与平台已经同步完成，设置同步状态标志. */
			ConfigService_App.SYNC_State = TRUE;//设备重启后默认会发一次系统所有信息  将同步状态该为已同步
			if(GSM_DEBUG_Flag ==TRUE)
			{
				printf("1、将系统所有信息同步到配置服务器上\n");
			}
//	ConfigService_SYNC();	/* 发送同步状态指令 */
		ConfigService_Cmd(ConfigService_UploadAll,0x01);			/* 上传当设备配置信息(同步平台参数) */
	}
	else
	{
			if(GSM_DEBUG_Flag ==TRUE)
			{
				printf("发送查询配置平台是否下发了配置参数消息\r\n");
			}
		/* 仪器已经进行同步完成 */
		/* 发送查询状态指令 */
 		ConfigService_Query();
		
	}

}

/************************************************************************************************/
/* 函数名	: ConfigService_UpdataConfig														*/
/* 输入  	: 无								 												*/
/* 输出		: 无		 																		*/
/* 作用	 	: 更新仪器配置数据										 							*/
/* 创建日期 	: 2016/08/16																		*/
/* 修改日期 	: 2016/08/16																		*/
/*   版本 	: V0: 创建“数据业务”逻辑															*/
/************************************************************************************************/
static ConfigServiceComContentStruct * ConfigService_UpdataConfig(ConfigServiceComContentStruct* config)
{
	/* 操作结束指针 */
	uint8_t* contentEndp = (uint8_t*)config;
	/* 地址 */
	uint16_t	adder = config->Adder_L + (((uint16_t)config->Adder_H)<<8);
	/* 数据长度 */
	uint32_t	len   = config->Len_L + (((uint16_t)config->Len_H)<<8);
	
	
	switch(config->Type)
	{
		/* 空数据包 */
		case CSCC_Null:
		{
			/* 空数据包不做任何操作，直接返回结束地址 */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* 系统信息 */
		case CSCC_System:
		{
			/* 设置系统信息 */
			/* 复制数据到“系统信息”数据区中 */
			memcpy(((uint8_t*)(&ConfigService.System)+adder),config->Data,len);
			/* 返回结束地址 */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* 电源信息 */
		case CSCC_Power:
		{
			/* 设置电源信息 */
			/* 复制数据到“电源信息”数据区中 */
			memcpy(((uint8_t*)(&ConfigService.Power)+adder),config->Data,len);
			/* 返回结束地址 */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* 系统时钟信息 */
		case CSCC_Clock:
		{
			/* 当前时钟信息没有需求做，无需考虑。 */
			/* 返回结束地址 */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* GSM模块信息 */
		case CSCC_GSM:
		{
		  /* 复制数据到“GSM模块信息”数据区中 */
			memcpy(((uint8_t*)(&ConfigService.Gsm)+adder),config->Data,len);
			/* 返回结束地址 */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* 传感器信息 */
		case CSCC_Sensor:
		{
			/* 设置传感器信息 */
			/* 复制数据到“传感器信息”数据区中 */
			memcpy(((uint8_t*)(&ConfigService.Sensor)+adder),config->Data,len);
			/* 返回结束地址 */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* 菜单信息 */
		case CSCC_Menu:
		{
			/* 设置菜单信息 */
			/* 复制数据到“菜单信息”数据区中 */
			memcpy(((uint8_t*)(&ConfigService.Menu)+adder),config->Data,len);
			/* 返回结束地址 */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
	
		/* 升级信息 */
		case CSCC_Update:
		{
			/* 设置升级信息 */
			/* 复制数据到“升级信息”数据区中 */
			memcpy(((uint8_t*)(&ConfigService.Update)+adder),config->Data,len);
						if(ConfigService.Update.Flag==0)
						{
							  if(GSM_DEBUG_Flag ==TRUE)
								{
								 printf("配置服务器下发了取消远程升级操作消息\r\n");	
								 IAP_App.RunState = FALSE;
								}
						}else if(ConfigService.Update.Flag==1)
						{
								if(GSM_DEBUG_Flag ==TRUE)
						    printf("配置服务器下发了远程升级操作消息\r\n");
						}
			/* 返回结束地址 */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		default:
		{
			/* 获取的通道目标不存在 */

			return FALSE;
		}
	}
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
static BOOL ConfigService_ParseData(uint8_t* datap,int32_t len,BOOL* comContentType,uint8_t* cmdtype)
{
	/* 消息头指针 */
	ConfigServiceComStruct              *ConfigService_parap;
	/* 操作内容寄存器 */                
	ConfigServiceComContentStruct       *configservicecomcontent;
	/* 校验位指针 */
	uint8_t                             *checkp;
	/* 校验数据 */
	uint8_t                             check = 0;
	/* 计数器 */
	uint32_t                            num;
	
	/* 1、反向转义数据 */
	/* 先将发送数据区清空(节省数据空间) */
	memset(ConfigService_TXDR,0X00, sizeof(ConfigService_TXDR));
	/* 将转义后的数据存到数据区 */
	len = GPRS_Network_To_Host_Translator_Data(ConfigService_TXDR,datap,len);
	/* 清空接收数据区(节省数据空间) */
	memset(ConfigService_RXDR,0X00, sizeof(ConfigService_RXDR));
	/* 将发送区的数据转移到接收数据区 */
	memcpy(datap,ConfigService_TXDR,len);
	
	/* 2、判断数据合法性 */
	/* 设置数据指针 */
	ConfigService_parap = (ConfigServiceComStruct *)ConfigService_TXDR;
	/* a、比较SN号是否一致 */
	if(memcmp(ConfigService_parap->SN_NO,
			 (uint8_t*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),
			  6) != 0)
	{
		/* SN号不一致 */
		return FALSE;
	}
	
	/* b、计算校验 */
	for(num = 1 ; num < (len - 2); num++)
	{
		/* 从 消息头 到 消息体 最后一个 数据 异或 */
		check ^= ConfigService_TXDR[num];
	}
	
	/* 设置校验指针 */
	checkp = &ConfigService_TXDR[len-2];
	/* 判断校验是否相同 */
	if(check != *checkp)
	{
		/* 校验不相同 */
		return FALSE;
	}
	
	/* c、消息体类型 */
	if(ConfigService_parap->Body_Type != 0xFB)
	{
		/* 消息体类型不是配置服务器类型 */
		return FALSE;
	}
	
	/* 设置操作类型 */
	*cmdtype = ConfigService_parap->Cmd_Type;
	
	/* 设置操作内容指针 */
	configservicecomcontent = (ConfigServiceComContentStruct*)((&ConfigService_parap->Num)+1);
	
	/* 3、解析操作参数 */
	for(num = 0 ; num < ConfigService_parap->Num; num++)
	{
		/* 判断当前“配置模块类型”是否为空包 */
		if(configservicecomcontent->Type != CSCC_Null)
		{
			/* 设置 操作内容 为非空操作 */
			*comContentType = TRUE;
		}
		
		/* 判断当前接收的操作类型 */
		if(ConfigService_parap->Cmd_Type != 0x02)
		{
			/* 操作类型不为同比类型，只需要对数据进行验证，需要设置参数。 */
			configservicecomcontent = ConfigService_UpdataConfig(configservicecomcontent);	
		}
	}
	
	/* 判断当前“配置模块类型”是否为空包 */
	if((*comContentType != TRUE) || (ConfigService_parap->Cmd_Type == 0x02))
	{
		/* 空包数据解析成功 */
		return TRUE;
	}
	
	/* 4、设置回复数据 */
	/* 设置数据指针 */
	ConfigService_parap = (ConfigServiceComStruct *)datap;
	/* 设置消息ID */
	ConfigService_parap->ID_Type_HSB = ConfigServiceComFix.ID_Type_HSB;
	ConfigService_parap->ID_Type_LSB = ConfigServiceComFix.ID_Type_LSB;
	/* 设置操作类型 */
	ConfigService_parap->Cmd_Type = 0x02;
	/* 计算校验 */
	check ^= (ConfigService_parap->ID_Type_HSB^
			      ConfigService_parap->ID_Type_LSB^
			      ConfigService_parap->Cmd_Type^
			      0x89^0x00^0x01);
	/* 设置校验 */
	datap[len-2] = check;
	/* 转义 *//* 获得GPRS发送数据长度 */
	num = GPRS_Host_To_Network_Translator_Data(
		 &ConfigService_TXDR[1],&datap[1],(len-2));
	/* 设置数据长度 */
	len = num+1;
	/* 设置尾标识 */
	ConfigService_TXDR[len] =  0X7E;
	/* 清空接收数据区 */
	memset(ConfigService_RXDR,0X00, sizeof(ConfigService_RXDR));
	/* 设置GPRS发送数据参数 */
	GPRS.ConfigService.Para.TXDR	= ConfigService_TXDR;
	GPRS.ConfigService.Para.TXLen	= len+1;//(加上1字节结束符长度)
	GPRS.ConfigService.Para.RXDR	= ConfigService_RXDR;
	GPRS.ConfigService.Para.RXLen	= 0;
	
	/* 非空数据解析成功 */
	return TRUE;
}

/************************************************************************************************/
/* 函数名	: ConfigService_UpdataPara												 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 无		 																		*/
/* 作用	 	: 更新仪器配置信息								 									*/
/* 创建日期 	: 2016/08/19																		*/
/* 修改日期 	: 2016/08/19																		*/
/*   版本 	: V0: 创建																			*/
/************************************************************************************************/
void ConfigService_UpdataPara(INSTRU_CONFIG_INF_TAB* scrPara)//更改服务器下发的配置参数操作
{
	/* 网络数据寄存器 */
	NetworkConfigStruct network;
	/* 升级端口 */
	uint32_t updateport;
	/* 更新 系统信息 *///系统信息不可更新
	if(memcmp(ConfigService.System.SerialNumber.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number,
			  sizeof(ConfigService.System.SerialNumber)) != 0)
	{
		/* SN不一致,说明有问题 */
		__nop();
	}
	if(memcmp(ConfigService.System.HardwareVersion.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number,
			  sizeof(ConfigService.System.HardwareVersion)) != 0)
	{
		/* 硬件版本号不一致,说明有问题 */
		__nop();
	}
	if(memcmp(ConfigService.System.SoftwareVersion.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Software_Version.Serial_Number,
			  sizeof(ConfigService.System.SoftwareVersion)) != 0)
	{
		/* 软件版本号不一致,说明有问题 */
		__nop();
	}

	/* 更新 电源信息 */
	if(ConfigService.Power.PowerState != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Link_State)
	{
		/* 外接电源连接状态 不一致 */
		__nop();	
	}
   
	if((ConfigService.Power.PowerSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch)||
	   (ConfigService.Power.PowerSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch))
	{
		/* 电源声光报警 不一致 */
		if(ConfigService.Power.PowerSLAlarmSwitch == 0)
		{
			scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch = FALSE;
			scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch = FALSE;
		}else
		{
			scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch = TRUE;
			scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch = TRUE;
		}	
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch));
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch));
	}
	
	if(ConfigService.Power.PowerSMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch)
	{
		/* 电源短信报警 不一致 */
		scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch = 
		ConfigService.Power.PowerSMSAlarmSwitch;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch));
	}
	
//	if(ConfigService.Power.PowerChargeState != 
//	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Charge_State)
//	{
//		/* 状态信息无需配置 */
//	}
//	
//	if(ConfigService.Power.PowerQuantityState != 
//	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Quantity_State)
//	{
//		/* 状态信息无需配置 */
//	}
	
	if(ConfigService.Power.BTAlarmSwitch != TRUE)
	{
		/* 外接电源报警开关 不一致 */
		__nop();
	}
	
	if((ConfigService.Power.BTSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch)||
	   (ConfigService.Power.BTSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch))
	{
		/* 电源声光报警 不一致 */
		if(ConfigService.Power.BTSLAlarmSwitch == FALSE)
		{
			scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch = FALSE;
			scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch = FALSE;
		}else
		{
			scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch = TRUE;
			scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch = TRUE;
		}
		
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch));
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch));
	}
	
	if(ConfigService.Power.BTSMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch)
	{
		/* 电源短信报警 不一致 */
		scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch = 
		ConfigService.Power.BTSMSAlarmSwitch;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch));
	}
	
	/* 初始化 配置服务器 传感器信息 */
#if (1)
	/* 探头 1 */
	if(ConfigService.Sensor[0].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type = 
		ConfigService.Sensor[0].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type != Sensor_Null)
	{	
		if((ConfigService.Sensor[0].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[0].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if(ConfigService.Sensor[0].SLAlarmSwitch == FALSE)
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[0].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[0].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[0].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[0].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[0].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[0].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[0].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[0].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[0].SN));
	    }
		
		if(ConfigService.Sensor[0].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[0].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[0].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit = 
			ConfigService.Sensor[0].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[0].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit = 
			ConfigService.Sensor[0].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 2 */
	if(ConfigService.Sensor[1].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type = 
		ConfigService.Sensor[1].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type != Sensor_Null)
	{	
		if((ConfigService.Sensor[1].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[1].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[1].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[1].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[1].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[1].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[1].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[1].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[1].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[1].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[1].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[1].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[1].SN));
	    }
		
		if(ConfigService.Sensor[1].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[1].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[1].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit = 
			ConfigService.Sensor[1].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[1].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit = 
			ConfigService.Sensor[1].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit));	
		}
	}	
	
	/* 探头 3 */
	if(ConfigService.Sensor[2].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type = 
		ConfigService.Sensor[2].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[2].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[2].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[2].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[2].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[2].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[2].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[2].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[2].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[2].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[2].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[2].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[2].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[2].SN));
	    }
		
		if(ConfigService.Sensor[2].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[2].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[2].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit = 
			ConfigService.Sensor[2].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[2].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit = 
			ConfigService.Sensor[2].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 4 */
	if(ConfigService.Sensor[3].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type = 
		ConfigService.Sensor[3].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type != Sensor_Null)
	{	
		if((ConfigService.Sensor[3].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[3].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[3].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[3].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[3].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[3].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[3].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[3].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[3].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[3].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[3].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[3].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[3].SN));
	    }
		
		if(ConfigService.Sensor[3].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[3].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[3].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit = 
			ConfigService.Sensor[3].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[3].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit = 
			ConfigService.Sensor[3].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 5 */
	if(ConfigService.Sensor[4].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type = 
		ConfigService.Sensor[4].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[4].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[4].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[4].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[4].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[4].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[4].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[4].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[4].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[4].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[4].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[4].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[4].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[4].SN));
	    }
		
		if(ConfigService.Sensor[4].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[4].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[4].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit = 
			ConfigService.Sensor[4].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[4].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit = 
			ConfigService.Sensor[4].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 6 */
	if(ConfigService.Sensor[5].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type = 
		ConfigService.Sensor[5].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[5].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[5].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[5].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[5].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[5].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[5].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[5].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[5].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[5].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[5].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[5].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[5].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[5].SN));
	    }
		
		if(ConfigService.Sensor[5].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[5].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[5].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit = 
			ConfigService.Sensor[5].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[5].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit = 
			ConfigService.Sensor[5].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 7 */
	if(ConfigService.Sensor[6].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type = 
		ConfigService.Sensor[6].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[6].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[6].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[6].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[6].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[6].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[6].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[6].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[6].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[6].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[6].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[6].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[6].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[6].SN));
	    }
		
		if(ConfigService.Sensor[6].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[6].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[6].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit = 
			ConfigService.Sensor[6].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[6].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit = 
			ConfigService.Sensor[6].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 8 */
	if(ConfigService.Sensor[7].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type = 
		ConfigService.Sensor[7].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[7].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[7].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[7].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[7].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[7].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[7].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[7].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[7].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[7].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[7].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[7].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[7].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[7].SN));
	    }
		
		if(ConfigService.Sensor[7].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[7].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[7].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit = 
			ConfigService.Sensor[7].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[7].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit = 
			ConfigService.Sensor[7].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 9 */
	if(ConfigService.Sensor[8].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type = 
		ConfigService.Sensor[8].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[8].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[8].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[8].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[8].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[8].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[8].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[8].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[8].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[8].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[8].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[8].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[8].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[8].SN));
	    }
		
		if(ConfigService.Sensor[8].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[8].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[8].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit = 
			ConfigService.Sensor[8].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[8].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit = 
			ConfigService.Sensor[8].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 10 */
	if(ConfigService.Sensor[9].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type = 
		ConfigService.Sensor[9].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[9].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[9].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[9].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[9].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[9].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[9].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[9].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[9].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[9].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[9].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[9].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[9].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[9].SN));
	    }
		
		if(ConfigService.Sensor[9].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[9].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[9].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit = 
			ConfigService.Sensor[9].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[9].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit = 
			ConfigService.Sensor[9].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 11 */
	if(ConfigService.Sensor[10].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type = 
		ConfigService.Sensor[10].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[10].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[10].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[10].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[10].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[10].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[10].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[10].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[10].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[10].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[10].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[10].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[10].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[10].SN));
	    }
		
		if(ConfigService.Sensor[10].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[10].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[10].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit = 
			ConfigService.Sensor[10].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[10].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit = 
			ConfigService.Sensor[10].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 12 */
	if(ConfigService.Sensor[11].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type = 
		ConfigService.Sensor[11].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[11].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[11].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[11].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[11].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[11].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[11].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[11].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[11].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[11].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[11].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[11].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[11].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[11].SN));
	    }
		
		if(ConfigService.Sensor[11].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[11].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[11].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit = 
			ConfigService.Sensor[11].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[11].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit = 
			ConfigService.Sensor[11].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 13 */
	if(ConfigService.Sensor[12].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type = 
		ConfigService.Sensor[12].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[12].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[12].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[12].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[12].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[12].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[12].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[12].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[12].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[12].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[12].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[12].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[12].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[12].SN));
	    }
		
		if(ConfigService.Sensor[12].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[12].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[12].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit = 
			ConfigService.Sensor[12].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[12].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit = 
			ConfigService.Sensor[12].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 14 */
	if(ConfigService.Sensor[13].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type = 
		ConfigService.Sensor[13].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[13].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[13].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[13].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[13].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[13].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[13].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[13].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[13].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[13].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[13].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[13].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[13].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[13].SN));
	    }
		
		if(ConfigService.Sensor[13].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[13].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[13].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit = 
			ConfigService.Sensor[13].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[13].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit = 
			ConfigService.Sensor[13].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 15 */
	if(ConfigService.Sensor[14].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type = 
		ConfigService.Sensor[14].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[14].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[14].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[14].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[14].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[14].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[14].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[14].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[14].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[14].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[14].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[14].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[14].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[14].SN));
	    }
		
		if(ConfigService.Sensor[14].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[14].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[14].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit = 
			ConfigService.Sensor[14].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[14].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit = 
			ConfigService.Sensor[14].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 16 */
	if(ConfigService.Sensor[15].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type = 
		ConfigService.Sensor[15].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[15].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[15].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[15].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[15].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[15].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[15].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[15].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[15].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[15].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[15].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[15].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[15].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[15].SN));
	    }
		
		if(ConfigService.Sensor[15].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[15].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[15].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit = 
			ConfigService.Sensor[15].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[15].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit = 
			ConfigService.Sensor[15].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 17 */
	if(ConfigService.Sensor[16].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type = 
		ConfigService.Sensor[16].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[16].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[16].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[16].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[16].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[16].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[16].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[16].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[16].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[16].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[16].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[16].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[16].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[16].SN));
	    }
		
		if(ConfigService.Sensor[16].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[16].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[16].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit = 
			ConfigService.Sensor[16].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[16].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit = 
			ConfigService.Sensor[16].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 18 */
	if(ConfigService.Sensor[17].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type = 
		ConfigService.Sensor[17].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[17].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[17].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[17].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[17].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[17].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[17].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[17].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[17].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[17].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[17].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[17].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[17].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[17].SN));
	    }
		
		if(ConfigService.Sensor[17].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[17].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[17].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit = 
			ConfigService.Sensor[17].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[17].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit = 
			ConfigService.Sensor[17].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 19 */
	if(ConfigService.Sensor[18].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type = 
		ConfigService.Sensor[18].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[18].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[18].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[18].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[18].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[18].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[18].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[18].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[18].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[18].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[18].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[18].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[18].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[18].SN));
	    }
		
		if(ConfigService.Sensor[18].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[18].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[18].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit = 
			ConfigService.Sensor[18].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[18].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit = 
			ConfigService.Sensor[18].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 20 */
	if(ConfigService.Sensor[19].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type = 
		ConfigService.Sensor[19].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[19].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[19].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[19].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[19].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[19].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[19].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[19].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[19].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[19].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[19].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[19].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[19].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[19].SN));
	    }
		
		if(ConfigService.Sensor[19].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[19].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[19].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit = 
			ConfigService.Sensor[19].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[19].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit = 
			ConfigService.Sensor[19].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 21 */
	if(ConfigService.Sensor[20].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type = 
		ConfigService.Sensor[20].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[20].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[20].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[20].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[20].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[20].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[20].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[20].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[20].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[20].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[20].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[20].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[20].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[20].SN));
	    }
		
		if(ConfigService.Sensor[20].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[20].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[20].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit = 
			ConfigService.Sensor[20].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[20].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit = 
			ConfigService.Sensor[20].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 22 */
	if(ConfigService.Sensor[21].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type = 
		ConfigService.Sensor[21].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[21].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[21].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[21].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[21].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[21].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[21].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[21].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[21].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[21].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[21].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[21].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[21].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[21].SN));
	    }
		
		if(ConfigService.Sensor[21].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[21].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[21].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit = 
			ConfigService.Sensor[21].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[21].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit = 
			ConfigService.Sensor[21].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 23 */
	if(ConfigService.Sensor[22].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type = 
		ConfigService.Sensor[22].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[22].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[22].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[22].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[22].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[22].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[22].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[22].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[22].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[22].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[22].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[22].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[22].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[22].SN));
	    }
		
		if(ConfigService.Sensor[22].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[22].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[22].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit = 
			ConfigService.Sensor[22].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[22].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit = 
			ConfigService.Sensor[22].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 24 */
	if(ConfigService.Sensor[23].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type = 
		ConfigService.Sensor[23].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[23].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[23].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[23].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[23].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[23].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[23].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[23].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[23].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[23].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[23].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[23].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[23].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[23].SN));
	    }
		
		if(ConfigService.Sensor[23].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[23].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[23].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit = 
			ConfigService.Sensor[23].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[23].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit = 
			ConfigService.Sensor[23].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 25 */
	if(ConfigService.Sensor[24].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type = 
		ConfigService.Sensor[24].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[24].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[24].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[24].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[24].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[24].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[24].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[24].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[24].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[24].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[24].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[24].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[24].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[24].SN));
	    }
		
		if(ConfigService.Sensor[24].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[24].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[24].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit = 
			ConfigService.Sensor[24].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[24].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit = 
			ConfigService.Sensor[24].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 26 */
	if(ConfigService.Sensor[25].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type = 
		ConfigService.Sensor[25].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[25].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[25].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[25].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[25].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[25].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[25].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[25].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[25].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[25].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[25].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[25].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[25].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[25].SN));
	    }
		
		if(ConfigService.Sensor[25].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[25].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[25].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit = 
			ConfigService.Sensor[25].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[25].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit = 
			ConfigService.Sensor[25].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 27 */
	if(ConfigService.Sensor[26].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type = 
		ConfigService.Sensor[26].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[26].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[26].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[26].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[26].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[26].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[26].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[26].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[26].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[26].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[26].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[26].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[26].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[26].SN));
	    }
		
		if(ConfigService.Sensor[26].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[26].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[26].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit = 
			ConfigService.Sensor[26].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[26].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit = 
			ConfigService.Sensor[26].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 28 */
	if(ConfigService.Sensor[27].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type = 
		ConfigService.Sensor[27].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[27].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[27].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[27].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[27].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[27].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[27].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[27].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[27].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[27].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[27].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[27].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[27].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[27].SN));
	    }
		
		if(ConfigService.Sensor[27].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[27].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[27].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit = 
			ConfigService.Sensor[27].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[27].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit = 
			ConfigService.Sensor[27].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 29 */
	if(ConfigService.Sensor[28].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type = 
		ConfigService.Sensor[28].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[28].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[28].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[28].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[28].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[28].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[28].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[28].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[28].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[28].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[28].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[28].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[28].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[28].SN));
	    }
		
		if(ConfigService.Sensor[28].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[28].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[28].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit = 
			ConfigService.Sensor[28].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[28].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit = 
			ConfigService.Sensor[28].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 30 */
	if(ConfigService.Sensor[29].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type = 
		ConfigService.Sensor[29].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[29].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[29].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[29].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[29].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[29].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[29].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[29].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[29].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[29].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[29].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[29].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[29].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[29].SN));
	    }
		
		if(ConfigService.Sensor[29].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[29].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[29].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit = 
			ConfigService.Sensor[29].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[29].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit = 
			ConfigService.Sensor[29].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 31 */
	if(ConfigService.Sensor[30].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type = 
		ConfigService.Sensor[30].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[30].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[30].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[30].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[30].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[30].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[30].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[30].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[30].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[30].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[30].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[30].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[30].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[30].SN));
	    }
		
		if(ConfigService.Sensor[30].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[30].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[30].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit = 
			ConfigService.Sensor[30].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[30].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit = 
			ConfigService.Sensor[30].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit));	
		}
	}
	
	/* 探头 32 */
	if(ConfigService.Sensor[31].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type = 
		ConfigService.Sensor[31].SensorType;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type));
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[31].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[31].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch))
		{
			/* 电源声光报警 不一致 */
			if((ConfigService.Sensor[31].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[31].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[31].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[31].SMSAlarmSwitch;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[31].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[31].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[31].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[31].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[31].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[31].SN[3];
			/* 更新存储配置 */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[31].SN));
	    }
		
		if(ConfigService.Sensor[31].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[31].ID;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[31].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit = 
			ConfigService.Sensor[31].UpLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[31].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit = 
			ConfigService.Sensor[31].LowLimit;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit));	
		}
	}
	
#endif
	
	/* 初始化 配置服务器 菜单信息 */
	if(ConfigService.Menu.SLAlarmSwitch !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)
	{
		/* 电源短信报警 不一致 */
		scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch = 
		ConfigService.Menu.SLAlarmSwitch;
		scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch = 
		ConfigService.Menu.SLAlarmSwitch;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch));
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch));
	}
	
	if(ConfigService.Menu.SMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)
	{
		/* 电源短信报警 不一致 */
		scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch = 
		ConfigService.Menu.SMSAlarmSwitch;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch));
	}
	
	if(ConfigService.Menu.SMSAlarm_RepeatTime !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval)
	{
		/* 电源短信报警 不一致 */
		scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval = 
		ConfigService.Menu.SMSAlarm_RepeatTime;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval));
	}
	
	if(ConfigService.Menu.SaveInterval_Alarm !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval)
	{
		/* 电源短信报警 不一致 */
		scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval = 
		ConfigService.Menu.SaveInterval_Alarm;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval));
	}
	
	if(ConfigService.Menu.SaveInterval_Normal !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval)
	{
		/* 电源短信报警 不一致 */
		scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval = 
		ConfigService.Menu.SaveInterval_Normal;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval));
	}
	
	/* 初始化 配置服务器 升级信息 */
	/* 判断固件升级状态 */	/* 更新 升级信息 */
	if(ConfigService.Update.Flag != 
	   scrPara->Inf_State.Instru_System.Inf.UpdateFlag)
	{
		/* 固件升级状态 不一致 */
		scrPara->Inf_State.Instru_System.Inf.UpdateFlag = 
		ConfigService.Update.Flag;
		
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_System.Inf.UpdateFlag,
		sizeof(scrPara->Inf_State.Instru_System.Inf.UpdateFlag));
	}
/************************判断平台是否修改了数据服务器的IP地址、域名及端口****************************/
		/* 判断数据服务器IP是否相同 */
		if(memcmp(&ConfigService.Gsm.Data_IP_Address,
				  &scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1,sizeof(ConfigService.Gsm.Data_IP_Address)) != 0)
		{
			/*设置数据服务器IP地址*/
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1 = ConfigService.Gsm.Data_IP_Address;
			/* 清除数据服务器域名 */
			memset(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,0x00,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));
			//同步GSM的域名参数
//			/* 重新配置GSM模块 */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			  GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
		}
		else if(memcmp(ConfigService.Gsm.DataDomainName,
			   scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1)) != 0)/* 域名 */
		{
			/* 复制域名信息 */
			memcpy(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			   ConfigService.Gsm.DataDomainName,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));
			/* 清除IP */
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D = 0;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));
			//同步GSM的域名参数
//			/* 重新配置GSM模块 */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
		}
		/*判断是否更新数据服务器端口信息 */
	if(ConfigService.Gsm.DataNetworkPort != scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1)
	{
		/* 端口号 不一致 */
		scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1 =htons(ConfigService.Gsm.DataNetworkPort);
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1,
		sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1));
//		/* 重新配置GSM模块 */
//		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
	}
	
	/* 判断配置服务器IP是否相同 */
	if(memcmp(&ConfigService.Gsm.Config_IP_Address,
				  &scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2,sizeof(ConfigService.Gsm.Config_IP_Address)) != 0)
		{
			/*设置配置服务器IP地址*/
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2 = ConfigService.Gsm.Config_IP_Address;
			/* 清除配置服务器域名 */
			memset(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,0x00,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));
			//同步GSM的域名参数
//			/* 重新配置GSM模块 */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
		}
		else if(memcmp(ConfigService.Gsm.ConfigDomainName,
			   scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2)) != 0)/* 域名 */
		{
			/* 复制域名信息 */
			memcpy(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			   ConfigService.Gsm.ConfigDomainName,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));
			/* 清除IP */
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D = 0;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));
			//同步GSM的域名参数
//			/* 重新配置GSM模块 */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
		}
		/*判断是否更新配置服务器端口信息 */
	if(ConfigService.Gsm.ConfigNetworkPort != scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2)
	{
		/* 端口号 不一致 */
		scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2 = htons(ConfigService.Gsm.ConfigNetworkPort);
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2,
		sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2));
//		/* 重新配置GSM模块 */
//		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
	}	
/************************短信报警号码**********************************************/	
			/*判断是否更新短信报警开关信息 */
	if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO1,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO1))!=0)
		 {
			 /*报警短信号码1 不一致 */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1=ConfigService.Gsm.Alarm_SMS_NO1;
		  /* 更新存储配置 */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1));
		 }
	if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO2,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO2))!=0)
		 {
			 /*报警短信号码2 不一致 */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2=ConfigService.Gsm.Alarm_SMS_NO2;
		  /* 更新存储配置 */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2));
		 }
  if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO3,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO3))!=0)
		 {
			 /*报警短信号码3不一致 */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3=ConfigService.Gsm.Alarm_SMS_NO3;
		  /* 更新存储配置 */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3));
		 }
	if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO4,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO4))!=0)
		 {
			 /*报警短信号码4 不一致 */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4=ConfigService.Gsm.Alarm_SMS_NO4;
		  /* 更新存储配置 */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4));
		 }	
  if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO5,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO5))!=0)
		 {
			 /*报警短信号码4 不一致 */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5=ConfigService.Gsm.Alarm_SMS_NO5;
		  /* 更新存储配置 */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5));
		 }
		 
/***************************通过配置服务器更改升级服务器地址zwc**************************************/	
	/* 判断当前的数据(IP/域名) */
	if(TRUE == GSM_Config_Network(ConfigService.Update.Network,&network))
	{
		/* IP */
		/* 判断IP是否相同 */
		if(memcmp(&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3,
				  &network.IP,1) != 0)
		{
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A = 
			network.IP.Serial_Number_Number.Address_A;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B = 
			network.IP.Serial_Number_Number.Address_B;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C = 
			network.IP.Serial_Number_Number.Address_C;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D = 
			network.IP.Serial_Number_Number.Address_D;
			/* 清除域名 */
			memset(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,0x00,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));
////			//同步GSM的域名参数
////			/* 重新配置GSM模块 */
////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
		}		
	}else
	{
		/* 域名 */
		if(memcmp(ConfigService.Update.Network,
			   scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3)) != 0)
		{
			/* 复制域名信息 */
			memcpy(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
			   ConfigService.Update.Network,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));
			/* 清除IP */
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D = 0;
			/* 更新存储配置 */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));
//////			//同步GSM的域名参数
//////			/* 重新配置GSM模块 */
//////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
		}
	}

	/* 设置升级固件端口号 */
	updateport = (((uint32_t)ConfigService.Update.Port_HSB)<<8)+
							 ConfigService.Update.Port_LSB;
	/* 更新升级固件端口信息 */
	if(updateport != scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID3)
	{
		/* 端口号 不一致 */
		scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID3 = updateport;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID3,
		sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID3));
//		/* 重新配置GSM模块 */
//		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//设置服务器参数
	}
	

}

/************************************************************************************************/
/* 函数名	: ConfigService_CheckParaModify												 		*/
/* 输入  	: scrPara:配置参数指针.			 													*/
/* 输出		: 修改参数项目.		 																*/
/* 作用	 	: 检查当前仪器配置选项中修改项.								 						*/
/* 创建日期 	: 2016/08/22																		*/
/* 修改日期 	: 2016/08/22																		*/
/*   版本 	: V0: 创建																			*/
/************************************************************************************************/
static uint8_t ConfigService_CheckParaModify(INSTRU_CONFIG_INF_TAB* scrPara)
{
	/* 更改项目类型寄存器 */
	uint8_t item = 0;
	
	/* 检查 系统信息 */
	if(memcmp(ConfigService.System.SerialNumber.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number,
			  sizeof(ConfigService.System.SerialNumber)) != 0)
	{
		/* SN不一致,设置 系统信息 修改项 */
		item  = item|ConfigService_UploadSystem;
		/* 拷贝SN号信息 */
		memcpy(ConfigService.System.SerialNumber.Serial_Number,
			   scrPara->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number,
			   sizeof(ConfigService.System.SerialNumber));
	}
	
	if(memcmp(ConfigService.System.HardwareVersion.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number,
			  sizeof(ConfigService.System.HardwareVersion)) != 0)
	{
		/* 硬件版本号不一致,设置 系统信息 修改项 */
		item  = item|ConfigService_UploadSystem;
		/* 拷贝 硬件版本号 */
		memcpy(ConfigService.System.HardwareVersion.Serial_Number,
			   scrPara->Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number,
			    sizeof(ConfigService.System.HardwareVersion));
	}

	if(memcmp(ConfigService.System.SoftwareVersion.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Software_Version.Serial_Number,
			  sizeof(ConfigService.System.SoftwareVersion)) != 0)
	{
		/* 软件版本号不一致,设置 系统信息 修改项 */
		item  = item|ConfigService_UploadSystem;
		/* 拷贝 软件版本号 */
		memcpy(ConfigService.System.SoftwareVersion.Serial_Number,
			   scrPara->Inf_State.Instru_System.Inf.Software_Version.Serial_Number,
			    sizeof(ConfigService.System.SoftwareVersion));
	}
	
	/* 检查 电源信息 */
	if((ConfigService.Power.PowerSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch)||
	   (ConfigService.Power.PowerSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch))
	{
		/* 电源声光报警不一致,设置 电源信息 修改项 */
		item  = item|ConfigService_UploadPower;
		/* 设置电源声光报警开光 */
		if((scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch == FALSE)&&
		   (scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch == FALSE))
		{
			ConfigService.Power.PowerSLAlarmSwitch = FALSE;
		}else
		{
			ConfigService.Power.PowerSLAlarmSwitch = TRUE;
		}
	}
	
	if(ConfigService.Power.PowerSMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch)
	{
		/* 电源短信报警不一致,设置 电源信息 修改项 */
		item  = item|ConfigService_UploadPower;
		/* 设置电源短信报警开关 */
		if(scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch == FALSE)
		{
			/* 设置电源短信报警开光 */
			ConfigService.Power.PowerSMSAlarmSwitch = FALSE;
		}else
		{
			/* 设置电源短信报警开光 */
			ConfigService.Power.PowerSMSAlarmSwitch = TRUE;
		}
	}
	
//	if(ConfigService.Power.PowerChargeState != 
//	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Charge_State)
//	{
//		/* 状态信息无需配置 */
//	}
//	
//	if(ConfigService.Power.PowerQuantityState != 
//	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Quantity_State)
//	{
//		/* 状态信息无需配置 */
//	}
	
	if(ConfigService.Power.BTAlarmSwitch != TRUE)
	{
		/* 电池报警开关 不一致 */
		__nop();
	}
	
	if((ConfigService.Power.BTSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch)||
	   (ConfigService.Power.BTSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch))
	{
		/* 电池声光报警不一致,设置 电源信息 修改项 */
		item  = item|ConfigService_UploadPower;
		/* 设置电池声光报警开关 */
		if((scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch == FALSE)&&
		   (scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch == FALSE))
		{
			ConfigService.Power.BTSLAlarmSwitch = FALSE;
		}else
		{
			ConfigService.Power.BTSLAlarmSwitch = TRUE;
		}
	}
	
	if(ConfigService.Power.BTSMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch)
	{
		/* 电池短信报警不一致,设置 电源信息 修改项 */
		item  = item|ConfigService_UploadPower;
		/* 设置电池短信报警开关 */
		if(scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch == FALSE)
		{
			/* 设置电源短信报警开光 */
			ConfigService.Power.BTSMSAlarmSwitch = FALSE;
		}else
		{
			/* 设置电源短信报警开光 */
			ConfigService.Power.BTSMSAlarmSwitch = TRUE;
		}
	}

	/* 初始化 配置服务器 传感器信息 */
#if (1)
	/* 探头 1 */
	if(ConfigService.Sensor[0].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[0].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[0].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[0].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[0].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[0].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[0].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[0].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[0].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[0].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[0].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[0].SN));
	    }
		
		if(ConfigService.Sensor[0].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[0].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[0].ID));
		}
		
		if(ConfigService.Sensor[0].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[0].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[0].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[0].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit;
		}
	}
	
	/* 探头 2 */
	if(ConfigService.Sensor[1].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[1].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[1].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[1].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((FALSE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch)&&
			   (FALSE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[1].SLAlarmSwitch = FALSE;
			}else if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch)&&
					 (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[1].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[1].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[1].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[1].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[1].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[1].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[1].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[1].SN));
	    }
		
		if(ConfigService.Sensor[1].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[1].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[1].ID));
		}
		
		if(ConfigService.Sensor[1].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[1].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[1].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[1].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 3 */
	if(ConfigService.Sensor[2].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[2].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[2].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[2].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[2].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[2].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[2].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[2].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[2].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[2].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[2].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[2].SN));
	    }
		
		if(ConfigService.Sensor[2].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[2].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[2].ID));
		}
		
		if(ConfigService.Sensor[2].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[2].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[2].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[2].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 4 */
	if(ConfigService.Sensor[3].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[3].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[3].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[3].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[3].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[3].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[3].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[3].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[3].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[3].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[3].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[3].SN));
	    }
		
		if(ConfigService.Sensor[3].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[3].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[3].ID));
		}
		
		if(ConfigService.Sensor[3].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[3].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[3].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[3].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit;
		}
	}
	
	/* 探头 5 */
	if(ConfigService.Sensor[4].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[4].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[4].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[4].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[4].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[4].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[4].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[4].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[4].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[4].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[4].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[4].SN));
	    }
		
		if(ConfigService.Sensor[4].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[4].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[4].ID));
		}
		
		if(ConfigService.Sensor[4].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[4].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[4].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[4].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 6 */
	if(ConfigService.Sensor[5].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[5].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[5].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[5].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[5].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[5].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[5].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[5].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[5].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[5].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[5].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[5].SN));
	    }
		
		if(ConfigService.Sensor[5].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[5].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[5].ID));
		}
		
		if(ConfigService.Sensor[5].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[5].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[5].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[5].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 7 */
	if(ConfigService.Sensor[6].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[6].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type != Sensor_Null)
	{	
		if((ConfigService.Sensor[6].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[6].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[6].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[6].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[6].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[6].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[6].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[6].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[6].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[6].SN));
	    }
		
		if(ConfigService.Sensor[6].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[6].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[6].ID));
		}
		
		if(ConfigService.Sensor[6].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[6].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[6].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[6].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 8 */
	if(ConfigService.Sensor[7].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[7].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[7].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[7].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[7].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[7].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[7].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[7].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[7].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[7].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[7].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[7].SN));
	    }
		
		if(ConfigService.Sensor[7].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[7].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[7].ID));
		}
		
		if(ConfigService.Sensor[7].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[7].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[7].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[7].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 9 */
	if(ConfigService.Sensor[8].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[8].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[8].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[8].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[8].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[8].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[8].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[8].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[8].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[8].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[8].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[8].SN));
	    }
		
		if(ConfigService.Sensor[8].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[8].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[8].ID));
		}
		
		if(ConfigService.Sensor[8].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[8].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[8].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[8].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 10 */
	if(ConfigService.Sensor[9].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[9].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[9].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[9].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[9].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[9].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[9].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[9].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[9].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[9].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[9].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[9].SN));
	    }
		
		if(ConfigService.Sensor[9].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[9].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[9].ID));
		}
		
		if(ConfigService.Sensor[9].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[9].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[9].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[9].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit;
		}
	}
	
	/* 探头 11 */
	if(ConfigService.Sensor[10].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[10].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[10].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[10].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[10].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[10].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[10].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[10].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[10].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[10].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[10].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[10].SN));
	    }
		
		if(ConfigService.Sensor[10].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[10].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[10].ID));
		}
		
		if(ConfigService.Sensor[10].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[10].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[10].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[10].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit;
		}
	}
	
	/* 探头 12 */
	if(ConfigService.Sensor[11].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[11].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[11].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[11].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[11].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[11].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[11].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[11].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[11].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[11].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[11].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[11].SN));
	    }
		
		if(ConfigService.Sensor[11].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[11].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[11].ID));
		}
		
		if(ConfigService.Sensor[11].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[11].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[11].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[11].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 13 */
	if(ConfigService.Sensor[12].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[12].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[12].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[12].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[12].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[12].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[12].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[12].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[12].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[12].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[12].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[12].SN));
	    }
		
		if(ConfigService.Sensor[12].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[12].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[12].ID));
		}
		
		if(ConfigService.Sensor[12].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[12].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[12].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[12].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit;
		}
	}	
	/* 探头 14 */
	if(ConfigService.Sensor[13].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[13].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[13].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[13].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[13].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[13].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[13].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[13].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[13].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[13].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[13].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[13].SN));
	    }
		
		if(ConfigService.Sensor[13].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[13].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[13].ID));
		}
		
		if(ConfigService.Sensor[13].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[13].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[13].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[13].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit;
		}
	}	
	
	/* 探头 15 */
	if(ConfigService.Sensor[14].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[14].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[14].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[14].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[14].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[14].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[14].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[14].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[14].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[14].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[14].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[14].SN));
	    }
		
		if(ConfigService.Sensor[14].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[14].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[14].ID));
		}
		
		if(ConfigService.Sensor[14].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[14].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[14].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[14].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit;
		}
	}	
	/* 探头 16 */
	if(ConfigService.Sensor[15].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[15].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[15].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[15].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[15].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[15].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[15].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[15].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[15].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[15].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[15].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[15].SN));
	    }
		
		if(ConfigService.Sensor[15].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[15].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[15].ID));
		}
		
		if(ConfigService.Sensor[15].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[15].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[15].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[15].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit;
		}
	}	
	/* 探头 17 */
	if(ConfigService.Sensor[16].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[16].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[16].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[16].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[16].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[16].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[16].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[16].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[16].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[16].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[16].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[16].SN));
	    }
		
		if(ConfigService.Sensor[16].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[16].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[16].ID));
		}
		
		if(ConfigService.Sensor[16].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[16].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[16].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[16].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit;
		}
	}	
	/* 探头 18 */
	if(ConfigService.Sensor[17].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[17].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[17].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[17].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[17].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[17].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[17].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[17].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[17].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[17].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[17].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[17].SN));
	    }
		
		if(ConfigService.Sensor[17].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[17].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[17].ID));
		}
		
		if(ConfigService.Sensor[17].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[17].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[17].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[17].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit;
		}
	}	
	/* 探头 19 */
	if(ConfigService.Sensor[18].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[18].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[18].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[18].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[18].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[18].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[18].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[18].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[18].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[18].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[18].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[18].SN));
	    }
		
		if(ConfigService.Sensor[18].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[18].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[18].ID));
		}
		
		if(ConfigService.Sensor[18].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[18].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[18].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[18].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit;
		}
	}	
	/* 探头 20 */
	if(ConfigService.Sensor[19].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[19].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[19].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[19].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[19].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[19].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[19].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[19].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[19].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[19].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[19].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[19].SN));
	    }
		
		if(ConfigService.Sensor[19].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[19].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[19].ID));
		}
		
		if(ConfigService.Sensor[19].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[19].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[19].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[19].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit;
		}
	}	
	/* 探头 21 */
	if(ConfigService.Sensor[20].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[20].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[20].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[20].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[20].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[20].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[20].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[20].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[20].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[20].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[20].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[20].SN));
	    }
		
		if(ConfigService.Sensor[20].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[20].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[20].ID));
		}
		
		if(ConfigService.Sensor[20].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[20].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[20].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[20].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit;
		}
	}	
	/* 探头 22 */
	if(ConfigService.Sensor[21].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[21].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[21].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[21].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[21].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[21].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[21].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[21].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[21].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[21].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[21].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[21].SN));
	    }
		
		if(ConfigService.Sensor[21].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[21].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[21].ID));
		}
		
		if(ConfigService.Sensor[21].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[21].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[21].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[21].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit;
		}
	}	
	/* 探头 23 */
	if(ConfigService.Sensor[22].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[22].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[22].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[22].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[22].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[22].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[22].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[22].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[22].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[22].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[22].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[22].SN));
	    }
		
		if(ConfigService.Sensor[22].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[22].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[22].ID));
		}
		
		if(ConfigService.Sensor[22].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[22].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[22].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[22].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit;
		}
	}	
	/* 探头 24 */
	if(ConfigService.Sensor[23].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[23].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[23].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[23].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[23].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[23].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[23].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[23].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[23].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[23].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[23].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[23].SN));
	    }
		
		if(ConfigService.Sensor[23].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[23].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[23].ID));
		}
		
		if(ConfigService.Sensor[23].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[23].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[23].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[23].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit;
		}
	}	
	/* 探头 25 */
	if(ConfigService.Sensor[24].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[24].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[24].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[24].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[24].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[24].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[24].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[24].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[24].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[24].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[24].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[24].SN));
	    }
		
		if(ConfigService.Sensor[24].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[24].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[24].ID));
		}
		
		if(ConfigService.Sensor[24].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[24].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[24].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[24].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit;
		}
	}	
	/* 探头 26 */
	if(ConfigService.Sensor[25].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[25].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[25].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[25].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[25].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[25].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[25].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[25].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[25].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[25].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[25].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[25].SN));
	    }
		
		if(ConfigService.Sensor[25].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[25].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[25].ID));
		}
		
		if(ConfigService.Sensor[25].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[25].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[25].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[25].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit;
		}
	}	
	/* 探头 27 */
	if(ConfigService.Sensor[26].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[26].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[26].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[26].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[26].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[26].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[26].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[26].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[26].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[26].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[26].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[26].SN));
	    }
		
		if(ConfigService.Sensor[26].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[26].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[26].ID));
		}
		
		if(ConfigService.Sensor[26].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[26].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[26].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[26].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit;
		}
	}	
	/* 探头 28 */
	if(ConfigService.Sensor[27].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[27].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[27].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[27].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[27].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[27].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[27].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[27].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[27].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[27].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[27].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[27].SN));
	    }
		
		if(ConfigService.Sensor[27].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[27].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[27].ID));
		}
		
		if(ConfigService.Sensor[27].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[27].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[27].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[27].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit;
		}
	}	
	/* 探头 29 */
	if(ConfigService.Sensor[28].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[28].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[28].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[28].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[28].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[28].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[28].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[28].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[28].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[28].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[28].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[28].SN));
	    }
		
		if(ConfigService.Sensor[28].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[28].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[28].ID));
		}
		
		if(ConfigService.Sensor[28].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[28].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[28].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[28].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit;
		}
	}	
	/* 探头 30 */
	if(ConfigService.Sensor[29].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[29].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[29].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[29].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[29].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[29].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[29].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[29].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[29].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[29].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[29].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[29].SN));
	    }
		
		if(ConfigService.Sensor[29].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[29].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[29].ID));
		}
		
		if(ConfigService.Sensor[29].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[29].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[29].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[29].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit;
		}
	}	
	/* 探头 31 */
	if(ConfigService.Sensor[30].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[30].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[30].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[30].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[30].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[30].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[30].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[30].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[30].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[30].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[30].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[30].SN));
	    }
		
		if(ConfigService.Sensor[30].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[30].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[30].ID));
		}
		
		if(ConfigService.Sensor[30].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[30].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[30].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[30].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit;
		}
	}	
	/* 探头 32 */
	if(ConfigService.Sensor[31].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type)
	{
		/* 探头类型不一致,设置 传感器信息 修改项 */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[31].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type;
	}
	/* 判断探头类型 */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[31].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[31].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch))
		{
			/* 探头声光报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[31].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[31].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[31].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch)
		{
			/* 探头短信报警不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[31].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[31].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[31].SN)) != 0)
	    {
			/* 探头SN号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[31].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[31].SN));
	    }
		
		if(ConfigService.Sensor[31].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO)
		{
			/* 探头ID号不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[31].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[31].ID));
		}
		
		if(ConfigService.Sensor[31].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit)
		{
			/* 探头报警上限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[31].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[31].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit)
		{
			/* 探头报警下限不一致,设置 传感器信息 修改项 */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[31].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit;
		}
	}	
#endif	
	  /* 初始化 配置服务器 菜单信息 */
		/* 声光报警开关配置*/
	if(ConfigService.Menu.SLAlarmSwitch !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)
	{
		/* 声光报警总开光不一致,设置 菜单信息 修改项 */
		item  = item|ConfigService_UploadMenu;
		/* 设置声光报警总开光 */
		if((FALSE == scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)&&
		   (FALSE == scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch))
		{
			ConfigService.Menu.SLAlarmSwitch = 
			scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
		}else if((TRUE == scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)&&
				 (TRUE == scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch))
		{
			ConfigService.Menu.SLAlarmSwitch = 
			scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
		}
		/* 设置声光报警总开光 */
		ConfigService.Menu.SLAlarmSwitch = 
		scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
	}
		/* 短信报警开关配置*/
	if(ConfigService.Menu.SMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)
	{
		/* 短信报警总开光不一致,设置 菜单信息 修改项 */
		item  = item|ConfigService_UploadMenu;
		/* 设置短信报警总开光 */
		ConfigService.Menu.SMSAlarmSwitch = 
		scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch;
	}
	
	if(ConfigService.Menu.SMSAlarm_RepeatTime !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval)
	{
		/* 短信重复报警时间间隔不一致,设置 菜单信息 修改项 */
		item  = item|ConfigService_UploadMenu;
		/* 设置短信报警总开光 */
		ConfigService.Menu.SMSAlarm_RepeatTime = 
		scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval;
	}
	
	if(ConfigService.Menu.SaveInterval_Alarm !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval)
	{
		/* 报警存储间隔不一致,设置 菜单信息 修改项 */
		item  = item|ConfigService_UploadMenu;
		/* 设置报警存储间隔 */
		ConfigService.Menu.SaveInterval_Alarm = 
		scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval;
	}
	
	if(ConfigService.Menu.SaveInterval_Normal !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval)
	{

		/* 正常存储间隔不一致,设置 菜单信息 修改项 */
		item  = item|ConfigService_UploadMenu;
		/* 设置正常存储间隔 */
		ConfigService.Menu.SaveInterval_Normal = 
		scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval;
	}
	
/*********************GSM模块信息***********************************/
	if(memcmp(&ConfigService.Gsm.Data_IP_Address,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1,4)!=0)
	{
		/*数据服务器ip不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置数据服务器IP地址*/
		 ConfigService.Gsm.Data_IP_Address=scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1;
	}
	if(memcmp(&ConfigService.Gsm.DataDomainName,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,sizeof(ConfigService.Gsm.DataDomainName))!=0)
	{
		/*数据服务器域名不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置数据服务器域名地址*/
		memcpy(ConfigService.Gsm.DataDomainName,scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
		    sizeof(ConfigService.Gsm.DataDomainName));
	}
	if(ConfigService.Gsm.DataNetworkPort!=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1))
	{
		/*数据服务器端口不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置数据服务器端口地址*/
	 ConfigService.Gsm.DataNetworkPort=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1);
	
	}
	
	if(memcmp(&ConfigService.Gsm.Config_IP_Address,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2,4)!=0)
	{
		/*配置服务器ip不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置配置服务器IP地址*/
		 ConfigService.Gsm.Config_IP_Address=scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2;
	}
	if(memcmp(&ConfigService.Gsm.ConfigDomainName,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,sizeof(ConfigService.Gsm.ConfigDomainName))!=0)
	{
		/*配置服务器域名不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置配置服务器域名地址*/
		memcpy(ConfigService.Gsm.ConfigDomainName,scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
		    sizeof(ConfigService.Gsm.ConfigDomainName));
	}
	if(ConfigService.Gsm.ConfigNetworkPort!=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2))
	{
		/*配置服务器端口不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置配置服务器端口地址*/
	 ConfigService.Gsm.ConfigNetworkPort=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2);
	
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO1,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1,sizeof(ConfigService.Gsm.Alarm_SMS_NO1))!=0)
	{
		/*配置服务器短信报警号码1不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置配置服务器短信报警号码1内容*/
		 ConfigService.Gsm.Alarm_SMS_NO1=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1;
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO2,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2,sizeof(ConfigService.Gsm.Alarm_SMS_NO2))!=0)
	{
		/*配置服务器短信报警号码2不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置配置服务器短信报警号码2内容*/
		 ConfigService.Gsm.Alarm_SMS_NO2=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2;
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO3,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3,sizeof(ConfigService.Gsm.Alarm_SMS_NO3))!=0)
	{
		/*配置服务器短信报警号码3不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置配置服务器短信报警号码3内容*/
		 ConfigService.Gsm.Alarm_SMS_NO3=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3;
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO4,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4,sizeof(ConfigService.Gsm.Alarm_SMS_NO4))!=0)
	{
		/*配置服务器短信报警号码4不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置配置服务器短信报警号码4内容*/
		 ConfigService.Gsm.Alarm_SMS_NO4=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4;
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO5,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5,sizeof(ConfigService.Gsm.Alarm_SMS_NO5))!=0)
	{
		/*配置服务器短信报警号码5不一致,设置 GSM模块信息 修改项 */
		 item  = item|ConfigService_Gsm;
		/* 设置配置服务器短信报警号码5内容*/
		 ConfigService.Gsm.Alarm_SMS_NO5=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5;
	}
/******************************************************************/
	/* 初始化 配置服务器 升级信息 */
	if(ConfigService.Update.Flag !=
	   scrPara->Inf_State.Instru_System.Inf.UpdateFlag)
	{

		/* 正常存储间隔不一致,设置 菜单信息 修改项 */
		item  = item|ConfigService_UploadUpdata;
		/* 设置正常存储间隔 */
		ConfigService.Update.Flag = 
		scrPara->Inf_State.Instru_System.Inf.UpdateFlag;
	}

	return item;
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
	 
	/* 数据解析状态 */
	BOOL state = FALSE;
	/* 操作类型 */
	uint8_t cmdType;
	/* 操作内容:空包/非空包 */
	BOOL comContent = FALSE;
	/* 判断通信状态 */
	if(gprs.ConfigService.State.Result == TRUE)
	{
		/* 通信成功 */
		/* 判断当前配置服务器业务流程 */
		if(ConfigService_App.SYNC_State == FALSE)//32位设备判断与服务器同步信息的操作是否完成
		{
			/* 同步状态 */ /* 解析数据 */
			state = ConfigService_ParseData( gprs.ConfigService.Para.RXDR,
											                gprs.ConfigService.Para.RXLen,
											                                  &comContent,
											                                    &cmdType);
			
			/* 判断数据解析状态 */
			if(FALSE == state)
			{
				/* 1、数据解析失败 *//* 2、回复平台信息 */
				/* 清除配置服务器正在运行状态标志 */
				ConfigService_StopRun();
				/* 返回 */
				return;
			}
			
			/* 判断操作内容是否为空包数据 */
			if(comContent == TRUE)
			{
				/* 当前为非空操作 */
				/* 判断操作类型 */
				if(cmdType == 0x00)//判断操作类型是否为同步操作
				{
					/* 同步类型 */
					/* 更新当前的配置 */
					ConfigService_UpdataPara(scrPara);
					/* 仪器与平台已经同步完成，设置同步状态标志. */
					ConfigService_App.SYNC_State = TRUE;
					/* 清除配置服务器正在运行状态标志 */
					ConfigService_StopRun();
					//这里我需要给平台回复吗？
				}else if(cmdType == 0x01)
				{
					  ConfigService_App.SYNC_State	=TRUE;//zwc强制设置同步成功(因为目前没有32位设置同步配置服务器平台信息功能)	
					
					/* 修改类型 *///现在暂时没有这个功能.
					/* 清除配置服务器正在运行状态标志 */
					ConfigService_StopRun();
				}else if(cmdType == 0x02)
				{
					/* 回复类型 *///结束同步过程
					/* 这个阶段收到回复说明仪器与平台已经同步完成，设置同步状态标志. */
					ConfigService_App.SYNC_State = TRUE;
					/* 清除配置服务器正在运行状态标志 */
					ConfigService_StopRun();
				}else
				{
					/* 未知类型 */
					/* 清除配置服务器正在运行状态标志 */
					ConfigService_StopRun();
				}
			}else
			{
				/* 当前为空包操作 */
				/* 上传当设备配置信息(同步平台参数) */
				ConfigService_Cmd(ConfigService_UploadAll,0x01);
			}
			
		}
		else//32位设备与服务器同步操作已经完成
		{
			/* 修改状态 */
			/* 解析数据 */
			state = ConfigService_ParseData(gprs.ConfigService.Para.RXDR,
											                gprs.ConfigService.Para.RXLen,
											                &comContent,
											                &cmdType);
			/* 判断数据解析状态 */
			if(FALSE == state)
			{
				/* 1、数据解析失败 *//* 2、回复平台信息 */
				/* 清除配置服务器正在运行状态标志 */
				ConfigService_StopRun();
				/* 返回 */
				return;
			}	
			/* 判断操作内容是否为空包数据 */
			if(comContent == TRUE)
			{
				/* 当前为非空操作 */
				/* 判断操作类型 */
				if(cmdType == 0x00)
				{
					/* 同步类型 *///现在暂时没有这个功能.
					/* 清除配置服务器正在运行状态标志 */
					ConfigService_StopRun();
				}else if(cmdType == 0x01)
				{
					/* 修改类型 */
					/* 更改项目类型寄存器 */
					uint8_t item = 0;
					/* 更新当前的配置 */
					ConfigService_UpdataPara(scrPara);
					/* 根据当前仪器配置表同步平台配置 */
					item = ConfigService_CheckParaModify(scrPara);
					
					if(GSM_DEBUG_Flag ==TRUE)
					{
						printf("已经修改完成服务器下发的配置参数\n");
					}
					/* 判断配置信息是否有修改项 */
					if(ConfigService_UploadNull != item)
					{
						/* 配置信息有修改项，上传当前设备配置修改信息(同步平台参数) */
						ConfigService_Cmd(item,0x01);
					}else//设备回复平台,设备已经收到了它下发的消息
					{
						/* 配置信息无修改项，回复信息 */
//				  ConfigService_Cmd(ConfigService_UploadAll,0x02);
//						GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,ConfigService_EVENT);	
						
						Server_Send_Data(
								              (char*)GPRS.ConfigService.Para.TXDR,
								              (GPRS.ConfigService.Para.TXLen),
								              GSM_GPRS_Config
								            );
						
	    	    IAP_CheckFlag();/* 检测固件升级状态 若满足条件则开始进行远程升级 */	
					  if(GSM_DEBUG_Flag ==TRUE)
					  {
						    printf("设备回复平台,设备已经收到了它下发的消息\r\n");
					  } 
					 
					}
				}else if(cmdType == 0x02)
				{
					/* 仪器与平台已经同步完成，设置同步状态标志. */
			    ConfigService_App.SYNC_State = TRUE;//设备重启后默认会发一次系统所有信息  将同步状态该为已同步
					/* 回复类型 *///修改同步过程
					/* 清除配置服务器正在运行状态标志 */
					ConfigService_App.Run_State = FALSE;		
					/* 判断当前固件升级状态 */
					if(IAP_UpdateSatte() == TRUE)
					{
						/* 清除固件升级状态 */
						scrPara->Inf_State.Instru_System.Inf.UpdateFlag = FALSE;
						/* 更新存储配置 */
						Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_System.Inf.UpdateFlag,
						sizeof(scrPara->Inf_State.Instru_System.Inf.UpdateFlag));
						mcu_reset();//重启设备  复位单片机zwc
					}
				}else
				{
					/* 未知类型 */
					/* 清除配置服务器正在运行状态标志 */
					ConfigService_StopRun();
				}
			}else
			{
				/* 当前为空包操作 */	/* 更改项目类型寄存器 */
				uint8_t item = 0;
				/* 根据当前仪器配置表同步平台配置 */
				item = ConfigService_CheckParaModify(scrPara);
				/* 判断配置信息是否有修改项 */
				if(ConfigService_UploadNull != item)
				{
					/* 配置信息有修改项，上传当设备配置修改信息(同步平台参数) */
					ConfigService_Cmd(item,0x01);
				}else
				{
					/* 配置信息无修改项 */
					/* 清除配置服务器正在运行状态标志 */
					ConfigService_StopRun();
				}
			}
		}
	}else
	{
		/* 通信失败 */
		/* 清除配置服务器正在运行状态标志 */
		ConfigService_StopRun();
	}
	

}

/************************************************************************************************/
/* 函数名	: ConfigService_InitPara												 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 无		 																		*/
/* 作用	 	: 配置服务器接收数据业务流程								 							*/
/* 创建日期 	: 2016/08/16																		*/
/* 修改日期 	: 2016/08/16																		*/
/*   版本 	: V0: 创建“数据业务”逻辑															*/
/************************************************************************************************/
void ConfigService_InitPara(INSTRU_CONFIG_INF_TAB* scrPara)
{
	/* 初始化 配置服务器 系统信息 */
	ConfigService.System.SerialNumber = 
	scrPara->Inf_State.Instru_System.Inf.Serial_Number;
	ConfigService.System.HardwareVersion = 
	scrPara->Inf_State.Instru_System.Inf.Hardware_Version;
	ConfigService.System.SoftwareVersion = 
	scrPara->Inf_State.Instru_System.Inf.Software_Version;
	
	/* 初始化 配置服务器 电源信息 */
	ConfigService.Power.PowerState = TRUE;
	ConfigService.Power.PowerAlarmSwitch = TRUE;
	if((scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch == TRUE)&&
	   (scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch == TRUE))
	{
		ConfigService.Power.PowerSLAlarmSwitch = TRUE;
	}else
	{
		ConfigService.Power.PowerSLAlarmSwitch = FALSE;
	}

	ConfigService.Power.PowerSMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch;
	
	ConfigService.Power.PowerQuantityState =  
	scrPara->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	
	ConfigService.Power.BTAlarmSwitch = TRUE;
	if((scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch == TRUE)&&
	   (scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch == TRUE))
	{
		ConfigService.Power.BTSLAlarmSwitch = TRUE;
	}else
	{
		ConfigService.Power.BTSLAlarmSwitch = FALSE;
	}
	ConfigService.Power.BTSMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch;
	
	/* 初始化 配置服务器 传感器信息 */
#if (1)
	/* 探头 1 */
	ConfigService.Sensor[0].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type;
	ConfigService.Sensor[0].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Alarm_Switch;
	ConfigService.Sensor[0].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[0].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[0].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[0].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[0];
	ConfigService.Sensor[0].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[1];
	ConfigService.Sensor[0].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[2];
	ConfigService.Sensor[0].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[3];
	ConfigService.Sensor[0].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[0].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit;
	ConfigService.Sensor[0].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit;

	/* 探头 2 */
	ConfigService.Sensor[1].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type;
	ConfigService.Sensor[1].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Alarm_Switch;
	ConfigService.Sensor[1].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[1].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[1].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[1].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[0];
	ConfigService.Sensor[1].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[1];
	ConfigService.Sensor[1].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[2];
	ConfigService.Sensor[1].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[3];
	ConfigService.Sensor[1].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[1].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit;
	ConfigService.Sensor[1].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit;

	/* 探头 3 */
	ConfigService.Sensor[2].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type;
	ConfigService.Sensor[2].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Alarm_Switch;
	ConfigService.Sensor[2].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[2].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[2].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[2].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[0];
	ConfigService.Sensor[2].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[1];
	ConfigService.Sensor[2].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[2];
	ConfigService.Sensor[2].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[3];
	ConfigService.Sensor[2].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[2].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit;
	ConfigService.Sensor[2].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit;


	/* 探头 4 */
	ConfigService.Sensor[3].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type;
	ConfigService.Sensor[3].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Alarm_Switch;
	ConfigService.Sensor[3].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[3].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[3].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[3].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[0];
	ConfigService.Sensor[3].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[1];
	ConfigService.Sensor[3].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[2];
	ConfigService.Sensor[3].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[3];
	ConfigService.Sensor[3].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[3].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit;
	ConfigService.Sensor[3].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit;


	/* 探头 5 */
	ConfigService.Sensor[4].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type;
	ConfigService.Sensor[4].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Alarm_Switch;
	ConfigService.Sensor[4].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[4].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[4].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[4].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[0];
	ConfigService.Sensor[4].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[1];
	ConfigService.Sensor[4].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[2];
	ConfigService.Sensor[4].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[3];
	ConfigService.Sensor[4].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[4].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit;
	ConfigService.Sensor[4].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit;

	/* 探头 6 */
	ConfigService.Sensor[5].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type;
	ConfigService.Sensor[5].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Alarm_Switch;
	ConfigService.Sensor[5].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[5].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[5].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[5].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[0];
	ConfigService.Sensor[5].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[1];
	ConfigService.Sensor[5].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[2];
	ConfigService.Sensor[5].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[3];
	ConfigService.Sensor[5].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[5].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit;
	ConfigService.Sensor[5].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit;


	/* 探头 7 */
	ConfigService.Sensor[6].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type;
	ConfigService.Sensor[6].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Alarm_Switch;
	ConfigService.Sensor[6].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[6].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[6].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[6].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[0];
	ConfigService.Sensor[6].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[1];
	ConfigService.Sensor[6].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[2];
	ConfigService.Sensor[6].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[3];
	ConfigService.Sensor[6].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[6].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit;
	ConfigService.Sensor[6].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit;


	/* 探头 8 */
	ConfigService.Sensor[7].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type;
	ConfigService.Sensor[7].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Alarm_Switch;
	ConfigService.Sensor[7].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[7].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[7].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[7].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[0];
	ConfigService.Sensor[7].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[1];
	ConfigService.Sensor[7].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[2];
	ConfigService.Sensor[7].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[3];
	ConfigService.Sensor[7].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[7].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit;
	ConfigService.Sensor[7].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit;

	/* 探头 9 */
	ConfigService.Sensor[8].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type;
	ConfigService.Sensor[8].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Alarm_Switch;
	ConfigService.Sensor[8].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[8].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[8].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[8].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[0];
	ConfigService.Sensor[8].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[1];
	ConfigService.Sensor[8].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[2];
	ConfigService.Sensor[8].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[3];
	ConfigService.Sensor[8].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[8].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit;
	ConfigService.Sensor[8].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit;

	/* 探头 10 */
	ConfigService.Sensor[9].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type;
	ConfigService.Sensor[9].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Alarm_Switch;
	ConfigService.Sensor[9].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[9].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[9].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[9].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[0];
	ConfigService.Sensor[9].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[1];
	ConfigService.Sensor[9].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[2];
	ConfigService.Sensor[9].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[3];
	ConfigService.Sensor[9].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[9].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit;
	ConfigService.Sensor[9].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit;

	/* 探头 11 */
	ConfigService.Sensor[10].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type;
	ConfigService.Sensor[10].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Alarm_Switch;
	ConfigService.Sensor[10].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[10].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[10].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[10].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[0];
	ConfigService.Sensor[10].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[1];
	ConfigService.Sensor[10].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[2];
	ConfigService.Sensor[10].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[3];
	ConfigService.Sensor[10].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[10].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit;
	ConfigService.Sensor[10].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit;

	/* 探头 12 */
	ConfigService.Sensor[11].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type;
	ConfigService.Sensor[11].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Alarm_Switch;
	ConfigService.Sensor[11].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[11].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[11].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[11].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[0];
	ConfigService.Sensor[11].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[1];
	ConfigService.Sensor[11].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[2];
	ConfigService.Sensor[11].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[3];
	ConfigService.Sensor[11].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[11].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit;
	ConfigService.Sensor[11].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit;

	/* 探头 13 */
	ConfigService.Sensor[12].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type;
	ConfigService.Sensor[12].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Alarm_Switch;
	ConfigService.Sensor[12].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[12].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[12].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[12].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[0];
	ConfigService.Sensor[12].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[1];
	ConfigService.Sensor[12].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[2];
	ConfigService.Sensor[12].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[3];
	ConfigService.Sensor[12].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[12].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit;
	ConfigService.Sensor[12].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit;

	/* 探头 14 */
	ConfigService.Sensor[13].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type;
	ConfigService.Sensor[13].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Alarm_Switch;
	ConfigService.Sensor[13].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[13].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[13].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[13].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[0];
	ConfigService.Sensor[13].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[1];
	ConfigService.Sensor[13].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[2];
	ConfigService.Sensor[13].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[3];
	ConfigService.Sensor[13].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[13].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit;
	ConfigService.Sensor[13].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit;

	/* 探头 15 */
	ConfigService.Sensor[14].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type;
	ConfigService.Sensor[14].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Alarm_Switch;
	ConfigService.Sensor[14].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[14].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[14].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[14].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[0];
	ConfigService.Sensor[14].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[1];
	ConfigService.Sensor[14].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[2];
	ConfigService.Sensor[14].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[3];
	ConfigService.Sensor[14].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[14].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit;
	ConfigService.Sensor[14].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit;

	/* 探头 16 */
	ConfigService.Sensor[15].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type;
	ConfigService.Sensor[15].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Alarm_Switch;
	ConfigService.Sensor[15].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[15].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[15].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[15].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[0];
	ConfigService.Sensor[15].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[1];
	ConfigService.Sensor[15].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[2];
	ConfigService.Sensor[15].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[3];
	ConfigService.Sensor[15].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[15].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit;
	ConfigService.Sensor[15].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit;

	/* 探头 17 */
	ConfigService.Sensor[16].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type;
	ConfigService.Sensor[16].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Alarm_Switch;
	ConfigService.Sensor[16].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[16].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[16].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[16].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[0];
	ConfigService.Sensor[16].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[1];
	ConfigService.Sensor[16].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[2];
	ConfigService.Sensor[16].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[3];
	ConfigService.Sensor[16].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[16].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit;
	ConfigService.Sensor[16].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit;

	/* 探头 18 */
	ConfigService.Sensor[17].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type;
	ConfigService.Sensor[17].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Alarm_Switch;
	ConfigService.Sensor[17].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[17].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[17].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[17].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[0];
	ConfigService.Sensor[17].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[1];
	ConfigService.Sensor[17].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[2];
	ConfigService.Sensor[17].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[3];
	ConfigService.Sensor[17].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[17].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit;
	ConfigService.Sensor[17].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit;

	/* 探头 19 */
	ConfigService.Sensor[18].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type;
	ConfigService.Sensor[18].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Alarm_Switch;
	ConfigService.Sensor[18].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[18].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[18].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[18].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[0];
	ConfigService.Sensor[18].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[1];
	ConfigService.Sensor[18].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[2];
	ConfigService.Sensor[18].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[3];
	ConfigService.Sensor[18].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[18].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit;
	ConfigService.Sensor[18].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit;

	/* 探头 20 */
	ConfigService.Sensor[19].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type;
	ConfigService.Sensor[19].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Alarm_Switch;
	ConfigService.Sensor[19].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[19].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[19].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[19].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[0];
	ConfigService.Sensor[19].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[1];
	ConfigService.Sensor[19].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[2];
	ConfigService.Sensor[19].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[3];
	ConfigService.Sensor[19].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[19].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit;
	ConfigService.Sensor[19].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit;

	/* 探头 21 */
	ConfigService.Sensor[20].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type;
	ConfigService.Sensor[20].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Alarm_Switch;
	ConfigService.Sensor[20].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[20].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[20].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[20].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[0];
	ConfigService.Sensor[20].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[1];
	ConfigService.Sensor[20].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[2];
	ConfigService.Sensor[20].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[3];
	ConfigService.Sensor[20].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[20].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit;
	ConfigService.Sensor[20].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit;

	/* 探头 22 */
	ConfigService.Sensor[21].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type;
	ConfigService.Sensor[21].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Alarm_Switch;
	ConfigService.Sensor[21].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[21].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[21].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[21].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[0];
	ConfigService.Sensor[21].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[1];
	ConfigService.Sensor[21].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[2];
	ConfigService.Sensor[21].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[3];
	ConfigService.Sensor[21].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[21].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit;
	ConfigService.Sensor[21].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit;

	/* 探头 23 */
	ConfigService.Sensor[22].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type;
	ConfigService.Sensor[22].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Alarm_Switch;
	ConfigService.Sensor[22].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[22].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[22].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[22].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[0];
	ConfigService.Sensor[22].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[1];
	ConfigService.Sensor[22].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[2];
	ConfigService.Sensor[22].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[3];
	ConfigService.Sensor[22].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[22].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit;
	ConfigService.Sensor[22].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit;

	/* 探头 24 */
	ConfigService.Sensor[23].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type;
	ConfigService.Sensor[23].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Alarm_Switch;
	ConfigService.Sensor[23].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[23].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[23].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[23].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[0];
	ConfigService.Sensor[23].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[1];
	ConfigService.Sensor[23].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[2];
	ConfigService.Sensor[23].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[3];
	ConfigService.Sensor[23].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[23].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit;
	ConfigService.Sensor[23].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit;

	/* 探头 25 */
	ConfigService.Sensor[24].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type;
	ConfigService.Sensor[24].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Alarm_Switch;
	ConfigService.Sensor[24].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[24].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[24].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[24].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[0];
	ConfigService.Sensor[24].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[1];
	ConfigService.Sensor[24].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[2];
	ConfigService.Sensor[24].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[3];
	ConfigService.Sensor[24].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[24].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit;
	ConfigService.Sensor[24].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit;

	/* 探头 26 */
	ConfigService.Sensor[25].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type;
	ConfigService.Sensor[25].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Alarm_Switch;
	ConfigService.Sensor[25].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[25].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[25].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[25].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[0];
	ConfigService.Sensor[25].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[1];
	ConfigService.Sensor[25].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[2];
	ConfigService.Sensor[25].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[3];
	ConfigService.Sensor[25].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[25].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit;
	ConfigService.Sensor[25].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit;

	/* 探头 27 */
	ConfigService.Sensor[26].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type;
	ConfigService.Sensor[26].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Alarm_Switch;
	ConfigService.Sensor[26].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[26].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[26].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[26].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[0];
	ConfigService.Sensor[26].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[1];
	ConfigService.Sensor[26].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[2];
	ConfigService.Sensor[26].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[3];
	ConfigService.Sensor[26].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[26].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit;
	ConfigService.Sensor[26].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit;

	/* 探头 28 */
	ConfigService.Sensor[27].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type;
	ConfigService.Sensor[27].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Alarm_Switch;
	ConfigService.Sensor[27].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[27].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[27].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[27].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[0];
	ConfigService.Sensor[27].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[1];
	ConfigService.Sensor[27].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[2];
	ConfigService.Sensor[27].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[3];
	ConfigService.Sensor[27].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[27].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit;
	ConfigService.Sensor[27].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit;

	/* 探头 29 */
	ConfigService.Sensor[28].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type;
	ConfigService.Sensor[28].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Alarm_Switch;
	ConfigService.Sensor[28].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[28].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[28].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[28].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[0];
	ConfigService.Sensor[28].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[1];
	ConfigService.Sensor[28].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[2];
	ConfigService.Sensor[28].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[3];
	ConfigService.Sensor[28].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[28].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit;
	ConfigService.Sensor[28].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit;

	/* 探头 30 */
	ConfigService.Sensor[29].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type;
	ConfigService.Sensor[29].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Alarm_Switch;
	ConfigService.Sensor[29].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[29].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[29].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[29].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[0];
	ConfigService.Sensor[29].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[1];
	ConfigService.Sensor[29].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[2];
	ConfigService.Sensor[29].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[3];
	ConfigService.Sensor[29].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[29].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit;
	ConfigService.Sensor[29].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit;

	/* 探头 31 */
	ConfigService.Sensor[30].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type;
	ConfigService.Sensor[30].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Alarm_Switch;
	ConfigService.Sensor[30].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[30].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[30].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[30].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[0];
	ConfigService.Sensor[30].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[1];
	ConfigService.Sensor[30].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[2];
	ConfigService.Sensor[30].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[3];
	ConfigService.Sensor[30].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[30].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit;
	ConfigService.Sensor[30].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit;

	/* 探头 32 */
	ConfigService.Sensor[31].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type;
	ConfigService.Sensor[31].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Alarm_Switch;
	ConfigService.Sensor[31].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[31].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[31].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[31].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[0];
	ConfigService.Sensor[31].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[1];
	ConfigService.Sensor[31].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[2];
	ConfigService.Sensor[31].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[3];
	ConfigService.Sensor[31].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[31].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit;
	ConfigService.Sensor[31].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit;

#endif


	/* 初始化 配置服务器 菜单信息 */
	ConfigService.Menu.SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
	ConfigService.Menu.SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch;
	ConfigService.Menu.SMSAlarm_RepeatTime = 
	scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval;
	ConfigService.Menu.SaveInterval_Alarm = 
	scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval;
	ConfigService.Menu.SaveInterval_Normal = 
	scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval;
	/* 初始化 配置服务器 GSM模块信息 */
		//1.1、数据服务器IP地址
	ConfigService.Gsm.Data_IP_Address	=	scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1;
	  //1.2、数据服务器域名
		                   memcpy(ConfigService.Gsm.DataDomainName,
		scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			                   sizeof(ConfigService.Gsm.DataDomainName));
	  //1.3、数据服务器端口号 		 
	ConfigService.Gsm.DataNetworkPort=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1);
		//2.1、配置服务器IP地址
	ConfigService.Gsm.Config_IP_Address	=	scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2;
	  //2.2、配置服务器域名
		                   memcpy(ConfigService.Gsm.ConfigDomainName,
		scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			                   sizeof(ConfigService.Gsm.ConfigDomainName));
	  //2.3、配置服务器端口号 		 
	ConfigService.Gsm.ConfigNetworkPort=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2);
    //3.1、短信报警号码	
  ConfigService.Gsm.Alarm_SMS_NO1=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1;
  ConfigService.Gsm.Alarm_SMS_NO2=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2;
  ConfigService.Gsm.Alarm_SMS_NO3=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3;
  ConfigService.Gsm.Alarm_SMS_NO4=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4;
  ConfigService.Gsm.Alarm_SMS_NO5=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5;
/******************************************************************************/	
	/* 初始化 程序远程升级状态标志位  */
	ConfigService.Update.Flag = FALSE;
	
	if(scrPara->Inf_State.Instru_System.Inf.UpdateFlag != FALSE)
	{
		scrPara->Inf_State.Instru_System.Inf.UpdateFlag = FALSE;
		/* 更新存储配置 */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_System.Inf.UpdateFlag,
		sizeof(scrPara->Inf_State.Instru_System.Inf.UpdateFlag));
	}
	
	memset(ConfigService.Update.DocumentName,
		   0x00,
		   sizeof(ConfigService.Update.DocumentName));
	memset(ConfigService.Update.Network,
		   0x00,
		   sizeof(ConfigService.Update.Network));
	ConfigService.Update.Port_HSB = 0;
	ConfigService.Update.Port_LSB = 0;
	
	/* 初始化固件升级参数指针 */
	IAP_ParaInit(&ConfigService.Update,&scrPara->Inf_State.Instru_System.Inf.UpdateFlag);
}

#endif

#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
