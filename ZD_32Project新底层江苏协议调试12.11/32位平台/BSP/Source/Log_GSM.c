/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/11/20																			 */
/* �޸����ڣ�2016/07/25																			 */
/*************************************************************************************************/
#include "interface.h"
#include "BSP.h"
#include "app.h"

#include "Ks0108.h"
#include "Log_GSM.h"


#include "Dev_RTC.h"//2018.12.04  ByZWC  Add
/***********���ĵײ����ӵ�ͷ�ļ�*************/
#include "GSM_app.h"
#include "Gsm_Interface.h"
#include "Gsm_Api.h"
/****************************/

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
	#include "Log_ConfigService.h"
	#include "ComProtocol_JS.h"//����ʡ���߹滮��������Э��
#elif (AppGPRSComProtocol == 0x01)
	#include "Log_ConfigService_Http.h"
#elif (AppGPRSComProtocol == 0x02)
	#include "ComProtocol_ZDE.h"
#endif
#endif

#if (AppGYForwardEnable == 0x01)
#include "ComProtocol_GY.h"
#include "App_Save.h"
#endif

#if (ComProtocol_WZ_Enable == 0x01)
#include "ComProtocol_WZ.h"
#include "App_Save.h"
#endif

#include "Log_Download.h"
#include "Log_HTTP.h"
#include "Log_IAP.h"
#include "Log_Save.h"
#include "Log_Sensor.h"
#include "Log_SMS.h"

#include "Dev_Flash.h"
#include "Dev_Fram.h"
#include "Dev_RTC.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern BOOL GSM_DEBUG_Flag;
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
//#define ntohs htons
//#define ntohl htohl

/* �����ļ� ���� */

/* �������Ĵ���������ת�������������� */
static void GSM_Module_Config(INSTRU_GSM* gprs_inf);
//static void GPRS_Config_Server_Inf(INSTRU_GSM* gprs_inf);

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
static void GPRS_Data_To_Network(void);
#endif

static BOOL GPRS_Parsing_Network_Data(void);//�������������ص�����ָ���Ƿ�Ϸ�
 void GPRS_Send_Network_Data(void);//��������������Ҷ�����
//static void Remote_Upgrade_Program(void);//������������ض���ָ��
#if ((GSM_Module_Type==0)||(GSM_Module_Type==3)) //SIM800C��������ģ��
static void GSM_SMS_Send(INSTRU_GSM* gprs_inf);
#endif
/* GPRS����ʱ��У׼���� */
static void GPRS_Send_System_Clock_Calibration_Data(void);
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))
/* �������������ص�ʱ�����ݣ����жϱ���GPRSͨ��״̬�� */
static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer);
#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x02))
extern CreateHTTPParaStruct HTTPPara;
#endif
#endif

/* ����1K������ ���ͻ��� */
#define GPRS_Send_Sensor_Data_Buffer_Size	(1536u)
/* GPRS �������ݻ��� */
static u8 GPRS_Send_Sensor_Data_Buffer[GPRS_Send_Sensor_Data_Buffer_Size];
/* ����1500�ֽڵ����� ���ջ���(�����Ǹ��ݵײ�Ľ��տռ�) */
#define GPRS_Rece_Sensor_Data_Buffer_Size	(1500u)
/* GPRS �������ݻ��� */
static u8 GPRS_Rece_Sensor_Data_Buffer[GPRS_Rece_Sensor_Data_Buffer_Size];


/* ����GSM ��Ϣ */
static GSM_QUE GSM_QUE_Init_Module			                   = {Init_GSM_Module_EVENT,	((u32*)0X0),TRUE};
/* �����ź�ǿ�� ��Ϣ */                                     
static GSM_QUE GSM_QUE_Update_Signal 		                   = {Update_SGL_EVENT,		  ((u32*)0X0),TRUE};
/* ���¸���״̬ ��Ϣ */                                     
static GSM_QUE GSM_QUE_Update_Link 			                   = {Update_Link_EVENT,		  ((u32*)0X0),TRUE};
/* �ϴ���ʪ������ ��Ϣ */                                   
static GSM_QUE GSM_QUE_Update_Data 			                   = {Updata_Data_EVENT,		  ((u32*)0X0),TRUE};
/* ���� ������� ���� */                                   
static GSM_QUE GSM_QUE_Download_IAP_Data	                 = {IAP_EVENT,				    ((u32*)0X0),TRUE};
/* ���Ͷ��� ���� */                                        
static GSM_QUE GSM_QUE_Send_SMS				                     = {Send_SMS_EVENT,			    ((u32*)0X0),TRUE};
/* �������� ���� */                                        
static GSM_QUE GSM_QUE_ConfigService		                   = {ConfigService_EVENT,		((u32*)0X0),TRUE};
/* ϵͳʱ��У׼��Ϣ */
static GSM_QUE GSM_QUE_Send_System_Clock_Calibration	     = {System_Clock_Calibration_EVENT,	((u32*)0X0),TRUE};

#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��
/* ����Wifi���ֺ�����*/
static GSM_QUE    Wifi_QUE_SetWifiNamePWD	                 = {SetWifiNamePWD_EVENT,	((u32*)0X0),TRUE};
/* ����WifiWlan����*/
static GSM_QUE    Wifi_QUE_SetWifiWLAN                     = {SetWifiWLAN_EVENT,	((u32*)0X0),TRUE};
/* ����Wifiģ���ȡIP��ַ��ʽ   STATIC����DHCPģʽ����*/
static GSM_QUE    Wifi_QUE_GET_IP_WAY                      = {SetWifiWLAN_GET_IP_WAY_EVENT,	((u32*)0X0),TRUE};
/*��λWifiģ��*/
static GSM_QUE    Wifi_QUE_Reset                           = {RestWifi_EVENT,	((u32*)0X0),TRUE};
/* Wifiģ��ָ���������*/
static GSM_QUE    Wifi_QUE_Factory_Data_Reset              = {Factory_Data_Reset_Wifi_EVENT,	((u32*)0X0),TRUE};
#endif	



//GSM��Ϣ ��ʶ��
#define	GPRS_COM_Identify			(0X7E)
//GSM ת�� ���
#define	GSM_Translator_Symbol_0X7E	GPRS_COM_Identify
#define	GSM_Translator_Symbol_0X02	0X02	
#define	GSM_Translator_Symbol_0X7D	0X7D
#define	GSM_Translator_Symbol_0X01	0X01
//GSM��Ϣ ��Ϣͷ ��ϢID(0X0900:����ͨ��;0X8900:����ͨ��;0X8001:ͨ��Ӧ��)
#define	GPRS_COM_ID_Upload			   (0X0900)
#define	GPRS_COM_ID_Download		   (0X8900)
#define GPRS_COM_ID_ACK				     (0X8001)
//��Ϣ�峤�ȣ�����:��Ϣ����+��Ϣ��������+������ǰ״̬+ʱ���+ͨ����+2��ͨ������=19Byte��
#define GPRS_COM_Body_Len			     (0u)
//��Ϣ ��ˮ��
#define GPRS_COM_Serial_Num			   (0u)
//��Ϣ������ ���ݼ��ܷ�ʽ(������)
#define GPRS_COM_Enc_Mode			     (0u)
//��Ϣ������ ���ݷְ���־(���ֱ�)
#define GPRS_COM_Sub_Flag			     (0u)
//��Ϣ����:WIFI ��Ƭʽ������
#define GPRS_COM_Message_Type		   (0XF0)


#if (AppGPRSComProtocol == 0x00)//����Э��
//��Ϣ����:32λƽ̨��ʪ������
#define GPRS_32Platforms_Type		(0XFC)//GPRS_ZWC
#endif

#if (AppGPRSComProtocol == 0x03)//�ӱ��Ӵ�Э��
//��Ϣ����:32λƽ̨��ʪ������
#define GPRS_32Platforms_Type		(0XFD)//GPRS_ZWC
#endif


//��Ϣ��������:0:ʵʱ���ݣ�1:��ʷ����
#define GPRS_COM_Data_Type			(0x01)
//������ͨ������ 
#define GPRS_COM_Sensor_Num			(Sensor_CH_Config_Num)
//Ӧ�� ��Ϣ�峤�� */
#define GPRS_COM_ACK_Att_Len		(5u)

// ϵͳ���õ�gprs��������ַ(GSMģ��֧�����3��GPRS��������)
#define GPRS_Server_CONN_Num_Max	(MAX_NUM_TCP_CONN)//��������Ϊʲô���ó�4..wo���Ǻ���⡣
// ������ ���
#define	GPRS_Server1_NO					(0u)
#define	GPRS_Server2_NO					(1u)
#define	GPRS_Server3_NO					(2u)


/************************************************************************************************/
/* GPRS ��ʪ��������Ϣ ���ݼĴ��� */
static GSM_GPRS_Send_Struct GPRS_Send_Reg;

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
/* GPRS ������ʪ��������Ϣ ���ݼĴ����̶����� */
static const GSM_GPRS_Send_Struct GPRS_Send_Reg_Fix =
{
	/* ͷ ��ʶλ */ //(1 Byte)
	GPRS_COM_Identify,
	/* ͨ�� ��Ϣͷ *///(16 Byte)
	/* ��Ϣ ID(����) *///(2 Byte)
	GPRS_COM_ID_Upload/0X100,
	GPRS_COM_ID_Upload%0X100,
	/* ��Ϣ�� ���� *///(2 Byte)
	//��Ϣ������ ����
	0X0,
	//��Ϣ������ �ְ�
	GPRS_COM_Sub_Flag,
	//��Ϣ������ ���ݼ��ܷ�ʽ
	GPRS_COM_Enc_Mode,
	//��Ϣ������ ��Ϣ�峤��
	GPRS_COM_Body_Len/0X100,
	//��Ϣ������ ��Ϣ�峤��
	GPRS_COM_Body_Len%0X100,
	/* �ն����к�(SN��): */	//(6 Byte)
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* ��Ϣ ��ˮ�� *///(2 Byte)
	GPRS_COM_Serial_Num/0X100,GPRS_COM_Serial_Num%0X100,
	
	/* ͨ�� ��Ϣ�� */
	/* ��Ϣ����:WIFI ��Ƭʽ������(����ֻ������ģʽ) *///(1 Byte)
	GPRS_32Platforms_Type,
	/* ��Ϣ��������:0->ʵʱ���ݣ�1->��ʷ���ݡ� *///(1 Byte)
	GPRS_COM_Data_Type,
	/* �������� *///(1 Byte)
	0,
	/* ��������(�����Ҫ��Ϊ�˸�����������ݵ�ָ��) *///(1 Byte)
	0,
	
};
/************************************************************************************************/
/* GPRS �������ݼĴ��� */
static GSM_GPRS_Data_Content_Struct GPRS_Data_Content_Reg;


/* GPRS �������ݼĴ��� �̶����� */
static const GSM_GPRS_Data_Content_Struct GPRS_Data_Content_Reg_Fix = 
{
	/* ������(ID��): */	//(4 Byte)
	0,
	/* ��ӵ�Դ״̬ */	//Ĭ��0 ������
	0,
	/* Ԥ��1 */
	0,
	/* Ԥ��2 */
	0,
	/* ��ص��� */		//Ĭ��0 ������
	0,
	/* ʱ��� */			//(6 Byte)
	//��//(1 Byte)
	0,
	//��//(1 Byte)
	0,
	//��//(1 Byte)
	0,
	//ʱ//(1 Byte)
	0,
	//��//(1 Byte)
	0,
	//��//(1 Byte)
	0,
	/* ͨ���� */
	0,
	/* ���������� */
	0,
	/* ����(�����Ҫ��Ϊ�˸�����������ݵ�ָ��) */
	0.0,
};

#endif
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))
/* GPRS ������ʪ��������Ϣ ���ݼĴ��� */
static GSM_GPRS_Rece_Struct GPRS_Rece_Reg;
#endif
#endif
/************************************************************************************************/
/* GPRS ������ʪ��������Ϣ ���ݼĴ��� */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_Sensor_Data_Reg = 
{
	/* GPRS ���� ������ �������� */
	0,
	/* GPRS ���� ������ �������� */
	0,
	/* GPRS ���ʹ���������ͨ��״̬��FALSE:û�з��ͣ�*/
	FALSE,
	/* GPRS �����򴫸������ݵȴ�����״̬��FALSE:û�����ݴ��� */
	FALSE,
	/* GPRS �����򴫸������ݵȴ�����״̬��FALSE:û�����ݴ��� */
	FALSE,
	/* GPRS ���ʹ��������ݽ�� */
	TRUE,
	/* GPRS ���մ��������ݽ�� */
	TRUE,
	/* GPRS ��������״̬ */
	TRUE,	
	/* GPRS ���ʹ��������� ʧ�ܴ��� */
	0,
};
/************************************************************************************************/
//static GSM_GPRS_SR_Struct GPRS_SR_Reg = //GSM_ZWC
 GSM_GPRS_SR_Struct GPRS_SR_Reg = 
{
	/* GPRS �ź� ǿ�� */
	0,
	/* GPRS �ź� ǿ�� ��ʾ(0-5:0��;6-8:1��;9-11:2��;12-14:3��;15-31:4��;) */
	0,
	/* GPRS ģ�鹤��״̬ */
	FALSE,
	/* GPRS ����״̬ */
	FALSE,
	/* ��GPRS�ź� */
	SIM_NoSignal,
};
/************************************************************************************************/
////GPRS��������Ϣ
//static GPRS_Connection GPRS_COM_Reg[GPRS_Server_CONN_Num_Max];
/************************************************************************************************/
//GSM ���� ״̬�Ĵ���
GSM_SMS_SR_Struct GSM_SMS_SR_Reg = 
{
	FALSE,
#if (SMS_Function_Enable == 0x01)
	
#if (SMS_User_Number == 0x00)
#error "��ʪ�ȿ�Ƭͨ�����ò���ȷ"
#endif	
	
#if (SMS_User_Number >= 0x01)
	TRUE,
#endif
	
#if (SMS_User_Number >= 0x02)
	TRUE,
#endif
	
#if (SMS_User_Number >= 0x03)
	TRUE,
#endif
	
#endif
};
/************************************************************************************************/
#if ((GSM_Module_Type==0)||(GSM_Module_Type==3)) //SIM800C��������ģ��
//GSM ���� ���ݼĴ���
static GSM_SMS_DR_Struct GSM_SMS_DR_Reg;
#endif
/************************************************************************************************/
/* GPRS Ӧ�üĴ��� */
GPRS_Struct GPRS;
/************************************************************************************************/

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
/************************************************************************************************/
/* ������	: GPRS_Timestamp_Hex_To_BCD_Code													*/
/* ����		: src:����HEX��;dest:���HEX��														*/
/* ���		: �� 																				*/
/* ����		: ��HEX��ʱ������ת����BCDʱ��������ʽ				 									*/
/* ��������	: 2015/03/30		HEX2BCD															*/
/************************************************************************************************/
static void GPRS_Timestamp_Hex_To_BCD_Code(GSM_GPRS_Data_Content_Struct* dest,Save_Sensor_Data_Struct* src)
{
	/* ��������� */
	dest->GPRS_DR_FIX.Timestamp_Sec 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS);
	/* ��÷����� */                        
	dest->GPRS_DR_FIX.Timestamp_Min 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS);
	/* ���ʱ���� */                        
	dest->GPRS_DR_FIX.Timestamp_Hour 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS);
	/* ��������� */                        
	dest->GPRS_DR_FIX.Timestamp_Day 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS);
	/* ��������� */                        
	dest->GPRS_DR_FIX.Timestamp_Month 	=   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS);
	/* ��������� *///������꣬��󱣴�63�꣬��2000�꿪ʼ���𣬳��������´�2010��ʼ��
	dest->GPRS_DR_FIX.Timestamp_Year 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS);
}
#endif

