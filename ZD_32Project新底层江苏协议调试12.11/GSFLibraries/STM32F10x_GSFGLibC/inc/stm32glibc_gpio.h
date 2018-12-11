/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_gpio.h
* Author             : Zhiyix Wang
* Version            : V1.0.0
* Create Date        : 2014-11-10
* Last Update        : 2014-11-10
* Description        : GPIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_GPIO_H
#define __STM32GLIBC_GPIO_H

#include "stm32glibc_type.h"


/* Exported constants --------------------------------------------------------*/
/* GPIO pins define ----------------------------------------------------------*/
//!
//! @{
#define GPIO_PERIPH_BANK            ((uint32_t)(APB2PERIPH_BASE))
//! @}
//!
//! Parameter 'uint32_t GPIO_Addr' enum value
//! @{
#define GPIO_Port_NULL              ((uint32_t)(0x00000000))
#define GPIO_Port_A                 ((uint32_t)(GPIOA_BASE << 16))  /* from map.h */
#define GPIO_Port_B                 ((uint32_t)(GPIOB_BASE << 16))  /* from map.h */
#define GPIO_Port_C                 ((uint32_t)(GPIOC_BASE << 16))  /* from map.h */
#define GPIO_Port_D                 ((uint32_t)(GPIOD_BASE << 16))  /* from map.h */
#define GPIO_Port_E                 ((uint32_t)(GPIOE_BASE << 16))  /* from map.h */
#define GPIO_Port_F                 ((uint32_t)(GPIOF_BASE << 16))  /* from map.h */
#define GPIO_Port_G                 ((uint32_t)(GPIOG_BASE << 16))  /* from map.h */
//! @}

namespace periph_c
{
    /**************** Generate by Stm32RegDefine v1.0 ****************/
    /****************             program by aleyn.wu ****************/
    struct GPIO_CRL_Struct
    {
        volatile uint32_t MODE0                         : 2; //Bit0 ~ Bit1
        volatile uint32_t CNF0                          : 2; //Bit2 ~ Bit3
        volatile uint32_t MODE1                         : 2; //Bit4 ~ Bit5
        volatile uint32_t CNF1                          : 2; //Bit6 ~ Bit7
        volatile uint32_t MODE2                         : 2; //Bit8 ~ Bit9
        volatile uint32_t CNF2                          : 2; //Bit10 ~ Bit11
        volatile uint32_t MODE3                         : 2; //Bit12 ~ Bit13
        volatile uint32_t CNF3                          : 2; //Bit14 ~ Bit15
        volatile uint32_t MODE4                         : 2; //Bit16 ~ Bit17
        volatile uint32_t CNF4                          : 2; //Bit18 ~ Bit19
        volatile uint32_t MODE5                         : 2; //Bit20 ~ Bit21
        volatile uint32_t CNF5                          : 2; //Bit22 ~ Bit23
        volatile uint32_t MODE6                         : 2; //Bit24 ~ Bit25
        volatile uint32_t CNF6                          : 2; //Bit26 ~ Bit27
        volatile uint32_t MODE7                         : 2; //Bit28 ~ Bit29
        volatile uint32_t CNF7                          : 2; //Bit30 ~ Bit31
    };

    typedef struct GPIO_CRL_Struct GPIO_CRL_Typedef;

    /**************** Generate by Stm32RegDefine v1.0 ****************/
    /****************             program by aleyn.wu ****************/
    struct GPIO_CRH_Struct
    {
        volatile uint32_t MODE8                         : 2; //Bit0 ~ Bit1
        volatile uint32_t CNF8                          : 2; //Bit2 ~ Bit3
        volatile uint32_t MODE9                         : 2; //Bit4 ~ Bit5
        volatile uint32_t CNF9                          : 2; //Bit6 ~ Bit7
        volatile uint32_t MODE10                        : 2; //Bit8 ~ Bit9
        volatile uint32_t CNF10                         : 2; //Bit10 ~ Bit11
        volatile uint32_t MODE11                        : 2; //Bit12 ~ Bit13
        volatile uint32_t CNF11                         : 2; //Bit14 ~ Bit15
        volatile uint32_t MODE12                        : 2; //Bit16 ~ Bit17
        volatile uint32_t CNF12                         : 2; //Bit18 ~ Bit19
        volatile uint32_t MODE13                        : 2; //Bit20 ~ Bit21
        volatile uint32_t CNF13                         : 2; //Bit22 ~ Bit23
        volatile uint32_t MODE14                        : 2; //Bit24 ~ Bit25
        volatile uint32_t CNF14                         : 2; //Bit26 ~ Bit27
        volatile uint32_t MODE15                        : 2; //Bit28 ~ Bit29
        volatile uint32_t CNF15                         : 2; //Bit30 ~ Bit31
    };

