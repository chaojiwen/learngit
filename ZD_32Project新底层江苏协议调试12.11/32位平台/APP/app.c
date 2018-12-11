/************************************************************************************************/
/** @file              : App.c																	*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月28日															*/
/** @brief             : c file																	*/
/** @description       : 32为平台应用文件															*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "interface.h"
#include "app.h"
#include "System_Config.h"
#include "App_GPS.h"
#include "App_Print.h"          
#include "App_Save.h"
#include "BSP.h"
#include "Mcu_ADC.h"
#include "Mcu_button.h"
#include "Mcu_Beep.h"
#include "Mcu_Config.h"
#include "Mcu_GPS.h"
#include "Mcu_led.h"
#include "Log_Adc1.h"
#include "Log_Alarm.h"
#include "Log_Battery.h"
#include "Log_COM.h"
#include "Log_GSM.h"
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
	#include "Log_ConfigService.h"
#elif (AppGPRSComProtocol == 0x01)
	#include "Log_ConfigService_Http.h"
#endif
#endif
#include "Log_Download.h"
#if (Gateway_NJSY_Enable == 0x01)
#include "App_Gateway_NJSY.h"
#endif
#if (Gateway_HZZH_Enable == 0x01)
#include "App_Gateway_HZZH.h"
#endif

#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	
#include "Log_GSM_Config.h"  
#endif



#include "Log_GPS.h"
#include "Log_Hum_Cal.h"
#include "Log_IAP.h"
#include "Log_Power.h"
#include "Log_Print.h"
#include "Log_RF_Card.h"
#include "Log_Save.h"
#include "Log_Sensor.h"
#include "Dev_GPS.h"
#include "Dev_Flash.h"
#include "Dev_RTC.h"
#include "Psl_AI2C.h"
#include "Menu.H"
#include <string.h>
/***************************************************************************/
/*****************************模块内全局变量**********************************/
/***************************************************************************/
/* 定义每个任务的堆栈空间，app_cfg.h文件中宏定义栈大小 */
#if (Wireless_RF_Card_Config_Enable == 0x01)
static OS_STK AppTaskRFCardStk	[APP_TASK_RFCard_STK_SIZE];
#endif

#if (GPS_Config_Enable == 0x01)
/* 定义GPS任务栈 */
static OS_STK AppTaskGPSStk		      [APP_TASK_GPS_STK_SIZE];
/* 定义GPS消息邮箱事件控制块指针 */
OS_EVENT *GPS_Box;
#endif

static OS_STK AppTaskSaveStk	      [APP_TASK_Save_STK_SIZE];
static OS_STK AppTaskDisStk 	      [APP_TASK_Dis_STK_SIZE];
static OS_STK AppTaskKeyStk 	      [APP_TASK_Key_STK_SIZE];
#if (Firmware_Urerade_Function_Enable == 0x01)
//static OS_STK AppTaskUpgStk 	[APP_TASK_Upg_STK_SIZE];
#endif
static OS_STK AppTaskCOMStk		      [APP_TASK_COM_STK_SIZE];
static OS_STK AppTaskBeepStk	      [APP_TASK_Beep_STK_SIZE];
static OS_STK AppTaskLightStk	      [APP_TASK_Light_STK_SIZE];
static OS_STK AppTaskADCStk		      [APP_TASK_ADC_STK_SIZE];
OS_STK AppTaskGSMStk 	[APP_TASK_GSM_STK_SIZE];
static OS_STK AppTaskStateStk 	    [APP_TASK_State_STK_SIZE];
static OS_STK AppTaskTimeStk 	      [APP_TASK_Time_STK_SIZE];

//#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	
//static OS_STK   AGsmConfigStk		      [GSM_Config_STK_SIZE];

///* GSM模块参数配置消息队列事件控制块 */
//OS_EVENT    *GSM_Config_EVENT_Str_Q;
///* GSM模块参数配置消息队列事件控制块数组 */
//void        *GSM_Config_EVENT_Grp[((u8)OS_MAX_QS)];
//#endif


//static OS_STK AppTaskUpgStk     [APP_TASK_Upg_STK_SIZE];  
#if (Print_Function_Enable == 0x01)
static OS_STK AppTaskPrintStk 	    [APP_TASK_Print_STK_SIZE];
static OS_STK AppTaskEasyPrintStk 	[APP_TASK_Easy_Print_STK_SIZE];
#endif


/************************************************************************************************/
/* RF卡片 信号量*/
OS_EVENT *RF_Card_QUE_Sem;
#if (Print_Function_Enable == 0x01)
/* 打印数据 信号量*/
OS_EVENT *Print_QUE_Sem;
#endif
/* Save 事件标志 */
OS_FLAG_GRP *Str_Save_Flag;

#if (Firmware_Urerade_Function_Enable == 0x01)
/* 固件升级 事件标志 */
OS_FLAG_GRP *Fwe_Ure_Flag;
#endif

/* GPRS 接收完成 程序升级数据包 事件标志 */
OS_FLAG_GRP *Str_GPRS_Upg_RXC_Data_Flag;
///* RTC8025 WakeUp 事件标志 */
//OS_FLAG_GRP *Str_RTC8025_WakeUp_Flag;
/* GSM消息队列事件控制块 */
OS_EVENT* GSM_EVENT_Str_Q;
/* GSM消息队列事件控制块数组 */
void    * GSM_EVENT_Grp[GSM_EVENT_Q_Len];
/* 定义蜂鸣器事件控制块 */
OS_EVENT* Beep_EVENT_Str_Q;
/* 定义蜂鸣器事件控制块数组 */
void 	* Beep_EVENT_Grp[GSM_EVENT_Q_Len];
/* 定义LED事件控制块 */
OS_EVENT* Light_EVENT_Str_Q;
/* 定义LED事件控制块数组 */
void 	* Light_EVENT_Grp[GSM_EVENT_Q_Len];
/* 定义Com通信事件控制块 */
OS_EVENT* COM_Sensor_Cal_EVENT_Q;
/* 定义Com通信事件控制块数组 */
void 	* COM_Sensor_Cal_EVENT_Grp[COM_Sensor_Cal_EVENT_Q_Len];
/************************************************************************************************/