/************************************************************************************************/
/* ������	: GPRS_GET_Save_BSP_Sensor_Data														*/
/* ����		: src:����HEX��;dest:���HEX��														*/
/* ���		: �� 																				*/
/* ����		: ��ȡ�洢�İ��ش���������							 									*/
/* ��������	: 2015/03/30																		*/
/************************************************************************************************/
#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
#if ((Sensor_Tmp_Num+Sensor_Hum_Num+Sensor_PT1000_Num) != 0)
static void GPRS_GET_Save_BSP_Sensor_Data(GSM_GPRS_Data_Content_Struct* dest,Save_Sensor_Data_Struct* src)
{
	/* ���������ݴ�����ݵĿ�ʼλ�� */
	u8 sensor_data_offset = 0;
	/* GPRS���ݰ�������ݵ�λ�� */
	u8 gprs_data_content_offset = 0;
	/* ��ʪ������ */
	float sensor_data;

	
/* �ж� ͨ��1 */
#if 	((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
		(Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH1_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH1_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* �ж� ͨ��2 */
#if 	((Sensor_CH2_Config_Type == Sensor_Tmp) ||\
(Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH2_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH2_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��3 */
#if 	((Sensor_CH3_Config_Type == Sensor_Tmp) ||\
(Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH3_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH3_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��4 */
#if 	((Sensor_CH4_Config_Type == Sensor_Tmp) ||\
(Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH4_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH4_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��5 */
#if 	((Sensor_CH5_Config_Type == Sensor_Tmp) ||\
(Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH5_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH5_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��6 */
#if 	((Sensor_CH6_Config_Type == Sensor_Tmp) ||\
(Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH6_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH6_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��7 */
#if 	((Sensor_CH7_Config_Type == Sensor_Tmp) ||\
(Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
(Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
		 (Sensor_CH7_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH7_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��8 */
#if 	((Sensor_CH8_Config_Type == Sensor_Tmp) ||\
(Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH8_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH8_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* �ж� ͨ��9 */
#if 	((Sensor_CH9_Config_Type == Sensor_Tmp) ||\
(Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH9_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH9_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* �ж� ͨ��10 */
#if 	((Sensor_CH10_Config_Type == Sensor_Tmp) ||\
(Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH10_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH10_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��11 */
#if 	((Sensor_CH11_Config_Type == Sensor_Tmp) ||\
(Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH11_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH11_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��12 */
#if 	((Sensor_CH12_Config_Type == Sensor_Tmp) ||\
(Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH12_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH12_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��13 */
#if 	((Sensor_CH13_Config_Type == Sensor_Tmp) ||\
(Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH13_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH13_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��14 */
#if 	((Sensor_CH14_Config_Type == Sensor_Tmp) ||\
(Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH14_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH14_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��15 */
#if 	((Sensor_CH15_Config_Type == Sensor_Tmp) ||\
(Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH15_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH15_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��16 */
#if 	((Sensor_CH16_Config_Type == Sensor_Tmp) ||\
(Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH16_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH16_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* �ж� ͨ��17 */
#if 	((Sensor_CH17_Config_Type == Sensor_Tmp) ||\
(Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH17_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH17_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* �ж� ͨ��18 */
#if 	((Sensor_CH18_Config_Type == Sensor_Tmp) ||\
(Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH18_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH18_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��19 */
#if 	((Sensor_CH19_Config_Type == Sensor_Tmp) ||\
(Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH19_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH19_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��20 */
#if 	((Sensor_CH20_Config_Type == Sensor_Tmp) ||\
(Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH20_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH20_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��21 */
#if 	((Sensor_CH21_Config_Type == Sensor_Tmp) ||\
(Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH21_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH21_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��22 */
#if 	((Sensor_CH22_Config_Type == Sensor_Tmp) ||\
(Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH22_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH22_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��23 */
#if 	((Sensor_CH23_Config_Type == Sensor_Tmp) ||\
(Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH23_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH23_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��24 */
#if 	((Sensor_CH24_Config_Type == Sensor_Tmp) ||\
(Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH24_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH24_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* �ж� ͨ��25 */
#if 	((Sensor_CH25_Config_Type == Sensor_Tmp) ||\
(Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH25_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH25_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* �ж� ͨ��26 */
#if 	((Sensor_CH26_Config_Type == Sensor_Tmp) ||\
(Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH26_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH26_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* �ж� ͨ��27 */
#if 	((Sensor_CH27_Config_Type == Sensor_Tmp) ||\
(Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH27_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH27_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��28 */
#if 	((Sensor_CH28_Config_Type == Sensor_Tmp) ||\
(Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH28_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH28_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��29 */
#if 	((Sensor_CH29_Config_Type == Sensor_Tmp) ||\
(Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH29_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH29_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* �ж� ͨ��30 */
#if 	((Sensor_CH30_Config_Type == Sensor_Tmp) ||\
(Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH30_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH30_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��31 */
#if 	((Sensor_CH31_Config_Type == Sensor_Tmp) ||\
(Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH31_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH31_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* �ж� ͨ��32 */
#if 	((Sensor_CH32_Config_Type == Sensor_Tmp) ||\
(Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH32_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH32_Config_Type == Sensor_PT1000))
		//d����������
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e������ͨ����
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


}

#endif
#endif
	
/************************************************************************************************/
/* ������	: GPRS_GET_RF_Humiture_Hum_Data														*/
/* ����		: src:����HEX��;dest:���HEX��														*/
/* ���		: �� 																				*/
/* ����		: ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ��					 									*/
/* ��������	: 2015/03/30																		*/
/************************************************************************************************/
#if ((Sensor_RF_Humiture_Num > 0)||\
	 (AppGPRSComProtocol == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
static u8 GPRS_GET_RF_Humiture_Hum_Data(u32 dest_sn,INSTRU_SENSOR *sensor_t)
{
/* û�в鵽ƥ���ͨ�� */
#define Sensors_CH_Null (0xFF)
	
	
/* �ж� ͨ��1 */
#if		((Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH1.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_1;
		}
#endif
	
/* �ж� ͨ��2 */
#if		((Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH2.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_2;
		}
#endif
	
/* �ж� ͨ��3 */
#if		((Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH3.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_3;
		}
#endif	
	
/* �ж� ͨ��4 */
#if		((Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH4.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_4;
		}
#endif	
	
/* �ж� ͨ��5 */
#if		((Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH5.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_5;
		}
#endif

/* �ж� ͨ��6 */
#if		((Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH6.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_6;
		}
#endif

/* �ж� ͨ��7 */
#if		((Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH7.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_7;
		}
#endif

/* �ж� ͨ��8 */
#if		((Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH8.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_8;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH9.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_9;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH10.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_10;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH11.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_11;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH12.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_12;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH13.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_13;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH14.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_14;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH15.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_15;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH16.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_16;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH17.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_17;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH18.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_18;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH19.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_19;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH20.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_20;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH21.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_21;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH22.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_22;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH23.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_23;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH24.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_24;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH25.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_25;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH26.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_26;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH27.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_27;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH28.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_28;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH29.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_29;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH30.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_30;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH31.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_31;
		}
#endif
		
/* �ж� ͨ��8 */
#if		((Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
		/* �ж� ʪ��ͨ��SN����Ŀ���SN���Ƿ���ͬ */
		if(sensor_t->Inf_State.CH32.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ��Ŀ���SN����ͬ��˵����ʪ��ͨ����Ŀ��Ϊͬһ��Ƭ */
			return Sensors_CH_32;
		}
#endif
	
		/* �����û���ҵ�ƥ���ͨ�� */
		return Sensors_CH_Null;
#undef Sensors_CH_Null
}
#endif

/************************************************************************************************/
/* ������	��GPRS_Config_Server_Inf															*/
/* ����		��GPRS��������Ϣ																		*/
/* ���		���� 																				*/
/* ����		������GPRS������������IP��ַ���˿ڡ� 									 		 		*/
/* ��������	��2015/04/10																		*/
/************************************************************************************************/
//static void GPRS_Config_Server_Inf(INSTRU_GSM* gprs_inf)
//{

//	/* ��ȡ GPRS �� ��������Ϣ */
//	//��ȡ����
//	sprintf(GPRS_COM_Reg[GPRS_Server1_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID1);
//	sprintf(GPRS_COM_Reg[GPRS_Server2_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID2);
//	sprintf(GPRS_COM_Reg[GPRS_Server3_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID3);
//	//��ȡIP
//	if((gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server1_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server1_NO].ip,"");
//	}
//	
//	if((gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server2_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server2_NO].ip,"");
//	}
//	
//	if((gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"");
//		//sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"115.28.105.232");//2017.11.06��������������IP��ַ�̶�
//	}
//	
//	//��ȡ�˿�
//	sprintf(GPRS_COM_Reg[GPRS_Server1_NO].port,"%05d",gprs_inf->Inf_State.Port_ID1);	
//	sprintf(GPRS_COM_Reg[GPRS_Server2_NO].port,"%05d",gprs_inf->Inf_State.Port_ID2);
//	sprintf(GPRS_COM_Reg[GPRS_Server3_NO].port,"%05d",gprs_inf->Inf_State.Port_ID3);
// // sprintf(GPRS_COM_Reg[GPRS_Server3_NO].port,"%05d",12342);//2017.11.06�������������Ķ˿ڹ̶�
//	
//}

/************************************************************************************************/
/* ������	��GSM_Module_Config																	*/
/* ����		��GPRS��������Ϣ																		*/
/* ���		���� 																				*/
/* ����		������ GSM ģ��					 									 		 		*/
/* ��������	��2015/04/10																		*/
/************************************************************************************************/
static void GSM_Module_Config(INSTRU_GSM* gprs_inf)
{
	
	/* ��� ����ʧ�ܴ��� */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum = 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum = 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum = 0;
	/* ���� GPRS ������ ��Ϣ */
//	GPRS_Config_Server_Inf(gprs_inf);
	
	
	GSMConfig_Server_Inf(gprs_inf);
	
	
	
	/* �������� GSM ģ�� */
//	GPRS_SR_Reg.GPRS_Work_State = (BOOL)gsm_env_init(GPRS_COM_Reg,(_SCSStruct*)&GPRS_SR_Reg.SimState);
	GPRS_SR_Reg.GPRS_Work_State = (BOOL)Sim800c_env_init((_SCSStruct*)&GPRS_SR_Reg.SimState);	
	

	//�����һЩ�ؼ��ĵ�״̬��Ϣ�Ժ����������Ļ��ʾ������ʾ����Щ�������ȡ������***��־
	/* �ж� ģ�� ���� ��� */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		
//		printf("\rGSMģ���ʼ�����óɹ�������\r\n");	
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* GSMģ�� ���� �ɹ� */
//			printf("\rGSMģ�����óɹ�\r\n");						
//		}				
	}else
	{
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* GSMģ�� ���� ʧ�� */
//			printf("\rGSMģ���ʼ������ʧ�ܣ�����\r\n");
//		}	
	}
}

/************************************************************************************************/
/* ������	�GSMConfig_Server_Inf(INSTRU_GSM* gprs_inf)													*/
/* ����		��GPRS��������Ϣ																		*/
/* ���		���� 																				*/
/* ����		������GPRS������������IP��ַ���˿ڡ� 									 		 		*/
/* ��������	��2018/08/30																		*/
/************************************************************************************************/
void GSMConfig_Server_Inf(INSTRU_GSM* gprs_inf)
{

	/* ��ȡ GPRS �� ��������Ϣ */
	//��ȡ����
	sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID1);
	sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID2);
	sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID3);
	//��ȡIP
	if((gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].ip,"");
	}
	
	if((gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].ip,"");
	}
	
	if((gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"");
		//sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"115.28.105.232");//2017.11.06��������������IP��ַ�̶�
	}
	
	//��ȡ�˿�
	sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].port,"%d",gprs_inf->Inf_State.Port_ID1);	
	sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].port,"%d",gprs_inf->Inf_State.Port_ID2);
	sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].port,"%d",gprs_inf->Inf_State.Port_ID3);
 // sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].port,"%05d",12342);//2017.11.06�������������Ķ˿ڹ̶�
	
}
//��ӡGSMģ�鷢�ͺͽ��յ�������Ϣ
void PrintGsmDebug(char *wbuf, int wlen, char *rbuf, int *rlen,u8 Server_Id)
{
	      unsigned int i;
       	if(DEBUG_Flag ==TRUE)	
				{
				   switch(Server_Id)
					{
						 case 0:
						 {
							     if(wlen>0)//����
							     {
							    	  printf("���͸����ݷ�����������:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("���͸����ݷ����������ݳ���Ϊ0������\n"); 
							     }
							     if(rlen>0)//����
							     {
							    	  printf("���յ����ݷ�����������:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("���յ����ݷ����������ݳ���Ϊ0������\n"); 
							     }	 
							     break;
						 }
						  case 1:
						 {
							     if(wlen>0)//����
							     {
							    	  printf("���͸����÷�����������:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("���͸����÷����������ݳ���Ϊ0������\n"); 
							     }
							     if(rlen>0)//����
							     {
							    	  printf("���յ����÷�����������:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("���յ����÷����������ݳ���Ϊ0������\n"); 
							     }	 
							     break;
						 }
						 	case 2:
						 {
							     if(wlen>0)//����
							     {
							    	  printf("���͸�����������������:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("���͸����������������ݳ���Ϊ0������\n"); 
							     }
							     if(rlen>0)//����
							     {
							    	  printf("���յ�����������������:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("���յ����������������ݳ���Ϊ0������\n"); 
							     }	 
							     break;
						 }
						 
					
					   default:
						break;
					
					}
					
				
				
				
				}					

}

/************************************************************************************************/
/* ������	��GSM_Function																		*/
/* ����		��gsm_que_inf:��ǰ����Ϣ����															*/
/* ���		���� 																				*/
/* ����		��ִ����Ϣ���� ���е� ��Ϣ����			 								 		 		*/
/* ��������	��2015/04/01																		*/
/************************************************************************************************/
void GSM_Function(GSM_QUE*  gsm_que_inf,INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
//	/* �ж� ��ǰ �ĵ�ص��� */
//	if(MCU_Battery.Power_State > Power_Null)
//	{
		/* ��ǰ�ĵ�ص����ܹ�ά�� GSMģ������������������3.5V�� */
		/* ���� ��Ϣ���� ���������� */
		switch(gsm_que_inf->EVENT_Cmd)
		{
			case Init_GSM_Module_EVENT:
			{
				
				/* GSM ��ʼ������ */
				GSM_Module_Config(&config_tab->Inf_State.Instru_GSM);
				break;
			}

			default :
			{
				/* ���ں���� ���� ���ǻ���ģ��ɹ����ú��������ʹ�õģ���������ж� */
				if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
				{
					/* GSM ģ�����óɹ� */
					/* ���� ��Ϣ���� ���������� */
					switch(gsm_que_inf->EVENT_Cmd)
					{
						//	/* �����ź�ǿ�� */
						case Update_SGL_EVENT:
						{	
							/* ��õ�ǰ �ź�ǿ�� */
//						GPRS_SR_Reg.GPRS_Sign_Stre = gsm_get_signal();
						GPRS_SR_Reg.GPRS_Sign_Stre =Gsm_get_signal();
						break;
						}										
/*********************************************************************************/				
						//	/* ���¸���״̬ */
						case Update_Link_EVENT:
						{
//							/* ��õ�ǰ�� ����״̬ */
//						GPRS_SR_Reg.GPRS_Link_State = (BOOL)gsm_get_gprs_state();
			
//////////						GPRS_SR_Reg.GPRS_Link_State =(BOOL)Gsm_get_gprs_state();
						break;
						}
/*********************************************************************************/						
						//	/* �ϴ���ʪ������ */
						case Updata_Data_EVENT:
						{		
////						    	/* �������� */
////					       GPRS_Send_Network_Data();
						break;
						}
/*********************************************************************************/		
						//	/* ���� ������� ���� */
						case IAP_EVENT:
						{	
							#if (AppGPRSComProtocolEnable == 0x01)
              #if (AppGPRSComProtocol == 0x00)	
							/* ��ȡ ���³��� ���� */
							GPRS.IAP.State.Result = 
							(BOOL)Gsm_gprs_comm(
							((char*)GPRS.IAP.Para.TXDR),
							(GPRS.IAP.Para.TXLen),
							((char*)GPRS.IAP.Para.RXDR),
							((&GPRS.IAP.Para.RXLen)),
							 GSM_GPRS_Update
							);
							
              PrintGsmDebug(
							((char*)GPRS.IAP.Para.TXDR),
							(GPRS.IAP.Para.TXLen),
							((char*)GPRS.IAP.Para.RXDR),
							((&GPRS.IAP.Para.RXLen)),
							 GSM_GPRS_Update
							);	
							/* ���ݷ���ʧ�� */
							if(FALSE == GPRS.IAP.State.Result)
							{
								GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum++;	

								/* �жϷ���ʧ�� ���� ����������������ʧ��10����ʱ������Ƭ�� */
								if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum >= 10)
								{
									   mcu_reset();//�����豸  ��λ��Ƭ��zwc
//									/* ��������GSMģ�� */
//									GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
								}							
							}else
							{
									/* ��� GPRS ���ݷ���ʧ�ܴ��� */
									GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum = 0;
							}
							/* ��Ϣִ����� */
							gsm_que_inf->EVENT_Getarg = TRUE;
							/* �̼����� */
							IAP_UploadLog(&GPRS);
						   #endif
							 #endif
							break;	
						}
/*********************************************************************************/		
#if (GSM_Module_Type==0x00) //SIM800Cģ��		
						/* ���Ͷ��� */
						case Send_SMS_EVENT:
						{

						 GSM_SMS_Send(&config_tab->Inf_State.Instru_GSM);
							
						   break;
						}
/*********************************************************************************/			
#endif				
						/* �ϴ� ���� ���� */
						case ConfigService_EVENT:
						{
////////								/* �������� */
////////								GPRS.ConfigService.State.Result = 
////////								(BOOL)Gsm_gprs_comm(
////////								(char*)GPRS.ConfigService.Para.TXDR,
////////								(GPRS.ConfigService.Para.TXLen),
////////								((char*)GPRS.ConfigService.Para.RXDR),
////////								((&GPRS.ConfigService.Para.RXLen)),
////////								  GSM_GPRS_Config
////////								);
								 #if (AppGPRSComProtocolEnable == 0x01)
                 #if (AppGPRSComProtocol == 0x00)	
								PrintGsmDebug(
								(char*)GPRS.ConfigService.Para.TXDR,
								(GPRS.ConfigService.Para.TXLen),
								((char*)GPRS.ConfigService.Para.RXDR),
								((&GPRS.ConfigService.Para.RXLen)),
								  GSM_GPRS_Config
								);
									#endif
							    #endif
							/* ���ݷ���ʧ�� */
							if(FALSE == GPRS.ConfigService.State.Result)
							{
								 GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum++;	
								/* �жϷ���ʧ�� ���� */
								if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum >= 5)
								{
										 
										/* ��������GSMģ�� */
									//	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
								}
							}else
							{
								/* ��� GPRS ���ݷ���ʧ�ܴ��� */
								GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum = 0;
							}
							/* ��Ϣִ����� */
							gsm_que_inf->EVENT_Getarg = TRUE;	
							ConfigService_RXLog(GPRS,&Instru_Config_Inf_Tab);
							break;
						}
/*************************************** ϵͳʱ��У׼ ******************************************/	
						case System_Clock_Calibration_EVENT:	
						{
							 /* GPRS����ʱ��У׼���� */
					  	GPRS_Send_System_Clock_Calibration_Data();	
							break;							
						}
/*********************************************************************************/		
						default :
						/* δ֪���͵� ��Ϣ */
						__nop();
						break;
					}
				}else
				{
					/* GSMģ������ ʧ�� */
					/* ģ�� ֹͣ���� ��Ҫ�������� */
					/* ���� ��Ϣִ�н�� */
					gsm_que_inf->EVENT_Getarg = TRUE;
					switch(gsm_que_inf->EVENT_Cmd)
					{
						//	/* �����ź�ǿ�� */
						case Update_SGL_EVENT:
						__nop();
						break;
/*********************************************************************************/				
						//	/* ���¸���״̬ */
						case Update_Link_EVENT:
						__nop();
						break;
/*********************************************************************************/						
						//	/* �ϴ���ʪ������ */
						case Updata_Data_EVENT:
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State			 				= FALSE;
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State 	= TRUE;//(��ǰ״̬����Ҫ����)
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result 						= FALSE;
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result 						= FALSE;
						break;
/*********************************************************************************/		
						/* ���� ������� ���� */
						case IAP_EVENT:
						{
							#if (AppGPRSComProtocolEnable == 0x01)
              #if (AppGPRSComProtocol == 0x00)
							/* GSMģ��û�й�����ֹͣ�̼����� */
							IAP_StopRun();
							#endif
							#endif
							break;
						}
/*********************************************************************************/		
						/* ���Ͷ��� */
						case Send_SMS_EVENT:
						/* δ֪���͵� ��Ϣ */
/* �ж��û����� */
#if SMS_User_Number == 0x01	
						/* GSMģ�鹤���쳣�������Ͷ��š� */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
#elif SMS_User_Number == 0x02
						/* GSMģ�鹤���쳣�������Ͷ��š� */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
						/* GSMģ�鹤���쳣�������Ͷ��š� */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = FALSE;
#elif SMS_User_Number == 0x03
						/* GSMģ�鹤���쳣�������Ͷ��š� */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
						/* GSMģ�鹤���쳣�������Ͷ��š� */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = FALSE;
						/* GSMģ�鹤���쳣�������Ͷ��š� */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = FALSE; 
#endif
						/* ���� ���ŷ������״̬ */
						GSM_SMS_SR_Reg.GSM_SMS_Send_State = FALSE;
						break;
/*********************************************************************************/					
						/* �ϴ� ���� ���� */
						case ConfigService_EVENT:
						{
//			 /* GSMģ��û�й�����ֹͣ���� */
							ConfigService_StopRun();
							break;
						}
/*************************************** ϵͳʱ��У׼ ******************************************/	
						case System_Clock_Calibration_EVENT:	
						{
							/* GSMģ��δ���ã�������ʱ��У׼û�б�ִ�У��ָ�ϵͳʱ��У׼״̬�Ĵ��� */
							RTC_System_Clock_Calibration_State_Reset();
							break;							
						}
/*********************************************************************************/		
						default :
						/* δ֪���͵� ��Ϣ */
						__nop();
						break;
					}
				}
				break;
			}
		}
//	}else
//	{
//		/* �޸� GSMģ��״̬:ʧ�� */
//		GPRS_SR_Reg.GPRS_Work_State = FALSE;
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* ��ص������㣬GSM�޷����������� */
//			printf("\r��ص������㣬GSM�޷�����������\r\n");				
//		}
//	}
	/* ��Ϣִ����� */
	gsm_que_inf->EVENT_Getarg = TRUE;
}
/************************************************************************************************/
/* ������	��GPRS_Send_Network_Data															*/
/* ����		��״̬��Ϣ��ӡ��־																	*/
/* ���		���� 																				*/
/* ����		��GPRS ���� ����������			 									 		 		*/
/* ��������	��2015/04/01																		*/
/************************************************************************************************/
/**
  * @brief  Description "GPRSͨѶ"
  * @param  wbuf		���ͻ�����
  * @param  wlen		���ͻ������ֽ�����С
  * @param  rbuf		���ջ�����
  * @param  rlen		���ջ������ֽ�����С
  * @retval bool		GPRSͨѶ�Ƿ�ɹ
bool gsm_gprs_comm(char *wbuf, int wlen, char *rbuf, int* rlen)
  */
 void GPRS_Send_Network_Data(void)//Ҫ�޸ĵĺ���ZWC
{
/* GPRS ��������������ʧ�ܴ������� */
#define GPRS_COM_Send_Network_Fail_Num_Limit	(3u)
	
	/* ���� GPRS����������ݳ��ȡ� */
	GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len = GPRS_Rece_Sensor_Data_Buffer_Size;
/* �������������� *//*****************���������������*********************************/
//	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = 
//	(BOOL)gsm_gprs_comm(
//	  (char*)GPRS_Send_Sensor_Data_Buffer,
//		(GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
//	  (char*)GPRS_Rece_Sensor_Data_Buffer,
//	  ((int*)(&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)));
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = 
	  (BOOL)Gsm_gprs_comm(
	                      (char*)GPRS_Send_Sensor_Data_Buffer,
		                    (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
	                      (char*)GPRS_Rece_Sensor_Data_Buffer,
	                      ((&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)),
									       GSM_GPRS_Data
								       );
	if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result)//��ӡ������Ϣ
	{
				PrintGsmDebug(
	                     (char*)GPRS_Send_Sensor_Data_Buffer,
		                   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
	                     (char*)GPRS_Rece_Sensor_Data_Buffer,
	                     ((&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)),
									     GSM_GPRS_Data
								     );	
	}		
					 
									 

/******************************END******************************************************/
	/* �ж� ���ݷ��ͽ�� */
	if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == FALSE)
	{
		/* ���ݷ���ʧ�� */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum++;//���ݷ���ʧ�ܼ�����1
		
		/* �жϷ���ʧ�� ���� */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum >= GPRS_COM_Send_Network_Fail_Num_Limit)//�����������ʧ�ܴ����ﵽ3�������¶�GSMģ���������
		{
			/* ��������GSMģ�� */
			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		}
		/* 
			���� ���������� ���ڷ��� ״̬:���·��ʹ�������ǰ����
			(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE) 
		*/;
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;//(��ǰ״̬����Ҫ����)
	}else//���ݷ��ͳɹ�
	{
		 Log_printf("�����ݷ��������ʹ��������ݳɹ�������\r\n");	
		
		/* ���� ���������ص����ݣ��жϱ���GPRSͨ��״̬ */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_Network_Data();
		/* �ж� ����GPRSͨ�� ��� */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE)//�����������ͨ������
		{
#if (AppGYForwardEnable == 0x01)			
			/* �жϴ�����ҩ���ݱ�־ */
			if(TRUE == AppComProtocolGY.SR.CreateDataFlag)
			{
				/* ���ݴ����ɹ�����ʼ��������. */
				/* ��������״̬ */
				BOOL gSendState;
				/* �������ݳ��� */
				int gSendLen;
				/* �������ݳ��� */
				int gReceLen = (128ul);


				/* ���㷢�����ݳ��� */
				gSendLen = strlen((char*)AppComProtocolGY.DR.TXDR);

				/* ���ù�ҩ�ϴ�IP/�˿� */
				GSM_Config_Data_Network((void*)&AppComProtocolGY.CR.NetworkConnection);
				
				/* ���͹�ҩ���� */
				gSendState = (BOOL)gsm_gprs_comm(
					(char*)AppComProtocolGY.DR.TXDR,
					(gSendLen),
					(char*)AppComProtocolGY.DR.RXDR,
					(&gReceLen));
					
				/* �ж����ݷ���״̬ */
				if(TRUE == gSendState)
				{
					/* ���ݷ��ͳɹ� */
					__nop();
				}
				
				/* ����Ĭ���ϴ�IP/�˿� */
				GSM_Config_Data_Network((void*)&GSM_Tcp_Conn[0]);	
			}
#endif
			
#if (ComProtocol_WZ_Enable == 0x01)			
			/* �жϴ����������ݱ�־ */
			if(TRUE == ComProtocolWZDR.SR.CreateDataFlag)
			{
				/* ���ݴ����ɹ�����ʼ��������. */
				/* ��������״̬ */
				BOOL gSendState;
				/* �������ݳ��� */
				int gSendLen;
				/* �������ݳ��� */
				int gReceLen = (128ul);


				/* ���㷢�����ݳ��� */
				gSendLen = strlen((char*)ComProtocolWZDR.DR.TXDR);

				/* ���ù�ҩ�ϴ�IP/�˿� */
				GSM_Config_Data_Network((void*)&ComProtocolWZDR.CR.NetworkConnection);
				
				/* ���͹�ҩ���� */
				gSendState = (BOOL)gsm_gprs_comm(
					(char*)ComProtocolWZDR.DR.TXDR,
					(gSendLen),
					(char*)ComProtocolWZDR.DR.RXDR,
					(&gReceLen));
					
				/* �ж����ݷ���״̬ */
				if(TRUE == gSendState)
				{
					/* ���ݷ��ͳɹ� */
					__nop();
				}
				
				/* ����Ĭ���ϴ�IP/�˿� */
				GSM_Config_Data_Network((void*)&GPRS_COM_Reg[0]);	
			}
#endif	
			
			/* 
				���� ���������� ���ڷ��� ״̬:�����µĴ�������ǰ����
				(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE)
			*/
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = FALSE;
			
			/* �ж� ����ʧ�� ���� */
			if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum)
			{
				/* ��� GPRS ���ݷ���ʧ�ܴ��� */
				GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum = 0;
			}
		}else
		{
			/* 
				���� ���������� ���ڷ��� ״̬:���·��ʹ�������ǰ����
				(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE) 
			*/
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;//(��ǰ״̬����Ҫ����)
			/* ���ݽ���ʧ�� */
			//�ڲ���дһ�²�����������
			//��HTTPЭ�飬�Ʒ������쳣�ᵼ�½��յ�"9999"(�������쳣ָ��)
			//����취������������¼�����ʱ1���ӷ������ݣ�����������������40M/�¡�
			/* ���ý���ʧ�ܱ�־ */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState = FALSE;
		}
	}
	/* 
		���� ���������� ���������� ���ڷ��� ״̬:���ݷ������
	*/
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = FALSE;
#undef GPRS_COM_Send_Network_Fail_Num_Limit
}

/*
*********************************************************************************************************
*	�� �� ��: GPRS_Get_Save_Sensor_Data
*	����˵��: ��ô洢�Ĵ���������
*	��    �Σ���
*	�� �� ֵ: �� Instru_Config_Inf_Tab  Instru_Fix_Inf_State_Tab
*********************************************************************************************************
*/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
static BOOL GPRS_Get_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
/* Ĭ��:��:0x00 */
#define GPRS_Sensor_Null				(0x00)		
/* ����������:��ʪ�ȴ�����:0x01 */
#define GPRS_Sensor_Board_On			(0x01)	
/* ����������:���¿�Ƭ:0x02 */
#define GPRS_Sensor_RF_Tmp				(0x02)	
/* ����������:��ʪ�ȿ�Ƭ:0x03 */	
#define GPRS_Sensor_RF_Hum				(0x03)
/* �������ݼĴ����ռ��С */
#define GPRS_Send_Reg_Size				(sizeof(GSM_GPRS_Send_Struct))
	
#if (AppGPRSComProtocol == 0x03)//�Ӵ�Э��
/* �������ݼĴ��� �������ݵĹ̶����� */
#define GPRS_Send_Data_Content_Fix_Len	(20u)//zwc
#endif

#if (AppGPRSComProtocol == 0x00)//����Э��
/* �������ݼĴ��� �������ݵĹ̶����� */
#define GPRS_Send_Data_Content_Fix_Len	(16u)//zwc
#endif
/* �������ݼĴ��� �̶����� */
#define GPRS_Send_Data_Fix_Len			(22u)
/* һ֡�����ڴ洢�ĳ��� */
#if ((Sensor_Tmp_Num+Sensor_Hum_Num+Sensor_CarbonDioxide_Num+Sensor_DoorContact_Num+Sensor_Lon_Num+Sensor_Lat_Num+Sensor_Spd_Num+Sensor_PT1000_Num) > 0)
#define Sensors_Data_Store_Size		(GPRS_Send_Data_Content_Fix_Len+\
									(Sensor_Tmp_Num+Sensor_CarbonDioxide_Num+Sensor_DoorContact_Num+Sensor_Hum_Num+Sensor_Lon_Num+Sensor_Lat_Num+Sensor_Spd_Num+Sensor_PT1000_Num)*sizeof(float)+\
									(Sensor_RF_Single_Tmp_Num)*(GPRS_Send_Data_Content_Fix_Len+sizeof(float))+\
									(Sensor_RF_Humiture_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									(Sensor_GW_NJSY_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									(Sensor_GW_HZZH_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float)))
#else
#define Sensors_Data_Store_Size		((Sensor_RF_Single_Tmp_Num)*(GPRS_Send_Data_Content_Fix_Len+sizeof(float))+\
									 (Sensor_RF_Humiture_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									 (Sensor_GW_NJSY_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									 (Sensor_GW_HZZH_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float)))
#endif			

#if (AppGYForwardEnable == 0x00)
/* ���η�����ʪ����������������֡���� */
#define GPRS_Send_Data_Frames_Max_Num	((u8)((GPRS_Send_Reg_Size-GPRS_Send_Data_Fix_Len)/Sensors_Data_Store_Size))
#elif (AppGYForwardEnable == 0x01)
#define GPRS_Send_Data_Frames_Max_Num	(1u)
#endif
									 /* һ֡���ݰ�������������(��������SN������) */
#define Sensors_One_Frames_Include_Data_Num		((((Sensor_Tmp_Num>0)||\
													  (Sensor_DoorContact_Num>0) ||\
													  (Sensor_CarbonDioxide_Num>0) ||\
												    (Sensor_Hum_Num>0)||\
												    (Sensor_PT1000_Num>0))?1:0)+\
													   Sensor_RF_Single_Tmp_Num+\
												     Sensor_RF_Humiture_Num+\
													   Sensor_GW_NJSY_Num+\
													   Sensor_GW_HZZH_Num)
/* û�в鵽ƥ���ͨ�� */
#define Sensors_CH_Null (0xFF)
	/* �� Fram ���ݵĻ���ָ�� */
	Save_Sensor_Data_Struct *read_sensor_data_p;
	/* �������ݼĴ��� ��������ָ�� */
	u8 * gprs_data_content_p = &GPRS_Send_Reg.GPRS_IR_FIX.Data_P;
#if ((Wireless_RF_Card_Config_Enable == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
	/* ��ʪ������ */
	float sensor_data;
	#if ((Sensor_RF_Humiture_Num > 0)||\
		 (Sensor_GW_NJSY_Num > 0)||\
		 (Sensor_GW_HZZH_Num > 0))
	/* ��ʪ�ȿ�Ƭ��ʪ��ͨ����� */
	u8 rf_hum_no;					
	#endif							
#endif								
	/* ��ǰ���������֡�� */
	u8 data_frames_num;
	/* ������ */
	u16 num;

	/* �ж� �Ƿ�����ʷ���� */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM < Save_Sensor_Data_Len)
	{
		/* û����ʷ���� */
		return FALSE;
	}
									 							
	//1����λ�������ݼĴ�������
	/* �������� �Ĵ��� ���� */
	GPRS_Send_Reg = GPRS_Send_Reg_Fix;
	/* ���� SN�� */
	memcpy(GPRS_Send_Reg.GPRS_IR_FIX.SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);

	//2����ȡ��������							 
	for(data_frames_num = 0;
		 (fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)&&
		 (data_frames_num < GPRS_Send_Data_Frames_Max_Num);
		 data_frames_num++
	   )
	{
		/* ��ȡ��ǰ�Ĵ�����ͨ������ */
		/* ���ݵ�ǰ�ⲿ�洢����W25Q64�������ݴ洢ָ������������ */
		if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)
		{
			/* Flash����ʷ���ݣ��Ͷ�ȡ���ݣ��ȷ���ʷ���� */
			read_sensor_data_p = (Save_Sensor_Data_Struct*)Flash_Read_Sensor_Data(
								  fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size,
								  Save_Sensor_Data_Len);

			//���� Flash ��ָ��
			/* ���� ��ȡ�����ݳ��� */
			fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer 		  += Save_Sensor_Data_Len; 
			/* �洢������ ��*/
			fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	-= Save_Sensor_Data_Len;
			
#if (AppGYForwardEnable == 0x01)
			/* ������ҩ̽ͷ���� */
			AppComProtocolGY.SR.CreateDataFlag = ComProtGY_CreateData(read_sensor_data_p);
#endif	
#if (ComProtocol_WZ_Enable == 0x01)
			/* ������ҩ̽ͷ���� */
			ComProtocolWZDR.SR.CreateDataFlag = ComProtWZ_CreateData(read_sensor_data_p);
#endif			
			
		
		}
		//I������������ͨ����������һ��Ҫ˵�������ڰ��صĴ��������ֻ�а˸���������������ķ�ΧҲ����ǰ��8��ͨ�����
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH1_Config_Type == Sensor_Hum) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH2_Config_Type == Sensor_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH2_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH3_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH3_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH4_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH4_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH5_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH5_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH6_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH6_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH7_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
	   (Sensor_CH7_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH8_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH8_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH9_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH9_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH10_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH10_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH11_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH11_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH12_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH12_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH13_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH13_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH14_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH14_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH15_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH15_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH16_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH16_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH17_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH17_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH18_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH18_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH19_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH19_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH20_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH20_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH21_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH21_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH22_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH22_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH23_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH23_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH24_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH24_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH25_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH25_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH26_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH26_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH27_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH27_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH28_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH28_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH29_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH29_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH30_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH30_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH31_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH31_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH32_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH32_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH32_Config_Type == Sensor_PT1000))
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		/* �����д��������ã��������������� */
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),sizeof(u32));
		//b����������״̬
		/* ��ȡ��Դ״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		//�ź�ǿ��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved1 = (u8)GPRS_SR_Reg.GPRS_Sign_Stre;
/******************************zwc����Э���޸�*************************************/
	     #if (AppGPRSComProtocol == 0x03)
		   {
				 //Ԥ���ֽ�zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved2 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved3 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved4 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved5 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved6 = 0x00;//zwc
	     }	
			 #endif
/*********************************************************************************/	
/* ���������״̬������ͬ�ģ�����������ͨ��1-8��˳������ȷ��ʹ���Ǹ�ͨ���ĵ�Դ״̬�͵�ص������� */		
#if	((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
     (Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH1_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH1_Config_Type == Sensor_PT1000))
		//��ص�������
			if(AppGPRSComProtocol == 0x03)//�ӱ��Ӵ�Э��
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//����Э��(��:����Э���)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}				
#elif ((Sensor_CH2_Config_Type == Sensor_Tmp) ||\
(Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH2_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000))
				//��ص�������
			if(AppGPRSComProtocol == 0x03)//�ӱ��Ӵ�Э��
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//����Э��(��:����Э���)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}
		
		
		
#elif ((Sensor_CH3_Config_Type == Sensor_Tmp) ||\
(Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH3_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH3_Config_Type == Sensor_PT1000))
				//��ص�������
			if(AppGPRSComProtocol == 0x03)//�ӱ��Ӵ�Э��
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//����Э��(��:����Э���)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}	
#elif ((Sensor_CH4_Config_Type == Sensor_Tmp) ||\
(Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH4_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH4_Config_Type == Sensor_PT1000))
				//��ص�������
			if(AppGPRSComProtocol == 0x03)//�ӱ��Ӵ�Э��
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//����Э��(��:����Э���)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4; 
			}
			
			
