#ifndef __Log_GSM_H
#define __Log_GSM_H

#include "stm32f10x.h"

#include "Mcu_Config.h"

#include "Log_Sensor.h"
#include "Log_SMS.h"

#include "ucos_ii.h"

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//===========================================================================
//
//===========================================================================
//
//  <e0> [�����ϴ�Э��]����.
//  <i>  Ŀǰ������Э��:����Э��(��׼Э��)��HTTP(�����Ŷ���).
//	<o1.0..2> ѡ������[����Э��]
//  <0=> [����Э��]Э��.
//  <1=> [HTTP]Э��.
//  <2=> [������]Э��.
//  <3=> [�ӱ�����]Э��.
//  <4=> [����ʡ���߹滮��������]Э��.
//
//  <o2.0..9>GPRS��������
//  <i>GPRS������������ʧ�ܣ�����ʱ�����ã�1����=42M��2����=21M�����μ�����
//  <60 => �ȴ�[1����]
//  <120=> �ȴ�[2����]
//  <180=> �ȴ�[3����]
//  <240=> �ȴ�[4����]
//  <300=> �ȴ�[5����]
//  <i>Ŀǰֻ�������ȴ�5����ʱ�䣬��������1���ӡ�
//
//  <o3.0..1> ��ҩ�ӿ�ת������
//  <i>  �ϴ�����������ʱ�������ҩ�ӿڷ���һ�����ݡ�
//  <0 => [�ر�]
//  <1 => [����]
//
//  <o4.0..1> ���ǽӿ�ת������
//  <i>  �ϴ�����������ʱ���������ǽӿڷ���һ�����ݡ�
//  <0 => [�ر�]
//  <1 => [����]
//
//  <o5.0..1> ��󿪷��Խӿڹ���
//  <i>  ����ӿ���Ҫ�ǿ��Ÿ����Լ�ƽ̨�Ŀͻ�����ʹ�á�
//  <0 => [�ر�]
//  <1 => [����]
//
//  </e>
#define AppGPRSComProtocolEnable		(0x01)//[�����ϴ�Э��]����
#define AppGPRSComProtocol				  (0x00)//ѡ���������Э��(0:����Э��;1:HttpЭ��;2:������Э�飻3���ӱ�����Э��;4:����ʡ���߹滮��������Э��)
#define AppGPRSUsageProtectTime			(300)//GPRS��������ʱ�䣬��λ����
#define AppGYForwardEnable				  (0)//��ҩ�ӿ�ת������(0:�ر�;1:����)
#define ComProtocol_WZ_Enable			  (0)//���ǽӿ�ת������(0:�ر�;1:����)
#define ComProtocol_ZDE_Enable			(0)// ��󿪷��Խӿڹ���(0:�ر�;1:����)
//	<<< end of configuration section >>>

/* GPRS SN�Žṹ��С */
#define	GPRS_SN_NO_Buffer_Size				(6u)
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
/* GPRS�������ݼĴ����ṹ��С */
//����Ϊʲô������1024�ֽ�����Ϊ���������δת��������ݣ�
//����ת��������������ӣ��������Ҫ��ת�����0X7E\0X7D��������
//�������ó�900�ֽڣ�Ҳ���൱�ڷ����ˣ�1024-924=100���ֽڵ�ת����ſ���
//һ��ֻ�м�������ʮ��ת�����
#define	GPRS_Send_Buffer_Size				(924u)
/* GPRS�������ݼĴ����ṹ��С */
#define	GPRS_Rece_Buffer_Size				(64u)
#elif (AppGPRSComProtocol == 0x01)
#define	GPRS_Send_Buffer_Size				(1536u)
/* GPRS�������ݼĴ����ṹ��С */
#define	GPRS_Rece_Buffer_Size				(1024u)
#elif (AppGPRSComProtocol == 0x04)//����ʡ���߹滮��������Э��	
#define	GPRS_Send_Buffer_Size				(512u)


