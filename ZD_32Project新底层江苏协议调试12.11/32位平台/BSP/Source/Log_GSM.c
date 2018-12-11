/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/11/20																			 */
/* 修改日期：2016/07/25																			 */
/*************************************************************************************************/
#include "interface.h"
#include "BSP.h"
#include "app.h"

#include "Ks0108.h"
#include "Log_GSM.h"


#include "Dev_RTC.h"//2018.12.04  ByZWC  Add
/***********更改底层增加的头文件*************/
#include "GSM_app.h"
#include "Gsm_Interface.h"
#include "Gsm_Api.h"
/****************************/

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
	#include "Log_ConfigService.h"
	#include "ComProtocol_JS.h"//江苏省免疫规划数据中心协议
#elif (AppGPRSComProtocol == 0x01)
	#include "Log_ConfigService_Http.h"
#elif (AppGPRSComProtocol == 0x02)
	#include "ComProtocol_ZDE.h"
#endif
#endif

#if (AppGYForwardEnable == 0x01)
#include "ComProtocol_GY.h"
#include "App_Save.h"
#endif

#if (ComProtocol_WZ_Enable == 0x01)
#include "ComProtocol_WZ.h"
#include "App_Save.h"
#endif

#include "Log_Download.h"
#include "Log_HTTP.h"
#include "Log_IAP.h"
#include "Log_Save.h"
#include "Log_Sensor.h"
#include "Log_SMS.h"

#include "Dev_Flash.h"
#include "Dev_Fram.h"
#include "Dev_RTC.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern BOOL GSM_DEBUG_Flag;
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
//#define ntohs htons
//#define ntohl htohl

/* 声明文件 函数 */

/* 将仪器的传感器数据转换成网络序数据 */
static void GSM_Module_Config(INSTRU_GSM* gprs_inf);
//static void GPRS_Config_Server_Inf(INSTRU_GSM* gprs_inf);

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
static void GPRS_Data_To_Network(void);
#endif

static BOOL GPRS_Parsing_Network_Data(void);//解析服务器返回的数据指令是否合法
 void GPRS_Send_Network_Data(void);//向服务器发送温室度数据
//static void Remote_Upgrade_Program(void);//向服务器发送特定的指令
#if ((GSM_Module_Type==0)||(GSM_Module_Type==3)) //SIM800C或者中兴模块
static void GSM_SMS_Send(INSTRU_GSM* gprs_inf);
#endif
/* GPRS发送时钟校准数据 */
static void GPRS_Send_System_Clock_Calibration_Data(void);
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))
/* 解析服务器返回的时间数据，并判断本次GPRS通信状态。 */
static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer);
#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x02))
extern CreateHTTPParaStruct HTTPPara;
#endif
#endif

/* 定义1K的数据 发送缓存 */
#define GPRS_Send_Sensor_Data_Buffer_Size	(1536u)
/* GPRS 发送数据缓存 */
static u8 GPRS_Send_Sensor_Data_Buffer[GPRS_Send_Sensor_Data_Buffer_Size];
/* 定义1500字节的数据 接收缓存(这里是根据底层的接收空间) */
#define GPRS_Rece_Sensor_Data_Buffer_Size	(1500u)
/* GPRS 接收数据缓存 */
static u8 GPRS_Rece_Sensor_Data_Buffer[GPRS_Rece_Sensor_Data_Buffer_Size];


/* 配置GSM 消息 */
static GSM_QUE GSM_QUE_Init_Module			                   = {Init_GSM_Module_EVENT,	((u32*)0X0),TRUE};
/* 更新信号强度 消息 */                                     
static GSM_QUE GSM_QUE_Update_Signal 		                   = {Update_SGL_EVENT,		  ((u32*)0X0),TRUE};
/* 更新附着状态 消息 */                                     
static GSM_QUE GSM_QUE_Update_Link 			                   = {Update_Link_EVENT,		  ((u32*)0X0),TRUE};
/* 上传温湿度数据 消息 */                                   
static GSM_QUE GSM_QUE_Update_Data 			                   = {Updata_Data_EVENT,		  ((u32*)0X0),TRUE};
/* 下载 程序更新 数据 */                                   
static GSM_QUE GSM_QUE_Download_IAP_Data	                 = {IAP_EVENT,				    ((u32*)0X0),TRUE};
/* 发送短信 数据 */                                        
static GSM_QUE GSM_QUE_Send_SMS				                     = {Send_SMS_EVENT,			    ((u32*)0X0),TRUE};
/* 发送配置 数据 */                                        
static GSM_QUE GSM_QUE_ConfigService		                   = {ConfigService_EVENT,		((u32*)0X0),TRUE};
/* 系统时钟校准信息 */
static GSM_QUE GSM_QUE_Send_System_Clock_Calibration	     = {System_Clock_Calibration_EVENT,	((u32*)0X0),TRUE};

#if (GSM_Module_Type==0x02) //ESP8266Wifi模块
/* 设置Wifi名字和密码*/
static GSM_QUE    Wifi_QUE_SetWifiNamePWD	                 = {SetWifiNamePWD_EVENT,	((u32*)0X0),TRUE};
/* 设置WifiWlan参数*/
static GSM_QUE    Wifi_QUE_SetWifiWLAN                     = {SetWifiWLAN_EVENT,	((u32*)0X0),TRUE};
/* 设置Wifi模块获取IP地址方式   STATIC或者DHCP模式参数*/
static GSM_QUE    Wifi_QUE_GET_IP_WAY                      = {SetWifiWLAN_GET_IP_WAY_EVENT,	((u32*)0X0),TRUE};
/*复位Wifi模块*/
static GSM_QUE    Wifi_QUE_Reset                           = {RestWifi_EVENT,	((u32*)0X0),TRUE};
/* Wifi模块恢复出厂设置*/
static GSM_QUE    Wifi_QUE_Factory_Data_Reset              = {Factory_Data_Reset_Wifi_EVENT,	((u32*)0X0),TRUE};
#endif	



//GSM消息 标识码
#define	GPRS_COM_Identify			(0X7E)
//GSM 转义 标号
#define	GSM_Translator_Symbol_0X7E	GPRS_COM_Identify
#define	GSM_Translator_Symbol_0X02	0X02	
#define	GSM_Translator_Symbol_0X7D	0X7D
#define	GSM_Translator_Symbol_0X01	0X01
//GSM消息 消息头 消息ID(0X0900:上行通信;0X8900:下行通信;0X8001:通用应答)
#define	GPRS_COM_ID_Upload			   (0X0900)
#define	GPRS_COM_ID_Download		   (0X8900)
#define GPRS_COM_ID_ACK				     (0X8001)
//消息体长度（比如:消息类型+消息数据类型+仪器当前状态+时间戳+通道数+2个通道数据=19Byte）
#define GPRS_COM_Body_Len			     (0u)
//消息 流水号
#define GPRS_COM_Serial_Num			   (0u)
//消息体属性 数据加密方式(不加密)
#define GPRS_COM_Enc_Mode			     (0u)
//消息体属性 数据分包标志(部分表)
#define GPRS_COM_Sub_Flag			     (0u)
//消息类型:WIFI 卡片式传感器
#define GPRS_COM_Message_Type		   (0XF0)


#if (AppGPRSComProtocol == 0x00)//部标协议
//消息类型:32位平台温湿度数据
#define GPRS_32Platforms_Type		(0XFC)//GPRS_ZWC
#endif

#if (AppGPRSComProtocol == 0x03)//河北视窗协议
//消息类型:32位平台温湿度数据
#define GPRS_32Platforms_Type		(0XFD)//GPRS_ZWC
#endif


//消息数据类型:0:实时数据；1:历史数据
#define GPRS_COM_Data_Type			(0x01)
//传感器通道数量 
#define GPRS_COM_Sensor_Num			(Sensor_CH_Config_Num)
//应答 消息体长度 */
#define GPRS_COM_ACK_Att_Len		(5u)

// 系统内置的gprs服务器地址(GSM模块支持最多3个GPRS并发连接)
#define GPRS_Server_CONN_Num_Max	(MAX_NUM_TCP_CONN)//但是这里为什么设置成4..wo不是很理解。
// 服务器 编号
#define	GPRS_Server1_NO					(0u)
#define	GPRS_Server2_NO					(1u)
#define	GPRS_Server3_NO					(2u)


/************************************************************************************************/
/* GPRS 温湿度数据信息 数据寄存器 */
static GSM_GPRS_Send_Struct GPRS_Send_Reg;

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
/* GPRS 发送温湿度数据信息 数据寄存器固定配置 */
static const GSM_GPRS_Send_Struct GPRS_Send_Reg_Fix =
{
	/* 头 标识位 */ //(1 Byte)
	GPRS_COM_Identify,
	/* 通信 消息头 *///(16 Byte)
	/* 消息 ID(命令) *///(2 Byte)
	GPRS_COM_ID_Upload/0X100,
	GPRS_COM_ID_Upload%0X100,
	/* 消息体 属性 *///(2 Byte)
	//消息体属性 保留
	0X0,
	//消息体属性 分包
	GPRS_COM_Sub_Flag,
	//消息体属性 数据加密方式
	GPRS_COM_Enc_Mode,
	//消息体属性 消息体长度
	GPRS_COM_Body_Len/0X100,
	//消息体属性 消息体长度
	GPRS_COM_Body_Len%0X100,
	/* 终端序列号(SN号): */	//(6 Byte)
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* 消息 流水号 *///(2 Byte)
	GPRS_COM_Serial_Num/0X100,GPRS_COM_Serial_Num%0X100,
	
	/* 通信 消息体 */
	/* 消息类型:WIFI 卡片式传感器(现在只用这种模式) *///(1 Byte)
	GPRS_32Platforms_Type,
	/* 消息数据类型:0->实时数据；1->历史数据。 *///(1 Byte)
	GPRS_COM_Data_Type,
	/* 数据条数 *///(1 Byte)
	0,
	/* 数据内容(这个主要是为了给后面添加数据的指针) *///(1 Byte)
	0,
	
};
/************************************************************************************************/
/* GPRS 数据内容寄存器 */
static GSM_GPRS_Data_Content_Struct GPRS_Data_Content_Reg;


/* GPRS 数据内容寄存器 固定配置 */
static const GSM_GPRS_Data_Content_Struct GPRS_Data_Content_Reg_Fix = 
{
	/* 传感器(ID号): */	//(4 Byte)
	0,
	/* 外接电源状态 */	//默认0 不正常
	0,
	/* 预留1 */
	0,
	/* 预留2 */
	0,
	/* 电池电量 */		//默认0 不正常
	0,
	/* 时间戳 */			//(6 Byte)
	//年//(1 Byte)
	0,
	//月//(1 Byte)
	0,
	//日//(1 Byte)
	0,
	//时//(1 Byte)
	0,
	//分//(1 Byte)
	0,
	//秒//(1 Byte)
	0,
	/* 通道数 */
	0,
	/* 传感器类型 */
	0,
	/* 数据(这个主要是为了给后面添加数据的指针) */
	0.0,
};

#endif
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))
/* GPRS 接收温湿度数据信息 数据寄存器 */
static GSM_GPRS_Rece_Struct GPRS_Rece_Reg;
#endif
#endif
/************************************************************************************************/
/* GPRS 发送温湿度数据信息 数据寄存器 */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_Sensor_Data_Reg = 
{
	/* GPRS 接收 传感器 数据数量 */
	0,
	/* GPRS 发送 传感器 数据数量 */
	0,
	/* GPRS 发送传感器数据通信状态（FALSE:没有发送）*/
	FALSE,
	/* GPRS 主机序传感器数据等待处理状态（FALSE:没有数据处理） */
	FALSE,
	/* GPRS 网络序传感器数据等待处理状态（FALSE:没有数据处理） */
	FALSE,
	/* GPRS 发送传感器数据结果 */
	TRUE,
	/* GPRS 接收传感器数据结果 */
	TRUE,
	/* GPRS 解析数据状态 */
	TRUE,	
	/* GPRS 发送传感器数据 失败次数 */
	0,
};
/************************************************************************************************/
//static GSM_GPRS_SR_Struct GPRS_SR_Reg = //GSM_ZWC
 GSM_GPRS_SR_Struct GPRS_SR_Reg = 
{
	/* GPRS 信号 强度 */
	0,
	/* GPRS 信号 强度 显示(0-5:0格;6-8:1格;9-11:2格;12-14:3格;15-31:4格;) */
	0,
	/* GPRS 模块工作状态 */
	FALSE,
	/* GPRS 附着状态 */
	FALSE,
	/* 无GPRS信号 */
	SIM_NoSignal,
};
/************************************************************************************************/
////GPRS服务器信息
//static GPRS_Connection GPRS_COM_Reg[GPRS_Server_CONN_Num_Max];
/************************************************************************************************/
//GSM 短信 状态寄存器
GSM_SMS_SR_Struct GSM_SMS_SR_Reg = 
{
	FALSE,
#if (SMS_Function_Enable == 0x01)
	
#if (SMS_User_Number == 0x00)
#error "温湿度卡片通道配置不正确"
#endif	
	
#if (SMS_User_Number >= 0x01)
	TRUE,
#endif
	
#if (SMS_User_Number >= 0x02)
	TRUE,
#endif
	
#if (SMS_User_Number >= 0x03)
	TRUE,
#endif
	
#endif
};
/************************************************************************************************/
#if ((GSM_Module_Type==0)||(GSM_Module_Type==3)) //SIM800C或者中兴模块
//GSM 短信 数据寄存器
static GSM_SMS_DR_Struct GSM_SMS_DR_Reg;
#endif
/************************************************************************************************/
/* GPRS 应用寄存器 */
GPRS_Struct GPRS;
/************************************************************************************************/

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
/************************************************************************************************/
/* 函数名	: GPRS_Timestamp_Hex_To_BCD_Code													*/
/* 输入		: src:输入HEX码;dest:输出HEX码														*/
/* 输出		: 无 																				*/
/* 作用		: 将HEX的时间日期转换成BCD时间日期形式				 									*/
/* 创建日期	: 2015/03/30		HEX2BCD															*/
/************************************************************************************************/
static void GPRS_Timestamp_Hex_To_BCD_Code(GSM_GPRS_Data_Content_Struct* dest,Save_Sensor_Data_Struct* src)
{
	/* 获得秒数据 */
	dest->GPRS_DR_FIX.Timestamp_Sec 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS);
	/* 获得分数据 */                        
	dest->GPRS_DR_FIX.Timestamp_Min 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS);
	/* 获得时数据 */                        
	dest->GPRS_DR_FIX.Timestamp_Hour 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS);
	/* 获得日数据 */                        
	dest->GPRS_DR_FIX.Timestamp_Day 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS);
	/* 获得月数据 */                        
	dest->GPRS_DR_FIX.Timestamp_Month 	=   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS);
	/* 获得年数据 *///这里的年，最大保存63年，从2000年开始算起，超过将重新从2010开始算
	dest->GPRS_DR_FIX.Timestamp_Year 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS);
}
#endif

