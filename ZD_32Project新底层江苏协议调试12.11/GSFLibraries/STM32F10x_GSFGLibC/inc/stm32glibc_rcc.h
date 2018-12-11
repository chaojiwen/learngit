/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_rcc.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : RCC firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_RCC_H
#define __STM32GLIBC_RCC_H

#include "stm32glibc_type.h"


#if (false)
/* Exported constants --------------------------------------------------------*/
/* GPIO pins define ----------------------------------------------------------*/
//!
//! @{
#define RCC_AHBPeriph_DMA1        		((uint32_t)0x00000001)
#define RCC_AHBPeriph_DMA2            	((uint32_t)0x00000002)
#define RCC_AHBPeriph_SRAM           	((uint32_t)0x00000004)
#define RCC_AHBPeriph_FLITF          	((uint32_t)0x00000010)
#define RCC_AHBPeriph_CRC            	((uint32_t)0x00000040)

#ifndef STM32F10X_CL
#define RCC_AHBPeriph_FSMC          	((uint32_t)0x00000100)
#define RCC_AHBPeriph_SDIO          	((uint32_t)0x00000400)
#else
#define RCC_AHBPeriph_OTG_FS         	((uint32_t)0x00001000)
#define RCC_AHBPeriph_ETH_MAC        	((uint32_t)0x00004000)
#define RCC_AHBPeriph_ETH_MAC_Tx    	((uint32_t)0x00008000)
#define RCC_AHBPeriph_ETH_MAC_Rx     	((uint32_t)0x00010000)
#endif /* STM32F10X_CL */
//! @}
//!
//! @{
#define RCC_APB2Periph_AFIO             ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA            ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB            ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC            ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD            ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE            ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF            ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG            ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC1             ((uint32_t)0x00000200)
#define RCC_APB2Periph_ADC2             ((uint32_t)0x00000400)
#define RCC_APB2Periph_TIM1             ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1             ((uint32_t)0x00001000)
#define RCC_APB2Periph_TIM8             ((uint32_t)0x00002000)
#define RCC_APB2Periph_USART1           ((uint32_t)0x00004000)
#define RCC_APB2Periph_ADC3             ((uint32_t)0x00008000)
#define RCC_APB2Periph_TIM15            ((uint32_t)0x00010000)
#define RCC_APB2Periph_TIM16            ((uint32_t)0x00020000)
#define RCC_APB2Periph_TIM17            ((uint32_t)0x00040000)
#define RCC_APB2Periph_TIM9             ((uint32_t)0x00080000)
#define RCC_APB2Periph_TIM10            ((uint32_t)0x00100000)
#define RCC_APB2Periph_TIM11            ((uint32_t)0x00200000)
//! @}
//!
//! @{
#define RCC_APB1Periph_TIM2             ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3             ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4             ((uint32_t)0x00000004)
#define RCC_APB1Periph_TIM5             ((uint32_t)0x00000008)
#define RCC_APB1Periph_TIM6             ((uint32_t)0x00000010)
#define RCC_APB1Periph_TIM7             ((uint32_t)0x00000020)
#define RCC_APB1Periph_TIM12            ((uint32_t)0x00000040)
#define RCC_APB1Periph_TIM13            ((uint32_t)0x00000080)
#define RCC_APB1Periph_TIM14            ((uint32_t)0x00000100)
#define RCC_APB1Periph_WWDG             ((uint32_t)0x00000800)
#define RCC_APB1Periph_SPI2             ((uint32_t)0x00004000)
#define RCC_APB1Periph_SPI3             ((uint32_t)0x00008000)
#define RCC_APB1Periph_USART2           ((uint32_t)0x00020000)
#define RCC_APB1Periph_USART3           ((uint32_t)0x00040000)
#define RCC_APB1Periph_UART4            ((uint32_t)0x00080000)
#define RCC_APB1Periph_UART5            ((uint32_t)0x00100000)
#define RCC_APB1Periph_I2C1             ((uint32_t)0x00200000)
#define RCC_APB1Periph_I2C2             ((uint32_t)0x00400000)
#define RCC_APB1Periph_USB              ((uint32_t)0x00800000)
#define RCC_APB1Periph_CAN1             ((uint32_t)0x02000000)
#define RCC_APB1Periph_CAN2             ((uint32_t)0x04000000)
#define RCC_APB1Periph_BKP              ((uint32_t)0x08000000)
#define RCC_APB1Periph_PWR              ((uint32_t)0x10000000)
#define RCC_APB1Periph_DAC              ((uint32_t)0x20000000)
#define RCC_APB1Periph_CEC              ((uint32_t)0x40000000)
//! @}

