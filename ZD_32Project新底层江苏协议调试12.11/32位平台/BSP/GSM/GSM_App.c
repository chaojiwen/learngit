#include "GSM_app.h"
#include "interface.h"
#include "Gsm_Interface.h"
#include "Gsm_Api.h"
#include "stdbool.h"
#include "ucos_ii.h"
#include "Mcu_Config.h"
#include "string.h"
#include "Log_GSM.h"

#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	
#include "Log_GSM_Config.h"
#endif

BOOL DEBUG_Flag = TRUE;//程序调试信息输出控制标志位  GSM_DEBUG_Flag = FALSE代表不打开调试信息输出  GSM_DEBUG_Flag = TRUE代表打开调试信息输出

extern  GSM_GPRS_SR_Struct GPRS_SR_Reg;



GSM_GPRS_Connection    GSM_Tcp_Conn[GSM_MAX_NUM_TCP_CONN];// 设备要访问的tcp服务器参数数组



bool Sim800c_env_init(_SCSStruct * state)
{
#if (GSM_Module_Type==0x00) //SIM800C模块
	  u8 GsmCsq=0;//gsm模块信号强度
		u8 times = 0;//执行命令次数
	  bool  ret=false;
    GSM_Transmittion_Init(&gsm_handle);//GSM模块相关接口函数注册 
  	gsm_handle.pGSM_IO_Init();//GSM模块相关IO口初始化	
    gsm_handle.pGSM_Usart_Init(115200);//GSM模块串口初始化	
		gsm_handle.pGSM_Power_Down();//对GSM模块进行关机操作 
    gsm_handle.pGSM_Power_Up();//对GSM模块进行开机操作      
	  OSTimeDly(5000);//延时5s	
		 gsm_handle.pGSM_send_cmd("ATE0","OK",1000);//关闭回显
		 gsm_handle.pGSM_send_cmd("AT+CIPHEAD=0","OK",1000);//关闭接收数据时显示+IPD头
	//1、检查SIM卡是否在位
    if(gsm_handle.sim800c.pGSM_Check_Sim_Card())	
		{
		    Log_printf("检测到SIM卡！！！\n");	  
		}
		else
		{
			 Log_printf("未检测到SIM卡！！！\n");	
			 /* 设置卡未检测信息 */
			 *state	= _SIM_NotDetected;
			 goto dev_gsm_init_err;

		}
		//2、检查模块信号强度
		 times = 0;//执行命令次数
		 ret=false;
	   do{
			    OSTimeDly(3000);//延时3s	
			   if(gsm_handle.sim800c.pGSM_CSQ(&GsmCsq)&&GsmCsq>0)
	       {
					  ret=true;
					  break;
	       }
				 else
				 {
				   ret=false;
				 }	
  				 
		   }while(times++<20);//不成功连续检测1分钟
			 if(!ret)//获取信号强度失败
		   {
				  Log_printf("模块无信号！！！\n");
			   	/* 设置无网络信号状态 */
			   *state	= _SIM_NoSignal;
				  goto dev_gsm_init_err;  	 
		   }
		   //3、检查模块是否已经注册上了网络
		   times = 0;//执行命令次数
		   ret=false;
		   do{
				    OSTimeDly(3000);//延时3s	 
			      if(gsm_handle.sim800c.pGSM_CheckNetwork())
	          {
					     ret=true;
					     break;
	          }
				    else
				    {
				      ret=false;
				    }
		   }while(times++<100);//不成功连续检测5分钟
			 if(!ret)//获取信号强度失败
		   {
				  Log_printf("模块无法注册上网络！！！\n");
			    /* 设置无网络信号状态 */
		    	*state	= _SIM_NoSignal;	
				  goto dev_gsm_init_err;  	 
		   }
		   //4、检查模块是否已经附着上了网络
		   times = 0;//执行命令次数
		   ret=false;
		   do{
				    OSTimeDly(3000);//延时3s	 
			      if(gsm_handle.sim800c.pGSM_CheckGPRSActive())
	          {
					     ret=true;
					     break;
	          }
				    else
				    {
				      ret=false;
				    }
		   }while(times++<100);//不成功连续检测5分钟
			 if(!ret)//获取信号强度失败
		   {
				  Log_printf("模块无法附着上网络！！！\n");
				 
		    	/* 设置卡欠费停机状态 */
			    *state	= _SIM_ShutDown;
				  goto dev_gsm_init_err;  	 
		   }
			 
			 //5、模块初始化成功
	     Log_printf(">>>初始化GSM模块成功！！！\r\n");
		  /* 设置卡未检测信息 */
	     *state	= _SIM_OK;
       return true;	
dev_gsm_init_err:
			   gsm_handle.pGSM_Power_Down();//将GSM模块进行关机
         return false;		 
#endif		 
#if (GSM_Module_Type==0x03) //中兴MG2639模块
		u8 GsmCsq=0;//gsm模块信号强度
		u8 times = 0;//执行命令次数
	  int 	baud_rate;   //GSM模块波特率	 
	  bool  ret=false;
    GSM_Transmittion_Init(&gsm_handle);//GSM模块相关接口函数注册 
  	gsm_handle.pGSM_IO_Init();//GSM模块相关IO口初始化	
    gsm_handle.pGSM_Usart_Init(115200);//GSM模块串口初始化	
		gsm_handle.pGSM_Power_Down();//将GSM模块进行关机	 
    gsm_handle.pGSM_Power_Up();//对GSM模块进行开机操作      
	  OSTimeDly(5000);//延时5s	
		//1、检测模块的波特率是否为9600   若不是则需要重新对其进行设置
		baud_rate=gsm_handle.mg2639.GSM_Adaptive_BaudRate();
    if(baud_rate!=115200)	
		{
			Log_printf("当前MG2639模块波特率为:%d     需要重新设置模块的波特率\n",baud_rate);
		    if(!gsm_handle.mg2639.GSM_SetBaudRate(115200))
				{
					Log_printf("配置MG2639模块波特率失败！！！\n");
			      	 /* 设置卡未检测信息 */
			       *state	= _SIM_NotDetected;
			       goto dev_gsm_init_err;
				}
		
		}	
		//2、设置模块回显方式
		if(gsm_handle.mg2639.pGSM_send_cmd("ATE1","OK",1000))//关闭回显
		{
		  Log_printf("开启MG2639模块回显功能成功！！！\n");
		}			
	//3、检查SIM卡是否在位
    if(gsm_handle.mg2639.pGSM_Check_Sim_Card())	
		{
		    Log_printf("MG2639模块检测到SIM卡！！！\n");	  
		}
		else
		{
			 Log_printf("MG2639模块未检测到SIM卡！！！\n");	
			 /* 设置卡未检测信息 */
			 *state	= _SIM_NotDetected;
			 goto dev_gsm_init_err;

		}
			//3、检查模块信号强度
		 times = 0;//执行命令次数
		 ret=false;
	   do{
			    OSTimeDly(3000);//延时3s	
			   if(gsm_handle.mg2639.pGSM_CSQ(&GsmCsq)&&GsmCsq>0)
	       {
					 Log_printf("查询MG2639模块信号强度成功！！！  当前MG2639模块的信号强度为:%d\n",GsmCsq);
					  ret=true;
					  break;
	       }
				 else
				 {
				   ret=false;
				 }	
  				 
		   }while(times++<20);//不成功连续检测1分钟
			 if(!ret)//获取信号强度失败
		   {
				  Log_printf("MG2639模块无信号！！！\n");
			   	/* 设置无网络信号状态 */
			   *state	= _SIM_NoSignal;
				  goto dev_gsm_init_err;  	 
		   }	
		//4、检查模块是否已经注册上了网络
		   times = 0;//执行命令次数
		   ret=false;
		   do{
				    OSTimeDly(3000);//延时3s	 
			      if(gsm_handle.mg2639.pGSM_CheckNetwork())
	          {
							 Log_printf("MG2639模块已经成功注册上网络！！！\n");
					     ret=true;
					     break;
	          }
				    else
				    {
				      ret=false;
				    }
		   }while(times++<100);//不成功连续检测5分钟
			 if(!ret)//获取信号强度失败
		   {
				  Log_printf("MG2639模块无法注册上网络！！！\n");
			    /* 设置无网络信号状态 */
		    	*state	= _SIM_NoSignal;	
				  goto dev_gsm_init_err;  	 
		   }
		
	 //5、检查模块是否已经附着上了网络
		   times = 0;//执行命令次数
		   ret=false;
		   do{
				    OSTimeDly(3000);//延时3s	 
			      if(gsm_handle.mg2639.pGSM_CheckGPRSActive())
	          {
							Log_printf("MG2639模块已经成功附着上网络！！！\n");
					     ret=true;
					     break;
	          }
				    else
				    {
				      ret=false;
				    }
		   }while(times++<100);//不成功连续检测5分钟
			 if(!ret)//获取信号强度失败
		   {
				  Log_printf("MG2639模块无法附着上网络！！！\n");
				 
		    	/* 设置卡欠费停机状态 */
			    *state	= _SIM_ShutDown;
				  goto dev_gsm_init_err;  	 
		   }		 
      //5、模块初始化成功
	     Log_printf(">>>初始化MG2639模块成功！！！\r\n");
		  /* 设置SIM检测信息 */
	     *state	= _SIM_OK;
       return true;			
dev_gsm_init_err:
	 /* 设置无网络信号状态 */
	 *state	= _SIM_NoSignal;	
   gsm_handle.pGSM_Power_Down();//将GSM模块进行关机
      return false;				 	 
#endif			 
#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	 
    bool Wifi_IP_Get_Way;//Wifi模块获取IP方式
		u8 times = 0;//执行命令次数
	  bool  ret=false;
    GSM_Transmittion_Init(&gsm_handle);//GSM模块相关接口函数注册 
  	gsm_handle.pGSM_IO_Init();//GSM模块相关IO口初始化	
    gsm_handle.pGSM_Usart_Init(115200);//GSM模块串口初始化	
    gsm_handle.pGSM_Power_Up();//对GSM模块进行开机操作 
	  OSTimeDly(5000);//延时3s	
		
	  //1、检测模块工作状态是否正常
   if(gsm_handle.esp8266.pGSM_Send_AT())
	 {
	   Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State=1; 
      Log_printf("1、WIFI模块工作状态正常！！！\n");	 			 
	 }
	 else
	 {
		 Instru_Wifi_Fix_Tab.Inf.Wifi_Work_State=0;
	   Log_printf("1、WIFI模块工作状态故障！！！\n");			
	 }
	  //2、关闭Wifi模块回显
	 if(gsm_handle.esp8266.pGSM_Echo(true))
	 {
	   Log_printf("2、关闭Wifi模块回显成功！！！\n");	 	
	 }
	 else
	 {
		  Log_printf("2、关闭Wifi模块回显失败！！！\n");	
	 }
	 
/**************************************************************************/	 
	 //3、将Wifi模块设置为Station模式
	 if(gsm_handle.esp8266.pGSM_SetWifiMode())
	 {
	   Log_printf("3、将Wifi模块设置为Station模式成功！！！\n");	 	
	 }
	 else
	 {
		  Log_printf("3、将Wifi模块设置为Station模式失败！！！\n");	
	 }
	 	//4、查询Wifi模块的MAC地址
	 	 times = 0;//执行命令次数
	   do{	
			     if(gsm_handle.esp8266.pGSM_AskMacAddress(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address))
	         {
						 ret=true;
				     break;
	         }
			 	   else
				   {
				     ret=false;
				   } 
           OSTimeDly(10);//延时10ms					  
		   }while(times++<5);//不成功连续检测5次
		 
			 if(ret)//查询Wifi模块的MAC地址
		   {
				   Log_printf("4、Wifi模块的MAC地址为:%s！！！\n",Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address);	
		   }
			 else
			 {
			     Log_printf("4、查询Wifi模块的MAC地址失败！！！\n");	
			 }

//判断是否需要配置Wifi模块参数
	 if(GSM_Config_Reg.GSM_Config_State==TRUE)
	 {
		      SetWifiPara();
		 GSM_Config_Reg.GSM_Config_State=FALSE;
	 }			 
			 
			 
	 	//5、查询WifiWlan参数
	 	 times = 0;//执行命令次数
	   do{	
			     if(gsm_handle.esp8266.pGSM_AskCipStatus(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP ,Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask))
	         {
						 ret=true;
				     break;
	         }
			 	   else
				   {
				     ret=false;
				   } 
           OSTimeDly(10);//延时10ms					  
		   }while(times++<5);//不成功连续检测5次
		 
			 if(ret)//查询WifiWlan参数
		   {
				   Log_printf("5、查询WifiWlan参数成功！！！\n");	
		   }
			 else
			 {
			     Log_printf("5、查询WifiWlan参数失败！！！\n");	
			 }	

	 	//5、查询模块获取IP地址方式
	 	 times = 0;//执行命令次数
	   do{	
			     if(gsm_handle.esp8266.pGSM_AskDHCP(&Wifi_IP_Get_Way))
	         {
						 Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way=Wifi_IP_Get_Way;
						 
						 ret=true;
				     break;
	         }
			 	   else
				   {
				     ret=false;
				   } 
           OSTimeDly(10);//延时10ms					  
		   }while(times++<5);//不成功连续检测5次
		 
			 if(ret)//查询WifiWlan参数
		   {
				   Log_printf("6、查询模块获取IP地址方式成功！！！\n");	
		   }
			 else
			 {
			     Log_printf("6、查询模块获取IP地址方式失败！！！\n");	
			 }

		 ret=true;
		 Log_printf("WIFI模块初始化成功！！！\n");	
		  /* 设置卡未检测信息 */
	   *state	= _SIM_OK;		
    return  ret;	
#endif
			 
}
#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	 
//设置Wifi模块参数
bool    SetWifiPara(void)
{
int                          times=0;
INSTRU_WIFI                  Current_Wifi_Fix_Tab;//当前Wifi工作状态、Wlan等参数
   //当前GSM模块需要配置参数
	    if(GSM_Config_Reg.WifiNamePWD_Config_State==TRUE)
			{//设置Wifi模块的名字及密码
				gsm_handle.esp8266.pGSM_OpenConnectAP(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name,
									                            Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password);
				
			  GSM_Config_Reg.WifiNamePWD_Config_State=FALSE;
			}		
			if(GSM_Config_Reg.WifiWLAN_Config_State==TRUE)
			{//设置Wifi模块的WLAN参数
				times=0;
							do{
								 gsm_handle.esp8266.pGSM_AskCipStatus(Current_Wifi_Fix_Tab.Inf.Wifi_Local_IP,
							                                     Current_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,
							                                     Current_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask);
								if((strcmp(Current_Wifi_Fix_Tab.Inf.Wifi_Local_IP,Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP)!=0)||\
								 (strcmp(Current_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP)!=0)||\
							   (strcmp(Current_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask)!=0))
								{
								    gsm_handle.esp8266.pGSM_SetStaticIP(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,
									                                 Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP ,
							                                     Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask  
							                                     );
								
								}else
								{
								   break;
								
								}
							   OSTimeDly(100);//延时100ms		
							}while(times++<3);	
				
			 GSM_Config_Reg.WifiWLAN_Config_State=FALSE;
			}
		
			 OSTimeDly(100);//延时10ms			
		 if(GSM_Config_Reg.WifiWLAN_GET_IP_WAY_Config_State==TRUE)
			{//设置Wifi模块获取IP地址方式
								 times=0;
									do{
											  gsm_handle.esp8266.pGSM_SetDHCP(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way);
									
									  OSTimeDly(100);//延时100ms		
									}while(times++<5);										
									if(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==1)
									{
									   gsm_handle.esp8266.pGSM_Rest();//对Wifi模块进行复位
									}	
			  GSM_Config_Reg.WifiWLAN_GET_IP_WAY_Config_State=FALSE;
			}	
				
		 if(GSM_Config_Reg.WifiRest_Config_State==TRUE)
			{//复位Wifi模块
			  	gsm_handle.esp8266.pGSM_Rest();//复位Wifi模块
				GSM_Config_Reg.WifiRest_Config_State=FALSE;
			}	
			if(GSM_Config_Reg.WifiNamePWD_Config_State==TRUE)
			{//Wifi模块恢复出厂设置
			 	gsm_handle.esp8266.pGSM_Restore();//复位Wifi模块	
				
				GSM_Config_Reg.WifiNamePWD_Config_State=FALSE;
			}
			return   true;
}

