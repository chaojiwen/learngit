/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_usart.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-05
* Last Update        : 2014-11-05
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_USART_H
#define __STM32GLIBC_USART_H

#include "stm32glibc_type.h"


/* Configuration Mode enumeration --------------------------------------------*/
#define USART_COUNT					5

/* Exported constants --------------------------------------------------------*/
namespace periph_c
{
    enum USART_SR  				//status register
    {
        CTS		= Bit(9),
        LBD		= Bit(8),		// LIN Break detection flag

        TXE		= Bit(7),		// Transmit data empty
        TC		= Bit(6),		// Transmit complete
        RXNE	= Bit(5),		// Read data register not empty
        ILDE	= Bit(4),		// IDLE line dected
        ORE		= Bit(3),		// Overrun error
        NE		= Bit(2),		// noise error flag
        FE		= Bit(1),		// framing error
        PE		= Bit(0),		// parity error
    };
    enum USART_BRR  			// Baud rate register
    {
        DIV_Mantissa = BitFromTo(15, 4),
        DIV_Fraction = BitFromTo(3, 0),
    };
    enum USART_CR1  			// control register 1
    {
        UE		= Bit(13),		// USART enable
        // Word length, 0: 1 Start bit, 8 Data bits, n Stop bit	1: 1 Start bit, 9 Data bits, n Stop bit
        M		= Bit(12),
        WAKE	= Bit(11),		// Wakeup method, 0: Idle Line, 1: Address Mark
        PCE		= Bit(10),		// parity control enable
        PS		= Bit(9),		// Parity selection
        PEIE	= Bit(8),		// PE interrupt enable
        TXEIE	= Bit(7),		// TXE interrupt enable
        TCIE	= Bit(6),		// transmit complete interrupt enable
        RXNEIE 	= Bit(5),		// RXNE interrupt enable
        IDLEIE 	= Bit(4),		// IDLE interrupt enable
        TE		= Bit(3),		// Transmit enable
        RE		= Bit(2),		// Receiver enable
        RWU		= Bit(1),		// Receiver wakeup
        SBK		= Bit(0),		// SendBreck
    };
    enum USART_CR2
    {
        LINEN 	= Bit(13),				// LIN mode enable
        STOP 	= BitFromTo(13, 12),	// stop bits
        CLKEN 	= Bit(11),				// clock enable, allows the user to enable the CK pin.
        CPOL 	= Bit(10),				// clock polarity
        CPHA 	= Bit(9),				// Clock phase

        //	This bit allows the user to select whether the clock pulse associated with the last data bit
        //	transmitted (MSB) has to be output on the CK pin in synchronous mode.
        LBCL 	= Bit(8),				// last bit clock pulse
        LBDIE 	= Bit(6),				// lin break detection enable
        LBDL 	= Bit(5),				// lin break detection length
        ADD 	= BitFromTo(3, 0),		// address of the usart node
    };
    enum USART_CR3
    {
        CTSIE 	= Bit(10),				// CTS interrupt enable, This bit is not available for UART4 & UART5.
        //if CTS mode enabled, data is only transmitted when the nCTS input is asserted (tied to 0)
        CTSE	= Bit(9),				// CTS enable, This bit is not available for UART4 & UART5.
        //if RTS interrupt enabled, data is only reques ted when there is space in the receive buffer.
        RTSE 	= Bit(8),				// RTS enable
        DMAT 	= Bit(7),				// DMA enable transmitter
        DMAR 	= Bit(6),				// DMA enable receiver
        SCEN 	= Bit(5),				// Smartcard mode enable
        NACK 	= Bit(4),				// Smartcard NACK enable
        HDSEL 	= Bit(3),				// Half-duplex selection, Selection of Single-wire Half-duplex mode
        IRLP 	= Bit(2),				// IrDA low-power
        IREN 	= Bit(1),				// IrDA mode enable
        EIE		= Bit(0),				//
    };
    enum USART_GTPR
    {
        GT		= BitFromTo(15, 8),		// Guard time value
        PSC		= BitFromTo(7, 0),		// Prescaler value
    };
    enum USART_StopBits
    {
        USART_STOPBITS_1B  = BByte(00),	// 1 stop bit
        USART_STOPBITS_0B5 = BByte(01),	// 0.5 stop bits
        USART_STOPBITS_2B  = BByte(10),	// 2 stop bits
        USART_STOPBITS_1B5 = BByte(11),	// 1.5 stop bits
    };
    enum USART_Parity
    {
        USART_PARITY_NO    = BByte(00),	// disable parity bit
        USART_PARITY_EVEN  = BByte(10),	// even parity bits
        USART_PARITY_ODD   = BByte(11),	// odd parity bits
    };
    // 数据长度设置, 注意校验位也属于 data bit
    enum USART_WordLength
    {
        USART_WORDLENGTH_8DB = 0, 		// 8 个数据位
        USART_WORDLENGTH_9DB = 1,		// 9 个数据位
    };
    enum USART_HardwareFlowCtrl
    {
        USART_HWFlowControl_None    = BByte(00),	//
        USART_HWFlowControl_RTS  	  = BByte(01),	//
        USART_HWFlowControl_CTS     = BByte(10),	//
        USART_HWFlowControl_RTS_CTS = BByte(11),	//
    };
    enum USART_Mode
    {
        USART_MODE_Rx = 0x1,	//
        USART_MODE_Tx = 0x2,	//
    };
    enum USART_Int
    {
        PEIT	= Bit(8),		// PE interrupt enable
        TXEIT	= Bit(7),		// TXE interrupt enable
        TCIT	= Bit(6),		// transmit complete interrupt enable
        RXNEIT 	= Bit(5),		// RXNE interrupt enable
        IDLEIT 	= Bit(4),		// IDLE interrupt enable
    };
    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 状态寄存器(USART_SR)
    struct USART_SR_Struct
    {
        __IO uint16_t PE		                    : 1; //Bit0
        __IO uint16_t FE		                    : 1; //Bit1
        __IO uint16_t NE			                : 1; //Bit2
        __IO uint16_t ORE		                    : 1; //Bit3
        __IO uint16_t IDLE		                    : 1; //Bit4
        __IO uint16_t RXNE		                    : 1; //Bit5
        __IO uint16_t TC		                    : 1; //Bit6
        __IO uint16_t TXE		                    : 1; //Bit7
        __IO uint16_t LBD		                    : 1; //Bit8
        __IO uint16_t CTS		                    : 1; //Bit9
        __IO uint16_t Resv1                         : 6; //Bit10 ~ Bit15
    };

