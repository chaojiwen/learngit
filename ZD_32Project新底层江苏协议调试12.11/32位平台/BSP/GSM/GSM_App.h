#ifndef _GSM_App_H   
#define _GSM_App_H	
#include "interface.h"
#include "stm32f10x.h"
#include <stdio.h>
extern BOOL   DEBUG_Flag;//���������Ϣ������Ʊ�־λ  GSM_DEBUG_Flag = FALSE�����򿪵�����Ϣ���  GSM_DEBUG_Flag = TRUE�����
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//===========================================================================
//
//  <e0> ��ӡ���Թ���
//  <i>  �Ƿ�ʹ�ô�������ӡ��־���ڵ��Թ���
//	<o1.0..3>��־������ͣ
//  <0=> ��������־����
//  <1=> ������־����
//	<i>�Ƿ����ô�ӡ��־�Ĺ���
// 
//  </e>
#define USART_Enable				                     (0x01)
#define USART1_Serial_Debug_Enable				       (0x01)
#if USART_Enable
	#if (USART1_Serial_Debug_Enable==0)   //��������־����
//		#define p_dbg(...) 
		#define Log_printf(...) 
	#elif (USART1_Serial_Debug_Enable==1) //������־����
//		#define p_dbg(...)  \
//		do{\
//			if(DEBUG_Flag==1)\
//			{\
//				printf("[D: %d.%03d] ",  OSTimeGet()/1000, OSTimeGet()%1000);\
//				printf(__VA_ARGS__); \
//				printf("\r\n");\
//			}\
//		}while(0)
		#define Log_printf(...) \
		do{\
			if(DEBUG_Flag==1)\
			{\
				printf(__VA_ARGS__);\
			}\
		}while(0)
	#endif
#else
//	#define p_dbg(...) 
	#define Log_printf(...) 
#endif
//	<<< end of configuration section >>>
	

bool  SetWifiPara(void);//����Wifiģ�����
bool  Sim800c_env_init(_SCSStruct * state);//sim800c��ʼ��
int   Gsm_get_signal(void);//��ȡGSMģ���ź�ǿ��
int   Gsm_get_gprs_state(void);//���GSM��GPRS�ĸ���״̬
bool  Gsm_gprs_comm(char *wbuf, int wlen, char *rbuf, int* rlen,u8 Server_Id);//��������������ݲ���������
bool  Server_Send_Data(char *wbuf, int wlen,u8 Server_Id);//��ָ����������������


#endif  /*_SIM800C_App_H_*/
