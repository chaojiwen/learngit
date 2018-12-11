/************************************************************************************************/
/** @file              : ComProtocol_ZDE.c														*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017��02��16��															*/
/** @brief             : c file																	*/
/** @description       : �����������--������--ͨ��Э��--�����ͨ��Э�顱Ӧ���ļ�					*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**�ļ���     : ComProtocol_ZDE.c																*/
/**����       : ���ݡ����ͨ��Э�顱�ṩ��ͨ��Э���ʽ��̽ͷ�����ϴ�����ƽ̨							*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2017��02��16��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "ComProtocol_ZDE.h"

#if (ComProtocol_ZDE_Enable == 0x01)
//#include <stdio.h>
#include <string.h>
#include "App_Save.h"

/*******************************************************************************/
/********************************* ȫ�ֱ������� *****************************/
/* ���̽ͷ�� */
#define ComProtocolZDE_ProbeNumMax			(16u)

/* ��ƥ��̽ͷֵ */
#define ComProtocolZDE_MatchingCHNull		(0xFF)

/* ��ʶλֵ */
#define ComProtocolZDE_StartIdentify		(0x7E)
#define ComProtocolZDE_EndIdentify			(ComProtocolZDE_StartIdentify)

/* ��ϢͷIDֵ */
#define ComProtocolZDE_MessageHeaderID		(0x0900)

/* ��ϢͷIDֵ */
#define ComProtocolZDE_ACKMessageID			(0x8001)

/* һ֡���ݳ��� */
#define ComProtocolZDE_FrameDataLen			(28u+(((IntProbeNum<2)?2:IntProbeNum)*4))



/********************************* �����ͨ��Э�顱�����ṹ **************/
/* ������Ϣͷ�ṹ */
typedef struct
{
	/* ��ϢID */
	uint16_t ID;
	/* ���ص�Դ״̬ */
	BOOL PowerLinkState;
	/* ���ص���״̬ */
	uint8_t PowerLevelState;
	/* ��λ��־ */
	BOOL GPSFlag;
	/* �ն����к�ģʽ */
	uint8_t	SN_Mode;
	/* �ն����к� */
	uint8_t SN[12];
}
ComProtocolZDEPara_MH;

/* ��Ϣ��֡����̽ͷ��Ϣ�ṹ */
typedef struct
{
	/* ̽ͷSN�� */
	uint8_t SN[4];
	/* ̽ͷ���� */
	uint8_t Level;
	/* ̽ͷ���� */
	uint8_t Type;
	/* ͨ������ */ 
	uint8_t ChannelNum;
	/* ���� */ 
	uint8_t Reserved;
	/* ���� */ 
	float  Data[8];
}
ComProtocolZDEPara_MBFI;

/* ������Ϣ��֡���ݽṹ */
typedef struct
{
	/* �ɼ�ʱ�� */
	uint8_t CollectionTime[6];
	/* ̽ͷ���� */
	uint8_t ProbeNum;
	/* �����ź�ǿ�� */
	uint8_t GSMSignal;
	/* ���ص�Դ״̬ */
	BOOL PowerLinkState;
	/* ���ص���״̬ */
	uint8_t PowerLevelState;
	/* ���� */
	float	Lon;
	/* γ�� */
	float	Lat;
	/* �ٶ� */
	float	Speed;
	/* ̽ͷ���� */
	ComProtocolZDEPara_MBFI ProbeInfo[16];
}
ComProtocolZDEPara_MBF;

/* ������Ϣ��ṹ */
typedef struct
{
	/* �������� */
	uint32_t FrameNum;
	/* ����֡ */
	ComProtocolZDEPara_MBF Frame;
}
ComProtocolZDEPara_MB;

/* �����ͨ��Э�顱 �����ṹ */
typedef struct
{
	/* ��Ϣͷ���� */
	ComProtocolZDEPara_MH MessageHeader;
	/* ��Ϣ����� */
	ComProtocolZDEPara_MB MessageBody;
	
}
ComProtocolZDEParaStruct;

/********************************* �����ͨ��Э�顱�ṹ **************/
/* ��Ϣ�����Խṹ */
typedef struct
{
	/* ��Ϣ�峤�� */
	uint16_t Len:10;
	/* ���ݼ��ܷ�ʽ */
	uint16_t EncryptMode:3;
	/* �ְ� */
	uint16_t PacketFlag:1;
	/* ���� */
	uint16_t Reserve:2;
}
ComProtocolZDE_MBA;

/* ��Ϣͷ�ṹ */
typedef struct
{
	/* ��ϢID */
	uint16_t ID;
	/* ��Ϣ������ */
	uint16_t Attribute;
	/* Ԥ�� */
	uint8_t Reserved[2];
//	/* ���ص�Դ״̬ */
//	BOOL PowerLinkState;
//	/* ���ص���״̬ */
//	uint8_t PowerLevelState;
	/* ��λ��־ */
	BOOL GPSFlag;
	/* �ն����к�ģʽ */
	uint8_t	SN_Mode;
	/* �ն����к� */
	uint8_t SN[12];

	/* ��Ϣ����װ�� */
	
}
ComProtocolZDE_MH;