/* GPRS�������ݼĴ����ṹ��С */
#define	GPRS_Rece_Buffer_Size				(1024u)
#else
#define	GPRS_Send_Buffer_Size				(1u)
/* GPRS�������ݼĴ����ṹ��С */
#define	GPRS_Rece_Buffer_Size				(1u)
#endif
#endif


/* GPRS�������ݼĴ����ṹ��С *///�������ó�  ��������ͨ���������ģ�������ͨ��8������Ҳ����˵���=12+32*4=140
#define	GSM_GPRS_Data_Content_Buffer_Size	(140u)
/* ������ SN�����С */
#define GPRS_Data_Sensor_Buffer_Size		(4u)
/* GPRS����ʱ��У׼���ݼĴ�����С */
#define GPRS_Rece_System_Clock_Calibration_Buffer_Size	(23u)


//GPRS ������ʪ��������Ϣ ���ݽṹ
typedef union
{
	struct
	{	
		//��·���䳵��ͨѶЭ Э����ô��ģʽ(big-endian)�������ֽ����������ֺ�˫�֡�
		//������ݽṹֻ����Ӧ2ͨ���Ĵ�����GPRS���ݷ��ͣ����Ҳ��ܴ���״̬��Ϣ��
		/* GPRS ������Ϣ */
		/* ͷ ��ʶλ */ //(1 Byte)
		u8				Start_Iden;
		/* ͨ�� ��Ϣͷ *///(16 Byte)
		/* ��Ϣ ID(����) */		//(2 Byte)
		u8				ID_Type_HSB;
		u8				ID_Type_LSB;
		/* ��Ϣ�� ���� */		//(2 Byte)
		//��Ϣ������ ��Ϣ�峤��
		u8	 			Attribute_HSB_Length:2;		
		//��Ϣ������ ���ݼ��ܷ�ʽ
		u8	 			Attribute_HSB_Enc_Mode:3;
		//��Ϣ������ �ְ�
		u8	 			Attribute_HSB_Sub_Flag:1;		
		//��Ϣ������ ����
		u8 				Attribute_HSB_Reserve:2;
		//��Ϣ������ ��Ϣ�峤��
		u8	 			Attribute_LSB_Length:8;
		/* �ն����к�(SN��): */	//(6 Byte)
		u8				SN_NO[GPRS_SN_NO_Buffer_Size];
		/* ��Ϣ ��ˮ�� */		//(2 Byte)
		u8				Serial_NO_HSB;
		u8				Serial_NO_LSB;

		/* ͨ�� ��Ϣ�� *///(3 Byte + C * (16 + 4 * N))
		/* ��Ϣ����:WIFI ��Ƭʽ������(����ֻ������ģʽ) *///(1 Byte)
		u8				Body_Type;//��Ϣ������
		/* ��Ϣ��������:0->ʵʱ���ݣ�1->��ʷ���ݡ� *///(1 Byte)
		u8				Data_Type;
		/* ����֡��(0-255) *///(1 Byte)
		u8				Data_Frame_Num;
		/* ��������(�����Ҫ��Ϊ�˸�����������ݵ�ָ��) */
		u8				Data_P;

	}GPRS_IR_FIX;
	
	u8 GSM_GPRS_Send_Buffer[GPRS_Send_Buffer_Size];
}GSM_GPRS_Send_Struct;
/************************************************************************************************/
#if (AppGPRSComProtocol == 0x00)//ԭ�Ȳ���Э��
typedef union
{
	struct
	{	
		/* ������(ID��): */	//(4 Byte)
		u8				SN_NO[GPRS_Data_Sensor_Buffer_Size];
		/* ��ӵ�Դ״̬ */
		u8				ET_State;
		/* Ԥ��1 */
		u8				Reserved1;
		/* Ԥ��2 */
		u8				Reserved2;
		/* ��ص��� */
		u8				BT_Charge_Alarm_State;
		/* ʱ��� */			//(6 Byte)
		//��//(1 Byte)
		u8				Timestamp_Year;
		//��//(1 Byte)
		u8				Timestamp_Month;
		//��//(1 Byte)
		u8				Timestamp_Day;
		//ʱ//(1 Byte)
		u8				Timestamp_Hour;
		//��//(1 Byte)
		u8				Timestamp_Min;
		//��//(1 Byte)
		u8				Timestamp_Sec;
		/* ͨ���� */
		u8				CH_Num;
		/* ���������� */
		u8				Sensor_Type;
		/* ����(�����Ҫ��Ϊ�˸�����������ݵ�ָ��) */
		float			Data_P;
		
	}GPRS_DR_FIX;
	
	u8 GSM_GPRS_Data_Content_Buffer[GSM_GPRS_Data_Content_Buffer_Size];
}GSM_GPRS_Data_Content_Struct;
#endif

