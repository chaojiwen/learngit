#ifndef __Log_GSM_H
#define __Log_GSM_H

#include "stm32f10x.h"

#include "Mcu_Config.h"

#include "Log_Sensor.h"
#include "Log_SMS.h"

#include "ucos_ii.h"

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//===========================================================================
//
//===========================================================================
//
//  <e0> [数据上传协议]设置.
//  <i>  目前有两种协议:部标协议(标准协议)、HTTP(金卫信定制).
//	<o1.0..2> 选择运行[程序协议]
//  <0=> [部标协议]协议.
//  <1=> [HTTP]协议.
//  <2=> [开放性]协议.
//  <3=> [河北世窗]协议.
//  <4=> [江苏省免疫规划数据中心]协议.
//
//  <o2.0..9>GPRS流量控制
//  <i>GPRS解析接收数据失败，保护时间设置（1分钟=42M、2分钟=21M，依次减）。
//  <60 => 等待[1分钟]
//  <120=> 等待[2分钟]
//  <180=> 等待[3分钟]
//  <240=> 等待[4分钟]
//  <300=> 等待[5分钟]
//  <i>目前只允许最多等待5分钟时间，最少设置1分钟。
//
//  <o3.0..1> 国药接口转发功能
//  <i>  上传传感器数据时，并向国药接口发送一次数据。
//  <0 => [关闭]
//  <1 => [启动]
//
//  <o4.0..1> 万智接口转发功能
//  <i>  上传传感器数据时，并向万智接口发送一次数据。
//  <0 => [关闭]
//  <1 => [启动]
//
//  <o5.0..1> 泽大开放性接口功能
//  <i>  这个接口主要是开放给有自己平台的客户开发使用。
//  <0 => [关闭]
//  <1 => [启动]
//
//  </e>
#define AppGPRSComProtocolEnable		(0x01)//[数据上传协议]设置
#define AppGPRSComProtocol				  (0x00)//选择程序运行协议(0:部标协议;1:Http协议;2:开放性协议；3：河北世窗协议;4:江苏省免疫规划数据中心协议)
#define AppGPRSUsageProtectTime			(300)//GPRS流量控制时间，单位：秒
#define AppGYForwardEnable				  (0)//国药接口转发功能(0:关闭;1:启动)
#define ComProtocol_WZ_Enable			  (0)//万智接口转发功能(0:关闭;1:启动)
#define ComProtocol_ZDE_Enable			(0)// 泽大开放性接口功能(0:关闭;1:启动)
//	<<< end of configuration section >>>

/* GPRS SN号结构大小 */
#define	GPRS_SN_NO_Buffer_Size				(6u)
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
/* GPRS发送数据寄存器结构大小 */
//这里为什么不分配1024字节是因为这个缓存是未转义过的数据，
//数据转义后数据量会曾加，具体多少要看转义符（0X7E\0X7D）的数量
//这里设置成900字节，也就相当于分配了（1024-924=100）字节的转义符号可用
//一般只有几个到几十个转义符号
#define	GPRS_Send_Buffer_Size				(924u)
/* GPRS接收数据寄存器结构大小 */
#define	GPRS_Rece_Buffer_Size				(64u)
#elif (AppGPRSComProtocol == 0x01)
#define	GPRS_Send_Buffer_Size				(1536u)
/* GPRS接收数据寄存器结构大小 */
#define	GPRS_Rece_Buffer_Size				(1024u)
#elif (AppGPRSComProtocol == 0x04)//江苏省免疫规划数据中心协议	
#define	GPRS_Send_Buffer_Size				(512u)


/* GPRS接收数据寄存器结构大小 */
#define	GPRS_Rece_Buffer_Size				(1024u)
#else
#define	GPRS_Send_Buffer_Size				(1u)
/* GPRS接收数据寄存器结构大小 */
#define	GPRS_Rece_Buffer_Size				(1u)
#endif
#endif


/* GPRS数据内容寄存器结构大小 *///这里设置成  是有最大的通道数决定的（传感器通道8个），也就是说最大=12+32*4=140
#define	GSM_GPRS_Data_Content_Buffer_Size	(140u)
/* 传感器 SN缓存大小 */
#define GPRS_Data_Sensor_Buffer_Size		(4u)
/* GPRS接收时钟校准数据寄存器大小 */
#define GPRS_Rece_System_Clock_Calibration_Buffer_Size	(23u)


