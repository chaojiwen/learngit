;**
; ******************************************************************************
; * @file              : vectors.s
; * @author            : HZ Zeda Team
; * @version           : V1.0.0.0
; * @date              : 13/09/2014 11:58:52
; * @brief             : s file
; * @description       : INTERRUPT VECTORS
; *                      ARM
;                        KEIL's uVision3 
;                       (RealView Microprocessor Developer Kit)
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
                AREA   VECT, CODE, READONLY                     ; Name this block of code                                   ;
                THUMB

                ENTRY

;******************************************************************************
;                                  IMPORTS
;******************************************************************************

				IMPORT	HAL_SysTickHandler
		        IMPORT	HAL_PendSVHandler

		        IMPORT	BSP_IntHandlerWWDG
		        IMPORT	BSP_IntHandlerPVD
		        IMPORT	BSP_IntHandlerTAMPER
		        IMPORT	BSP_IntHandlerRTC
		        IMPORT	BSP_IntHandlerFLASH
		        IMPORT	BSP_IntHandlerRCC
		        IMPORT	BSP_IntHandlerEXTI0
		        IMPORT	BSP_IntHandlerEXTI1
		        IMPORT	BSP_IntHandlerEXTI2
		        IMPORT	BSP_IntHandlerEXTI3
		        IMPORT	BSP_IntHandlerEXTI4
		        IMPORT	BSP_IntHandlerDMA1_CH1
		        IMPORT	BSP_IntHandlerDMA1_CH2
		        IMPORT	BSP_IntHandlerDMA1_CH3
		        IMPORT	BSP_IntHandlerDMA1_CH4
		        IMPORT	BSP_IntHandlerDMA1_CH5

		        IMPORT	BSP_IntHandlerDMA1_CH6
		        IMPORT	BSP_IntHandlerDMA1_CH7
		        IMPORT	BSP_IntHandlerADC1_2
		        IMPORT	BSP_IntHandlerUSB_HP_CAN_TX
		        IMPORT	BSP_IntHandlerUSB_LP_CAN_RX0
		        IMPORT	BSP_IntHandlerCAN_RX1
		        IMPORT	BSP_IntHandlerCAN_SCE
		        IMPORT	BSP_IntHandlerEXTI9_5
		        IMPORT	BSP_IntHandlerTIM1_BRK
		        IMPORT	BSP_IntHandlerTIM1_UP
		        IMPORT	BSP_IntHandlerTIM1_TRG_COM
		        IMPORT	BSP_IntHandlerTIM1_CC
		        IMPORT	BSP_IntHandlerTIM2
		        IMPORT	BSP_IntHandlerTIM3
		        IMPORT	BSP_IntHandlerTIM4
		        IMPORT	BSP_IntHandlerI2C1_EV

		        IMPORT	BSP_IntHandlerI2C1_ER
		        IMPORT	BSP_IntHandlerI2C2_EV
		        IMPORT	BSP_IntHandlerI2C2_ER
		        IMPORT	BSP_IntHandlerSPI1
		        IMPORT	BSP_IntHandlerSPI2
		        ;IMPORT	BSP_IntHandlerUSART1
		        ;IMPORT	BSP_IntHandlerUSART2
		        ;IMPORT	BSP_IntHandlerUSART3 
		        IMPORT	BSP_IntHandlerEXTI15_10
		        IMPORT	BSP_IntHandlerRTCAlarm
		        IMPORT	BSP_IntHandlerUSBWakeUp
				
				IMPORT  BSP_IntHandlerTIM8_BRK        				
				IMPORT  BSP_IntHandlerTIM8_UP         				
				IMPORT  BSP_IntHandlerTIM8_TRG_COM    				
				IMPORT  BSP_IntHandlerTIM8_CC         				
				IMPORT  BSP_IntHandlerADC3            				
				IMPORT  BSP_IntHandlerFSMC            				
				IMPORT  BSP_IntHandlerSDIO            				
				IMPORT  BSP_IntHandlerTIM5            				
				IMPORT  BSP_IntHandlerSPI3            				
				;IMPORT  BSP_IntHandlerUART4           				
				;IMPORT  BSP_IntHandlerUART5           				
				IMPORT  BSP_IntHandlerTIM6            				
				IMPORT  BSP_IntHandlerTIM7            				
				IMPORT  BSP_IntHandlerDMA2_CH1   				
				IMPORT  BSP_IntHandlerDMA2_CH2   				
				IMPORT  BSP_IntHandlerDMA2_CH3   				
				IMPORT  BSP_IntHandlerDMA2_CH4_5 

                IMPORT  ResetHndlr
                IMPORT  ||Image$$ARM_LIB_STACK$$ZI$$Limit||     ; Import stack limit from scatter-loading file              ;

