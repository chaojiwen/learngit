/************************************************************************************************/
/** @file              : ComProtocol_WZ.c														*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年12月21日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--变送器--通信协议--万智String协议 应用文件					*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : ComProtocol_WZ.c																	*/
/**作用       : 根据万智String协议提供的通信协议格式将探头数据上传到云平台									*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2017年02月13日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "ComProtocol_WZ.h"
#if (ComProtocol_WZ_Enable == 0x01)
#include "ComProtocol_SetPlateNo.h"

#include "Log_Time.h"
#include "BSP.h"
#include <stdio.h>
#include <string.h>

/*******************************************************************************/
/* 文档温度探头数量限制 */
#define	ComProtocol_TempProbeNumMax			(4u)
/* 文档湿度探头数量限制 */
#define	ComProtocol_HumProbeNumMax			(1u)

/* 设备探头总数量 */
#define	ComProtocol_ProbeTotalNum		(Sensor_Tmp_Num+\
										 Sensor_Hum_Num+\
										 Sensor_RF_Single_Tmp_Num+\
										 Sensor_RF_Humiture_Num+\
										 Sensor_PT1000_Num+\
										 Sensor_GW_NJSY_Num)

/* 判断探头数量是否大于协议设定最大探头数量 */
#if (ComProtocol_TempProbeNumMax < Sensor_Tmp_Num +\
								   Sensor_RF_Single_Tmp_Num +\
								   Sensor_PT1000_Num +\
							       Sensor_GW_NJSY_Tmp_Num)
#error "探头数量大于协议设定最大探头数量4个"
#endif

/* 判断探头数量是否大于协议设定最大探头数量 */
#if (ComProtocol_HumProbeNumMax < Sensor_Hum_Num +\
								  Sensor_RF_Humiture_Hum_Num +\
								  Sensor_GW_NJSY_Hum_Num +\
								  Sensor_GW_HZZH_Hum_Num)
#error "探头数量大于协议设定最大探头数量1个"
#endif

/********************************* 宏定义 *********************************/
/* 通信协议 发送的最大数据长度 */
#define ComProtocol_MaxDataLen				(1024ul)


/* 通信协议 厂商编码 */
#define ComProtocol_VendorCode				(3170)

/* 通信协议 命令字 */
#define ComProtocol_Cmd						("GPSDU")
/* 通信协议 要求应答标识 */
#define ComProtocol_ACKFlag					("res")



/* 通信协议 设备区分模式:1/车牌号、2/设备号或sim卡号(推荐) */
#define ComProtocol_PlateNoMode				(1u)
#define ComProtocol_SN_SimMode				(2u)

/* 无GPS定位数据参数 */
#define ComProtocol_NullGPSSignalPara		(0.0)

/* 无探头配置数据参数 */
#define ComProtocol_NullProbePara			(-9000)

/* 协议的探头类型(万智协议将探头分为2种类型:1温度，2湿度) */
#define ComProtocol_ProbeTempType			(1u)
#define ComProtocol_ProbeHumType			(2u)


/*******************************************************************************/
/********************************* 数据结构 *************************************/
/* 万智String协议协议 数据结构 */
typedef struct
{
	/* 实时数据模式 */
	uint8_t RTDMode;
	/* 车牌号 */
	uint8_t PlateNo[27];
	/* 采集时间 */
	Times CollectTime;
	/* 经度 */
	float Lon;
	/* 纬度 */
	float Lat;
	/* 速度 */
	float Speed;
	/* 方向 */
	float Cog;
	
	/* 状态寄存器 */
	struct
	{
		/* ACC连接状态 */
		BOOL PowerLink;
		/* GPS定位状态 */
		BOOL GPS;
		
		/* 探头数量 */
		uint8_t ProbeNum;
		/* 探头类型 */
		uint8_t ProbeType[5];
		
		/* 温度 */
		float Temp[4];
		/* 温度 报警状态 */
		uint8_t TempAlarm[4];
		
		/* 湿度 */
		float Hum;
		/* 湿度 报警状态 */
		uint8_t HumAlarm;
		
		/* 电池电量 */
		uint8_t BatteryValue;
		
//		/* 门磁 */
//		BOOL Door;
//		/* Gps卫星定位数 */
//		uint8_t gpsnum;
//		/* Gsm定位数 */
//		uint8_t gsmnum;
//		/* 终端总里程 */
//		uint32_t distance;
	}State;
	
	/* 报警寄存器 */
	struct
	{
		/* 终端主电源掉电 */
		uint32_t PowerDown:1;
		
	}Alarm;
	
}ComProtocolWZCRStruct;

