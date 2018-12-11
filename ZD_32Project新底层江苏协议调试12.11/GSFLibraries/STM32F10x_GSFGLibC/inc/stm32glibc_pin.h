/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32fwlib_pin.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32FWLIB_PIN_H
#define __STM32FWLIB_PIN_H

#include "stm32fwlib_gpio.h"
#include "stm32fwlib_bitband.h"

/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* GPIO pins define ----------------------------------------------------------*/
//!
//! GPIO IO 口地址映射
//! @{
//!
#define GPIOA_IDR_ADDR		(GPIOA_BASE+0x8) //0x40010808
#define GPIOB_IDR_ADDR		(GPIOB_BASE+0x8) //0x40010C08
#define GPIOC_IDR_ADDR		(GPIOC_BASE+0x8) //0x40011008
#define GPIOD_IDR_ADDR		(GPIOD_BASE+0x8) //0x40011408
#define GPIOE_IDR_ADDR		(GPIOE_BASE+0x8) //0x40011808
#define GPIOF_IDR_ADDR		(GPIOF_BASE+0x8) //0x40011A08
#define GPIOG_IDR_ADDR		(GPIOG_BASE+0x8) //0x40011E08
//!
#define GPIOA_ODR_ADDR		(GPIOA_BASE+0xC) //0x4001080C
#define GPIOB_ODR_ADDR		(GPIOB_BASE+0xC) //0x40010C0C
#define GPIOC_ODR_ADDR		(GPIOC_BASE+0xC) //0x4001100C
#define GPIOD_ODR_ADDR		(GPIOD_BASE+0xC) //0x4001140C
#define GPIOE_ODR_ADDR		(GPIOE_BASE+0xC) //0x4001180C
#define GPIOF_ODR_ADDR		(GPIOF_BASE+0xC) //0x40011A0C
#define GPIOG_ODR_ADDR		(GPIOG_BASE+0xC) //0x40011E0C
//! @}
//!
//! IO口操作 只对单一的IO pIN!(确保n的值小于16!)
//! 输出, 如PAOUT(0) = 1;	会将PA0置1
//! 输入, 如x = PAIN(0);	读PA0的值到变量x中
//! @{
#define PAOUT(n)   			BIT_ADDR(GPIOA_ODR_ADDR,n)  //输出
#define PAIN(n)   			BIT_ADDR(GPIOA_IDR_ADDR,n)  //输入

#define PBOUT(n)   			BIT_ADDR(GPIOB_ODR_ADDR,n)  //输出
#define PBIN(n)   			BIT_ADDR(GPIOB_IDR_ADDR,n)  //输入 

#define PCOUT(n)   			BIT_ADDR(GPIOC_ODR_ADDR,n)  //输出 
#define PCIN(n)   			BIT_ADDR(GPIOC_IDR_ADDR,n)  //输入 

#define PDOUT(n)   			BIT_ADDR(GPIOD_ODR_ADDR,n)  //输出 
#define PDIN(n)   			BIT_ADDR(GPIOD_IDR_ADDR,n)  //输入 

#define PEOUT(n)   			BIT_ADDR(GPIOE_ODR_ADDR,n)  //输出 
#define PEIN(n)   			BIT_ADDR(GPIOE_IDR_ADDR,n)  //输入

#define PFOUT(n)   			BIT_ADDR(GPIOF_ODR_ADDR,n)  //输出 
#define PFIN(n)   			BIT_ADDR(GPIOF_IDR_ADDR,n)  //输入

#define PGOUT(n)   			BIT_ADDR(GPIOG_ODR_ADDR,n)  //输出
#define PGIN(n)   			BIT_ADDR(GPIOG_IDR_ADDR,n)  //输入
//! @}

namespace periph
{
    class CPort_InfoA
    {
    public:
        enum {PORT  = (u32)GPIOA_BASE};
        enum {CLOCK = (u32)RCC_APB2Periph_GPIOA};
        static __forceinline void Output(u8 pinNum, u32 val)
        {
            PAOUT(pinNum) = val;
        }
        static __forceinline u32 Input(u8 pinNum)
        {
            return PAIN(pinNum);
        }
    };

