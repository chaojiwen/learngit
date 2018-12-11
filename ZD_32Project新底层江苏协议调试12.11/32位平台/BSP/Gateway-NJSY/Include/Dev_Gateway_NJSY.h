/************************************************************************************************/
/** @file              : Dev_Gateway_NJSY.h															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��11��08��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--�Ͼ�ʢ�����صײ������ļ�						*/
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
#ifndef __Dev_Gateway_NJSY_h
#define	__Dev_Gateway_NJSY_h

#include "System_Config.h"

#if (Gateway_NJSY_Enable == 0x01)


/********************************* �˿ڶ��� *********************************/

///* ��Դ����GPIO */
//#define DevGW_PowerPORT		(GPIOE)
//#define DevGW_PowerCLK		(RCC_APB2Periph_GPIOE)
//#define DevGW_PowerPIN		(GPIO_Pin_15)


/* ��Դ����GPIO */
#define DevGW_PowerPORT		(GPIOE)
#define DevGW_PowerCLK		(RCC_APB2Periph_GPIOE)
#define DevGW_PowerPIN		(GPIO_Pin_12)

///* ������Դ �������� */
//#define DevGW_PowerOpen()	{GPIO_ResetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
//#define DevGW_PowerClose() 	{GPIO_SetBits(DevGW_PowerPORT,DevGW_PowerPIN);}

/* ������Դ �������� */
#define DevGW_PowerOpen()	{GPIO_SetBits(DevGW_PowerPORT,DevGW_PowerPIN);}
#define DevGW_PowerClose() 	{GPIO_ResetBits(DevGW_PowerPORT,DevGW_PowerPIN);}






/* ******************* Gateway_NJSY �������� ******************* */
void DevGW_Init(uint32_t baudrate,void (*funtp)(uint8_t));
void DevGW_OpenPower(void);
void DevGW_ClosePower(void);
void DevGW_SendData(uint8_t* datap,uint16_t len);
void DevGW_ReceData(uint8_t* datap,uint8_t len);

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
