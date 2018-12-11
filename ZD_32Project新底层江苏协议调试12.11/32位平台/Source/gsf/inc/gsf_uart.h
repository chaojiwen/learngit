/**
  ******************************************************************************
  * @file              : gsf_uart.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:58:58
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#ifndef GSF_GSF_UART_H
#define GSF_GSF_UART_H

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
/** @defgroup UART_Private_TypesDefinitions
* @{
*/
//!                         	(Special symbols are not allowed here)
#define UART_COUNT           	5
#define UART0_BASE_ADDRESS    	USART1_BASE
#define UART1_BASE_ADDRESS     	USART2_BASE
#define UART2_BASE_ADDRESS    	USART3_BASE
#define UART3_BASE_ADDRESS     	UART4_BASE
#define UART4_BASE_ADDRESS    	UART5_BASE
//! @}

/** @defgroup UART_LOW_LEVEL_Private_TypesDefinitions
  * @{
  */                     
//!                         	
#define UART0_CLK_ID    		HAL_PERIPH_ID_USART1
#define UART1_CLK_ID     		HAL_PERIPH_ID_USART2
#define UART2_CLK_ID    		HAL_PERIPH_ID_USART3
#define UART3_CLK_ID     		HAL_PERIPH_ID_UART4
#define UART4_CLK_ID    		HAL_PERIPH_ID_UART5
//!                         	
#define UART0_IT_IRQn    		HAL_INT_ID_USART1
#define UART1_IT_IRQn    		HAL_INT_ID_USART2
#define UART2_IT_IRQn   		HAL_INT_ID_USART3
#define UART3_IT_IRQn    		HAL_INT_ID_UART4
#define UART4_IT_IRQn   		HAL_INT_ID_UART5
//! @}

/*============================ TYPES =========================================*/
//! \name uart pins 
//! @{
typedef enum 
{
	 UART_PARITY_NO     = ((uint16_t)0x0000)  //!< Don't use a parity bit.
	,UART_PARITY_EVEN   = ((uint16_t)0x0400)  //!< Use even parity on character transmission.
	,UART_PARITY_ODD    = ((uint16_t)0x0600)  //!< Use odd parity on character transmission.

} em_uart_parity_t;
//! @}

/*! \name Stop Bits Settings
 */
//! @{
typedef enum {
	 UART_STOPBITS_1    = ((uint16_t)0x0000)  //!< Use 1 stop bit.
    ,UART_STOPBITS_0_5  = ((uint16_t)0x1000)  //!< Use 2 stop bits.
    ,UART_STOPBITS_2    = ((uint16_t)0x2000)  //!< Use 2 stop bits.
    ,UART_STOPBITS_1_5  = ((uint16_t)0x3000)  //!< Use 2 stop bits.

} em_uart_stop_bits_t;
//! @}

/*! \name Char length
 */
//! @{
typedef enum {
     UART_CHAR_LENGTH_8b = ((uint16_t)0x0000)  //!< Use 8 bits char length
    ,UART_CHAR_LENGTH_9b = ((uint16_t)0x1000)  //!< Use 9 bits char length

} em_uart_bit_length_t;
//! @} 

/*! \name USART_Mode
 */
//! @{
typedef enum {
	 UART_Mode_Rx        = ((uint16_t)0x0004)
	,UART_Mode_Tx        = ((uint16_t)0x0008)

} em_uart_mode_t;
//! @} 

/*! \name USART_Hardware_Flow_Control
 */
//! @{
typedef enum {
	 UART_HardwareFlowControl_None      = ((uint16_t)0x0000)
	,UART_HardwareFlowControl_RTS       = ((uint16_t)0x0100)
	,UART_HardwareFlowControl_CTS       = ((uint16_t)0x0200)
	,UART_HardwareFlowControl_RTS_CTS   = ((uint16_t)0x0300)

} em_uart_hardware_flowctrl_t;
//! @} 

/*! \name USART_IT
 */
//! @{
typedef enum {
	 HAL_USART_IT_PE    = ((uint16_t)0x0028)
	,HAL_USART_IT_TXE   = ((uint16_t)0x0727)
	,HAL_USART_IT_TC    = ((uint16_t)0x0626)
	,HAL_USART_IT_RXNE  = ((uint16_t)0x0525)
	,HAL_USART_IT_IDLE  = ((uint16_t)0x0424)
	,HAL_USART_IT_LBD   = ((uint16_t)0x0846)
	,HAL_USART_IT_CTS   = ((uint16_t)0x096A)
	,HAL_USART_IT_ERR   = ((uint16_t)0x0060)
	,HAL_USART_IT_ORE   = ((uint16_t)0x0360)
	,HAL_USART_IT_NE    = ((uint16_t)0x0260)
	,HAL_USART_IT_FE    = ((uint16_t)0x0160)

} em_uart_it_status_t;
//! @} 

