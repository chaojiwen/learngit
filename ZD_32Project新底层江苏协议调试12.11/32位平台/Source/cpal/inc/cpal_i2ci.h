/**
  ******************************************************************************
  * @file              : cpal_i2ci.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:59:01
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used BI2C pins.
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
#ifndef CPAL_CPAL_I2CI_H
#define CPAL_CPAL_I2CI_H

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include ".\app_cfg.h"
#include ".\cpal_conf.h"
#include ".\bsp_bi2c.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//! @}


/*============================ MACROS ========================================*/
/** @defgroup I2CI_Private_TypesDefinitions
* @{
*/
//! (Special symbols are not allowed here)
#define I2CIn						1
#define I2CI_COUNT					1
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name i2ci configuration structure
//! @{
typedef enum 
{
	I2CI0,
} i2ci_reg_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for i2ci interface function prototypes        
//!
#if defined (CPAL_I2C_MASTER_MODE) || ! defined (CPAL_I2C_LISTEN_MODE)
#define __EXTERN_I2CI_WR_INTERFACE_PROTOTYPES(__N,__DATA)                      \
    extern uint32_t i2ci##__N##_write(CPAL_InitTypeDef* pDevInitStruct);       \
    extern uint32_t i2ci##__N##_read(CPAL_InitTypeDef* pDevInitStruct);                
#else
#define __EXTERN_I2CI_WR_INTERFACE_PROTOTYPES(__N,__DATA)
#endif /* CPAL_I2C_MASTER_MODE || ! CPAL_I2C_LISTEN_MODE */
#if defined (CPAL_I2C_LISTEN_MODE) && defined (CPAL_I2C_SLAVE_MODE)
#define __EXTERN_I2CI_LIS_INTERFACE_PROTOTYPES(__N,__DATA)                     \
    extern uint32_t i2ci##__N##_listen(CPAL_InitTypeDef* pDevInitStruct);                 
#else
#define __EXTERN_I2CI_LIS_INTERFACE_PROTOTYPES(__N,__DATA)
#endif /* CPAL_I2C_LISTEN_MODE && CPAL_I2C_SLAVE_MODE */
//! \brief Macro for i2ci interface function prototypes
#define __EXTERN_I2CI_INTERFACE(__N,__DATA)                                    \
    extern uint32_t i2ci##__N##_init(CPAL_InitTypeDef* pDevInitStruct);        \
    extern uint32_t i2ci##__N##_deinit(CPAL_InitTypeDef* pDevInitStruct);      \
    extern uint32_t i2ci##__N##_structinit(CPAL_InitTypeDef* pDevInitStruct);  \
    extern uint32_t i2ci##__N##_swreset(CPAL_InitTypeDef* pDevInitStruct);     \
	__EXTERN_I2CI_WR_INTERFACE_PROTOTYPES(__N,__DATA)                          \
	__EXTERN_I2CI_LIS_INTERFACE_PROTOTYPES(__N,__DATA)                         \
    extern uint32_t i2ci##__N##_isdeviceready(CPAL_InitTypeDef* pDevInitStruct);    


/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: i2ci_t
//! @{
DEF_INTERFACE(i2ci_t)
    uint32_t     (*Init)(CPAL_InitTypeDef* pDevInitStruct);			//!< initialize i2ci
    uint32_t     (*DeInit)(CPAL_InitTypeDef* pDevInitStruct);		//!< initialize i2ci
    uint32_t     (*StructInit)(CPAL_InitTypeDef* pDevInitStruct);	//!< initialize i2ci
    uint32_t     (*SWReset)(CPAL_InitTypeDef* pDevInitStruct);		//!< initialize i2ci
#if defined (CPAL_I2C_MASTER_MODE) || ! defined (CPAL_I2C_LISTEN_MODE)
    uint32_t     (*Write)(CPAL_InitTypeDef* pDevInitStruct);		//!< read i2ci 
    uint32_t     (*Read)(CPAL_InitTypeDef* pDevInitStruct);			//!< write i2ci 
#endif /* CPAL_I2C_MASTER_MODE || ! CPAL_I2C_LISTEN_MODE */
#if defined (CPAL_I2C_LISTEN_MODE) && defined (CPAL_I2C_SLAVE_MODE)
    uint32_t     (*Listen)(CPAL_InitTypeDef* pDevInitStruct);		//!< listen i2ci 
#endif /* CPAL_I2C_LISTEN_MODE && CPAL_I2C_SLAVE_MODE */
    uint32_t     (*IsDeviceReady)(CPAL_InitTypeDef* pDevInitStruct);//!< 
END_DEF_INTERFACE(i2ci_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief i2ci interface function prototypes
MREPEAT(I2CI_COUNT, __EXTERN_I2CI_INTERFACE, NULL)


/*============================ HAL_I2CI ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief i2ci object
extern ROOT 
const i2ci_t I2CI[I2CI_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* CPAL_CPAL_I2CI_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
