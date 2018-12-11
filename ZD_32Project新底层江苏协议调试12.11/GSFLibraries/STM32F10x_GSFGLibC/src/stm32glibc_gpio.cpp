/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_gpio.cpp
* Author             : Zhiyix Wang
* Version            : V1.0.0
* Create Date        : 2014-11-10
* Last Update        : 2014-11-10
* Description        : GPIO firmware library (for IAR C++).
********************************************************************************/
#include "stm32glibc_gpio.h"

namespace periph_c
{
    void GPIO_Typedef::Config(const u32 GPIO_Pin, GPIO_Mode_Typedef GPIO_Mode, GPIO_Speed_Typedef GPIO_Speed)
    {
        u16 Pin = GPIO_Pin & GPIO_Pin_All;
        u32 CR_Value = 0;
        u32 CR_Set = 0;
        u32 CR_Mask = 0;
        u8  i;

        /*---------------------------- GPIO Mode Configuration -----------------------*/
        if ((GPIO_Mode & 0x10) != 0x00)
        {
            CR_Set = ( GPIO_Mode & 0x0F ) | GPIO_Speed;
        }
        else
        {
            CR_Set = ( GPIO_Mode & 0x0F );
        }

        /*---------------------------- GPIO CRL Configuration ------------------------*/
        if ((Pin & 0x00FF) != 0x00)
        {
            CR_Value = 0;
            CR_Mask  = 0;

            for (i = 0; i < 8; i++)
            {
                if (Pin & BIT(i))
                {
                    CR_Mask  |= (0x0F << (i * 4));
                    CR_Value |= (CR_Set << (i * 4));
                }
            }
            CRL &= ~CR_Mask;
            CRL |= CR_Value;
        }

        /*---------------------------- GPIO CRH Configuration ------------------------*/
        if ((Pin & 0xFF00) != 0x00)
        {
            CR_Value = 0;
            CR_Mask  = 0;

            for (i = 0; i < 8; i++)
            {
                if (Pin & BIT(i + 8))
                {
                    CR_Mask  |= (0x0F << (i * 4));
                    CR_Value |= (CR_Set << (i * 4));
                }
            }
            CRH &= ~CR_Mask;
            CRH |= CR_Value;
        }

        if (GPIO_Mode == GPIO_MODE_IPD)
        {
            BRR |= Pin;
        }

        if (GPIO_Mode == GPIO_MODE_IPU)
        {
            BSRR |= Pin;
        }
    }
    u8   GPIO_Typedef::ReadInputDataBit(u32 GPIO_Pin)
    {
        u16 Pin = GPIO_Pin & GPIO_Pin_All;
        u8 Result;

        if (IDR & Pin)
        {
            Result = 1;
        }
        else
        {
            Result = 0;
        }

        return (Result);
    }
    u16  GPIO_Typedef::ReadInputData(const u32 GPIO_Addr)
    {
        return (IDR);
    }
    u8   GPIO_Typedef::ReadOutputDataBit(u32 GPIO_Pin)
    {
        u16 Pin = GPIO_Pin & GPIO_Pin_All;
        u8 Result;

        if (ODR & Pin)
        {
            Result = 1;
        }
        else
        {
            Result = 0;
        }

        return (Result);
    }
    u16  GPIO_Typedef::ReadOutputData(const u32 GPIO_Addr)
    {
        return (ODR);
    }
    void GPIO_Typedef::SetBits(u32 GPIO_Pin)
    {
        u16 Pin = GPIO_Pin & GPIO_Pin_All;

        BSRR = Pin;
    }
    void GPIO_Typedef::ResetBits(u32 GPIO_Pin)
    {
        u16 Pin = GPIO_Pin & GPIO_Pin_All;

        BRR = Pin;
    }
    void GPIO_Typedef::WriteBit(u32 GPIO_Pin, u8 BitVal)
    {
        u16 Pin = GPIO_Pin & GPIO_Pin_All;

        if (BitVal != 0)
        {
            BSRR = Pin;
        }
        else
        {
            BRR = Pin;
        }
    }
    void GPIO_Typedef::Write(u16 PortVal, const u32 GPIO_Addr)
    {
        ODR = PortVal;
    }
    void GPIO_Typedef::ToggleBits(u32 GPIO_Pin)
    {
        u16 Pin = GPIO_Pin & GPIO_Pin_All;

        ODR ^= Pin;
    }
}


/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
