/************************************************************************************************/
/** @file              : ComProtocol_GY.c														*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��12��21��															*/
/** @brief             : c file																	*/
/** @description       : �����������--������--ͨ��Э��--��ҩ Ӧ���ļ�								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**�ļ���     : ComProtocol_GY.c																	*/
/**����       : ���ݹ�ҩ�ṩ��ͨ��Э���ʽ��̽ͷ�����ϴ�����ƽ̨									*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��06��03��   	 ��ʼ�汾												*/
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
/* �ĵ�̽ͷ�������� */
#define	ComProtocol_MaxProbeNum			(13u)

/* �豸̽ͷ������ */
#define	ComProtocol_ProbeTotalNum		(Sensor_Tmp_Num+\
										 Sensor_Hum_Num+\
										 Sensor_RF_Single_Tmp_Num+\
										 Sensor_RF_Humiture_Num+\
										 Sensor_PT1000_Num+\
										 Sensor_GW_NJSY_Num)

/* �ж�̽ͷ�����Ƿ����Э���趨���̽ͷ���� */
#if (ComProtocol_MaxProbeNum < ComProtocol_ProbeTotalNum)
#error "̽ͷ��������Э���趨���̽ͷ����13��"
#endif

/********************************* �궨�� *********************************/
/* ͨ��Э�� ���͵�������ݳ��� */
#define ComProtocol_MaxDataLen				(1024ul)


/* ͨ��Э�� ̽ͷ���������ʶ */
#define ComProtocol_ProbeCmd				("~UPTMP")

/* ͨ��Э�� GPS���������ʶ */
#define ComProtocol_GPSCmd					("~UPGPS")

/* ͨ��Э�� ̽ͷ����ID */
#define ComProtocol_ProbeDataID				("ProbeID")

/* ͨ��Э�� GPS����ID */
#define ComProtocol_GPSDataID				("GPSID")


/* �޴��������ݲ���(̽ͷ��û�д���������ʱ�Ĳ���) */
#define ComProtocol_NullSensor				(-1000.0)





/*******************************************************************************/
/********************************* ���ݽṹ *************************************/
/* ��ҩЭ�� ���ݽṹ */
typedef struct
{
	/* ���ƺ� */
	uint8_t PlateNo[27];
	/* �ɼ�ʱ�� */
	Times CollectTime;
	/* ̽ͷ���� */
	uint8_t ProbeNum;
	
	/* ̽ͷ�Ĵ��� */
	struct
	{
		/* ̽ͷ�� */
		uint8_t No[21];
		/* ̽ͷ���� */
		uint8_t	Type;
		/* ���� */
		float Level;
		/* �¶� */
		float Temp;
		/* ʪ�� */
		float Hum;
		/* ̽ͷ����ID */
		uint8_t ID[8];
	}Probe[Sensor_CH_Config_Num];
	
	/* GPS�Ĵ��� */
	struct
	{
		/* ����״̬ */
		BOOL WorkState;
		/* �˵��� */
		uint8_t TransportNo[24];
		/* �ٶ� */
		float Speed;
		/* ���� */
		float Lon;
		/* γ�� */
		float Lat;
		/* ���� */
		float Cog;
	}GPS;
	
}AppComProtocolStruct;


/********************************* ���ݼĴ��� *******************************/
/* ��ҩͨ��Э��Ӧ������ */
AppComProtocolGYStruct AppComProtocolGY = 
{
	/* ���üĴ��� */

	/* ��ҩ����ӿ� */
	/* ���� */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,	0X00,
	/* IP */
	'2' ,'2' ,'1' ,'.' , 	'1' ,'3' ,'3' ,'.' ,	'2' ,'3' ,'7' ,'.' ,
	'2' ,'3','3',0X00,	0X00,0X00,0X00,0X00,	0X00,
	/* �˿� */
	'8' ,'0' ,'8' ,'9' , 	0X00,0X00,0X00,0X00,	0X00,
		
	
	/* ״̬�Ĵ��� */
	/* �������ݱ�־ */
	FALSE,

	
	/* ���ݼĴ��� */
	0X00,
};


/********************************* �ļ��������� *********************************/
/* ������ҩӦ������ */
//static void ComProtGY_ParseData(void);
/* ��������֡ */
//static void ComProtGY_FrameData(AppComProtocolStruct* protocolPara);

/*******************************************************************************/
/********************************* �ļ��������� *********************************/


