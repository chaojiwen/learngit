/**
  ******************************************************************************
  * @file              : interface.h
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
#ifndef INTERFACE_H
#define INTERFACE_H

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdio.h>
#include "stm32glibc_type.h"
//! @}
	 
/*============================ MACROS ========================================*/
#ifdef USE_STM32100B_EVAL
#define MESSAGE1   "STM32 MD Value Line "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "  STM32100B-EVAL    "
#elif defined (USE_STM3210B_EVAL)
#define MESSAGE1   "STM32 Medium Density"
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "   STM3210B-EVAL    "
#elif defined (STM32F10X_XL) && defined (USE_STM3210E_EVAL)
#define MESSAGE1   "  STM32 XL Density  "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "   STM3210E-EVAL    "
#elif defined (USE_STM3210E_EVAL)
#define MESSAGE1   " STM32 High Density "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "   STM3210E-EVAL    "
#elif defined (USE_STM3210C_EVAL)
#define MESSAGE1   " STM32 Connectivity "
#define MESSAGE2   " Line Device running"
#define MESSAGE3   " on STM3210C-EVAL   "
#elif defined (USE_STM32100E_EVAL)
#define MESSAGE1   "STM32 HD Value Line "
#define MESSAGE2   " Device running on  "
#define MESSAGE3   "  STM32100E-EVAL    "
#endif



#if !defined(USE_STM3210E_EVAL)
//! Ctrl-Beep-LED
//#define BSP_BEEP_CH0			GPIO_Pin_12
//#define BEEPIO					IOPB
//#define BSP_LED_CH1				GPIO_Pin_6
//#define LEDIO1					IOPF
//#define BSP_LED_CH2				GPIO_Pin_7
//#define LEDIO2					IOPF
//#define BSP_LED_CH3				GPIO_Pin_8
//#define LEDIO3					IOPF
//#define BSP_LED_CH4				GPIO_Pin_9
//#define LEDIO4					IOPF
//#define BSP_LED_CH5				GPIO_Pin_10
//#define LEDIO5					IOPF
//! PWM
//#define BSP_GPIOB_CH3			GPIO_Pin_1
//#define BSP_GPIOB_CH4			GPIO_Pin_0
//#define BSP_GPIOF_PWM_GRP		BSP_GPIOB_CH3 | BSP_GPIOB_CH4
//! DEV-GSM
#define BSP_GSM_PWRCTRL			GPIO_Pin_1
#define GSMPWRCTRL				IOPB
//! COM1
#define BSP_COM1_TX				GPIO_Pin_9
#define BSP_COM1_RX				GPIO_Pin_10
#define COM1IO					IOPA
//!	GPIO_Remap_USART1			PB6 | PB7
#define COM1REMAP				GPIO_Remap_NULL
#define BSP_COM1_GRP			BSP_COM1_TX | BSP_COM1_RX
//! COM2
#define BSP_COM2_TX				GPIO_Pin_2
#define BSP_COM2_RX				GPIO_Pin_3
#define COM2IO					IOPA
//!	GPIO_Remap_USART2			PD5 | PD6
#define COM2REMAP				GPIO_Remap_NULL
#define BSP_COM2_GRP			BSP_COM2_TX | BSP_COM2_RX
//! COM3
#define BSP_COM3_TX				GPIO_Pin_10
#define BSP_COM3_RX				GPIO_Pin_11
#define COM3IO					IOPB
//!	GPIO_PartialRemap_USART3 	PC10 | PC11
//!	GPIO_FullRemap_USART3		PD8 | PD9
#define COM3REMAP				GPIO_Remap_NULL
#define BSP_COM3_GRP			BSP_COM3_TX | BSP_COM3_RX
//!*********************************************************
#else
//!*********************************************************
//! Ctrl-Beep-LED
//#define BSP_BEEP_CH0			GPIO_Pin_4
//#define BEEPIO					IOPC
//#define BSP_LED_CH1				GPIO_Pin_5
//#define LEDIO1					IOPC
//#define BSP_LED_CH2				GPIO_Pin_0
//#define LEDIO2					IOPB
//#define BSP_LED_CH3				GPIO_Pin_1
//#define LEDIO3					IOPB
//#define BSP_LED_CH4				GPIO_Pin_11
//#define LEDIO4					IOPE
//#define BSP_LED_CH5				GPIO_Pin_12
//#define LEDIO5					IOPE
////! PWM
//#define BSP_GPIOB_CH3			GPIO_Pin_1
//#define BSP_GPIOB_CH4			GPIO_Pin_0
//#define BSP_GPIOF_PWM_GRP		BSP_GPIOB_CH3 | BSP_GPIOB_CH4
//! I2C
#define BSP_I2C_SCL				GPIO_Pin_8
#define BSP_I2C_SDA				GPIO_Pin_9
#define I2CIO					IOPB
#define BSP_I2C_GRP				BSP_I2C_SCL | BSP_I2C_SDA
//! DEV-GSM
#define BSP_GSM_PWRCTRL			GPIO_Pin_1
#define GSMPWRCTRL				IOPB
#define BSP_GSM_ONOFF			GPIO_Pin_2
#define GSMONOFF				IOPB
#define BSP_GSM_WAKEUP			GPIO_Pin_9
#define GSMWAKEUP				IOPD
#define BSP_GSM_RING			GPIO_Pin_8
#define GSMRING					IOPD
#define BSP_PORTSRC_RING		GPIO_PortSourceGPIOD
#define BSP_PPINSRC_RING		GPIO_PinSource8
#define BSP_EXTILINE_RING		EXTI_Line8
//! COM1
#define BSP_COM1_TX				GPIO_Pin_9
#define BSP_COM1_RX				GPIO_Pin_10
#define COM1IO					IOPA
//!	GPIO_Remap_USART1			PB6 | PB7
#define COM1REMAP				GPIO_Remap_NULL
#define BSP_COM1_GRP			BSP_COM1_TX | BSP_COM1_RX
//! COM2
#define BSP_COM2_TX				GPIO_Pin_2
#define BSP_COM2_RX				GPIO_Pin_3
#define COM2IO					IOPA
//!	GPIO_Remap_USART2			PD5 | PD6
#define COM2REMAP				GPIO_Remap_NULL
#define BSP_COM2_GRP			BSP_COM2_TX | BSP_COM2_RX
//! COM3
#define BSP_COM3_TX				GPIO_Pin_10
#define BSP_COM3_RX				GPIO_Pin_11
#define COM3IO					IOPB
//!	GPIO_PartialRemap_USART3 	PC10 | PC11
//!	GPIO_FullRemap_USART3		PD8 | PD9
#define COM3REMAP				GPIO_Remap_NULL
#define BSP_COM3_GRP			BSP_COM3_TX | BSP_COM3_RX