/* ��Ϣ��֡����̽ͷ��Ϣ�ṹ */
typedef struct
{
	/* ̽ͷSN�� */
	uint8_t	SN[4];
	/* ̽ͷ���� */
	uint8_t Level;
	/* ̽ͷ���� */
	uint8_t Type;
	/* ͨ������ */ 
	uint8_t ChannelNum;
	/* ���� */ 
	uint8_t Reserve;
	/* ���� */ 
	float* Data;
}
ComProtocolZDE_MBFI;

/* ��Ϣ��֡���ݽṹ */
typedef struct
{
	/* �ɼ�ʱ�� */
	uint8_t CollectionTime[6];
	/* ̽ͷ���� */
	uint8_t ProbeNum;
	/* �����ź�ǿ�� */
	uint8_t GSMSignal;
	/* ���ص�Դ״̬ */
	uint8_t PowerLinkState;
	/* ���ص���״̬ */
	uint8_t PowerLevelState;
	/* ����1  */
	uint8_t Reserved1[2];
	/* ���� */
	float	Lon;
	/* γ�� */
	float	Lat;
	/* �ٶ� */
	float	Speed;
	/* ����2  */
	uint8_t Reserved2[4];
	/* ̽ͷ���� */
	ComProtocolZDE_MBFI ProbeInfo;
}
ComProtocolZDE_MBF;

/* ��Ϣ��ṹ */
typedef struct
{
	/* ����֡�� */
	uint32_t FrameNum;
	/* �������� */
	ComProtocolZDE_MBF* FrameData;
}
ComProtocolZDE_MB;

typedef struct
{
	/* ��Ϣͷ */
	ComProtocolZDE_MH	MessageHeader;
	
	/* ��Ϣ�� */
	ComProtocolZDE_MB	MessageBody;
	
	/* ������ */
	uint8_t CheckCode;
}
ComProtocolZDEStruct;

/* ����Ӧ����Ϣ�ṹ */
typedef struct
{
	/* ��ʶλ */
	uint8_t StartIdentify;
	
	/* ��ϢӦ��ID */
	uint8_t ID1;
	uint8_t ID2;
	/* Ӧ���� */
	uint8_t Result;
	
	/* ������ */
	uint8_t CheckCode;
	
	/* ��ʶλ */
	uint8_t EndIdentify;
	
	
}
ComProtocolZDEACKStruct;


/********************************* ���ݼĴ��� *******************************/
/* ����StringЭ��ͨ��Э��Ӧ������ */
ComProtocolZDEAppStruct ComProtocolZDEApp;

#if (1) //�ļ�����
/************************************************************************************************/
/* ������	: ComProtocolZDE_SetPara										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ����Э�����					 													*/
/* ��������	: 2017/02/17																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static uint8_t ComProtocolZDE_SearchMatchingSN(ComProtocolZDEPara_MBF* probeInfo,uint8_t* sn)
{
	/* ������ */
	uint8_t cNum;
	
	/* ����ƥ���SN�� */
	for(cNum = 0;cNum < ComProtocolZDE_ProbeNumMax; cNum++)
	{
		/* �жϵ�ǰ��SN���Ƿ���Ч */
		if((0x00 != probeInfo->ProbeInfo[cNum].SN[0])||
		   (0x00 != probeInfo->ProbeInfo[cNum].SN[1])||
		   (0x00 != probeInfo->ProbeInfo[cNum].SN[2])||
		   (0x00 != probeInfo->ProbeInfo[cNum].SN[3]))
		{
			/* SN�����ù� */
			/* �жϵ�ǰ��SN����Ŀ��SN���Ƿ���ͬ */
			if((*sn == probeInfo->ProbeInfo[cNum].SN[0])&&
			   (*(sn+1) == probeInfo->ProbeInfo[cNum].SN[1])&&
			   (*(sn+2) == probeInfo->ProbeInfo[cNum].SN[2])&&
			   (*(sn+3) == probeInfo->ProbeInfo[cNum].SN[3]))
			{
				/* ���ص�ǰ��ͨ����� */
				return cNum;
			}
		}
	}

	/* Ϊ���ҵ�ƥ���SN�� */
	return ComProtocolZDE_MatchingCHNull;
}

