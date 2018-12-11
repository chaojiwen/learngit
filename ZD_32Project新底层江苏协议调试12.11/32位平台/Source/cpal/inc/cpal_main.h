/**
  ******************************************************************************
  * @file              : cpal_main.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:59:00
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used CPLM pins.
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
#ifndef __CPAL_MAIN_H
#define __CPAL_MAIN_H

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
/** @defgroup CPLM_Private_TypesDefinitions
* @{
*/
//! (Special symbols are not allowed here)
#define CPLMn						1
#define CPLM_COUNT					1
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name cplm configuration structure
//! @{
typedef enum 
{
	CPLM_EEPROM = 0,
} cplm_reg_t;
//<! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for cplm interface function prototypes
#define __EXTERN_CPLM_INTERFACE(__N,__DATA)                                    \
    extern bool cplm##__N##_init(void);                                        \
    extern fsm_rt_t cplm##__N##_read(uint8_t* pBuffer,                         \
									 uint16_t ReadAddr,                        \
	                                 uint32_t NumByteToRead);                  \
    extern fsm_rt_t cplm##__N##_write(uint8_t* pBuffer,                        \
	                                  uint16_t WriteAddr,                      \
	                                  uint32_t NumByteToWrite);     
	
	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: cplm_t
//! @{
DEF_INTERFACE(cplm_t)
    bool         (*Init)(void);                     //!< initialize cplm
    fsm_rt_t     (*Read)(uint8_t* pBuffer, 
	                     uint16_t ReadAddr,      
	                     uint32_t NumByteToRead);   //!< read cplm 
    fsm_rt_t     (*Write)(uint8_t* pBuffer, 
	                      uint16_t WriteAddr,      
	                      uint32_t NumByteToWrite); //!< write cplm 
END_DEF_INTERFACE(cplm_t)
//! @}E(cplm_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief cplm interface function prototypes
MREPEAT(CPLM_COUNT, __EXTERN_CPLM_INTERFACE, NULL)


/*============================ HAL_CPLM ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief cplm object
extern ROOT 
const cplm_t CPLM[CPLM_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* __CPAL_MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
