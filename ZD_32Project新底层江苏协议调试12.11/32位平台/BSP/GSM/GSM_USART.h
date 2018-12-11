#ifndef _GSM_USART_H
#define _GSM_USART_H		
#include "stm32f10x.h"

//���ڷ��ͻ��������� 
#define   GSM_MAX_SEND_LEN    (1500u)
//���ڽ��ջ��������� 
#define   GSM_MAX_RECV_LEN    (2500u)


extern vu8  GSM_TX_BUF[GSM_MAX_SEND_LEN];//���ͻ���,���GSM_MAX_SEND_LEN�ֽ�
extern vu8  GSM_RX_BUF[GSM_MAX_RECV_LEN];//���ջ���,���GSM_MAX_RECV_LEN���ֽ�.
extern vu16 GSM_RX_BUF_LEN;//���ڽ������ݳ���



//��ʼ��IO ����3
//bound:������	  
void GSM_USART_Init(u32 bound);
//����3,SIMCOM_Printf����
//ȷ��һ�η������ݲ�����GSM_MAX_SEND_LEN�ֽ�
void SIMCOM_Printf(char* fmt,...);
void Clear_GSM_RX_BUF(u8 *UsartBuf);//�����������;//�����������
/*****************  ָ�����ȵķ����ַ��� **********************/
void Usart_SendStr_length( USART_TypeDef * pUSARTx, char *str,vu32 strlen);
#endif


