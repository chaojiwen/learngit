/**
  ******************************************************************************
  * @file              : dev_gsm.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 27/09/2014 17:41:57
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2014 STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef DEV_GSM_H
#define DEV_GSM_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdio.h>
#include <string.h>
#include "stm32glibc_type.h"
#include "interface.h"
//! @}


/*============================ MACROS ========================================*/
//!
//! @{
/*******************************************************************************
									ȫ�ֺ궨��
*******************************************************************************/
//!
#define MAX_DECIMAL_DIGITS_FOR_INT			10	// 32λint�������ܱ�ʾ�����ʮ��������λ��
//!
#define MAX_LEN_PN							32
#define STD_LEN_PN							11	// �й������ֻ������׼����Ϊ11λ����
//!
#define MAX_PN_OPERATON						4	// ϵͳ�����趨�������Ȩ��������
#define STD_LEN_OF_CMD_PWD					6	// ��������̶�Ϊ6���ַ�����	
//!
#define DEF_CMD_PWD_SMS			     "123456"
#define DEF_CMD_PWD_DTMF		    "999999#"	// ��������������������β����#
//!
//! @}

////////////////////////////////////////////////////////////////////////////////
/*//////////////////////////////////////////////////////////////////////////////
TP-DCS
------------------------------
Bit 3 	Bit2 	Alphabet:
------------------------------
0 		0 		Default alphabet
0 		1 		8 bit data
1 		0 		UCS2 (16bit)
1 		1 		Reserved
------------------------------
******************************************************************************/
#define SMS_PDU_ENCODING_7BIT				(0<<2)	// Default alphabet
#define SMS_PDU_ENCODING_8BIT				(1<<2)	// 8 bit data
#define SMS_PDU_ENCODING_UCS2				(2<<2)	// UCS2 (16bit)
#define SMS_PDU_ALPHABET_RESERVED			(3<<2)	// Reserved

#define SMS_MODE_TXT						1
#define SMS_MODE_PDU						0

////////////////////////////////////////////////////////////////////////////////
//! @{
// GSM global
#define MAX_LEN_GSM_TELECODE				8		// ���Ҵ������󳤶�
#define STD_LEN_GSM_IMEI					15		// GSM imei�ı�׼����
#define STD_LEN_GSM_ICCID					20		// GSM ICCID�ı�׼����
#define TO_GSM_REGISTRATION					20		// GSM�ϵ��ע�ᵽ����ĳ�ʱʱ��(һ��Ϊ10������)
#define MIN_RSSI_FOR_COMMUNICATION			5		// ά��GSM����ͨѶ��������RSSIֵ

//!sms global
// SMS���ͷ����͵�ʱ�䣬����������ʱ�����Լ�ʱ����Ϣ
#define MAX_LEN_SMS_TXTIME					32
// PDU SMS ��󳤶ȣ�M660+/M660_1120_D7S13000_V009: 140�ֽڣ�
// ���Ǹ���GSMЭ�鵥��������󳤶�Ϊ144�ֽڣ��й��ƶ�/�й���ͨ��70�����֣�
#define MAX_BYTE_SMS_PDU					140
// TXT SMS ��󳤶ȣ�M660+/M660_1120_D7S13000_V009: 159�ֽڣ�
// ���Ǹ���GSMЭ�鵥��������󳤶�Ϊ144�ֽڣ��й��ƶ�/�й���ͨ��70�����֣�
#define MAX_BYTE_SMS_TXT					140
// ���ڲ���7bit���뷢��Ӣ�Ķ���ʱ�Է���������ĩλ�ַ����룬��˸���8bit���롣
//!
// �������ŵĲ�������(���������Ļ���Ӣ�ģ����������ŵ���󳤶�Ϊ
// MAX_LEN_CNC_SMS*MAX_NUM_CNC_SMS����˵������������ɷ���
// MAX_LEN_CNC_SMS*MAX_NUM_CNC_SMS*8/7��Ӣ���ַ����Զ�ɷ���
// MAX_LEN_CNC_SMS*MAX_NUM_CNC_SMS/2�������ַ�)
//!
// �����������ſɷ��͵���������ַ���(����USC2���룬ÿ������ռ��2�ֽ�)
#define MAX_CHAR_CNC_SMS_CN					67
// �����������ſɷ��͵����Ӣ���ַ���(����8bit���룬ÿ����ĸռ��1�ֽ�)
#define MAX_CHAR_CNC_SMS_EN					(MAX_CHAR_CNC_SMS_CN*2)
// ÿ���������ŵ���󳤶�
#define MAX_LEN_CNC_SMS						(MAX_CHAR_CNC_SMS_CN*2)
// ϵͳ֧�ֵĵ����������������(������RAM�ռ�)
#define MAX_NUM_CNC_SMS						4
//!
// +CMGR:"REC UNREAD","13430981508","","2012/09/08 16:30:08+32"
#define MAX_LEN_SMS_HEAD					60
// ���ͳ�����ʱUDH�Ĺ̶�����
#define DEF_LEN_SMS_UDH						7
// RSSI=15����ʱ�����ŷ���ʱ��ԼΪ8s
#define TO_SMS_TX							10

