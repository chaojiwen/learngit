/**
  ******************************************************************************
  * @file              : main_cpp.h
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
#ifndef MAIN_CPP_H
#define MAIN_CPP_H

//#ifdef __cplusplus
// extern "C" {
//#endif

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32f10x.h"
#include <stdio.h>
#include <stdint.h>
#include "interface.h"
//! @}

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/

/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx
extern __IO int32_t 			TimingDelay;
extern __IO uint32_t		 	nSystemTimingSystick;
/*============================ EXTERN FUNCTIONS ==============================*/
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
finline void Delay(int nTime)
{
    TimingDelay = nTime;

    while(TimingDelay > 0);
}

finline int GetSysTick(void)
{
    return nSystemTimingSystick;
}

//#ifdef __cplusplus
//}
//#endif

#endif /* MAIN_CPP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

