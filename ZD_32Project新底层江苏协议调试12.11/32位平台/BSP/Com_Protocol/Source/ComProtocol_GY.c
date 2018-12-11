/************************************************************************************************/
/** @file              : ComProtocol_GY.c														*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年12月21日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--变送器--通信协议--国药 应用文件								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : ComProtocol_GY.c																	*/
/**作用       : 根据国药提供的通信协议格式将探头数据上传到云平台									*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年06月03日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "ComProtocol_GY.h"
#if (AppGYForwardEnable == 0x01)
#include "ComProtocol_SetPlateNo.h"

#include "Log_Time.h"
#include "BSP.h"
#include <stdio.h>
#include <string.h>

/*******************************************************************************/
/* 文档探头数量限制 */
#define	ComProtocol_MaxProbeNum			(13u)

/* 设备探头总数量 */
#define	ComProtocol_ProbeTotalNum		(Sensor_Tmp_Num+\
										 Sensor_Hum_Num+\
										 Sensor_RF_Single_Tmp_Num+\
										 Sensor_RF_Humiture_Num+\
										 Sensor_PT1000_Num+\
										 Sensor_GW_NJSY_Num)

/* 判断探头数量是否大于协议设定最大探头数量 */
#if (ComProtocol_MaxProbeNum < ComProtocol_ProbeTotalNum)
#error "探头数量大于协议设定最大探头数量13个"
#endif

/********************************* 宏定义 *********************************/
/* 通信协议 发送的最大数据长度 */
#define ComProtocol_MaxDataLen				(1024ul)


/* 通信协议 探头数据命令标识 */
#define ComProtocol_ProbeCmd				("~UPTMP")

/* 通信协议 GPS数据命令标识 */
#define ComProtocol_GPSCmd					("~UPGPS")

/* 通信协议 探头数据ID */
#define ComProtocol_ProbeDataID				("ProbeID")

/* 通信协议 GPS数据ID */
#define ComProtocol_GPSDataID				("GPSID")


/* 无传感器数据参数(探头中没有传感器数据时的参数) */
#define ComProtocol_NullSensor				(-1000.0)





/*******************************************************************************/
/********************************* 数据结构 *************************************/
/* 国药协议 数据结构 */
typedef struct
{
	/* 车牌号 */
	uint8_t PlateNo[27];
	/* 采集时间 */
	Times CollectTime;
	/* 探头数量 */
	uint8_t ProbeNum;
	
	/* 探头寄存器 */
	struct
	{
		/* 探头号 */
		uint8_t No[21];
		/* 探头类型 */
		uint8_t	Type;
		/* 电量 */
		float Level;
		/* 温度 */
		float Temp;
		/* 湿度 */
		float Hum;
		/* 探头数据ID */
		uint8_t ID[8];
	}Probe[Sensor_CH_Config_Num];
	
	/* GPS寄存器 */
	struct
	{
		/* 工作状态 */
		BOOL WorkState;
		/* 运单号 */
		uint8_t TransportNo[24];
		/* 速度 */
		float Speed;
		/* 经度 */
		float Lon;
		/* 纬度 */
		float Lat;
		/* 方向 */
		float Cog;
	}GPS;
	
}AppComProtocolStruct;


/********************************* 数据寄存器 *******************************/
/* 国药通信协议应用数据 */
AppComProtocolGYStruct AppComProtocolGY = 
{
	/* 配置寄存器 */

	/* 国药网络接口 */
	/* 域名 */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,	0X00,
	/* IP */
	'2' ,'2' ,'1' ,'.' , 	'1' ,'3' ,'3' ,'.' ,	'2' ,'3' ,'7' ,'.' ,
	'2' ,'3','3',0X00,	0X00,0X00,0X00,0X00,	0X00,
	/* 端口 */
	'8' ,'0' ,'8' ,'9' , 	0X00,0X00,0X00,0X00,	0X00,
		
	
	/* 状态寄存器 */
	/* 创建数据标志 */
	FALSE,

	
	/* 数据寄存器 */
	0X00,
};


