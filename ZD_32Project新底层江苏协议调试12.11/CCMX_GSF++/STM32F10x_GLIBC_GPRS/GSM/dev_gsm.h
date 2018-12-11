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
									全局宏定义
*******************************************************************************/
//!
#define MAX_DECIMAL_DIGITS_FOR_INT			10	// 32位int型数据能表示的最大十进制数的位数
//!
#define MAX_LEN_PN							32
#define STD_LEN_PN							11	// 中国地区手机号码标准长度为11位数字
//!
#define MAX_PN_OPERATON						4	// 系统允许设定的最多授权号码数量
#define STD_LEN_OF_CMD_PWD					6	// 操作密码固定为6个字符长度	
//!
#define DEF_CMD_PWD_SMS			     "123456"
#define DEF_CMD_PWD_DTMF		    "999999#"	// 数字命令密码需带命令结尾符号#
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
#define MAX_LEN_GSM_TELECODE				8		// 国家代码的最大长度
#define STD_LEN_GSM_IMEI					15		// GSM imei的标准长度
#define STD_LEN_GSM_ICCID					20		// GSM ICCID的标准长度
#define TO_GSM_REGISTRATION					20		// GSM上电后注册到网络的超时时间(一般为10秒左右)
#define MIN_RSSI_FOR_COMMUNICATION			5		// 维持GSM正常通讯所需的最低RSSI值

//!sms global
// SMS发送方发送的时间，包含年月日时分秒以及时区信息
#define MAX_LEN_SMS_TXTIME					32
// PDU SMS 最大长度（M660+/M660_1120_D7S13000_V009: 140字节，
// 但是根据GSM协议单条短信最大长度为144字节，中国移动/中国联通：70个汉字）
#define MAX_BYTE_SMS_PDU					140
// TXT SMS 最大长度（M660+/M660_1120_D7S13000_V009: 159字节，
// 但是根据GSM协议单条短信最大长度为144字节，中国移动/中国联通：70个汉字）
#define MAX_BYTE_SMS_TXT					140
// 由于采用7bit编码发送英文短信时对方解码会出现末位字符乱码，因此改用8bit编码。
//!
// 级联短信的参数设置(不管是中文还是英文，单条长短信的最大长度为
// MAX_LEN_CNC_SMS*MAX_NUM_CNC_SMS，因此单条长短信最多可发送
// MAX_LEN_CNC_SMS*MAX_NUM_CNC_SMS*8/7个英文字符、对多可发送
// MAX_LEN_CNC_SMS*MAX_NUM_CNC_SMS/2个中文字符)
//!
// 单条级联短信可发送的最大中文字符数(采用USC2编码，每个汉字占用2字节)
#define MAX_CHAR_CNC_SMS_CN					67
// 单条级联短信可发送的最大英文字符数(采用8bit编码，每个字母占用1字节)
#define MAX_CHAR_CNC_SMS_EN					(MAX_CHAR_CNC_SMS_CN*2)
// 每条级联短信的最大长度
#define MAX_LEN_CNC_SMS						(MAX_CHAR_CNC_SMS_CN*2)
// 系统支持的单条长短信最大级联数(受限于RAM空间)
#define MAX_NUM_CNC_SMS						4
//!
// +CMGR:"REC UNREAD","13430981508","","2012/09/08 16:30:08+32"
#define MAX_LEN_SMS_HEAD					60
// 发送长短信时UDH的固定长度
#define DEF_LEN_SMS_UDH						7
// RSSI=15左右时，短信发送时间约为8s
#define TO_SMS_TX							10

