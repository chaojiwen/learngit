/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_scb.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : USART firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_scb.h"
#include "template.h"

namespace mcucore
{
    /**
      * @brief  Description ""
      * @param  None
      * @retval None
      */
    PriorityGroupingStyle SCB_Typedef::GetPriorityGrouping()
    {
        return (PriorityGroupingStyle)ReadField(AIRCR, PRIGROUP);
    }

    void SCB_Typedef::SetPriorityGrouping(PriorityGroupingStyle s)
    {
        AIRCR = assignf(assignf(AIRCR, PRIGROUP, s), VECTKEYSTAT, CONST_REGISTER_KEY) ;
    }

    void SCB_Typedef::SystemReset(void)
    {
        AIRCR = assignf(assignf(AIRCR, SYSRESETREQ, 0x1), VECTKEYSTAT, CONST_REGISTER_KEY) ;
		__dsb(0);
		while (true);
    }
}
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