    typedef struct USART_SR_Struct USART_SR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 数据 寄存器(USART_DR)
    struct USART_DR_Struct
    {
        __IO uint16_t DR	                        : 8; //Bit0 ~ Bit7
        __IO uint16_t Resv1                         : 8; //Bit8 ~ Bit15
    };

    typedef struct USART_DR_Struct USART_DR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 波特率 寄存器(USART_BRR)
    struct USART_BRR_Struct
    {
        __IO uint16_t DIV_Fraction                  : 4; //Bit0 ~ Bit3
        __IO uint16_t DIV_Mantissa                  : 12; //Bit4 ~ Bit15
    };

    typedef struct USART_BRR_Struct USART_BRR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 控制寄存器1寄存器(USART_CR1)
    struct USART_CR1_Struct
    {
        __IO uint16_t SBK		                    : 1; //Bit0
        __IO uint16_t RWU		                    : 1; //Bit1
        __IO uint16_t RE			                : 1; //Bit2
        __IO uint16_t TE		                    : 1; //Bit3
        __IO uint16_t IDLEIE	                    : 1; //Bit4
        __IO uint16_t RXNEIE	                    : 1; //Bit5
        __IO uint16_t TCIE		                    : 1; //Bit6
        __IO uint16_t TXEIE		                    : 1; //Bit7
        __IO uint16_t PEIE		                    : 1; //Bit8
        __IO uint16_t PCE_PS	                    : 2; //Bit9 ~ Bit10
        __IO uint16_t WAKE		                    : 1; //Bit11
        __IO uint16_t M 		                    : 1; //Bit12
        __IO uint16_t UE		                    : 1; //Bit13
        __IO uint16_t Resv1                         : 2; //Bit14 ~ Bit15
    };

    typedef struct USART_CR1_Struct USART_CR1_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 控制寄存器2寄存器(USART_CR2)
    struct USART_CR2_Struct
    {
        __IO uint16_t ADD		                    : 4; //Bit0 ~ Bit3
        __IO uint16_t Resv1		                    : 1; //Bit4
        __IO uint16_t LBDL		                    : 1; //Bit5
        __IO uint16_t LBDIE		                    : 1; //Bit6
        __IO uint16_t Resv2		                    : 1; //Bit7
        __IO uint16_t LBCL		                    : 1; //Bit8
        __IO uint16_t CPHA		                    : 1; //Bit9
        __IO uint16_t CPOL		                    : 1; //Bit10
        __IO uint16_t CLKEN		                    : 1; //Bit11
        __IO uint16_t STOP		                    : 2; //Bit12 ~ Bit13
        __IO uint16_t LINEN		                    : 1; //Bit14
        __IO uint16_t Resv3                         : 1; //Bit15
    };

