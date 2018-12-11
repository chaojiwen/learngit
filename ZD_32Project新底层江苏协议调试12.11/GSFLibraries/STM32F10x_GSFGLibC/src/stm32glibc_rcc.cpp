/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_rcc.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-04
* Last Update        : 2014-11-04
* Description        : RCC firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_rcc.h"

namespace periph_c
{
    /**
      * @brief  Description "Configures the External High Speed oscillator (HSE)."
      * @brief  Description "Configures the External Low Speed oscillator  (LSE)."
      * @param  RCC_Param: specifies the new state of the LSE.
      * @retval None
      */
    void RCC_Typedef::Config(u32 RCC_Source, u32 RCC_Param)
    {
        RCC_CR_Typedef   *RCC_CR   = ( RCC_CR_Typedef *   ) & (CR);
        RCC_BDCR_Typedef *RCC_BDCR = ( RCC_BDCR_Typedef * ) & (BDCR);
        /*---------------------------- GPIO Mode Configuration -----------------------*/
        if (RCC_Source_LSE == RCC_Source)
        {
            /* Reset LSEON and LSEBYP bits before configuring the LSE ----------------*/
            RCC_BDCR->LSEON = 0;
            RCC_BDCR->LSEBYP = 0;

            switch(RCC_Param)
            {
            case RCC_EXTOSC_ON:
                /* Set LSEON bit */
                RCC_BDCR->LSEON = 1;
                break;

            case RCC_EXTOSC_Bypass:
                /* Set LSEBYP and LSEON bits */
                RCC_BDCR->LSEON = 1;
                RCC_BDCR->LSEBYP = 1;
                break;

            default:
                break;
            }
        }
        else if (RCC_Source_HSE == RCC_Source)
        {
            /* Reset HSEON and HSEBYP bits before configuring the HSE ------------------*/
            RCC_CR->HSEON = 0;
            RCC_CR->HSEBYP = 0;

            switch(RCC_Param)
            {
            case RCC_EXTOSC_ON:
                /* Set HSEON bit */
                RCC_CR->HSEON = 1;
                break;

            case RCC_EXTOSC_Bypass:
                /* Set HSEBYP and HSEON bits */
                RCC_CR->HSEON = 1;
                RCC_CR->HSEBYP = 1;
                break;

            default:
                break;
            }
        }
    }

    /**
      * @brief  Description "Configures the AHB clock (HCLK)."
      * @brief  Description "Configures the Low Speed APB clock (PCLK1)."
      * @brief  Description "Configures the Low Speed APB clock (PCLK2)."
      * @param  RCC_Prescaler: defines the RCCx clock divider.
      * @retval None
      */
    void RCC_Typedef::PreConfig(u32 RCC_Source, u32 RCC_Prescaler)
    {
        RCC_CFGR_Typedef *RCC_CFGR = ( RCC_CFGR_Typedef * ) & (CFGR);
        RCC_BDCR_Typedef *RCC_BDCR = ( RCC_BDCR_Typedef * ) & (BDCR);
        /*---------------------------- GPIO Mode Configuration -----------------------*/
        if (RCC_Source_SYSCLK == RCC_Source)
        {
            /* Clear SW[1:0] bits */
            RCC_CFGR->SW = 0;
            /* Set SW[1:0] bits according to RCC_SYSCLKSource value */
            RCC_CFGR->SW = RCC_Prescaler;
        }
        else if (RCC_Source_HCLK == RCC_Source)
        {
            /* Clear HPRE[3:0] bits */
            RCC_CFGR->HPRE = 0;
            /* Set HPRE[3:0] bits according to RCC_SYSCLK value */
            RCC_CFGR->HPRE = RCC_Prescaler;
        }
        else if (RCC_Source_APB1 == RCC_Source)
        {
            /* Clear PPRE1[2:0] bits */
            RCC_CFGR->HPRE1 = 0;
            /* Set PPRE1[2:0] bits according to RCC_HCLK value */
            RCC_CFGR->HPRE1 = RCC_Prescaler;
        }
        else if (RCC_Source_APB2 == RCC_Source)
        {
            /* Clear PPRE2[2:0] bits */
            RCC_CFGR->HPRE2 = 0;
            /* Set PPRE2[2:0] bits according to RCC_HCLK value */
            RCC_CFGR->HPRE2 = RCC_Prescaler;
        }
        else if (RCC_Source_ADCCLK == RCC_Source)
        {
            /* Clear ADCPRE[1:0] bits */
            RCC_CFGR->ADCPRE = 0;
            /* Set ADCPRE[1:0] bits according to RCC_PCLK2 value */
            RCC_CFGR->ADCPRE = RCC_Prescaler;
        }
        else if (RCC_Source_RTCCLK == RCC_Source)
        {
            /* Clear RTCSEL[1:0] bits */
            RCC_BDCR->RTCSEL = 0;
            /* Select the RTC clock source */
            RCC_BDCR->RTCSEL = RCC_Prescaler;
        }
    }

