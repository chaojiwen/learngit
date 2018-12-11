/************************************************************************************************/
/** @file              : App_Gateway_NJSY.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��18��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--�Ͼ�ʢ������Ӧ�ò��ļ�							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��11��18��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __App_Gateway_NJSY_h
#define	__App_Gateway_NJSY_h

#include "System_Config.h"
#if (Gateway_NJSY_Enable == 0x01)
#include "Mcu_Config.h"
#include "Log_Gateway_NJSY.h"
#include "ucos_ii.h"

/********************************* ����ѡ�� *********************************/
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h> �Ͼ�ʢ�����ع�������.
//	<e0> ������Ϣ.
//	<i>  Ŀǰ������Ϣ�е���Ŀֻ��1��.
//  <o1.0..2> �жϵ�����Ϣ
//	<i>  ���Խ׶���Ҫ�ж�����
//  <0=> [�ر�]������Ϣ.
//  <1=> [��]������Ϣ.
#define AppGatewayDebugEnable			(0x01)
#define AppGatewayDebugInform			(1)
//  </e>


// </h>
//	<<< end of configuration section >>>

/********************************* �������в������� *********************************/
/* ���������ջ��С */
#define AppTask_GatewayNJSY_StkSize 	(128u)
/* ���س�ʼ������ */
typedef INSTRU_Gateway_FIX  AppGWInitPara; 


/********************************* ����Ӧ�ýӿڽṹ *********************************/
typedef struct
{	
	void 	  (*Init)			(AppGWInitPara* initPara);
	GW_Result (*Restart)		(void);		
	void 	  (*ConfigFunc)		(void);	
	GW_Result (*ReadProbe)		(uint8_t probeNo,GW_ProbeType probetype,float* probe);
	GW_Result (*ReadLevel)		(uint8_t probeNo,GW_Level* level);
	GW_Result (*UpdataProbeData)(void);	
	void      (*UpdataState)	(void);
	GW_Result (*SetAdder)		(uint8_t adder);
	GW_Result (*SetChannel)		(uint8_t channel);
	GW_Result (*SetActiveTime)	(uint8_t time);
	GW_Result (*SetMode)		(GW_Mode mode);
	GW_Result (*SetBaudrate)	(uint32_t baudrate);
	
}AppGatewayAPIStruct;
/********************************* Ӧ�ýӿ����� *********************************/
extern AppGatewayAPIStruct AppGW_API;
extern OS_STK AppTaskGatewayNJSYStk[AppTask_GatewayNJSY_StkSize];

/* ******************* �Ͼ�ʢ�����ز����������� ******************* */
void AppTaskGatewayNJSY(void *p_arg);
void BSP_AppGateway_Init(void);


#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
