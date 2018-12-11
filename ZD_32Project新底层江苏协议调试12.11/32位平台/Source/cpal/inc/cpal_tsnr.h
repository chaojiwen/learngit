/**
  ******************************************************************************
  * @file              : cpal_tsnr.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:59:01
  * @brief             : h file
  * @description       : This file contains all the functions prototypes for the 
  *                      stm32_eval_i2c_tsensor_cpal.c firmware driver.
  ******************************************************************************
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
#ifndef CPAL_CPAL_TSNR_H
#define CPAL_CPAL_TSNR_H

#ifdef __cplusplus
 extern "C" {
#endif


/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include ".\app_cfg.h"
#include ".\cpal_i2c.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//! @}



/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  TSENSOR Status  
  */ 
typedef enum
{
  LM75_OK = 0,
  LM75_FAIL
}LM75_Status_TypDef;

/* Exported constants --------------------------------------------------------*/
    
/* CPAL Structure configuration : Select I2C device (uncomment relative define) */

//#define LM75_DevStructure                I2C1_DevStructure   
#define LM75_DevStructure                I2C2_DevStructure  
//#define LM75_DevStructure                I2C3_DevStructure 

   
/* Select clock Speed */
/* To use the I2C at 400 KHz (in fast mode), the PCLK1 frequency (I2C peripheral
   input clock) must be a multiple of 10 MHz */

#define I2C_SPEED                        300000

/* Select interrupt programming model : By default DMA programming model is selected.
 To select interrupt programming model uncomment this define */
//#define LM75_IT

/* Maximum Timeout values for waiting until device is ready for communication.*/
   
#define LM75_TIMEOUT        ((uint32_t)0x3FFFF)

/**
  * @brief  Internal register Memory
  */
#define LM75_REG_TEMP       0x00  /*!< Temperature Register of LM75 */
#define LM75_REG_CONF       0x01  /*!< Configuration Register of LM75 */
#define LM75_REG_THYS       0x02  /*!< Temperature Register of LM75 */
#define LM75_REG_TOS        0x03  /*!< Over-temp Shutdown threshold Register of LM75 */
#define LM75_ADDR           0x90   /*!< LM75 address */
   

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

void LM75_DeInit(void);
void LM75_Config(void);
ErrorStatus LM75_GetStatus(void);
uint16_t LM75_ReadTemp(void);
uint16_t LM75_ReadReg(uint8_t RegName);
uint8_t LM75_WriteReg(uint8_t RegName, uint16_t RegValue);
uint8_t LM75_ReadConfReg(void);
uint8_t LM75_WriteConfReg(uint8_t RegValue);
uint8_t LM75_ShutDown(FunctionalState NewState);
#ifdef __cplusplus
}
#endif

#endif /* CPAL_CPAL_TSNR_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
