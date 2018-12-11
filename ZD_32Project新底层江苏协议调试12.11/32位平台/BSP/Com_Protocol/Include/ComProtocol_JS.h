/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**�ļ���     : ComProtocol_JS.h																*/
/**����       : ���ݽ���ʡ���߹滮���������ṩ��ͨ��Э���ʽ��̽ͷ�����ϴ�����ƽ̨							*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/************************************************************************************************/ 
#ifndef __ComProtocol_JS_h
#define	__ComProtocol_JS_h
#include "Log_GSM.h"
#if (AppGPRSComProtocol == 0x04)
#include "App_Save.h"

#define ComProtocolZDE_TXSize                    (512u)
#define ComProtocolZDE_RXSize                    (512u)

#define  COM_END_Bytes                           (8u)//Э��ĩβռ8���ֽ� =����״̬����ʶ(2)+����״̬����ʶ(2)+�ź�״̬����ʶ(2)+У����(2) 
#define  SENSOR_CH_NUM_LEN_Bytes                 (1u)//ͨ��������ֵռ1���ֽ� 
#define  SN_LEN_Bytes                            (1u)//SN����ֵռ1���ֽ� 
#define  Single_Sensor_Bytes                     (2u)//ÿ��������ͨ�����ݳ���
#define  Time_Stamp_Bytes                        (4u)//ʱ������ݳ���
#define  SN_DATA_MAX_Len	                       (50u)//SN��������󳤶�(��������Ϊuint8_t)
#define  SENSOR_DATA_MAX_Len	                   (32u)//��������󳤶�  (32��ͨ����ÿ��ͨ������������Ϊuint16_t)
                                                 
#define  COMReg1ExceptSN_Len                     (4u)//��SN��COMReg1�ĳ��� 
#define  COMReg2_Len                             (5u)//COMReg2_Len�ĳ��� 

/*************************************************************************************/
#define JS_UL_SYNC_Identify		                   (0xAA)//�豸��������ͬ����ʶλֵ
#define JS_DL_SYNC_Identify		                   (0x55)//�豸��������ͬ����ʶλֵ
                                                 
#define JS_UL_STX_Identify		                   (0x75)//�豸����������ʼ��ʶλֵ
#define JS_DL_STX_Identify		                   (0x7A)//�豸����������ʼ��ʶλֵ
                                                   
#define JS_YUN_STATE_Identify		                 (0xFD)//�豸����״̬��ʶλֵ
                                                 
#define JS_POWER_STATE_Identify		               (0xFC)//�豸��ص�����ʶλֵ
                                                 
#define JS_SIGNLE_STATE_Identify		             (0xFB)//�豸GSM�ź�ǿ�ȱ�ʶλֵ
                                                 
#define  Sensor_CH_Num                           (2u)//������ͨ������ 


typedef struct
{	
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
//��Ϣͷ���ݽṹ1
  typedef struct
  {
  /* ͬ�����ݱ�־ */
    uint8_t   JS_SYNC_Flag;
  /* ��ʼ���ݱ�־ */
    uint8_t   JS_STX_Flag;
  /*�ܵ������峤��=SN�ų���(4���ֽ�)+ʱ�������(4���ֽ�)+CH_Num*Single_Sensor_Bytes    CH_Num��ʾ��ʪ��ͨ������*/
    uint8_t   JS_DATA_BODY_Len;
  /*SN�����������ݳ��� */
  	uint8_t   JS_SN_DATA_Len;
  /*SN������������ */
    uint8_t   JS_SN_DATA[SN_DATA_MAX_Len];
  } ComProtocolJS_MH1; 


//��Ϣͷ���ݽṹ2
  typedef struct
  {
  	/*ʱ������������� */
    uint32_t   JS_SN_Time_Stamp;	
   /*������ͨ������ */
    uint8_t   JS_SENSOR_CH_Num;	
  }ComProtocolJS_MH2; //5���ֽ� 
 
 //��Ϣͷ���ݽṹ3
  typedef struct
  {
   /*���������������� */
    uint16_t  JS_SENSOR_Data[32];
  }ComProtocolJS_MH3;  
  
//��Ϣͷ���ݽṹ4
  typedef struct
  {	
  /*�豸����״̬��־λ */
    uint8_t   JS_RUN_STATE_Flag;//0xFD
  	/*�豸����״̬ */
    uint8_t   JS_RUN_State;//��0λ������״̬ 0������ 1���� 	  ��1~7λΪԤ��״̬λ
  	/*�豸��ص�����־λ */
    uint8_t   JS_POWER_STATE_Flag;//0xFC
  	/*�豸��ص����ٷֱ� */
    uint8_t   JS_POWER_State;//��ֵ��Χ��0%~100%
  	/*�豸GSMģ���ź�ǿ�ȱ�־λ */
    uint8_t   JS_SIGNLE_STATE_Flag;//0xFB
  	/*�豸GSMģ���ź�ǿ�Ȱٷֱ� */
    uint8_t   JS_SIGNLE_State;//��ֵ��Χ��0%~100%
    /* ����CRCУ��ֵ*/
    unsigned short    JS_DATA_CRC_CheckCode;	
  } ComProtocolJS_MH4;//�� 8���ֽ� 

typedef struct
{
	uint16_t Year;
	int8_t Mon;
	int8_t Day;	
	int8_t Week;	
	int8_t Hour;		
	int8_t Min;				
	int8_t Sec;					
}RTC_T;



BOOL ComProtocol_JS_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab, uint8_t* destBuf,int* dataLen);

int docrc16(uint8_t *pSrcData,int length);//У���׼  CRC16_CCITT(1021)�㷨

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