/************************************************************************************************/
/* ������	: ComProtocolZDE_SetMessageHeaderPara										 		*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ����Э�����Ϣͷ����					 											*/
/* ��������	: 2017/02/21																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
BOOL PowerLinkState;
uint8_t PowerLevelState;
static void ComProtocolZDE_SetMessageHeaderPara(INSTRU_CONFIG_INF_TAB* config_tab,
												ComProtocolZDEStruct* cProtocolReg)
{
	/* ��Ϣ�����ԼĴ��� */
	ComProtocolZDE_MBA cMessageAttribute = {0};
	/* ������ */
	uint8_t cNum;
	
//	/* �������üĴ����������� */
//	cProtocolReg->StartIdentify = ComProtocolZDE_StartIdentify;
	
	/* ��ϢͷID */
	cProtocolReg->MessageHeader.ID = ComProtocolZDE_MessageHeaderID;
	
	/* ��Ϣ������ */
	/* ���ܷ�ʽ:������ */
	cMessageAttribute.EncryptMode = 0;
	/* �������ݳ��� */
	cMessageAttribute.Len = 0;
	/* �ְ���־:���ְ� */
	cMessageAttribute.PacketFlag = 0;
	/* ������Ϣ�����Բ��� */
	cProtocolReg->MessageHeader.Attribute = *((uint16_t*)(&cMessageAttribute));
	
//	/* ���ص�Դ״̬ */
//	PowerLinkState = 
//	config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
//	
//	/* ���ص���״̬ */
//	PowerLevelState = 
//	config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	
	//Ԥ��
	cProtocolReg->MessageHeader.Reserved[0]=0;
	cProtocolReg->MessageHeader.Reserved[1]=0;

	/* �������� */
	if(0x01 == GPS_Config_Enable)
	{
		/* ���ض�λ�豸 */
		cProtocolReg->MessageHeader.GPSFlag = TRUE;
	}else
	{
		/* δ���ض�λ�豸 */
		cProtocolReg->MessageHeader.GPSFlag = FALSE;
	}

	/* �ն����к�ģʽ Ŀǰ�ȹ̶�Ϊ1:�豸��ģʽ */
	cProtocolReg->MessageHeader.SN_Mode = 0x01;
	
	/* ����ʹ��memcpyΪʲô�ᵼ��HardFault_Handler������� */
	
	for(cNum = 0;cNum <sizeof(config_tab->Inf_State.Instru_System.Inf.Serial_Number);cNum++)
	{
		/* ���� */
		cProtocolReg->MessageHeader.SN[cNum] = 
		config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[cNum];
	}
	
}

