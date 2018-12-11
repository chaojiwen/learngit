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
#define BSP_BEEP_CH0			GPIO_Pin_12
#define BEEPIO					IOPB
#define BSP_LED_CH1				GPIO_Pin_6
#define LEDIO1					IOPF
#define BSP_LED_CH2				GPIO_Pin_7
#define LEDIO2					IOPF
#define BSP_LED_CH3				GPIO_Pin_8
#define LEDIO3					IOPF
#define BSP_LED_CH4				GPIO_Pin_9
#define LEDIO4					IOPF
#define BSP_LED_CH5				GPIO_Pin_10
#define LEDIO5					IOPF
//! PWM
#define BSP_GPIOB_CH3			GPIO_Pin_1
#define BSP_GPIOB_CH4			GPIO_Pin_0
#define BSP_GPIOF_PWM_GRP		BSP_GPIOB_CH3 | BSP_GPIOB_CH4
//! DEV-GSM
#define BSP_GSM_PWRCTRL			GPIO_Pin_0
#define GSMPWRCTRL				IOPE
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
#define BSP_BEEP_CH0			GPIO_Pin_4
#define BEEPIO					IOPC
#define BSP_LED_CH1				GPIO_Pin_5
#define LEDIO1					IOPC
#define BSP_LED_CH2				GPIO_Pin_0
#define LEDIO2					IOPB
#define BSP_LED_CH3				GPIO_Pin_1
#define LEDIO3					IOPB
#define BSP_LED_CH4				GPIO_Pin_11
#define LEDIO4					IOPE
#define BSP_LED_CH5				GPIO_Pin_12
#define LEDIO5					IOPE
//! PWM
#define BSP_GPIOB_CH3			GPIO_Pin_1
#define BSP_GPIOB_CH4			GPIO_Pin_0
#define BSP_GPIOF_PWM_GRP		BSP_GPIOB_CH3 | BSP_GPIOB_CH4
//! I2C
#define BSP_I2C_SCL				GPIO_Pin_8
#define BSP_I2C_SDA				GPIO_Pin_9
#define I2CIO					IOPB
#define BSP_I2C_GRP				BSP_I2C_SCL | BSP_I2C_SDA
//! DEV-GSM
#define BSP_GSM_PWRCTRL			GPIO_Pin_0
#define GSMPWRCTRL				IOPE
#define BSP_GSM_ONOFF			GPIO_Pin_3
#define GSMONOFF				IOPD
#define BSP_GSM_WAKEUP			GPIO_Pin_6
#define GSMWAKEUP				IOPD
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

#endif

/*============================ TYPES =========================================*/

/**
  * @brief  Description "�ṩ��ʱ����"
  * @param  time		��λms
  * @retval None
  */
typedef void (*Func_Delay)(vs32 time);

/**
  * @brief  Description "�ṩ��ʱ���ĺ�����"
  * @param  None
  * @retval ��ʱ���ĺ�����
  */
typedef vu32 (*Func_Systick)(void);

/*============================ INTERFACE =====================================*/

/**
  * @brief  Description "������Ϣ���--�ַ��������"
  * @param  ch			����ַ�
  * @retval None
  */
void DebugOutChar(int ch);

/**
  * @brief  Description "GSM �⺯��������ʼ��"
  * @param  None
  * @retval bool		��ʼ���ɹ����
  */
bool gsm_env_init(void);

/**
  * @brief  Description "���GSM���ź�ǿ��"
  * @param  None
  * @retval int			GSM���ź�ǿ��(5~32Ϊ������ֵ)
  */
int gsm_get_signal(void);

/**
  * @brief  Description "���GSM��GPRS�ĸ���״̬"
  * @param  None
  * @retval bool		GPRSͨѶ�Ƿ�����
  */
int gsm_get_gprs_state(void);

/**
  * @brief  Description "GPRSͨѶ"
  * @param  wbuf		���ͻ�����
  * @param  wlen		���ͻ������ֽ�����С
  * @param  rbuf		���ջ�����
  * @param  rlen		���ջ������ֽ�����С
  * @retval bool		GPRSͨѶ�Ƿ�ɹ�
  */
bool gsm_gprs_comm(char* wbuf, int wlen, char* rbuf, int rlen);

/**
  * @brief  Description "GSM���Ͷ���"
  * @param  pn			�����ֻ�����
  * @param  ascii_utf8	���ͻ�����
  * @retval bool		GSM���Ͷ����Ƿ�ɹ�
  */
bool gsm_send_sms(char* pn, char* ascii_utf8);

/**
  * @brief  Description "ģ��I2Cд����"
  * @param  pdata  		д������ݻ�����ָ��
  * @param  addr		д��������ڴ��ַ
  * @param  count		д����������ݴ�С
  * @retval bool		I2Cд�����Ƿ�ɹ�
  */
bool AI2C_Write(char *pdata, int addr, int count);

/**
  * @brief  Description "ģ��I2C������"
  * @param  pdata  		��ȡ�����ݻ�����ָ��
  * @param  addr		��ȡ�������ڴ��ַ
  * @param  count		��ȡ���������ݴ�С
  * @retval bool		I2C�������Ƿ�ɹ�
  */
bool AI2C_Read(char *pdata, int addr, int count);


/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx
/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