/**
 * @brief In the following line adjust the value of External High Speed oscillator (HSE)
   used in your application

   Tip: To avoid modifying this file each time you need to use different HSE, you
        can define the HSE value in your toolchain compiler preprocessor.
  */
#if !defined  HSE_VALUE
#ifdef STM32F10X_CL
#define HSE_VALUE    ((uint32_t)25000000) /*!< Value of the External oscillator in Hz */
#else
#define HSE_VALUE    ((uint32_t)8000000) /*!< Value of the External oscillator in Hz */
#endif /* STM32F10X_CL */
#endif /* HSE_VALUE */


/**
 * @brief In the following line adjust the External High Speed oscillator (HSE) Startup
   Timeout value
   */
#define HSE_STARTUP_TIMEOUT   ((uint16_t)0x0500) /*!< Time out for HSE start up */

#define HSI_VALUE    ((uint32_t)8000000) /*!< Value of the Internal oscillator in Hz*/
#endif

namespace periph_c
{

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 时钟控制寄存器(RCC_CR)
    struct RCC_CR_Struct
    {
        volatile uint32_t HSION                         : 1; //Bit0
        volatile uint32_t HSIRDY                        : 1; //Bit1
        volatile uint32_t Resv1                         : 1; //Bit2
        volatile uint32_t HSITRIM                       : 5; //Bit3 ~ Bit7
        volatile uint32_t HSICAL                        : 8; //Bit8 ~ Bit15
        volatile uint32_t HSEON                         : 1; //Bit16
        volatile uint32_t HSERDY                        : 1; //Bit17
        volatile uint32_t HSEBYP                        : 1; //Bit18
        volatile uint32_t CSSON                         : 1; //Bit19
        volatile uint32_t Resv2                         : 4; //Bit20 ~ Bit23
        volatile uint32_t PLLON                         : 1; //Bit24
        volatile uint32_t PLLRDY                        : 1; //Bit25
        volatile uint32_t PLL2ON                        : 1; //Bit26
        volatile uint32_t PLL2RDY                       : 1; //Bit27
        volatile uint32_t PLL3ON                        : 1; //Bit28
        volatile uint32_t PLL3RDY                       : 1; //Bit29
        volatile uint32_t Resv3                         : 2; //Bit30 ~ Bit31
    };

    typedef struct RCC_CR_Struct RCC_CR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 时钟控制寄存器(RCC_CFGR)
    struct RCC_CFGR_Struct
    {
        volatile uint32_t SW                            : 2; //Bit0 ~ Bit1
        volatile uint32_t SWS                           : 2; //Bit2 ~ Bit3
        volatile uint32_t HPRE                          : 4; //Bit4 ~ Bit7
        volatile uint32_t HPRE1                         : 3; //Bit8 ~ Bit10
        volatile uint32_t HPRE2                         : 3; //Bit11 ~ Bit13
        volatile uint32_t ADCPRE                        : 2; //Bit14 ~ Bit15
        volatile uint32_t PLLSRC                        : 1; //Bit16
        volatile uint32_t PLLXTPRE                      : 1; //Bit17
        volatile uint32_t PLLMUL                        : 4; //Bit18 ~ Bit21
        volatile uint32_t OTGFSPRE                      : 1; //Bit22
        volatile uint32_t Resv1                         : 1; //Bit23
        volatile uint32_t MCO                           : 4; //Bit24 ~ Bit27
        volatile uint32_t Resv2                         : 4; //Bit28 ~ Bit31
    };

