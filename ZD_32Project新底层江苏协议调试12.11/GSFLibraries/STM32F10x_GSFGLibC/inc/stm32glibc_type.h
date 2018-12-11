/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32fwlib_type.h
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
#ifndef __STM32FWLIB_TYPE_H
#define __STM32FWLIB_TYPE_H

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include "stm32f10x.h"
#include ".\macro_function.h"
#include ".\compiler.h"
//! @}

/*============================ MACROS ========================================*/
//!
typedef unsigned char				SWITCH;	// 开关型变量类型
typedef int							STATUS;	// 状态型变量类型
//!
typedef int							ERR_NO;	// 函数返回的错误类型
//!
#define U8_MAX    					((u8)255)
#define S8_MAX    					((s8)127)
#define S8_MIN    					((s8)-128)
#define U16_MAX    					((u16)65535u)
#define S16_MAX    					((s16)32767)
#define S16_MIN    					((s16)-32768)
#define U32_MAX    					((u32)4294967295uL)
#define S32_MAX    					((s32)2147483647)
#define S32_MIN    					((s32)-2147483648)

/*============================ MACROS ========================================*/
//! \brief 强制内联
#define finline					__forceinline

//! \brief C++引用C函数
#ifndef ARMAPI
#define ARMAPI					extern "C"
#endif

//! \brief 位移
#ifndef BIT
#define BIT(x)					((uint32_t)0x01L<<(x))
#endif

//! \brief 位移
#ifndef Bit
#define Bit(x)					(0x01L<<(x))
#endif

/* Exported functions ------------------------------------------------------- */
/* Includes ------------------------------------------------------------------*/
#include ".\string_utils.h"

#endif /* __STM32FWLIB_TYPE_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