#elif ((Sensor_CH5_Config_Type == Sensor_Tmp) ||\
(Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH5_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH5_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
#elif ((Sensor_CH6_Config_Type == Sensor_Tmp) ||\
(Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH6_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH6_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
#elif ((Sensor_CH7_Config_Type == Sensor_Tmp) ||\
			(Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
			(Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
	   (Sensor_CH7_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH7_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
#elif ((Sensor_CH8_Config_Type == Sensor_Tmp) ||\
(Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH8_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH8_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;


#elif ((Sensor_CH9_Config_Type == Sensor_Tmp) ||\
(Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH9_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH9_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
#elif ((Sensor_CH10_Config_Type == Sensor_Tmp) ||\
(Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH10_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH10_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
#elif ((Sensor_CH11_Config_Type == Sensor_Tmp) ||\
(Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH11_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH11_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
#elif ((Sensor_CH12_Config_Type == Sensor_Tmp) ||\
(Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH12_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH12_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
#elif ((Sensor_CH13_Config_Type == Sensor_Tmp) ||\
(Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH13_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH13_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
#elif ((Sensor_CH14_Config_Type == Sensor_Tmp) ||\
(Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH14_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH14_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
#elif ((Sensor_CH15_Config_Type == Sensor_Tmp) ||\
(Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH15_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH15_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
#elif ((Sensor_CH16_Config_Type == Sensor_Tmp) ||\
(Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH16_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH16_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;

#elif ((Sensor_CH17_Config_Type == Sensor_Tmp) ||\
(Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH17_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH17_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
#elif ((Sensor_CH18_Config_Type == Sensor_Tmp) ||\
(Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH18_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH18_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
#elif ((Sensor_CH19_Config_Type == Sensor_Tmp) ||\
(Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH19_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH19_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
#elif ((Sensor_CH20_Config_Type == Sensor_Tmp) ||\
(Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH20_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH20_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
#elif ((Sensor_CH21_Config_Type == Sensor_Tmp) ||\
(Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH21_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH21_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
#elif ((Sensor_CH22_Config_Type == Sensor_Tmp) ||\
(Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH22_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH22_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
#elif ((Sensor_CH23_Config_Type == Sensor_Tmp) ||\
(Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH23_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH23_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
#elif ((Sensor_CH24_Config_Type == Sensor_Tmp) ||\
(Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH24_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH24_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
#elif ((Sensor_CH25_Config_Type == Sensor_Tmp) ||\
(Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH25_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH25_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
#elif ((Sensor_CH26_Config_Type == Sensor_Tmp) ||\
(Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH26_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
#elif ((Sensor_CH27_Config_Type == Sensor_Tmp) ||\
(Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH27_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH27_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
#elif ((Sensor_CH28_Config_Type == Sensor_Tmp) ||\
(Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH28_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH28_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
#elif ((Sensor_CH29_Config_Type == Sensor_Tmp) ||\
(Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH29_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH29_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
#elif ((Sensor_CH30_Config_Type == Sensor_Tmp) ||\
(Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH30_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH30_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
#elif ((Sensor_CH31_Config_Type == Sensor_Tmp) ||\
(Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH31_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH31_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
#elif ((Sensor_CH32_Config_Type == Sensor_Tmp) ||\
(Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH32_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH32_Config_Type == Sensor_PT1000))
		/* ��ȡ��ص�������״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;

#endif

		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_Board_On;
		//d���������� �� ����ͨ����
		GPRS_GET_Save_BSP_Sensor_Data(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//e�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//f����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#endif
		//II���������¿�Ƭͨ������Ƭͨ���������õ���󴫸�������������������
#if (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO, config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///��������/������������ֻ��������״̬
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum)
		__nop();	
#else
		__nop();
#endif		



#if (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif


#if (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif
		
		
#if (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();	
#else
		__nop();
#endif	
		
		
#if (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	

		
#if (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif


#if (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	

		
#if (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif		


#if (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


#if (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


#if (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif		


#if (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
		//ͨ��һ�����óɵ��¿�Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e����������
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp))
		//ͨ��һ�����ó���ʪ��Ƭ����
		/* ��λ�������ݼĴ��� */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a������ SN��
		/* ��ȡ SN�� */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b����������״̬
		/* ��ȡ��Դ״̬ *///��Ƭû��δ�ӵ�Դ�����״̬�̶�Ϊ������1��
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* ��ȡ��ص�������״̬ *///���״̬��Ϣ��ʱû�ж��壬����������Ϣ�̶�������ӡ�
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b������ʱ���
		/* ��ȡ ʱ��� */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c������ͨ������
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d������ͨ����
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e�������¶�����
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* ������д�С������ת������Ȼ����е��ѿ��� */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* �ж���ʪ�ȿ�Ƭ�Ƿ���ƥ���ʪ��ͨ�� */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f������ʪ������
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ������д�С������ת������Ȼ����е��ѿ��� */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g�������õ��������ݵ��뵽��ʪ�����ݼĴ�����
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h����ָ��ָ������ĩβ
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


	}
	/* ������Ϣ�峤�� *///����֡��*һ�����ݵĳ���+��Ϣ���ͣ�1�ֽڣ�+ �������ͣ�1�ֽڣ�+ ����֡����1�ֽڣ�
	GPRS_Send_Reg.GPRS_IR_FIX.Attribute_HSB_Length = (((u16)Sensors_Data_Store_Size*data_frames_num+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num))/0X100);
	GPRS_Send_Reg.GPRS_IR_FIX.Attribute_LSB_Length = (((u16)Sensors_Data_Store_Size*data_frames_num+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num))%0X100);
	/* ��������֡�� */
	GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num = data_frames_num*Sensors_One_Frames_Include_Data_Num;

/**********************************����У����**************************************************/															
	/* ���� ��Ϣͷ����Ϣ�� ������ ��� У���� */
	for(num = 1,*(gprs_data_content_p) = 0;num < ((u16)(gprs_data_content_p - GPRS_Send_Reg.GSM_GPRS_Send_Buffer)); num++)
	{
		/* �� ��Ϣͷ �� ��Ϣ�� ���һ�� ���� ��� */
		*(gprs_data_content_p) ^= GPRS_Send_Reg.GSM_GPRS_Send_Buffer[num];
	}
/**************************************END*****************************************************/	

	/* ִ�е����� ˵�� ���óɹ� */
	return TRUE;
	
#undef GPRS_Sensor_Null
#undef GPRS_Sensor_Board_On
#undef GPRS_Sensor_RF_Tmp
#undef GPRS_Sensor_RF_Hum
#undef GPRS_Send_Reg_Size
#undef GPRS_Send_Data_Content_Fix_Len
#undef GPRS_Send_Data_Fix_Len
#undef Sensors_Data_Store_Size	
#undef GPRS_Send_Data_Frames_Max_Num
#undef Sensors_One_Frames_Include_Data_Num	
#undef Sensors_CH_Null
}

#elif (AppGPRSComProtocol == 0x01)//HttpЭ�鴫����ʪ������
static BOOL GPRS_Get_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* �����ݵĻ���ָ�� */
	Save_Sensor_Data_Struct            *sensordata_p;
	/* ��ʱʱ�仺�� */
	Times                                       time;
	/* ������ */
	unsigned char                                num;
	/* ̽ͷ��� */
	unsigned char                           probeNum;
	/* ���ر��ָ�� */
	SERIAL_NUMBER* gatewayIdNoP         = &config_tab->Inf_State.Instru_System.Inf.Serial_Number;
	/* ������ͨ��ָ�� */
	INSTRU_SENSOR_CHANNEL* sensorchP    = &config_tab->Inf_State.Instru_Sensor.Inf_State.CH1;


	/* ������ݽӿڲ��� */
	memset((char*)&HTTPPara.ApiPara,0x00,sizeof(HTTPPara.ApiPara));
	
	
	/* �ж� �Ƿ�����ʷ���� */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM < Save_Sensor_Data_Len)
	{
		/* û����ʷ���� */
		return FALSE;
	}
	
	/* �������ر�� */
	sprintf(HTTPPara.ApiPara.GatewayIdNo,"%04X%05d",
	((((*(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0x00ff) << 8)),
	((((*(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0x00ff) << 8)));
	
	
	/* ��ȡ��ǰ�Ĵ�����ͨ������ */
	/* ���ݵ�ǰ�ⲿ�洢����W25Q64�������ݴ洢ָ������������ */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)
	{
		/* Flash����ʷ���ݣ��Ͷ�ȡ���ݣ��ȷ���ʷ���� */
		sensordata_p = (Save_Sensor_Data_Struct*)Flash_Read_Sensor_Data(
						fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size,
						Save_Sensor_Data_Len);

		//���� Flash ��ָ��
		/* ���� ��ȡ�����ݳ��� */
		fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer 		+= Save_Sensor_Data_Len; 
		/* �洢������ ��*/
		fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	-= Save_Sensor_Data_Len;
		
	}
	
	/* �����ϴ�ʱ��� */
	time.Year 	= config_tab->Inf_State.Instru_RTC.Inf_State.Year;
	time.Month	= config_tab->Inf_State.Instru_RTC.Inf_State.Month;
	time.Day 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Day;
	time.Hour 	= config_tab->Inf_State.Instru_RTC.Inf_State.Hour;
	time.Min 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Min;
	time.Sec 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Sec;
	/* �������Ĳɼ�ʱ��ת�����룬�������������Ϊ����ǩ����ʱ�����ݡ� */
	HTTPPara.ApiPara.TimeStamp = RTC_Date_Time_To_Second(&time);
	
	/* �������ǩ�����ݻ��档 */
	memset(HTTPPara.ApiPara.Sign,0x00,sizeof(HTTPPara.ApiPara.Sign));
	
	/* ����GPS�豸״̬ */
	if((GPS_Config_Enable == 0x01)&&
	   ((Sensor_Lon_Num + Sensor_Lat_Num + Sensor_Spd_Num) > 0))
	{
		/* ʹ��GPS�豸 */
		/* δʹ��GPS�豸 */
		HTTPPara.ApiPara.GPS_State = TRUE;
		/* ��ѯ��γ������ */
		for(num = 0;num < 32 ;num++)
		{
			/* �ж�ͨ������ */
			if(Sensor_GPS_POS_Lon == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
			{
				/* ���þ������� */
				HTTPPara.ApiPara.Lng = sensordata_p->GPRS_IR.Sensor_Data[num];
			}else if(Sensor_GPS_POS_Lat == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
			{
				/* ����γ������ */
				HTTPPara.ApiPara.Lat = sensordata_p->GPRS_IR.Sensor_Data[num];
			}
		}
	}else
	{
		/* δʹ��GPS�豸 */
		HTTPPara.ApiPara.GPS_State = FALSE;
		/* ����������� *///�ݶ�0���������ó����ߺ����ݡ�
		HTTPPara.ApiPara.Lng = 0;
		/* ���γ������ *///�ݶ�0���������ó����ߺ����ݡ�
		HTTPPara.ApiPara.Lat = 0;
	}
	
	/* ���òɼ�ʱ�� */
	time.Year 	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS + 2000;
	time.Month	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS;
	time.Day 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS;
	time.Hour 	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS;
	time.Min 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS;
	time.Sec 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS;
	
	/* ���òɼ�ʱ�� */
	sprintf(HTTPPara.ApiPara.CollectTime,"%04d-%02d-%02d %02d:%02d:%02d",
			time.Year,time.Month,time.Day,time.Hour,time.Min,time.Sec);
	
	/* ��ȡ��ǰ̽ͷ���� */
	HTTPPara.ApiPara.ProbeNum = Sensor_Tmp_Num + Sensor_Hum_Num +
								Sensor_RF_Single_Tmp_Num + 
								(Sensor_RF_Humiture_Tmp_Num +
								Sensor_RF_Humiture_Hum_Num)/2 +
								Sensor_PT1000_Num +
								Sensor_GW_NJSY_Num;
	
	
	/* ��˳��������̽ͷ���� */
	for(num = 0,probeNum = 0;(probeNum < HTTPPara.ApiPara.ProbeNum)&&(num < 32);num++)
	{
		if(Sensor_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ����̽ͷ��� */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* ����̽ͷ���� */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ���õ������� */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_Hum == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ����̽ͷ��� */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* ����̽ͷ���� */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ���õ������� */
			HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_RF_Card_Single_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* �жϿ�ƬSN�� */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* ��ƬSN��δ���ù��������͵�ǰͨ������. */
				/* ����һ��ͨ��. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ���� */
				continue;
			}
			
			/* ����̽ͷ��� */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* ����̽ͷ���� */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ���õ������� */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_RF_Card_Humiture_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ʪ�ȴ�������� */
			unsigned char humiNO;
			
			/* �жϿ�ƬSN�� */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* ��ƬSN��δ���ù��������͵�ǰͨ������. */
				/* ����һ��ͨ��. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ���� */
				continue;
			}
			
			/* ����̽ͷ��� */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* ����̽ͷ���� */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* �����¶����� */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
			/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
			humiNO = GPRS_GET_RF_Humiture_Hum_Data(((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
			/* �ж�������� */
			if(humiNO != 0xFF)
			{
				/* �����¶����� */
				HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[humiNO];		
			}else
			{
				/* �����¶����� */
				HTTPPara.ApiPara.Logs[probeNum].Humi = Wireless_RF_Card_Data_Rece_Timeout_Para;		
			}
			
			/* �ж��Ƿ���յ����� */
			if((Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp))
			{
				/* ��ƬSN��δ���ù��������͵�ǰͨ������. */
				/* ����һ��ͨ��. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ���� */
				continue;
			}
		}else if(Sensor_PT1000 == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ����̽ͷ��� */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* ����̽ͷ���� */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ���õ������� */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_GW_NJSY_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ʪ�ȴ�������� */
			unsigned char humiNO;
			
			/* �жϿ�ƬSN�� */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* ��ƬSN��δ���ù��������͵�ǰͨ������. */
				/* ����һ��ͨ��. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ���� */
				continue;
			}
			
			/* ����̽ͷ��� */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* ����̽ͷ���� */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* �����¶����� */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
			/* ���Ҷ�Ӧ����ʪ�ȿ�Ƭ��ʪ��ͨ�� */
			humiNO = GPRS_GET_RF_Humiture_Hum_Data(((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
			/* �ж�������� */
			if(humiNO != 0xFF)
			{
				/* �����¶����� */
				HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[humiNO];		
			}else
			{
				/* �����¶����� */
				HTTPPara.ApiPara.Logs[probeNum].Humi = Gateway_NJSY_TimeoutPara;		
			}
			
			/* �ж��Ƿ���յ����� */
			if((Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp))
			{
				/* ��ƬSN��δ���ù��������͵�ǰͨ������. */
				/* ����һ��ͨ��. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ���� */
				continue;
			}
		}else
		{
			continue;
		}
		
		/* ����ͨ����� */
		probeNum++;
		
	}
	/* ����HTTP�������ݡ� */
HTTP_Create_Data((char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
	/* û����ʷ���� */
	return TRUE;
}

#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x02)||(AppGPRSComProtocol == 0x04))
/*
*********************************************************************************************************
*	�� �� ��		: GPRS_Data_To_Network
*	����˵��		: ������������װ�����������ݣ��ȴ����͡� 
*	��    ��		: ��
*	�� �� ֵ		: ��
*********************************************************************************************************
*/
void GPRS_Data_To_Network(void)
{
	
/* �������ݼĴ��� ��Ϣ������֮ǰ�� �̶����ݳ��� *///��֮ǰ�Ĳ�ͬ
#define GPRS_Send_Data_Before_Fix_Len	((u32)(&GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)-\
										 (u32)(&GPRS_Send_Reg.GPRS_IR_FIX.ID_Type_HSB))
/* У��������λ�� */
#define GPRS_Send_Data_Check_Byte		(1u)
	
	/*  GPRS�������ݳ��� */
	u16 gprs_send_sensor_data_len = 0;	
	

	//1��ת�� ����
	/* ת�� *//* ���GPRS�������ݳ��� */
	gprs_send_sensor_data_len = GPRS_Host_To_Network_Translator_Data(&GPRS_Send_Sensor_Data_Buffer[1],
																	 &GPRS_Send_Reg.GSM_GPRS_Send_Buffer[1],
																	 (GPRS_Send_Data_Before_Fix_Len+
																((u16)GPRS_Send_Reg.GPRS_IR_FIX.Attribute_HSB_Length*256)+
																((u16)GPRS_Send_Reg.GPRS_IR_FIX.Attribute_LSB_Length)+GPRS_Send_Data_Check_Byte));
	
	//2������ ͷ��β ��ʶ
	/* ��ͷ��־  */
	GPRS_Send_Sensor_Data_Buffer[0] = GPRS_COM_Identify;
	/* GPRS�������ݳ���  */
	gprs_send_sensor_data_len++;
	/* ��β��־ */
	GPRS_Send_Sensor_Data_Buffer[gprs_send_sensor_data_len] = GPRS_COM_Identify;
	/* GPRS�������ݳ���  */
	gprs_send_sensor_data_len++;
	/* ���� ��������� ���� *///ͷ��־(1 Byte)β��־(1 Byte)
	GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = gprs_send_sensor_data_len;
	
	
#undef GPRS_Send_Data_Before_Fix_Len
#undef GPRS_Send_Data_Check_Byte
}

/*
*********************************************************************************************************
*	�� �� ��: GPRS_Host_To_Network_Translator_Data
*	����˵��: ��������ת��������������
*	��    �Σ�network_endian   host_endian	T_num
*	�� �� ֵ: ת��������ݻ��泤��
*********************************************************************************************************
*/
u16 GPRS_Host_To_Network_Translator_Data(u8* network_endian,u8* host_endian,u16 T_num)
{
	u16 num;
	u16 b_sp;
	
	/* ����ָ����0 */
	b_sp = 0;

	for(num = 0;num < T_num; num++)
	{
		if(host_endian[num] == GSM_Translator_Symbol_0X7E)
		{
			/* ��ǰ���� ��ת�� ����0X7E */
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X7D;
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X02;
		}else if(host_endian[num] == GSM_Translator_Symbol_0X7D)
		{
			/* ��ǰ���� ��ת�� ����0X7D */
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X7D;
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X01;
		}else
		{
			/* ��ǰ���� ��ת�� ���� */
			*(network_endian+b_sp++) = *(host_endian+num);
		}
	}
	return b_sp;
}

/************************************************************************************************/
/* ������	��GPRS_Network_To_Host_Translator_Data												*/
/* ����		��network_endian   host_endian	T_num												*/
/* ���		��ת��������ݻ��泤��																	*/
/* ����		����������ת��������������			 									 		 		*/
/* ��������	��2015/04/01																		*/
/************************************************************************************************/
u16 GPRS_Network_To_Host_Translator_Data(u8* host_endian,u8* network_endian,u16 T_num)
{
	u16 num;
	u16 b_sp;
	
	/* ����ָ����0 */
	b_sp = 0;
	/* ��ȡ���� */
	for(num = 0;num < T_num; num++)
	{
		/* �ж� ��ǰ������ �Ƿ�Ϊת�����. */
		if(network_endian[num] == GSM_Translator_Symbol_0X7D)
		{
			/* ��ת����� */
			/* �ж� ��2�������е��Ǹ�? */
			if(network_endian[num+1] == GSM_Translator_Symbol_0X02)
			{
				/* ˵�� �ñ���Դ����0X7E */
				host_endian[b_sp++] = GSM_Translator_Symbol_0X7E;
				/* ����ָ�� ָ�� ���¸����� */
				num++;
			}else if(network_endian[num+1] == GSM_Translator_Symbol_0X01)
			{
				/* ˵�� �ñ���Դ����0X7D */
				host_endian[b_sp++] = GSM_Translator_Symbol_0X7D;
				/* ����ָ�� ָ�� ���¸����� */
				num++;
			}else
			{
				/* λ�����͵ı��룬��������Ǵ����ת�Ƶ��µġ� */
				b_sp = 0;
				return b_sp;
			}
		}else
		{
			/* �������ݣ�ֱ�ӻ�ȡ */
			host_endian[b_sp++] = network_endian[num];
		}
	}
	return b_sp;
}
#endif
#endif
/*
*********************************************************************************************************
*	�� �� ��: GSM_Send_Q
*	����˵��: ���� GSM ��Ϣ����
*	��    �Σ�pevent:��Ϣ���е�ָ�룬GSM_CMD_TYPE:��Ϣ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
BOOL GSM_Send_Q(OS_EVENT* pevent,GSM_CMD_TYPE EVENT_Type)
{
	INT8U     	err;
	OS_Q_DATA	Q_data;
	GSM_QUE*	EVENT_p;	
	u16 		num;
	u32 		Q_p;

	
	/* ���͵� �¼� ����  */
	if(EVENT_Type == Init_GSM_Module_EVENT)
	{
		/* ����GSMģ�� */
		EVENT_p = &GSM_QUE_Init_Module;
	}else if(EVENT_Type == IAP_EVENT)
	{
		/* ���� ������� ���� */	
		EVENT_p = &GSM_QUE_Download_IAP_Data;	
	}else if(EVENT_Type == Update_SGL_EVENT)
	{
		/* �����ź�ǿ�� */
		EVENT_p = &GSM_QUE_Update_Signal;	
	}else if(EVENT_Type == Update_Link_EVENT)
	{
		/* ���¸���״̬ */
		EVENT_p = &GSM_QUE_Update_Link;	
	}else if(EVENT_Type == Updata_Data_EVENT)
	{
		/* �ϴ���ʪ������ */
		EVENT_p = &GSM_QUE_Update_Data;	
	}else if(EVENT_Type == Send_SMS_EVENT)
	{
		/* ���� ���� */		
		EVENT_p = &GSM_QUE_Send_SMS;	
	}else if(EVENT_Type == ConfigService_EVENT)
	{
		/* �ϴ� ���� ���� */		
		EVENT_p = &GSM_QUE_ConfigService;		
	}
//	else if(EVENT_Type == Updata_Config_Back_EVENT)
//	{
//		/* �ϴ� ���� ���� */		
//		EVENT_p = &GSM_QUE_Send_Config_Back;		
//	}
	else if(EVENT_Type == System_Clock_Calibration_EVENT)
	{
		/* �ϴ� ���� ���� */		
		EVENT_p = &GSM_QUE_Send_System_Clock_Calibration;		
	}
#if (GSM_Module_Type==0x02) //ESP8266Wifiģ��	
	else if(EVENT_Type == SetWifiNamePWD_EVENT)
	{
		/* ����Wifi���ֺ����� */		
		EVENT_p = &Wifi_QUE_SetWifiNamePWD;		
	}
	else if(EVENT_Type == SetWifiWLAN_EVENT)
	{
		/* ����WifiWlan���� */		
		EVENT_p = &Wifi_QUE_SetWifiWLAN;		
	}
	else if(EVENT_Type == SetWifiWLAN_GET_IP_WAY_EVENT)
	{
		/* ����Wifiģ��IP��ȡ��ʽ���� */		
		EVENT_p = &Wifi_QUE_GET_IP_WAY;		
	}
	else if(EVENT_Type == RestWifi_EVENT)
	{
		/*��λWifiģ��*/		
		EVENT_p = &Wifi_QUE_Reset;		
	}	
	else if(EVENT_Type == Factory_Data_Reset_Wifi_EVENT)
	{
		/* Wifiģ��ָ��������� */		
		EVENT_p = &Wifi_QUE_Factory_Data_Reset;		
	}		

#endif	
	
	/* ���򱨵����� ˵�������в����� �ϴ�������Ϣ */
	/* �ж� �ϴ����� ִ�н�� */
	if(EVENT_p->EVENT_Getarg == FALSE)
	{
		/* �ϴ�������Ϣ ���� ��ִ�У��ȴ�ִ�н��� */
		return FALSE;
	}
	
	//���� �ڴ�ռ����� �������Ϣֻ��һ�����ݿռ䣬��ˣ��ڷ�����Ϣǰ��Ҫȷ���ϴε���Ϣ�Ѿ���ִ���ˡ�
	//����ͬһ����Ϣ��ռ��2����Ϣ�ռ䣬����������Ϣ�޷����롣
	/* �����Ϣ�������Ƿ���� ���� ����״̬ ��Ϣ�� */
	err = OSQQuery(pevent,&Q_data);
	
	/* �жϲ�ѯ��� */
	if(err == OS_ERR_NONE)
	{
		/* ��ѯ�ɹ� */
		
		/* ���� �������Ƿ��Ѿ����� ����״̬ ��Ϣ�� */
		for(num = 0,Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQOut);
			num < Q_data.OSNMsgs;
			num ++)
		{
			/* �жϵ�ǰ����Ϣָ���뷢��ָ���Ƿ���ͬ */
			if((*(u32*)Q_p) == (u32)EVENT_p)
			{
				/* ��ȣ������д��� ����״̬ ��Ϣ,��ô�Ͳ��ڷ��� ����״̬ ��Ϣ */
				return FALSE;
			}
			/* ����ָ�뵽��һ��λ�� */
			Q_p += sizeof(GSM_QUE);
			/* �ж�ָ���Ƿ�Խ�� */
			if(Q_p == (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQEnd))
			{
				/* Խ��ͽ� ָ�� ָ��ͷλ�� */
				Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQStart);
			}
		}
	}else
	{
		/* ��ѯʧ�� */
		return FALSE;
	}
	
	/* ���������˵�� ������û�д��� �ź�ǿ�� ��Ϣ��
	   ��ˣ���Ҫ����һ�� �ź�ǿ�ȸ��� ��Ϣ�� */
	/* ��� ��Ϣ ִ�н�� */
	EVENT_p->EVENT_Getarg = FALSE;
	/* ���� ��Ϣ *///FIFO	
	err = OSQPost(pevent, EVENT_p);  
	/* �ж� ��Ϣ ���� ״̬ */
	if(err == OS_ERR_Q_FULL)
	{
		/* ��Ϣ��������,�����ٽ�������Ϣ */
		/* ������� ������ ���� */
		return FALSE;
	}
	/* �ж� ��Ϣ ���� �ɹ� */
	return TRUE;
}

/************************************************************************************************/
/* ������	��GPRS_Save_Sensor_Data_Check														*/
/* ����		����																				*/
/* ���		���� 																				*/
/* ����		����� �Ƿ����µ� �洢�� ��ʪ�� ���� 									 		 		*/
/* ��������	��2015/04/01																		*/
/************************************************************************************************/
void GPRS_Save_Sensor_Data_Check(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	
	/* �жϵ�ǰģ��Ĺ���״̬�Ƿ����� */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		/* ģ�鹤������ */
		
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x02)
	
		/* �ж� �Ƿ��д����������ڵȴ����� */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)
		{
			/* �������� */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
			ComProtocolZDE_CreateData(config_tab,fix_tab,
			GPRS_Send_Sensor_Data_Buffer,
			&GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len);
			/* �������� */
//			memcpy((char*)GPRS_Send_Sensor_Data_Buffer,
//				   ComProtocolZDEApp.DR.TXDR,
//					TXLen);
			
			/* �ж� */
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
			   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE) &&
			 (((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == TRUE) &&
			   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE))||
			   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0)))
			{
					/* �������� */
				GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;		
			}
		}
		
#else		
#if (AppGPRSComProtocol == 0x04)//����ʡ���߹滮��������Э��
/* �ж� �Ƿ��д����������ڵȴ����� */
if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)&&(RTC_System_Clock_Calibration_State_Reg == TRUE)&&(ConfigService_App.SYNC_State == TRUE))
{
	/* �������� */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
	ComProtocol_JS_CreateData(config_tab,fix_tab,
	GPRS_Send_Sensor_Data_Buffer,
	&GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len);
}
 #endif
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))//����Э�����HTTPЭ��
		//����Э��
		/* �ж� �Ƿ��д����������ڵȴ����� */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)
		{
			/* û�� �����������ڵȴ�ת�� */
			/* ��� �Ƿ����µ� �洢�� ��ʪ�� ���� */
 			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
			GPRS_Get_Save_Sensor_Data(config_tab,fix_tab);
		}
#endif			
		/* �ж� GPRS ���ʹ���������ͨ��״̬ */
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE :˵����ǰû�������ϴ�
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE :˵�������ݵȴ��ϴ�
		//GPRS_COM_Send_Result == TRUE && GPRS_COM_Rece_Result == TRUE :˵��֮ǰ�������ϴ��ɹ������Կ�ʼ�ϴ���һ������
		//GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0 : ˵����û�����ݷ��ͻ��߷��͵����ݱ���������Կ�ʼ�ϴ���һ�����ݡ�
		if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
		   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE) &&
		 (((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == TRUE) &&
		   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE))||
		   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0))
		)
		{
			/* û�� ���������� ���ڷ��� */
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
			/* ������������װ�����������ݣ��ȴ�����. */
			GPRS_Data_To_Network();
#elif (AppGPRSComProtocol == 0x01)
			/* ���㷢�����ݳ��� */
			GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = strlen((char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
			/* ������ݻ��� */
			memset(GPRS_Send_Sensor_Data_Buffer,0x00,GPRS_Send_Sensor_Data_Buffer_Size);
			/* �������� */
			strcpy((char*)GPRS_Send_Sensor_Data_Buffer,
				   (char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
#endif
#endif
			/* ��� GPRS �����򴫸������ݵȴ�����״̬ */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = FALSE;
			/* ���� GPRS �����򴫸������ݵȴ�����״̬ */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;
		}
#endif
#endif
	}
}

/************************************************************************************************/
/* ������	��GPRSSendData_ParseStateCheck														*/
/* ����		����																					*/
/* ���		���� 																				*/
/* ����		��GPRS���ս�������״̬���													 		 */
/* ��������	��2016/09/09																		*/
/************************************************************************************************/
void GPRSSendData_ParseStateCheck(void)
{
	/* ������ */
	static uint32_t num = 0;
	
	/* �ж� GPRS���ս�������״̬��� */
	if(FALSE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState)
	{
		/* ����ʧ�� */
		/* ��1����ʱ */
		num++;
		/* �ж���ʱʱ�� */
		if(num >= AppGPRSUsageProtectTime)
		{
			/* ��ʱʱ�䵽�����״̬��־ */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState = TRUE;
			/* ������� */
			num = 0;
		}
	}
}

/************************************************************************************************/
/* ������	��GPRS_Send_Sensor_Data																*/
/* ����		����																					*/
/* ���		���� 																				*/
/* ����		�������µĴ���������													 		 		*/
/* ��������	��2015/04/01																		*/
/************************************************************************************************/
void GPRS_Send_Sensor_Data(void)
{

	/* �ж� GPRS ���ʹ���������ͨ��״̬ */
	/* �ж� ���������� ���ڷ��� && �����������ݵȴ����� */
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE :˵����ǰû�������ϴ������Կ�ʼ�ϴ�����
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE :˵�������ݵȴ��ϴ�
		//GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0 : ˵�������ݷ��ͣ����Կ�ʼ�ϴ����ݡ
	 //  IAP_App.RunState == FALSE;   IAP_App.RunState=FALSE��ʾû�����ڽ���Զ������;
// printf("1 ������������GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,
//	  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
	
	
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x00)
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State == TRUE) &&
	   (TRUE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0)&&
		  IAP_App.RunState == FALSE)
	{
		/* ���� �ϴ�������Ϣ */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Updata_Data_EVENT);
		
//	printf("2 ������������GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n   GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
		
	}
	
		#endif
		#endif
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x01)
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State == TRUE) &&
	   (TRUE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0))
	{
		/* ���� �ϴ�������Ϣ */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Updata_Data_EVENT);
		
//	printf("2 ������������GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n   GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
		
	}
	
		#endif
		#endif

}


/************************************************************************************************/
/* ������	��GPRS_Parsing_Network_Data															*/
/* ����		����																					*/
/* ���		���� 																				*/
/* ����		�����ݷ��������ص�������Ϣ���жϴ����������ϴ�״̬��������ز�����			 		 		*/
/* ��������	��2015/04/01																		*/
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))//��������Э�鷵����������
static BOOL GPRS_Parsing_Network_Data(void)
{
/* ���յ����������ݳ��� */
#define GPRS_Rece_Network_Data_Len		(20u)
/* ͨ��Ӧ��Э��Ľ�� */
/* �����ϴ��ɹ� */
#define GPRS_Rece_ACK_Result_Success	(0X00)
/* �����ϴ�ʧ�� */
#define GPRS_Rece_ACK_Result_Failure	(0X01)
/* �����ϴ����� */
#define GPRS_Rece_ACK_Result_Error		(0X02)
/* ���ݲ�֧�� */	
#define GPRS_Rece_ACK_ON_Format			(0X03)
/* ��������ȷ�� */	
#define GPRS_Rece_ACK_Alarm_Confirm		(0X04)	
	
	/* У���� */
	u8 check_code;
	/* ������ */
	u8 num;
	/* ���������ص����ݳ��� */
	u8 gprs_rece_data_len;
	
	//1������ת������
	/* �ȶ� �������� ��������ת�� */
	gprs_rece_data_len = GPRS_Network_To_Host_Translator_Data(GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,
															  GPRS_Rece_Sensor_Data_Buffer,
															  GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len);
	
	//2����������
	/* �ж����ݳ��� *//* ����Ӧ����Ϣ�����ǹ̶���(20�ֽ�) */
	if(gprs_rece_data_len != GPRS_Rece_Network_Data_Len)
	{
		/* ���յ������ݳ������趨�Ĳ�һ�� */
		return FALSE;
	}
	
	/* �жϱ�ʶλ���� */
	if((GPRS_Rece_Reg.GPRS_IR_FIX.Start_Iden != GPRS_COM_Identify)||
	   (GPRS_Rece_Reg.GPRS_IR_FIX.End_Iden   != GPRS_COM_Identify))
	{
		/* ͷ��ʶ��β��ʶ��Э�鲻һ�� */
		return FALSE;
	}
	
	/* �ж���Ϣͷ ��ϢID���� */
	if((GPRS_Rece_Reg.GPRS_IR_FIX.ACK_ID_Type_HSE != (GPRS_COM_ID_Upload/0X100))||	
	   (GPRS_Rece_Reg.GPRS_IR_FIX.ACK_ID_Type_LSE != (GPRS_COM_ID_Upload%0X100)))
	{
		/* ��Ϣͷ��ϢID��Э�鲻һ�� */		
		return FALSE;
	}
	
	/* �ж���Ϣ�峤�� */	
	if((GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Length != (GPRS_COM_ACK_Att_Len/0X100))||	
	   (GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_LSB_Length != (GPRS_COM_ACK_Att_Len%0X100)))
	{
		/* ��Ϣ�峤����Э�鲻һ�� */
		return FALSE;
	}
	
	/* �ж���Ϣ���ܷ�ʽ */	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Enc_Mode != GPRS_COM_Enc_Mode)
	{
		/* ��Ϣ���ܷ�ʽ�����ò�һ�� */
		return FALSE;
	}
	
	/* �ж���Ϣ�ְ���־ */	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Sub_Flag != GPRS_COM_Sub_Flag)
	{
		/* ��Ϣ�ְ���־�����ò�һ�� */
		return FALSE;
	}	
	
//	/* �ж���Ϣ�ն����к� */	
//	if((GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||	
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != ))	
//	{
//		/* ��Ϣ�ն����к�������SN�Ų�һ�� */
//		return FALSE;	
//	}
	
	//����� ��Ӧ����ˮ�š� �� ��Ӧ�� ID��û��ʲôʵ���Ե��������ԾͲ������ж�
	
	/* �ж���ϢӦ���� */
	if(GPRS_Rece_Reg.GPRS_IR_FIX.ACK_Result != GPRS_Rece_ACK_Result_Success)
	{
		/* ��ϢӦ���쳣 */
		//�������ò鿴�쳣����
		//�������һ��nop����λ�������÷������鿴�����������Ҫ��ӡ����ʾ�����͵�����Ӧ�ĺ������ܡ�
		__nop();
		return FALSE;
	}
	
	/* ��� У�� ���ݼĴ��� */
	check_code = 0;
	/* �ж�����У�� */
	/* ���� ����ת�������������ݵ�У��  */
	for(num = 1;num < (GPRS_Rece_Network_Data_Len-2); num++)
	{
		/* �����У�鷽ʽ:����Ч�����ݶ�ȡ��� */
		check_code ^= GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer[num];
	}
	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Check_Code != check_code)
	{
		/* ��Ϣ����У���쳣 */
		return FALSE;
	}
	
	/* �������������������ݷ��ͳɹ��� */
	return TRUE;
	
#undef GPRS_Rece_Network_Data_Len	
#undef GPRS_Rece_ACK_Result_Success
#undef GPRS_Rece_ACK_Result_Failure
#undef GPRS_Rece_ACK_Result_Error
#undef GPRS_Rece_ACK_ON_Format
#undef GPRS_Rece_ACK_Alarm_Confirm
}
#elif (AppGPRSComProtocol == 0x01)//����HTTPЭ�鷵����������
static BOOL GPRS_Parsing_Network_Data(void)
{
	/* ����ָ�� */
	char * datap;
		
	/* ������ݻ��� */
	memset(GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,0x00,GPRS_Rece_Buffer_Size);
	/* ������������ */
	strcpy((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,(char*)GPRS_Rece_Sensor_Data_Buffer);
	
	
	/* ��ѯHTTP����ɹ��ַ��� */
	datap = strstr((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,"HTTP/1.1 200 OK");
	/* �ж�HTTP�����Ƿ�ɹ� */
	if(NULL == datap)
	{
		/* ����ʧ�� */
		return FALSE;
	}
	
	/* ��ѯ����ƽ̨�ɹ������ַ��� */
	datap = strstr((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,"\"code\":\"0000\"");
	/* �ж�����ƽ̨�Ƿ�ɹ��������� */
	if(NULL == datap)
	{
		/* ����ʧ�� */
		return FALSE;
	}

	return TRUE;
}
#elif (AppGPRSComProtocol == 0x02)//����������Э�鷵����������
static BOOL GPRS_Parsing_Network_Data(void)
{
	
	/* û����ʷ���� */
	return ComProtZDE_ParseData(GPRS_Rece_Sensor_Data_Buffer,
		   &GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len);
}
#elif (AppGPRSComProtocol == 0x04)//��������ʡ���߹滮��������Э�鷵����������
static const u8 JS_DL_CMD_Buffer[6]={0x55,0x7A,0x04 ,0x85 ,0x00 ,0x00 };

static BOOL GPRS_Parsing_Network_Data(void)
{
	 if( GPRS_Rece_Sensor_Data_Buffer[0]!=JS_DL_CMD_Buffer[0]  || \
		   GPRS_Rece_Sensor_Data_Buffer[1]!=JS_DL_CMD_Buffer[1]  || \
	     GPRS_Rece_Sensor_Data_Buffer[2]!=JS_DL_CMD_Buffer[2]  || \
		   GPRS_Rece_Sensor_Data_Buffer[3]!=JS_DL_CMD_Buffer[3]  || \
	     GPRS_Rece_Sensor_Data_Buffer[4]!=JS_DL_CMD_Buffer[4]  || \
		   GPRS_Rece_Sensor_Data_Buffer[5]!=JS_DL_CMD_Buffer[5] )
{
	
			 Log_printf("�����������豸�ϴ�������ʧ�ܣ�����\n");

        return FALSE;//��������������ʧ��

}
	 return TRUE;
}
#else
static BOOL GPRS_Parsing_Network_Data(void)
{
	__NOP();
	return TRUE;
}
#endif
#endif

/*
*********************************************************************************************************
*	�� �� ��: GSM_Signal_Strength
*	����˵��: �������ڵ��ź�ǿ�ȣ�������ʾ��
*	��    �Σ�signal_num :��ǰ �� GSM ģ����ź�ǿ����Ϣ��
*			 max_signal :���������ź�ǿ�ȡ�
*			 sub		:�ź�ϸ�֡�
*	�� �� ֵ: �ڲ˵���������ʾ���źš�
*********************************************************************************************************
*/
u8 GSM_Signal_Strength(int* signal_num)
{
/* ��ʾ�ź�ǿ�ȵ��������� */
//0-5:0��
#define GPRS_Signal_Strength_0		(5u)
//6-8:1��
#define GPRS_Signal_Strength_1		(8u)
//9-11:2��
#define GPRS_Signal_Strength_2		(11u)	
//12-14:3��
#define GPRS_Signal_Strength_3		(14u)		
//15-31:4��
#define GPRS_Signal_Strength_4		(15u)
/* GPRS ����ź�ǿ�� */	
#define GPRS_Max_Signal				(31u)
	
	
	
	if((*signal_num > GPRS_Max_Signal)||(*signal_num <= 0))
	{
		/* �ź� ���ʧ�� */
		/* ��ǰ�ź�ǿ�ȣ�0�� */
		return 0;
	}

	/* �жϵ�ǰ�ź�ǿ�� */
	if(*signal_num <= GPRS_Signal_Strength_0)
	{
		/* ��ǰ�ź�ǿ�ȣ�0�� */
		return 0;
	}else if((*signal_num > GPRS_Signal_Strength_0)&&(*signal_num <= GPRS_Signal_Strength_1))
	{
		/* ��ǰ�ź�ǿ�ȣ�1�� */
		return 1;
	}else if((*signal_num > GPRS_Signal_Strength_1)&&(*signal_num <= GPRS_Signal_Strength_2))
	{
		/* ��ǰ�ź�ǿ�ȣ�2�� */
		return 2;
	}else if((*signal_num > GPRS_Signal_Strength_2)&&(*signal_num <= GPRS_Signal_Strength_3))
	{
		/* ��ǰ�ź�ǿ�ȣ�3�� */
		return 3;
	}else if((*signal_num > GPRS_Signal_Strength_3)&&(*signal_num <= GPRS_Max_Signal))
	{
		/* ��ǰ�ź�ǿ�ȣ�4�� */
		return 4;
	}else
	{
		/* ��ǰ�ź�ǿ�ȣ�0�� */
		return 0;
	}
	
#undef GPRS_Signal_Strength_0
#undef GPRS_Signal_Strength_1
#undef GPRS_Signal_Strength_2
#undef GPRS_Signal_Strength_3	
#undef GPRS_Signal_Strength_4
#undef GPRS_Max_Signal
}

/************************************************************************************************/
/* ������	: GSMGetSignalStrength										 	 					*/
/* ����  	: ��			 																	*/
/* ���		: GSM�ź�ǿ��																		*/
/* ����	 	: ��ȡ��ǰGSM�ź�ǿ��					 											*/
/* ��������	: 2017/02/23																		*/
/* ����汾	: V0																				*/
/************************************************************************************************/
uint8_t GSMGetSignalStrength(void)
{
	return GPRS_SR_Reg.GPRS_Sign_Stre;
}

/*
*********************************************************************************************************
*	�� �� ��: GSM_Signal_Check
*	����˵��: ��⵱ǰ GSM�ź�:����ź���������ʾ�ź�ǿ�ȣ��������� �������� GSM ģ��(��ֹģ������쳣�Ĺ���״̬)
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GSM_Signal_Check(void)
{
/* GSM �ź��쳣 */
#define GPRS_Signal_Error		                          	(-1)
/* GPRS ����ź�ǿ�� */	                              
#define GPRS_Max_Signal			                          	(31u)
	                                                    
	/* �ж� GSMģ������״̬ */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		/* ģ�����óɹ� */
		if((GPRS_SR_Reg.GPRS_Sign_Stre >= 0)&&(GPRS_SR_Reg.GPRS_Sign_Stre <= GPRS_Max_Signal))
		{
			/* GSM�������� */
			/* �ź�ǿ�� ��ʾ���� */
			GPRS_SR_Reg.GPRS_Sign_Stre_Disp = GSM_Signal_Strength(&GPRS_SR_Reg.GPRS_Sign_Stre);
		}else if(GPRS_SR_Reg.GPRS_Sign_Stre == GPRS_Signal_Error)//(-1��
		{
//			/* GSM���������� */
			
			/* ���� ��ʾ�� �ź�ǿ��:0 */
//			GSM_Tab.Signal_Display = 0;
//			/* �޸� GSMģ��״̬:ʧ�� */
//			GSM_Tab.GSM_State = FALSE;
//			/* ��Ҫ ���� ����GSMģ�� */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
//			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//			{
//				/* GSMģ���ź�ǿ���쳣 */
//				printf("\rGSMģ���ź�ǿ���쳣\r\n");
//			}
		}else
		{
			/* δ֪״̬ */
			/* ���� ��ʾ�� �ź�ǿ��:0 */
//			GSM_Tab.Signal_Display = 0;
			
		}
//		/* �жϵ�ǰ����״̬ */
//		if(GPRS_SR_Reg.GPRS_Link_State == FALSE)
//		{
//			/* ���� ʧ�� */
//			/* ���� ʧ�� ʱ�� */
//			if(GSM_GPRS_Link_Fail_Num >= 300)
//			{
//				/* ����ʱ�䳬�� ����ֵ ���� ����GSMģ�� */
//				GSM_GPRS_Link_Fail_Num = 0;
//				/* ���� ��ʾ�� �ź�ǿ��:0 */
//				GSM_Tab.Signal_Display = 0;
//				/* �޸� GSMģ��״̬:ʧ�� */
//				GSM_Tab.GSM_State = FALSE;
//				/* ��Ҫ ���� ����GSMģ�� */
//				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/* GSMģ�� ����״̬ �쳣 */
//					printf("\rGSMģ�鸽��״̬�쳣\r\n");
//				}				
//			}else
//			{
//				/* GSM��GPRS��������ʧ�� */
//				GSM_GPRS_Link_Fail_Num++;
//			}
//		}else
//		{
//			/* ���� �ɹ� */
//			if(GSM_GPRS_Link_Fail_Num)
//			{
//				/* ��� ���� ʧ�� ʱ�� */
//				GSM_GPRS_Link_Fail_Num = 0;
//			}
//		} 
	}else
	{
		/* ģ�� ��δ���á� �� ������ʧ�ܡ� */
		/* �����ź�ǿ�� Ϊ 0 */
		GPRS_SR_Reg.GPRS_Sign_Stre = 0;
		/* ���� ����״̬ Ϊ δ���� */
		GPRS_SR_Reg.GPRS_Link_State = FALSE;
		/* ��Ҫ ���� ����GSMģ�� */
		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
	}
	

#undef GPRS_Signal_Error
#undef GPRS_Max_Signal
}

/*
*********************************************************************************************************
*	�� �� ��: GSM_Link_Check
*	����˵��: GPRS ����״̬ ��� 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void GSM_Link_Check(void)
{

	/* �ж� GSMģ������״̬ */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
	
	
	}

}
#if (GSM_Module_Type==0x00) //SIM800Cģ��	
/************************************************************************************************/
/* ������	��GSM_Send_SMS																		*/
/* ����		�����ŷ��͵��ֻ�����																	*/
/* ���		���� 																				*/
/* ����		�����Ͷ���Ϣ						 									 		 		*/
/* ��������	��2015/04/10																		*/
/************************************************************************************************/
static void GSM_SMS_Send(INSTRU_GSM* gprs_inf)
{

/* �ж��û����� */
#if SMS_User_Number == 0x01	         //���ű����������
	/* �ж� ���ź��� �Ƿ�����Ҫ���ͱ����ĺ��� */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))//����Ϊ��ʱ�����Ͷ���
	{
		/* ������벻���Ͷ��� */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* ���� �û�1 �Ķ��� */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//���ͺ���1���ź���
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//���ͺ���1���ź���
		 if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("��%s�û����ͱ������ųɹ�������\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("��%s�û����ͱ�������ʧ�ܣ�����\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
		
	}
#elif SMS_User_Number == 0x02        //���ű����������
	/* �ж� ���ź��� �Ƿ�����Ҫ���ͱ����ĺ��� */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))
	{
		/* ������벻���Ͷ��� */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* ���� �û�1 �Ķ��� */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);		
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//���ͺ���1���ź���
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("��%s�û����ͱ������ųɹ�������\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("��%s�û����ͱ�������ʧ�ܣ�����\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}
	
	/* �ж� ���ź��� �Ƿ�����Ҫ���ͱ����ĺ��� */
	if((gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11 == 0))
	{
		/* ������벻���Ͷ��� */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = TRUE;
	}else
	{
		/* ���� �û�2 �Ķ��� */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11+'0';
////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//���ͺ���2���ź���
		 if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]){
		  Log_printf("��%s�û����ͱ������ųɹ�������\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("��%s�û����ͱ�������ʧ�ܣ�����\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}

#elif SMS_User_Number == 0x03      //���ű����������
	/* �ж� ���ź��� �Ƿ�����Ҫ���ͱ����ĺ��� */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))
	{
		/* ������벻���Ͷ��� */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* ���� �û�1 �Ķ��� */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//���Ͷ��ű������뺯��
	  GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//���ͺ���1���ź���	
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("��%s�û����ͱ������ųɹ�������\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("��%s�û����ͱ�������ʧ�ܣ�����\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
		
	}                                                                         //���ű�������        //���ű�������
	
	/* �ж� ���ź��� �Ƿ�����Ҫ���ͱ����ĺ��� */
	if((gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11 == 0))
	{
		/* ������벻���Ͷ��� */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = TRUE;
	}else
	{
		/* ���� �û�2 �Ķ��� */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11+'0';
////////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);//���Ͷ��ű������뺯��
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//���ͺ���2���ź���
			if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]){
		  Log_printf("��%s�û����ͱ������ųɹ�������\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("��%s�û����ͱ�������ʧ�ܣ�����\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}                                                                                     //���ű�������         //���ű�������
	
	/* �ж� ���ź��� �Ƿ�����Ҫ���ͱ����ĺ��� */
	if((gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_11 == 0))
	{
		/* ������벻���Ͷ��� */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = TRUE;
	}else
	{
		/* ���� �û�3 �Ķ��� */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_11+'0';
//////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);//���Ͷ��ű������뺯��
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//���ͺ���3���ź���
	                                                                                   //���ű�������              //���ű�������
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2]){
		    Log_printf("��%s�û����ͱ������ųɹ�������\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		   }else{
		    Log_printf("��%s�û����ͱ�������ʧ�ܣ�����\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		   }
	
	}                                                                                
	
	

#endif
	
	/* ���� ���ŷ������״̬ */
	GSM_SMS_SR_Reg.GSM_SMS_Send_State = FALSE;
}

#endif


/*
*********************************************************************************************************
*	�� �� ��: 
*	����˵��: �������ݻ��棬�ж����ñ��Ƿ����޸ģ����޸ĵĻ��������ñ���Ķ�Ӧ����
*	���ݲ�����*buf���������黺�棬�ǿ�(������Ϣ)
					  len�����黺�� ����
						* Config_Content�����ñ� ��Ϣ �ṹ��
						Packet_num�����黺���� ������������!
						* tab�����ݻ����޸ĵ����ݣ��޸��ڴ��� �������ñ�
*	�� �� ֵ: �� __nop(); 
*********************************************************************************************************
*/
void RECBUF_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE (*Config_Content)[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB)
{
	u32 data_Length=0;
	u16 j=0,i=0;
	
	//�����õ������� Ҫ������������������⣡
	for(i=0;i<Packet_num;i++)//������ �м�������(С������) ������
	{
		/* ���� ���� */
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type = buf[0+data_Length];
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder = buf[2+data_Length]+buf[1+data_Length]*256;
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len = buf[4+data_Length]+buf[3+data_Length]*256;
//		memcpy(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR_REG,&buf[0+data_Length],Config_Tab_Chane_Size); 
		data_Length=data_Length+Config_Tab_Chane_Size;//���� ��ַ ���ݳ��� 5�ֽ�	
		
		if(0x01==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//ϵͳ��Ϣ
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_System.Inf_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}
			data_Length=data_Length+j;
		}
		else if(0x02==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//��Դ״̬��Ϣ
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Power.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}	  
			data_Length=data_Length+j;			
		}
		else if(0x03==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//ϵͳʱ����Ϣ
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_RTC.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x04==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//GSMģ����Ϣ
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_GSM.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}			
			data_Length=data_Length+j;			
		}
		else if(0x05==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//��������Ϣ
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Sensor.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) = 
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x06==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//�˵�������Ϣ,�������ܿ�����Ϣ��ַ
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Menu.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x07==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//������У׼���ݱ���Ϣ
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Data_Tab.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		
	}
  
}

