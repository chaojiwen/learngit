/**
  ******************************************************************************
  * @file              : bsp_bi2c.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 15:05:38
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
#ifndef BSP_BSP_BI2C_H
#define BSP_BSP_BI2C_H

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
/** @defgroup BI2C_Private_TypesDefinitions
* @{
*/
/*========= Common Defines =========*/ 
/* This define set the number of I2C devices that can be used with this product family */
#define CPAL_I2C_DEV_NUM            2
//! (Special symbols are not allowed here)
#define BI2Cn						2
#define BI2C_COUNT					2
//! @}
/* Exported constants --------------------------------------------------------*/
//! @filename : HAL_CPAL_I2C.h
//! @{
/*========= Common Defines =========*/  

/* This define set the number of I2C devices that can be used with this product family */
//#define CPAL_I2C_DEV_NUM				2   

/* This define is used to enable DMA Channel */
#define DMA_CCR_EN                      ((uint16_t)0x0001)

/* This define is used to check I2C errors ( BERR, ARLO, AF and OVR) */ 
#define CPAL_I2C_STATUS_ERR_MASK        ((uint16_t)0x0F00)  /*!< I2C errors Mask  */

/* This define is used to check I2C events ( TXE, RXNE, STOPF, ADD10, BTF, ADDR and SB) */ 
#define CPAL_I2C_STATUS1_EVT_MASK       ((uint16_t)0x00DF)  /*!< I2C event Mask for Status Register 1  */

/* This define is used to check I2C events ( DUALF, GENCALL, TRA, BUSY and MSL) */ 
#define CPAL_I2C_STATUS2_EVT_MASK       ((uint16_t)0x0097)  /*!< I2C event Mask for Status Register 2  */


/* I2C Event Defines */  
#define CPAL_I2C_EVT_SB                 I2C_SR1_SB       /*!<Start Bit (Master mode) */
#define CPAL_I2C_EVT_ADDR               I2C_SR1_ADDR     /*!<Address sent (master mode)/matched (slave mode) */
#define CPAL_I2C_EVT_ADD10              I2C_SR1_ADD10    /*!<10-bit header sent (Master mode) */
#define CPAL_I2C_EVT_STOPF              I2C_SR1_STOPF    /*!<Stop detection (Slave mode) */
#define CPAL_I2C_EVT_RXNE               I2C_SR1_RXNE     /*!<Data Register not Empty (receivers) */
#define CPAL_I2C_EVT_TXE                I2C_SR1_TXE      /*!<Data Register Empty (transmitters) */



/*========= DMA =========*/
/* DMA channels enable/disable */  

#define __CPAL_I2C_HAL_ENABLE_DMATX(device)      BI2C[device].CPAL_I2C_DMA_TX_Channel->CCR |= DMA_CCR_EN  
  
#define __CPAL_I2C_HAL_DISABLE_DMATX(device)     BI2C[device].CPAL_I2C_DMA_TX_Channel->CCR &= ~DMA_CCR_EN 
  
#define __CPAL_I2C_HAL_ENABLE_DMARX(device)      BI2C[device].CPAL_I2C_DMA_RX_Channel->CCR |= DMA_CCR_EN  

#define __CPAL_I2C_HAL_DISABLE_DMARX(device)     BI2C[device].CPAL_I2C_DMA_RX_Channel->CCR &= ~DMA_CCR_EN

/* DMA interrupts enable/disable */  
#define __I2C_HAL_ENABLE_DMATX_TCIT(device)      BI2C[device].CPAL_I2C_DMA_TX_Channel->CCR |= DMA_IT_TC
  
#define __I2C_HAL_ENABLE_DMATX_HTIT(device)      BI2C[device].CPAL_I2C_DMA_TX_Channel->CCR |= DMA_IT_HT
  
#define __I2C_HAL_ENABLE_DMATX_TEIT(device)      BI2C[device].CPAL_I2C_DMA_TX_Channel->CCR |= DMA_IT_TE
  
#define __I2C_HAL_ENABLE_DMARX_TCIT(device)      BI2C[device].CPAL_I2C_DMA_RX_Channel->CCR |= DMA_IT_TC
  
#define __I2C_HAL_ENABLE_DMARX_HTIT(device)      BI2C[device].CPAL_I2C_DMA_RX_Channel->CCR |= DMA_IT_HT
  
#define __I2C_HAL_ENABLE_DMARX_TEIT(device)      BI2C[device].CPAL_I2C_DMA_RX_Channel->CCR |= DMA_IT_TE
  
