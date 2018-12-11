/************************************************************************************************/
/** @file              : Log_ConfigService.c													*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��01��07��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--���÷�����Ӧ�ò��ļ�							*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��08��02��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_ConfigService.h"
/****************����ͷ�ļ�************************/
#include "GSM_app.h"
#include "Gsm_Interface.h"
/************************************************************************************************/
/*	����n��ʾnetwork��h��ʾhost��																	*/
/*	htons( ) htonl( )���ڱ����ֽ��������ֽ�ת���ĳ��ϣ�												*/
/*	s��ʾshort������2�ֽڲ�����																	*/
/*	l��ʾlong,  ����4�ֽڲ�����																	*/
/*	ͬ��ntohs( )ntohl( )���������ֽ��򱾵ظ�ʽת���ĳ��ϡ� 											*/
/************************************************************************************************/
#define htons(A)     ((((u16)(A) & 0xff00) >> 8) | \
(((u16)(A) & 0x00ff) << 8))
#define htonl(A)     ((((u32)(A) & 0xff000000) >> 24) | \
(((u32)(A) & 0x00ff0000) >> 8) | \
(((u32)(A) & 0x0000ff00) << 8) | \
(((u32)(A) & 0x000000ff) << 24))

#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)//����Э��
#include "Dev_Fram.h"
#include "Dev_RTC.h"
#include "Log_HTTP.h"
#include "Log_IAP.h"
#include "app.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* ���÷�����ͨ��Э��ṹ */
typedef struct
{
	//��·���䳵��ͨѶЭ Э����ô��ģʽ(big-endian)�������ֽ����������ֺ�˫�֡�
	//������ݽṹֻ����Ӧ2ͨ���Ĵ�����GPRS���ݷ��ͣ����Ҳ��ܴ���״̬��Ϣ��
	/* ͷ ��ʶλ */ //(1 Byte)
	uint8_t			Start_Iden;
	/* ͨ�� ��Ϣͷ *///(16 Byte)
	/* ��Ϣ ID(����) *///(2 Byte)
	uint8_t			ID_Type_HSB;
	uint8_t			ID_Type_LSB;
	/* ��Ϣ�� ���� *///(2 Byte)
	//��Ϣ������ ��Ϣ�峤��
	uint8_t	 		Attribute_HSB_Length:2;		
	//��Ϣ������ ���ݼ��ܷ�ʽ
	uint8_t	 		Attribute_HSB_Enc_Mode:3;
	//��Ϣ������ �ְ�
	uint8_t	 		Attribute_HSB_Sub_Flag:1;		
	//��Ϣ������ ����
	uint8_t 		Attribute_HSB_Reserve:2;
	//��Ϣ������ ��Ϣ�峤��
	uint8_t	 		Attribute_LSB_Length:8;
	/* �ն����к�(SN��): *///(6 Byte)
	uint8_t			SN_NO[6];
	/* ��Ϣ ��ˮ�� *///(2 Byte)
	uint8_t			Serial_NO_HSB;
	uint8_t			Serial_NO_LSB;

	/* ͨ�� ��Ϣ�� */
	/* ��Ϣ����:0XFB(���÷�������Ϣ) *///(1 Byte)
	uint8_t			Body_Type;
	/* ��Ϣ��������:00->ͬ��״̬;01->�޸�״̬;02->�ظ� *///(1 Byte)
	uint8_t			Cmd_Type;
	/* �������� *///(1 Byte)
	uint8_t			Num;

}ConfigServiceComStruct;


/* ̽ͷ���� */
typedef enum
{
	/* �����ݰ� */
	CSCC_Null    = 0x00,
	/* ϵͳ��Ϣ */
	CSCC_System  = 0x01,
	/* ��Դ��Ϣ */
	CSCC_Power   = 0x02,
	/* ϵͳʱ����Ϣ */
	CSCC_Clock   = 0x03,
	/* GSMģ����Ϣ */
	CSCC_GSM     = 0x04,
	/* ��������Ϣ */
	CSCC_Sensor  = 0x05,
	/* �˵���Ϣ */
	CSCC_Menu    = 0x06,
	/* ������Ϣ */
	CSCC_Update  = 0x08,
}
ConfigService_ComContentType;

/* ���÷�����ͨ��Э������ṹ */
typedef struct
{
	/* ���� */
	ConfigService_ComContentType	Type;
	/* ��ַ */
	uint8_t		Adder_H;	
	uint8_t		Adder_L;
	
	/* ���� */
	uint8_t		Len_H;	
	uint8_t		Len_L;

	/* ���� */
	uint8_t		Data[640];	

}ConfigServiceComContentStruct;


/* ���÷���������������Ϣ�̶����� */
static const ConfigServiceComStruct  ConfigServiceComFix = 
{
	/* ͷ ��ʶλ */
	0X7E,
/****************************************************/
	/* ͨ�� ��Ϣͷ */
	/* ��Ϣ ID(����) */
	0X09,
	0X00,
	/* ��Ϣ�� ���� */
	/* ��Ϣ������ ��Ϣ�峤�� */
	0X00,	
	/* ��Ϣ������ ���ݼ��ܷ�ʽ */
	0X00,
	/* ��Ϣ������ �ְ� */
	0X00,		
	/* ��Ϣ������ ���� */
	0X00,
	/* ��Ϣ������ ��Ϣ�峤�� */
	0X00,
	/* �ն����к�(SN��): */
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* ��Ϣ ��ˮ�� */
	0X00,
	0X00,
/****************************************************/
	/* ͨ�� ��Ϣ�� */
	/* ��Ϣ����: */
	0XFB,
	/* ��Ϣ�������� */
	0X00,
	/* �������� */
	0X00,

/****************************************************/
};

/* ���÷��������� */
ConfigService_ParaTabDef ConfigService = 
{

	/* ϵͳ��Ϣ */
	/* ���к�(6�ֽ�) *///ע�⣺����ĸߵ�Ϊ����
	0X0,0X0,0X0,0X0, 0X0,0X0,0X0,0X0, 0X0,0X0,0X0,0X0,
	
	/* ����汾(3 �ֽ�)  */
	0, 0, 1,
	
	/* Ӳ���汾(3 �ֽ�)  */
	0, 0, 1,
	
	
	
	/* ��Դ״̬ */
	TRUE,
	/* ��ӵ�Դ �������� */
	FALSE,
	/* ��ӵ�Դ ���ⱨ������ */
	FALSE,
	/* ��ӵ�Դ ���ű������� */
	FALSE,

	/* ����״̬ */
	100,
	/* �͵��� �������� */
	FALSE,
	/* �͵��� ���ⱨ������ */
	FALSE,
	/* �͵��� ���ű������� */
	FALSE,
	
	/* ̽ͷ��Ϣ */
#if (1)	
	/* ̽ͷ 1 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,
	
	/* ̽ͷ 2 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,
	
	/* ̽ͷ 3 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 4 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 5 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 6 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 7 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 8 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 9 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 10 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 11 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 12 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 13 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 14 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 15 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 16 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 17 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 18 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 19 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 20 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 21 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 22 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 23 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 24 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 25 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 26 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 27 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 28 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 29 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,
	
	/* ̽ͷ 30 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,
	
	/* ̽ͷ 31 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,

	/* ̽ͷ 32 ���� */
	CSPS_Null,
	/* ̽ͷ�������� */
	FALSE,
	/* �������������� */
	FALSE,
	/* ���ű������� */
	FALSE,

	/* ̽ͷSN�� */
	0X00,0X00,0X00,0X00,
	/* ��ƬID�� */
	0X00000000, 
	/* �������� */
	0.0,
	/* �������� */
	0.0,
#endif

	/* �˵���Ϣ */
	/* ���� �������� */
	FALSE,
	/* ���� �������� */
	FALSE,
	/* ���� */
	0,
	/* ���� */
	0,
	
	/* �ظ����ű���ʱ�� */
	600,
	/* �������ݼ�¼��� */
	120,
	/* �������ݼ�¼��� */
	300,
	
	/* ������Ϣ */
	0,
	/* �����ļ��� */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,
	/* ������������ַ��IP/������ */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,	
	/* �����������˿� */
	0X00,
	0X00,
/* �����ļ�crc32У��λ */
	0,
 /* ���ݷ�������ַ�������� */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, /* ���ݷ�����Ԥ���ֽڹ�4���ֽ�*/
	121,42,49,249, /* ���ݷ�������ַ��IP�� */
	/*���ݷ������˿� */
	5902,
	0,
 /* ���÷�������ַ�������� */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, /* ���÷�����Ԥ���ֽڹ�4���ֽ�*/
	115,28,105,232, /* ���÷�������ַ��IP�� */
	/*���÷������˿� */
	12346,
	0,
	 /* ������������ַ�������� */
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, 	0X00,0X00,0X00,0X00,
	0X00,0X00,0X00,0X00, /* ����������Ԥ���ֽڹ�4���ֽ�*/
	0,0,0,0, /* ������������ַ��IP�� */
	/*�����������˿� */
	0,
	0,
	/* GSMģ�� �������ź���_1(6�ֽ�)  */		//15067448280
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0,		 0X0,0X0,0X0,
	/* GSMģ�� �������ź���_2(6�ֽ�)  */
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0,		 0X0,0X0,0X0,
	/* GSMģ�� �������ź���_3(6�ֽ�)  */
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0,		 0X0,0X0,0X0,
	/* GSMģ�� �������� ����4(6�ֽ�)  */
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0, 		 0X0,0X0,0X0,
	/* GSMģ�� �������� ����5(6�ֽ�)  */
	0X0,0X0,0X0,0X0, 		0X0,0X0,0X0,0X0, 		 0X0,0X0,0X0,
	0X00,0X00,0X00,0X00, /* Ԥ���ֽڹ�4���ֽ�*/
};


/* ���ݼĴ��� */
/* �������ݼĴ��� */
static uint8_t ConfigService_TXDR[1536];
/* �������ݼĴ��� */
static uint8_t ConfigService_RXDR[1500];

/* ���÷�����Ӧ�ò��� */
ConfigService_AppDef ConfigService_App = 
{
	FALSE,
	FALSE,
};



/************************************************************************************************/
/* ������	: ConfigService_SetTXContentData												 	*/
/* ����  	: dest:Ŀ���ַ��src:Դ��ַ			 												*/
/* ���		: ������ַ 																				*/
/* ����	 	: ���÷��Ͳ������ݵ�����								 									*/
/* �������� 	: 2016/08/15																		*/
/************************************************************************************************/
static uint8_t* ConfigService_SetTXContentData(uint8_t* dest,ConfigServiceComContentStruct* src)
{
	/* ���ݳ��� */
	uint32_t len = 0;
	
	/* �������ݳ��� */
	len = 5 + (src->Len_H * 256) + src->Len_L;
	
	/* ����SN�� */
	memcpy(dest,(uint8_t*)src,len);


	/* ���ؽ�����ַ */
	return (dest+len);
}

/************************************************************************************************/
/* ������	: ConfigService_StopRun												 				*/
/* ����  	: ��			 																	*/
/* ���		: ��		 																		*/
/* ����	 	: ֹͣ����								 											*/
/* �������� 	: 2016/08/30																		*/
/* �޸����� 	: 2016/08/30																		*/
/*   �汾 	: Ver:0																				*/
/************************************************************************************************/
void ConfigService_StopRun(void)
{
	/* ֹͣ���� */
	ConfigService_App.Run_State = FALSE;
}

/************************************************************************************************/
/* ������	: ConfigService_StartRun												 				*/
/* ����  	: ��			 																	*/
/* ���		: ��		 																		*/
/* ����	 	: ��������								 											*/
/* �������� 	: 2016/08/30																		*/
/* �޸����� 	: 2016/08/30																		*/
/*   �汾 	: Ver:0																				*/
/************************************************************************************************/
void ConfigService_StartRun(void)
{
	/* ֹͣ���� */
	ConfigService_App.Run_State = TRUE;
}

/* �����ϴ�������Ϣ��Ŀ */
/* ϵͳ��Ϣ  */
#define ConfigService_UploadSystem	       (0x01)
/* ��Դ��Ϣ */
#define ConfigService_UploadPower	         (0x02)
/* ϵͳʱ����Ϣ */
#define ConfigService_Clock	               (0x03)
/* GSMģ����Ϣ */
#define ConfigService_Gsm	                 (0x04)
/* ������̽ͷ��Ϣ */
#define ConfigService_UploadSensor	       (0x05)
/* �˵���Ϣ */
#define ConfigService_UploadMenu	         (0x06)
/* ������Ϣ */
#define ConfigService_UploadUpdata	       (0x08)