    typedef struct RCC_CFGR_Struct RCC_CFGR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 时钟中断寄存器(RCC_CIR)
    struct RCC_CIR_Struct
    {
        volatile uint32_t LSIRDYF                       : 1; //Bit0
        volatile uint32_t LSERDYF                       : 1; //Bit1
        volatile uint32_t HSIRDYF                       : 1; //Bit2
        volatile uint32_t HSERDYF                       : 1; //Bit3
        volatile uint32_t PLLRDYF                       : 1; //Bit4
        volatile uint32_t PLL2RDYF                      : 1; //Bit5
        volatile uint32_t PLL3RDYF                      : 1; //Bit6
        volatile uint32_t CSSF                          : 1; //Bit7
        volatile uint32_t LSIRDYIE                      : 1; //Bit8
        volatile uint32_t LSERDYIE                      : 1; //Bit9
        volatile uint32_t HSIRDYIE                      : 1; //Bit10
        volatile uint32_t HSERDYIE                      : 1; //Bit11
        volatile uint32_t PLLRDYIE                      : 1; //Bit12
        volatile uint32_t PLL2RDYIE                     : 1; //Bit13
        volatile uint32_t PLL3RDYIE                     : 1; //Bit14
        volatile uint32_t Resv1                         : 1; //Bit15
        volatile uint32_t LSIRDYC                       : 1; //Bit16
        volatile uint32_t LSERDYC                       : 1; //Bit17
        volatile uint32_t HSIRDYC                       : 1; //Bit18
        volatile uint32_t HSERDYC                       : 1; //Bit19
        volatile uint32_t PLLRDYC                       : 1; //Bit20
        volatile uint32_t PLL2RDYC                      : 1; //Bit21
        volatile uint32_t PLL3RDYC                      : 1; //Bit22
        volatile uint32_t CSSC                          : 1; //Bit23
        volatile uint32_t Resv16_31                     : 8; //Bit24 ~ Bit31
    };

    typedef struct RCC_CIR_Struct RCC_CIR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! AHB外设时钟复位寄存器(RCC_AHBRSTR)
    struct RCC_AHBRSTR_Struct
    {
        volatile uint32_t Resv1                         : 12; //Bit0 ~ Bit11
        volatile uint32_t OTGFSRST                      : 1; //Bit12
        volatile uint32_t Resv2                         : 1; //Bit13
        volatile uint32_t ETHMACRST                     : 1; //Bit14
        volatile uint32_t Resv3                         : 1; //Bit15
        volatile uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct RCC_AHBRSTR_Struct RCC_AHBRSTR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! APB2外设复位寄存器(RCC_APB2RSTR)
    struct RCC_APB2RSTR_Struct
    {
        volatile uint32_t AFIORST                       : 1; //Bit0
        volatile uint32_t Resv1                         : 1; //Bit1
        volatile uint32_t IOPARST                       : 1; //Bit2
        volatile uint32_t IOPBRST                       : 1; //Bit3
        volatile uint32_t IOPCRST                       : 1; //Bit4
        volatile uint32_t IOPDRST                       : 1; //Bit5
        volatile uint32_t IOPERST                       : 1; //Bit6
        volatile uint32_t Resv2                         : 2; //Bit7 ~ bIT8
        volatile uint32_t ADC1RST                       : 1; //Bit9
        volatile uint32_t ADC2RST                       : 1; //Bit10
        volatile uint32_t TIM1RST                       : 1; //Bit11
        volatile uint32_t SPI1RST                       : 1; //Bit12
        volatile uint32_t Resv3                         : 1; //Bit13
        volatile uint32_t USART1RST                     : 1; //Bit14
        volatile uint32_t Resv4                         : 1; //Bit15
        volatile uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct RCC_APB2RSTR_Struct RCC_APB2RSTR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! APB1外设复位寄存器(RCC_APB1RSTR)
    struct RCC_APB1RSTR_Struct
    {
        volatile uint32_t TIM2RST                       : 1; //Bit0
        volatile uint32_t TIM3RST                       : 1; //Bit1
        volatile uint32_t TIM4RST                       : 1; //Bit2
        volatile uint32_t TIM5RST                       : 1; //Bit3
        volatile uint32_t TIM6RST                       : 1; //Bit4
        volatile uint32_t TIM7RST                       : 1; //Bit5
        volatile uint32_t Resv1                         : 5; //Bit6 ~ Bit10
        volatile uint32_t WWDGRST                       : 1; //Bit11
        volatile uint32_t Resv2                         : 2; //Bit12 ~ Bit13
        volatile uint32_t SPI2RST                       : 1; //Bit14
        volatile uint32_t SPI3RST                       : 1; //Bit15
        volatile uint32_t Resv3                         : 1; //Bit16
        volatile uint32_t USART2RST                     : 1; //Bit17
        volatile uint32_t USART3RST                     : 1; //Bit18
        volatile uint32_t UART4RST                      : 1; //Bit19
        volatile uint32_t UART5RST                      : 1; //Bit20
        volatile uint32_t I2C1RST                       : 1; //Bit21
        volatile uint32_t I2C2RST                       : 1; //Bit22
        volatile uint32_t Resv4                         : 2; //Bit23 ~ Bit24
        volatile uint32_t CAN1RST                       : 1; //Bit25
        volatile uint32_t CAN2RST                       : 1; //Bit26
        volatile uint32_t BKPRST                        : 1; //Bit27
        volatile uint32_t PWRRST                        : 1; //Bit28
        volatile uint32_t DACRST                        : 1; //Bit29
        volatile uint32_t Resv5                         : 2; //Bit30 ~ Bit31
    };

