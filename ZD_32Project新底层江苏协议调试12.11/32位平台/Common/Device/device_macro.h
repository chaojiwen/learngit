/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_conf.h 
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
#ifndef __DEVICE_MACRO_H
#define __DEVICE_MACRO_H

/* Includes ------------------------------------------------------------------*/
/*============================ INCLUDES ======================================*/
#include "stdarg.h"
#include "stdbool.h"
#include "stdint.h"

/*============================ MACROS ========================================*/
/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
//! \name const definition
//! @{
/*
#define REG_FLAG_TIMEOUT        ((uint32_t)0x0)
#define REG_LONG_TIMEOUT        ((uint32_t)(10 * REG_FLAG_TIMEOUT))
*/
//! @}


//! @{
#define CLASS(__NAME)         	__##__NAME
#define ROOT
//! @}

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name interface definition
//! @{
#define DEF_INTERFACE(__NAME,...)           \
            typedef struct __NAME __NAME;   \
            __VA_ARGS__                     \
            struct __NAME {

#define END_DEF_INTERFACE(__NAME)           \
            };
//! @}


//! \name extern class
//! @{
#define EXTERN_CLASS(__NAME,...)            \
    typedef union __NAME __NAME;            \
    __VA_ARGS__                             \
    union __NAME {                          \
        uint_fast8_t chMask[(sizeof(struct {

#define END_EXTERN_CLASS(__NAME)            \
        }) + sizeof(uint_fast8_t) - 1) / sizeof(uint_fast8_t)];\
    };
//! @}


//! \name class definition
//! @{
#define DEF_CLASS(__NAME,...)               \
    typedef union __NAME __NAME;            \
    __VA_ARGS__                             \
    typedef struct __##__NAME __##__NAME;   \
    struct __##__NAME{

#define END_DEF_CLASS(__NAME)               \
    };                                      \
    union __NAME {                          \
        uint_fast8_t chMask[(sizeof(__##__NAME) + sizeof(uint_fast8_t) - 1) / sizeof(uint_fast8_t)];\
    };
//! @}

/*============================ TYPES =========================================*/
//! \name IO model
//! @{
typedef enum {
    fsm_rt_on_going = 0,
    fsm_rt_cpl = 1,
    fsm_rt_wait_for_obj = -1,
    fsm_rt_time_out = -2,
    fsm_rt_err = -127,
}fsm_rt_t;
//! @}

/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif /* __DEVICE_MACRO_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
