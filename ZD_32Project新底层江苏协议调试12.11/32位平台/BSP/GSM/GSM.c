#include "Gsm_Api.h"
#if (GSM_Module_Type==0x00) //SIM800C模块
#include "GSM.h"
#include "ucos_ii.h"
#include <stdio.h> 
#include <string.h>
#include <stdbool.h>
#include "string_utility.h"

/************************************************************************************************
*函数名   ：u8* Gsm_check_cmd(char *str,u8 *UsartBuf)															
*输入参数 ：str :要寻找的目标字符串											
*输出参数 ：	  																				
*作用     ：查找特征字符串 	  
*创建日期 ： 2018/08/23	
*说明     ： //发送命令后,检测接收到的应答
//str:期待的应答结果  返回值:0,没有得到期待的应答结果 其他,期待应答结果的位置(str的位置)
************************************************************************************************/
bool   Gsm_check_cmd(char *str,u8 *UsartBuf)
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
*函数名   ：u8 Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)											
*输入参数 ：cmd:要发送的AT指令字符串	ack：要匹配的特征字符串 waittime:匹配特征字符串需要等待的时间 单位10ms										
*输出参数 ：命令执行成功返回 1   失败返回 0  																				
*作用     ：发送AT指令 	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
bool Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
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
	    		 if(Gsm_check_cmd("ERROR",UsartBuf))
	    	   {
	    			  res=0;
             break;//得到有效数据 			 
	    	   }
					 if(Gsm_check_cmd("CLOSED",UsartBuf))
	    	   {
	    			  res=0;
             break;//得到有效数据 			 
	    	   }
	    	   if(Gsm_check_cmd("ALREADY CONNECT",UsartBuf))
	    	   {
             break;//得到有效数据 			 
	    	   }					 
	    	   if(Gsm_check_cmd(ack,UsartBuf))
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
*函数名   ： Gsm_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
*输入参数 ：cmd:要发送的AT指令字符串	ack：要匹配的特征字符串 waittime:匹配特征字符串需要等待的时间 单位10ms										
*输出参数 ：命令执行成功返回 1   失败返回 0  																				
*作用     ：发送不带回车符号AT指令 	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
bool Gsm_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf)
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
	    		 if(Gsm_check_cmd("ERROR",UsartBuf))
	    	   {
	    			  res=0;
             break;//得到有效数据 			 
	    	   }
					 if(Gsm_check_cmd("CLOSED",UsartBuf))
	    	   {
	    			  res=0;
             break;//得到有效数据 			 
	    	   }
	    	   if(Gsm_check_cmd("ALREADY CONNECT",UsartBuf))
	    	   {
             break;//得到有效数据 			 
	    	   }					 
	    	   if(Gsm_check_cmd(ack,UsartBuf))
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

