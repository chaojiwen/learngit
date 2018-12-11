/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32glibc_afio.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_AFIO_H
#define __STM32GLIBC_AFIO_H

#include "stm32glibc_type.h"

/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* GPIO pins define ----------------------------------------------------------*/
#define GPIO_Remap_NULL             ((uint32_t)0x00000000) 
#if (false)
//! GPIO_Remap_define
//! @{
#define GPIO_Remap_SPI1             ((uint32_t)0x00000001)  /*!< SPI1 Alternate Function mapping */
#define GPIO_Remap_I2C1             ((uint32_t)0x00000002)  /*!< I2C1 Alternate Function mapping */
#define GPIO_Remap_USART1           ((uint32_t)0x00000004)  /*!< USART1 Alternate Function mapping */
#define GPIO_Remap_USART2           ((uint32_t)0x00000008)  /*!< USART2 Alternate Function mapping */
#define GPIO_PartialRemap_USART3    ((uint32_t)0x00140010)  /*!< USART3 Partial Alternate Function mapping */
#define GPIO_FullRemap_USART3       ((uint32_t)0x00140030)  /*!< USART3 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM1      ((uint32_t)0x00160040)  /*!< TIM1 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM1         ((uint32_t)0x001600C0)  /*!< TIM1 Full Alternate Function mapping */
#define GPIO_PartialRemap1_TIM2     ((uint32_t)0x00180100)  /*!< TIM2 Partial1 Alternate Function mapping */
#define GPIO_PartialRemap2_TIM2     ((uint32_t)0x00180200)  /*!< TIM2 Partial2 Alternate Function mapping */
#define GPIO_FullRemap_TIM2         ((uint32_t)0x00180300)  /*!< TIM2 Full Alternate Function mapping */
#define GPIO_PartialRemap_TIM3      ((uint32_t)0x001A0800)  /*!< TIM3 Partial Alternate Function mapping */
#define GPIO_FullRemap_TIM3         ((uint32_t)0x001A0C00)  /*!< TIM3 Full Alternate Function mapping */
#define GPIO_Remap_TIM4             ((uint32_t)0x00001000)  /*!< TIM4 Alternate Function mapping */
#define GPIO_Remap1_CAN1            ((uint32_t)0x001D4000)  /*!< CAN1 Alternate Function mapping */
#define GPIO_Remap2_CAN1            ((uint32_t)0x001D6000)  /*!< CAN1 Alternate Function mapping */
#define GPIO_Remap_PD01             ((uint32_t)0x00008000)  /*!< PD01 Alternate Function mapping */
#define GPIO_Remap_TIM5CH4_LSI      ((uint32_t)0x00200001)  /*!< LSI connected to TIM5 Channel4 input capture for calibration */
#define GPIO_Remap_ADC1_ETRGINJ     ((uint32_t)0x00200002)  /*!< ADC1 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC1_ETRGREG     ((uint32_t)0x00200004)  /*!< ADC1 External Trigger Regular Conversion remapping */
#define GPIO_Remap_ADC2_ETRGINJ     ((uint32_t)0x00200008)  /*!< ADC2 External Trigger Injected Conversion remapping */
#define GPIO_Remap_ADC2_ETRGREG     ((uint32_t)0x00200010)  /*!< ADC2 External Trigger Regular Conversion remapping */
#define GPIO_Remap_ETH              ((uint32_t)0x00200020)  /*!< Ethernet remapping (only for Connectivity line devices) */
#define GPIO_Remap_CAN2             ((uint32_t)0x00200040)  /*!< CAN2 remapping (only for Connectivity line devices) */
#define GPIO_Remap_SWJ_NoJTRST      ((uint32_t)0x00300100)  /*!< Full SWJ Enabled (JTAG-DP + SW-DP) but without JTRST */
#define GPIO_Remap_SWJ_JTAGDisable  ((uint32_t)0x00300200)  /*!< JTAG-DP Disabled and SW-DP Enabled */
#define GPIO_Remap_SWJ_Disable      ((uint32_t)0x00300400)  /*!< Full SWJ Disabled (JTAG-DP + SW-DP) */
#define GPIO_Remap_SPI3             ((uint32_t)0x00201100)  /*!< SPI3/I2S3 Alternate Function mapping (only for Connectivity line devices) */
#define GPIO_Remap_TIM2ITR1_PTP_SOF ((uint32_t)0x00202000)  /*!< Ethernet PTP output or USB OTG SOF (Start of Frame) connected to TIM2 Internal Trigger 1 for calibration(only for Connectivity line devices) */
#define GPIO_Remap_PTP_PPS          ((uint32_t)0x00204000)  /*!< Ethernet MAC PPS_PTS output on PB05 (only for Connectivity line devices) */

