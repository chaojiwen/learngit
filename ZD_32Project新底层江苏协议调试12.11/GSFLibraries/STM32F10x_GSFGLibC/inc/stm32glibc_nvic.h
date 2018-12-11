/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_nvic.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32FWLIB_NVIC_H
#define __STM32FWLIB_NVIC_H

#include "stm32glibc_type.h"
#include "stm32glibc_scb.h"



/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
namespace	mcucore
{
    /**
     * @brief STM32F10x Interrupt Number Definition, according to the selected device
     *        in @ref Library_configuration_section
     */
    typedef enum IRQn
    {
        /******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
        NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                             */
        MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M3 Memory Management Interrupt              */
        BusFault_IRQn               = -11,    /*!< 5 Cortex-M3 Bus Fault Interrupt                      */
        UsageFault_IRQn             = -10,    /*!< 6 Cortex-M3 Usage Fault Interrupt                    */
        SVCall_IRQn                 = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                       */
        DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                 */
        PendSV_IRQn                 = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                       */
        SysTick_IRQn                = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                   */

        /******  STM32 specific Interrupt Numbers *********************************************************/
        WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
        PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
        TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
        RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
        FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
        RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
        EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
        EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
        EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
        EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
        EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
        DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
        DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
        DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
        DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
        DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
        DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
        DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */

#ifdef STM32F10X_LD
        ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
        USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
        USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
        CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
        CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
        EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
        TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
        TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
        TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
        TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
        TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
        TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
        I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
        I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
        SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
        USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
        USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
        EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
        RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
        USBWakeUp_IRQn              = 42,     /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
#endif /* STM32F10X_LD */

#ifdef STM32F10X_LD_VL
        ADC1_IRQn                   = 18,     /*!< ADC1 global Interrupt                                */
        EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
        TIM1_BRK_TIM15_IRQn         = 24,     /*!< TIM1 Break and TIM15 Interrupts                      */
        TIM1_UP_TIM16_IRQn          = 25,     /*!< TIM1 Update and TIM16 Interrupts                     */
        TIM1_TRG_COM_TIM17_IRQn     = 26,     /*!< TIM1 Trigger and Commutation and TIM17 Interrupt     */
        TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
        TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
        TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
        I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
        I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
        SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
        USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
        USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
        EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
        RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
        CEC_IRQn                    = 42,     /*!< HDMI-CEC Interrupt                                   */
        TIM6_DAC_IRQn               = 54,     /*!< TIM6 and DAC underrun Interrupt                      */
        TIM7_IRQn                   = 55,     /*!< TIM7 Interrupt                                       */
#endif /* STM32F10X_LD_VL */

#ifdef STM32F10X_MD
        ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
        USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
        USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
        CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
        CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
        EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
        TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
        TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
        TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
        TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
        TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
        TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
        TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
        I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
        I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
        I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
        I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
        SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
        SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
        USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
        USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
        USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
        EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
        RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
        USBWakeUp_IRQn              = 42,     /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
#endif /* STM32F10X_MD */

#ifdef STM32F10X_MD_VL
        ADC1_IRQn                   = 18,     /*!< ADC1 global Interrupt                                */
        EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
        TIM1_BRK_TIM15_IRQn         = 24,     /*!< TIM1 Break and TIM15 Interrupts                      */
        TIM1_UP_TIM16_IRQn          = 25,     /*!< TIM1 Update and TIM16 Interrupts                     */
        TIM1_TRG_COM_TIM17_IRQn     = 26,     /*!< TIM1 Trigger and Commutation and TIM17 Interrupt     */
        TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
        TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
        TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
        TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
        I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
        I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
        I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
        I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
        SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
        SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
        USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
        USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
        USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
        EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
        RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
        CEC_IRQn                    = 42,     /*!< HDMI-CEC Interrupt                                   */
        TIM6_DAC_IRQn               = 54,     /*!< TIM6 and DAC underrun Interrupt                      */
        TIM7_IRQn                   = 55,     /*!< TIM7 Interrupt                                       */
#endif /* STM32F10X_MD_VL */

#ifdef STM32F10X_HD
        ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
        USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
        USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
        CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
        CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
        EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
        TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
        TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
        TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
        TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
        TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
        TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
        TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
        I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
        I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
        I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
        I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
        SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
        SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
        USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
        USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
        USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
        EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
        RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
        USBWakeUp_IRQn              = 42,     /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
        TIM8_BRK_IRQn               = 43,     /*!< TIM8 Break Interrupt                                 */
        TIM8_UP_IRQn                = 44,     /*!< TIM8 Update Interrupt                                */
        TIM8_TRG_COM_IRQn           = 45,     /*!< TIM8 Trigger and Commutation Interrupt               */
        TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                       */
        ADC3_IRQn                   = 47,     /*!< ADC3 global Interrupt                                */
        FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                */
        SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                */
        TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                */
        SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
        UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
        UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
        TIM6_IRQn                   = 54,     /*!< TIM6 global Interrupt                                */
        TIM7_IRQn                   = 55,     /*!< TIM7 global Interrupt                                */
        DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
        DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
        DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
        DMA2_Channel4_5_IRQn        = 59,     /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
#endif /* STM32F10X_HD */

#ifdef STM32F10X_HD_VL
        ADC1_IRQn                   = 18,     /*!< ADC1 global Interrupt                                */
        EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
        TIM1_BRK_TIM15_IRQn         = 24,     /*!< TIM1 Break and TIM15 Interrupts                      */
        TIM1_UP_TIM16_IRQn          = 25,     /*!< TIM1 Update and TIM16 Interrupts                     */
        TIM1_TRG_COM_TIM17_IRQn     = 26,     /*!< TIM1 Trigger and Commutation and TIM17 Interrupt     */
        TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
        TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
        TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
        TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
        I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
        I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
        I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
        I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
        SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
        SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
        USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
        USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
        USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
        EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
        RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
        CEC_IRQn                    = 42,     /*!< HDMI-CEC Interrupt                                   */
        TIM12_IRQn                  = 43,     /*!< TIM12 global Interrupt                               */
        TIM13_IRQn                  = 44,     /*!< TIM13 global Interrupt                               */
        TIM14_IRQn                  = 45,     /*!< TIM14 global Interrupt                               */
        TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                */
        SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
        UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
        UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
        TIM6_DAC_IRQn               = 54,     /*!< TIM6 and DAC underrun Interrupt                      */
        TIM7_IRQn                   = 55,     /*!< TIM7 Interrupt                                       */
        DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
        DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
        DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
        DMA2_Channel4_5_IRQn        = 59,     /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
        DMA2_Channel5_IRQn          = 60,     /*!< DMA2 Channel 5 global Interrupt (DMA2 Channel 5 is
		                                         mapped at position 60 only if the MISC_REMAP bit in
		                                         the AFIO_MAPR2 register is set)                      */
#endif /* STM32F10X_HD_VL */

#ifdef STM32F10X_XL
        ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
        USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
        USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
        CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
        CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
        EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
        TIM1_BRK_TIM9_IRQn          = 24,     /*!< TIM1 Break Interrupt and TIM9 global Interrupt       */
        TIM1_UP_TIM10_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM10 global Interrupt     */
        TIM1_TRG_COM_TIM11_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
        TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
        TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
        TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
        TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
        I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
        I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
        I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
        I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
        SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
        SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
        USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
        USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
        USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
        EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
        RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
        USBWakeUp_IRQn              = 42,     /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
        TIM8_BRK_TIM12_IRQn         = 43,     /*!< TIM8 Break Interrupt and TIM12 global Interrupt      */
        TIM8_UP_TIM13_IRQn          = 44,     /*!< TIM8 Update Interrupt and TIM13 global Interrupt     */
        TIM8_TRG_COM_TIM14_IRQn     = 45,     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
        TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                       */
        ADC3_IRQn                   = 47,     /*!< ADC3 global Interrupt                                */
        FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                */
        SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                */
        TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                */
        SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
        UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
        UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
        TIM6_IRQn                   = 54,     /*!< TIM6 global Interrupt                                */
        TIM7_IRQn                   = 55,     /*!< TIM7 global Interrupt                                */
        DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
        DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
        DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
        DMA2_Channel4_5_IRQn        = 59,     /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
#endif /* STM32F10X_XL */

#ifdef STM32F10X_CL
        ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
        CAN1_TX_IRQn                = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
        CAN1_RX0_IRQn               = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
        CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
        CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
        EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
        TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
        TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
        TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
        TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
        TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
        TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
        TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
        I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
        I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
        I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
        I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
        SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
        SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
        USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
        USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
        USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
        EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
        RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
        OTG_FS_WKUP_IRQn            = 42,     /*!< USB OTG FS WakeUp from suspend through EXTI Line Interrupt */
        TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                */
        SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
        UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
        UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
        TIM6_IRQn                   = 54,     /*!< TIM6 global Interrupt                                */
        TIM7_IRQn                   = 55,     /*!< TIM7 global Interrupt                                */
        DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
        DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
        DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
        DMA2_Channel4_IRQn          = 59,     /*!< DMA2 Channel 4 global Interrupt                      */
        DMA2_Channel5_IRQn          = 60,     /*!< DMA2 Channel 5 global Interrupt                      */
        ETH_IRQn                    = 61,     /*!< Ethernet global Interrupt                            */
        ETH_WKUP_IRQn               = 62,     /*!< Ethernet Wakeup through EXTI line Interrupt          */
        CAN2_TX_IRQn                = 63,     /*!< CAN2 TX Interrupt                                    */
        CAN2_RX0_IRQn               = 64,     /*!< CAN2 RX0 Interrupt                                   */
        CAN2_RX1_IRQn               = 65,     /*!< CAN2 RX1 Interrupt                                   */
        CAN2_SCE_IRQn               = 66,     /*!< CAN2 SCE Interrupt                                   */
        OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                          */
#endif /* STM32F10X_CL */
    } IRQn_Type;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ÖÐ¶ÏÓÅÏÈ¼¶¼Ä´æÆ÷ ¼Ä´æÆ÷(NVIC_IP)
    struct NVIC_IP_Struct
    {
        volatile uint32_t PRI_0	                        : 8; //Bit0 ~ Bit7
        volatile uint32_t PRI_1	                        : 8; //Bit8 ~ Bit15
        volatile uint32_t PRI_2	                        : 8; //Bit16 ~ Bit23
        volatile uint32_t PRI_3	                        : 8; //Bit24 ~ Bit31
        volatile uint32_t PRI_4	                        : 8; //Bit0 ~ Bit7
        volatile uint32_t PRI_5	                        : 8; //Bit8 ~ Bit15
        volatile uint32_t PRI_6	                        : 8; //Bit16 ~ Bit23
        volatile uint32_t PRI_7	                        : 8; //Bit24 ~ Bit31
        volatile uint32_t PRI_8	                        : 8; //Bit0 ~ Bit7
        volatile uint32_t PRI_9	                        : 8; //Bit8 ~ Bit15
        volatile uint32_t PRI_10                        : 8; //Bit16 ~ Bit23
        volatile uint32_t PRI_11                        : 8; //Bit24 ~ Bit31
        volatile uint32_t PRI_12                        : 8; //Bit0 ~ Bit7
        volatile uint32_t PRI_13                        : 8; //Bit8 ~ Bit15
        volatile uint32_t PRI_14                        : 8; //Bit16 ~ Bit23
        volatile uint32_t PRI_15                        : 8; //Bit24 ~ Bit31
        volatile uint32_t PRI_16                        : 8; //Bit0 ~ Bit7
        volatile uint32_t PRI_17                        : 8; //Bit8 ~ Bit15
        volatile uint32_t PRI_18                        : 8; //Bit16 ~ Bit23
        volatile uint32_t PRI_19                        : 8; //Bit24 ~ Bit31
        volatile uint32_t PRI_20                        : 8; //Bit0 ~ Bit7
        volatile uint32_t PRI_21                        : 8; //Bit8 ~ Bit15
        volatile uint32_t PRI_22                        : 8; //Bit16 ~ Bit23
        volatile uint32_t PRI_23                        : 8; //Bit24 ~ Bit31
        volatile uint32_t PRI_24                        : 8; //Bit0 ~ Bit7
        volatile uint32_t PRI_25                        : 8; //Bit8 ~ Bit15
        volatile uint32_t PRI_26                        : 8; //Bit16 ~ Bit23
        volatile uint32_t PRI_27                        : 8; //Bit24 ~ Bit31
        volatile uint32_t PRI_28                        : 8; //Bit0 ~ Bit7
        volatile uint32_t PRI_29                        : 8; //Bit8 ~ Bit15
        volatile uint32_t PRI_30                        : 8; //Bit16 ~ Bit23
        volatile uint32_t PRI_31                        : 8; //Bit24 ~ Bit31
    };
    typedef struct NVIC_IP_Struct NVIC_IP_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! Èí¼þ´¥·¢ÖÐ¶Ï¼Ä´æÆ÷ ¼Ä´æÆ÷(NVIC_STIR)
    struct NVIC_STIR_Struct
    {
        volatile uint32_t INTID	                        : 8;  //Bit0 ~ Bit7
        volatile uint32_t Resv1	                        : 24; //Bit8 ~ Bit31
    };
    typedef struct NVIC_STIR_Struct NVIC_STIR_Typedef;


