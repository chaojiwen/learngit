#ifndef _GSM_ESP8266_H   
#define _GSM_ESP8266_H	

#include <stdbool.h>
#include "GSM_USART.h"
#include "stm32f10x.h"

#define   ON  1  //开
#define   OFF 0  //关

//GSM模块电路负载开关控制脚
#define   GSM_CIRCUIT_CLK			RCC_APB2Periph_GPIOB
#define   GSM_CIRCUIT_PORT			GPIOB
#define   GSM_CIRCUIT_PIN			GPIO_Pin_1 

//GSM模块电源控制脚
#define   GSM_PWRCTRL_CLK           RCC_APB2Periph_GPIOC
#define   GSM_PWRCTRL_PORT          GPIOC
#define   GSM_PWRCTRL_PIN           GPIO_Pin_15

//GSM模块开机控制脚 (PWRKEY)
#define   GSM_ONOFF_CLK             RCC_APB2Periph_GPIOB
#define   GSM_ONOFF_PORT            GPIOB
#define   GSM_ONOFF_PIN             GPIO_Pin_2 
 
//GSM模块休眠控制脚
#define   GSM_SLEEP_CLK             RCC_APB2Periph_GPIOD
#define   GSM_SLEEP_PORT            GPIOD
#define   GSM_SLEEP_PIN             GPIO_Pin_9 

//GSM模块电路负载开关控制脚输出控制函数	
#define   GSM_Circuit_Power(a)		if (a)	\
					GPIO_SetBits(GSM_CIRCUIT_PORT,GSM_CIRCUIT_PIN); \
					else		\
					GPIO_ResetBits(GSM_CIRCUIT_PORT,GSM_CIRCUIT_PIN) 
					
					
//GSM模块电源控制脚输出控制函数	
#define   GSM_Power(a)		if (a)	\
					GPIO_SetBits(GSM_PWRCTRL_PORT, GSM_PWRCTRL_PIN); \
					else		\
					GPIO_ResetBits(GSM_PWRCTRL_PORT, GSM_PWRCTRL_PIN) 
					
					
//GSM模块开机控制脚输出控制函数		
#define   GSM_OnOff(a)		if (a)	\
					GPIO_SetBits(GSM_ONOFF_PORT, GSM_ONOFF_PIN); \
					else		\
					GPIO_ResetBits(GSM_ONOFF_PORT, GSM_ONOFF_PIN) 

//GSM模块休眠控制脚输出控制函数	
#define   GSM_Sleep(a)		if (a)	\
					GPIO_SetBits(GSM_SLEEP_PORT, GSM_SLEEP_PIN); \
					else		\
					GPIO_ResetBits(GSM_SLEEP_PORT, GSM_SLEEP_PIN) 
					
					
//模块响应普通指令等待的时间单位为:10ms
#define   Ordinary_AT_WaitTime_n10ms		    100	

//指令等待的时间单位为:10ms 连接AP网络 
#define   Open_AP_AT_WaitTime_n10ms				2000	//最长等待20秒			

//指令等待的时间单位为:10ms 断开AP网络 
#define   Close_AP_AT_WaitTime_n10ms			400		//最长等待4秒	
					
//指令等待的时间单位为:10ms 打开TCP服务器连接
#define   Open_Server_AT_WaitTime_n10ms		    600		//最长等待6秒

//指令等待的时间单位为:10ms 关闭TCP服务器连接
#define   Close_Server_AT_WaitTime_n10ms	    600		//最长等待6秒
					
//指令等待的时间单位为:10ms 发送服务器数据
#define   SendData_Server_AT_WaitTime_n10ms		600					


/****************************** 辅助函数 ******************************/
void GSM_ESP8266_DelayMS(uint32_t ms);

/************************** 绑定模块控制接口 **************************/
void Gsm_Io_Init(void); //GSM模块相关IO口初始化
void Gsm_Power_Up(void); //打开GSM模块电源	
void Gsm_Power_Down(void); //关闭GSM模块电源
bool Gsm_Power_Rest(void); //重启GSM模块
void Gsm_Module_Sleep(void); //GSM模块进入休眠
void Gsm_Module_WakeUp(void); //GSM模块唤醒

/************************** 绑定模块驱动接口 ***************************/
bool Gsm_SetWifiMode_Station(uint8_t *UartBuf); //设置WIFI模式为Station模式
bool Gsm_OpenConnectAP(char *SSID, char *password, uint8_t *UartBuf); //加入AP网络
bool Gsm_CloseConnectAP(uint8_t *UartBuf); //断开AP网络
bool Gsm_CheckStatus(uint8_t *Status, uint8_t *UartBuf); //查询网络连接信息
bool Gsm_StationMode_SetDHCP(bool flag, uint8_t *UartBuf); //Station模式下设置DHCP是否使能
bool Gsm_StationMode_SetStaticIP(char *ip, char *gateway, char* netmask, uint8_t *UartBuf); //设置station模式静态IP等参数
bool Gsm_CheckCipStatus(char *station_ip, char *gateway, char *netmask, uint8_t *UartBuf); //查询已经设置station模式下的静态IP地址等参数
bool Gsm_SetSingleConnect(uint8_t *UartBuf); //建立单连接
bool Gsm_SetTransmittionMode(uint8_t *UartBuf); //设置普通传输模式（非透传）
bool Gsm_CheckLocalIPAddress(char *station_ip, uint8_t *UartBuf); //查询本地IP地址(Station模式下)
bool Gsm_AskRssi(int *rssi, uint8_t *UartBuf); //查询信号强度RSSI
bool Gsm_AskStationMacAddress(char *MacAddress, uint8_t *UartBuf); //查询MAC地址
bool Gsm_Echo(bool flag, u8 *UsartBuf); //是否设置GSM回显
bool Gsm_Send_AT(u8 *UsartBuf); //向模块发送"AT"
bool Gsm_Restore(u8 *UsartBuf);
bool Gsm_Into_LightSleep(bool flag, u8 *UsartBuf);
bool Gsm_Set_LightSleepWakeUpPin(uint8_t pin, uint8_t level, u8 *UsartBuf);
bool Gsm_AskDHCP(bool *flag, u8 *UsartBuf);
bool Gsm_Rest(u8 *UsartBuf);//重启Wifi模块



/**************************** 绑定通信接口 *****************************/
bool Gsm_ClientConnectTCP(char *server, char *port, uint8_t *UartBuf); //作为Client建立TCP连接
bool Gsm_ClientCloseTCP(uint8_t *UartBuf); //断开TCP连接
bool  Gsm_check_cmd(char *str,u8 *UsartBuf); //查找特征字符串 
u8 Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf); //发送AT指令
u8 Gsm_send_data_Status(char *wbuf, int wlen, char *ack, u16 waittime, u8 *UsartBuf); 
u8 Gsm_send_data(char *wbuf, int wlen, u16 waittime, uint8_t *UartBuf); //发送数据
u8 Gsm_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf); //接收数据

#endif  


























