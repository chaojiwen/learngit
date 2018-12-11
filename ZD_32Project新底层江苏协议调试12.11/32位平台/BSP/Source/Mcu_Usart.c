/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2012/12/20																			 */
/*************************************************************************************************/
#include "Mcu_Usart.h"
#include <stdarg.h>


/************************************************************************************************/
/* USART1	�������ݻ����� */	   
u8 	COM1_RX_Buffer[COM1_RX_Buffer_Size] = {0};


/************************************************************************************************/

/********************************* ����1���ú���  *********************************

 * ��������USART1_Init
 * ����  ��USART1 GPIO ����,����ģʽ����
 * ����  ����
 * ���  : ��
 * ����  ���ⲿ����

***********************************************************************************/
void USART1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* ����ʱ�� GPIOA��USART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
						   RCC_APB2Periph_GPIOE | 
						   RCC_APB2Periph_USART1, ENABLE);//����GPIOA��GPIOE��USART1��ʱ��

	
	/* ����USART1��TX���ŵĹ���ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;												//ȷ��TX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;											//ȷ��TX���ŵĹ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//TX����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//������д��Ĵ���
	/* ����USART1��RX���ŵĹ���ģʽ */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;												//ȷ��RX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;											//ȷ��RX���ŵĹ���ģʽ����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//������д��Ĵ���
	/* USART1����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;								//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;									//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;										//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;			//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;							//ʹ�ܷ��ͺͽ���
	USART_Init(USART1,&USART_InitStructure);												//������д��Ĵ���
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);											//ʹ��USART1�жϽ���
	USART_Cmd(USART1 , ENABLE );															//��ʹUSADRT1		 

//	/* ����SP3223_EN���ŵĹ���ģʽ */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;												//ȷ��SP3223_EN����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;										//ȷ��SP3223_EN���ŵĹ���ģʽ
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//SP3223_EN����������
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
}

/************************************************************************************************/
/* ������	: USART2_Config													 	 				*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: USART2����ģʽ����								 									*/
/* ��������	: 2015/04/12																		*/
/************************************************************************************************/
void USART2_Config(u32 baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* ����ʱ�� GPIOA��USART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//����GPIOA��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);//����USART2��ʱ��
	/* ����USART2��TX���ŵĹ���ģʽ */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;												//ȷ��TX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;											//ȷ��TX���ŵĹ���ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//TX����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//������д��Ĵ���
	/* ����USART2��RX���ŵĹ���ģʽ */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;												//ȷ��RX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;											//ȷ��RX���ŵĹ���ģʽ����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//������д��Ĵ���
	/* USART2����ģʽ���� */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;								//8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;									//1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;										//����żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;			//��Ӳ��������
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;							//ʹ�ܽ���

	USART_Init(USART2,&USART_InitStructure);												//������д��Ĵ���													//��ʹUSADRT2		 

}


/* **********************************************************************************

 * ��������fputc
 * ����  ���ض���c�⺯��printf��USART1
 * ����  ����
 * ���  ����
 * ����  ����printf����

 * **********************************************************************************/

int fputc(int ch, FILE *f)
{
	
/* ��Printf���ݷ�������  */
#if (USART1_Serial_Debug_Enable == 0x01)
	USART_SendData(USART1, (unsigned char) ch);
#endif
#if (USART1_SWD_Debug_Enable == 0x01)
	ITM_SendChar((uint32_t)(ch));
#endif
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}						
    							   

/* ***********************************************************************************
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
 * ***********************************************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


/* ***********************************************************************************
 * ��������USART1_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 * ***********************************************************************************/
void USART_printf(USART_TypeDef* USARTx,uint8_t *Data,u16 len)
{
	u16 tx_num;
	
	/* ���������ɱ�־�� */
	USART_ClearFlag(USARTx,USART_FLAG_TC);
	/* ѭ���������� */
	for(tx_num = 0;tx_num < len;tx_num++)
	{
		USART_SendData(USARTx,Data[tx_num]);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}
/***************************���ڷ��͵����ַ�����***********************************/
void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data)  
{  
    USART_SendData(USARTx, Data);  
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}  
}
/***************************���ڷ����ַ����ַ�����***********************************/
void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str)    
{    
    while (0 != *str)    
    {    
        UART_PutChar(USARTx, *str);    
        str++;    
    }    
}   







/******************* (C) ZHOU *****END OF FILE****/