    class CPort_InfoB
    {
    public:
        enum {PORT  = (u32)GPIOB_BASE};
        enum {CLOCK = (u32)RCC_APB2Periph_GPIOB};
        static __forceinline void Output(u8 pinNum, u32 val)
        {
            PBOUT(pinNum) = val;
        }
        static __forceinline u32 Input(u8 pinNum)
        {
            return PBIN(pinNum);
        }
    };

    class CPort_InfoC
    {
    public:
        enum {PORT  = (u32)GPIOC_BASE};
        enum {CLOCK = (u32)RCC_APB2Periph_GPIOC};
        static __forceinline void Output(u8 pinNum, u32 val)
        {
            PCOUT(pinNum) = val;
        }
        static __forceinline u32 Input(u8 pinNum)
        {
            return PCIN(pinNum);
        }
    };

    class CPort_InfoD
    {
    public:
        enum {PORT  = (u32)GPIOD_BASE};
        enum {CLOCK = (u32)RCC_APB2Periph_GPIOD};
        static __forceinline void Output(u8 pinNum, u32 val)
        {
            PDOUT(pinNum) = val;
        }
        static __forceinline u32 Input(u8 pinNum)
        {
            return PDIN(pinNum);
        }
    };

    class CPort_InfoE
    {
    public:
        enum {PORT  = (u32)GPIOE_BASE};
        enum {CLOCK = (u32)RCC_APB2Periph_GPIOE};
        static __forceinline void Output(u8 pinNum, u32 val)
        {
            PEOUT(pinNum) = val;
        }
        static __forceinline u32 Input(u8 pinNum)
        {
            return PEIN(pinNum);
        }
    };


    class CPin_Info0
    {
    public:
        enum {PIN = Pin0,	NUM = 0};
    };
    class CPin_Info1
    {
    public:
        enum {PIN = Pin1,	NUM = 1};
    };
    class CPin_Info2
    {
    public:
        enum {PIN = Pin2,	NUM = 2};
    };
    class CPin_Info3
    {
    public:
        enum {PIN = Pin3,	NUM = 3};
    };
    class CPin_Info4
    {
    public:
        enum {PIN = Pin4,	NUM = 4};
    };
    class CPin_Info5
    {
    public:
        enum {PIN = Pin5,	NUM = 5};
    };
    class CPin_Info6
    {
    public:
        enum {PIN = Pin6,	NUM = 6};
    };
    class CPin_Info7
    {
    public:
        enum {PIN = Pin7,	NUM = 7};
    };
    class CPin_Info8
    {
    public:
        enum {PIN = Pin8,	NUM = 8};
    };
    class CPin_Info9
    {
    public:
        enum {PIN = Pin9,	NUM = 9};
    };
    class CPin_Info10
    {
    public:
        enum {PIN = Pin10,	NUM = 10};
    };
    class CPin_Info11
    {
    public:
        enum {PIN = Pin11,	NUM = 11};
    };
    class CPin_Info12
    {
    public:
        enum {PIN = Pin12,	NUM = 12};
    };
    class CPin_Info13
    {
    public:
        enum {PIN = Pin13,	NUM = 13};
    };
    class CPin_Info14
    {
    public:
        enum {PIN = Pin14,	NUM = 14};
    };
    class CPin_Info15
    {
    public:
        enum {PIN = Pin15,	NUM = 15};
    };

