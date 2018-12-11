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
#ifndef _USE_COMPILER_H_
#define _USE_COMPILER_H_

//! \name The macros to identify the compiler
//! @{

//! \note for IAR
#ifdef __IS_COMPILER_IAR__
    #undef __IS_COMPILER_IAR__
#endif
#define __IS_COMPILER_IAR__ defined(__IAR_SYSTEMS_ICC__)

//! \note for avr-gcc
#ifdef __IS_COMPILER_GCC__
    #undef __IS_COMPILER_GCC__
#endif
#define __IS_COMPILER_GCC__ defined(__GNUC__)
//! @}

//! \brief include I/O head file
#if __IS_COMPILER_ICC__
    #include <iccioavr.h>
#elif __IS_COMPILER_GCC__
    #include <avr/io.h>
    #include <avr/pgmspace.h>
#elif __IS_COMPILER_IAR__
#ifndef _USE_IGNORE_IAR_IOAVR_H_
    #ifndef _DEVICE_IO
    #define _DEVICE_IO
        #include <ioavr.h>
    #endif
    #include<intrinsics.h>
#endif
#endif

#include ".\preprocessor\mrepeat.h"

/*!  \note using the ANSI-C99 standard type,if the file stdint.h dose not exit
 *!        you should define it all by yourself.
 *!
 */
#include <stdint.h>
#include <stdbool.h>

//! \name byte stream
//! @{
typedef struct
{
    volatile uint8_t *pchStream;    //!< stream pointer
    uint8_t chLength;               //!< stream length
}byte_stream_t;
//! @}


//! \name boolean value
//! @{
#ifndef false
    # define false          0x00
#endif

#ifndef true
    # define true           (!false)
#endif

//! @}


//! \name assmebly support
//! @{
#if __IS_COMPILER_IAR__
    # define __asm__            asm
    # define __volatile__
#endif
#ifndef NOP
    #define NOP()               __asm__ __volatile__ ("nop");
#endif

//! @}


//! \brief The mcu memory align mode
# define MCU_MEM_ALIGN_SIZE     sizeof(int)


//! \brief The mcu memory endian mode
# define MCU_MEM_LITTLE_ENDIAN  true

//! \brief The default NULL value
#ifndef NULL
    # define NULL               (0x00)
#endif


//! \brief system macros
#define MAX(__A,__B)  (((__A) > (__B)) ? (__A) : (__B))
#define MIN(__A,__B)  (((__A) < (__B)) ? (__A) : (__B))

#define UBOUND(__ARRAY) (sizeof(__ARRAY)/sizeof(__ARRAY[0]))

#define ABS(__NUM)    (((__NUM) < 0)?(-(__NUM)):(__NUM))

#ifndef BIT
#define BIT(__N)		((uint16_t)1 << (__N))
#endif
#ifndef _BV
#define _BV(__N)        ((uint16_t)1 << (__N))
#endif
#ifndef _BM
#define _BM(__N)		(_BV(__N) - 1)
#endif
#ifndef _BME
#define _BME(__N1,__N2)	(_BM((__N2) + 1) - _BM((__N1)))
#endif
#ifndef _BVP
#define _BVP(__N1,__N2)	(_BV(__N2) + _BV(__N1))
#endif

//! \brief This macro convert variable types between different datatypes.
# define TYPE_CONVERTER(__ADDR,__TYPE)     (*((__TYPE *)(__ADDR)))

//! \brief initialize large object
# define OBJECT_INIT_ZERO(__OBJECT) \
            do\
            {\
                struct OBJECT_INIT\
                {\
                    uint8_t StructMask[sizeof(__OBJECT)];\
                }NULL_OBJECT = {{0}};\
                \
                (*((struct OBJECT_INIT *)&(__OBJECT))) = NULL_OBJECT;\
            }\
            while (false)



#if __IS_COMPILER_IAR__
# define FLASH		__flash
# define EEPROM     __eeprom
# define NO_INIT    __no_init
#elif __IS_COMPILER_GCC__
# define FLASH		const
# define EEPROM     const
# define NO_INIT
#endif



#endif