/* 打印调试标志 */
extern BOOL GSM_DEBUG_Flag;// TRUE
/************************************************************************************************/
/* 函数名	: Task_Start(系统启动的开始任务)														*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 程序的主体，用以创建，删除，传的其他类型的事件 									 	*/
/* 创建日期 	: 2012/12/20																		*/
/************************************************************************************************/
void Task_Start(void *p_arg)
{
	INT8U      err;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告 
	/* 检测CPU能力，统计模块初始化。该函数将检测最低CPU占有率 注意：此函数会延迟100ms再返回 */
	#if (OS_TASK_STAT_EN > 0)
		OSStatInit();
	#endif
	/* 板级初始化 */
	BSP_Init();
	/* 模拟电路电源稳定 */
	OSTimeDly(1000);
	/* 上电检测电池电压 */
	ADC_Sam_Pwr_Sta();
	/* 初始化(这个I2C是用IO模拟的) */
	Psl_AI2C_Init();	
	/* 系统参数配置 */
	System_Para_Init();
	/* RTC 初始化 */
	if(RTC_Init(FALSE) == FALSE)
	{
		/* 时钟数据寄存器 */
		Times time  = {0};
		time.Year	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year;
		time.Month	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month;
		time.Day	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day;
		time.Hour	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour;
		time.Min	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min;
		time.Sec	  = Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec;
		/* 设置 时间 */
		Write_System_Date_Time_Data(&time);
	}
	
	/* 创建 Save 	事件标志 */
	Str_Save_Flag 	= OSFlagCreate (0, &err);

#if (Firmware_Urerade_Function_Enable == 0x01)
  	/* 创建 程序升级 信号量标志 */
	Fwe_Ure_Flag 	= OSFlagCreate (0, &err);
#endif
	
  	/* 创建 程序升级 信号量标志 */
	Str_GPRS_Upg_RXC_Data_Flag = OSFlagCreate (0, &err);
	/* 创建 RF卡片 信号量 */
	RF_Card_QUE_Sem	= OSSemCreate(0);
	
	/* 创建 GSM事件 					消息队列 */
	GSM_EVENT_Str_Q   			= OSQCreate(GSM_EVENT_Grp,				GSM_EVENT_Q_Len);
	/* 创建 传感器校准事件队列			消息队列 */
	COM_Sensor_Cal_EVENT_Q   = OSQCreate(COM_Sensor_Cal_EVENT_Grp,	COM_Sensor_Cal_EVENT_Q_Len);	
	/* 创建 Beep事件 				消息队列 */
	Beep_EVENT_Str_Q  			= OSQCreate(Beep_EVENT_Grp,				Beep_EVENT_Q_Len);
	/* 创建 Light事件 				消息队列 */
	Light_EVENT_Str_Q 			= OSQCreate(Light_EVENT_Grp,			Light_EVENT_Q_Len);
	
////	#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	
////   //创建GSM模块配置事件消息队列
////	 GSM_Config_EVENT_Str_Q = OSQCreate(GSM_Config_EVENT_Grp,((u8)OS_MAX_QS));
////	 
////	 	/*  建立GSM模块参数配置 任务 */
////	OSTaskCreate(GSM_Config_Task,(void *)0,
////       			(OS_STK *)&AGsmConfigStk[GSM_Config_STK_SIZE - 1],
////				     APP_TASK_GSM_Config_PRIO); 
////  #endif
	
	
//  	/* 创建  信号量标志 */
//	Str_RTC8025_WakeUp_Flag    = OSFlagCreate (0, &err);

#if (Wireless_RF_Card_Config_Enable == 0x01)
	/*  建立 RF卡片 任务 */
	OSTaskCreate(AppTaskRFCard,(void *)0,
       			(OS_STK *)&AppTaskRFCardStk[APP_TASK_RFCard_STK_SIZE - 1],
				 APP_TASK_RFCard_PRIO); 
#endif

#if (GPS_Config_Enable == 0x01)

	/*  建立 GPS 任务 */
	OSTaskCreate(AppTaskGPS,(void *)0,    
       			(OS_STK *)&AppTaskGPSStk[APP_TASK_GPS_STK_SIZE - 1],
				 APP_TASK_GPS_PRIO); 
#endif
	/*  建立 信息保存 任务 */
	OSTaskCreate(AppTaskSave,(void *)0,    
       			(OS_STK *)&AppTaskSaveStk[APP_TASK_Save_STK_SIZE - 1],
				 APP_TASK_Save_PRIO); 

	/*  建立 显示 任务 */
	OSTaskCreate(AppTaskDis,(void *)0,    
       			(OS_STK *)&AppTaskDisStk[APP_TASK_Dis_STK_SIZE - 1],
				 APP_TASK_Dis_PRIO); 

	/*  建立 按键 任务 */
	OSTaskCreate(AppTaskKey,(void *)0,    
       			(OS_STK *)&AppTaskKeyStk[APP_TASK_Key_STK_SIZE - 1],
				 APP_TASK_Key_PRIO);

	/*  建立 时间 任务 */
	OSTaskCreate(AppTaskTime,(void *)0,    
       			(OS_STK *)&AppTaskTimeStk[APP_TASK_Time_STK_SIZE - 1],
				 APP_TASK_Time_PRIO);
	
#if (Firmware_Urerade_Function_Enable == 0x01)

	/*  建立 程序升级任务 任务 */
//	OSTaskCreate(AppTaskUpg,(void *)0,    
//       			(OS_STK *)&AppTaskUpgStk[APP_TASK_Upg_STK_SIZE - 1],
//				 APP_TASK_Upg_PRIO);

#endif
	/*  建立 状态检测 任务 */
	OSTaskCreate(AppTaskState,(void *)0,    
       			(OS_STK *)&AppTaskStateStk[APP_TASK_State_STK_SIZE - 1],
				 APP_TASK_State_PRIO);
	
	/*  建立 ADC采样 任务 */
	OSTaskCreate(AppTaskADC,(void *)0,    
       			(OS_STK *)&AppTaskADCStk[APP_TASK_ADC_STK_SIZE - 1],
				 APP_TASK_ADC_PRIO); 
	
	/*  建立串口通信 任务 */
	OSTaskCreate(AppTaskCOM,(void *)0,    
       			(OS_STK *)&AppTaskCOMStk[APP_TASK_COM_STK_SIZE - 1],
				 APP_TASK_COM_PRIO);	
	/*  建立 蜂鸣器 任务 */
	OSTaskCreate(AppTaskBeep,(void *)0,    
       			(OS_STK *)&AppTaskBeepStk[APP_TASK_Beep_STK_SIZE - 1],
				 APP_TASK_Beep_PRIO);
	
	/*  建立 光提示 任务 */
	OSTaskCreate(AppTaskLight,(void *)0,    
       			(OS_STK *)&AppTaskLightStk[APP_TASK_Light_STK_SIZE - 1],
				 APP_TASK_Light_PRIO);
	
	/*  建立 GPRS 任务 */
	OSTaskCreate(AppTaskGSM,(void *)0,    
       			(OS_STK *)&AppTaskGSMStk[APP_TASK_GSM_STK_SIZE - 1],
				 APP_TASK_GSM_PRIO);
	
#if (Print_Function_Enable == 0x01)
	/*  建立 打印 任务 */
	OSTaskCreate(AppTaskPrint,(void *)0,   
       			(OS_STK *)&AppTaskPrintStk[APP_TASK_Print_STK_SIZE - 1],
				 APP_TASK_Print_PRIO);
	
	/*  建立 一键打印 任务 */
	OSTaskCreate(AppTaskEasyPrint,(void *)0,   
       			(OS_STK *)&AppTaskEasyPrintStk[APP_TASK_Easy_Print_STK_SIZE - 1],
				 APP_TASK_Easy_Print_PRIO);
#endif
	
#if (Gateway_NJSY_Enable == 0x01)
	/*  创建 网关 任务 */
	OSTaskCreate(AppTaskGatewayNJSY,(void *)0,
       			(OS_STK *)&AppTaskGatewayNJSYStk[AppTask_GatewayNJSY_StkSize - 1],
				 APP_TASK_Gateway_PRIO);
#endif
#if (Gateway_HZZH_Enable == 0x01)
	/*  创建 杭州创辉网关 任务 */
	OSTaskCreate(AppTaskGatewayHZZH,(void *)0,
       			(OS_STK *)&AppTaskGatewayHZZHStk[AppTask_GatewayHZZH_StkSize - 1],
				 APP_TASK_GatewayHZZH_PRIO);
#endif
	while (1)  
	{    
		/* 挂起自己 */
		OSTaskSuspend (OS_PRIO_SELF);
	} 
}  

