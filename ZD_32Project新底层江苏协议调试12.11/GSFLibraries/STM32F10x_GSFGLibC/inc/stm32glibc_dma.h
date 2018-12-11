/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32glibc_dma.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-05
* Last Update        : 2014-11-05
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_DMA_H
#define __STM32GLIBC_DMA_H

#include "stm32glibc_type.h"


/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
namespace	periph_c
{
    enum Enum_DMA_DIR
    {
        DMA_DIR_PeriphSRC = 0,
        DMA_DIR_PeriphDST = 1,
    };

    enum Enum_DMA_PeriphInc
    {
        DMA_PeriphInc_Disable = 0,
        DMA_PeriphInc_Enable = 1,
    };

    enum Enum_DMA_MemInc
    {
        DMA_MemInc_Disable = 0,
        DMA_MemInc_Enable = 1,
    };

    enum Enum_DMA_PeriphDataSize
    {
        DMA_PeriphDataSize_Byte = 0,
        DMA_PeriphDataSize_HalfWord = 1,
        DMA_PeriphDataSize_Word = 2,
    };

    enum Enum_DMA_MemDataSize
    {
        DMA_MemDataSize_Byte = 0,
        DMA_MemDataSize_HalfWord = 1,
        DMA_MemDataSize_Word = 2,
    };

    enum Enum_DMA_Circular_Mode
    {
        DMA_MODE_Normal = 0,
        DMA_MODE_Circular = 1,
    };

    enum Enum_DMA_Priority_Level
    {
        DMA_PRIORITY_Low = 0,
        DMA_PRIORITY_Medium = 1,
        DMA_PRIORITY_High = 2,
        DMA_PRIORITY_VeryHigh = 3,
    };

    enum Enum_DMA_M2M_Mode
    {
        DMA_M2M_DISABLE = 0,
        DMA_M2M_ENABLE = 1,
    };

    enum Enum_DMA_Channel
    {
        Channel1 = (0),
        Channel2 = (1),
        Channel3 = (2),
        Channel4 = (3),
        Channel5 = (4),
        Channel6 = (5),
        Channel7 = (6),
    };

    enum Enum_DMA_IntFlag
    {
        DMA_Global 		= (0),
        DMA_TransComp 	= (1),
        DMA_HalfTrans 	= (2),
        DMA_TransErr	= (3),
    };