//GPRS 发送温湿度数据信息 数据结构
typedef union
{
	struct
	{	
		//道路运输车辆通讯协 协议采用大端模式(big-endian)的网络字节序来传递字和双字。
		//这个数据结构只能适应2通道的传感器GPRS数据发送，并且不能传输状态信息。
		/* GPRS 数据信息 */
		/* 头 标识位 */ //(1 Byte)
		u8				Start_Iden;
		/* 通信 消息头 *///(16 Byte)
		/* 消息 ID(命令) */		//(2 Byte)
		u8				ID_Type_HSB;
		u8				ID_Type_LSB;
		/* 消息体 属性 */		//(2 Byte)
		//消息体属性 消息体长度
		u8	 			Attribute_HSB_Length:2;		
		//消息体属性 数据加密方式
		u8	 			Attribute_HSB_Enc_Mode:3;
		//消息体属性 分包
		u8	 			Attribute_HSB_Sub_Flag:1;		
		//消息体属性 保留
		u8 				Attribute_HSB_Reserve:2;
		//消息体属性 消息体长度
		u8	 			Attribute_LSB_Length:8;
		/* 终端序列号(SN号): */	//(6 Byte)
		u8				SN_NO[GPRS_SN_NO_Buffer_Size];
		/* 消息 流水号 */		//(2 Byte)
		u8				Serial_NO_HSB;
		u8				Serial_NO_LSB;

		/* 通信 消息体 *///(3 Byte + C * (16 + 4 * N))
		/* 消息类型:WIFI 卡片式传感器(现在只用这种模式) *///(1 Byte)
		u8				Body_Type;//消息体类型
		/* 消息数据类型:0->实时数据；1->历史数据。 *///(1 Byte)
		u8				Data_Type;
		/* 数据帧数(0-255) *///(1 Byte)
		u8				Data_Frame_Num;
		/* 数据内容(这个主要是为了给后面添加数据的指针) */
		u8				Data_P;

	}GPRS_IR_FIX;
	
	u8 GSM_GPRS_Send_Buffer[GPRS_Send_Buffer_Size];
}GSM_GPRS_Send_Struct;
/************************************************************************************************/
#if (AppGPRSComProtocol == 0x00)//原先部标协议
typedef union
{
	struct
	{	
		/* 传感器(ID号): */	//(4 Byte)
		u8				SN_NO[GPRS_Data_Sensor_Buffer_Size];
		/* 外接电源状态 */
		u8				ET_State;
		/* 预留1 */
		u8				Reserved1;
		/* 预留2 */
		u8				Reserved2;
		/* 电池电量 */
		u8				BT_Charge_Alarm_State;
		/* 时间戳 */			//(6 Byte)
		//年//(1 Byte)
		u8				Timestamp_Year;
		//月//(1 Byte)
		u8				Timestamp_Month;
		//日//(1 Byte)
		u8				Timestamp_Day;
		//时//(1 Byte)
		u8				Timestamp_Hour;
		//分//(1 Byte)
		u8				Timestamp_Min;
		//秒//(1 Byte)
		u8				Timestamp_Sec;
		/* 通道数 */
		u8				CH_Num;
		/* 传感器类型 */
		u8				Sensor_Type;
		/* 数据(这个主要是为了给后面添加数据的指针) */
		float			Data_P;
		
	}GPRS_DR_FIX;
	
	u8 GSM_GPRS_Data_Content_Buffer[GSM_GPRS_Data_Content_Buffer_Size];
}GSM_GPRS_Data_Content_Struct;
#endif

