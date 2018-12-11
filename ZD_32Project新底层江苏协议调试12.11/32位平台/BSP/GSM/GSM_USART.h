#ifndef _GSM_USART_H
#define _GSM_USART_H		
#include "stm32f10x.h"

//串口发送缓存区长度 
#define   GSM_MAX_SEND_LEN    (1500u)
//串口接收缓存区长度 
#define   GSM_MAX_RECV_LEN    (2500u)


extern vu8  GSM_TX_BUF[GSM_MAX_SEND_LEN];//发送缓冲,最大GSM_MAX_SEND_LEN字节
extern vu8  GSM_RX_BUF[GSM_MAX_RECV_LEN];//接收缓冲,最大GSM_MAX_RECV_LEN个字节.
extern vu16 GSM_RX_BUF_LEN;//串口接收数据长度



//初始化IO 串口3
//bound:波特率	  
void GSM_USART_Init(u32 bound);
//串口3,SIMCOM_Printf函数
//确保一次发送数据不超过GSM_MAX_SEND_LEN字节
void SIMCOM_Printf(char* fmt,...);
void Clear_GSM_RX_BUF(u8 *UsartBuf);//清除缓存数据;//清除缓存数据
/*****************  指定长度的发送字符串 **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, char *str,vu32 strlen);
#endif


