/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32glibc_tim.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-05
* Last Update        : 2014-11-05
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_TIM_H
#define __STM32GLIBC_TIM_H

#include "stm32glibc_type.h"


/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
namespace	periph_c
{
    enum BTIM_StopBits
    {
        BTIM_STOPBITS_1B  = BByte(00),	// 1 stop bit
        BTIM_STOPBITS_0B5 = BByte(01),	// 0.5 stop bits
        BTIM_STOPBITS_2B  = BByte(10),	// 2 stop bits
        BTIM_STOPBITS_1B5 = BByte(11),	// 1.5 stop bits
    };
    enum BTIM_Parity
    {
        BTIM_PARITY_NO    = BByte(00),	// disable parity bit
        BTIM_PARITY_EVEN  = BByte(10),	// even parity bits
        BTIM_PARITY_ODD   = BByte(11),	// odd parity bits
    };
    // 数据长度设置, 注意校验位也属于 data bit
    enum BTIM_WordLength
    {
        BTIM_WORDLENGTH_8DB = 0, 		// 8 个数据位
        BTIM_WORDLENGTH_9DB = 1,		// 9 个数据位
    };
    enum BTIM_ClockDiv
    {
        BTIM_CKD_DIV1  		= BByte(00),	//
        BTIM_CKD_DIV2     	= BByte(01),	//
        BTIM_CKD_DIV4    	= BByte(10),	//
    };
    enum BTIM_CNT_Mode
    {
        BTIM_Mode_Up = 0x0, 		//
        BTIM_Mode_Down = 0x1,		//
    };
    enum BTIM_CtrlBit
    {
        CNT_EN	= 0,			// Counter enable
        UPD_DIS	= 1,			// Update disable
        UPD_REQ	= 2,			// Update request source
        OPL_MOD	= 3,			// One-pulse mode
        DIRECTION = 4,
        RLOD_EN	= 7,			// Auto-reload preload enable
    };
    enum BTIM_Interrupt
    {
        UPD_INT_EN	= (0),		// Update interrupt enable
        UPD_DMA_EN	= (8),		// Update DMA request enable
    };

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 控制寄存器(BTIM_CR1)
    struct BTIM_CR1_Struct
    {
        __IO uint16_t CEN		                    : 1; //Bit0
        __IO uint16_t UDIS		                    : 1; //Bit1
        __IO uint16_t URS			                : 1; //Bit2
        __IO uint16_t OPM		                    : 1; //Bit3
        __IO uint16_t Resv1		                    : 3; //Bit4 ~ Bit6
        __IO uint16_t ARPE		                    : 1; //Bit7
        __IO uint16_t Resv2		                    : 8; //Bit8 ~ Bit15
    };
    typedef struct BTIM_CR1_Struct BTIM_CR1_Typedef;

    struct TIM_CR1_Struct
    {
        __IO uint16_t CEN		                    : 1; //Bit0
        __IO uint16_t UDIS		                    : 1; //Bit1
        __IO uint16_t URS			                : 1; //Bit2
        __IO uint16_t OPM		                    : 1; //Bit3
        __IO uint16_t DIR		                    : 1; //Bit4
        __IO uint16_t CMS		                    : 2; //Bit5 ~ Bit6
        __IO uint16_t ARPE		                    : 1; //Bit7
        __IO uint16_t CKD		                    : 2; //Bit8 ~ Bit9
        __IO uint16_t Resv2		                    : 6; //Bit10 ~ Bit15
    };
    typedef struct TIM_CR1_Struct TIM_CR1_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 控制寄存器(BTIM_CR2)
    struct BTIM_CR2_Struct
    {
        __IO uint16_t Resv1		                    : 4; //Bit0 ~ Bit3
        __IO uint16_t MMS		                    : 3; //Bit4 ~ Bit6
        __IO uint16_t Resv2		                    : 1; //Bit7
        __IO uint16_t Resv3		                    : 8; //Bit8 ~ Bit15
    };
    typedef struct BTIM_CR2_Struct BTIM_CR2_Typedef;

