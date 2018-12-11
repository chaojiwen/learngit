/**
  ******************************************************************************
  * @file              : bsp_bdma.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 15:05:36
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used BDMA pins.
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
#ifndef BSP_BSP_BDMA_H
#define BSP_BSP_BDMA_H

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
/** @defgroup BDMA_Private_TypesDefinitions
  * @{
  */ 
//!									(Special symbols are not allowed here)
#define BDMAn						12
#define BDMA_COUNT					12
//! @}

//! @{
#define HAL_OPT_DMATX_HTIT          ((uint32_t)0x00000200)  /*!<Enable the Transmitter DMA Half Transfer Complete interrupt */

#define HAL_OPT_DMARX_HTIT          ((uint32_t)0x00001000)  /*!<Enable the Receiver DMA Half Transfer Complete interrupt */

#define HAL_OPT_DMATX_CIRCULAR      ((uint32_t)0x00004000)  /*!<Enable the Circular Mode for DMA Transmitter */
  
#define HAL_OPT_DMARX_CIRCULAR      ((uint32_t)0x00008000)  /*!<Enable the Circular Mode for DMA Receiver */
//! @}


/*============================ TYPES =========================================*/
/** @defgroup STM32_EVAL_Exported_Types
  * \name bdma configuration structure
  * @{
  */  
typedef enum 
{
	 BDMA0 = 0
	,BDMA1 = 1
	,BDMA2 = 2
	,BDMA3 = 3
	,BDMA4 = 4
	,BDMA5 = 5
	,BDMA6 = 6
	,BDMA7 = 7
	,BDMA8 = 8
	,BDMA9 = 9
	,BDMA10 = 10
	,BDMA11 = 11

} bdma_reg_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */                     
typedef enum 
{
    BDMA_PERIPH_SERIAL    = 0x01,         /*!<Transmitter only direction */

    BDMA_PERIPH_I2C       = 0x02,         /*!<Receiver only direction */

    BDMA_PERIPH_SPI       = 0x03,         /*!<Transmitter and Receiver direction */

} em_bdma_periph_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */                     
typedef enum 
{
    BDMA_DIRECTION_TX        = 0x01,         /*!<Transmitter only direction */

    BDMA_DIRECTION_RX        = 0x02,         /*!<Receiver only direction */

    BDMA_DIRECTION_TXRX      = 0x03,         /*!<Transmitter and Receiver direction */

} em_bdma_direction_t;
//! @}

//! @{
//! \brief bdma_flag_define 
//!
#define HAL_IS_BDMA_FLAG(flag)				\
	(((flag) == BDMA_DIRECTION_TX) ||		\
     ((flag) == BDMA_DIRECTION_RX) ||		\
     ((flag) == BDMA_DIRECTION_TXRX))
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */                          
typedef struct 
{
    CPU_FUCT_VOID pfDMATC_UserCallback;	//<! /* Enable DMA TX Channel TCIT  */
    CPU_FUCT_VOID pfDMATE_UserCallback;	//<! /* Enable DMA TX Channel TEIT  */    
    CPU_FUCT_VOID pfDMAHT_UserCallback;	//<! /* Enable DMA TX Channel HTIT  */    
} bdma_uscb_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */  
typedef struct 
{
	uint32_t	              DestAddr;	// 外设目的地址
	uint8_t                  Direction;
	uint8_t                  DMAPeriph;
	uint16_t                    lParam;
	uint32_t                   Options;
    bdma_uscb_t           UserCallback;	  
} bdma_cfg_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */                     
typedef struct 
{    
	uint8_t*         pbBuffer;   	/*!<The address of the buffer from/to which the transfer should start */ 
	__IO uint32_t    wNumData;     	/*!<Number of data to be transferred for the current transaction */                     
	uint32_t           wAddr1;     	/*!<Contains the Target device Address (optional)*/
	uint32_t           wAddr2;   	/*!<Contains the Register/Physical Address into the device (optional) */
	uint32_t          Options;		//<!  
} bdma_it_cfg_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for bdma interface function prototypes
#define __EXTERN_BDMA_INTERFACE(__N,__DATA)                                    \
    extern bool bdma##__N##_init(bdma_cfg_t* cfg);                             \
    extern bool bdma##__N##_struct_init(DMA_InitTypeDef* st);                  \
    extern bool bdma##__N##_deinit(void);                                      \
    extern fsm_rt_t bdma##__N##_txcfg(bdma_it_cfg_t *cfg);                     \
    extern fsm_rt_t bdma##__N##_rxcfg(bdma_it_cfg_t *cfg);                     \
    extern uint32_t bdma##__N##_get_count(void);                               \
    extern bool bdma##__N##_irq_handler(void);                                                                 

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: bdma_t
//! @{
DEF_INTERFACE(bdma_t)
    bool         (*Init)(bdma_cfg_t* cfg);          //!< initialize bdma
    bool         (*StructInit)(DMA_InitTypeDef* st);//!< initialize bdma
    bool         (*DeInit)(void);           	    //!< deinitialize  bdma
    fsm_rt_t     (*TxCfg)(bdma_it_cfg_t *cfg);      //!< save bdma
    fsm_rt_t     (*RxCfg)(bdma_it_cfg_t *cfg);      //!< recovery bdma
	uint32_t     (*GetCount)();
	bool         (*IRQHandler)(void);
END_DEF_INTERFACE(bdma_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief bdma interface function prototypes
MREPEAT(BDMA_COUNT, __EXTERN_BDMA_INTERFACE, NULL)


/*============================ HAL_BDMA ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief bdma object
extern ROOT 
const bdma_t BDMA[BDMA_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* BSP_BSP_BDMA_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
