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

#ifndef __TINY_FSM_H__
#define __TINY_FSM_H__

/*============================ INCLUDES ======================================*/
#include ".\compiler.h"

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define DEFINE_TINY_FSM_STATE(__NAME)  \
    typedef struct tiny_fsm_##__NAME##_arg tiny_fsm_##__NAME##_arg_t;\
    typedef void *(*tiny_fsm_##__NAME##_task)(tiny_fsm_##__NAME##_arg_t *pArg);\
    struct tiny_fsm_##__NAME##_arg

#define PARAM       {
#define END_PARAM   };

#define END_DEFINE

#define NEW_TINY_FSM(__NAME, __VAR,__START_STATE) \
    tiny_fsm_##__NAME##_task s_TinyFSM##__VAR = (tiny_fsm_state_##__START_STATE);

#define TINY_FSM_STATE(__NAME, __STATE_NAME)    \
    void *tiny_fsm_state_##__STATE_NAME(tiny_fsm_##__NAME##_arg_t *pArg) 
#define BEGIN       {

#define END         }
#define PRIVATE     static
#define PUBLIC      

#define TINY_FSM_END    return NULL;
#define TINY_FSM_TRS_TO(__STATE_NAME)     return (void *)tiny_fsm_state_##__STATE_NAME;
#define IS_TINY_FSM_CPL(__VAR)      (NULL == s_TinyFSM##__VAR)

#define CALL_TINY_FSM(__NAME,__VAR)       do {\
    tiny_fsm_##__NAME##_task *s_TinyFSM##__NAME##Temp = &s_TinyFSM##__VAR;\
    tiny_fsm_##__NAME##_arg_t tParam = {0};\
    
#define END_CALL(__NAME)    \
    *s_TinyFSM##__NAME##Temp = (tiny_fsm_##__NAME##_task)(*s_TinyFSM##__NAME##Temp)( &tParam );\
    } while(false);
    

/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/


#endif
/* EOF */