/* DMA interrupts flag management */
#define __CPAL_I2C_HAL_GET_DMATX_TCIT(device)    (uint32_t)(BI2C[device].CPAL_I2C_DMA->ISR & BI2C[device].CPAL_I2C_DMA_TX_TC_FLAG)

#define __CPAL_I2C_HAL_GET_DMATX_HTIT(device)    (uint32_t)(BI2C[device].CPAL_I2C_DMA->ISR & BI2C[device].CPAL_I2C_DMA_TX_HT_FLAG)

#define __CPAL_I2C_HAL_GET_DMATX_TEIT(device)    (uint32_t)(BI2C[device].CPAL_I2C_DMA->ISR & BI2C[device].CPAL_I2C_DMA_TX_TE_FLAG)

#define __CPAL_I2C_HAL_GET_DMARX_TCIT(device)    (uint32_t)(BI2C[device].CPAL_I2C_DMA->ISR & BI2C[device].CPAL_I2C_DMA_RX_TC_FLAG)

#define __CPAL_I2C_HAL_GET_DMARX_HTIT(device)    (uint32_t)(BI2C[device].CPAL_I2C_DMA->ISR & BI2C[device].CPAL_I2C_DMA_RX_HT_FLAG)

#define __CPAL_I2C_HAL_GET_DMARX_TEIT(device)    (uint32_t)(BI2C[device].CPAL_I2C_DMA->ISR & BI2C[device].CPAL_I2C_DMA_RX_TE_FLAG)
  
#define __CPAL_I2C_HAL_CLEAR_DMATX_IT(device)    BI2C[device].CPAL_I2C_DMA->IFCR = \
                                                 (BI2C[device].CPAL_I2C_DMA_TX_TC_FLAG | \
                                                  BI2C[device].CPAL_I2C_DMA_TX_HT_FLAG | \
												  BI2C[device].CPAL_I2C_DMA_TX_TE_FLAG)

#define __CPAL_I2C_HAL_CLEAR_DMARX_IT(device)    BI2C[device].CPAL_I2C_DMA->IFCR = \
                                                 (BI2C[device].CPAL_I2C_DMA_RX_TC_FLAG | \
                                                  BI2C[device].CPAL_I2C_DMA_RX_HT_FLAG | \
												  BI2C[device].CPAL_I2C_DMA_RX_TE_FLAG) 

/* Get DMA data counter */  
#define __CPAL_I2C_HAL_DMATX_GET_CNDT(device)    (uint32_t)(BI2C[device].CPAL_I2C_DMA_TX_Channel->CNDTR)
 
#define __CPAL_I2C_HAL_DMARX_GET_CNDT(device)    (uint32_t)(BI2C[device].CPAL_I2C_DMA_RX_Channel->CNDTR) 
 

/*========= I2C =========*/  
/* I2C enable/disable */  
#define __CPAL_I2C_HAL_ENABLE_DEV(device)        BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_PE  
  
#define __CPAL_I2C_HAL_DISABLE_DEV(device)       BI2C[device].CPAL_I2C_DEVICE->CR1 &= ~I2C_CR1_PE  
     
/* I2C software reset */
#define __CPAL_I2C_HAL_SWRST(device)             BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_SWRST; \
                                                 BI2C[device].CPAL_I2C_DEVICE->CR1 &= ~I2C_CR1_SWRST     
  
/* I2C interrupts enable/disable */  
#define __CPAL_I2C_HAL_ENABLE_ERRIT(device)      BI2C[device].CPAL_I2C_DEVICE->CR2 |= I2C_CR2_ITERREN   
   
#define __CPAL_I2C_HAL_DISABLE_ERRIT(device)     BI2C[device].CPAL_I2C_DEVICE->CR2 &= ~I2C_CR2_ITERREN   
  
#define __CPAL_I2C_HAL_ENABLE_EVTIT(device)      BI2C[device].CPAL_I2C_DEVICE->CR2 |= I2C_CR2_ITEVTEN   
   
#define __CPAL_I2C_HAL_DISABLE_EVTIT(device)     BI2C[device].CPAL_I2C_DEVICE->CR2 &= ~I2C_CR2_ITEVTEN 
  
#define __CPAL_I2C_HAL_ENABLE_BUFIT(device)      BI2C[device].CPAL_I2C_DEVICE->CR2 |= I2C_CR2_ITBUFEN   
   
#define __CPAL_I2C_HAL_DISABLE_BUFIT(device)     BI2C[device].CPAL_I2C_DEVICE->CR2 &= ~I2C_CR2_ITBUFEN 
  