    template<class CPort_Info, class CPin_Info>
    class CPin
    {
    protected:
        //! 注意一定要定义为static的,
        //! 否则 bool() 运算符重载编译不通过
        static __forceinline u32 GetPort()
        {
            return CPort_Info::PORT;
        }
        static __forceinline u32 GetClock()
        {
            return CPort_Info::CLOCK;
        }
        static GPIO_Pin GetPin()
        {
            return (GPIO_Pin)(CPin_Info::PIN);
        }
        static __forceinline void Output(u32 val)
        {
            CPort_Info::Output(CPin_Info::NUM, val);
        }
        static __forceinline u32 Input()
        {
            return CPort_Info::Input(CPin_Info::NUM);
        }
    public:
        static void __forceinline Init(GPIO_Mode_Typedef GPIO_Mode,
                                       GPIO_Speed_Typedef GPIO_Speed = GPIO_Speed_2MHz)
        {
            CGPIO m_Port(GetPort());
			CRCC  m_Clock;
            //! 一定要先使能端口后才能配置端口!!!否则无效
			m_Clock.APB2PeriphClockCmd(GetClock(), ENABLE);
            m_Port.Config(GetPin(), GPIO_Mode, GPIO_Speed);
        }
		
        __forceinline operator u32() const
        {
            return Input();
        }
        __forceinline CPin &operator =(u32 val)
        {
            Output(val);
            return *this;
        }
        __forceinline CPin &operator =(CPin<CPort_Info, CPin_Info> pin)
        {
            Output(pin.Input());
            return *this;
        }
        __forceinline CPin &operator <<(u32 val)
        {
            Output(val);
            return *this;
        }
        //	读引脚, 如果引脚为高电平, 则读到非零值, 否则读到0
        __forceinline CPin &operator >>(bool &val)
        {
            val = Input();
            return *this;
        }
        __forceinline CPin &operator >>(int &val)
        {
            val = Input();
            return *this;
        }
        __forceinline CPin &operator >>(u16 &val)
        {
            val = Input();
            return *this;
        }
        __forceinline CPin &operator >>(u32 &val)
        {
            val = Input();
            return *this;
        }
    };

    namespace gpio_pin_declaration
    {
        extern CPin<CPort_InfoA, CPin_Info0>	PA0;
        extern CPin<CPort_InfoA, CPin_Info1>	PA1;
        extern CPin<CPort_InfoA, CPin_Info2>	PA2;
        extern CPin<CPort_InfoA, CPin_Info3>	PA3;
        extern CPin<CPort_InfoA, CPin_Info4>	PA4;
        extern CPin<CPort_InfoA, CPin_Info5>	PA5;
        extern CPin<CPort_InfoA, CPin_Info6>	PA6;
        extern CPin<CPort_InfoA, CPin_Info7>	PA7;
        extern CPin<CPort_InfoA, CPin_Info8>	PA8;
        extern CPin<CPort_InfoA, CPin_Info9>	PA9;
        extern CPin<CPort_InfoA, CPin_Info10>	PA10;
        extern CPin<CPort_InfoA, CPin_Info11>	PA11;
        extern CPin<CPort_InfoA, CPin_Info12>	PA12;
        extern CPin<CPort_InfoA, CPin_Info13>	PA13;
        extern CPin<CPort_InfoA, CPin_Info14>	PA14;
        extern CPin<CPort_InfoA, CPin_Info15>	PA15;

        extern CPin<CPort_InfoB, CPin_Info0>	PB0;
        extern CPin<CPort_InfoB, CPin_Info1>	PB1;
        extern CPin<CPort_InfoB, CPin_Info2>	PB2;
        extern CPin<CPort_InfoB, CPin_Info3>	PB3;
        extern CPin<CPort_InfoB, CPin_Info4>	PB4;
        extern CPin<CPort_InfoB, CPin_Info5>	PB5;
        extern CPin<CPort_InfoB, CPin_Info6>	PB6;
        extern CPin<CPort_InfoB, CPin_Info7>	PB7;
        extern CPin<CPort_InfoB, CPin_Info8>	PB8;
        extern CPin<CPort_InfoB, CPin_Info9>	PB9;
        extern CPin<CPort_InfoB, CPin_Info10>	PB10;
        extern CPin<CPort_InfoB, CPin_Info11>	PB11;
        extern CPin<CPort_InfoB, CPin_Info12>	PB12;
        extern CPin<CPort_InfoB, CPin_Info13>	PB13;
        extern CPin<CPort_InfoB, CPin_Info14>	PB14;
        extern CPin<CPort_InfoB, CPin_Info15>	PB15;