;******************************************************************************
;                                  EXPORTS
;******************************************************************************


;******************************************************************************
;                                DEFINITIONS
;******************************************************************************


;******************************************************************************
;                      INITIALIZE EXCEPTION VECTORS
;******************************************************************************

__Vectors
				DCD     ||Image$$ARM_LIB_STACK$$ZI$$Limit||		;  0, SP start value.                                         
				DCD     ResetHndlr                             	;  1, PC start value.                                         
				DCD     App_NMI_ISR                            	;  2, NMI                                                     
				DCD     App_Fault_ISR                          	;  3, Hard Fault                                              
				DCD     App_MemFault_ISR                       	;  4, Memory Management                                      
				DCD     App_BusFault_ISR                       	;  5, Bus Fault                                               
				DCD     App_UsageFault_ISR                     	;  6, Usage Fault                                             
				DCD     App_Reserved_ISR                       	;  7, Reserved                                                
				DCD     App_Reserved_ISR                       	;  8, Reserved                                                
				DCD     App_Reserved_ISR                      	;  9, Reserved                                                
				DCD     App_Reserved_ISR                       	; 10, Reserved                                                
				DCD     App_Spurious_ISR                       	; 11, SVCall                                                  
				DCD     App_Spurious_ISR                       	; 12, Debug Monitor                                           
				DCD     App_Spurious_ISR                       	; 13, Reserved                                                
				DCD     HAL_PendSVHandler                     	; 14, PendSV Handler                                          
				DCD     HAL_SysTickHandler                     	; 15, uC/OS-II Tick ISR Handler
                                       
				DCD     BSP_IntHandlerWWDG                     	; 16, INTISR[  0]  Window Watchdog.                   
				DCD     BSP_IntHandlerPVD                      	; 17, INTISR[  1]  PVD through EXTI Line Detection.    
				DCD     BSP_IntHandlerTAMPER                  	; 18, INTISR[  2]  Tamper Interrupt.                   
				DCD     BSP_IntHandlerRTC                      	; 19, INTISR[  3]  RTC Global Interrupt.               
				DCD     BSP_IntHandlerFLASH                    	; 20, INTISR[  4]  FLASH Global Interrupt.             
				DCD     BSP_IntHandlerRCC                      	; 21, INTISR[  5]  RCC Global Interrupt.               
				DCD     BSP_IntHandlerEXTI0                    	; 22, INTISR[  6]  EXTI Line0 Interrupt.               
				DCD     BSP_IntHandlerEXTI1                    	; 23, INTISR[  7]  EXTI Line1 Interrupt.               
				DCD     BSP_IntHandlerEXTI2                    	; 24, INTISR[  8]  EXTI Line2 Interrupt.               
				DCD     BSP_IntHandlerEXTI3                    	; 25, INTISR[  9]  EXTI Line3 Interrupt.               
				DCD     BSP_IntHandlerEXTI4                    	; 26, INTISR[ 10]  EXTI Line4 Interrupt.               
				DCD     BSP_IntHandlerDMA1_CH1                 	; 27, INTISR[ 11]  DMA Channel1 Global Interrupt.      
				DCD     BSP_IntHandlerDMA1_CH2					; 28, INTISR[ 12]  DMA Channel2 Global Interrupt.      
				DCD     BSP_IntHandlerDMA1_CH3                	; 29, INTISR[ 13]  DMA Channel3 Global Interrupt.      
				DCD     BSP_IntHandlerDMA1_CH4                	; 30, INTISR[ 14]  DMA Channel4 Global Interrupt.      
				DCD     BSP_IntHandlerDMA1_CH5                	; 31, INTISR[ 15]  DMA Channel5 Global Interrupt.      

				DCD     BSP_IntHandlerDMA1_CH6                	; 32, INTISR[ 16]  DMA Channel6 Global Interrupt.      
				DCD     BSP_IntHandlerDMA1_CH7                	; 33, INTISR[ 17]  DMA Channel7 Global Interrupt.      
				DCD     BSP_IntHandlerADC1_2                   	; 34, INTISR[ 18]  ADC1 & ADC2 Global Interrupt.       
				DCD     BSP_IntHandlerUSB_HP_CAN_TX            	; 35, INTISR[ 19]  USB High Prio / CAN TX  Interrupts. 
				DCD     BSP_IntHandlerUSB_LP_CAN_RX0           	; 36, INTISR[ 20]  USB Low  Prio / CAN RX0 Interrupts. 
				DCD     BSP_IntHandlerCAN_RX1                  	; 37, INTISR[ 21]  CAN RX1 Interrupt.                  
				DCD     BSP_IntHandlerCAN_SCE                  	; 38, INTISR[ 22]  CAN SCE Interrupt.                  
				DCD     BSP_IntHandlerEXTI9_5                  	; 39, INTISR[ 23]  EXTI Line[9:5] Interrupt.           
				DCD     BSP_IntHandlerTIM1_BRK                	; 40, INTISR[ 24]  TIM1 Break  Interrupt.              
				DCD     BSP_IntHandlerTIM1_UP                  	; 41, INTISR[ 25]  TIM1 Update Interrupt.              
				DCD     BSP_IntHandlerTIM1_TRG_COM            	; 42, INTISR[ 26]  TIM1 Trig & Commutation Interrupts. 
				DCD     BSP_IntHandlerTIM1_CC                  	; 43, INTISR[ 27]  TIM1 Capture Compare Interrupt.     
				DCD     BSP_IntHandlerTIM2                     	; 44, INTISR[ 28]  TIM2 Global Interrupt.              
				DCD     BSP_IntHandlerTIM3                     	; 45, INTISR[ 29]  TIM3 Global Interrupt.              
				DCD     BSP_IntHandlerTIM4                     	; 46, INTISR[ 30]  TIM4 Global Interrupt.              
				DCD     BSP_IntHandlerI2C1_EV                  	; 47, INTISR[ 31]  I2C1 Event  Interrupt.              

				DCD     BSP_IntHandlerI2C1_ER                  	; 48, INTISR[ 32]  I2C1 Error  Interrupt.              
				DCD     BSP_IntHandlerI2C2_EV                  	; 49, INTISR[ 33]  I2C2 Event  Interrupt.              
				DCD     BSP_IntHandlerI2C2_ER                  	; 50, INTISR[ 34]  I2C2 Error  Interrupt.             
				DCD     BSP_IntHandlerSPI1                    	; 51, INTISR[ 35]  SPI1 Global Interrupt.              
				DCD     BSP_IntHandlerSPI2                     	; 52, INTISR[ 36]  SPI2 Global Interrupt.              
				DCD     BSP_IntHandlerUSART1                 	; 53, INTISR[ 37]  USART1 Global Interrupt.            
				DCD     BSP_IntHandlerUSART2                   	; 54, INTISR[ 38]  USART2 Global Interrupt.            
				DCD     BSP_IntHandlerUSART3                   	; 55, INTISR[ 39]  USART3 Global Interrupt.            
				DCD     BSP_IntHandlerEXTI15_10                	; 56, INTISR[ 40]  EXTI Line [15:10] Interrupts.       
				DCD     BSP_IntHandlerRTCAlarm                 	; 57, INTISR[ 41]  RTC Alarm EXT Line Interrupt.       
				DCD     BSP_IntHandlerUSBWakeUp                	; 58, INTISR[ 42]  USB Wakeup from Suspend EXTI Int.   
		
				DCD     BSP_IntHandlerTIM8_BRK        			; 59, INTISR[ 43]  TIM8 Break
				DCD     BSP_IntHandlerTIM8_UP         			; 60, INTISR[ 44]  TIM8 Update
				DCD     BSP_IntHandlerTIM8_TRG_COM    			; 61, INTISR[ 45]  TIM8 Trigger and Commutation
				DCD     BSP_IntHandlerTIM8_CC         			; 62, INTISR[ 46]  TIM8 Capture Compare
				DCD     BSP_IntHandlerADC3            			; 63, INTISR[ 47]  ADC3
				DCD     BSP_IntHandlerFSMC            			; 64, INTISR[ 48]  FSMC
				DCD     BSP_IntHandlerSDIO            			; 65, INTISR[ 49]  SDIO
				DCD     BSP_IntHandlerTIM5            			; 66, INTISR[ 50]  TIM5
				DCD     BSP_IntHandlerSPI3            			; 67, INTISR[ 51]  SPI3
				DCD     BSP_IntHandlerUART4           			; 68, INTISR[ 52]  UART4
				DCD     BSP_IntHandlerUART5           			; 69, INTISR[ 53]  UART5
				DCD     BSP_IntHandlerTIM6            			; 70, INTISR[ 54]  TIM6
				DCD     BSP_IntHandlerTIM7            			; 71, INTISR[ 55]  TIM7
				DCD     BSP_IntHandlerDMA2_CH1   				; 72, INTISR[ 56]  DMA2 Channel1
				DCD     BSP_IntHandlerDMA2_CH2   				; 73, INTISR[ 57]  DMA2 Channel2
				DCD     BSP_IntHandlerDMA2_CH3   				; 74, INTISR[ 58]  DMA2 Channel3
				DCD     BSP_IntHandlerDMA2_CH4_5 				; 75, INTISR[ 59]  DMA2 Channel4 & Channel5
        
        
