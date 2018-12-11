#ifndef _GSM_App_H   
#define _GSM_App_H	
#include "interface.h"
#include "stm32f10x.h"
#include <stdio.h>
extern BOOL   DEBUG_Flag;//程序调试信息输出控制标志位  GSM_DEBUG_Flag = FALSE代表不打开调试信息输出  GSM_DEBUG_Flag = TRUE代表打
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//===========================================================================
//
//  <e0> 打印调试功能
//  <i>  是否使用串口来打印日志用于调试功能
//	<o1.0..3>日志功能启停
//  <0=> 不启用日志功能
//  <1=> 启用日志功能
//	<i>是否启用打印日志的功能
// 
//  </e>
#define USART_Enable				                     (0x01)
#define USART1_Serial_Debug_Enable				       (0x01)
#if USART_Enable
	#if (USART1_Serial_Debug_Enable==0)   //不启用日志功能
//		#define p_dbg(...) 
		#define Log_printf(...) 
	#elif (USART1_Serial_Debug_Enable==1) //启用日志功能
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
	

bool  SetWifiPara(void);//设置Wifi模块参数
bool  Sim800c_env_init(_SCSStruct * state);//sim800c初始化
int   Gsm_get_signal(void);//获取GSM模块信号强度
int   Gsm_get_gprs_state(void);//获得GSM的GPRS的附着状态
bool  Gsm_gprs_comm(char *wbuf, int wlen, char *rbuf, int* rlen,u8 Server_Id);//向服务器发送数据并接收数据
bool  Server_Send_Data(char *wbuf, int wlen,u8 Server_Id);//向指定服务器发送数据


#endif  /*_SIM800C_App_H_*/