// GPRS ID
// 系统内置的gprs服务器地址(GSM模块支持最多3个GPRS并发连接)
#define MAX_NUM_TCP_CONN					4

// ip地址的最大长度，如"128.128.128.128"
#define MAX_LEN_GPRS_IP						20
// 域名的最大长度，如"www.google.com"
#define MAX_LEN_GPRS_DN						32
// 端口的最大长度，如"8080"
#define MAX_LEN_GPRS_PORT					8

#endif

/*============================ TYPES =========================================*/

/**
  * @brief  Description "提供延时服务"
  * @param  time		单位ms
  * @retval None
  */
typedef void (*Func_Delay)(int time);

/**
  * @brief  Description "提供定时器的毫秒数"
  * @param  None
  * @retval 定时器的毫秒数
  */
typedef int (*Func_Systick)(void);

void Delay(int nTime);
/**
  * @brief  Description "RTC8025日历时钟寄存器"
  * @param  None
  * @retval 
  */
#define SYS_RTC_8025_BUF_SIZE	16
typedef union __rt8025_t
{
	struct __rt8025_regs
	{
		uint8_t 	Seconds:7;
		uint8_t		Resv1:1;
		uint8_t 	Minutes:7;
		uint8_t		Resv2:1;
		uint8_t 	Hours:6;
		uint8_t		Resv3:2;
		uint8_t 	Weekdays:3;
		uint8_t		Resv4:5;
		uint8_t 	Days:6;
		uint8_t		Resv5:2;
		uint8_t 	Months:5;
		uint8_t		Resv6:2;
		uint8_t		YearCarry:1;
		uint8_t 	Years;
		uint8_t 	DigitalOffset;
		uint8_t 	Alarm_W_Minute:7;
		uint8_t		Resv7:1;
		uint8_t 	Alarm_W_Hour:6;
		uint8_t		Resv8:2;
		uint8_t 	Alarm_W_day:7;
		uint8_t		Resv9:1;
		uint8_t 	Alarm_D_Minute:7;
		uint8_t		Resv10:1;
		uint8_t 	Alarm_D_Hour:6;
		uint8_t		Resv11:2;
		uint8_t		Resv12;
		uint16_t	Control;
	} STREG;
	char REGS[SYS_RTC_8025_BUF_SIZE];
} RTC8025_Typedef;

typedef union __rt8025_ctrl_t
{
	struct __rt8025_ctrl_regs
	{
		uint16_t 	CT:3;		// INTA interrupt periodic
		uint16_t	Resv1:1;	// TEST for manufacturer
		uint16_t 	CLEN2:1;	// FOUT en RST_VAL:0
		uint16_t	H12_24:1;	// 1:24-Hour Clock
		uint16_t 	DALE:1;		// Alarm D en
		uint16_t	WALE:1;		// Alarm W en
		uint16_t 	DAFG:1;		// Alarm D match occurs, INTA = "L"
		uint16_t	WAFG:1;		// Alarm W match occurs, INTB = "L"
		uint16_t 	CTFG:1;		// Priodic interrupt output, INTA
		uint16_t	CLEN1:1;	// FOUT en RST_VAL:0
		uint16_t 	PON:1;		// Power-On Reset Detection
		uint16_t	XST:1;		// Oscillation Stop Detection
		uint16_t	VDET:1;		// Power Drop Detection
		uint16_t 	VDSL:1;		// Set Power Drop Threshold Voltage
	} STREG;
	uint16_t Control;
} RTC8025_Ctrl_Typedef;

