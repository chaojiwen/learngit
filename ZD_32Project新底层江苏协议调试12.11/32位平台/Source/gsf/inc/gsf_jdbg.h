/**
  ******************************************************************************
  * @file              : gsf_jdbg.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:58:58
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used JDBG pins.
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
#ifndef GSF_GSF_JDBG_H
#define GSF_GSF_JDBG_H

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
/** @defgroup JDBG_Private_TypesDefinitions
* @{
*/
//! (Special symbols are not allowed here)
#define JDBG_COUNT           	1
//! @}
 
/*============================ TYPES =========================================*/
//! \name jdbg configuration structure
//! @{
typedef struct{
	uint32_t JDBG_Val;
	
} jdbg_cfg_t;
//! @}

/*============================ INTERFACE =====================================*/
//! \brief Macro for extern jdbg interface function prototypes
#define __EXTERN_JDBG_INTERFACE(__N, __DATA)                                    \
    extern bool jdbg##__N##_init(jdbg_cfg_t *ptCfg);                            \
    extern fsm_rt_t jdbg##__N##_open(void);                                     \
    extern fsm_rt_t jdbg##__N##_close(void);                                    \
    extern bool jdbg##__N##_putchar(uint32_t wValue);                           \
    extern bool jdbg##__N##_write(uint16_t hwPin, uint16_t hwOut);              \
    extern bool jdbg##__N##_read(uint16_t hwPin,uint16_t *hwIn);
	
	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: jdbg_t
//! @{
DEF_INTERFACE(jdbg_t)
    bool         (*Init)(jdbg_cfg_t *ptJdbgCfg);    //!< initialize jdbg
    fsm_rt_t     (*Open)(void);                     //!< open jdbg 
    fsm_rt_t     (*Close)(void);                    //!< close jdbg
    bool         (*PutChar)(uint32_t wValue);          
    bool         (*Write)(uint16_t hwPin, uint16_t hwOut);          
                                                    //!< write data to jdbg buffer
    bool         (*Read)(uint16_t hwPin,uint16_t *hwIn);          
	                                                //!< read data from jdbg buffer
END_DEF_INTERFACE(jdbg_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief jdbg interface function prototypes
MREPEAT(JDBG_COUNT, __EXTERN_JDBG_INTERFACE, NULL)


/*============================ HAL_JDBG ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief jdbg object
extern ROOT 
const jdbg_t JDBG[JDBG_COUNT];

/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* GSF_GSF_JDBG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
