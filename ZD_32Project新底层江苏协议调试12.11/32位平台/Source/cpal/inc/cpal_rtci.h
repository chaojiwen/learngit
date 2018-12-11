/**
  ******************************************************************************
  * @file              : cpal_rtci.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:59:02
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used BI2C pins.
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
#ifndef CPAL_CPAL_RTCI_H
#define CPAL_CPAL_RTCI_H

#ifdef __cplusplus
 extern "C" {
#endif


/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include ".\app_cfg.h"
#include ".\cpal_main.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//! @}



/*============================ MACROS ========================================*/
//! @{
//! \brief The reference for current object
#define this							(*ptThis)
//! @}

/* USER CODE BEGIN 1 */
/** @defgroup RTCI_Private_TypesDefinitions
  * @{
  */
//!										(Special symbols are not allowed here)
#define RTCIn							1
#define RTCI_COUNT						1
#define RTCI_BUFFER_SIZE				(16)
//! @}
/* USER CODE END 1 */

/*============================ TYPES =========================================*/
/** @defgroup STM32_EVAL_Exported_Types  
  * \name rtci configuration structure
  * @{  
  */
typedef enum{
    RTCI0 = 0,
} rtci_reg_t;
//! @}

/*============================ INTERFACE =====================================*/
//! \brief Macro for rtci interface function prototypes
#define __EXTERN_RTCI_INTERFACE(__N,__DATA)                                    \
    extern bool rtci##__N##_init(void);                                        \
	extern bool rtci##__N##_deinit(void);                                      \
	extern int32_t rtci##__N##_read(uint8_t* pBuffer,                          \
									uint16_t ReadAddr,                         \
									uint32_t NumByteToRead);                   \
	extern int32_t rtci##__N##_write(uint8_t* pBuffer,                         \
									 uint16_t WriteAddr,                       \
									 uint32_t NumByteToWrite);                 \
	extern int32_t rtci##__N##_writemore(uint8_t* pBuffer,                     \
									 uint16_t WriteAddr,                       \
									 uint32_t NumByteToWrite);                 \
	extern int32_t rtci##__N##_writebuff(uint8_t* pBuffer,                     \
									 uint16_t WriteAddr,                       \
									 uint32_t NumByteToWrite);                 \
	extern int32_t rtci##__N##_waitstandby(void);                              \
	extern int32_t rtci##__N##_writemorecompl(void);


/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: rtci_t
//! @{
DEF_INTERFACE(rtci_t)
	bool         (*Init)(void);                     //!< initialize rtci
	bool         (*DeInit)(void);                   //!< initialize rtci
	int32_t      (*Read)(uint8_t *pBuffer,
						 uint16_t ReadAddr,
						 uint32_t NumByteToRead);   //!< read rtci
	int32_t      (*Write)(uint8_t *pBuffer,
						  uint16_t WriteAddr,
						  uint32_t NumByteToWrite); //!< write rtci
	int32_t      (*WriteMore)(uint8_t *pBuffer,
						  uint16_t WriteAddr,
						  uint32_t NumByteToWrite); //!< write rtci
	int32_t      (*WriteBuffer)(uint8_t *pBuffer,
						  uint16_t WriteAddr,
						  uint32_t NumByteToWrite); //!< write rtci
	int32_t      (*WaitEepromStandbyState)(void);
	int32_t      (*WriteMoreCompl)(void);
END_DEF_INTERFACE(rtci_t)
//! @}

/*============================ PROTOTYPES ====================================*/
//! \brief rtci interface function prototypes
MREPEAT(RTCI_COUNT, __EXTERN_RTCI_INTERFACE, NULL)

/*============================ HAL_RTCI ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief rtci object (interface function)
extern ROOT
const rtci_t RTCI[RTCI_COUNT];

/*============================ EXTERN FUNCTIONS ==============================*/

//! end of file

#ifdef __cplusplus
}
#endif

#endif /* CPAL_CPAL_RTCI_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