#define GPIO_Remap_TIM15            ((uint32_t)0x80000001)  /*!< TIM15 Alternate Function mapping (only for Value line devices) */
#define GPIO_Remap_TIM16            ((uint32_t)0x80000002)  /*!< TIM16 Alternate Function mapping (only for Value line devices) */
#define GPIO_Remap_TIM17            ((uint32_t)0x80000004)  /*!< TIM17 Alternate Function mapping (only for Value line devices) */
#define GPIO_Remap_CEC              ((uint32_t)0x80000008)  /*!< CEC Alternate Function mapping (only for Value line devices) */
#define GPIO_Remap_TIM1_DMA         ((uint32_t)0x80000010)  /*!< TIM1 DMA requests mapping (only for Value line devices) */

#define GPIO_Remap_TIM9             ((uint32_t)0x80000020)  /*!< TIM9 Alternate Function mapping (only for XL-density devices) */
#define GPIO_Remap_TIM10            ((uint32_t)0x80000040)  /*!< TIM10 Alternate Function mapping (only for XL-density devices) */
#define GPIO_Remap_TIM11            ((uint32_t)0x80000080)  /*!< TIM11 Alternate Function mapping (only for XL-density devices) */
#define GPIO_Remap_TIM13            ((uint32_t)0x80000100)  /*!< TIM13 Alternate Function mapping (only for High density Value line and XL-density devices) */
#define GPIO_Remap_TIM14            ((uint32_t)0x80000200)  /*!< TIM14 Alternate Function mapping (only for High density Value line and XL-density devices) */
#define GPIO_Remap_FSMC_NADV        ((uint32_t)0x80000400)  /*!< FSMC_NADV Alternate Function mapping (only for High density Value line and XL-density devices) */

#define GPIO_Remap_TIM67_DAC_DMA    ((uint32_t)0x80000800)  /*!< TIM6/TIM7 and DAC DMA requests remapping (only for High density Value line devices) */
#define GPIO_Remap_TIM12            ((uint32_t)0x80001000)  /*!< TIM12 Alternate Function mapping (only for High density Value line devices) */
#define GPIO_Remap_MISC             ((uint32_t)0x80002000)  /*!< Miscellaneous Remap (DMA2 Channel5 Position and DAC Trigger remapping, only for High density Value line devices) */
//! @}
#endif
//! @{
#define LSB_MASK                    ((u16)0xFFFF)
#define DBGAFR_SWJCFG_MASK          ((uint32_t)0xF0FFFFFF)
#define DBGAFR_POSITION_MASK        ((uint32_t)0x000F0000)
#define DBGAFR_LOCATION_MASK        ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((uint32_t)0x00100000)
//! @}
//!
//! @{
#define AFIO_PERIPH_BANK            ((uint32_t)(APB2PERIPH_BASE))
//! @}

//! GPIO_Port_Sources
//! @{
#define AFIO_PortSourceGPIOA       	((uint8_t)0x00)
#define AFIO_PortSourceGPIOB       	((uint8_t)0x01)
#define AFIO_PortSourceGPIOC       	((uint8_t)0x02)
#define AFIO_PortSourceGPIOD       	((uint8_t)0x03)
#define AFIO_PortSourceGPIOE       	((uint8_t)0x04)
#define AFIO_PortSourceGPIOF       	((uint8_t)0x05)
#define AFIO_PortSourceGPIOG       	((uint8_t)0x06)
//! @}
//! GPIO_Pin_sources
//! @{
#define GPIO_PinSource0            	((uint8_t)0x00)
#define GPIO_PinSource1				((uint8_t)0x01)
#define GPIO_PinSource2				((uint8_t)0x02)
#define GPIO_PinSource3				((uint8_t)0x03)
#define GPIO_PinSource4				((uint8_t)0x04)
#define GPIO_PinSource5				((uint8_t)0x05)
#define GPIO_PinSource6				((uint8_t)0x06)
#define GPIO_PinSource7				((uint8_t)0x07)
#define GPIO_PinSource8				((uint8_t)0x08)
#define GPIO_PinSource9				((uint8_t)0x09)
#define GPIO_PinSource10			((uint8_t)0x0A)
#define GPIO_PinSource11			((uint8_t)0x0B)
#define GPIO_PinSource12			((uint8_t)0x0C)
#define GPIO_PinSource13			((uint8_t)0x0D)
#define GPIO_PinSource14			((uint8_t)0x0E)
#define GPIO_PinSource15			((uint8_t)0x0F)
//! @}