    /**
      * @brief  Description "Enables or disables the Internal High Speed oscillator (HSI)."
      * @note   HSI can not be stopped if it is used directly or through the PLL as system clock.
      * @param  NewState: new state of the HSI. This parameter can be: ENABLE or DISABLE.
      * @retval None
      */
    void RCC_Typedef::Cmd(u32 RCC_Source, u8 NewState)
    {
        RCC_CR_Typedef   *RCC_CR   = ( RCC_CR_Typedef *   ) & (CR);
        RCC_BDCR_Typedef *RCC_BDCR = ( RCC_BDCR_Typedef * ) & (BDCR);
        RCC_CSR_Typedef  *RCC_CSR  = ( RCC_CSR_Typedef *  ) & (CSR);
        /*---------------------------- GPIO Mode Configuration -----------------------*/
        if (RCC_Source_HSI == RCC_Source)
        {
            RCC_CR->HSION = NewState;
        }
        else if (RCC_Source_LSE == RCC_Source)
        {
            RCC_BDCR->LSEON = NewState;
        }
        else if (RCC_Source_LSI == RCC_Source)
        {
            RCC_CSR->LSION = NewState;
        }
        else if (RCC_Source_HSE == RCC_Source)
        {
            RCC_CR->HSEON = NewState;
        }
        else if (RCC_Source_PLL == RCC_Source)
        {
            RCC_CR->PLLON = NewState;
        }
        else if (RCC_Source_RTCCLK == RCC_Source)
        {
            RCC_BDCR->RTCEN = NewState;
        }
    }

    /**
      * @brief  Description "Configures the PLL clock source and multiplication factor."
      * @note   This function must be used only when the PLL is disabled.
      * @param  RCC_PLLSource: specifies the PLL entry clock source.
      * @param  RCC_PLLMul: specifies the PLL multiplication factor.
      * @retval None
      */
    void RCC_Typedef::PLLConfig(u32 RCC_PLLSource, u32 RCC_PLLMul)
    {
        RCC_CFGR_Typedef *RCC_CFGR = ( RCC_CFGR_Typedef * ) & (CFGR);

        RCC_CFGR->PLLSRC = 0;
        RCC_CFGR->PLLXTPRE = 0;
        RCC_CFGR->PLLMUL = 0;

        RCC_CFGR->PLLSRC = RCC_PLLSource;
        RCC_CFGR->PLLMUL = RCC_PLLMul;
    }

    /**
      * @brief  Description "Selects the clock source to output on MCO pin."
      * @param  RCC_MCO: specifies the clock source to output.
      * @retval None
      */
    void RCC_Typedef::MCOConfig(u32 RCC_MCO)
    {
        RCC_CFGR_Typedef *RCC_CFGR = ( RCC_CFGR_Typedef * ) & (CFGR);

        /* Perform Byte access to MCO bits to select the MCO source */
        RCC_CFGR->MCO = RCC_MCO;
    }