    typedef struct RCC_APB1RSTR_Struct RCC_APB1RSTR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! AHB外设时钟使能寄存器(RCC_AHBENR)
    struct RCC_AHBENR_Struct
    {
        volatile uint32_t DMA1EN                        : 1; //Bit0
        volatile uint32_t DMA2EN                        : 1; //Bit1
        volatile uint32_t SRAMEN                        : 1; //Bit2
        volatile uint32_t Resv1                         : 1; //Bit3
        volatile uint32_t FLITFEN                       : 1; //Bit4
        volatile uint32_t Resv2                         : 1; //Bit5
        volatile uint32_t CRCEN                         : 1; //Bit6
        volatile uint32_t Resv3                         : 5; //Bit7 ~ Bit11
        volatile uint32_t OTGFSEN                       : 1; //Bit12
        volatile uint32_t Resv4                         : 1; //Bit13
        volatile uint32_t ETHMACEN                      : 1; //Bit14
        volatile uint32_t ETHMACTXEN                    : 1; //Bit15
        volatile uint32_t ETHMACRXEN                    : 1; //Bit16
        volatile uint32_t Resv17_31                     : 15; //Bit17 ~ Bit31
    };

    typedef struct RCC_AHBENR_Struct RCC_AHBENR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! APB2外设时钟使能寄存器(RCC_APB2ENR)
    struct RCC_APB2ENR_Struct
    {
        volatile uint32_t AFIOEN                        : 1; //Bit0
        volatile uint32_t Resv1                         : 1; //Bit1
        volatile uint32_t IOPAEN                        : 1; //Bit2
        volatile uint32_t IOPBEN                        : 1; //Bit3
        volatile uint32_t IOPCEN                        : 1; //Bit4
        volatile uint32_t IOPDEN                        : 1; //Bit5
        volatile uint32_t IOPEEN                        : 1; //Bit6
        volatile uint32_t Resv2                         : 2; //Bit7 ~ Bit8
        volatile uint32_t ADC1EN                        : 1; //Bit9
        volatile uint32_t ADC2EN                        : 1; //Bit10
        volatile uint32_t TIM1EN                        : 1; //Bit11
        volatile uint32_t SPI1EN                        : 1; //Bit12
        volatile uint32_t Resv3                         : 1; //Bit13
        volatile uint32_t USART1EN                      : 1; //Bit14
        volatile uint32_t Resv4                         : 1; //Bit15
        volatile uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct RCC_APB2ENR_Struct RCC_APB2ENR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! APB1外设时钟使能寄存器(RCC_APB1ENR)
    struct RCC_APB1ENR_Struct
    {
        volatile uint32_t TIM2EN                        : 1; //Bit0
        volatile uint32_t TIM3EN                        : 1; //Bit1
        volatile uint32_t TIM4EN                        : 1; //Bit2
        volatile uint32_t TIM5EN                        : 1; //Bit3
        volatile uint32_t TIM6EN                        : 1; //Bit4
        volatile uint32_t TIM7EN                        : 1; //Bit5
        volatile uint32_t Resv1                         : 5; //Bit6 ~ Bit10
        volatile uint32_t WWDGEN                        : 1; //Bit11
        volatile uint32_t Resv2                         : 2; //Bit12 ~ Bit13
        volatile uint32_t SPI2EN                        : 1; //Bit14
        volatile uint32_t SPI3EN                        : 1; //Bit15
        volatile uint32_t Resv3                         : 1; //Bit16
        volatile uint32_t USART2EN                      : 1; //Bit17
        volatile uint32_t USART3EN                      : 1; //Bit18
        volatile uint32_t UART4EN                       : 1; //Bit19
        volatile uint32_t UART5EN                       : 1; //Bit20
        volatile uint32_t I2C1EN                        : 1; //Bit21
        volatile uint32_t I2C2EN                        : 1; //Bit22
        volatile uint32_t Resv4                         : 2; //Bit23 ~ Bit24
        volatile uint32_t CAN1EN                        : 1; //Bit25
        volatile uint32_t CAN2EN                        : 1; //Bit26
        volatile uint32_t BKPEN                         : 1; //Bit27
        volatile uint32_t PWREN                         : 1; //Bit28
        volatile uint32_t DACEN                         : 1; //Bit29
        volatile uint32_t Resv5                         : 2; //Bit30 ~ Bit31
    };

