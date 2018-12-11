/**
  ******************************************************************************
  * @file              : gsf_gpio.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:58:57
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
#ifndef GSF_GSF_GPIO_H
#define GSF_GSF_GPIO_H

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
//!                         	(Special symbols are not allowed here)
#define GPIO_COUNT           	7
#define GPIO0_BASE_ADDRESS    	GPIOA_BASE
#define GPIO1_BASE_ADDRESS     	GPIOB_BASE
#define GPIO2_BASE_ADDRESS    	GPIOC_BASE
#define GPIO3_BASE_ADDRESS     	GPIOD_BASE
#define GPIO4_BASE_ADDRESS    	GPIOE_BASE
#define GPIO5_BASE_ADDRESS    	GPIOF_BASE
#define GPIO6_BASE_ADDRESS    	GPIOG_BASE
//! @}

//! \name gpio remap mode (ref for 'GPIO_Remap_')
//! @{
#define HAL_GPIO_Remap_NONE             ((uint32_t)0x00000000) 
#define HAL_GPIO_Remap_SPI1             ((uint32_t)0x00000001)  /*!< SPI1 Alternate Function mapping */
#define HAL_GPIO_Remap_I2C1             ((uint32_t)0x00000002)  /*!< I2C1 Alternate Function mapping */
#define HAL_GPIO_Remap_USART1           ((uint32_t)0x00000004)  /*!< USART1 Alternate Function mapping */
#define HAL_GPIO_Remap_USART2           ((uint32_t)0x00000008)  /*!< USART2 Alternate Function mapping */
#define HAL_GPIO_PartialRemap_USART3    ((uint32_t)0x00140010)  /*!< USART3 Partial Alternate Function mapping */
#define HAL_GPIO_FullRemap_USART3       ((uint32_t)0x00140030)  /*!< USART3 Full Alternate Function mapping */
#define HAL_GPIO_PartialRemap_TIM1      ((uint32_t)0x00160040)  /*!< TIM1 Partial Alternate Function mapping */
#define HAL_GPIO_FullRemap_TIM1         ((uint32_t)0x001600C0)  /*!< TIM1 Full Alternate Function mapping */
#define HAL_GPIO_PartialRemap1_TIM2     ((uint32_t)0x00180100)  /*!< TIM2 Partial1 Alternate Function mapping */
#define HAL_GPIO_PartialRemap2_TIM2     ((uint32_t)0x00180200)  /*!< TIM2 Partial2 Alternate Function mapping */
#define HAL_GPIO_FullRemap_TIM2         ((uint32_t)0x00180300)  /*!< TIM2 Full Alternate Function mapping */
#define HAL_GPIO_PartialRemap_TIM3      ((uint32_t)0x001A0800)  /*!< TIM3 Partial Alternate Function mapping */
#define HAL_GPIO_FullRemap_TIM3         ((uint32_t)0x001A0C00)  /*!< TIM3 Full Alternate Function mapping */
#define HAL_GPIO_Remap_TIM4             ((uint32_t)0x00001000)  /*!< TIM4 Alternate Function mapping */
#define HAL_GPIO_Remap1_CAN1            ((uint32_t)0x001D4000)  /*!< CAN1 Alternate Function mapping */
#define HAL_GPIO_Remap2_CAN1            ((uint32_t)0x001D6000)  /*!< CAN1 Alternate Function mapping */
#define HAL_GPIO_Remap_PD01             ((uint32_t)0x00008000)  /*!< PD01 Alternate Function mapping */
#define HAL_GPIO_Remap_TIM5CH4_LSI      ((uint32_t)0x00200001)  /*!< LSI connected to TIM5 Channel4 input capture for calibration */
#define HAL_GPIO_Remap_ADC1_ETRGINJ     ((uint32_t)0x00200002)  /*!< ADC1 External Trigger Injected Conversion remapping */
#define HAL_GPIO_Remap_ADC1_ETRGREG     ((uint32_t)0x00200004)  /*!< ADC1 External Trigger Regular Conversion remapping */
#define HAL_GPIO_Remap_ADC2_ETRGINJ     ((uint32_t)0x00200008)  /*!< ADC2 External Trigger Injected Conversion remapping */
#define HAL_GPIO_Remap_ADC2_ETRGREG     ((uint32_t)0x00200010)  /*!< ADC2 External Trigger Regular Conversion remapping */
#define HAL_GPIO_Remap_ETH              ((uint32_t)0x00200020)  /*!< Ethernet remapping (only for Connectivity line devices) */
#define HAL_GPIO_Remap_CAN2             ((uint32_t)0x00200040)  /*!< CAN2 remapping (only for Connectivity line devices) */
#define HAL_GPIO_Remap_SWJ_NoJTRST      ((uint32_t)0x00300100)  /*!< Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST */
#define HAL_GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */
#define HAL_GPIO_Remap_SWJ_Disable      ((uint32_t)0x00300400)  /*!< Full SWJ Disabled (JTAG-DP + SW-DP) */
#define HAL_GPIO_Remap_SPI3             ((uint32_t)0x00201100)  /*!< SPI3/I2S3 Alternate Function mapping (only for Connectivity line devices) */
#define HAL_GPIO_Remap_TIM2ITR1_PTP_SOF ((uint32_t)0x00202000)  /*!< Ethernet PTP output or USB OTG SOF (Start of Frame) connected
                                                                 to TIM2 Internal Trigger 1 for calibration
                                                                 (only for Connectivity line devices) */