// GPRS
// GPRS apn，如中国移动为"cmnet"
#define MAX_LEN_GSM_APN						64
// ip地址的最大长度，如"128.128.128.128"
#define MAX_LEN_GPRS_IP						20
// 域名的最大长度，如"www.google.com"
#define MAX_LEN_GPRS_DN						32
// 端口的最大长度，如"8080"
#define MAX_LEN_GPRS_PORT					8
// 普通指令的超时时间
#define TO_GPRS_GPCMD						3
// 普通指令的超时时间(返回OK)
#define TO_GPRS_RT_OK						2
// 域名解析
#define TO_GPRS_DO_DNS						10
// 设置APN
#define TO_GPRS_SET_APN						2
// 搜索模块的时间
#define TO_GPRS_FIND_MODULE					15
// 建立GPRS连接
#define TO_GPRS_SETUP_CONNECTION			19
// 断开GPRS连接
#define TO_GPRS_CLOSE_CONNECTION			2
// 打开透传模式
#define TO_GPRS_TRANSPARENT_ENTER			2
// 关闭透传模式
#define TO_GPRS_TRANSPARENT_EXIT			2
// 发送一帧数据
#define TO_GPRS_TX_FRAME					3
// 接收一帧数据(实验测试单帧最大长度为1500字节)
#define TO_GPRS_RX_FRAME					15
// 更新gsm模块固件的超时时间
#define TO_GSM_DOWNLOAD_MODE				120
// 远程下载程序文件的超时时间
#define TO_GET_IMAGE_VIA_TCP				90
// GSM模块接收到GPRS数据后从串口输出的单个数据包最大长度
//（发送方应将数据分割后再发送，以免接收方接收出错）
//（极限为1500字节，但是有时输出1500时会出异常：
// "data length over 1500 bytes,exception!"）
#define MAX_LEN_GPRS_FRAMERX				1428
// 采用AT+SOCSEND命令发送GPRS数据帧时的最大重发次数
// (设为1次，以免发送长数据时某帧发送失败后重发导致接收方重复接收该帧)
#define MAX_TIMES_GPRS_SEND_PACKET			1
// 发送指定数据(不定长)的最大重试次数
#define MAX_TIMES_GPRS_SEND_DATA			1
// 建立GPRS连接的最大重试次数
#define MAX_TIMES_GPRS_SETUP_CONNECTION		2
// 查询GSM基站信息的最大次数
#define MAX_TIMES_GSM_GET_CELLID			2
// 确认rssi是否正常时查询rssi的最大次数
#define MAX_TIEMS_GSM_GET_RSSI				2
// 连续恢复gsm的最大次数
#define MAX_TIMES_GSM_RECOVER				3
// AT+SOCSEND命令单次发送的最大数据长度（MMA3328：496字节）
#define MAX_LEN_GPRS_PACKET_SEND			496

// GPRS ID
// 系统内置的gprs服务器地址(GSM模块支持最多3个GPRS并发连接)
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
// 打开dtmf检测后用户输入dtmf命令的超时时间
#define TO_DTMF_INPUT_PASSWORD				15
// dtmf密码输入正确后到关闭咪头静音之间的延迟时间
#define DELAY_SET_MICMUTE_OFF				1
// dtmf字符串的最大长度(包括密码和命令)
#define MAX_LEN_GSM_DTMF					64

// cell-id
// AT+POSI查询的最多基站数量
#define MAX_NUM_GSM_CELL					7
// GSM基站信息的最大域个数
#define MAX_NUM_GSM_CELL_DMN				7
// GSM基站信息域的最大长度
#define MAX_LEN_GSM_CELL_DMN				6

// server
#define MAX_LEN_EPHSERVER_UASER				32
#define MAX_LEN_EPHSERVER_PASSWORD			16

//!
//!@{
// 30s
#define DEF_PERIOD_CHECK_GSM_MODULE			30

// 来电时自动接通来电需要达到的振铃次数
#define DEF_TIMES_RING_TO_ANSWER_CALL		1
// 来电接通后，允许输入dtmf命令密码的最多次数
#define MAX_TIMES_INPUT_DTMF_COMMAND		3

// 等带来电
#define STS_DTMF_COMMAND_CALL_WAITING		0
// 接收到来电
#define STS_DTMF_COMMAND_CALL_INCOMING		1
// 来电接通
#define STS_DTMF_COMMAND_CALL_ACCEPTED		2
// 打开咪头静音
#define STS_DTMF_COMMAND_MIC_MUTE_ON		3
// dtmf密码校验通过
#define STS_DTMF_COMMAND_AUTHORIZED			4
// 咪头静音关闭
#define STS_DTMF_COMMAND_MIC_MUTE_OFF		5
//!@}

//!
//! @{
/*******************************************************************************
									用户定义参数
*******************************************************************************/
// 用于测试目的的短信接收手机号码，一般为用户手机号码
#define TEST_PHONE_NUMBER					"18072868677" 		//"15824400095"

// 对于中国移动上网卡，APN可以是cmnet或cnwap
#define GPRS_APN							"cmnet"
#define GSM_TELECODE						"86"

// 系统支持的三个GPRS连接对应服务器IP地址和端口后(默认使用GPRS_ID0连接配置参数，且为TCP端口)
// 地图服务器的连接参数(TBD)
#define GPRS_ID0_DN							"zjuee.net"
#define GPRS_ID0_IP							""	//"210.83.80.208"
#define GPRS_ID0_PORT						"2502"

