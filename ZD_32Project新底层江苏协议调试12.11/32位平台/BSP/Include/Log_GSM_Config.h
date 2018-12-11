#ifndef __Log_GSM_Config_H
#define __Log_GSM_Config_H
#include "stm32f10x.h"
#include "ucos_ii.h"
#include "Mcu_Config.h"
#include "Gsm_Api.h"
/* GSM的 消息体的 消息数据类型(1 Byte) */
typedef enum {
	
	/* 配置Wifi名字和密码 事件 */
	WifiNamePWD_EVENT		                 = 0X00,
	/* 配置WLAN参数(本地IP、子网掩码、网关地址) 事件 */
	WifiWLAN_EVENT		                   = 0X01,
	/* 配置Wifi IP地址获取方式 STATIC或者DHCP模式事件 */
	WifiWLAN_GET_IP_WAY_EVENT		         = 0X02,
	/* 复位Wifi模块事件 */
	WifiRest_EVENT		                   = 0X03,
	/* Wifi模块事件 */
	Factory_Data_Wifi_EVENT		           = 0X04,
}GSM_Config_TYPE;

typedef struct
{
	GSM_Config_TYPE	           EVENT_Cmd;		// 事件 命令
	u32*			                 EVENT_Str;		// 事件 数据 指针
	BOOL			                 EVENT_Getarg;	// 事件 执行结果(FALSE:信号正在执行，TRUE:型号执行结束)
}GSM_Config_QUE;//GSM模块消息队列结构体类型


typedef struct
{
   BOOL   GSM_Config_State;//GSM_Config_State=FALSE代表不需要配置GSM模块参数     GSM_Config_State=TRUE代表需要配置GSM模块参数
   BOOL   WifiNamePWD_Config_State;//配置Wifi模块的名称及密码状态标志
	 BOOL   WifiWLAN_Config_State;//配置Wifi模块的WLAN参数状态标志
	 BOOL   WifiWLAN_GET_IP_WAY_Config_State;//配置Wifi模块获取IP地址方式状态标志
	 BOOL   WifiRest_Config_State;//复位Wifi模块状态标志
   BOOL   WifiFactory_Data_Config_State;//Wifi模块恢复出厂设置状态标志
}GSM_Config_Struct;


extern  GSM_Config_Struct    GSM_Config_Reg;//GSM模块配置数据寄存器



void    GSM_Config_Function(GSM_Config_QUE*  gsm_config_que_inf,INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);

BOOL    GSM_Config_Send_Q(OS_EVENT* pevent,GSM_Config_TYPE EVENT_Type);

#endif