    typedef struct
    {
        //!  ÖÐ¶ÏÊ¹ÄÜÉèÖÃ¼Ä´æÆ÷					 0xE000E100-0xE000E11C
        volatile uint32_t ISER[8];				/*!< Offset: 0x000  Interrupt Set Enable Register           */
        uint32_t  RESERVED0[24];
        //!  ÖÐ¶ÏÊ¹ÄÜÇå³ý¼Ä´æÆ÷					 0xE000E180-0xE000E19C
        volatile uint32_t ICER[8];				/*!< Offset: 0x080  Interrupt Clear Enable Register         */
        uint32_t  RSERVED1[24];
        //!  ÖÐ¶Ï¹ÒÆð£¨pend£©ÉèÖÃ¼Ä´æÆ÷			 0xE000E200£­0xE000E21C
        volatile uint32_t ISPR[8];				/*!< Offset: 0x100  Interrupt Set Pending Register          */
        uint32_t  RESERVED2[24];
        //!  ÖÐ¶Ï¹ÒÆðÇå³ý¼Ä´æÆ÷					 0xE000E280-0xE000E29C
        volatile uint32_t ICPR[8];				/*!< Offset: 0x180  Interrupt Clear Pending Register        */
        uint32_t  RESERVED3[24];
        //!  ¼¤»îÎ»¼Ä´æÆ÷						 0xE000E300£­0xE000E31C
        volatile uint32_t IABR[8];				/*!< Offset: 0x200  Interrupt Active bit Register           */
        uint32_t  RESERVED4[56];
        //!  ÖÐ¶ÏÓÅÏÈ¼¶¼Ä´æÆ÷					 0xE000E400-0xE000E41C
        volatile uint8_t  IP[240];				/*!< Offset: 0x300  Interrupt Priority Register (8Bit wide) */
        uint32_t  RESERVED5[644];
        //!  Èí¼þ´¥·¢ÖÐ¶Ï¼Ä´æÆ÷					 0xE000EF00
        uint32_t  STIR;					/*!< Offset: 0xE00  Software Trigger Interrupt Register     */

        // Èí¼þ´¥·¢ÖÐ¶Ï
        finline void SoftTrig(IRQn_Type n)
        {
            STIR = n;
        }
        // NVIC ÖÐ¶ÏÅäÖÃ,Ä¬ÈÏ×îµÍÓÅÏÈ¼¶
        bool IRQConfig(IRQn_Type n, FunctionalState e = ENABLE, uint8_t groupPri = 0xFF, uint8_t subPri = 0xFF);
    } NVIC_Typedef;
}
//!
//! @{
using mcucore::NVIC_Typedef;
#define stNVIC	((NVIC_Typedef *)           NVIC_BASE)
#define MCUNVIC	(*stNVIC)
//! @}
//!
//! @{
/*
#define ITM_BASE            (0xE0000000)	//!< ITM Base Address
#define SCS_BASE            (0xE000E000)	//!< System Control Space Base Address
#define SysTick_BASE        (0xE000E010)	//!< SysTick Base Address
#define NVIC_BASE           (0xE000E100)	//!< NVIC Base Address
#define SCB_BASE            (0xE000ED00)	//!< System Control Block Base Address
#define CoreDebug_BASE      (0xE000EDF0)	//!< Core Debug Base Address
*/
//! @}

#endif /* __STM32FWLIB_NVIC_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
