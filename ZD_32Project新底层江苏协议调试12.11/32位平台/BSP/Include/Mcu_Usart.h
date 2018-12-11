#ifndef __Mcu_Usart_H
#define __Mcu_Usart_H


#include "stm32f10x.h"
#include <stdarg.h>
#include <stdio.h>

/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>USART��صĺ�����
//	<e>USART������Ŀʹ��
//	<i>ʹ��\�ر�<USART>������Ŀ
#define USART_Enable					(0x01)

//	<e>ʹ��\�ر�<USART1>���ڵ��Թ���
#define USART1_Serial_Debug_Enable		(0x01)
//  </e>

//	<e>ʹ��\�ر�<USART1>SWD���Թ���
#define USART1_SWD_Debug_Enable			(0x00)
//  </e>


//  </e>

// </h>
//	<<< end of configuration section >>>

/************************************************************************************************/
/* USART1	�������ݻ�����	��С  */
#define COM1_RX_Buffer_Size   	(128u)
/* USART1	�������ݻ����� */	      
extern u8	COM1_RX_Buffer[COM1_RX_Buffer_Size];



/************************************************************************************************/


/****************************************************  ��������  *********************************************************/
void USART1_Init(void);
void USART2_Config(u32 baudrate);
int fputc(int ch, FILE *f);
char *itoa(int value, char *string, int radix);
void USART_printf(USART_TypeDef* USARTx,uint8_t *Data,u16 len);

#endif
