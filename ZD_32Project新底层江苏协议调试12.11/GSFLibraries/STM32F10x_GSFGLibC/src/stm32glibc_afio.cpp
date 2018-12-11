/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32glibc_afio.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_afio.h"

namespace periph_c
{
    /**
      * @brief  Description "Changes the mapping of the specified pin."
      * @param  AFIO_Remap: selects the pin to remap.
      * @retval None
      */
    void AFIO_Typedef::PinRemapConfig(u32 AFIO_Remap, FunctionalState NewState)
    {
		u32 tmpval = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;

        if((AFIO_Remap & 0x80000000) == 0x80000000)
        {
            tmpreg = MAPR2;
        }
        else
        {
            tmpreg = MAPR;
        }
        tmpmask = (AFIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
        tmpval = AFIO_Remap & LSB_MASK;

        //！ trible BITs
        if ((AFIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) ==
                (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
        {
            tmpreg &= DBGAFR_SWJCFG_MASK;
            MAPR &= DBGAFR_SWJCFG_MASK;
        }
        //! double BITs
        else if ((AFIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
        {
            // get BIT location in register
            tmp1 = ((u32)0x03) << tmpmask;
            tmpreg &= ~tmp1;
            tmpreg |= ~DBGAFR_SWJCFG_MASK;
        }
        //! single BIT
        else
        {
            // (AFIO_Remap >> 0x15) 判断是否在高半字节 Yes:1,No:0
            tmpreg &= ~(tmpval << ((AFIO_Remap >> 0x15) * 0x10));
            tmpreg |= ~DBGAFR_SWJCFG_MASK;
        }

        if (NewState != DISABLE)
        {
            tmpreg |= (tmpval << ((AFIO_Remap >> 0x15) * 0x10));
        }

        if((AFIO_Remap & 0x80000000) == 0x80000000)
        {
            MAPR2 = tmpreg;
        }
        else
        {
            MAPR = tmpreg;
        }
    }

    /**
      * @brief  Description "Selects the GPIO pin used as EXTI Line."
      * @param  GPIO_PortSource: selects the GPIO port to be used as source for EXTI lines.
      * @param  GPIO_PinSource: specifies the EXTI line to be configured.
      * @retval None
      */
    void AFIO_Typedef::EXTILineConfig(u8 AFIO_PortSource, u8 AFIO_PinSource)
    {
        u32 tmpval = 0x00;

        tmpval = ((u32)0x0F) << (0x04 * (AFIO_PortSource & (u8)0x03));
        EXTICR[AFIO_PinSource >> 0x02] &= ~tmpval;
        EXTICR[AFIO_PinSource >> 0x02] |= (((u32)AFIO_PortSource) << (0x04 * (AFIO_PinSource & (u8)0x03)));
    }
}
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