//* 时间数据结构体->类型 */
typedef struct 
{
  unsigned int  Year;
  unsigned char Month;
  unsigned char Day;
  unsigned char Hour;
  unsigned char Min;
  unsigned char Sec;
  unsigned char Week;
  char			Zone;
} RTC_Times; 

typedef struct
{
	char	dn[MAX_LEN_GPRS_DN + 1];			// Domain Name
	char	ip[MAX_LEN_GPRS_IP + 1];			// IP Address
	char	port[MAX_LEN_GPRS_PORT + 1];		// Port
}GPRS_Connection;

/* SIM卡状态结构 */
typedef enum 
{
	/* SIM正常 */
	_SIM_OK = 0x00,
	
	/* 无GPRS信号 */
	_SIM_NoSignal = 0X01,
	
	/* SIM未连接 */
	_SIM_NotDetected = 0x02,
	
	/* SIM欠费停机,也有可能是空号 */
	_SIM_ShutDown = 0x03,
	
}_SCSStruct;


/*============================ INTERFACE =====================================*/

/**
  * @brief  Description "调试信息输出--字符输出函数"
  * @param  ch			输出字符
  * @retval None
  */
void DebugOutChar(int ch);

/**
  * @brief  Description "GSM 库函数环境初始化"
  * @param  None
  * @retval bool		初始化成功与否
  */
bool gsm_env_init(GPRS_Connection* ip_com,_SCSStruct * state);

/**
  * @brief  Description "获得GSM的信号强度"
  * @param  None
  * @retval int			GSM的信号强度(5~32为正常数值)
  */
int gsm_get_signal(void);

/**
  * @brief  Description "获得GSM的GPRS的附着状态"
  * @param  None
  * @retval bool		GPRS通讯是否正常
  */
int gsm_get_gprs_state(void);

/**
  * @brief  Description "GPRS通讯"
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
  * @param  rbuf		接收缓冲区
  * @param  rlen		接收缓冲区字节数大小
  * @retval bool		GPRS通讯是否成功
  */
bool gsm_gprs_comm(char* wbuf, int wlen, char* rbuf, int* rlen);

/**
  * @brief  Description "GPRS通讯"
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
  * @param  rbuf		接收缓冲区
  * @param  rlen		接收缓冲区字节数大小
  * @retval bool		GPRS通讯是否成功
  */
bool gsm_gprs_config(char *wbuf, int wlen, char *rbuf, int* rlen);

/**
  * @brief  Description "GPRS通讯"
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
  * @param  rbuf		接收缓冲区
  * @param  rlen		接收缓冲区字节数大小
  * @retval bool		GPRS通讯是否成功
  */
bool gsm_gprs_update(char *wbuf, int wlen, char *rbuf, int* rlen);

/**
  * @brief  Description "GSM发送短信"
  * @param  pn			发送手机号码
  * @param  ascii_utf8	发送缓冲区
  * @retval bool		GSM发送短信是否成功
  */
bool gsm_send_sms(char* pn, char* ascii_utf8);

/**
  * @brief  Description "配置数据服务器网络参数"
  * @param  pn			ip_com:IP、端口、域名
  */
void GSM_Config_Data_Network(void * ip_com);

/**
  * @brief  Description "GSM接收短信"
  * @param  pn			接收手机号码
  * @param  ascii_utf8	接收缓冲区
  * @retval bool		GSM接收短信是否成功
  */
bool gsm_recv_sms(char* pn, RTC_Times* time, char* ascii_utf8, int* len);

/**
  * @brief  Description "模拟I2C写函数"
  * @param  pdata  		写入的数据缓冲区指针
  * @param  addr		写入的器件内存地址
  * @param  count		写入的器件数据大小
  * @retval bool		I2C写函数是否成功
  */
bool AI2C_Write(char *pdata, int addr, int count);

/**
  * @brief  Description "模拟I2C读函数"
  * @param  pdata  		读取的数据缓冲区指针
  * @param  addr		读取的器件内存地址
  * @param  count		读取的器件数据大小
  * @retval bool		I2C读函数是否成功
  */
bool AI2C_Read(char *pdata, int addr, int count);

/**
  * @brief  Description "模拟I2C写RTC8025函数"
  * @param  pdata  		写入的数据缓冲区指针
  * @param  addr		写入的器件内存地址
  * @param  count		写入的器件数据大小
  * @retval bool		RTC8025写函数是否成功
  */
bool RTC8025_Write(char *pdata, int addr, int count);

/**
  * @brief  Description "模拟I2C读RTC8025函数"
  * @param  pdata  		读取的数据缓冲区指针
  * @param  addr		读取的器件内存地址
  * @param  count		读取的器件数据大小
  * @retval bool		RTC8025读函数是否成功
  */
bool RTC8025_Read(char *pdata, int addr, int count);

/**
  * @brief  Description "RTC8025状态位复位函数"
  * @param  None		主动复位标志：0 不主动复位；1、主动复位。
  * @retval bool		RTC8025读函数是否成功
  */
bool RTC8025_Reset(bool need_reset);




/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx
/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

