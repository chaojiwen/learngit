/**
  ******************************************************************************
  * @file              : bsp_ai2c.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 10:55:55
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used AI2C pins.
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
#ifndef BSP_BSP_AI2C_H
#define BSP_BSP_AI2C_H

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
/** @defgroup AI2C_Private_TypesDefinitions
* @{
*/
//! 						(Special symbols are not allowed here)
#define AI2Cn					1
#define AI2C_COUNT				1
//! @}

/*============================ TYPES =========================================*/
/** @defgroup STM32_EVAL_Exported_Types
  * \name ai2c configuration structure
  * @{
  */  
typedef enum 
{
	AI2C0 = 0
} ai2c_reg_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for ai2c interface function prototypes
#define __EXTERN_AI2C_INTERFACE(__N,__DATA)                                    \
    extern bool ai2c##__N##_init(void);                                        \
    extern bool ai2c##__N##_deinit(void);                                      \
    extern bool ai2c##__N##_delay(void);                                       \
    extern bool ai2c##__N##_start(void);                                       \
    extern bool ai2c##__N##_stop(void);                                        \
    extern bool ai2c##__N##_wait_ack(void);                                    \
    extern bool ai2c##__N##_ack(bool Ack);                                     \
    extern bool ai2c##__N##_sendbyte(uint8_t chdata);                          \
    extern bool ai2c##__N##_recvbyte(uint8_t *pdata);                          \
    extern fsm_rt_t ai2c##__N##_write(uint8_t *pdata, uint16_t addr, uint16_t count);  \
    extern fsm_rt_t ai2c##__N##_read(uint8_t *pdata, uint16_t addr, uint16_t count);                          

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: ai2c_t
//! @{
DEF_INTERFACE(ai2c_t)
    bool         (*Init)(void);               //!< initialize ai2c
    bool         (*DeInit)(void);             //!< deinitialize ai2c 
    bool         (*Delay)(void);
    bool         (*Start)(void);
    bool         (*Stop)(void);
    bool         (*WaitAck)(void);
    bool         (*Ack)(bool Ack);
    bool         (*SendByte)(uint8_t chdata);
    bool         (*RecvByte)(uint8_t *pdata);
    fsm_rt_t     (*Write)(uint8_t *pdata, uint16_t addr, uint16_t count);			  
    fsm_rt_t     (*Read)(uint8_t *pdata, uint16_t addr, uint16_t count);             
END_DEF_INTERFACE(ai2c_t)
//! @}

/*============================ PROTOTYPES ====================================*/
//! \brief ai2c interface function prototypes
MREPEAT(AI2C_COUNT, __EXTERN_AI2C_INTERFACE, NULL)


/*============================ HAL_AI2C ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief ai2c object
extern ROOT 
const ai2c_t AI2C[AI2C_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* BSP_BSP_AI2C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
