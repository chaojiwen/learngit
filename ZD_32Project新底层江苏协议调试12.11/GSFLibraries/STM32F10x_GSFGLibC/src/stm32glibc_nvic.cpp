/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_nvic.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : USART firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_nvic.h"

namespace mcucore
{
    /**
      * @brief  Description "Initializes the USARTx peripheral according to the specified
      *                      parameters in the USART_InitStruct ."
      * @param  None
      * @retval None
      */
    bool NVIC_Typedef::IRQConfig(IRQn_Type n, FunctionalState e, u8 groupPri, u8 subPri)
    {
        if(e == DISABLE)
            ICER[n / 32] = Bit(n % 32);
        else
            ISER[n / 32] = Bit(n % 32);

        PriorityGroupingStyle s = MCUSCB.GetPriorityGrouping();
        // 下面计算并用groupPri来保存最终要写到IP寄存器的值
        switch(s)
        {
        case GRP16_SUB1:
        {
            groupPri <<= 4;
        }
        break;
        case GRP8_SUB2:
        {
            groupPri <<= 5;
            groupPri |= ((subPri & BByte(1)) << 4);
        }
        break;
        case GRP4_SUB4:
        {
            groupPri <<= 6;
            groupPri |= ((subPri & BByte(11)) << 4);
        }
        break;
        case GRP2_SUB8:
        {
            groupPri <<= 7;
            groupPri |= ((subPri & BByte(111)) << 4);
        }
        break;
        case GRP1_SUB16:
        {
            groupPri = (subPri << 4);
        }
        break;
        default:
        {
            // 出错了?
            return false;
        }
        }
        IP[n] = groupPri;
        return true;
    }
}
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
