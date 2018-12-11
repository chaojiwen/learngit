/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2018 STMicroelectronics															*/
/**																								*/
/**文件名     : GSM_MG2639.c																*/
/**作用       : 中兴GSM模块底层驱动					*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1     ZWC         2018年12月06日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Gsm_Api.h"
#if (GSM_Module_Type==0x03) //中兴MG2639模块
#include "GSM_MG2639.h"
#include "GSM_USART.h"
#include "ucos_ii.h"
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include "string_utility.h"
/************************************************************************************************
*函数名   ：u8* GSM_MG2639_check_cmd(char *str,u8 *UsartBuf)															
*输入参数 ：str :要寻找的目标字符串											
*输出参数 ：	  																				
*作用     ：查找特征字符串 	  
*创建日期 ： 2018/12/06	
*说明     ： //发送命令后,检测接收到的应答
//str:期待的应答结果  返回值:0,没有得到期待的应答结果 其他,期待应答结果的位置(str的位置)
************************************************************************************************/
bool   GSM_MG2639_check_cmd(char *str,u8 *UsartBuf)
{
	 int ret=0;
	ret=String_KMPMatch((char*)UsartBuf,str);
	if(ret==-1)
  {
		
		return false;
	}
	
	return true;
}
/************************************************************************************************
*函数名   ：u8 GSM_MG2639_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)											
*输入参数 ：cmd:要发送的AT指令字符串	ack：要匹配的特征字符串 waittime:匹配特征字符串需要等待的时间 单位10ms										
*输出参数 ：命令执行成功返回 1   失败返回 0  																				
*作用     ：发送AT指令 	  
*创建日期 ： 2018/12/06 	
*说明     ：
************************************************************************************************/
bool GSM_MG2639_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
{
	 u8 res=1;
   Clear_GSM_RX_BUF(UsartBuf);//清空缓存数组
	 SIMCOM_Printf("%s\r\n",cmd);//发送字符命令
	 if(cmd==NULL)//如果没有特征字符串匹配的话函数直接返回成功，不执行字符串匹配检测
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
	    		 if(GSM_MG2639_check_cmd("ERROR",UsartBuf))
	    	   {
	    			  res=0;
             break;//得到有效数据 			 
	    	   }
					 if(GSM_MG2639_check_cmd("CLOSED",UsartBuf))
	    	   {
	    			  res=0;
             break;//得到有效数据 			 
	    	   }
	    	   if(GSM_MG2639_check_cmd("ALREADY CONNECT",UsartBuf))
	    	   {
             break;//得到有效数据 			 
	    	   }					 
	    	   if(GSM_MG2639_check_cmd(ack,UsartBuf))
	    	   {
             break;//得到有效数据 			 
	    	   }
           OSTimeDly(10);//延时10ms					 
	    	}
	    	if(waittime==0)
	    	{
	    		res=0; 
	    	}
	    }
		}
	return res;
}

/************************************************************************************************
*函数名   ： GSM_MG2639_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
*输入参数 ：cmd:要发送的AT指令字符串	ack：要匹配的特征字符串 waittime:匹配特征字符串需要等待的时间 单位10ms										
*输出参数 ：命令执行成功返回 1   失败返回 0  																				
*作用     ：发送不带回车符号AT指令 	  
*创建日期 ： 2018/12/06 	
*说明     ：
************************************************************************************************/
bool GSM_MG2639_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
{
	 u8 res=1;
   Clear_GSM_RX_BUF(UsartBuf);//清空缓存数组
	 SIMCOM_Printf("%s",cmd);//发送字符命令
	 if(cmd==NULL)//如果没有特征字符串匹配的话函数直接返回成功，不执行字符串匹配检测
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
	    		 if(GSM_MG2639_check_cmd("ERROR",UsartBuf))
	    	   {
	    			  res=0;
             break;//得到有效数据 			 
	    	   }
					 if(GSM_MG2639_check_cmd("CLOSED",UsartBuf))
	    	   {
	    			  res=0;
             break;//得到有效数据 			 
	    	   }
	    	   if(GSM_MG2639_check_cmd("ALREADY CONNECT",UsartBuf))
	    	   {
             break;//得到有效数据 			 
	    	   }					 
	    	   if(GSM_MG2639_check_cmd(ack,UsartBuf))
	    	   {
             break;//得到有效数据 			 
	    	   }
           OSTimeDly(10);//延时10ms					 
	    	}
	    	if(waittime==0)
	    	{
	    		res=0; 
	    	}
	    }
		}
	return res;
}