// GPRS
// GPRS apn�����й��ƶ�Ϊ"cmnet"
#define MAX_LEN_GSM_APN						64
// ip��ַ����󳤶ȣ���"128.128.128.128"
#define MAX_LEN_GPRS_IP						20
// ��������󳤶ȣ���"www.google.com"
#define MAX_LEN_GPRS_DN						32
// �˿ڵ���󳤶ȣ���"8080"
#define MAX_LEN_GPRS_PORT					8
// ��ָͨ��ĳ�ʱʱ��
#define TO_GPRS_GPCMD						3
// ��ָͨ��ĳ�ʱʱ��(����OK)
#define TO_GPRS_RT_OK						2
// ��������
#define TO_GPRS_DO_DNS						10
// ����APN
#define TO_GPRS_SET_APN						2
// ����ģ���ʱ��
#define TO_GPRS_FIND_MODULE					15
// ����GPRS����
#define TO_GPRS_SETUP_CONNECTION			19
// �Ͽ�GPRS����
#define TO_GPRS_CLOSE_CONNECTION			2
// ��͸��ģʽ
#define TO_GPRS_TRANSPARENT_ENTER			2
// �ر�͸��ģʽ
#define TO_GPRS_TRANSPARENT_EXIT			2
// ����һ֡����
#define TO_GPRS_TX_FRAME					3
// ����һ֡����(ʵ����Ե�֡��󳤶�Ϊ1500�ֽ�)
#define TO_GPRS_RX_FRAME					15
// ����gsmģ��̼��ĳ�ʱʱ��
#define TO_GSM_DOWNLOAD_MODE				120
// Զ�����س����ļ��ĳ�ʱʱ��
#define TO_GET_IMAGE_VIA_TCP				90
// GSMģ����յ�GPRS���ݺ�Ӵ�������ĵ������ݰ���󳤶�
//�����ͷ�Ӧ�����ݷָ���ٷ��ͣ�������շ����ճ���
//������Ϊ1500�ֽڣ�������ʱ���1500ʱ����쳣��
// "data length over 1500 bytes,exception!"��
#define MAX_LEN_GPRS_FRAMERX				1428
// ����AT+SOCSEND�����GPRS����֡ʱ������ط�����
// (��Ϊ1�Σ����ⷢ�ͳ�����ʱĳ֡����ʧ�ܺ��ط����½��շ��ظ����ո�֡)
#define MAX_TIMES_GPRS_SEND_PACKET			1
// ����ָ������(������)��������Դ���
#define MAX_TIMES_GPRS_SEND_DATA			1
// ����GPRS���ӵ�������Դ���
#define MAX_TIMES_GPRS_SETUP_CONNECTION		2
// ��ѯGSM��վ��Ϣ��������
#define MAX_TIMES_GSM_GET_CELLID			2
// ȷ��rssi�Ƿ�����ʱ��ѯrssi��������
#define MAX_TIEMS_GSM_GET_RSSI				2
// �����ָ�gsm��������
#define MAX_TIMES_GSM_RECOVER				3
// AT+SOCSEND����η��͵�������ݳ��ȣ�MMA3328��496�ֽڣ�
#define MAX_LEN_GPRS_PACKET_SEND			496

