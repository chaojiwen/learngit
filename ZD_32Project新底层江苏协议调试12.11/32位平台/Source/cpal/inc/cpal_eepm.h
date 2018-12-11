/**
  ******************************************************************************
  * @file              : cpal_eepm.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:59:01
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used EEPM pins.
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
#ifndef __CPAL_EEPM_H
#define __CPAL_EEPM_H

#ifdef __cplusplus
 extern "C" {
#endif
 
/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include ".\app_cfg.h"
#include ".\cpal_i2ci.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//! @}

/*==========================================================================================================
                                             User NOTES
============================================================================================================

---------------------------------
   How To use the EEPROM Driver:
---------------------------------
----- All EEPROM operations are controlled and monitored through a single
      structure. This structure holds all necessary parameters to manage EEPROM
      communication (pointer to CPAL I2C device structure, EEPROM address, 
      EEPROM page size, EEPROM memory addressing mode, EEPROM state).
         
----- User should follow these steps to use this driver correctly :

      -1-  STRUCTURE INITIALIZATION 
      Start by initializing the structure holding EEPROM Information. To perform 
      this action, the global variable sEEx_DevStructure declared in EEPROM driver 
      as sEE_InitTypeDef (sEE1_DevStructure for EEPROM connected with I2C1, 
      sEE2_DevStructure for EEPROM connected with I2C1 ...) must be used.

      sEE_InitTypeDef structure contains five parameters:

           *- CPAL_InitTypeDef* sEE_CPALStructure : Pointer on a CPAL Device structure 
                                                    relative to the device instantiated 
                                                    to communicate with EEPROM.

           *- uint32_t sEEAddress : Contains the EEPROM device Address.
  
           *- uint32_t sEEPageSize : Contains the page size of EEPROM Memory.

           *- uint8_t sEEMemoryAddrMode : Bit-field value specifying Memory Addressing Mode.

           *- __IO sEE_StateTypeDef sEEState : Holds the current State of the EEPROM device. 

       To configure this structure, user must initialize only three parameters 
       (sEEAddress, sEEPageSize, sEEMemoryAddrMode).
       
       Example:
         sEE1_DevStructure.sEEAddress = 0xA0; // set EEPROM address to 0xA0 
         sEE1_DevStructure.sEEPageSize = 32;  // set page size to 32
         sEE1_DevStructure.sEEMemoryAddrMode = sEE_OPT_16BIT_REG; // enable 16Bit memory addressing mode
                           
      -2- DEVICE CONFIGURATION 
      Call the function sEE_StructInit() to initialize I2Cx CPAL device structure 
      relative to EEPROM than call sEE_Init() to configure the selected device
      with the selected configuration.
      
      -3- READ / WRITE OPERATIONS 
      Call the function sEE_WriteBuffer() or sEE_ReadBuffer() to perform transfer operations. 
      These functions start data transfer and exit. sEE_WriteHandler() and sEE_ReadHandler()
      handle the remainder of the communication. sEE_WriteHandler() must be called in 
      CPAL_I2C_TXTC_UserCallback() and sEE_ReadHandler() must be called in CPAL_I2C_RXTC_UserCallback().
      These two callbacks are implemented in "cpal_usercallback.c" file.      
           
      Example of how to implement sEE_WriteHandler() in CPAL_I2C_TXTC_UserCallback():
      
            1** Comment "#define CPAL_I2C_TXTC_UserCallback   (void)" in cpal_conf.h.
            2** Implement CPAL_I2C_TXTC_UserCallback in "cpal_usercallback.c" file.
               
               void CPAL_I2C_TXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
               {
                 sEE_WriteHandler(pDevInitStruct->CPAL_Dev);
               }
      
      User should monitor transfer by testing the value of sEEState parameter of
      sEEx_DevStructure. When transfer is ongoing sEEState is equal to sEE_STATE_WRITING
      or sEE_STATE_READING. After transfer complete this parameter is set to sEE_STATE_IDLE.
      
      Example of how to wait until EEPROM communication finishes:
            
            while(sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE)
              {
                //Application may perform other tasks while transfer operation is ongoing 
              }
      
      -4- DEVICE DEINITIALIZATION 
      When transfer operations are finished, you may call sEE_DeInit() to disable I2Cx device 
      and related resources (GPIO, DMA , IT and NVIC) relative to used EEPROM. 
      
*********END OF User Notes**********************************************************************************/

/*============================ MACROS ========================================*/
/** @defgroup EEPM_Private_TypesDefinitions
* @{
*/
//! (Special symbols are not allowed here)
#define EEPMn						3
#define EEPM_COUNT					3
//! @}

//! @{
/* Select interrupt programming model : By default DMA programming model is selected.
 To select interrupt programming model uncomment this define */
//#define sEE_IT
//! @}

//! @{
/*========= sEE_Memory_Addressing_Mode_Defines =========*/
/* sEE Memory_Addressing_Mode defines can be affected to sEEMemoryAddrMode which is 
   a bit-field argument so any combination of these parameters can be selected. 
   If one option is not selected then the relative feature is disabled.*/
#define sEE_OPT_NO_MEM_ADDR            ((uint8_t)0x01)  /*!<Enable No Memory addressing mode for read operation : only EEPROM 
                                                            device address sent. No Register/Physical address to be sent after 
                                                            addressing phase */  