#define HAL_GPIO_Remap_PTP_PPS          ((uint32_t)0x00204000)  /*!< Ethernet MAC PPS_PTS output on PB05 (only for Connectivity line devices) */

#define HAL_GPIO_Remap_TIM15            ((uint32_t)0x80000001)  /*!< TIM15 Alternate Function mapping (only for Value line devices) */
#define HAL_GPIO_Remap_TIM16            ((uint32_t)0x80000002)  /*!< TIM16 Alternate Function mapping (only for Value line devices) */
#define HAL_GPIO_Remap_TIM17            ((uint32_t)0x80000004)  /*!< TIM17 Alternate Function mapping (only for Value line devices) */
#define HAL_GPIO_Remap_CEC              ((uint32_t)0x80000008)  /*!< CEC Alternate Function mapping (only for Value line devices) */
#define HAL_GPIO_Remap_TIM1_DMA         ((uint32_t)0x80000010)  /*!< TIM1 DMA requests mapping (only for Value line devices) */

#define HAL_GPIO_Remap_TIM9             ((uint32_t)0x80000020)  /*!< TIM9 Alternate Function mapping (only for XL-density devices) */
#define HAL_GPIO_Remap_TIM10            ((uint32_t)0x80000040)  /*!< TIM10 Alternate Function mapping (only for XL-density devices) */
#define HAL_GPIO_Remap_TIM11            ((uint32_t)0x80000080)  /*!< TIM11 Alternate Function mapping (only for XL-density devices) */
#define HAL_GPIO_Remap_TIM13            ((uint32_t)0x80000100)  /*!< TIM13 Alternate Function mapping (only for High density Value line and XL-density devices) */
#define HAL_GPIO_Remap_TIM14            ((uint32_t)0x80000200)  /*!< TIM14 Alternate Function mapping (only for High density Value line and XL-density devices) */
#define HAL_GPIO_Remap_FSMC_NADV        ((uint32_t)0x80000400)  /*!< FSMC_NADV Alternate Function mapping (only for High density Value line and XL-density devices) */

#define HAL_GPIO_Remap_TIM67_DAC_DMA    ((uint32_t)0x80000800)  /*!< TIM6/TIM7 and DAC DMA requests remapping (only for High density Value line devices) */
#define HAL_GPIO_Remap_TIM12            ((uint32_t)0x80001000)  /*!< TIM12 Alternate Function mapping (only for High density Value line devices) */
#define HAL_GPIO_Remap_MISC             ((uint32_t)0x80002000)  /*!< Miscellaneous Remap (DMA2 Channel5 Position and DAC Trigger remapping, 
                                                                 only for High density Value line devices) */                                                       
