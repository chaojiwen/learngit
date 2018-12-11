/***************************************************************************
 *   Copyright(C)2009-2012 by Gorgon Meducer<Embedded_zhuoran@hotmail.com> *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef __USE_ARM_COMPILER_H__
#define __USE_ARM_COMPILER_H__

/*============================ INCLUDES ======================================*/
#if __IS_COMPILER_IAR__
    #include<intrinsics.h>
#else

#endif


#ifndef __TEMPLATE_ENVIRONMENT_CFG__
#define __TEMPLATE_ENVIRONMENT_CFG__

/*============================ INCLUDES ======================================*/
/*============================ MACROS ========================================*/

/*! \note DO NOT modify this
 */
#if   defined(__FREESCALE_K60__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M4__                    //!< cortex-m4
#   define __FREESCALE__                    //!< Freescale
#   define __KINETIS__                      //!< Kinetis series
#   define __K60__                          //!< K60
#   define __MK64FN1__                      //!< Part Number
#elif   defined(__FREESCALE_K20__)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M4__                    //!< cortex-m4
#   define __FREESCALE__                    //!< Freescale
#   define __KINETIS__                      //!< Kinetis series
#   define __K20__                          //!< K20
#   define __MK20D5__                       //!< Part Number
#elif   defined(STM32F10X_HD)
#   define __CPU_ARM__                      //!< arm series
#   define __CORTEX_M3__                    //!< cortex-m3
#   define __STMICROELECTRONIC__            //!< ST
#   define __STM32F1__                    	//!< F1 series
#   define __F103__                         //!< F103
#   define __ZET__                       	//!< Part Number
#endif

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
/*============================ IMPLEMENTATION ================================*/

#endif


/*============================ MACROS ========================================*/

#ifndef DEF_REG
#define DEF_REG                     \
        union  {                    \
            struct {
#endif
    
#ifndef END_DEF_REG
#define END_DEF_REG(__NAME)         \
            };                      \
            reg32_t Value;          \
        }__NAME;
#endif

#ifndef __REG_MACRO__
#define __REG_MACRO__
#endif


#ifndef REG_RSVD_0x10
#define REG_RSVD_0x10                   \
    reg32_t                     : 32;   \
    reg32_t                     : 32;   \
    reg32_t                     : 32;   \
    reg32_t                     : 32;   
#endif
#ifndef REG_RSVD_0x80       
#define REG_RSVD_0x80                   \
            REG_RSVD_0x10               \
            REG_RSVD_0x10               \
            REG_RSVD_0x10               \
            REG_RSVD_0x10               \
            REG_RSVD_0x10               \
            REG_RSVD_0x10               \
            REG_RSVD_0x10               \
            REG_RSVD_0x10
#endif

#ifndef REG_RSVD_0x100                 
#define REG_RSVD_0x100                  \
            REG_RSVD_0x80               \
            REG_RSVD_0x80
#endif

#ifndef REG_RSVD_0x800
#define REG_RSVD_0x800                  \
            REG_RSVD_0x100              \
            REG_RSVD_0x100              \
            REG_RSVD_0x100              \
            REG_RSVD_0x100              \
            REG_RSVD_0x100              \
            REG_RSVD_0x100              \
            REG_RSVD_0x100              \
            REG_RSVD_0x100
#endif

//! \brief The mcu memory align mode
# define ATOM_INT_SIZE     					sizeof(int)

//! ALU integer width in byte
//# define ATOM_INT_SIZE					4

//! \brief The mcu memory align mode
# define MCU_MEM_ALIGN_SIZE             	ATOM_INT_SIZE

//! \brief The mcu memory endian mode
# define MCU_MEM_LITTLE_ENDIAN  true
	
#ifndef __volatile__
#define __volatile__
#endif

//! \brief 1 cycle nop operation
#ifndef NOP
    #define NOP()                       	__asm__ __volatile__ ("nop");
#endif