#endif


/**
  * @brief  Description "获得GSM的信号强度"
  * @param  None
  * @retval int			GSM的信号强度(5~32为正常数值)
  */
int Gsm_get_signal(void)
{
    u8 rssi;
    bool ret;
	#if (GSM_Module_Type==0x00) //SIM800C模块
    ret = gsm_handle.sim800c.pGSM_CSQ(&rssi);
	#endif
	#if (GSM_Module_Type==0x02) //ESP8266Wifi模块
	 ret = gsm_handle.esp8266.pGSM_CSQ(&rssi);
	#endif
	#if (GSM_Module_Type==0x03) //中兴MG2639模块
   ret = gsm_handle.mg2639.pGSM_CSQ(&rssi);
	#endif
    if (ret)
    {
        return rssi;
    }
    return -1;
}

/**
  * @brief  Description "获得GSM的GPRS的附着状态"
  * @param  None
  * @retval bool		GPRS通讯是否正常
  */
int Gsm_get_gprs_state(void)
{
    bool ret;
    ret = gsm_handle.sim800c.pGSM_CheckGPRSActive();
    return ret;
}
/**
  * @brief  服务器收发数据
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
  * @param  rbuf		接收缓冲区
  * @param  rlen		接收缓冲区字节数大小
* @param  Sever_Id 代表向哪个服务器发送数据 0:代表数据服务器; 1:代表配置服务器; 2:代表升级服务器
  * @retval bool		GPRS通讯是否成功
  */