/********************************* 数据寄存器 *******************************/
/* 万智String协议通信协议应用数据 */
ComProtocolWZDRStruct ComProtocolWZDR = 
{
	/* 配置寄存器 */

	/* 万智String协议网络接口 */
	/* 域名 */
	'g' ,'p' ,'s' ,'t' ,	'x' ,'h' ,'h' ,'.' ,	'h' ,'u' ,'o' ,'y' ,
	'u' ,'n' ,'r' ,'e' ,	'n' ,'.' ,'c' ,'o' ,	'm' ,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,	0X00,
	/* IP */
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,
	/* 端口 */
	'9' ,'0' ,'3' ,0X00, 	0X00,0X00,0X00,0X00,	0X00,
		
	
	/* 状态寄存器 */
	/* 创建数据标志 */
	FALSE,

	
	/* 数据寄存器 */
	0X00,
};


/********************************* 文件函数声明 *********************************/


/*******************************************************************************/
/********************************* 文件函数声明 *********************************/


/************************************************************************************************/
#if (1) //文件函数
/************************************************************************************************/
/* 函数名	: ComProtocolWZ_SetPara										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 设置协议参数					 													*/
/* 创建日期	: 2016/12/22																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static BOOL ComProtocolWZ_SetPara(AppSaveFrameStruct* dataFrame,
								  ComProtocolWZCRStruct* protocolReg)
{
	/* 计数器 */ 
	uint8_t num;
	/* 车牌号指针 */
	char* cPlateNo = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO;
	/* 传感器通道指针 */
	INSTRU_SENSOR_CHANNEL* cSensorChp = &Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1;
	/* 网关电池电量数据指针 */
	uint8_t* cBatteryLevelp = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	/* 电源连接状态 */
	BOOL* cPowerLinkState = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Link_State;
	
	printf("ComProtocolWZ_SetPara\r\n");
	
	/* 获取当前探头数量 */
	protocolReg->State.ProbeNum = Sensor_Tmp_Num+
								  Sensor_Hum_Num+
									Sensor_DoorContact+
								  Sensor_RF_Single_Tmp_Num+
								  Sensor_RF_Humiture_Tmp_Num+
								  Sensor_RF_Humiture_Hum_Num+
								  Sensor_PT1000_Num+
								  Sensor_GW_NJSY_Tmp_Num+
								  Sensor_GW_NJSY_Hum_Num+
								  Sensor_GW_HZZH_Tmp_Num+
								  Sensor_GW_HZZH_Hum_Num;
	
	protocolReg->RTDMode = ComProtocol_PlateNoMode;
	
	/* 设置实时数据模式:固定为车牌号 */
	ComProtocol_SetPlateNo(protocolReg->PlateNo,cPlateNo);
	
	/* 设置采集时间 */
	protocolReg->CollectTime = dataFrame->Timestamp;

	/* 设置状态寄存器 */
	/* ACC状态 */
	if(TRUE == (*cPowerLinkState))
	{
		/* 当前电源已连接 */
		protocolReg->State.PowerLink = TRUE;
	}else
	{
		/* 当前电源未连接 */
		protocolReg->State.PowerLink = FALSE;
	}
	
	/* 设置探头参数 */
	for(num = 0;num < Sensor_CH_Config_Num;num++)
	{
		/* 判断探头类型 */
		if((Sensor_Tmp 					== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_PT1000 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_RF_Card_Single_Tmp 	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_RF_Card_Humiture_Tmp == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_GW_NJSY_Tmp 		  	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type))
		{

			/* 温度探头 */
			/* 设置探头类型 */
			protocolReg->State.ProbeType[num] = ComProtocol_ProbeTempType;
			/* 设置数据 */
			protocolReg->State.Temp[num] = dataFrame->Sensor[num];
			/* 判断报警状态 */
			if(protocolReg->State.Temp[num] > (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_Up_Limit))
			{
				/* 高温报警 */
				protocolReg->State.TempAlarm[num] = 2;
				
			}else if(protocolReg->State.Temp[num] < (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_Low_Limit))
			{
				/* 低温报警 */
				protocolReg->State.TempAlarm[num] = 4;
			}else
			{
				/* 正常 */
				protocolReg->State.TempAlarm[num] = 0;
			}
		}else if((Sensor_Hum 					== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
				 (Sensor_RF_Card_Humiture_Hum	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
				 (Sensor_GW_NJSY_Hum	 		== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
				 (Sensor_GW_HZZH_Hum 			== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type))
		{
			/* 湿度探头 */
			/* 设置探头类型 */
			protocolReg->State.ProbeType[num] = ComProtocol_ProbeHumType;
			/* 设置数据 */
			protocolReg->State.Hum = dataFrame->Sensor[num];
			/* 判断报警状态 */
			if(protocolReg->State.Hum > (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_Up_Limit))
			{
				/* 高湿报警 */
				protocolReg->State.HumAlarm = 2;
				
			}else if(protocolReg->State.Hum < (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_Low_Limit))
			{
				/* 低湿报警 */
				protocolReg->State.HumAlarm = 4;
			}else
			{
				/* 正常 */
				protocolReg->State.HumAlarm = 0;
			}
		}else if(Sensor_GPS_POS_Lon 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* 经度探头 */
			/* 设置经度参数 */
			if(dataFrame->Sensor[num] != GPS_Data_Rece_Timeout_Value)
			{
				/* 已定位数据 */
				protocolReg->Lon = dataFrame->Sensor[num];
			}else
			{
				/* 未定位数据 */
				protocolReg->Lon = ComProtocol_NullGPSSignalPara;
			}
		}else if(Sensor_GPS_POS_Lat 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* 纬度探头 */
			/* 设置经度参数 */
			if(dataFrame->Sensor[num] != GPS_Data_Rece_Timeout_Value)
			{
				/* 已定位数据 */
				protocolReg->Lat = dataFrame->Sensor[num];
			}else
			{
				/* 未定位数据 */
				protocolReg->Lat = ComProtocol_NullGPSSignalPara;
			}
		}else if(Sensor_GPS_POS_Spd 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* 速度探头 */
			/* 设置经度参数 */
			if(dataFrame->Sensor[num] != GPS_Data_Rece_Timeout_Value)
			{
				/* 已定位数据 */
				protocolReg->Speed = dataFrame->Sensor[num];
			}else
			{
				/* 未定位数据 */
				protocolReg->Speed = ComProtocol_NullGPSSignalPara;
			}
		}else
		{
			continue;
		}

	}
	
	/* 判断GPS定位数据是否有信号 */
	if((0 == protocolReg->Lon) &&
	   (0 == protocolReg->Lat) &&
	   (0 == protocolReg->Speed))
	{
		/* 无信号 */
		/* 设置GPS状态为无信号0 */
		protocolReg->State.GPS = FALSE;
	}else
	{
		/* 有信号 */
		/* 设置GPS状态 */
		protocolReg->State.GPS = TRUE;
	}
	
	/* 设置方向参数为0 */
	protocolReg->Cog = 0;
	
	/* 设置电量 */
	protocolReg->State.BatteryValue = *cBatteryLevelp;
	
	/* 判断当前参数配置状态 */
	if(protocolReg->State.ProbeNum > 0)
	{
		/* 操作成功 */
		return TRUE;
	}
	
	/* 操作成功 */
	return FALSE;
}

/************************************************************************************************/
/* 函数名	: ComProtocolWZ_FrameData										 		 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 根据协议参数，组成一个完整的数据帧。					 								*/
/* 创建日期	: 2016/12/22																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
static void ComProtocolWZ_FrameData(ComProtocolWZCRStruct* protocolCR,uint8_t* cDataBuf)
{
	/* 定位时间字符串寄存器 */
	char cGPSTimeString[16]	={'\0'};
	
	char cGPSLonString[16]	={'\0'};
	char cGPSLatString[16]	={'\0'};
	char cGPSSpeedString[16]={'\0'};
	char cGPSCogString[16]	={'\0'};
	
	char cProbeDataString[5][16] ={'\0'};
	
	char cProbeString[60]	={'\0'};
	
	/* 计数器 */
	uint8_t num;
	/* 字符串长度计数器 */
	uint8_t cStringLen;
	/* 湿度探头标签 */
	uint8_t cProbeHumNum = 0;
	
	
	/* 根据定位时间设置定位时间字符串 */
	sprintf(cGPSTimeString,"%04d%02d%02d%02d%02d%02d",
			protocolCR->CollectTime.Year,
			protocolCR->CollectTime.Month,
			protocolCR->CollectTime.Day,
			protocolCR->CollectTime.Hour,
			protocolCR->CollectTime.Min,
			protocolCR->CollectTime.Sec);
	
	/* 设置GPS参数 */
	float_to_string(protocolCR->Lon,cGPSLonString);
	float_to_string(protocolCR->Lat,cGPSLatString);
	float_to_string(protocolCR->Speed,cGPSSpeedString);
	float_to_string(protocolCR->Cog,cGPSCogString);
	
	/* 设置探头参数字符串 */
	for(num = 0,cStringLen = 0;num < protocolCR->State.ProbeNum;num++)
	{
		/* 报警寄存器 */
		char cProbeAlarmString[12] = {'\0'};
		
		/* 判断当前探头数据 */
		if(ComProtocol_ProbeTempType == protocolCR->State.ProbeType[num])
		{
			/* 温度探头 */
			/* 设置探头数据 */
			float_to_string(protocolCR->State.Temp[num],cProbeDataString[num]);
			/* 设置报警状态字 */
			if((0 != protocolCR->State.TempAlarm[num])&&
			   (TRUE == protocolCR->State.PowerLink))
			{
				/* 报警状态，这只报警字符串 */
				sprintf(cProbeAlarmString,"ta%d:%d,",(1 + num - cProbeHumNum),protocolCR->State.TempAlarm[num]);
			}
			/* 温度探头 */
			sprintf((cProbeString+cStringLen),"t%d:%s,%s",
					 (1 + num - cProbeHumNum),cProbeDataString[num],cProbeAlarmString);
		}else if(ComProtocol_ProbeHumType == protocolCR->State.ProbeType[num])
		{
			/* 湿度探头 */
			/* 设置探头数据 */
			float_to_string(protocolCR->State.Hum,cProbeDataString[num]);
			/* 设置报警状态字 */
			if((0 != protocolCR->State.HumAlarm)&&
			   (TRUE == protocolCR->State.PowerLink))
			{
				/* 报警状态，这只报警字符串 */
				sprintf(cProbeAlarmString,"humia:%d,",protocolCR->State.HumAlarm);
			}
			/* 温度探头 */
			sprintf((cProbeString+cStringLen),"humi:%s,%s",cProbeDataString[num],cProbeAlarmString);
			/* 设置湿度通道数量 */
			cProbeHumNum++;
		}else
		{
			/* 未定义类型 */
			__nop();
		}
		
		/* 计算当前字符串长度 */
		cStringLen = strlen(cProbeString);
	}
	
	/* 将设置好的数据拷贝到数据寄存器中 */
	sprintf((char*)cDataBuf,"~%d&%s&%d|%s|%s|%s|%s|%s|%s|{acc:%d,gps:%d,%sbattery:%d}|%d|%s#",
			ComProtocol_VendorCode,//厂商编号
			ComProtocol_Cmd,//命令字
			protocolCR->RTDMode,//模式（这个固定为车牌号模式）
			protocolCR->PlateNo,//车牌号
			cGPSTimeString,//定位时间
			cGPSLonString,//经度
			cGPSLatString,//纬度
			cGPSSpeedString,//速度
			cGPSCogString,//方向
			((protocolCR->State.PowerLink == TRUE)?1:0),//电源连接状态
			((protocolCR->State.GPS == TRUE)?1:0),//GPS定位状态
			cProbeString,//探头参数字符串
			protocolCR->State.BatteryValue,//网关电池电量
			(protocolCR->State.PowerLink ==1 )?0:1,//终端主电源掉电报警状态
			ComProtocol_ACKFlag//消息应答标识
			);
}

#endif

#if (1) //功能函数接口
/************************************************************************************************/
/* 函数名	: ComProtWZ_CreateSendData										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 解析平台应答是否成功																*/
/* 作用	 	: 根据“万智String协议”通信协议，解析接收到的数据。					 				*/
/* 创建日期	: 2016/12/22																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
BOOL ComProtWZ_ParseData(uint8_t* receBuf)
{
//	/* 数据指针 */
//	char * datap;
//	
//	
//	/* 查询上传探头数据命令字符串 */
//	datap = strstr((char*)receBuf,ComProtocol_ProbeCmd);
//	/* 判断HTTP传输是否成功 */
//	if(NULL == datap)
//	{
//		/* 查询上传GPS数据命令字符串 */
//		datap = strstr((char*)receBuf,ComProtocol_GPSCmd);
//		if(NULL == datap)
//		{
//			/* 传输失败 */
//			return FALSE;
//		}
//	}
//	
//	/* 查询上传探头ID字符串 */
//	datap = strstr((char*)receBuf,ComProtocol_ProbeDataID);
//	/* 判断HTTP传输是否成功 */
//	if(NULL == datap)
//	{
//		/* 查询上传GPS ID字符串 */
//		datap = strstr((char*)receBuf,ComProtocol_GPSDataID);
//		if(NULL == datap)
//		{
//			/* 传输失败 */
//			return FALSE;
//		}
//	}
	
	/* 平台接收数据成功 */
	return TRUE;
}

