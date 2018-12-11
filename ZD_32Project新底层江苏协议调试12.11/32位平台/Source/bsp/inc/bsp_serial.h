/**
  ******************************************************************************
  * @file              : bsp_serial.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:58:59
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
#ifndef BSP_BSP_SERIAL_H
#define BSP_BSP_SERIAL_H

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
#include "..\inc\bsp_bdma.h"
//! @}

/*============================ MACROS ========================================*/
/** @defgroup SERIAL_Private_TypesDefinitions
* @{
*/
//!									(Special symbols are not allowed here)
#define SERIALn						5
#define SERIAL_COUNT				5
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name serial configuration structure
//! @{
typedef enum 
{
	 SERIAL0 = 0
	,SERIAL1 = 1
	,SERIAL2 = 2
	,SERIAL3 = 3
	,SERIAL4 = 4
} serial_reg_t;
//! @}

/*! \name Parity Settings
 */
//! @{
typedef enum {
    SERIAL_NO_PARITY   = 0,    //!< Don't use a parity bit.
    SERIAL_EVEN_PARITY = 2,    //!< Use even parity on character transmission.
    SERIAL_ODD_PARITY  = 3,    //!< Use odd parity on character transmission.
} serial_parity_mode_t;
//! @}

//! \name serial service manamgent item
//! @{   
typedef struct {
	CPU_FUCT_VOID   pfTx_UserCallback;
	CPU_FUCT_VOID   pfRx_UserCallback;
	CPU_FUCT_VOID   pfIdle_UserCallback;
	CPU_FUCT_VOID   pfErr_UserCallback;
    uint32_t        wBaudrate;       	//!< baudrate
    uint8_t         tParity;       		//!< baudrate
    uint8_t         tState;            	//!< state
} serial_cfg_t;
//! @}

//! \name usart service
//! @{
typedef struct {
	CPU_FUCT_VOID	pfITF_UserCallback;
	bdma_uscb_t  	DMA_UserCallback;
    uint32_t	    wDMAMode;
    bool  		    bDMAState;
    bool  			bITState; 			//!< Peripherals ID

} serial_service_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for serial interface function prototypes
#define __EXTERN_SERIAL_INTERFACE(__N,__DATA)                                   \
    extern bool serial##__N##_init(serial_cfg_t *ptSerialCfg);                  \
    extern fsm_rt_t serial##__N##_open(void);                                   \
    extern fsm_rt_t serial##__N##_close(void);                                  \
    extern fsm_rt_t serial##__N##_write(uint8_t *ptBuffer, uint16_t hwLen);     \
    extern fsm_rt_t serial##__N##_read(uint8_t *ptBuffer, uint16_t hwLen);      \
    extern fsm_rt_t serial##__N##_dma_send(uint8_t *ptBuffer, uint16_t hwLen);  \
    extern fsm_rt_t serial##__N##_dma_recv(uint8_t *ptBuffer, uint16_t hwLen);  \
    extern uint32_t serial##__N##_get_dma_count(void);                          \
    extern fsm_rt_t serial##__N##_rx_it_config(serial_service_t *ptServ);       \
    extern fsm_rt_t serial##__N##_tx_it_config(serial_service_t *ptServ);       \
    extern fsm_rt_t serial##__N##_idle_it_config(serial_service_t *ptServ);     \
    extern bool serial##__N##_irq_handler(void);   

	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: serial_t
//! @{
DEF_INTERFACE(serial_t)
    bool         (*Init)(serial_cfg_t *ptSerialCfg);//!< initialize serial
    fsm_rt_t     (*Open)(void);                     //!< open serial 
    fsm_rt_t     (*Close)(void);                    //!< close serial
	fsm_rt_t 	 (*Write)(uint8_t *ptBuffer, uint16_t hwLen);   
													//!< write serial
	fsm_rt_t 	 (*Read)(uint8_t *ptBuffer, uint16_t hwLen);   
	fsm_rt_t 	 (*DMASend)(uint8_t *ptBuffer, uint16_t hwLen);   
	fsm_rt_t 	 (*DMARecv)(uint8_t *ptBuffer, uint16_t hwLen);   
	uint32_t 	 (*GetDMACount)(void);  
	fsm_rt_t 	 (*RxITConfig)(serial_service_t *ptServ);   
	fsm_rt_t 	 (*TxITConfig)(serial_service_t *ptServ);   
	fsm_rt_t 	 (*IdleITConfig)(serial_service_t *ptServ);   
	bool     	 (*IRQHandler)(void);    
END_DEF_INTERFACE(serial_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief serial interface function prototypes
MREPEAT(SERIAL_COUNT, __EXTERN_SERIAL_INTERFACE, NULL)


/*============================ GLOBAL VARIABLES ==============================*/
//! \brief serial object (interface function)
extern ROOT
const serial_t SERIAL[SERIAL_COUNT];


#ifdef __cplusplus
}
#endif

#endif /* __BSP_SERIAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