/************************************************************************************************/
//GPRS 接收温湿度数据信息 数据结构
typedef union
{
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))		
	struct
	{	//道路运输车辆通讯协 协议采用大端模式(big-endian)的网络字节序来传递字和双字。
		//这个数据结构只能适应2通道的传感器GPRS数据发送，并且不能传输状态信息。
		/* GPRS 数据信息 */
		/* 头 标识位 */ //(1 Byte)
		u8				Start_Iden;		
		/* 通信 消息头 *///(16 Byte)
		/* 消息 ID(命令) */		//(2 Byte)
		u8				ID_Type_HSB;
		u8				ID_Type_LSB;
		/* 消息体 属性 */		//(2 Byte)
		//消息体属性 消息体长度
		u8	 			Attribute_HSB_Length:2;
		//消息体属性 数据加密方式
		u8	 			Attribute_HSB_Enc_Mode:3;
		//消息体属性 分包
		u8	 			Attribute_HSB_Sub_Flag:1;
		//消息体属性 保留
		u8 				Attribute_HSB_Reserve:2;
		//消息体属性 消息体长度
		u8	 			Attribute_LSB_Length:8;
		/* 终端序列号(SN号): */	//(6 Byte)
		u8				SN_NO[GPRS_SN_NO_Buffer_Size];		
		/* 消息 流水号 */		//(2 Byte)
		u8				Serial_NO_HSB;
		u8				Serial_NO_LSB;

		/* 应答 流水号 */		//(2 Byte)
		u8				ACK_Serial_NO_HSE;
		u8				ACK_Serial_NO_LSE;
		/* 应答 ID */			//(2 Byte)
		u8				ACK_ID_Type_HSE;
		u8				ACK_ID_Type_LSE;
		/* 应答 结果 */			//(1 Byte)
		u8				ACK_Result;
		
		/* 校验码 */				//(1 Byte)
		u8				Check_Code;		
		/* 尾 标识位 */			//(1 Byte)
		u8				End_Iden;	
	}GPRS_IR_FIX;
	u8 GSM_GPRS_Rece_Buffer[GPRS_Rece_Buffer_Size];
#endif		
	
#if (AppGPRSComProtocol == 0x04)//江苏省免疫规划数据中心协议			
	struct
	{
	  u8	 JS_DL_SYNC_ID;//设备接收数据同步标识位值
		u8   JS_DL_STX_ID;//设备接收数据起始标识位值
		u8   JS_Rev_Data_Len;   //设备接收数据数据长度
		u8   JS_Rev_Succeed_Flag;//服务器接收到正确的数据返回的成功标志位
	 int  JS_Rev_Check_Code;////设备接收到的数据校验码
	}GPRS_IR_FIX;
	u8 GSM_GPRS_Rece_Buffer[GPRS_Rece_Buffer_Size];
#endif	
	
	
	
	
}GSM_GPRS_Rece_Struct;





/************************************************************************************************/
/* GPRS接收时钟校准数据结构 */
typedef union
{
	struct
	{	
		//道路运输车辆通讯协 协议采用大端模式(big-endian)的网络字节序来传递字和双字。
		//这个数据结构只能适应2通道的传感器GPRS数据发送，并且不能传输状态信息。
		/* GPRS 数据信息 */
		/* 头 标识位 */ 			//(1 Byte)
		u8				Start_Iden;		
		/* 通信 消息头 */
		/* 消息 ID(命令) */		//(2 Byte)
		u8				ID_Type_HSB;
		u8				ID_Type_LSB;
		/* 消息体 属性 */		//(2 Byte)
		//消息体属性 消息体长度
		u8	 			Attribute_HSB_Length:2;
		//消息体属性 数据加密方式
		u8	 			Attribute_HSB_Enc_Mode:3;
		//消息体属性 分包
		u8	 			Attribute_HSB_Sub_Flag:1;
		//消息体属性 保留
		u8 				Attribute_HSB_Reserve:2;
		//消息体属性 消息体长度
		u8	 			Attribute_LSB_Length:8;
		/* 终端序列号(SN号): */	//(6 Byte)
		u8				SN_NO[GPRS_SN_NO_Buffer_Size];
		/* 消息 流水号 */		//(2 Byte)
		u8				Serial_NO_HSB;
		u8				Serial_NO_LSB;

		/* 应答 流水号 */		//(2 Byte)
		u8				ACK_Serial_NO_HSE;
		u8				ACK_Serial_NO_LSE;
		/* 服务器时间 */			//(6 Byte)
		u8				Server_Year;
		u8				Server_Month;
		u8				Server_Day;
		u8				Server_Hour;
		u8				Server_Min;
		u8				Server_Sec;

		/* 校验码 */				//(1 Byte)
		u8				Check_Code;		
		/* 尾 标识位 */			//(1 Byte)
		u8				End_Iden;	
	}GSM_GPRS_Rece_IR;

	u8 GSM_GPRS_Rece_Buffer[GPRS_Rece_System_Clock_Calibration_Buffer_Size];
}GSM_GPRS_RECE_CLOCK_CALI_Struct;