/************************************************************************************************/
/* ������	: GPRS_Disp_Sign_Stre															 	*/
/* ����		: �� 																				*/
/* ���		: ��ǰGPRS�ź�ǿ��(0-5:0��;6-8:1��;9-11:2��;12-14:3��;15-31:4��;)						*/
/* ����		: ���������еĸ���״̬��Ϣ						 									*/
/* ��������	: 2015/04/07																		*/
/************************************************************************************************/
GSM_GPRS_SR_Struct GPRS_Disp_Sign_Stre(void)
{
	return GPRS_SR_Reg;
}


#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
/************************************************************************************************/
/* GPRS����ʱ��У׼������Ϣͷ��ϢID */
#define GPRS_Send_ID_System_Clock_Calibration			(0x0FF0)
/* GPRS����ʱ��У׼������Ϣͷ��ϢID */
#define GPRS_Rece_ID_System_Clock_Calibration			(0x8FF0)
/* GPRS����ʱ��У׼������У�� */
#define GPRS_Send_ID_System_Clock_Calibration_Check		(0xFF)
/* GPRS����ʱ��У׼���ݼĴ�����С */
#define GPRS_Send_System_Clock_Calibration_Buffer_Size	(15u)

/************************************************************************************************/
/* GPRS����ʱ��У׼���ݼĴ��� */
static const u8 GPRS_Send_System_Clock_Calibration_Buffer[GPRS_Send_System_Clock_Calibration_Buffer_Size] = 
{
	/* ͷ ��ʶλ */ 				//(1 Byte)
	GPRS_COM_Identify,
	/* ͨ�� ��Ϣͷ */			//
	/* ��Ϣ ID(����) */			//(2 Byte)
	GPRS_Send_ID_System_Clock_Calibration/0X100,
	GPRS_Send_ID_System_Clock_Calibration%0X100,
	/* ��Ϣ�� ���� */			//(2 Byte)
	0x00,0x00,
	/* �ն����к�(SN��): */		//(6 Byte)
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* ��Ϣ ��ˮ�� */			//(2 Byte)
	GPRS_COM_Serial_Num,GPRS_COM_Serial_Num,
	/* ��ϢУ�� */				//(1 Byte)
	GPRS_Send_ID_System_Clock_Calibration_Check,
	/* β ��ʶλ */				//(1 Byte)
	GPRS_COM_Identify,
};
/************************************************************************************************/
/* GPRS����ʱ��У׼���ݻ��� */
static u8 GPRS_Rece_System_Clock_Calibration_Buffer[GPRS_Rece_System_Clock_Calibration_Buffer_Size];
/************************************************************************************************/
/* GPRS����ʱ��У׼���ݿ��ƼĴ��� */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_System_Clock_Calibration_CR_Reg = 
{
	/* GPRS ���� ������ �������� */
	GPRS_Rece_System_Clock_Calibration_Buffer_Size,
	/* GPRS ���� ������ �������� */
	GPRS_Send_System_Clock_Calibration_Buffer_Size,
	
	/* GPRS ���ʹ���������ͨ��״̬��TURE:���ڷ��ͣ�FALSE:û�з��ͣ� */
	FALSE,
	/* GPRS �����򴫸������ݵȴ�����״̬��TURE:�ȴ�����FALSE:û�����ݴ��� */
	FALSE,
	/* GPRS �����򴫸������ݵȴ�����״̬��TURE:�ȴ�����FALSE:û�����ݴ��� */
	FALSE,
	/* GPRS ���ʹ��������ݽ�� */
	TRUE,
	/* GPRS ���մ��������ݽ�� */
	TRUE,	
	/* GPRS ��������״̬   */	
	TRUE,
	/* GPRS ���ʹ��������� ʧ�ܴ��� */
	0,
};
/************************************************************************************************/
/* GPRS����ʱ��У׼���ݼĴ��� */
static GSM_GPRS_RECE_CLOCK_CALI_Struct GPRS_Rece_System_Clock_Calibration_DR;
#elif (AppGPRSComProtocol == 0x01)
/* GPRS����ʱ��У׼���ݿ��ƼĴ��� */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_System_Clock_Calibration_CR_Reg = 
{
	/* GPRS ���� ������ �������� */
	0,
	/* GPRS ���� ������ �������� */
	0,
	
	/* GPRS ���ʹ���������ͨ��״̬��TURE:���ڷ��ͣ�FALSE:û�з��ͣ� */
	FALSE,
	/* GPRS �����򴫸������ݵȴ�����״̬��TURE:�ȴ�����FALSE:û�����ݴ��� */
	FALSE,
	/* GPRS �����򴫸������ݵȴ�����״̬��TURE:�ȴ�����FALSE:û�����ݴ��� */
	FALSE,
	/* GPRS ���ʹ��������ݽ�� */
	TRUE,
	/* GPRS ���մ��������ݽ�� */
	TRUE,
	/* GPRS ��������״̬   */	
	TRUE,
	/* GPRS ���ʹ��������� ʧ�ܴ��� */
	0,
};
#endif
#endif
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x04))
/* ϵͳʱ��������ݼĴ��� */
static Times System_Clock_Updata_DR;
#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
/************************************************************************************************/
/* ������	: GPRS_Parsing_System_Clock_Calibration_Data										*/
/* ����		: parsing_data_buffer:���յ���������ʱ�����ݻ��档									*/
/* ���		: TRUR:�ɹ����շ�����ʱ�����ݣ�FALSE:	��������������ʱ�������쳣						*/
/* ����		: �������������ص�ʱ�����ݣ����жϱ���GPRSͨ��״̬��							 		*/
/* ��������	: 2015/06/26																		*/
/************************************************************************************************/
static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer)
{
/* ���յ����ݳ��� */
#define GPRS_Rece_Data_Len				(GPRS_Rece_System_Clock_Calibration_Buffer_Size)
/* ���յ���Ϣ�����ݳ��� */
#define GPRS_Rece_Attribute_Length_Len	(8u)
	
	/* У���� */
	u8 check_code;
	/* ������ */
	u8 num;
	/* ���������ص����ݳ��� */
	u8 gprs_rece_data_len;
	
#if	(System_Clock_Enable == 0x01)
	/* ϵͳ����ʱ�����Ĵ��� */
	u32 system_clock_updata_second_b;
#endif
	
	//1������ת������
	/* �ȶ� �������� ��������ת�� */
	gprs_rece_data_len = GPRS_Network_To_Host_Translator_Data(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_Buffer,
															  parsing_data_buffer,
															  GPRS_Rece_Data_Len);
	
	//2����������
	/* �ж����ݳ��� *//* ����Ӧ����Ϣ�����ǹ̶���(20�ֽ�) */
	if(gprs_rece_data_len != GPRS_Rece_Data_Len)
	{
		/* ���յ������ݳ������趨�Ĳ�һ�� */
		return FALSE;
	}
	
	/* �жϱ�ʶλ���� */
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Start_Iden != GPRS_COM_Identify)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.End_Iden   != GPRS_COM_Identify))
	{
		/* ͷ��ʶ��β��ʶ��Э�鲻һ�� */
		return FALSE;
	}
	
	/* �ж���ϢID */
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.ID_Type_HSB != (GPRS_Rece_ID_System_Clock_Calibration/0X100))||	
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.ID_Type_LSB != (GPRS_Rece_ID_System_Clock_Calibration%0X100)))
	{
		/* ���յ���Ϣͷ��Э�鲻һ�� */
		return FALSE;
	}
	
	/* �ж���Ϣ�峤�� */	
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Attribute_HSB_Length != (GPRS_Rece_Attribute_Length_Len/0X100))||	
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Attribute_LSB_Length != (GPRS_Rece_Attribute_Length_Len%0X100)))
	{
		/* ��Ϣ�峤����Э�鲻һ�� */
		return FALSE;
	}
	
	/* �ж���Ϣ�ն����к� */	
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[0] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[1] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[2] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[3] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[4] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[5] != 0))
	{
		/* ��Ϣ�ն����к�������SN�Ų�һ�� */
		return FALSE;
	}
	
	/* ��� У�� ���ݼĴ��� */
	check_code = 0;
	/* �ж�����У�� */
	/* ���� ����ת�������������ݵ�У��  */
	for(num = 1;num < (GPRS_Rece_Data_Len-2); num++)
	{
		/* �����У�鷽ʽ:����Ч�����ݶ�ȡ��� */
		check_code ^= GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_Buffer[num];
	}
	
	if(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Check_Code != check_code)
	{
		/* ��Ϣ����У���쳣 */
		return FALSE;
	}
	
	/* ����ȫ��������ȡ������ʱ������ */
	System_Clock_Updata_DR.Year		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Year) + 2000;
	System_Clock_Updata_DR.Month 	= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Month);
	System_Clock_Updata_DR.Day 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Day);
	System_Clock_Updata_DR.Hour 	= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Hour);
	System_Clock_Updata_DR.Min 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Min);
	System_Clock_Updata_DR.Sec 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Sec);
	

	/* ����ϵͳʱ�䳬ǰ�� */
	/* ����ǰ��ʱ������ת���������ݡ� */
	system_clock_updata_second_b = RTC_Date_Time_To_Second(&System_Clock_Updata_DR);
	//�������ڷ���������������ݽ��ճ�ʱ�������ܣ��ᵼ�������ʵ�����޷��洢��ƽ̨���棬������������ƽ̨����ʱ��У׼ʱ��ʱ�䳬ǰ������ƽ̨,
	//�����Ϳ��Գ�ǰ��������������ǰ�������ϴ�����ƽ̨�ϣ��������Լ��ٴ����ı������ݡ�
