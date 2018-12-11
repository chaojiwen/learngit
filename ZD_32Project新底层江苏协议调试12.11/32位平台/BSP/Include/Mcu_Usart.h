#ifndef __Mcu_Usart_H
#define __Mcu_Usart_H


#include "stm32f10x.h"
#include <stdarg.h>
#include <stdio.h>

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>USART相关的宏设置
//	<e>USART功能项目使能
//	<i>使能\关闭<USART>功能项目
#define USART_Enable					(0x01)

//	<e>使能\关闭<USART1>串口调试功能
#define USART1_Serial_Debug_Enable		(0x01)
//  </e>

//	<e>使能\关闭<USART1>SWD调试功能
#define USART1_SWD_Debug_Enable			(0x00)
//  </e>


//  </e>

// </h>
//	<<< end of configuration section >>>

/************************************************************************************************/
/* USART1	接受数据缓冲区	大小  */
#define COM1_RX_Buffer_Size   	(128u)
/* USART1	接受数据缓冲区 */	      
extern u8	COM1_RX_Buffer[COM1_RX_Buffer_Size];



/************************************************************************************************/


/****************************************************  函数声明  *********************************************************/
void USART1_Init(void);
void USART2_Config(u32 baudrate);
int fputc(int ch, FILE *f);
char *itoa(int value, char *string, int radix);
void USART_printf(USART_TypeDef* USARTx,uint8_t *Data,u16 len);

#endif