/************************************************************************************************/
/* 函数名	: GPRS_GET_Save_BSP_Sensor_Data														*/
/* 输入		: src:输入HEX码;dest:输出HEX码														*/
/* 输出		: 无 																				*/
/* 作用		: 获取存储的板载传感器数据							 									*/
/* 创建日期	: 2015/03/30																		*/
/************************************************************************************************/
#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
#if ((Sensor_Tmp_Num+Sensor_Hum_Num+Sensor_PT1000_Num) != 0)
static void GPRS_GET_Save_BSP_Sensor_Data(GSM_GPRS_Data_Content_Struct* dest,Save_Sensor_Data_Struct* src)
{
	/* 传感器数据存放数据的开始位置 */
	u8 sensor_data_offset = 0;
	/* GPRS数据包存放数据的位置 */
	u8 gprs_data_content_offset = 0;
	/* 温湿度数据 */
	float sensor_data;

	
/* 判断 通道1 */
#if 	((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
		(Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH1_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH1_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* 判断 通道2 */
#if 	((Sensor_CH2_Config_Type == Sensor_Tmp) ||\
(Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH2_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH2_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道3 */
#if 	((Sensor_CH3_Config_Type == Sensor_Tmp) ||\
(Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH3_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH3_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道4 */
#if 	((Sensor_CH4_Config_Type == Sensor_Tmp) ||\
(Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH4_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH4_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道5 */
#if 	((Sensor_CH5_Config_Type == Sensor_Tmp) ||\
(Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH5_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH5_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道6 */
#if 	((Sensor_CH6_Config_Type == Sensor_Tmp) ||\
(Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH6_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH6_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道7 */
#if 	((Sensor_CH7_Config_Type == Sensor_Tmp) ||\
(Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
(Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
		 (Sensor_CH7_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH7_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道8 */
#if 	((Sensor_CH8_Config_Type == Sensor_Tmp) ||\
(Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH8_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH8_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* 判断 通道9 */
#if 	((Sensor_CH9_Config_Type == Sensor_Tmp) ||\
(Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH9_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH9_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* 判断 通道10 */
#if 	((Sensor_CH10_Config_Type == Sensor_Tmp) ||\
(Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH10_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH10_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道11 */
#if 	((Sensor_CH11_Config_Type == Sensor_Tmp) ||\
(Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH11_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH11_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道12 */
#if 	((Sensor_CH12_Config_Type == Sensor_Tmp) ||\
(Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH12_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH12_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道13 */
#if 	((Sensor_CH13_Config_Type == Sensor_Tmp) ||\
(Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH13_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH13_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道14 */
#if 	((Sensor_CH14_Config_Type == Sensor_Tmp) ||\
(Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH14_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH14_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道15 */
#if 	((Sensor_CH15_Config_Type == Sensor_Tmp) ||\
(Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH15_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH15_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道16 */
#if 	((Sensor_CH16_Config_Type == Sensor_Tmp) ||\
(Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH16_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH16_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* 判断 通道17 */
#if 	((Sensor_CH17_Config_Type == Sensor_Tmp) ||\
(Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH17_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH17_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* 判断 通道18 */
#if 	((Sensor_CH18_Config_Type == Sensor_Tmp) ||\
(Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH18_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH18_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道19 */
#if 	((Sensor_CH19_Config_Type == Sensor_Tmp) ||\
(Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH19_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH19_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道20 */
#if 	((Sensor_CH20_Config_Type == Sensor_Tmp) ||\
(Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH20_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH20_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道21 */
#if 	((Sensor_CH21_Config_Type == Sensor_Tmp) ||\
(Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH21_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH21_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道22 */
#if 	((Sensor_CH22_Config_Type == Sensor_Tmp) ||\
(Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH22_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH22_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道23 */
#if 	((Sensor_CH23_Config_Type == Sensor_Tmp) ||\
(Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH23_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH23_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道24 */
#if 	((Sensor_CH24_Config_Type == Sensor_Tmp) ||\
(Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH24_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH24_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* 判断 通道25 */
#if 	((Sensor_CH25_Config_Type == Sensor_Tmp) ||\
(Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH25_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH25_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* 判断 通道26 */
#if 	((Sensor_CH26_Config_Type == Sensor_Tmp) ||\
(Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH26_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH26_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* 判断 通道27 */
#if 	((Sensor_CH27_Config_Type == Sensor_Tmp) ||\
(Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH27_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH27_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道28 */
#if 	((Sensor_CH28_Config_Type == Sensor_Tmp) ||\
(Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH28_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH28_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道29 */
#if 	((Sensor_CH29_Config_Type == Sensor_Tmp) ||\
(Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH29_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH29_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* 判断 通道30 */
#if 	((Sensor_CH30_Config_Type == Sensor_Tmp) ||\
(Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH30_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH30_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道31 */
#if 	((Sensor_CH31_Config_Type == Sensor_Tmp) ||\
(Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH31_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH31_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* 判断 通道32 */
#if 	((Sensor_CH32_Config_Type == Sensor_Tmp) ||\
(Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH32_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH32_Config_Type == Sensor_PT1000))
		//d、设置数据
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e、设置通道数
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


}

#endif
#endif
	
/************************************************************************************************/
/* 函数名	: GPRS_GET_RF_Humiture_Hum_Data														*/
/* 输入		: src:输入HEX码;dest:输出HEX码														*/
/* 输出		: 无 																				*/
/* 作用		: 查找对应的温湿度卡片的湿度通道					 									*/
/* 创建日期	: 2015/03/30																		*/
/************************************************************************************************/
#if ((Sensor_RF_Humiture_Num > 0)||\
	 (AppGPRSComProtocol == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
static u8 GPRS_GET_RF_Humiture_Hum_Data(u32 dest_sn,INSTRU_SENSOR *sensor_t)
{
/* 没有查到匹配的通道 */
#define Sensors_CH_Null (0xFF)
	
	
/* 判断 通道1 */
#if		((Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH1.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_1;
		}
#endif
	
/* 判断 通道2 */
#if		((Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH2.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_2;
		}
#endif
	
/* 判断 通道3 */
#if		((Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH3.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_3;
		}
#endif	
	
/* 判断 通道4 */
#if		((Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH4.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_4;
		}
#endif	
	
/* 判断 通道5 */
#if		((Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH5.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_5;
		}
#endif

/* 判断 通道6 */
#if		((Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH6.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_6;
		}
#endif

/* 判断 通道7 */
#if		((Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH7.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_7;
		}
#endif

/* 判断 通道8 */
#if		((Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH8.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_8;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH9.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_9;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH10.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_10;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH11.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_11;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH12.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_12;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH13.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_13;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH14.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_14;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH15.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_15;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH16.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_16;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH17.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_17;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH18.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_18;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH19.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_19;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH20.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_20;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH21.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_21;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH22.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_22;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH23.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_23;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH24.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_24;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH25.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_25;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH26.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_26;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH27.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_27;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH28.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_28;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH29.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_29;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH30.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_30;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH31.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_31;
		}
#endif
		
/* 判断 通道8 */
#if		((Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
		/* 判断 湿度通道SN号与目标的SN号是否相同 */
		if(sensor_t->Inf_State.CH32.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* 于目标的SN号相同，说明该湿度通道与目标为同一卡片 */
			return Sensors_CH_32;
		}
#endif
	
		/* 到最后，没有找到匹配的通道 */
		return Sensors_CH_Null;
#undef Sensors_CH_Null
}
#endif

/************************************************************************************************/
/* 函数名	：GPRS_Config_Server_Inf															*/
/* 输入		：GPRS服务器信息																		*/
/* 输出		：无 																				*/
/* 作用		：配置GPRS服务器域名、IP地址、端口。 									 		 		*/
/* 创建日期	：2015/04/10																		*/
/************************************************************************************************/
//static void GPRS_Config_Server_Inf(INSTRU_GSM* gprs_inf)
//{

//	/* 获取 GPRS 的 服务器信息 */
//	//获取域名
//	sprintf(GPRS_COM_Reg[GPRS_Server1_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID1);
//	sprintf(GPRS_COM_Reg[GPRS_Server2_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID2);
//	sprintf(GPRS_COM_Reg[GPRS_Server3_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID3);
//	//获取IP
//	if((gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server1_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server1_NO].ip,"");
//	}
//	
//	if((gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server2_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server2_NO].ip,"");
//	}
//	
//	if((gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"");
//		//sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"115.28.105.232");//2017.11.06将升级服务器的IP地址固定
//	}
//	
//	//获取端口
//	sprintf(GPRS_COM_Reg[GPRS_Server1_NO].port,"%05d",gprs_inf->Inf_State.Port_ID1);	
//	sprintf(GPRS_COM_Reg[GPRS_Server2_NO].port,"%05d",gprs_inf->Inf_State.Port_ID2);
//	sprintf(GPRS_COM_Reg[GPRS_Server3_NO].port,"%05d",gprs_inf->Inf_State.Port_ID3);
// // sprintf(GPRS_COM_Reg[GPRS_Server3_NO].port,"%05d",12342);//2017.11.06将升级服务器的端口固定
//	
//}

/************************************************************************************************/
/* 函数名	：GSM_Module_Config																	*/
/* 输入		：GPRS服务器信息																		*/
/* 输出		：无 																				*/
/* 作用		：配置 GSM 模块					 									 		 		*/
/* 创建日期	：2015/04/10																		*/
/************************************************************************************************/
static void GSM_Module_Config(INSTRU_GSM* gprs_inf)
{
	
	/* 清除 发送失败次数 */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum = 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum = 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum = 0;
	/* 配置 GPRS 服务器 信息 */
//	GPRS_Config_Server_Inf(gprs_inf);
	
	
	GSMConfig_Server_Inf(gprs_inf);
	
	
	
	/* 重新配置 GSM 模块 */
//	GPRS_SR_Reg.GPRS_Work_State = (BOOL)gsm_env_init(GPRS_COM_Reg,(_SCSStruct*)&GPRS_SR_Reg.SimState);
	GPRS_SR_Reg.GPRS_Work_State = (BOOL)Sim800c_env_init((_SCSStruct*)&GPRS_SR_Reg.SimState);	
	

	//这里的一些关键的的状态信息以后后用怕你屏幕显示出来提示，这些代码可以取消掉。***标志
	/* 判断 模块 配置 结果 */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		
//		printf("\rGSM模块初始化配置成功！！！\r\n");	
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* GSM模块 配置 成功 */
//			printf("\rGSM模块配置成功\r\n");						
//		}				
	}else
	{
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* GSM模块 配置 失败 */
//			printf("\rGSM模块初始化配置失败！！！\r\n");
//		}	
	}
}

/************************************************************************************************/
/* 函数名	SMConfig_Server_Inf(INSTRU_GSM* gprs_inf)													*/
/* 输入		：GPRS服务器信息																		*/
/* 输出		：无 																				*/
/* 作用		：配置GPRS服务器域名、IP地址、端口。 									 		 		*/
/* 创建日期	：2018/08/30																		*/
/************************************************************************************************/
void GSMConfig_Server_Inf(INSTRU_GSM* gprs_inf)
{

	/* 获取 GPRS 的 服务器信息 */
	//获取域名
	sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID1);
	sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID2);
	sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID3);
	//获取IP
	if((gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].ip,"");
	}
	
	if((gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].ip,"");
	}
	
	if((gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"");
		//sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"115.28.105.232");//2017.11.06将升级服务器的IP地址固定
	}
	
	//获取端口
	sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].port,"%d",gprs_inf->Inf_State.Port_ID1);	
	sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].port,"%d",gprs_inf->Inf_State.Port_ID2);
	sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].port,"%d",gprs_inf->Inf_State.Port_ID3);
 // sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].port,"%05d",12342);//2017.11.06将升级服务器的端口固定
	
}
//打印GSM模块发送和接收的数据信息
void PrintGsmDebug(char *wbuf, int wlen, char *rbuf, int *rlen,u8 Server_Id)
{
	      unsigned int i;
       	if(DEBUG_Flag ==TRUE)	
				{
				   switch(Server_Id)
					{
						 case 0:
						 {
							     if(wlen>0)//发送
							     {
							    	  printf("发送给数据服务器的数据:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("发送给数据服务器的数据长度为0！！！\n"); 
							     }
							     if(rlen>0)//接收
							     {
							    	  printf("接收到数据服务器的数据:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("接收到数据服务器的数据长度为0！！！\n"); 
							     }	 
							     break;
						 }
						  case 1:
						 {
							     if(wlen>0)//发送
							     {
							    	  printf("发送给配置服务器的数据:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("发送给配置服务器的数据长度为0！！！\n"); 
							     }
							     if(rlen>0)//接收
							     {
							    	  printf("接收到配置服务器的数据:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("接收到配置服务器的数据长度为0！！！\n"); 
							     }	 
							     break;
						 }
						 	case 2:
						 {
							     if(wlen>0)//发送
							     {
							    	  printf("发送给升级服务器的数据:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("发送给升级服务器的数据长度为0！！！\n"); 
							     }
							     if(rlen>0)//接收
							     {
							    	  printf("接收到升级服务器的数据:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("接收到升级服务器的数据长度为0！！！\n"); 
							     }	 
							     break;
						 }
						 
					
					   default:
						break;
					
					}
					
				
				
				
				}					

}

/************************************************************************************************/
/* 函数名	：GSM_Function																		*/
/* 输入		：gsm_que_inf:当前的消息队列															*/
/* 输出		：无 																				*/
/* 作用		：执行消息队列 当中的 消息功能			 								 		 		*/
/* 创建日期	：2015/04/01																		*/
/************************************************************************************************/
void GSM_Function(GSM_QUE*  gsm_que_inf,INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
//	/* 判断 当前 的电池电量 */
//	if(MCU_Battery.Power_State > Power_Null)
//	{
		/* 当前的电池电量能够维持 GSM模块正常工作。（大于3.5V） */
		/* 根据 消息队列 的命令类型 */
		switch(gsm_que_inf->EVENT_Cmd)
		{
			case Init_GSM_Module_EVENT:
			{
				
				/* GSM 初始化配置 */
				GSM_Module_Config(&config_tab->Inf_State.Instru_GSM);
				break;
			}

			default :
			{
				/* 由于后面的 功能 都是基于模块成功配置后才能正常使用的，因此先做判断 */
				if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
				{
					/* GSM 模块配置成功 */
					/* 根据 消息队列 的命令类型 */
					switch(gsm_que_inf->EVENT_Cmd)
					{
						//	/* 更新信号强度 */
						case Update_SGL_EVENT:
						{	
							/* 获得当前 信号强度 */
//						GPRS_SR_Reg.GPRS_Sign_Stre = gsm_get_signal();
						GPRS_SR_Reg.GPRS_Sign_Stre =Gsm_get_signal();
						break;
						}										
/*********************************************************************************/				
						//	/* 更新附着状态 */
						case Update_Link_EVENT:
						{
//							/* 获得当前的 附着状态 */
//						GPRS_SR_Reg.GPRS_Link_State = (BOOL)gsm_get_gprs_state();
			
//////////						GPRS_SR_Reg.GPRS_Link_State =(BOOL)Gsm_get_gprs_state();
						break;
						}
/*********************************************************************************/						
						//	/* 上传温湿度数据 */
						case Updata_Data_EVENT:
						{		
////						    	/* 发送数据 */
////					       GPRS_Send_Network_Data();
						break;
						}
/*********************************************************************************/		
						//	/* 下载 程序更新 数据 */
						case IAP_EVENT:
						{	
							#if (AppGPRSComProtocolEnable == 0x01)
              #if (AppGPRSComProtocol == 0x00)	
							/* 获取 更新程序 数据 */
							GPRS.IAP.State.Result = 
							(BOOL)Gsm_gprs_comm(
							((char*)GPRS.IAP.Para.TXDR),
							(GPRS.IAP.Para.TXLen),
							((char*)GPRS.IAP.Para.RXDR),
							((&GPRS.IAP.Para.RXLen)),
							 GSM_GPRS_Update
							);
							
              PrintGsmDebug(
							((char*)GPRS.IAP.Para.TXDR),
							(GPRS.IAP.Para.TXLen),
							((char*)GPRS.IAP.Para.RXDR),
							((&GPRS.IAP.Para.RXLen)),
							 GSM_GPRS_Update
							);	
							/* 数据发送失败 */
							if(FALSE == GPRS.IAP.State.Result)
							{
								GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum++;	

								/* 判断发送失败 次数 当连接升级服务器失败10以上时重启单片机 */
								if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum >= 10)
								{
									   mcu_reset();//重启设备  复位单片机zwc
//									/* 重新配置GSM模块 */
//									GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
								}							
							}else
							{
									/* 清除 GPRS 数据发送失败次数 */
									GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum = 0;
							}
							/* 消息执行完成 */
							gsm_que_inf->EVENT_Getarg = TRUE;
							/* 固件升级 */
							IAP_UploadLog(&GPRS);
						   #endif
							 #endif
							break;	
						}
/*********************************************************************************/		
#if (GSM_Module_Type==0x00) //SIM800C模块		
						/* 发送短信 */
						case Send_SMS_EVENT:
						{

						 GSM_SMS_Send(&config_tab->Inf_State.Instru_GSM);
							
						   break;
						}
/*********************************************************************************/			
#endif				
						/* 上传 配置 数据 */
						case ConfigService_EVENT:
						{
////////								/* 发送数据 */
////////								GPRS.ConfigService.State.Result = 
////////								(BOOL)Gsm_gprs_comm(
////////								(char*)GPRS.ConfigService.Para.TXDR,
////////								(GPRS.ConfigService.Para.TXLen),
////////								((char*)GPRS.ConfigService.Para.RXDR),
////////								((&GPRS.ConfigService.Para.RXLen)),
////////								  GSM_GPRS_Config
////////								);
								 #if (AppGPRSComProtocolEnable == 0x01)
                 #if (AppGPRSComProtocol == 0x00)	
								PrintGsmDebug(
								(char*)GPRS.ConfigService.Para.TXDR,
								(GPRS.ConfigService.Para.TXLen),
								((char*)GPRS.ConfigService.Para.RXDR),
								((&GPRS.ConfigService.Para.RXLen)),
								  GSM_GPRS_Config
								);
									#endif
							    #endif
							/* 数据发送失败 */
							if(FALSE == GPRS.ConfigService.State.Result)
							{
								 GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum++;	
								/* 判断发送失败 次数 */
								if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum >= 5)
								{
										 
										/* 重新配置GSM模块 */
									//	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
								}
							}else
							{
								/* 清除 GPRS 数据发送失败次数 */
								GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum = 0;
							}
							/* 消息执行完成 */
							gsm_que_inf->EVENT_Getarg = TRUE;	
							ConfigService_RXLog(GPRS,&Instru_Config_Inf_Tab);
							break;
						}
/*************************************** 系统时钟校准 ******************************************/	
						case System_Clock_Calibration_EVENT:	
						{
							 /* GPRS发送时钟校准数据 */
					  	GPRS_Send_System_Clock_Calibration_Data();	
							break;							
						}
/*********************************************************************************/		
						default :
						/* 未知类型的 消息 */
						__nop();
						break;
					}
				}else
				{
					/* GSM模块配置 失败 */
					/* 模块 停止工作 需要重新配置 */
					/* 设置 消息执行结果 */
					gsm_que_inf->EVENT_Getarg = TRUE;
					switch(gsm_que_inf->EVENT_Cmd)
					{
						//	/* 更新信号强度 */
						case Update_SGL_EVENT:
						__nop();
						break;
/*********************************************************************************/				
						//	/* 更新附着状态 */
						case Update_Link_EVENT:
						__nop();
						break;
/*********************************************************************************/						
						//	/* 上传温湿度数据 */
						case Updata_Data_EVENT:
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State			 				= FALSE;
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State 	= TRUE;//(当前状态不需要设置)
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result 						= FALSE;
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result 						= FALSE;
						break;
/*********************************************************************************/		
						/* 下载 程序更新 数据 */
						case IAP_EVENT:
						{
							#if (AppGPRSComProtocolEnable == 0x01)
              #if (AppGPRSComProtocol == 0x00)
							/* GSM模块没有工作，停止固件升级 */
							IAP_StopRun();
							#endif
							#endif
							break;
						}
/*********************************************************************************/		
						/* 发送短信 */
						case Send_SMS_EVENT:
						/* 未知类型的 消息 */
/* 判断用户数量 */
#if SMS_User_Number == 0x01	
						/* GSM模块工作异常，不发送短信。 */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
#elif SMS_User_Number == 0x02
						/* GSM模块工作异常，不发送短信。 */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
						/* GSM模块工作异常，不发送短信。 */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = FALSE;
#elif SMS_User_Number == 0x03
						/* GSM模块工作异常，不发送短信。 */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
						/* GSM模块工作异常，不发送短信。 */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = FALSE;
						/* GSM模块工作异常，不发送短信。 */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = FALSE; 
#endif
						/* 设置 短信发送完成状态 */
						GSM_SMS_SR_Reg.GSM_SMS_Send_State = FALSE;
						break;
/*********************************************************************************/					
						/* 上传 配置 数据 */
						case ConfigService_EVENT:
						{
//			 /* GSM模块没有工作，停止配置 */
							ConfigService_StopRun();
							break;
						}
/*************************************** 系统时钟校准 ******************************************/	
						case System_Clock_Calibration_EVENT:	
						{
							/* GSM模块未配置，因此这次时钟校准没有被执行，恢复系统时钟校准状态寄存器 */
							RTC_System_Clock_Calibration_State_Reset();
							break;							
						}
/*********************************************************************************/		
						default :
						/* 未知类型的 消息 */
						__nop();
						break;
					}
				}
				break;
			}
		}
//	}else
//	{
//		/* 修改 GSM模块状态:失败 */
//		GPRS_SR_Reg.GPRS_Work_State = FALSE;
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* 电池电量不足，GSM无法正常工作。 */
//			printf("\r电池电量不足，GSM无法正常工作。\r\n");				
//		}
//	}
	/* 消息执行完成 */
	gsm_que_inf->EVENT_Getarg = TRUE;
}
/************************************************************************************************/
/* 函数名	：GPRS_Send_Network_Data															*/
/* 输入		：状态信息打印标志																	*/
/* 输出		：无 																				*/
/* 作用		：GPRS 发送 网络序数据			 									 		 		*/
/* 创建日期	：2015/04/01																		*/
/************************************************************************************************/
/**
  * @brief  Description "GPRS通讯"
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
  * @param  rbuf		接收缓冲区
  * @param  rlen		接收缓冲区字节数大小
  * @retval bool		GPRS通讯是否成�
bool gsm_gprs_comm(char *wbuf, int wlen, char *rbuf, int* rlen)
  */
 void GPRS_Send_Network_Data(void)//要修改的函数ZWC
{
/* GPRS 传输网络序数据失败次数限制 */
#define GPRS_COM_Send_Network_Fail_Num_Limit	(3u)
	
	/* 设置 GPRS接收最大数据长度。 */
	GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len = GPRS_Rece_Sensor_Data_Buffer_Size;
/* 发送网络序数据 *//*****************向服务器发送数据*********************************/
//	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = 
//	(BOOL)gsm_gprs_comm(
//	  (char*)GPRS_Send_Sensor_Data_Buffer,
//		(GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
//	  (char*)GPRS_Rece_Sensor_Data_Buffer,
//	  ((int*)(&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)));
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = 
	  (BOOL)Gsm_gprs_comm(
	                      (char*)GPRS_Send_Sensor_Data_Buffer,
		                    (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
	                      (char*)GPRS_Rece_Sensor_Data_Buffer,
	                      ((&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)),
									       GSM_GPRS_Data
								       );
	if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result)//打印调试信息
	{
				PrintGsmDebug(
	                     (char*)GPRS_Send_Sensor_Data_Buffer,
		                   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
	                     (char*)GPRS_Rece_Sensor_Data_Buffer,
	                     ((&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)),
									     GSM_GPRS_Data
								     );	
	}		
					 
									 

/******************************END******************************************************/
	/* 判断 数据发送结果 */
	if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == FALSE)
	{
		/* 数据发送失败 */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum++;//数据发送失败计数加1
		
		/* 判断发送失败 次数 */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum >= GPRS_COM_Send_Network_Fail_Num_Limit)//如果发送数据失败次数达到3次则重新对GSM模块进行配置
		{
			/* 重新配置GSM模块 */
			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		}
		/* 
			设置 传感器数据 正在发送 状态:重新发送传感器当前数据
			(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE) 
		*/;
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;//(当前状态不需要设置)
	}else//数据发送成功
	{
		 Log_printf("向数据服务器发送传感器数据成功！！！\r\n");	
		
		/* 解析 服务器返回的数据，判断本次GPRS通信状态 */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_Network_Data();
		/* 判断 本次GPRS通信 结果 */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE)//如果本次数据通信正常
		{
#if (AppGYForwardEnable == 0x01)			
			/* 判断创建国药数据标志 */
			if(TRUE == AppComProtocolGY.SR.CreateDataFlag)
			{
				/* 数据创建成功，开始发送数据. */
				/* 发送数据状态 */
				BOOL gSendState;
				/* 发送数据长度 */
				int gSendLen;
				/* 接收数据长度 */
				int gReceLen = (128ul);


				/* 计算发送数据长度 */
				gSendLen = strlen((char*)AppComProtocolGY.DR.TXDR);

				/* 设置国药上传IP/端口 */
				GSM_Config_Data_Network((void*)&AppComProtocolGY.CR.NetworkConnection);
				
				/* 发送国药数据 */
				gSendState = (BOOL)gsm_gprs_comm(
					(char*)AppComProtocolGY.DR.TXDR,
					(gSendLen),
					(char*)AppComProtocolGY.DR.RXDR,
					(&gReceLen));
					
				/* 判断数据发送状态 */
				if(TRUE == gSendState)
				{
					/* 数据发送成功 */
					__nop();
				}
				
				/* 设置默认上传IP/端口 */
				GSM_Config_Data_Network((void*)&GSM_Tcp_Conn[0]);	
			}
#endif
			
#if (ComProtocol_WZ_Enable == 0x01)			
			/* 判断创建万智数据标志 */
			if(TRUE == ComProtocolWZDR.SR.CreateDataFlag)
			{
				/* 数据创建成功，开始发送数据. */
				/* 发送数据状态 */
				BOOL gSendState;
				/* 发送数据长度 */
				int gSendLen;
				/* 接收数据长度 */
				int gReceLen = (128ul);


				/* 计算发送数据长度 */
				gSendLen = strlen((char*)ComProtocolWZDR.DR.TXDR);

				/* 设置国药上传IP/端口 */
				GSM_Config_Data_Network((void*)&ComProtocolWZDR.CR.NetworkConnection);
				
				/* 发送国药数据 */
				gSendState = (BOOL)gsm_gprs_comm(
					(char*)ComProtocolWZDR.DR.TXDR,
					(gSendLen),
					(char*)ComProtocolWZDR.DR.RXDR,
					(&gReceLen));
					
				/* 判断数据发送状态 */
				if(TRUE == gSendState)
				{
					/* 数据发送成功 */
					__nop();
				}
				
				/* 设置默认上传IP/端口 */
				GSM_Config_Data_Network((void*)&GPRS_COM_Reg[0]);	
			}
#endif	
			
			/* 
				设置 传感器数据 正在发送 状态:发送新的传感器当前数据
				(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE)
			*/
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = FALSE;
			
			/* 判断 发送失败 次数 */
			if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum)
			{
				/* 清除 GPRS 数据发送失败次数 */
				GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum = 0;
			}
		}else
		{
			/* 
				设置 传感器数据 正在发送 状态:重新发送传感器当前数据
				(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE) 
			*/
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;//(当前状态不需要设置)
			/* 数据解析失败 */
			//在部标写一下不存在这个情况
			//在HTTP协议，云服务器异常会导致接收到"9999"(服务器异常指令)
			//处理办法：当发现这个事件后延时1分钟发送数据，减少流量，估算在40M/月。
			/* 设置解析失败标志 */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState = FALSE;
		}
	}
	/* 
		设置 传感器数据 传感器数据 正在发送 状态:数据发送完成
	*/
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = FALSE;
#undef GPRS_COM_Send_Network_Fail_Num_Limit
}

/*
*********************************************************************************************************
*	函 数 名: GPRS_Get_Save_Sensor_Data
*	功能说明: 获得存储的传感器数据
*	形    参：无
*	返 回 值: 无 Instru_Config_Inf_Tab  Instru_Fix_Inf_State_Tab
*********************************************************************************************************
*/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
static BOOL GPRS_Get_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
/* 默认:无:0x00 */
#define GPRS_Sensor_Null				(0x00)		
/* 传感器类型:温湿度传感器:0x01 */
#define GPRS_Sensor_Board_On			(0x01)	
/* 传感器类型:单温卡片:0x02 */
#define GPRS_Sensor_RF_Tmp				(0x02)	
/* 传感器类型:温湿度卡片:0x03 */	
#define GPRS_Sensor_RF_Hum				(0x03)
/* 发送数据寄存器空间大小 */
#define GPRS_Send_Reg_Size				(sizeof(GSM_GPRS_Send_Struct))
	
#if (AppGPRSComProtocol == 0x03)//视窗协议
/* 发送数据寄存器 数据内容的固定长度 */
#define GPRS_Send_Data_Content_Fix_Len	(20u)//zwc
#endif

#if (AppGPRSComProtocol == 0x00)//部标协议
/* 发送数据寄存器 数据内容的固定长度 */
#define GPRS_Send_Data_Content_Fix_Len	(16u)//zwc
#endif
/* 发送数据寄存器 固定长度 */
#define GPRS_Send_Data_Fix_Len			(22u)
/* 一帧数据在存储的长度 */
#if ((Sensor_Tmp_Num+Sensor_Hum_Num+Sensor_CarbonDioxide_Num+Sensor_DoorContact_Num+Sensor_Lon_Num+Sensor_Lat_Num+Sensor_Spd_Num+Sensor_PT1000_Num) > 0)
#define Sensors_Data_Store_Size		(GPRS_Send_Data_Content_Fix_Len+\
									(Sensor_Tmp_Num+Sensor_CarbonDioxide_Num+Sensor_DoorContact_Num+Sensor_Hum_Num+Sensor_Lon_Num+Sensor_Lat_Num+Sensor_Spd_Num+Sensor_PT1000_Num)*sizeof(float)+\
									(Sensor_RF_Single_Tmp_Num)*(GPRS_Send_Data_Content_Fix_Len+sizeof(float))+\
									(Sensor_RF_Humiture_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									(Sensor_GW_NJSY_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									(Sensor_GW_HZZH_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float)))
#else
#define Sensors_Data_Store_Size		((Sensor_RF_Single_Tmp_Num)*(GPRS_Send_Data_Content_Fix_Len+sizeof(float))+\
									 (Sensor_RF_Humiture_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									 (Sensor_GW_NJSY_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									 (Sensor_GW_HZZH_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float)))
#endif			

#if (AppGYForwardEnable == 0x00)
/* 单次发送温湿度数据最大传输的数据帧数量 */
#define GPRS_Send_Data_Frames_Max_Num	((u8)((GPRS_Send_Reg_Size-GPRS_Send_Data_Fix_Len)/Sensors_Data_Store_Size))
#elif (AppGYForwardEnable == 0x01)
#define GPRS_Send_Data_Frames_Max_Num	(1u)
#endif
									 /* 一帧数据包含的数据条数(包含几个SN号数据) */
#define Sensors_One_Frames_Include_Data_Num		((((Sensor_Tmp_Num>0)||\
													  (Sensor_DoorContact_Num>0) ||\
													  (Sensor_CarbonDioxide_Num>0) ||\
												    (Sensor_Hum_Num>0)||\
												    (Sensor_PT1000_Num>0))?1:0)+\
													   Sensor_RF_Single_Tmp_Num+\
												     Sensor_RF_Humiture_Num+\
													   Sensor_GW_NJSY_Num+\
													   Sensor_GW_HZZH_Num)
/* 没有查到匹配的通道 */
#define Sensors_CH_Null (0xFF)
	/* 读 Fram 数据的缓存指针 */
	Save_Sensor_Data_Struct *read_sensor_data_p;
	/* 发送数据寄存器 数据内容指针 */
	u8 * gprs_data_content_p = &GPRS_Send_Reg.GPRS_IR_FIX.Data_P;
#if ((Wireless_RF_Card_Config_Enable == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
	/* 温湿度数据 */
	float sensor_data;
	#if ((Sensor_RF_Humiture_Num > 0)||\
		 (Sensor_GW_NJSY_Num > 0)||\
		 (Sensor_GW_HZZH_Num > 0))
	/* 温湿度卡片的湿度通道编号 */
	u8 rf_hum_no;					
	#endif							
#endif								
	/* 当前传输的数据帧数 */
	u8 data_frames_num;
	/* 计数器 */
	u16 num;

	/* 判断 是否有历史数据 */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM < Save_Sensor_Data_Len)
	{
		/* 没有历史数据 */
		return FALSE;
	}
									 							
	//1、复位发送数据寄存器内容
	/* 重新设置 寄存器 数据 */
	GPRS_Send_Reg = GPRS_Send_Reg_Fix;
	/* 设置 SN号 */
	memcpy(GPRS_Send_Reg.GPRS_IR_FIX.SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);

	//2、获取数据内容							 
	for(data_frames_num = 0;
		 (fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)&&
		 (data_frames_num < GPRS_Send_Data_Frames_Max_Num);
		 data_frames_num++
	   )
	{
		/* 获取当前的传感器通道数据 */
		/* 根据当前外部存储器（W25Q64）的数据存储指针来发送数据 */
		if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)
		{
			/* Flash有历史数据，就读取数据，先发历史数据 */
			read_sensor_data_p = (Save_Sensor_Data_Struct*)Flash_Read_Sensor_Data(
								  fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size,
								  Save_Sensor_Data_Len);

			//设置 Flash 读指针
			/* 曾加 读取的数据长度 */
			fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer 		  += Save_Sensor_Data_Len; 
			/* 存储数据量 减*/
			fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	-= Save_Sensor_Data_Len;
			
#if (AppGYForwardEnable == 0x01)
			/* 创建国药探头数据 */
			AppComProtocolGY.SR.CreateDataFlag = ComProtGY_CreateData(read_sensor_data_p);
#endif	
#if (ComProtocol_WZ_Enable == 0x01)
			/* 创建国药探头数据 */
			ComProtocolWZDR.SR.CreateDataFlag = ComProtWZ_CreateData(read_sensor_data_p);
#endif			
			
		
		}
		//I、搜索传感器通道（这里有一点要说明，现在板载的传感器最多只有八个，因此我们搜索的范围也就在前面8个通道里。）
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH1_Config_Type == Sensor_Hum) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH2_Config_Type == Sensor_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH2_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH3_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH3_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH4_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH4_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH5_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH5_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH6_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH6_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH7_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
	   (Sensor_CH7_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH8_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH8_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH9_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH9_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH10_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH10_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH11_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH11_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH12_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH12_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH13_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH13_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH14_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH14_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH15_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH15_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH16_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH16_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH17_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH17_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH18_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH18_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH19_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH19_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH20_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH20_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH21_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH21_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH22_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH22_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH23_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH23_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH24_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH24_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH25_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH25_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH26_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH26_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH27_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH27_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH28_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH28_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH29_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH29_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH30_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH30_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH31_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH31_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH32_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH32_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH32_Config_Type == Sensor_PT1000))
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		/* 仪器有传感器配置，先设置数据内容 */
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),sizeof(u32));
		//b、设置仪器状态
		/* 获取电源状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		//信号强度
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved1 = (u8)GPRS_SR_Reg.GPRS_Sign_Stre;
/******************************zwc世窗协议修改*************************************/
	     #if (AppGPRSComProtocol == 0x03)
		   {
				 //预留字节zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved2 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved3 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved4 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved5 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved6 = 0x00;//zwc
	     }	
			 #endif
/*********************************************************************************/	
/* 这里的仪器状态都是相同的，这样都根据通道1-8的顺序检查来确定使用那个通道的电源状态和电池电量数据 */		
#if	((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
     (Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH1_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH1_Config_Type == Sensor_PT1000))
		//电池电量格数
			if(AppGPRSComProtocol == 0x03)//河北视窗协议
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//其他协议(如:部标协议等)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}				
#elif ((Sensor_CH2_Config_Type == Sensor_Tmp) ||\
(Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH2_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000))
				//电池电量格数
			if(AppGPRSComProtocol == 0x03)//河北视窗协议
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//其他协议(如:部标协议等)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}
		
		
		
#elif ((Sensor_CH3_Config_Type == Sensor_Tmp) ||\
(Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH3_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH3_Config_Type == Sensor_PT1000))
				//电池电量格数
			if(AppGPRSComProtocol == 0x03)//河北视窗协议
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//其他协议(如:部标协议等)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}	
#elif ((Sensor_CH4_Config_Type == Sensor_Tmp) ||\
(Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH4_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH4_Config_Type == Sensor_PT1000))
				//电池电量格数
			if(AppGPRSComProtocol == 0x03)//河北视窗协议
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//其他协议(如:部标协议等)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4; 
			}
			
			
