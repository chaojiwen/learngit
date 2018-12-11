/************************************************************************************************/
/** @file              : ComProtocol_ZDE.c														*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017年02月16日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--变送器--通信协议--“泽大通用协议”应用文件					*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : ComProtocol_ZDE.c																*/
/**作用       : 根据“泽大通用协议”提供的通信协议格式将探头数据上传到云平台							*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2017年02月16日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "ComProtocol_ZDE.h"

#if (ComProtocol_ZDE_Enable == 0x01)
//#include <stdio.h>
#include <string.h>
#include "App_Save.h"

/*******************************************************************************/
/********************************* 全局变量声明 *****************************/
/* 最大探头数 */
#define ComProtocolZDE_ProbeNumMax			(16u)

/* 无匹配探头值 */
#define ComProtocolZDE_MatchingCHNull		(0xFF)

/* 标识位值 */
#define ComProtocolZDE_StartIdentify		(0x7E)
#define ComProtocolZDE_EndIdentify			(ComProtocolZDE_StartIdentify)

/* 消息头ID值 */
#define ComProtocolZDE_MessageHeaderID		(0x0900)

/* 消息头ID值 */
#define ComProtocolZDE_ACKMessageID			(0x8001)

/* 一帧数据长度 */
#define ComProtocolZDE_FrameDataLen			(28u+(((IntProbeNum<2)?2:IntProbeNum)*4))



/********************************* “泽大通用协议”参数结构 **************/
/* 参数消息头结构 */
typedef struct
{
	/* 消息ID */
	uint16_t ID;
	/* 网关电源状态 */
	BOOL PowerLinkState;
	/* 网关电量状态 */
	uint8_t PowerLevelState;
	/* 定位标志 */
	BOOL GPSFlag;
	/* 终端序列号模式 */
	uint8_t	SN_Mode;
	/* 终端序列号 */
	uint8_t SN[12];
}
ComProtocolZDEPara_MH;

/* 消息体帧数据探头信息结构 */
typedef struct
{
	/* 探头SN号 */
	uint8_t SN[4];
	/* 探头电量 */
	uint8_t Level;
	/* 探头类型 */
	uint8_t Type;
	/* 通道数量 */ 
	uint8_t ChannelNum;
	/* 保留 */ 
	uint8_t Reserved;
	/* 数据 */ 
	float  Data[8];
}
ComProtocolZDEPara_MBFI;

/* 参数消息体帧数据结构 */
typedef struct
{
	/* 采集时间 */
	uint8_t CollectionTime[6];
	/* 探头数量 */
	uint8_t ProbeNum;
	/* 网关信号强度 */
	uint8_t GSMSignal;
	/* 网关电源状态 */
	BOOL PowerLinkState;
	/* 网关电量状态 */
	uint8_t PowerLevelState;
	/* 经度 */
	float	Lon;
	/* 纬度 */
	float	Lat;
	/* 速度 */
	float	Speed;
	/* 探头数据 */
	ComProtocolZDEPara_MBFI ProbeInfo[16];
}
ComProtocolZDEPara_MBF;

/* 参数消息体结构 */
typedef struct
{
	/* 数据组数 */
	uint32_t FrameNum;
	/* 参数帧 */
	ComProtocolZDEPara_MBF Frame;
}
ComProtocolZDEPara_MB;

/* “泽大通用协议” 参数结构 */
typedef struct
{
	/* 消息头参数 */
	ComProtocolZDEPara_MH MessageHeader;
	/* 消息体参数 */
	ComProtocolZDEPara_MB MessageBody;
	
}
ComProtocolZDEParaStruct;

/********************************* “泽大通用协议”结构 **************/
/* 消息体属性结构 */
typedef struct
{
	/* 消息体长度 */
	uint16_t Len:10;
	/* 数据加密方式 */
	uint16_t EncryptMode:3;
	/* 分包 */
	uint16_t PacketFlag:1;
	/* 保留 */
	uint16_t Reserve:2;
}
ComProtocolZDE_MBA;

/* 消息头结构 */
typedef struct
{
	/* 消息ID */
	uint16_t ID;
	/* 消息体属性 */
	uint16_t Attribute;
	/* 预留 */
	uint8_t Reserved[2];
//	/* 网关电源状态 */
//	BOOL PowerLinkState;
//	/* 网关电量状态 */
//	uint8_t PowerLevelState;
	/* 定位标志 */
	BOOL GPSFlag;
	/* 终端序列号模式 */
	uint8_t	SN_Mode;
	/* 终端序列号 */
	uint8_t SN[12];

	/* 消息包封装项 */
	
}
ComProtocolZDE_MH;

