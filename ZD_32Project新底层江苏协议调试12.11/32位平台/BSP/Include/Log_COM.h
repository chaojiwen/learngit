#ifndef __Log_COM_H
#define __Log_COM_H
			  	 
#include "stm32f10x.h"	     
#include "Mcu_Config.h"

#include "Gsm_Api.h"

/************************************************************************************************/
/* 传感器校准通信协议 数据大小 */
#define Com_Sensor_Cal_Data_Sise					(60u)
/* 传感器校准通信协议 数据结构 */
typedef struct {
/* 第一个参数 头码 */
	u8	Head_Code;
/* 第二个参数 设备地址(我感觉叫仪器编号) */
	u8	Instru_NO;
/* 第三个参数 操作码（操作命令） */	
	u8	Cmd_Code;
/* 第四个参数 通道号 */
	u8	Chanel_Num;
/* 第五个参数 数据长度 */
	u8	Data_Len;
/* 第六个参数 数据 */
	u8	Com_Data[Com_Sensor_Cal_Data_Sise];
/* 第七个参数 常数0X66（这个用来断数据帧用） */
	u8	Constant;
/* 第八个参数 校验 */
	u8	Check;
}COM_SENSOR_CAL_TYPEDEF;

/***********************串口1配置值传感器校准数据的结构和变量 声明和定义*******************************/
/* 传感器校准 发送 头码 */
#define Com_Sensor_Cal_Send_Head_Code				          (0X25)
/* 传感器校准 接收 头码 */
#define Com_Sensor_Cal_Rece_Head_Code				          (0X3c)
/* 传感器校准 常数 */
#define Com_Sensor_Cal_Constant						            (0X66)
/* 传感器校准 读实时数据 的通道编码 */
#define Com_Sensor_Cal_Read_Real_Data_CH_NO			      (0XFF)
/* 传感器校准 通信数据缓存大小 */
#define Com_Sensor_Cal_Data_Buffer_Size				 (INSTRU_SENSOR_Data_Tab_Size*2*2)//8个校准点*每个校准点2字节*每个通道2调曲线
/* 传感器校准 数据帧固定数据长度 */
#define Com_Sensor_Cal_Fix_Data_Len					          (7u)
/* 传感器校准 通信数据最大长度 */
#define Com_Sensor_Cal_Packet_Len					     (Com_Sensor_Cal_Data_Sise+Com_Sensor_Cal_Fix_Data_Len)
/* 传感器校准 读写曲线 数据长度 */
#define Com_Sensor_Cal_Curve_Data_Len				   (INSTRU_SENSOR_Data_Tab_Size*2)
/* 传感器校准 通信 超时时间 */
#define Com_Sensor_Cal_Timeout						            (600u)
/* 传感器校准 参数设置 成功 */
#define Com_Sensor_Cal_Set_Para_Succeed				        (0X55)
/* 传感器校准 参数设置 失败 */
#define Com_Sensor_Cal_Set_Para_Fail				          (0XAA)
/* 传感器校准 操作码 */


#if (GSM_Module_Type==2) //ESP8266Wifi模块
/**************ESP8266Wifi模块相关操作码**********************/
/* 读和写Wifi名称操作码 通过操作码后面的一个字节数据来区分读和写 0x55表示读取  0xAA表示写入 WSSSID*/
#define Com_Read_Write_Wifi_Name_Cmd_Code	    (0X20)
/* 读和写Wifi的WSKEY相关参数(加密方式和密码)操作码 通过操作码后面的一个字节数据来区分读和写 0x55表示读取  0xAA表示写入 WSSSID*/
#define Com_Read_Write_Wifi_WSKEY_Cmd_Code    (0X21)
/* 读和写Wifi的WANN操作码相关参数(本地IP地址、子网掩码、网关IP地址 )通过操作码后面的一个字节数据来区分读和写 0x55表示读取  0xAA表示写入 WSSSID*/
#define Com_Read_Write_Wifi_WALN_Cmd_Code	    (0X22)
/* 读和写Wifi的NETP操作码相关参数(要连接的数据服务器的IP地址及端口号)通过操作码后面的一个字节数据来区分读和写 0x55表示读取  0xAA表示写入 WSSSID*/
#define Com_Read_Write_Wifi_NETP_Cmd_Code	    (0X23)
/*复位Wifi模块或者将Wifi模块回复出厂设置*/
#define Com_Set_Rest_Wifi_Cmd_Code	          (0X25)
/*读和写Wifi获取方式*/
#define Com_Read_Write_Wifi_DHCP_Cmd_Code	    (0X26)
/*读和写Wifi模块的MAC地址*/
#define Com_Read_Write_Wifi_MAC_Cmd_Code	    (0X27)


