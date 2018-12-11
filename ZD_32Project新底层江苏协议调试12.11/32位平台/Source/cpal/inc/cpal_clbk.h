/**
  ******************************************************************************
  * @file              : cpal_clbk.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:59:02
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used CLBK pins.
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
#ifndef CPAL_CPAL_CLBK_H
#define CPAL_CPAL_CLBK_H

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include ".\app_cfg.h"
#include ".\bsp_bi2c.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//! @}

/*============================ MACROS ========================================*/
/** @defgroup CLBK_Private_TypesDefinitions
* @{
*/
//! (Special symbols are not allowed here)
#define CLBKn						1
#define CLBK_COUNT					1
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name clbk configuration structure
//! @{
typedef enum 
{
	CLBK0,
} clbk_reg_t;
//<! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for clbk interface function prototypes
#define __EXTERN_CLBK_INTERFACE(__N,__DATA)                                    \
    extern uint32_t clbk##__N##_init(CPAL_InitTypeDef* pDevInitStruct);        \
    extern uint32_t clbk##__N##_timeout(CPAL_InitTypeDef* pDevInitStruct);     \
    extern uint32_t clbk##__N##_txtc(CPAL_InitTypeDef* pDevInitStruct);        \
    extern uint32_t clbk##__N##_rxtc(CPAL_InitTypeDef* pDevInitStruct);        \
    extern uint32_t clbk##__N##_tx(CPAL_InitTypeDef* pDevInitStruct);          \
    extern uint32_t clbk##__N##_rx(CPAL_InitTypeDef* pDevInitStruct);          \
    extern uint32_t clbk##__N##_dmatxtc(CPAL_InitTypeDef* pDevInitStruct);     \
    extern uint32_t clbk##__N##_dmatxht(CPAL_InitTypeDef* pDevInitStruct);     \
    extern uint32_t clbk##__N##_dmatxte(CPAL_InitTypeDef* pDevInitStruct);     \
    extern uint32_t clbk##__N##_dmarxtc(CPAL_InitTypeDef* pDevInitStruct);     \
    extern uint32_t clbk##__N##_dmarxht(CPAL_InitTypeDef* pDevInitStruct);     \
    extern uint32_t clbk##__N##_dmarxte(CPAL_InitTypeDef* pDevInitStruct);     \
    extern uint32_t clbk##__N##_err(CPAL_DevTypeDef pDevInstance,              \
	                                uint32_t DeviceError);                     \
    extern uint32_t clbk##__N##_berr(CPAL_InitTypeDef* pDevInitStruct);        \
    extern uint32_t clbk##__N##_arlo(CPAL_InitTypeDef* pDevInitStruct);        \
    extern uint32_t clbk##__N##_ovr(CPAL_InitTypeDef* pDevInitStruct);         \
    extern uint32_t clbk##__N##_af(CPAL_InitTypeDef* pDevInitStruct);          \
    extern uint32_t clbk##__N##_gencall(CPAL_InitTypeDef* pDevInitStruct);     \
    extern uint32_t clbk##__N##_dualf(CPAL_InitTypeDef* pDevInitStruct);       \
    extern uint32_t clbk##__N##_slaveread(CPAL_InitTypeDef* pDevInitStruct);   \
    extern uint32_t clbk##__N##_slavewrite(CPAL_InitTypeDef* pDevInitStruct);   
	
	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: clbk_t
//! @{
DEF_INTERFACE(clbk_t)
    uint32_t     (*Init)(CPAL_InitTypeDef* pDevInitStruct);			//!< initialize i2ce
	/*=========== Timeout UserCallback ===========*/
    uint32_t     (*TIMEOUT)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
	/*=========== Transfer UserCallback ===========*/
    uint32_t     (*TXTC)(CPAL_InitTypeDef* pDevInitStruct);			//!< 
    uint32_t     (*RXTC)(CPAL_InitTypeDef* pDevInitStruct);			//!< 
    uint32_t     (*TX)(CPAL_InitTypeDef* pDevInitStruct);			//!< 
    uint32_t     (*RX)(CPAL_InitTypeDef* pDevInitStruct);			//!< 
    uint32_t     (*DMATXTC)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
    uint32_t     (*DMATXHT)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
    uint32_t     (*DMATXTE)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
    uint32_t     (*DMARXTC)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
    uint32_t     (*DMARXHT)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
    uint32_t     (*DMARXTE)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
	/*=========== Error UserCallback ===========*/
    uint32_t     (*ERR)(CPAL_DevTypeDef pDevInstance,              
	                    uint32_t DeviceError);						//!< 
    uint32_t     (*BERR)(CPAL_InitTypeDef* pDevInitStruct);			//!< 
    uint32_t     (*ARLO)(CPAL_InitTypeDef* pDevInitStruct);			//!< 
    uint32_t     (*OVR)(CPAL_InitTypeDef* pDevInitStruct);			//!< 
    uint32_t     (*AF)(CPAL_InitTypeDef* pDevInitStruct);			//!< 
	/*=========== Addressing Mode UserCallback ===========*/
    uint32_t     (*GENCALL)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
    uint32_t     (*DUALF)(CPAL_InitTypeDef* pDevInitStruct);		//!< 
    uint32_t     (*SLAVE_READ)(CPAL_InitTypeDef* pDevInitStruct);	//!< 
    uint32_t     (*SLAVE_WRITE)(CPAL_InitTypeDef* pDevInitStruct);	//!< 
END_DEF_INTERFACE(clbk_t)
//! @}E(clbk_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief clbk interface function prototypes
MREPEAT(CLBK_COUNT, __EXTERN_CLBK_INTERFACE, NULL)


/*============================ HAL_CLBK ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief clbk object
extern ROOT 
const clbk_t CLBK[CLBK_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* CPAL_CPAL_CLBK_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