/* �հ���Ϣ */
#define ConfigService_UploadNull	(0x00)
/* ȫ����Ϣ */
#define ConfigService_UploadAll		(0xFF)
/************************************************************************************************/
/* ������	: ConfigService_Cmd																 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ����������Ϣ����ָ��								 									*/
/* �������� 	: 2016/08/11																		*/
/* �޸����� 	: 2016/08/11																		*/
/************************************************************************************************/
void ConfigService_Cmd(uint8_t item,uint8_t type)
{
	ConfigServiceComStruct             *ConfigService_parap;/* ��Ϣͷָ�� */
	ConfigServiceComContentStruct   configservicecomcontent;/* �������ݼĴ��� */
	uint8_t                                      *end_Adder;/* ���ݽ�����ַ */
	uint8_t                                          *check;/* У��λָ�� */
	uint8_t                                       *end_Iden;/* β ��ʶλ */
	uint32_t                                            len;/* ���ݳ��� */
	uint32_t                                            num;/* ������ */  
	if((type != 0x00)&&(type != 0x01)&&(type != 0x02))	/* �жϲ��������Ƿ�Ϸ� */
	{
		return;
	}
	
	/* ����������ݼĴ��� */
	memset(ConfigService_TXDR,0X00, sizeof(ConfigService_TXDR));
	
	/* ����Ĭ��ֵ */
	memcpy(ConfigService_TXDR,((uint8_t*)(&ConfigServiceComFix)),sizeof(ConfigServiceComFix));

	/* ���á���Ϣͷ��ָ�� */
	ConfigService_parap = (ConfigServiceComStruct *)ConfigService_TXDR;
	
	/* ����SN�� */
	memcpy(ConfigService_parap->SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);
//	/* ������Ϣ������(Ĭ��) */
//	ConfigService_parap->Body_Type = 0xFB;
	/* ���ò��������� */
	ConfigService_parap->Cmd_Type = type;
	
	/* ���ò����� */
	ConfigService_parap->Num = 0;
	
	/* ���ò������ݵ�ַ */
	end_Adder = ((&ConfigService_parap->Num)+1);
	
	/* �ж��Ƿ���Ҫ���� ϵͳ���� */
	if(ConfigService_UploadSystem & item)
	{
		/* ���� ϵͳ���� */
		configservicecomcontent.Type 		          = CSCC_System;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H          	= 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.System)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.System)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.System),sizeof(ConfigService.System));
		
		/* ���ò������� */
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);
		
		/* ���ò����� */
		ConfigService_parap->Num++;
	}

	/* �ж��Ƿ���Ҫ���� ��Դ��Ϣ */
	if(ConfigService_UploadPower & item)
	{
		/* ���� ��Դ��Ϣ */
		configservicecomcontent.Type 		          = CSCC_Power;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H 	        = 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.Power)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.Power)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.Power),sizeof(ConfigService.Power));
		/* ���ò������� */
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);
		/* ���ò����� */
		ConfigService_parap->Num++;
	}

	/* ������ݳ��� */
	len = 0;
	/* �ж��Ƿ���Ҫ���� ��������Ϣ */
	if(ConfigService_UploadSensor & item)
	{
		/* ���� ��������Ϣ */
		configservicecomcontent.Type 		= CSCC_Sensor;
		configservicecomcontent.Adder_L 	= 0;
		configservicecomcontent.Adder_H 	= 0;
		for(num = 0 ; num < 32 ; num++)
		{
			if(ConfigService.Sensor[num].SensorType != CSPS_Null)	/* �жϵ�ǰ��̽ͷ�����Ƿ����ù� */
			{
				/* ��ǰ̽ͷ�Ѿ����� */
				memcpy(&configservicecomcontent.Data[sizeof(ConfigService_Para_Sensor)*num],
					  (uint8_t*)(&ConfigService.Sensor[num]),
					  sizeof(ConfigService_Para_Sensor));
				len += sizeof(ConfigService_Para_Sensor);/* �������ݳ��� */ 
			}
		}
		configservicecomcontent.Len_L		          = len%256;
		configservicecomcontent.Len_H		          = len/256;	
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);			/* ���ò������� */	
		ConfigService_parap->Num++;		/* ���ò����� */
	}
	
	/* �ж��Ƿ���Ҫ���� �˵���Ϣ */
	if(ConfigService_UploadMenu & item)
	{
		/* ���� �˵���Ϣ */
		configservicecomcontent.Type 		          = CSCC_Menu;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H 	        = 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.Menu)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.Menu)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.Menu),sizeof(ConfigService.Menu));	
		/* ���ò������� */
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);
		/* ���ò����� */
		ConfigService_parap->Num++;
	}
	
	/* �ж��Ƿ���Ҫ���� Զ��������Ϣ */
	if(ConfigService_UploadUpdata & item)
	{
		ConfigService.Update.Flag=0;
		/* ���� Զ��������Ϣ */
		configservicecomcontent.Type 		          = CSCC_Update;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H 	        = 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.Update.Flag)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.Update.Flag)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.Update.Flag),sizeof(ConfigService.Update.Flag));
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);	/* ���ò������� */
		ConfigService_parap->Num++;/* ���ò����� */
	}
 /* �ж��Ƿ���Ҫ���� GSMģ����Ϣ */	
	if(ConfigService_Gsm & item)
	{
		/* ���� GSMģ����Ϣ */
		configservicecomcontent.Type 		          = CSCC_GSM;
		configservicecomcontent.Adder_L 	        = 0;
		configservicecomcontent.Adder_H 	        = 0;
		configservicecomcontent.Len_L		          = sizeof(ConfigService.Gsm)%256;
		configservicecomcontent.Len_H		          = sizeof(ConfigService.Gsm)/256;
		memcpy(configservicecomcontent.Data,(uint8_t*)(&ConfigService.Gsm),sizeof(ConfigService.Gsm));
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);	/* ���ò������� */
		ConfigService_parap->Num++;/* ���ò����� */
	}
	if(ConfigService_UploadNull == item)/* �ж��Ƿ�Ϊ �հ���Ϣ */
	{
		/* ���� �հ���Ϣ */
		configservicecomcontent.Type 		           = CSCC_Null;
		configservicecomcontent.Adder_L 	         = 0;
		configservicecomcontent.Adder_H 	         = 0;
		configservicecomcontent.Len_L		           = 0;
		configservicecomcontent.Len_H		           = 0;
		memset(configservicecomcontent.Data,0x00,sizeof(configservicecomcontent.Data));
		end_Adder = ConfigService_SetTXContentData(end_Adder,&configservicecomcontent);/* ���ò������� */
		ConfigService_parap->Num++;/* ���ò����� */
	}
	len = ((uint32_t)(end_Adder - &ConfigService_parap->Body_Type));	/* �������ݳ��� */
	/* ������Ϣ�峤�� */
	ConfigService_parap->Attribute_HSB_Length = (len>>8);
	ConfigService_parap->Attribute_LSB_Length = (len%256);	
	check = end_Adder;/* ����У��λָ�� */
	/* �������ݳ��� */
	len = len + 13;//(13��ͨ��Э��̶����ݳ���)
	/* ����У���� */
	for(num = 1 ; num < len ; num++)
	{
		/* �� ��Ϣͷ �� ��Ϣ�� ���һ�� ���� ��� */
		*check ^= ConfigService_TXDR[num];
	}
	/* �������ݳ��� */
	len = len + 1;//(+У��)
	//1��ת�� ����/* ת�� *//* ���GPRS�������ݳ��� */
	num = GPRS_Host_To_Network_Translator_Data(&ConfigService_RXDR[1],
	                                           &ConfigService_TXDR[1],len);	
	memcpy(&ConfigService_TXDR[1],&ConfigService_RXDR[1],num);/* ����ת��������� */
	memset(ConfigService_RXDR,0x00,sizeof(ConfigService_RXDR));/* ����������ݼĴ��� */
	len = num;/* �������ݳ��� */
	end_Iden  = &ConfigService_TXDR[len];/* ����β��ʶλָ�� */
  *end_Iden = 0X7E;/* ����β��ʶ */
	/* ����GPRS�������ݲ��� */
	GPRS.ConfigService.Para.TXDR	= ConfigService_TXDR;
	GPRS.ConfigService.Para.TXLen	= len+1;//(����1�ֽڽ���������)
	GPRS.ConfigService.Para.RXDR	= ConfigService_RXDR;
	GPRS.ConfigService.Para.RXLen	= 1500;
	/* ���Ͷ���(ConfigService) */
	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,ConfigService_EVENT);
	/* �������÷���������״̬ */
	ConfigService_StartRun();
}


/************************************************************************************************/
/* ������	: ConfigService_SYNC															 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ����ͬ��״ָ̬��								 									*/
/* �������� 	: 2016/08/11																		*/
/************************************************************************************************/
//static void ConfigService_SYNC(void)
//{

//	/* ����ͬ��״ָ̬�� */
//	ConfigService_Cmd(ConfigService_UploadNull,0x00);
//	
//}

/************************************************************************************************/
/* ������	: ConfigService_Query															 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���Ͳ�ѯ״ָ̬��								 									*/
/* �������� 	: 2016/08/11																		*/
/************************************************************************************************/
static void ConfigService_Query(void)
{
	
	/* �����޸�״ָ̬�� */
	ConfigService_Cmd(ConfigService_UploadNull,0x01);
	
}

/************************************************************************************************/
/* ������	: ConfigService_TXLog															 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���÷�������������ҵ������								 							*/
/* �������� 	: 2016/08/11																		*/
/* �޸����� 	: 2016/08/16																		*/
/*   �汾 	: V0: ����������ҵ���߼�															*/
/*   �汾 	: V1: ��ԭ���ġ�����ҵ�񡱸�Ϊ���ڵġ���������ҵ��									*/
/************************************************************************************************/
extern  BOOL     GSM_DEBUG_Flag; //zwc
void ConfigService_TXLog(INSTRU_CONFIG_INF_TAB* scrPara)
{
	(void)scrPara;
	
	/* �ж��Ƿ����ڽ������÷��������� */
	if(ConfigService_App.Run_State == TRUE)
	{
		/* ���ڽ������÷��������������ٷ�������������� */
		return;
	}
/* 1���ж������Ƿ�ͬ����� ϵͳÿ�θ�λ�������һ�ν�ϵͳ�����в���ͬ�������÷�������*/
	if(ConfigService_App.SYNC_State == FALSE)	/* ����δ����ͬ������ͬ��ʧ�� */
	{
			/* ������ƽ̨�Ѿ�ͬ����ɣ�����ͬ��״̬��־. */
			ConfigService_App.SYNC_State = TRUE;//�豸������Ĭ�ϻᷢһ��ϵͳ������Ϣ  ��ͬ��״̬��Ϊ��ͬ��
			if(GSM_DEBUG_Flag ==TRUE)
			{
				printf("1����ϵͳ������Ϣͬ�������÷�������\n");
			}
//	ConfigService_SYNC();	/* ����ͬ��״ָ̬�� */
		ConfigService_Cmd(ConfigService_UploadAll,0x01);			/* �ϴ����豸������Ϣ(ͬ��ƽ̨����) */
	}
	else
	{
			if(GSM_DEBUG_Flag ==TRUE)
			{
				printf("���Ͳ�ѯ����ƽ̨�Ƿ��·������ò�����Ϣ\r\n");
			}
		/* �����Ѿ�����ͬ����� */
		/* ���Ͳ�ѯ״ָ̬�� */
 		ConfigService_Query();
		
	}

}