#elif ((Sensor_CH5_Config_Type == Sensor_Tmp) ||\
(Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH5_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH5_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
#elif ((Sensor_CH6_Config_Type == Sensor_Tmp) ||\
(Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH6_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH6_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
#elif ((Sensor_CH7_Config_Type == Sensor_Tmp) ||\
			(Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
			(Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
	   (Sensor_CH7_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH7_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
#elif ((Sensor_CH8_Config_Type == Sensor_Tmp) ||\
(Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH8_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH8_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;


#elif ((Sensor_CH9_Config_Type == Sensor_Tmp) ||\
(Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH9_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH9_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
#elif ((Sensor_CH10_Config_Type == Sensor_Tmp) ||\
(Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH10_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH10_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
#elif ((Sensor_CH11_Config_Type == Sensor_Tmp) ||\
(Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH11_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH11_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
#elif ((Sensor_CH12_Config_Type == Sensor_Tmp) ||\
(Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH12_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH12_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
#elif ((Sensor_CH13_Config_Type == Sensor_Tmp) ||\
(Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH13_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH13_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
#elif ((Sensor_CH14_Config_Type == Sensor_Tmp) ||\
(Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH14_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH14_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
#elif ((Sensor_CH15_Config_Type == Sensor_Tmp) ||\
(Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH15_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH15_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
#elif ((Sensor_CH16_Config_Type == Sensor_Tmp) ||\
(Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH16_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH16_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;

#elif ((Sensor_CH17_Config_Type == Sensor_Tmp) ||\
(Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH17_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH17_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
#elif ((Sensor_CH18_Config_Type == Sensor_Tmp) ||\
(Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH18_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH18_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
#elif ((Sensor_CH19_Config_Type == Sensor_Tmp) ||\
(Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH19_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH19_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
#elif ((Sensor_CH20_Config_Type == Sensor_Tmp) ||\
(Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH20_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH20_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
#elif ((Sensor_CH21_Config_Type == Sensor_Tmp) ||\
(Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH21_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH21_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
#elif ((Sensor_CH22_Config_Type == Sensor_Tmp) ||\
(Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH22_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH22_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
#elif ((Sensor_CH23_Config_Type == Sensor_Tmp) ||\
(Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH23_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH23_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
#elif ((Sensor_CH24_Config_Type == Sensor_Tmp) ||\
(Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH24_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH24_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
#elif ((Sensor_CH25_Config_Type == Sensor_Tmp) ||\
(Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH25_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH25_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
#elif ((Sensor_CH26_Config_Type == Sensor_Tmp) ||\
(Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH26_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
#elif ((Sensor_CH27_Config_Type == Sensor_Tmp) ||\
(Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH27_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH27_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
#elif ((Sensor_CH28_Config_Type == Sensor_Tmp) ||\
(Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH28_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH28_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
#elif ((Sensor_CH29_Config_Type == Sensor_Tmp) ||\
(Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH29_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH29_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
#elif ((Sensor_CH30_Config_Type == Sensor_Tmp) ||\
(Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH30_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH30_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
#elif ((Sensor_CH31_Config_Type == Sensor_Tmp) ||\
(Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH31_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH31_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
#elif ((Sensor_CH32_Config_Type == Sensor_Tmp) ||\
(Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH32_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH32_Config_Type == Sensor_PT1000))
		/* 获取电池电量报警状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;

#endif

		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_Board_On;
		//d、设置数据 和 设置通道数
		GPRS_GET_Save_BSP_Sensor_Data(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//e、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//f、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#endif
		//II、搜索单温卡片通道（卡片通道根据设置的最大传感器配置数量来搜索）
#if (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO, config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///电量正常/电量不足现在只有这两种状态
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum)
		__nop();	
#else
		__nop();
#endif		



#if (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif


#if (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif
		
		
#if (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();	
#else
		__nop();
#endif	
		
		
#if (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	

		
#if (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif


#if (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	

		
#if (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif		


#if (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


#if (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


#if (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif		


#if (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
		//通道一被配置成单温卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e、设置数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp))
		//通道一被配置成温湿卡片类型
		/* 复位数据内容寄存器 */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a、设置 SN号
		/* 获取 SN号 */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b、设置仪器状态
		/* 获取电源状态 *///卡片没有未接电源，这个状态固定为正常（1）
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* 获取电池电量报警状态 *///这个状态信息暂时没有定义，先用正常信息固定后面添加。
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b、设置时间戳
		/* 获取 时间戳 */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c、设置通道类型
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d、设置通道数
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e、设置温度数据
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* 这里进行大小端数据转换，虽然结果有点难看。 */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* 查找对应的温湿度卡片的湿度通道 */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* 判断温湿度卡片是否有匹配的湿度通道 */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f、设置湿度数据
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* 这里进行大小端数据转换，虽然结果有点难看。 */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g、将设置的数据内容导入到温湿度数据寄存器中
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h、将指针指向数据末尾
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


	}
	/* 计算消息体长度 *///数据帧数*一组数据的长度+消息类型（1字节）+ 数据类型（1字节）+ 数据帧数（1字节）
	GPRS_Send_Reg.GPRS_IR_FIX.Attribute_HSB_Length = (((u16)Sensors_Data_Store_Size*data_frames_num+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num))/0X100);
	GPRS_Send_Reg.GPRS_IR_FIX.Attribute_LSB_Length = (((u16)Sensors_Data_Store_Size*data_frames_num+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num))%0X100);
	/* 计算数据帧数 */
	GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num = data_frames_num*Sensors_One_Frames_Include_Data_Num;

/**********************************计算校验码**************************************************/															
	/* 根据 消息头和消息体 的数据 获得 校验码 */
	for(num = 1,*(gprs_data_content_p) = 0;num < ((u16)(gprs_data_content_p - GPRS_Send_Reg.GSM_GPRS_Send_Buffer)); num++)
	{
		/* 从 消息头 到 消息体 最后一个 数据 异或 */
		*(gprs_data_content_p) ^= GPRS_Send_Reg.GSM_GPRS_Send_Buffer[num];
	}
/**************************************END*****************************************************/	

	/* 执行到这里 说明 设置成功 */
	return TRUE;
	
#undef GPRS_Sensor_Null
#undef GPRS_Sensor_Board_On
#undef GPRS_Sensor_RF_Tmp
#undef GPRS_Sensor_RF_Hum
#undef GPRS_Send_Reg_Size
#undef GPRS_Send_Data_Content_Fix_Len
#undef GPRS_Send_Data_Fix_Len
#undef Sensors_Data_Store_Size	
#undef GPRS_Send_Data_Frames_Max_Num
#undef Sensors_One_Frames_Include_Data_Num	
#undef Sensors_CH_Null
}

#elif (AppGPRSComProtocol == 0x01)//Http协议传输温湿度数据
static BOOL GPRS_Get_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* 读数据的缓存指针 */
	Save_Sensor_Data_Struct            *sensordata_p;
	/* 临时时间缓存 */
	Times                                       time;
	/* 计数器 */
	unsigned char                                num;
	/* 探头编号 */
	unsigned char                           probeNum;
	/* 网关编号指针 */
	SERIAL_NUMBER* gatewayIdNoP         = &config_tab->Inf_State.Instru_System.Inf.Serial_Number;
	/* 传感器通道指针 */
	INSTRU_SENSOR_CHANNEL* sensorchP    = &config_tab->Inf_State.Instru_Sensor.Inf_State.CH1;


	/* 清除数据接口参数 */
	memset((char*)&HTTPPara.ApiPara,0x00,sizeof(HTTPPara.ApiPara));
	
	
	/* 判断 是否有历史数据 */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM < Save_Sensor_Data_Len)
	{
		/* 没有历史数据 */
		return FALSE;
	}
	
	/* 设置网关编号 */
	sprintf(HTTPPara.ApiPara.GatewayIdNo,"%04X%05d",
	((((*(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0x00ff) << 8)),
	((((*(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0x00ff) << 8)));
	
	
	/* 获取当前的传感器通道数据 */
	/* 根据当前外部存储器（W25Q64）的数据存储指针来发送数据 */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)
	{
		/* Flash有历史数据，就读取数据，先发历史数据 */
		sensordata_p = (Save_Sensor_Data_Struct*)Flash_Read_Sensor_Data(
						fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size,
						Save_Sensor_Data_Len);

		//设置 Flash 读指针
		/* 曾加 读取的数据长度 */
		fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer 		+= Save_Sensor_Data_Len; 
		/* 存储数据量 减*/
		fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	-= Save_Sensor_Data_Len;
		
	}
	
	/* 设置上传时间戳 */
	time.Year 	= config_tab->Inf_State.Instru_RTC.Inf_State.Year;
	time.Month	= config_tab->Inf_State.Instru_RTC.Inf_State.Month;
	time.Day 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Day;
	time.Hour 	= config_tab->Inf_State.Instru_RTC.Inf_State.Hour;
	time.Min 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Min;
	time.Sec 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Sec;
	/* 将参数的采集时间转换成秒，将这个数据设置为参数签名的时间数据。 */
	HTTPPara.ApiPara.TimeStamp = RTC_Date_Time_To_Second(&time);
	
	/* 清除参数签名数据缓存。 */
	memset(HTTPPara.ApiPara.Sign,0x00,sizeof(HTTPPara.ApiPara.Sign));
	
	/* 设置GPS设备状态 */
	if((GPS_Config_Enable == 0x01)&&
	   ((Sensor_Lon_Num + Sensor_Lat_Num + Sensor_Spd_Num) > 0))
	{
		/* 使用GPS设备 */
		/* 未使用GPS设备 */
		HTTPPara.ApiPara.GPS_State = TRUE;
		/* 查询经纬度数据 */
		for(num = 0;num < 32 ;num++)
		{
			/* 判断通道类型 */
			if(Sensor_GPS_POS_Lon == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
			{
				/* 设置经度数据 */
				HTTPPara.ApiPara.Lng = sensordata_p->GPRS_IR.Sensor_Data[num];
			}else if(Sensor_GPS_POS_Lat == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
			{
				/* 设置纬度数据 */
				HTTPPara.ApiPara.Lat = sensordata_p->GPRS_IR.Sensor_Data[num];
			}
		}
	}else
	{
		/* 未使用GPS设备 */
		HTTPPara.ApiPara.GPS_State = FALSE;
		/* 清除经度数据 *///暂定0，后面设置成无线号数据。
		HTTPPara.ApiPara.Lng = 0;
		/* 清除纬度数据 *///暂定0，后面设置成无线号数据。
		HTTPPara.ApiPara.Lat = 0;
	}
	
	/* 设置采集时间 */
	time.Year 	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS + 2000;
	time.Month	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS;
	time.Day 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS;
	time.Hour 	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS;
	time.Min 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS;
	time.Sec 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS;
	
	/* 设置采集时间 */
	sprintf(HTTPPara.ApiPara.CollectTime,"%04d-%02d-%02d %02d:%02d:%02d",
			time.Year,time.Month,time.Day,time.Hour,time.Min,time.Sec);
	
	/* 获取当前探头数量 */
	HTTPPara.ApiPara.ProbeNum = Sensor_Tmp_Num + Sensor_Hum_Num +
								Sensor_RF_Single_Tmp_Num + 
								(Sensor_RF_Humiture_Tmp_Num +
								Sensor_RF_Humiture_Hum_Num)/2 +
								Sensor_PT1000_Num +
								Sensor_GW_NJSY_Num;
	
	
	/* 按顺序来设置探头数据 */
	for(num = 0,probeNum = 0;(probeNum < HTTPPara.ApiPara.ProbeNum)&&(num < 32);num++)
	{
		if(Sensor_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* 设置探头编号 */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* 设置探头类型 */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* 设置单温数据 */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_Hum == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* 设置探头编号 */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* 设置探头类型 */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* 设置单温数据 */
			HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_RF_Card_Single_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* 判断卡片SN号 */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* 卡片SN号未配置过，不发送当前通道数据. */
				/* 减掉一个通道. */
				HTTPPara.ApiPara.ProbeNum--;
				/* 继续 */
				continue;
			}
			
			/* 设置探头编号 */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* 设置探头类型 */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* 设置单温数据 */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_RF_Card_Humiture_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* 湿度传感器编号 */
			unsigned char humiNO;
			
			/* 判断卡片SN号 */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* 卡片SN号未配置过，不发送当前通道数据. */
				/* 减掉一个通道. */
				HTTPPara.ApiPara.ProbeNum--;
				/* 继续 */
				continue;
			}
			
			/* 设置探头编号 */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* 设置探头类型 */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* 设置温度数据 */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
			/* 查找对应的温湿度卡片的湿度通道 */
			humiNO = GPRS_GET_RF_Humiture_Hum_Data(((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
			/* 判断收索结果 */
			if(humiNO != 0xFF)
			{
				/* 设置温度数据 */
				HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[humiNO];		
			}else
			{
				/* 设置温度数据 */
				HTTPPara.ApiPara.Logs[probeNum].Humi = Wireless_RF_Card_Data_Rece_Timeout_Para;		
			}
			
			/* 判断是否接收到数据 */
			if((Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp))
			{
				/* 卡片SN号未配置过，不发送当前通道数据. */
				/* 减掉一个通道. */
				HTTPPara.ApiPara.ProbeNum--;
				/* 继续 */
				continue;
			}
		}else if(Sensor_PT1000 == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* 设置探头编号 */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* 设置探头类型 */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* 设置单温数据 */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_GW_NJSY_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* 湿度传感器编号 */
			unsigned char humiNO;
			
			/* 判断卡片SN号 */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* 卡片SN号未配置过，不发送当前通道数据. */
				/* 减掉一个通道. */
				HTTPPara.ApiPara.ProbeNum--;
				/* 继续 */
				continue;
			}
			
			/* 设置探头编号 */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* 设置探头类型 */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* 设置温度数据 */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
			/* 查找对应的温湿度卡片的湿度通道 */
			humiNO = GPRS_GET_RF_Humiture_Hum_Data(((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
			/* 判断收索结果 */
			if(humiNO != 0xFF)
			{
				/* 设置温度数据 */
				HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[humiNO];		
			}else
			{
				/* 设置温度数据 */
				HTTPPara.ApiPara.Logs[probeNum].Humi = Gateway_NJSY_TimeoutPara;		
			}
			
			/* 判断是否接收到数据 */
			if((Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp))
			{
				/* 卡片SN号未配置过，不发送当前通道数据. */
				/* 减掉一个通道. */
				HTTPPara.ApiPara.ProbeNum--;
				/* 继续 */
				continue;
			}
		}else
		{
			continue;
		}
		
		/* 设置通道编号 */
		probeNum++;
		
	}
	/* 创建HTTP发送数据。 */
HTTP_Create_Data((char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
	/* 没有历史数据 */
	return TRUE;
}

#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x02)||(AppGPRSComProtocol == 0x04))
/*
*********************************************************************************************************
*	函 数 名		: GPRS_Data_To_Network
*	功能说明		: 将仪器的数据装换成网络数据，等待发送。 
*	形    参		: 无
*	返 回 值		: 无
*********************************************************************************************************
*/
void GPRS_Data_To_Network(void)
{
	
/* 发送数据寄存器 消息体数据之前的 固定数据长度 *///与之前的不同
#define GPRS_Send_Data_Before_Fix_Len	((u32)(&GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)-\
										 (u32)(&GPRS_Send_Reg.GPRS_IR_FIX.ID_Type_HSB))
/* 校验码数据位数 */
#define GPRS_Send_Data_Check_Byte		(1u)
	
	/*  GPRS发送数据长度 */
	u16 gprs_send_sensor_data_len = 0;	
	

	//1、转义 数据
	/* 转义 *//* 获得GPRS发送数据长度 */
	gprs_send_sensor_data_len = GPRS_Host_To_Network_Translator_Data(&GPRS_Send_Sensor_Data_Buffer[1],
																	 &GPRS_Send_Reg.GSM_GPRS_Send_Buffer[1],
																	 (GPRS_Send_Data_Before_Fix_Len+
																((u16)GPRS_Send_Reg.GPRS_IR_FIX.Attribute_HSB_Length*256)+
																((u16)GPRS_Send_Reg.GPRS_IR_FIX.Attribute_LSB_Length)+GPRS_Send_Data_Check_Byte));
	
	//2、增加 头和尾 标识
	/* 加头标志  */
	GPRS_Send_Sensor_Data_Buffer[0] = GPRS_COM_Identify;
	/* GPRS发送数据长度  */
	gprs_send_sensor_data_len++;
	/* 加尾标志 */
	GPRS_Send_Sensor_Data_Buffer[gprs_send_sensor_data_len] = GPRS_COM_Identify;
	/* GPRS发送数据长度  */
	gprs_send_sensor_data_len++;
	/* 计算 传输的数据 长度 *///头标志(1 Byte)尾标志(1 Byte)
	GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = gprs_send_sensor_data_len;
	
	
#undef GPRS_Send_Data_Before_Fix_Len
#undef GPRS_Send_Data_Check_Byte
}

/*
*********************************************************************************************************
*	函 数 名: GPRS_Host_To_Network_Translator_Data
*	功能说明: 将主机序转换成网络序数据
*	形    参：network_endian   host_endian	T_num
*	返 回 值: 转换后的数据缓存长度
*********************************************************************************************************
*/
u16 GPRS_Host_To_Network_Translator_Data(u8* network_endian,u8* host_endian,u16 T_num)
{
	u16 num;
	u16 b_sp;
	
	/* 缓存指针清0 */
	b_sp = 0;

	for(num = 0;num < T_num; num++)
	{
		if(host_endian[num] == GSM_Translator_Symbol_0X7E)
		{
			/* 当前数据 是转译 符号0X7E */
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X7D;
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X02;
		}else if(host_endian[num] == GSM_Translator_Symbol_0X7D)
		{
			/* 当前数据 是转译 符号0X7D */
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X7D;
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X01;
		}else
		{
			/* 当前数据 非转译 符号 */
			*(network_endian+b_sp++) = *(host_endian+num);
		}
	}
	return b_sp;
}

/************************************************************************************************/
/* 函数名	：GPRS_Network_To_Host_Translator_Data												*/
/* 输入		：network_endian   host_endian	T_num												*/
/* 输出		：转换后的数据缓存长度																	*/
/* 作用		：将网络序转换成主机序数据			 									 		 		*/
/* 创建日期	：2015/04/01																		*/
/************************************************************************************************/
u16 GPRS_Network_To_Host_Translator_Data(u8* host_endian,u8* network_endian,u16 T_num)
{
	u16 num;
	u16 b_sp;
	
	/* 缓存指针清0 */
	b_sp = 0;
	/* 读取数据 */
	for(num = 0;num < T_num; num++)
	{
		/* 判断 当前的数据 是否为转义编码. */
		if(network_endian[num] == GSM_Translator_Symbol_0X7D)
		{
			/* 是转义编码 */
			/* 判断 是2个编码中的那个? */
			if(network_endian[num+1] == GSM_Translator_Symbol_0X02)
			{
				/* 说明 该编码源码是0X7E */
				host_endian[b_sp++] = GSM_Translator_Symbol_0X7E;
				/* 数据指针 指向 下下个数据 */
				num++;
			}else if(network_endian[num+1] == GSM_Translator_Symbol_0X01)
			{
				/* 说明 该编码源码是0X7D */
				host_endian[b_sp++] = GSM_Translator_Symbol_0X7D;
				/* 数据指针 指向 下下个数据 */
				num++;
			}else
			{
				/* 位置类型的编码，这种情况是错误的转移导致的。 */
				b_sp = 0;
				return b_sp;
			}
		}else
		{
			/* 正常数据，直接获取 */
			host_endian[b_sp++] = network_endian[num];
		}
	}
	return b_sp;
}
#endif
#endif
/*
*********************************************************************************************************
*	函 数 名: GSM_Send_Q
*	功能说明: 发送 GSM 消息队列
*	形    参：pevent:消息队列的指针，GSM_CMD_TYPE:消息队列类型
*	返 回 值: 无
*********************************************************************************************************
*/
BOOL GSM_Send_Q(OS_EVENT* pevent,GSM_CMD_TYPE EVENT_Type)
{
	INT8U     	err;
	OS_Q_DATA	Q_data;
	GSM_QUE*	EVENT_p;	
	u16 		num;
	u32 		Q_p;

	
	/* 发送的 事件 类型  */
	if(EVENT_Type == Init_GSM_Module_EVENT)
	{
		/* 配置GSM模块 */
		EVENT_p = &GSM_QUE_Init_Module;
	}else if(EVENT_Type == IAP_EVENT)
	{
		/* 下载 程序更新 数据 */	
		EVENT_p = &GSM_QUE_Download_IAP_Data;	
	}else if(EVENT_Type == Update_SGL_EVENT)
	{
		/* 更新信号强度 */
		EVENT_p = &GSM_QUE_Update_Signal;	
	}else if(EVENT_Type == Update_Link_EVENT)
	{
		/* 更新附着状态 */
		EVENT_p = &GSM_QUE_Update_Link;	
	}else if(EVENT_Type == Updata_Data_EVENT)
	{
		/* 上传温湿度数据 */
		EVENT_p = &GSM_QUE_Update_Data;	
	}else if(EVENT_Type == Send_SMS_EVENT)
	{
		/* 发送 短信 */		
		EVENT_p = &GSM_QUE_Send_SMS;	
	}else if(EVENT_Type == ConfigService_EVENT)
	{
		/* 上传 配置 数据 */		
		EVENT_p = &GSM_QUE_ConfigService;		
	}
//	else if(EVENT_Type == Updata_Config_Back_EVENT)
//	{
//		/* 上传 配置 数据 */		
//		EVENT_p = &GSM_QUE_Send_Config_Back;		
//	}
	else if(EVENT_Type == System_Clock_Calibration_EVENT)
	{
		/* 上传 配置 数据 */		
		EVENT_p = &GSM_QUE_Send_System_Clock_Calibration;		
	}
#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	
	else if(EVENT_Type == SetWifiNamePWD_EVENT)
	{
		/* 设置Wifi名字和密码 */		
		EVENT_p = &Wifi_QUE_SetWifiNamePWD;		
	}
	else if(EVENT_Type == SetWifiWLAN_EVENT)
	{
		/* 设置WifiWlan参数 */		
		EVENT_p = &Wifi_QUE_SetWifiWLAN;		
	}
	else if(EVENT_Type == SetWifiWLAN_GET_IP_WAY_EVENT)
	{
		/* 设置Wifi模块IP获取方式参数 */		
		EVENT_p = &Wifi_QUE_GET_IP_WAY;		
	}
	else if(EVENT_Type == RestWifi_EVENT)
	{
		/*复位Wifi模块*/		
		EVENT_p = &Wifi_QUE_Reset;		
	}	
	else if(EVENT_Type == Factory_Data_Reset_Wifi_EVENT)
	{
		/* Wifi模块恢复出厂设置 */		
		EVENT_p = &Wifi_QUE_Factory_Data_Reset;		
	}		

#endif	
	
	/* 程序报道这里 说明程序中不存在 上传数据消息 */
	/* 判断 上传数据 执行结果 */
	if(EVENT_p->EVENT_Getarg == FALSE)
	{
		/* 上传数据消息 正在 被执行，等待执行结束 */
		return FALSE;
	}
	
	//由于 内存空间有限 这里的消息只有一个数据空间，因此，在发送消息前需要确定上次的消息已经被执行了。
	//否则，同一种消息将占据2个消息空间，导致其他消息无法进入。
	/* 检查消息队列中是否存在 更新 附着状态 消息。 */
	err = OSQQuery(pevent,&Q_data);
	
	/* 判断查询结果 */
	if(err == OS_ERR_NONE)
	{
		/* 查询成功 */
		
		/* 查找 队列中是否已经存在 附着状态 消息。 */
		for(num = 0,Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQOut);
			num < Q_data.OSNMsgs;
			num ++)
		{
			/* 判断单前的消息指针与发送指针是否相同 */
			if((*(u32*)Q_p) == (u32)EVENT_p)
			{
				/* 相等，队列中存在 附着状态 消息,那么就不在发送 附着状态 消息 */
				return FALSE;
			}
			/* 设置指针到下一个位置 */
			Q_p += sizeof(GSM_QUE);
			/* 判断指针是否越界 */
			if(Q_p == (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQEnd))
			{
				/* 越界就将 指针 指向头位置 */
				Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQStart);
			}
		}
	}else
	{
		/* 查询失败 */
		return FALSE;
	}
	
	/* 程序到这里就说明 队列中没有存在 信号强度 消息，
	   因此，需要发送一次 信号强度更新 消息。 */
	/* 清楚 消息 执行结果 */
	EVENT_p->EVENT_Getarg = FALSE;
	/* 发送 消息 *///FIFO	
	err = OSQPost(pevent, EVENT_p);  
	/* 判断 消息 发送 状态 */
	if(err == OS_ERR_Q_FULL)
	{
		/* 消息队列已满,不能再接收新消息 */
		/* 这种情况 不可能 出现 */
		return FALSE;
	}
	/* 判断 消息 发送 成功 */
	return TRUE;
}

/************************************************************************************************/
/* 函数名	：GPRS_Save_Sensor_Data_Check														*/
/* 输入		：无																				*/
/* 输出		：无 																				*/
/* 作用		：检测 是否有新的 存储的 温湿度 数据 									 		 		*/
/* 创建日期	：2015/04/01																		*/
/************************************************************************************************/
void GPRS_Save_Sensor_Data_Check(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	
	/* 判断单前模块的工作状态是否正常 */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		/* 模块工作正常 */
		
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x02)
	
		/* 判断 是否有传感器数据在等待发送 */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)
		{
			/* 创建数据 */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
			ComProtocolZDE_CreateData(config_tab,fix_tab,
			GPRS_Send_Sensor_Data_Buffer,
			&GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len);
			/* 拷贝数据 */
//			memcpy((char*)GPRS_Send_Sensor_Data_Buffer,
//				   ComProtocolZDEApp.DR.TXDR,
//					TXLen);
			
			/* 判断 */
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
			   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE) &&
			 (((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == TRUE) &&
			   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE))||
			   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0)))
			{
					/* 发送数据 */
				GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;		
			}
		}
		
#else		
#if (AppGPRSComProtocol == 0x04)//江苏省免疫规划数据中心协议
/* 判断 是否有传感器数据在等待发送 */
if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)&&(RTC_System_Clock_Calibration_State_Reg == TRUE)&&(ConfigService_App.SYNC_State == TRUE))
{
	/* 创建数据 */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
	ComProtocol_JS_CreateData(config_tab,fix_tab,
	GPRS_Send_Sensor_Data_Buffer,
	&GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len);
}
 #endif
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))//部标协议或者HTTP协议
		//部标协议
		/* 判断 是否有传感器数据在等待发送 */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)
		{
			/* 没有 传感器数据在等待转换 */
			/* 检测 是否有新的 存储的 温湿度 数据 */
 			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
			GPRS_Get_Save_Sensor_Data(config_tab,fix_tab);
		}
#endif			
		/* 判断 GPRS 发送传感器数据通信状态 */
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE :说明当前没有数据上传
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE :说明有数据等待上传
		//GPRS_COM_Send_Result == TRUE && GPRS_COM_Rece_Result == TRUE :说明之前的数据上传成功、可以开始上传下一个数据
		//GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0 : 说明发没有数据发送或者发送的数据被清除，可以开始上传下一个数据。
		if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
		   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE) &&
		 (((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == TRUE) &&
		   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE))||
		   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0))
		)
		{
			/* 没有 传感器数据 正在发送 */
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
			/* 将仪器的数据装换成网络数据，等待发送. */
			GPRS_Data_To_Network();
#elif (AppGPRSComProtocol == 0x01)
			/* 计算发送数据长度 */
			GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = strlen((char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
			/* 清除数据缓存 */
			memset(GPRS_Send_Sensor_Data_Buffer,0x00,GPRS_Send_Sensor_Data_Buffer_Size);
			/* 拷贝缓存 */
			strcpy((char*)GPRS_Send_Sensor_Data_Buffer,
				   (char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
#endif
#endif
			/* 清除 GPRS 主机序传感器数据等待处理状态 */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = FALSE;
			/* 设置 GPRS 网络序传感器数据等待处理状态 */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;
		}
#endif
#endif
	}
}

/************************************************************************************************/
/* 函数名	：GPRSSendData_ParseStateCheck														*/
/* 输入		：无																					*/
/* 输出		：无 																				*/
/* 作用		：GPRS接收解析数据状态检测													 		 */
/* 创建日期	：2016/09/09																		*/
/************************************************************************************************/
void GPRSSendData_ParseStateCheck(void)
{
	/* 计数器 */
	static uint32_t num = 0;
	
	/* 判断 GPRS接收解析数据状态检测 */
	if(FALSE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState)
	{
		/* 解析失败 */
		/* 加1，延时 */
		num++;
		/* 判断延时时间 */
		if(num >= AppGPRSUsageProtectTime)
		{
			/* 延时时间到，清除状态标志 */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState = TRUE;
			/* 清除计数 */
			num = 0;
		}
	}
}

/************************************************************************************************/
/* 函数名	：GPRS_Send_Sensor_Data																*/
/* 输入		：无																					*/
/* 输出		：无 																				*/
/* 作用		：发送新的传感器数据													 		 		*/
/* 创建日期	：2015/04/01																		*/
/************************************************************************************************/
void GPRS_Send_Sensor_Data(void)
{

	/* 判断 GPRS 发送传感器数据通信状态 */
	/* 判断 传感器数据 正在发送 && 有网络序数据等待发送 */
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE :说明当前没有数据上传，可以开始上传数据
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE :说明有数据等待上传
		//GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0 : 说明有数据发送，可以开始上传数据�
	 //  IAP_App.RunState == FALSE;   IAP_App.RunState=FALSE表示没有正在进行远程升级;
// printf("1 》》》》》》GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,
//	  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
	
	
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x00)
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State == TRUE) &&
	   (TRUE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0)&&
		  IAP_App.RunState == FALSE)
	{
		/* 发送 上传数据消息 */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Updata_Data_EVENT);
		
//	printf("2 》》》》》》GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n   GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
		
	}
	
		#endif
		#endif
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x01)
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State == TRUE) &&
	   (TRUE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0))
	{
		/* 发送 上传数据消息 */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Updata_Data_EVENT);
		
//	printf("2 》》》》》》GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n   GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
		
	}
	
		#endif
		#endif

}


/************************************************************************************************/
/* 函数名	：GPRS_Parsing_Network_Data															*/
/* 输入		：无																					*/
/* 输出		：无 																				*/
/* 作用		：根据服务器返回的数据信息，判断传感器数据上传状态，设置相关参数。			 		 		*/
/* 创建日期	：2015/04/01																		*/
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))//解析部标协议返回来的数据
static BOOL GPRS_Parsing_Network_Data(void)
{
/* 接收的网络序数据长度 */
#define GPRS_Rece_Network_Data_Len		(20u)
/* 通用应答协议的结果 */
/* 数据上传成功 */
#define GPRS_Rece_ACK_Result_Success	(0X00)
/* 数据上传失败 */
#define GPRS_Rece_ACK_Result_Failure	(0X01)
/* 数据上传有误 */
#define GPRS_Rece_ACK_Result_Error		(0X02)
/* 数据不支持 */	
#define GPRS_Rece_ACK_ON_Format			(0X03)
/* 报警处理确认 */	
#define GPRS_Rece_ACK_Alarm_Confirm		(0X04)	
	
	/* 校验码 */
	u8 check_code;
	/* 计数器 */
	u8 num;
	/* 服务器返回的数据长度 */
	u8 gprs_rece_data_len;
	
	//1、逆向转义数据
	/* 先对 接收数据 进行逆向转义 */
	gprs_rece_data_len = GPRS_Network_To_Host_Translator_Data(GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,
															  GPRS_Rece_Sensor_Data_Buffer,
															  GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len);
	
	//2、解析数据
	/* 判断数据长度 *//* 现在应答消息长度是固定的(20字节) */
	if(gprs_rece_data_len != GPRS_Rece_Network_Data_Len)
	{
		/* 接收到的数据长度与设定的不一致 */
		return FALSE;
	}
	
	/* 判断标识位数据 */
	if((GPRS_Rece_Reg.GPRS_IR_FIX.Start_Iden != GPRS_COM_Identify)||
	   (GPRS_Rece_Reg.GPRS_IR_FIX.End_Iden   != GPRS_COM_Identify))
	{
		/* 头标识或尾标识与协议不一致 */
		return FALSE;
	}
	
	/* 判断消息头 消息ID数据 */
	if((GPRS_Rece_Reg.GPRS_IR_FIX.ACK_ID_Type_HSE != (GPRS_COM_ID_Upload/0X100))||	
	   (GPRS_Rece_Reg.GPRS_IR_FIX.ACK_ID_Type_LSE != (GPRS_COM_ID_Upload%0X100)))
	{
		/* 消息头消息ID与协议不一致 */		
		return FALSE;
	}
	
	/* 判断消息体长度 */	
	if((GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Length != (GPRS_COM_ACK_Att_Len/0X100))||	
	   (GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_LSB_Length != (GPRS_COM_ACK_Att_Len%0X100)))
	{
		/* 消息体长度与协议不一致 */
		return FALSE;
	}
	
	/* 判断消息加密方式 */	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Enc_Mode != GPRS_COM_Enc_Mode)
	{
		/* 消息加密方式与设置不一致 */
		return FALSE;
	}
	
	/* 判断消息分包标志 */	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Sub_Flag != GPRS_COM_Sub_Flag)
	{
		/* 消息分包标志与设置不一致 */
		return FALSE;
	}	
	
//	/* 判断消息终端序列号 */	
//	if((GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||	
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != ))	
//	{
//		/* 消息终端序列号与仪器SN号不一致 */
//		return FALSE;	
//	}
	
	//这里的 “应答流水号” 和 “应答 ID”没有什么实质性的意义所以就不进行判断
	
	/* 判断消息应答结果 */
	if(GPRS_Rece_Reg.GPRS_IR_FIX.ACK_Result != GPRS_Rece_ACK_Result_Success)
	{
		/* 消息应答异常 */
		//可以设置查看异常类型
		//这里就用一个nop来定位，具体用仿真来查看结果，假如需要打印或显示出来就的做相应的函数功能。
		__nop();
		return FALSE;
	}
	
	/* 清除 校验 数据寄存器 */
	check_code = 0;
	/* 判断数据校验 */
	/* 计算 逆向转义后的网络序数据的校验  */
	for(num = 1;num < (GPRS_Rece_Network_Data_Len-2); num++)
	{
		/* 这里的校验方式:对有效的数据段取异或 */
		check_code ^= GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer[num];
	}
	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Check_Code != check_code)
	{
		/* 消息数据校验异常 */
		return FALSE;
	}
	
	/* 网络序数据正常，数据发送成功。 */
	return TRUE;
	
#undef GPRS_Rece_Network_Data_Len	
#undef GPRS_Rece_ACK_Result_Success
#undef GPRS_Rece_ACK_Result_Failure
#undef GPRS_Rece_ACK_Result_Error
#undef GPRS_Rece_ACK_ON_Format
#undef GPRS_Rece_ACK_Alarm_Confirm
}
#elif (AppGPRSComProtocol == 0x01)//解析HTTP协议返回来的数据
static BOOL GPRS_Parsing_Network_Data(void)
{
	/* 数据指针 */
	char * datap;
		
	/* 清除数据缓存 */
	memset(GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,0x00,GPRS_Rece_Buffer_Size);
	/* 拷贝接收数据 */
	strcpy((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,(char*)GPRS_Rece_Sensor_Data_Buffer);
	
	
	/* 查询HTTP传输成功字符串 */
	datap = strstr((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,"HTTP/1.1 200 OK");
	/* 判断HTTP传输是否成功 */
	if(NULL == datap)
	{
		/* 传输失败 */
		return FALSE;
	}
	
	/* 查询数据平台成功接收字符串 */
	datap = strstr((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,"\"code\":\"0000\"");
	/* 判断数据平台是否成功接收数据 */
	if(NULL == datap)
	{
		/* 传输失败 */
		return FALSE;
	}

	return TRUE;
}
#elif (AppGPRSComProtocol == 0x02)//解析开放性协议返回来的数据
static BOOL GPRS_Parsing_Network_Data(void)
{
	
	/* 没有历史数据 */
	return ComProtZDE_ParseData(GPRS_Rece_Sensor_Data_Buffer,
		   &GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len);
}
#elif (AppGPRSComProtocol == 0x04)//解析江苏省免疫规划数据中心协议返回来的数据
static const u8 JS_DL_CMD_Buffer[6]={0x55,0x7A,0x04 ,0x85 ,0x00 ,0x00 };

static BOOL GPRS_Parsing_Network_Data(void)
{
	 if( GPRS_Rece_Sensor_Data_Buffer[0]!=JS_DL_CMD_Buffer[0]  || \
		   GPRS_Rece_Sensor_Data_Buffer[1]!=JS_DL_CMD_Buffer[1]  || \
	     GPRS_Rece_Sensor_Data_Buffer[2]!=JS_DL_CMD_Buffer[2]  || \
		   GPRS_Rece_Sensor_Data_Buffer[3]!=JS_DL_CMD_Buffer[3]  || \
	     GPRS_Rece_Sensor_Data_Buffer[4]!=JS_DL_CMD_Buffer[4]  || \
		   GPRS_Rece_Sensor_Data_Buffer[5]!=JS_DL_CMD_Buffer[5] )
{
	
			 Log_printf("服务器解析设备上传的数据失败！！！\n");

        return FALSE;//服务器解析数据失败

}
	 return TRUE;
}
#else
static BOOL GPRS_Parsing_Network_Data(void)
{
	__NOP();
	return TRUE;
}
#endif
#endif

/*
*********************************************************************************************************
*	函 数 名: GSM_Signal_Strength
*	功能说明: 根据现在的信号强度，量化显示。
*	形    参：signal_num :当前 的 GSM 模块的信号强度信息。
*			 max_signal :最大的正常信号强度。
*			 sub		:信号细分。
*	返 回 值: 在菜单界面上显示的信号。
*********************************************************************************************************
*/
u8 GSM_Signal_Strength(int* signal_num)
{
/* 显示信号强度的区间设置 */
//0-5:0格
#define GPRS_Signal_Strength_0		(5u)
//6-8:1格
#define GPRS_Signal_Strength_1		(8u)
//9-11:2格
#define GPRS_Signal_Strength_2		(11u)	
//12-14:3格
#define GPRS_Signal_Strength_3		(14u)		
//15-31:4格
#define GPRS_Signal_Strength_4		(15u)
/* GPRS 最大信号强度 */	
#define GPRS_Max_Signal				(31u)
	
	
	
	if((*signal_num > GPRS_Max_Signal)||(*signal_num <= 0))
	{
		/* 信号 检测失败 */
		/* 当前信号强度：0格 */
		return 0;
	}

	/* 判断当前信号强度 */
	if(*signal_num <= GPRS_Signal_Strength_0)
	{
		/* 当前信号强度：0格 */
		return 0;
	}else if((*signal_num > GPRS_Signal_Strength_0)&&(*signal_num <= GPRS_Signal_Strength_1))
	{
		/* 当前信号强度：1格 */
		return 1;
	}else if((*signal_num > GPRS_Signal_Strength_1)&&(*signal_num <= GPRS_Signal_Strength_2))
	{
		/* 当前信号强度：2格 */
		return 2;
	}else if((*signal_num > GPRS_Signal_Strength_2)&&(*signal_num <= GPRS_Signal_Strength_3))
	{
		/* 当前信号强度：3格 */
		return 3;
	}else if((*signal_num > GPRS_Signal_Strength_3)&&(*signal_num <= GPRS_Max_Signal))
	{
		/* 当前信号强度：4格 */
		return 4;
	}else
	{
		/* 当前信号强度：0格 */
		return 0;
	}
	
#undef GPRS_Signal_Strength_0
#undef GPRS_Signal_Strength_1
#undef GPRS_Signal_Strength_2
#undef GPRS_Signal_Strength_3	
#undef GPRS_Signal_Strength_4
#undef GPRS_Max_Signal
}

/************************************************************************************************/
/* 函数名	: GSMGetSignalStrength										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: GSM信号强度																		*/
/* 作用	 	: 读取当前GSM信号强度					 											*/
/* 创建日期	: 2017/02/23																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
uint8_t GSMGetSignalStrength(void)
{
	return GPRS_SR_Reg.GPRS_Sign_Stre;
}

/*
*********************************************************************************************************
*	函 数 名: GSM_Signal_Check
*	功能说明: 检测当前 GSM信号:如果信号正常就显示信号强度；不正常就 重新配置 GSM 模块(防止模块进入异常的工作状态)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GSM_Signal_Check(void)
{
/* GSM 信号异常 */
#define GPRS_Signal_Error		                          	(-1)
/* GPRS 最大信号强度 */	                              
#define GPRS_Max_Signal			                          	(31u)
	                                                    
	/* 判断 GSM模块配置状态 */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		/* 模块配置成功 */
		if((GPRS_SR_Reg.GPRS_Sign_Stre >= 0)&&(GPRS_SR_Reg.GPRS_Sign_Stre <= GPRS_Max_Signal))
		{
			/* GSM工作正常 */
			/* 信号强度 显示处理 */
			GPRS_SR_Reg.GPRS_Sign_Stre_Disp = GSM_Signal_Strength(&GPRS_SR_Reg.GPRS_Sign_Stre);
		}else if(GPRS_SR_Reg.GPRS_Sign_Stre == GPRS_Signal_Error)//(-1）
		{
//			/* GSM工作不正常 */
			
			/* 设置 显示的 信号强度:0 */
//			GSM_Tab.Signal_Display = 0;
//			/* 修改 GSM模块状态:失败 */
//			GSM_Tab.GSM_State = FALSE;
//			/* 需要 重新 配置GSM模块 */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
//			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//			{
//				/* GSM模块信号强度异常 */
//				printf("\rGSM模块信号强度异常\r\n");
//			}
		}else
		{
			/* 未知状态 */
			/* 设置 显示的 信号强度:0 */
//			GSM_Tab.Signal_Display = 0;
			
		}
//		/* 判断单前附着状态 */
//		if(GPRS_SR_Reg.GPRS_Link_State == FALSE)
//		{
//			/* 附着 失败 */
//			/* 连接 失败 时间 */
//			if(GSM_GPRS_Link_Fail_Num >= 300)
//			{
//				/* 连接时间超出 设置值 重新 配置GSM模块 */
//				GSM_GPRS_Link_Fail_Num = 0;
//				/* 设置 显示的 信号强度:0 */
//				GSM_Tab.Signal_Display = 0;
//				/* 修改 GSM模块状态:失败 */
//				GSM_Tab.GSM_State = FALSE;
//				/* 需要 重新 配置GSM模块 */
//				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/* GSM模块 附着状态 异常 */
//					printf("\rGSM模块附着状态异常\r\n");
//				}				
//			}else
//			{
//				/* GSM的GPRS功能连接失败 */
//				GSM_GPRS_Link_Fail_Num++;
//			}
//		}else
//		{
//			/* 附着 成功 */
//			if(GSM_GPRS_Link_Fail_Num)
//			{
//				/* 清除 附着 失败 时间 */
//				GSM_GPRS_Link_Fail_Num = 0;
//			}
//		} 
	}else
	{
		/* 模块 “未配置” 或 “配置失败” */
		/* 设置信号强度 为 0 */
		GPRS_SR_Reg.GPRS_Sign_Stre = 0;
		/* 设置 附着状态 为 未连接 */
		GPRS_SR_Reg.GPRS_Link_State = FALSE;
		/* 需要 重新 配置GSM模块 */
		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
	}
	

#undef GPRS_Signal_Error
#undef GPRS_Max_Signal
}

/*
*********************************************************************************************************
*	函 数 名: GSM_Link_Check
*	功能说明: GPRS 附着状态 检查 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void GSM_Link_Check(void)
{

	/* 判断 GSM模块配置状态 */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
	
	
	}

}
#if (GSM_Module_Type==0x00) //SIM800C模块	
/************************************************************************************************/
/* 函数名	：GSM_Send_SMS																		*/
/* 输入		：短信发送的手机号码																	*/
/* 输出		：无 																				*/
/* 作用		：发送短信息						 									 		 		*/
/* 创建日期	：2015/04/10																		*/
/************************************************************************************************/
static void GSM_SMS_Send(INSTRU_GSM* gprs_inf)
{

/* 判断用户数量 */
#if SMS_User_Number == 0x01	         //短信报警号码个数
	/* 判断 短信号码 是否是需要发送报警的号码 */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))//号码为空时不发送短信
	{
		/* 这个号码不发送短信 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* 发送 用户1 的短信 */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//发送号码1短信函数
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//发送号码1短信函数
		 if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("向%s用户发送报警短信成功！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("向%s用户发送报警短信失败！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
		
	}
#elif SMS_User_Number == 0x02        //短信报警号码个数
	/* 判断 短信号码 是否是需要发送报警的号码 */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))
	{
		/* 这个号码不发送短信 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* 发送 用户1 的短信 */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);		
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//发送号码1短信函数
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("向%s用户发送报警短信成功！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("向%s用户发送报警短信失败！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}
	
	/* 判断 短信号码 是否是需要发送报警的号码 */
	if((gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11 == 0))
	{
		/* 这个号码不发送短信 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = TRUE;
	}else
	{
		/* 发送 用户2 的短信 */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11+'0';
////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//发送号码2短信函数
		 if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]){
		  Log_printf("向%s用户发送报警短信成功！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("向%s用户发送报警短信失败！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}

#elif SMS_User_Number == 0x03      //短信报警号码个数
	/* 判断 短信号码 是否是需要发送报警的号码 */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))
	{
		/* 这个号码不发送短信 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* 发送 用户1 的短信 */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//发送短信报警号码函数
	  GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//发送号码1短信函数	
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("向%s用户发送报警短信成功！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("向%s用户发送报警短信失败！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
		
	}                                                                         //短信报警号码        //短信报警内容
	
	/* 判断 短信号码 是否是需要发送报警的号码 */
	if((gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11 == 0))
	{
		/* 这个号码不发送短信 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = TRUE;
	}else
	{
		/* 发送 用户2 的短信 */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11+'0';
////////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);//发送短信报警号码函数
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//发送号码2短信函数
			if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]){
		  Log_printf("向%s用户发送报警短信成功！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("向%s用户发送报警短信失败！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}                                                                                     //短信报警号码         //短信报警内容
	
	/* 判断 短信号码 是否是需要发送报警的号码 */
	if((gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_11 == 0))
	{
		/* 这个号码不发送短信 */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = TRUE;
	}else
	{
		/* 发送 用户3 的短信 */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_11+'0';
//////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);//发送短信报警号码函数
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//发送号码3短信函数
	                                                                                   //短信报警号码              //短信报警内容
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2]){
		    Log_printf("向%s用户发送报警短信成功！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		   }else{
		    Log_printf("向%s用户发送报警短信失败！！！\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		   }
	
	}                                                                                
	
	

#endif
	
	/* 设置 短信发送完成状态 */
	GSM_SMS_SR_Reg.GSM_SMS_Send_State = FALSE;
}

#endif


/*
*********************************************************************************************************
*	函 数 名: 
*	功能说明: 接收数据缓存，判断配置表是否有修改，有修改的话，改配置表里的对应数据
*	传递参数：*buf：接收数组缓存，非空(配置信息)
					  len：数组缓存 长度
						* Config_Content：配置表 信息 结构体
						Packet_num：数组缓存里 包含几包数据!
						* tab：根据缓存修改的内容，修改内存中 仪器配置表
*	返 回 值: 无 __nop(); 
*********************************************************************************************************
*/
void RECBUF_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE (*Config_Content)[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB)
{
	u32 data_Length=0;
	u16 j=0,i=0;
	
	//里面用到的数组 要考虑下溢出保护的问题！
	for(i=0;i<Packet_num;i++)//数组中 有几包数据(小包数据) 处理几次
	{
		/* 复制 参数 */
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type = buf[0+data_Length];
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder = buf[2+data_Length]+buf[1+data_Length]*256;
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len = buf[4+data_Length]+buf[3+data_Length]*256;
//		memcpy(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR_REG,&buf[0+data_Length],Config_Tab_Chane_Size); 
		data_Length=data_Length+Config_Tab_Chane_Size;//类型 地址 数据长度 5字节	
		
		if(0x01==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//系统信息
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_System.Inf_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}
			data_Length=data_Length+j;
		}
		else if(0x02==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//电源状态信息
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Power.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}	  
			data_Length=data_Length+j;			
		}
		else if(0x03==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//系统时钟信息
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_RTC.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x04==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//GSM模块信息
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_GSM.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}			
			data_Length=data_Length+j;			
		}
		else if(0x05==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//传感器信息
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Sensor.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) = 
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x06==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//菜单控制信息,即报警总开关信息地址
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Menu.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x07==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//传感器校准数据表信息
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Data_Tab.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		
	}
  
}