/************************************************************************************************/
//GPRS ������ʪ��������Ϣ ���ݽṹ
typedef union
{
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))		
	struct
	{	//��·���䳵��ͨѶЭ Э����ô��ģʽ(big-endian)�������ֽ����������ֺ�˫�֡�
		//������ݽṹֻ����Ӧ2ͨ���Ĵ�����GPRS���ݷ��ͣ����Ҳ��ܴ���״̬��Ϣ��
		/* GPRS ������Ϣ */
		/* ͷ ��ʶλ */ //(1 Byte)
		u8				Start_Iden;		
		/* ͨ�� ��Ϣͷ *///(16 Byte)
		/* ��Ϣ ID(����) */		//(2 Byte)
		u8				ID_Type_HSB;
		u8				ID_Type_LSB;
		/* ��Ϣ�� ���� */		//(2 Byte)
		//��Ϣ������ ��Ϣ�峤��
		u8	 			Attribute_HSB_Length:2;
		//��Ϣ������ ���ݼ��ܷ�ʽ
		u8	 			Attribute_HSB_Enc_Mode:3;
		//��Ϣ������ �ְ�
		u8	 			Attribute_HSB_Sub_Flag:1;
		//��Ϣ������ ����
		u8 				Attribute_HSB_Reserve:2;
		//��Ϣ������ ��Ϣ�峤��
		u8	 			Attribute_LSB_Length:8;
		/* �ն����к�(SN��): */	//(6 Byte)
		u8				SN_NO[GPRS_SN_NO_Buffer_Size];		
		/* ��Ϣ ��ˮ�� */		//(2 Byte)
		u8				Serial_NO_HSB;
		u8				Serial_NO_LSB;

		/* Ӧ�� ��ˮ�� */		//(2 Byte)
		u8				ACK_Serial_NO_HSE;
		u8				ACK_Serial_NO_LSE;
		/* Ӧ�� ID */			//(2 Byte)
		u8				ACK_ID_Type_HSE;
		u8				ACK_ID_Type_LSE;
		/* Ӧ�� ��� */			//(1 Byte)
		u8				ACK_Result;
		
		/* У���� */				//(1 Byte)
		u8				Check_Code;		
		/* β ��ʶλ */			//(1 Byte)
		u8				End_Iden;	
	}GPRS_IR_FIX;
	u8 GSM_GPRS_Rece_Buffer[GPRS_Rece_Buffer_Size];
#endif		
	
#if (AppGPRSComProtocol == 0x04)//����ʡ���߹滮��������Э��			
	struct
	{
	  u8	 JS_DL_SYNC_ID;//�豸��������ͬ����ʶλֵ
		u8   JS_DL_STX_ID;//�豸����������ʼ��ʶλֵ
		u8   JS_Rev_Data_Len;   //�豸�����������ݳ���
		u8   JS_Rev_Succeed_Flag;//���������յ���ȷ�����ݷ��صĳɹ���־λ
	 int  JS_Rev_Check_Code;////�豸���յ�������У����
	}GPRS_IR_FIX;
	u8 GSM_GPRS_Rece_Buffer[GPRS_Rece_Buffer_Size];