/************************************************************/
/* Wifi的WLAN配置数据结构 */
typedef struct 
{
	/* 本地IP地址 */
	SERVER_IP_ADDRESS LOCALIP;
		/* 子网掩码IP地址 */
	SERVER_IP_ADDRESS MASK;
		/* 网关IP地址 */
	SERVER_IP_ADDRESS GATEWAY;

}WifiWlanConfigStruct;

#endif


/* 读实时数据 */
#define Com_Sensor_Cal_Cmd_Code_Read_Real_Data		    (0X01)
/* 写曲线 */                                      
#define Com_Sensor_Cal_Cmd_Code_Write_Curve_Data	    (0X03)
/* 读曲线 */                                      
#define Com_Sensor_Cal_Cmd_Code_Read_Curve_Data		    (0X07)
/* 读原始值数据 */                                
#define Com_Sensor_Cal_Cmd_Code_Read_Orig_Data		    (0X30)
/* 写配置SN号 */                                  
#define Com_Config_SN_Cmd_Code_Write				          (0X31)
/* 读配置SN号 */                                  
#define Com_Config_SN_Cmd_Code_Read					          (0X32)
/* 写配置传感器通道配置 */                        
#define Com_Config_Sensor_Cmd_Code_Write			        (0X33)
/* 读配置传感器通道配置 */                        
#define Com_Config_Sensor_Cmd_Code_Read				        (0X34)
                                                  
/* 读配置打印车牌号 */                            
#define Com_Config_Card_NO_Cmd_Code_Read			        (0x35)
/* 写配置打印车牌号 */                            
#define Com_Config_Card_NO_Cmd_Code_Write			        (0x36)
                                                  
/* 读配置打印公司名称 */                          
#define Com_Config_Company_Name_Cmd_Code_Read		      (0x37)
/* 写配置打印公司名称 */                          
#define Com_Config_Company_Name_Cmd_Code_Write		    (0x38)
                                                  
/* 读数据做假功能 */                              
#define Com_Config_Fad_Data_Cmd_Code_Read			        (0x39)
/* 写数据做假功能 */                                  
#define Com_Config_Fad_Data_Cmd_Code_Write			      (0x3A)
                                                      
/* 写数据服务器域名、IP、端口 */                      
#define ComConfigDataNetworkWrite					            (0x3B)
/* 读数据服务器域名、IP、端口 */                      
#define ComConfigDataNetworkRead					            (0x3C)
                                                      
                                                  
/* 写配置服务器域名、IP、端口 */                  
#define ComConfigNetworkWrite					                (0x56)
/* 读配置服务器域名、IP、端口 */                      
#define ComConfigNetworkRead					                (0x57)
/* 写配置服务器域名、IP、端口 */                      
#define ComConfigUpgradeNetworkWrite	                (0x58)
/* 读配置服务器域名、IP、端口 */                      
#define ComConfigUpgradeNetworkRead		                (0x59)