#if	(System_Clock_Enable == 0x01)
	/* ����ϵͳ��ǰʱ�� */
	system_clock_updata_second_b += System_Time_Lead_Num;
#endif
	/* ����ʱ��ת����ʵ��ʱ��. */
	RTC_Second_To_Date_Time(system_clock_updata_second_b,&System_Clock_Updata_DR);

	
	/* �������������������ݷ��ͳɹ��� */
	return TRUE;
	
/* ���յ����ݳ��� */
#undef GPRS_Rece_Data_Len
/* ���յ���Ϣ�����ݳ��� */	
#undef GPRS_Rece_Attribute_Length_Len
}

#elif (AppGPRSComProtocol == 0x01)
/* httpʱ��У׼�ṹ */
typedef struct 
{
	/* �̶��ַ�����Date: Tue, 07 Jun 2016 01:16:00 GMT��  */
	char Date[4];
	/* �̶��ַ�����: �� */
	char Sign_1[2];
	/* ���� */
	char Week[3];
	/* �̶��ַ�����, �� */
	char Sign_2[2];
	/* ���� */
	char Day[2];
	/* �̶��ַ����� �� */
	char Sign_3;
	/* �·� */
	char Month[3];
	/* �̶��ַ����� �� */
	char Sign_4;
	/* ��� */
	char Year[4];
	/* �̶��ַ����� �� */
	char Sign_5;
	/* ʱ */
	char Hour[2];
	/* �̶��ַ�����:�� */
	char Sign_6;
	/* �� */
	char Min[2];
	/* �̶��ַ�����:�� */
	char Sign_7;
	/* �� */
	char Sec[2];
	/* �̶��ַ�����:�� */
	char Sign_8;
	/* ��������ʱ�䡰GMT�� */
	char Sign_9[3];
}HTTPClockCalibrationStruct;



