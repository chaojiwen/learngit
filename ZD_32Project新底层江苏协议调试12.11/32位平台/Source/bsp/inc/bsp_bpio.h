/**
  ******************************************************************************
  * @file              : bsp_bpio.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 15:05:36
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used BPIO pins.
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
#ifndef BSP_BSP_BPIO_H
#define BSP_BSP_BPIO_H

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
/** @defgroup BPIO_Private_TypesDefinitions
* @{
*/
//<! "targetName" Pinout
//<! "targetID" 0
//<!
//<! "targetName" Pinout
//<! "targetID" 1
//<!
//<! "targetName" Pinout
//<! "targetID" 2
//<!
//<! "targetName" Pinout
//<! "targetID" 3
//<!
//<! "targetName" Pinout
//<! "targetID" 4
//<!
//! 						(Special symbols are not allowed here)
#define BPIOn					26
#define BPIO_COUNT				1
//! @}

/*============================ TYPES =========================================*/
/** @defgroup STM32_EVAL_Exported_Types
  * \name bpio configuration structure
  * @{
  */  
typedef enum 
{
	 BPIO0 = 0
	,BPIO1 = 1
	,BPIO2 = 2
	,BPIO3 = 3
	,BPIO4 = 4
	,BPIO5 = 5
	,BPIO6 = 6
	,BPIO7 = 7
	,BPIO8 = 8
	,BPIO9 = 9
	,BPIO10 = 10
	,BPIO11 = 11
	,BPIO12 = 12
	,BPIO13 = 13
	,BPIO14 = 14
	,BPIO15 = 15
	,BPIO16 = 16
	,BPIO17 = 17
	,BPIO18 = 18
	,BPIO19 = 19
	,BPIO20 = 20
	,BPIO21 = 21
	,BPIO22 = 22
	,BPIO23 = 23
	,BPIO24 = 24
	,BPIO25 = 25

} bpio_reg_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for bpio interface function prototypes
#define __EXTERN_BPIO_INTERFACE(__N,__DATA)                                    \
    extern bool bpio##__N##_init(uint8_t gpio);                                \
    extern fsm_rt_t bpio##__N##_on(uint8_t gpio);                              \
    extern fsm_rt_t bpio##__N##_off(uint8_t gpio);                             \
    extern fsm_rt_t bpio##__N##_toggle(uint8_t gpio);                          \
    extern int bpio##__N##_read(uint8_t gpio);                                 \
    extern int bpio##__N##_read_state(uint8_t gpio);                           \
    extern bool bpio##__N##_swjcfg(uint32_t cfg);                              \
    extern fsm_rt_t bpio##__N##_save(void);                                    \
    extern fsm_rt_t bpio##__N##_recovery(void);                                \
    extern bool bpio##__N##_flash_init(uint8_t *leds, int times, int ms100);   \
    extern fsm_rt_t bpio##__N##_flash(void);                                   \
    extern int bpio##__N##_getid(char* name);     

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: bpio_t
//! @{
DEF_INTERFACE(bpio_t)
    bool         (*Init)(uint8_t gpio);             //!< initialize bpio
    fsm_rt_t     (*On)(uint8_t gpio);               //!< open bpio 
    fsm_rt_t     (*Off)(uint8_t gpio);              //!< close bpio
    fsm_rt_t     (*Toggle)(uint8_t gpio);           //!< toggle bpio
    int          (*Read)(uint8_t gpio);             //!< read bpio
    int          (*ReadState)(uint8_t gpio);        //!< read bpio
    bool         (*SWJCfg)(uint32_t cfg);           //!< initialize swj bpio
    fsm_rt_t     (*Save)(void);                     //!< save bpio
    fsm_rt_t     (*Recovery)(void);                 //!< recovery bpio
	bool         (*FlashInit)(uint8_t *leds, int times, int ms100);
													//!< flash bpio
    fsm_rt_t     (*Flash)(void);                    //!< close bpio
    int          (*GetID)(char* name);		    	//!< get bpio id by bpio name
END_DEF_INTERFACE(bpio_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief bpio interface function prototypes
MREPEAT(BPIO_COUNT, __EXTERN_BPIO_INTERFACE, NULL)


/*============================ HAL_BPIO ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief bpio object
extern ROOT 
const bpio_t BPIO[BPIO_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* BSP_BSP_BPIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