// GPRS ID
// ϵͳ���õ�gprs��������ַ(GSMģ��֧�����3��GPRS��������)
#define MAX_NUM_TCP_CONN					4
//!@{
//! Version-STD
#define	GPRS_ID0							0
#define	GPRS_ID1							1
#define	GPRS_ID2							2
//! Version-PRO
#define	GPRS_ID_MAPSERVER					0
#define	GPRS_ID_APPSERVER					1
#define GPRS_ID_EPHSERVER					2
//!@}

// dtmf
// ��dtmf�����û�����dtmf����ĳ�ʱʱ��
#define TO_DTMF_INPUT_PASSWORD				15
// dtmf����������ȷ�󵽹ر���ͷ����֮����ӳ�ʱ��
#define DELAY_SET_MICMUTE_OFF				1
// dtmf�ַ�������󳤶�(�������������)
#define MAX_LEN_GSM_DTMF					64

// cell-id
// AT+POSI��ѯ������վ����
#define MAX_NUM_GSM_CELL					7
// GSM��վ��Ϣ����������
#define MAX_NUM_GSM_CELL_DMN				7
// GSM��վ��Ϣ�����󳤶�
#define MAX_LEN_GSM_CELL_DMN				6

// server
#define MAX_LEN_EPHSERVER_UASER				32
#define MAX_LEN_EPHSERVER_PASSWORD			16

//!
//!@{
// 30s
#define DEF_PERIOD_CHECK_GSM_MODULE			30

// ����ʱ�Զ���ͨ������Ҫ�ﵽ���������
#define DEF_TIMES_RING_TO_ANSWER_CALL		1
// �����ͨ����������dtmf���������������
#define MAX_TIMES_INPUT_DTMF_COMMAND		3

// �ȴ�����
#define STS_DTMF_COMMAND_CALL_WAITING		0
// ���յ�����
#define STS_DTMF_COMMAND_CALL_INCOMING		1
// �����ͨ
#define STS_DTMF_COMMAND_CALL_ACCEPTED		2
// ����ͷ����
#define STS_DTMF_COMMAND_MIC_MUTE_ON		3
// dtmf����У��ͨ��
#define STS_DTMF_COMMAND_AUTHORIZED			4
// ��ͷ�����ر�
#define STS_DTMF_COMMAND_MIC_MUTE_OFF		5
//!@}

//!
//! @{
/*******************************************************************************
									�û��������
*******************************************************************************/
// ���ڲ���Ŀ�ĵĶ��Ž����ֻ����룬һ��Ϊ�û��ֻ�����
#define TEST_PHONE_NUMBER					"18072868677" 		//"15824400095"

// �����й��ƶ���������APN������cmnet��cnwap
#define GPRS_APN							"cmnet"
#define GSM_TELECODE						"86"

// ϵͳ֧�ֵ�����GPRS���Ӷ�Ӧ������IP��ַ�Ͷ˿ں�(Ĭ��ʹ��GPRS_ID0�������ò�������ΪTCP�˿�)
// ��ͼ�����������Ӳ���(TBD)
#define GPRS_ID0_DN							"zjuee.net"
#define GPRS_ID0_IP							""	//"210.83.80.208"
#define GPRS_ID0_PORT						"2502"

// Ӧ�÷����������Ӳ���
#define GPRS_ID1_DN							""
#define GPRS_ID1_IP							""
#define GPRS_ID1_PORT						""