// 应用服务器的连接参数
#define GPRS_ID1_DN							""
#define GPRS_ID1_IP							""
#define GPRS_ID1_PORT						""

// 星历服务器的连接参数
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
        // 纯软件错误(和GSM模块无关)
        // 输入的模式字符串参数为空
        ER_GSM_PATTERN_NULL					= 0,

        ER_GSM_PN_TOOLONG					= -1,
        ER_GSM_PN_TOOSHORT					= -2,
        // pn中出现非数字字符
        ER_GSM_PN_CHAOS						= -3,

        ER_GSM_SMS_PDU_CHAOS				= -4,

        ER_GSM_SMS_FAILED					= -5,

        // gprs连接失败
        ER_GSM_GPRS_LINK_ERROR  			= -7,
        // GPRS发送失败
        ER_GSM_GPRS_SEND_ERROR  			= -8,
        // GPRS接收失败
        ER_GSM_GPRS_RECV_ERROR  			= -9,

        // 接口相关错误(一般由GSM模块故障引起)
        // GSM串口无输出(一般为严重硬件错误)
        ER_GSM_UART_RECV_NOTHING			= -10,
        // GSM串口接收超时(串口有输出但在指定时间内接收不到期望的字符或字符串)
        ER_GSM_UART_RECV_TIMEOUT			= -11,
        // GSM串口接收到乱码
        ER_GSM_UART_RECV_CHAOS				= -12,

        // 网络相关错误(可能由GSM模块或网络故障引起)
        ER_GSM_NETWORK_UNREGISTERED			= -13,
        // GSM恢复失败
        ER_GSM_RECOVER_FAILURE				= -14,
        // RSSI = 99
        ER_GSM_RSSI_UNKNOW					= -15,
        // RSSI < 4
        ER_GSM_RSSI_TOOLOW					= -16,

        // gprs连接超时
        ER_GSM_GPRS_LINK_TIMEOUT			= -17,
        // GPRS发送超时
        ER_GSM_GPRS_SEND_TIMEOUT			= -18,
        // GPRS接收超时
        ER_GSM_GPRS_RECV_TIMEOUT			= -19,

        // GSM初始化错误
        // SIM卡为检测到
        ER_GSM_INIT_SIM 					= -20,
        // 注册网络失败
        ER_GSM_INIT_REGISTRATION			= -21,
        // 查询固件版本失败
        ER_GSM_INIT_SWVERSION				= -22,
        // 查询imei失败
        ER_GSM_INIT_IMEI1					= -23,
        // 查询得到的imei长度不对
        ER_GSM_INIT_IMEI2					= -24,
        // 设置sms模式为text失败
        ER_GSM_INIT_SMS_MODE				= -25,
        // 设置smsflag-ring失败
        ER_GSM_INIT_SMS_SETTING				= -26,
        // 删除sms失败
        ER_GSM_INIT_SMS_DELETE				= -27,
        // 设置来电号码自动输出失败
        ER_GSM_INIT_CLIP					= -28,
        // 设置tcp数据接收传输失败
        ER_GSM_INIT_DATAFORMAT				= -29,
        // 查询rssi失败
        ER_GSM_INIT_RSSI					= -30,
        // 查询模块ip失败
        ER_GSM_INIT_MDIP					= -31,

        // 错误:丢失返回值"OK"
        ER_GSM_AT_MISS_OK					= -51,

        // 错误:AT未准备好
        ER_GSM_AT_NOT_RDY					= -52,

        // 未知错误
        ER_GSM_ERROR_UNKNOW					= -100,
    };

    //事件标志枚举体
    typedef enum
    {
        EV_READY,                   /*!< Startup finished. */
        EV_FRAME_RECEIVED,          /*!< Frame received. */
        EV_EXECUTE,                 /*!< Execute function. */
        EV_FRAME_SENT               /*!< Frame sent. */
    } eMBEventType;


    //状态标志枚举体
    typedef enum
    {
        // 错误:没有准备好,不能接收AT指令
        ER_GSM_STS_NOT_RDY					= 1,
        // 错误:不能识别的状态
        ER_GSM_STS_UNKNOW					= 2,
        // 错误:电话呼入(响铃)
        ER_GSM_STS_CALLING					= 3,
        // 错误:可以接受AT指令,但是正在通话中
        ER_GSM_STS_CALL						= 4,
        // 错误:低功耗模式,不能正常接收AT指令
        ER_GSM_STS_STANDBY					= 5,
    } eGSMStateType;

    // 短信的语言类型
    typedef enum
    {
        SMS_LANGUAGE_ENGLISH = 0,
        SMS_LANGUAGE_CHINESE = 1
    } T_SMS_LANGUAGE;
    //! @}

    // 保存GPRS连网参数的结构体
    typedef struct
    {
        char	dn[MAX_LEN_GPRS_DN + 1];			// Domain Name
        char	ip[MAX_LEN_GPRS_IP + 1];			// IP Address
        char	port[MAX_LEN_GPRS_PORT + 1];		// Port
    } T_GPRS_CONNECTION;
    //! @}

    // GSM基站信息
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
    // WIFI AP信息
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
        unsigned char	sca_len;		// SCA号码长度(包括号码格式字节)
        unsigned char	sca_fmt;		// SCA号码格式
        char			sca_str[20];	// SCA号码

        unsigned char	tp_mti;			// Message Type indicator
        unsigned char	tp_mms;			// More Messages to Send
        unsigned char	tp_sri;			// Status Report Indication
        unsigned char	tp_udhi;			// User Data header Indicator: 0 - without UDH, 1 - with UDH
        unsigned char	tp_rp;			// Reply Path

        unsigned char	tp_oal;			// Originating Address length
        unsigned char	tp_oat;			// Originating Address type
        //unsigned char tp_contry;  	// 国家代码
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
        T_GPRS_CONNECTION tcp_conn[MAX_NUM_TCP_CONN];	// 设备要访问的tcp服务器参数数组
        char	gsm_telecode[MAX_LEN_GSM_TELECODE + 1];	// 发送PDU短信时需要电话号码的国家/地区区号(如中国为86，+非必需)
        char                   gsm_sca[MAX_LEN_PN + 1];	// 短信中心
        char              gsm_apn[MAX_LEN_GSM_APN + 1];	// GSM APN
        char de_gsm_cell[MAX_NUM_GSM_CELL][MAX_NUM_GSM_CELL_DMN][MAX_LEN_GSM_CELL_DMN + 1];
        // 用于保存GSM基站信息的全局数组(最多7个基站、
        //!每个基站固定7个域、每个域的长度不超过6字节)
        char               de_gsm_pnin[MAX_LEN_PN + 1];	// 当前检测到的来电号码信息
        char         de_gsm_dtmf[MAX_LEN_GSM_DTMF + 1];	// 当前检测到的DTMF信息
        char            gsm_imei[STD_LEN_GSM_IMEI + 1];	// GSM模块的IMEI串
        char          gsm_iccid[STD_LEN_GSM_ICCID + 1];	// SIM卡的ICCID(集成电路卡识别码)
        STATUS                         sts_gsm_power;
        bool		                    is_gsm_ready;	// GSM模块是否正常初始化的标志
        bool                          is_gsm_calling;	// gsm是否处于通话状态(通话状态下不能建立tcp连接，也不能发送tcp数据)
        bool		              is_gsmring_pending;	// gsm模块的ring中断事件是否待处理
        int			            cnt_gsmring_asserted;	// GSM模块RING中断产生的次数(GSM模块上电后会自动产生一个RING中断)
        int			               cnt_gsm_recovered;	// GSM模块故障恢复的次数
        // dtmf related
        int			              swtimer_input_dtmf;	// dtmf检测打开后接收用户输入dtmf密码的超时检测定时器
        int			         swtimer_set_micmute_off;	// 用户输入dtmf密码正确后关闭咪头静音的延迟定时器
        bool		       is_dtmf_detection_enabled;	// 当前DTMF检测是否打开
        STATUS		                sts_dtmf_command;	// DTMF命令处理的当前状态
        char	   dtmf_password[MAX_LEN_GSM_DTMF + 1];	// DTMF交互密码
        int			       times_input_dtmf_password;	// 已经输入DTMF交互密码的次数
        char pn_operation[MAX_PN_OPERATON][MAX_LEN_PN + 1];
        // 授权号码+权限列表
        char	   cmd_pwd_sms[STD_LEN_OF_CMD_PWD + 1];	// 短信命令操作密码
        char	  cmd_pwd_dtmf[STD_LEN_OF_CMD_PWD + 1];	// 数字命令操作密码
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
		// 		将十六进制字符串形式的pdu包解码并返回解码后短信内容的字符数(字符可能为ascii编码或ucs2编码)。
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

