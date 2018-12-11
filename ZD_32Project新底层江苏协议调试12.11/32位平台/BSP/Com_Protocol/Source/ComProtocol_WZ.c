/************************************************************************************************/
/** @file              : ComProtocol_WZ.c														*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��12��21��															*/
/** @brief             : c file																	*/
/** @description       : �����������--������--ͨ��Э��--����StringЭ�� Ӧ���ļ�					*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**�ļ���     : ComProtocol_WZ.c																	*/
/**����       : ��������StringЭ���ṩ��ͨ��Э���ʽ��̽ͷ�����ϴ�����ƽ̨									*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2017��02��13��   	 ��ʼ�汾												*/
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
/* �ĵ��¶�̽ͷ�������� */
#define	ComProtocol_TempProbeNumMax			(4u)
/* �ĵ�ʪ��̽ͷ�������� */
#define	ComProtocol_HumProbeNumMax			(1u)

/* �豸̽ͷ������ */
#define	ComProtocol_ProbeTotalNum		(Sensor_Tmp_Num+\
										 Sensor_Hum_Num+\
										 Sensor_RF_Single_Tmp_Num+\
										 Sensor_RF_Humiture_Num+\
										 Sensor_PT1000_Num+\
										 Sensor_GW_NJSY_Num)

/* �ж�̽ͷ�����Ƿ����Э���趨���̽ͷ���� */
#if (ComProtocol_TempProbeNumMax < Sensor_Tmp_Num +\
								   Sensor_RF_Single_Tmp_Num +\
								   Sensor_PT1000_Num +\
							       Sensor_GW_NJSY_Tmp_Num)
#error "̽ͷ��������Э���趨���̽ͷ����4��"
#endif

/* �ж�̽ͷ�����Ƿ����Э���趨���̽ͷ���� */
#if (ComProtocol_HumProbeNumMax < Sensor_Hum_Num +\
								  Sensor_RF_Humiture_Hum_Num +\
								  Sensor_GW_NJSY_Hum_Num +\
								  Sensor_GW_HZZH_Hum_Num)
#error "̽ͷ��������Э���趨���̽ͷ����1��"
#endif

/********************************* �궨�� *********************************/
/* ͨ��Э�� ���͵�������ݳ��� */
#define ComProtocol_MaxDataLen				(1024ul)


/* ͨ��Э�� ���̱��� */
#define ComProtocol_VendorCode				(3170)

/* ͨ��Э�� ������ */
#define ComProtocol_Cmd						("GPSDU")
/* ͨ��Э�� Ҫ��Ӧ���ʶ */
#define ComProtocol_ACKFlag					("res")



/* ͨ��Э�� �豸����ģʽ:1/���ƺš�2/�豸�Ż�sim����(�Ƽ�) */
#define ComProtocol_PlateNoMode				(1u)
#define ComProtocol_SN_SimMode				(2u)

/* ��GPS��λ���ݲ��� */
#define ComProtocol_NullGPSSignalPara		(0.0)

/* ��̽ͷ�������ݲ��� */
#define ComProtocol_NullProbePara			(-9000)

/* Э���̽ͷ����(����Э�齫̽ͷ��Ϊ2������:1�¶ȣ�2ʪ��) */
#define ComProtocol_ProbeTempType			(1u)
#define ComProtocol_ProbeHumType			(2u)


/*******************************************************************************/
/********************************* ���ݽṹ *************************************/
/* ����StringЭ��Э�� ���ݽṹ */
typedef struct
{
	/* ʵʱ����ģʽ */
	uint8_t RTDMode;
	/* ���ƺ� */
	uint8_t PlateNo[27];
	/* �ɼ�ʱ�� */
	Times CollectTime;
	/* ���� */
	float Lon;
	/* γ�� */
	float Lat;
	/* �ٶ� */
	float Speed;
	/* ���� */
	float Cog;
	
	/* ״̬�Ĵ��� */
	struct
	{
		/* ACC����״̬ */
		BOOL PowerLink;
		/* GPS��λ״̬ */
		BOOL GPS;
		
		/* ̽ͷ���� */
		uint8_t ProbeNum;
		/* ̽ͷ���� */
		uint8_t ProbeType[5];
		
		/* �¶� */
		float Temp[4];
		/* �¶� ����״̬ */
		uint8_t TempAlarm[4];
		
		/* ʪ�� */
		float Hum;
		/* ʪ�� ����״̬ */
		uint8_t HumAlarm;
		
		/* ��ص��� */
		uint8_t BatteryValue;
		
//		/* �Ŵ� */
//		BOOL Door;
//		/* Gps���Ƕ�λ�� */
//		uint8_t gpsnum;
//		/* Gsm��λ�� */
//		uint8_t gsmnum;
//		/* �ն������ */
//		uint32_t distance;
	}State;
	
	/* �����Ĵ��� */
	struct
	{
		/* �ն�����Դ���� */
		uint32_t PowerDown:1;
		
	}Alarm;
	
}ComProtocolWZCRStruct;