#endif	
	
	
	
	
}GSM_GPRS_Rece_Struct;





/************************************************************************************************/
/* GPRS����ʱ��У׼���ݽṹ */
typedef union
{
	struct
	{	
		//��·���䳵��ͨѶЭ Э����ô��ģʽ(big-endian)�������ֽ����������ֺ�˫�֡�
		//������ݽṹֻ����Ӧ2ͨ���Ĵ�����GPRS���ݷ��ͣ����Ҳ��ܴ���״̬��Ϣ��
		/* GPRS ������Ϣ */
		/* ͷ ��ʶλ */ 			//(1 Byte)
		u8				Start_Iden;		
		/* ͨ�� ��Ϣͷ */
		/* ��Ϣ ID(����) */		//(2 Byte)
		u8				ID_Type_HSB;
		u8				ID_Type_LSB;
		/* ��Ϣ�� ���� */		//(2 Byte)
		//��Ϣ������ ��Ϣ�峤��
		u8	 			Attribute_HSB_Length:2;
		//��Ϣ������ ���ݼ��ܷ�ʽ
		u8	 			Attribute_HSB_Enc_Mode:3;
		//��Ϣ������ �ְ�
		u8	 			Attribute_HSB_Sub_Flag:1;
		//��Ϣ������ ����
		u8 				Attribute_HSB_Reserve:2;
		//��Ϣ������ ��Ϣ�峤��
		u8	 			Attribute_LSB_Length:8;
		/* �ն����к�(SN��): */	//(6 Byte)
		u8				SN_NO[GPRS_SN_NO_Buffer_Size];
		/* ��Ϣ ��ˮ�� */		//(2 Byte)
		u8				Serial_NO_HSB;
		u8				Serial_NO_LSB;

		/* Ӧ�� ��ˮ�� */		//(2 Byte)
		u8				ACK_Serial_NO_HSE;
		u8				ACK_Serial_NO_LSE;
		/* ������ʱ�� */			//(6 Byte)
		u8				Server_Year;
		u8				Server_Month;
		u8				Server_Day;
		u8				Server_Hour;
		u8				Server_Min;
		u8				Server_Sec;

		/* У���� */				//(1 Byte)
		u8				Check_Code;		
		/* β ��ʶλ */			//(1 Byte)
		u8				End_Iden;	
	}GSM_GPRS_Rece_IR;

	u8 GSM_GPRS_Rece_Buffer[GPRS_Rece_System_Clock_Calibration_Buffer_Size];
}GSM_GPRS_RECE_CLOCK_CALI_Struct;

/* GSM�� ��Ϣ��� ��Ϣ��������(1 Byte) */
typedef enum {
	/* ����GSMģ�� */
	Init_GSM_Module_EVENT	                 = 0X00,
	/* �����ź�ǿ�� */                      
	Update_SGL_EVENT		                   = 0X01,
	/* ���¸���״̬ */                      
	Update_Link_EVENT		                   = 0X02,
	/* �ϴ���ʪ������ */                    
	Updata_Data_EVENT 		                 = 0X03,
	/* ���� ������� ���� */                
	IAP_EVENT 				                     = 0X04,
	/* ���Ͷ��� */                         
	Send_SMS_EVENT			                   = 0X05,
	/* ���÷����� �¼� */                   
	ConfigService_EVENT		                 = 0X06,
//	/* �ϴ� ���ûظ� ���� */	
//	Updata_Config_Back_EVENT = 0X07,
	/* ϵͳʱ��У׼ */
	System_Clock_Calibration_EVENT         = 0x08,
	
	
		/* ����Wifi���ֺ����� �¼� */
	SetWifiNamePWD_EVENT		               = 0X09,
	/* ����WLAN����(����IP���������롢���ص�ַ) �¼� */
	SetWifiWLAN_EVENT		                   = 0X0A,
	/* ����Wifi IP��ַ��ȡ��ʽ STATIC����DHCPģʽ�¼� */
	SetWifiWLAN_GET_IP_WAY_EVENT		       = 0X0B,
	/* ��λWifiģ���¼� */
	RestWifi_EVENT		                     = 0X0C,
	/* Wifiģ���¼� */
	Factory_Data_Reset_Wifi_EVENT		       = 0X0D,
	
	
}GSM_CMD_TYPE;

