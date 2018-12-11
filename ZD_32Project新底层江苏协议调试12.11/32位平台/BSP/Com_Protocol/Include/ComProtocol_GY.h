/************************************************************************************************/
/** @file              : ComProtocol_GY.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��12��22��															*/
/** @brief             : c file																	*/
/** @description       : �����������--������--ͨ��Э��--��ҩ Ӧ���ļ�								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��12��22��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __ComProtocol_GY_h
#define	__ComProtocol_GY_h
#include "Log_GSM.h"
#if (AppGYForwardEnable == 0x01)
#include "App_Save.h"
#include "interface.h"

/********************************* API�ṹ ******************************/


/********************************* ��ҩͨ��Э��Ӧ�����ݽṹ **************/
typedef struct
{
	/* ���üĴ��� */
	struct
	{
		/* ��ҩ����ӿ� */
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
		/* ��ҩ���ݼĴ��� */
		uint8_t TXDR[1024ul];
		uint8_t RXDR[128ul];
	}DR;

}AppComProtocolGYStruct;

/********************************* ȫ�ֱ������� *****************************/
/* ��ҩ���ݼĴ�����С */
#define ProbeDataRegSize			(1024ul)
/* ��ҩ���ݼĴ��� */
extern AppComProtocolGYStruct AppComProtocolGY;

/********************************* �������� *********************************/
BOOL ComProtGY_CreateData(Save_Sensor_Data_Struct* cSaveFrameReg);
BOOL ComProtGY_ParseData(uint8_t* receBuf);


#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