    /**
      * @brief  Description "Waits for HSE start-up"
      * @param  None
      * @retval None
      */
    ErrorStatus RCC_Typedef::WaitForStartUp(u32 RCC_Source)
    {
        RCC_CR_Typedef   *RCC_CR   = ( RCC_CR_Typedef *   ) & (CR);

        u32 StartUpCounter = 0;
        u32 HSEStatus = RESET;
        ErrorStatus status = ERROR;

        if (RCC_Source_HSE == RCC_Source)
        {
            /* Wait till HSE is ready and if Time out is reached exit */
            do
            {
                HSEStatus = RCC_CR->HSERDY;
                StartUpCounter++;
            }
            while((StartUpCounter != HSE_STARTUP_TIMEOUT) && (HSEStatus == RESET));

            if (RCC_CR->HSERDY != RESET)
            {
                status = SUCCESS;
            }
            else
            {
                status = ERROR;
            }
        }
        return (status);
    }

    /**
      * @brief  Description "Enables or disables the AHB peripheral clock."
      * @param  RCC_AHBPeriph: specifies the AHB peripheral to gates its clock.
      * @retval None
      */
    void RCC_Typedef::AHBPeriphClockCmd(u32 RCC_Periph_Source, u8 NewState)
    {
        if (NewState != DISABLE)
        {
            AHBENR |= RCC_Periph_Source;
        }
        else
        {
            AHBENR &= ~RCC_Periph_Source;
        }
    }

    /**
      * @brief  Enables or disables the High Speed APB (APB2) peripheral clock.
      * @param  RCC_APB2Periph: specifies the APB2 peripheral to gates its clock.
      * @retval None
      */
    void RCC_Typedef::APB2PeriphClockCmd(u32 RCC_Periph_Source, u8 NewState)
    {
        if (NewState != DISABLE)
        {
            APB2ENR |= RCC_Periph_Source;
        }
        else
        {
            APB2ENR &= ~RCC_Periph_Source;
        }
    }

    /**
      * @brief  Enables or disables the Low Speed APB (APB1) peripheral clock.
      * @param  RCC_APB1Periph: specifies the APB1 peripheral to gates its clock.
      * @retval None
      */
    void RCC_Typedef::APB1PeriphClockCmd(u32 RCC_Periph_Source, u8 NewState)
    {
        if (NewState != DISABLE)
        {
            APB1ENR |= RCC_Periph_Source;
        }
        else
        {
            APB1ENR &= ~RCC_Periph_Source;
        }
    }

    /**
      * @brief  Forces or releases High Speed APB (APB2) peripheral reset.
      * @param  RCC_APB2Periph: specifies the APB2 peripheral to reset.
      * @retval None
      */
    void RCC_Typedef::APB2PeriphResetCmd(u32 RCC_Periph_Source, u8 NewState)
    {
        if (NewState != DISABLE)
        {
            APB2RSTR |= RCC_Periph_Source;
        }
        else
        {
            APB2RSTR &= ~RCC_Periph_Source;
        }
    }

    /**
      * @brief  Forces or releases Low Speed APB (APB1) peripheral reset.
      * @param  RCC_APB1Periph: specifies the APB1 peripheral to gates its clock.
      * @retval None
      */
    void RCC_Typedef::APB1PeriphResetCmd(u32 RCC_Periph_Source, u8 NewState)
    {
        if (NewState != DISABLE)
        {
            APB1RSTR |= RCC_Periph_Source;
        }
        else
        {
            APB1RSTR &= ~RCC_Periph_Source;
        }
    }