/************************************************************************************************/
/* ������	: ConfigService_UpdataConfig														*/
/* ����  	: ��								 												*/
/* ���		: ��		 																		*/
/* ����	 	: ����������������										 							*/
/* �������� 	: 2016/08/16																		*/
/* �޸����� 	: 2016/08/16																		*/
/*   �汾 	: V0: ����������ҵ���߼�															*/
/************************************************************************************************/
static ConfigServiceComContentStruct * ConfigService_UpdataConfig(ConfigServiceComContentStruct* config)
{
	/* ��������ָ�� */
	uint8_t* contentEndp = (uint8_t*)config;
	/* ��ַ */
	uint16_t	adder = config->Adder_L + (((uint16_t)config->Adder_H)<<8);
	/* ���ݳ��� */
	uint32_t	len   = config->Len_L + (((uint16_t)config->Len_H)<<8);
	
	
	switch(config->Type)
	{
		/* �����ݰ� */
		case CSCC_Null:
		{
			/* �����ݰ������κβ�����ֱ�ӷ��ؽ�����ַ */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* ϵͳ��Ϣ */
		case CSCC_System:
		{
			/* ����ϵͳ��Ϣ */
			/* �������ݵ���ϵͳ��Ϣ���������� */
			memcpy(((uint8_t*)(&ConfigService.System)+adder),config->Data,len);
			/* ���ؽ�����ַ */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* ��Դ��Ϣ */
		case CSCC_Power:
		{
			/* ���õ�Դ��Ϣ */
			/* �������ݵ�����Դ��Ϣ���������� */
			memcpy(((uint8_t*)(&ConfigService.Power)+adder),config->Data,len);
			/* ���ؽ�����ַ */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* ϵͳʱ����Ϣ */
		case CSCC_Clock:
		{
			/* ��ǰʱ����Ϣû�������������迼�ǡ� */
			/* ���ؽ�����ַ */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* GSMģ����Ϣ */
		case CSCC_GSM:
		{
		  /* �������ݵ���GSMģ����Ϣ���������� */
			memcpy(((uint8_t*)(&ConfigService.Gsm)+adder),config->Data,len);
			/* ���ؽ�����ַ */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* ��������Ϣ */
		case CSCC_Sensor:
		{
			/* ���ô�������Ϣ */
			/* �������ݵ�����������Ϣ���������� */
			memcpy(((uint8_t*)(&ConfigService.Sensor)+adder),config->Data,len);
			/* ���ؽ�����ַ */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		/* �˵���Ϣ */
		case CSCC_Menu:
		{
			/* ���ò˵���Ϣ */
			/* �������ݵ����˵���Ϣ���������� */
			memcpy(((uint8_t*)(&ConfigService.Menu)+adder),config->Data,len);
			/* ���ؽ�����ַ */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
	
		/* ������Ϣ */
		case CSCC_Update:
		{
			/* ����������Ϣ */
			/* �������ݵ���������Ϣ���������� */
			memcpy(((uint8_t*)(&ConfigService.Update)+adder),config->Data,len);
						if(ConfigService.Update.Flag==0)
						{
							  if(GSM_DEBUG_Flag ==TRUE)
								{
								 printf("���÷������·���ȡ��Զ������������Ϣ\r\n");	
								 IAP_App.RunState = FALSE;
								}
						}else if(ConfigService.Update.Flag==1)
						{
								if(GSM_DEBUG_Flag ==TRUE)
						    printf("���÷������·���Զ������������Ϣ\r\n");
						}
			/* ���ؽ�����ַ */
			return ((ConfigServiceComContentStruct*)(contentEndp+5+len));
		}
		
		default:
		{
			/* ��ȡ��ͨ��Ŀ�겻���� */

			return FALSE;
		}
	}
}

/************************************************************************************************/
/* ������	: ConfigService_ParseData															*/
/* ����  	: ��								 												*/
/* ���		: ��		 																		*/
/* ����	 	: ���÷�������������								 									*/
/* �������� 	: 2016/08/16																		*/
/* �޸����� 	: 2016/08/16																		*/
/*   �汾 	: V0: ����																			*/
/************************************************************************************************/
static BOOL ConfigService_ParseData(uint8_t* datap,int32_t len,BOOL* comContentType,uint8_t* cmdtype)
{
	/* ��Ϣͷָ�� */
	ConfigServiceComStruct              *ConfigService_parap;
	/* �������ݼĴ��� */                
	ConfigServiceComContentStruct       *configservicecomcontent;
	/* У��λָ�� */
	uint8_t                             *checkp;
	/* У������ */
	uint8_t                             check = 0;
	/* ������ */
	uint32_t                            num;
	
	/* 1������ת������ */
	/* �Ƚ��������������(��ʡ���ݿռ�) */
	memset(ConfigService_TXDR,0X00, sizeof(ConfigService_TXDR));
	/* ��ת�������ݴ浽������ */
	len = GPRS_Network_To_Host_Translator_Data(ConfigService_TXDR,datap,len);
	/* ��ս���������(��ʡ���ݿռ�) */
	memset(ConfigService_RXDR,0X00, sizeof(ConfigService_RXDR));
	/* ��������������ת�Ƶ����������� */
	memcpy(datap,ConfigService_TXDR,len);
	
	/* 2���ж����ݺϷ��� */
	/* ��������ָ�� */
	ConfigService_parap = (ConfigServiceComStruct *)ConfigService_TXDR;
	/* a���Ƚ�SN���Ƿ�һ�� */
	if(memcmp(ConfigService_parap->SN_NO,
			 (uint8_t*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),
			  6) != 0)
	{
		/* SN�Ų�һ�� */
		return FALSE;
	}
	
	/* b������У�� */
	for(num = 1 ; num < (len - 2); num++)
	{
		/* �� ��Ϣͷ �� ��Ϣ�� ���һ�� ���� ��� */
		check ^= ConfigService_TXDR[num];
	}
	
	/* ����У��ָ�� */
	checkp = &ConfigService_TXDR[len-2];
	/* �ж�У���Ƿ���ͬ */
	if(check != *checkp)
	{
		/* У�鲻��ͬ */
		return FALSE;
	}
	
	/* c����Ϣ������ */
	if(ConfigService_parap->Body_Type != 0xFB)
	{
		/* ��Ϣ�����Ͳ������÷��������� */
		return FALSE;
	}
	
	/* ���ò������� */
	*cmdtype = ConfigService_parap->Cmd_Type;
	
	/* ���ò�������ָ�� */
	configservicecomcontent = (ConfigServiceComContentStruct*)((&ConfigService_parap->Num)+1);
	
	/* 3�������������� */
	for(num = 0 ; num < ConfigService_parap->Num; num++)
	{
		/* �жϵ�ǰ������ģ�����͡��Ƿ�Ϊ�հ� */
		if(configservicecomcontent->Type != CSCC_Null)
		{
			/* ���� �������� Ϊ�ǿղ��� */
			*comContentType = TRUE;
		}
		
		/* �жϵ�ǰ���յĲ������� */
		if(ConfigService_parap->Cmd_Type != 0x02)
		{
			/* �������Ͳ�Ϊͬ�����ͣ�ֻ��Ҫ�����ݽ�����֤����Ҫ���ò����� */
			configservicecomcontent = ConfigService_UpdataConfig(configservicecomcontent);	
		}
	}
	
	/* �жϵ�ǰ������ģ�����͡��Ƿ�Ϊ�հ� */
	if((*comContentType != TRUE) || (ConfigService_parap->Cmd_Type == 0x02))
	{
		/* �հ����ݽ����ɹ� */
		return TRUE;
	}
	
	/* 4�����ûظ����� */
	/* ��������ָ�� */
	ConfigService_parap = (ConfigServiceComStruct *)datap;
	/* ������ϢID */
	ConfigService_parap->ID_Type_HSB = ConfigServiceComFix.ID_Type_HSB;
	ConfigService_parap->ID_Type_LSB = ConfigServiceComFix.ID_Type_LSB;
	/* ���ò������� */
	ConfigService_parap->Cmd_Type = 0x02;
	/* ����У�� */
	check ^= (ConfigService_parap->ID_Type_HSB^
			      ConfigService_parap->ID_Type_LSB^
			      ConfigService_parap->Cmd_Type^
			      0x89^0x00^0x01);
	/* ����У�� */
	datap[len-2] = check;
	/* ת�� *//* ���GPRS�������ݳ��� */
	num = GPRS_Host_To_Network_Translator_Data(
		 &ConfigService_TXDR[1],&datap[1],(len-2));
	/* �������ݳ��� */
	len = num+1;
	/* ����β��ʶ */
	ConfigService_TXDR[len] =  0X7E;
	/* ��ս��������� */
	memset(ConfigService_RXDR,0X00, sizeof(ConfigService_RXDR));
	/* ����GPRS�������ݲ��� */
	GPRS.ConfigService.Para.TXDR	= ConfigService_TXDR;
	GPRS.ConfigService.Para.TXLen	= len+1;//(����1�ֽڽ���������)
	GPRS.ConfigService.Para.RXDR	= ConfigService_RXDR;
	GPRS.ConfigService.Para.RXLen	= 0;
	
	/* �ǿ����ݽ����ɹ� */
	return TRUE;
}

/************************************************************************************************/
/* ������	: ConfigService_UpdataPara												 			*/
/* ����  	: ��			 																	*/
/* ���		: ��		 																		*/
/* ����	 	: ��������������Ϣ								 									*/
/* �������� 	: 2016/08/19																		*/
/* �޸����� 	: 2016/08/19																		*/
/*   �汾 	: V0: ����																			*/
/************************************************************************************************/
void ConfigService_UpdataPara(INSTRU_CONFIG_INF_TAB* scrPara)//���ķ������·������ò�������
{
	/* �������ݼĴ��� */
	NetworkConfigStruct network;
	/* �����˿� */
	uint32_t updateport;
	/* ���� ϵͳ��Ϣ *///ϵͳ��Ϣ���ɸ���
	if(memcmp(ConfigService.System.SerialNumber.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number,
			  sizeof(ConfigService.System.SerialNumber)) != 0)
	{
		/* SN��һ��,˵�������� */
		__nop();
	}
	if(memcmp(ConfigService.System.HardwareVersion.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number,
			  sizeof(ConfigService.System.HardwareVersion)) != 0)
	{
		/* Ӳ���汾�Ų�һ��,˵�������� */
		__nop();
	}
	if(memcmp(ConfigService.System.SoftwareVersion.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Software_Version.Serial_Number,
			  sizeof(ConfigService.System.SoftwareVersion)) != 0)
	{
		/* ����汾�Ų�һ��,˵�������� */
		__nop();
	}

	/* ���� ��Դ��Ϣ */
	if(ConfigService.Power.PowerState != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Link_State)
	{
		/* ��ӵ�Դ����״̬ ��һ�� */
		__nop();	
	}
   
	if((ConfigService.Power.PowerSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch)||
	   (ConfigService.Power.PowerSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch))
	{
		/* ��Դ���ⱨ�� ��һ�� */
		if(ConfigService.Power.PowerSLAlarmSwitch == 0)
		{
			scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch = FALSE;
			scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch = FALSE;
		}else
		{
			scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch = TRUE;
			scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch = TRUE;
		}	
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch));
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch));
	}
	
	if(ConfigService.Power.PowerSMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch)
	{
		/* ��Դ���ű��� ��һ�� */
		scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch = 
		ConfigService.Power.PowerSMSAlarmSwitch;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch));
	}
	
//	if(ConfigService.Power.PowerChargeState != 
//	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Charge_State)
//	{
//		/* ״̬��Ϣ�������� */
//	}
//	
//	if(ConfigService.Power.PowerQuantityState != 
//	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Quantity_State)
//	{
//		/* ״̬��Ϣ�������� */
//	}
	
	if(ConfigService.Power.BTAlarmSwitch != TRUE)
	{
		/* ��ӵ�Դ�������� ��һ�� */
		__nop();
	}
	
	if((ConfigService.Power.BTSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch)||
	   (ConfigService.Power.BTSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch))
	{
		/* ��Դ���ⱨ�� ��һ�� */
		if(ConfigService.Power.BTSLAlarmSwitch == FALSE)
		{
			scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch = FALSE;
			scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch = FALSE;
		}else
		{
			scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch = TRUE;
			scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch = TRUE;
		}
		
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch));
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch));
	}
	
	if(ConfigService.Power.BTSMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch)
	{
		/* ��Դ���ű��� ��һ�� */
		scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch = 
		ConfigService.Power.BTSMSAlarmSwitch;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch,
		sizeof(scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch));
	}
	
	/* ��ʼ�� ���÷����� ��������Ϣ */
