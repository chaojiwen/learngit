/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_macro.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Library configuration file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10X_MACRO_H
#define __STM32F10X_MACRO_H

/* Includes ------------------------------------------------------------------*/
/*============================ INCLUDES ======================================*/
#include "stdarg.h"
#include "stdbool.h"
#include "stdint.h"

/*============================ MACROS ========================================*/
//! \name null definition
//! @{

//! @}


/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name interface definition
//! @{

//! @}

/*============================ TYPES =========================================*/
//! \name Configure Standard Data Types : Data Base Type
//! @{   
/* Defines   CPU status register size (see Note #3).    */
typedef int32_t CORE_CPU_SAFE; 
//! @}

//! \name Configure Standard Data Types : Function
//! @{   
//!
typedef          bool (*CPU_FUCT_VOID)(void);
//!
typedef          void (*CPU_FUCT_PTR )(void *);
//!
typedef CORE_CPU_SAFE (*CPU_FUCT_BYTE)(CORE_CPU_SAFE);
//!
typedef          bool (*CPU_FUCT_CHAR)(void *, int);
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif /* __STM32F10X_MACRO_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