static  bool Server_Send_Rev_Data(char *wbuf, int wlen, char *rbuf, int* rlen,u8 Server_Id)
{
	#if ((GSM_Module_Type==0) ||(GSM_Module_Type==2))//SIM800C模块
	  bool ret=false;	
	   //连接服务器
 if((__strlen(GSM_Tcp_Conn[Server_Id].ip) <= 0)&&((__strlen(GSM_Tcp_Conn[Server_Id].dn) > 0)))
	{
	   if(gsm_handle.pGSM_Open_Connect(GSM_Tcp_Conn[Server_Id].dn,GSM_Tcp_Conn[Server_Id].port))//域名连接zwc
		 {
		     ret=true;	//服务器连接成功 	
			 
			   GPRS_SR_Reg.GPRS_Work_State=TRUE;//连接服务器成功
		 }else
		 {
		        GPRS_SR_Reg.GPRS_Work_State=FALSE;//连接服务器失败
			      GPRS_SR_Reg.SimState=SIM_NoSignal;//图标显示为无GPRS信号
		 }
	}
	else
	{
	   if(gsm_handle.pGSM_Open_Connect(GSM_Tcp_Conn[Server_Id].ip,GSM_Tcp_Conn[Server_Id].port))//IP连接
		 {
		     ret=true;	//服务器连接成功 
			 
			  GPRS_SR_Reg.GPRS_Work_State=TRUE;//连接服务器成功
		 }
		 else
		 {
		        GPRS_SR_Reg.GPRS_Work_State=FALSE;//连接服务器失败
			      GPRS_SR_Reg.SimState=SIM_NoSignal;//图标显示为GPRS信号
		 }
	}
	
 //发送数据
	 if(ret)//服务器连接成功 
	 {		
	    if(gsm_handle.pGSM_SendData(wbuf,wlen))//判读发送数据是否成功
			{
				//数据发送成功后开始接收数据
				if(gsm_handle.pGSM_RevData(rbuf,rlen))
				{
				  ret=true;	//服务器接收数据成功 
				}
				else
				{
				    goto gsm_rev_data_err;//服务器接收数据失败
				}
			} 
	 }
	 
//关闭与服务器连接 
  gsm_handle.pGSM_CloseConnect();
 return ret;
 
gsm_rev_data_err:
 gsm_handle.pGSM_CloseConnect();//关闭与服务器连接
 return false;
	#endif 
	 
	#if (GSM_Module_Type==3)//中兴MG2639模块
	  bool ret=false;	
	   //连接服务器
 if((__strlen(GSM_Tcp_Conn[Server_Id].ip) <= 0)&&((__strlen(GSM_Tcp_Conn[Server_Id].dn) > 0)))
	{
	   if(gsm_handle.mg2639.pGSM_Open_Connect(Server_Id,GSM_Tcp_Conn[Server_Id].dn,GSM_Tcp_Conn[Server_Id].port))//域名连接zwc
		 {
		     ret=true;	//服务器连接成功 		 
			   GPRS_SR_Reg.GPRS_Work_State=TRUE;//连接服务器成功
		 }else
		 {
		        GPRS_SR_Reg.GPRS_Work_State=FALSE;//连接服务器失败
			      GPRS_SR_Reg.SimState=SIM_NoSignal;//图标显示为无GPRS信号
		 }
	}
	else
	{
	   if(gsm_handle.mg2639.pGSM_Open_Connect(Server_Id,GSM_Tcp_Conn[Server_Id].ip,GSM_Tcp_Conn[Server_Id].port))//IP连接
		 {
		     ret=true;	//服务器连接成功 
			 
			  GPRS_SR_Reg.GPRS_Work_State=TRUE;//连接服务器成功
		 }
		 else
		 {
		        GPRS_SR_Reg.GPRS_Work_State=FALSE;//连接服务器失败
			      GPRS_SR_Reg.SimState=SIM_NoSignal;//图标显示为GPRS信号
		 }
	}
	
 //发送数据
	 if(ret)//服务器连接成功 
	 {		
	    if(gsm_handle.mg2639.pGSM_SendData(Server_Id,wbuf,wlen))//判读发送数据是否成功
			{
				//数据发送成功后开始接收数据
				if(gsm_handle.pGSM_RevData(rbuf,rlen))
				{
				  ret=true;	//服务器接收数据成功 
				}
				else
				{
				    goto gsm_rev_data_err;//服务器接收数据失败
				}
			} 
	 }
	 
//关闭与服务器连接 
  gsm_handle.pGSM_CloseConnect();
 return ret;
 
gsm_rev_data_err:
 gsm_handle.pGSM_CloseConnect();//关闭与服务器连接
 return false;
	#endif	 
	 
	 
	 
	 
 
}
/**
  * @brief  向服务器发数据
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
* @param  Sever_Id 代表向哪个服务器发送数据 0:代表数据服务器; 1:代表配置服务器; 2:代表升级服务器
  * @retval bool		GPRS通讯是否成功
  */