/********************************* 文件函数声明 *********************************/
/* 解析国药应答数据 */
//static void ComProtGY_ParseData(void);
/* 创建数据帧 */
//static void ComProtGY_FrameData(AppComProtocolStruct* protocolPara);

/*******************************************************************************/
/********************************* 文件函数声明 *********************************/


/************************************************************************************************/
#if (1) //文件函数
/************************************************************************************************/
/* 函数名	: ComProtGY_ScanMatchHumChannel										 	 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 设置车牌号码					 													*/
/* 创建日期	: 2016/12/23																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static uint8_t ComProtGY_ScanMatchHumChannel(uint32_t channelSN,INSTRU_SENSOR_CHANNEL* cSensorchP)
{
	/* 计数器 */
	uint8_t num;
	

	/* 查询 */
	for(num = 0;num < 32; num++)
	{
		
		/* 判断当前通道是否为湿度通道 */
		if((Sensor_Hum == (((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)) ||
		   (Sensor_RF_Card_Humiture_Hum == (((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)) ||
		   (Sensor_GW_NJSY_Hum == (((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)))
		{
			/* 当前通道为湿度通道 */
			/* 判断ID号是否相同 */
			
			if(((*((uint32_t*)(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO))) != 0x00000000) &&
			   ((*((uint32_t*)(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO))) == channelSN))
			{
				/* 目标的ID号相同 */
				return num;
			}
		}
	}
	
	return (0xFF);
}

/************************************************************************************************/
/* 函数名	: ComProtGY_SetProtocolPara										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 设置协议参数					 													*/
/* 创建日期	: 2016/12/22																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static BOOL ComProtGY_SetProtocolPara(AppSaveFrameStruct* dataFrame,
									  AppComProtocolStruct* protocolPara)
{
	/* 计数器 */ 
	uint8_t num;
	/* 传感器通道编号 */
	uint8_t channelNo;
	/* 湿度通道编号 */
	uint8_t humchannelNo;
	/* 车牌号指针 */
	char* cPlateNo = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO;
	/* 传感器通道指针 */
	INSTRU_SENSOR_CHANNEL* cSensorChp = &Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1;
	/* 传感器电量数据指针 */
	INSTRU_SENSOR_CHANNEL_FIX* cSensorLevelp = &Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1;
	
	
	/* 设置车牌号 */
	ComProtocol_SetPlateNo(protocolPara->PlateNo,cPlateNo);
	/* 设置采集时间 */
	protocolPara->CollectTime = dataFrame->Timestamp;
	/* 获取当前探头数量 */
	protocolPara->ProbeNum = Sensor_Tmp_Num + 
							 Sensor_Hum_Num + 
							 Sensor_Hum_Num + 
							 Sensor_DoorContact_Num +
							 Sensor_RF_Single_Tmp_Num +
							 Sensor_RF_Humiture_Num +
							 Sensor_Lon_Num +
							 Sensor_Lat_Num +
							 Sensor_Spd_Num +
							 Sensor_PT1000_Num +
							 Sensor_GW_NJSY_Num;
	
#if (GPS_Config_Enable == 0x01)
	/* 设置GPS工作状态 */
	protocolPara->GPS.WorkState = TRUE;
	/* 设置运单号 */
	sprintf((char*)protocolPara->GPS.TransportNo,"");
#else
	/* 设置GPS工作状态 */
	protocolPara->GPS.WorkState = FALSE;
#endif
	
	/* 按顺序来设置探头数据 */
	for(num = 0,channelNo = 0;(channelNo < protocolPara->ProbeNum)&&(channelNo < Sensor_CH_Config_Num);num++)
	{
		if((Sensor_Tmp 					== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
				Sensor_DoorContact 					== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_PT1000 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_RF_Card_Single_Tmp 	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_RF_Card_Humiture_Tmp == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_GW_NJSY_Tmp 		  	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type))
		{
			/* 判断探头SN号 */
			if((((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* SN号未配置过，不发送当前通道数据. */
				/* 减掉一个通道. */
				protocolPara->ProbeNum--;
				/* 继续 */
				continue;
			}
			
			/* 设置探头编号 */
			sprintf((char*)protocolPara->Probe[channelNo].No,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			
			/* 设置探头ID */
			sprintf((char*)protocolPara->Probe[channelNo].ID,ComProtocol_ProbeDataID);
			
			/* 设置探头类型 */
			protocolPara->Probe[channelNo].Type = ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type;
			
			/* 设置温度数据 */
			protocolPara->Probe[channelNo].Temp = dataFrame->Sensor[num];
			
			/* 设置电量 */
			protocolPara->Probe[channelNo].Level = ((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorLevelp+num))->Sensor_Power_State;
			
			/* 查找对应的温湿度卡片的湿度通道 */
			humchannelNo = ComProtGY_ScanMatchHumChannel(
			*((uint32_t*)(((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO)),
			&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1);
			
			/* 判断收索结果 */
			if(0xFF != humchannelNo)
			{
				/* 设置温度数据 */
				protocolPara->Probe[channelNo].Hum = dataFrame->Sensor[humchannelNo];		
			}else
			{
				/* 设置湿度数据 */
				protocolPara->Probe[channelNo].Hum = ComProtocol_NullSensor;
			}
			
			/* 判断是否接收到温度数据 */
			if(Wireless_RF_Card_Data_Rece_Timeout_Para == protocolPara->Probe[channelNo].Temp)
			{
				/* 未收到无线卡片数据，设置温度数据为空。 */
				protocolPara->Probe[channelNo].Temp = ComProtocol_NullSensor;
			}
			/* 判断是否接收到湿度数据 */
			if(Wireless_RF_Card_Data_Rece_Timeout_Para == protocolPara->Probe[channelNo].Hum)
			{
				/* 未收到无线卡片数据，设置湿度数据为空。 */
				protocolPara->Probe[channelNo].Hum = ComProtocol_NullSensor;
			}
			
		}else if(Sensor_GPS_POS_Lon == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* 设置经度数据 */
			protocolPara->GPS.Lon = dataFrame->Sensor[num];
			
			/* 判断是否接收到经度数据 */
			if(GPS_Data_Rece_Timeout_Value == protocolPara->GPS.Lon)
			{
				/* 未收到GPS经度数据，设置经度数据为空。 */
				protocolPara->GPS.Lon = ComProtocol_NullSensor;
			}
			
			/* 减掉一个通道. */
			protocolPara->ProbeNum--;
			continue;
		}else if(Sensor_GPS_POS_Lat == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* 设置纬度数据 */
			protocolPara->GPS.Lat = dataFrame->Sensor[num];
			/* 判断是否接收到纬度数据 */
			if(GPS_Data_Rece_Timeout_Value == protocolPara->GPS.Lat)
			{
				/* 未收到GPS经度数据，设置纬度数据为空。 */
				protocolPara->GPS.Lat = ComProtocol_NullSensor;
			}
			
			/* 减掉一个通道. */
			protocolPara->ProbeNum--;
			continue;
		}else if(Sensor_GPS_POS_Spd == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* 设置速度数据 */
			protocolPara->GPS.Speed = dataFrame->Sensor[num];
			/* 判断是否接收到速度数据 */
			if(GPS_Data_Rece_Timeout_Value == protocolPara->GPS.Speed)
			{
				/* 未收到GPS速度数据，设置速度数据为空。 */
				protocolPara->GPS.Speed = ComProtocol_NullSensor;
			}
			
			/* 减掉一个通道. */
			protocolPara->ProbeNum--;
			continue;
		}else
		{
			/* 减掉一个通道. */
			protocolPara->ProbeNum--;
			continue;
		}
		/* 设置通道编号 */
		channelNo++;
	}
	
	/* 判断当前参数配置状态 */
	if((protocolPara->ProbeNum > 0)||(protocolPara->GPS.WorkState))
	{
		/* 操作成功 */
		return TRUE;
	}
	
	/* 操作成功 */
	return FALSE;
}

/************************************************************************************************/
/* 函数名	: ComProtGY_FrameData										 		 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 根据协议参数，组成一个完整的数据帧。					 								*/
/* 创建日期	: 2016/12/22																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void ComProtGY_FrameData(AppComProtocolStruct* protocolPara,uint8_t* cDataBuf)
{
	/* 计数器 */
	uint8_t num;
	/* 数据指针 */
	uint32_t ProbeDataIndex;
	/* 采集时间 */
	char cProbeCollectTime[24] = {0};
	/* 数据ID时间 */
	char cComProtocol_DataIDTime[24] = {0};
	/* 探头数据寄存器 */
	uint8_t cComProtocol_ProbeDataReg[1024] = {0};
	/* GPS数据寄存器 */
	uint8_t cComProtocol_GPSDataReg[128] = {0};
	/* 探头数据ID编号 */
	static uint8_t cProbeDataIDNum = 0;
	/* GPS数据ID编号 */
	static uint8_t cGPSDataIDNum = 0;
	
	
	/* 采集时间 */
	sprintf(cProbeCollectTime,"%04d-%02d-%02d %02d:%02d:%02d",
	protocolPara->CollectTime.Year,protocolPara->CollectTime.Month,
	protocolPara->CollectTime.Day,protocolPara->CollectTime.Hour,
	protocolPara->CollectTime.Min,protocolPara->CollectTime.Sec);
	
	/* 设置数据ID编号 */
	sprintf(cComProtocol_DataIDTime,"%04d%02d%02d%02d%02d%02d",
	protocolPara->CollectTime.Year,protocolPara->CollectTime.Month,
	protocolPara->CollectTime.Day,protocolPara->CollectTime.Hour,
	protocolPara->CollectTime.Min,protocolPara->CollectTime.Sec);
	
	/* 判断是否需要配置探头数据 */
	if(protocolPara->ProbeNum > 0)
	{
		/* 清除温度数据缓存 */
		memset(cComProtocol_ProbeDataReg,0x00,sizeof(cComProtocol_ProbeDataReg));
		
		/* 设置数据命令 */
		sprintf((char*)cComProtocol_ProbeDataReg,"%s",ComProtocol_ProbeCmd);
		
		/* 计算数据指针位置 */
		ProbeDataIndex = strlen((char*)cComProtocol_ProbeDataReg);
		
		/* 根据探头配置，创建数据。 */
		for(num = 0;num < protocolPara->ProbeNum ;num++)
		{
			/* 探头电量 */
			char cProbeLevel[12] = {0};
			/* 探头电量 */
			char cProbeTemp[12] = {'\0'};
			/* 探头电量 */
			char cProbeHum[12] = {'\0'};
			
			
			/* 设置电池电量 */
			float_to_string(protocolPara->Probe[num].Level,cProbeLevel);
			
			/* 判断温度数据是否为无数据状态 */
			if(ComProtocol_NullSensor != protocolPara->Probe[num].Temp)
			{
				/* 设置探头温度 */
				float_to_string(protocolPara->Probe[num].Temp,cProbeTemp);
			}
			
			/* 判断湿度数据是否为无数据状态 */
			if(ComProtocol_NullSensor != protocolPara->Probe[num].Hum)
			{
				/* 设置探头湿度 */
				float_to_string(protocolPara->Probe[num].Hum,cProbeHum);
			}
			
			/* 设置探头数据 */
			sprintf((char*)(&cComProtocol_ProbeDataReg[ProbeDataIndex]),
					"&%s|%s|%s|%s|%s|%s|%s%s%d",
					protocolPara->PlateNo,
					protocolPara->Probe[num].No,
					cProbeLevel,
					cProbeTemp,
					cProbeHum,
					cProbeCollectTime,
					ComProtocol_ProbeDataID,
					cComProtocol_DataIDTime,
					cProbeDataIDNum);
			
			/* 探头数据ID编号 */
			cProbeDataIDNum++;
			
			/* 计算数据指针位置 */
			ProbeDataIndex = strlen((char*)cComProtocol_ProbeDataReg);
		}
		
		/* 设置探头数据结束符 */
		sprintf((char*)(&cComProtocol_ProbeDataReg[ProbeDataIndex]),"%s","#");
		/* 设置探头数据长度指针 */
		ProbeDataIndex += 1;
	}
	
	
	/* 判断是否需要配置GPS数据 */
	if(protocolPara->GPS.WorkState == TRUE)
	{
		/* 需要配置GPS数据 */
		/* GPS数据指针 */
		uint32_t GPSDataIndex;
		/* GPS车速 */
		char cGPSSpeed[12] = {0};
		/* GPS经度 */
		char cGPSLon[12] = {0};
		/* GPS纬度 */
		char cGPSLat[12] = {0};
		/* GPS方向 */
		char cGPSCog[12] = {0};
		
		
		/* 清除GPS数据缓存 */
		memset(cComProtocol_GPSDataReg,0x00,sizeof(cComProtocol_GPSDataReg));
		
		/* 设置GPS数据命令 */
		sprintf((char*)cComProtocol_GPSDataReg,"%s",ComProtocol_GPSCmd);
		
		/* 计算数据指针位置 */
		GPSDataIndex = strlen((char*)cComProtocol_GPSDataReg);
		
		/* 判断速度数据是否为无数据状态 */
		if(ComProtocol_NullSensor != protocolPara->GPS.Speed)
		{
			/* 设置速度数据 */
			float_to_string(protocolPara->GPS.Speed,cGPSSpeed);
		}
		
		/* 判断经度数据是否为无数据状态 */
		if(ComProtocol_NullSensor != protocolPara->GPS.Lon)
		{
			/* 设置经度数据 */
			float_to_string(protocolPara->GPS.Lon,cGPSLon);
		}
		
		/* 判断纬度数据是否为无数据状态 */
		if(ComProtocol_NullSensor != protocolPara->GPS.Lat)
		{
			/* 设置纬度数据 */
			float_to_string(protocolPara->GPS.Lat,cGPSLat);
		}
		
		/* 判断纬度数据是否为无数据状态 */
		if(ComProtocol_NullSensor != protocolPara->GPS.Cog)
		{
			/* 设置纬度数据 */
			float_to_string(protocolPara->GPS.Cog,cGPSCog);
		}
		
		/* 设置GPS数据 */
		sprintf((char*)(&cComProtocol_GPSDataReg[GPSDataIndex]),"&%s|%s|%s|%s|%s|%s|%s|%s%s%d",
				protocolPara->PlateNo,
				protocolPara->GPS.TransportNo,
				cGPSSpeed,
				cGPSLon,
				cGPSLat,
				cGPSCog,
				cProbeCollectTime,
				ComProtocol_GPSDataID,
				cComProtocol_DataIDTime,
				cGPSDataIDNum);
		
		/* 探头数据ID编号 */
		cGPSDataIDNum++;
		
		/* 计算数据指针位置 */
		GPSDataIndex = strlen((char*)cComProtocol_GPSDataReg);
		
		/* 设置探头数据结束符 */
		sprintf((char*)(&cComProtocol_GPSDataReg[GPSDataIndex]),"%s","#");
		
		/* 设置探头数据长度指针 */
		ProbeDataIndex += 1;
			
	}
	
	/* 将设置好的数据拷贝到数据寄存器中 */
	sprintf((char*)cDataBuf,"%s%s",
			cComProtocol_ProbeDataReg,
			cComProtocol_GPSDataReg);
	
	
}

#endif

#if (1) //功能函数接口
/************************************************************************************************/
/* 函数名	: ComProtGY_CreateSendData										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 解析平台应答是否成功																				*/
/* 作用	 	: 根据“国药”通信协议，解析接收到的数据。					 							*/
/* 创建日期	: 2016/12/22																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
BOOL ComProtGY_ParseData(uint8_t* receBuf)
{
	/* 数据指针 */
	char * datap;
	
	
	/* 查询上传探头数据命令字符串 */
	datap = strstr((char*)receBuf,ComProtocol_ProbeCmd);
	/* 判断HTTP传输是否成功 */
	if(NULL == datap)
	{
		/* 查询上传GPS数据命令字符串 */
		datap = strstr((char*)receBuf,ComProtocol_GPSCmd);
		if(NULL == datap)
		{
			/* 传输失败 */
			return FALSE;
		}
	}
	
	/* 查询上传探头ID字符串 */
	datap = strstr((char*)receBuf,ComProtocol_ProbeDataID);
	/* 判断HTTP传输是否成功 */
	if(NULL == datap)
	{
		/* 查询上传GPS ID字符串 */
		datap = strstr((char*)receBuf,ComProtocol_GPSDataID);
		if(NULL == datap)
		{
			/* 传输失败 */
			return FALSE;
		}
	}
	
	/* 平台接收数据成功 */
	return TRUE;
}

/************************************************************************************************/
/* 函数名	: ComProtGY_CreateData										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 根据“国药”通信协议，创建数据。					 								*/
/* 创建日期	: 2016/12/22																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
BOOL ComProtGY_CreateData(Save_Sensor_Data_Struct* cSaveDatap)
{
	/* 设置参数状态 */
	BOOL SetProtState;
	/* 协议参数寄存器 */
	AppComProtocolStruct cProtocolParaReg = {0};
	/* 应用存储数据 */
	AppSaveFrameStruct cSaveFrameReg;
	
	
	/* 初始化协议参数寄存器 */
	cProtocolParaReg.GPS.Cog 	= 0;
	cProtocolParaReg.GPS.Lat 	= ComProtocol_NullSensor;
	cProtocolParaReg.GPS.Lon 	= ComProtocol_NullSensor;
	cProtocolParaReg.GPS.Speed 	= ComProtocol_NullSensor;
	
	/* 设置采集时间 */
	cSaveFrameReg.Timestamp.Year 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS + 2000;
	cSaveFrameReg.Timestamp.Month	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS;
	cSaveFrameReg.Timestamp.Day 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS;
	cSaveFrameReg.Timestamp.Hour 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS;
	cSaveFrameReg.Timestamp.Min 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS;
	cSaveFrameReg.Timestamp.Sec 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS;
	
	/* 复制传感器数据 */
	memcpy(cSaveFrameReg.Sensor,cSaveDatap->GPRS_IR.Sensor_Data,sizeof(cSaveFrameReg.Sensor));
	
	/* 根据存储数据信息，设置通信协议参数 */
	SetProtState = ComProtGY_SetProtocolPara(&cSaveFrameReg,&cProtocolParaReg);
		
	/* 判断设置参数状态 */
	if(FALSE == SetProtState)
	{
		/* 没有需要发送的参数，不发送数据 */
		return FALSE;
	}
	
	/* 清除发送数据寄存器 */
	memset(AppComProtocolGY.DR.TXDR,0x00,sizeof(AppComProtocolGY.DR.TXDR));
	
	/* 根据通信协议参数，组装发送数据帧 */
	ComProtGY_FrameData(&cProtocolParaReg,AppComProtocolGY.DR.TXDR);

	return TRUE;
}

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
