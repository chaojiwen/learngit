#ifndef _GSM_API_H   
#define _GSM_API_H	
#include <stdbool.h>
#include "stm32f10x.h"
/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>

//	<o> 相应的通用模块选择
//  <0=> [SIM800C]模块.
//  <1=> [SIM7020C]模块.
//  <2=> [ESP8266Wifi]模块.
//  <3=> [中兴MG2639]模块.
//  <4=> [待定1]模块.
//  <5=> [待定2]模块.
#define  GSM_Module_Type			       (0x03)
//	<<< end of configuration section >>>	

//GSM通信模块句柄
typedef struct
{
		//绑定模块控制接口
	 void  (*pGSM_IO_Init)           (void);//GSM模块IO初始化
	 void  (*pGSM_Power_Up)          (void);//打开GSM模块电源
	 void  (*pGSM_Power_Down)        (void);//关闭GSM模块电源	
	 void  (*pGSM_Usart_Init)        (u32 bound);//GSM模块串口初始化
	 void  (*pGSM_Sleep)             (void); //GSM模块进入睡眠
	 void  (*pGSM_WakeUp)            (void); //GSM模块唤醒
	 bool  (*pGSM_Power_Rest)        (void);//重启GSM模块

	//绑定模块驱动接口
	 struct sim800c	 
	 {
	  bool  (*pGSM_Echo)             (bool flag);//设置GSM模块是否回显
	  bool  (*pGSM_Send_AT)          (void);//向模块发送"AT"
	  bool  (*pGSM_CSQ)              (uint8_t *csq); //获取GSM模块的信号强度
	  bool  (*pGSM_CheckNetwork)     (void);//检查SIM卡是否注册上了网络
	  bool  (*pGSM_CheckGPRSActive)  (void);//检查SIM卡是否附着上了GPRS网络
	  bool  (*pGSM_Check_Sim_Card)   (void);//检查SIM卡是否在位
		bool  (*pGSM_Send_Sms)(char *num,char *smstext);//发送短信内容数据
	 }sim800c;
	 	//绑定模块驱动接口
	 struct mg2639	 
	 {
	  bool  (*pGSM_Echo)             (bool flag);//设置GSM模块是否回显
	  bool  (*pGSM_Send_AT)          (void);//向模块发送"AT"
	  bool  (*pGSM_send_cmd)          (char *cmd,char *ack,u16 waittime);//向GSM模块发送AT指令
	  bool  (*pGSM_CSQ)              (uint8_t *csq); //获取GSM模块的信号强度
		int	  (*GSM_Adaptive_BaudRate) (void);  /* 自适应波特率 */
		bool	   (*GSM_SetBaudRate)       (int br);//设置模块波特率
	  bool  (*pGSM_CheckNetwork)     (void);//检查SIM卡是否注册上了网络
	  bool  (*pGSM_CheckGPRSActive)  (void);//检查SIM卡是否附着上了GPRS网络
	  bool  (*pGSM_Check_Sim_Card)   (void);//检查SIM卡是否在位
	  bool  (*pGSM_Open_Connect)      (uint8_t ServerID,char *ServerName,char *ServerPort);//打开TCP服务器连接
		bool  (*pGSM_SendData)          (uint8_t ServerID,char *wbuf, int wlen);//向服务器发送数据
		bool  (*pGSM_Send_Sms)(char *num,char *smstext);//发送短信内容数据	 
	 }mg2639;
	 	//绑定模块驱动接口
	struct esp8266
	{
		bool  (*pGSM_Echo)(bool flag); //设置GSM模块是否回显
		bool  (*pGSM_Send_AT)(void); //向模块发送"AT"
		bool  (*pGSM_CSQ)(uint8_t *csq); //获取GSM模块的信号强度
		bool  (*pGSM_CheckNetwork)(void); //检查SIM卡是否注册上了网络
		bool  (*pGSM_SetWifiMode)(void); //设置WIFI模式
		bool  (*pGSM_OpenConnectAP)(char *SSID, char *password); //加入AP网络
		bool  (*pGSM_CloseConnectAP)(void); //断开AP网络
		bool  (*pGSM_CheckNetStatus)(uint8_t *Status); //查询网络连接信息
		bool  (*pGSM_SetDHCP)(bool flag); //设置IP地址是否动态获取
		bool  (*pGSM_SetStaticIP)(char *ip, char *gateway, char* netmask); //设置静态IP
		bool  (*pGSM_SetSingleConnect)(void); //建立单连接
		bool  (*pGSM_SetTransmittionMode)(void); //设置传输模式（目前为非透传）
		bool  (*pGSM_AskLocalIPAddress)(char *station_ip); //查询本地IP地址(Station模式下)
		bool  (*pGSM_AskCipStatus)(char *station_ip, char *gateway, char *netmask); //查询已经设置station模式下的静态IP地址等参数
		bool  (*pGSM_AskMacAddress)(char *MacAddress); //查询MAC地址
		bool  (*pGSM_Restore)(void); //恢复出厂设置
		bool  (*pGSM_LightSleep)(bool flag); //进入浅睡眠模式
		bool  (*pGSM_SetLightSleepWakeUpPin)(uint8_t pin, uint8_t level); //设置唤醒模块浅睡眠的引脚
		bool  (*pGSM_AskDHCP)(bool *flag); //查询DHCP状态(Station模式下)
	  bool  (*pGSM_Rest)(void); //重启Wifi模块
	}esp8266; 
	//绑定通信接口
   bool  (*pGSM_send_cmd)          (char *cmd,char *ack,u16 waittime);//向GSM模块发送AT指令
	 bool  (*pGSM_Open_Connect)      (char *ServerName,char *ServerPort);//打开TCP服务器连接
	 bool  (*pGSM_CloseConnect)      (void);//关闭TCP连接
	 bool  (*pGSM_SendData)          (char *wbuf, int wlen);//向服务器发送数据
	 bool  (*pGSM_RevData)           (char *rbuf, int *rlen);//向服务器发送数据 
}GSM_HANDLE;


bool   GSM_Transmittion_Init(GSM_HANDLE *pHandle);//GSM模块应用层与底层接口初始化函数
	
extern  GSM_HANDLE   gsm_handle; //定义GSM通讯句柄

#endif /*_GSMAPI_H_*/
