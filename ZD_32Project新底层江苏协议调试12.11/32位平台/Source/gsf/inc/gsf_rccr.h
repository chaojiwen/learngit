/**
  ******************************************************************************
  * @file              : gsf_rccr.h
  * @author            : HZ Zeda Team
  * @version           : V0.5.0
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
#ifndef GSF_GSF_RCCR_H
#define GSF_GSF_RCCR_H

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
/** @defgroup SPI_Private_TypesDefinitions
* @{
*/
//! (Special symbols are not allowed here)
#define RCCR_COUNT           		1
#define RCCR0_BASE_ADDRESS    		RCC_BASE
//! @}


/*============================ TYPES =========================================*/
//! \name rccr periph id 
//! @{
typedef enum 
{
	 HAL_PERIPH_ID_DMA1     = 0
	,HAL_PERIPH_ID_DMA2     = 1
	,HAL_PERIPH_ID_SRAM     = 2
	,HAL_PERIPH_ID_RESRV3   = 3
	,HAL_PERIPH_ID_FLITF    = 4
	,HAL_PERIPH_ID_RESRV5   = 5
	,HAL_PERIPH_ID_CRC      = 6
	,HAL_PERIPH_ID_RESRV7   = 7
	,HAL_PERIPH_ID_FSMC     = 8
	,HAL_PERIPH_ID_RESRV9   = 9
	,HAL_PERIPH_ID_SDIO     = 10

	,HAL_PERIPH_ID_AFIO     = 32
	,HAL_PERIPH_ID_GPIOA    = 34
	,HAL_PERIPH_ID_GPIOB    = 35
	,HAL_PERIPH_ID_GPIOC    = 36
	,HAL_PERIPH_ID_GPIOD    = 37
	,HAL_PERIPH_ID_GPIOE    = 38
	,HAL_PERIPH_ID_GPIOF    = 39
	,HAL_PERIPH_ID_GPIOG    = 40
	,HAL_PERIPH_ID_ADC1     = 41
	,HAL_PERIPH_ID_ADC2     = 42
	,HAL_PERIPH_ID_TIM1     = 43
	,HAL_PERIPH_ID_SPI1     = 44
	,HAL_PERIPH_ID_TIM8     = 45
	,HAL_PERIPH_ID_USART1   = 46
	,HAL_PERIPH_ID_ADC3     = 47
	,HAL_PERIPH_ID_TIM15    = 48
	,HAL_PERIPH_ID_TIM16    = 49
	,HAL_PERIPH_ID_TIM17    = 50
	,HAL_PERIPH_ID_TIM9     = 51
	,HAL_PERIPH_ID_TIM10    = 52
	,HAL_PERIPH_ID_TIM11    = 53

	,HAL_PERIPH_ID_TIM2     = 64
	,HAL_PERIPH_ID_TIM3     = 65
	,HAL_PERIPH_ID_TIM4     = 66
	,HAL_PERIPH_ID_TIM5     = 67
	,HAL_PERIPH_ID_TIM6     = 68
	,HAL_PERIPH_ID_TIM7     = 69
	,HAL_PERIPH_ID_TIM12    = 70
	,HAL_PERIPH_ID_TIM13    = 71
	,HAL_PERIPH_ID_TIM14    = 72
	,HAL_PERIPH_ID_WWDG     = 75
	,HAL_PERIPH_ID_SPI2     = 78
	,HAL_PERIPH_ID_SPI3     = 79
	,HAL_PERIPH_ID_USART2   = 81
	,HAL_PERIPH_ID_USART3   = 82
	,HAL_PERIPH_ID_UART4    = 83
	,HAL_PERIPH_ID_UART5    = 84
	,HAL_PERIPH_ID_I2C1     = 85
	,HAL_PERIPH_ID_I2C2     = 86
	,HAL_PERIPH_ID_USB      = 87
	,HAL_PERIPH_ID_CAN1     = 89
	,HAL_PERIPH_ID_CAN2     = 90
	,HAL_PERIPH_ID_BKP      = 91
	,HAL_PERIPH_ID_PWR      = 92
	,HAL_PERIPH_ID_DAC      = 93
	,HAL_PERIPH_ID_CEC      = 94
	
} em_rccr_periph_t;
//! @}

//! \name rccr port 
//! @{
typedef enum 
{
     RCCRCLK_FREQ_HSI        = ((uint8_t)0x00)    //!< STM32 HSI
    ,RCCRCLK_FREQ_HSE        = ((uint8_t)0x01)    //!< STM32 HSE
    ,RCCRCLK_FREQ_24M        = ((uint8_t)0x02)    //!< STM32 24MHz
    ,RCCRCLK_FREQ_36M        = ((uint8_t)0x03)    //!< STM32 36MHz
    ,RCCRCLK_FREQ_48M        = ((uint8_t)0x04)    //!< STM32 48MHz
    ,RCCRCLK_FREQ_56M        = ((uint8_t)0x05)    //!< STM32 56MHz
    ,RCCRCLK_FREQ_72M        = ((uint8_t)0x06)    //!< STM32 72MHz
	
} em_rccr_freq_t;
//! @}