u8 Gsm_send_data_Status(char *wbuf, int wlen,char *ack,u16 waittime,u8 *UsartBuf)
{
  		u8 res=1;
      Clear_GSM_RX_BUF(UsartBuf);//清空缓存数组
		  Usart_SendStr_length(USART3,wbuf,wlen);//发送数据内容
	    waittime=waittime*10;
	   if(ack&&waittime)		//需要等待应答
	   {
	   	 while(--waittime)	//等待倒计时
	   	 { 	
	   	    if(Gsm_check_cmd(ack,UsartBuf))
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




/************************************************************************************************
*函数名   ：bool GSM_AskCSQ(uint8_t *csq, uint8_t *UartBuf)														
*输入参数 ：uint8_t *csq  	,	信号强度uint8_t型指针										
*输出参数 ：	获取信号强度是否成功  																				
*作用     ：	 	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
bool Gsm_AskCSQ(uint8_t *csq, uint8_t *UartBuf)
{
	char *p;
	bool ret = false;

	ret = Gsm_send_cmd("AT+CSQ", "+CSQ", Ordinary_AT_WaitTime_n10ms, UartBuf);
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





u8 Gsm_send_data(char *wbuf, int wlen,u16 waittime,uint8_t *UartBuf)//发送数据
{
	u8 res=1;
  char  cmd[32]={0};	
	sprintf(cmd,"AT+CIPSEND=%d\r\n",wlen);
	if(Gsm_send_cmd(cmd,">",Ordinary_AT_WaitTime_n10ms,UartBuf))
	{
		 if(Gsm_send_data_Status(wbuf,wlen,"SEND OK",waittime,UartBuf))
		 {
//			 	 Log_printf("模块成功上传数据！！！\n");	
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

static bool  Gsm_check_Rev_Data(int *Location1_0x7E, int *Location2_0x7E,int rlen,u8 *UsartBuf)//检查接收到的数据是否合法
{
	   bool  ret=false; 
	   int i=0;
//Location1_0x7E:0x7E第一次出现的位置;Location2_0x7E:0x7E第二次出现的位置;
		for(i=0;i<rlen;i++)//查找第一次0x7E出现的位置
		{
		  if(UsartBuf[i]==0x7E)
			{
			  *Location1_0x7E=i;
				 ret=true;
				  break;
			}
			else
			{
			   ret=false;
			}
		}
		
		if(ret)
		{
		   for(i=*Location1_0x7E;i<rlen;i++)//查找第二次次0x7E出现的位置
		   {
		    if(UsartBuf[i+1]==0x7E)
		   	 {
		   	   *Location2_0x7E=i+1;
				 	  ret=true;
		   	 	  break;
		   	 }
			  else
			   {
			      ret=false;
			   }
				
		   }
		}
		else
		{
		  ret=false;
		}
	

	 return ret;
}

u8 Gsm_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf)//接收数据
{
      u8 res=1;
	int Location1=0,Location2=0;//Location1:0x7E第一次出现的位置;Location2:0x7E第二次出现的位置;
//      Clear_GSM_RX_BUF(UartBuf);//清空缓存数组
	 //1、找0x7E　
	   if(waittime)		//需要等待应答
	   {
	   	 while(--waittime)	//等待倒计时
	   	 { 
				 /*检查接收到的数据是否合法*/
	   	    if(Gsm_check_Rev_Data(&Location1,&Location2,GSM_RX_BUF_LEN,UartBuf))
	   	    {
						*rlen=(Location2-Location1)+1;
						memcpy(rbuf,&UartBuf[Location1],*rlen);//拷贝数据
             break;//得到有效数据 			 
	   	    }
					OSTimeDly(10);//延时10ms
	   	 }
	   	 if(waittime==0)
	   	 {
	   	 	res=0; 
	   	 }
	   }
	   return res;
} 



