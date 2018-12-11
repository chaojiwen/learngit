/**
  ******************************************************************************
  * @file              : gsf_fmem.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:59:00
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used FMEM pins.
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
#ifndef GSF_GSF_FMEM_H
#define GSF_GSF_FMEM_H

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
//! 								(Special symbols are not allowed here)
#define FMEMn						1
#define FMEM_COUNT					1
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name fmem configuration structure
//! @{
typedef enum 
{
	 FMEM_BANK0 = 0
	,
} fmem_reg_t;
//<! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for fmem interface function prototypes
#define __EXTERN_FMEM_INTERFACE(__N,__DATA)                                    \
    extern bool fmem##__N##_init(void);                                        \
    extern bool fmem##__N##_deinit(void);                                      \
    extern fsm_rt_t fmem##__N##_read(uint8_t* pBuffer,                         \
									 uint32_t ReadAddr,                        \
	                                 uint32_t NumByteToRead);                  \
    extern fsm_rt_t fmem##__N##_write(uint8_t* pBuffer,                        \
	                                  uint32_t WriteAddr,                      \
	                                  uint32_t NumByteToWrite);                                    

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: fmem_t
//! @{
DEF_INTERFACE(fmem_t)
    bool         (*Init)(void);                     //!< initialize fmem
    bool         (*DeInit)(void);                   
    fsm_rt_t     (*Read)(uint8_t* pBuffer, 
	                     uint32_t ReadAddr,      
	                     uint32_t NumByteToRead);   //!< read fmem 
    fsm_rt_t     (*Write)(uint8_t* pBuffer, 
	                      uint32_t WriteAddr,      
	                      uint32_t NumByteToWrite); //!< write fmem 
END_DEF_INTERFACE(fmem_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief fmem interface function prototypes
MREPEAT(FMEM_COUNT, __EXTERN_FMEM_INTERFACE, NULL)


/*============================ HAL_FMEM ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief fmem object
extern ROOT 
const fmem_t FMEM[FMEM_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* GSF_GSF_FMEM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
