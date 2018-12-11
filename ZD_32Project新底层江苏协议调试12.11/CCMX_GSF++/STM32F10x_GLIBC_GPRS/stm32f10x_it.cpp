/**
  ******************************************************************************
  * @file              : stm32f10x_it.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 27/09/2014 17:41:56
  * @brief             : c file
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
#ifndef STM32F10X_IT_C
#define STM32F10X_IT_C

/* Includes ------------------------------------------------------------------*/
#include "app_gsm.h"

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32f10x_it.h"
//! @}

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<e>开启串口接收
//	<i>开启/关闭串口接收功能
#define USART_RECEIVE_EN		1
//	</e>
//
//	<<< end of configuration section >>>

/*============================ TYPES =========================================*/

/*============================ LOCAL VARIABLES ===============================*/


/*============================ GLOBAL VARIABLES ==============================*/


/*============================ EXTERN FUNCTIONS ==============================*/
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */
  
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  * /
void SysTick_Handler(void)
{
}*/

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

#if(USART_RECEIVE_EN!=0)
u8 USART_STATE = 0;
enum
{
    USART_Error = Bit(7)
};

/**
  * @brief  Description "This function handles PPP interrupt request."
  * @param  None
  * @retval None
  */
ARMAPI void USART1_IRQHandler(void)
{
    //接收到数据
    if (COM1.Received())
    {
        COM1.ReadByte();
    }
}
#endif

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
  
#ifdef __cplusplus
}
#endif

#endif /* STM32F10X_IT_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