// ���������������Ӳ���
#define GPRS_ID2_IP							""
#define GPRS_ID2_PORT						""
//! @}
/*============================ TYPES =========================================*/
//!
namespace periph_c
{
    class USARTx_PUTCHAR
    {
    public:
        static const u32 TMR_FREQ = 3;
        static const u32 TMR_RATE = 10;// 1: 1s, 2: 500ms, 10: 100ms
        static finline void Send(char ch)
        {
            COM3.PutChar(ch);
        }
        static void TimoutInit(u32 time)
        {
            // Min:1100
            int tmr_frequency = TMR_FREQ * 1000;
            // time:Max = 0xFFFF (tmr_period)
            TIMER7.Config(tmr_frequency, TMR_FREQ * time / TMR_RATE);
            TIMER7.EnableAutoReload(ENABLE);
            TIMER7.ClearUpdate();
            TIMER7.UpdateInterrupt(DISABLE);
            TIMER7.EnableCounter(DISABLE);
        }
        static void TimoutEnable(FunctionalState e = ENABLE)
        {
            TIMER7.ClearUpdate();
            TIMER7.UpdateInterrupt(e);
            TIMER7.SetCounter(0);
            TIMER7.EnableCounter(e);
        }
    };
}
//!
namespace device
{
    enum DEV_GSM_SMS_Mode
    {
        GSM_SMS_MODE_TXT					= 1,

        GSM_SMS_MODE_PDU					= 0,
    };
    enum DEV_GPRS_Mode
    {
        GSM_GPRS_MODE_UDP					= 1,

        GSM_GPRS_MODE_TCP					= 0,
    };
    typedef enum
    {
        GSM_PWR_N = 0,
        GSM_ONOFF_N = 1,
        GSM_DTR_N = 2,
        GPS_PWR_N = 3,
        GSM_RING_N = 4,

    } gsm_n_t;
    enum DEV_GSM_ERR
    {
        // error code
        // ���������(��GSMģ���޹�)
        // �����ģʽ�ַ�������Ϊ��
        ER_GSM_PATTERN_NULL					= 0,

        ER_GSM_PN_TOOLONG					= -1,
        ER_GSM_PN_TOOSHORT					= -2,
        // pn�г��ַ������ַ�
        ER_GSM_PN_CHAOS						= -3,

        ER_GSM_SMS_PDU_CHAOS				= -4,

        ER_GSM_SMS_FAILED					= -5,

        // gprs����ʧ��
        ER_GSM_GPRS_LINK_ERROR  			= -7,
        // GPRS����ʧ��
        ER_GSM_GPRS_SEND_ERROR  			= -8,
        // GPRS����ʧ��
        ER_GSM_GPRS_RECV_ERROR  			= -9,

        // �ӿ���ش���(һ����GSMģ���������)
        // GSM���������(һ��Ϊ����Ӳ������)
        ER_GSM_UART_RECV_NOTHING			= -10,
        // GSM���ڽ��ճ�ʱ(�������������ָ��ʱ���ڽ��ղ����������ַ����ַ���)
        ER_GSM_UART_RECV_TIMEOUT			= -11,
        // GSM���ڽ��յ�����
        ER_GSM_UART_RECV_CHAOS				= -12,

        // ������ش���(������GSMģ��������������)
        ER_GSM_NETWORK_UNREGISTERED			= -13,
        // GSM�ָ�ʧ��
        ER_GSM_RECOVER_FAILURE				= -14,
        // RSSI = 99
        ER_GSM_RSSI_UNKNOW					= -15,
        // RSSI < 4
        ER_GSM_RSSI_TOOLOW					= -16,

        // gprs���ӳ�ʱ
        ER_GSM_GPRS_LINK_TIMEOUT			= -17,
        // GPRS���ͳ�ʱ
        ER_GSM_GPRS_SEND_TIMEOUT			= -18,
        // GPRS���ճ�ʱ
        ER_GSM_GPRS_RECV_TIMEOUT			= -19,