/************************************************************************************************/
/* 函数名	: GPRS_Disp_Sign_Stre															 	*/
/* 输入		: 无 																				*/
/* 输出		: 当前GPRS信号强度(0-5:0格;6-8:1格;9-11:2格;12-14:3格;15-31:4格;)						*/
/* 作用		: 更新任务当中的各种状态信息						 									*/
/* 创建日期	: 2015/04/07																		*/
/************************************************************************************************/
GSM_GPRS_SR_Struct GPRS_Disp_Sign_Stre(void)
{
	return GPRS_SR_Reg;
}


#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
/************************************************************************************************/
/* GPRS发送时钟校准数据消息头消息ID */
#define GPRS_Send_ID_System_Clock_Calibration			(0x0FF0)
/* GPRS接收时钟校准数据消息头消息ID */
#define GPRS_Rece_ID_System_Clock_Calibration			(0x8FF0)
/* GPRS发送时钟校准数据消校验 */
#define GPRS_Send_ID_System_Clock_Calibration_Check		(0xFF)
/* GPRS发送时钟校准数据寄存器大小 */
#define GPRS_Send_System_Clock_Calibration_Buffer_Size	(15u)

/************************************************************************************************/
/* GPRS发送时钟校准数据寄存器 */
static const u8 GPRS_Send_System_Clock_Calibration_Buffer[GPRS_Send_System_Clock_Calibration_Buffer_Size] = 
{
	/* 头 标识位 */ 				//(1 Byte)
	GPRS_COM_Identify,
	/* 通信 消息头 */			//
	/* 消息 ID(命令) */			//(2 Byte)
	GPRS_Send_ID_System_Clock_Calibration/0X100,
	GPRS_Send_ID_System_Clock_Calibration%0X100,
	/* 消息体 属性 */			//(2 Byte)
	0x00,0x00,
	/* 终端序列号(SN号): */		//(6 Byte)
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* 消息 流水号 */			//(2 Byte)
	GPRS_COM_Serial_Num,GPRS_COM_Serial_Num,
	/* 消息校验 */				//(1 Byte)
	GPRS_Send_ID_System_Clock_Calibration_Check,
	/* 尾 标识位 */				//(1 Byte)
	GPRS_COM_Identify,
};
/************************************************************************************************/
/* GPRS发送时钟校准数据缓存 */
static u8 GPRS_Rece_System_Clock_Calibration_Buffer[GPRS_Rece_System_Clock_Calibration_Buffer_Size];
/************************************************************************************************/
/* GPRS发送时钟校准数据控制寄存器 */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_System_Clock_Calibration_CR_Reg = 
{
	/* GPRS 接收 传感器 数据数量 */
	GPRS_Rece_System_Clock_Calibration_Buffer_Size,
	/* GPRS 发送 传感器 数据数量 */
	GPRS_Send_System_Clock_Calibration_Buffer_Size,
	
	/* GPRS 发送传感器数据通信状态（TURE:正在发送，FALSE:没有发送） */
	FALSE,
	/* GPRS 主机序传感器数据等待处理状态（TURE:等待处理，FALSE:没有数据处理） */
	FALSE,
	/* GPRS 网络序传感器数据等待处理状态（TURE:等待处理，FALSE:没有数据处理） */
	FALSE,
	/* GPRS 发送传感器数据结果 */
	TRUE,
	/* GPRS 接收传感器数据结果 */
	TRUE,	
	/* GPRS 解析数据状态   */	
	TRUE,
	/* GPRS 发送传感器数据 失败次数 */
	0,
};
/************************************************************************************************/
/* GPRS发送时钟校准数据寄存器 */
static GSM_GPRS_RECE_CLOCK_CALI_Struct GPRS_Rece_System_Clock_Calibration_DR;
#elif (AppGPRSComProtocol == 0x01)
/* GPRS发送时钟校准数据控制寄存器 */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_System_Clock_Calibration_CR_Reg = 
{
	/* GPRS 接收 传感器 数据数量 */
	0,
	/* GPRS 发送 传感器 数据数量 */
	0,
	
	/* GPRS 发送传感器数据通信状态（TURE:正在发送，FALSE:没有发送） */
	FALSE,
	/* GPRS 主机序传感器数据等待处理状态（TURE:等待处理，FALSE:没有数据处理） */
	FALSE,
	/* GPRS 网络序传感器数据等待处理状态（TURE:等待处理，FALSE:没有数据处理） */
	FALSE,
	/* GPRS 发送传感器数据结果 */
	TRUE,
	/* GPRS 接收传感器数据结果 */
	TRUE,
	/* GPRS 解析数据状态   */	
	TRUE,
	/* GPRS 发送传感器数据 失败次数 */
	0,
};
#endif
#endif
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x04))
/* 系统时间更新数据寄存器 */
static Times System_Clock_Updata_DR;
#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
/************************************************************************************************/
/* 函数名	: GPRS_Parsing_System_Clock_Calibration_Data										*/
/* 输入		: parsing_data_buffer:接收到服务器的时钟数据缓存。									*/
/* 输出		: TRUR:成功接收服务器时钟数据；FALSE:	服务器返回数据时钟数据异常						*/
/* 作用		: 解析服务器返回的时间数据，并判断本次GPRS通信状态。							 		*/
/* 创建日期	: 2015/06/26																		*/
/************************************************************************************************/
static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer)
{
/* 接收的数据长度 */
#define GPRS_Rece_Data_Len				(GPRS_Rece_System_Clock_Calibration_Buffer_Size)
/* 接收的消息体数据长度 */
#define GPRS_Rece_Attribute_Length_Len	(8u)
	
	/* 校验码 */
	u8 check_code;
	/* 计数器 */
	u8 num;
	/* 服务器返回的数据长度 */
	u8 gprs_rece_data_len;
	
#if	(System_Clock_Enable == 0x01)
	/* 系统更新时间的秒寄存器 */
	u32 system_clock_updata_second_b;
#endif
	
	//1、逆向转义数据
	/* 先对 接收数据 进行逆向转义 */
	gprs_rece_data_len = GPRS_Network_To_Host_Translator_Data(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_Buffer,
															  parsing_data_buffer,
															  GPRS_Rece_Data_Len);
	
	//2、解析数据
	/* 判断数据长度 *//* 现在应答消息长度是固定的(20字节) */
	if(gprs_rece_data_len != GPRS_Rece_Data_Len)
	{
		/* 接收到的数据长度与设定的不一致 */
		return FALSE;
	}
	
	/* 判断标识位数据 */
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Start_Iden != GPRS_COM_Identify)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.End_Iden   != GPRS_COM_Identify))
	{
		/* 头标识或尾标识与协议不一致 */
		return FALSE;
	}
	
	/* 判断消息ID */
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.ID_Type_HSB != (GPRS_Rece_ID_System_Clock_Calibration/0X100))||	
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.ID_Type_LSB != (GPRS_Rece_ID_System_Clock_Calibration%0X100)))
	{
		/* 接收的消息头与协议不一致 */
		return FALSE;
	}
	
	/* 判断消息体长度 */	
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Attribute_HSB_Length != (GPRS_Rece_Attribute_Length_Len/0X100))||	
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Attribute_LSB_Length != (GPRS_Rece_Attribute_Length_Len%0X100)))
	{
		/* 消息体长度与协议不一致 */
		return FALSE;
	}
	
	/* 判断消息终端序列号 */	
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[0] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[1] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[2] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[3] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[4] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[5] != 0))
	{
		/* 消息终端序列号与仪器SN号不一致 */
		return FALSE;
	}
	
	/* 清除 校验 数据寄存器 */
	check_code = 0;
	/* 判断数据校验 */
	/* 计算 逆向转义后的网络序数据的校验  */
	for(num = 1;num < (GPRS_Rece_Data_Len-2); num++)
	{
		/* 这里的校验方式:对有效的数据段取异或 */
		check_code ^= GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_Buffer[num];
	}
	
	if(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Check_Code != check_code)
	{
		/* 消息数据校验异常 */
		return FALSE;
	}
	
	/* 数据全部正常获取服务器时间数据 */
	System_Clock_Updata_DR.Year		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Year) + 2000;
	System_Clock_Updata_DR.Month 	= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Month);
	System_Clock_Updata_DR.Day 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Day);
	System_Clock_Updata_DR.Hour 	= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Hour);
	System_Clock_Updata_DR.Min 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Min);
	System_Clock_Updata_DR.Sec 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Sec);
	

	/* 设置系统时间超前量 */
	/* 将当前的时间数据转换成秒数据。 */
	system_clock_updata_second_b = RTC_Date_Time_To_Second(&System_Clock_Updata_DR);
	//现在由于服务器软件存在数据接收超时保护功能，会导致许多真实数据无法存储到平台上面，因此在与服务器平台进行时间校准时将时间超前服务器平台,
	//这样就可以超前服务器保护数据前将数据上传到云平台上，这样可以减少大量的保护数据。
