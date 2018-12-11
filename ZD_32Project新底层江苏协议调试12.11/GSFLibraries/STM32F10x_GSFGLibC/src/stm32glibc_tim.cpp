/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_tim.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : USART firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_tim.h"
#include "stm32glibc_rcc.h"

namespace periph_c
{
    /**
      * @brief  Description "Initializes the USARTx peripheral according to the specified
      *                      parameters in the USART_InitStruct ."
      * @param  TIM_Prescaler  定时器频率：Hz
      * @retval None
      */
    void BTIM_Typedef::Config(u16 TIM_Prescaler, u16 TIM_Period)
    {
        RCC_ClocksTypeDef RCC_ClocksStatus;

        CLOCK.GetClocksFreq(RCC_ClocksStatus);
        //rcc.APB2PeriphClockCmd(T::CLOCK, ENABLE);

        // 先失能
        EnableCounter(DISABLE);
        SetPrescaler((RCC_ClocksStatus.SYSCLK_Frequency / TIM_Prescaler) - 1);
        SetReloadVal((TIM_Period));
        EnableCounter(ENABLE);
    }
}
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
