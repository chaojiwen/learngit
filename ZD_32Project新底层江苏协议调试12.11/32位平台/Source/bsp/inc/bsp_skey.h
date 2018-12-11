/**
  ******************************************************************************
  * @file              : bsp_skey.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 13:24:27
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used SKEY pins.
  ******************************************************************************
  *
  * COPYRIGHT 2014 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BSP_BSP_SKEY_H
#define BSP_BSP_SKEY_H

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include ".\app_cfg.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//! @}

/*============================ MACROS ========================================*/
/** @defgroup SKEY_Private_TypesDefinitions
  * @{
  */ 
//!								(Special symbols are not allowed here)
#define SKEYn					1
#define SKEY_COUNT				1
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name skey configuration structure
//! @{
typedef enum 
{
	SKEY0 = 0
} skey_reg_t;
//<! @}

//! @defgroup STM32_EVAL_Exported_Types
//! \name <<按键检测>>结构体定义
//! @{
typedef struct 
{
    uint32_t KeyValue;       //按键的数值
    uint32_t KeyType;        //按键的类型
    uint32_t KeyLocked;      //按键锁状态
} skey_data_t;
//! @}

/*============================ INTERFACE =====================================*/
//! \brief Macro for skey interface function prototypes
#define __EXTERN_SKEY_INTERFACE(__N,__DATA)                                    \
    extern bool skey##__N##_init(uint8_t key_port, int32_t tim,                \
	                             CPU_FUCT_PTR isr);                            \
    extern bool skey##__N##_deinit(void);                                      \
    extern bool skey##__N##_clear(void);                                       \
    extern fsm_rt_t skey##__N##_scan(void);                               

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: skey_t
//! @{
DEF_INTERFACE(skey_t)
    bool         (*Init)(uint8_t key_port, int32_t tim,                
	                     CPU_FUCT_PTR isr);   //!< initialize skey
    bool         (*DeInit)(void);                   //!< deinitialize skey 
    bool         (*Clear)(void);                    //!< clear skey 
    fsm_rt_t     (*Scan)(void);						//!< scan skey
END_DEF_INTERFACE(skey_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief skey interface function prototypes
MREPEAT(SKEY_COUNT, __EXTERN_SKEY_INTERFACE, NULL)


/*============================ HAL_SKEY ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief skey object
extern ROOT 
const skey_t SKEY[SKEY_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* BSP_BSP_SKEY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
