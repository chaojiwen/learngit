/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32fwlib_bitband.h
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : This file contains all the common data types used for the
*                      STM32F10x firmware library.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32FWLIB_BITBAND_H
#define __STM32FWLIB_BITBAND_H

/* Includes ------------------------------------------------------------------*/
#include "stm32fwlib_type.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
//!
//! GPIO registers bit address in the alias region
//! @{
#define PERIPH_BITBAND(ADDR, BIT_NUM) 		\
	(((ADDR & 0xF0000000) + 0x2000000) +	\
	 ((ADDR & 0xFFFFF) << 5 )          +	\
	 ((u8)BIT_NUM << 2))
#define MEM_ADDR(ADDR)  			*((volatile unsigned long  *)(ADDR))
#define BIT_ADDR(ADDR, BIT_NUM)   	MEM_ADDR(PERIPH_BITBAND(ADDR, BIT_NUM))
//! @}

/* Exported functions ------------------------------------------------------- */

#endif /* __STM32FWLIB_BITBAND_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