namespace periph_c
{
    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ÊÂ¼þ¿ØÖÆ¼Ä´æÆ÷(AFIO_EVCR)
    struct AFIO_EVCR_Struct
    {
        __IO uint32_t PIN                           : 4; //Bit0 ~ Bit3
        __IO uint32_t PORT                          : 3; //Bit4 ~ Bit6
        __IO uint32_t EVOE                          : 1; //Bit7
        __IO uint32_t Resv1                         : 8; //Bit8 ~ Bit15
        __IO uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct AFIO_EVCR_Struct AFIO_EVCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ¸´ÓÃÖØÓ³ÉäºÍµ÷ÊÔIOÅäÖÃ¼Ä´æÆ÷(AFIO_MAPR)
    struct AFIO_MAPR_Struct
    {
        __IO uint32_t SPI1_REMAP                    : 1; //Bit0
        __IO uint32_t I2C1_REMAP                    : 1; //Bit1
        __IO uint32_t USART1_REMAP                  : 1; //Bit2
        __IO uint32_t USART2_REMAP                  : 1; //Bit3
        __IO uint32_t USART3_REMAP                  : 2; //Bit4 ~ Bit5
        __IO uint32_t TIM1_REMAP                    : 2; //Bit6 ~ Bit7
        __IO uint32_t TIM2_REMAP                    : 2; //Bit8 ~ Bit9
        __IO uint32_t TIM3_REMAP                    : 2; //Bit10 ~ Bit11
        __IO uint32_t TIM4_REMAP                    : 1; //Bit12
        __IO uint32_t CAN_REMAP                     : 2; //Bit13 ~ Bit14
        __IO uint32_t PD01_REMAP                    : 1; //Bit15
        __IO uint32_t TIM5CH4_IREMAP                : 1; //Bit16
        __IO uint32_t ADC1_ETRGINJ_REMAP            : 1; //Bit17
        __IO uint32_t ADC1_ETRGREG_REMAP            : 1; //Bit18
        __IO uint32_t ADC2_ETRGINJ_REMAP            : 1; //Bit19
        __IO uint32_t ADC3_ETRGREG_REMAP            : 1; //Bit20
        __IO uint32_t SWJ_CFG                       : 3; //Bit21 ~ Bit23
        __IO uint32_t Resv1                         : 3; //Bit24 ~ Bit26
        __IO uint32_t Resv27_31                     : 5; //Bit27 ~ Bit31
    };

    typedef struct AFIO_MAPR_Struct AFIO_MAPR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! Íâ²¿ÖÐ¶ÏÅäÖÃ¼Ä´æÆ÷(AFIO_EXTICR1)
    struct AFIO_EXTICR1_Struct
    {
        __IO uint32_t EXTI0                         : 4; //Bit0 ~ Bit3
        __IO uint32_t EXTI1                         : 4; //Bit4 ~ Bit7
        __IO uint32_t EXTI2                         : 4; //Bit8 ~ Bit11
        __IO uint32_t EXTI3                         : 4; //Bit12 ~ Bit15
        __IO uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct AFIO_EXTICR1_Struct AFIO_EXTICR1_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! Íâ²¿ÖÐ¶ÏÅäÖÃ¼Ä´æÆ÷(AFIO_EXTICR2)
    struct AFIO_EXTICR2_Struct
    {
        __IO uint32_t EXTI4                         : 4; //Bit0 ~ Bit3
        __IO uint32_t EXTI5                         : 4; //Bit4 ~ Bit7
        __IO uint32_t EXTI6                         : 4; //Bit8 ~ Bit11
        __IO uint32_t EXTI7                         : 4; //Bit12 ~ Bit15
        __IO uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct AFIO_EXTICR2_Struct AFIO_EXTICR2_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! Íâ²¿ÖÐ¶ÏÅäÖÃ¼Ä´æÆ÷(AFIO_EXTICR3)
    struct AFIO_EXTICR3_Struct
    {
        __IO uint32_t EXTI8                         : 4; //Bit0 ~ Bit3
        __IO uint32_t EXTI9                         : 4; //Bit4 ~ Bit7
        __IO uint32_t EXTI10                        : 4; //Bit8 ~ Bit11
        __IO uint32_t EXTI11                        : 4; //Bit12 ~ Bit15
        __IO uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct AFIO_EXTICR3_Struct AFIO_EXTICR3_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! Íâ²¿ÖÐ¶ÏÅäÖÃ¼Ä´æÆ÷(AFIO_EXTICR4)
    struct AFIO_EXTICR4_Struct
    {
        __IO uint32_t EXTI12                        : 4; //Bit0 ~ Bit3
        __IO uint32_t EXTI13                        : 4; //Bit4 ~ Bit7
        __IO uint32_t EXTI14                        : 4; //Bit8 ~ Bit11
        __IO uint32_t EXTI15                        : 4; //Bit12 ~ Bit15
        __IO uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct AFIO_EXTICR4_Struct AFIO_EXTICR4_Typedef;

    typedef struct AFIO_Struct
    {
        __IO uint32_t EVCR;
        __IO uint32_t MAPR;
        __IO uint32_t EXTICR[4];
        uint32_t  RESERVED0;
        __IO uint32_t MAPR2;
    public:
        void PinRemapConfig(uint32_t AFIO_Remap, FunctionalState NewState = ENABLE);
        void EXTILineConfig(uint8_t AFIO_PortSource, uint8_t AFIO_PinSource);
    } AFIO_Typedef;
}
using periph_c::AFIO_Typedef;

#define stAFIO				((AFIO_Typedef*)AFIO_BASE)
#define IOAF				(*stAFIO)

#endif /* __STM32GLIBC_AFIO_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