    struct GTIM_CR2_Struct
    {
        __IO uint16_t Resv1		                    : 3; //Bit0 ~ Bit2
        __IO uint16_t CCDS		                    : 1; //Bit3
        __IO uint16_t MMS		                    : 3; //Bit4 ~ Bit6
        __IO uint16_t TI1S		                    : 1; //Bit7
        __IO uint16_t Resv3		                    : 8; //Bit8 ~ Bit15
    };
    typedef struct GTIM_CR2_Struct GTIM_CR2_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 从模式控制寄存器(GTIM_SMCR)
    struct GTIM_SMCR_Struct
    {
        __IO uint16_t SMS		                    : 3; //Bit0 ~ Bit2
        __IO uint16_t Resv1		                    : 1; //Bit3
        __IO uint16_t TS		                    : 3; //Bit4 ~ Bit6
        __IO uint16_t MSM		                    : 1; //Bit7
        __IO uint16_t ETF		                    : 4; //Bit8 ~ Bit11
        __IO uint16_t ETPS		                    : 2; //Bit12 ~ Bit13
        __IO uint16_t ECE		                    : 1; //Bit14
        __IO uint16_t ETP		                    : 1; //Bit15
    };
    typedef struct GTIM_SMCR_Struct GTIM_SMCR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 中断使能 寄存器(BTIM_DIER)
    struct BTIM_DIER_Struct
    {
        __IO uint16_t UIE		                    : 1; //Bit0
        __IO uint16_t Resv1	                        : 7; //Bit1 ~ Bit7
        __IO uint16_t UDE		                    : 1; //Bit8
        __IO uint16_t Resv2	                        : 7; //Bit9 ~ Bit15
    };
    typedef struct BTIM_DIER_Struct BTIM_DIER_Typedef;

    struct GTIM_DIER_Struct
    {
        __IO uint16_t UIE		                    : 1; //Bit0
        __IO uint16_t CC1IE		                    : 1; //Bit1
        __IO uint16_t CC2IE		                    : 1; //Bit2
        __IO uint16_t CC3IE		                    : 1; //Bit3
        __IO uint16_t CC4IE		                    : 1; //Bit4
        __IO uint16_t COMIE		                    : 1; //Bit5
        __IO uint16_t TIE		                    : 1; //Bit6
        __IO uint16_t BIE		                    : 1; //Bit7
        __IO uint16_t UDE		                    : 1; //Bit8
        __IO uint16_t CC1DE		                    : 1; //Bit9
        __IO uint16_t CC2DE		                    : 1; //Bit10
        __IO uint16_t CC3DE		                    : 1; //Bit11
        __IO uint16_t CC4DE		                    : 1; //Bit12
        __IO uint16_t COMDE		                    : 1; //Bit13
        __IO uint16_t TDE		                    : 1; //Bit14
        __IO uint16_t Resv1		                    : 1; //Bit15
    };

    typedef struct GTIM_DIER_Struct GTIM_DIER_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 状态 寄存器(BTIM_SR)
    struct GTIM_SR_Struct
    {
        __IO uint16_t UIF                  			: 1; //Bit0
        __IO uint16_t Resv1			                : 15; //Bit1 ~ Bit15
    };
    typedef struct GTIM_SR_Struct GTIM_SR_Typedef;

    struct BTIM_SR_Struct
    {
        __IO uint16_t UIF                  			: 1; //Bit0
        __IO uint16_t CC1IF                			: 1; //Bit1
        __IO uint16_t CC2IF                			: 1; //Bit2
        __IO uint16_t CC3IF                			: 1; //Bit3
        __IO uint16_t CC4IF                			: 1; //Bit4
        __IO uint16_t Resv1		                    : 1; //Bit5
        __IO uint16_t TIF                  			: 1; //Bit6
        __IO uint16_t Resv2		                    : 2; //Bit7 ~ Bit8
        __IO uint16_t CC1OF                			: 1; //Bit9
        __IO uint16_t CC2OF                			: 1; //Bit10
        __IO uint16_t CC3OF                			: 1; //Bit11
        __IO uint16_t CC4OF                			: 1; //Bit12
        __IO uint16_t Resv3			                : 3; //Bit13 ~ Bit15
    };
    typedef struct BTIM_SR_Struct BTIM_SR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 事件产生寄存器(BTIM_EGR)
    struct BTIM_EGR_Struct
    {
        __IO uint16_t UG		                    : 1; //Bit0
        __IO uint16_t Resv1		                    : 15; //Bit1 ~ Bit15
    };
    typedef struct BTIM_EGR_Struct BTIM_EGR_Typedef;

