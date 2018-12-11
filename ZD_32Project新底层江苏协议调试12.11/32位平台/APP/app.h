#ifndef _APP_H_    
#define _APP_H_    
#include "stm32f10x.h" 
#include "ucos_ii.h"
#include "System_Config.h"
/************************************************************************************************/
/* Save 事件标志 */
extern OS_FLAG_GRP *Str_Save_Flag;
#if (Firmware_Urerade_Function_Enable == 0x01)
/* 程序升级 事件标志 */
extern OS_FLAG_GRP *Fwe_Ure_Flag;
#endif
/* GPRS 接收完成 程序升级数据包 事件标志 */
extern OS_FLAG_GRP *Str_GPRS_Upg_RXC_Data_Flag;
///* RTC8025 WakeUp 事件标志 */
//extern OS_FLAG_GRP *Str_RTC8025_WakeUp_Flag;
/* GSM消息 	队列的长度 */
#define GSM_EVENT_Q_Len				((u8)OS_MAX_QS)
/* Beep 		队列的长度 */
#define Beep_EVENT_Q_Len			((u8)OS_MAX_QS)
/* Light 	队列的长度 */
#define Light_EVENT_Q_Len			((u8)OS_MAX_QS)
/* 传感器校准通信事件 	队列的长度 */
#define COM_Sensor_Cal_EVENT_Q_Len	((u8)OS_MAX_QS)
/* 无线卡片 信号量 */
extern OS_EVENT   *RF_Card_QUE_Sem;
/* 打印数据 信号量 */
extern OS_EVENT   *Print_QUE_Sem;
/* GSM消息队列事件控制块 */
extern OS_EVENT* GSM_EVENT_Str_Q;
/* GSM消息队列事件控制块数组 */
extern void    * GSM_EVENT_Grp[GSM_EVENT_Q_Len];
/* 定义蜂鸣器事件控制块 */
extern OS_EVENT* Beep_EVENT_Str_Q;
/* 定义蜂鸣器事件控制块数组 */
extern void    * Beep_EVENT_Grp[Beep_EVENT_Q_Len];
/* 定义LED事件控制块 */
extern OS_EVENT* Light_EVENT_Str_Q;
/* 定义LED事件控制块数组 */
extern void    * Light_EVENT_Grp[Light_EVENT_Q_Len];
/* 定义Com通信事件控制块 */
extern OS_EVENT* COM_Sensor_Cal_EVENT_Q;
/* 定义Com通信事件控制块数组 */
extern void 	* COM_Sensor_Cal_EVENT_Grp[COM_Sensor_Cal_EVENT_Q_Len];
#if (GPS_Config_Enable == 0x01)
/* 声明GPS消息邮箱事件控制块指针  */
extern OS_EVENT *GPS_Box;
#endif
/**************** 用户任务声明 *******************/   
void Task_Start(void *p_arg);  //启动所有任务开始任务
void AppTaskRFCard(void *p_arg);//无线卡片任务
#if (GPS_Config_Enable == 0x01)
void AppTaskGPS(void * p_arg);//GPS任务 
#endif
#if (Gateway_NJSY_Enable == 0x01)
void AppTaskGateway(void * p_arg);/*南京盛亿网关任务*/
#endif
void AppTaskSave       (void *p_arg);//传感器数据存储任务
void AppTaskDis        (void *p_arg);//界面显示任务
void AppTaskKey        (void *p_arg);//触摸按键扫描任务
void AppTaskTime       (void *p_arg);//看门狗喂狗任务
void AppTaskUpg        (void *p_arg);//本地手动触发远程程序更新任务
void AppTaskGSM        (void *p_arg);//GSM模块网络通信任务
void AppTaskState      (void *p_arg);//各个状态信息更新任务
void AppTaskADC        (void *p_arg);//ADC采集传感器模拟电压信息任务
void AppTaskCOM        (void *p_arg);//与配置工具通信对一起相关参数进行配置任务
void AppTaskBeep       (void *p_arg);//声音报警(蜂鸣器)任务
void AppTaskLight      (void *p_arg);//光报警(指示灯)任务
void AppTaskPrint      (void *p_arg);//打印机打印任务
void AppTaskEasyPrint  (void *p_arg);//打印机一键打印任务
void GSM_Config_Task   (void *p_arg);//GSM模块参数配置任务
#endif  //_APP_H_  