#if (Wireless_RF_Card_Config_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: AppTaskRFCard(无线卡片任务)													*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 程序的主体，用以创建，删除，传的其他类型的事件											*/
/* 创建日期 	: 2015/04/14																		*/
/************************************************************************************************/
void AppTaskRFCard(void *p_arg)
{
	INT8U      err;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	while (1)
	{
		/* 等待 队列 消息 */
		OSSemPend((OS_EVENT*)RF_Card_QUE_Sem,0,&err);
		
		/* 接收消息 状态 判断 */
		if(err == OS_ERR_NONE)
		{
			/* RF_Card 数据筛选 执行对应的动作 */
			RF_Card_Screen(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor);
		}
	}
} 
#endif


#if (GSM_Module_Type==0x02) //ESP8266Wifi模块	
/************************************************************************************************/
/* 函数名	: GSM_Config_Task(GSM模块参数配置任务)													 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 配置GSM模块参数							 									*/
/* 创建日期 	: 2018/11/12																		*/
/************************************************************************************************/
//////////void GSM_Config_Task(void *p_arg)
//////////{
//////////	INT8U                                   err;
//////////	GSM_Config_QUE*          GSM_Config_QUE_inf;
//////////	OS_Q_DATA                         	 Q_data; 
//////////	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
//////////	while (1)
//////////	{
//////////		/* 等待 队列 消息 */
//////////		GSM_Config_QUE_inf = (GSM_Config_QUE*)OSQPend((OS_EVENT*)GSM_Config_EVENT_Str_Q,0,&err);
//////////		/* 接收消息 状态 判断 */
//////////		if(err == OS_ERR_NONE)
//////////		{
//////////			/* 消息被正确的接收 */
//////////			/* GSM模块参数配置执行对应的动作 */
//////////			GSM_Config_Function(GSM_Config_QUE_inf,&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
//////////		}	
//////////		/* 查询 GSM 消息队列 */
//////////		err = OSQQuery((OS_EVENT*)GSM_Config_EVENT_Str_Q,&Q_data);
//////////	
//////////		/* 判断查询结果 */
//////////		if(err == OS_ERR_NONE)
//////////		{
//////////			/* 查询成功 */
//////////			/* 寻找上一条发送成功的消息 */
//////////			if((((OS_Q *)(GSM_Config_EVENT_Str_Q->OSEventPtr))->OSQOut) == (((OS_Q *)(GSM_Config_EVENT_Str_Q->OSEventPtr))->OSQStart))
//////////			{
//////////				/* 清除GSM模块参数配置消息队列上一条消息指针 */
//////////				*((u32*)(((OS_Q *)(GSM_Config_EVENT_Str_Q->OSEventPtr))->OSQEnd)-1) = 0;
//////////			}else
//////////			{
//////////				/* 清除GSM模块参数配置消息队列上一条消息指针 */
//////////				*((u32*)(((OS_Q *)(GSM_Config_EVENT_Str_Q->OSEventPtr))->OSQOut)-1) = 0;
//////////			}
//////////		}
//////////	 } 
//////////	}



	 
	 