/* I2C Addressing configuration */  
#define __CPAL_I2C_HAL_OAR2_CONF(device,value)   BI2C[device].CPAL_I2C_DEVICE->OAR2 &= ((uint16_t)0xFF01);\
                                                 BI2C[device].CPAL_I2C_DEVICE->OAR2 |= (uint16_t)((value) & 0x00FE)
 

#define __CPAL_I2C_HAL_ENABLE_GENCALL(device)    BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_ENGC   
       
#define __CPAL_I2C_HAL_ENABLE_DUALADDR(device)   BI2C[device].CPAL_I2C_DEVICE->OAR2 |= I2C_OAR2_ENDUAL  
  

/* I2C misc configuration */   
#define __CPAL_I2C_HAL_ENABLE_DMAREQ(device)     BI2C[device].CPAL_I2C_DEVICE->CR2 |= I2C_CR2_DMAEN  
  
#define __CPAL_I2C_HAL_DISABLE_DMAREQ(device)    BI2C[device].CPAL_I2C_DEVICE->CR2 &= ~I2C_CR2_DMAEN 
  
  
#define __CPAL_I2C_HAL_ENABLE_ACK(device)        BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_ACK  
  
#define __CPAL_I2C_HAL_DISABLE_ACK(device)       BI2C[device].CPAL_I2C_DEVICE->CR1 &= ~I2C_CR1_ACK  
  
 
#define __CPAL_I2C_HAL_ENABLE_POS(device)        BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_POS   
   
#define __CPAL_I2C_HAL_DISABLE_POS(device)       BI2C[device].CPAL_I2C_DEVICE->CR1 &= ~I2C_CR1_POS 
  
    
#define __CPAL_I2C_HAL_ENABLE_LAST(device)       BI2C[device].CPAL_I2C_DEVICE->CR2 |= I2C_CR2_LAST   
   
#define __CPAL_I2C_HAL_DISABLE_LAST(device)      BI2C[device].CPAL_I2C_DEVICE->CR2 &= ~I2C_CR2_LAST   
  
  
#define __CPAL_I2C_HAL_ENABLE_NOSTRETCH(device)  BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_NOSTRETCH   
   
#define __CPAL_I2C_HAL_DISABLE_NOSTRETCH(device) BI2C[device].CPAL_I2C_DEVICE->CR1 &= ~I2C_CR1_NOSTRETCH   
 

#define __CPAL_I2C_HAL_START(device)             BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_START
  
#define __CPAL_I2C_HAL_STOP(device)              BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_STOP 


/* I2C data management */
#define __CPAL_I2C_HAL_RECEIVE(device)           (uint8_t)(BI2C[device].CPAL_I2C_DEVICE->DR) 

#define __CPAL_I2C_HAL_SEND(device,value)        BI2C[device].CPAL_I2C_DEVICE->DR = (uint8_t)((value)) 
  

/* I2C flags management */
#define __CPAL_I2C_HAL_GET_EVENT(device)         (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & CPAL_I2C_STATUS1_EVT_MASK)

#define __CPAL_I2C_HAL_GET_ERROR(device)         (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & CPAL_I2C_STATUS_ERR_MASK)
  
#define __CPAL_I2C_HAL_GET_SB(device)            (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_SB) 

#define __CPAL_I2C_HAL_GET_ADDR(device)          (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_ADDR) 
  
#define __CPAL_I2C_HAL_GET_ADD10(device)         (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_ADD10) 
  
#define __CPAL_I2C_HAL_GET_STOPF(device)         (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_STOPF) 
  
#define __CPAL_I2C_HAL_GET_BTF(device)           (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_BTF) 
  
#define __CPAL_I2C_HAL_GET_TXE(device)           (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_TXE) 
  
#define __CPAL_I2C_HAL_GET_RXNE(device)          (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_RXNE) 
  
#define __CPAL_I2C_HAL_GET_BUSY(device)          (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR2 & I2C_SR2_BUSY) 
  
#define __CPAL_I2C_HAL_GET_GENCALL(device)       (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR2 & I2C_SR2_GENCALL) 
  
#define __CPAL_I2C_HAL_GET_DUALF(device)         (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR2 & I2C_SR2_DUALF) 

#define __CPAL_I2C_HAL_GET_TRA(device)           (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR2 & I2C_SR2_TRA)  
  
#define __CPAL_I2C_HAL_GET_OVR(device)           (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_OVR) 
  
#define __CPAL_I2C_HAL_GET_AF(device)            (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_AF) 
  
#define __CPAL_I2C_HAL_GET_ARLO(device)          (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_ARLO) 
  
#define __CPAL_I2C_HAL_GET_BERR(device)          (uint16_t)(BI2C[device].CPAL_I2C_DEVICE->SR1 & I2C_SR1_BERR)   

