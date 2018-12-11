#ifndef  APP_CFG_H    
#define  APP_CFG_H   
   
/*******************设置任务优先级*******************/
#define Startup_Task_PRIO      (4u)   						//开始		任务优先级

#define OS_TASK_TMR_PRIO       (Startup_Task_PRIO+1)  		//软件定时器     优先级

#define APP_TASK_RFCard_PRIO   (OS_TASK_TMR_PRIO+1)   		//RF卡片		任务优先级

#define APP_TASK_Save_PRIO     (APP_TASK_RFCard_PRIO+1)   	//信息保存	任务优先级

#define APP_TASK_Dis_PRIO      (APP_TASK_Save_PRIO+1)   	//显示		任务优先级

#define APP_TASK_Key_PRIO      (APP_TASK_Dis_PRIO+1)   		//按键		任务优先级

#define APP_TASK_Time_PRIO     (APP_TASK_Key_PRIO+1)   		//时间		任务优先级

#define APP_TASK_GPS_PRIO      (APP_TASK_Time_PRIO+1)   	//GPS		任务优先级

#define APP_TASK_State_PRIO    (APP_TASK_GPS_PRIO+1)   	    //状态更新	任务优先级

#define APP_TASK_Gateway_PRIO  (APP_TASK_State_PRIO+1)   	//网关		任务优先级

#define APP_TASK_GatewayHZZH_PRIO  (APP_TASK_Gateway_PRIO+1)   	//杭州创辉网关任务优先级

#define APP_TASK_Upg_PRIO      (APP_TASK_GatewayHZZH_PRIO+1)   	//程序升级	任务优先级

#define APP_TASK_COM_PRIO      (APP_TASK_Upg_PRIO+1)   		//通信		任务优先级

#define APP_TASK_Beep_PRIO     (APP_TASK_COM_PRIO+1)   		//Beep		任务优先级

#define APP_TASK_Light_PRIO    (APP_TASK_Beep_PRIO+1)   	//Light		任务优先级

#define APP_TASK_ADC_PRIO      (APP_TASK_Light_PRIO+1)	  	//ADC采样	任务优先级

#define APP_TASK_Print_PRIO    (APP_TASK_ADC_PRIO+1)	  	//打印		任务优先级

#define APP_TASK_Easy_Print_PRIO (APP_TASK_Print_PRIO+1)	//一键打印	任务优先级

#define APP_TASK_GSM_PRIO       (APP_TASK_Easy_Print_PRIO+1) //GPRS		任务优先级

#define APP_TASK_GSM_Config_PRIO       (APP_TASK_GSM_PRIO+1) //GSM	配置	任务优先级



/************设置栈大小（单位为 OS_STK ）************/   
#define STARTUP_TASK_STK_SIZE   	80			//开始		任务堆栈大小

#define APP_TASK_RFCard_STK_SIZE   	80			//RF卡片		任务堆栈大小

#define APP_TASK_Save_STK_SIZE  	80			//信息保存	任务堆栈大小

#define APP_TASK_ADC_STK_SIZE  		256			//ADC采样	任务堆栈大小

#define APP_TASK_Dis_STK_SIZE  		256			//显示		任务堆栈大小

#define APP_TASK_Key_STK_SIZE  		80			//按键		任务堆栈大小

#define APP_TASK_Time_STK_SIZE  	80			//时间		任务堆栈大小

#define APP_TASK_GPS_STK_SIZE  		256			//GPS		任务堆栈大小

#define APP_TASK_State_STK_SIZE 	(768u)		//状态更新	任务堆栈大小
 
#define APP_TASK_Upg_STK_SIZE   	80			//程序升级	任务堆栈大小

#define APP_TASK_COM_STK_SIZE  		512			//通信		任务堆栈大小

#define APP_TASK_Beep_STK_SIZE  	80			//蜂鸣器		任务堆栈大小

#define APP_TASK_Light_STK_SIZE 	80			//光提示		任务堆栈大小

#define APP_TASK_Print_STK_SIZE 	128			//打印		任务堆栈大小

#define APP_TASK_Easy_Print_STK_SIZE (128u)		//一键打印	任务堆栈大小


#define GSM_Config_STK_SIZE (128u)		//GSM模块配置任务(主要是Wifi模块的配置)



#define APP_TASK_GSM_STK_SIZE   	(2048u)		//GPRS		任务堆栈大小



#endif 