/************************************************************************************************
*函数名   ：u8 GSM_Open_Connect_Srever(char *ServerName,char *ServerPort,u16 waittime)															
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：与服务器建立TCP连接 	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
u8 Gsm_Open_Connect_Srever(char *ServerName,char *ServerPort,u16 waittime,uint8_t *UartBuf)
{
	    u8  ret=0;
	   char	server[64];
	 if(Gsm_CheckNetwork(UartBuf))//先检查GSM模块是否已经注册上了网络
	 {
		if(!Gsm_send_cmd("AT+CIPSTATUS","STATE: CONNECT OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))
		{
			sprintf(server,"AT+CIPSTART=\"%s\",\"%s\",\"%s\"\r\n","TCP",ServerName,ServerPort);//IP连接
	    if(Gsm_send_cmd(server,"CONNECT OK",waittime,(u8*)UartBuf))        ret=1;	 				
			else                                                               ret=0;
		}                                                                    
		else                                                                 ret=1;	
	 
	 }
	 else                                                                  ret=0;
 return ret;                                                             
}
/************************************************************************************************
*函数名   ：bool Gsm_Close_Connect_Srever(uint8_t *UartBuf)													
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：关闭与服务器的TCP连接 	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
bool Gsm_Close_Connect_Srever(uint8_t *UartBuf)
{
	    bool  ret=false;
	   //关闭TCP连接 
      if(Gsm_send_cmd("AT+CIPCLOSE=1\r\n","CLOSE OK",Close_Server_AT_WaitTime_n10ms,(u8*)UartBuf)) ret=true;	 
			else                                                              ret=false;
      
 return ret;
}
/************************************************************************************************
*函数名   ：bool Gsm_Check_SIm_Card(uint8_t *UartBuf)												
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：检查SIM卡是否在位;   true:代表在位  fasle:代表不在位 	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
bool Gsm_Check_SIm_Card(uint8_t *UartBuf)
{
	    bool  ret=false;
	   //检查SIM卡是否在位
      if(Gsm_send_cmd("AT+CPIN?","+CPIN: READY",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else                                                                                  ret=false;
      
 return ret;
}
/************************************************************************************************
*函数名   ：bool Gsm_CheckNetwork(uint8_t *UartBuf)												
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：检查GSM模块是否已经注册上了网络;   true:代表注册上了  fasle:代表未注册上	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
bool Gsm_CheckNetwork(uint8_t *UartBuf)
{
      bool  ret=false;
	
	   Gsm_send_cmd("AT+CREG=1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf);//设置检测注册网络状态时返回格式
	
	
	   //检查GSM模块是否已经注册上了网络
      if(Gsm_send_cmd("AT+CREG?","+CREG: 1,1",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else
			{
		  	if(Gsm_send_cmd("AT+CREG?","+CREG: 1,5",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf)) ret=true;
			}				

 return ret;
}
/************************************************************************************************
*函数名   ：bool Gsm_CheckGPRSActive(uint8_t *UartBuf)												
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：检查GSM模块是否已经附着上了GPRS网络;   true:代表注册上了  fasle:代表未附着上GPRS网络	  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
bool Gsm_CheckGPRSActive(uint8_t *UartBuf)
{
      bool  ret=false;
	    Gsm_send_cmd("AT+CGATT=1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf);//附着GPRS业务
	   //检查GSM模块是否已经附着上了GPRS网络
      if(Gsm_send_cmd("AT+CGATT?","+CGATT: 1",Ordinary_AT_WaitTime_n10ms,(u8*)UartBuf))   ret=true;	 
			else                                                                                ret=false;
						

    return ret;
}

/************************************************************************************************
*函数名   ：bool Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf)											
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：SIM800C发送短信
*创建日期 ： 2018/11/26 	
*说明     ：
************************************************************************************************/
bool     Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf)//发送短信内容
{
	        // 经特定编码后的短信内容(中文采用UCS2编码、英文采用7bit编码)
        unsigned char	    encoded[512]={0};//存放短信内容经过编码的内容	
				unsigned char	    Pn_encoded[64]={0};//报警号码经过编码的内容
				int				        SmsContentLen = 0;//短信内容编码成unicode码后的长度
        char              converbuff[512]={0};//最大发送512个字节内容
	      char              cmdbuff[64]={0};//发送命令缓冲区
	      char              end= 0x1A;//结束命令
	     uint8_t            testSend = 0;
	//1、检测短信号码和短息内容是否为空
	  if((Utils_strlen(num)<=0)||(Utils_strlen(smstext)<=0)){
		    goto sms_failure;
		}
	
	
    if(IsASSIC(smstext)==true)
    {
			//英文
			//"GSM"字符集
			if(!Gsm_send_no_entercmd("AT+CSCS=\"GSM\"\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

			//文本模式
			if(!Gsm_send_no_entercmd("AT+CMGF=1\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

			//生成电话号码命令 
			sprintf(cmdbuff,"AT+CMGS=\"%s\"\r",num);
			//复制短信内容
			strcpy(converbuff,smstext);

    }
    else
    {
			//中文
			//"UCS2"字符集
			if(!Gsm_send_no_entercmd("AT+CSCS=\"UCS2\"\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;
			//文本模式
			if(!Gsm_send_no_entercmd("AT+CMGF=1\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

			if(!Gsm_send_no_entercmd("AT+CSMP=17,167,0,8\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf) ) goto sms_failure;

			 Utils_pdu_encode16bit(Pn_encoded, num, strlen(num));//对短信号码进行unicode编码	
			 Utils_Hex2Str((char*)Pn_encoded,converbuff,(strlen(num)*2));//将Unicode码转化为字符串(报警手机号码的 Unicode码的字符串)	
			
			 sprintf(cmdbuff,"AT+CMGS=\"%s\"\r",converbuff);  //组装电话号码Unicode编码字符串
			
			 SmsContentLen=Utils_pdu_encode16bit(encoded, smstext,Utils_strlen(smstext));//对中文内容进行unicode编码
			 Utils_Hex2Str((char*)encoded,converbuff,SmsContentLen);//将Unicode码转化为字符串(短信内容的 Unicode码的字符串)	

    }
		
	//发送电话号码	
	if(Gsm_send_cmd(cmdbuff,">",Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf))
	{
		SIMCOM_Printf("%s",converbuff);//发送短信内容
		
		Gsm_send_cmd(&end,0,Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf);		//0x1A结束符
		
		//检测是否发送完成
		while(Gsm_check_cmd("OK",(u8*)UartBuf) != true )
		{		
			if(++testSend >100)//最长等待10秒  (100次  每次100ms)
			{
				goto sms_failure;
			}
			OSTimeDly(Send_Sms_AT_WaitTime_n10ms);//延时100ms				
		}	
		return true;
	}
	else
	{				
sms_failure:		
		end = 0x1B;
		Gsm_send_cmd(&end,0,0,(u8*)UartBuf);	//ESC,取消发送 
		return false;		
	}		
}
///////************************************************************************************************
//////*函数名   ：bool Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf)											
//////*输入参数 ：											
//////*输出参数 ：	  																				
//////*作用     ：SIM800C发送短信
//////*创建日期 ： 2018/11/26 	
//////*说明     ：
//////************************************************************************************************/
//////bool     Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf)//发送短信内容
//////{
////////////	  bool  ret=false;
//////    char converbuff[512];//最大发送512个字节内容
//////	  char cmdbuff[80];//发送命令缓冲区
//////	  char end= 0x1A;//结束命令
//////	  uint8_t testSend = 0;
//////	
//////    if(IsASSIC(smstext)==true)
//////    {
//////			//英文
//////			//"GSM"字符集
//////			if(!Gsm_send_no_entercmd("AT+CSCS=\"GSM\"\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

//////			//文本模式
//////			if(!Gsm_send_no_entercmd("AT+CMGF=1\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

//////			//生成电话号码命令 
//////			sprintf(cmdbuff,"AT+CMGS=\"%s\"\r",num);
//////			//复制短信内容
//////			strcpy(converbuff,smstext);

//////    }
//////    else
//////    {
//////			//中文
//////			//"UCS2"字符集
//////			if(!Gsm_send_no_entercmd("AT+CSCS=\"UCS2\"\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;
//////			//文本模式
//////			if(!Gsm_send_no_entercmd("AT+CMGF=1\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf)) goto sms_failure;

//////			if(!Gsm_send_no_entercmd("AT+CSMP=17,167,0,8\r","OK", Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf) ) goto sms_failure;

//////			//生成电话号码命令，UCS2的电话号码(需要转成 ucs2码)
//////			gsm_gbk2ucs2hex(converbuff,num);//转换为UCS2
//////			sprintf(cmdbuff,"AT+CMGS=\"%s\"\r",converbuff);  
//////			
//////			//转换短信内容
//////			gsm_gbk2ucs2hex(converbuff,smstext);
//////    }
//////		
//////	//发送电话号码	
//////	if(Gsm_send_cmd(cmdbuff,">",Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf))
//////	{
//////		SIMCOM_Printf("%s",converbuff);//发送短信内容
//////		
//////		Gsm_send_cmd(&end,0,Send_Sms_AT_WaitTime_n10ms,(u8*)UartBuf);		//0x1A结束符
//////		
//////		//检测是否发送完成
//////		while(Gsm_check_cmd("OK",(u8*)UartBuf) != true )
//////		{		
//////			if(++testSend >100)//最长等待10秒  (100次  每次100ms)
//////			{
//////				goto sms_failure;
//////			}
//////			OSTimeDly(Send_Sms_AT_WaitTime_n10ms);//延时100ms				
//////		}	
//////		return true;
//////	}
//////	else
//////	{				
//////sms_failure:		
//////		end = 0x1B;
//////		Gsm_send_cmd(&end,0,0,(u8*)UartBuf);	//ESC,取消发送 
//////		return false;		
//////	}		
//////}




/************************************************************************************************
*函数名   ：void GSM_Init(void)														
*输入参数 ：											
*输出参数 ：	  																				
*作用     ：GSM(sim800C)硬件初始化  
*创建日期 ： 2018/08/23 	
*说明     ：
************************************************************************************************/
//void Gsm_Init(void)
//{
// 	Gsm_Power_Down();//关闭GSM模块电源
//  Gsm_Power_Up();//打开GSM模块电源	
//	Gsm_Boot();//GSM模块开机
//}

/***************************GSM电源操作函数***************************/
void Gsm_Io_Init(void)//GSM模块相关IO口初始化	
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
		//GSM电源控制脚
	  RCC_APB2PeriphClockCmd(GSM_PWRCTRL_CLK, ENABLE);
	  GPIO_InitStructure.GPIO_Pin		  = GSM_PWRCTRL_PIN;
	  GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  GPIO_Init(GSM_PWRCTRL_PORT, &GPIO_InitStructure);
	  GSM_Power(OFF);//默认关闭GSM模块电源
			//GSM开机控制脚
	  RCC_APB2PeriphClockCmd(GSM_ONOFF_CLK, ENABLE);
	  GPIO_InitStructure.GPIO_Pin		  = GSM_ONOFF_PIN;
	  GPIO_InitStructure.GPIO_Mode  	= GPIO_Mode_Out_PP;  
	  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	  GPIO_Init(GSM_ONOFF_PORT, &GPIO_InitStructure);
		GSM_OnOff(ON);//默认GSM模块处于关机状态
}

static void Gsm_Boot(void)//GSM模块开机和关机
{
	GSM_OnOff(ON);
	OSTimeDly(3000);//延时3s
	GSM_OnOff(OFF);
}
void Gsm_Power_Up(void)//打开GSM模块电源	
{
   GSM_Power(ON);
	 OSTimeDly(300);//延时300ms以便系统电源稳定
	 Gsm_Boot();//GSM模块开机和关机
}	
void Gsm_Power_Down(void)//关闭GSM模块电源
{
	 Gsm_Boot();//GSM模块开机和关机
	 OSTimeDly(300);//延时300ms以便系统电源稳定
   GSM_Power(OFF);

}
bool Gsm_Power_Rest(void)//重启GSM模块
{
	 bool ret;
	  ret=true;
  Gsm_Power_Down();//关闭GSM模块电源
	OSTimeDly(300);//延时300ms
  Gsm_Power_Up();//打开GSM模块电源
	
	 return ret;
}

bool Gsm_Echo(bool flag,u8 *UsartBuf)//是否设置GSM回显 flag:true代表回显 flag:false代表不回显
{
	 bool  ret=false; 
	if(flag==true)
	{
		  if( Gsm_send_cmd("ATE1","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))      ret=true;   
	  
	}
	else
	{
	    if( Gsm_send_cmd("ATE0","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))       ret=true;    
	
	}
	return ret;
}
bool    Gsm_Send_AT(u8 *UsartBuf)//向模块发送"AT"
{
    bool  ret=false; 
	
	 if( Gsm_send_cmd("AT","OK",Ordinary_AT_WaitTime_n10ms,(u8*)UsartBuf))            ret=true;   
	
	  return ret;
}


#endif