/* 消息体帧数据探头信息结构 */
typedef struct
{
	/* 探头SN号 */
	uint8_t	SN[4];
	/* 探头电量 */
	uint8_t Level;
	/* 探头类型 */
	uint8_t Type;
	/* 通道数量 */ 
	uint8_t ChannelNum;
	/* 保留 */ 
	uint8_t Reserve;
	/* 数据 */ 
	float* Data;
}
ComProtocolZDE_MBFI;

/* 消息体帧数据结构 */
typedef struct
{
	/* 采集时间 */
	uint8_t CollectionTime[6];
	/* 探头数量 */
	uint8_t ProbeNum;
	/* 网关信号强度 */
	uint8_t GSMSignal;
	/* 网关电源状态 */
	uint8_t PowerLinkState;
	/* 网关电量状态 */
	uint8_t PowerLevelState;
	/* 保留1  */
	uint8_t Reserved1[2];
	/* 经度 */
	float	Lon;
	/* 纬度 */
	float	Lat;
	/* 速度 */
	float	Speed;
	/* 保留2  */
	uint8_t Reserved2[4];
	/* 探头数据 */
	ComProtocolZDE_MBFI ProbeInfo;
}
ComProtocolZDE_MBF;

/* 消息体结构 */
typedef struct
{
	/* 数据帧数 */
	uint32_t FrameNum;
	/* 数据内容 */
	ComProtocolZDE_MBF* FrameData;
}
ComProtocolZDE_MB;

typedef struct
{
	/* 消息头 */
	ComProtocolZDE_MH	MessageHeader;
	
	/* 消息体 */
	ComProtocolZDE_MB	MessageBody;
	
	/* 检验码 */
	uint8_t CheckCode;
}
ComProtocolZDEStruct;

/* 数据应答消息结构 */
typedef struct
{
	/* 标识位 */
	uint8_t StartIdentify;
	
	/* 消息应答ID */
	uint8_t ID1;
	uint8_t ID2;
	/* 应答结果 */
	uint8_t Result;
	
	/* 检验码 */
	uint8_t CheckCode;
	
	/* 标识位 */
	uint8_t EndIdentify;
	
	
}
ComProtocolZDEACKStruct;


/********************************* 数据寄存器 *******************************/
/* 万智String协议通信协议应用数据 */
ComProtocolZDEAppStruct ComProtocolZDEApp;

#if (1) //文件函数
/************************************************************************************************/
/* 函数名	: ComProtocolZDE_SetPara										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 设置协议参数					 													*/
/* 创建日期	: 2017/02/17																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static uint8_t ComProtocolZDE_SearchMatchingSN(ComProtocolZDEPara_MBF* probeInfo,uint8_t* sn)
{
	/* 计数器 */
	uint8_t cNum;
	
	/* 查找匹配的SN号 */
	for(cNum = 0;cNum < ComProtocolZDE_ProbeNumMax; cNum++)
	{
		/* 判断当前的SN号是否有效 */
		if((0x00 != probeInfo->ProbeInfo[cNum].SN[0])||
		   (0x00 != probeInfo->ProbeInfo[cNum].SN[1])||
		   (0x00 != probeInfo->ProbeInfo[cNum].SN[2])||
		   (0x00 != probeInfo->ProbeInfo[cNum].SN[3]))
		{
			/* SN号配置过 */
			/* 判断当前的SN号与目标SN号是否相同 */
			if((*sn == probeInfo->ProbeInfo[cNum].SN[0])&&
			   (*(sn+1) == probeInfo->ProbeInfo[cNum].SN[1])&&
			   (*(sn+2) == probeInfo->ProbeInfo[cNum].SN[2])&&
			   (*(sn+3) == probeInfo->ProbeInfo[cNum].SN[3]))
			{
				/* 返回当前的通道编号 */
				return cNum;
			}
		}
	}

	/* 为查找到匹配的SN号 */
	return ComProtocolZDE_MatchingCHNull;
}

/************************************************************************************************/
/* 函数名	: ComProtocolZDE_SetMessageHeaderPara										 		*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 设置协议的消息头参数					 											*/
/* 创建日期	: 2017/02/21																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
BOOL PowerLinkState;
uint8_t PowerLevelState;
static void ComProtocolZDE_SetMessageHeaderPara(INSTRU_CONFIG_INF_TAB* config_tab,
												ComProtocolZDEStruct* cProtocolReg)
{
	/* 消息体属性寄存器 */
	ComProtocolZDE_MBA cMessageAttribute = {0};
	/* 计数器 */
	uint8_t cNum;
	