#if (1)
	/* ̽ͷ 1 */
	if(ConfigService.Sensor[0].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type = 
		ConfigService.Sensor[0].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type != Sensor_Null)
	{	
		if((ConfigService.Sensor[0].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[0].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if(ConfigService.Sensor[0].SLAlarmSwitch == FALSE)
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[0].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[0].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[0].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[0].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[0].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[0].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[0].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[0].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[0].SN));
	    }
		
		if(ConfigService.Sensor[0].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[0].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[0].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit = 
			ConfigService.Sensor[0].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[0].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit = 
			ConfigService.Sensor[0].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 2 */
	if(ConfigService.Sensor[1].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type = 
		ConfigService.Sensor[1].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type != Sensor_Null)
	{	
		if((ConfigService.Sensor[1].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[1].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[1].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[1].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[1].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[1].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[1].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[1].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[1].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[1].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[1].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[1].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[1].SN));
	    }
		
		if(ConfigService.Sensor[1].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[1].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[1].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit = 
			ConfigService.Sensor[1].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[1].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit = 
			ConfigService.Sensor[1].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit));	
		}
	}	
	
	/* ̽ͷ 3 */
	if(ConfigService.Sensor[2].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type = 
		ConfigService.Sensor[2].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[2].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[2].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[2].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[2].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[2].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[2].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[2].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[2].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[2].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[2].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[2].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[2].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[2].SN));
	    }
		
		if(ConfigService.Sensor[2].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[2].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[2].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit = 
			ConfigService.Sensor[2].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[2].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit = 
			ConfigService.Sensor[2].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 4 */
	if(ConfigService.Sensor[3].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type = 
		ConfigService.Sensor[3].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type != Sensor_Null)
	{	
		if((ConfigService.Sensor[3].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[3].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[3].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[3].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[3].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[3].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[3].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[3].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[3].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[3].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[3].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[3].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[3].SN));
	    }
		
		if(ConfigService.Sensor[3].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[3].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[3].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit = 
			ConfigService.Sensor[3].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[3].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit = 
			ConfigService.Sensor[3].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 5 */
	if(ConfigService.Sensor[4].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type = 
		ConfigService.Sensor[4].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[4].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[4].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[4].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[4].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[4].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[4].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[4].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[4].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[4].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[4].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[4].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[4].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[4].SN));
	    }
		
		if(ConfigService.Sensor[4].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[4].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[4].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit = 
			ConfigService.Sensor[4].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[4].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit = 
			ConfigService.Sensor[4].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 6 */
	if(ConfigService.Sensor[5].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type = 
		ConfigService.Sensor[5].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[5].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[5].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[5].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[5].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[5].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[5].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[5].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[5].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[5].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[5].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[5].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[5].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[5].SN));
	    }
		
		if(ConfigService.Sensor[5].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[5].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[5].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit = 
			ConfigService.Sensor[5].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[5].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit = 
			ConfigService.Sensor[5].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 7 */
	if(ConfigService.Sensor[6].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type = 
		ConfigService.Sensor[6].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[6].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[6].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[6].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[6].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[6].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[6].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[6].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[6].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[6].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[6].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[6].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[6].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[6].SN));
	    }
		
		if(ConfigService.Sensor[6].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[6].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[6].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit = 
			ConfigService.Sensor[6].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[6].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit = 
			ConfigService.Sensor[6].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 8 */
	if(ConfigService.Sensor[7].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type = 
		ConfigService.Sensor[7].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[7].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[7].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[7].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[7].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[7].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[7].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[7].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[7].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[7].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[7].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[7].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[7].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[7].SN));
	    }
		
		if(ConfigService.Sensor[7].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[7].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[7].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit = 
			ConfigService.Sensor[7].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[7].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit = 
			ConfigService.Sensor[7].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 9 */
	if(ConfigService.Sensor[8].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type = 
		ConfigService.Sensor[8].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[8].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[8].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[8].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[8].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[8].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[8].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[8].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[8].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[8].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[8].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[8].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[8].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[8].SN));
	    }
		
		if(ConfigService.Sensor[8].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[8].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[8].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit = 
			ConfigService.Sensor[8].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[8].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit = 
			ConfigService.Sensor[8].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 10 */
	if(ConfigService.Sensor[9].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type = 
		ConfigService.Sensor[9].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[9].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[9].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[9].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[9].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[9].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[9].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[9].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[9].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[9].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[9].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[9].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[9].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[9].SN));
	    }
		
		if(ConfigService.Sensor[9].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[9].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[9].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit = 
			ConfigService.Sensor[9].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[9].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit = 
			ConfigService.Sensor[9].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 11 */
	if(ConfigService.Sensor[10].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type = 
		ConfigService.Sensor[10].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[10].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[10].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[10].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[10].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[10].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[10].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[10].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[10].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[10].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[10].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[10].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[10].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[10].SN));
	    }
		
		if(ConfigService.Sensor[10].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[10].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[10].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit = 
			ConfigService.Sensor[10].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[10].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit = 
			ConfigService.Sensor[10].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 12 */
	if(ConfigService.Sensor[11].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type = 
		ConfigService.Sensor[11].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[11].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[11].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[11].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[11].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[11].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[11].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[11].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[11].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[11].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[11].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[11].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[11].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[11].SN));
	    }
		
		if(ConfigService.Sensor[11].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[11].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[11].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit = 
			ConfigService.Sensor[11].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[11].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit = 
			ConfigService.Sensor[11].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 13 */
	if(ConfigService.Sensor[12].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type = 
		ConfigService.Sensor[12].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[12].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[12].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[12].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[12].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[12].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[12].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[12].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[12].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[12].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[12].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[12].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[12].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[12].SN));
	    }
		
		if(ConfigService.Sensor[12].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[12].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[12].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit = 
			ConfigService.Sensor[12].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[12].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit = 
			ConfigService.Sensor[12].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 14 */
	if(ConfigService.Sensor[13].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type = 
		ConfigService.Sensor[13].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[13].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[13].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[13].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[13].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[13].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[13].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[13].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[13].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[13].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[13].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[13].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[13].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[13].SN));
	    }
		
		if(ConfigService.Sensor[13].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[13].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[13].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit = 
			ConfigService.Sensor[13].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[13].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit = 
			ConfigService.Sensor[13].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 15 */
	if(ConfigService.Sensor[14].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type = 
		ConfigService.Sensor[14].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[14].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[14].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[14].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[14].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[14].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[14].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[14].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[14].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[14].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[14].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[14].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[14].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[14].SN));
	    }
		
		if(ConfigService.Sensor[14].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[14].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[14].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit = 
			ConfigService.Sensor[14].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[14].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit = 
			ConfigService.Sensor[14].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 16 */
	if(ConfigService.Sensor[15].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type = 
		ConfigService.Sensor[15].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[15].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[15].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[15].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[15].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[15].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[15].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[15].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[15].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[15].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[15].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[15].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[15].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[15].SN));
	    }
		
		if(ConfigService.Sensor[15].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[15].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[15].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit = 
			ConfigService.Sensor[15].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[15].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit = 
			ConfigService.Sensor[15].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 17 */
	if(ConfigService.Sensor[16].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type = 
		ConfigService.Sensor[16].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[16].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[16].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[16].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[16].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[16].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[16].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[16].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[16].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[16].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[16].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[16].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[16].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[16].SN));
	    }
		
		if(ConfigService.Sensor[16].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[16].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[16].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit = 
			ConfigService.Sensor[16].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[16].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit = 
			ConfigService.Sensor[16].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 18 */
	if(ConfigService.Sensor[17].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type = 
		ConfigService.Sensor[17].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[17].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[17].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[17].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[17].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[17].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[17].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[17].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[17].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[17].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[17].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[17].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[17].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[17].SN));
	    }
		
		if(ConfigService.Sensor[17].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[17].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[17].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit = 
			ConfigService.Sensor[17].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[17].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit = 
			ConfigService.Sensor[17].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 19 */
	if(ConfigService.Sensor[18].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type = 
		ConfigService.Sensor[18].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[18].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[18].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[18].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[18].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[18].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[18].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[18].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[18].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[18].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[18].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[18].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[18].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[18].SN));
	    }
		
		if(ConfigService.Sensor[18].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[18].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[18].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit = 
			ConfigService.Sensor[18].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[18].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit = 
			ConfigService.Sensor[18].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 20 */
	if(ConfigService.Sensor[19].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type = 
		ConfigService.Sensor[19].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[19].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[19].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[19].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[19].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[19].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[19].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[19].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[19].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[19].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[19].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[19].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[19].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[19].SN));
	    }
		
		if(ConfigService.Sensor[19].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[19].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[19].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit = 
			ConfigService.Sensor[19].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[19].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit = 
			ConfigService.Sensor[19].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 21 */
	if(ConfigService.Sensor[20].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type = 
		ConfigService.Sensor[20].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[20].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[20].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[20].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[20].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[20].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[20].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[20].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[20].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[20].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[20].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[20].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[20].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[20].SN));
	    }
		
		if(ConfigService.Sensor[20].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[20].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[20].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit = 
			ConfigService.Sensor[20].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[20].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit = 
			ConfigService.Sensor[20].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 22 */
	if(ConfigService.Sensor[21].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type = 
		ConfigService.Sensor[21].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[21].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[21].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[21].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[21].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[21].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[21].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[21].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[21].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[21].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[21].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[21].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[21].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[21].SN));
	    }
		
		if(ConfigService.Sensor[21].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[21].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[21].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit = 
			ConfigService.Sensor[21].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[21].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit = 
			ConfigService.Sensor[21].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 23 */
	if(ConfigService.Sensor[22].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type = 
		ConfigService.Sensor[22].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[22].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[22].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[22].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[22].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[22].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[22].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[22].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[22].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[22].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[22].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[22].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[22].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[22].SN));
	    }
		
		if(ConfigService.Sensor[22].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[22].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[22].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit = 
			ConfigService.Sensor[22].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[22].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit = 
			ConfigService.Sensor[22].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 24 */
	if(ConfigService.Sensor[23].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type = 
		ConfigService.Sensor[23].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[23].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[23].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[23].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[23].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[23].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[23].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[23].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[23].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[23].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[23].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[23].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[23].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[23].SN));
	    }
		
		if(ConfigService.Sensor[23].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[23].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[23].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit = 
			ConfigService.Sensor[23].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[23].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit = 
			ConfigService.Sensor[23].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 25 */
	if(ConfigService.Sensor[24].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type = 
		ConfigService.Sensor[24].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[24].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[24].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[24].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[24].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[24].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[24].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[24].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[24].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[24].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[24].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[24].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[24].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[24].SN));
	    }
		
		if(ConfigService.Sensor[24].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[24].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[24].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit = 
			ConfigService.Sensor[24].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[24].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit = 
			ConfigService.Sensor[24].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 26 */
	if(ConfigService.Sensor[25].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type = 
		ConfigService.Sensor[25].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[25].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[25].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[25].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[25].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[25].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[25].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[25].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[25].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[25].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[25].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[25].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[25].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[25].SN));
	    }
		
		if(ConfigService.Sensor[25].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[25].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[25].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit = 
			ConfigService.Sensor[25].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[25].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit = 
			ConfigService.Sensor[25].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 27 */
	if(ConfigService.Sensor[26].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type = 
		ConfigService.Sensor[26].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[26].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[26].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[26].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[26].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[26].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[26].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[26].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[26].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[26].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[26].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[26].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[26].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[26].SN));
	    }
		
		if(ConfigService.Sensor[26].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[26].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[26].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit = 
			ConfigService.Sensor[26].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[26].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit = 
			ConfigService.Sensor[26].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 28 */
	if(ConfigService.Sensor[27].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type = 
		ConfigService.Sensor[27].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[27].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[27].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[27].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[27].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[27].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[27].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[27].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[27].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[27].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[27].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[27].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[27].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[27].SN));
	    }
		
		if(ConfigService.Sensor[27].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[27].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[27].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit = 
			ConfigService.Sensor[27].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[27].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit = 
			ConfigService.Sensor[27].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 29 */
	if(ConfigService.Sensor[28].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type = 
		ConfigService.Sensor[28].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[28].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[28].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[28].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[28].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[28].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[28].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[28].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[28].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[28].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[28].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[28].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[28].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[28].SN));
	    }
		
		if(ConfigService.Sensor[28].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[28].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[28].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit = 
			ConfigService.Sensor[28].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[28].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit = 
			ConfigService.Sensor[28].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 30 */
	if(ConfigService.Sensor[29].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type = 
		ConfigService.Sensor[29].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[29].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[29].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[29].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[29].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[29].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[29].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[29].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[29].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[29].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[29].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[29].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[29].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[29].SN));
	    }
		
		if(ConfigService.Sensor[29].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[29].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[29].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit = 
			ConfigService.Sensor[29].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[29].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit = 
			ConfigService.Sensor[29].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 31 */
	if(ConfigService.Sensor[30].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type = 
		ConfigService.Sensor[30].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[30].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[30].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[30].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[30].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[30].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[30].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[30].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[30].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[30].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[30].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[30].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[30].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[30].SN));
	    }
		
		if(ConfigService.Sensor[30].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[30].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[30].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit = 
			ConfigService.Sensor[30].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[30].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit = 
			ConfigService.Sensor[30].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit));	
		}
	}
	
	/* ̽ͷ 32 */
	if(ConfigService.Sensor[31].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type)
	{
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type = 
		ConfigService.Sensor[31].SensorType;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type,
		sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type));
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[31].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[31].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch))
		{
			/* ��Դ���ⱨ�� ��һ�� */
			if((ConfigService.Sensor[31].SLAlarmSwitch == 0)&&
			   (ConfigService.Sensor[31].SLAlarmSwitch == 0))
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch = FALSE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch = FALSE;
			}else
			{
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch = TRUE;
				scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch = TRUE;
			}
			
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch));
		}
		
		if(ConfigService.Sensor[31].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch = 
			ConfigService.Sensor[31].SMSAlarmSwitch;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch));
		}
		
		if(memcmp(ConfigService.Sensor[31].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[31].SN)) != 0)
	    {
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[0] = 
			ConfigService.Sensor[31].SN[0];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[1] = 
			ConfigService.Sensor[31].SN[1];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[2] = 
			ConfigService.Sensor[31].SN[2];
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[3] = 
			ConfigService.Sensor[31].SN[3];
			/* ���´洢���� */
			Write_Config_Tab((u8*)scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			sizeof(ConfigService.Sensor[31].SN));
	    }
		
		if(ConfigService.Sensor[31].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO = 
			ConfigService.Sensor[31].ID;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO));	
		}
		
		if(ConfigService.Sensor[31].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit = 
			ConfigService.Sensor[31].UpLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit));	
		}
		
		if(ConfigService.Sensor[31].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit)
		{
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit = 
			ConfigService.Sensor[31].LowLimit;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit,
			sizeof(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit));	
		}
	}
	
#endif
	
	/* ��ʼ�� ���÷����� �˵���Ϣ */
	if(ConfigService.Menu.SLAlarmSwitch !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)
	{
		/* ��Դ���ű��� ��һ�� */
		scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch = 
		ConfigService.Menu.SLAlarmSwitch;
		scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch = 
		ConfigService.Menu.SLAlarmSwitch;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch));
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch));
	}
	
	if(ConfigService.Menu.SMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)
	{
		/* ��Դ���ű��� ��һ�� */
		scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch = 
		ConfigService.Menu.SMSAlarmSwitch;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch));
	}
	
	if(ConfigService.Menu.SMSAlarm_RepeatTime !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval)
	{
		/* ��Դ���ű��� ��һ�� */
		scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval = 
		ConfigService.Menu.SMSAlarm_RepeatTime;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval));
	}
	
	if(ConfigService.Menu.SaveInterval_Alarm !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval)
	{
		/* ��Դ���ű��� ��һ�� */
		scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval = 
		ConfigService.Menu.SaveInterval_Alarm;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval));
	}
	
	if(ConfigService.Menu.SaveInterval_Normal !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval)
	{
		/* ��Դ���ű��� ��һ�� */
		scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval = 
		ConfigService.Menu.SaveInterval_Normal;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval,
		sizeof(scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval));
	}
	
	/* ��ʼ�� ���÷����� ������Ϣ */
	/* �жϹ̼�����״̬ */	/* ���� ������Ϣ */
	if(ConfigService.Update.Flag != 
	   scrPara->Inf_State.Instru_System.Inf.UpdateFlag)
	{
		/* �̼�����״̬ ��һ�� */
		scrPara->Inf_State.Instru_System.Inf.UpdateFlag = 
		ConfigService.Update.Flag;
		
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_System.Inf.UpdateFlag,
		sizeof(scrPara->Inf_State.Instru_System.Inf.UpdateFlag));
	}
/************************�ж�ƽ̨�Ƿ��޸������ݷ�������IP��ַ���������˿�****************************/
		/* �ж����ݷ�����IP�Ƿ���ͬ */
		if(memcmp(&ConfigService.Gsm.Data_IP_Address,
				  &scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1,sizeof(ConfigService.Gsm.Data_IP_Address)) != 0)
		{
			/*�������ݷ�����IP��ַ*/
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1 = ConfigService.Gsm.Data_IP_Address;
			/* ������ݷ��������� */
			memset(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,0x00,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));
			//ͬ��GSM����������
//			/* ��������GSMģ�� */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			  GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
		}
		else if(memcmp(ConfigService.Gsm.DataDomainName,
			   scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1)) != 0)/* ���� */
		{
			/* ����������Ϣ */
			memcpy(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			   ConfigService.Gsm.DataDomainName,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));
			/* ���IP */
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D = 0;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));
			//ͬ��GSM����������
//			/* ��������GSMģ�� */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
		}
		/*�ж��Ƿ�������ݷ������˿���Ϣ */
	if(ConfigService.Gsm.DataNetworkPort != scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1)
	{
		/* �˿ں� ��һ�� */
		scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1 =htons(ConfigService.Gsm.DataNetworkPort);
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1,
		sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1));
//		/* ��������GSMģ�� */
//		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
	}
	
	/* �ж����÷�����IP�Ƿ���ͬ */
	if(memcmp(&ConfigService.Gsm.Config_IP_Address,
				  &scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2,sizeof(ConfigService.Gsm.Config_IP_Address)) != 0)
		{
			/*�������÷�����IP��ַ*/
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2 = ConfigService.Gsm.Config_IP_Address;
			/* ������÷��������� */
			memset(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,0x00,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));
			//ͬ��GSM����������
//			/* ��������GSMģ�� */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
		}
		else if(memcmp(ConfigService.Gsm.ConfigDomainName,
			   scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2)) != 0)/* ���� */
		{
			/* ����������Ϣ */
			memcpy(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			   ConfigService.Gsm.ConfigDomainName,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));
			/* ���IP */
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D = 0;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));
			//ͬ��GSM����������
//			/* ��������GSMģ�� */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
		}
		/*�ж��Ƿ�������÷������˿���Ϣ */
	if(ConfigService.Gsm.ConfigNetworkPort != scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2)
	{
		/* �˿ں� ��һ�� */
		scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2 = htons(ConfigService.Gsm.ConfigNetworkPort);
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2,
		sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2));
//		/* ��������GSMģ�� */
//		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
	}	