        extern CPin<CPort_InfoC, CPin_Info0>	PC0;
        extern CPin<CPort_InfoC, CPin_Info1>	PC1;
        extern CPin<CPort_InfoC, CPin_Info2>	PC2;
        extern CPin<CPort_InfoC, CPin_Info3>	PC3;
        extern CPin<CPort_InfoC, CPin_Info4>	PC4;
        extern CPin<CPort_InfoC, CPin_Info5>	PC5;
        extern CPin<CPort_InfoC, CPin_Info6>	PC6;
        extern CPin<CPort_InfoC, CPin_Info7>	PC7;
        extern CPin<CPort_InfoC, CPin_Info8>	PC8;
        extern CPin<CPort_InfoC, CPin_Info9>	PC9;
        extern CPin<CPort_InfoC, CPin_Info10>	PC10;
        extern CPin<CPort_InfoC, CPin_Info11>	PC11;
        extern CPin<CPort_InfoC, CPin_Info12>	PC12;
        extern CPin<CPort_InfoC, CPin_Info13>	PC13;
        extern CPin<CPort_InfoC, CPin_Info14>	PC14;
        extern CPin<CPort_InfoC, CPin_Info15>	PC15;

        extern CPin<CPort_InfoD, CPin_Info0>	PD0;
        extern CPin<CPort_InfoD, CPin_Info1>	PD1;
        extern CPin<CPort_InfoD, CPin_Info2>	PD2;
        extern CPin<CPort_InfoD, CPin_Info3>	PD3;
        extern CPin<CPort_InfoD, CPin_Info4>	PD4;
        extern CPin<CPort_InfoD, CPin_Info5>	PD5;
        extern CPin<CPort_InfoD, CPin_Info6>	PD6;
        extern CPin<CPort_InfoD, CPin_Info7>	PD7;
        extern CPin<CPort_InfoD, CPin_Info8>	PD8;
        extern CPin<CPort_InfoD, CPin_Info9>	PD9;
        extern CPin<CPort_InfoD, CPin_Info10>	PD10;
        extern CPin<CPort_InfoD, CPin_Info11>	PD11;
        extern CPin<CPort_InfoD, CPin_Info12>	PD12;
        extern CPin<CPort_InfoD, CPin_Info13>	PD13;
        extern CPin<CPort_InfoD, CPin_Info14>	PD14;
        extern CPin<CPort_InfoD, CPin_Info15>	PD15;

        extern CPin<CPort_InfoE, CPin_Info0>	PE0;
        extern CPin<CPort_InfoE, CPin_Info1>	PE1;
        extern CPin<CPort_InfoE, CPin_Info2>	PE2;
        extern CPin<CPort_InfoE, CPin_Info3>	PE3;
        extern CPin<CPort_InfoE, CPin_Info4>	PE4;
        extern CPin<CPort_InfoE, CPin_Info5>	PE5;
        extern CPin<CPort_InfoE, CPin_Info6>	PE6;
        extern CPin<CPort_InfoE, CPin_Info7>	PE7;
        extern CPin<CPort_InfoE, CPin_Info8>	PE8;
        extern CPin<CPort_InfoE, CPin_Info9>	PE9;
        extern CPin<CPort_InfoE, CPin_Info10>	PE10;
        extern CPin<CPort_InfoE, CPin_Info11>	PE11;
        extern CPin<CPort_InfoE, CPin_Info12>	PE12;
        extern CPin<CPort_InfoE, CPin_Info13>	PE13;
        extern CPin<CPort_InfoE, CPin_Info14>	PE14;
        extern CPin<CPort_InfoE, CPin_Info15>	PE15;
    }
}
//!
//! @{
using namespace periph::gpio_pin_declaration;
//! @}

#endif /* __STM32FWLIB_PIN_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