#if	(System_Clock_Enable == 0x01)
	/* 设置系统超前时间 */
	system_clock_updata_second_b += System_Time_Lead_Num;
#endif
	/* 将秒时间转换成实践时间. */
	RTC_Second_To_Date_Time(system_clock_updata_second_b,&System_Clock_Updata_DR);

	
	/* 网络序数据正常，数据发送成功。 */
	return TRUE;
	
/* 接收的数据长度 */
#undef GPRS_Rece_Data_Len
/* 接收的消息体数据长度 */	
#undef GPRS_Rece_Attribute_Length_Len
}

#elif (AppGPRSComProtocol == 0x01)
/* http时间校准结构 */
typedef struct 
{
	/* 固定字符串（Date: Tue, 07 Jun 2016 01:16:00 GMT）  */
	char Date[4];
	/* 固定字符串“: ” */
	char Sign_1[2];
	/* 星期 */
	char Week[3];
	/* 固定字符串“, ” */
	char Sign_2[2];
	/* 日期 */
	char Day[2];
	/* 固定字符串“ ” */
	char Sign_3;
	/* 月份 */
	char Month[3];
	/* 固定字符串“ ” */
	char Sign_4;
	/* 年份 */
	char Year[4];
	/* 固定字符串“ ” */
	char Sign_5;
	/* 时 */
	char Hour[2];
	/* 固定字符串“:” */
	char Sign_6;
	/* 分 */
	char Min[2];
	/* 固定字符串“:” */
	char Sign_7;
	/* 秒 */
	char Sec[2];
	/* 固定字符串“:” */
	char Sign_8;
	/* 格林威治时间“GMT” */
	char Sign_9[3];
}HTTPClockCalibrationStruct;



