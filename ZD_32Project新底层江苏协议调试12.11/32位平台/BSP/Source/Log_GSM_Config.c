#include "Log_GSM_Config.h"
#include <stdio.h>
#if (GSM_Module_Type==0x02) //ESP8266Wifi模块

//GSM模块配置数据寄存器
GSM_Config_Struct    GSM_Config_Reg =
{
   FALSE,//默认GSM没有在进行配置
	 FALSE,//默认不配置Wifi模块的名字及密码
	 FALSE,//默认不配置Wifi模块的Wlan参数
	 FALSE,//默认不配置Wifi模块获取IP地址方式
	 FALSE,//默认不对Wifi模块进行复位操作
	 FALSE,//默认不对Wifi模块进行恢复出厂设置
};




/* 设置Wifi名字和密码*/
static GSM_Config_QUE    GSM_Wifi_QUE_SetWifiNamePWD	                 = {WifiNamePWD_EVENT,	             ((u32*)0X0),TRUE};
/* 设置WifiWlan参数*/
static GSM_Config_QUE    GSM_Wifi_QUE_SetWifiWLAN                      = {WifiWLAN_EVENT,	                 ((u32*)0X0),TRUE};
/* 设置Wifi模块获取IP地址方式   STATIC或者DHCP模式参数*/
static GSM_Config_QUE    GSM_Wifi_QUE_GET_IP_WAY                       = {WifiWLAN_GET_IP_WAY_EVENT,	      ((u32*)0X0),TRUE};
/*复位Wifi模块*/
static GSM_Config_QUE    GSM_Wifi_QUE_Reset                            = {WifiRest_EVENT,	                  ((u32*)0X0),TRUE};
/* Wifi模块恢复出厂设置*/
static GSM_Config_QUE    GSM_Wifi_QUE_Factory_Data_Reset               = {Factory_Data_Wifi_EVENT,	        ((u32*)0X0),TRUE};
#endif	
/************************************************************************************************/
/* 函数名	：GSM_Config_Function																		*/
/* 输入		：gsm_que_inf:当前的消息队列															*/
/* 输出		：无 																				*/
/* 作用		：执行消息队列 当中的 消息功能			 								 		 		*/
/* 创建日期	：2018/11/21																		*/
/************************************************************************************************/
void GSM_Config_Function(GSM_Config_QUE*  gsm_config_que_inf,INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	
      switch(gsm_config_que_inf->EVENT_Cmd)
			{
			
			  case  WifiNamePWD_EVENT :/* 配置Wifi名字和密码 事件 */
				{
				    printf("接收到配置Wifi名字和密码 事件\n");
				
				     break;
				}
			  case  WifiWLAN_EVENT :/* 配置WLAN参数(本地IP、子网掩码、网关地址) 事件 */
				{
				    printf("接收到配置WLAN参数(本地IP、子网掩码、网关地址) 事件\n");
				     break;
				}
			 case  WifiWLAN_GET_IP_WAY_EVENT :/* 配置Wifi IP地址获取方式 STATIC或者DHCP模式事件 */
				{
				    printf("接收到配置WifiIP地址获取方式 STATIC或者DHCP模式 事件\n");
				     break;
				}
			 case  WifiRest_EVENT :	/* 复位Wifi模块事件 */
				{
				    printf("接收到复位Wifi模块事件\n");
				     break;
				}
				case  Factory_Data_Wifi_EVENT :	/* Wifi模块恢复出厂设置事件 */
				{
				    printf("接收到配置Wifi模块恢复出厂设置事件\n");
				     break;
				}
				default:
				{
				
				  break;
				}
			
			}
				/* 消息执行完成 */
	gsm_config_que_inf->EVENT_Getarg = TRUE;
}

/*
*********************************************************************************************************
*	函 数 名: GSM_Send_Q
*	功能说明: 发送 GSM 消息队列
*	形    参：pevent:消息队列的指针，GSM_Config_TYPE:消息队列类型
*	返 回 值: 无
*********************************************************************************************************
*/
BOOL GSM_Config_Send_Q(OS_EVENT* pevent,GSM_Config_TYPE EVENT_Type)
{
	INT8U     	            err;
	OS_Q_DATA	           Q_data;
	GSM_Config_QUE*	    EVENT_p;	
	u16 		                num;
	u32 		                Q_p;
#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	
	if(EVENT_Type == WifiNamePWD_EVENT)
	{
		/* 设置Wifi名字和密码 */		
		EVENT_p = &GSM_Wifi_QUE_SetWifiNamePWD;		
	}
	else if(EVENT_Type == WifiWLAN_EVENT)
	{
		/* 设置WifiWlan参数 */		
		EVENT_p = &GSM_Wifi_QUE_SetWifiWLAN;		
	}
	else if(EVENT_Type == WifiWLAN_GET_IP_WAY_EVENT)
	{
		/* 设置Wifi模块IP获取方式参数 */		
		EVENT_p = &GSM_Wifi_QUE_GET_IP_WAY;		
	}
	else if(EVENT_Type == WifiRest_EVENT)
	{
		/*复位Wifi模块*/		
		EVENT_p = &GSM_Wifi_QUE_Reset;		
	}	
	else if(EVENT_Type == Factory_Data_Wifi_EVENT)
	{
		/* Wifi模块恢复出厂设置 */		
		EVENT_p = &GSM_Wifi_QUE_Factory_Data_Reset;		
	}		

#endif	
	
	/* 程序报道这里 说明程序中不存在 上传数据消息 */
	/* 判断 上传数据 执行结果 */
	if(EVENT_p->EVENT_Getarg == FALSE)
	{
		/*数据消息 正在 被执行，等待执行结束 */
		return FALSE;
	}
	
	//由于 内存空间有限 这里的消息只有一个数据空间，因此，在发送消息前需要确定上次的消息已经被执行了。
	//否则，同一种消息将占据2个消息空间，导致其他消息无法进入。
	/* 检查消息队列中是否存在 更新 附着状态 消息。 */
	err = OSQQuery(pevent,&Q_data);
	
	/* 判断查询结果 */
	if(err == OS_ERR_NONE)
	{
		/* 查询成功 */
		
		/* 查找 队列中是否已经存在 附着状态 消息。 */
		for(num = 0,Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQOut);
			num < Q_data.OSNMsgs;
			num ++)
		{
			/* 判断单前的消息指针与发送指针是否相同 */
			if((*(u32*)Q_p) == (u32)EVENT_p)
			{
				/* 相等，队列中存在 附着状态 消息,那么就不在发送 附着状态 消息 */
				return FALSE;
			}
			/* 设置指针到下一个位置 */
			Q_p += sizeof(GSM_Config_QUE);
			/* 判断指针是否越界 */
			if(Q_p == (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQEnd))
			{
				/* 越界就将 指针 指向头位置 */
				Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQStart);
			}
		}
	}else
	{
		/* 查询失败 */
		return FALSE;
	}

	/* 清除 消息 执行结果 */
	EVENT_p->EVENT_Getarg = FALSE;
	/* 发送 消息 *///FIFO	  先进先出
	err = OSQPost(pevent, EVENT_p);  //发送消息值
	/* 判断 消息 发送 状态 */
	if(err == OS_ERR_Q_FULL)
	{
		/* 消息队列已满,不能再接收新消息 */
		/* 这种情况 不可能 出现 */
		return FALSE;
	}
	/* 判断 消息 发送 成功 */
	return TRUE;
}