/********************************* ���ݼĴ��� *******************************/
/* ����StringЭ��ͨ��Э��Ӧ������ */
ComProtocolWZDRStruct ComProtocolWZDR = 
{
	/* ���üĴ��� */

	/* ����StringЭ������ӿ� */
	/* ���� */
	'g' ,'p' ,'s' ,'t' ,	'x' ,'h' ,'h' ,'.' ,	'h' ,'u' ,'o' ,'y' ,
	'u' ,'n' ,'r' ,'e' ,	'n' ,'.' ,'c' ,'o' ,	'm' ,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,	0X00,
	/* IP */
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,
	/* �˿� */
	'9' ,'0' ,'3' ,0X00, 	0X00,0X00,0X00,0X00,	0X00,
		
	
	/* ״̬�Ĵ��� */
	/* �������ݱ�־ */
	FALSE,

	
	/* ���ݼĴ��� */
	0X00,
};


/********************************* �ļ��������� *********************************/


/*******************************************************************************/
/********************************* �ļ��������� *********************************/


/************************************************************************************************/
#if (1) //�ļ�����
/************************************************************************************************/
/* ������	: ComProtocolWZ_SetPara										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ����Э�����					 													*/
/* ��������	: 2016/12/22																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static BOOL ComProtocolWZ_SetPara(AppSaveFrameStruct* dataFrame,
								  ComProtocolWZCRStruct* protocolReg)
{
	/* ������ */ 
	uint8_t num;
	/* ���ƺ�ָ�� */
	char* cPlateNo = Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO;
	/* ������ͨ��ָ�� */
	INSTRU_SENSOR_CHANNEL* cSensorChp = &Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State.CH1;
	/* ���ص�ص�������ָ�� */
	uint8_t* cBatteryLevelp = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	/* ��Դ����״̬ */
	BOOL* cPowerLinkState = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Link_State;
	
	printf("ComProtocolWZ_SetPara\r\n");
	
	/* ��ȡ��ǰ̽ͷ���� */
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
	
	/* ����ʵʱ����ģʽ:�̶�Ϊ���ƺ� */
	ComProtocol_SetPlateNo(protocolReg->PlateNo,cPlateNo);
	
	/* ���òɼ�ʱ�� */
	protocolReg->CollectTime = dataFrame->Timestamp;

	/* ����״̬�Ĵ��� */
	/* ACC״̬ */
	if(TRUE == (*cPowerLinkState))
	{
		/* ��ǰ��Դ������ */
		protocolReg->State.PowerLink = TRUE;
	}else
	{
		/* ��ǰ��Դδ���� */
		protocolReg->State.PowerLink = FALSE;
	}
	
	/* ����̽ͷ���� */
	for(num = 0;num < Sensor_CH_Config_Num;num++)
	{
		/* �ж�̽ͷ���� */
		if((Sensor_Tmp 					== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_PT1000 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_RF_Card_Single_Tmp 	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_RF_Card_Humiture_Tmp == ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
		   (Sensor_GW_NJSY_Tmp 		  	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type))
		{

			/* �¶�̽ͷ */
			/* ����̽ͷ���� */
			protocolReg->State.ProbeType[num] = ComProtocol_ProbeTempType;
			/* �������� */
			protocolReg->State.Temp[num] = dataFrame->Sensor[num];
			/* �жϱ���״̬ */
			if(protocolReg->State.Temp[num] > (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_Up_Limit))
			{
				/* ���±��� */
				protocolReg->State.TempAlarm[num] = 2;
				
			}else if(protocolReg->State.Temp[num] < (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_Low_Limit))
			{
				/* ���±��� */
				protocolReg->State.TempAlarm[num] = 4;
			}else
			{
				/* ���� */
				protocolReg->State.TempAlarm[num] = 0;
			}
		}else if((Sensor_Hum 					== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
				 (Sensor_RF_Card_Humiture_Hum	== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
				 (Sensor_GW_NJSY_Hum	 		== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)||
				 (Sensor_GW_HZZH_Hum 			== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type))
		{
			/* ʪ��̽ͷ */
			/* ����̽ͷ���� */
			protocolReg->State.ProbeType[num] = ComProtocol_ProbeHumType;
			/* �������� */
			protocolReg->State.Hum = dataFrame->Sensor[num];
			/* �жϱ���״̬ */
			if(protocolReg->State.Hum > (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_Up_Limit))
			{
				/* ��ʪ���� */
				protocolReg->State.HumAlarm = 2;
				
			}else if(protocolReg->State.Hum < (((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->SENSOR_Low_Limit))
			{
				/* ��ʪ���� */
				protocolReg->State.HumAlarm = 4;
			}else
			{
				/* ���� */
				protocolReg->State.HumAlarm = 0;
			}
		}else if(Sensor_GPS_POS_Lon 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* ����̽ͷ */
			/* ���þ��Ȳ��� */
			if(dataFrame->Sensor[num] != GPS_Data_Rece_Timeout_Value)
			{
				/* �Ѷ�λ���� */
				protocolReg->Lon = dataFrame->Sensor[num];
			}else
			{
				/* δ��λ���� */
				protocolReg->Lon = ComProtocol_NullGPSSignalPara;
			}
		}else if(Sensor_GPS_POS_Lat 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* γ��̽ͷ */
			/* ���þ��Ȳ��� */
			if(dataFrame->Sensor[num] != GPS_Data_Rece_Timeout_Value)
			{
				/* �Ѷ�λ���� */
				protocolReg->Lat = dataFrame->Sensor[num];
			}else
			{
				/* δ��λ���� */
				protocolReg->Lat = ComProtocol_NullGPSSignalPara;
			}
		}else if(Sensor_GPS_POS_Spd 				== ((INSTRU_SENSOR_CHANNEL*)(cSensorChp+num))->Type)
		{
			/* �ٶ�̽ͷ */
			/* ���þ��Ȳ��� */
			if(dataFrame->Sensor[num] != GPS_Data_Rece_Timeout_Value)
			{
				/* �Ѷ�λ���� */
				protocolReg->Speed = dataFrame->Sensor[num];
			}else
			{
				/* δ��λ���� */
				protocolReg->Speed = ComProtocol_NullGPSSignalPara;
			}
		}else
		{
			continue;
		}

	}
	
	/* �ж�GPS��λ�����Ƿ����ź� */
	if((0 == protocolReg->Lon) &&
	   (0 == protocolReg->Lat) &&
	   (0 == protocolReg->Speed))
	{
		/* ���ź� */
		/* ����GPS״̬Ϊ���ź�0 */
		protocolReg->State.GPS = FALSE;
	}else
	{
		/* ���ź� */
		/* ����GPS״̬ */
		protocolReg->State.GPS = TRUE;
	}
	
	/* ���÷������Ϊ0 */
	protocolReg->Cog = 0;
	
	/* ���õ��� */
	protocolReg->State.BatteryValue = *cBatteryLevelp;
	
	/* �жϵ�ǰ��������״̬ */
	if(protocolReg->State.ProbeNum > 0)
	{
		/* �����ɹ� */
		return TRUE;
	}
	
	/* �����ɹ� */
	return FALSE;
}

/************************************************************************************************/
/* ������	: ComProtocolWZ_FrameData										 		 			*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ����Э����������һ������������֡��					 								*/
/* ��������	: 2016/12/22																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
static void ComProtocolWZ_FrameData(ComProtocolWZCRStruct* protocolCR,uint8_t* cDataBuf)
{
	/* ��λʱ���ַ����Ĵ��� */
	char cGPSTimeString[16]	={'\0'};
	
	char cGPSLonString[16]	={'\0'};
	char cGPSLatString[16]	={'\0'};
	char cGPSSpeedString[16]={'\0'};
	char cGPSCogString[16]	={'\0'};
	
	char cProbeDataString[5][16] ={'\0'};
	
	char cProbeString[60]	={'\0'};
	
	/* ������ */
	uint8_t num;
	/* �ַ������ȼ����� */
	uint8_t cStringLen;
	/* ʪ��̽ͷ��ǩ */
	uint8_t cProbeHumNum = 0;
	
	
	/* ���ݶ�λʱ�����ö�λʱ���ַ��� */
	sprintf(cGPSTimeString,"%04d%02d%02d%02d%02d%02d",
			protocolCR->CollectTime.Year,
			protocolCR->CollectTime.Month,
			protocolCR->CollectTime.Day,
			protocolCR->CollectTime.Hour,
			protocolCR->CollectTime.Min,
			protocolCR->CollectTime.Sec);
	
	/* ����GPS���� */
	float_to_string(protocolCR->Lon,cGPSLonString);
	float_to_string(protocolCR->Lat,cGPSLatString);
	float_to_string(protocolCR->Speed,cGPSSpeedString);
	float_to_string(protocolCR->Cog,cGPSCogString);
	
	/* ����̽ͷ�����ַ��� */
	for(num = 0,cStringLen = 0;num < protocolCR->State.ProbeNum;num++)
	{
		/* �����Ĵ��� */
		char cProbeAlarmString[12] = {'\0'};
		
		/* �жϵ�ǰ̽ͷ���� */
		if(ComProtocol_ProbeTempType == protocolCR->State.ProbeType[num])
		{
			/* �¶�̽ͷ */
			/* ����̽ͷ���� */
			float_to_string(protocolCR->State.Temp[num],cProbeDataString[num]);
			/* ���ñ���״̬�� */
			if((0 != protocolCR->State.TempAlarm[num])&&
			   (TRUE == protocolCR->State.PowerLink))
			{
				/* ����״̬����ֻ�����ַ��� */
				sprintf(cProbeAlarmString,"ta%d:%d,",(1 + num - cProbeHumNum),protocolCR->State.TempAlarm[num]);
			}
			/* �¶�̽ͷ */
			sprintf((cProbeString+cStringLen),"t%d:%s,%s",
					 (1 + num - cProbeHumNum),cProbeDataString[num],cProbeAlarmString);
		}else if(ComProtocol_ProbeHumType == protocolCR->State.ProbeType[num])
		{
			/* ʪ��̽ͷ */
			/* ����̽ͷ���� */
			float_to_string(protocolCR->State.Hum,cProbeDataString[num]);
			/* ���ñ���״̬�� */
			if((0 != protocolCR->State.HumAlarm)&&
			   (TRUE == protocolCR->State.PowerLink))
			{
				/* ����״̬����ֻ�����ַ��� */
				sprintf(cProbeAlarmString,"humia:%d,",protocolCR->State.HumAlarm);
			}
			/* �¶�̽ͷ */
			sprintf((cProbeString+cStringLen),"humi:%s,%s",cProbeDataString[num],cProbeAlarmString);
			/* ����ʪ��ͨ������ */
			cProbeHumNum++;
		}else
		{
			/* δ�������� */
			__nop();
		}
		
		/* ���㵱ǰ�ַ������� */
		cStringLen = strlen(cProbeString);
	}
	
	/* �����úõ����ݿ��������ݼĴ����� */
	sprintf((char*)cDataBuf,"~%d&%s&%d|%s|%s|%s|%s|%s|%s|{acc:%d,gps:%d,%sbattery:%d}|%d|%s#",
			ComProtocol_VendorCode,//���̱��
			ComProtocol_Cmd,//������
			protocolCR->RTDMode,//ģʽ������̶�Ϊ���ƺ�ģʽ��
			protocolCR->PlateNo,//���ƺ�
			cGPSTimeString,//��λʱ��
			cGPSLonString,//����
			cGPSLatString,//γ��
			cGPSSpeedString,//�ٶ�
			cGPSCogString,//����
			((protocolCR->State.PowerLink == TRUE)?1:0),//��Դ����״̬
			((protocolCR->State.GPS == TRUE)?1:0),//GPS��λ״̬
			cProbeString,//̽ͷ�����ַ���
			protocolCR->State.BatteryValue,//���ص�ص���
			(protocolCR->State.PowerLink ==1 )?0:1,//�ն�����Դ���籨��״̬
			ComProtocol_ACKFlag//��ϢӦ���ʶ
			);
}

#endif

#if (1) //���ܺ����ӿ�
/************************************************************************************************/
/* ������	: ComProtWZ_CreateSendData										 	 				*/
/* ����  	: ��			 																	*/
/* ���		: ����ƽ̨Ӧ���Ƿ�ɹ�																*/
/* ����	 	: ���ݡ�����StringЭ�顱ͨ��Э�飬�������յ������ݡ�					 				*/
/* ��������	: 2016/12/22																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
BOOL ComProtWZ_ParseData(uint8_t* receBuf)
{
//	/* ����ָ�� */
//	char * datap;
//	
//	
//	/* ��ѯ�ϴ�̽ͷ���������ַ��� */
//	datap = strstr((char*)receBuf,ComProtocol_ProbeCmd);
//	/* �ж�HTTP�����Ƿ�ɹ� */
//	if(NULL == datap)
//	{
//		/* ��ѯ�ϴ�GPS���������ַ��� */
//		datap = strstr((char*)receBuf,ComProtocol_GPSCmd);
//		if(NULL == datap)
//		{
//			/* ����ʧ�� */
//			return FALSE;
//		}
//	}
//	
//	/* ��ѯ�ϴ�̽ͷID�ַ��� */
//	datap = strstr((char*)receBuf,ComProtocol_ProbeDataID);
//	/* �ж�HTTP�����Ƿ�ɹ� */
//	if(NULL == datap)
//	{
//		/* ��ѯ�ϴ�GPS ID�ַ��� */
//		datap = strstr((char*)receBuf,ComProtocol_GPSDataID);
//		if(NULL == datap)
//		{
//			/* ����ʧ�� */
//			return FALSE;
//		}
//	}
	
	/* ƽ̨�������ݳɹ� */
	return TRUE;
}

/************************************************************************************************/
/* ������	: ComProtWZ_CreateData										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: ��																				*/
/* ����	 	: ���ݡ�����StringЭ�顱ͨ��Э�飬�������ݡ�					 								*/
/* ��������	: 2016/12/22																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
BOOL ComProtWZ_CreateData(Save_Sensor_Data_Struct* cSaveDatap)
{
	/* ���ò���״̬ */
	BOOL SetProtState;
	/* Э������Ĵ��� */
	ComProtocolWZCRStruct cProtocolParaReg = {0};
	/* Ӧ�ô洢���� */
	AppSaveFrameStruct cSaveFrameReg;
	
	
	/* ���ÿղ���ֵ */
	cProtocolParaReg.State.Temp[0] = ComProtocol_NullProbePara;
	cProtocolParaReg.State.Temp[1] = ComProtocol_NullProbePara;
	cProtocolParaReg.State.Temp[2] = ComProtocol_NullProbePara;
	cProtocolParaReg.State.Temp[3] = ComProtocol_NullProbePara;
	cProtocolParaReg.State.Hum	   = ComProtocol_NullProbePara;
	
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
	SetProtState = ComProtocolWZ_SetPara(&cSaveFrameReg,&cProtocolParaReg);
	
	/* �ж����ò���״̬ */
	if(FALSE == SetProtState)
	{
		/* û����Ҫ���͵Ĳ��������������� */
		return FALSE;
	}
	
	/* ����������ݼĴ��� */
	memset(ComProtocolWZDR.DR.TXDR,0x00,sizeof(ComProtocolWZDR.DR.TXDR));
	
	/* ����ͨ��Э���������װ��������֡ */
	ComProtocolWZ_FrameData(&cProtocolParaReg,ComProtocolWZDR.DR.TXDR);

	return TRUE;
}

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