        // GSM��ʼ������
        // SIM��Ϊ��⵽
        ER_GSM_INIT_SIM 					= -20,
        // ע������ʧ��
        ER_GSM_INIT_REGISTRATION			= -21,
        // ��ѯ�̼��汾ʧ��
        ER_GSM_INIT_SWVERSION				= -22,
        // ��ѯimeiʧ��
        ER_GSM_INIT_IMEI1					= -23,
        // ��ѯ�õ���imei���Ȳ���
        ER_GSM_INIT_IMEI2					= -24,
        // ����smsģʽΪtextʧ��
        ER_GSM_INIT_SMS_MODE				= -25,
        // ����smsflag-ringʧ��
        ER_GSM_INIT_SMS_SETTING				= -26,
        // ɾ��smsʧ��
        ER_GSM_INIT_SMS_DELETE				= -27,
        // ������������Զ����ʧ��
        ER_GSM_INIT_CLIP					= -28,
        // ����tcp���ݽ��մ���ʧ��
        ER_GSM_INIT_DATAFORMAT				= -29,
        // ��ѯrssiʧ��
        ER_GSM_INIT_RSSI					= -30,
        // ��ѯģ��ipʧ��
        ER_GSM_INIT_MDIP					= -31,

        // ����:��ʧ����ֵ"OK"
        ER_GSM_AT_MISS_OK					= -51,

        // ����:ATδ׼����
        ER_GSM_AT_NOT_RDY					= -52,

        // δ֪����
        ER_GSM_ERROR_UNKNOW					= -100,
    };

    //�¼���־ö����
    typedef enum
    {
        EV_READY,                   /*!< Startup finished. */
        EV_FRAME_RECEIVED,          /*!< Frame received. */
        EV_EXECUTE,                 /*!< Execute function. */
        EV_FRAME_SENT               /*!< Frame sent. */
    } eMBEventType;


    //״̬��־ö����
    typedef enum
    {
        // ����:û��׼����,���ܽ���ATָ��
        ER_GSM_STS_NOT_RDY					= 1,
        // ����:����ʶ���״̬
        ER_GSM_STS_UNKNOW					= 2,
        // ����:�绰����(����)
        ER_GSM_STS_CALLING					= 3,
        // ����:���Խ���ATָ��,��������ͨ����
        ER_GSM_STS_CALL						= 4,
        // ����:�͹���ģʽ,������������ATָ��
        ER_GSM_STS_STANDBY					= 5,
    } eGSMStateType;

    // ���ŵ���������
    typedef enum
    {
        SMS_LANGUAGE_ENGLISH = 0,
        SMS_LANGUAGE_CHINESE = 1
    } T_SMS_LANGUAGE;
    //! @}

    // ����GPRS���������Ľṹ��
    typedef struct
    {
        char	dn[MAX_LEN_GPRS_DN + 1];			// Domain Name
        char	ip[MAX_LEN_GPRS_IP + 1];			// IP Address
        char	port[MAX_LEN_GPRS_PORT + 1];		// Port
    } T_GPRS_CONNECTION;
    //! @}

    // GSM��վ��Ϣ
    typedef struct
    {
        char	mcc[16];
        char	mnc[16];
        char	lac[16];
        char	cellid[16];
        char	RSSI[4];
    } T_INFO_GSMCELL;
    //! @}

    // for future extension
    // WIFI AP��Ϣ
    typedef struct
    {
        char	bssid[18];
        char	RSSI[6];
    } T_INFO_WIFIAP;
    //! @}