//检测数据是否发送成功
u8 GSM_MG2639_send_data_Status(char *wbuf, int wlen,char *ack,u16 waittime,u8 *UsartBuf)
{
  		u8 res=1;
      Clear_GSM_RX_BUF(UsartBuf);//清空缓存数组
		  Usart_SendStr_length(USART3,wbuf,wlen);//发送数据内容
	    waittime=waittime*10;
	   if(ack&&waittime)		//需要等待应答
	   {
	   	 while(--waittime)	//等待倒计时
	   	 { 	
	   	    if(GSM_MG2639_check_cmd(ack,UsartBuf))
	   	    {
             break;//得到有效数据 			 
	   	    }
					OSTimeDly(1);//延时1ms
	   	 }
	   	 if(waittime==0)
	   	 {
	   	 	res=0; 
	   	 }
	   
	   }
	   return res;
}

//模块向服务器发送数据
u8   GSM_MG2639_send_data(uint8_t ServerID,char *wbuf, int wlen,u16 waittime,uint8_t *UartBuf)//发送数据
{
	u8 res=1;
  char  cmd[32]={0};	
	sprintf(cmd,"AT+ZIPSEND=%d,%d\r\n",ServerID,wlen);
	if(GSM_MG2639_send_cmd(cmd,">",Ordinary_AT_WaitTime_n10ms,UartBuf))
	{
		 if(GSM_MG2639_send_data_Status(wbuf,wlen,"+ZIPSEND:OK",waittime,UartBuf))
		 {
			 	 printf("模块成功上传数据！！！\n");	
                res=1;
		 }
		 else
		 {
			          res=0;
		 }
	}
	else          res=0;

	return res;
}

/************************************************************************************************
*函数名   ：bool GSM_MG2639_AskCSQ(uint8_t *csq, uint8_t *UartBuf)														
*输入参数 ：uint8_t *csq  	,	信号强度uint8_t型指针										
*输出参数 ：	获取信号强度是否成功  																				
*作用     ：	 	  
*创建日期 ： 2018/12/06 	
*说明     ：
************************************************************************************************/
bool GSM_MG2639_AskCSQ(uint8_t *csq, uint8_t *UartBuf)
{
	char *p;
	bool ret = false;

	ret = GSM_MG2639_send_cmd("AT+CSQ", "+CSQ", Ordinary_AT_WaitTime_n10ms, UartBuf);
	if(ret == 0x00) //没及时返回则判为查询操作失败
	{
		*csq = 0;
		return false;
	}
	p = strstr((const char*)UartBuf, "+CSQ:"); //接收正确则返回后者在前者首次出现的首地址，否则返回NULL
	if(p != NULL)
	{
		ret = true;
		if((*(p+7)) == ',') //如果只有1位长度的信号强度
		{
			*csq = (*(p+6)-0x30);
		}
		else if((*(p+8)) == ',')//2位长度的信号强度
		{
			*csq = (*(p+6)-0x30)*10 + (*(p+7)-0x30); //将字符串对应数字位转为数字
		}
		else //其他格式则判别为接收失败
		{
			*csq = 0;
			ret = false;
		}
	}
	return ret;
}