//! \brief none standard memory types
#if __IS_COMPILER_IAR__
#   define __FLASH          const
#   define EEPROM           const
#   define NO_INIT          __no_init
#   define ROOT             __root
#   define IN_LINE          inline
#   define WEAK             __weak
#   define RAMFUNC          __ramfunc
#   define __asm__          __asm
#   define ALIGN(__N)       _Pragma(__STR(data_alignment=__N))
#   define AT_ADDR(__ADDR)  @ __ADDR
#   define SECTION(__SEC)   _Pragma(__STR(location=__SEC))
#elif __IS_COMPILER_GCC__
#   define __FLASH          const
#   define EEPROM           const
#   define NO_INIT          __attribute__(( section( "noinit"))
#   define ROOT             __attribute__((used))    
#   define IN_LINE          inline
#   define WEAK             __attribute__((weak))
#   define RAMFUNC          __attribute__((section ("textrw")))
#   define __asm__          __asm
#   define ALIGN(__N)       __attribute__((aligned (__N)))
#   define AT_ADDR(__ADDR)  __attribute__((at(__ADDR))) 
#   define SECTION(__SEC)   __attribute__((section (__SEC)))
#elif __IS_COMPILER_MDK__
#   define __FLASH          const
#   define EEPROM           const
#   define NO_INIT          __attribute__( ( section( "noinit"),zero_init) )
#   define ROOT             __attribute__((used))    
#   define IN_LINE          __inline
#   define WEAK             __attribute__((weak))
#   define RAMFUNC          __attribute__((section ("textrw")))
#   define __asm__          __asm
#   define ALIGN(__N)       __attribute__((aligned (__N))) 
#   define AT_ADDR(__ADDR)  __attribute__((at(__ADDR)))
#   define SECTION(__SEC)   __attribute__((section (__SEC)))
#endif

/*----------------------------------------------------------------------------*
 * Signal & Interrupt Definition                                              *
 *----------------------------------------------------------------------------*/

  /*!< Macro to enable all interrupts. */
#if __IS_COMPILER_IAR__
#define ENABLE_GLOBAL_INTERRUPT()      	 	__enable_interrupt()
#else
#define ENABLE_GLOBAL_INTERRUPT()       	__asm__ __volatile__ (" CPSIE i")
#endif

  /*!< Macro to disable all interrupts. */
#if __IS_COMPILER_IAR__
#define DISABLE_GLOBAL_INTERRUPT()      	__disable_interrupt()
#else
#define DISABLE_GLOBAL_INTERRUPT()       	__asm__ __volatile__ (" CPSID i");
#endif

#if __IS_COMPILER_IAR__
#define GET_GLOBAL_INTERRUPT_STATE()        __get_interrupt_state()
#define SET_GLOBAL_INTERRUPT_STATE(__STATE) __set_interrupt_state(__STATE)
typedef __istate_t   istate_t;
#elif __IS_COMPILER_MDK__
#define GET_GLOBAL_INTERRUPT_STATE()        __get_PRIMASK()
#define SET_GLOBAL_INTERRUPT_STATE(__STATE)	__set_PRIMASK(__STATE)
typedef unsigned long __istate_t;
typedef __istate_t   istate_t;
#endif

/*============================ TYPES =========================================*/
/*============================ INCLUDES ======================================*/

/*----------------------------------------------------------------------------*
 * Device Dependent Compiler Files                                            *
 *----------------------------------------------------------------------------*/
#if     defined(__CORTEX_M0__)
#include ".\cortex_m0_compiler.h"
#elif   defined(__CORTEX_M0P__)
#include ".\cortex_m0p_compiler.h"
#elif   defined(__CORTEX_M3__)
#include ".\cortex_m3_compiler.h"
#elif   defined(__CORTEX_M4__)
#include ".\cortex_m4_compiler.h"
#else
#warning No supported compiler.h file!
#endif

/*!  \note using the ANSI-C99 standard type,if the file stdint.h dose not exit
 *!        you should define it all by yourself.
 *!
 */
#include ".\app_type.h"

//! \brief for interrupt 
#include ".\signal.h"

#endif