    typedef struct
    {
        // SCA
        unsigned char	sca_len;		// number of octets represent sca_fmt AND sca_str
        unsigned char	sca_fmt;		// normally fixed to 0x91 if SCA specified explicitly
        char			sca_str[20];	// encoded destination address(invert withi a pair and attach 'F' if neccessary)
        // Note: SCA phone number MUST be with 86 prefix if internal numbering format set
        // MTI and friends			   	// set to 0x11 normally for SMS-Submit
        unsigned char	tp_mti;			// Message Type indicator
        unsigned char	tp_rd;			// Reject Duplicates
        unsigned char	tp_vpf;			// Validity Period Format
        unsigned char	tp_srr;			// Status Report Request
        unsigned char	tp_udhi;			// User Data header Indicator: 0 - without UDH, 1 - with UDH
        unsigned char	tp_rp;			// Reply Path

        // MR
        unsigned char	tp_mr;			// Message reference field

        // DA
        unsigned char	tp_dal;			// number of decimal digits represent RAW destination address(exclude tp_dat and attached 'F')
        unsigned char	tp_dat;			// normally fixed to 0x91, which implies international numbering format used
        unsigned char	tp_das[20];		// encoded destination address(invert withi a pair and attach 'F' if neccessary)
        // Note: destination phone number MUST be with 86 prefix if internal numbering format set
        // PDI
        unsigned char	tp_pid;			// Protocol identifier, set to 0x00 normally

        // DCS
        unsigned char	tp_dcs;			// Data coding scheme: , set to 0x00 if 7-bit encoding used, to 0x08 if UCS2 encoding used

        // VP
        unsigned char	tp_vp;			// Validity period, set to 0x00 normally which implies 5min

        // User Data
        unsigned char	tp_udl;			// bytes of RAW User Data(not encoded User Data)
        unsigned char	tp_uds[160];	// User Data string, can be up to 140 octets
        // Note: User Data is sent to MMA3328 module via UART in ASCII format, which implies a unsigned char
        // occupies 2 char space after conversion
    } T_SMS_SUBMIT;	// SMS format definition while sending

    typedef struct
    {
        unsigned char	udh_cr;			// SM concatenation 8bit ref.
        unsigned char	udh_eb;			// Bytes Information Element
        unsigned char	udh_sr;			// SM reference number
        unsigned char	udh_nm;			// number of messages
        unsigned char	udh_sn;			// this SM sequence number
    } T_SMS_DELIVER_UDH;				// User Data Header, used in concatenation SMS

    typedef struct
    {
        unsigned char	sca_len;		// SCA���볤��(���������ʽ�ֽ�)
        unsigned char	sca_fmt;		// SCA�����ʽ
        char			sca_str[20];	// SCA����

        unsigned char	tp_mti;			// Message Type indicator
        unsigned char	tp_mms;			// More Messages to Send
        unsigned char	tp_sri;			// Status Report Indication
        unsigned char	tp_udhi;			// User Data header Indicator: 0 - without UDH, 1 - with UDH
        unsigned char	tp_rp;			// Reply Path

        unsigned char	tp_oal;			// Originating Address length
        unsigned char	tp_oat;			// Originating Address type
        //unsigned char tp_contry;  	// ���Ҵ���
        char			tp_oas[20];		// Originating Address string

        unsigned char	tp_pid;			// Protocol identifier

        unsigned char	tp_dcs;			// Data coding scheme

        char			tp_scts[16];	// The service center time stamp

        unsigned char	tp_udl;			// User data length
        unsigned char	tp_udhl;		// User Data Header length
        unsigned char	tp_udh[32];		// user Data Header
        unsigned char	tp_uds[281];	// User Data, can be up to 140 octets, but in ASCII encoding in MMA3328 output.

    } T_SMS_DELIVER;	// SMS format definition while receiving

