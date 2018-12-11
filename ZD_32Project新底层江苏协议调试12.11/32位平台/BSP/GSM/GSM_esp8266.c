#include "Gsm_Api.h"
#if (GSM_Module_Type==0x02) //ESP8266Wifi模块
#include "GSM_esp8266.h" //添加gsm模块底层头文件(比如SIM800C,SIM7020C等模块的底层驱动程序)
#include "GSM_USART.h"
#include "ucos_ii.h"
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "String_Utility.h"
#include "GSM_app.h"
/****************************** 辅助函数 ******************************/
void GSM_ESP8266_DelayMS(uint32_t ms)
{
	     OSTimeDly(ms);//延时10ms		
}
/**********************************************************************/


/************************** 绑定模块控制接口 **************************/
//GSM模块相关IO口初始化	
void Gsm_Io_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//GSM模块电路负载开关控制脚
	RCC_APB2PeriphClockCmd(GSM_CIRCUIT_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin		= GSM_CIRCUIT_PIN;
	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GSM_CIRCUIT_PORT, &GPIO_InitStructure);
	GSM_Circuit_Power(OFF); //默认关闭GSM模块电路负载开关
	
	//GSM电源控制脚
	RCC_APB2PeriphClockCmd(GSM_PWRCTRL_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin		= GSM_PWRCTRL_PIN;
	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GSM_PWRCTRL_PORT, &GPIO_InitStructure);
	GSM_Power(ON); //默认关闭GSM模块电源
	
	//GSM开机控制脚
	RCC_APB2PeriphClockCmd(GSM_ONOFF_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin	    = GSM_ONOFF_PIN;
	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GSM_ONOFF_PORT, &GPIO_InitStructure);
	GSM_OnOff(ON); //默认GSM模块处于关机状态
	
	//GSM模块休眠控制脚
	RCC_APB2PeriphClockCmd(GSM_SLEEP_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin	    = GSM_SLEEP_PIN;
	GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GSM_SLEEP_PORT, &GPIO_InitStructure);
	GSM_Sleep(ON); //默认GSM模块处于非休眠状态
}

////////////GSM模块开机和关机
//////////static void Gsm_Boot(void)
//////////{
//////////	GSM_OnOff(OFF);
//////////	GSM_ESP8266_DelayMS(3000);//延时3s
//////////	GSM_OnOff(ON);
//////////}

//打开GSM模块电源	
void Gsm_Power_Up(void)
{
	GSM_Circuit_Power(ON);
//	GSM_ESP8266_DelayMS(300); //延时300ms以便系统电源稳定
//	Gsm_Boot(); //GSM模块开机和关机
}	

//关闭GSM模块电源
void Gsm_Power_Down(void)
{
//	Gsm_Boot();//GSM模块开机和关机
//	GSM_ESP8266_DelayMS(300);//延时300ms以便系统电源稳定
	GSM_Circuit_Power(OFF);
}

//重启GSM模块
bool Gsm_Power_Rest(void)
{
	bool ret;
	ret=true;
	Gsm_Power_Down();//关闭GSM模块电源
	GSM_ESP8266_DelayMS(300);//延时300ms
	Gsm_Power_Up();//打开GSM模块电源
	return ret;
}

//GSM模块进入休眠
void Gsm_Module_Sleep(void)
{
	GSM_Sleep(ON);
	GSM_ESP8266_DelayMS(60);
}

//GSM模块唤醒
void Gsm_Module_WakeUp(void)
{
	GSM_Sleep(OFF);
	GSM_ESP8266_DelayMS(60);
}
/**********************************************************************/


