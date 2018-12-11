/**
  ******************************************************************************
  * @file              : [#list fname as fnm]${fnm}[/#list]
  * @author            : HZ Zeda Team
  * @version           : V[#list version as ver]${ver}[/#list]
  * @date              : [#list date as dt]${dt}[/#list]
  * @brief             : c file
  * @description       : This file provides code for the configuration
  *                      of all used CLBK pins.
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT [#list year as y]${y}[/#list] STMicroelectronics
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
#ifndef __STM32F10X_IT_C
#define __STM32F10X_IT_C

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "stm32f10x.h"

/* Private macro -------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

	 
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void  HAL_PendSVHandler           (void)  {                                             }

void  BSP_IntHandlerWWDG          (void)  {                                             }
void  BSP_IntHandlerPVD           (void)  {                                             }
void  BSP_IntHandlerTAMPER        (void)  {                                             }
void  BSP_IntHandlerRTC           (void)  {                                             }
void  BSP_IntHandlerFLASH         (void)  {                                             }
void  BSP_IntHandlerRCC           (void)  {                                             }
/*
void  BSP_IntHandlerEXTI0         (void)  {                                             }
void  BSP_IntHandlerEXTI1         (void)  {                                             }
void  BSP_IntHandlerEXTI2         (void)  {                                             }
void  BSP_IntHandlerEXTI3         (void)  {                                             }
void  BSP_IntHandlerEXTI4         (void)  {                                             }
void  BSP_IntHandlerDMA1_CH1      (void)  {                                             }
void  BSP_IntHandlerDMA1_CH2      (void)  {                                             }
void  BSP_IntHandlerDMA1_CH3      (void)  {                                             }
void  BSP_IntHandlerDMA1_CH4      (void)  { DMA1_Channel4_IRQHandler();                 }
void  BSP_IntHandlerDMA1_CH5      (void)  { DMA1_Channel5_IRQHandler();                 }
void  BSP_IntHandlerDMA1_CH6      (void)  { DMA1_Channel6_IRQHandler();                 }
void  BSP_IntHandlerDMA1_CH7      (void)  { DMA1_Channel7_IRQHandler();                 }
void  BSP_IntHandlerDMA1_CH6      (void)  { DMA1_Channel6_IRQHandler();                 }
void  BSP_IntHandlerDMA1_CH7      (void)  { DMA1_Channel7_IRQHandler();                 }
*/
void  BSP_IntHandlerADC1_2        (void)  {                                             }
void  BSP_IntHandlerUSB_HP_CAN_TX (void)  {                                             }
void  BSP_IntHandlerUSB_LP_CAN_RX0(void)  {                                             }
void  BSP_IntHandlerCAN_RX1       (void)  {                                             }
void  BSP_IntHandlerCAN_SCE       (void)  {                                             }
/*
void  BSP_IntHandlerEXTI9_5       (void)  {                                             }
*/
void  BSP_IntHandlerTIM1_BRK      (void)  {                                             }
void  BSP_IntHandlerTIM1_UP       (void)  {                                             }
void  BSP_IntHandlerTIM1_TRG_COM  (void)  {                                             }
void  BSP_IntHandlerTIM1_CC       (void)  {                                             }
/*
void  BSP_IntHandlerTIM2          (void)  {                                             }
void  BSP_IntHandlerTIM3          (void)  {                                             }
void  BSP_IntHandlerTIM4          (void)  {                                             }
void  BSP_IntHandlerI2C1_EV      (void)   { ITSC[HAL_INT_ID_I2C1_EV].Handler();         }
void  BSP_IntHandlerI2C1_ER       (void)  { ITSC[HAL_INT_ID_I2C1_ER].Handler();         }
*/
void  BSP_IntHandlerI2C2_EV       (void)  {                                             }
void  BSP_IntHandlerI2C2_ER       (void)  {                                             }
void  BSP_IntHandlerSPI1          (void)  {                                             }
void  BSP_IntHandlerSPI2          (void)  {                                             }
/*
void  BSP_IntHandlerUSART1        (void)  { USART1_IRQHandler();                        }
void  BSP_IntHandlerUSART2        (void)  {                                             }
void  BSP_IntHandlerUSART3        (void)  {                                             }
void  BSP_IntHandlerEXTI15_10     (void)  {                                             }
*/
void  BSP_IntHandlerRTCAlarm      (void)  {                                             }
void  BSP_IntHandlerUSBWakeUp     (void)  {                                             }
void  BSP_IntHandlerTIM8_BRK      (void)  {                                             }
void  BSP_IntHandlerTIM8_UP       (void)  {                                             }
void  BSP_IntHandlerTIM8_TRG_COM  (void)  {                                             }
void  BSP_IntHandlerTIM8_CC       (void)  {                                             }
void  BSP_IntHandlerADC3          (void)  {                                             }
void  BSP_IntHandlerFSMC          (void)  {                                             }
void  BSP_IntHandlerSDIO          (void)  {                                             }
/*
void  BSP_IntHandlerTIM5          (void)  {                                             }
*/
void  BSP_IntHandlerSPI3          (void)  {                                             }
/*
void  BSP_IntHandlerUART4         (void)  {                                             }
void  BSP_IntHandlerUART5         (void)  {                                             }
void  BSP_IntHandlerTIM6          (void)  {                                             }
void  BSP_IntHandlerTIM7          (void)  { TIM7_IRQHandler();                          }
void  BSP_IntHandlerDMA2_CH1      (void)  {                                             }
void  BSP_IntHandlerDMA2_CH2      (void)  {                                             }
void  BSP_IntHandlerDMA2_CH3      (void)  {                                             }
void  BSP_IntHandlerDMA2_CH4_5    (void)  {                                             }
*/



//! end of file

#ifdef __cplusplus
}
#endif

#endif /* [#list macro as mcr]${mcr}[/#list] */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

