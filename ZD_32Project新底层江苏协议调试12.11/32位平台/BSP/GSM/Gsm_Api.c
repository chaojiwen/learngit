#include "Gsm_Api.h"
GSM_HANDLE  gsm_handle; //定义GSM通讯句柄
#if (GSM_Module_Type==0) //SIM800C模块
#include "GSM.h" //添加gsm模块底层头文件(比如SIM800C,SIM7020C等模块的底层驱动程序)
static bool GSM_AskCSQ(uint8_t *csq)//获取信号强度
{
	 bool  ret=false;
	if(Gsm_AskCSQ(csq,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
  return ret;
}
static bool GSM_Open_Connect(char *ServerName,char *ServerPort)//打开TCP服务器连接
{
    bool  ret=false;   

  if(Gsm_Open_Connect_Srever(ServerName,ServerPort,Open_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;
	 
}
static bool GSM_Close_Connect(void)//关闭TCP服务器连接
{
    bool  ret=false;   

  if(Gsm_Close_Connect_Srever((u8*)GSM_RX_BUF))
	{
	     ret=true;
	 }
    return ret;
}
static bool GSM_Send_Data(char *wbuf, int wlen)//发送数据
{
   bool  ret=false;    

  if(Gsm_send_data(wbuf,wlen,SendData_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	 }
    return ret;

}
static bool GSM_Rev_Data(char *rbuf, int *rlen)//接收数据
{
   bool  ret=false;    
	
  if(Gsm_Rev_data(rbuf,rlen,SendData_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	 }
    return ret;

}
//发送短信（支持中英文,中文为GBK码）
/**
 * @brief  gsm_sms 发送短信（支持中英文,中文为GBK码）
 * @param  num: 电话号码，smstext：短信内容
 * @retval 无
 */
static bool GSM_Send_Sms(char *num,char *smstext)//发送短信内容数据
{
    bool  ret=false;
	if(Gsm_Send_Sms(num,smstext,(u8*)GSM_RX_BUF))
	{
	    ret=true;
	}

return   ret;


}
static void GSM_IO_Init(void)//GSM模块IO初始化
{
     Gsm_Io_Init();
}
static void GSM_Power_Down(void)//关闭GSM模块电源
{
     Gsm_Power_Down();//关闭GSM模块电源
    
}
static void GSM_Power_Up(void)//打开GSM模块电源	
{
     Gsm_Power_Up();//打开GSM模块电源	  
}
//GSM模块进入休眠
static void GSM_Module_Sleep(void)
{
//	Gsm_Module_Sleep();
}

//GSM模块唤醒
static void GSM_Module_WakeUp(void)
{
//	Gsm_Module_WakeUp();
}


static void GSM_Usart_Init(u32 bound)//GSM模块串口初始化
{
	   GSM_USART_Init(bound);//GSM模块串口初始化
}
static bool GSM_Check_Sim_Card(void)//检查SIM卡是否在位
{
	  bool  ret=false;    
  if(Gsm_Check_SIm_Card((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;
}

static bool GSM_CheckNetwork(void)//检查SIM卡是否已经注册上了网络
{
	 bool  ret=false;    
  if(Gsm_CheckNetwork((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;

}
static bool GSM_CheckGPRSActive(void)//检查SIM卡是否已经附着上了GPRS网络
{
	 bool  ret=false;    
  if(Gsm_CheckGPRSActive((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;

}
static bool GSM_Power_Rest(void)//重启GSM模块
{	 
	  bool  ret; 
	  ret=true; 
    Gsm_Power_Rest();//重启GSM模块  //后期可以根据检测GSM模块的开机叫来判断重启模块是否成功
	 return ret;
}
static bool GSM_send_cmd(char *cmd,char *ack,u16 waittime)
{
    bool  ret=false; 
   if(Gsm_send_cmd(cmd,ack,waittime,(u8*)GSM_RX_BUF))
	 {
	     ret=true;
	 }
    return ret;


}

static bool GSM_Echo(bool flag)//是否设置GSM回显 flag:true代表回显 flag:false代表不回显
{	 
	  bool  ret=false; 
	if(flag==true)
	{
		 if(Gsm_Echo(true,(u8*)GSM_RX_BUF))      ret=true;   
	  
	}
	else
	{
	    if(Gsm_Echo(false,(u8*)GSM_RX_BUF))     ret=true;   
	
	}
	 return ret;
}

static bool  GSM_Send_AT(void)//向模块发送"AT"
{
  bool  ret=false;
	
	if(Gsm_Send_AT((u8*)GSM_RX_BUF))           ret=true;  
	
	 return ret;
}

#endif
#if (GSM_Module_Type==1) //SIM7020C模块

//添加有关SIM7020C模块的接口代码

#endif

#if (GSM_Module_Type==2) //ESP8266Wifi模块
#include "GSM_esp8266.h" //添加gsm模块底层头文件(比如SIM800C,SIM7020C等模块的底层驱动程序)
/************************** 绑定模块控制接口 **************************/
//GSM模块IO初始化
static void GSM_IO_Init(void)
{
	Gsm_Io_Init();
}

//关闭GSM模块
static void GSM_Power_Down(void)
{
	Gsm_Power_Down();
}

//打开GSM模块	
static void GSM_Power_Up(void)
{
	Gsm_Power_Up();
}

//GSM模块进入休眠
static void GSM_Module_Sleep(void)
{
	Gsm_Module_Sleep();
}

//GSM模块唤醒
static void GSM_Module_WakeUp(void)
{
	Gsm_Module_WakeUp();
}

//GSM模块串口初始化
static void GSM_Usart_Init(u32 bound)
{
	GSM_USART_Init(bound);
}

//重启GSM模块
static bool GSM_Power_Rest(void)
{	 
	bool ret; 
	ret = true; 
	Gsm_Power_Rest(); //后期可以根据检测GSM模块的开机叫来判断重启模块是否成功
	return ret;
}
/**********************************************************************/


/************************** 绑定模块驱动接口 **************************/
//初次上电发送AT
static bool GSM_Send_AT(void)//向模块发送"AT"
{
	bool ret = false;

	if(Gsm_Send_AT((u8*)GSM_RX_BUF))
		ret=true;  

	return ret;
}

//取消回显
static bool GSM_Echo(bool flag) //是否设置GSM回显 flag:true代表回显 flag:false代表不回显
{	 
	bool ret = false; 
	
	if(flag==true)
	{
		 if(Gsm_Echo(true,(u8*)GSM_RX_BUF))
			 ret=true;   
	}
	else
	{
	    if(Gsm_Echo(false,(u8*)GSM_RX_BUF))
			ret=true;   
	}
	
	 return ret;
}

//发送具体的AT指令
static bool GSM_send_cmd(char *cmd, char *ack, u16 waittime)
{
	bool ret = false; 
	
	if(Gsm_send_cmd(cmd, ack, waittime, (u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	
	return ret;
}

//获取信号强度     //查询信号强度RSSI    信号范围0~31      0代表信号最差        31代表信号最强   
static bool GSM_AskCSQ(uint8_t *csq)
{
	bool ret = false;
	int rssi;
	
	if(Gsm_AskRssi(&rssi, (u8*)GSM_RX_BUF))
	{
		rssi = (rssi+113)/2; //将RSSI转换成CSQ
		*csq = rssi;
/**********WIFI模块信号修改**************/		
   	if(*csq>=31){
   			*csq=31;
   	}
   	if(*csq<1){
   			*csq=1;
   	}
/***************************************/		
		ret = true;
	}
	
	return ret;
}

//检查是否已经注册上了网络
static bool GSM_CheckNetwork(void)
{
	bool ret = false;  
	int rssi;
	
	if(Gsm_AskRssi(&rssi, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//设置WIFI模式
static bool GSM_SetWifiMode(void)
{
	bool ret = false;  
	
	//目前设置为Station模式
	if(Gsm_SetWifiMode_Station((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//加入AP网络
static bool GSM_OpenConnectAP(char *SSID, char *password)
{
	bool ret = false;  
	
	if(GSM_SetWifiMode())//设置Wifi模块处于Station模式
	{
	  	//目前为Station模式下加入AP网络
	   if(Gsm_OpenConnectAP(SSID, password, (u8*)GSM_RX_BUF))
	   {
	   	ret = true;
	   }
	}

	return ret;
}

//断开AP网络
static bool GSM_CloseConnectAP(void)
{
	bool ret = false;  
	
	//目前为Station模式下断开AP网络
	if(Gsm_CloseConnectAP((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//查询网络连接信息
static bool GSM_CheckNetStatus(uint8_t *Status)
{
	bool ret = false;  
	
	if(Gsm_CheckStatus(Status, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//设置IP地址是否动态获取
static bool GSM_SetDHCP(bool flag)
{
	bool ret = false;  
	
	if(Gsm_StationMode_SetDHCP(flag, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//设置静态IP
static bool GSM_SetStaticIP(char *ip, char *gateway, char* netmask)
{
	bool ret = false;  
	
	if(Gsm_StationMode_SetStaticIP(ip, gateway, netmask, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//建立连接模式
static bool GSM_SetSingleConnect(void)
{
	bool ret = false;  
	
	//目前设置为单连接
	if(Gsm_SetSingleConnect((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//设置传输模式
static bool GSM_SetTransmittionMode(void)
{
	bool ret = false;  
	
	//目前设置为非透传模式
	if(Gsm_SetTransmittionMode((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//查询本地IP地址
static bool GSM_AskLocalIPAddress(char *station_ip)
{
	bool ret = false;  
	
	//目前在Station模式下查找本地IP
	if(Gsm_CheckLocalIPAddress(station_ip, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//查询静态IP地址等参数
static bool GSM_AskCipStatus(char *station_ip, char *gateway, char *netmask)
{
	bool ret = false;  
	
	//目前查找的处于Station模式下的
	if(Gsm_CheckCipStatus(station_ip, gateway, netmask, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//查询MAC地址
static bool GSM_AskMacAddress(char *MacAddress)
{
	bool ret = false;  
	
	if(Gsm_AskStationMacAddress(MacAddress, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//恢复出厂设置
static bool GSM_Restore(void)
{
	bool ret = false;  
	
	if(Gsm_Restore((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

//进入浅睡眠模式
//如果设置了浅睡眠，唤醒脚为低电平唤醒，那么如果唤醒脚处于高电平，则模块继续进入睡眠，直到唤醒脚为低电平
//注意，模块刚开机一段时间12s左右不能进入低功耗！
static bool GSM_LightSleep(bool flag)
{
	bool ret = false;  
	
//	if(Gsm_Into_LightSleep(flag, (u8*)GSM_RX_BUF))
//	{
//		ret = true;
//	}
	
	return ret;
}

//设置唤醒模块浅睡眠的引脚
static bool GSM_SetLightSleepWakeUpPin(uint8_t pin, uint8_t level)
{
	bool ret = false;  
	
//	//唤醒脚为模块的GPIO5，低电平唤醒
//	if(Gsm_Set_LightSleepWakeUpPin(5, 0,(u8*)GSM_RX_BUF))
//	{
//		ret = true;
//	}
	
	return ret;
}

//读取模块获取IP地址方式
static bool GSM_AskDHCP(bool *flag)
{
	bool ret = false;  
	
	if(Gsm_AskDHCP(flag, (u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}
//重启WIFI模块
static bool GSM_Rest(void)
{
	bool ret = false;  
	
	if(Gsm_Rest((u8*)GSM_RX_BUF))
	{
		ret = true;
	}
	
	return ret;
}

/**********************************************************************/


/**************************** 绑定通信接口 ****************************/
//打开TCP服务器连接
static bool GSM_Open_Connect(char *ServerName, char *ServerPort)
{
	bool ret = false;   
	
	if(Gsm_ClientConnectTCP(ServerName, ServerPort, (u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	
	return ret;
}

//关闭TCP服务器连接
static bool GSM_Close_Connect(void)
{
	bool ret = false;   

	if(Gsm_ClientCloseTCP((u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	return ret;
}

//发送数据
static bool GSM_Send_Data(char *wbuf, int wlen)
{
	bool ret = false;    

	if(Gsm_send_data(wbuf, wlen, SendData_Server_AT_WaitTime_n10ms, (u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	
	return ret;
}

//接收数据
static bool GSM_Rev_Data(char *rbuf, int *rlen)
{

	bool ret = false;    
	
	if(Gsm_Rev_data(rbuf, rlen, SendData_Server_AT_WaitTime_n10ms, (u8*)GSM_RX_BUF))
	{
		ret=true;
	}
	return ret;
}
#endif
#if (GSM_Module_Type==3)//中兴MG2639模块 
#include "GSM_MG2639.h"
static bool GSM_AskCSQ(uint8_t *csq)//获取信号强度
{
	 bool  ret=false;
	if(GSM_MG2639_AskCSQ(csq,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
  return ret;
}
static bool GSM_Echo(bool flag)//是否设置GSM回显 flag:true代表回显 flag:false代表不回显
{	 
	  bool  ret=false; 
	if(flag==true)
	{
		 if(GSM_MG2639_Echo(true,(u8*)GSM_RX_BUF))      ret=true;   
	  
	}
	else
	{
	   if(GSM_MG2639_Echo(false,(u8*)GSM_RX_BUF))     ret=true;   
	
	}
	 return ret;
}

static void GSM_IO_Init(void)//GSM模块IO初始化
{
      GSM_MG2639_Io_Init();
}
static void GSM_Power_Down(void)//关闭GSM模块电源
{
      GSM_MG2639_Power_Down();//关闭GSM模块电源
    
}
static void GSM_Power_Up(void)//打开GSM模块电源	
{
      GSM_MG2639_Power_Up();//打开GSM模块电源	  
}
static void GSM_Usart_Init(u32 bound)//GSM模块串口初始化
{
	   GSM_USART_Init(bound);//GSM模块串口初始化
}
static bool GSM_Power_Rest(void)//重启GSM模块
{	 
	  bool  ret; 
	  ret=true; 
    GSM_MG2639_Power_Rest();//重启GSM模块  //后期可以根据检测GSM模块的开机叫来判断重启模块是否成功
	 return ret;
}
//GSM模块进入休眠
static void GSM_Module_Sleep(void)
{
//	Gsm_Module_Sleep();
}

//GSM模块唤醒
static void GSM_Module_WakeUp(void)
{
//	Gsm_Module_WakeUp();
}
static bool GSM_send_cmd(char *cmd,char *ack,u16 waittime)
{
    bool  ret=false; 
   if(GSM_MG2639_send_cmd(cmd,ack,waittime,(u8*)GSM_RX_BUF))
	 {
	     ret=true;
	 }
    return ret;


}
static bool GSM_Open_Connect(uint8_t ServerID,char *ServerName,char *ServerPort)//打开TCP服务器连接
{
    bool  ret=false;   

  if(GSM_MG2639_Open_Connect_Srever(ServerID,ServerName,ServerPort,Open_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;
	 
}
static bool GSM_Close_Connect(void)//关闭TCP服务器连接
{
    bool  ret=false;   

////  if(Gsm_Close_Connect_Srever((u8*)GSM_RX_BUF))
////	{
////	     ret=true;
////	 }
    return ret;
}
static bool GSM_Send_Data(uint8_t ServerID,char *wbuf, int wlen)//发送数据
{
   bool  ret=false;    

  if(GSM_MG2639_send_data(ServerID,wbuf,wlen,SendData_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
	{
	     ret=true;
	 }
    return ret;

}
static bool GSM_Rev_Data(char *rbuf, int *rlen)//接收数据
{
   bool  ret=false;    
	
////  if(Gsm_Rev_data(rbuf,rlen,SendData_Server_AT_WaitTime_n10ms,(u8*)GSM_RX_BUF))
////	{
////	     ret=true;
////	 }
    return ret;

}
static bool GSM_Check_Sim_Card(void)//检查SIM卡是否在位
{
	  bool  ret=false;    
  if(GSM_MG2639_Check_SIm_Card((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;
}

static bool GSM_CheckNetwork(void)//检查SIM卡是否已经注册上了网络
{
	 bool  ret=false;    
  if(GSM_MG2639_CheckNetwork((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;

}
static bool GSM_CheckGPRSActive(void)//检查SIM卡是否已经附着上了GPRS网络
{
	 bool  ret=false;    
  if(GSM_MG2639_CheckGPRSActive((u8*)GSM_RX_BUF))
	{
	     ret=true;
	}
    return ret;

}
static bool  GSM_Send_AT(void)//向模块发送"AT"
{
  bool  ret=false;
	
	if(GSM_MG2639_Send_AT((u8*)GSM_RX_BUF))           ret=true;  
	
	 return ret;
}
static int	 GSM_Adaptive_BaudRate(void)  /* 自适应波特率 */
{
   int  BaudRate;
	BaudRate=GSM_MG2639_Adaptive_BaudRate((u8*)GSM_RX_BUF);
    return   BaudRate;
}
static bool	GSM_SetBaudRate(int br)//设置模块波特率
{
  bool  ret=false;
	 if(GSM_MG2639_SetBaudRate(br,(u8*)GSM_RX_BUF))//设置模块波特率
	 {
	     ret=true;
	 }
  return   ret;
}


//发送短信（支持中英文,中文为GBK码）
/**
 * @brief  gsm_sms 发送短信（支持中英文,中文为GBK码）
 * @param  num: 电话号码，smstext：短信内容
 * @retval 无
 */
static bool GSM_Send_Sms(char *num,char *smstext)//发送短信内容数据
{
    bool  ret=false;
////	if(Gsm_Send_Sms(num,smstext,(u8*)GSM_RX_BUF))
////	{
////	    ret=true;
////	}

return   ret;


}

//添加有关中兴MG2639模块的接口代码

#endif
#if (GSM_Module_Type==4) //待定1模块

//添加有关待定1模块的接口代码

#endif
#if (GSM_Module_Type==5) //待定2模块

//添加有关待定2模块的接口代码

#endif

	//GSM通信接口初始化
bool GSM_Transmittion_Init(GSM_HANDLE *pHandle)
{
		//绑定模块控制接口
		pHandle->pGSM_IO_Init=GSM_IO_Init;//GSM模块IO初始化
		pHandle->pGSM_Usart_Init=GSM_Usart_Init;//GSM模块串口初始化
		pHandle->pGSM_Power_Rest=GSM_Power_Rest;//重启GSM模块
		pHandle->pGSM_Power_Down=GSM_Power_Down;//关闭GSM模块电源
	  pHandle->pGSM_Power_Up=GSM_Power_Up;//打开GSM模块电源
		pHandle->pGSM_Sleep = GSM_Module_Sleep; //GSM模块进入睡眠
	  pHandle->pGSM_WakeUp = GSM_Module_WakeUp; //GSM模块唤醒
	
		//绑定模块驱动接口
    #if (GSM_Module_Type==0) //SIM800C模块
	  //注册有关SIM800C模块的接口代码
		pHandle->sim800c.pGSM_Echo=GSM_Echo;//设置模块是否关闭回显功能
		pHandle->sim800c.pGSM_Send_AT=GSM_Send_AT;//向串口发送"AT"
		pHandle->sim800c.pGSM_CSQ = GSM_AskCSQ;//获取信号强度
		pHandle->sim800c.pGSM_CheckNetwork=GSM_CheckNetwork;//检查SIM卡是否注册上了网络
		pHandle->sim800c.pGSM_CheckGPRSActive=GSM_CheckGPRSActive;//检查SIM卡的附着态
		pHandle->sim800c.pGSM_Check_Sim_Card=GSM_Check_Sim_Card;//检查SIM卡是否在位
	  pHandle->sim800c.pGSM_Send_Sms=GSM_Send_Sms;//发送短信内容
	
		 	//绑定通信接口
		pHandle->pGSM_send_cmd=GSM_send_cmd;//向串口发送AT指令
	  pHandle->pGSM_Open_Connect=GSM_Open_Connect;//打开TCP连接
	  pHandle->pGSM_CloseConnect=GSM_Close_Connect;//关闭TCP连接
	  pHandle->pGSM_SendData=GSM_Send_Data;//向服务器发送数据
		pHandle->pGSM_RevData=GSM_Rev_Data;//向服务器发送数据
	  #endif
	  #if (GSM_Module_Type==1) //SIM7020C模块
   //注册有关SIM7020C模块的接口代码
    #endif
    #if (GSM_Module_Type==2) //ESP8266Wifi模块
	  //绑定模块驱动接口
	  pHandle->esp8266.pGSM_Echo = GSM_Echo; //设置回显模式 （true开回显，false关回显）
	  pHandle->esp8266.pGSM_Send_AT = GSM_Send_AT; //初次上电发送AT 
	  pHandle->esp8266.pGSM_CSQ = GSM_AskCSQ; //获取信号强度
	  pHandle->esp8266.pGSM_CheckNetwork = GSM_CheckNetwork; //检查SIM卡是否注册上了网络
	  
	  pHandle->esp8266.pGSM_SetWifiMode = GSM_SetWifiMode; //设置WIFI模式
	  pHandle->esp8266.pGSM_OpenConnectAP = GSM_OpenConnectAP; //加入AP网络
	  pHandle->esp8266.pGSM_CloseConnectAP = GSM_CloseConnectAP; //断开AP网络
	  pHandle->esp8266.pGSM_CheckNetStatus = GSM_CheckNetStatus; //查询网络连接信息
	  pHandle->esp8266.pGSM_SetDHCP = GSM_SetDHCP; //设置IP地址是否动态获取
	  pHandle->esp8266.pGSM_SetStaticIP = GSM_SetStaticIP; //设置静态IP
	  pHandle->esp8266.pGSM_SetSingleConnect = GSM_SetSingleConnect; //建立单连接
	  pHandle->esp8266.pGSM_SetTransmittionMode = GSM_SetTransmittionMode; //设置传输模式（目前为非透传）
	  pHandle->esp8266.pGSM_AskLocalIPAddress = GSM_AskLocalIPAddress; //查询本地IP地址(Station模式下)
	  pHandle->esp8266.pGSM_AskCipStatus = GSM_AskCipStatus; //查询已经设置station模式下的静态IP地址等参数
	  pHandle->esp8266.pGSM_AskMacAddress = GSM_AskMacAddress; //查询MAC地址 
		pHandle->esp8266.pGSM_Restore = GSM_Restore; //恢复出厂设置
	  pHandle->esp8266.pGSM_LightSleep = GSM_LightSleep; //进入浅睡眠模式
	  pHandle->esp8266.pGSM_SetLightSleepWakeUpPin = GSM_SetLightSleepWakeUpPin; //设置唤醒模块浅睡眠的引脚
		pHandle->esp8266.pGSM_AskDHCP = GSM_AskDHCP; //查询DHCP状态(Station模式下)
		pHandle->esp8266.pGSM_Rest=GSM_Rest;//重启Wifi模块
			 	//绑定通信接口
		pHandle->pGSM_send_cmd=GSM_send_cmd;//向串口发送AT指令
	  pHandle->pGSM_Open_Connect=GSM_Open_Connect;//打开TCP连接
	  pHandle->pGSM_CloseConnect=GSM_Close_Connect;//关闭TCP连接
	  pHandle->pGSM_SendData=GSM_Send_Data;//向服务器发送数据
		pHandle->pGSM_RevData=GSM_Rev_Data;//向服务器发送数据

    #endif
    #if (GSM_Module_Type==3)//中兴MG2639模块 
    pHandle->mg2639.pGSM_Echo=GSM_Echo;//设置模块是否关闭回显功能
		pHandle->mg2639.pGSM_Send_AT=GSM_Send_AT;//向串口发送"AT"指令
		pHandle->mg2639.pGSM_send_cmd=GSM_send_cmd;//向模块发送AT指令
		pHandle->mg2639.pGSM_CSQ=GSM_AskCSQ;//获取信号强度
		pHandle->mg2639.GSM_Adaptive_BaudRate=GSM_Adaptive_BaudRate;/* 自适应波特率 */
		pHandle->mg2639.GSM_SetBaudRate=GSM_SetBaudRate;//设置模块的波特率
		pHandle->mg2639.pGSM_CheckNetwork=GSM_CheckNetwork;//检查SIM卡是否注册上了网络
		pHandle->mg2639.pGSM_CheckGPRSActive=GSM_CheckGPRSActive;//检查SIM卡的附着态
		pHandle->mg2639.pGSM_Check_Sim_Card=GSM_Check_Sim_Card;//检查SIM卡是否在位
	  pHandle->mg2639.pGSM_Send_Sms=GSM_Send_Sms;//发送短信内容
		
		//绑定通信接口
	  pHandle->mg2639.pGSM_Open_Connect=GSM_Open_Connect;//打开TCP连接
	  pHandle->pGSM_CloseConnect=GSM_Close_Connect;//关闭TCP连接
	  pHandle->mg2639.pGSM_SendData=GSM_Send_Data;//向服务器发送数据
		pHandle->pGSM_RevData=GSM_Rev_Data;//向服务器发送数据
    //注册有关中兴MG2639模块的接口代码
    #endif
    #if (GSM_Module_Type==4) //待定1模块
    
    //注册有关待定1模块的接口代码 
    #endif
    #if (GSM_Module_Type==5) //待定2模块
    //注册有关待定2模块的接口代码 
    #endif
	  return true;
}   





