/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_systick.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : USART firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_systick.h"
#include "stm32glibc_rcc.h"
#include "stm32glibc_scb.h"
#include "template.h"

namespace mcucore
{
    /**
      * @brief  Description ""
      * @param  None
      * @retval None
      */
    bool Systick_Typedef::Config(uint32_t ticks, uint32_t CLKSource)
    {
        uint32_t Systick_ClockDiv = 1;
        SYSTICK_CTRL_Typedef *SYSTICK_CTRL = ( SYSTICK_CTRL_Typedef * ) & (CTRL);
        /* Check the parameters */
        /* set Priority for Cortex-M0 System Interrupts */
        MCUSCB.SystemPriorityConfig(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);

        /* Load the SysTick Counter Value */
        VAL = 0;
        /* Enable SysTick IRQ and SysTick Timer */
        if (CLKSource == SYSTICK_CLKSRC_HCLK)
        {
            SYSTICK_CTRL->CLKSOURCE = 1;
            Systick_ClockDiv = 1;
        }
        else
        {
            SYSTICK_CTRL->CLKSOURCE = 0;
            Systick_ClockDiv = 8;
        }
        /* Reload value impossible */
        if ((ticks / Systick_ClockDiv) > SYSTICK_LOAD_RELOAD)
            return (false);
        /* set reload register */
        LOAD  = ((ticks / Systick_ClockDiv) & SYSTICK_LOAD_RELOAD) - 1;
        SYSTICK_CTRL->TICKINT = 1;
        SYSTICK_CTRL->ENABLE = 1;
        /* Function successful */
        return (true);
    }

    void Systick_Typedef::DelayUS(uint32_t times)
    {
        SYSTICK_CTRL_Typedef *SYSTICK_CTRL = ( SYSTICK_CTRL_Typedef * ) & (CTRL);
        uint32_t temp;
        unsigned __int64 load;
        RCC_ClocksTypeDef 	MCU_Clocks;
        CLOCK.GetClocksFreq(MCU_Clocks);

        load = times * MCU_Clocks.SYSCLK_Frequency;
        if (1 == SYSTICK_CTRL->CLKSOURCE)
        {
            load /= (1000000);
        }
        else
        {
            load /= (8000000);
        }
        LOAD = (load & SYSTICK_LOAD_RELOAD) - 1;	//时间加载
        VAL = 0x00;      							//清空计数器
        SYSTICK_CTRL->TICKINT = 0;
        SYSTICK_CTRL->ENABLE = 0x01;				//开始倒数
        do
        {
            temp = SYSTICK_CTRL->COUNTFLAG;
        }
        while(SYSTICK_CTRL->ENABLE && !(temp)); 	//等待时间到达
        SYSTICK_CTRL->ENABLE = 0x00;				//关闭计数器
        VAL = 0x00;      							//清空计数器
    }

    void Systick_Typedef::DelayMS(uint32_t times)
    {
        SYSTICK_CTRL_Typedef *SYSTICK_CTRL = ( SYSTICK_CTRL_Typedef * ) & (CTRL);
        uint32_t temp;
        unsigned __int64 load;
        RCC_ClocksTypeDef 	MCU_Clocks;
        CLOCK.GetClocksFreq(MCU_Clocks);

        load = times * MCU_Clocks.SYSCLK_Frequency;
        if (1 == SYSTICK_CTRL->CLKSOURCE)
        {
            load /= (1000);
        }
        else
        {
            load /= (8000);
        }
        LOAD = (load & SYSTICK_LOAD_RELOAD) - 1;	//时间加载
        VAL = 0x00;      							//清空计数器
        SYSTICK_CTRL->TICKINT = 0;
        SYSTICK_CTRL->ENABLE = 0x01;				//开始倒数
        do
        {
            temp = SYSTICK_CTRL->COUNTFLAG;
        }
        while(SYSTICK_CTRL->ENABLE && !(temp)); 	//等待时间到达
        SYSTICK_CTRL->ENABLE = 0x00;				//关闭计数器
        VAL = 0x00;      							//清空计数器
    }
}
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