#define sEE_OPT_16BIT_REG              ((uint8_t)0x02)  /*!<Enable 16-Bit Register/Physical addressing mode (two bytes, 
                                                            MSB first). This option is supported only when sEE_OPT_NO_MEM_ADDR 
                                                            option is not set */ 
//! @}

/*============================ TYPES =========================================*/
//! @{
/*========= sEE_State_Enum =========*/
/* sEE global State enumeration contains the current state of EEPROM. 
   Before starting each operation this state is tested. After each operation 
   sEE_State is updated with the new value resulting from the relative operation.*/

 typedef enum
{ 
  sEE_STATE_IDLE        = 0x01,         /*!<This state indicates that the EEPROM device is in idle state */ 

  sEE_STATE_WRITING     = 0x02,         /*!<This state indicates that write operation is ongoing */

  sEE_STATE_READING     = 0x03,         /*!<This state indicates that read operation is ongoing */
  
  sEE_STATE_ERROR       = 0x04,         /*!<This state indicates that an error is occurred during 
                                            last operation */
  
}sEE_StateTypeDef;
//! @}

//! @{
/* sEE Device structure definition */
typedef struct  
{
  CPAL_InitTypeDef* sEE_CPALStructure;  /*!< Pointer on a CPAL Device structure relative to the device 
                                             instantiated to communicate with EEPROM */
  
  uint16_t sEEPageSize;                 /*!< Contains the page size of EEPROM Memory*/ 
  
  uint8_t sEEAddress;                   /*!< Contains the EEPROM device Address */
  
  
  uint8_t sEEMemoryAddrMode;            /*!< Bit-field value specifying Memory Addressing Mode. Can be 
                                             any combination of following values: 
                                             sEE_Memory_Addressing_Mode_Defines */ 
  
  __IO sEE_StateTypeDef sEEState;       /*!< Holds the current State of the EEPROM device. The state 
                                             parameter can be one of the following values: sEE_State_Enum  */
  
} sEE_InitTypeDef;   
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for eepm interface function prototypes
#define __EXTERN_EEPM_INTERFACE(__N,__DATA)                                    \
    extern bool eepm##__N##_init(void);                                        \
    extern bool eepm##__N##_deinit(void);                                      \
    extern bool eepm##__N##_structinit(sEE_InitTypeDef *psEE_DevStruct);       \
    extern bool eepm##__N##_swreset(void);                                     \
    extern int32_t eepm##__N##_getstate(void);                                 \
	extern int32_t eepm##__N##_setstate(int32_t state);                        \
	extern int32_t eepm##__N##_settimeout(int32_t tim);                        \
    extern int32_t eepm##__N##_readbuffer(uint8_t* pBuffer,                    \
									  uint16_t ReadAddr,                       \
	                                  uint32_t NumByteToRead);                 \
    extern int32_t eepm##__N##_writebuffer(uint8_t* pBuffer,                   \
	                                  uint16_t WriteAddr,                      \
	                                  uint32_t NumByteToWrite);                \
    extern int32_t eepm##__N##_writepage(uint8_t* pBuffer,                     \
	                                  uint16_t WriteAddr,                      \
	                                  uint32_t NumByteToWrite);                \
    extern int32_t eepm##__N##_readhandler(CPAL_DevTypeDef Device);            \
    extern int32_t eepm##__N##_writehandler(CPAL_DevTypeDef Device);           \
    extern int32_t eepm##__N##_waitstandby(CPAL_DevTypeDef Device);    
	
	
/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: eepm_t
//! @{
DEF_INTERFACE(eepm_t)
    bool        (*Init)(void);                      //!< initialize eepm
    bool        (*DeInit)(void);                    //!< initialize eepm
    bool        (*StructInit)(sEE_InitTypeDef *psEE_DevStruct);  
	                                                //!< initialize eepm
	bool        (*SWReset)(void);                   //!<
    int32_t     (*GetState)(void);                  //!< 
    int32_t     (*SetState)(int32_t state);         //!< 
    int32_t     (*SetTimeout)(int32_t tim);         //!< 
    int32_t     (*ReadBuffer)(uint8_t* pBuffer, 
	                     uint16_t ReadAddr,      
	                     uint32_t NumByteToRead);   //!< read eepm 
    int32_t     (*WriteBuffer)(uint8_t* pBuffer, 
	                      uint16_t WriteAddr,      
	                      uint32_t NumByteToWrite); //!< write eepm 
    int32_t     (*WritePage)(uint8_t* pBuffer, 
	                      uint16_t WriteAddr,      
	                      uint32_t NumByteToWrite); //!< write page 
    int32_t     (*ReadHandler)(
	                       CPAL_DevTypeDef Device); //!< 
    int32_t     (*WriteHandler)(
	                       CPAL_DevTypeDef Device); //!< 
    int32_t     (*WaitStandby)(
	                       CPAL_DevTypeDef Device); //!< 
END_DEF_INTERFACE(eepm_t)
//! @}E(eepm_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief eepm interface function prototypes
MREPEAT(EEPM_COUNT, __EXTERN_EEPM_INTERFACE, NULL)


/*============================ HAL_EEPM ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief eepm object
extern ROOT 
const eepm_t EEPM[EEPM_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* __CPAL_EEPM_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