    struct GTIM_EGR_Struct
    {
        __IO uint16_t UG		                    : 1; //Bit0
        __IO uint16_t CC1G		                    : 1; //Bit1
        __IO uint16_t CC2G		                    : 1; //Bit2
        __IO uint16_t CC3G		                    : 1; //Bit3
        __IO uint16_t CC4G		                    : 1; //Bit4
        __IO uint16_t Resv1		                    : 1; //Bit5
        __IO uint16_t TG                  			: 1; //Bit6
        __IO uint16_t Resv2		                    : 9; //Bit7 ~ Bit15
    };
    typedef struct GTIM_EGR_Struct GTIM_EGR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 捕获/比较使能寄存器(GTIM_CCER)
    struct GTIM_CCER_Struct
    {
        __IO uint16_t CC1E		                    : 1; //Bit0
        __IO uint16_t CC1P		                    : 1; //Bit1
        __IO uint16_t Resv1		                    : 1; //Bit2 ~ Bit3
        __IO uint16_t CC2E		                    : 1; //Bit4
        __IO uint16_t CC2P		                    : 1; //Bit5
        __IO uint16_t Resv2                			: 1; //Bit6 ~ Bit7
        __IO uint16_t CC3E		                    : 1; //Bit8
        __IO uint16_t CC3P		                    : 1; //Bit9
        __IO uint16_t Resv3		                    : 1; //Bit10 ~ Bit11
        __IO uint16_t CC4E		                    : 1; //Bit12
        __IO uint16_t CC4P		                    : 1; //Bit13
        __IO uint16_t Resv4                			: 1; //Bit14 ~ Bit15
    };
    typedef struct GTIM_CCER_Struct GTIM_CCER_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 计数 寄存器(TIM_CNT)
    struct TIM_CNT_Struct
    {
        __IO uint16_t CNT                  			: 16; //Bit0 ~ Bit15
    };
    typedef struct TIM_CNT_Struct TIM_CNT_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 预分频 寄存器(TIM_PSC)
    struct TIM_PSC_Struct
    {
        __IO uint16_t PSC                  			: 16; //Bit0 ~ Bit15
    };

    typedef struct TIM_PSC_Struct TIM_PSC_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! 重载 寄存器(TIM_ARR)
    struct TIM_ARR_Struct
    {
        __IO uint16_t PSC                  			: 16; //Bit0 ~ Bit15
    };

    typedef struct TIM_ARR_Struct TIM_ARR_Typedef;