#endif



/************************************************************************************************/
/* 函数名	: AppTaskSave(传感器数据存储任务)													 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 程序的主体，用以创建，删除，传的其他类型的事件											*/
/* 创建日期 	: 2014/9/11																			*/
/************************************************************************************************/
void AppTaskSave(void *p_arg)
{
	INT8U      err;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	/* 等待其它任务运行 */
	OSTimeDly(1000);
	while (1)  
	{    
		/* 等待数据 */
		OSFlagPend(								  //请求标志信号
		Str_Save_Flag,							//请求标志信号指针
		(OS_FLAGS)0xff,							//过滤器 请求第0和第1位信号 0011 这里是把数据3强制转化为OS_FLAGS类型的数据，
												        //因为过滤器和标志信号中的信号都是OS_FLAGS类型的数据
												        //OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,   
												        //信号全是1表示信号有效 参数OS_FLAG_CONSUME表示当
												        //任务等待的事件发生后，清除相应的事件标志位
		OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,	//信号全是1表示信号有效  没有加参数OS_FLAG_CONSUME，所以不会清除标志位
		0,										      //等待时限，0表示无限等待
		&err									      //错误信息
		);

		/* 周期性调用 保存 “日期时间”、“传感器数据”、“GPS数据”、等 */
		Save_Sensor_Data(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
		
#if (Print_Function_Enable == 0x01)
		/* 记录当前保存数据的系统时间和Flash存储地址 */
		Print_Index_Save_Sensor_Data(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
#endif		
	}
}         

/************************************************************************************************/
/* 函数名	: AppTaskDis(界面显示任务)															 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 程序的主体，用以创建，删除，传的其他类型的事件 									 		*/
/* 创建日期 	: 2014/9/12																			*/
/************************************************************************************************/

void AppTaskDis(void *p_arg)
{
	/* 按键状态寄存器 */
	u8 Key_State_REG=0;
	/* 系统时间寄存器 */
	u32 Cur_Time;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	while (1)
	{
		/* 获取当前系统时间 */
		Cur_Time = OSTimeGet();
		
		/* 菜单空闲状态检测. */
		Menu_Idle_State_Check(Cur_Time);
		
		/* 菜单首界面项目循序显示 */
		Menu_DeskTop_Switch_Disp(Cur_Time);
		
	   #if (AppGPRSComProtocolEnable == 0x01)
     #if (AppGPRSComProtocol == 0x00)//部标协议		
		if(*IAP_App.UpdateFlag != TRUE)//当设备处于远程升级程序时按键失效
		{
		  /* 获取按键状态 */
		  Key_State_REG = Get_Key_State();
		}
    #endif
    #endif	
		
	   #if (AppGPRSComProtocolEnable == 0x01)
     #if (AppGPRSComProtocol == 0x01)	//Http协议	
		  /* 获取按键状态 */
		  Key_State_REG = Get_Key_State();

    #endif
    #endif
		/* 判断按键状态。 */
		if(Key_State_REG != KEY_NONE)
		{
			/* 按键被按下. */
			/* 清除菜单空闲状态时间寄存器数据 */
			Menu_Idle_State_Reset(Cur_Time);
			/* 清除首界面项目切换时间寄存器数据 */
			Menu_DeskTop_Switch_Reset(Cur_Time);
		}
		/* 按键扫描 */
		Menu_Display(Key_State_REG);
		/* 等待100mS，进入等待，放弃CPU使用权。 */
		OSTimeDlyHMSM(0,0,0,100); 
	}
}

/************************************************************************************************/
/* 函数名	: AppTaskKey(按键状态信息扫描打印任务)													 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 按键时时扫描任务								 									*/
/* 创建日期 	: 2014/11/14																		*/
/************************************************************************************************/
void AppTaskKey(void *p_arg)
{
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	/* 开 按键 电源 */
	KEY_Power_Open;
	
	while (1)  
	{
		/* 按键扫描 */
		Key_State_Check_All();
		/* 等待10mS，进入等待，放弃CPU使用权。 */
		OSTimeDlyHMSM(0,0,0,10); 
	}
}

/************************************************************************************************/
/* 函数名	: AppTaskTime(看门狗定时喂狗时间任务)															 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 程序的主体，用以创建，删除，传的其他类型的事件											*/
/* 创建日期 	: 2014/10/08																		*/
/************************************************************************************************/
void AppTaskTime(void *p_arg)
{
	static u16 wdg_state;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
//////	/* 喂狗 引脚 配置 */
//////	MAX823_WDI_GPIO_Config();	
	while (1)
	{
////		/* 喂狗 信号输出 */
////		if(wdg_state == TRUE)
////		{
////			wdg_state = FALSE;
////			MAX823_WDI_High;
////		}else
////		{
////			wdg_state = TRUE;
////			MAX823_WDI_Low;			
////		}	
		
		/* 定时500MS喂狗一次 */
		OSTimeDly(500);
	}
}

#if (Firmware_Urerade_Function_Enable == 0x01)
/************************************************************************************************/
/* 函数名	：AppTaskUpg(本地触发远程升级任务)													 	 				*/
/* 输入		：(void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用		：升级程序									 									    */
/* 创建日期	：2014/12/07																		*/
/************************************************************************************************/
void AppTaskUpg(void *p_arg)
{
	INT8U err;
	u16 Upgrade_Result;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	/* 程序升级 任务 */
	while (1)  
	{
		/* 开始等待 系统升级 信号 */
		OSFlagPend(								//请求标志信号
		Fwe_Ure_Flag,							//请求标志信号指针
		(OS_FLAGS)0xff,							//过滤器 请求第0和第1位信号 0011 这里是把数据3强制转化为OS_FLAGS类型的数据，
												//因为过滤器和标志信号中的信号都是OS_FLAGS类型的数据
												//OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,   
												//信号全是1表示信号有效 参数OS_FLAG_CONSUME表示当
												//任务等待的事件发生后，清除相应的事件标志位
		OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME,	//信号全是1表示信号有效  没有加参数OS_FLAG_CONSUME，所以不会清除标志位
		0,										//等待时限，0表示无限等待
		&err									//错误信息
		);
		/* 判断判断 信号请求 状态 */
		if(err == OS_ERR_NONE)
		{
			/* 收到 程序升级 信号 */
			/* 进入 升级 流程 	*/
			Upgrade_Result = Remote_Upgrade();
			/* 提示当前 升序升级 结果 */
			if(Upgrade_Result == FALSE)
			{
				/* 程序升级 失败... */
				__nop();
			}else
			{
				/* 程序升级 成功*/
				__nop();
			}
		}
	}
}
#endif

/************************************************************************************************/
/* 函数名	: AppTaskGPRS(GSM模块任务)													 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 上传传感器数据、远程配置、远程升级等(有关网络数据通信)						 									 	*/
/* 创建日期 	: 2014/11/19																		*/
/************************************************************************************************/
void AppTaskGSM(void *p_arg)
{
	INT8U               err;
	GSM_QUE*            GSM_QUE_inf;
	OS_Q_DATA	          Q_data;
	(void)p_arg;      // 'p_arg' 并没有用到，防止编译器提示警告
	while (1)
	{
		/* 等待 队列 消息 */
		GSM_QUE_inf = (GSM_QUE*)OSQPend((OS_EVENT*)GSM_EVENT_Str_Q,0,&err);
		/* 接收消息 状态 判断 */
		if(err == OS_ERR_NONE)
		{
			/* 消息被正确的接收 */
			/* GSM 执行对应的动作 */
			GSM_Function(GSM_QUE_inf,&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
		}	
		/* 查询 GSM 消息队列 */
		err = OSQQuery((OS_EVENT*)GSM_EVENT_Str_Q,&Q_data);
	
		/* 判断查询结果 */
		if(err == OS_ERR_NONE)
		{
			/* 查询成功 */
			/* 寻找上一条发送成功的消息 */
			if((((OS_Q *)(GSM_EVENT_Str_Q->OSEventPtr))->OSQOut) == (((OS_Q *)(GSM_EVENT_Str_Q->OSEventPtr))->OSQStart))
			{
				/* 清除GSM消息队列上一条消息指针 */
				*((u32*)(((OS_Q *)(GSM_EVENT_Str_Q->OSEventPtr))->OSQEnd)-1) = 0;
			}else
			{
				/* 清除GSM消息队列上一条消息指针 */
				*((u32*)(((OS_Q *)(GSM_EVENT_Str_Q->OSEventPtr))->OSQOut)-1) = 0;
			}
		}
	}
}

/************************************************************************************************/
/* 函数名	: AppTaskState(更新状态任务)													 	 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 更新任务当中的各种状态信息						 									*/
/* 创建日期 	: 2014/12/19																		*/
/************************************************************************************************/
void AppTaskState(void *p_arg)
{
/* 状态 任务 的跟新时间频率 */
#define Task_State_Update_Time					  (10ul)
#define Task_State_Base_time_50ms				  (50u)
#define Task_State_Base_time_100ms				(100u)
#define Task_State_Base_time_500ms				(500u)
#define Task_State_Base_time_1000ms				(1000u)
#define Task_State_Base_time_10000ms			(10000u)
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)	//部标协议
#define Task_State_Base_time_60000ms			(60000u) //1分钟
	
#define Task_State_Base_time_300000ms			(300000u)//5分钟	
#endif
#endif	
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x01)	//Http协议
#define Task_State_Base_time_120000ms			(120000u) //2分钟
#endif
#endif
	
	static u32 Base_time_50ms_Reg            = 0;
	static u32 Base_time_100ms_Reg           = 0;
	static u32 Base_time_500ms_Reg           = 0;
	static u32 Base_time_1000ms_Reg          = 0;
	static u32 Base_time_10000ms_Reg         = 0;
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x00)	//部标协议	
	static u32 Base_time_60000ms_Reg         = 0;
	static u32 Base_time_300000ms_Reg        = 0;
#endif
#endif
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x01)//Http协议		
	static u32 Base_time_120000ms_Reg         = 0;