    typedef struct USART_CR2_Struct USART_CR2_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 控制寄存器3寄存器(USART_CR3)
    struct USART_CR3_Struct
    {
        __IO uint16_t EIE		                    : 1; //Bit0
        __IO uint16_t IREN		                    : 1; //Bit1
        __IO uint16_t IRLP			                : 1; //Bit2
        __IO uint16_t HDSEL		                    : 1; //Bit3
        __IO uint16_t NACK		                    : 1; //Bit4
        __IO uint16_t SCEN		                    : 1; //Bit5
        __IO uint16_t DMAR		                    : 1; //Bit6
        __IO uint16_t DMAT		                    : 1; //Bit7
        __IO uint16_t RTSE		                    : 1; //Bit8
        __IO uint16_t CTSE		                    : 1; //Bit9
        __IO uint16_t CTSIE		                    : 1; //Bit10
        __IO uint16_t Resv1		                    : 5; //Bit11 ~ Bit15
    };

    typedef struct USART_CR3_Struct USART_CR3_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 保护时间和预分频 寄存器(USART_GTPR)
    struct USART_GTPR_Struct
    {
        __IO uint16_t PSC	                        : 8; //Bit0 ~ Bit7
        __IO uint16_t GT	                        : 8; //Bit8 ~ Bit15
    };

    typedef struct USART_GTPR_Struct USART_GTPR_Typedef;