typedef struct
{
	GSM_CMD_TYPE	       EVENT_Cmd;		// �¼� ����
	u32*			           EVENT_Str;		// �¼� ���� ָ��
	BOOL			           EVENT_Getarg;	// �¼� ִ�н��(FALSE:�ź�����ִ�У�TRUE:�ͺ�ִ�н���)
}GSM_QUE;

/* ���ñ� ��Ϣ �޸� ���ݽṹ */	
typedef union 
{
	struct			 
	{
		/* �޸����ñ� ���� */
		u32		Change_Type			:8;
		/* �޸����ñ� ��ַ */
		u32		Change_Adder		:16;
		/* �޸����ñ� ���ݳ��� */
		u32		Change_Data_Len		:16;
	}CR;
	u8 CR_REG[Config_Tab_Chane_Size];
}CONFIG_TAB_CHANGE;
/************************************************************************************************/
typedef struct {
	/* GPRS ���� ������ �������� */
	int 		GPRS_Rece_Sensor_Data_Len;
	/* GPRS ���� ������ �������� */
	int 		GPRS_Send_Sensor_Data_Len;	
	
	/* GPRS ���ʹ���������ͨ��״̬��TURE:���ڷ��ͣ�FALSE:û�з��ͣ� */
	BOOL		GPRS_COM_Send_State;
	/* GPRS �����򴫸������ݵȴ�����״̬��TURE:�ȴ�����FALSE:û�����ݴ��� */
	BOOL		GPRS_COM_Host_Waiting_State;
	/* GPRS �����򴫸������ݵȴ�����״̬��TURE:�ȴ�����FALSE:û�����ݴ��� */
	BOOL		GPRS_COM_Network_Waiting_State;	
	/* GPRS ���ʹ��������ݽ�� */
	BOOL		GPRS_COM_Send_Result;
	/* GPRS ���մ��������ݽ�� */
	BOOL		GPRS_COM_Rece_Result;
	/* GPRS ��������״̬   */	
	BOOL		GPRS_COM_ParseState;
	/* GPRS ���ʹ��������� ʧ�ܴ��� */
	u8			GPRS_COM_DataFailNum;
	/* GPRS ������������ ʧ�ܴ��� */
	u8			GPRS_COM_ConfigServiceFailNum;
	/* GPRS ������������ ʧ�ܴ��� */
	u8			GPRS_COM_IAPFailNum;
	
}GSM_GPRS_Send_Sensor_Data_Struct;
/************************************************************************************************/
/* SIM��״̬�ṹ */
typedef enum 
{
	/* SIM���� */
	SIM_OK = 0x00,
	
	/* ��GPRS�ź� */
	SIM_NoSignal = 0X01,
	
	/* SIMδ���� */
	SIM_NotDetected = 0x02,
	
	/* SIMǷ��ͣ��,Ҳ�п����ǿպ� */
	SIM_ShutDown = 0x03,
	
}SCSStruct;
/************************************************************************************************/
//GPRS ״̬ �ṹ
typedef struct {
	/* GPRS �ź� ǿ�� */
	int 		GPRS_Sign_Stre;
	/* GPRS �ź� ǿ�� ��ʾ(0-5:0��;6-8:1��;9-11:2��;12-14:3��;15-31:4��;) */
	u8			GPRS_Sign_Stre_Disp;
	/* GPRS ģ�鹤��״̬ */
	BOOL		GPRS_Work_State;
	/* GPRS ����״̬ */
	BOOL 		GPRS_Link_State;
	/* Sim��״̬��Ϣ */ 
	SCSStruct	SimState;

}GSM_GPRS_SR_Struct;