/************************** 绑定模块驱动接口 ***************************/
//设置WIFI模式为Station模式
bool Gsm_SetWifiMode_Station(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CWMODE_DEF=1", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//加入AP网络 AT+CWJAP="SSID","password"
bool Gsm_OpenConnectAP(char *SSID, char *password, uint8_t *UartBuf)
{
	bool ret = false;
	char ap_data[64] = {0};
	
	//仅当非连接状态下去发起连接请求 
//	if(!Gsm_send_cmd("AT+CIPSTATUS", "STATUS:2", Ordinary_AT_WaitTime_n10ms, UartBuf))
//	{	
		sprintf(ap_data, "AT+CWJAP_DEF=\"%s\",\"%s\"", SSID, password); //组装AP网络账号和密码
		if(Gsm_send_cmd(ap_data, "WIFI CONNECTED", Open_AP_AT_WaitTime_n10ms, UartBuf))
			ret = true;
//	}
//	else
//		ret = true;
	return ret;
}

//断开AP网络
bool Gsm_CloseConnectAP(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CWQAP", "OK", Close_AP_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//查询网络连接信息
//Status = 2 (ESP8266 station 已连接AP，获得IP地址)
//Status = 3 (ESP8266 station 已建立TCP或UDP传输)
//Status = 4 (ESP8266 station 断开TCP或UDP网络连接)
//Status = 5 (ESP8266 station 未连接IP)
bool Gsm_CheckStatus(uint8_t *Status, uint8_t *UartBuf)
{
	char *p;
	bool ret = false;

	ret = Gsm_send_cmd("AT+CIPSTATUS", "STATUS", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //没及时返回则判为查询操作失败
	{
		*Status = 0; 
		return false;
	}
	p = strstr((const char*)UartBuf, "STATUS:"); //接收正确则返回后者在前者首次出现的首地址，否则返回NULL
	if(p != NULL)
	{
		ret = true;
		*Status = *(p+7)-0x30; //将字符串对应数字位转为数字
	}
	return ret;
}

//DHCP使能下，IP地址是动态获取的
//Station模式下设置DHCP是否使能
bool Gsm_StationMode_SetDHCP(bool flag, uint8_t *UartBuf)
{
	bool ret = false; 
	if(flag == true)
	{
		if(Gsm_send_cmd("AT+CWDHCP_DEF=1,1", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
			ret = true;
	}
	else
	{
		if(Gsm_send_cmd("AT+CWDHCP_DEF=1,0", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
			ret = true;
	}
	return ret;
}

//!!! 使用前需要先关闭Station模式的DHCP使能
//AT+ CIPSTA_DEF – 设置ESP8266 station的静态IP地址，并保存到flash
//AT+CIPSTA_DEF="192.168.6.100","192.168.6.1","255.255.255.0"
//<ip> ESP8266 station的IP地址
//<gateway> 网关
//<netmask> 子网掩码
bool Gsm_StationMode_SetStaticIP(char *ip, char *gateway, char* netmask, uint8_t *UartBuf)
{
	bool ret = false;
	char ip_data[64] = {0};
	
	sprintf(ip_data, "AT+CIPSTA_DEF=\"%s\",\"%s\",\"%s\"", ip, gateway, netmask); //组装AP网络账号和密码
	if(Gsm_send_cmd(ip_data, "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//建立单连接
bool Gsm_SetSingleConnect(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CIPMUX=0", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//设置普通传输模式（非透传）
bool Gsm_SetTransmittionMode(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CIPMODE=0", "OK", Ordinary_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}

//查询本地IP地址(Station模式下)
bool Gsm_CheckLocalIPAddress(char *station_ip, uint8_t *UartBuf)
{
	bool ret = false;
	uint16_t index_start = 0, index_end = 0;
	
	ret = Gsm_send_cmd("AT+CIFSR", "+CIFSR:", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //没及时返回则判为查询操作失败
	{
		return false;
	}
	
	index_start = String_KMPMatch((char*)UartBuf, "\""); //第一个引号的位置
	index_end = String_KMPMatch(((char*)UartBuf+index_start+1), "\"") + index_start + 1; //最后一个引号的位置
	memcpy(station_ip, (const char*)(UartBuf+index_start+1), index_end-index_start-1); //取出IP字符串

	return ret;
}

//查询已经设置station模式下的静态IP地址等参数
bool Gsm_CheckCipStatus(char *station_ip, char *gateway, char *netmask, uint8_t *UartBuf)
{
	bool ret = false;
	uint16_t index_start_ip = 0, index_end_ip = 0;
	uint16_t index_start_gateway = 0, index_end_gateway = 0;
	uint16_t index_start_netmask = 0, index_end_netmask = 0;
	
	ret = Gsm_send_cmd("AT+CIPSTA_DEF?", "+CIPSTA_DEF:", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //没及时返回则判为查询操作失败
	{ 
		return false;
	}
	
	index_start_ip = String_KMPMatch((char*)UartBuf, "\""); //第一个引号的位置
	index_end_ip = String_KMPMatch(((char*)UartBuf+index_start_ip+1), "\"") + index_start_ip + 1; //第二个引号的位置(加1是因为检索起始地址加了1)
	memcpy(station_ip, (const char*)(UartBuf+index_start_ip+1), index_end_ip-index_start_ip-1); //取出IP字符串
	
	index_start_gateway = String_KMPMatch(((char*)UartBuf+index_end_ip+1), "\"") + index_end_ip + 1; //第三个引号的位置(加1是因为检索起始地址加了1)
	index_end_gateway = String_KMPMatch(((char*)UartBuf+index_start_gateway+1), "\"") + index_start_gateway + 1; //第四个引号的位置(加1是因为检索起始地址加了1)
	memcpy(gateway, (const char*)(UartBuf+index_start_gateway+1), index_end_gateway-index_start_gateway-1); //取出网关字符串
	
	index_start_netmask = String_KMPMatch(((char*)UartBuf+index_end_gateway+1), "\"") + index_end_gateway + 1; //第五个引号的位置(加1是因为检索起始地址加了1)
	index_end_netmask = String_KMPMatch(((char*)UartBuf+index_start_netmask+1), "\"") + index_start_netmask + 1; //第六个引号的位置(加1是因为检索起始地址加了1)
	memcpy(netmask, (const char*)(UartBuf+index_start_netmask+1), index_end_netmask-index_start_netmask-1); //取出子网掩码字符串
	
	return ret;
}

bool Gsm_AskRssi(int *rssi, uint8_t *UartBuf)
{
	bool ret = false;
	uint16_t index_start = 0, index_end = 0;
	char str_rssi[6] = "";
	
	ret = Gsm_send_cmd("AT+CWJAP?", "+CWJAP:", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //没及时返回则判为查询操作失败
	{ 
		return false;
	}
	index_start = String_KMPMatch((char*)UartBuf, ","); //第一个逗号的位置
	
	index_start = String_KMPMatch(((char*)UartBuf+index_start+1), ",")+index_start+1; //第二个逗号的位置(加1是因为检索起始地址加了1)
	
	index_start = String_KMPMatch(((char*)UartBuf+index_start+1), ",")+index_start+1; //第三个逗号的位置(加1是因为检索起始地址加了1)
	
	index_end = String_KMPMatch(((char*)UartBuf+index_start+1), "\r\n")+index_start+1; //紧接着的回车换行结束
	
	//取出RSSI字符串
	memcpy(str_rssi, (const char*)(UartBuf+index_start+1), index_end-index_start-1); 
	*rssi = atoi(str_rssi); //转为数字
	

	return ret;
}

//查询MAC地址
bool Gsm_AskStationMacAddress(char *MacAddress, uint8_t *UartBuf)
{
	bool ret = false;
	uint16_t index_start = 0, index_end = 0;
	
	ret = Gsm_send_cmd("AT+CIPSTAMAC?", "+CIPSTAMAC:", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //没及时返回则判为查询操作失败
	{ 
		return false;
	}
	index_start = String_KMPMatch((char*)UartBuf, "\""); //第一个引号的位置
	
	index_end = String_KMPMatch(((char*)UartBuf+index_start+1), "\"")+index_start+1; //第二个引号的位置(加1是因为检索起始地址加了1)
	
	//取出Mac地址
	memcpy(MacAddress, (const char*)(UartBuf+index_start+1), index_end-index_start-1); 
	
	return ret;
}



//是否设置GSM回显 flag:true代表回显 flag:false代表不回显
bool Gsm_Echo(bool flag, u8 *UsartBuf)
{
	bool  ret=false; 
	if(flag == true)
	{
		if( Gsm_send_cmd("ATE1", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
			ret = true;   
	}
	else
	{
		if( Gsm_send_cmd("ATE0", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
			ret = true;    
	}
	return ret;
}

//向模块发送"AT"
bool Gsm_Send_AT(u8 *UsartBuf)
{
	bool ret = false; 

	if( Gsm_send_cmd("AT", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	return ret;
}

//恢复出厂设置
bool Gsm_Restore(u8 *UsartBuf)
{
	bool ret = false; 

	if( Gsm_send_cmd("AT+RESTORE", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	return ret;
}

//进入浅睡眠模式
//bool flag : 是否进入浅睡眠模式的标志
bool Gsm_Into_LightSleep(bool flag, u8 *UsartBuf)
{
	bool ret = false; 

	if(flag == true)
	{
		//light sleep模式
		if( Gsm_send_cmd("AT+SLEEP=1", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
			ret=true;   
	}
	else
	{
		//modem sleep模式 即平时使用的
		if( Gsm_send_cmd("AT+SLEEP=2", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
			ret=true;
	}	
	return ret;
}

//设置唤醒模块浅睡眠的引脚
bool Gsm_Set_LightSleepWakeUpPin(uint8_t pin, uint8_t level, u8 *UsartBuf)
{
	bool ret = false; 
	char cmd[20] = {0};
	
	sprintf(cmd, "AT+WAKEUPGPIO=1,%d,%d", pin, level);
	if( Gsm_send_cmd(cmd, "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	return ret;
}

//查询DHCP状态(station模式下)
bool Gsm_AskDHCP(bool *flag, u8 *UsartBuf)
{
	//返回 +CWDHCP_DEF:3 
	//bit0=0为AP模式DHCP关闭,bit0=1为AP模式DHCP开启
	//bit1=0为Station模式DHCP关闭,bit1=1为Station模式DHCP开启
	bool ret = false; 
	char cmd[20] = "AT+CWDHCP_DEF?";
	uint16_t index_start = 0;
	uint8_t DHCP_Status = 0;
	
	if( Gsm_send_cmd(cmd, "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	index_start = String_KMPMatch((char*)UsartBuf, ":");
	DHCP_Status = UsartBuf[index_start + 1] - 0x30;
	if((DHCP_Status&(1<<1)) == 0x02) //判断Station位的标志
		*flag = true;
	else if((DHCP_Status&(1<<1)) == 0x00)
		*flag = false;
	
	return ret;
}

//重启Wifi模块
bool Gsm_Rest(u8 *UsartBuf)
{
	bool ret = false; 

	if( Gsm_send_cmd("AT+RST", "OK", Ordinary_AT_WaitTime_n10ms, (u8*)UsartBuf))
		ret=true;   
	return ret;
}




/**********************************************************************/


/**************************** 绑定通信接口 *****************************/
//作为Client建立TCP连接 AT+CIPSTART="TCP","192.168.3.116",8080
bool Gsm_ClientConnectTCP(char *server, char *port, uint8_t *UartBuf)
{
	bool ret = false;
	char ap_data[64] = {0};
	
	//仅当非连接状态下去发起连接请求
	if(!Gsm_send_cmd("AT+CIPSTATUS", "STATUS:3", Ordinary_AT_WaitTime_n10ms, UartBuf))
	{  
		sprintf(ap_data, "AT+CIPSTART=\"TCP\",\"%s\",%s", server, port); //组装AP网络账号和密码
		
		if(Gsm_send_cmd(ap_data, "CONNECT", Open_Server_AT_WaitTime_n10ms, UartBuf))
			ret = true;
	}
	else
		ret = true;
	return ret;
}

//断开TCP连接
bool Gsm_ClientCloseTCP(uint8_t *UartBuf)
{
	bool ret = false; 
	if(Gsm_send_cmd("AT+CIPCLOSE", "OK", Close_Server_AT_WaitTime_n10ms, UartBuf))
		ret = true;
	return ret;
}	

/************************************************************************************************
*函数名   ：u8* Gsm_check_cmd(char *str,u8 *UsartBuf)															
*输入参数 ：str :要寻找的目标字符串											
*输出参数 ：	  																				
*作用     ：查找特征字符串 	  
*创建日期 ： 2018/08/23	
*说明     ： //发送命令后,检测接收到的应答
//str:期待的应答结果  返回值:0,没有得到期待的应答结果 其他,期待应答结果的位置(str的位置)
************************************************************************************************/
bool  Gsm_check_cmd(char *str,u8 *UsartBuf)
{
	int ret=0;
	
	ret = String_KMPMatch((char*)UsartBuf, str);
	if(ret==-1)
	{
		return false;
	}
	return true;
}

/************************************************************************************************
*函数名   ：u8 Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)											
*输入参数 ：cmd:要发送的AT指令字符串	ack：要匹配的特征字符串 waittime:匹配特征字符串需要等待的时间 单位10ms										
*输出参数 ：命令执行成功返回 1   失败返回 0  																				
*作用     ：发送AT指令 	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
u8 Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
{
	u8 res=1;
	Clear_GSM_RX_BUF(UsartBuf);//清空缓存数组
	SIMCOM_Printf("%s\r\n", cmd);//发送字符命令
	if(cmd == NULL)//如果没有特征字符串匹配的话函数直接返回成功，不执行字符串匹配检测
	{
		return res=0;
	}
	else //有字符串需要匹配检测
	{ 
		if(ack&&waittime)		//需要等待应答
		{
			while(--waittime)	//等待倒计时
			{ 
				//查找是否会有ERROR这个特征字符串,如果有的话直接退出等待检测
				if(Gsm_check_cmd("ERROR",UsartBuf)) //TCP连接失败会返回该指令
				{
					res=0;
					break;//得到有效数据 			 
				}
//////				if(Gsm_check_cmd("CLOSED",UsartBuf)) //TCP连接失败会返回该指令
//////				{
//////					res=0;
//////					break;//得到有效数据 			 
//////				}
				if(Gsm_check_cmd("FAIL",UsartBuf)) //加入AP网路失败会返回该指令
				{
					res=0;
					break;//得到有效数据 			 
				}
////////				if(Gsm_check_cmd("WIFI DISCONNECT",UsartBuf)) //加入AP网路失败会返回该指令
//////// 				{
////////					res=0;
////////					break;//得到有效数据 			 
////////				}	
				if(Gsm_check_cmd("No AP",UsartBuf)) //未加入AP网络会返回该指令
 				{
					res=0;
					break;//得到有效数据 			 
				}
				if(Gsm_check_cmd(ack, UsartBuf))
				{
					break;//得到有效数据 			 
				}
				GSM_ESP8266_DelayMS(10); //延时10ms					 
			}
			if(waittime==0)
			{
				res=0; 
			}
		}
	}
	return res;
}

u8 Gsm_send_data_Status(char *wbuf, int wlen, char *ack, u16 waittime, u8 *UsartBuf)
{
	u8 res=1;
	Clear_GSM_RX_BUF(UsartBuf);//清空缓存数组
	Usart_SendStr_length(USART3,wbuf,wlen);//发送数据内容
	waittime=waittime*10;
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{ 	
			if(Gsm_check_cmd(ack, UsartBuf))
			{
				break;//得到有效数据 			 
			}
			GSM_ESP8266_DelayMS(1); //延时1ms
		}
		if(waittime==0)
		{
			res=0; 
		}
	}
	return res;
}

//发送数据
u8 Gsm_send_data(char *wbuf, int wlen, u16 waittime, uint8_t *UartBuf)
{
	u8 res=1;
	char  cmd[32]={0};	
	sprintf(cmd,"AT+CIPSEND=%d",wlen);
	if(Gsm_send_cmd(cmd, ">", Ordinary_AT_WaitTime_n10ms, UartBuf)) //0.5s
	{
		if(Gsm_send_data_Status(wbuf, wlen, "SEND OK", waittime, UartBuf))
		{
			res=1;
		}
		else
		{
			res=0;
		}
	}
	else
		res=0;
	return res;
}

//////////检查接收到的数据是否合法
//////////Location1_0x7E:0x7E第一次出现的位置;Location2_0x7E:0x7E第二次出现的位置;
////////static bool  Gsm_check_Rev_Data(int *Location1_0x7E, int *Location2_0x7E,int rlen,u8 *UsartBuf)
////////{
////////	bool  ret=false; 
////////	int i=0;

////////	for(i=0;i<rlen;i++)//查找第一次0x7E出现的位置
////////	{
////////		if(UsartBuf[i]==0x7E)
////////		{
////////			*Location1_0x7E=i;
////////			ret=true;
////////			break;
////////		}
////////		else
////////		{
////////			ret=false;
////////		}
////////	}

////////	if(ret)
////////	{
////////		for(i=*Location1_0x7E;i<rlen;i++)//查找第二次次0x7E出现的位置
////////		{
////////			if(UsartBuf[i+1]==0x7E)
////////			{
////////				*Location2_0x7E=i+1;
////////				ret=true;
////////				break;
////////			}
////////			else
////////			{
////////				ret=false;
////////			}
////////		}
////////	}
////////	else
////////	{
////////		ret=false;
////////	}
////////	return ret;
////////}

//////////接收数据
////////u8 Gsm_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf)
////////{
////////	u8 res=1;
////////	int Location1=0,Location2=0;//Location1:0x7E第一次出现的位置;Location2:0x7E第二次出现的位置;
////////	
////////	if(waittime)		//需要等待应答
////////	{
////////		while(--waittime)	//等待倒计时
////////		{ 
////////			/*检查接收到的数据是否合法*/
////////			if(Gsm_check_Rev_Data(&Location1,&Location2,GSM_RX_BUF_LEN,UartBuf))
////////			{
////////				*rlen=(Location2-Location1)+1;
////////				memcpy(rbuf,&UartBuf[Location1],*rlen);//拷贝数据
////////				break;//得到有效数据 			 
////////			}
////////			GSM_ESP8266_DelayMS(10); //延时10ms
////////		}
////////		if(waittime==0)
////////		{
////////			res=0; 
////////		}
////////	}
////////	return res;
////////} 
static bool Gsm_RevData_Check(char *rbuf, uint16_t rlen)
{
	if((rbuf[0] == 0x7e) && (rbuf[rlen-1] == 0x7e))
	{
		return true;
	}
	else
		return false;
}

//bool Gsm_ReceiveData(char *rbuf, int *rlen, uint16_t UartRevLength, uint8_t *UartBuf)
//{
//	//下列参数与分包参数相同，为了和分包接收程序保持一致
//	int Mark_Buf[3] = {0}; //记录特征字符串"+IPD,"每个出现在缓冲区中的索引位置
//	int Mark_Cnt = 0; //记录特征字符串"+IPD,"出现次数
//	int Mark_Colon_Buf[3] = {0}; //记录特征字符串":"每个出现在缓冲区中的索引位置
//	char SubPackageLength[3][6] = { {0}, {0}, {0}}; //记录每个分包的数据长度 (字符串形式)
//	int SubLength[3] = {0}; //记录每个分包的数据长度(数字形式)
//	int length_1=0;
//////////////	int length_1=0, length_2=0, length_3=0;
//	
//	//寻找接收标志符
//	Mark_Cnt = Find_SubStringEx((char*)UartBuf, "+IPD,", UartRevLength, Mark_Buf);
//	if(Mark_Cnt == 0) //注意，程序bin文件里会含有+IPD，故不能采用分包形式判别+IPD
//	{

//		return false;
//	}
//	
//	//寻找数据长度结束符
//	if(Find_SubStringEx((char*)UartBuf+Mark_Buf[0], ":", 5, Mark_Colon_Buf) == 1)
//		length_1 = Mark_Colon_Buf[0] - 1;
//	
//	//转换得到接收数据的长度
//	memcpy(&(SubPackageLength[0]), (char*)UartBuf+Mark_Buf[0], length_1);
//	SubLength[0] = atoi(SubPackageLength[0]);
//	*rlen = SubLength[0];

//	
//	//将串口缓存区数据放到处理区中
//	memcpy(rbuf, (char*)UartBuf+Mark_Buf[0]+length_1+1, SubLength[0]);
//	
//	return true;
//}
//另外一个接收GSM模块从服务器获取的数据子函数
////bool Gsm_ReceiveDataChild(char *rbuf, int *rlen, uint16_t UartRevLength, uint8_t *UartBuf)
bool Gsm_ReceiveData(char *rbuf, int *rlen, uint16_t UartRevLength, uint8_t *UartBuf)

{
	//下列参数与分包参数相同，为了和分包接收程序保持一致
	int Mark_Buf[10] = {0}; //记录特征字符串"+IPD,"每个出现在缓冲区中的索引位置
	int Mark_Cnt = 0; //记录特征字符串"+IPD,"出现次数
	int Mark_Colon_Buf[10] = {0}; //记录特征字符串":"每个出现在缓冲区中的索引位置
	char SubPackageLength[10][6] = { {0}, {0}, {0}}; //记录每个分包的数据长度 (字符串形式)
	int SubLength[10] = {0}; //记录每个分包的数据长度(数字形式)
	int length_1[10]={0},i=0;
////////////	int length_1=0, length_2=0, length_3=0;
	  *rlen=0;
	//寻找接收标志符
	Mark_Cnt = Find_SubStringEx((char*)UartBuf, "+IPD,", UartRevLength, Mark_Buf);
	if(Mark_Cnt == 0) //注意，程序bin文件里会含有+IPD，故不能采用分包形式判别+IPD
	{
		return false;
	}
	
	for(i=0;i<Mark_Cnt;i++)
	{
	  	//寻找数据长度结束符
	  if(Find_SubStringEx((char*)UartBuf+Mark_Buf[i], ":", 5, Mark_Colon_Buf) == 1)
		{
		   length_1[i]= Mark_Colon_Buf[0] - 1;
				  //转换得到接收数据的长度
	     memcpy(&(SubPackageLength[i]), (char*)UartBuf+Mark_Buf[i], length_1[i]);
	     SubLength[i] = atoi(SubPackageLength[i]);
	     *rlen += SubLength[i];

			if(i==0)
			{
			  //将串口缓存区数据放到处理区中
	     memcpy(rbuf, (char*)UartBuf+Mark_Buf[i]+Mark_Colon_Buf[0], SubLength[i]);
			}
			else
			{
			  //将串口缓存区数据放到处理区中
	     memcpy((char*)rbuf+SubLength[0], (char*)UartBuf+Mark_Buf[i]+Mark_Colon_Buf[0], SubLength[i]);
			
			}
			
		}
	  	
	}
	return true;
}



//接收数据
u8 Gsm_Rev_data(char *rbuf, int *rlen, u16 waittime, uint8_t *UartBuf)
{
	u8 res=1;
	
	if(waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			/*检查接收到的数据是否合法*/
			if(Gsm_ReceiveData(rbuf, rlen, GSM_RX_BUF_LEN, UartBuf))
			{
				if(Gsm_RevData_Check(rbuf, *rlen))
				{
					break;
				}
			}
			GSM_ESP8266_DelayMS(10); //延时10ms
		}
		if(waittime==0)
		{
			res=0; 
		}
	}
	return res;
} 






#endif
/**********************************************************************/

