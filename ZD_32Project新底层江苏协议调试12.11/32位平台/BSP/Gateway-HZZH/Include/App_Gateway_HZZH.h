/************************************************************************************************/
/** @file              : App_Gateway_HZZH.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��12��15��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���ݴ�������Ӧ�ò��ļ�							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��12��15��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __App_Gateway_HZZH_h
#define	__App_Gateway_HZZH_h

#include "System_Config.h"
#if (Gateway_HZZH_Enable == 0x01)
#include "Mcu_Config.h"
#include "Log_Gateway_HZZH.h"
#include "ucos_ii.h"

/********************************* ����ѡ�� *********************************/
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h> ���ݴ������ع�������.
//	<e0> ������Ϣ.
//	<i>  Ŀǰ������Ϣ�е���Ŀֻ��1��.
//  <o1.0..2> �жϵ�����Ϣ
//	<i>  ���Խ׶���Ҫ�ж�����
//  <0=> [�ر�]������Ϣ.
//  <1=> [��]������Ϣ.
#define AppGatewayHZZHDebugEnable		(0x01)
#define AppGatewayHZZHDebugInform		(1)
//  </e>


// </h>
//	<<< end of configuration section >>>

/********************************* �������в������� *********************************/
/* ���������ջ��С */
#define AppTask_GatewayHZZH_StkSize 	(128u)

/********************************* ����̽ͷ���ݽṹ *********************************/
typedef struct
{
	/* ��ص������� */
	u32 PowerInteger		:4;
	/* ��ص���С�� */
	u32 PowerDecimal		:4;
	/* �¶����� */
	u32 TempInteger			:7;
	/* �¶�������־ */
	u32 TempPM				:1;
	/* ʪ��С�� */
	u32 HumiDecimal			:4;
	/* �¶�С�� */
	u32 TempDecimal			:4;
	/* ʪ������ */
	u32 HumiInteger			:8;
}AppGatewayHZZHProbeDataStruct;
/********************************* ����Ӧ�ýӿڽṹ *********************************/
typedef struct
{	
	void 			(*Init)					(void);
	void 			(*Restart)				(void);
	GWHZZH_Result	(*UpdateState)			(void);
	GWHZZH_Result	(*GetState)				(void);
	GWHZZH_Result	(*ReadWkupTimeProbeNum)	(uint16_t* time,uint16_t* probeNum);	
	GWHZZH_Result	(*SetWkupTime)			(uint16_t time,uint16_t probeNum);
	GWHZZH_Result	(*SetProbeNum)			(uint16_t time,uint16_t probeNum);
	
	
}AppGatewayHZZHAPIStruct;
/********************************* Ӧ�ýӿ����� *********************************/
extern AppGatewayHZZHAPIStruct AppGWHZZH_API;
extern OS_STK AppTaskGatewayHZZHStk[AppTask_GatewayHZZH_StkSize];

/* ******************* ���ݴ������ز����������� ******************* */
void AppTaskGatewayHZZH(void *p_arg);



#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