#define __CPAL_I2C_HAL_CLEAR_SB(device)          BI2C[device].CPAL_I2C_DEVICE->SR1;\
                                                 BI2C[device].CPAL_I2C_DEVICE->SR2

#define __CPAL_I2C_HAL_CLEAR_ADDR(device)        BI2C[device].CPAL_I2C_DEVICE->SR1;\
                                                 BI2C[device].CPAL_I2C_DEVICE->SR2

#define __CPAL_I2C_HAL_CLEAR_BTF(device)         BI2C[device].CPAL_I2C_DEVICE->SR1;\
                                                 BI2C[device].CPAL_I2C_DEVICE->DR 

#define __CPAL_I2C_HAL_CLEAR_STOPF(device)       BI2C[device].CPAL_I2C_DEVICE->SR1;\
                                                 BI2C[device].CPAL_I2C_DEVICE->CR1 |= I2C_CR1_PE 

#define __CPAL_I2C_HAL_CLEAR_AF(device)          BI2C[device].CPAL_I2C_DEVICE->SR1 = ~I2C_SR1_AF  

#define __CPAL_I2C_HAL_CLEAR_ERROR(device)       BI2C[device].CPAL_I2C_DEVICE->SR1 = ~CPAL_I2C_STATUS_ERR_MASK
	


/* Exported constants --------------------------------------------------------*/
//! @filename : HAL_CPAL_IO.h
//! @{
/*========= CPAL_Options_TypeDef =========*/
/* CPAL Options defines contains configuration options which can be affected
   to wCPAL_Options which is a bit-field argument so any combination of these
   parameters can be selected. If one option is not selected then the relative
   feature is disabled.
   There are common options and device specific options.*/
#define CPAL_OPT_I2C_DUALADDR           ((uint32_t)0x00000001)  /*!<Use Dual Address Mode (available in Slave Mode only). 
																	To use this option enable it by affecting this define
																	and own address2 to wCPAL_Options */

#define CPAL_OPT_DMATX_HTIT             ((uint32_t)0x00000200)  /*!<Enable the Transmitter DMA Half Transfer Complete interrupt */

#define CPAL_OPT_DMARX_HTIT             ((uint32_t)0x00001000)  /*!<Enable the Receiver DMA Half Transfer Complete interrupt */

#define CPAL_OPT_DMATX_CIRCULAR         ((uint32_t)0x00004000)  /*!<Enable the Circular Mode for DMA Transmitter */

#define CPAL_OPT_DMARX_CIRCULAR         ((uint32_t)0x00008000)  /*!<Enable the Circular Mode for DMA Receiver */

#define CPAL_OPT_NO_MEM_ADDR            ((uint32_t)0x00010000)  /*!<Enable No Memory addressing mode: only slave device address sent 
																	No Register/Physical address to be sent after slave address */

#define CPAL_OPT_16BIT_REG              ((uint32_t)0x00020000)  /*!<Enable 16-Bit Register/Physical addressing mode (two bytes, 
																	MSB first). This option is supported only when CPAL_OPT_NO_MEM_ADDR
																	option is not set */

#define CPAL_OPT_I2C_GENCALL            ((uint32_t)0x00100000)  /*!<Use General Call Address Mode (available in Slave Mode only) 
																	(General Call Address = 0x00) */

#define CPAL_DMA_1BYTE_CASE             ((uint32_t)0x00200000)  /*!<This define is used internally in the library
																	(not by user) and handle 1Byte transfer by IT
																	when DMA Programming Model is selected for reception */

#define CPAL_OPT_I2C_ERRIT_DISABLE      ((uint32_t)0x00400000)  /*!<Disable I2C Errors interrupt (Bus Error, Arbitration Loss, 
																	Acknowledge Failure and Overrun/Underrun Errors).
																	By default, errors interrupt is enabled to manage errors efficiently */

#define CPAL_OPT_I2C_NOSTOP             ((uint32_t)0x00800000)  /*!<Use communication mode with no STOP generation at the end 
																	of data transfer (for multi-read/write operations) */

#define CPAL_OPT_I2C_NOSTOP_MODE        ((uint32_t)0x01000000)  /*!<Start communication in No STOP generation mode */

#define CPAL_OPT_I2C_NACK_ADD           ((uint32_t)0x40000000)  /*!<Initialize the I2C Slave device without enabling the acknowledgement of its 
																	own address. This option must not be used with No Stop generation mode */

/*========= CPAL_Interne_Defines =========*/
#define CPAL_PASS						((uint32_t)0x00000000)  /*!<This value is returned if the last operation succeed */

