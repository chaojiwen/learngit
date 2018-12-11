/************************************************************************************************/
/** @file              : Log_IAP.H																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��08��24��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--�̼�����Ӧ�ò��ļ�								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2016 (C) HZ Zeda Team																*/
/**																								*/
/**�ļ���     : Log_IAP.H																		*/
/**����       : �̼�����Ӧ�ù���																	*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��08��24��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_GSM.h"

#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)
#ifndef __Log_IAP_H
#define __Log_IAP_H

#include "stm32f10x.h"
#include "Log_ConfigService.h"

/****************************************������**************************************************/
/************************************************************************************************/ 
/* �̼�����Ӧ�ò����ṹ */
typedef struct
{
	/* ����״̬ */
	BOOL RunState;
	/* ������־ */
	uint8_t* UpdateFlag;
	/* ����״̬ */
	BOOL UpdateState;
	/* �������Ȱٷֱ� */
	uint8_t UpdatePercent;
}
IAP_AppStruct;


/* �̼�����Ӧ�ò��� */
extern IAP_AppStruct IAP_App;



/*********************************  ��������  ********************************/
void IAP_UploadLog(GPRS_Struct* gprs);
void IAP_CheckFlag(void);
void IAP_ParaInit(ConfigService_Para_Update* updateparap,uint8_t* updateflag);
BOOL IAP_UpdateSatte(void);
void IAP_StartRun(void);
void IAP_StopRun(void);

void mcu_reset(void);//�����豸  ��λ��Ƭ��
#endif
#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