/************************************************************************************************/
/* ������	: ComProtocolZDE_SetMessageBodyPara										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ����Э�����					 													*/
/* ��������	: 2017/02/17																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static BOOL ComProtocolZDE_SetMessageBodyPara(AppSaveFrameStruct* dataFrame,
								   ComProtocolZDEParaStruct* protocolParaReg,
								   INSTRU_CONFIG_INF_TAB* config_tab,
								   INSTRU_FIX_INF_TAB* fix_tab)
{
	/* ������ */
	uint8_t num;
	/* ̽ͷ��Ϣ�Ĵ������� */
	uint8_t cProbeIndex = 0;
	/* �洢�������� */
	uint8_t cSaveDataIndex = 0;
	/* ͨ������ */
	INSTRU_SENSOR_CHANNEL* cSensorchP = &config_tab->Inf_State.Instru_Sensor.Inf_State.CH1;
	/* ͨ������ */
	INSTRU_SENSOR_CHANNEL_FIX* cSensorChDataP = &fix_tab->Inf_State.Instru_Sensor.Inf.CH1;
	
	//printf("ComProtocolZDE_SetMessageBodyPara\r\n");
	
	if((dataFrame->Timestamp.Year<=0)\
		|| (dataFrame->Timestamp.Month<=0)\
		|| (dataFrame->Timestamp.Day<=0))
	{
		return FALSE;
	}
	
	
	/* ̽ͷ��Ϣ */
	/* ���òɼ�ʱ��  */
	protocolParaReg->MessageBody.Frame.CollectionTime[0] = (dataFrame->Timestamp.Year - 2000u)/10*16+(dataFrame->Timestamp.Year - 2000u)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[1] = (dataFrame->Timestamp.Month)/10*16+(dataFrame->Timestamp.Month)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[2] = (dataFrame->Timestamp.Day)/10*16+(dataFrame->Timestamp.Day)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[3] = (dataFrame->Timestamp.Hour)/10*16+(dataFrame->Timestamp.Hour)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[4] = (dataFrame->Timestamp.Min)/10*16+(dataFrame->Timestamp.Min)%10;
	protocolParaReg->MessageBody.Frame.CollectionTime[5] = (dataFrame->Timestamp.Sec)/10*16+(dataFrame->Timestamp.Sec)%10;
	
	/* ���ص�Դ״̬ */
	PowerLinkState = 
	config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
	
	/* ���ص���״̬ */
	PowerLevelState = 
	config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	
	/* �жϵ�ǰ�豸�Ƿ���ض�λ�豸 */
	if(0x01 == GPS_Config_Enable)
	{
		/* ���ض�λ�豸������GPS���� */
		for(num = 0;num < Sensor_CH_Config_Num; num++)
		{
			/* ����GPS����ͨ�� */
			switch(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)
			{
				/* ����ͨ�� */
				case Sensor_GPS_POS_Lon:
				{
					/* ���þ��Ȳ��� */
					protocolParaReg->MessageBody.Frame.Lon = dataFrame->Sensor[num];
					break;
				}
				
				/* γ��ͨ�� */
				case Sensor_GPS_POS_Lat:
				{
					/* ����γ�Ȳ��� */
					protocolParaReg->MessageBody.Frame.Lat = dataFrame->Sensor[num];
					break;
				}
				
				/* γ��ͨ�� */
				case Sensor_GPS_POS_Spd:
				{
					/* ����γ�Ȳ��� */
					protocolParaReg->MessageBody.Frame.Speed = dataFrame->Sensor[num];
					break;
				}
				
				default:
				{
					/* ��GPS����ͨ�� */
					__nop();
				}
			}
		}
	}else
	{
		/* δ���ض�λ�豸������GPS���� */
		protocolParaReg->MessageBody.Frame.Lon 		= GPS_Data_Rece_Timeout_Value;
		protocolParaReg->MessageBody.Frame.Lat 		= GPS_Data_Rece_Timeout_Value;
		protocolParaReg->MessageBody.Frame.Speed 	= GPS_Data_Rece_Timeout_Value;
	}
	
	/* �жϼ�������̽ͷ���� */
	if(0 < IntProbeNum)
	{
		/* ���ɵ�ͨ������ */
		uint8_t cChannelnum = 0;
		
		
		/* ����̽ͷ��Ϣ */
		/* ����SN�� *///����ֻ��ʹ��SN��ʽ������ʹ��SIM�����ֻ��ŷ�ʽ��Ҫ�޸Ĵ��룡
		memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
			   protocolParaReg->MessageHeader.SN,
				sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
		
		/* ̽ͷ���� */
		protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x03;
		
		/* ��ص��� */
		protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level = 
		config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
		
		for(num = 0;num < Sensor_CH_Config_Num; num++)
		{
			switch(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)
			{
				/* �¶�ͨ�� */
				case Sensor_Tmp:
				{
					/* ���ò��� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[num] = dataFrame->Sensor[num];
					/* ����ͨ������ */
					cChannelnum++;
					break;
				}
				
				/* ʪ��ͨ�� */
				case Sensor_Hum:
				{
					/* ���ò��� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[num] = dataFrame->Sensor[num];
					/* ����ͨ������ */
					cChannelnum++;
					break;
				}
				
				/* �Ŵſ���ͨ�� */
				case Sensor_DoorContact:
				{
					/* ���ò��� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[num] = dataFrame->Sensor[num];
					/* ����ͨ������ */
					cChannelnum++;
					break;
				}
				
				/* PT1000�¶�ͨ�� */
				case Sensor_PT1000:
				{
					/* ���ò��� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[num] = dataFrame->Sensor[num];
					/* ����ͨ������ */
					cChannelnum++;
					break;
				}
				
				default:
				{
					/* �Ǽ����������ϵĴ���������ͨ�� */
					__nop();
					break;
				}
			}
		}
		
		/* ����ͨ������ */
		protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = cChannelnum;
		
		/* ����̽ͷ���� */
		cProbeIndex++;
	}
	
	/* �ж�����̽ͷ���� */
	if(0 < WirelessProbeNum)
	{
		/* ����̽ͷ */
		
		
		for(num = 0;num < Sensor_CH_Config_Num; num++)
		{
			/* �жϵ�ǰͨ��SN���Ƿ����ù� */
			if((0x00 == ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO[0])&&
			   (0x00 == ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO[1])&&
			   (0x00 == ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO[2])&&
			   (0x00 == ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO[3]))
			{
				/* Ϊ���ù���ͨ�����ϴ��͸�ͨ��Ϣ */
				continue;
			}
			
			/* ��ǰͨ��SN���Ѿ����ù����ж�ͨ������ */
			switch(((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->Type)
			{
				/* ����ͨ�������߿�Ƭ */
				case Sensor_RF_Card_Single_Tmp:
				{
					/* ����SN�� */
					memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
						   ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO,
						   sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
					
					/* ̽ͷ���� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x01;
					
					/* ��ص��� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level = 
					((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorChDataP+num))->Sensor_Power_State;
					
					/* ���ò���,�ж������Ƿ�Ϊ���ߺ����� */
					if(Wireless_RF_Card_Data_Rece_Timeout_Para == dataFrame->Sensor[cSaveDataIndex])
					{
						/* ���ź����� */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = 
						Wireless_RF_Card_Data_Rece_Timeout_Value;
					}else
					{
						/* �������� */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = dataFrame->Sensor[cSaveDataIndex];
					}
					/* ����ͨ������ */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 1;
					/* ����̽ͷ���� */
					cProbeIndex++;
					break;
				}
				
				/* ����ͨ��ʪ��Ƭ�¶�ͨ�����߿�Ƭ */
				case Sensor_RF_Card_Humiture_Tmp:
				{
					/* ����SN�� */
					memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
						   ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO,
						   sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
					
					/* ̽ͷ���� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x02;
					
					/* ��ص��� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level =
					((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorChDataP+num))->Sensor_Power_State;
					
					/* ���ò���,�ж������Ƿ�Ϊ���ߺ����� */
					if(Wireless_RF_Card_Data_Rece_Timeout_Para == dataFrame->Sensor[cSaveDataIndex])
					{
						/* ���ź����� */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = 
						Wireless_RF_Card_Data_Rece_Timeout_Value;
					}else
					{
						/* �������� */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = dataFrame->Sensor[cSaveDataIndex];
					}
					
					/* ����ʪ��ͨ��Ϊ���ߺ����� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
					Wireless_RF_Card_Data_Rece_Timeout_Value;

					/* ����ͨ������ */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 1;
					/* ����̽ͷ���� */
					cProbeIndex++;
					
					break;
				}
				
				/* ����ͨ��ʪ��Ƭʪ��ͨ�����߿�Ƭ */
				case Sensor_RF_Card_Humiture_Hum:
				{
					/* ƥ���ͨ�� */
					uint8_t cMatchingCH;
					
					
					/* �����뵱ǰSN��һ�µ�ͨ�� */
					cMatchingCH = ComProtocolZDE_SearchMatchingSN(&protocolParaReg->MessageBody.Frame,
					((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO);
					
					/* �ж�������� */
					if(ComProtocolZDE_MatchingCHNull != cMatchingCH)
					{
						/* ��ƥ���ͨ�� */
						/* ���ò���,�ж������Ƿ�Ϊ���ߺ����� */
						if(Wireless_RF_Card_Data_Rece_Timeout_Para == dataFrame->Sensor[cSaveDataIndex])
						{
							/* ���ź����� */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
							Wireless_RF_Card_Data_Rece_Timeout_Value;
						}else
						{
							/* �������� */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = dataFrame->Sensor[cSaveDataIndex];
						}
						/* ����ͨ������ */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 2;
					}
					break;
				}
				
				/* �Ͼ�ʢ����ʪ��Ƭ�¶�ͨ�����߿�Ƭ */
				case Sensor_GW_NJSY_Tmp:
				{
					/* ����SN�� */
					memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
						   ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO,
						   sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
					
					/* ̽ͷ���� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x02;
					
					/* ��ص��� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level = 
					((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorChDataP+num))->Sensor_Power_State;
					
					/* ���ò���,�ж������Ƿ�Ϊ���ߺ����� */
					if(Gateway_NJSY_TimeoutPara == dataFrame->Sensor[cSaveDataIndex])
					{
						/* ���ź����� */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = 
						Gateway_NJSY_TimeoutPara;
					}else
					{
						/* �������� */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = dataFrame->Sensor[cSaveDataIndex];
					}
					
					/* ����ʪ��ͨ��Ϊ���ߺ����� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
					Gateway_NJSY_TimeoutPara;

					/* ����ͨ������ */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 1;
					/* ����̽ͷ���� */
					cProbeIndex++;
					break;
				}
				
				/* �Ͼ�ʢ����ʪ��Ƭʪ��ͨ�����߿�Ƭ */
				case Sensor_GW_NJSY_Hum:
				{
					/* ƥ���ͨ�� */
					uint8_t cMatchingCH;
					
					
					/* �����뵱ǰSN��һ�µ�ͨ�� */
					cMatchingCH = ComProtocolZDE_SearchMatchingSN(&protocolParaReg->MessageBody.Frame,
					((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO);
					
					/* �ж�������� */
					if(ComProtocolZDE_MatchingCHNull != cMatchingCH)
					{
						/* ��ƥ���ͨ�� */
						/* ���ò���,�ж������Ƿ�Ϊ���ߺ����� */
						if(Gateway_NJSY_TimeoutPara == dataFrame->Sensor[cSaveDataIndex])
						{
							/* ���ź����� */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
							Gateway_NJSY_TimeoutPara;
						}else
						{
							/* �������� */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = dataFrame->Sensor[cSaveDataIndex];
						}
						/* ����ͨ������ */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 2;
					}

					break;
				}
				
				/* ���ݴ�����ʪ��Ƭ�¶�ͨ�����߿�Ƭ */
				case Sensor_GW_HZZH_Tmp:
				{
					/* ����SN�� */
					memcpy(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN,
						   ((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO,
						   sizeof(protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].SN));
					
					/* ̽ͷ���� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Type = 0x02;
					
					/* ��ص��� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Level = 
					((INSTRU_SENSOR_CHANNEL_FIX*)(cSensorChDataP+num))->Sensor_Power_State;
					
					/* ���ò���,�ж������Ƿ�Ϊ���ߺ����� */
					if(Gateway_HZZH_TimeoutPara == dataFrame->Sensor[cSaveDataIndex])
					{
						/* ���ź����� */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = 
						Gateway_HZZH_TimeoutPara;
					}else
					{
						/* �������� */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[0] = dataFrame->Sensor[cSaveDataIndex];
					}
					
					/* ����ʪ��ͨ��Ϊ���ߺ����� */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
					Gateway_HZZH_TimeoutPara;

					/* ����ͨ������ */
					protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 1;
					/* ����̽ͷ���� */
					cProbeIndex++;
					break;
				}
				
				/* ���ݴ�����ʪ��Ƭʪ��ͨ�����߿�Ƭ */
				case Sensor_GW_HZZH_Hum:
				{
					/* ƥ���ͨ�� */
					uint8_t cMatchingCH;
					
					
					/* �����뵱ǰSN��һ�µ�ͨ�� */
					cMatchingCH = ComProtocolZDE_SearchMatchingSN(&protocolParaReg->MessageBody.Frame,
					((INSTRU_SENSOR_CHANNEL*)(cSensorchP+num))->SENSOR_SN_NO);
					
					/* �ж�������� */
					if(ComProtocolZDE_MatchingCHNull != cMatchingCH)
					{
						/* ��ƥ���ͨ�� */
						/* ���ò���,�ж������Ƿ�Ϊ���ߺ����� */
						if(Gateway_HZZH_TimeoutPara == dataFrame->Sensor[cSaveDataIndex])
						{
							/* ���ź����� */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = 
							Gateway_HZZH_TimeoutPara;
						}else
						{
							/* �������� */
							protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].Data[1] = dataFrame->Sensor[cSaveDataIndex];
						}
						/* ����ͨ������ */
						protocolParaReg->MessageBody.Frame.ProbeInfo[cProbeIndex].ChannelNum = 2;
					}
					break;
				}
				
				default:
				{
					/* ��GPS����ͨ�� */
					__nop();
				}
			}
		}
	}
	
	/* �ж�̽ͷ���� */
	if((0 == cProbeIndex)&&(0x00 == GPS_Config_Enable))
	{
		/* û��̽ͷ��Ϣ��Ҫ���� */
			return FALSE;
	}
	
	/* ����̽ͷ���� */
	protocolParaReg->MessageBody.Frame.ProbeNum = cProbeIndex;
	
	return TRUE;
}

/************************************************************************************************/
/* ������	: ComProtocolZDE_FrameData										 		 			*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ����Э����������һ������������֡��					 								*/
/* ��������	: 2017/02/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void ComProtocolZDE_FrameData(ComProtocolZDEParaStruct* protocolCR,ComProtocolZDE_MBF* protocolFrame)
{
	/* ������ */
	uint8_t cNum;
	/* ̽ͷSN���ݴ�Ĵ��� */
	uint8_t SN_f[4];

	/* �ɼ�ʱ�� */
	protocolFrame->CollectionTime[0] = protocolCR->MessageBody.Frame.CollectionTime[0];
	protocolFrame->CollectionTime[1] = protocolCR->MessageBody.Frame.CollectionTime[1];
	protocolFrame->CollectionTime[2] = protocolCR->MessageBody.Frame.CollectionTime[2];
	protocolFrame->CollectionTime[3] = protocolCR->MessageBody.Frame.CollectionTime[3];
	protocolFrame->CollectionTime[4] = protocolCR->MessageBody.Frame.CollectionTime[4];
	protocolFrame->CollectionTime[5] = protocolCR->MessageBody.Frame.CollectionTime[5];
	
	/* ̽ͷ���� */	
	protocolFrame->ProbeNum =  protocolCR->MessageBody.Frame.ProbeNum;
	
	/* �����ź�ǿ�� */
	protocolFrame->GSMSignal = GSMGetSignalStrength();
	
	protocolFrame->PowerLinkState =(uint8_t)PowerLinkState;
	protocolFrame->PowerLevelState=PowerLevelState;
	/* ����1 */
	protocolFrame->Reserved1[0] =  0x00;
	protocolFrame->Reserved1[1] =  0x00;
	/* ���� */
	protocolFrame->Lon = protocolCR->MessageBody.Frame.Lon;

	/* γ�� */
	protocolFrame->Lat = protocolCR->MessageBody.Frame.Lat;

	/* �ٶ� */
	protocolFrame->Speed = protocolCR->MessageBody.Frame.Speed;
	
	/* ����2 */	
	protocolFrame->Reserved2[0] =  0x00;
	protocolFrame->Reserved2[1] =  0x00;
	protocolFrame->Reserved2[2] =  0x00;
	protocolFrame->Reserved2[3] =  0x00;
	
	//printf("protocolFrame->ProbeNum=%d\r\n",protocolFrame->ProbeNum);
	/* ̽ͷ��Ϣ */
	for(cNum = 0;cNum < protocolFrame->ProbeNum; cNum++)
	{
		ComProtocolZDE_MBFI* cMBFIP = &protocolFrame->ProbeInfo+cNum;
		/* ����SN�� */
		
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
		
		/* ̽ͷ���� */
		cMBFIP->Level = 
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].Level;
		
		/* ̽ͷ���� */
		cMBFIP->Type = 
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].Type;
		
		/* ͨ������ */
		cMBFIP->ChannelNum = 
		//printf("ChannelNum=%d\r\n",protocolCR->MessageBody.Frame.ProbeInfo[cNum].ChannelNum);
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].ChannelNum;
		/* ���� */	
		protocolCR->MessageBody.Frame.ProbeInfo[cNum].Reserved =  0x00;
		/* ���� */
		memcpy((float*)(&cMBFIP->Data),
			protocolCR->MessageBody.Frame.ProbeInfo[cNum].Data,
			sizeof(float)*cMBFIP->ChannelNum);

	}
}

#endif


#if (1) //���ܺ����ӿ�
/************************************************************************************************/
/* ������	: ComProtZDE_CreateSendData										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ����ƽ̨Ӧ���Ƿ�ɹ�																*/
/* ����	 	: ���ݡ����ͨ��Э�顱ͨ��Э�飬�������յ������ݡ�					 					*/
/* ��������	: 2017/02/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
BOOL ComProtZDE_ParseData(uint8_t* destBuf,int* dataLen)
{
	/* У���� */
	uint8_t cCheckCode;
	/* ������ */
	uint8_t num;
	/* ���������ص����ݳ��� */
	uint16_t cDataLen;
	/* Ӧ�����ݽṹ */
	ComProtocolZDEACKStruct* cACKDataP;
	
	
	//1������ת������
	/* �ȶ� �������� ��������ת�� */
	cDataLen = GPRS_Network_To_Host_Translator_Data(ComProtocolZDEApp.DR.RXDR,
													destBuf,*dataLen);
	
	//2����������
	/* �жϱ�ʶλ���� */
	if((ComProtocolZDEApp.DR.RXDR[0] != (u8)ComProtocolZDE_StartIdentify)||
	   (ComProtocolZDEApp.DR.RXDR[cDataLen-1] != (u8)ComProtocolZDE_EndIdentify))
	{
		/* ͷ��ʶ��β��ʶ��Э�鲻һ�� */
		return FALSE;
	}
	
	/* ��������ָ�� */
	cACKDataP = (ComProtocolZDEACKStruct*)ComProtocolZDEApp.DR.RXDR;
	
	//printf("cACKDataP->ID1=%04X\r\n",cACKDataP->ID1);
	//printf("cACKDataP->ID2=%04X\r\n",cACKDataP->ID2);
	/* �ж���Ϣͷ ��ϢID���� */
	//if(cACKDataP->ID != ComProtocolZDE_ACKMessageID)
	if((((u16)cACKDataP->ID2<<8)| (u16)cACKDataP->ID1)!= (u16)ComProtocolZDE_ACKMessageID)
	{
		/* ��Ϣͷ��ϢID��Э�鲻һ�� */		
		return FALSE;
	}

	
	/* �ж���ϢӦ���� */
	if(0x00 != cACKDataP->Result)
	{
		/* ��ϢӦ���쳣 */
		return FALSE;
	}
	
	/* ��� У�� ���ݼĴ��� */
	cCheckCode = 0;
	/* �ж�����У�� */
	/* ���� ����ת�������������ݵ�У��  */
	for(num = 1;num < (cDataLen-2); num++)
	{
		/* �����У�鷽ʽ:����Ч�����ݶ�ȡ��� */
		cCheckCode ^= ComProtocolZDEApp.DR.RXDR[num];
	}
	
	if(cACKDataP->CheckCode != cCheckCode)
	{
		/* ��Ϣ����У���쳣 */
		return FALSE;
	}
	
	/* ƽ̨�������ݳɹ� */
	return TRUE;
}