    /**
      * @brief  DMA Init structure definition
      */
    typedef struct
    {
        uint32_t DMA_PeripheralBaseAddr;
        uint32_t DMA_MemoryBaseAddr;
        uint32_t DMA_DIR;
        uint32_t DMA_BufferSize;
        uint32_t DMA_PeripheralInc;
        uint32_t DMA_MemoryInc;
        uint32_t DMA_PeripheralDataSize;
        uint32_t DMA_MemoryDataSize;
        uint32_t DMA_Mode;
        uint32_t DMA_Priority;
        uint32_t DMA_M2M;
    } DMA_Init;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ÖÐ¶Ï×´Ì¬¼Ä´æÆ÷(DMA_ISR)
    struct DMA_ISR_Struct
    {
        __IO uint32_t GIF1		                    : 1; //Bit0
        __IO uint32_t TCIF1		                    : 1; //Bit1
        __IO uint32_t HTIF1			                : 1; //Bit2
        __IO uint32_t TEIF1		                    : 1; //Bit3
        __IO uint32_t GIF2		                    : 1; //Bit4
        __IO uint32_t TCIF2		                    : 1; //Bit5
        __IO uint32_t HTIF2		                    : 1; //Bit6
        __IO uint32_t TEIF2		                    : 1; //Bit7
        __IO uint32_t GIF3		                    : 1; //Bit8
        __IO uint32_t TCIF3		                    : 1; //Bit9
        __IO uint32_t HTIF3		                    : 1; //Bit10
        __IO uint32_t TEIF3		                    : 1; //Bit11
        __IO uint32_t GIF4		                    : 1; //Bit12
        __IO uint32_t TCIF4		                    : 1; //Bit13
        __IO uint32_t HTIF4		                    : 1; //Bit14
        __IO uint32_t TEIF4		                    : 1; //Bit15
        __IO uint32_t GIF5		                    : 1; //Bit16
        __IO uint32_t TCIF5		                    : 1; //Bit17
        __IO uint32_t HTIF5		                    : 1; //Bit18
        __IO uint32_t TEIF5		                    : 1; //Bit19
        __IO uint32_t GIF6		                    : 1; //Bit20
        __IO uint32_t TCIF6		                    : 1; //Bit21
        __IO uint32_t HTIF6		                    : 1; //Bit22
        __IO uint32_t TEIF6		                    : 1; //Bit23
        __IO uint32_t GIF7		                    : 1; //Bit24
        __IO uint32_t TCIF7		                    : 1; //Bit25
        __IO uint32_t HTIF7		                    : 1; //Bit26
        __IO uint32_t TEIF7		                    : 1; //Bit27
        __IO uint32_t Resv28_31                     : 4; //Bit28 ~ Bit31
    };
    typedef struct DMA_ISR_Struct DMA_ISR_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ÖÐ¶Ï±êÖ¾Çå³þ¼Ä´æÆ÷(DMA_IFCR)
    struct DMA_IFCR_Struct
    {
        __IO uint32_t CGIF1		                    : 1; //Bit0
        __IO uint32_t CTCIF1						: 1; //Bit1
        __IO uint32_t CHTIF1						: 1; //Bit2
        __IO uint32_t CTEIF1						: 1; //Bit3
        __IO uint32_t CGIF2		                    : 1; //Bit4
        __IO uint32_t CTCIF2						: 1; //Bit5
        __IO uint32_t CHTIF2						: 1; //Bit6
        __IO uint32_t CTEIF2						: 1; //Bit7
        __IO uint32_t CGIF3		                    : 1; //Bit8
        __IO uint32_t CTCIF3						: 1; //Bit9
        __IO uint32_t CHTIF3						: 1; //Bit10
        __IO uint32_t CTEIF3						: 1; //Bit11
        __IO uint32_t CGIF4		                    : 1; //Bit12
        __IO uint32_t CTCIF4						: 1; //Bit13
        __IO uint32_t CHTIF4						: 1; //Bit14
        __IO uint32_t CTEIF4						: 1; //Bit15
        __IO uint32_t CGIF5		                    : 1; //Bit16
        __IO uint32_t CTCIF5						: 1; //Bit17
        __IO uint32_t CHTIF5						: 1; //Bit18
        __IO uint32_t CTEIF5						: 1; //Bit19
        __IO uint32_t CGIF6		                    : 1; //Bit20
        __IO uint32_t CTCIF6						: 1; //Bit21
        __IO uint32_t CHTIF6						: 1; //Bit22
        __IO uint32_t CTEIF6						: 1; //Bit23
        __IO uint32_t CGIF7		                    : 1; //Bit24
        __IO uint32_t CTCIF7						: 1; //Bit25
        __IO uint32_t CHTIF7						: 1; //Bit26
        __IO uint32_t CTEIF7						: 1; //Bit27
        __IO uint32_t Resv28_31                     : 4; //Bit28 ~ Bit31
    };
    typedef struct DMA_IFCR_Struct DMA_IFCR_Typedef;

    typedef struct DMA_Struct
    {
        __IO uint32_t ISR;
        __IO uint32_t IFCR;

    public:
        bool GetFlagStatus(enum Enum_DMA_IntFlag flg, FunctionalState e = ENABLE)
        {
            /* Check the status of the specified DMAy flag */
            if ((ISR & flg) != (uint32_t)RESET)
            {
                /* DMAy_FLAG is set */
                return true;
            }
            else
            {
                /* DMAy_FLAG is reset */
                return false;
            }
        }
        void ClearFlag(enum Enum_DMA_IntFlag flg)
        {
            /* Clear the selected DMAy flags */
            IFCR = flg;
        }
    } DMA_Typedef;

    /**************** Generate by STM32 StdPeriph v3.5 ****************/
    /****************           program by Wang Zhiyix ****************/
    //! ÅäÖÃ¼Ä´æÆ÷(DMA_CCR)
    struct DMA_CCR_Struct
    {
        __IO uint32_t EN		                    : 1; //Bit0
        __IO uint32_t TCIE		                    : 1; //Bit1
        __IO uint32_t HTIE			                : 1; //Bit2
        __IO uint32_t TEIE		                    : 1; //Bit3
        __IO uint32_t DIR		                    : 1; //Bit4
        __IO uint32_t CIRC		                    : 1; //Bit5
        __IO uint32_t PINC		                    : 1; //Bit6
        __IO uint32_t MINC		                    : 1; //Bit7
        __IO uint32_t PSIZE		                    : 2; //Bit8 ~ Bit9
        __IO uint32_t MSIZE		                    : 2; //Bit10 ~ Bit11
        __IO uint32_t PL		                    : 2; //Bit12 ~ Bit13
        __IO uint32_t MEM2MEM	                    : 1; //Bit14
        __IO uint32_t Resv15_31                     : 17; //Bit15 ~ Bit31
    };
    typedef struct DMA_CCR_Struct DMA_CCR_Typedef;