    class DEV_GSM_Struct
    {
        Func_Delay		__DEV_Delay;
        Func_Systick	__DEV_SysTick;
        u32				__SMS_READY;
        u32				__DEV_StartupTime;
    public:
        T_GPRS_CONNECTION tcp_conn[MAX_NUM_TCP_CONN];	// �豸Ҫ���ʵ�tcp��������������
        char	gsm_telecode[MAX_LEN_GSM_TELECODE + 1];	// ����PDU����ʱ��Ҫ�绰����Ĺ���/��������(���й�Ϊ86��+�Ǳ���)
        char                   gsm_sca[MAX_LEN_PN + 1];	// ��������
        char              gsm_apn[MAX_LEN_GSM_APN + 1];	// GSM APN
        char de_gsm_cell[MAX_NUM_GSM_CELL][MAX_NUM_GSM_CELL_DMN][MAX_LEN_GSM_CELL_DMN + 1];
        // ���ڱ���GSM��վ��Ϣ��ȫ������(���7����վ��
        //!ÿ����վ�̶�7����ÿ����ĳ��Ȳ�����6�ֽ�)
        char               de_gsm_pnin[MAX_LEN_PN + 1];	// ��ǰ��⵽�����������Ϣ
        char         de_gsm_dtmf[MAX_LEN_GSM_DTMF + 1];	// ��ǰ��⵽��DTMF��Ϣ
        char            gsm_imei[STD_LEN_GSM_IMEI + 1];	// GSMģ���IMEI��
        char          gsm_iccid[STD_LEN_GSM_ICCID + 1];	// SIM����ICCID(���ɵ�·��ʶ����)
        STATUS                         sts_gsm_power;
        bool		                    is_gsm_ready;	// GSMģ���Ƿ�������ʼ���ı�־
        bool                          is_gsm_calling;	// gsm�Ƿ���ͨ��״̬(ͨ��״̬�²��ܽ���tcp���ӣ�Ҳ���ܷ���tcp����)
        bool		              is_gsmring_pending;	// gsmģ���ring�ж��¼��Ƿ������
        int			            cnt_gsmring_asserted;	// GSMģ��RING�жϲ����Ĵ���(GSMģ���ϵ����Զ�����һ��RING�ж�)
        int			               cnt_gsm_recovered;	// GSMģ����ϻָ��Ĵ���
        // dtmf related
        int			              swtimer_input_dtmf;	// dtmf���򿪺�����û�����dtmf����ĳ�ʱ��ⶨʱ��
        int			         swtimer_set_micmute_off;	// �û�����dtmf������ȷ��ر���ͷ�������ӳٶ�ʱ��
        bool		       is_dtmf_detection_enabled;	// ��ǰDTMF����Ƿ��
        STATUS		                sts_dtmf_command;	// DTMF�����ĵ�ǰ״̬
        char	   dtmf_password[MAX_LEN_GSM_DTMF + 1];	// DTMF��������
        int			       times_input_dtmf_password;	// �Ѿ�����DTMF��������Ĵ���
        char pn_operation[MAX_PN_OPERATON][MAX_LEN_PN + 1];
        // ��Ȩ����+Ȩ���б�
        char	   cmd_pwd_sms[STD_LEN_OF_CMD_PWD + 1];	// ���������������
        char	  cmd_pwd_dtmf[STD_LEN_OF_CMD_PWD + 1];	// ���������������
    public:
        bool 	DEV_GSM_Init(Func_Delay func1, Func_Systick func2, int baudrate, int &err);
        void   	DEV_GSM_Exit(bool param = false);
        void   	DEV_GSM_PowerUp(void);
        void   	DEV_GSM_PowerDown(void);
        void   	DEV_GSM_Wakeup(void);
        void   	DEV_GSM_Sleep(void);
        void   	DEV_GSM_OnOff(void);
        //!
        bool	DEV_SendAT(char *at, char *ptn, int to, int &pos, int &err, bool ok = false);
        bool	DEV_FindPattern(char *ptn, int to, int &pos, int &err);
        bool	DEV_FindFlag(char ptn, int to, int &err);
        bool	DEV_FetchPattern(char *ptn, int to, int &pos, int &err);
        bool	DEV_FetchString(char *ptn, char *str, int len, int to, int &pos);
        bool	DEV_FetchHex(char *ptn, char *str, int &len, int to, int &pos);
        bool	DEV_FetchValue(char *ptn, int *val, int to, int &pos);
        void	DEV_SEND_DATA(char *str, int size);
        int		DEV_FormatPN(char *pn);
        //!
        bool	GSM_CheckBaudRate(int br);
        bool	GSM_CheckSIM(void);
        bool	GSM_CheckNetwork(void);
        bool	GSM_CheckGPRSActive(void);
        bool	GSM_GetVender(char *vender, int len);
        bool	GSM_GetSWVersion(char *version, int len);
        bool 	GSM_GetIMEI(char *imei, int len);
        bool	GSM_GetState(int *state);
        bool 	GSM_GetICCID(char *iccid, int len);
        bool	GSM_GetRSSI(int *rssi);
        bool	GSM_GetCCLK(char *cclk, int len);
        bool	GSM_GetIP(char *ip, int len);
        bool	GSM_GetDNS(char *ip, int len_ip, char *dn);
        bool	GSM_SetGPRSAPN(void);
        bool	GSM_SetBaudRate(int br);
        bool	GSM_SetSMSMode(enum DEV_GSM_SMS_Mode mode);
        bool	GSM_SMS_PDUConstruct(char *pdu, char *pn, char *sca, unsigned char dcs,
                                     unsigned char *encoded, unsigned int size,
                                     unsigned char udhi, unsigned char *udh, unsigned int len_udh);
		// 		��ʮ�������ַ�����ʽ��pdu�����벢���ؽ����������ݵ��ַ���(�ַ�����Ϊascii�����ucs2����)��
        bool	GSM_SMS_PDUDeConstruct(T_SMS_DELIVER * deliver, char* asc);
        bool	GSM_SMS_SendPDU(T_SMS_LANGUAGE language, char *pns, char *ascii_utf8, int &err);
		bool	GSM_SendSMS(char* pdu, int len, int &err);
        bool	GPRS_LINK_Status(void);
        bool	GPRS_SOC_Status(int id,
                                enum DEV_GPRS_Mode mode = GSM_GPRS_MODE_TCP);
        bool	GPRS_LINK_Setup(void);
        bool	GPRS_SOC_Setup(int id,
                               enum DEV_GPRS_Mode mode = GSM_GPRS_MODE_TCP);
        bool	GPRS_LINK_Open(void);
        bool	GPRS_SOC_Open(int id,
                              enum DEV_GPRS_Mode mode = GSM_GPRS_MODE_TCP);
        bool	GPRS_LINK_Close(void);
        bool	GPRS_SOC_Close(int id,
                               enum DEV_GPRS_Mode mode = GSM_GPRS_MODE_TCP);
        bool	GPRS_SOC_TX(int id, char *data, int size, int &pos, int &err,
                            enum DEV_GPRS_Mode mode = GSM_GPRS_MODE_TCP);
        bool	GPRS_SOC_RX(int id, char *data, int &rlen, int to, int &err,
                            enum DEV_GPRS_Mode mode = GSM_GPRS_MODE_TCP);
        bool	GSM_SendData(int id, char *data, int size, int &err,
                             enum DEV_GPRS_Mode mode = GSM_GPRS_MODE_TCP);
        bool	GSM_SendSMS(char *pn, char *ascii_utf8, int &err);
        bool	GSM_ConnnectServerExample(char *data, int len, char *recv, int &rlen, int &err,
                                          enum DEV_GPRS_Mode mode = GSM_GPRS_MODE_TCP);
        //!
        finline void DEV_GSM_Delay(u32 time)
        {
            if (__DEV_Delay)
            {
                __DEV_Delay(time);
            }
        }
        finline vu32 DEV_GSM_Timing(void)
        {
            if (__DEV_SysTick)
                return __DEV_SysTick();
            return 0;
        }
        finline bool WaitForSMSReady(u32 delta)
        {
            return DEV_GSM_Timing() > __DEV_StartupTime + delta;
        }
    };
}
/*============================ INTERFACE =====================================*/
using namespace device;
/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx

/*============================ EXTERN FUNCTIONS ==============================*/
//#ifdef __cplusplus
//}
//#endif

#endif /* DEV_GSM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

