/************************************************************************************************/
/** @file              : Log_Gateway_NJSY.h														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��15��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--�����߼����ļ�									*/
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
#ifndef __Log_Gateway_NJSY_h
#define	__Log_Gateway_NJSY_h
#include "System_Config.h"

#if (Gateway_NJSY_Enable == 0x01)

/********************************* �궨�� *********************************/
/* ���ؽ��ճ�ʱʱ�� */
#define LogGW_ReceOvertimeValue	(20u)     // 
/* ��������ʱ�� */
#define LogGW_RestartTimeValue	(50u)



/********************************* �������в������� *********************************/
/* ̽ͷ���� */
typedef enum {GWProbe_Temp = (0u), GWProbe_Hum = (1u)} GW_ProbeType;
/* �������н�� */
typedef enum {GW_Fail = (0u), GW_Success = (1u)}  GW_Result;
/* ���ع���ģʽ */
typedef enum {GW_Release = (0u), GW_Debug = (1u)} GW_Mode;
/* ���ص�ص���״̬ */
typedef enum {GW_Level_FALSE = (0u), GW_Level_TRUE = (1u)} GW_Level;

/********************************* ����API�ṹ *********************************/
typedef struct
{	
	/* ���ص�ַ */
	uint8_t Adder;
	/* �ŵ�(����) */
	uint8_t Channel;
	/* ������Чʱ�� */
	uint8_t ActiveTime;
	/* ����ģʽ������/���ԣ� */
	GW_Mode Mode;
	/* ͨ�Ų����� */
	uint32_t Baudrate;
}LogGatewayInitParaStruct;

typedef struct
{	
	void	  (*Init)			(LogGatewayInitParaStruct initPara);
	GW_Result (*Restart)		(void);
	GW_Result (*ConfigFunc)		(void);
	GW_Result (*ReadProbe)		(uint8_t probeNo,GW_ProbeType probetype,float* probe);
	GW_Result (*ReadLevel)		(uint8_t probeNo,GW_Level* level);
	GW_Result (*UpdataProbeData)(void);
	GW_Result (*UpdataState)	(void);
	GW_Result (*ReadAdder)		(uint8_t* adder);	
	GW_Result (*SetAdder)		(uint8_t adder);
	GW_Result (*SetChannel)		(uint8_t channel);
	GW_Result (*SetActiveTime)	(uint8_t time);
	GW_Result (*SetMode)		(GW_Mode mode);
	GW_Result (*SetBaudrate)	(uint32_t baudrate);
	
}LogGatewayAPIStruct;

/********************************* �ӿ����� *********************************/
extern LogGatewayAPIStruct LogGW_API;

/* ******************* Gateway_NJSY �������� ******************* */
void BSP_LogGW_API_Init(void);


#endif
#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