    typedef struct RCC_APB1ENR_Struct RCC_APB1ENR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 备份域控制寄存器(RCC_BDCR)
    struct RCC_BDCR_Struct
    {
        volatile uint32_t LSEON                         : 1; //Bit0
        volatile uint32_t LSERDY                        : 1; //Bit1
        volatile uint32_t LSEBYP                        : 1; //Bit2
        volatile uint32_t Resv1                         : 5; //Bit3 ~ Bit7
        volatile uint32_t RTCSEL                        : 2; //Bit8 ~ Bit9
        volatile uint32_t Resv2                         : 5; //Bit10 ~ Bit14
        volatile uint32_t RTCEN                         : 1; //Bit15
        volatile uint32_t BDRST                         : 1; //Bit16
        volatile uint32_t Resv17_31                     : 15; //Bit17 ~ Bit31
    };

    typedef struct RCC_BDCR_Struct RCC_BDCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 状态控制寄存器(RCC_CSR)
    struct RCC_CSR_Struct
    {
        volatile uint32_t LSION                         : 1; //Bit0
        volatile uint32_t LSIRDY                        : 1; //Bit1
        volatile uint32_t Resv1                         : 14; //Bit2 ~ Bit15
        volatile uint32_t Resv2                         : 8; //Bit16 ~ Bit23
        volatile uint32_t RMVF                          : 1; //Bit24
        volatile uint32_t Resv3                         : 1; //Bit25
        volatile uint32_t PINRSTF                       : 1; //Bit26
        volatile uint32_t PORRSTF                       : 1; //Bit27
        volatile uint32_t SFTRSTF                       : 1; //Bit28
        volatile uint32_t IWDGRSTF                      : 1; //Bit29
        volatile uint32_t WWDGRSTF                      : 1; //Bit30
        volatile uint32_t LPWRRSTF                      : 1; //Bit31
    };

    typedef struct RCC_CSR_Struct RCC_CSR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 时钟配置寄存器(RCC_CFGR2)
    struct RCC_CFGR2_Struct
    {
        volatile uint32_t PREDIV1                       : 4; //Bit0 ~ Bit3
        volatile uint32_t PREDIV2                       : 4; //Bit4 ~ Bit7
        volatile uint32_t PLL2MUL                       : 4; //Bit8 ~ Bit11
        volatile uint32_t PLL3MUL                       : 4; //Bit12 ~ Bit15
        volatile uint32_t PREDIVSRC                     : 1; //Bit16
        volatile uint32_t I2S2SRC                       : 1; //Bit17
        volatile uint32_t I2S3SRC                       : 1; //Bit18
        volatile uint32_t Resv19_31                     : 13; //Bit19 ~ Bit31
    };