#endif
#endif
	u32 Cur_time;	
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	/* 初始化 状态参数。 */
	/* 初始化配置电池状态参数 */
	Battery_Para_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_Power);
	/* 初始化配置外接电源状态参数 */
	Power_ET_Para_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_Power);
	
#if (Print_Function_Enable == 0x01)
	/* 自动更新打印"起始时间"和"结束时间". */
	App_Print_Date_Init();
#endif

	/* 等待1S，等其他任务都据需后，开始状态更新。 */
	OSTimeDlyHMSM(0,0,2,0);
	while (1)
	{
		/* 根据 各种状态 的跟新速率 */
		Cur_time = OSTimeGet();
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x00)//部标协议
		/* 默认5分钟更新一次,当在进行远程升级时修改为1分钟连接一次 */
		if((Cur_time-Base_time_300000ms_Reg) >= Task_State_Base_time_300000ms)	
		{
	    		/* 设置 当前 时间为300000ms基准时间 */
	    		  Base_time_300000ms_Reg = Cur_time;
	    			/* 给调度器上锁 */
	    	   	OSSchedLock();
	    	if(IAP_App.RunState==FALSE)//判断如果32设备在未进行远程升级时将连接配置服务器的时间间隔更该为默认5分钟
	    	{
	    			/* 进行配置服务应用 */
	    		 ConfigService_TXLog(&Instru_Config_Inf_Tab);
	    		/* 检测固件升级状态 */
	    	   IAP_CheckFlag();	
	    	}
	    		/* 给调度器解锁 */
	    		  OSSchedUnlock();
		}
	
     /* 1分钟更新一次*//* 默认5分钟更新一次,当在进行远程升级时修改为1分钟连接一次 */
     if((Cur_time-Base_time_60000ms_Reg) >= Task_State_Base_time_60000ms)
     {
     	  /* 设置 当前 时间为60000ms基准时间 */
     	  Base_time_60000ms_Reg = Cur_time;	
     	  /* 给调度器上锁 */
     	  OSSchedLock();
/**********************上电进行一次设备信息同步操作*****************************************/
			 /* 1、判断仪器是否同步完成 系统每次复位都会进行一次将系统上所有参数同步到配置服务器上*/
	     if((ConfigService_App.SYNC_State == FALSE))	/* 仪器未进行同步或则同步失败 */
	     {
	     		if(GSM_DEBUG_Flag ==TRUE)
	     		{
	     			printf("1、将系统所有信息同步到配置服务器上\n");
	     		}
	     	  ConfigService_Cmd(0xFF,0x01);			/* 上传当设备配置信息(同步平台参数) */
	     }
/*******************************************************************************************/			 
     	  if(IAP_App.RunState==TRUE)//判断如果32设备在进行远程升级时将连接配置服务器的时间间隔更该为1分钟
     	  {
     	  	/* 进行配置服务应用 */
     	    ConfigService_TXLog(&Instru_Config_Inf_Tab);
					/* 检测固件升级状态 */
		      IAP_CheckFlag();	
     	  }
     	  /* 给调度器解锁 */
     	  OSSchedUnlock();
     }
		  #endif
		  #endif
		    
		   #if (AppGPRSComProtocolEnable == 0x01)
       #if (AppGPRSComProtocol == 0x01) //Http协议
		      /* 1分钟向金卫信服务器更新一次设备状态信息*/
     if((Cur_time-Base_time_120000ms_Reg) >= Task_State_Base_time_120000ms)
     {
     	  /* 设置 当前 时间为60000ms基准时间 */
     	  Base_time_120000ms_Reg = Cur_time;	
     	  /* 给调度器上锁 */
     	  OSSchedLock();
     	  	/* 进行配置服务应用 */
     	    ConfigService_TXLog(&Instru_Config_Inf_Tab);
		
     	  /* 给调度器解锁 */
     	  OSSchedUnlock();
     }
		 	  #endif
		    #endif
		
		/* 10000MS 更新 */
		if((Cur_time-Base_time_10000ms_Reg) >= Task_State_Base_time_10000ms)
		{
			/* 设置 当前 时间为1000ms基准时间 */
			Base_time_10000ms_Reg = Cur_time;
			
			/* 给调度器上锁 */
			OSSchedLock();
		  #if (AppGPRSComProtocolEnable == 0x01)
      #if (AppGPRSComProtocol == 0x00)//部标协议
			if(IAP_App.RunState == FALSE)
			{
						/* GSM 信号强度 */
			  GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Update_SGL_EVENT);
			  /* GSM 附着状态 */
			  GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Update_Link_EVENT);
			
			}
			#endif
		  #endif