    typedef struct GPIO_CRH_Struct GPIO_CRH_Typedef;

    /**************** Generate by Stm32RegDefine v1.0 ****************/
    /****************             program by aleyn.wu ****************/
    struct GPIO_IDR_Struct
    {
        volatile uint32_t IDR0                          : 1; //Bit0
        volatile uint32_t IDR1                          : 1; //Bit1
        volatile uint32_t IDR2                          : 1; //Bit2
        volatile uint32_t IDR3                          : 1; //Bit3
        volatile uint32_t IDR4                          : 1; //Bit4
        volatile uint32_t IDR5                          : 1; //Bit5
        volatile uint32_t IDR6                          : 1; //Bit6
        volatile uint32_t IDR7                          : 1; //Bit7
        volatile uint32_t IDR8                          : 1; //Bit8
        volatile uint32_t IDR9                          : 1; //Bit9
        volatile uint32_t IDR10                         : 1; //Bit10
        volatile uint32_t IDR11                         : 1; //Bit11
        volatile uint32_t IDR12                         : 1; //Bit12
        volatile uint32_t IDR13                         : 1; //Bit13
        volatile uint32_t IDR14                         : 1; //Bit14
        volatile uint32_t IDR15                         : 1; //Bit15
        volatile uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct GPIO_IDR_Struct GPIO_IDR_Typedef;

    /**************** Generate by Stm32RegDefine v1.0 ****************/
    /****************             program by aleyn.wu ****************/
    struct GPIO_ODR_Struct
    {
        volatile uint32_t ODR0                          : 1; //Bit0
        volatile uint32_t ODR1                          : 1; //Bit1
        volatile uint32_t ODR2                          : 1; //Bit2
        volatile uint32_t ODR3                          : 1; //Bit3
        volatile uint32_t ODR4                          : 1; //Bit4
        volatile uint32_t ODR5                          : 1; //Bit5
        volatile uint32_t ODR6                          : 1; //Bit6
        volatile uint32_t ODR7                          : 1; //Bit7
        volatile uint32_t ODR8                          : 1; //Bit8
        volatile uint32_t ODR9                          : 1; //Bit9
        volatile uint32_t ODR10                         : 1; //Bit10
        volatile uint32_t ODR11                         : 1; //Bit11
        volatile uint32_t ODR12                         : 1; //Bit12
        volatile uint32_t ODR13                         : 1; //Bit13
        volatile uint32_t ODR14                         : 1; //Bit14
        volatile uint32_t ODR15                         : 1; //Bit15
        volatile uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct GPIO_ODR_Struct GPIO_ODR_Typedef;