/* GSM的 消息体的 消息数据类型(1 Byte) */
typedef enum {
	/* 配置GSM模块 */
	Init_GSM_Module_EVENT	                 = 0X00,
	/* 更新信号强度 */                      
	Update_SGL_EVENT		                   = 0X01,
	/* 更新附着状态 */                      
	Update_Link_EVENT		                   = 0X02,
	/* 上传温湿度数据 */                    
	Updata_Data_EVENT 		                 = 0X03,
	/* 下载 程序更新 数据 */                
	IAP_EVENT 				                     = 0X04,
	/* 发送短信 */                         
	Send_SMS_EVENT			                   = 0X05,
	/* 配置服务器 事件 */                   
	ConfigService_EVENT		                 = 0X06,
//	/* 上传 配置回复 数据 */	
//	Updata_Config_Back_EVENT = 0X07,
	/* 系统时钟校准 */
	System_Clock_Calibration_EVENT         = 0x08,
	
	
		/* 配置Wifi名字和密码 事件 */
	SetWifiNamePWD_EVENT		               = 0X09,
	/* 配置WLAN参数(本地IP、子网掩码、网关地址) 事件 */
	SetWifiWLAN_EVENT		                   = 0X0A,
	/* 配置Wifi IP地址获取方式 STATIC或者DHCP模式事件 */
	SetWifiWLAN_GET_IP_WAY_EVENT		       = 0X0B,
	/* 复位Wifi模块事件 */
	RestWifi_EVENT		                     = 0X0C,
	/* Wifi模块事件 */
	Factory_Data_Reset_Wifi_EVENT		       = 0X0D,
	
	
}GSM_CMD_TYPE;

typedef struct
{
	GSM_CMD_TYPE	       EVENT_Cmd;		// 事件 命令
	u32*			           EVENT_Str;		// 事件 数据 指针
	BOOL			           EVENT_Getarg;	// 事件 执行结果(FALSE:信号正在执行，TRUE:型号执行结束)
}GSM_QUE;

/* 配置表 信息 修改 数据结构 */	
typedef union 
{
	struct			 
	{
		/* 修改配置表 类型 */
		u32		Change_Type			:8;
		/* 修改配置表 地址 */
		u32		Change_Adder		:16;
		/* 修改配置表 数据长度 */
		u32		Change_Data_Len		:16;
	}CR;
	u8 CR_REG[Config_Tab_Chane_Size];
}CONFIG_TAB_CHANGE;
/************************************************************************************************/
typedef struct {
	/* GPRS 接收 传感器 数据数量 */
	int 		GPRS_Rece_Sensor_Data_Len;
	/* GPRS 发送 传感器 数据数量 */
	int 		GPRS_Send_Sensor_Data_Len;	
	
	/* GPRS 发送传感器数据通信状态（TURE:正在发送，FALSE:没有发送） */
	BOOL		GPRS_COM_Send_State;
	/* GPRS 主机序传感器数据等待处理状态（TURE:等待处理，FALSE:没有数据处理） */
	BOOL		GPRS_COM_Host_Waiting_State;
	/* GPRS 网络序传感器数据等待处理状态（TURE:等待处理，FALSE:没有数据处理） */
	BOOL		GPRS_COM_Network_Waiting_State;	
	/* GPRS 发送传感器数据结果 */
	BOOL		GPRS_COM_Send_Result;
	/* GPRS 接收传感器数据结果 */
	BOOL		GPRS_COM_Rece_Result;
	/* GPRS 解析数据状态   */	
	BOOL		GPRS_COM_ParseState;
	/* GPRS 发送传感器数据 失败次数 */
	u8			GPRS_COM_DataFailNum;
	/* GPRS 发送配置数据 失败次数 */
	u8			GPRS_COM_ConfigServiceFailNum;
	/* GPRS 发送升级数据 失败次数 */
	u8			GPRS_COM_IAPFailNum;
	
}GSM_GPRS_Send_Sensor_Data_Struct;
/************************************************************************************************/
/* SIM卡状态结构 */
typedef enum 
{
	/* SIM正常 */
	SIM_OK = 0x00,
	
	/* 无GPRS信号 */
	SIM_NoSignal = 0X01,
	
	/* SIM未连接 */
	SIM_NotDetected = 0x02,
	
	/* SIM欠费停机,也有可能是空号 */
	SIM_ShutDown = 0x03,
	
}SCSStruct;
/************************************************************************************************/
//GPRS 状态 结构
typedef struct {
	/* GPRS 信号 强度 */
	int 		GPRS_Sign_Stre;
	/* GPRS 信号 强度 显示(0-5:0格;6-8:1格;9-11:2格;12-14:3格;15-31:4格;) */
	u8			GPRS_Sign_Stre_Disp;
	/* GPRS 模块工作状态 */
	BOOL		GPRS_Work_State;
	/* GPRS 附着状态 */
	BOOL 		GPRS_Link_State;
	/* Sim卡状态信息 */ 
	SCSStruct	SimState;

}GSM_GPRS_SR_Struct;