/************************************************************************************************/
/* ������	: ComProtocolZDE_CreateData										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ���ݡ����ͨ��Э�顱ͨ��Э�飬�������ݡ�					 							*/
/* ��������	: 2017/02/16																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
BOOL ComProtocolZDE_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,
							   uint8_t* destBuf,int* dataLen)
{
	/* Э������Ĵ��� */
	ComProtocolZDEStruct* cProtocolReg = (ComProtocolZDEStruct*)ComProtocolZDEApp.DR.TXDR;
	/* Ӧ�ô洢���� */
	AppSaveFrameStruct cSaveFrameReg;
	/* �洢����״̬ */
	Save_ReadState SaveDataState;
	/* ͨ��Э������Ĵ��� */
	ComProtocolZDEParaStruct cProtocolParaReg = {0};
	/* �������� */
	ComProtocolZDE_MBF* cDataIndex = (ComProtocolZDE_MBF*)(&cProtocolReg->MessageBody.FrameData);
	/* �����ݳ��� */
	uint16_t cPacketLen = 0;
	/* ������ */
	uint16_t cNum;
	
	
	/* ������ǰ�洢����״̬ */
	SaveDataState = AppSaveAPI.ReadSaveDataFrame(&cSaveFrameReg);

	/* �ж϶����洢����״̬ */
	if(Save_ReadFail == SaveDataState)
	{
		/* û�д洢���� */
		return FALSE;
	}
	
	/* ����������ݼĴ��� */
	memset(ComProtocolZDEApp.DR.TXDR,0x00,sizeof(ComProtocolZDEApp.DR.TXDR));
	
	/* ������Ϣͷ���� */
	ComProtocolZDE_SetMessageHeaderPara(config_tab,cProtocolReg);
	
	/* �����ϴ����� */
	for(cProtocolParaReg.MessageBody.FrameNum = 0;
	   ((((uint8_t*)cDataIndex)+ComProtocolZDE_FrameDataLen) < 
	   &ComProtocolZDEApp.DR.TXDR[ComProtocolZDE_TXSize-1]);
	   )
	{
		/* ��������Ҫ�ϴ�,��ȡ�洢���ݡ� */
		/* ����Э�����״̬ */
		BOOL cSetProtocolState;
		/* ͨ���� */
		uint8_t cChannelNum = 0;
		
		
		/* ���ݴ洢������Ϣ������ͨ��Э����Ϣ����� */
		cSetProtocolState = ComProtocolZDE_SetMessageBodyPara(&cSaveFrameReg,&cProtocolParaReg,config_tab,fix_tab);
		
		/* �жϲ�������״̬ */
		if(FALSE == cSetProtocolState)
		{
			/* û����Ϣ��Ҫ�ϴ� */
			return FALSE;
		}
		
		/* ����֡���� */
		cProtocolReg->MessageBody.FrameNum++;
		
		/* ����ͨ��Э���������װ��������֡ */
		ComProtocolZDE_FrameData(&cProtocolParaReg,cDataIndex);
		
		/* ����ͨ��������Ҫ��Ϊ�˼���ÿ��̽ͷ����ĵ�ַ�� */
		for(cNum = 0,cChannelNum = 0;cNum < cProtocolParaReg.MessageBody.Frame.ProbeNum; cNum++)
		{
			cChannelNum += cProtocolParaReg.MessageBody.Frame.ProbeInfo[cNum].ChannelNum-1;
		}
		
		/* ������������λ�� */
		cDataIndex = (ComProtocolZDE_MBF*)
		(((uint8_t*)(&cDataIndex->ProbeInfo+cProtocolParaReg.MessageBody.Frame.ProbeNum))+
		(cChannelNum)*sizeof(float));
		
		/* ������ǰ�洢����״̬ */
		SaveDataState = AppSaveAPI.ReadSaveDataFrame(&cSaveFrameReg);
		
		/* �ж϶��洢����״̬ */
		if(Save_ReadFail == SaveDataState)
		{
			/* �޴洢�������������� */
			break;
		}
	}
	
	/* ����������� */
	cPacketLen = ((uint8_t*)cDataIndex) - 
	ComProtocolZDEApp.DR.TXDR-1;
	
	ComProtocolZDEApp.DR.TXDR[2] = (cPacketLen+1-20)%255;
	ComProtocolZDEApp.DR.TXDR[3] = (cPacketLen+1-20)/255;
	
	
	//printf("cPacketLen=%d\r\n",cPacketLen);
	/* ����У���� */
	for(cNum = 0,ComProtocolZDEApp.DR.TXDR[cPacketLen+1] = 0;cNum < cPacketLen+1; cNum++)
	{
		/* ����У��ֵ */
		ComProtocolZDEApp.DR.TXDR[cPacketLen+1] ^= ComProtocolZDEApp.DR.TXDR[cNum];
	}
	
	/* ת������֡ */
	*dataLen = 
	GPRS_Host_To_Network_Translator_Data((destBuf+1),
	ComProtocolZDEApp.DR.TXDR,
	cPacketLen+2);
	
	/* ���ý�����ʶ�� */
	*destBuf = ComProtocolZDE_StartIdentify;
	*(destBuf+*dataLen+1) = ComProtocolZDE_EndIdentify;
	*dataLen += 2;
	
	return TRUE;
}

#endif


#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

