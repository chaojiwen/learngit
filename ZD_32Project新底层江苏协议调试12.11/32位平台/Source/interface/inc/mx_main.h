/**
  ******************************************************************************
  * @file              : mx_main.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 27/09/2014 17:41:57
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2014 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACE_MX_MAIN_H
#define INTERFACE_MX_MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include <stdio.h>
#include "stm32f10x.h"	 
#include "..\..\..\app_cfg.h"
#include "..\..\gsf\inc\gsf_gpio.h"
#include "..\..\gsf\inc\gsf_rccr.h"
#include "..\..\gsf\inc\gsf_jdbg.h"
#include "..\..\gsf\inc\gsf_svic.h"
#include "..\..\gsf\inc\gsf_uart.h"
#include "..\..\gsf\inc\gsf_fmem.h"
#include "..\..\bsp\inc\bsp_bdma.h"
#include "..\..\bsp\inc\bsp_bpio.h"
#include "..\..\bsp\inc\bsp_eint.h"
#include "..\..\bsp\inc\bsp_serial.h"
#include "..\..\bsp\inc\bsp_skey.h"
#include "..\..\bsp\inc\bsp_ai2c.h"
#include "..\..\bsp\inc\bsp_timr.h"
#include "..\..\bsp\inc\bsp_lrcv.h"
	 
/*============================ MACROS ========================================*/
/*============================ TYPES =========================================*/
/*============================ INTERFACE =====================================*/
/*============================ PROTOTYPES ====================================*/
// MCU:STM32F103C(8-B)Tx
int mx_pinout_config(void);

/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_MX_MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