/************************************************************************************************/
//GSM 短信 状态寄存器
typedef struct {
	/* GSM  短信发送状态（TURE:正在发送，FALSE:发送完成/没有发送） */
	BOOL		GSM_SMS_Send_State;
	/* GSM  短信发送结果（TURE:正在成功，FALSE:发送失败） */
	BOOL		GSM_SMS_Send_User_Result[SMS_User_Number];

}GSM_SMS_SR_Struct;

/************************************************************************************************/
//GSM 短信 数据寄存器
typedef struct {
	/* GSM  短信发送号码寄存器.
	   国内手机号码都为11位，因此这里强制设置为12.最后一位为结束符。 */
	char		GSM_SMS_User_NO[12];


}GSM_SMS_DR_Struct;

/* GSM模块网络配置数据结构 */
typedef struct 
{
	/* IP地址 */
	SERVER_IP_ADDRESS IP;
	/* 端口号 */
	u16 Port;
	/* 域名 */
	u8 DomainName[32];	
}NetworkConfigStruct;

/************************************************************************************************/
/* GPRS运行参数结构 */
typedef struct {

	struct ConfigService
	{
		struct 
		{
			/* 通信结果 */
			BOOL Result;
			
			
		}State;
		
		struct 
		{
			/* 发送数据指针 */
			uint8_t * 	TXDR;
			/* 发送数据长度 */
			int32_t	TXLen;
			/* 接收数据指针 */
			uint8_t * 	RXDR;
			/* 接收数据长度 */
			int32_t	RXLen;
		}Para;
		
	}ConfigService;
	
	struct IAP
	{
		struct 
		{
			/* 通信结果 */
			BOOL Result;
			
			
		}State;
		
		struct 
		{
			/* 发送数据指针 */
			uint8_t * 	TXDR;
			/* 发送数据长度 */
			int32_t	TXLen;
			/* 接收数据指针 */
			uint8_t * 	RXDR;
			/* 接收数据长度 */
			int32_t	RXLen;
		}Para;
		
	}IAP;

}GPRS_Struct;

/************************************************************************************************/
/* GSM 短信 状态寄存器 */
extern GSM_SMS_SR_Struct    GSM_SMS_SR_Reg;
/************************************************************************************************/
/* GPRS 应用寄存器 */
extern GPRS_Struct GPRS;

extern  GSM_GPRS_SR_Struct   GPRS_SR_Reg;
/************************************************************************************************/

/* 函数声明 */
/************************************************************************************************/
void GSM_Function(GSM_QUE*  gsm_que_inf,
				  INSTRU_CONFIG_INF_TAB* config_tab,
				  INSTRU_FIX_INF_TAB* 	 fix_tab);
BOOL GSM_Send_Q(OS_EVENT* pevent,GSM_CMD_TYPE EVENT_Type);

u8 GSM_Signal_Strength(int* signal_num);
uint8_t GSMGetSignalStrength(void);
void GSM_Signal_Check(void);
void RECBUF_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE(*Config_Content)[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB);
void GPRS_Save_Sensor_Data_Check(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
void GPRS_Send_Sensor_Data(void);
GSM_GPRS_SR_Struct GPRS_Disp_Sign_Stre(void);
void GPRS_Reset_Sensor_Data_Reg(void);
u16 GPRS_Host_To_Network_Translator_Data(u8* network_endian,u8* host_endian,u16 T_num);
u16 GPRS_Network_To_Host_Translator_Data(u8* host_endian,u8* network_endian,u16 T_num);
BOOL GSM_Config_Network(char * networkstring,NetworkConfigStruct* network);
void GPRSSendData_ParseStateCheck(void);
void GSMConfig_Server_Inf(INSTRU_GSM* gprs_inf);

void   Remote_upgrade(void);//远程升级函数

#endif /* Log_GSM_H */