#define CPAL_FAIL						((uint32_t)0x00000001)  /*!<This value is returned if the last operation failed */

//#define pNULL							(void*)0				/*!<This Value is used to initialise a null pointer */
//! @}

/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name bi2c configuration structure
//! @{
typedef enum 
{
	BI2C0 = 0,
	BI2C1 = 1,
} bi2c_reg_t;
//! @}


/*========= CPAL_I2CError_TypeDef =========*/ 
//! @filename : cpal_i2c_type.h
/* I2C Errors TypeDef */
typedef enum
{
  CPAL_I2C_ERR_NONE      = 0x0000, /*!<No Error: This is the default state for an Idle peripheral */

  CPAL_I2C_ERR_TIMEOUT   = 0x00FF, /*!<Timeout error: The specified timeout has been elapsed without 
                                         any response (expected flag or data didn't happen at expected time). */

  CPAL_I2C_ERR_BERR      = 0x0100, /*!<Bus error: This error occurs when I2C peripheral detects an external
                                       Stop or Start condition during address or data transfer. In this case:
                                          - The BERR bit is set and an interrupt is generated if the ITERREN bit is set.
                                       In Slave mode: 
                                         data are discarded and the lines are released by hardware:
                                          - In case of a misplaced Start, the slave considers it is a restart and 
                                            waits for an address, or a Stop condition.
                                          - In case of a misplaced Stop, the slave behaves like for a Stop condition and 
                                           the lines are released by hardware.
                                       In Master mode: 
                                         the lines are not released and the state of the current transmission is not 
                                         affected. It is up to the software to abort or not the current transmission.
                                       
                                       Software Clearing sequence for the BERR bit:      
                                         1. Writing '0' to this bit  */
                                            
                                                      
  CPAL_I2C_ERR_ARLO        = 0x0200, /*!<Arbitration Lost error: This error occurs when the I2C interface detects 
                                         an arbitration lost condition. In this case:
                                          - The ARLO bit is set by hardware (and an interrupt is generated if the 
                                            ITERREN bit is set).
                                         the I2C Interface goes automatically back to slave mode (the M/SL bit 
                                         is cleared). 
                                         When the I2C loses the arbitration, it is not able to acknowledge its slave
                                         address in the same transfer, but it can acknowledge it after a repeated 
                                         Start from the winning master.
                                         Lines are released by hardware.
                                              
                                         Software Clearing sequence for the BERR bit:      
                                          1. Writing '0' to this bit  */
                                                  
  CPAL_I2C_ERR_AF          = 0x0400, /*!<Acknowledge Failure : This error occurs when the interface detects 
                                         a non-acknowledge bit. In this case:
                                          - The AF bit is set and an interrupt is generated if the ITERREN bit 
                                            is set.
                                         A transmitter which receives a NACK must reset the communication:
                                          - If Slave: lines are released by hardware.
                                          - If Master: a Stop or repeated Start condition must be generated 
                                            by software.
                                                 
                                         Software Clearing sequence for the ARLO bit:
                                         1. Writing '0' to this bit */                                        
                                                      
  CPAL_I2C_ERR_OVR          = 0x0800, /*!<Overrun/Underrun error: An overrun error can occur in slave mode when clock 
                                          stretching is disabled and the I2C interface is receiving data. The interface has
                                          received a byte (RxNE=1) and the data in DR has not been read, before the next 
                                          byte is received by the interface. 
                                          In this case:
                                          The last received byte is lost.
                                           - In case of Overrun error, software should clear the RxNE bit and the transmitter 
                                             should retransmit the last received byte.
                                          
                                          Underrun error can occur in slave mode when clock stretching is disabled and the 
                                          I2C interface is transmitting data. The interface has not updated the DR with the 
                                          next byte(TxE=1), before the clock comes for the next byte. In this case:
                                           - The same byte in the DR register will be sent again.
                                           - The user should make sure that data received on the receiver side during an 
                                             underrun error are discarded and that the next bytes are written within the 
                                             clock low time specified in the I2C bus standard.
                                          For the first byte to be transmitted, the DR must be written after ADDR is 
                                          cleared and before the first SCL rising edge. If not possible, the receiver 
                                          must discard the first data.
                                      
                                       Software Clearing sequence for the ARLO bit:
                                        1. Writing '0' to this bit */
                                                  
 }CPAL_I2CErrorTypeDef;

 
 
/*============================ CPAL_Dev_TypeDef ==============================*/
/* CPAL devices enumeration contains the device instances */
typedef enum
{
    CPAL_I2C1       =   0x00,	  /*!< Use I2C1 device */

    CPAL_I2C2       =   0x01,     /*!< Use I2C2 device */

    CPAL_I2C3       =   0x02      /*!< Use I2C3 device */

}
CPAL_DevTypeDef;