static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer)
{
	/* 数据指针 */
	char * datap;
	/* 计数器 */
	u8 num;
	char   *stop_at;
	
	/* 星期 */
	char* weekData[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun",};
	/* 月份 */
	char* monthData[] = {"Jan" ,"Feb" ,"Mar" ,"Apr" ,"May" ,"Jun",
						 "Jul" ,"Aug" ,"Sep" ,"Oct" ,"Nov" ,"Dec",};
	
	/* 日期时间结构 */
	HTTPClockCalibrationStruct * timestamp;
						 

	/* 系统更新时间的秒寄存器 */
	u32 system_clock_updata_second_b;
					 
	
	/* 查询HTTP传输成功字符串 */
	datap = strstr((char *)parsing_data_buffer,"HTTP/1.1 200 OK");
	/* 判断HTTP传输是否成功 */
	if(NULL == datap)
	{
		/* 传输失败 */
		return FALSE;
	}
	
	/* 查询数据平台成功接收字符串 */
	datap = strstr((char *)parsing_data_buffer,"\"code\":\"0000\"");
	/* 判断数据平台是否成功接收数据 */
	if(NULL == datap)
	{
		/* 传输失败 */
		return FALSE;
	}
	
	{
		/* 查询数据平台成功接收字符串 */
		datap = strstr((char *)parsing_data_buffer,"Date: ");
		/* 判断数据平台是否成功接收数据 */
		if(NULL == datap)
		{
			/* 传输失败 */
			return FALSE;
		}
		
		/* 设置时间戳 */
		timestamp = (HTTPClockCalibrationStruct *)datap;
		
		/* 设置星期 */
		/* 数据全部正常获取服务器时间数据 */
		for(num = 0;num < 7;num++)
		{
			/* 查询数据平台成功接收字符串 */
			datap = strstr((char *)parsing_data_buffer,weekData[num]);
			/* 判断数据平台是否成功接收数据 */
			if(NULL != datap)
			{
				/* 设置星期 */
				System_Clock_Updata_DR.Week = num+1;
				break;
			}
		}
		
		/* 判断星期数是否大于等于7 */
		if(num >= 7)
		{
			/* 传输失败 */
			return FALSE;
		}
		
		/* 设置月份 */
		/* 数据全部正常获取服务器时间数据 */
		for(num = 0;num < 12;num++)
		{
			/* 查询数据平台成功接收字符串 */
			datap = strstr((char *)parsing_data_buffer,monthData[num]);
			/* 判断数据平台是否成功接收数据 */
			if(NULL != datap)
			{
				/* 设置星期 */
				System_Clock_Updata_DR.Month = num+1;
				break;
			}
		}
		
		/* 判断星期数是否大于等于7 */
		if(num >= 12)
		{
			/* 传输失败 */
			return FALSE;
		}
		
		/* 设置日期 */
		stop_at = &timestamp->Sign_3;

		System_Clock_Updata_DR.Day = strtoul(timestamp->Day,&stop_at,10);
		
		/* 设置年份 */
		stop_at = &timestamp->Sign_5;

		System_Clock_Updata_DR.Year = strtoul(timestamp->Year,&stop_at,10);
		
		/* 设置"时" */
		stop_at = &timestamp->Sign_6;

		System_Clock_Updata_DR.Hour = strtoul(timestamp->Hour,&stop_at,10);
		
		/* 设置"分" */
		stop_at = &timestamp->Sign_7;	

		System_Clock_Updata_DR.Min 	= strtoul(timestamp->Min,&stop_at,10);
		
		/* 设置"秒" */
		stop_at = &timestamp->Sign_8;

		System_Clock_Updata_DR.Sec 	= strtoul(timestamp->Sec,&stop_at,10);
	}
	/* 平台接收数据正常，时间解析 */
	

	/* 将当前的时间数据转换成秒数据。 */
	system_clock_updata_second_b = RTC_Date_Time_To_Second(&System_Clock_Updata_DR);
	//现在由于服务器软件存在数据接收超时保护功能，会导致许多真实数据无法存储到平台上面，因此在与服务器平台进行时间校准时将时间超前服务器平台,
	//这样就可以超前服务器保护数据前将数据上传到云平台上，这样可以减少大量的保护数据。
#if	(System_Clock_Enable == 0x01)
	/* 设置系统时间超前量 */
	system_clock_updata_second_b += System_Time_Lead_Num;
#endif
	/* 由于HTTP用的时间是格林威治时间晚北京时间8个小时，因此这里要补上8个小时 */
	system_clock_updata_second_b += 8*60*60;
	/* 将秒时间转换成实践时间. */
	RTC_Second_To_Date_Time(system_clock_updata_second_b,&System_Clock_Updata_DR);
	
	/* 网络序数据正常，数据发送成功。 */
	return TRUE;
	
}
#endif
#endif

/************************************************************************************************/
/* 函数名	: GPRS_Send_System_Clock_Calibration_Data											*/
/* 输入		: 无 																				*/
/* 输出		: 无																				*/
/* 作用		: GPRS发送时钟校准数据							 									*/
/* 创建日期	: 2015/06/26																		*/
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))//部标协议和
static  void GPRS_Send_System_Clock_Calibration_Data(void)//zwc
{

	//1、发送数据
	/* 设置接收发送数据长度。 */
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len = GPRS_Send_System_Clock_Calibration_Buffer_Size;//接收数据长度
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len = GPRS_Rece_System_Clock_Calibration_Buffer_Size;//发送数据长度

#if	(AppGPRSComProtocol == 0x00)
	/* 发送时钟校准信息   当是部标协议时时钟校准的服务器为数据服务器*/
		/* 发送时钟校准信息 */
  GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
   (BOOL)Gsm_gprs_comm(
                       (char*)GPRS_Send_System_Clock_Calibration_Buffer,
  	                   (GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len),
                       (char*)GPRS_Rece_System_Clock_Calibration_Buffer,
                       ((&GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len)),
  								      GSM_GPRS_Data
  				             );
#endif	
#if	(AppGPRSComProtocol == 0x04)
	/* 发送时钟校准信息   当是江苏省免疫规划数据中心协议时时钟校准的服务器为配置服务器*/
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
	(BOOL)Gsm_gprs_comm(
	   (char*)GPRS_Send_System_Clock_Calibration_Buffer,
		 GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len,
	   (char*)GPRS_Rece_System_Clock_Calibration_Buffer,
	   ((int*)(&GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len)),
		 GSM_GPRS_Config);
#endif	
	//2、接收数据	  
	/* 判断 数据发送结果 */
	if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result == FALSE)
	{
		  /* 恢复系统时钟校准状态寄存器,重新获取时钟数据 */
		  RTC_System_Clock_Calibration_State_Reset();
	}else
	{
		Log_printf("向数据服务器获取时间信息成功！！！\r\n");	
		/* 数据发送成功 */
		/* 解析服务器返回的时间数据，判断本次GPRS通信状态。 */
		GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_System_Clock_Calibration_Data(GPRS_Rece_System_Clock_Calibration_Buffer);
		/* 判断 本次GPRS通信 结果 */
		if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result == TRUE)
		{
			/* 服务器接收数据成功 */
			/* 更新RTC时钟 */
			/* 复位 RTC */
			RTC_Init(TRUE);
			/* 设置 时间 */
			Write_System_Date_Time_Data(&System_Clock_Updata_DR);

		}else
		{
			/* 恢复系统时钟校准状态寄存器,重新获取时钟数据 */
			RTC_System_Clock_Calibration_State_Reset();
		}
	}
	
}
#elif (AppGPRSComProtocol == 0x01)
static void GPRS_Send_System_Clock_Calibration_Data(void)
{
	/* 发送时间校准数据缓存 */
	char clockCalibrationSendBuf[512];
	/* 接收时间校准数据缓存 */
	char clockCalibrationReceBuf[512];
	/* 发送数据长度 */
	int sendLen;
	/* 接收数据长度 */
	int receLen;
	/* 临时时间寄存器 */
	Times time;
	
	
	/* 清除数据缓存 */
	memset(clockCalibrationSendBuf,0x00,512);
	memset(clockCalibrationReceBuf,0x00,512);
	memset(((char*)(&time)),0x00,sizeof(Times));
	/* 设置数据 */

	/* 清除数据接口参数 */
	memset((char*)&HTTPPara,0x00,sizeof(HTTPPara));

	/* 设置网关编号 */
	sprintf(HTTPPara.ApiPara.GatewayIdNo,"%04X%05d",
	((((*(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[0])) & 0xff00) >> 8)|
	((( *(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[0])) & 0x00ff) << 8)),
	((((*(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[2])) & 0xff00) >> 8)|
	((( *(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[2])) & 0x00ff) << 8)));
	/* 将参数的采集时间转换成秒，将这个数据设置为参数签名的时间数据。 */
	HTTPPara.ApiPara.TimeStamp = 0;
	
	/* 清除参数签名数据缓存。 */
	memset(HTTPPara.ApiPara.Sign,0x00,sizeof(HTTPPara.ApiPara.Sign));
	
	/* 未使用GPS设备 */
	HTTPPara.ApiPara.GPS_State = FALSE;
	/* 清除经度数据 *///暂定0，后面设置成无线号数据。
	HTTPPara.ApiPara.Lng = 0;
	/* 清除纬度数据 *///暂定0，后面设置成无线号数据。
	HTTPPara.ApiPara.Lat = 0;
	
	/* 设置采集时间 */
	time.Year 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year;
	time.Month	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month;
	time.Day 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day;
	time.Hour 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour;
	time.Min 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min;
	time.Sec 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec;
	
	/* 设置采集时间 */
	sprintf(HTTPPara.ApiPara.CollectTime,"%04d-%02d-%02d %02d:%02d:%02d",
			time.Year,time.Month,time.Day,time.Hour,time.Min,time.Sec);
	
	/* 获取当前探头数量 */
	HTTPPara.ApiPara.ProbeNum = 0;

	/* 获取时间校准数据 */
	HTTP_Create_Data(clockCalibrationSendBuf);
	
	/* 计算发送数据长度 */
	sendLen = strlen(clockCalibrationSendBuf);
	/* 设置接收数据长度 */
	receLen = 512;
	
	/* 发送时钟校准信息 */
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
	(BOOL)gsm_gprs_comm(
	  (char*)(clockCalibrationSendBuf),
			 (sendLen),
	  (char*)(clockCalibrationReceBuf),
	  ((int*)(&receLen)));

	//2、接收数据	  
	/* 判断 数据发送结果 */
	if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result == FALSE)
	{
		/* 恢复系统时钟校准状态寄存器,重新获取时钟数据 */
		RTC_System_Clock_Calibration_State_Reset();
	}else
	{
		/* 数据发送成功 */
		/* 解析服务器返回的时间数据，判断本次GPRS通信状态。 */
		GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_System_Clock_Calibration_Data((u8*)clockCalibrationReceBuf);
		/* 判断 本次GPRS通信 结果 */
		if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result == TRUE)
		{
			/* 服务器接收数据成功 */
			/* 更新RTC时钟 */
			/* 复位 RTC */
			RTC_Init(TRUE);
			/* 设置 时间 */
			Write_System_Date_Time_Data(&System_Clock_Updata_DR);
			
		}else
		{
			
			/* 恢复系统时钟校准状态寄存器,重新获取时钟数据 */
			RTC_System_Clock_Calibration_State_Reset();
		}
	}
}
#else
static void GPRS_Send_System_Clock_Calibration_Data(void)
{
	__nop();
}

