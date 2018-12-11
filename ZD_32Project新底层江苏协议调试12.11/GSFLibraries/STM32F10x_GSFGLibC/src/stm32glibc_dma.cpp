/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_dma.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-11
* Last Update        : 2014-11-11
* Description        : USART firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_dma.h"
#include "stm32glibc_rcc.h"

namespace periph_c
{
    /**
      * @brief  Description "Initializes the DMAx peripheral according to the specified
      *                      parameters in the USART_InitStruct ."
      * @param  None
      * @retval None
      */
    void DMA_Channel_Typedef::Config(DMA_Init &cfg)
    {
        DMA_CCR_Typedef *DMA_CCR = ( DMA_CCR_Typedef * ) & (CCR);

        DMA_CCR->DIR   		= cfg.DMA_DIR;
        DMA_CCR->PINC  		= cfg.DMA_PeripheralInc;
        DMA_CCR->MINC  		= cfg.DMA_MemoryInc;
        DMA_CCR->PSIZE 		= cfg.DMA_PeripheralDataSize;
        DMA_CCR->MSIZE 		= cfg.DMA_MemoryDataSize;
        DMA_CCR->CIRC  		= cfg.DMA_Mode;
        DMA_CCR->PL    		= cfg.DMA_Priority;
        DMA_CCR->MEM2MEM	= cfg.DMA_M2M;
    }
}
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
