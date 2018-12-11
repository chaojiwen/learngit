/**
  ******************************************************************************
  * @file              : cpu_h.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 13/09/2014 11:58:55
  * @brief             : h file
  * @description       : This file provides code for the configuration
  *                      of all used CPU_ASM pins.
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
#ifndef __ASM_CPU_H_H
#define __ASM_CPU_H_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
/*============================ INCLUDES ======================================*/
#define pNULL ((void*)0)
/*============================ MACROS ========================================*/
//! \name Configure Standard Data Types : Function
//! @{   
/* Defines   CPU status register size (see Note #3).    */
//typedef uint32_t      CORE_CPU_SAFE; 
//!
//typedef          bool (*CPU_FUCT_VOID)(void);
//!
//typedef          void (*CPU_FUCT_PTR )(void *);
//!
//typedef CORE_CPU_SAFE (*CPU_FUCT_BYTE)(CORE_CPU_SAFE);
//! @}
	 
/*
********************************************************************************
*                          CRITICAL SECTION CONFIGURATION
*
* Note(s) : (1) Configure CORE_CFG_CRITICAL_METHOD with CPU's/compiler's 
*               critical section method :
*
*               Enter/Exit critical sections by ...
*
*               CORE_CRITICAL_METHOD_INT_DIS_EN       Disable/Enable interrupts;
*               CORE_CRITICAL_METHOD_STATUS_STK       Push/Pop
*                                                   interrupt status onto stack;
*               CORE_CRITICAL_METHOD_STATUS_LOCAL     Save/Restore
*                                            interrupt status to local variable;
*
*               (a) CORE_CRITICAL_METHOD_INT_DIS_EN  is NOT a preferred method 
*                   since it does NOT support multiple levels of interrupts.  
*                   However, with some CPUs/compilers, this is the only
*                   available method.
*
*               (b) CORE_CRITICAL_METHOD_STATUS_STK  is one preferred method 
*                   since it DOES support multiple levels of interrupts.  
*                   However, this method assumes that the compiler allows in-line
*                   assembly AND will correctly modify the local stack pointer 
*                   when interrupt status is pushed/popped onto the stack.
*
*               (c) CORE_CRITICAL_METHOD_STATUS_LOCAL  is one preferred method 
*                   since it DOES support multiple levels of interrupts. 
*                   However, this method assumes that the compiler provides C-level
*                   &/or assembly-level functionality for the following :
*
*                     ENTER CRITICAL SECTION :
*                       (a) Save    interrupt status into a local variable
*                       (b) Disable interrupts
*
*                     EXIT  CRITICAL SECTION :
*                       (c) Restore interrupt status from a local variable
*
*           (2) Critical section macro's most likely require inline assembly.  
*               If the compiler does NOT allow inline assembly in C source files, 
*               critical section macro's MUST call an assembly subroutine defined in 
*               a 'cpu_a.asm' file located in the following software directory :
*
*               \<CPU-Compiler Directory>\<cpu>\<compiler>\
*
*               where
*               <CPU-Compiler Directory>    directory path for common   CPU-compiler software
*               <cpu>                       directory name for specific CPU
*               <compiler>                  directory name for specific compiler
*
*           (3) To save/restore interrupt status, a local variable 'cpu_sr' of 
*               type 'CPU_SR' MAY need to be declared 
*               (e.g. if 'CPU_CRITICAL_METHOD_STATUS_LOCAL' method is configured).  
*               Configure 'CPU_SR' data type with the appropriate-sized CPU data type 
*               large enough to completely store the CPU's/compiler's status word.
********************************************************************************
*/
//! \brief 'CPU_DEF.h' CPU CRITICAL SECTION METHODS
//! @{
#ifndef CORE_CRITICAL_METHOD
#define CORE_CRITICAL_METHOD
typedef enum 
{
	 CORE_CRITICAL_METHOD_NONE          =	0 	
	,CORE_CRITICAL_METHOD_INT_DIS_EN  	=	1	/* DIS/EN       ints.                   */
	,CORE_CRITICAL_METHOD_STATUS_STK	=	2	/* Push/Pop     int status onto stk.    */
	,CORE_CRITICAL_METHOD_STATUS_LOCAL	=	3	/* Save/Restore int status to local var */
} cpu_critical_t;
#endif
//! @}
	
//! \brief CRITICAL SECTION CONFIGURATION 
//! @{
//!
/* Defines   CPU status register size (see Note #3).    */
typedef CORE_CPU_SAFE				CORE_SR; 
//!
/* Configure CPU critical method      (see Note #1) :   */
#define CORE_CFG_CRITICAL_METHOD 	CORE_CRITICAL_METHOD_STATUS_LOCAL
//!
//#define  CORE_CRITICAL_ENTER() 	{ core_sr = CPU_SR_Save(); }
#define  CORE_CRITICAL_ENTER()     	\
	{  								\
		core_sr = __get_PRIMASK(); 	\
		__disable_irq();			\
	}
//!
//#define  CORE_CRITICAL_EXIT()     { CPU_SR_Restore(core_sr); }
#define  CORE_CRITICAL_EXIT()       \
	{  								\
		__set_PRIMASK(core_sr); 	\
		__enable_irq();				\
	}
//!
#define  CORE_IntDis()				{ CPU_IntDis();            }
//! @}

/*============================ FUNCTION PROTOTYPES ===========================*/
void			CPU_IntDis			(void);
void			CPU_IntEn			(void);

CORE_SR     	CPU_SR_Save			(void);
void			CPU_SR_Restore		(CORE_SR core_sr);

/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* __ASM_CPU_H_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