/************************���ű�������**********************************************/	
			/*�ж��Ƿ���¶��ű���������Ϣ */
	if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO1,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO1))!=0)
		 {
			 /*�������ź���1 ��һ�� */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1=ConfigService.Gsm.Alarm_SMS_NO1;
		  /* ���´洢���� */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1));
		 }
	if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO2,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO2))!=0)
		 {
			 /*�������ź���2 ��һ�� */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2=ConfigService.Gsm.Alarm_SMS_NO2;
		  /* ���´洢���� */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2));
		 }
  if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO3,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO3))!=0)
		 {
			 /*�������ź���3��һ�� */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3=ConfigService.Gsm.Alarm_SMS_NO3;
		  /* ���´洢���� */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3));
		 }
	if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO4,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO4))!=0)
		 {
			 /*�������ź���4 ��һ�� */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4=ConfigService.Gsm.Alarm_SMS_NO4;
		  /* ���´洢���� */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4));
		 }	
  if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO5,&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5,
		 sizeof(ConfigService.Gsm.Alarm_SMS_NO5))!=0)
		 {
			 /*�������ź���4 ��һ�� */
		  scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5=ConfigService.Gsm.Alarm_SMS_NO5;
		  /* ���´洢���� */
		  Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5,
		  sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5));
		 }
		 
/***************************ͨ�����÷���������������������ַzwc**************************************/	
	/* �жϵ�ǰ������(IP/����) */
	if(TRUE == GSM_Config_Network(ConfigService.Update.Network,&network))
	{
		/* IP */
		/* �ж�IP�Ƿ���ͬ */
		if(memcmp(&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3,
				  &network.IP,1) != 0)
		{
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A = 
			network.IP.Serial_Number_Number.Address_A;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B = 
			network.IP.Serial_Number_Number.Address_B;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C = 
			network.IP.Serial_Number_Number.Address_C;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D = 
			network.IP.Serial_Number_Number.Address_D;
			/* ������� */
			memset(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,0x00,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));
////			//ͬ��GSM����������
////			/* ��������GSMģ�� */
////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
		}		
	}else
	{
		/* ���� */
		if(memcmp(ConfigService.Update.Network,
			   scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3)) != 0)
		{
			/* ����������Ϣ */
			memcpy(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
			   ConfigService.Update.Network,
			   sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));
			/* ���IP */
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C = 0;
			scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D = 0;
			/* ���´洢���� */
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID3));
			Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
			sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));
//////			//ͬ��GSM����������
//////			/* ��������GSMģ�� */
//////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
		}
	}

	/* ���������̼��˿ں� */
	updateport = (((uint32_t)ConfigService.Update.Port_HSB)<<8)+
							 ConfigService.Update.Port_LSB;
	/* ���������̼��˿���Ϣ */
	if(updateport != scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID3)
	{
		/* �˿ں� ��һ�� */
		scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID3 = updateport;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID3,
		sizeof(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID3));
//		/* ��������GSMģ�� */
//		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//���÷���������
	}
	

}