static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer)
{
	/* ����ָ�� */
	char * datap;
	/* ������ */
	u8 num;
	char   *stop_at;
	
	/* ���� */
	char* weekData[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun",};
	/* �·� */
	char* monthData[] = {"Jan" ,"Feb" ,"Mar" ,"Apr" ,"May" ,"Jun",
						 "Jul" ,"Aug" ,"Sep" ,"Oct" ,"Nov" ,"Dec",};
	
	/* ����ʱ��ṹ */
	HTTPClockCalibrationStruct * timestamp;
						 

	/* ϵͳ����ʱ�����Ĵ��� */
	u32 system_clock_updata_second_b;
					 
	
	/* ��ѯHTTP����ɹ��ַ��� */
	datap = strstr((char *)parsing_data_buffer,"HTTP/1.1 200 OK");
	/* �ж�HTTP�����Ƿ�ɹ� */
	if(NULL == datap)
	{
		/* ����ʧ�� */
		return FALSE;
	}
	
	/* ��ѯ����ƽ̨�ɹ������ַ��� */
	datap = strstr((char *)parsing_data_buffer,"\"code\":\"0000\"");
	/* �ж�����ƽ̨�Ƿ�ɹ��������� */
	if(NULL == datap)
	{
		/* ����ʧ�� */
		return FALSE;
	}
	
	{
		/* ��ѯ����ƽ̨�ɹ������ַ��� */
		datap = strstr((char *)parsing_data_buffer,"Date: ");
		/* �ж�����ƽ̨�Ƿ�ɹ��������� */
		if(NULL == datap)
		{
			/* ����ʧ�� */
			return FALSE;
		}
		
		/* ����ʱ��� */
		timestamp = (HTTPClockCalibrationStruct *)datap;
		
		/* �������� */
		/* ����ȫ��������ȡ������ʱ������ */
		for(num = 0;num < 7;num++)
		{
			/* ��ѯ����ƽ̨�ɹ������ַ��� */
			datap = strstr((char *)parsing_data_buffer,weekData[num]);
			/* �ж�����ƽ̨�Ƿ�ɹ��������� */
			if(NULL != datap)
			{
				/* �������� */
				System_Clock_Updata_DR.Week = num+1;
				break;
			}
		}
		
		/* �ж��������Ƿ���ڵ���7 */
		if(num >= 7)
		{
			/* ����ʧ�� */
			return FALSE;
		}
		
		/* �����·� */
		/* ����ȫ��������ȡ������ʱ������ */
		for(num = 0;num < 12;num++)
		{
			/* ��ѯ����ƽ̨�ɹ������ַ��� */
			datap = strstr((char *)parsing_data_buffer,monthData[num]);
			/* �ж�����ƽ̨�Ƿ�ɹ��������� */
			if(NULL != datap)
			{
				/* �������� */
				System_Clock_Updata_DR.Month = num+1;
				break;
			}
		}
		
		/* �ж��������Ƿ���ڵ���7 */
		if(num >= 12)
		{
			/* ����ʧ�� */
			return FALSE;
		}
		
		/* �������� */
		stop_at = &timestamp->Sign_3;

		System_Clock_Updata_DR.Day = strtoul(timestamp->Day,&stop_at,10);
		
		/* ������� */
		stop_at = &timestamp->Sign_5;

		System_Clock_Updata_DR.Year = strtoul(timestamp->Year,&stop_at,10);
		
		/* ����"ʱ" */
		stop_at = &timestamp->Sign_6;

		System_Clock_Updata_DR.Hour = strtoul(timestamp->Hour,&stop_at,10);
		
		/* ����"��" */
		stop_at = &timestamp->Sign_7;	

		System_Clock_Updata_DR.Min 	= strtoul(timestamp->Min,&stop_at,10);
		
		/* ����"��" */
		stop_at = &timestamp->Sign_8;

		System_Clock_Updata_DR.Sec 	= strtoul(timestamp->Sec,&stop_at,10);
	}
	/* ƽ̨��������������ʱ����� */
	

	/* ����ǰ��ʱ������ת���������ݡ� */
	system_clock_updata_second_b = RTC_Date_Time_To_Second(&System_Clock_Updata_DR);
	//�������ڷ���������������ݽ��ճ�ʱ�������ܣ��ᵼ�������ʵ�����޷��洢��ƽ̨���棬������������ƽ̨����ʱ��У׼ʱ��ʱ�䳬ǰ������ƽ̨,
	//�����Ϳ��Գ�ǰ��������������ǰ�������ϴ�����ƽ̨�ϣ��������Լ��ٴ����ı������ݡ�
#if	(System_Clock_Enable == 0x01)
	/* ����ϵͳʱ�䳬ǰ�� */
	system_clock_updata_second_b += System_Time_Lead_Num;
#endif
	/* ����HTTP�õ�ʱ���Ǹ�������ʱ������ʱ��8��Сʱ���������Ҫ����8��Сʱ */
	system_clock_updata_second_b += 8*60*60;
	/* ����ʱ��ת����ʵ��ʱ��. */
	RTC_Second_To_Date_Time(system_clock_updata_second_b,&System_Clock_Updata_DR);
	
	/* �������������������ݷ��ͳɹ��� */
	return TRUE;
	
}
#endif
#endif

/************************************************************************************************/
/* ������	: GPRS_Send_System_Clock_Calibration_Data											*/
/* ����		: �� 																				*/
/* ���		: ��																				*/
/* ����		: GPRS����ʱ��У׼����							 									*/
/* ��������	: 2015/06/26																		*/
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))//����Э���
static  void GPRS_Send_System_Clock_Calibration_Data(void)//zwc
{

	//1����������
	/* ���ý��շ������ݳ��ȡ� */
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len = GPRS_Send_System_Clock_Calibration_Buffer_Size;//�������ݳ���
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len = GPRS_Rece_System_Clock_Calibration_Buffer_Size;//�������ݳ���

#if	(AppGPRSComProtocol == 0x00)
	/* ����ʱ��У׼��Ϣ   ���ǲ���Э��ʱʱ��У׼�ķ�����Ϊ���ݷ�����*/
		/* ����ʱ��У׼��Ϣ */
  GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
   (BOOL)Gsm_gprs_comm(
                       (char*)GPRS_Send_System_Clock_Calibration_Buffer,
  	                   (GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len),
                       (char*)GPRS_Rece_System_Clock_Calibration_Buffer,
                       ((&GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len)),
  								      GSM_GPRS_Data
  				             );
#endif	
#if	(AppGPRSComProtocol == 0x04)
	/* ����ʱ��У׼��Ϣ   ���ǽ���ʡ���߹滮��������Э��ʱʱ��У׼�ķ�����Ϊ���÷�����*/
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
	(BOOL)Gsm_gprs_comm(
	   (char*)GPRS_Send_System_Clock_Calibration_Buffer,
		 GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len,
	   (char*)GPRS_Rece_System_Clock_Calibration_Buffer,
	   ((int*)(&GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len)),
		 GSM_GPRS_Config);
#endif	
	//2����������	  
	/* �ж� ���ݷ��ͽ�� */
	if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result == FALSE)
	{
		  /* �ָ�ϵͳʱ��У׼״̬�Ĵ���,���»�ȡʱ������ */
		  RTC_System_Clock_Calibration_State_Reset();
	}else
	{
		Log_printf("�����ݷ�������ȡʱ����Ϣ�ɹ�������\r\n");	
		/* ���ݷ��ͳɹ� */
		/* �������������ص�ʱ�����ݣ��жϱ���GPRSͨ��״̬�� */
		GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_System_Clock_Calibration_Data(GPRS_Rece_System_Clock_Calibration_Buffer);
		/* �ж� ����GPRSͨ�� ��� */
		if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result == TRUE)
		{
			/* �������������ݳɹ� */
			/* ����RTCʱ�� */
			/* ��λ RTC */
			RTC_Init(TRUE);
			/* ���� ʱ�� */
			Write_System_Date_Time_Data(&System_Clock_Updata_DR);

		}else
		{
			/* �ָ�ϵͳʱ��У׼״̬�Ĵ���,���»�ȡʱ������ */
			RTC_System_Clock_Calibration_State_Reset();
		}
	}
	
}
#elif (AppGPRSComProtocol == 0x01)
static void GPRS_Send_System_Clock_Calibration_Data(void)
{
	/* ����ʱ��У׼���ݻ��� */
	char clockCalibrationSendBuf[512];
	/* ����ʱ��У׼���ݻ��� */
	char clockCalibrationReceBuf[512];
	/* �������ݳ��� */
	int sendLen;
	/* �������ݳ��� */
	int receLen;
	/* ��ʱʱ��Ĵ��� */
	Times time;
	
	
	/* ������ݻ��� */
	memset(clockCalibrationSendBuf,0x00,512);
	memset(clockCalibrationReceBuf,0x00,512);
	memset(((char*)(&time)),0x00,sizeof(Times));
	/* �������� */

	/* ������ݽӿڲ��� */
	memset((char*)&HTTPPara,0x00,sizeof(HTTPPara));

	/* �������ر�� */
	sprintf(HTTPPara.ApiPara.GatewayIdNo,"%04X%05d",
	((((*(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[0])) & 0xff00) >> 8)|
	((( *(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[0])) & 0x00ff) << 8)),
	((((*(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[2])) & 0xff00) >> 8)|
	((( *(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[2])) & 0x00ff) << 8)));
	/* �������Ĳɼ�ʱ��ת�����룬�������������Ϊ����ǩ����ʱ�����ݡ� */
	HTTPPara.ApiPara.TimeStamp = 0;
	
	/* �������ǩ�����ݻ��档 */
	memset(HTTPPara.ApiPara.Sign,0x00,sizeof(HTTPPara.ApiPara.Sign));
	
	/* δʹ��GPS�豸 */
	HTTPPara.ApiPara.GPS_State = FALSE;
	/* ����������� *///�ݶ�0���������ó����ߺ����ݡ�
	HTTPPara.ApiPara.Lng = 0;
	/* ���γ������ *///�ݶ�0���������ó����ߺ����ݡ�
	HTTPPara.ApiPara.Lat = 0;
	
	/* ���òɼ�ʱ�� */
	time.Year 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year;
	time.Month	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month;
	time.Day 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day;
	time.Hour 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour;
	time.Min 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min;
	time.Sec 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec;
	
	/* ���òɼ�ʱ�� */
	sprintf(HTTPPara.ApiPara.CollectTime,"%04d-%02d-%02d %02d:%02d:%02d",
			time.Year,time.Month,time.Day,time.Hour,time.Min,time.Sec);
	
	/* ��ȡ��ǰ̽ͷ���� */
	HTTPPara.ApiPara.ProbeNum = 0;

	/* ��ȡʱ��У׼���� */
	HTTP_Create_Data(clockCalibrationSendBuf);
	
	/* ���㷢�����ݳ��� */
	sendLen = strlen(clockCalibrationSendBuf);
	/* ���ý������ݳ��� */
	receLen = 512;
	
	/* ����ʱ��У׼��Ϣ */
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
	(BOOL)gsm_gprs_comm(
	  (char*)(clockCalibrationSendBuf),
			 (sendLen),
	  (char*)(clockCalibrationReceBuf),
	  ((int*)(&receLen)));

	//2����������	  
	/* �ж� ���ݷ��ͽ�� */
	if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result == FALSE)
	{
		/* �ָ�ϵͳʱ��У׼״̬�Ĵ���,���»�ȡʱ������ */
		RTC_System_Clock_Calibration_State_Reset();
	}else
	{
		/* ���ݷ��ͳɹ� */
		/* �������������ص�ʱ�����ݣ��жϱ���GPRSͨ��״̬�� */
		GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_System_Clock_Calibration_Data((u8*)clockCalibrationReceBuf);
		/* �ж� ����GPRSͨ�� ��� */
		if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result == TRUE)
		{
			/* �������������ݳɹ� */
			/* ����RTCʱ�� */
			/* ��λ RTC */
			RTC_Init(TRUE);
			/* ���� ʱ�� */
			Write_System_Date_Time_Data(&System_Clock_Updata_DR);
			
		}else
		{
			
			/* �ָ�ϵͳʱ��У׼״̬�Ĵ���,���»�ȡʱ������ */
			RTC_System_Clock_Calibration_State_Reset();
		}
	}
}
#else
static void GPRS_Send_System_Clock_Calibration_Data(void)
{
	__nop();
}