    typedef struct RCC_CFGR2_Struct RCC_CFGR2_Typedef;

    //!
    //! @{
    typedef enum RCC_EXTOSC
    {
        RCC_EXTOSC_OFF              	= ((uint32_t)(0x00000000)),
        RCC_EXTOSC_ON                 	= ((uint32_t)(0x00000001 << 0)),
        RCC_EXTOSC_Bypass              	= ((uint32_t)(0x00000001 << 2)),
    } RCC_EXTOSC_Mode;

    //!
    //! @{
    typedef enum RCC_CLK
    {
        RCC_Source_Null              	= ((uint32_t)(0x00000000)),
        RCC_Source_HSI                 	= ((uint32_t)(0x00000001 << 1)),
        RCC_Source_LSE                 	= ((uint32_t)(0x00000001 << 2)),
        RCC_Source_LSI                 	= ((uint32_t)(0x00000001 << 3)),
        //!
        RCC_Source_SYSCLK              	= ((uint32_t)(0x00000001 << 8)),
        RCC_Source_HCLK              	= ((uint32_t)(0x00000001 << 9)),
        RCC_Source_APB1              	= ((uint32_t)(0x00000001 << 10)),
        RCC_Source_APB2              	= ((uint32_t)(0x00000001 << 11)),
        RCC_Source_RTCCLK           	= ((uint32_t)(0x00000001 << 12)),
        RCC_Source_ADCCLK              	= ((uint32_t)(0x00000001 << 14)),
        //!
        RCC_Source_HSE                 	= ((uint32_t)(0x00000001 << 16)),
        RCC_Source_PLL                 	= ((uint32_t)(0x00000001 << 24)),
        RCC_Source_PLL2                	= ((uint32_t)(0x00000001 << 26)),
        RCC_Source_PLL3                	= ((uint32_t)(0x00000001 << 28)),
    } RCC_Clock;

    typedef struct RCC_Struct
    {
        //下面的内容不要更改，尤其是变量的顺序！
        volatile uint32_t CR;
        volatile uint32_t CFGR;
        volatile uint32_t CIR;
        volatile uint32_t APB2RSTR;
        volatile uint32_t APB1RSTR;
        volatile uint32_t AHBENR;
        volatile uint32_t APB2ENR;
        volatile uint32_t APB1ENR;
        volatile uint32_t BDCR;
        volatile uint32_t CSR;

#ifdef STM32F10X_CL
        volatile uint32_t AHBRSTR;
        volatile uint32_t CFGR2;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
        uint32_t  RESERVED0;
        volatile uint32_t CFGR2;
#endif /* STM32F10X_LD_VL || STM32F10X_MD_VL || STM32F10X_HD_VL */
        //上面的内容不要更改，尤其是变量的顺序！
    public:
        void Config(uint32_t RCC_Source, uint32_t RCC_Mode);
        void PreConfig(uint32_t RCC_Source, uint32_t RCC_Prescaler);
        void PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul);
        void MCOConfig(uint32_t RCC_MCO);

        void Cmd(uint32_t RCC_Source, uint8_t NewState);
        void AHBPeriphClockCmd(uint32_t RCC_Periph_Source, uint8_t NewState);
        void APB2PeriphClockCmd(uint32_t RCC_Periph_Source, uint8_t NewState);
        void APB1PeriphClockCmd(uint32_t RCC_Periph_Source, uint8_t NewState);
        void APB2PeriphResetCmd(uint32_t RCC_Periph_Source, uint8_t NewState);
        void APB1PeriphResetCmd(uint32_t RCC_Periph_Source, uint8_t NewState);

        ErrorStatus WaitForStartUp(uint32_t RCC_Source);
        void AdjustHSICalibrationValue(uint8_t HSICalibrationValue);
        uint8_t   GetSYSCLKSource(uint32_t RCC_Source = 0);
        void GetClocksFreq(RCC_ClocksTypeDef &RCC_Clocks);
    protected:
        RCC_Struct() {};
    } RCC_Typedef;
}
using periph_c::RCC_Typedef;

#define stRCC				((RCC_Typedef*)RCC_BASE)
#define CLOCK				(*stRCC)

#endif /* __STM32GLIBC_RCC_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