//	/* 根据配置寄存器设置数据 */
//	cProtocolReg->StartIdentify = ComProtocolZDE_StartIdentify;
	
	/* 消息头ID */
	cProtocolReg->MessageHeader.ID = ComProtocolZDE_MessageHeaderID;
	
	/* 消息体属性 */
	/* 机密方式:不加密 */
	cMessageAttribute.EncryptMode = 0;
	/* 发送数据长度 */
	cMessageAttribute.Len = 0;
	/* 分包标志:不分包 */
	cMessageAttribute.PacketFlag = 0;
	/* 设置消息体属性参数 */
	cProtocolReg->MessageHeader.Attribute = *((uint16_t*)(&cMessageAttribute));
	
//	/* 网关电源状态 */
//	PowerLinkState = 
//	config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
//	
//	/* 网关电量状态 */
//	PowerLevelState = 
//	config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	
	//预留
	cProtocolReg->MessageHeader.Reserved[0]=0;
	cProtocolReg->MessageHeader.Reserved[1]=0;

	/* 网关类型 */
	if(0x01 == GPS_Config_Enable)
	{
		/* 搭载定位设备 */
		cProtocolReg->MessageHeader.GPSFlag = TRUE;
	}else
	{
		/* 未搭载定位设备 */
		cProtocolReg->MessageHeader.GPSFlag = FALSE;
	}

	/* 终端序列号模式 目前先固定为1:设备号模式 */
	cProtocolReg->MessageHeader.SN_Mode = 0x01;
	
	/* 这里使用memcpy为什么会导致HardFault_Handler？不清楚 */
	
	for(cNum = 0;cNum <sizeof(config_tab->Inf_State.Instru_System.Inf.Serial_Number);cNum++)
	{
		/* 拷贝 */
		cProtocolReg->MessageHeader.SN[cNum] = 
		config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[cNum];
	}
	
}

