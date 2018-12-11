/**
  ******************************************************************************
  * @file              : gsf_svic.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:58:58
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used SVIC pins.
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
#ifndef GSF_GSF_SVIC_H
#define GSF_GSF_SVIC_H

#ifdef __cplusplus
 extern "C" {
#endif 

/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include ".\app_cfg.h"
//! @}

/*============================ MACROS ========================================*/
//! @{
//! \brief SVIC_pins_define 
//!
//!                         	(此处不能加括号)
#define SVIC_COUNT           	1
//! @}

/*============================ TYPES =========================================*/
//! \name svic pins 
//! @{
typedef enum 
{
	 HAL_INT_ID_NMI  	        =  -9
	,HAL_INT_ID_FAULT	        =  -8
	,HAL_INT_ID_MEMFAULT        =  -7
	,HAL_INT_ID_BUSFAULT        =  -6
	,HAL_INT_ID_USEFAULT        =  -5
	,HAL_INT_ID_SVCALL	        =  -4
	,HAL_INT_ID_DBGMON	        =  -3
	,HAL_INT_ID_PENDSV	        =  -2
	,HAL_INT_ID_SYSTICK	        =  -1
	,HAL_INT_ID_WWDG	        =  0    /* Window WatchDog Interrupt	=     */
	,HAL_INT_ID_PVD	            =  1    /* PVD through EXTI Line detection Interrupt            */
	,HAL_INT_ID_TAMPER	        =  2    /* Tamper Interrupt	=    */
	,HAL_INT_ID_RTC	            =  3    /* RTC global Interrupt	=    */
	,HAL_INT_ID_FLASH	        =  4    /* FLASH global Interrupt	=  */
	,HAL_INT_ID_RCC	            =  5    /* RCC global Interrupt	=    */
	,HAL_INT_ID_EXTI0	        =  6    /* EXTI Line0 Interrupt	=    */
	,HAL_INT_ID_EXTI1	        =  7    /* EXTI Line1 Interrupt	=    */
	,HAL_INT_ID_EXTI2	        =  8    /* EXTI Line2 Interrupt	=    */
	,HAL_INT_ID_EXTI3	        =  9    /* EXTI Line3 Interrupt	=    */
	,HAL_INT_ID_EXTI4	        = 10    /* EXTI Line4 Interrupt	=    */
	,HAL_INT_ID_DMA1_CH1	    = 11    /* DMA1 Channel 1 global Interrupt                      */
	,HAL_INT_ID_DMA1_CH2	    = 12    /* DMA1 Channel 2 global Interrupt                      */
	,HAL_INT_ID_DMA1_CH3	    = 13    /* DMA1 Channel 3 global Interrupt                      */
	,HAL_INT_ID_DMA1_CH4	    = 14    /* DMA1 Channel 4 global Interrupt                      */
	,HAL_INT_ID_DMA1_CH5	    = 15    /* DMA1 Channel 5 global Interrupt                      */
	,HAL_INT_ID_DMA1_CH6	    = 16    /* DMA1 Channel 6 global Interrupt                      */
	,HAL_INT_ID_DMA1_CH7	    = 17    /* DMA1 Channel 7 global Interrupt                      */
	,HAL_INT_ID_ADC1_2	        = 18    /* ADC1 et ADC2 global Interrupt	= */
	,HAL_INT_ID_USB_HP_CAN_TX	= 19    /* USB High Priority or CAN TX Interrupts               */
	,HAL_INT_ID_USB_LP_CAN_RX0	= 20    /* USB Low Priority or CAN RX0 Interrupts               */
	,HAL_INT_ID_CAN_RX1	        = 21    /* CAN RX1 Interrupt	=   */
	,HAL_INT_ID_CAN_SCE	        = 22    /* CAN SCE Interrupt	=   */
	,HAL_INT_ID_EXTI9_5      	= 23    /* External Line[9:5] Interrupts	= */
	,HAL_INT_ID_TIM1_BRK	    = 24    /* TIM1 Break Interrupt	=    */
	,HAL_INT_ID_TIM1_UP	        = 25    /* TIM1 Update Interrupt	=   */
	,HAL_INT_ID_TIM1_TRG_COM	= 26    /* TIM1 Trigger and Commutation Interrupt               */
	,HAL_INT_ID_TIM1_CC	        = 27    /* TIM1 Capture Compare Interrupt                       */
	,HAL_INT_ID_TIM2	        = 28    /* TIM2 global Interrupt	=   */
	,HAL_INT_ID_TIM3	        = 29    /* TIM3 global Interrupt	=   */
	,HAL_INT_ID_TIM4	        = 30    /* TIM4 global Interrupt	=   */
	,HAL_INT_ID_I2C1_EV	        = 31    /* I2C1 Event Interrupt	=    */
	,HAL_INT_ID_I2C1_ER	        = 32    /* I2C1 Error Interrupt	=    */
	,HAL_INT_ID_I2C2_EV	        = 33    /* I2C2 Event Interrupt	=    */
	,HAL_INT_ID_I2C2_ER	        = 34    /* I2C2 Error Interrupt	=    */
	,HAL_INT_ID_SPI1	        = 35    /* SPI1 global Interrupt	=   */
	,HAL_INT_ID_SPI2	        = 36    /* SPI2 global Interrupt	=   */
	,HAL_INT_ID_USART1	        = 37    /* USART1 global Interrupt	= */
	,HAL_INT_ID_USART2	        = 38    /* USART2 global Interrupt	= */
	,HAL_INT_ID_USART3	        = 39    /* USART3 global Interrupt	= */
	,HAL_INT_ID_EXTI15_10	    = 40    /* External Line[15:10] Interrupts                      */
	,HAL_INT_ID_RTCAlarm	    = 41    /* RTC Alarm through EXTI Line Interrupt                */
	,HAL_INT_ID_USBWakeUp	    = 42    /* USB WakeUp from suspend through EXTI Line Interrupt  */
	,HAL_INT_ID_TIM8_BRK	    = 43    /* TIM8 Break Interrupt	=    */
	,HAL_INT_ID_TIM8_UP	        = 44    /* TIM8 Update Interrupt	=   */
	,HAL_INT_ID_TIM8_TRG_COM	= 45    /* TIM8 Trigger and Commutation Interrupt               */
	,HAL_INT_ID_TIM8_CC	        = 46    /* TIM8 Capture Compare Interrupt                       */
	,HAL_INT_ID_ADC3	        = 47    /* ADC3 global Interrupt	=   */
	,HAL_INT_ID_FSMC	        = 48    /* FSMC global Interrupt	=   */
	,HAL_INT_ID_SDIO	        = 49    /* SDIO global Interrupt	=   */
	,HAL_INT_ID_TIM5	        = 50    /* TIM5 global Interrupt	=   */
	,HAL_INT_ID_SPI3	        = 51    /* SPI3 global Interrupt	=   */
	,HAL_INT_ID_UART4	        = 52    /* UART4 global Interrupt	=  */
	,HAL_INT_ID_UART5	        = 53    /* UART5 global Interrupt	=  */
	,HAL_INT_ID_TIM6	        = 54    /* TIM6 global Interrupt	=   */
	,HAL_INT_ID_TIM7	        = 55    /* TIM7 global Interrupt	=   */
	,HAL_INT_ID_DMA2_CH1	    = 56    /* DMA2 Channel 1 global Interrupt                      */
	,HAL_INT_ID_DMA2_CH2	    = 57    /* DMA2 Channel 2 global Interrupt                      */
	,HAL_INT_ID_DMA2_CH3	    = 58    /* DMA2 Channel 3 global Interrupt                      */
	,HAL_INT_ID_DMA2_CH4_5	    = 59    /* DMA2 Channel 4 and DMA2 Channel 5 global Interrupt   */

} em_svic_vect_t;
//! @}