/*============================ CPAL_Direction_TypeDef ========================*/
/* CPAL transfer directions enumeration is used to configure DMA channels
   (TX and RX) if CPAL_PROGMODEL_DMA is selected or TX/RX interrupts
   if CPAL_PROGMODEL_INTERRUPT is selected:
       - If CPAL_DIRECTION_TX is selected only TX DMA Channel or TX Interrupt is configured
       - If CPAL_DIRECTION_RX is selected only RX DMA Channel or RX Interrupt is configured
       - If CPAL_DIRECTION_TXRX is selected both TX and Rx DMA Channels or Interrupts are configured.*/
typedef enum
{
    CPAL_DIRECTION_TX        = 0x01,         /*!<Transmitter only direction */

    CPAL_DIRECTION_RX        = 0x02,         /*!<Receiver only direction */

    CPAL_DIRECTION_TXRX      = 0x03,         /*!<Transmitter and Receiver direction */

} CPAL_DirectionTypeDef;


/*============================ CPAL_Mode_TypeDef =============================*/
/* CPAL device Mode enumeration is used to select in which mode the
   device will proceed : in master mode or slave mode.
   When an I2C device is used in master mode, it will initiate communication
   by sending start bit. When it is used in slave mode, it will wait till
   receiving its own address to start communication.*/
typedef enum
{
    CPAL_MODE_MASTER   =   0x00,    /*!< Use device as master */

    CPAL_MODE_SLAVE    =   0x01     /*!< Use device as slave */

} CPAL_ModeTypeDef;


/*============================ CPAL_ProgModel_TypeDef ========================*/
/* CPAL Programming Models enumeration  is used to define the programming
   model of device communication.
          - CPAL_PROGMODEL_DMA : device is programmed to communicate using DMA.
          - CPAL_PROGMODEL_INTERRUPT : device is programmed to communicate using
            interruption ( TXE , RXNE ...) handlers .
          - CPAL_PROGMODEL_POLLING : device is programmed to communicate using
            application polling routines (interrupts and DMA will not be used).*/
typedef enum
{
    CPAL_PROGMODEL_INTERRUPT = 0x01,         /*!<Interrupt transfer programming model */

    CPAL_PROGMODEL_DMA       = 0x02          /*!<DMA transfer programming model */

} CPAL_ProgModelTypeDef;


/*============================ CPAL_Transfer_TypeDef =========================*/
/* CPAL Transfer structure contains all transfer parameters which are used
   in every Read or Write operation.*/
typedef struct
{
    uint8_t        *pbBuffer;        /*!<The address of the buffer from/to which the transfer should start */

    __IO uint32_t   wNumData;        /*!<Number of data to be transferred for the current transaction */

    uint32_t        wAddr1;          /*!<Contains the Target device Address (optional)*/

    uint32_t        wAddr2;          /*!<Contains the Register/Physical Address into the device (optional) */

} CPAL_TransferTypeDef;


/*============================ CPAL_State_TypeDef ============================*/
/* CPAL global State enumeration contains the current state of CPAL communication.
   Before starting each operation this state is tested. After each operation
   CPAL_State is updated with the new value resulting from the relative operation.*/

typedef enum
{
    CPAL_STATE_DISABLED = 0x00,        /*!<The Disabled state indicates that the device
                                         is not configured. */

    CPAL_STATE_READY    = 0x01,        /*!<The Ready state indicates that the device is configured
                                         correctly and is ready for read or write operation and/or
                                         the last transaction has been successfully completed */

    CPAL_STATE_READY_TX = 0x03,        /*!<The Ready_TX state indicates that the device is ready for
                                         transmission operation */

    CPAL_STATE_READY_RX = 0x05,        /*!<The Ready_RX state indicates that the device is ready for
                                         reception operation */

    CPAL_STATE_BUSY     = 0x02,        /*!<The Busy state indicates that a Write or Read
                                         operation started */

    CPAL_STATE_BUSY_TX  = 0x06,        /*!<The Busy_TX state indicates that a transmission
                                         operation is on going */

    CPAL_STATE_BUSY_RX  = 0x0A,        /*!<The Busy_RX state indicates that a reception
                                         operation is on going */

    CPAL_STATE_ERROR    = 0x10,        /*!<The Error state indicates that the last operation failed.
                                         To determine which error caused the failure, read the
                                         device status structure. */
} CPAL_StateTypeDef;


/*============================ CPAL_Dev_TypeDef ==============================*/
/* CPAL Device structure definition */