/************************************************************************************************/
/* ������	: ConfigService_CheckParaModify												 		*/
/* ����  	: scrPara:���ò���ָ��.			 													*/
/* ���		: �޸Ĳ�����Ŀ.		 																*/
/* ����	 	: ��鵱ǰ��������ѡ�����޸���.								 						*/
/* �������� 	: 2016/08/22																		*/
/* �޸����� 	: 2016/08/22																		*/
/*   �汾 	: V0: ����																			*/
/************************************************************************************************/
static uint8_t ConfigService_CheckParaModify(INSTRU_CONFIG_INF_TAB* scrPara)
{
	/* ������Ŀ���ͼĴ��� */
	uint8_t item = 0;
	
	/* ��� ϵͳ��Ϣ */
	if(memcmp(ConfigService.System.SerialNumber.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number,
			  sizeof(ConfigService.System.SerialNumber)) != 0)
	{
		/* SN��һ��,���� ϵͳ��Ϣ �޸��� */
		item  = item|ConfigService_UploadSystem;
		/* ����SN����Ϣ */
		memcpy(ConfigService.System.SerialNumber.Serial_Number,
			   scrPara->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number,
			   sizeof(ConfigService.System.SerialNumber));
	}
	
	if(memcmp(ConfigService.System.HardwareVersion.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number,
			  sizeof(ConfigService.System.HardwareVersion)) != 0)
	{
		/* Ӳ���汾�Ų�һ��,���� ϵͳ��Ϣ �޸��� */
		item  = item|ConfigService_UploadSystem;
		/* ���� Ӳ���汾�� */
		memcpy(ConfigService.System.HardwareVersion.Serial_Number,
			   scrPara->Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number,
			    sizeof(ConfigService.System.HardwareVersion));
	}

	if(memcmp(ConfigService.System.SoftwareVersion.Serial_Number,
			  scrPara->Inf_State.Instru_System.Inf.Software_Version.Serial_Number,
			  sizeof(ConfigService.System.SoftwareVersion)) != 0)
	{
		/* ����汾�Ų�һ��,���� ϵͳ��Ϣ �޸��� */
		item  = item|ConfigService_UploadSystem;
		/* ���� ����汾�� */
		memcpy(ConfigService.System.SoftwareVersion.Serial_Number,
			   scrPara->Inf_State.Instru_System.Inf.Software_Version.Serial_Number,
			    sizeof(ConfigService.System.SoftwareVersion));
	}
	
	/* ��� ��Դ��Ϣ */
	if((ConfigService.Power.PowerSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch)||
	   (ConfigService.Power.PowerSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch))
	{
		/* ��Դ���ⱨ����һ��,���� ��Դ��Ϣ �޸��� */
		item  = item|ConfigService_UploadPower;
		/* ���õ�Դ���ⱨ������ */
		if((scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch == FALSE)&&
		   (scrPara->Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch == FALSE))
		{
			ConfigService.Power.PowerSLAlarmSwitch = FALSE;
		}else
		{
			ConfigService.Power.PowerSLAlarmSwitch = TRUE;
		}
	}
	
	if(ConfigService.Power.PowerSMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch)
	{
		/* ��Դ���ű�����һ��,���� ��Դ��Ϣ �޸��� */
		item  = item|ConfigService_UploadPower;
		/* ���õ�Դ���ű������� */
		if(scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch == FALSE)
		{
			/* ���õ�Դ���ű������� */
			ConfigService.Power.PowerSMSAlarmSwitch = FALSE;
		}else
		{
			/* ���õ�Դ���ű������� */
			ConfigService.Power.PowerSMSAlarmSwitch = TRUE;
		}
	}
	
//	if(ConfigService.Power.PowerChargeState != 
//	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Charge_State)
//	{
//		/* ״̬��Ϣ�������� */
//	}
//	
//	if(ConfigService.Power.PowerQuantityState != 
//	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Quantity_State)
//	{
//		/* ״̬��Ϣ�������� */
//	}
	
	if(ConfigService.Power.BTAlarmSwitch != TRUE)
	{
		/* ��ر������� ��һ�� */
		__nop();
	}
	
	if((ConfigService.Power.BTSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch)||
	   (ConfigService.Power.BTSLAlarmSwitch != 
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch))
	{
		/* ������ⱨ����һ��,���� ��Դ��Ϣ �޸��� */
		item  = item|ConfigService_UploadPower;
		/* ���õ�����ⱨ������ */
		if((scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch == FALSE)&&
		   (scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch == FALSE))
		{
			ConfigService.Power.BTSLAlarmSwitch = FALSE;
		}else
		{
			ConfigService.Power.BTSLAlarmSwitch = TRUE;
		}
	}
	
	if(ConfigService.Power.BTSMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch)
	{
		/* ��ض��ű�����һ��,���� ��Դ��Ϣ �޸��� */
		item  = item|ConfigService_UploadPower;
		/* ���õ�ض��ű������� */
		if(scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch == FALSE)
		{
			/* ���õ�Դ���ű������� */
			ConfigService.Power.BTSMSAlarmSwitch = FALSE;
		}else
		{
			/* ���õ�Դ���ű������� */
			ConfigService.Power.BTSMSAlarmSwitch = TRUE;
		}
	}

	/* ��ʼ�� ���÷����� ��������Ϣ */
#if (1)
	/* ̽ͷ 1 */
	if(ConfigService.Sensor[0].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[0].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[0].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[0].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[0].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[0].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[0].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[0].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[0].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[0].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[0].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[0].SN));
	    }
		
		if(ConfigService.Sensor[0].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[0].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[0].ID));
		}
		
		if(ConfigService.Sensor[0].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[0].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[0].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[0].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit;
		}
	}
	
	/* ̽ͷ 2 */
	if(ConfigService.Sensor[1].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[1].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[1].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[1].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((FALSE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch)&&
			   (FALSE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[1].SLAlarmSwitch = FALSE;
			}else if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch)&&
					 (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[1].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[1].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[1].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[1].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[1].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[1].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[1].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[1].SN));
	    }
		
		if(ConfigService.Sensor[1].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[1].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[1].ID));
		}
		
		if(ConfigService.Sensor[1].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[1].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[1].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[1].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 3 */
	if(ConfigService.Sensor[2].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[2].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[2].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[2].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[2].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[2].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[2].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[2].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[2].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[2].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[2].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[2].SN));
	    }
		
		if(ConfigService.Sensor[2].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[2].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[2].ID));
		}
		
		if(ConfigService.Sensor[2].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[2].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[2].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[2].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 4 */
	if(ConfigService.Sensor[3].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[3].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[3].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[3].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[3].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[3].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[3].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[3].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[3].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[3].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[3].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[3].SN));
	    }
		
		if(ConfigService.Sensor[3].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[3].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[3].ID));
		}
		
		if(ConfigService.Sensor[3].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[3].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[3].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[3].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit;
		}
	}
	
	/* ̽ͷ 5 */
	if(ConfigService.Sensor[4].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[4].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[4].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[4].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[4].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[4].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[4].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[4].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[4].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[4].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[4].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[4].SN));
	    }
		
		if(ConfigService.Sensor[4].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[4].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[4].ID));
		}
		
		if(ConfigService.Sensor[4].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[4].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[4].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[4].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 6 */
	if(ConfigService.Sensor[5].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[5].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[5].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[5].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[5].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[5].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[5].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[5].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[5].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[5].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[5].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[5].SN));
	    }
		
		if(ConfigService.Sensor[5].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[5].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[5].ID));
		}
		
		if(ConfigService.Sensor[5].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[5].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[5].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[5].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 7 */
	if(ConfigService.Sensor[6].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[6].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type != Sensor_Null)
	{	
		if((ConfigService.Sensor[6].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[6].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[6].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[6].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[6].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[6].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[6].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[6].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[6].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[6].SN));
	    }
		
		if(ConfigService.Sensor[6].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[6].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[6].ID));
		}
		
		if(ConfigService.Sensor[6].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[6].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[6].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[6].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 8 */
	if(ConfigService.Sensor[7].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[7].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type != Sensor_Null)
	{

		if((ConfigService.Sensor[7].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[7].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[7].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[7].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[7].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[7].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[7].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[7].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[7].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[7].SN));
	    }
		
		if(ConfigService.Sensor[7].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[7].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[7].ID));
		}
		
		if(ConfigService.Sensor[7].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[7].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[7].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[7].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 9 */
	if(ConfigService.Sensor[8].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[8].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[8].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[8].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[8].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[8].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[8].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[8].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[8].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[8].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[8].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[8].SN));
	    }
		
		if(ConfigService.Sensor[8].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[8].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[8].ID));
		}
		
		if(ConfigService.Sensor[8].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[8].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[8].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[8].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 10 */
	if(ConfigService.Sensor[9].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[9].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[9].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[9].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[9].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[9].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[9].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[9].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[9].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[9].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[9].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[9].SN));
	    }
		
		if(ConfigService.Sensor[9].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[9].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[9].ID));
		}
		
		if(ConfigService.Sensor[9].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[9].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[9].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[9].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit;
		}
	}
	
	/* ̽ͷ 11 */
	if(ConfigService.Sensor[10].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[10].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[10].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[10].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[10].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[10].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[10].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[10].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[10].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[10].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[10].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[10].SN));
	    }
		
		if(ConfigService.Sensor[10].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[10].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[10].ID));
		}
		
		if(ConfigService.Sensor[10].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[10].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[10].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[10].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit;
		}
	}
	
	/* ̽ͷ 12 */
	if(ConfigService.Sensor[11].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[11].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[11].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[11].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[11].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[11].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[11].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[11].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[11].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[11].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[11].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[11].SN));
	    }
		
		if(ConfigService.Sensor[11].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[11].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[11].ID));
		}
		
		if(ConfigService.Sensor[11].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[11].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[11].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[11].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 13 */
	if(ConfigService.Sensor[12].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[12].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[12].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[12].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[12].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[12].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[12].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[12].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[12].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[12].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[12].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[12].SN));
	    }
		
		if(ConfigService.Sensor[12].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[12].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[12].ID));
		}
		
		if(ConfigService.Sensor[12].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[12].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[12].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[12].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 14 */
	if(ConfigService.Sensor[13].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[13].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[13].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[13].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[13].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[13].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[13].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[13].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[13].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[13].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[13].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[13].SN));
	    }
		
		if(ConfigService.Sensor[13].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[13].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[13].ID));
		}
		
		if(ConfigService.Sensor[13].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[13].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[13].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[13].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit;
		}
	}	
	
	/* ̽ͷ 15 */
	if(ConfigService.Sensor[14].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[14].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[14].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[14].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[14].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[14].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[14].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[14].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[14].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[14].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[14].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[14].SN));
	    }
		
		if(ConfigService.Sensor[14].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[14].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[14].ID));
		}
		
		if(ConfigService.Sensor[14].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[14].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[14].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[14].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 16 */
	if(ConfigService.Sensor[15].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[15].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[15].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[15].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[15].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[15].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[15].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[15].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[15].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[15].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[15].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[15].SN));
	    }
		
		if(ConfigService.Sensor[15].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[15].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[15].ID));
		}
		
		if(ConfigService.Sensor[15].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[15].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[15].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[15].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 17 */
	if(ConfigService.Sensor[16].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[16].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[16].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[16].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[16].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[16].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[16].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[16].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[16].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[16].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[16].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[16].SN));
	    }
		
		if(ConfigService.Sensor[16].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[16].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[16].ID));
		}
		
		if(ConfigService.Sensor[16].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[16].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[16].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[16].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 18 */
	if(ConfigService.Sensor[17].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[17].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[17].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[17].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[17].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[17].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[17].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[17].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[17].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[17].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[17].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[17].SN));
	    }
		
		if(ConfigService.Sensor[17].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[17].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[17].ID));
		}
		
		if(ConfigService.Sensor[17].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[17].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[17].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[17].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 19 */
	if(ConfigService.Sensor[18].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[18].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[18].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[18].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[18].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[18].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[18].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[18].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[18].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[18].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[18].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[18].SN));
	    }
		
		if(ConfigService.Sensor[18].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[18].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[18].ID));
		}
		
		if(ConfigService.Sensor[18].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[18].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[18].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[18].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 20 */
	if(ConfigService.Sensor[19].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[19].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[19].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[19].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[19].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[19].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[19].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[19].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[19].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[19].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[19].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[19].SN));
	    }
		
		if(ConfigService.Sensor[19].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[19].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[19].ID));
		}
		
		if(ConfigService.Sensor[19].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[19].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[19].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[19].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 21 */
	if(ConfigService.Sensor[20].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[20].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[20].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[20].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[20].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[20].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[20].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[20].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[20].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[20].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[20].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[20].SN));
	    }
		
		if(ConfigService.Sensor[20].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[20].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[20].ID));
		}
		
		if(ConfigService.Sensor[20].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[20].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[20].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[20].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 22 */
	if(ConfigService.Sensor[21].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[21].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[21].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[21].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[21].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[21].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[21].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[21].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[21].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[21].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[21].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[21].SN));
	    }
		
		if(ConfigService.Sensor[21].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[21].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[21].ID));
		}
		
		if(ConfigService.Sensor[21].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[21].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[21].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[21].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 23 */
	if(ConfigService.Sensor[22].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[22].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[22].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[22].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[22].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[22].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[22].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[22].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[22].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[22].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[22].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[22].SN));
	    }
		
		if(ConfigService.Sensor[22].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[22].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[22].ID));
		}
		
		if(ConfigService.Sensor[22].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[22].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[22].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[22].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 24 */
	if(ConfigService.Sensor[23].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[23].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[23].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[23].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[23].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[23].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[23].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[23].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[23].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[23].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[23].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[23].SN));
	    }
		
		if(ConfigService.Sensor[23].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[23].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[23].ID));
		}
		
		if(ConfigService.Sensor[23].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[23].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[23].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[23].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 25 */
	if(ConfigService.Sensor[24].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[24].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[24].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[24].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[24].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[24].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[24].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[24].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[24].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[24].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[24].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[24].SN));
	    }
		
		if(ConfigService.Sensor[24].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[24].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[24].ID));
		}
		
		if(ConfigService.Sensor[24].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[24].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[24].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[24].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 26 */
	if(ConfigService.Sensor[25].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[25].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[25].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[25].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[25].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[25].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[25].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[25].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[25].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[25].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[25].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[25].SN));
	    }
		
		if(ConfigService.Sensor[25].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[25].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[25].ID));
		}
		
		if(ConfigService.Sensor[25].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[25].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[25].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[25].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 27 */
	if(ConfigService.Sensor[26].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[26].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[26].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[26].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[26].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[26].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[26].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[26].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[26].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[26].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[26].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[26].SN));
	    }
		
		if(ConfigService.Sensor[26].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[26].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[26].ID));
		}
		
		if(ConfigService.Sensor[26].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[26].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[26].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[26].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 28 */
	if(ConfigService.Sensor[27].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[27].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[27].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[27].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[27].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[27].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[27].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[27].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[27].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[27].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[27].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[27].SN));
	    }
		
		if(ConfigService.Sensor[27].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[27].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[27].ID));
		}
		
		if(ConfigService.Sensor[27].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[27].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[27].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[27].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 29 */
	if(ConfigService.Sensor[28].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[28].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[28].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[28].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[28].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[28].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[28].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[28].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[28].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[28].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[28].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[28].SN));
	    }
		
		if(ConfigService.Sensor[28].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[28].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[28].ID));
		}
		
		if(ConfigService.Sensor[28].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[28].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[28].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[28].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 30 */
	if(ConfigService.Sensor[29].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[29].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[29].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[29].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[29].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[29].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[29].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[29].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[29].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[29].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[29].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[29].SN));
	    }
		
		if(ConfigService.Sensor[29].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[29].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[29].ID));
		}
		
		if(ConfigService.Sensor[29].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[29].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[29].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[29].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 31 */
	if(ConfigService.Sensor[30].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[30].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[30].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[30].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[30].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[30].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[30].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[30].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[30].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[30].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[30].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[30].SN));
	    }
		
		if(ConfigService.Sensor[30].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[30].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[30].ID));
		}
		
		if(ConfigService.Sensor[30].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[30].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[30].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[30].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit;
		}
	}	
	/* ̽ͷ 32 */
	if(ConfigService.Sensor[31].SensorType != 
	   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type)
	{
		/* ̽ͷ���Ͳ�һ��,���� ��������Ϣ �޸��� */
		item  = item|ConfigService_UploadSensor;
		ConfigService.Sensor[31].SensorType = 
		(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type;
	}
	/* �ж�̽ͷ���� */
	if(scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type != Sensor_Null)
	{
		if((ConfigService.Sensor[31].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch)||
		   (ConfigService.Sensor[31].SLAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch))
		{
			/* ̽ͷ���ⱨ����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			if((TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch)&&
			   (TRUE == scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch))
			{
				ConfigService.Sensor[31].SLAlarmSwitch = TRUE;
			}else
			{
				ConfigService.Sensor[31].SLAlarmSwitch = FALSE;
			}
		}
		
		if(ConfigService.Sensor[31].SMSAlarmSwitch != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch)
		{
			/* ̽ͷ���ű�����һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[31].SMSAlarmSwitch = 
			scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch;
		}
		
		if(memcmp(ConfigService.Sensor[31].SN,
		scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
	    sizeof(ConfigService.Sensor[31].SN)) != 0)
	    {
			/* ̽ͷSN�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			memcpy(ConfigService.Sensor[31].SN,
				   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
				   sizeof(ConfigService.Sensor[31].SN));
	    }
		
		if(ConfigService.Sensor[31].ID != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO)
		{
			/* ̽ͷID�Ų�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;	
			memcpy((uint32_t*)&ConfigService.Sensor[31].ID,
				   (uint32_t*)&scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,
				   sizeof(ConfigService.Sensor[31].ID));
		}
		
		if(ConfigService.Sensor[31].UpLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[31].UpLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit;
		}
		
		if(ConfigService.Sensor[31].LowLimit != 
		   scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit)
		{
			/* ̽ͷ�������޲�һ��,���� ��������Ϣ �޸��� */
			item  = item|ConfigService_UploadSensor;
			ConfigService.Sensor[31].LowLimit = scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit;
		}
	}	
#endif	
	  /* ��ʼ�� ���÷����� �˵���Ϣ */
		/* ���ⱨ����������*/
	if(ConfigService.Menu.SLAlarmSwitch !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)
	{
		/* ���ⱨ���ܿ��ⲻһ��,���� �˵���Ϣ �޸��� */
		item  = item|ConfigService_UploadMenu;
		/* �������ⱨ���ܿ��� */
		if((FALSE == scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)&&
		   (FALSE == scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch))
		{
			ConfigService.Menu.SLAlarmSwitch = 
			scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
		}else if((TRUE == scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)&&
				 (TRUE == scrPara->Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch))
		{
			ConfigService.Menu.SLAlarmSwitch = 
			scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
		}
		/* �������ⱨ���ܿ��� */
		ConfigService.Menu.SLAlarmSwitch = 
		scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
	}
		/* ���ű�����������*/
	if(ConfigService.Menu.SMSAlarmSwitch !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)
	{
		/* ���ű����ܿ��ⲻһ��,���� �˵���Ϣ �޸��� */
		item  = item|ConfigService_UploadMenu;
		/* ���ö��ű����ܿ��� */
		ConfigService.Menu.SMSAlarmSwitch = 
		scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch;
	}
	
	if(ConfigService.Menu.SMSAlarm_RepeatTime !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval)
	{
		/* �����ظ�����ʱ������һ��,���� �˵���Ϣ �޸��� */
		item  = item|ConfigService_UploadMenu;
		/* ���ö��ű����ܿ��� */
		ConfigService.Menu.SMSAlarm_RepeatTime = 
		scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval;
	}
	
	if(ConfigService.Menu.SaveInterval_Alarm !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval)
	{
		/* �����洢�����һ��,���� �˵���Ϣ �޸��� */
		item  = item|ConfigService_UploadMenu;
		/* ���ñ����洢��� */
		ConfigService.Menu.SaveInterval_Alarm = 
		scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval;
	}
	
	if(ConfigService.Menu.SaveInterval_Normal !=
	   scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval)
	{

		/* �����洢�����һ��,���� �˵���Ϣ �޸��� */
		item  = item|ConfigService_UploadMenu;
		/* ���������洢��� */
		ConfigService.Menu.SaveInterval_Normal = 
		scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval;
	}
	
/*********************GSMģ����Ϣ***********************************/
	if(memcmp(&ConfigService.Gsm.Data_IP_Address,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1,4)!=0)
	{
		/*���ݷ�����ip��һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������ݷ�����IP��ַ*/
		 ConfigService.Gsm.Data_IP_Address=scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1;
	}
	if(memcmp(&ConfigService.Gsm.DataDomainName,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,sizeof(ConfigService.Gsm.DataDomainName))!=0)
	{
		/*���ݷ�����������һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������ݷ�����������ַ*/
		memcpy(ConfigService.Gsm.DataDomainName,scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
		    sizeof(ConfigService.Gsm.DataDomainName));
	}
	if(ConfigService.Gsm.DataNetworkPort!=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1))
	{
		/*���ݷ������˿ڲ�һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������ݷ������˿ڵ�ַ*/
	 ConfigService.Gsm.DataNetworkPort=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1);
	
	}
	
	if(memcmp(&ConfigService.Gsm.Config_IP_Address,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2,4)!=0)
	{
		/*���÷�����ip��һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������÷�����IP��ַ*/
		 ConfigService.Gsm.Config_IP_Address=scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2;
	}
	if(memcmp(&ConfigService.Gsm.ConfigDomainName,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,sizeof(ConfigService.Gsm.ConfigDomainName))!=0)
	{
		/*���÷�����������һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������÷�����������ַ*/
		memcpy(ConfigService.Gsm.ConfigDomainName,scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
		    sizeof(ConfigService.Gsm.ConfigDomainName));
	}
	if(ConfigService.Gsm.ConfigNetworkPort!=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2))
	{
		/*���÷������˿ڲ�һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������÷������˿ڵ�ַ*/
	 ConfigService.Gsm.ConfigNetworkPort=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2);
	
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO1,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1,sizeof(ConfigService.Gsm.Alarm_SMS_NO1))!=0)
	{
		/*���÷��������ű�������1��һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������÷��������ű�������1����*/
		 ConfigService.Gsm.Alarm_SMS_NO1=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1;
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO2,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2,sizeof(ConfigService.Gsm.Alarm_SMS_NO2))!=0)
	{
		/*���÷��������ű�������2��һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������÷��������ű�������2����*/
		 ConfigService.Gsm.Alarm_SMS_NO2=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2;
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO3,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3,sizeof(ConfigService.Gsm.Alarm_SMS_NO3))!=0)
	{
		/*���÷��������ű�������3��һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������÷��������ű�������3����*/
		 ConfigService.Gsm.Alarm_SMS_NO3=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3;
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO4,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4,sizeof(ConfigService.Gsm.Alarm_SMS_NO4))!=0)
	{
		/*���÷��������ű�������4��һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������÷��������ű�������4����*/
		 ConfigService.Gsm.Alarm_SMS_NO4=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4;
	}
 if(memcmp(&ConfigService.Gsm.Alarm_SMS_NO5,
	          &scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5,sizeof(ConfigService.Gsm.Alarm_SMS_NO5))!=0)
	{
		/*���÷��������ű�������5��һ��,���� GSMģ����Ϣ �޸��� */
		 item  = item|ConfigService_Gsm;
		/* �������÷��������ű�������5����*/
		 ConfigService.Gsm.Alarm_SMS_NO5=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5;
	}
/******************************************************************/
	/* ��ʼ�� ���÷����� ������Ϣ */
	if(ConfigService.Update.Flag !=
	   scrPara->Inf_State.Instru_System.Inf.UpdateFlag)
	{

		/* �����洢�����һ��,���� �˵���Ϣ �޸��� */
		item  = item|ConfigService_UploadUpdata;
		/* ���������洢��� */
		ConfigService.Update.Flag = 
		scrPara->Inf_State.Instru_System.Inf.UpdateFlag;
	}

	return item;
}



