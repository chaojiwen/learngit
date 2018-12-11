/**
  ******************************************************************************
  * @file              : bsp_lrcv.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 24/09/2014 15:05:36
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used LRCV pins.
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
#ifndef BSP_BSP_LRCV_H
#define BSP_BSP_LRCV_H

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include ".\app_cfg.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
//! @}

/*============================ MACROS ========================================*/
/** @defgroup LRCV_Private_TypesDefinitions
  * @{
  */ 
//!									(Special symbols are not allowed here)
#define LRCVn						1
#define LRCV_COUNT					1
//! @}

//!
//! @{
#define LRCV_PROC_SIZE				32
#define LRCV_PROC_POINTER_SIZE		2
//!
#define LRCV_RECV_RETS				20
//!
#define LRCV_RECV_MAX_TCB 			20
//!                                 (0xFF+1)
#define LRCV_RECV_MAX_BUF 			512
//! @}


/*============================ TYPES =========================================*/
//! @defgroup STM32_EVAL_Exported_Types
//! \name lrcv configuration structure
//! @{
typedef enum 
{
	 LRCV0 = 0

} lrcv_reg_t;
//<! @}

/** @defgroup STM32_EVAL_Exported_Types
  * \name lrcv configuration structure
  * @{
  */
typedef enum
{
    LRCV_PROC_PTN = ((uint16_t)0x0001)
                    , LRCV_PROC_DAT = ((uint16_t)0x0002)
                                      , LRCV_PROC_STR = ((uint16_t)0x0004)
                                              , LRCV_PROC_DIG = ((uint16_t)0x0008)
                                                      , LRCV_PROC_VAL = ((uint16_t)0x0010)
                                                              , LRCV_PROC_SPL = ((uint16_t)0x0020)

} lrcv_proc_type_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */
typedef struct
{
    uint32_t FLAG;
    uint32_t PROC_TYPE;//分割类型
    uint32_t PROC_LENG;//数据长度
    uint32_t PROC_HEAD;//数据头地址
    uint32_t PROC_TAIL;//数据尾地址
    uint32_t PROC_TIME;
    bool     PROC_NEXT;
    void*    PROC_PTRS[LRCV_PROC_POINTER_SIZE];
    char     PROC_PTRN[LRCV_PROC_SIZE];//分割标志
    char     PROC_PFMT[LRCV_PROC_SIZE*2];
}  lrcv_proc_tcb_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */  
typedef struct 
{
	char* dat;
	int len;
	unsigned int to;
	bool go;
} lrcv_serv_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */  
typedef struct 
{
    char *ptn;
	char fmt[LRCV_PROC_SIZE];
	void *ptr[LRCV_PROC_POINTER_SIZE];
	int len;
	int size;
	int mode;
    unsigned int to;
    bool go;
	CPU_FUCT_BYTE pfTimout;
} lrcv_serv_ptn_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */  
typedef struct 
{
	char* str;
	int len;
	unsigned int to;
	bool go;
} lrcv_serv_str_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */  
typedef struct 
{
	char* dig;
	int len;
	unsigned int to;
	bool go;
} lrcv_serv_dig_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */  
typedef struct 
{
	int* val;
	unsigned int to;
	bool go;
} lrcv_serv_val_t;
//! @}

/** @defgroup Ctrl_Private_TypesDefinitions
  * @{
  */  
typedef struct 
{
	char  ptn;
	char* str;
	int len;
	unsigned int to;
	bool go;
} lrcv_serv_spl_t;
//! @}


/*============================ INTERFACE =====================================*/
//! \brief Macro for lrcv interface function prototypes
#define __EXTERN_LRCV_INTERFACE(__N,__DATA)                                    \
    extern bool lrcv##__N##_init(uint8_t serial_port, uint32_t size);          \
    extern bool lrcv##__N##_wait_fetch(lrcv_serv_t* serv);                     \
    extern bool lrcv##__N##_pend_fetch(lrcv_serv_ptn_t* serv);                 \
    extern bool lrcv##__N##_profit_ptn(lrcv_serv_ptn_t* serv);                 \
    extern bool lrcv##__N##_fetch_dat(lrcv_serv_t* serv);                      \
    extern bool lrcv##__N##_fetch_str(lrcv_serv_str_t* serv);                  \
    extern bool lrcv##__N##_fetch_dig(lrcv_serv_dig_t* serv);                  \
    extern bool lrcv##__N##_fetch_val(lrcv_serv_val_t* serv);                  \
    extern bool lrcv##__N##_fetch_spl(lrcv_serv_spl_t* serv);                  \
    extern bool lrcv##__N##_fetch_init(lrcv_serv_ptn_t* serv);                 \
    extern int32_t lrcv##__N##_serial_isr(void);                               \
    extern int32_t lrcv##__N##_transm_isr(void);                               \
    extern int32_t lrcv##__N##_fetch_isr(void);

/*============================ MACROFIED FUNCTIONS ===========================*/
//! \name class: lrcv_t
//! @{
DEF_INTERFACE(lrcv_t)
bool         (*Init)(uint8_t serial_port, uint32_t size); //!< initialize lrcv
bool         (*WaitFetch)(lrcv_serv_t *serv);
bool         (*PendFetch)(lrcv_serv_ptn_t *serv);
bool         (*ProfitPtn)(lrcv_serv_ptn_t *serv);         //!< profit_isr lrcv
bool         (*FetchData)(lrcv_serv_t *serv);
bool         (*FetchStr)(lrcv_serv_str_t *serv);
bool         (*FetchDig)(lrcv_serv_dig_t *serv);
bool         (*FetchVal)(lrcv_serv_val_t *serv);
bool         (*FetchSpl)(lrcv_serv_spl_t *serv);
bool         (*FetchInit)(lrcv_serv_ptn_t *serv);
int32_t      (*SerialISR)(void);            			  //!< serial interrupt lrcv
int32_t      (*TransmISR)(void);            			  //!< dma transmit interrupt lrcv
int32_t      (*FetchISR)(void);
END_DEF_INTERFACE(lrcv_t)
//! @}


/*============================ PROTOTYPES ====================================*/
//! \brief lrcv interface function prototypes
MREPEAT(LRCV_COUNT, __EXTERN_LRCV_INTERFACE, NULL)


/*============================ HAL_LRCV ======================================*/
/*============================ CLASS =========================================*/

/*============================ GLOBAL VARIABLES ==============================*/
//! \brief lrcv object
extern ROOT 
const lrcv_t LRCV[LRCV_COUNT];


/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* BSP_BSP_LRCV_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