////////			/* 检测当前 GSM信号 */
////////			GSM_Signal_Check();		
/* 判断温度补偿功能是否启动 */	
#if ((Hum_Sensor_Function_Config_Enable == 0x01) &&\
	 (Hum_Sensor_Tmp_Comp_Enable == 0x01) &&\
	 (Hum_Sensor_Tmp_Comp_CH > 0))
			/* 获得湿度传感器温度补偿参数 */
			Humi_Get_Temp_Comp_Value();
#endif

			/* 给调度器解锁 */
			OSSchedUnlock();		
		}
		/* 1000MS 更新 */
		if((Cur_time-Base_time_1000ms_Reg) >= Task_State_Base_time_1000ms)
		{
			/* 检测当前 GSM信号 */
			GSM_Signal_Check();	//每1秒钟更新一下信号值
			
			Heart_Runing_Led();//心跳运行指示灯  每隔1秒闪烁一次
			/* 设置 当前 时间为1000ms基准时间 */
			Base_time_1000ms_Reg = Cur_time;

			/* 更新传感器数据 */
			Sensor_Updata_Data(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
			
			/* 给调度器上锁 */
			OSSchedLock();
			
			/* 传感器状态检测 */
			Sensor_State_Check_ALL(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
			
			/* 电池电量检测 */
			Power_BT_Quantity_Check(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);
			
			/* 检测 是否有新的 存储的 温湿度 */
			GPRS_Save_Sensor_Data_Check(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);
			
			/* 检查 GPRS数据解析状态 */
			GPRSSendData_ParseStateCheck();
/***************固件升级时不执行数据上传操作任务**********************/
			/* GPRS 发送传感器数据 */
			 GPRS_Send_Sensor_Data();
/***************************END***************************************/					
#if (Wireless_RF_Card_Config_Enable == 0x01)
			/* 检测卡片接收数据是否超时 */
			RF_Card_Rece_Data_State_Check();
#endif

#if (GPS_Config_Enable == 0x01)
			GPS_Log_Rece_Data_State_Check();
#endif
			/* 检测系统启动后时钟是否校准 */
			RTC_System_Clock_Calibration_State_Check();	
			/* 定时更新（1s） 菜单 界面 */
			Flash = FlashMode_DeskTop;	
			/* 给调度器解锁 */
			OSSchedUnlock();
		}
		
		/* 500MS 更新 */
		if((Cur_time-Base_time_500ms_Reg) >= Task_State_Base_time_500ms)
		{
		
			/* 设置 当前 时间为100ms基准时间 */
			Base_time_500ms_Reg = Cur_time;
			/* 给调度器上锁 */
			OSSchedLock();
			/* 读取 RTC8025 的时间数据 */
			Read_System_Date_Time_Data((Times*)&System_Time);
			
			/* 将当前的时间数据压缩。 */
			Compression_Times_Data((Times*)&System_Time,&Instru_Config_Inf_Tab);
			
			/* 传感器 数据存储 检测 */
			Log_Save_Flag_Check(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu,&System_Time);
#if (Print_Function_Enable == 0x01)
			/* 自动更新打印"起始时间"和"结束时间". */
			App_Print_Updata_Date(System_Time);
#endif

			RTC_System_Clock_Calibration(System_Time); //zwc
			
			/* 给调度器解锁 */
			OSSchedUnlock();
		}
		
		/* 100MS 更新 */
		if((Cur_time-Base_time_100ms_Reg) >= Task_State_Base_time_100ms)
		{
			/* 设置 当前 时间为100ms基准时间 */
			Base_time_100ms_Reg = Cur_time;
			/* 给调度器上锁 */
			OSSchedLock();
			
			/* 报警 报警提示 */
			Alarm_Reminding(&Instru_Config_Inf_Tab,&Instru_Fix_Inf_State_Tab);

			
			/* 给调度器解锁 */
			OSSchedUnlock();		
			/* 检测 配置数据 状态:有：发送数据;无:循环检测  */
//			Instru_Config_Inf_Check((GSM_SR_DR*)&GSM_Tab,&GSM_Config_Inf_Data,(void*)GSM_Config_Tab,&Instru_Config_Inf_Tab);
		}
		/* 50MS 更新 */
		if((Cur_time-Base_time_50ms_Reg) >= Task_State_Base_time_50ms)
		{
			/* 给调度器上锁 */
			OSSchedLock();

			/* 传感器校准 通信超时检测 */
			COM_Sensor_Cal_Timeout_Check(&Instru_Fix_Inf_State_Tab,Cur_time-Base_time_50ms_Reg);
			
			/* 设置 当前 时间为50ms基准时间 */
			Base_time_50ms_Reg = Cur_time;
			
			/* 电源连接状态检测 */
			Power_ET_Link_State_Check(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);
			/* 设置 外电报警状态 */
			Power_ET_SET_Alarm_State(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);
			/* 电池充电状态检测 */
			Power_BT_Charge_State_Check(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);
			/* 设置 电池电量报警状态 */
			Power_BT_SET_Alarm_State(&Instru_Config_Inf_Tab.Inf_State.Instru_Power,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Power);


			/* 给调度器解锁 */
			OSSchedUnlock();
		}
		/* 等待1MS，进入等待，放弃CPU使用权。 */
		OSTimeDly(Task_State_Update_Time);
	}
	
#undef Task_State_Update_Time
#undef Task_State_Base_time_50ms
#undef Task_State_Base_time_100ms
#undef Task_State_Base_time_500ms
#undef Task_State_Base_time_1000ms
#undef Task_State_Base_time_10000ms
#undef Task_State_Base_time_60000ms
}