/************************************************************************************************/
/* ������	: ConfigService_RXLog												 				*/
/* ����  	: gprs:GPRS���в���.			 														*/
/* ���		: ��		 																		*/
/* ����	 	: ���÷�������������ҵ������								 							*/
/* �������� 	: 2016/08/16																		*/
/* �޸����� 	: 2016/08/16																		*/
/*   �汾 	: V0: ����																			*/
/************************************************************************************************/
void ConfigService_RXLog(GPRS_Struct gprs,INSTRU_CONFIG_INF_TAB* scrPara)
{	
	 
	/* ���ݽ���״̬ */
	BOOL state = FALSE;
	/* �������� */
	uint8_t cmdType;
	/* ��������:�հ�/�ǿհ� */
	BOOL comContent = FALSE;
	/* �ж�ͨ��״̬ */
	if(gprs.ConfigService.State.Result == TRUE)
	{
		/* ͨ�ųɹ� */
		/* �жϵ�ǰ���÷�����ҵ������ */
		if(ConfigService_App.SYNC_State == FALSE)//32λ�豸�ж��������ͬ����Ϣ�Ĳ����Ƿ����
		{
			/* ͬ��״̬ */ /* �������� */
			state = ConfigService_ParseData( gprs.ConfigService.Para.RXDR,
											                gprs.ConfigService.Para.RXLen,
											                                  &comContent,
											                                    &cmdType);
			
			/* �ж����ݽ���״̬ */
			if(FALSE == state)
			{
				/* 1�����ݽ���ʧ�� *//* 2���ظ�ƽ̨��Ϣ */
				/* ������÷�������������״̬��־ */
				ConfigService_StopRun();
				/* ���� */
				return;
			}
			
			/* �жϲ��������Ƿ�Ϊ�հ����� */
			if(comContent == TRUE)
			{
				/* ��ǰΪ�ǿղ��� */
				/* �жϲ������� */
				if(cmdType == 0x00)//�жϲ��������Ƿ�Ϊͬ������
				{
					/* ͬ������ */
					/* ���µ�ǰ������ */
					ConfigService_UpdataPara(scrPara);
					/* ������ƽ̨�Ѿ�ͬ����ɣ�����ͬ��״̬��־. */
					ConfigService_App.SYNC_State = TRUE;
					/* ������÷�������������״̬��־ */
					ConfigService_StopRun();
					//��������Ҫ��ƽ̨�ظ���
				}else if(cmdType == 0x01)
				{
					  ConfigService_App.SYNC_State	=TRUE;//zwcǿ������ͬ���ɹ�(��ΪĿǰû��32λ����ͬ�����÷�����ƽ̨��Ϣ����)	
					
					/* �޸����� *///������ʱû���������.
					/* ������÷�������������״̬��־ */
					ConfigService_StopRun();
				}else if(cmdType == 0x02)
				{
					/* �ظ����� *///����ͬ������
					/* ����׶��յ��ظ�˵��������ƽ̨�Ѿ�ͬ����ɣ�����ͬ��״̬��־. */
					ConfigService_App.SYNC_State = TRUE;
					/* ������÷�������������״̬��־ */
					ConfigService_StopRun();
				}else
				{
					/* δ֪���� */
					/* ������÷�������������״̬��־ */
					ConfigService_StopRun();
				}
			}else
			{
				/* ��ǰΪ�հ����� */
				/* �ϴ����豸������Ϣ(ͬ��ƽ̨����) */
				ConfigService_Cmd(ConfigService_UploadAll,0x01);
			}
			
		}
		else//32λ�豸�������ͬ�������Ѿ����
		{
			/* �޸�״̬ */
			/* �������� */
			state = ConfigService_ParseData(gprs.ConfigService.Para.RXDR,
											                gprs.ConfigService.Para.RXLen,
											                &comContent,
											                &cmdType);
			/* �ж����ݽ���״̬ */
			if(FALSE == state)
			{
				/* 1�����ݽ���ʧ�� *//* 2���ظ�ƽ̨��Ϣ */
				/* ������÷�������������״̬��־ */
				ConfigService_StopRun();
				/* ���� */
				return;
			}	
			/* �жϲ��������Ƿ�Ϊ�հ����� */
			if(comContent == TRUE)
			{
				/* ��ǰΪ�ǿղ��� */
				/* �жϲ������� */
				if(cmdType == 0x00)
				{
					/* ͬ������ *///������ʱû���������.
					/* ������÷�������������״̬��־ */
					ConfigService_StopRun();
				}else if(cmdType == 0x01)
				{
					/* �޸����� */
					/* ������Ŀ���ͼĴ��� */
					uint8_t item = 0;
					/* ���µ�ǰ������ */
					ConfigService_UpdataPara(scrPara);
					/* ���ݵ�ǰ�������ñ�ͬ��ƽ̨���� */
					item = ConfigService_CheckParaModify(scrPara);
					
					if(GSM_DEBUG_Flag ==TRUE)
					{
						printf("�Ѿ��޸���ɷ������·������ò���\n");
					}
					/* �ж�������Ϣ�Ƿ����޸��� */
					if(ConfigService_UploadNull != item)
					{
						/* ������Ϣ���޸���ϴ���ǰ�豸�����޸���Ϣ(ͬ��ƽ̨����) */
						ConfigService_Cmd(item,0x01);
					}else//�豸�ظ�ƽ̨,�豸�Ѿ��յ������·�����Ϣ
					{
						/* ������Ϣ���޸���ظ���Ϣ */
//				  ConfigService_Cmd(ConfigService_UploadAll,0x02);
//						GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,ConfigService_EVENT);	
						
						Server_Send_Data(
								              (char*)GPRS.ConfigService.Para.TXDR,
								              (GPRS.ConfigService.Para.TXLen),
								              GSM_GPRS_Config
								            );
						
	    	    IAP_CheckFlag();/* ���̼�����״̬ ������������ʼ����Զ������ */	
					  if(GSM_DEBUG_Flag ==TRUE)
					  {
						    printf("�豸�ظ�ƽ̨,�豸�Ѿ��յ������·�����Ϣ\r\n");
					  } 
					 
					}
				}else if(cmdType == 0x02)
				{
					/* ������ƽ̨�Ѿ�ͬ����ɣ�����ͬ��״̬��־. */
			    ConfigService_App.SYNC_State = TRUE;//�豸������Ĭ�ϻᷢһ��ϵͳ������Ϣ  ��ͬ��״̬��Ϊ��ͬ��
					/* �ظ����� *///�޸�ͬ������
					/* ������÷�������������״̬��־ */
					ConfigService_App.Run_State = FALSE;		
					/* �жϵ�ǰ�̼�����״̬ */
					if(IAP_UpdateSatte() == TRUE)
					{
						/* ����̼�����״̬ */
						scrPara->Inf_State.Instru_System.Inf.UpdateFlag = FALSE;
						/* ���´洢���� */
						Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_System.Inf.UpdateFlag,
						sizeof(scrPara->Inf_State.Instru_System.Inf.UpdateFlag));
						mcu_reset();//�����豸  ��λ��Ƭ��zwc
					}
				}else
				{
					/* δ֪���� */
					/* ������÷�������������״̬��־ */
					ConfigService_StopRun();
				}
			}else
			{
				/* ��ǰΪ�հ����� */	/* ������Ŀ���ͼĴ��� */
				uint8_t item = 0;
				/* ���ݵ�ǰ�������ñ�ͬ��ƽ̨���� */
				item = ConfigService_CheckParaModify(scrPara);
				/* �ж�������Ϣ�Ƿ����޸��� */
				if(ConfigService_UploadNull != item)
				{
					/* ������Ϣ���޸���ϴ����豸�����޸���Ϣ(ͬ��ƽ̨����) */
					ConfigService_Cmd(item,0x01);
				}else
				{
					/* ������Ϣ���޸��� */
					/* ������÷�������������״̬��־ */
					ConfigService_StopRun();
				}
			}
		}
	}else
	{
		/* ͨ��ʧ�� */
		/* ������÷�������������״̬��־ */
		ConfigService_StopRun();
	}
	

}