    /**************** Generate by Stm32RegDefine v1.0 ****************/
    /****************             program by aleyn.wu ****************/
    struct GPIO_BSRR_Struct
    {
        volatile uint32_t BS0                           : 1; //Bit0
        volatile uint32_t BS1                           : 1; //Bit1
        volatile uint32_t BS2                           : 1; //Bit2
        volatile uint32_t BS3                           : 1; //Bit3
        volatile uint32_t BS4                           : 1; //Bit4
        volatile uint32_t BS5                           : 1; //Bit5
        volatile uint32_t BS6                           : 1; //Bit6
        volatile uint32_t BS7                           : 1; //Bit7
        volatile uint32_t BS8                           : 1; //Bit8
        volatile uint32_t BS9                           : 1; //Bit9
        volatile uint32_t BS10                          : 1; //Bit10
        volatile uint32_t BS11                          : 1; //Bit11
        volatile uint32_t BS12                          : 1; //Bit12
        volatile uint32_t BS13                          : 1; //Bit13
        volatile uint32_t BS14                          : 1; //Bit14
        volatile uint32_t BS15                          : 1; //Bit15
        volatile uint32_t BR0                           : 1; //Bit16
        volatile uint32_t BR1                           : 1; //Bit17
        volatile uint32_t BR2                           : 1; //Bit18
        volatile uint32_t BR3                           : 1; //Bit19
        volatile uint32_t BR4                           : 1; //Bit20
        volatile uint32_t BR5                           : 1; //Bit21
        volatile uint32_t BR6                           : 1; //Bit22
        volatile uint32_t BR7                           : 1; //Bit23
        volatile uint32_t BR8                           : 1; //Bit24
        volatile uint32_t BR9                           : 1; //Bit25
        volatile uint32_t BR10                          : 1; //Bit26
        volatile uint32_t BR11                          : 1; //Bit27
        volatile uint32_t BR12                          : 1; //Bit28
        volatile uint32_t BR13                          : 1; //Bit29
        volatile uint32_t BR14                          : 1; //Bit30
        volatile uint32_t BR15                          : 1; //Bit31
    };

    typedef struct GPIO_BSRR_Struct GPIO_BSRR_Typedef;

    /**************** Generate by Stm32RegDefine v1.0 ****************/
    /****************             program by aleyn.wu ****************/
    struct GPIO_BRR_Struct
    {
        volatile uint32_t BR0                           : 1; //Bit0
        volatile uint32_t BR1                           : 1; //Bit1
        volatile uint32_t BR2                           : 1; //Bit2
        volatile uint32_t BR3                           : 1; //Bit3
        volatile uint32_t BR4                           : 1; //Bit4
        volatile uint32_t BR5                           : 1; //Bit5
        volatile uint32_t BR6                           : 1; //Bit6
        volatile uint32_t BR7                           : 1; //Bit7
        volatile uint32_t BR8                           : 1; //Bit8
        volatile uint32_t BR9                           : 1; //Bit9
        volatile uint32_t BR10                          : 1; //Bit10
        volatile uint32_t BR11                          : 1; //Bit11
        volatile uint32_t BR12                          : 1; //Bit12
        volatile uint32_t BR13                          : 1; //Bit13
        volatile uint32_t BR14                          : 1; //Bit14
        volatile uint32_t BR15                          : 1; //Bit15
        volatile uint32_t Resv16_31                     : 16; //Bit16 ~ Bit31
    };

    typedef struct GPIO_BRR_Struct GPIO_BRR_Typedef;

    /**************** Generate by Stm32RegDefine v1.0 ****************/
    /****************             program by aleyn.wu ****************/
    struct GPIO_LCKR_Struct
    {
        volatile uint32_t LCK0                          : 1; //Bit0
        volatile uint32_t LCK1                          : 1; //Bit1
        volatile uint32_t LCK2                          : 1; //Bit2
        volatile uint32_t LCK3                          : 1; //Bit3
        volatile uint32_t LCK4                          : 1; //Bit4
        volatile uint32_t LCK5                          : 1; //Bit5
        volatile uint32_t LCK6                          : 1; //Bit6
        volatile uint32_t LCK7                          : 1; //Bit7
        volatile uint32_t LCK8                          : 1; //Bit8
        volatile uint32_t LCK9                          : 1; //Bit9
        volatile uint32_t LCK10                         : 1; //Bit10
        volatile uint32_t LCK11                         : 1; //Bit11
        volatile uint32_t LCK12                         : 1; //Bit12
        volatile uint32_t LCK13                         : 1; //Bit13
        volatile uint32_t LCK14                         : 1; //Bit14
        volatile uint32_t LCK15                         : 1; //Bit15
        volatile uint32_t LCKK                          : 1; //Bit16
        volatile uint32_t Resv17_31                     : 15; //Bit17 ~ Bit31
    };