bool GSM_MG2639_Echo(bool flag,u8 *UsartBuf)//是否设置GSM回显 flag:true代表回显 flag:false代表不回显
{
	 bool  ret=false; 
	if(flag==true)
	{
		  if(GSM_MG2639_send_cmd("ATE1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))      ret=true;   
	  
	}
	else
	{
	    if(GSM_MG2639_send_cmd("ATE0","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))       ret=true;    
	
	}
	return ret;
}
/************************************************************************************************
*函数名   ：bool GSM_MG2639_CheckNetwork(uint8_t *UartBuf)												
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：检查GSM模块是否已经注册上了网络;   true:代表注册上了  fasle:代表未注册上	  
*创建日期 ： 2018/12/10 	
*说明     ：
************************************************************************************************/
bool GSM_MG2639_CheckNetwork(uint8_t *UartBuf)
{
      bool  ret=false;
	
	   GSM_MG2639_send_cmd("AT+CREG=1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf);//设置检测注册网络状态时返回格式
	   //检查GSM模块是否已经注册上了网络
      if(GSM_MG2639_send_cmd("AT+CREG?","+CREG: 1, 1",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else
			{
		  	if(GSM_MG2639_send_cmd("AT+CREG?","+CREG: 1, 5",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf)) ret=true;
			}				

 return ret;
}
/************************************************************************************************
*函数名   ：bool GSM_MG2639_CheckGPRSActive(uint8_t *UartBuf)												
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：检查GSM模块是否已经附着上了GPRS网络;   true:代表注册上了  fasle:代表未附着上GPRS网络	  
*创建日期 ： 2018/12/10 	
*说明     ：
************************************************************************************************/
bool GSM_MG2639_CheckGPRSActive(uint8_t *UartBuf)
{
      bool  ret=false;
	    GSM_MG2639_send_cmd("AT+CGATT=1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf);//附着GPRS业务
	   //检查GSM模块是否已经附着上了GPRS网络
      if(GSM_MG2639_send_cmd("AT+CGATT?","+CGATT: 1",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else                                                                                ret=false;
						

    return ret;
}
/************************************************************************************************
*函数名   ：u8 GSM_MG2639_Open_Connect_Srever(char *ServerName,char *ServerPort,u16 waittime)															
*输入参数 ：											
*输出参数 ：	  该模块支持多链路模式  最多可以同时与5个服务器进行连接   ServerID:表示服务器ID号   ID号:1~5																				
*作用     ：与服务器建立TCP连接 	  
*创建日期 ： 2018/12/10 	
*说明     ：
************************************************************************************************/
u8 GSM_MG2639_Open_Connect_Srever(uint8_t ServerID,char *ServerName,char *ServerPort,u16 waittime,uint8_t *UartBuf)
{
	    u8  ret=0;
	   char	server[64];
	   char at[32];
	 if(GSM_MG2639_CheckNetwork(UartBuf))//先检查GSM模块是否已经注册上了网络
	 {
		 //1、打开GPRS数据链路
		 if(GSM_MG2639_send_cmd("AT+ZPPPOPEN","+ZPPPOPEN:CONNECTED",Mg2639_Open_GPRSdatalink_n10ms,(u8*)UartBuf))
		 {
			 //2、查询数据链路连接状态
			 		  sprintf(at,"AT+ZIPSTATUS=%d",ServerID);
	     	if(!GSM_MG2639_send_cmd(at,"+ZIPSTATUS: ESTABLISHED",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))
	     	{
	     		sprintf(server,"AT+ZIPSETUP=%d,%s,%s",ServerID,ServerName,ServerPort);//IP连接
	         if(GSM_MG2639_send_cmd(server,"+ZIPSETUP:CONNECTED",waittime,(u8*)UartBuf))        ret=1;	 				
	     		else                                                                                ret=0;
	     	}                                                                    
	     	else                                                                                  ret=1;	
	      
	   }
	    else                                                                                    ret=0;
	      
       return ret;
	}
	else
	 {
	    ret=0;
	 }
		 return ret;                                                            
}


/***************************GSM电源操作函数***************************/
void GSM_MG2639_Io_Init(void)//GSM模块相关IO口初始化	
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
		//GSM电源控制脚
	  RCC_APB2PeriphClockCmd(GSM_MG2639_PWRCTRL_CLK, ENABLE);
	  GPIO_InitStructure.GPIO_Pin		  = GSM_MG2639_PWRCTRL_PIN;
	  GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  GPIO_Init(GSM_MG2639_PWRCTRL_PORT, &GPIO_InitStructure);
	  GSM_MG2639_Power(OFF);//默认关闭GSM模块电源
			//GSM开机控制脚
	  RCC_APB2PeriphClockCmd(GSM_MG2639_ONOFF_CLK, ENABLE);
	  GPIO_InitStructure.GPIO_Pin		  = GSM_MG2639_ONOFF_PIN;
	  GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  GPIO_Init(GSM_MG2639_ONOFF_PORT, &GPIO_InitStructure);
		GSM_MG2639_OnOff(ON);//默认GSM模块处于关机状态
}

static void MG2639_Boot(void)//GSM模块开机和关机
{
	GSM_MG2639_OnOff(ON);
	OSTimeDly(3000);//延时3s
	GSM_MG2639_OnOff(OFF);
}
void GSM_MG2639_Power_Up(void)//打开GSM模块电源	
{
   GSM_MG2639_Power(ON);
	 OSTimeDly(300);//延时300ms以便系统电源稳定
	 MG2639_Boot();//GSM模块开机和关机
}	
void GSM_MG2639_Power_Down(void)//关闭GSM模块电源
{
	 MG2639_Boot();//GSM模块开机和关机
	 OSTimeDly(300);//延时300ms以便系统电源稳定
   GSM_MG2639_Power(OFF);

}
bool GSM_MG2639_Power_Rest(void)//重启GSM模块
{
	 bool ret;
	  ret=true;
  GSM_MG2639_Power_Down();//关闭GSM模块电源
	OSTimeDly(300);//延时300ms
  GSM_MG2639_Power_Up();//打开GSM模块电源
	 return ret;
}

/************************************************************************************************
*函数名   ：bool Gsm_Check_SIm_Card(uint8_t *UartBuf)												
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：检查SIM卡是否在位;   true:代表在位  fasle:代表不在位 	  
*创建日期 ： 2018/12/06 	
*说明     ：
************************************************************************************************/
bool GSM_MG2639_Check_SIm_Card(uint8_t *UartBuf)
{
	    bool  ret=false;
	   //检查SIM卡是否在位
      if(GSM_MG2639_send_cmd("AT+CPIN?","+CPIN: READY",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else                                                                                         ret=false;
      
 return ret;
}
bool    GSM_MG2639_Send_AT(u8 *UsartBuf)//向模块发送"AT"
{
    bool  ret=false; 
	
	 if(GSM_MG2639_send_cmd("AT","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))            ret=true;   
	
	  return ret;
}
  /* 自适应波特率 */
	int	GSM_MG2639_Adaptive_BaudRate(u8 *UartBuf)
	{
		/* 波特率码表 */
		int baudratetab[7] = {9600,38400,115200,19200,14400,56000,57600};
		/* 计数器 */
		int num0;
		
		/* 循环查询 */
		for(num0 = 0;num0 < 7;num0++)
		{
			/* 设置波特率 */
		 GSM_USART_Init(baudratetab[num0]);//GSM模块串口初始化  设置波特率为9600
			/* 发送AT指令 */
			if(GSM_MG2639_send_cmd("AT","OK",Ordinary_AT_WaitTime_n10ms,UartBuf) == true)
			{
				/* 当前波特率为模块通信波特率 */
				return baudratetab[num0];
			}
		}
		
		return 0;
	}	
 bool	GSM_MG2639_SetBaudRate(int br,u8 *UartBuf)//设置模块波特率
    {
        char	at[32];
        sprintf(at, "AT+IPR=%d\r\n", br);
        if(GSM_MG2639_send_cmd(at, "OK", Ordinary_AT_WaitTime_n10ms,UartBuf) == false)
        {
            return false;
        }
        return true;
    }
#endif