//! @}

/*============================ TYPES =========================================*/
//! \name gpio port 
//! @{
typedef enum 
{
     HAL_GPIOA         = ((uint8_t)0x00)    //!< STM32 GPIOA
    ,HAL_GPIOB         = ((uint8_t)0x01)    //!< STM32 GPIOB
    ,HAL_GPIOC         = ((uint8_t)0x02)    //!< STM32 GPIOC
    ,HAL_GPIOD         = ((uint8_t)0x03)    //!< STM32 GPIOD
    ,HAL_GPIOE         = ((uint8_t)0x04)    //!< STM32 GPIOE
    ,HAL_GPIOF         = ((uint8_t)0x05)    //!< STM32 GPIOF
    ,HAL_GPIOG         = ((uint8_t)0x06)    //!< STM32 GPIOG
} em_gpio_port_t;
//! @}

//! \name gpio clock (ref for 'RCC_APB2Periph_GPIOA')
//! @{
/*
typedef enum 
{
     HAL_RCC_CLK_GPIOA      = ((uint32_t)0x00000004)    //!< GPIOA RCC CLOCK
    ,HAL_RCC_CLK_GPIOB      = ((uint32_t)0x00000008)    //!< GPIOB RCC CLOCK
    ,HAL_RCC_CLK_GPIOC      = ((uint32_t)0x00000010)    //!< GPIOC RCC CLOCK
    ,HAL_RCC_CLK_GPIOD      = ((uint32_t)0x00000020)    //!< GPIOD RCC CLOCK
    ,HAL_RCC_CLK_GPIOE      = ((uint32_t)0x00000040)    //!< GPIOE RCC CLOCK
    ,HAL_RCC_CLK_GPIOF      = ((uint32_t)0x00000080)    //!< GPIOF RCC CLOCK
    ,HAL_RCC_CLK_GPIOG      = ((uint32_t)0x00000100)    //!< GPIOG RCC CLOCK
} em_gpio_clock_t;
*/
//! @}

//! \name gpio pins 
//! @{
typedef enum 
{
	 HAL_GPIO_Pin_0         = _BV(0)  /*!< Pin 0 selected */
	,HAL_GPIO_Pin_1         = _BV(1)  /*!< Pin 1 selected */
	,HAL_GPIO_Pin_2         = _BV(2)  /*!< Pin 2 selected */
	,HAL_GPIO_Pin_3         = _BV(3)  /*!< Pin 3 selected */
	,HAL_GPIO_Pin_4         = _BV(4)  /*!< Pin 4 selected */
	,HAL_GPIO_Pin_5         = _BV(5)  /*!< Pin 5 selected */
	,HAL_GPIO_Pin_6         = _BV(6)  /*!< Pin 6 selected */
	,HAL_GPIO_Pin_7         = _BV(7)  /*!< Pin 7 selected */
	,HAL_GPIO_Pin_8         = _BV(8)  /*!< Pin 8 selected */
	,HAL_GPIO_Pin_9         = _BV(9)  /*!< Pin 9 selected */
	,HAL_GPIO_Pin_10        = _BV(10) /*!< Pin 10 selected */
	,HAL_GPIO_Pin_11        = _BV(11) /*!< Pin 11 selected */
	,HAL_GPIO_Pin_12        = _BV(12) /*!< Pin 12 selected */
	,HAL_GPIO_Pin_13        = _BV(13) /*!< Pin 13 selected */
	,HAL_GPIO_Pin_14        = _BV(14) /*!< Pin 14 selected */
	,HAL_GPIO_Pin_15        = _BV(15) /*!< Pin 15 selected */
	,HAL_GPIO_Pin_ALL       = ((uint16_t)0xFFFF)  /*!< All pins selected */
} em_gpio_pin_t;
//! @}