;******************************************************************************
;                          DEFAULT HANDLERS
;******************************************************************************

App_NMI_ISR\
                PROC
                EXPORT  App_NMI_ISR                 [WEAK]
                B       .
                ENDP

App_Fault_ISR\
                PROC
                EXPORT  App_Fault_ISR               [WEAK]
                B       .
                ENDP

App_MemFault_ISR\
                PROC
                EXPORT  App_MemFault_ISR            [WEAK]
                B       .
                ENDP

App_BusFault_ISR\
                PROC
                EXPORT  App_BusFault_ISR            [WEAK]
                B       .
                ENDP

App_UsageFault_ISR\
                PROC
                EXPORT  App_UsageFault_ISR          [WEAK]
                B       .
                ENDP

App_Spurious_ISR\
                PROC
                EXPORT  App_Spurious_ISR            [WEAK]
                B       .
                ENDP
					
App_Reserved_ISR\
                PROC
                EXPORT  App_Reserved_ISR            [WEAK]
                B       .
                ENDP

;******************************************************************************
;                       USER DEFAULT HANDLERS
;******************************************************************************
Default_Handler\
                PROC
                EXPORT  BSP_IntHandlerUSART1        [WEAK]
                EXPORT  BSP_IntHandlerUSART2        [WEAK]
                EXPORT  BSP_IntHandlerUSART3        [WEAK]
                EXPORT  BSP_IntHandlerUART4         [WEAK]
                EXPORT  BSP_IntHandlerUART5         [WEAK]
					
BSP_IntHandlerUSART1
BSP_IntHandlerUSART2
BSP_IntHandlerUSART3
BSP_IntHandlerUART4
BSP_IntHandlerUART5
                B       .
                ENDP
					
					
                ALIGN
					
                END
;******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE*****
