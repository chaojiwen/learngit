/************************************************************************************************/
/** @file              : ComProtocol_WZ.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017��02��13��															*/
/** @brief             : c file																	*/
/** @description       : �����������--������--ͨ��Э��--����StringЭ�� Ӧ���ļ�					*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2017��02��13��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __ComProtocol_WZ_h
#define	__ComProtocol_WZ_h
#include "Log_GSM.h"
#if (ComProtocol_WZ_Enable == 0x01)
#include "App_Save.h"
#include "interface.h"

/********************************* API�ṹ ******************************/


/********************************* ����StringЭ��ͨ��Э��Ӧ�����ݽṹ **************/
typedef struct
{
	/* ���üĴ��� */
	struct
	{
		/* ����StringЭ������ӿ� */
		const GPRS_Connection NetworkConnection;
		
		
	}CR;
	
	/* ״̬�Ĵ��� */
	struct
	{
		/* �������ݱ�־ */
		BOOL CreateDataFlag;
	
	}SR;
	
	/* ���ݼĴ��� */
	struct
	{
		/* ����StringЭ�����ݼĴ��� */
		uint8_t TXDR[1024ul];
		uint8_t RXDR[128ul];
	}DR;
	
}ComProtocolWZDRStruct;

/********************************* ȫ�ֱ������� *****************************/
/* ����StringЭ�����ݼĴ�����С */
#define ProbeDataRegSize			(1024ul)
/* ����StringЭ�����ݼĴ��� */
extern ComProtocolWZDRStruct		ComProtocolWZDR;

/********************************* �������� *********************************/
BOOL ComProtWZ_CreateData(Save_Sensor_Data_Struct* cSaveFrameReg);
BOOL ComProtWZ_ParseData(uint8_t* receBuf);


#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
