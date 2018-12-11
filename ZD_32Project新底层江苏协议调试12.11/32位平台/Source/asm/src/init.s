;**
; ******************************************************************************
; * @file              : init.s
; * @author            : HZ Zeda Team
; * @version           : V1.0.0.0
; * @date              : 13/09/2014 11:58:55
; * @brief             : s file
; * @description       : Initialiation
; *	For                : ARMv7M Cortex-M3
; * Mode			   : Thumb2
; * Toolchain          : RealView Development Suite
;                        RealView Microcontroller Development Kit (MDK)
;                        ARM Developer Suite (ADS)
;                        Keil uVision
; ******************************************************************************
; * @attention
; *
; * COPYRIGHT 2014 STMicroelectronics
; *
; * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
; * You may not use this file except in compliance with the License.
; * You may obtain a copy of the License at:
; *
; *        http://www.st.com/software_license_agreement_liberty_v2
; *
; * Unless required by applicable law or agreed to in writing, software 
; * distributed under the License is distributed on an "AS IS" BASIS, 
; * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; *
; ******************************************************************************
; */

				PRESERVE8
                AREA   INIT, CODE, READONLY                     ; Name this block of code                                   ;
                THUMB

;******************************************************************************
;                                  IMPORTS
;******************************************************************************
                                
                IMPORT  __main
                IMPORT  SystemInit

;******************************************************************************
;                                  EXPORTS
;******************************************************************************


;******************************************************************************
;                                DEFINITIONS
;******************************************************************************


;******************************************************************************
;                             PERFORM ROM/RAM REMAPPING
;******************************************************************************                

                ENTRY
;ResetHndlr      

;******************************************************************************
;                              SETUP STACK POINTERS
;******************************************************************************
                

;******************************************************************************
;                                   MOVE TO MAIN
;******************************************************************************
ResetHndlr      PROC
                EXPORT  ResetHndlr                 [WEAK]
				LDR     R0, =SystemInit
                BLX     R0         
                LDR     R0, =__main
                BX      R0                             	    ; Save this in register for possible long jump              ;
				ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************

                 END

;******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE*****