/************************************************************************************************/
/* 函数名	: ComProtWZ_CreateData										 	 					*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 根据“万智String协议”通信协议，创建数据。					 								*/
/* 创建日期	: 2016/12/22																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
BOOL ComProtWZ_CreateData(Save_Sensor_Data_Struct* cSaveDatap)
{
	/* 设置参数状态 */
	BOOL SetProtState;
	/* 协议参数寄存器 */
	ComProtocolWZCRStruct cProtocolParaReg = {0};
	/* 应用存储数据 */
	AppSaveFrameStruct cSaveFrameReg;
	
	
	/* 设置空参数值 */
	cProtocolParaReg.State.Temp[0] = ComProtocol_NullProbePara;
	cProtocolParaReg.State.Temp[1] = ComProtocol_NullProbePara;
	cProtocolParaReg.State.Temp[2] = ComProtocol_NullProbePara;
	cProtocolParaReg.State.Temp[3] = ComProtocol_NullProbePara;
	cProtocolParaReg.State.Hum	   = ComProtocol_NullProbePara;
	
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
	SetProtState = ComProtocolWZ_SetPara(&cSaveFrameReg,&cProtocolParaReg);
	
	/* 判断设置参数状态 */
	if(FALSE == SetProtState)
	{
		/* 没有需要发送的参数，不发送数据 */
		return FALSE;
	}
	
	/* 清除发送数据寄存器 */
	memset(ComProtocolWZDR.DR.TXDR,0x00,sizeof(ComProtocolWZDR.DR.TXDR));
	
	/* 根据通信协议参数，组装发送数据帧 */
	ComProtocolWZ_FrameData(&cProtocolParaReg,ComProtocolWZDR.DR.TXDR);

	return TRUE;
}

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