bool Server_Send_Data(char *wbuf, int wlen,u8 Server_Id)
{
	  bool ret=false;	
	   //连接服务器
 if((__strlen(GSM_Tcp_Conn[Server_Id].ip) <= 0)&&((__strlen(GSM_Tcp_Conn[Server_Id].dn) > 0)))
	{
	   if(gsm_handle.pGSM_Open_Connect(GSM_Tcp_Conn[Server_Id].dn,GSM_Tcp_Conn[Server_Id].port))//域名连接zwc
		 {
		     ret=true;	//服务器连接成功 
		 }
	}
	else
	{
	   if(gsm_handle.pGSM_Open_Connect(GSM_Tcp_Conn[Server_Id].ip,GSM_Tcp_Conn[Server_Id].port))//IP连接
		 {
		 
		     ret=true;	//服务器连接成功 
		 }
	}
 //发送数据
	 if(ret)//服务器连接成功 
	 {
	    if(gsm_handle.mg2639.pGSM_SendData(Server_Id,wbuf,wlen))//判读发送数据是否成功
			{		
				  ret=true;	//向服务器发送数据成功
	
			} 
	 }
//关闭服务器连接成功  	 
 if(ret)
 {
    if(gsm_handle.pGSM_CloseConnect())
		{
		  ret=true;	//服务器连接成功 
		}
		else
		{
		 ret=false;	
		}
 }
 
 return ret;
}