typedef struct
{
    CPAL_DevTypeDef         CPAL_Dev;          /*!<Instance of the device. This parameter can be one
                                                 of the following values: CPAL_Dev_TypeDef */

    CPAL_DirectionTypeDef   CPAL_Direction;    /*!<Specifies the direction for the device transfers.
                                                 It can be one of the following values: CPAL_Direction_TypeDef */

    CPAL_ModeTypeDef        CPAL_Mode;         /*!<Specifies the maser/slave mode of device. It can be one of the
                                                 following values: CPAL_Mode_TypeDef */

    CPAL_ProgModelTypeDef   CPAL_ProgModel;    /*!<Specifies the programming model for the device transfers.
                                                 It can be one of the following values:  CPAL_ProgModel_Enum */

    CPAL_TransferTypeDef   *pCPAL_TransferTx;  /*!<Pointer on a structure specifying the parameters of the current
                                                 transmission operations. The structure fields are specified as
                                                 follows: CPAL_Transfer_TypeDef. Use pNULL value if this direction
                                                 is not needed. */

    CPAL_TransferTypeDef   *pCPAL_TransferRx;  /*!<Pointer on a structure specifying the parameters of the current
                                                 reception operations. The structure fields are specified as
                                                 follows: CPAL_Transfer_TypeDef. Use pNULL value if this direction
                                                 is not needed. */

    __IO CPAL_StateTypeDef  CPAL_State;        /*!<Holds the current State of the CPAL driver relative to the device
                                                 instantiated by CPAL_Dev field. The state parameter can be one of
                                                 the following values: CPAL_State_Enum */

    __IO uint32_t           wCPAL_DevError;    /*!<Specifies the error code for the current operation.The error codes
                                                 are specified for each device type as follows:
                                                 CPAL_I2CError_Enum for I2C devices */

    uint32_t                wCPAL_Options;     /*!<Bit-field value specifying additional options for the configuration
                                                 of the device: The bit-field value can be any combination of following
                                                 values: CPAL_Options_Enum. When a value is not selected the relative
                                                 feature is disabled */

    __IO uint32_t           wCPAL_Timeout;     /*!<This field is with timeout procedure. its used to detect timeout */

    I2C_InitTypeDef        *pCPAL_I2C_Struct;  /*!<Pointer to a device Initialization structure as described
                                                 in the standard device library driver.
                                                 A NULL pointer can be provided for this field and, in this case,
                                                 the default values will be used for all the device initialization.
                                                 If only some fields need to be modified for the initialization,
                                                 one can use the CPAL_PPP_StructInit() function just before setting
                                                 the needed fields.
                                                   Example:
                                                     CPAL_InitTypeDef   I2C1_DevStructure;
                                                     CPAL_I2C_StructInit(&I2C1_DevStructure);
                                                     I2C1_DevStructure->pI2C_Struct->I2C_Speed = 100000;
                                                     CPAL_I2C_Init(&I2C1_DevStructure); */
} CPAL_InitTypeDef;

/*============================ CPAL_I2C_Exported_Functions ===================*/
/* CPAL Device structure definition */
typedef uint32_t (*CPAL_FISQ)(CPAL_InitTypeDef*);
typedef struct
{
#if defined (CPAL_I2C_LISTEN_MODE) && defined (CPAL_I2C_SLAVE_MODE)
	CPAL_FISQ CPAL_Listen_ISQ;					/*<!This function allows the specified I2C device to enter listen mode 
                                                    All information relative to the read or write transfer parameters and
                                                    current status are extracted from fields defined in @ref CPAL_Transfer_TypeDef */
#endif /* CPAL_I2C_LISTEN_MODE && CPAL_I2C_SLAVE_MODE */
	CPAL_FISQ CPAL_IsDeviceReady; 				/*<!This function can be used to wait until target device is ready 
                                                    for communication (ie. for memories after write operations) */
	CPAL_FISQ CPAL_EV_ISQ; 						/*<!This function manages all I2C device events */ 
	CPAL_FISQ CPAL_ER_ISQ; 						/*<!This function manages all I2C device errors  */ 
#ifdef CPAL_I2C_DMA_PROGMODEL
	CPAL_FISQ CPAL_DMA_TX_ISQ; 					/*<!This function Handles DMA TX Interrupts */
	CPAL_FISQ CPAL_DMA_RX_ISQ; 					/*<!This function Handles DMA RX Interrupts */
#endif /* CPAL_I2C_DMA_PROGMODEL */
} CPAL_FISQTypeDef;


