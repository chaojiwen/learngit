/**
  ******************************************************************************
  * @file              : gsf_crcu.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:58:58
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used CRCU pins.
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
#ifndef GSF_GSF_CRCU_H
#define GSF_GSF_CRCU_H

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
/** @defgroup CRCU_Private_TypesDefinitions
* @{
*/
//! (Special symbols are not allowed here)
#define CRCU_COUNT           	1
//! @}
 
/*============================ TYPES =========================================*/
//! \name crcu configuration structure
//! @{
// Debug MCU
typedef enum 
{
	 crcu0 = 0
	,
} crcu_reg_t;


/*============================ INTERFACE =====================================*/
//! \brief Macro for crcu interface function prototypes
#define __EXTERN_CRCU_INTERFACE(__N,__DATA)                                     \
    extern bool crcu##__N##_init(void);                                         \
    extern bool crcu##__N##_calcrc16(uint8_t pBuffer[], uint32_t Length);       \
    extern bool crcu##__N##_calcrc32(uint32_t pBuffer[],uint32_t Length);

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: crcu_t
//! @{
DEF_INTERFACE(crcu_t)
    bool         (*Init)(void);    //!< initialize crcu       
    bool         (*Calcrc16)(uint8_t pBuffer[], uint32_t Length);          
    bool         (*Calcrc32)(uint32_t pBuffer[],uint32_t Length);          
	
END_DEF_INTERFACE(crcu_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief crcu interface function prototypes
MREPEAT(CRCU_COUNT, __EXTERN_CRCU_INTERFACE, NULL)


/*============================ HAL_CRCU ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief crcu object
extern ROOT 
const crcu_t CRCU[CRCU_COUNT];

/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* GSF_GSF_CRCU_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