/************************************************************************************************/
/* 函数名	: ComProtocolZDE_SetMessageBodyPara										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 设置协议参数					 													*/
/* 创建日期	: 2017/02/17																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static BOOL ComProtocolZDE_SetMessageBodyPara(AppSaveFrameStruct* dataFrame,
								   ComProtocolZDEParaStruct* protocolParaReg,
								   INSTRU_CONFIG_INF_TAB* config_tab,
								   INSTRU_FIX_INF_TAB* fix_tab)
{
	/* 计数器 */
	uint8_t num;
	/* 探头信息寄存器索引 */
	uint8_t cProbeIndex = 0;
	/* 存储数据索引 */
	uint8_t cSaveDataIndex = 0;
	/* 通道配置 */
	INSTRU_SENSOR_CHANNEL* cSensorchP = &config_tab->Inf_State.Instru_Sensor.Inf_State.CH1;
	/* 通道数据 */
	INSTRU_SENSOR_CHANNEL_FIX* cSensorChDataP = &fix_tab->Inf_State.Instru_Sensor.Inf.CH1;
	
	//printf("ComProtocolZDE_SetMessageBodyPara\r\n");
	
	if((dataFrame->Timestamp.Year<=0)\
		|| (dataFrame->Timestamp.Month<=0)\
		|| (dataFrame->Timestamp.Day<=0))
	{
		return FALSE;
	}
	
	
	/* 探头信息 */
	/* 设置采集时间  */
	protocolParaReg->MessageBody.Frame.CollectionTime[0] = (dataFrame->Timestamp.Year - 2000u)/10*16+(dataFrame->Timestamp.Year - 2000u)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[1] = (dataFrame->Timestamp.Month)/10*16+(dataFrame->Timestamp.Month)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[2] = (dataFrame->Timestamp.Day)/10*16+(dataFrame->Timestamp.Day)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[3] = (dataFrame->Timestamp.Hour)/10*16+(dataFrame->Timestamp.Hour)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[4] = (dataFrame->Timestamp.Min)/10*16+(dataFrame->Timestamp.Min)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[5] = (dataFrame->Timestamp.Sec)/10*16+(dataFrame->Timestamp.Sec)%10;
	
	/* 网关电源状态 */
	PowerLinkState = 
	config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
	
	/* 网关电量状态 */
	PowerLevelState = 
	config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	
	/* 判断当前设备是否搭载定位设备 */
	if(0x01 == GPS_Config_Enable)
	{
		/* 搭载定位设备，设置GPS参数 */
		for(num = 0;num < Sensor_CH_Config_Num; num++)
		{
			/* 查找GPS类型通道 */
			switch(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)
			{
				/* 经度通道 */
				case Sensor_GPS_POS_Lon:
				{
					/* 设置经度参数 */
					protocolParaReg->MessageBody.Frame.Lon = dataFrame->Sensor[num];
					break;
				}
				
				/* 纬度通道 */
				case Sensor_GPS_POS_Lat:
				{
					/* 设置纬度参数 */
					protocolParaReg->MessageBody.Frame.Lat = dataFrame->Sensor[num];
					break;
				}
				
				/* 纬度通道 */
				case Sensor_GPS_POS_Spd:
				{
					/* 设置纬度参数 */
					protocolParaReg->MessageBody.Frame.Speed = dataFrame->Sensor[num];
					break;
				}
				
				default:
				{
					/* 非GPS类型通道 */
					__nop();
				}
			}
		}
	}else
	{
		/* 未搭载定位设备，设置GPS参数 */
		protocolParaReg->MessageBody.Frame.Lon 		= GPS_Data_Rece_Timeout_Value;
		protocolParaReg->MessageBody.Frame.Lat 		= GPS_Data_Rece_Timeout_Value;
		protocolParaReg->MessageBody.Frame.Speed 	= GPS_Data_Rece_Timeout_Value;
	}
	
	/* 判断集成网关探头数量 */
	if(0 < IntProbeNum)
	{
		/* 集成的通道数量 */
		uint8_t cChannelnum = 0;
		
		
		/* 设置探头信息 */
		/* 设置SN号 *///这里只用使用SN方式，假如使用SIM卡或手机号方式需要修改代码！
		memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
			   protocolParaReg->MessageHeader.SN,
				sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
		
		/* 探头类型 */
		protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x03;
		
		/* 电池电量 */
		protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level = 
		config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		
		for(num = 0;num < Sensor_CH_Config_Num; num++)
		{
			switch(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)
			{
				/* 温度通道 */
				case Sensor_Tmp:
				{
					/* 设置参数 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[num] = dataFrame->Sensor[num];
					/* 设置通道数量 */
					cChannelnum++;
					break;
				}
				
				/* 湿度通道 */
				case Sensor_Hum:
				{
					/* 设置参数 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[num] = dataFrame->Sensor[num];
					/* 设置通道数量 */
					cChannelnum++;
					break;
				}
				
				/* 门磁开关通道 */
				case Sensor_DoorContact:
				{
					/* 设置参数 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[num] = dataFrame->Sensor[num];
					/* 设置通道数量 */
					cChannelnum++;
					break;
				}
				
				/* PT1000温度通道 */
				case Sensor_PT1000:
				{
					/* 设置参数 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[num] = dataFrame->Sensor[num];
					/* 设置通道数量 */
					cChannelnum++;
					break;
				}
				
				default:
				{
					/* 非集成在网关上的传感器类型通道 */
					__nop();
					break;
				}
			}
		}
		
		/* 设置通道数量 */
		protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = cChannelnum;
		
		/* 设置探头索引 */
		cProbeIndex++;
	}
	
	/* 判断无线探头数量 */
	if(0 < WirelessProbeNum)
	{
		/* 无线探头 */
		
		
		for(num = 0;num < Sensor_CH_Config_Num; num++)
		{
			/* 判断当前通道SN号是否配置过 */
			if((0x00 == ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO[0])&&
			   (0x00 == ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO[1])&&
			   (0x00 == ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO[2])&&
			   (0x00 == ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO[3]))
			{
				/* 为配置过的通道，上传送该通信息 */
				continue;
			}
			
			/* 当前通道SN号已经配置过，判断通道类型 */
			switch(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)
			{
				/* 华奥通单温无线卡片 */
				case Sensor_RF_Card_Single_Tmp:
				{
					/* 设置SN号 */
					memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
						   ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO,
						   sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
					
					/* 探头类型 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x01;
					
					/* 电池电量 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level = 
					((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorChDataP+num))->Sensor_Power_State;
					
					/* 设置参数,判断数据是否为无线号数据 */
					if(Wireless_RF_Card_Data_Rece_Timeout_Para == dataFrame->Sensor[cSaveDataIndex])
					{
						/* 无信号数据 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = 
						Wireless_RF_Card_Data_Rece_Timeout_Value;
					}else
					{
						/* 正常数据 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = dataFrame->Sensor[cSaveDataIndex];
					}
					/* 设置通道数量 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 1;
					/* 设置探头索引 */
					cProbeIndex++;
					break;
				}
				
				/* 华奥通温湿卡片温度通道无线卡片 */
				case Sensor_RF_Card_Humiture_Tmp:
				{
					/* 设置SN号 */
					memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
						   ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO,
						   sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
					
					/* 探头类型 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x02;
					
					/* 电池电量 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level =
					((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorChDataP+num))->Sensor_Power_State;
					
					/* 设置参数,判断数据是否为无线号数据 */
					if(Wireless_RF_Card_Data_Rece_Timeout_Para == dataFrame->Sensor[cSaveDataIndex])
					{
						/* 无信号数据 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = 
						Wireless_RF_Card_Data_Rece_Timeout_Value;
					}else
					{
						/* 正常数据 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = dataFrame->Sensor[cSaveDataIndex];
					}
					
					/* 设置湿度通道为无线号数据 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
					Wireless_RF_Card_Data_Rece_Timeout_Value;

					/* 设置通道数量 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 1;
					/* 设置探头索引 */
					cProbeIndex++;
					
					break;
				}
				
				/* 华奥通温湿卡片湿度通道无线卡片 */
				case Sensor_RF_Card_Humiture_Hum:
				{
					/* 匹配的通道 */
					uint8_t cMatchingCH;
					
					
					/* 查找与当前SN号一致的通道 */
					cMatchingCH = ComProtocolZDE_SearchMatchingSN(&protocolParaReg->MessageBody.Frame,
					((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO);
					
					/* 判断收索结果 */
					if(ComProtocolZDE_MatchingCHNull != cMatchingCH)
					{
						/* 有匹配的通道 */
						/* 设置参数,判断数据是否为无线号数据 */
						if(Wireless_RF_Card_Data_Rece_Timeout_Para == dataFrame->Sensor[cSaveDataIndex])
						{
							/* 无信号数据 */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
							Wireless_RF_Card_Data_Rece_Timeout_Value;
						}else
						{
							/* 正常数据 */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = dataFrame->Sensor[cSaveDataIndex];
						}
						/* 设置通道数量 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 2;
					}
					break;
				}
				
				/* 南京盛亿温湿卡片温度通道无线卡片 */
				case Sensor_GW_NJSY_Tmp:
				{
					/* 设置SN号 */
					memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
						   ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO,
						   sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
					
					/* 探头类型 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x02;
					
					/* 电池电量 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level = 
					((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorChDataP+num))->Sensor_Power_State;
					
					/* 设置参数,判断数据是否为无线号数据 */
					if(Gateway_NJSY_TimeoutPara == dataFrame->Sensor[cSaveDataIndex])
					{
						/* 无信号数据 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = 
						Gateway_NJSY_TimeoutPara;
					}else
					{
						/* 正常数据 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = dataFrame->Sensor[cSaveDataIndex];
					}
					
					/* 设置湿度通道为无线号数据 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
					Gateway_NJSY_TimeoutPara;

					/* 设置通道数量 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 1;
					/* 设置探头索引 */
					cProbeIndex++;
					break;
				}
				
				/* 南京盛亿温湿卡片湿度通道无线卡片 */
				case Sensor_GW_NJSY_Hum:
				{
					/* 匹配的通道 */
					uint8_t cMatchingCH;
					
					
					/* 查找与当前SN号一致的通道 */
					cMatchingCH = ComProtocolZDE_SearchMatchingSN(&protocolParaReg->MessageBody.Frame,
					((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO);
					
					/* 判断收索结果 */
					if(ComProtocolZDE_MatchingCHNull != cMatchingCH)
					{
						/* 有匹配的通道 */
						/* 设置参数,判断数据是否为无线号数据 */
						if(Gateway_NJSY_TimeoutPara == dataFrame->Sensor[cSaveDataIndex])
						{
							/* 无信号数据 */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
							Gateway_NJSY_TimeoutPara;
						}else
						{
							/* 正常数据 */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = dataFrame->Sensor[cSaveDataIndex];
						}
						/* 设置通道数量 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 2;
					}

					break;
				}
				
				/* 杭州创辉温湿卡片温度通道无线卡片 */
				case Sensor_GW_HZZH_Tmp:
				{
					/* 设置SN号 */
					memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
						   ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO,
						   sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
					
					/* 探头类型 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x02;
					
					/* 电池电量 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level = 
					((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorChDataP+num))->Sensor_Power_State;
					
					/* 设置参数,判断数据是否为无线号数据 */
					if(Gateway_HZZH_TimeoutPara == dataFrame->Sensor[cSaveDataIndex])
					{
						/* 无信号数据 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = 
						Gateway_HZZH_TimeoutPara;
					}else
					{
						/* 正常数据 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = dataFrame->Sensor[cSaveDataIndex];
					}
					
					/* 设置湿度通道为无线号数据 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
					Gateway_HZZH_TimeoutPara;

					/* 设置通道数量 */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 1;
					/* 设置探头索引 */
					cProbeIndex++;
					break;
				}
				
				/* 杭州创辉温湿卡片湿度通道无线卡片 */
				case Sensor_GW_HZZH_Hum:
				{
					/* 匹配的通道 */
					uint8_t cMatchingCH;
					
					
					/* 查找与当前SN号一致的通道 */
					cMatchingCH = ComProtocolZDE_SearchMatchingSN(&protocolParaReg->MessageBody.Frame,
					((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO);
					
					/* 判断收索结果 */
					if(ComProtocolZDE_MatchingCHNull != cMatchingCH)
					{
						/* 有匹配的通道 */
						/* 设置参数,判断数据是否为无线号数据 */
						if(Gateway_HZZH_TimeoutPara == dataFrame->Sensor[cSaveDataIndex])
						{
							/* 无信号数据 */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
							Gateway_HZZH_TimeoutPara;
						}else
						{
							/* 正常数据 */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = dataFrame->Sensor[cSaveDataIndex];
						}
						/* 设置通道数量 */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 2;
					}
					break;
				}
				
				default:
				{
					/* 非GPS类型通道 */
					__nop();
				}
			}
		}
	}
	
	/* 判断探头数量 */
	if((0 == cProbeIndex)&&(0x00 == GPS_Config_Enable))
	{
		/* 没有探头信息需要发送 */
			return FALSE;
	}
	
	/* 设置探头数量 */
	protocolParaReg->MessageBody.Frame.ProbeNum = cProbeIndex;
	
	return TRUE;
}

/************************************************************************************************/
/* 函数名	: ComProtocolZDE_FrameData										 		 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 根据协议参数，组成一个完整的数据帧。					 								*/
/* 创建日期	: 2017/02/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void ComProtocolZDE_FrameData(ComProtocolZDEParaStruct* protocolCR,ComProtocolZDE_MBF* protocolFrame)
{
	/* 计数器 */
	uint8_t cNum;
	/* 探头SN号暂存寄存器 */
	uint8_t SN_f[4];

	/* 采集时间 */
	protocolFrame->CollectionTime[0] = protocolCR->MessageBody.Frame.CollectionTime[0];
	protocolFrame->CollectionTime[1] = protocolCR->MessageBody.Frame.CollectionTime[1];
	protocolFrame->CollectionTime[2] = protocolCR->MessageBody.Frame.CollectionTime[2];
	protocolFrame->CollectionTime[3] = protocolCR->MessageBody.Frame.CollectionTime[3];
	protocolFrame->CollectionTime[4] = protocolCR->MessageBody.Frame.CollectionTime[4];
	protocolFrame->CollectionTime[5] = protocolCR->MessageBody.Frame.CollectionTime[5];
	
	/* 探头数量 */	
	protocolFrame->ProbeNum =  protocolCR->MessageBody.Frame.ProbeNum;
	
	/* 设置信号强度 */
	protocolFrame->GSMSignal = GSMGetSignalStrength();
	
	protocolFrame->PowerLinkState =(uint8_t)PowerLinkState;
	protocolFrame->PowerLevelState=PowerLevelState;
	/* 保留1 */
	protocolFrame->Reserved1[0] =  0x00;
	protocolFrame->Reserved1[1] =  0x00;
	/* 经度 */
	protocolFrame->Lon = protocolCR->MessageBody.Frame.Lon;

	/* 纬度 */
	protocolFrame->Lat = protocolCR->MessageBody.Frame.Lat;

	/* 速度 */
	protocolFrame->Speed = protocolCR->MessageBody.Frame.Speed;
	
	/* 保留2 */	
	protocolFrame->Reserved2[0] =  0x00;
	protocolFrame->Reserved2[1] =  0x00;
	protocolFrame->Reserved2[2] =  0x00;
	protocolFrame->Reserved2[3] =  0x00;
	
	//printf("protocolFrame->ProbeNum=%d\r\n",protocolFrame->ProbeNum);
	/* 探头信息 */
	for(cNum = 0;cNum < protocolFrame->ProbeNum; cNum++)
	{
		ComProtocolZDE_MBFI* cMBFIP = &protocolFrame->ProbeInfo+cNum;
		/* 设置SN号 */
		
		SN_f[0]=protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN[0];
		SN_f[1]=protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN[1];
		SN_f[2]=protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN[2];
		SN_f[3]=protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN[3];
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN[0]=SN_f[3];
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN[1]=SN_f[2];
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN[2]=SN_f[1];
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN[3]=SN_f[0];

		memcpy(cMBFIP->SN,
			protocolCR->MessageBody.Frame.ProbeInfo[cNum].SN,
			sizeof(cMBFIP->SN));
		
		/* 探头电量 */
		cMBFIP->Level = 
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].Level;
		
		/* 探头类型 */
		cMBFIP->Type = 
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].Type;
		
		/* 通道数量 */
		cMBFIP->ChannelNum = 
		//printf("ChannelNum=%d\r\n",protocolCR->MessageBody.Frame.ProbeInfo[cNum].ChannelNum);
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].ChannelNum;
		/* 保留 */	
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].Reserved =  0x00;
		/* 数据 */
		memcpy((float*)(&cMBFIP->Data),
			protocolCR->MessageBody.Frame.ProbeInfo[cNum].Data,
			sizeof(float)*cMBFIP->ChannelNum);

	}
}

#endif


#if (1) //功能函数接口
/************************************************************************************************/
/* 函数名	: ComProtZDE_CreateSendData										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 解析平台应答是否成功																*/
/* 作用	 	: 根据“泽大通用协议”通信协议，解析接收到的数据。					 					*/
/* 创建日期	: 2017/02/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
BOOL ComProtZDE_ParseData(uint8_t* destBuf,int* dataLen)
{
	/* 校验码 */
	uint8_t cCheckCode;
	/* 计数器 */
	uint8_t num;
	/* 服务器返回的数据长度 */
	uint16_t cDataLen;
	/* 应答数据结构 */
	ComProtocolZDEACKStruct* cACKDataP;
	
	
	//1、逆向转义数据
	/* 先对 接收数据 进行逆向转义 */
	cDataLen = GPRS_Network_To_Host_Translator_Data(ComProtocolZDEApp.DR.RXDR,
													destBuf,*dataLen);
	
	//2、解析数据
	/* 判断标识位数据 */
	if((ComProtocolZDEApp.DR.RXDR[0] != (u8)ComProtocolZDE_StartIdentify)||
	   (ComProtocolZDEApp.DR.RXDR[cDataLen-1] != (u8)ComProtocolZDE_EndIdentify))
	{
		/* 头标识或尾标识与协议不一致 */
		return FALSE;
	}
	
	/* 设置数据指针 */
	cACKDataP = (ComProtocolZDEACKStruct*)ComProtocolZDEApp.DR.RXDR;
	
	//printf("cACKDataP->ID1=%04X\r\n",cACKDataP->ID1);
	//printf("cACKDataP->ID2=%04X\r\n",cACKDataP->ID2);
	/* 判断消息头 消息ID数据 */
	//if(cACKDataP->ID != ComProtocolZDE_ACKMessageID)
	if((((u16)cACKDataP->ID2<<8)| (u16)cACKDataP->ID1)!= (u16)ComProtocolZDE_ACKMessageID)
	{
		/* 消息头消息ID与协议不一致 */		
		return FALSE;
	}

	
	/* 判断消息应答结果 */
	if(0x00 != cACKDataP->Result)
	{
		/* 消息应答异常 */
		return FALSE;
	}
	
	/* 清除 校验 数据寄存器 */
	cCheckCode = 0;
	/* 判断数据校验 */
	/* 计算 逆向转义后的网络序数据的校验  */
	for(num = 1;num < (cDataLen-2); num++)
	{
		/* 这里的校验方式:对有效的数据段取异或 */
		cCheckCode ^= ComProtocolZDEApp.DR.RXDR[num];
	}
	
	if(cACKDataP->CheckCode != cCheckCode)
	{
		/* 消息数据校验异常 */
		return FALSE;
	}
	
	/* 平台接收数据成功 */
	return TRUE;
}

/************************************************************************************************/
/* 函数名	: ComProtocolZDE_CreateData										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 根据“泽大通用协议”通信协议，创建数据。					 							*/
/* 创建日期	: 2017/02/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
BOOL ComProtocolZDE_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,
							   uint8_t* destBuf,int* dataLen)
{
	/* 协议参数寄存器 */
	ComProtocolZDEStruct* cProtocolReg = (ComProtocolZDEStruct*)ComProtocolZDEApp.DR.TXDR;
	/* 应用存储数据 */
	AppSaveFrameStruct cSaveFrameReg;
	/* 存储数据状态 */
	Save_ReadState SaveDataState;
	/* 通信协议参数寄存器 */
	ComProtocolZDEParaStruct cProtocolParaReg = {0};
	/* 数据索引 */
	ComProtocolZDE_MBF* cDataIndex = (ComProtocolZDE_MBF*)(&cProtocolReg->MessageBody.FrameData);
	/* 包数据长度 */
	uint16_t cPacketLen = 0;
	/* 计数器 */
	uint16_t cNum;
	
	
	/* 读当当前存储数据状态 */
	SaveDataState = AppSaveAPI.ReadSaveDataFrame(&cSaveFrameReg);

	/* 判断读读存储数据状态 */
	if(Save_ReadFail == SaveDataState)
	{
		/* 没有存储数据 */
		return FALSE;
	}
	
	/* 清除发送数据寄存器 */
	memset(ComProtocolZDEApp.DR.TXDR,0x00,sizeof(ComProtocolZDEApp.DR.TXDR));
	
	/* 设置消息头参数 */
	ComProtocolZDE_SetMessageHeaderPara(config_tab,cProtocolReg);
	
	/* 配置上传数据 */
	for(cProtocolParaReg.MessageBody.FrameNum = 0;
	   ((((uint8_t*)cDataIndex)+ComProtocolZDE_FrameDataLen) < 
	   &ComProtocolZDEApp.DR.TXDR[ComProtocolZDE_TXSize-1]);
	   )
	{
		/* 有数据需要上传,读取存储数据。 */
		/* 设置协议参数状态 */
		BOOL cSetProtocolState;
		/* 通道数 */
		uint8_t cChannelNum = 0;
		
		
		/* 根据存储数据信息，设置通信协议消息体参数 */
		cSetProtocolState = ComProtocolZDE_SetMessageBodyPara(&cSaveFrameReg,&cProtocolParaReg,config_tab,fix_tab);
		
		/* 判断参数设置状态 */
		if(FALSE == cSetProtocolState)
		{
			/* 没有信息需要上传 */
			return FALSE;
		}
		
		/* 设置帧数据 */
		cProtocolReg->MessageBody.FrameNum++;
		
		/* 根据通信协议参数，组装发送数据帧 */
		ComProtocolZDE_FrameData(&cProtocolParaReg,cDataIndex);
		
		/* 计算通道数，主要是为了计算每个探头多出的地址数 */
		for(cNum = 0,cChannelNum = 0;cNum < cProtocolParaReg.MessageBody.Frame.ProbeNum; cNum++)
		{
			cChannelNum += cProtocolParaReg.MessageBody.Frame.ProbeInfo[cNum].ChannelNum-1;
		}
		
		/* 计算数据索引位置 */
		cDataIndex = (ComProtocolZDE_MBF*)
		(((uint8_t*)(&cDataIndex->ProbeInfo+cProtocolParaReg.MessageBody.Frame.ProbeNum))+
		(cChannelNum)*sizeof(float));
		
		/* 读当当前存储数据状态 */
		SaveDataState = AppSaveAPI.ReadSaveDataFrame(&cSaveFrameReg);
		
		/* 判断读存储数据状态 */
		if(Save_ReadFail == SaveDataState)
		{
			/* 无存储，跳出参数设置 */
			break;
		}
	}
	
	/* 计算数据序号 */
	cPacketLen = ((uint8_t*)cDataIndex) - 
	ComProtocolZDEApp.DR.TXDR-1;
	
	ComProtocolZDEApp.DR.TXDR[2] = (cPacketLen+1-20)%255;
	ComProtocolZDEApp.DR.TXDR[3] = (cPacketLen+1-20)/255;
	
	
	//printf("cPacketLen=%d\r\n",cPacketLen);
	/* 计算校验码 */
	for(cNum = 0,ComProtocolZDEApp.DR.TXDR[cPacketLen+1] = 0;cNum < cPacketLen+1; cNum++)
	{
		/* 计算校验值 */
		ComProtocolZDEApp.DR.TXDR[cPacketLen+1] ^= ComProtocolZDEApp.DR.TXDR[cNum];
	}
	
	/* 转义数据帧 */
	*dataLen = 
	GPRS_Host_To_Network_Translator_Data((destBuf+1),
	ComProtocolZDEApp.DR.TXDR,
	cPacketLen+2);
	
	/* 设置结束标识符 */
	*destBuf = ComProtocolZDE_StartIdentify;
	*(destBuf+*dataLen+1) = ComProtocolZDE_EndIdentify;
	*dataLen += 2;
	
	return TRUE;
}

#endif


#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