    /**
      * @brief  Description "Returns the frequencies of different on chip clocks."
      * @param  None
      * @note   The result of this function could be not correct when using
      *         fractional value for HSE crystal.
      * @retval None
      */
    void RCC_Typedef::GetClocksFreq(RCC_ClocksTypeDef &RCC_Clocks)
    {
        RCC_CFGR_Typedef *RCC_CFGR = ( RCC_CFGR_Typedef * ) & (CFGR);

        uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

        const u8 APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
        const u8 ADCPrescTable[4] = {2, 4, 6, 8};

#ifdef  STM32F10X_CL
        uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
        uint32_t prediv1factor = 0;
#endif

        /* Get SYSCLK source -------------------------------------------------------*/
        tmp = RCC_CFGR->SWS;

        switch (tmp)
        {
        case 0x00:  /* HSI used as system clock */
            RCC_Clocks.SYSCLK_Frequency = HSI_VALUE;
            break;
        case 0x01:  /* HSE used as system clock */
            RCC_Clocks.SYSCLK_Frequency = HSE_VALUE;
            break;
        case 0x02:  /* PLL used as system clock */

            /* Get PLL clock source and multiplication factor ----------------------*/
            pllmull = RCC_CFGR->PLLMUL;
            pllsource = RCC_CFGR->PLLSRC;

#ifndef STM32F10X_CL
            pllmull = ( pllmull) + 2;

            if (pllsource == 0x00)
            {
                /* HSI oscillator clock divided by 2 selected as PLL clock entry */
                RCC_Clocks.SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
            }
            else
            {
#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
                prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
                /* HSE oscillator clock selected as PREDIV1 clock entry */
                RCC_Clocks.SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * pllmull;
#else
                /* HSE selected as PLL clock entry */
                if ((RCC_CFGR->PLLXTPRE) != (uint32_t)RESET)
                {
                    /* HSE oscillator clock divided by 2 */
                    RCC_Clocks.SYSCLK_Frequency = (HSE_VALUE >> 1) * pllmull;
                }
                else
                {
                    RCC_Clocks.SYSCLK_Frequency = HSE_VALUE * pllmull;
                }
#endif
            }
#else
            if (pllmull != 0x0D)
            {
                pllmull += 2;
            }
            else
            {
                /* PLL multiplication factor = PLL input clock * 6.5 */
                pllmull = 13 / 2;
            }

            if (pllsource == 0x00)
            {
                /* HSI oscillator clock divided by 2 selected as PLL clock entry */
                RCC_Clocks.SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
            }
            else
            {
                /* PREDIV1 selected as PLL clock entry */

                /* Get PREDIV1 clock source and division factor */
                prediv1source = RCC->CFGR2 & CFGR2_PREDIV1SRC;
                prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;

                if (prediv1source == 0)
                {
                    /* HSE oscillator clock selected as PREDIV1 clock entry */
                    RCC_Clocks.SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * pllmull;
                }
                else
                {
                    /* PLL2 clock selected as PREDIV1 clock entry */

                    /* Get PREDIV2 division factor and PLL2 multiplication factor */
                    prediv2factor = ((RCC->CFGR2 & CFGR2_PREDIV2) >> 4) + 1;
                    pll2mull = ((RCC->CFGR2 & CFGR2_PLL2MUL) >> 8 ) + 2;
                    RCC_Clocks.SYSCLK_Frequency = (((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * pllmull;
                }
            }
#endif /* STM32F10X_CL */
            break;

        default:
            RCC_Clocks.SYSCLK_Frequency = HSI_VALUE;
            break;
        }

        /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
        /* Get HCLK prescaler */
        tmp = RCC_CFGR->HPRE;
        presc = APBAHBPrescTable[tmp];
        /* HCLK clock frequency */
        RCC_Clocks.HCLK_Frequency = RCC_Clocks.SYSCLK_Frequency >> presc;
        /* Get PCLK1 prescaler */
        tmp = RCC_CFGR->HPRE1;
        presc = APBAHBPrescTable[tmp];
        /* PCLK1 clock frequency */
        RCC_Clocks.PCLK1_Frequency = RCC_Clocks.HCLK_Frequency >> presc;
        /* Get PCLK2 prescaler */
        tmp = RCC_CFGR->HPRE2;
        presc = APBAHBPrescTable[tmp];
        /* PCLK2 clock frequency */
        RCC_Clocks.PCLK2_Frequency = RCC_Clocks.HCLK_Frequency >> presc;
        /* Get ADCCLK prescaler */
        tmp = RCC_CFGR->ADCPRE;
        presc = ADCPrescTable[tmp];
        /* ADCCLK clock frequency */
        RCC_Clocks.ADCCLK_Frequency = RCC_Clocks.PCLK2_Frequency / presc;
    }
}
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
