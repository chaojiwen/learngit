/************************************************************************************************/
/** @file              : Dev_Gateway_HZZH.h															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��08��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���ݴ������صײ������ļ�						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0      ZRDang      2016��11��08��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __Dev_Gateway_HZZH_h
#define	__Dev_Gateway_HZZH_h

#include "System_Config.h"

#if (Gateway_HZZH_Enable == 0x01)


/********************************* �˿ڶ��� *********************************/

/* ��Դ����GPIO */
#define DevGW_PowerPORT		(GPIOC)
#define DevGW_PowerCLK		(RCC_APB2Periph_GPIOC)
#define DevGW_PowerPIN		(GPIO_Pin_15)


#ifdef Hardware_Version_V131
	/* ������Դ �������� */
	#define DevGW_PowerOpen()		{GPIO_ResetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
	#define DevGW_PowerClose() 	{GPIO_SetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
#else
/* ������Դ �������� */
	#define DevGW_PowerOpen()		{GPIO_SetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
	#define DevGW_PowerClose() 	{GPIO_ResetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
#endif

/********************************* ����API�ṹ *********************************/
typedef struct
{	
	void	  (*Init)			(uint32_t baudrate,void (*funtp)(uint8_t));
	void 	  (*OpenPower)		(void);
	void	  (*ClosePower)		(void);
	void 	  (*SendData)		(uint8_t* datap,uint16_t len);
	void 	  (*ReceData)		(uint8_t* datap,uint8_t len);
}DevGatewayHZZHAPIStruct;


/* ******************* Gateway_HZZH ȫ�ֱ������� ******************* */
extern DevGatewayHZZHAPIStruct DevGWHZZHAPI;

/* ******************* Gateway_HZZH �������� ******************* */
void DevGW_InitAPI(void);


#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