/************************************************************************************************/
#if (1) //�ļ�����
/************************************************************************************************/
/* ������	: ComProtGY_ScanMatchHumChannel										 	 			*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ���ó��ƺ���					 													*/
/* ��������	: 2016/12/23																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static uint8_t ComProtGY_ScanMatchHumChannel(uint32_t channelSN,INSTRU_SENSOR_CHANNEL* cSensorchP)
{
	/* ������ */
	uint8_t num;
	

	/* ��ѯ */
	for(num = 0;num < 32; num++)
	{
		
		/* �жϵ�ǰͨ���Ƿ�Ϊʪ��ͨ�� */
		if((Sensor_Hum == (((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)) ||
		   (Sensor_RF_Card_Humiture_Hum == (((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)) ||
		   (Sensor_GW_NJSY_Hum == (((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)))
		{
			/* ��ǰͨ��Ϊʪ��ͨ�� */
			/* �ж�ID���Ƿ���ͬ */
			
			if(((*((uint32_t*)(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO))) != 0x00000000) &&
			   ((*((uint32_t*)(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO))) == channelSN))
			{
				/* Ŀ���ID����ͬ */
				return num;
			}
		}
	}
	
	return (0xFF);
}

/************************************************************************************************/
/* ������	: ComProtGY_SetProtocolPara										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ����Э�����					 													*/
/* ��������	: 2016/12/22																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static BOOL ComProtGY_SetProtocolPara(AppSaveFrameStruct* dataFrame,
									  AppComProtocolStruct* protocolPara)
{
	/* ������ */ 
	uint8_t num;
	/* ������ͨ����� */
	uint8_t channelNo;
	/* ʪ��ͨ����� */
	uint8_t humchannelNo;
	/* ���ƺ�ָ�� */
	char* cPlateNo = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO;
	/* ������ͨ��ָ�� */
	INSTRU_SENSOR_CHANNEL* cSensorChp = &Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1;
	/* ��������������ָ�� */
	INSTRU_SENSOR_CHANNEL_FIX* cSensorLevelp = &Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1;
	
	
	/* ���ó��ƺ� */
	ComProtocol_SetPlateNo(protocolPara->PlateNo,cPlateNo);
	/* ���òɼ�ʱ�� */
	protocolPara->CollectTime = dataFrame->Timestamp;
	/* ��ȡ��ǰ̽ͷ���� */
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
	/* ����GPS����״̬ */
	protocolPara->GPS.WorkState = TRUE;
	/* �����˵��� */
	sprintf((char*)protocolPara->GPS.TransportNo,"");
#else
	/* ����GPS����״̬ */
	protocolPara->GPS.WorkState = FALSE;
#endif
	
	/* ��˳��������̽ͷ���� */
	for(num = 0,channelNo = 0;(channelNo < protocolPara->ProbeNum)&&(channelNo < Sensor_CH_Config_Num);num++)
	{
		if((Sensor_Tmp 					== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
				Sensor_DoorContact 					== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_PT1000 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_RF_Card_Single_Tmp 	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_RF_Card_Humiture_Tmp == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_GW_NJSY_Tmp 		  	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type))
		{
			/* �ж�̽ͷSN�� */
			if((((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* SN��δ���ù��������͵�ǰͨ������. */
				/* ����һ��ͨ��. */
				protocolPara->ProbeNum--;
				/* ���� */
				continue;
			}
			
			/* ����̽ͷ��� */
			sprintf((char*)protocolPara->Probe[channelNo].No,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			
			/* ����̽ͷID */
			sprintf((char*)protocolPara->Probe[channelNo].ID,ComProtocol_ProbeDataID);
			
			/* ����̽ͷ���� */
			protocolPara->Probe[channelNo].Type = ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type;
			
			/* �����¶����� */
			protocolPara->Probe[channelNo].Temp = dataFrame->Sensor[num];
			
			/* ���õ��� */
			protocolPara->Probe[channelNo].Level = ((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorLevelp+num))->Sensor_Power_State;
			
			/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
			humchannelNo = ComProtGY_ScanMatchHumChannel(
			*((uint32_t*)(((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_SN_NO)),
			&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1);
			
			/* �ж�������� */
			if(0xFF != humchannelNo)
			{
				/* �����¶����� */
				protocolPara->Probe[channelNo].Hum = dataFrame->Sensor[humchannelNo];		
			}else
			{
				/* ����ʪ������ */
				protocolPara->Probe[channelNo].Hum = ComProtocol_NullSensor;
			}
			
			/* �ж��Ƿ���յ��¶����� */
			if(Wireless_RF_Card_Data_Rece_Timeout_Para == protocolPara->Probe[channelNo].Temp)
			{
				/* δ�յ����߿�Ƭ���ݣ������¶�����Ϊ�ա� */
				protocolPara->Probe[channelNo].Temp = ComProtocol_NullSensor;
			}
			/* �ж��Ƿ���յ�ʪ������ */
			if(Wireless_RF_Card_Data_Rece_Timeout_Para == protocolPara->Probe[channelNo].Hum)
			{
				/* δ�յ����߿�Ƭ���ݣ�����ʪ������Ϊ�ա� */
				protocolPara->Probe[channelNo].Hum = ComProtocol_NullSensor;
			}
			
		}else if(Sensor_GPS_POS_Lon == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* ���þ������� */
			protocolPara->GPS.Lon = dataFrame->Sensor[num];
			
			/* �ж��Ƿ���յ��������� */
			if(GPS_Data_Rece_Timeout_Value == protocolPara->GPS.Lon)
			{
				/* δ�յ�GPS�������ݣ����þ�������Ϊ�ա� */
				protocolPara->GPS.Lon = ComProtocol_NullSensor;
			}
			
			/* ����һ��ͨ��. */
			protocolPara->ProbeNum--;
			continue;
		}else if(Sensor_GPS_POS_Lat == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* ����γ������ */
			protocolPara->GPS.Lat = dataFrame->Sensor[num];
			/* �ж��Ƿ���յ�γ������ */
			if(GPS_Data_Rece_Timeout_Value == protocolPara->GPS.Lat)
			{
				/* δ�յ�GPS�������ݣ�����γ������Ϊ�ա� */
				protocolPara->GPS.Lat = ComProtocol_NullSensor;
			}
			
			/* ����һ��ͨ��. */
			protocolPara->ProbeNum--;
			continue;
		}else if(Sensor_GPS_POS_Spd == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* �����ٶ����� */
			protocolPara->GPS.Speed = dataFrame->Sensor[num];
			/* �ж��Ƿ���յ��ٶ����� */
			if(GPS_Data_Rece_Timeout_Value == protocolPara->GPS.Speed)
			{
				/* δ�յ�GPS�ٶ����ݣ������ٶ�����Ϊ�ա� */
				protocolPara->GPS.Speed = ComProtocol_NullSensor;
			}
			
			/* ����һ��ͨ��. */
			protocolPara->ProbeNum--;
			continue;
		}else
		{
			/* ����һ��ͨ��. */
			protocolPara->ProbeNum--;
			continue;
		}
		/* ����ͨ����� */
		channelNo++;
	}
	
	/* �жϵ�ǰ��������״̬ */
	if((protocolPara->ProbeNum > 0)||(protocolPara->GPS.WorkState))
	{
		/* �����ɹ� */
		return TRUE;
	}
	
	/* �����ɹ� */
	return FALSE;
}

/************************************************************************************************/
/* ������	: ComProtGY_FrameData										 		 				*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ����Э����������һ������������֡��					 								*/
/* ��������	: 2016/12/22																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void ComProtGY_FrameData(AppComProtocolStruct* protocolPara,uint8_t* cDataBuf)
{
	/* ������ */
	uint8_t num;
	/* ����ָ�� */
	uint32_t ProbeDataIndex;
	/* �ɼ�ʱ�� */
	char cProbeCollectTime[24] = {0};
	/* ����IDʱ�� */
	char cComProtocol_DataIDTime[24] = {0};
	/* ̽ͷ���ݼĴ��� */
	uint8_t cComProtocol_ProbeDataReg[1024] = {0};
	/* GPS���ݼĴ��� */
	uint8_t cComProtocol_GPSDataReg[128] = {0};
	/* ̽ͷ����ID��� */
	static uint8_t cProbeDataIDNum = 0;
	/* GPS����ID��� */
	static uint8_t cGPSDataIDNum = 0;
	
	
	/* �ɼ�ʱ�� */
	sprintf(cProbeCollectTime,"%04d-%02d-%02d %02d:%02d:%02d",
	protocolPara->CollectTime.Year,protocolPara->CollectTime.Month,
	protocolPara->CollectTime.Day,protocolPara->CollectTime.Hour,
	protocolPara->CollectTime.Min,protocolPara->CollectTime.Sec);
	
	/* ��������ID��� */
	sprintf(cComProtocol_DataIDTime,"%04d%02d%02d%02d%02d%02d",
	protocolPara->CollectTime.Year,protocolPara->CollectTime.Month,
	protocolPara->CollectTime.Day,protocolPara->CollectTime.Hour,
	protocolPara->CollectTime.Min,protocolPara->CollectTime.Sec);
	
	/* �ж��Ƿ���Ҫ����̽ͷ���� */
	if(protocolPara->ProbeNum > 0)
	{
		/* ����¶����ݻ��� */
		memset(cComProtocol_ProbeDataReg,0x00,sizeof(cComProtocol_ProbeDataReg));
		
		/* ������������ */
		sprintf((char*)cComProtocol_ProbeDataReg,"%s",ComProtocol_ProbeCmd);
		
		/* ��������ָ��λ�� */
		ProbeDataIndex = strlen((char*)cComProtocol_ProbeDataReg);
		
		/* ����̽ͷ���ã��������ݡ� */
		for(num = 0;num < protocolPara->ProbeNum ;num++)
		{
			/* ̽ͷ���� */
			char cProbeLevel[12] = {0};
			/* ̽ͷ���� */
			char cProbeTemp[12] = {'\0'};
			/* ̽ͷ���� */
			char cProbeHum[12] = {'\0'};
			
			
			/* ���õ�ص��� */
			float_to_string(protocolPara->Probe[num].Level,cProbeLevel);
			
			/* �ж��¶������Ƿ�Ϊ������״̬ */
			if(ComProtocol_NullSensor != protocolPara->Probe[num].Temp)
			{
				/* ����̽ͷ�¶� */
				float_to_string(protocolPara->Probe[num].Temp,cProbeTemp);
			}
			
			/* �ж�ʪ�������Ƿ�Ϊ������״̬ */
			if(ComProtocol_NullSensor != protocolPara->Probe[num].Hum)
			{
				/* ����̽ͷʪ�� */
				float_to_string(protocolPara->Probe[num].Hum,cProbeHum);
			}
			
			/* ����̽ͷ���� */
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
			
			/* ̽ͷ����ID��� */
			cProbeDataIDNum++;
			
			/* ��������ָ��λ�� */
			ProbeDataIndex = strlen((char*)cComProtocol_ProbeDataReg);
		}
		
		/* ����̽ͷ���ݽ����� */
		sprintf((char*)(&cComProtocol_ProbeDataReg[ProbeDataIndex]),"%s","#");
		/* ����̽ͷ���ݳ���ָ�� */
		ProbeDataIndex += 1;
	}
	
	
	/* �ж��Ƿ���Ҫ����GPS���� */
	if(protocolPara->GPS.WorkState == TRUE)
	{
		/* ��Ҫ����GPS���� */
		/* GPS����ָ�� */
		uint32_t GPSDataIndex;
		/* GPS���� */
		char cGPSSpeed[12] = {0};
		/* GPS���� */
		char cGPSLon[12] = {0};
		/* GPSγ�� */
		char cGPSLat[12] = {0};
		/* GPS���� */
		char cGPSCog[12] = {0};
		
		
		/* ���GPS���ݻ��� */
		memset(cComProtocol_GPSDataReg,0x00,sizeof(cComProtocol_GPSDataReg));
		
		/* ����GPS�������� */
		sprintf((char*)cComProtocol_GPSDataReg,"%s",ComProtocol_GPSCmd);
		
		/* ��������ָ��λ�� */
		GPSDataIndex = strlen((char*)cComProtocol_GPSDataReg);
		
		/* �ж��ٶ������Ƿ�Ϊ������״̬ */
		if(ComProtocol_NullSensor != protocolPara->GPS.Speed)
		{
			/* �����ٶ����� */
			float_to_string(protocolPara->GPS.Speed,cGPSSpeed);
		}
		
		/* �жϾ��������Ƿ�Ϊ������״̬ */
		if(ComProtocol_NullSensor != protocolPara->GPS.Lon)
		{
			/* ���þ������� */
			float_to_string(protocolPara->GPS.Lon,cGPSLon);
		}
		
		/* �ж�γ�������Ƿ�Ϊ������״̬ */
		if(ComProtocol_NullSensor != protocolPara->GPS.Lat)
		{
			/* ����γ������ */
			float_to_string(protocolPara->GPS.Lat,cGPSLat);
		}
		
		/* �ж�γ�������Ƿ�Ϊ������״̬ */
		if(ComProtocol_NullSensor != protocolPara->GPS.Cog)
		{
			/* ����γ������ */
			float_to_string(protocolPara->GPS.Cog,cGPSCog);
		}
		
		/* ����GPS���� */
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
		
		/* ̽ͷ����ID��� */
		cGPSDataIDNum++;
		
		/* ��������ָ��λ�� */
		GPSDataIndex = strlen((char*)cComProtocol_GPSDataReg);
		
		/* ����̽ͷ���ݽ����� */
		sprintf((char*)(&cComProtocol_GPSDataReg[GPSDataIndex]),"%s","#");
		
		/* ����̽ͷ���ݳ���ָ�� */
		ProbeDataIndex += 1;
			
	}
	
	/* �����úõ����ݿ��������ݼĴ����� */
	sprintf((char*)cDataBuf,"%s%s",
			cComProtocol_ProbeDataReg,
			cComProtocol_GPSDataReg);
	
	
}

#endif

#if (1) //���ܺ����ӿ�
/************************************************************************************************/
/* ������	: ComProtGY_CreateSendData										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ����ƽ̨Ӧ���Ƿ�ɹ�																				*/
/* ����	 	: ���ݡ���ҩ��ͨ��Э�飬�������յ������ݡ�					 							*/
/* ��������	: 2016/12/22																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
BOOL ComProtGY_ParseData(uint8_t* receBuf)
{
	/* ����ָ�� */
	char * datap;
	
	
	/* ��ѯ�ϴ�̽ͷ���������ַ��� */
	datap = strstr((char*)receBuf,ComProtocol_ProbeCmd);
	/* �ж�HTTP�����Ƿ�ɹ� */
	if(NULL == datap)
	{
		/* ��ѯ�ϴ�GPS���������ַ��� */
		datap = strstr((char*)receBuf,ComProtocol_GPSCmd);
		if(NULL == datap)
		{
			/* ����ʧ�� */
			return FALSE;
		}
	}
	
	/* ��ѯ�ϴ�̽ͷID�ַ��� */
	datap = strstr((char*)receBuf,ComProtocol_ProbeDataID);
	/* �ж�HTTP�����Ƿ�ɹ� */
	if(NULL == datap)
	{
		/* ��ѯ�ϴ�GPS ID�ַ��� */
		datap = strstr((char*)receBuf,ComProtocol_GPSDataID);
		if(NULL == datap)
		{
			/* ����ʧ�� */
			return FALSE;
		}
	}
	
	/* ƽ̨�������ݳɹ� */
	return TRUE;
}

/************************************************************************************************/
/* ������	: ComProtGY_CreateData										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ���ݡ���ҩ��ͨ��Э�飬�������ݡ�					 								*/
/* ��������	: 2016/12/22																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
BOOL ComProtGY_CreateData(Save_Sensor_Data_Struct* cSaveDatap)
{
	/* ���ò���״̬ */
	BOOL SetProtState;
	/* Э������Ĵ��� */
	AppComProtocolStruct cProtocolParaReg = {0};
	/* Ӧ�ô洢���� */
	AppSaveFrameStruct cSaveFrameReg;
	
	
	/* ��ʼ��Э������Ĵ��� */
	cProtocolParaReg.GPS.Cog 	= 0;
	cProtocolParaReg.GPS.Lat 	= ComProtocol_NullSensor;
	cProtocolParaReg.GPS.Lon 	= ComProtocol_NullSensor;
	cProtocolParaReg.GPS.Speed 	= ComProtocol_NullSensor;
	
	/* ���òɼ�ʱ�� */
	cSaveFrameReg.Timestamp.Year 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS + 2000;
	cSaveFrameReg.Timestamp.Month	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS;
	cSaveFrameReg.Timestamp.Day 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS;
	cSaveFrameReg.Timestamp.Hour 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS;
	cSaveFrameReg.Timestamp.Min 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS;
	cSaveFrameReg.Timestamp.Sec 	= cSaveDatap->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS;
	
	/* ���ƴ��������� */
	memcpy(cSaveFrameReg.Sensor,cSaveDatap->GPRS_IR.Sensor_Data,sizeof(cSaveFrameReg.Sensor));
	
	/* ���ݴ洢������Ϣ������ͨ��Э����� */
	SetProtState = ComProtGY_SetProtocolPara(&cSaveFrameReg,&cProtocolParaReg);
		
	/* �ж����ò���״̬ */
	if(FALSE == SetProtState)
	{
		/* û����Ҫ���͵Ĳ��������������� */
		return FALSE;
	}
	
	/* ����������ݼĴ��� */
	memset(AppComProtocolGY.DR.TXDR,0x00,sizeof(AppComProtocolGY.DR.TXDR));
	
	/* ����ͨ��Э���������װ��������֡ */
	ComProtGY_FrameData(&cProtocolParaReg,AppComProtocolGY.DR.TXDR);

	return TRUE;
}

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