    typedef struct GPIO_LCKR_Struct GPIO_LCKR_Typedef;

    enum Enum_GPIO_Speed
    {
        GPIO_Speed_Null  = 0x00,
        GPIO_Speed_10MHz = 0x01,
        GPIO_Speed_2MHz  = 0x02,
        GPIO_Speed_50MHz = 0x03
    };

    typedef enum Enum_GPIO_Speed GPIO_Speed_Typedef;

    /* Configuration Mode enumeration --------------------------------------------*/
    enum Enum_GPIO_Mode
    {
        GPIO_MODE_AIN         	= 0x00,
        GPIO_MODE_IN_FLOATING	= 0x04,
        GPIO_MODE_IPD         	= 0x28,
        GPIO_MODE_IPU         	= 0x48,
        GPIO_MODE_OUT_PP      	= 0x10,
        GPIO_MODE_OUT_OD      	= 0x14,
        GPIO_MODE_AF_PP       	= 0x18,
        GPIO_MODE_AF_OD       	= 0x1C
    };
    typedef enum Enum_GPIO_Mode GPIO_Mode_Typedef;


    typedef enum GPIO_Pin
    {
        Pin0 = Bit(0),
        Pin1 = Bit(1),
        Pin2 = Bit(2),
        Pin3 = Bit(3),
        Pin4 = Bit(4),
        Pin5 = Bit(5),
        Pin6 = Bit(6),
        Pin7 = Bit(7),
        Pin8 = Bit(8),
        Pin9 = Bit(9),
        Pin10 = Bit(10),
        Pin11 = Bit(11),
        Pin12 = Bit(12),
        Pin13 = Bit(13),
        Pin14 = Bit(14),
        Pin15 = Bit(15),
    } GPIO_PinSource;

    typedef struct GPIO_Struct
    {
        //注意：下面的成员变量的顺序不可修改，切记!!!
        __IO uint32_t CRL;
        __IO uint32_t CRH;
        __IO uint32_t IDR;
        __IO uint32_t ODR;
        __IO uint32_t BSRR;
        __IO uint32_t BRR;
        __IO uint32_t LCKR;
    public:
        void Config(const uint32_t GPIO_Pin, 
			GPIO_Mode_Typedef GPIO_Mode = GPIO_MODE_IN_FLOATING, 
			GPIO_Speed_Typedef GPIO_Speed = GPIO_Speed_2MHz);
        uint8_t   ReadInputDataBit(uint32_t GPIO_Pin);
        uint16_t  ReadInputData(const uint32_t GPIO_Addr = 0);
        uint8_t   ReadOutputDataBit(uint32_t GPIO_Pin);
        uint16_t  ReadOutputData(const uint32_t GPIO_Addr = 0);
        void SetBits(uint32_t GPIO_Pin);
        void ResetBits(uint32_t GPIO_Pin);
        void WriteBit(uint32_t GPIO_Pin, uint8_t BitVal);
        void Write(uint16_t PortVal, const uint32_t GPIO_Addr = 0);
        void ToggleBits(uint32_t GPIO_Pin);

    protected:
        GPIO_Struct() {};	//不允许外部建立对象!
    } GPIO_Typedef;
}
using periph_c::GPIO_Typedef;

#define stGPIOA				((GPIO_Typedef*)GPIOA_BASE)
#define stGPIOB				((GPIO_Typedef*)GPIOB_BASE)
#define stGPIOC				((GPIO_Typedef*)GPIOC_BASE)
#define stGPIOD				((GPIO_Typedef*)GPIOD_BASE)
#define stGPIOE				((GPIO_Typedef*)GPIOE_BASE)
#define stGPIOF				((GPIO_Typedef*)GPIOF_BASE)
#define IOPA				(*stGPIOA)
#define IOPB				(*stGPIOB)
#define IOPC				(*stGPIOC)
#define IOPD				(*stGPIOD)
#define IOPE				(*stGPIOE)
#define IOPF				(*stGPIOF)

#endif /* __STM32GLIBC_GPIO_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
