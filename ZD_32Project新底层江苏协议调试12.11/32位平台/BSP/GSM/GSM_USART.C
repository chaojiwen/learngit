#include "GSM_USART.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "GSM_app.h"
//串口发送缓存区 	
vu8  GSM_TX_BUF[GSM_MAX_SEND_LEN];//发送缓冲,最大GSM_MAX_SEND_LEN字节
//串口接收缓存区 
vu8  GSM_RX_BUF[GSM_MAX_RECV_LEN];//接收缓冲,最大GSM_MAX_RECV_LEN个字节.
vu16 GSM_RX_BUF_LEN=0;//串口接收数据长度
/************************************************************************************************
*函数名   ：void USART3_IRQHandler(void)															
*输入参数 ：无											
*输出参数 ：	  																				
*作用     ：串口中断接收函数	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
void USART3_IRQHandler(void)
{
	 vu8 res;	      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	 
		  res =USART_ReceiveData(USART3);	
		  if(DEBUG_Flag==TRUE)
		  USART_SendData(USART1, res) ;//串口3接到的内容都从串口1中打印出来
      if(GSM_RX_BUF_LEN<=GSM_MAX_RECV_LEN)//接收数据长度小于等于最大接收数据长度 符合接收数据的条件
		  {
			   GSM_RX_BUF[GSM_RX_BUF_LEN++]=res;	//存放串口接收到的值	
			}
			else //接收数据超过了最大缓存长度，将其置零
			{
			    GSM_RX_BUF_LEN=0;
			}
	}  				 											 
}
void Clear_GSM_RX_BUF(u8 *UsartBuf)//清除缓存数据
{
	
	  GSM_RX_BUF_LEN=0;
	  memset(UsartBuf,0x00,GSM_MAX_RECV_LEN);


}

/************************************************************************************************
*函数名   ：GSM_USART_Init(u32 bound);														
*输入参数 ：bound :波特率										
*输出参数 ：无	  																				
*作用     ：串口初始化	 	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
void GSM_USART_Init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	                       //GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);                          //串口3时钟使能

 	USART_DeInit(USART3);  //复位串口3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                     //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                               //复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);                                         //初始化PB10
  
  //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                          //浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);                                         //初始化PB11
	
	USART_InitStructure.USART_BaudRate = bound;                                    //波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //收发模式
  
	USART_Init(USART3, &USART_InitStructure);       //初始化串口3
	USART_Cmd(USART3, ENABLE);                      //使能串口 
	//使能接收中断
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  //开启中断   
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

//串口3,SIMCOM_Printf函数
//确保一次发送数据不超过GSM_MAX_SEND_LEN字节
void SIMCOM_Printf(char* fmt,...)
{
  vu16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)GSM_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)GSM_TX_BUF);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		USART_SendData(USART3,GSM_TX_BUF[j]); 
	} 
}
/************************************************************************************************
*函数名   ：void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )															
*输入参数 ：											
*输出参数 ：	  																				
*作用     ： 发送一个字符	 	  
*创建日期 ： 2018/08/23	
*说明     ：
************************************************************************************************/
static void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	/* 发送一个字节数据到USART1 */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/************************************************************************************************
*函数名   ：Usart_SendStr_length( USART_TypeDef * pUSARTx, char *str,vu32 strlen )														
*输入参数 ：											
*输出参数 ：	  																				
*作用     ： 指定长度的发送字符串  
*创建日期 ： 2018/08/23	
*说明     ：
************************************************************************************************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, char *str,vu32 strlen )
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(k < strlen);
}