//! \name SVIC int service
//! @{
//!
typedef struct {
	FunctionalState              state;
	uint32_t                    lParam;
	uint32_t                    wParam;
} svic_it_cfg_t;
//! @}

/*============================ INTERFACE =====================================*/
//! \brief Macro for svic interface function prototypes
#define __EXTERN_SVIC_INTERFACE(__N,__DATA)                                     \
    extern bool svic##__N##_init(void);                						    \
    extern bool svic##__N##_deinit(void);                                       \
    extern fsm_rt_t svic##__N##_enable(int8_t vect);                            \
    extern fsm_rt_t svic##__N##_disable(int8_t vect);                           \
    extern fsm_rt_t svic##__N##_disableall(void);                               \
    extern bool svic##__N##_setvect(int8_t vect,CPU_FUCT_VOID isr);             \
    extern bool svic##__N##_setvect_systick(bool opr,CPU_FUCT_VOID isr);        \
    extern bool svic##__N##_setprio(int8_t vect,uint8_t preemption_prio,        \
	                                uint8_t sub_prio);                          \
	extern bool svic##__N##_setprigroup(uint16_t prigroup);                     \
	extern bool svic##__N##_systickcfg(svic_it_cfg_t *cfg);                     \
    extern fsm_rt_t svic##__N##_handler(int8_t vect); 


/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: svic_t
//! @{
DEF_INTERFACE(svic_t)
    bool         (*Init)(void);        				//!< initialize svic
    bool         (*DeInit)(void);                   //!< deinit svic
    fsm_rt_t     (*Enable)(int8_t vect);			//!< enable svic 
    fsm_rt_t     (*Disable)(int8_t vect);			//!< disable svic.
    fsm_rt_t     (*DisableAll)(void);				//!< disable aLL svic
    bool         (*SetVect)(int8_t vect, CPU_FUCT_VOID isr);     
    bool         (*SetVectForSystick)(bool opr, CPU_FUCT_VOID isr);    
	                                                //!< assign ISR handler
    bool         (*SetPrio)(int8_t vect, uint8_t preemption_prio, uint8_t sub_prio);          
													//!< assign ISR priority.
	bool         (*SetPriGroup)(uint16_t prigroup); //!<
	bool         (*SysTickConfig)(svic_it_cfg_t *cfg);                  
	fsm_rt_t     (*Handler)(int8_t vect);      		//!< Central interrupt handler.
	
END_DEF_INTERFACE(svic_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief gpio interface function prototypes
MREPEAT(SVIC_COUNT, __EXTERN_SVIC_INTERFACE, NULL)


/*============================ HAL_SVIC ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief svic object
extern ROOT 
const svic_t SVIC[SVIC_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* GSF_GSF_SVIC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
