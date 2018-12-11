/************************************************************************************************/
/** @file              : ComProtocol_ZDE.h														*/
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
/**�ļ���     : ComProtocol_ZDE.h																*/
/**����       : ���ݡ����ͨ��Э�顱�ṩ��ͨ��Э���ʽ��̽ͷ�����ϴ�����ƽ̨							*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2017��02��16��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/ 
#ifndef __ComProtocol_ZDE_h
#define	__ComProtocol_ZDE_h
#include "Log_GSM.h"
#if (ComProtocol_ZDE_Enable == 0x01)
#include "App_Save.h"

/********************************* �궨����� **************/
/* �����ͨ��Э�顱�������ݼĴ�����С */
#define ComProtocolZDE_TXSize		(1024ul)
/* �����ͨ��Э�顱�������ݼĴ�����С */
#define ComProtocolZDE_RXSize		(128ul)

/********************************* �����ͨ��Э�顱Ӧ�����ݽṹ **************/
typedef struct
{
	/* ���üĴ��� */
	struct
	{
		uint8_t null;
		
	}CR;
	
	/* ״̬�Ĵ��� */
	struct
	{
		uint8_t null;
	
	}SR;
	
	/* ���ݼĴ��� */
	struct
	{
		/* �����ͨ��Э�顱���ݼĴ��� */
		uint8_t TXDR[ComProtocolZDE_TXSize];
		uint8_t RXDR[ComProtocolZDE_RXSize];
		/* �������ݳ��� */
		uint16_t TXLen;
	}DR;
	
}
ComProtocolZDEAppStruct;

/********************************* ȫ�ֱ������� *****************************/
/* �����ͨ��Э�顱���ݼĴ��� */
extern ComProtocolZDEAppStruct		ComProtocolZDEApp;

/********************************* �������� *********************************/
BOOL ComProtocolZDE_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,
							   uint8_t* destBuf,int* dataLen);
BOOL ComProtZDE_ParseData(uint8_t* destBuf,int* dataLen);

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

