/************************************************************************************************/
/** @file              : Log_Gateway_HZZH.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��15��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���ݴ��������߼����ļ�									*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��11��15��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __Log_Gateway_HZZH_h
#define	__Log_Gateway_HZZH_h
#include "System_Config.h"

#if (Gateway_HZZH_Enable == 0x01)

/********************************* �궨�� *********************************/



/********************************* �������в������� *********************************/
/* ���ݴ����������н�� */
typedef enum {GWHZZH_Fail = (0u), GWHZZH_Success = (1u)}  GWHZZH_Result;
/* ���ݴ��������������� */
typedef enum {
				/* �� */
				GWHZZHCmd_Null = (0X01),	
				/* ̽ͷ��Ӧ */
				GWHZZHCmd_ProbeDataACK = (0X01),	
				/* ���û���ʱ�� */
				GWHZZHCmd_SetWkupTimeProbeNum = (0X81),
				/* ���û���ʱ����Ӧ */
				GWHZZHCmd_SetWkupTimeProbeNumACK = (0X82),
				/* ������ʱ�� */
			    GWHZZHCmd_ReadWkupTimeProbeNum = (0X83),
				/* ������ʱ����Ӧ */
				GWHZZHCmd_ReadWkupTimeProbeNumACK = (0X84),
			 }  GWHZZHCmdType;



/********************************* ����API�ṹ *********************************/


typedef struct
{	
	void	 		 (*Init)		(void);
	void	 		 (*Restart)		(void);
	GWHZZH_Result	 (*State)		(void);
	GWHZZH_Result	 (*ReadWkupTimeProbeNum)	(void);
	GWHZZH_Result	 (*SetWkupTime)	(uint8_t time,uint8_t probeNum);
	GWHZZH_Result	 (*SetProbeNum)	(uint8_t time,uint8_t probeNum);
	
}LogGatewayHZZHAPIStruct;

/********************************* �ӿ����� *********************************/
extern LogGatewayHZZHAPIStruct LogGWHZZHAPI;

/* ******************* Gateway_HZZH �������� ******************* */
void LogGW_InitAPI(void (*probeDataACKp)(uint8_t* id,uint8_t* data),
				   void (*setWkupTimeProbeNumACKp)(void),
				   void (*readWkupTimeProbeNumACKp)(uint8_t wkupTime,uint8_t ProbeNum));


#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