    typedef struct USART_Struct
    {
        __IO uint16_t SR;
        uint16_t  RESERVED0;
        __IO uint16_t DR;
        uint16_t  RESERVED1;
        __IO uint16_t BRR;
        uint16_t  RESERVED2;
        __IO uint16_t CR1;
        uint16_t  RESERVED3;
        __IO uint16_t CR2;
        uint16_t  RESERVED4;
        __IO uint16_t CR3;
        uint16_t  RESERVED5;
        __IO uint16_t GTPR;
        uint16_t  RESERVED6;

        // 设置是8个数据位，还是9个数据位
        void __forceinline __WordLen(USART_WordLength l)
        {
            USART_CR1_Typedef *USART_CR1 = ( USART_CR1_Typedef * ) & (CR1);

            /* Clear M bits */
            USART_CR1->M = 0;
            /* Set the M bits according to USART_WordLength value */
            USART_CR1->M = l;
        }
        // 设置校验位
        void __forceinline __Parity(USART_Parity p)
        {
            USART_CR1_Typedef *USART_CR1 = ( USART_CR1_Typedef * ) & (CR1);

            /* Clear M bits */
            USART_CR1->PCE_PS = 0;
            /* Set the M bits according to USART_WordLength value */
            USART_CR1->PCE_PS = p;
        }
        // 设置停止位
        void __forceinline __StopBits(USART_StopBits s)
        {
            USART_CR2_Typedef *USART_CR2 = ( USART_CR2_Typedef * ) & (CR2);

            /* Clear STOP[13:12] bits */
            USART_CR2->STOP = 0;
            /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit ------------*/
            /* Set STOP[13:12] bits according to USART_StopBits value */
            USART_CR2->STOP = s;
        }
        // 设置硬件流控
        void __forceinline __HardwareFlowControl(USART_HardwareFlowCtrl hfc)
        {
            USART_CR3_Typedef *USART_CR3 = ( USART_CR3_Typedef * ) & (CR3);

            /* Clear CTSE and RTSE bits */
            USART_CR3->CTSE = 0;
            USART_CR3->RTSE = 0;
            /* Configure the USART HFC -------------------------------------------------*/
            /* Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
            if (hfc & USART_HardwareFlowControl_CTS)
            {
                USART_CR3->CTSE = 1;
            }
            if (hfc & USART_HardwareFlowControl_RTS)
            {
                USART_CR3->RTSE = 1;
            }
        }
        // 设置波特率
        void __forceinline __BaudRate(uint32_t br, uint32_t apbclock)
        {
            /* Determine the integer part */
            /* Integer part computing in case Oversampling mode is 16 Samples */
            BRR = ((apbclock) / (br));
        }
        // 获得USART外设的 时钟
        bool __forceinline __GetClockID(uint32_t &wClockID, uint32_t& wAddress)
        {
			struct __RCC_PeriphID
			{
				uint32_t uPeriphID;
				uint32_t uPeriphRCCID;
			};
			int nIndex;
            const __RCC_PeriphID USART_Periph[USART_COUNT] =
            {
                {USART1_BASE, RCC_APB2Periph_USART1,},
                {USART2_BASE, RCC_APB1Periph_USART2,},
                {USART3_BASE, RCC_APB1Periph_USART3,},
                {UART4_BASE, RCC_APB1Periph_UART4,},
                {UART5_BASE, RCC_APB1Periph_UART5,},
            };
			for (nIndex = 0; nIndex < USART_COUNT; nIndex++)
			{
				if (USART_Periph[nIndex].uPeriphID == wAddress)
				{
					wClockID =  USART_Periph[nIndex].uPeriphRCCID;
					return true;
				}
			}
            return false;
        }
    public:
        bool Config(uint32_t ulBaudRate,
                    uint32_t eMode = USART_Mode_Tx | USART_Mode_Rx,
                    USART_WordLength eDataBits = USART_WORDLENGTH_8DB,	//数据位
                    USART_Parity eParity = USART_PARITY_NO,				//校验位
                    USART_StopBits eStopBits = USART_STOPBITS_1B);
        //! "设置串口的中断"
        void InterruptConfig(USART_Int IntBits, FunctionalState e = ENABLE);
        //! "发送器使能/失能"
        void USART_Cmd(uint32_t flg, FunctionalState e = ENABLE);
        //! "USART使能"
        void __forceinline Enable(FunctionalState e = ENABLE)
        {
            USART_CR1_Typedef *USART_CR1 = ( USART_CR1_Typedef * ) & (CR1);

            /* Enable the selected USART by setting the UE bit in the CR1 register */
            USART_CR1->UE = e;
        }
        //! "USART失能"
        void __forceinline Disable(FunctionalState e = DISABLE)
        {
            USART_CR1_Typedef *USART_CR1 = ( USART_CR1_Typedef * ) & (CR1);

            /* Disable the selected USART by clearing the UE bit in the CR1 register */
            USART_CR1->UE = e;
        }
        //! "检测是否有接收到数据"
        // 检测USART的状态，如果返回真，则不能使用Write写入数据
        bool __forceinline Received()
        {
            USART_SR_Typedef *USART_SR = ( USART_SR_Typedef * ) & (SR);

            return (USART_SR->RXNE != RESET);
        }
        //! "检测数据是否发送"
        bool __forceinline Transmitting()
        {
            USART_SR_Typedef *USART_SR = ( USART_SR_Typedef * ) & (SR);

            return (USART_SR->TXE != RESET);
        }
        //! "检测数据是否发送完成"
        bool __forceinline Transmitted()
        {
            USART_SR_Typedef *USART_SR = ( USART_SR_Typedef * ) & (SR);

            return (USART_SR->TC != RESET);
        }
        void __forceinline WriteByte(uint8_t Data)
        {
            (void)SR;
            /* Transmit Data */
            DR = (Data & (uint16_t)0x01FF);
        }
        uint8_t __forceinline ReadByte(void)
        {
            (void)SR;
            return DR;
        }
		
        /**
          * @brief  Description "Transmits single data through the USARTx peripheral."
          * @param  None
          * @retval None
          */
        void __forceinline PutChar(uint8_t Data)
        {
            while(!Transmitted());
            WriteByte(Data);
        }

        void Print(char const *str)
        {
            while(*str)
            {
                PutChar(*str);
                str++;
            }
        }

        /**
          * @brief  Description ""
          * @param  None
          * @retval None
          * /
        operator uint32_t() const
        {
            return *this;
        }
		
        operator struct USART_Struct *() const
        {
            return *this;
        }*/
		
        struct USART_Struct &operator << (char const *str)
        {
            Print(str);
            return *this;
        }

        struct USART_Struct &operator << (char c)
        {
            PutChar(c);
            return *this;
        }
    } USART_Typedef;
}
//!
//! @{
using periph_c::USART_Typedef;
//! @}

#define stUSART1			((USART_Typedef*)USART1_BASE)
#define stUSART2			((USART_Typedef*)USART2_BASE)
#define stUSART3			((USART_Typedef*)USART3_BASE)
#define stUSART4			((USART_Typedef*)UART4_BASE)
#define stUSART5			((USART_Typedef*)UART5_BASE)
#define COM1				(*stUSART1)
#define COM2				(*stUSART2)
#define COM3				(*stUSART3)
#define COM4				(*stUSART4)
#define COM5				(*stUSART5)

#endif /* __STM32GLIBC_USART_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
