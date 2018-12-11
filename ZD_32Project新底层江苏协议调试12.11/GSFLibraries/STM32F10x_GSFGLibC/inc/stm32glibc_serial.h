/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32fwlib_serial.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32FWLIB_SERIAL_H
#define __STM32FWLIB_SERIAL_H

#include "stm32fwlib_usart.h"
#include "stm32fwlib_bitband.h"

/* Configuration Mode enumeration --------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* GPIO pins define ----------------------------------------------------------*/
//!
//! USART SR
//! @{
#define UART1_SR_ADDR		(USART1_BASE+0x0) 	//0x40013800
#define UART2_SR_ADDR		(USART2_BASE+0x0) 	//0x40004400
#define UART3_SR_ADDR		(USART3_BASE+0x0) 	//0x40004800
#define UART4_SR_ADDR		(UART4_BASE+0x0) 	//0x40004C00
#define UART5_SR_ADDR		(UART5_BASE+0x0) 	//0x40005000
//! @}
//!
//! USART DR
//! @{
#define UART1_DR_ADDR		(USART1_BASE+0x4) 	//0x40013804
#define UART2_DR_ADDR		(USART2_BASE+0x4) 	//0x40004404
#define UART3_DR_ADDR		(USART3_BASE+0x4) 	//0x40004804
#define UART4_DR_ADDR		(UART4_BASE+0x4) 	//0x40004C04
#define UART5_DR_ADDR		(UART5_BASE+0x4) 	//0x40005004
//! @}
//!
//! USART CR
//! @{
#define UART1_CR1_ADDR		(USART1_BASE+0xC) 	//0x4001380C
#define UART2_CR1_ADDR		(USART2_BASE+0xC) 	//0x4000440C
#define UART3_CR1_ADDR		(USART3_BASE+0xC) 	//0x4000480C
#define UART4_CR1_ADDR		(UART4_BASE+0xC) 	//0x40004C0C
#define UART5_CR1_ADDR		(UART5_BASE+0xC) 	//0x4000500C
//! @}
//!
//! @{
#define URT1_SR(n) 			BIT_ADDR(UART1_SR_ADDR,n)
#define URT2_SR(n)   		BIT_ADDR(UART2_SR_ADDR,n)
#define URT3_SR(n)   		BIT_ADDR(UART3_SR_ADDR,n)
#define URT4_SR(n)   		BIT_ADDR(UART4_SR_ADDR,n)
#define URT5_SR(n)   		BIT_ADDR(UART5_SR_ADDR,n)
//!
#define URT1_CR1(n) 		BIT_ADDR(UART1_CR1_ADDR,n)
#define URT2_CR1(n)   		BIT_ADDR(UART2_CR1_ADDR,n)
#define URT3_CR1(n)   		BIT_ADDR(UART3_CR1_ADDR,n)
#define URT4_CR1(n)   		BIT_ADDR(UART4_CR1_ADDR,n)
#define URT5_CR1(n)   		BIT_ADDR(UART5_CR1_ADDR,n)
//! @}

namespace periph
{
    enum USART_State
    {
        PE = 0,	FE,		NE,		ORE,
        IDLE,	RXNE,	TC,		TXE,
        LBD,	CTS
    };
    enum USART_IntBit
    {
        IDLEIE = 4,	RXNEIE,	TCIE,	TXEIE,
        PEIE,
    };
    class CSerial_Info
    {
    public:
        enum {PORT  };
        enum {DATA  };
        enum {CLOCK };
        enum {BAUDRATE  = (u32)9600};
        enum {MODE  = USART_Mode_Tx | USART_Mode_Rx};
        static __forceinline u32 ReadRegNEmpty(void)
        {
            return 0;
        }
        static __forceinline u32 TransRegEmpty(void)
        {
            return 0;
        }
        static __forceinline void Interrupt(USART_IntBit id, FunctionalState e = ENABLE)
        {
           (void)id;
        }
    };
    class CSerial_Info1 : CSerial_Info
    {
    public:
        enum {PORT  = (u32)USART1_BASE};
        enum {DATA  = (u32)UART1_DR_ADDR};
        enum {CLOCK = (u32)RCC_APB2Periph_USART1};
        enum {BAUDRATE  = (u32)115200};
        enum {MODE  = USART_Mode_Tx | USART_Mode_Rx};
        static __forceinline u32 ReadRegNEmpty(void)
        {
            return URT1_SR(RXNE);
        }
        static __forceinline u32 TransRegEmpty(void)
        {
            return (u32)(URT1_SR(TXE) == RESET);
        }
        static __forceinline void Interrupt(USART_IntBit id, FunctionalState e = ENABLE)
        {
            URT1_CR1(id) = e;
        }
    };

    class CSerial_Info2 : CSerial_Info
    {
    public:
        enum {PORT  = (u32)USART2_BASE};
        enum {DATA  = (u32)UART2_DR_ADDR};
        enum {CLOCK = (u32)RCC_APB1Periph_USART2};
        enum {BAUDRATE  = (u32)9600};
        enum {MODE  = USART_Mode_Tx | USART_Mode_Rx};
        static __forceinline u32 ReadRegNEmpty(void)
        {
            return URT2_SR(RXNE);
        }
        static __forceinline u32 TransRegEmpty(void)
        {
            return (u32)(URT2_SR(TXE) == RESET);
        }
        static __forceinline void Interrupt(USART_IntBit id, FunctionalState e = ENABLE)
        {
            URT2_CR1(id) = e;
        }
    };