    typedef struct BTIM_Struct
    {
        __IO uint16_t CR1;
        uint16_t  RESERVED0;
        __IO uint16_t CR2;
        uint16_t  RESERVED1;
        __IO uint16_t SMCR;
        uint16_t  RESERVED2;
        __IO uint16_t DIER;
        uint16_t  RESERVED3;
        __IO uint16_t SR;
        uint16_t  RESERVED4;
        __IO uint16_t EGR;
        uint16_t  RESERVED5;
        __IO uint16_t CCMR1;
        uint16_t  RESERVED6;
        __IO uint16_t CCMR2;
        uint16_t  RESERVED7;
        __IO uint16_t CCER;
        uint16_t  RESERVED8;
        __IO uint16_t CNT;
        uint16_t  RESERVED9;
        __IO uint16_t PSC;
        uint16_t  RESERVED10;
        __IO uint16_t ARR;
        uint16_t  RESERVED11;
        __IO uint16_t RCR;
        uint16_t  RESERVED12;
        __IO uint16_t CCR1;
        uint16_t  RESERVED13;
        __IO uint16_t CCR2;
        uint16_t  RESERVED14;
        __IO uint16_t CCR3;
        uint16_t  RESERVED15;
        __IO uint16_t CCR4;
        uint16_t  RESERVED16;
        __IO uint16_t BDTR;
        uint16_t  RESERVED17;
        __IO uint16_t DCR;
        uint16_t  RESERVED18;
        __IO uint16_t DMAR;
        uint16_t  RESERVED19;

        void Ctrl1(uint32_t cid, FunctionalState e = ENABLE)
        {
            CR1 = AssignBit(uint16_t, CR1, Bit(cid), (e) << cid);
        }
        void Ctrl2(uint32_t cid, FunctionalState e = ENABLE)
        {
            CR2 = AssignBit(uint16_t, CR2, Bit(cid), (e) << cid);
        }
    public:
        /**
          * @brief  Description ""
          * @param  TIM_Prescaler  定时器频率：Hz
          * @retval None
          */
        void Config(uint16_t TIM_Prescaler,
                    uint16_t TIM_Period);
        /**
          * @brief  Description "设置定时器Update中断"
          * @param  BTIMInt	由 enum BTIM_Interrupt 中的成员相或得到
          * @param  e			使能(ENABLE)/失能(DISABLE)
          * @retval None
          */
        void UpdateInterrupt(FunctionalState e = ENABLE)
        {
            BTIM_DIER_Typedef *BTIM_IE = ( BTIM_DIER_Typedef * ) & (DIER);

            BTIM_IE->UIE = e;
        }
        /**
          * @brief  Description "使能指定定时器的计数器"
          * @param  BTIMInt	由 enum BTIM_Interrupt 中的成员相或得到
          * @param  e		使能(ENABLE)/失能(DISABLE)
          * @retval None
          */
        void EnableCounter(FunctionalState e = ENABLE)
        {
            Ctrl1(CNT_EN, e);
        }
        void EnableUpdate(FunctionalState e = ENABLE)
        {
            Ctrl1(UPD_DIS, e ? DISABLE : ENABLE);
        }
        void EnableAutoReload(FunctionalState e = ENABLE)
        {
            Ctrl1(RLOD_EN, e);
        }
        void ClearUpdate(void)
        {
            BTIM_SR_Typedef *BTIM_SR = ( BTIM_SR_Typedef * ) & (SR);

            BTIM_SR->UIF = 0;
        }
        void SetCounterMode(uint16_t mode)
        {
            Ctrl1(DIRECTION, (FunctionalState)mode);
        }
        // 设置定时器的时钟分频因子
        void SetClockDiv(BTIM_ClockDiv TIM_ClockDivision)
        {
            TIM_CR1_Typedef *TIM_CR1 = ( TIM_CR1_Typedef * ) & (CR1);

            TIM_CR1->CKD = TIM_ClockDivision;
        }
        // 设置定时器的时钟预分频器
        void SetPrescaler(uint16_t TIM_Prescaler)
        {
            PSC = ((TIM_Prescaler != 0) ? (TIM_Prescaler - 1) : (TIM_Prescaler));
        }
        // 设置定时器自动重载的值
        void SetReloadVal(uint16_t TIM_Period)
        {
            ARR = TIM_Period;
        }
        // 设置定时器的计数器，一般用来设置初始值
        void SetCounter(uint16_t value)
        {
            CNT = value;
        }
        bool GetUpdateFlag(void)
        {
            BTIM_SR_Typedef *BTIM_SR     = ( BTIM_SR_Typedef * ) & (SR);

            if ((BTIM_SR->UIF != (uint16_t)RESET))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        bool GetUpdateStatus(void)
        {
            BTIM_SR_Typedef *BTIM_SR     = ( BTIM_SR_Typedef * ) & (SR);
            BTIM_DIER_Typedef *BTIM_DIER = ( BTIM_DIER_Typedef * ) & (DIER);

            if ((BTIM_SR->UIF != (uint16_t)RESET) && (BTIM_DIER->UIE != (uint16_t)RESET))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    } BTIM_Typedef;
}
//!
//! @{
using periph_c::BTIM_Typedef;
//! @}
//!
//! @{
#define stTIM6				((BTIM_Typedef*)TIM6_BASE)
#define TIMER6				(*stTIM6)
#define stTIM7				((BTIM_Typedef*)TIM7_BASE)
#define TIMER7				(*stTIM7)
//! @}

#endif /* __STM32GLIBC_TIM_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
