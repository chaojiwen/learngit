/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2012/12/20																			 */
/*************************************************************************************************/
#include "Mcu_Usart.h"
#include <stdarg.h>


/************************************************************************************************/
/* USART1	接受数据缓冲区 */	   
u8 	COM1_RX_Buffer[COM1_RX_Buffer_Size] = {0};


/************************************************************************************************/

/********************************* 串口1配置函数  *********************************

 * 函数名：USART1_Init
 * 描述  ：USART1 GPIO 配置,工作模式配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用

***********************************************************************************/
void USART1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* 配置时钟 GPIOA和USART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
						   RCC_APB2Periph_GPIOE | 
						   RCC_APB2Periph_USART1, ENABLE);//配置GPIOA、GPIOE和USART1的时钟

	
	/* 配置USART1的TX引脚的工作模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;												//确定TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;											//确定TX引脚的工作模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//TX输出最高速率
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//将数据写入寄存器
	/* 配置USART1的RX引脚的工作模式 */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;												//确定RX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;											//确定RX引脚的工作模式：上拉输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//将数据写入寄存器
	/* USART1工作模式配置 */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;								//8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;									//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;										//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;			//无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;							//使能发送和接收
	USART_Init(USART1,&USART_InitStructure);												//将数据写入寄存器
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);											//使能USART1中断接收
	USART_Cmd(USART1 , ENABLE );															//能使USADRT1		 

//	/* 配置SP3223_EN引脚的工作模式 */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;												//确定SP3223_EN引脚
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;										//确定SP3223_EN引脚的工作模式
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//SP3223_EN输出最高速率
//	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
}

/************************************************************************************************/
/* 函数名	: USART2_Config													 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: USART2工作模式配置								 									*/
/* 创建日期	: 2015/04/12																		*/
/************************************************************************************************/
void USART2_Config(u32 baudrate)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* 配置时钟 GPIOA和USART1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//配置GPIOA的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);//配置USART2的时钟
	/* 配置USART2的TX引脚的工作模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;												//确定TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;											//确定TX引脚的工作模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;										//TX输出最高速率
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//将数据写入寄存器
	/* 配置USART2的RX引脚的工作模式 */
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;												//确定RX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;											//确定RX引脚的工作模式：上拉输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);													//将数据写入寄存器
	/* USART2工作模式配置 */
	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;								//8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;									//1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;										//无奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;			//无硬件流控制
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;							//使能接收

	USART_Init(USART2,&USART_InitStructure);												//将数据写入寄存器													//能使USADRT2		 

}


/* **********************************************************************************

 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用

 * **********************************************************************************/

int fputc(int ch, FILE *f)
{
	
/* 将Printf内容发往串口  */
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
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
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
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 * ***********************************************************************************/
void USART_printf(USART_TypeDef* USARTx,uint8_t *Data,u16 len)
{
	u16 tx_num;
	
	/* 清除发送完成标志。 */
	USART_ClearFlag(USARTx,USART_FLAG_TC);
	/* 循环发送数据 */
	for(tx_num = 0;tx_num < len;tx_num++)
	{
		USART_SendData(USARTx,Data[tx_num]);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}
/***************************串口发送单个字符函数***********************************/
void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data)  
{  
    USART_SendData(USARTx, Data);  
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET){}  
}
/***************************串口发送字符串字符函数***********************************/
void UART_PutStr (USART_TypeDef* USARTx, uint8_t *str)    
{    
    while (0 != *str)    
    {    
        UART_PutChar(USARTx, *str);    
        str++;    
    }    
}   







/******************* (C) ZHOU *****END OF FILE****/