/**
  * @brief  Description "GPRS通讯"
  * @param  wbuf		发送缓冲区
  * @param  wlen		发送缓冲区字节数大小
  * @param  rbuf		接收缓冲区
  * @param  rlen		接收缓冲区字节数大小
* @param  Sever_Id 代表向哪个服务器发送数据 0:代表数据服务器; 1:代表配置服务器; 2:代表升级服务器
  * @retval bool		GPRS通讯是否成功
  */
bool Gsm_gprs_comm(char *wbuf, int wlen, char *rbuf, int* rlen,u8 Server_Id)
{
  bool ret=false;	
	
	switch(Server_Id)
	{
		case GSM_GPRS_Data:  //数据服务器
			if(Server_Send_Rev_Data(wbuf,wlen,rbuf,rlen,GSM_GPRS_Data))     ret=true;	
			break;
	  case GSM_GPRS_Config: //配置服务器
			if(Server_Send_Rev_Data(wbuf,wlen,rbuf,rlen,GSM_GPRS_Config))   ret=true;	
			break;
	  case GSM_GPRS_Update://升级服务器
			if(Server_Send_Rev_Data(wbuf,wlen,rbuf,rlen,GSM_GPRS_Update))   ret=true;	
			break;
	  default:
			break;
	}
  return ret;
}







	






