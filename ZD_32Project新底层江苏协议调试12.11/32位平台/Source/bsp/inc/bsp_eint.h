/**
  ******************************************************************************
  * @file              : bsp_eint.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 15:05:37
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used EINT pins.
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
#ifndef BSP_BSP_EINT_H
#define BSP_BSP_EINT_H

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
/** @defgroup EINT_Private_TypesDefinitions
* @{
*/
//<! "targetName" Pinout
//<! "targetName" ADC1
//<! "targetID" 0
//<!
//<! "targetName" Pinout
//<! "targetName" USART1
//<! "targetID" 1
//<!
//<! "targetName" Pinout
//<! "targetName" USART2
//<! "targetID" 2
//<!
//<! "targetName" Pinout
//<! "targetName" USART3
//<! "targetID" 3
//<!
//<! "targetName" Pinout
//<! "targetName" GPIO
//<! "targetID" 4
//<!
//! 						(Special symbols are not allowed here)
#define EINTn					6
#define EINT_COUNT				6
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name eint configuration structure
//! @{
typedef enum 
{
	 EINT0 = 0
	,EINT1 = 1
	,EINT2 = 2
	,EINT3 = 3
	,EINT4 = 4
	,EINT5 = 5

} eint_reg_t;
//<! @}


/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */                     
typedef enum 
{
	HAL_EINT_Trigger_Rising = 0x08,
	HAL_EINT_Trigger_Falling = 0x0C,  
	HAL_EINT_Trigger_Rising_Falling = 0x10
} eint_trigger_t;
//! @}
/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */                     
typedef enum 
{
	HAL_EINT_Mode_Interrupt = 0x00,
	HAL_EINT_Mode_Event = 0x04
} eint_mode_t;
//! @}


/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */                     
typedef struct 
{
	CPU_FUCT_VOID    pfExtInt_UserCallback;
	uint8_t                    ExtInt_Mode;
	uint8_t                 ExtInt_Trigger;
} eint_cfg_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for eint interface function prototypes
#define __EXTERN_EINT_INTERFACE(__N,__DATA)                                    \
    extern bool eint##__N##_init(eint_cfg_t* ptCfg);                           \
    extern bool eint##__N##_deinit(void);                                      \
    extern fsm_rt_t eint##__N##_softit(void);                                  \
    extern int eint##__N##_getid(char* name);                                  \
    extern bool eint##__N##_irq0_handler(void);                       		   \
    extern bool eint##__N##_irq1_handler(void);                       		   \
    extern bool eint##__N##_irq2_handler(void);                       		   \
    extern bool eint##__N##_irq3_handler(void);                       		   \
    extern bool eint##__N##_irq4_handler(void);                       		   \
    extern bool eint##__N##_irq9_5_handler(void);                     		   \
    extern bool eint##__N##_irq15_10_handler(void);                             

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: eint_t
//! @{
DEF_INTERFACE(eint_t)
    bool         (*Init)(eint_cfg_t* ptCfg);   	//!< initialize eint
    bool         (*DeInit)(void);           	//!< deinitialize eint 
    fsm_rt_t     (*SoftIT)(void);			 	//!< close eint
    int          (*GetID)(char* name);       	//!< close eint
    bool         (*IRQ0Handler)(void);     		//!< irq eint
    bool         (*IRQ1Handler)(void);     		//!< irq eint
    bool         (*IRQ2Handler)(void);     		//!< irq eint
    bool         (*IRQ3Handler)(void);     		//!< irq eint
    bool         (*IRQ4Handler)(void);     		//!< irq eint
    bool         (*IRQ9_5Handler)(void);   		//!< irq eint
    bool         (*IRQ15_10Handler)(void); 		//!< irq eint
END_DEF_INTERFACE(eint_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! wazhiyi <1>
//! \brief eint interface function prototypes
MREPEAT(EINT_COUNT, __EXTERN_EINT_INTERFACE, NULL)


/*============================ HAL_EINT ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief eint object
extern ROOT 
const eint_t EINT[EINT_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* BSP_BSP_EINT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
