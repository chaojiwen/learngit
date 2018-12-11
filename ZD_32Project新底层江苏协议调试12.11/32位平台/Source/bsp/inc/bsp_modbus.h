/**
  ******************************************************************************
  * @file              : bsp_modbus.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:59:00
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used BPIO pins.
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
#ifndef BSP_BSP_MODBUS_H
#define BSP_BSP_MODBUS_H

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
/** @defgroup MODBUS_Private_TypesDefinitions
* @{
*/
//!									(Special symbols are not allowed here)
#define MODBUSn						5
#define MODBUS_COUNT				5
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name modbus configuration structure
//! @{
typedef enum 
{
	 MODBUS0 = 0
	,MODBUS1 = 1
	,MODBUS2 = 2
	,MODBUS3 = 3
	,MODBUS4 = 4
} modbus_reg_t;
//! @}

/*! \name Parity Settings
 */
//! @{
typedef enum {
    MODBUS_NO_PARITY   = 0,    //!< Don't use a parity bit.
    MODBUS_EVEN_PARITY = 2,    //!< Use even parity on character transmission.
    MODBUS_ODD_PARITY  = 3,    //!< Use odd parity on character transmission.
} modbus_parity_mode_t;
//! @}

//! \name modbus service manamgent item
//! @{   
typedef struct {
	CPU_FUCT_BYTE         pfTxISQ;
	CPU_FUCT_BYTE         pfRxISQ;
    uint32_t            wBaudrate;       		//!< baudrate
    uint8_t               tParity;       		//!< baudrate
    uint8_t                tState;            	//!< state
} modbus_cfg_t;
//! @}

//! \name usart service
//! @{
typedef struct {
	CPU_FUCT_VOID	pfITFISQ;
    uint16_t 		hwITFlag;			//!< Peripherals ID
    bool  			bITState; 			//!< Peripherals ID

} modbus_service_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for modbus interface function prototypes
#define __EXTERN_MODBUS_INTERFACE(__N,__DATA)                                   \
    extern bool modbus##__N##_init(modbus_cfg_t *ptModbusCfg);                  \
    extern fsm_rt_t modbus##__N##_open(void);                                   \
    extern fsm_rt_t modbus##__N##_close(void);                                  \
    extern fsm_rt_t modbus##__N##_write(uint8_t *ptBuffer, uint16_t hwLength);  \
    extern fsm_rt_t modbus##__N##_rx_config(modbus_service_t *ptServ);          \
    extern fsm_rt_t modbus##__N##_tx_config(modbus_service_t *ptServ);          \
    extern bool modbus##__N##_irq_handler(void);                

	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: modbus_t
//! @{
DEF_INTERFACE(modbus_t)
    bool         (*Init)(modbus_cfg_t *ptModbusCfg);//!< initialize modbus
    fsm_rt_t     (*Open)(void);                     //!< open modbus 
    fsm_rt_t     (*Close)(void);                    //!< close modbus
	fsm_rt_t 	 (*Write)(uint8_t *ptBuffer, uint16_t hwLength);   
													//!< write modbus
	fsm_rt_t 	 (*RxConfig)(modbus_service_t *ptServ);   
	fsm_rt_t 	 (*TxConfig)(modbus_service_t *ptServ);   
	bool     	 (*IRQHandler)(void);    
END_DEF_INTERFACE(modbus_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief modbus interface function prototypes
MREPEAT(MODBUS_COUNT, __EXTERN_MODBUS_INTERFACE, NULL)


/*============================ GLOBAL VARIABLES ==============================*/
//! \brief modbus object (interface function)
extern ROOT
const modbus_t MODBUS[MODBUS_COUNT];


#ifdef __cplusplus
}
#endif

#endif /* __BSP_MODBUS_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