/*******************写报警手机号码信息*************************/
/* 读手机号码信息*/                                   
#define  Com_Config_SMSAamPoeNO_Cmd_Code_Read			    (0x41)
#define  Check_SMSAamPhone_Len_Read                   (0x0F)//用于判断 发送  手机号码内容长度的位数,正常是15位
/*写报警手机号码信息*/                                
#define  Com_Config_SMSAamPoeNO_Cmd_Code_Write			  (0x42)
/* 写报警手机号码 数据帧固定数据长度 */
#define  Com_Config_SMSAamPoeNO_Fix_Data_Len			    (7u)
#define  Check_SMSAamPhone_Len                        (0x0B)//用于判断手机号码长度的位数,正常是11位
/*****************设置时间间隔（上传时间间隔、历史记录时间间隔、打印时间间隔、 报警记录间隔）***************************/
/*******读时间间隔*******/
#define  Com_Config_Time_Interval_Cmd_Code_Read       (0x47)//读时间间隔操作码
/*******写时间间隔*******/
#define  Com_Config_Time_Interval_Cmd_Code_Write      (0x48)//设置时间间隔操作码
#define  Com_Config_Time_Interval_Set_Max_Second_Num  (0xFFFF)//设置时间间隔 的最大值65535
/***********************设置报警延时 (以秒为单位) *********************************/
#define  Com_Config_Alarm_time_delay_SetMaxSecond     3600u//最大报警延时间隔时间为1个小时 
/************读报警延时间隔***************/
#define  Com_Config_Alarm_time_delay_Cmd_Code_Read    (0x49)//读报警延时操作码
/************写报警延时间隔***************/
#define  Com_Config_Alarm_time_delay_Cmd_Code_Write   (0x4A)//写报警延时操作码
/************读声光报警总开关***************/
#define  Com_ConfigTotal_audible_light_alarm_Cmd_Code_Read    (0x4B)//读声光报警操作码
/********  写声光报警总开关***************/
#define  Com_ConfigTotal_audible_light_alarm_Cmd_Code_Write    (0x4C)//写声光报警操作码
/************读短信报警总开关***************/
#define  Com_Config_SMS_alarm_total_switch_Cmd_Code_Read     (0x4D)//读短信报警操作码
#define  Com_Config_SMS_alarm_total_switch_Cmd_Code_Write    (0x4E)//写短信报警操作码

       
#define  Com_Config_GSM_Signal_Strength_Cmd_Code_Read    (0x80)//读GSM模块信号强度

#define  Cancel_Sound_Light_Alarm_Cmd_Code            (0x81)//取消声光报警（该功能与处于待机界面按“返回”键的作用相同）

#define  Clear_Historical_Data_Cmd_Code               (0x09)//清除历史数据
#define  Open_GSM_Debug_Information_Cmd_Code          (0x83)//打开GSM模块调试信息
#define  Check_PowerLink_And_Battery_Voltage_Cmd_Code (0x84)//查看外接电状态和电池电压状态信息

#define  Open_DataSave_Cmd_Code                       (0x85)//开启数据记录
//#define  Close_DataSave_Cmd_Code           (0x57)//关闭数据记录
#define  Check_DataSave_Status_Cmd_Code               (0x86)//查询数据记录的状态


#define  Com_Config_SystemTime_Cmd_Code_Read_Write    (0x08)//读取和设置系统时间


#define  Com_Config_Software_Version_NO_Major_Cmd_Code_Read   (0x0C)//读取32位平台软件版本号和硬件版本号

#define  Com_Config_System_Factory_Setting            (0x87)//恢复出厂设置


#define  Switch_ON                                    (0x01)//开关开
#define  Switch_OFF                                   (0x00)//开关关
 
/************************************************************************************************/

/* 函数声明 */
void Com_Sensor_Cal_Send_Q			         (COM_SENSOR_CAL_TYPEDEF* Cal_Inf);
void COM_Sensor_Cal_Deal			           (COM_SENSOR_CAL_TYPEDEF* Cal_Inf);
u8 	 Com_Sensor_Cal_Get_Check		         (COM_SENSOR_CAL_TYPEDEF* Cal_Inf);
void COM_Sensor_Cal_Timeout_Check	       (INSTRU_FIX_INF_TAB* fix_tab,u16 check_time);
#endif  