/************************************************************************************************/
//GSM ���� ״̬�Ĵ���
typedef struct {
	/* GSM  ���ŷ���״̬��TURE:���ڷ��ͣ�FALSE:�������/û�з��ͣ� */
	BOOL		GSM_SMS_Send_State;
	/* GSM  ���ŷ��ͽ����TURE:���ڳɹ���FALSE:����ʧ�ܣ� */
	BOOL		GSM_SMS_Send_User_Result[SMS_User_Number];

}GSM_SMS_SR_Struct;

/************************************************************************************************/
//GSM ���� ���ݼĴ���
typedef struct {
	/* GSM  ���ŷ��ͺ���Ĵ���.
	   �����ֻ����붼Ϊ11λ���������ǿ������Ϊ12.���һλΪ�������� */
	char		GSM_SMS_User_NO[12];


}GSM_SMS_DR_Struct;

/* GSMģ�������������ݽṹ */
typedef struct 
{
	/* IP��ַ */
	SERVER_IP_ADDRESS IP;
	/* �˿ں� */
	u16 Port;
	/* ���� */
	u8 DomainName[32];	
}NetworkConfigStruct;

/************************************************************************************************/
/* GPRS���в����ṹ */
typedef struct {

	struct ConfigService
	{
		struct 
		{
			/* ͨ�Ž�� */
			BOOL Result;
			
			
		}State;
		
		struct 
		{
			/* ��������ָ�� */
			uint8_t * 	TXDR;
			/* �������ݳ��� */
			int32_t	TXLen;
			/* ��������ָ�� */
			uint8_t * 	RXDR;
			/* �������ݳ��� */
			int32_t	RXLen;
		}Para;
		
	}ConfigService;
	
	struct IAP
	{
		struct 
		{
			/* ͨ�Ž�� */
			BOOL Result;
			
			
		}State;
		
		struct 
		{
			/* ��������ָ�� */
			uint8_t * 	TXDR;
			/* �������ݳ��� */
			int32_t	TXLen;
			/* ��������ָ�� */
			uint8_t * 	RXDR;
			/* �������ݳ��� */
			int32_t	RXLen;
		}Para;
		
	}IAP;

}GPRS_Struct;

/************************************************************************************************/
/* GSM ���� ״̬�Ĵ��� */
extern GSM_SMS_SR_Struct    GSM_SMS_SR_Reg;
/************************************************************************************************/
/* GPRS Ӧ�üĴ��� */
extern GPRS_Struct GPRS;

extern  GSM_GPRS_SR_Struct   GPRS_SR_Reg;
/************************************************************************************************/

/* �������� */
/************************************************************************************************/
void GSM_Function(GSM_QUE*  gsm_que_inf,
				  INSTRU_CONFIG_INF_TAB* config_tab,
				  INSTRU_FIX_INF_TAB* 	 fix_tab);
BOOL GSM_Send_Q(OS_EVENT* pevent,GSM_CMD_TYPE EVENT_Type);

u8 GSM_Signal_Strength(int* signal_num);
uint8_t GSMGetSignalStrength(void);
void GSM_Signal_Check(void);
void RECBUF_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE(*Config_Content)[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB);
void GPRS_Save_Sensor_Data_Check(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
void GPRS_Send_Sensor_Data(void);
GSM_GPRS_SR_Struct GPRS_Disp_Sign_Stre(void);
void GPRS_Reset_Sensor_Data_Reg(void);
u16 GPRS_Host_To_Network_Translator_Data(u8* network_endian,u8* host_endian,u16 T_num);
u16 GPRS_Network_To_Host_Translator_Data(u8* host_endian,u8* network_endian,u16 T_num);
BOOL GSM_Config_Network(char * networkstring,NetworkConfigStruct* network);
void GPRSSendData_ParseStateCheck(void);
void GSMConfig_Server_Inf(INSTRU_GSM* gprs_inf);

void   Remote_upgrade(void);//Զ����������

#endif /* Log_GSM_H */