/*============================ INTERFACE =====================================*/
//! \brief Macro for bi2c interface function prototypes
#ifdef CPAL_I2C_DMA_PROGMODEL    
#define __EXTERN_BI2C_DMA_INTERFACE_PROTOTYPES(__N,__DATA)                     \
    extern bool bi2c##__N##_dmainit(CPAL_DirectionTypeDef Direction,           \
	                                uint32_t Options);                         \
    extern bool bi2c##__N##_dmadeinit(CPAL_DirectionTypeDef Direction);        \
	extern bool bi2c##__N##_dmatxcfg(CPAL_TransferTypeDef *TxXferStruct,       \
	                                uint32_t Options);                         \
	extern bool bi2c##__N##_dmarxcfg(CPAL_TransferTypeDef *RxXferStruct,       \
	                                uint32_t Options);
#else
#define __EXTERN_BI2C_DMA_INTERFACE_PROTOTYPES(__N,__DATA)                            
#endif /* CPAL_I2C_DMA_PROGMODEL */                                                 
//! \brief Macro for bi2c interface function prototypes
#define __EXTERN_BI2C_INTERFACE(__N,__DATA)                                    \
    extern bool bi2c##__N##_init(void);                                        \
    extern bool bi2c##__N##_nvicinit(void);                                    \
    extern bool bi2c##__N##_clkinit(void);                                     \
    extern bool bi2c##__N##_clkdeinit(void);                                   \
    extern bool bi2c##__N##_gpioinit(void);                                    \
    extern bool bi2c##__N##_gpiodeinit(void);                                  \
    __EXTERN_BI2C_DMA_INTERFACE_PROTOTYPES(__N,__DATA);                        \
    extern bool bi2c##__N##_itinit(uint32_t Options,                           \
                                   CPAL_DirectionTypeDef Direction,            \
							       CPAL_ProgModelTypeDef ProgModel);           \
    extern bool bi2c##__N##_itdeinit(uint32_t Options,                         \
                                     CPAL_DirectionTypeDef Direction,          \
							         CPAL_ProgModelTypeDef ProgModel);         
	
	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: bi2c_t
//! @{
DEF_INTERFACE(bi2c_t)
    bool         (*Init)(void);                     //!< initialize bi2c
    bool		 (*NvicInit)(void);                                     
    bool		 (*CLKInit)(void);                                     
    bool		 (*CLKDeInit)(void);                                   
    bool		 (*GPIOInit)(void);                                    
    bool		 (*GPIODeInit)(void);                                
#ifdef CPAL_I2C_DMA_PROGMODEL                                                  
    bool		 (*DMAInit)(CPAL_DirectionTypeDef Direction,           
	                        uint32_t Options);                         
    bool		 (*DMADeInit)(CPAL_DirectionTypeDef Direction);        
	bool		 (*DMATXConfig)(CPAL_TransferTypeDef *TxXferStruct,       
	                            uint32_t Options);                         
	bool		 (*DMARXConfig)(CPAL_TransferTypeDef *RxXferStruct,       
	                            uint32_t Options);                         
#endif /* CPAL_I2C_DMA_PROGMODEL */                                            
    bool		 (*ITInit)(uint32_t Options,                         
                           CPAL_DirectionTypeDef Direction,          
						   CPAL_ProgModelTypeDef ProgModel);         
    bool		 (*ITDeInit)(uint32_t Options,                         
                             CPAL_DirectionTypeDef Direction,          
							 CPAL_ProgModelTypeDef ProgModel); 
				I2C_TypeDef *CPAL_I2C_DEVICE;
				DMA_TypeDef *CPAL_I2C_DMA;	
				DMA_Channel_TypeDef *CPAL_I2C_DMA_TX_Channel;
				DMA_Channel_TypeDef *CPAL_I2C_DMA_RX_Channel;
	const 		uint32_t CPAL_I2C_DMA_TX_TC_FLAG;
	const 		uint32_t CPAL_I2C_DMA_TX_HT_FLAG;
	const 		uint32_t CPAL_I2C_DMA_TX_TE_FLAG;
	const 		uint32_t CPAL_I2C_DMA_RX_TC_FLAG;
	const 		uint32_t CPAL_I2C_DMA_RX_HT_FLAG;
	const 		uint32_t CPAL_I2C_DMA_RX_TE_FLAG;
END_DEF_INTERFACE(bi2c_t)
//! @}E(bi2c_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief bi2c interface function prototypes
MREPEAT(BI2C_COUNT, __EXTERN_BI2C_INTERFACE, NULL)


/*============================ HAL_BI2C ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief bi2c object
extern ROOT 
const bi2c_t BI2C[BI2C_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* BSP_BSP_BI2C_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
