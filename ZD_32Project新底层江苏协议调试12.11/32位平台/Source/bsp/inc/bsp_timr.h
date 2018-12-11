/**
  ******************************************************************************
  * @file              : bsp_timr.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 10:55:55
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used TIMR pins.
  ******************************************************************************
  * @attention
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
#ifndef BSP_BSP_TIMR_H
#define BSP_BSP_TIMR_H

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
/** @defgroup TIMR_Private_TypesDefinitions
* @{
*/
//! 						(Special symbols are not allowed here)
#define TIMRn					6
#define TIMR_COUNT				6
//! @}

/*============================ TYPES =========================================*/
/** @defgroup STM32_EVAL_Exported_Types
  * \name timr configuration structure
  * @{
  */  
typedef enum 
{
	 TIMR0 = 0
	,TIMR1 = 1
	,TIMR2 = 2
	,TIMR3 = 3
	,TIMR4 = 4
	,TIMR5 = 5
} timr_reg_t;
//! @}

//! @{
//!  
typedef struct 
{
	CPU_FUCT_VOID 	pfIntVect;
	uint32_t		wTimClock;
	uint32_t		wTimCount;
	bool 			bState;
} timr_cfg_t;
//! @}

/*============================ INTERFACE =====================================*/
//! \brief Macro for timr interface function prototypes
#define __EXTERN_TIMR_INTERFACE(__N,__DATA)                                    \
    extern bool timr##__N##_init(timr_cfg_t * cfg);                            \
    extern bool timr##__N##_enable(void);                                      \
    extern bool timr##__N##_disable(void);                         

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: timr_t
//! @{
DEF_INTERFACE(timr_t)
    bool         (*Init)(timr_cfg_t * cfg);   //!< initialize timr
    bool         (*Enable)(void);             //!< deinitialize timr 
    bool         (*Disable)(void);            
END_DEF_INTERFACE(timr_t)
//! @}

/*============================ PROTOTYPES ====================================*/
//! \brief timr interface function prototypes
MREPEAT(TIMR_COUNT, __EXTERN_TIMR_INTERFACE, NULL)

/*============================ HAL_TIMR ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief timr object
extern ROOT 
const timr_t TIMR[TIMR_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* BSP_BSP_TIMR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