/************************************************************************************************/
/* 函数名	: AppTaskCOM(串口配置仪器相关信息任务)														*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 通过配置工具来配置仪器的SN号、数据服务器IP地址及端口等信息										*/
/* 创建日期 	: 2014/9/11																			*/
/************************************************************************************************/
void AppTaskCOM(void *p_arg)
{
	INT8U      err;
	COM_SENSOR_CAL_TYPEDEF*   com_sensor_cal_q;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	while (1)  
	{    
		/* 等待 传感器校准队列 消息 */
		com_sensor_cal_q = (COM_SENSOR_CAL_TYPEDEF*)OSQPend(COM_Sensor_Cal_EVENT_Q,0,&err);
		
		/* 接收消息 状态 判断 */
		if(err == OS_ERR_NONE)
		{
			/* 消息被正确的接收 */
			/* 处理 传感器校准数据 */
			COM_Sensor_Cal_Deal(com_sensor_cal_q);
		}
	} 
} 

/************************************************************************************************/
/* 函数名	: AppTaskBeep(声音报警蜂鸣器操作任务)													 	 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用		: 根据报警类型发出不同的报警声音						 									*/
/* 创建日期 	: 2015/2/27																			*/
/************************************************************************************************/
void AppTaskBeep(void *p_arg)
{
	INT8U      err;
	Beep_QUE*   Beep_QUE_inf;
	OS_Q_DATA	Beep_Q_data;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	while (1)
	{
		/* 等待 队列 消息 */
		Beep_QUE_inf = (Beep_QUE*)OSQPend((OS_EVENT*)Beep_EVENT_Str_Q,0,&err);
		
		/* 接收消息 状态 判断 */
		if(err == OS_ERR_NONE)
		{
			/* 开 蜂鸣器 */
			Beep(Beep_ON);
			/* 等待 设置 时间。 */
			OSTimeDly(Beep_QUE_inf->Beep_Open_Time);
			/* 关 蜂鸣器 */
			Beep(Beep_OFF);
			/* 等待 设置 时间。 */
			OSTimeDly(Beep_QUE_inf->Beep_Close_Time); 
		}
		/* 设置 信息队列 运行完成标志 */
		Beep_QUE_inf->EVENT_Getarg = TRUE;
		
		/* 查询 蜂鸣器 消息队列 */
		err = OSQQuery((OS_EVENT*)Beep_EVENT_Str_Q,&Beep_Q_data);
	
		/* 判断查询结果 */
		if(err == OS_ERR_NONE)
		{
			/* 查询成功 */
			/* 寻找上一条发送成功的消息 */
			if((((OS_Q *)(Beep_EVENT_Str_Q->OSEventPtr))->OSQOut) == (((OS_Q *)(Beep_EVENT_Str_Q->OSEventPtr))->OSQStart))
			{
				/* 清除GSM消息队列上一条消息指针 */
				*((u32*)(((OS_Q *)(Beep_EVENT_Str_Q->OSEventPtr))->OSQEnd)-1) = 0;
			}else
			{
				/* 清除GSM消息队列上一条消息指针 */
				*((u32*)(((OS_Q *)(Beep_EVENT_Str_Q->OSEventPtr))->OSQOut)-1) = 0;
			}
		}
	}
}