#endif
#endif
/************************************************************************************************/
/* ������	: GPRS_Reset_Sensor_Data_Reg														*/
/* ����		: �� 																				*/
/* ���		: ��																				*/
/* ����		: ��λ��ʪ�����ݼĴ���								 									*/
/* ��������	: 2015/06/26																		*/
/************************************************************************************************/
void GPRS_Reset_Sensor_Data_Reg(void)
{
	
	
	/* GPRS ���� ������ �������� */	
	GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len = 0;
	/* GPRS ���� ������ �������� */
	GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = 0;
	/* GPRS ���ʹ���������ͨ��״̬��TURE:���ڷ��ͣ�FALSE:û�з��ͣ� */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = FALSE;
	/* GPRS �����򴫸������ݵȴ�����״̬��TURE:�ȴ�����FALSE:û�����ݴ��� */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = FALSE;
	/* GPRS �����򴫸������ݵȴ�����״̬��TURE:�ȴ�����FALSE:û�����ݴ��� */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = FALSE;
	/* GPRS ���ʹ��������ݽ�� */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = TRUE;
	/* GPRS ���մ��������ݽ�� */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result = TRUE;
	/* GPRS ���ʹ��������� ʧ�ܴ��� */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum	= 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum	= 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum	= 0;
	
}