    typedef struct DMA_Channel_Struct
    {
        __IO uint32_t CCR;
        __IO uint32_t CNDTR;
        __IO uint32_t CPAR;
        __IO uint32_t CMAR;

    public:
        void Config(DMA_Init &cfg);
        /**
          * @brief  Description "Í¨µÀ¿ªÆô (Channel enable) "
          * @param  e		Ê¹ÄÜ(ENABLE)/Ê§ÄÜ(DISABLE)
          * @retval None
          */
        void ChannelEnable(FunctionalState e = ENABLE)
        {
            DMA_CCR_Typedef *DMA_CCR = ( DMA_CCR_Typedef * ) & (CCR);

            DMA_CCR->EN = e;
        }
        finline uint32_t GetTransNumber(void)
        {
            return (CNDTR);
        }
        finline void SetTransNumber(uint32_t val)
        {
            (CNDTR) = val & 0xFFFF;
        }
        finline uint32_t GetPeriphAddress(void)
        {
            return (CPAR);
        }
        finline void SetPeriphAddress(uint32_t addr)
        {
            (CPAR) = addr;
        }
        finline uint32_t GetMemAddress(void)
        {
            return (CMAR);
        }
        finline void SetMemAddress(uint32_t addr)
        {
            (CMAR) = addr;
        }
    } DMA_Channel_Typedef;
}
//!
//! @{
using periph_c::DMA_Typedef;
using periph_c::DMA_Channel_Typedef;
//! @}
//!
//! @{
#define stDMA1				((DMA_Typedef*)DMA1_BASE)
#define DMAG1				(*stDMA1)
#define stDMA2				((DMA_Typedef*)DMA2_BASE)
#define DMAG2				(*stDMA2)
//! @}
//!
//! @{
#define stDMA1C1			((DMA_Channel_Typedef*)DMA1_Channel1_BASE)
#define DMA1Chan1			(*stDMA1C1)
#define stDMA1C2			((DMA_Channel_Typedef*)DMA1_Channel2_BASE)
#define DMA1Chan2			(*stDMA1C2)
#define stDMA1C3			((DMA_Channel_Typedef*)DMA1_Channel3_BASE)
#define DMA1Chan3			(*stDMA1C3)
#define stDMA1C4			((DMA_Channel_Typedef*)DMA1_Channel4_BASE)
#define DMA1Chan4			(*stDMA1C4)
#define stDMA1C5			((DMA_Channel_Typedef*)DMA1_Channel5_BASE)
#define DMA1Chan5			(*stDMA1C5)
#define stDMA1C6			((DMA_Channel_Typedef*)DMA1_Channel6_BASE)
#define DMA1Chan6			(*stDMA1C6)
#define stDMA1C7			((DMA_Channel_Typedef*)DMA1_Channel7_BASE)
#define DMA1Chan7			(*stDMA1C7)
//! @}
//!
//! @{
#define stDMA2C1			((DMA_Channel_Typedef*)DMA2_Channel1_BASE)
#define DMA2Chan1			(*stDMA2C1)
#define stDMA2C2			((DMA_Channel_Typedef*)DMA2_Channel2_BASE)
#define DMA2Chan2			(*stDMA2C2)
#define stDMA2C3			((DMA_Channel_Typedef*)DMA2_Channel3_BASE)
#define DMA2Chan3			(*stDMA2C3)
#define stDMA2C4			((DMA_Channel_Typedef*)DMA2_Channel4_BASE)
#define DMA2Chan4			(*stDMA2C4)
#define stDMA2C5			((DMA_Channel_Typedef*)DMA2_Channel5_BASE)
#define DMA2Chan5			(*stDMA2C5)
//! @}

#endif /* __STM32GLIBC_DMA_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