/************************************************************************************************/
/* 函数名	: AppTaskBeep(光报警状态指示灯任务)													 	 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用		: 根据报警类型让报警指示灯进行闪烁						 									*/
/* 创建日期 	: 2015/2/27																			*/
/************************************************************************************************/
void AppTaskLight(void *p_arg)
{
	INT8U      err;
	LIGHT_QUE*   Light_QUE_inf;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	while (1)
	{
		/* 等待 队列 消息 */
		Light_QUE_inf = (LIGHT_QUE*)OSQPend((OS_EVENT*)Light_EVENT_Str_Q,0,&err);
		/* 接收消息 状态 判断 */
		if(err == OS_ERR_NONE)
		{
			/* 开 报警指示灯 */
			Light(Light_ON);
			/* 等待 设置 时间。 */
			OSTimeDly(Light_QUE_inf->Light_Open_Time);
			/* 关 报警指示灯 */
			Light(Light_OFF);
			/* 等待 设置 时间。 */
			OSTimeDly(Light_QUE_inf->Light_Close_Time); 
		}
		/* 设置 信息队列 运行完成标志 */
		Light_QUE_inf->EVENT_Getarg = TRUE;
	}

}

/************************************************************************************************/
/* 函数名	: AppTaskADC(ADC数据采集任务)														*/
/* 输入		: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用		: 通过ADC采集传感器模拟电压信息									*/
/* 创建日期	: 2014/9/11																			*/
/************************************************************************************************/
void AppTaskADC(void *p_arg)
{
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	
/* 判断温度补偿功能是否启动 */
#if ((Hum_Sensor_Function_Config_Enable == 0x01) &&\
	 (Hum_Sensor_Tmp_Comp_Enable == 0x01) &&\
	 (Hum_Sensor_Tmp_Comp_CH > 0))
	/* 获得湿度传感器温度补偿参数 */
	Humi_Get_Temp_Comp_Value();
#endif
	while (1)
	{    
		/* 设置模拟看门狗 */
		ADC_AWD_Run_Init();
		
		/* 采集ADC数据 */
		ADC_Get_Value();
		
		/*进入等待，	放弃CPU使用权*/
		OSTimeDly(ADC1_Collect_Interval); //ADC1_Collect_Interval代表传感器采集时间(是可调的)
		
	}
}

#if (Print_Function_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: AppTaskPrint(打印机任务)												*/
/* 输入		: (void *p_arg) 																	*/
/* 输出		: 无 																				      */
/* 作用		: 通过打印机将记录的数据打印出来									*/
/* 创建日期	: 2015/7/6																			*/
/************************************************************************************************/
void AppTaskPrint(void *p_arg)
{
	INT8U      err;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	/* 创建 打印 信号量 */
	Print_QUE_Sem	= OSSemCreate(0);	
	
	while (1)
	{ 
		/* 等待 队列 消息 */
		OSSemPend((OS_EVENT*)Print_QUE_Sem,0,&err);		
		
		/* 接收消息 状态 判断 */
		if(err == OS_ERR_NONE)
		{
			/* RF_Card 数据筛选 执行对应的动作 */
			Print_Data_Output(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print);
		}

	}
}

/************************************************************************************************/
/* 函数名	: AppTaskEasyPrint(一键打印任务)														*/
/* 输入		: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用		: 通过长按操作面板上的"左键"来实现一键打印功能										*/
/* 创建日期	: 2016/04/08																		*/
/************************************************************************************************/
void AppTaskEasyPrint(void *p_arg)
{
	(void)p_arg;
	while (1)
	{ 
		/*执行一键打印数据流程*/
		Easy_Print_Process_Flow(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor,&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print);
		/* 周期延时 */
		OSTimeDly(10);
	}
}
#endif

#if (GPS_Config_Enable == 0x01)
/************************************************************************************************/
/* 函数名	: AppTaskGPS(GPS任务)																*/
/* 输入		: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用		: 获取GPS定位信息*/
/* 创建日期	: 2015/7/30																			*/
/************************************************************************************************/
void AppTaskGPS(void *p_arg)
{
	/* 返回消息邮箱状态 */
	INT8U   err;
	(void)p_arg;   // 'p_arg' 并没有用到，防止编译器提示警告
	/* 创建GPS消息邮箱 */
	GPS_Box = OSMboxCreate((void *)0);
	/* 初始化GPS_Mcu */
	GPS_Mcu_Init();
	/* 初始化GPS_Log */
	GPS_Log_Init();
	/* 初始化GPS_Dev */
	GPS_Dev_Init();
	//这段时间是电源刚上电，模块开始运行，因此在模块稳定前不好向模块发送命令，
	//以免配置失败，还不知道。
	/* 设置延时等待GPS模块稳定 */
	OSTimeDly(500);
	/* 选择GPS模块输出消息内容。 */
	GPS_CMD_Config();
	while (1)
	{
		/* 等待消息邮箱 */
		OSMboxPend((OS_EVENT*)GPS_Box,0,&err);	
		/* 判断消息邮箱状态是否接收成功。 */
		if(err == OS_ERR_NONE)
		{
			/* GPS消息邮箱接收成功。*/
			/* 解析GPS定位信息。(将获得信息流数据转换成数据流。) */
			GPS_POS_Log_SR.Ana_State = GPS_Log_Ana_Inf();
			
			/* GPS定位信息解析成功。更新GPS信息数据. */
			GPS_App_Get_Data(&GPS_POS_Log_SR,&GPS_POS_Inf);
				
		}
	}
}
#endif
/******************* (C) ZHOU *****END OF FILE****/

