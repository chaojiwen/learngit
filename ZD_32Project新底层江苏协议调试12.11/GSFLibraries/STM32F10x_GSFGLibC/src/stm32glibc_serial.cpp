/******************** (C)Copyright e-midas.cn **********************************
* File Name          : stm32fwlib_serial.cpp
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#include "stm32fwlib_serial.h"

namespace periph
{
    namespace serial_declaration
    {
        CSerial<CSerial_Info1>	Serial1;
        CSerial<CSerial_Info2>	Serial2;
        CSerial<CSerial_Info3>	Serial3;
        CSerial<CSerial_Info4>	Serial4;
        CSerial<CSerial_Info5>	Serial5;
    }
}

/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
