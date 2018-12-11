#include "Mcu_led.h"
#include "app.h"



/* 光 无信号 类型 消息 */
//static LIGHT_QUE Light_QUE_Null_Alarm_Signal_Module		= {1u,1000u,TRUE};
/* 传感器 光 报警 消息 */
static LIGHT_QUE Light_QUE_Sensor_Alarm_Signal_Module		= {500u,500u,TRUE};
/* 掉电 光 报警 消息 */
static LIGHT_QUE Light_QUE_Power_Alarm_Signal_Module		= {100u,100u,TRUE};
/* 掉电 光 报警 消息 */
static LIGHT_QUE Light_QUE_Battery_Alarm_Signal_Module		= {200u,200u,TRUE};

/*
 * 函数名：LED_GPIO_Init
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void LED_GPIO_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	//LED2/LED1
	GPIO_InitStructure.GPIO_Pin		= LED2_PIN | LED1_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	LED1(LED_OFF);
	Light(Light_OFF);
}

/*
*********************************************************************************************************
*	函 数 名: Battery_Alarm_Light
*	功能说明: 电池电量 光报警
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Battery_Alarm_Light(void)
{					
	Light_Send_Q((OS_EVENT*)Light_EVENT_Str_Q,Battery_Signal_Light_EVENT);
}

/*
*********************************************************************************************************
*	函 数 名: Power_Alarm_Light
*	功能说明: 外接电 光报警
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Power_Alarm_Light(void)
{					
	Light_Send_Q((OS_EVENT*)Light_EVENT_Str_Q,Power_Signal_Light_EVENT);
}

/*
*********************************************************************************************************
*	函 数 名: Sensor_Alarm_Light
*	功能说明: 传感器 光报警
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Sensor_Alarm_Light(void)
{					
	Light_Send_Q((OS_EVENT*)Light_EVENT_Str_Q,Sensor_Signal_Light_EVENT);
}



/*
*********************************************************************************************************
*	函 数 名: Heart_Runing_Led
*	功能说明: 心跳运行指示灯
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Heart_Runing_Led(void)
{				
   static BOOL LED1_Flag=FALSE;//控制心跳灯亮灭Flag 默认是灭的状态	
	/*********************心跳灯控制方法LEDZWC****************************/			
			   if(LED1_Flag==TRUE)
				 {
					 LED1_Flag=FALSE;
				   LED1(LED_ON);//ZWCled
				 
				 }
				 else if(LED1_Flag==FALSE)
				 {
					 LED1_Flag=TRUE;
				   LED1(LED_OFF);//ZWCled
				 }				 					
/***************************END***************************************/
	
}



/*
*********************************************************************************************************
*	函 数 名: Light_Send_Q
*	功能说明: 光报警
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Light_Send_Q(OS_EVENT* pevent,LIGHT_CMD_TYPE EVENT_Type)
	{
	INT8U     	err;
	OS_Q_DATA	Q_data;
	LIGHT_QUE*	EVENT_p;
	u16 		num;
	u32 		Q_p;

	
	/* 发送的 事件 类型  */
	if(EVENT_Type == Null_Signal_Light_EVENT)
	{
		/* 无信号 类型 */
		/* 暂时 没有用 */
		return;
		
	}else if(EVENT_Type == Sensor_Signal_Light_EVENT)
	{
		/* 传感器 声音 报警 */
		EVENT_p = &Light_QUE_Sensor_Alarm_Signal_Module;	
	}else if(EVENT_Type == Power_Signal_Light_EVENT)
	{
		/* 传感器 声音 报警 */
		EVENT_p = &Light_QUE_Power_Alarm_Signal_Module;	
	}else if(EVENT_Type == Battery_Signal_Light_EVENT)
	{
		/* 传感器 声音 报警 */
		EVENT_p = &Light_QUE_Battery_Alarm_Signal_Module;	
	}
	
	
	/* 程序报道这里 说明程序中不存在 上传数据消息 */
	/* 判断 上传数据 执行结果 */
	if(EVENT_p->EVENT_Getarg == FALSE)
	{
		/* 上传数据消息 正在 被执行，等待执行结束 */
		return;
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
				return;
			}
			/* 设置指针到下一个位置 */
			Q_p += sizeof(LIGHT_QUE);
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
		__nop();
	}
	
	/* 程序到这里就说明 队列中没有存在 信号强度 消息，
	   因此，需要发送一次 信号强度更新 消息。 */
	/* 清楚 消息 执行结果 */
	EVENT_p->EVENT_Getarg = FALSE;
	/* 发送 消息 *///FIFO	
	err = OSQPost(pevent, EVENT_p);  
	/* 判断 消息 发送 状态 */
	if(err == OS_ERR_NONE)
	{
		/* 判断 消息 发送 成功 */
		__nop();
	}else if(err == OS_ERR_Q_FULL)
	{
		/* 消息队列已满,不能再接收新消息 */
		/* 这种情况 不可能 出现 */
		while(1);
	}else
	{
		__nop();
	}
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

