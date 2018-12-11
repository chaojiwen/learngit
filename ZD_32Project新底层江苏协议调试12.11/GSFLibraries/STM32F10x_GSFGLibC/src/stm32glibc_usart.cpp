/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_usart.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : USART firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_usart.h"
#include "stm32glibc_rcc.h"
#include ".\template.h"


namespace periph_c
{

    /**
      * @brief  Description "Initializes the USARTx peripheral according to the specified
      *                      parameters in the USART_InitStruct ."
      * @param  None
      * @retval None
      */
    bool USART_Typedef::Config(u32 ulBaudRate,
                               u32 eMode,
                               USART_WordLength eDataBits,
                               USART_Parity eParity,
                               USART_StopBits eStopBits)
    {
        RCC_ClocksTypeDef RCC_ClocksStatus;
        u32 RCC_PeriphID = 0;
		u32 CUSART_BASE = (u32)this;
		
        if (!this->__GetClockID(RCC_PeriphID, CUSART_BASE))
            return false;
        if (CUSART_BASE == USART1_BASE)
        {
            CLOCK.APB2PeriphClockCmd(RCC_PeriphID, ENABLE);
        }
        else
        {
            CLOCK.APB1PeriphClockCmd(RCC_PeriphID, ENABLE);
        }
        CLOCK.GetClocksFreq(RCC_ClocksStatus);

        // 先失能
        this->Disable();
        // 设置波对率发生器
        if (CUSART_BASE == USART1_BASE)
        {
            this->__BaudRate(ulBaudRate, RCC_ClocksStatus.PCLK2_Frequency);
        }
        else
        {
            this->__BaudRate(ulBaudRate, RCC_ClocksStatus.PCLK1_Frequency);
        }
        // 设置数据位
        if (eParity == USART_PARITY_NO)
        {
            // 校验位
            this->__Parity(USART_PARITY_NO);
            this->__WordLen(USART_WORDLENGTH_8DB);
        }
        else
        {
            // 校验位
            this->__Parity(eParity);
            this->__WordLen(USART_WORDLENGTH_9DB);
        }
        // 停止位
        this->__StopBits(eStopBits);
        // 接收使能
        if (eMode & USART_Mode_Rx)
        {
            this->USART_Cmd(RE, ENABLE);
        }
        // 发送使能
        if (eMode & USART_Mode_Tx)
        {
            this->USART_Cmd(TE, ENABLE);
        }
        // 使能串口
        this->Enable();
        return true;
    }

    /**
      * @brief  Description "设置串口的中断"
      * @param  None
      * @retval None
      */
    void USART_Typedef::InterruptConfig(USART_Int IntBits, FunctionalState e)
    {
        CR1 = (e == DISABLE) ? ClearBBit((CR1), IntBits) : SetBBit((CR1), IntBits);
    }
    /**
      * @brief  Description "发送器使能/失能"
      * @param  None
      * @retval None
      */
    void USART_Typedef::USART_Cmd(u32 flg, FunctionalState e)
    {
        CR1 = (e == DISABLE) ? ClearBBit((CR1), flg) : SetBBit((CR1), flg);
    }
}

/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