/*! \name USART_Flags
 */
//! @{
typedef enum {
	 HAL_USART_FLAG_CTS  = ((uint16_t)0x0200)
	,HAL_USART_FLAG_LBD  = ((uint16_t)0x0100)
	,HAL_USART_FLAG_TXE  = ((uint16_t)0x0080)
	,HAL_USART_FLAG_TC   = ((uint16_t)0x0040)
	,HAL_USART_FLAG_RXNE = ((uint16_t)0x0020)
	,HAL_USART_FLAG_IDLE = ((uint16_t)0x0010)
	,HAL_USART_FLAG_ORE  = ((uint16_t)0x0008)
	,HAL_USART_FLAG_NE   = ((uint16_t)0x0004)
	,HAL_USART_FLAG_FE   = ((uint16_t)0x0002)
	,HAL_USART_FLAG_PE   = ((uint16_t)0x0001)

} em_uart_flag_t;
//! @} 

//! \name UART Register
//! @{
typedef struct {
    volatile uint16_t Value;
    uint16_t Reserved;
} uart_type_t;
//!
typedef struct {
  uart_type_t   SR;
  uart_type_t   DR;
  uart_type_t   BRR;
  uart_type_t   CR1;
  uart_type_t   CR2;
  uart_type_t   CR3;
  uart_type_t   GTPR;
} uart_reg_t;
//! @}

//! \name usart configuration structure
//! @{
typedef struct {
    //! Set baud rate of the USART (unused in slave modes).
    uint32_t BaudRate;

    //! Number of bits to transmit as a character (5 to 9).
    uint16_t CharLength;

    //! Number of stop bits between two characters: \ref USART_1_STOPBIT,
    //! \ref USART_2_STOPBITS 
    //! which will result in a time guard period of that length between characters.
    uint16_t Stopbits;

    //! How to calculate the parity bit: \ref USART_EVEN_PARITY, \ref USART_ODD_PARITY,
    //! \ref USART_NO_PARITY 
    uint16_t ParityType;

    //! Specifies wether the Receive or Transmit mode is enabled or disabled.
    //! This parameter can be a value of @ref UART_Mode
    uint16_t Mode;

    //! Specifies wether the haardware flow control mode is enabled or disabled.
    //! This parameter can be a value of @ref UART_Hardware_Flow_Control
    uint16_t HardwareFlowCtrl;

} uart_cfg_t;
//! @}

//! \name usart service
//! @{   
typedef struct {
	CPU_FUCT_BYTE	pfITFISQ;
    uint16_t 		hwITFlag;			//!< Peripherals ID
    bool  			bITState; 			//!< Peripherals ID

} uart_service_t;
//! @}

/*============================ INTERFACE =====================================*/
//! \brief Macro for uart interface function prototypes
#define __EXTERN_UART_INTERFACE(__N,__DATA)                                     \
    extern bool uart##__N##_init(uart_cfg_t *ptCfg);                            \
    extern fsm_rt_t uart##__N##_open(void);                                     \
    extern fsm_rt_t uart##__N##_close(void);                                    \
    extern bool uart##__N##_write(uint16_t hwByte);                             \
    extern bool uart##__N##_read(uint16_t *pwByte);                             \
    extern bool uart##__N##_dma_cmd(uint16_t hwDMAReq, FunctionalState state);  \
	extern bool uart##__N##_it_config(uart_service_t *ptITService);				\
	extern bool uart##__N##_irq_handler(uint16_t *pFlag);
	
	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: uart_t
//! @{
DEF_INTERFACE(uart_t)
    bool         (*Init)(uart_cfg_t *ptUartCfg);    //!< initialize uart
    fsm_rt_t     (*Open)(void);                     //!< open uart 
    fsm_rt_t     (*Close)(void);                    //!< close uart
    bool         (*Write)(uint16_t hwByte);         //!< write data to uart buffer
    bool         (*Read)(uint16_t *pwByte);         //!< read data from uart buffer
	bool         (*DMACmd)(uint16_t hwDMAReq, FunctionalState state);
	bool         (*ITConfig)(uart_service_t *ptITService);  
	bool		 (*IRQHandler)(uint16_t *pFlag);	//!<
    uart_reg_t * const ptRegPage;                   //!< reference to register page
	
END_DEF_INTERFACE(uart_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief uart interface function prototypes
MREPEAT(UART_COUNT, __EXTERN_UART_INTERFACE, NULL)


/*============================ HAL_UART ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief uart object
extern ROOT 
const uart_t UART[UART_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* GSF_GSF_UART_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
