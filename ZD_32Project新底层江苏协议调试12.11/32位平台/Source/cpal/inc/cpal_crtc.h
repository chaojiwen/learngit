/**
  ******************************************************************************
  * @file              : cpal_crtc.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 15:05:40
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used CRTC pins.
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
#ifndef CPAL_CPAL_CRTC_H
#define CPAL_CPAL_CRTC_H

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
/** @defgroup CRTC_Private_TypesDefinitions
* @{
*/
//! (Special symbols are not allowed here)
#define CRTCn						1
#define CRTC_COUNT					1
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name crtc configuration structure
//! @{
typedef enum 
{
	CRTC0 = 0,
} crtc_reg_t;
//<! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for crtc interface function prototypes
#define __EXTERN_CRTC_INTERFACE(__N,__DATA)                                    \
    extern bool crtc##__N##_init(void);                                        \
    extern fsm_rt_t crtc##__N##_read(uint8_t* pBuffer,                         \
									 uint16_t ReadAddr,                        \
	                                 uint32_t NumByteToRead);                  \
    extern fsm_rt_t crtc##__N##_write(uint8_t* pBuffer,                        \
	                                  uint16_t WriteAddr,                      \
	                                  uint32_t NumByteToWrite);                \
    extern uint32_t crtc##__N##_showtime(void);     
	
	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: crtc_t
//! @{
DEF_INTERFACE(crtc_t)
    bool         (*Init)(void);                     //!< initialize crtc
    fsm_rt_t     (*Read)(uint8_t* pBuffer, 
	                     uint16_t ReadAddr,      
	                     uint32_t NumByteToRead);   //!< read crtc 
    fsm_rt_t     (*Write)(uint8_t* pBuffer, 
	                      uint16_t WriteAddr,      
	                      uint32_t NumByteToWrite); //!< write crtc 
    uint32_t     (*ShowTime)(void);                 //!< 
END_DEF_INTERFACE(crtc_t)
//! @}E(crtc_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief crtc interface function prototypes
MREPEAT(CRTC_COUNT, __EXTERN_CRTC_INTERFACE, NULL)


/*============================ HAL_CRTC ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief crtc object
extern ROOT 
const crtc_t CRTC[CRTC_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* CPAL_CPAL_CRTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