//! \name rccr MCO Config 
//! @{
typedef enum 
{
     RCCR_MCO_NoClock        = ((uint8_t)0x00)    //!< STM32 No clock selected
    ,RCCR_MCO_SYSCLK         = ((uint8_t)0x04)    //!< STM32 System clock selected
    ,RCCR_MCO_HSI            = ((uint8_t)0x05)    //!< STM32 HSI oscillator clock selected
    ,RCCR_MCO_HSE            = ((uint8_t)0x06)    //!< STM32 HSE oscillator clock selected
    ,RCCR_MCO_PLLCLK_Div2    = ((uint8_t)0x07)    //!< STM32 PLL clock divided by 2 selected
	
} em_rccr_mcocfg_t;
//! @}

//! \name Reset and Clock Control
//! @{
//!
typedef struct {
  __IO uint32_t CR;			// Ê±ÖÓ¿ØÖÆ¼Ä´æÆ÷
  __IO uint32_t CFGR;		// Ê±ÖÓÅäÖÃ¼Ä´æÆ÷
  __IO uint32_t CIR;		// Ê±ÖÓÖÐ¶Ï¼Ä´æÆ÷
  __IO uint32_t APB2RSTR;	// APB2ÍâÉè¸´Î»¼Ä´æÆ÷
  __IO uint32_t APB1RSTR;	// APB1ÍâÉè¸´Î»¼Ä´æÆ÷
  __IO uint32_t AHBENR;		// AHBÍâÉèÊ±ÖÓÊ¹ÄÜ¼Ä´æÆ÷
  __IO uint32_t APB2ENR;	// Ê±ÖÓÅäÖÃ¼Ä´æÆ÷
  __IO uint32_t APB1ENR;	// Ê±ÖÓÅäÖÃ¼Ä´æÆ÷
  __IO uint32_t BDCR;		// Ê±ÖÓÅäÖÃ¼Ä´æÆ÷
  __IO uint32_t CSR;		// Ê±ÖÓÅäÖÃ¼Ä´æÆ÷

#ifdef STM32F10X_CL  
  __IO uint32_t AHBRSTR;	// AHBÍâÉèÊ±ÖÓ¸´Î»¼Ä´æÆ÷
  __IO uint32_t CFGR2;		// Ê±ÖÓÅäÖÃ¼Ä´æÆ÷2
#endif /* STM32F10X_CL */ 

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)   
  uint32_t RESERVED0;		// Ô¤Áô¼Ä´æÆ÷
  __IO uint32_t CFGR2;		// Ê±ÖÓÅäÖÃ¼Ä´æÆ÷2
#endif /* STM32F10X_LD_VL || STM32F10X_MD_VL || STM32F10X_HD_VL */ 
} rccr_reg_t;
//! @}

//! \name rccr configuration structure
//! @{
typedef struct {
	uint32_t SYSCLK_Value;  /*!< returns SYSCLK clock frequency expressed in Hz */
	uint32_t HCLK_Value;    /*!< returns HCLK clock frequency expressed in Hz */
	uint32_t PCLK1_Value;   /*!< returns PCLK1 clock frequency expressed in Hz */
	uint32_t PCLK2_Value;   /*!< returns PCLK2 clock frequency expressed in Hz */
	uint32_t ADCCLK_Value;  /*!< returns ADCCLK clock frequency expressed in Hz */
	
} rccr_cfg_t;
//! @}

/*============================ INTERFACE =====================================*/
//! \brief Macro for rccr interface function prototypes
#define __EXTERN_RCCR_INTERFACE(__N,__DATA)                                     \
    extern bool rccr##__N##_init(uint32_t dwXTAL, uint32_t dwPLL);              \
    extern fsm_rt_t rccr##__N##_open(uint32_t pwr_clk_id);                      \
    extern fsm_rt_t rccr##__N##_close(uint32_t pwr_clk_id);                     \
    extern fsm_rt_t rccr##__N##_reset(uint32_t pwr_clk_id,                      \
		                              FunctionalState NewState);                \
    extern fsm_rt_t rccr##__N##_get_freq(rccr_cfg_t *ptRccrCfg);                \
    extern fsm_rt_t rccr##__N##_get_coreclk(uint32_t *ptCoreCLK);               \
    extern fsm_rt_t rccr##__N##_get_rst_flag(uint8_t *ptRSTFlag);               \
    extern fsm_rt_t rccr##__N##_set_mcocfg(uint32_t dwMcoCfg);     

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: rccr_t
//! @{
DEF_INTERFACE(rccr_t)
    bool         (*Init)(uint32_t dwXTAL, uint32_t dwPLL);    
													//!< initialize rccr
    fsm_rt_t     (*Open)(uint32_t pwr_clk_id);      //!< open rccr 
    fsm_rt_t     (*Close)(uint32_t pwr_clk_id);     //!< close rccr
    fsm_rt_t     (*Reset)(uint32_t pwr_clk_id, FunctionalState NewState);     
													//!< reset rccr
    fsm_rt_t     (*GetFreq)(rccr_cfg_t *ptRccrCfg); //! method for ...
    fsm_rt_t     (*GetCoreCLK)(uint32_t *ptCoreCLK);//! method for ...
    fsm_rt_t     (*GetRstFlag)(uint8_t *ptRSTFlag); //! method for ...
    fsm_rt_t     (*SetMCOCfg)(uint32_t dwMcoCfg);   //! method for ...
    rccr_reg_t * const ptRegPage;                   //!< reference to register page
	
END_DEF_INTERFACE(rccr_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief rccr interface function prototypes
MREPEAT(RCCR_COUNT, __EXTERN_RCCR_INTERFACE, NULL)


/*============================ HAL_RCCR ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief rccr object (interface function)
extern ROOT
const rccr_t RCCR[RCCR_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* GSF_GSF_RCCR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