#endif
#endif
/************************************************************************************************/
/* 函数名	: GPRS_Reset_Sensor_Data_Reg														*/
/* 输入		: 无 																				*/
/* 输出		: 无																				*/
/* 作用		: 复位温湿度数据寄存器								 									*/
/* 创建日期	: 2015/06/26																		*/
/************************************************************************************************/
void GPRS_Reset_Sensor_Data_Reg(void)
{
	
	
	/* GPRS 接收 传感器 数据数量 */	
	GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len = 0;
	/* GPRS 发送 传感器 数据数量 */
	GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = 0;
	/* GPRS 发送传感器数据通信状态（TURE:正在发送，FALSE:没有发送） */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = FALSE;
	/* GPRS 主机序传感器数据等待处理状态（TURE:等待处理，FALSE:没有数据处理） */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = FALSE;
	/* GPRS 网络序传感器数据等待处理状态（TURE:等待处理，FALSE:没有数据处理） */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = FALSE;
	/* GPRS 发送传感器数据结果 */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = TRUE;
	/* GPRS 接收传感器数据结果 */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result = TRUE;
	/* GPRS 发送传感器数据 失败次数 */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum	= 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum	= 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum	= 0;
	
}

/************************************************************************************************/
/* 函数名	: GSM_Config_GPRS_IP																*/
/* 输入		: string:IP字符串																	*/
/* 输出		: 无 																				*/
/* 作用		: 设置数据服务器网络IP。																*/
/* 创建日期	: 2016/02/22																		*/
/* 修改日期	: 2016/02/22																		*/
/************************************************************************************************/
BOOL GSM_Config_Network(char * networkstring,NetworkConfigStruct* network)
{

	/* ip值 */
	uint16_t ip[4] ={0,0,0,0};
	/* 计数器 */
	uint32_t num1,num2;
	/* 当前字符值 */
	int8_t asciiValue;
	/* 计算字符串长度 */
	int len = strlen((char*)networkstring);
	
	
	/* 清除数据内容 */
	memset((int8_t*)network,0x00,sizeof(NetworkConfigStruct));
	

	/* 字符串长度大于最大长度 */
	if(len > sizeof(network->DomainName))
	{
		/* 大于域名设计最大长度，不合理 */
		return FALSE;
	}
	
	/* 判断当前字符串参数是否合法 */
	for(num1 = 0,num2 = 0; num1 < len; num1++)
	{
		/* 判断当前字符串 */
		if(networkstring[num1] == '.')
		{
			/*分割符*/
			/* 设置字符位置 */
			num2 ++;
		}else
		{
			/* 获取ASCII值 */
			asciiValue = networkstring[num1] - '0';
			/* 判断ASCII的值是否在0-9范围内 */
            if(asciiValue > 9 || asciiValue < 0)
			{
				/* 存在非数字参数，说明是域名。 */
				memcpy(network->DomainName,networkstring,len);
				return FALSE;
			}else
			{
				/* 计算 */
				ip[num2] = ip[num2] * 10 + asciiValue;
				/* 判断IP值是否合理 */
				if(ip[num2] > 255)
				{
					/* 非IP格式数据 */
					memcpy(network->DomainName,networkstring,len);
					return FALSE;
				}
			}
		}
	}
	
	network->IP.Serial_Number_Number.Address_A = ip[0];
	network->IP.Serial_Number_Number.Address_B = ip[1];
	network->IP.Serial_Number_Number.Address_C = ip[2];
	network->IP.Serial_Number_Number.Address_D = ip[3];

	/* IP设置成功 */
	return TRUE;
}

/******************* (C) ZHOU *****END OF FILE****/