    class CSerial_Info3 : CSerial_Info
    {
    public:
        enum {PORT  = (u32)USART3_BASE};
        enum {DATA  = (u32)UART3_DR_ADDR};
        enum {CLOCK = (u32)RCC_APB1Periph_USART3};
        enum {BAUDRATE  = (u32)9600};
        enum {MODE  = USART_Mode_Tx | USART_Mode_Rx};
        static __forceinline u32 ReadRegNEmpty(void)
        {
            return URT3_SR(RXNE);
        }
        static __forceinline u32 TransRegEmpty(void)
        {
            return (u32)(URT3_SR(TXE) == RESET);
        }
        static __forceinline void Interrupt(USART_IntBit id, FunctionalState e = ENABLE)
        {
            URT3_CR1(id) = e;
        }
    };

    class CSerial_Info4 : CSerial_Info
    {
    public:
        enum {PORT  = (u32)UART4_BASE};
        enum {DATA  = (u32)UART4_DR_ADDR};
        enum {CLOCK = (u32)RCC_APB1Periph_UART4};
        enum {BAUDRATE  = (u32)9600};
        enum {MODE  = USART_Mode_Tx | USART_Mode_Rx};
        static __forceinline u32 ReadRegNEmpty(void)
        {
            return URT4_SR(RXNE);
        }
        static __forceinline u32 TransRegEmpty(void)
        {
            return (u32)(URT4_SR(TXE) == RESET);
        }
        static __forceinline void Interrupt(USART_IntBit id, FunctionalState e = ENABLE)
        {
            URT4_CR1(id) = e;
        }
    };

    class CSerial_Info5 : CSerial_Info
    {
    public:
        enum {PORT  = (u32)UART5_BASE};
        enum {DATA  = (u32)UART5_DR_ADDR};
        enum {CLOCK = (u32)RCC_APB1Periph_UART5};
        enum {BAUDRATE  = (u32)9600};
        enum {MODE  = USART_Mode_Tx | USART_Mode_Rx};
        static __forceinline u32 ReadRegNEmpty(void)
        {
            return URT5_SR(RXNE);
        }
        static __forceinline u32 TransRegEmpty(void)
        {
            return (u32)(URT5_SR(TXE) == RESET);
        }
        static __forceinline void Interrupt(USART_IntBit id, FunctionalState e = ENABLE)
        {
            URT5_CR1(id) = e;
        }
    };

    template<class T>
    class CSerial : CUSART
    {
        typedef void (*Manipulator)(CSerial &u);
    protected:
        //! 注意一定要定义为static的,
        //! 否则 bool() 运算符重载编译不通过
        static __forceinline u32 GetPort()
        {
            return T::PORT;
        }
        static __forceinline u32 GetClock()
        {
            return T::CLOCK;
        }

    public:
        static __forceinline void Init(void)
        {
            CUSART Usartx = GetPort();
            Usartx.Config(T::BAUDRATE, T::MODE);
        }
        static __forceinline void ReceiveInterrupt(FunctionalState e = ENABLE)
        {
            T::Interrupt(RXNEIE, e);
        }
        static __forceinline void TransmittInterrupt(FunctionalState e = ENABLE)
        {
            T::Interrupt(TXEIE, e);
        }
        static __forceinline void IdleInterrupt(FunctionalState e = ENABLE)
        {
            T::Interrupt(IDLEIE, e);
        }
        // USART使能
        // USART失能
        static __forceinline void Enable(FunctionalState e = ENABLE)
        {
            URT1_CR1(13) = 0;
            URT1_CR1(13) = e;
        }
        static bool __forceinline Received(void)
        {
            return (bool)T::ReadRegNEmpty();
        }
        static bool __forceinline Transmitted(void)
        {
            return (bool)T::TransRegEmpty();
        }
        static u8 __forceinline Read(void)
        {
            return (u8)(* ((vu32 *) T::DATA));
        }
        static __forceinline void Write(u8 Data)
        {
            (* ((vu32 *) T::DATA)) = (Data & (uint16_t)0x01FF);
        }
        /**
          * @brief  Description "Transmits single data through the USARTx peripheral."
          * @param  None
          * @retval None
          */
        static __forceinline void Print(char const *str)
        {
            while(*str)
            {
                while(Transmitted());
                Write(*str);
                str++;
            }
        }
        /**
          * @brief  Description ""
          * @param  None
          * @retval None
          */
        CSerial &operator << (Manipulator m)
        {
            (*m)(*this);
            return *this;
        }

        CSerial &operator << (char const *str)
        {
            Print(str);
            return *this;
        }

        CSerial &operator << (char c)
        {
            while(Transmitted());
            Write(c);
            return *this;
        }
    };

    namespace serial_declaration
    {
        extern CSerial<CSerial_Info1>	Serial1;
        extern CSerial<CSerial_Info2>	Serial2;
        extern CSerial<CSerial_Info3>	Serial3;
        extern CSerial<CSerial_Info4>	Serial4;
        extern CSerial<CSerial_Info5>	Serial5;
    }
}
//!
//! @{
using namespace periph::serial_declaration;
//! @}

#endif /* __STM32FWLIB_SERIAL_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