//! \name gpio cfg speed (ref for 'GPIOSpeed_TypeDef')
//! @{
typedef enum {
	 HAL_GPIO_Speed_10MHz = 1
	,HAL_GPIO_Speed_2MHz
	,HAL_GPIO_Speed_50MHz

} em_gpio_speed_t;
//! @}

//! \name gpio cfg mode (ref for 'GPIOMode_TypeDef')
//! @{
typedef enum {
     HAL_GPIO_Mode_AIN      = _BV(0)        //!< 模拟输入
    ,HAL_GPIO_Mode_IN_FLOATING        
	                        = _BV(2)        //!< 浮空输入 (Default Mode) 
          
    ,HAL_GPIO_Mode_IPD      = _BVP(3,5)     //!< 上拉/下拉输入
    ,HAL_GPIO_Mode_IPU      = _BVP(4,5)     //!< 上拉/下拉输入
    
    ,HAL_GPIO_Mode_Out_OD   = _BVP(2,4)     //!< 通用开漏输出
    ,HAL_GPIO_Mode_Out_PP   = _BV(4)        //!< 通用推挽输出
    
    ,HAL_GPIO_Mode_AF_OD    = _BME(2,4)     //!< 复用功能推挽输出
    ,HAL_GPIO_Mode_AF_PP    = _BVP(3,4)     //!< 复用功能开漏输出
    
} em_gpio_mode_t;
//! @}

//! \name GPIO Register
//! @{
typedef struct {
    volatile uint16_t Value;
    uint16_t Reserved;
} hw_type_t;
//!
typedef struct {
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	hw_type_t         IDR;
	hw_type_t         ODR;
	hw_type_t         BSRR;
	hw_type_t         BRR;
	hw_type_t         LCKR;
} gpio_reg_t;
//! @}

//! \name gpio configuration structure
//! @{
typedef struct
{
	uint32_t  GPIO_RCC;

	uint16_t  GPIO_Pin;   /*!< Specifies the GPIO pins to be configured.
                               This parameter can be any value of @ref GPIO_pins_define */
	uint8_t  GPIO_Mode;   /*!< Specifies the operating mode for the selected pins.
                               This parameter can be a value of @ref GPIOMode_TypeDef */
	uint8_t GPIO_Speed;	  /*!< Specifies the speed for the selected pins.
                               This parameter can be a value of @ref GPIOSpeed_TypeDef */
	uint32_t GPIO_Remap;
	
	uint8_t   GPIO_DVal;
} gpio_cfg_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for extern gpio interface function prototypes
#define __EXTERN_GPIO_INTERFACE(__N, __DATA)                                    \
    extern bool gpio##__N##_init(gpio_cfg_t *ptCfg);                            \
    extern fsm_rt_t gpio##__N##_open(void);                                     \
    extern fsm_rt_t gpio##__N##_close(void);                                    \
    extern bool gpio##__N##_toggle(uint16_t hwPin);                             \
    extern bool gpio##__N##_read(uint16_t hwPin,uint16_t *hwIn);                \
    extern bool gpio##__N##_write(uint16_t hwPin, uint16_t hwOut);

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: gpio_t
//! @{
DEF_INTERFACE(gpio_t)
    bool         (*Init)(gpio_cfg_t *ptGpioCfg);    //!< initialize gpio
    fsm_rt_t     (*Open)(void);                     //!< open gpio 
    fsm_rt_t     (*Close)(void);                    //!< close gpio
    bool         (*Toggle)(uint16_t hwPin);         
    bool         (*Read)(uint16_t hwPin,uint16_t *hwIn);          
	                                                //!< read data from gpio buffer 
    bool         (*Write)(uint16_t hwPin, uint16_t hwOut);          
                                                    //!< write data to gpio buffer
    gpio_reg_t * const ptRegPage;                   //!< reference to register page

END_DEF_INTERFACE(gpio_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief gpio interface function prototypes
MREPEAT(GPIO_COUNT, __EXTERN_GPIO_INTERFACE, NULL)


/*============================ HAL_GPIO ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief gpio object
extern ROOT 
const gpio_t GPIO[GPIO_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* GSF_GSF_GPIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