/************************************************************************************************/
/* ������	: GSM_Config_GPRS_IP																*/
/* ����		: string:IP�ַ���																	*/
/* ���		: �� 																				*/
/* ����		: �������ݷ���������IP��																*/
/* ��������	: 2016/02/22																		*/
/* �޸�����	: 2016/02/22																		*/
/************************************************************************************************/
BOOL GSM_Config_Network(char * networkstring,NetworkConfigStruct* network)
{

	/* ipֵ */
	uint16_t ip[4] ={0,0,0,0};
	/* ������ */
	uint32_t num1,num2;
	/* ��ǰ�ַ�ֵ */
	int8_t asciiValue;
	/* �����ַ������� */
	int len = strlen((char*)networkstring);
	
	
	/* ����������� */
	memset((int8_t*)network,0x00,sizeof(NetworkConfigStruct));
	

	/* �ַ������ȴ�����󳤶� */
	if(len > sizeof(network->DomainName))
	{
		/* �������������󳤶ȣ������� */
		return FALSE;
	}
	
	/* �жϵ�ǰ�ַ��������Ƿ�Ϸ� */
	for(num1 = 0,num2 = 0; num1 < len; num1++)
	{
		/* �жϵ�ǰ�ַ��� */
		if(networkstring[num1] == '.')
		{
			/*�ָ��*/
			/* �����ַ�λ�� */
			num2 ++;
		}else
		{
			/* ��ȡASCIIֵ */
			asciiValue = networkstring[num1] - '0';
			/* �ж�ASCII��ֵ�Ƿ���0-9��Χ�� */
            if(asciiValue > 9 || asciiValue < 0)
			{
				/* ���ڷ����ֲ�����˵���������� */
				memcpy(network->DomainName,networkstring,len);
				return FALSE;
			}else
			{
				/* ���� */
				ip[num2] = ip[num2] * 10 + asciiValue;
				/* �ж�IPֵ�Ƿ���� */
				if(ip[num2] > 255)
				{
					/* ��IP��ʽ���� */
					memcpy(network->DomainName,networkstring,len);
					return FALSE;
				}
			}
		}
	}
	
	network->IP.Serial_Number_Number.Address_A = ip[0];
	network->IP.Serial_Number_Number.Address_B = ip[1];
	network->IP.Serial_Number_Number.Address_C = ip[2];
	network->IP.Serial_Number_Number.Address_D = ip[3];

	/* IP���óɹ� */
	return TRUE;
}

/******************* (C) ZHOU *****END OF FILE****/
