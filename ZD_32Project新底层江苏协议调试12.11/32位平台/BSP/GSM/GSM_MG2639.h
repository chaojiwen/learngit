#ifndef _GSM_MG2639_H   
#define _GSM_MG2639_H	
#include "Gsm_Api.h"
#if (GSM_Module_Type==3)//中兴MG2639模块 

#include <stdbool.h>
#include "GSM_USART.h"
#include "stm32f10x.h"

#define   ON  1  //开
#define   OFF 0  //关

//GSM模块电源控制脚
#define   GSM_MG2639_PWRCTRL_PORT          GPIOB
#define   GSM_MG2639_PWRCTRL_CLK           RCC_APB2Periph_GPIOB
#define   GSM_MG2639_PWRCTRL_PIN           GPIO_Pin_1 
//GSM模块开机控制脚
#define   GSM_MG2639_ONOFF_PORT            GPIOB
#define   GSM_MG2639_ONOFF_CLK             RCC_APB2Periph_GPIOB
#define   GSM_MG2639_ONOFF_PIN             GPIO_Pin_2 
 
/*GSM模块电源控制脚输出控制函数*/		
#define   GSM_MG2639_Power(a)		if (a)	\
					GPIO_SetBits(GSM_MG2639_PWRCTRL_PORT,GSM_MG2639_PWRCTRL_PIN); \
					else		\
					GPIO_ResetBits(GSM_MG2639_PWRCTRL_PORT,GSM_MG2639_PWRCTRL_PIN) 
					
					
/*GSM模块开机控制脚输出控制函数*/		
#define   GSM_MG2639_OnOff(a)		if (a)	\
					GPIO_SetBits(GSM_MG2639_ONOFF_PORT,GSM_MG2639_ONOFF_PIN); \
					else		\
					GPIO_ResetBits(GSM_MG2639_ONOFF_PORT,GSM_MG2639_ONOFF_PIN) 

// 模块响应普通指令等待的时间单位为:10ms
#define   Ordinary_AT_WaitTime_n10ms					50	
					
// 模块响应普通指令等待的时间单位为:10ms
#define   Mg2639_Open_GPRSdatalink_n10ms			  500						

//指令等待的时间单位为:10ms 打开服务器连接
#define   Open_Server_AT_WaitTime_n10ms		    6000

//指令等待的时间单位为:10ms 关闭服务器连接
#define   Close_Server_AT_WaitTime_n10ms	    500	
//指令等待的时间单位为:10ms 发送服务器数据
#define   SendData_Server_AT_WaitTime_n10ms		6000	

//指令等待的时间单位为:10ms 发送短信
#define   Send_Sms_AT_WaitTime_n10ms	        100					
		
void     GSM_MG2639_Io_Init(void);//GSM模块相关IO口初始化					
void     GSM_MG2639_Power_Up(void);//打开GSM模块电源					
void     GSM_MG2639_Power_Down(void);//关闭GSM模块电源
bool     GSM_MG2639_Power_Rest(void);//重启GSM模块
u8       GSM_MG2639_send_data_Status(char *wbuf, int wlen,char *ack,u16 waittime,u8 *UsartBuf);//检测数据是否发送成功
u8       GSM_MG2639_send_data(uint8_t ServerID,char *wbuf, int wlen,u16 waittime,uint8_t *UartBuf);//发送数据函数
bool     GSM_MG2639_AskCSQ(uint8_t *csq, uint8_t *UartBuf);//获取模块信号强度
u8       GSM_MG2639_Open_Connect_Srever(uint8_t ServerID,char *ServerName,char *ServerPort,u16 waittime,uint8_t *UartBuf);//打开TCP服务器连接
bool     GSM_MG2639_Close_Connect_Srever(uint8_t *UartBuf);//关闭服务器连接
bool     GSM_MG2639_check_cmd(char *str,u8 *UsartBuf);//检查Gsm模块返回的数据内容是否正确
bool     GSM_MG2639_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf);//向Gsm模块发送指定的命令,并在规定的时间内检查是否有要返回的目标内容
u8       GSM_MG2639_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf);//接收数据;//接收数据
bool     GSM_MG2639_Check_SIm_Card(uint8_t *UartBuf);//检查SIM是否在位
bool     GSM_MG2639_CheckNetwork(uint8_t *UartBuf);//检查SIM是否已经注册上了网路
bool     GSM_MG2639_CheckGPRSActive(uint8_t *UartBuf);//检查SIM是否已经附着上了网路
bool     GSM_MG2639_Echo(bool flag,u8 *UsartBuf);//是否设置GSM回显 flag:true代表回显 flag:false代表不回显
bool     GSM_MG2639_Send_AT(u8 *UsartBuf);//向模块发送"AT"
bool     GSM_MG2639_Send_Sms(char *num,char *smstext,uint8_t *UartBuf);//发送短信内容
bool     GSM_MG2639_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf);//发送不默认带回车符的AT指令
int	     GSM_MG2639_Adaptive_BaudRate(u8 *UartBuf);  /* 自适应波特率 */
bool	   GSM_MG2639_SetBaudRate(int br,u8 *UartBuf);//设置模块波特率;//设置模块波特率
bool     GSM_MG2639_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf);//向模块发送AT指令
#endif
#endif  /*_GSM_MG2639_H*/