/************************************************************************************************/
/* ������	: ConfigService_InitPara												 			*/
/* ����  	: ��			 																	*/
/* ���		: ��		 																		*/
/* ����	 	: ���÷�������������ҵ������								 							*/
/* �������� 	: 2016/08/16																		*/
/* �޸����� 	: 2016/08/16																		*/
/*   �汾 	: V0: ����������ҵ���߼�															*/
/************************************************************************************************/
void ConfigService_InitPara(INSTRU_CONFIG_INF_TAB* scrPara)
{
	/* ��ʼ�� ���÷����� ϵͳ��Ϣ */
	ConfigService.System.SerialNumber = 
	scrPara->Inf_State.Instru_System.Inf.Serial_Number;
	ConfigService.System.HardwareVersion = 
	scrPara->Inf_State.Instru_System.Inf.Hardware_Version;
	ConfigService.System.SoftwareVersion = 
	scrPara->Inf_State.Instru_System.Inf.Software_Version;
	
	/* ��ʼ�� ���÷����� ��Դ��Ϣ */
	ConfigService.Power.PowerState = TRUE;
	ConfigService.Power.PowerAlarmSwitch = TRUE;
	if((scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch == TRUE)&&
	   (scrPara->Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch == TRUE))
	{
		ConfigService.Power.PowerSLAlarmSwitch = TRUE;
	}else
	{
		ConfigService.Power.PowerSLAlarmSwitch = FALSE;
	}

	ConfigService.Power.PowerSMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch;
	
	ConfigService.Power.PowerQuantityState =  
	scrPara->Inf_State.Instru_Power.Inf_State.BT_Quantity_State;
	
	ConfigService.Power.BTAlarmSwitch = TRUE;
	if((scrPara->Inf_State.Instru_Power.Inf_State.BT_Sound_Alarm_Switch == TRUE)&&
	   (scrPara->Inf_State.Instru_Power.Inf_State.BT_Light_Alarm_Switch == TRUE))
	{
		ConfigService.Power.BTSLAlarmSwitch = TRUE;
	}else
	{
		ConfigService.Power.BTSLAlarmSwitch = FALSE;
	}
	ConfigService.Power.BTSMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Power.Inf_State.BT_SMS_Alarm_Switch;
	
	/* ��ʼ�� ���÷����� ��������Ϣ */
#if (1)
	/* ̽ͷ 1 */
	ConfigService.Sensor[0].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.Type;
	ConfigService.Sensor[0].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Alarm_Switch;
	ConfigService.Sensor[0].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[0].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[0].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[0].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[0];
	ConfigService.Sensor[0].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[1];
	ConfigService.Sensor[0].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[2];
	ConfigService.Sensor[0].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO[3];
	ConfigService.Sensor[0].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[0].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Up_Limit;
	ConfigService.Sensor[0].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_Low_Limit;

	/* ̽ͷ 2 */
	ConfigService.Sensor[1].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.Type;
	ConfigService.Sensor[1].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Alarm_Switch;
	ConfigService.Sensor[1].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[1].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[1].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[1].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[0];
	ConfigService.Sensor[1].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[1];
	ConfigService.Sensor[1].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[2];
	ConfigService.Sensor[1].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO[3];
	ConfigService.Sensor[1].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[1].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Up_Limit;
	ConfigService.Sensor[1].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_Low_Limit;

	/* ̽ͷ 3 */
	ConfigService.Sensor[2].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.Type;
	ConfigService.Sensor[2].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Alarm_Switch;
	ConfigService.Sensor[2].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[2].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[2].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[2].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[0];
	ConfigService.Sensor[2].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[1];
	ConfigService.Sensor[2].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[2];
	ConfigService.Sensor[2].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO[3];
	ConfigService.Sensor[2].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[2].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Up_Limit;
	ConfigService.Sensor[2].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_Low_Limit;


	/* ̽ͷ 4 */
	ConfigService.Sensor[3].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.Type;
	ConfigService.Sensor[3].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Alarm_Switch;
	ConfigService.Sensor[3].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[3].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[3].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[3].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[0];
	ConfigService.Sensor[3].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[1];
	ConfigService.Sensor[3].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[2];
	ConfigService.Sensor[3].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO[3];
	ConfigService.Sensor[3].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[3].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Up_Limit;
	ConfigService.Sensor[3].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_Low_Limit;


	/* ̽ͷ 5 */
	ConfigService.Sensor[4].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.Type;
	ConfigService.Sensor[4].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Alarm_Switch;
	ConfigService.Sensor[4].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[4].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[4].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[4].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[0];
	ConfigService.Sensor[4].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[1];
	ConfigService.Sensor[4].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[2];
	ConfigService.Sensor[4].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO[3];
	ConfigService.Sensor[4].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[4].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Up_Limit;
	ConfigService.Sensor[4].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_Low_Limit;

	/* ̽ͷ 6 */
	ConfigService.Sensor[5].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.Type;
	ConfigService.Sensor[5].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Alarm_Switch;
	ConfigService.Sensor[5].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[5].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[5].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[5].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[0];
	ConfigService.Sensor[5].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[1];
	ConfigService.Sensor[5].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[2];
	ConfigService.Sensor[5].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO[3];
	ConfigService.Sensor[5].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[5].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Up_Limit;
	ConfigService.Sensor[5].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_Low_Limit;


	/* ̽ͷ 7 */
	ConfigService.Sensor[6].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.Type;
	ConfigService.Sensor[6].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Alarm_Switch;
	ConfigService.Sensor[6].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[6].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[6].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[6].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[0];
	ConfigService.Sensor[6].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[1];
	ConfigService.Sensor[6].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[2];
	ConfigService.Sensor[6].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO[3];
	ConfigService.Sensor[6].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[6].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Up_Limit;
	ConfigService.Sensor[6].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_Low_Limit;


	/* ̽ͷ 8 */
	ConfigService.Sensor[7].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.Type;
	ConfigService.Sensor[7].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Alarm_Switch;
	ConfigService.Sensor[7].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[7].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[7].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[7].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[0];
	ConfigService.Sensor[7].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[1];
	ConfigService.Sensor[7].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[2];
	ConfigService.Sensor[7].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO[3];
	ConfigService.Sensor[7].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[7].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Up_Limit;
	ConfigService.Sensor[7].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_Low_Limit;

	/* ̽ͷ 9 */
	ConfigService.Sensor[8].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.Type;
	ConfigService.Sensor[8].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Alarm_Switch;
	ConfigService.Sensor[8].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[8].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[8].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[8].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[0];
	ConfigService.Sensor[8].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[1];
	ConfigService.Sensor[8].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[2];
	ConfigService.Sensor[8].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO[3];
	ConfigService.Sensor[8].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[8].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Up_Limit;
	ConfigService.Sensor[8].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_Low_Limit;

	/* ̽ͷ 10 */
	ConfigService.Sensor[9].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.Type;
	ConfigService.Sensor[9].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Alarm_Switch;
	ConfigService.Sensor[9].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[9].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[9].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[9].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[0];
	ConfigService.Sensor[9].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[1];
	ConfigService.Sensor[9].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[2];
	ConfigService.Sensor[9].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO[3];
	ConfigService.Sensor[9].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[9].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Up_Limit;
	ConfigService.Sensor[9].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_Low_Limit;

	/* ̽ͷ 11 */
	ConfigService.Sensor[10].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.Type;
	ConfigService.Sensor[10].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Alarm_Switch;
	ConfigService.Sensor[10].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[10].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[10].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[10].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[0];
	ConfigService.Sensor[10].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[1];
	ConfigService.Sensor[10].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[2];
	ConfigService.Sensor[10].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO[3];
	ConfigService.Sensor[10].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[10].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Up_Limit;
	ConfigService.Sensor[10].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_Low_Limit;

	/* ̽ͷ 12 */
	ConfigService.Sensor[11].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.Type;
	ConfigService.Sensor[11].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Alarm_Switch;
	ConfigService.Sensor[11].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[11].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[11].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[11].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[0];
	ConfigService.Sensor[11].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[1];
	ConfigService.Sensor[11].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[2];
	ConfigService.Sensor[11].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO[3];
	ConfigService.Sensor[11].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[11].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Up_Limit;
	ConfigService.Sensor[11].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_Low_Limit;

	/* ̽ͷ 13 */
	ConfigService.Sensor[12].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.Type;
	ConfigService.Sensor[12].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Alarm_Switch;
	ConfigService.Sensor[12].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[12].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[12].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[12].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[0];
	ConfigService.Sensor[12].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[1];
	ConfigService.Sensor[12].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[2];
	ConfigService.Sensor[12].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO[3];
	ConfigService.Sensor[12].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[12].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Up_Limit;
	ConfigService.Sensor[12].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_Low_Limit;

	/* ̽ͷ 14 */
	ConfigService.Sensor[13].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.Type;
	ConfigService.Sensor[13].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Alarm_Switch;
	ConfigService.Sensor[13].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[13].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[13].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[13].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[0];
	ConfigService.Sensor[13].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[1];
	ConfigService.Sensor[13].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[2];
	ConfigService.Sensor[13].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO[3];
	ConfigService.Sensor[13].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[13].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Up_Limit;
	ConfigService.Sensor[13].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_Low_Limit;

	/* ̽ͷ 15 */
	ConfigService.Sensor[14].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.Type;
	ConfigService.Sensor[14].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Alarm_Switch;
	ConfigService.Sensor[14].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[14].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[14].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[14].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[0];
	ConfigService.Sensor[14].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[1];
	ConfigService.Sensor[14].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[2];
	ConfigService.Sensor[14].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO[3];
	ConfigService.Sensor[14].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[14].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Up_Limit;
	ConfigService.Sensor[14].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_Low_Limit;

	/* ̽ͷ 16 */
	ConfigService.Sensor[15].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.Type;
	ConfigService.Sensor[15].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Alarm_Switch;
	ConfigService.Sensor[15].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[15].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[15].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[15].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[0];
	ConfigService.Sensor[15].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[1];
	ConfigService.Sensor[15].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[2];
	ConfigService.Sensor[15].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO[3];
	ConfigService.Sensor[15].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[15].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Up_Limit;
	ConfigService.Sensor[15].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_Low_Limit;

	/* ̽ͷ 17 */
	ConfigService.Sensor[16].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.Type;
	ConfigService.Sensor[16].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Alarm_Switch;
	ConfigService.Sensor[16].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[16].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[16].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[16].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[0];
	ConfigService.Sensor[16].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[1];
	ConfigService.Sensor[16].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[2];
	ConfigService.Sensor[16].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO[3];
	ConfigService.Sensor[16].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[16].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Up_Limit;
	ConfigService.Sensor[16].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_Low_Limit;

	/* ̽ͷ 18 */
	ConfigService.Sensor[17].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.Type;
	ConfigService.Sensor[17].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Alarm_Switch;
	ConfigService.Sensor[17].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[17].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[17].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[17].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[0];
	ConfigService.Sensor[17].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[1];
	ConfigService.Sensor[17].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[2];
	ConfigService.Sensor[17].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO[3];
	ConfigService.Sensor[17].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[17].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Up_Limit;
	ConfigService.Sensor[17].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_Low_Limit;

	/* ̽ͷ 19 */
	ConfigService.Sensor[18].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.Type;
	ConfigService.Sensor[18].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Alarm_Switch;
	ConfigService.Sensor[18].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[18].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[18].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[18].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[0];
	ConfigService.Sensor[18].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[1];
	ConfigService.Sensor[18].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[2];
	ConfigService.Sensor[18].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO[3];
	ConfigService.Sensor[18].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[18].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Up_Limit;
	ConfigService.Sensor[18].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_Low_Limit;

	/* ̽ͷ 20 */
	ConfigService.Sensor[19].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.Type;
	ConfigService.Sensor[19].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Alarm_Switch;
	ConfigService.Sensor[19].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[19].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[19].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[19].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[0];
	ConfigService.Sensor[19].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[1];
	ConfigService.Sensor[19].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[2];
	ConfigService.Sensor[19].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO[3];
	ConfigService.Sensor[19].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[19].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Up_Limit;
	ConfigService.Sensor[19].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_Low_Limit;

	/* ̽ͷ 21 */
	ConfigService.Sensor[20].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.Type;
	ConfigService.Sensor[20].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Alarm_Switch;
	ConfigService.Sensor[20].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[20].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[20].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[20].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[0];
	ConfigService.Sensor[20].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[1];
	ConfigService.Sensor[20].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[2];
	ConfigService.Sensor[20].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO[3];
	ConfigService.Sensor[20].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[20].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Up_Limit;
	ConfigService.Sensor[20].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_Low_Limit;

	/* ̽ͷ 22 */
	ConfigService.Sensor[21].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.Type;
	ConfigService.Sensor[21].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Alarm_Switch;
	ConfigService.Sensor[21].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[21].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[21].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[21].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[0];
	ConfigService.Sensor[21].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[1];
	ConfigService.Sensor[21].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[2];
	ConfigService.Sensor[21].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO[3];
	ConfigService.Sensor[21].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[21].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Up_Limit;
	ConfigService.Sensor[21].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_Low_Limit;

	/* ̽ͷ 23 */
	ConfigService.Sensor[22].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.Type;
	ConfigService.Sensor[22].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Alarm_Switch;
	ConfigService.Sensor[22].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[22].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[22].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[22].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[0];
	ConfigService.Sensor[22].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[1];
	ConfigService.Sensor[22].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[2];
	ConfigService.Sensor[22].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO[3];
	ConfigService.Sensor[22].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[22].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Up_Limit;
	ConfigService.Sensor[22].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_Low_Limit;

	/* ̽ͷ 24 */
	ConfigService.Sensor[23].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.Type;
	ConfigService.Sensor[23].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Alarm_Switch;
	ConfigService.Sensor[23].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[23].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[23].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[23].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[0];
	ConfigService.Sensor[23].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[1];
	ConfigService.Sensor[23].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[2];
	ConfigService.Sensor[23].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO[3];
	ConfigService.Sensor[23].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[23].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Up_Limit;
	ConfigService.Sensor[23].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_Low_Limit;

	/* ̽ͷ 25 */
	ConfigService.Sensor[24].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.Type;
	ConfigService.Sensor[24].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Alarm_Switch;
	ConfigService.Sensor[24].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[24].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[24].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[24].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[0];
	ConfigService.Sensor[24].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[1];
	ConfigService.Sensor[24].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[2];
	ConfigService.Sensor[24].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO[3];
	ConfigService.Sensor[24].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[24].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Up_Limit;
	ConfigService.Sensor[24].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_Low_Limit;

	/* ̽ͷ 26 */
	ConfigService.Sensor[25].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.Type;
	ConfigService.Sensor[25].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Alarm_Switch;
	ConfigService.Sensor[25].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[25].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[25].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[25].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[0];
	ConfigService.Sensor[25].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[1];
	ConfigService.Sensor[25].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[2];
	ConfigService.Sensor[25].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO[3];
	ConfigService.Sensor[25].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[25].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Up_Limit;
	ConfigService.Sensor[25].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_Low_Limit;

	/* ̽ͷ 27 */
	ConfigService.Sensor[26].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.Type;
	ConfigService.Sensor[26].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Alarm_Switch;
	ConfigService.Sensor[26].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[26].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[26].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[26].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[0];
	ConfigService.Sensor[26].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[1];
	ConfigService.Sensor[26].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[2];
	ConfigService.Sensor[26].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO[3];
	ConfigService.Sensor[26].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[26].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Up_Limit;
	ConfigService.Sensor[26].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_Low_Limit;

	/* ̽ͷ 28 */
	ConfigService.Sensor[27].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.Type;
	ConfigService.Sensor[27].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Alarm_Switch;
	ConfigService.Sensor[27].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[27].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[27].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[27].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[0];
	ConfigService.Sensor[27].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[1];
	ConfigService.Sensor[27].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[2];
	ConfigService.Sensor[27].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO[3];
	ConfigService.Sensor[27].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[27].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Up_Limit;
	ConfigService.Sensor[27].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_Low_Limit;

	/* ̽ͷ 29 */
	ConfigService.Sensor[28].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.Type;
	ConfigService.Sensor[28].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Alarm_Switch;
	ConfigService.Sensor[28].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[28].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[28].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[28].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[0];
	ConfigService.Sensor[28].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[1];
	ConfigService.Sensor[28].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[2];
	ConfigService.Sensor[28].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO[3];
	ConfigService.Sensor[28].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[28].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Up_Limit;
	ConfigService.Sensor[28].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_Low_Limit;

	/* ̽ͷ 30 */
	ConfigService.Sensor[29].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.Type;
	ConfigService.Sensor[29].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Alarm_Switch;
	ConfigService.Sensor[29].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[29].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[29].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[29].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[0];
	ConfigService.Sensor[29].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[1];
	ConfigService.Sensor[29].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[2];
	ConfigService.Sensor[29].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO[3];
	ConfigService.Sensor[29].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[29].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Up_Limit;
	ConfigService.Sensor[29].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_Low_Limit;

	/* ̽ͷ 31 */
	ConfigService.Sensor[30].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.Type;
	ConfigService.Sensor[30].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Alarm_Switch;
	ConfigService.Sensor[30].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[30].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[30].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[30].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[0];
	ConfigService.Sensor[30].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[1];
	ConfigService.Sensor[30].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[2];
	ConfigService.Sensor[30].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO[3];
	ConfigService.Sensor[30].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[30].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Up_Limit;
	ConfigService.Sensor[30].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_Low_Limit;

	/* ̽ͷ 32 */
	ConfigService.Sensor[31].SensorType = 
	(ConfigService_Para_SensorType)scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.Type;
	ConfigService.Sensor[31].SensorAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Alarm_Switch;
	ConfigService.Sensor[31].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Sound_Alarm_Switch;
	ConfigService.Sensor[31].SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Light_Alarm_Switch;
	ConfigService.Sensor[31].SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SMS_Alarm_Switch;
	ConfigService.Sensor[31].SN[0] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[0];
	ConfigService.Sensor[31].SN[1] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[1];
	ConfigService.Sensor[31].SN[2] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[2];
	ConfigService.Sensor[31].SN[3] = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO[3];
	ConfigService.Sensor[31].ID = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO;
	ConfigService.Sensor[31].UpLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Up_Limit;
	ConfigService.Sensor[31].LowLimit = 
	scrPara->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_Low_Limit;

#endif


	/* ��ʼ�� ���÷����� �˵���Ϣ */
	ConfigService.Menu.SLAlarmSwitch = 
	scrPara->Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch;
	ConfigService.Menu.SMSAlarmSwitch = 
	scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch;
	ConfigService.Menu.SMSAlarm_RepeatTime = 
	scrPara->Inf_State.Instru_Menu.Inf_State.SMS_Repeat_Alarm_Interval;
	ConfigService.Menu.SaveInterval_Alarm = 
	scrPara->Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval;
	ConfigService.Menu.SaveInterval_Normal = 
	scrPara->Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval;
	/* ��ʼ�� ���÷����� GSMģ����Ϣ */
		//1.1�����ݷ�����IP��ַ
	ConfigService.Gsm.Data_IP_Address	=	scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID1;
	  //1.2�����ݷ���������
		                   memcpy(ConfigService.Gsm.DataDomainName,
		scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
			                   sizeof(ConfigService.Gsm.DataDomainName));
	  //1.3�����ݷ������˿ں� 		 
	ConfigService.Gsm.DataNetworkPort=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID1);
		//2.1�����÷�����IP��ַ
	ConfigService.Gsm.Config_IP_Address	=	scrPara->Inf_State.Instru_GSM.Inf_State.IP_Address_ID2;
	  //2.2�����÷���������
		                   memcpy(ConfigService.Gsm.ConfigDomainName,
		scrPara->Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
			                   sizeof(ConfigService.Gsm.ConfigDomainName));
	  //2.3�����÷������˿ں� 		 
	ConfigService.Gsm.ConfigNetworkPort=htons(scrPara->Inf_State.Instru_GSM.Inf_State.Port_ID2);
    //3.1�����ű�������	
  ConfigService.Gsm.Alarm_SMS_NO1=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1;
  ConfigService.Gsm.Alarm_SMS_NO2=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO2;
  ConfigService.Gsm.Alarm_SMS_NO3=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO3;
  ConfigService.Gsm.Alarm_SMS_NO4=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve4;
  ConfigService.Gsm.Alarm_SMS_NO5=scrPara->Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO_Reserve5;
/******************************************************************************/	
	/* ��ʼ�� ����Զ������״̬��־λ  */
	ConfigService.Update.Flag = FALSE;
	
	if(scrPara->Inf_State.Instru_System.Inf.UpdateFlag != FALSE)
	{
		scrPara->Inf_State.Instru_System.Inf.UpdateFlag = FALSE;
		/* ���´洢���� */
		Write_Config_Tab((u8*)&scrPara->Inf_State.Instru_System.Inf.UpdateFlag,
		sizeof(scrPara->Inf_State.Instru_System.Inf.UpdateFlag));
	}
	
	memset(ConfigService.Update.DocumentName,
		   0x00,
		   sizeof(ConfigService.Update.DocumentName));
	memset(ConfigService.Update.Network,
		   0x00,
		   sizeof(ConfigService.Update.Network));
	ConfigService.Update.Port_HSB = 0;
	ConfigService.Update.Port_LSB = 0;
	
	/* ��ʼ���̼���������ָ�� */
	IAP_ParaInit(&ConfigService.Update,&scrPara->Inf_State.Instru_System.Inf.UpdateFlag);
}

#endif

#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
