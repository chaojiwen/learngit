/************************************************************************************************/
/** @file              : Dev_RTC.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月25日															*/
/** @brief             : c file																	*/
/** @description       : RTC 驱动文件														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Log_GSM.h"
#include "Log_Save.h"

#include "Dev_RTC.h"

#include "app.h"
#include "interface.h"


/* ********************************************************************************** */
/* 说明：																			  */
/* 1、RTC最大能保存100年的数据新数据。（从2000年1月1日到2099年12月31日）					  */
/* 2、																				  */
/* 3、																				  */
/* ********************************************************************************** */



/* ******************************* 月份对应的日期数 ************************************ */
u16 month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/* ***************************** 秒中数与公历时间换算 ********************************** */
/* 基准时间相关 */
#define FEBRUARY		(2u)
#define	STARTOFTIME		(2000u)
#define SECDAY			(86400uL)

#define	STARTYEAR		STARTOFTIME
#define	ENDYEAR			(2100u)
#define	MONTH			(12u)
#define	DAY				(28u)

#define	HOUR			(23u)
#define	MINUTE			(59u)
#define	SECOND			(59u)

#define SECYR				(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)  //判断a % 4 == 0，返回1或0，对应输出366或365
#define	days_in_month(a) 	(month_days[(a) - 1])

/* ***************************** RTC 内部寄存器 ********************************** */
#define RTC_Sec_Reg_Adder			((int)0X00000000)		//秒寄存器
#define RTC_Min_Reg_Adder			((int)0X00000001)		//分寄存器
#define RTC_Hour_Reg_Adder			((int)0X00000002)		//时寄存器
#define RTC_Week_Reg_Adder			((int)0X00000003)		//星期寄存器
#define RTC_Day_Reg_Adder			((int)0X00000004)		//日期寄存器
#define RTC_Mon_Reg_Adder			((int)0X00000005)		//月份寄存器
#define RTC_Year_Reg_Adder			((int)0X00000006)		//年份寄存器
	
#define RTC_Digit_Offset_Reg_Adder	((int)0X00000007)		//时钟调整寄存器
	
#define RTC_Alarm_W_Min_Reg_Adder	((int)0X00000008)		//报警W 分 寄存器
#define RTC_Alarm_W_Hour_Reg_Adder	((int)0X00000009)		//报警W 时 寄存器
#define RTC_Alarm_W_Week_Reg_Adder	((int)0X0000000A)		//报警W 星期 寄存器
	
#define RTC_Alarm_D_Min_Reg_Adder	((int)0X0000000B)		//报警D 分 寄存器
#define RTC_Alarm_D_Hour_Reg_Adder	((int)0X0000000C)		//报警D 时 寄存器
	
#define RTC_Reserved				((int)0X0000000D)		//保留

#define RTC_Control_1_Reg_Adder		((int)0X0000000E)		//控制寄存器 1 寄存器
#define RTC_Control_2_Reg_Adder		((int)0X0000000F)		//控制寄存器 2 寄存器
	
/* 定义 RTC 数据结构 */
typedef RTC8025_Typedef RTC_TYPEDEF;
/* 定义 RTC 控制结构 */
typedef RTC8025_Ctrl_Typedef RTC_CTRL_TYPEDEF;

/* RTC 文件内部的变量. */
static RTC_TYPEDEF	RTC_Time;	//zwc	
static RTC_TYPEDEF	Last_RTC_Time;//上一时刻RTC的值
static Times  RTC_Offline_Cok_DR;
extern void Write_Config_Tab(u8* buffer_p,u16 count);//写仪器配置表信息



static RTC_CTRL_TYPEDEF RTC_Ctrl;	

/* 系统时钟校准状态寄存器(FALSE:未校准;TRUE:已校准) */
////////static BOOL RTC_System_Clock_Calibration_State_Reg = FALSE;

BOOL RTC_System_Clock_Calibration_State_Reg = FALSE;//2018.12.4  By  ZWC  
/************************************************************************************************/
/* 函数名	: RTC_Write																		 	*/
/* 输入  	: *pdata:数据指针;addr:操作地址;	count:操作数量;										*/
/* 输出		: 执行结果。																			*/
/* 作用	 	: 写RTC操作。								 									 	*/
/* 创建日期 	: 2015/8/27																			*/
/************************************************************************************************/
static BOOL RTC_Write(char *pdata, int addr, int count)
{
	/* 操作结果. */
	BOOL res = FALSE;
	
	/* 给调度器上锁 */   
	OSSchedLock();
	res = (BOOL)RTC8025_Write(pdata, addr, count);
	/* 给调度器解锁 */
	OSSchedUnlock();
	
	return res;
}

/************************************************************************************************/
/* 函数名	: RTC_Read																		 	*/
/* 输入  	: *pdata:数据指针;addr:操作地址;	count:操作数量;										*/
/* 输出		: 执行结果。																			*/
/* 作用	 	: 读RTC操作。								 									 	*/
/* 创建日期 	: 2015/8/27																			*/
/************************************************************************************************/
BOOL RTC_Read(char *pdata, int addr, int count)
{

	/* 操作结果. */
	BOOL res = FALSE;
	
	/* 给调度器上锁 */   
	OSSchedLock();
	res = (BOOL)RTC8025_Read(pdata, addr, count);
	/* 给调度器解锁 */
	OSSchedUnlock();
	
	return res;
}

/************************************************************************************************/
/* 函数名	: RTC_Reset																		 	*/
/* 输入  	: need_reset:复位需求;																*/
/* 输出		: 执行结果。																			*/
/* 作用	 	: 根据need_reset和检测RTC状态寄存器 复位RTC。	 									 	*/
/* 创建日期 	: 2015/8/27																			*/
/************************************************************************************************/
BOOL RTC_Reset(BOOL need_reset)
{

	/* 操作结果. */
	BOOL res = FALSE;
	
	/* 给调度器上锁 */
	OSSchedLock();
	res = (BOOL)RTC8025_Reset(need_reset);
	/* 给调度器解锁 */
	OSSchedUnlock();
	
	return res;
}

/* **********************************************************************************
 * 函数名：BCD_To_HEX
 * 描述  ：将 BCD码 转换 成 HEX码
 * 输入  ：bcd_data：BCD码数据
 * 输出  ：HEX码数据
 * **********************************************************************************/
u8 BCD_To_HEX(u8 bcd_data)   
{   
    u8 temp;   
    temp=(bcd_data/16*10 + bcd_data%16);   
    return temp;   
}   

/* **********************************************************************************
 * 函数名：HEX_To_BCD
 * 描述  ：将 HEX码 转换 成 BCD码
 * 输入  ：hex_data: HEX妈数据
 * 输出  ：BCD码数据
 * **********************************************************************************/
u8 HEX_To_BCD(u8 hex_data)    //HEX转为BCD子程序      
{   
    u8 temp;   
    temp=(hex_data/10*16 + hex_data%10);   
    return temp;   
}
/**********取出一个字节数据的高4位****************/
u8 HEX_Hi4(u8 Data)
{
	u8 DataHi4=0;
  DataHi4 = (Data & 0xf0) >> 4;
   return DataHi4;
}
/**********取出一个字节数据的低4位****************/
u8 HEX_Lo4(u8 Data)
{
	u8 DataLo4=0;
  DataLo4 = Data & 0x0f;
	return  DataLo4;
}






/* **********************************************************************************
 * 函数名：RTC_Init
 * 描述  ：根据 需要 对 RTC8025 进行复位操作
 * 输入  ：reset_state: 手动/自动复位状态标志
 * 输出  ：无
 * **********************************************************************************/
BOOL RTC_Init(BOOL reset_state)
{   
	BOOL state;
	
	state = RTC_Reset(reset_state);
	
	/* 根据对 RTC 复位检测 */
	if(state == FALSE)
	{
		// Setting DateTime
		RTC_Time.STREG.Years 		= 1;
		RTC_Time.STREG.Months 		= 1;
		RTC_Time.STREG.Days			= 1;
		RTC_Time.STREG.Hours		= 0;
		RTC_Time.STREG.Minutes		= 0;
		RTC_Time.STREG.Seconds 		= 0;
		RTC_Time.STREG.Weekdays 	= 0;
		RTC_Time.STREG.YearCarry 	= 0;
		RTC_Write(RTC_Time.REGS, 0, 7);	
	}
	
	return state;
}

/* **********************************************************************************
 * 函数名：RTC_FOE_GPIO_Config
 * 描述  ：控制 RTC 32.768Khz时钟 输出
 * 输入  ：无
 * 输出  ：无
 * **********************************************************************************/
void RTC_FOE_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//LED2/LED1
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/* 根据 RTC 的FOE引脚说明， 低电平或浮空状态时，不输出32.768Khz脉冲. */	
	GPIO_ResetBits(GPIOC,GPIO_Pin_10);// turn off all led  
}

/* **********************************************************************************
 * 函数名：RTC_WakeUp_PA0_Config
 * 描述  ：配置 RTC 看门狗功能（PA0 作为 外部中断引脚，间隔1s触发一次）
 * 输入  ：无
 * 输出  ：无
 * **********************************************************************************/
void RTC_WakeUp_PA0_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/* config the extiline(PA0) clock and AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

	/* EXTI line gpio config(PA0) */	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line(PA0) mode config */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0); 
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //边沿触发中断
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

/* **********************************************************************************
 * 函数名：RTC_Mcu_Out_Config
 * 描述  ：Mcu 接收到RTC的触发信号，结果信号处理，输出一个脉冲信号，
		  对外部的看门狗芯片MAX823进行 喂狗操作。
 * 输入  ：无
 * 输出  ：无
 * **********************************************************************************/
void MAX823_WDI_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(MAX823_WDI_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin		= MAX823_WDI_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(MAX823_WDI_PORT, &GPIO_InitStructure);	
	GPIO_SetBits(MAX823_WDI_PORT, MAX823_WDI_PIN );// turn off all led
}

/* **********************************************************************************
 * 函数名：Read_System_Date_Time_Data
 * 描述  ：从 RTC 中读取 当前时间 数据
 * 输入  ：time：存放数据的 指针
 * 输出  ：无
 * **********************************************************************************/
void Read_System_Date_Time_Data(Times* time)
{
	/* 读取 RTC 的时间数据 */
	RTC_Read(RTC_Time.REGS,0x00,7);
	/* 由于 读出的是 BCD 码，需要转换成HEX码. */
	time->Year 	= (BCD_To_HEX(RTC_Time.STREG.Years) + STARTYEAR);
	time->Month = BCD_To_HEX(RTC_Time.STREG.Months);
	time->Day	= BCD_To_HEX(RTC_Time.STREG.Days);
	time->Hour	= BCD_To_HEX(RTC_Time.STREG.Hours);
	time->Min	= BCD_To_HEX(RTC_Time.STREG.Minutes);
	time->Sec 	= BCD_To_HEX(RTC_Time.STREG.Seconds);
	time->Week 	= BCD_To_HEX(RTC_Time.STREG.Weekdays);

}

/* **********************************************************************************
 * 函数名：RTC_Alarm_D_Config
 * 描述  ：配置 报警 D 的固定周期中断功能
 * 输入  ：reg：存放数据的 指针
 * 输出  ：无
 * **********************************************************************************/
void RTC_Alarm_D_Config(u8 reg)
{
	/* 写入 RTC 的时间数据 */
	RTC_Write((char*)&reg,RTC_Control_1_Reg_Adder,1);
}

/* **********************************************************************************
 * 函数名：Write_System_Date_Time_Data
 * 描述  ：向 RTC 中写入 当前时间 数据
 * 输入  ：time：存放数据的 指针
 * 输出  ：无
 * **********************************************************************************/
void Write_System_Date_Time_Data(Times* time)
{
	/* 由于 读出的是 BCD 码，需要转换成HEX码. */
	if(time->Year >= ENDYEAR)
	{
		/* RTC 年数据溢出，取百位以后的数据. */
		RTC_Time.STREG.Years	  = HEX_To_BCD(time->Year%100);
		/* RTC 年数据溢出，设置溢出标志. */
		RTC_Time.STREG.YearCarry  = 1;
	}else
	{
		/* 年数据未溢出， */
		RTC_Time.STREG.Years	  = HEX_To_BCD(time->Year - STARTYEAR);		
	}
	/* 月份 */
	RTC_Time.STREG.Months   = HEX_To_BCD(time->Month);
	/* 日期 */
	RTC_Time.STREG.Days	  	= HEX_To_BCD(time->Day);
	/* 时 */
	RTC_Time.STREG.Hours	= HEX_To_BCD(time->Hour);
	/* 分 */
	RTC_Time.STREG.Minutes  = HEX_To_BCD(time->Min);
	/* 秒 */
	RTC_Time.STREG.Seconds  = HEX_To_BCD(time->Sec);
	/* 星期 */
	RTC_Time.STREG.Weekdays = BCD_To_HEX(time->Week);
	
	/* 写入 RTC 的时间数据 */
	RTC_Write(RTC_Time.REGS,0x00,7);

}
/*****************************************************/
void Write_System_Set(Times* time)
{
	/* 由于 读出的是 BCD 码，需要转换成HEX码. */
	if(time->Year >= ENDYEAR)
	{
		/* RTC 年数据溢出，取百位以后的数据. */
		RTC_Time.STREG.Years	  = HEX_To_BCD(time->Year%100);
		/* RTC 年数据溢出，设置溢出标志. */
		RTC_Time.STREG.YearCarry  = 1;
	}else
	{
		/* 年数据未溢出， */
		RTC_Time.STREG.Years	  = (time->Year - STARTYEAR);		
	}
	/* 月份 */
	RTC_Time.STREG.Months   =(time->Month);
	/* 日期 */
	RTC_Time.STREG.Days	  	= (time->Day);
	/* 时 */
	RTC_Time.STREG.Hours	= (time->Hour);
	/* 分 */
	RTC_Time.STREG.Minutes  =(time->Min);
	/* 秒 */
	RTC_Time.STREG.Seconds  = (time->Sec);
	/* 星期 */
	RTC_Time.STREG.Weekdays = (time->Week);
	
	/* 写入 RTC 的时间数据 */
	RTC_Write(RTC_Time.REGS,0x00,7);

}

/*************************************************************************************************/
/* 函数名  ：Compression_Times_Data(时间任务)												 		 */
/* 输入    ：(void *p_arg) 																		 */
/* 输出	   : 无 																				 */
/* 作用	   ：将当前的时间数据转换成压缩格式的时间数据。		 										 */
/* 创建日期：2014/10/09																			 */
/*************************************************************************************************/
void Compression_Times_Data(Times* Tm,INSTRU_CONFIG_INF_TAB* tab)
{
	/* 存储数据的时间戳 */
	DATETIME_Storage_Format save_data_timestamp;

	/* 获得秒数据 */
	save_data_timestamp.DATETIME_Bit.SECOND_BITS = Tm->Sec;
	/* 获得分数据 */
	save_data_timestamp.DATETIME_Bit.MINUTE_BITS = Tm->Min;
	/* 获得时数据 */
	save_data_timestamp.DATETIME_Bit.HOUR_BITS   = Tm->Hour;
	/* 获得日数据 */
	save_data_timestamp.DATETIME_Bit.DAY_BITS    = Tm->Day;
	/* 获得月数据 */
	save_data_timestamp.DATETIME_Bit.MONTH_BITS  = Tm->Month;
	/* 获得年数据 *///这里的年，最大保存63年，从2010年开始算起，超过将重新从2010开始算
	save_data_timestamp.DATETIME_Bit.YEAR_BITS   = Tm->Year - STARTOFTIME;	
	
	tab->Inf_State.Instru_RTC.Inf_State.Year	= Tm->Year;
	tab->Inf_State.Instru_RTC.Inf_State.Month	= Tm->Month;
	tab->Inf_State.Instru_RTC.Inf_State.Day		= Tm->Day;
	tab->Inf_State.Instru_RTC.Inf_State.Hour	= Tm->Hour;
	tab->Inf_State.Instru_RTC.Inf_State.Min		= Tm->Min;
	tab->Inf_State.Instru_RTC.Inf_State.Sec		= Tm->Sec;
	
	/* 获取 存储 的时间戳 */
	Save_Get_Timestamp(&save_data_timestamp);
	
}

/* **********************************************************************************
 * 函数名：RTC_WakeUp_Func
 * 描述  ：RTC 中断功能
 * 输入  ：无
 * 输出  ：无
 * **********************************************************************************/
void RTC_WakeUp_Func(void)
{


}

/* **********************************************************************************
 * 函数名：RTC_Read_Control_Reg
 * 描述  ：读取控制器寄存器数据
 * 输入  ：无
 * 输出  ：无
 * **********************************************************************************/
void RTC_Read_Control_Reg(void)
{
	
	RTC_Read((char*)&RTC_Ctrl.Control, 0x0E, 2);
	
}

/* **********************************************************************************
 * 函数名：Date_Time_To_Second
 * 描述  ：将公历时间日期转换成秒数据
 * 输入  ：时间日期数据结构
 * 输出  ：秒数据
 * **********************************************************************************/
u32 RTC_Date_Time_To_Second(Times *tm)
{
	if (tm->Month > 2) 			/* 1..12 -> 11,12,1..10 */
	{								
		tm->Month -= 2;
	}else
	{
		tm->Month += 10;		/* Puts Feb last since it has leap day */
		tm->Year -= 1;
	}

	return (((
		(u32) (tm->Year/4 - tm->Year/100 + tm->Year/400 + 367*tm->Month/12 + tm->Day) +
			tm->Year*365 - 730456
	    )*24 + tm->Hour /* now have hours 719499 */
	  )*60 + tm->Min /* now have minutes */
	)*60 + tm->Sec; /* finally seconds */
}


/* **********************************************************************************
 * 函数名：Second_To_Date_Time
 * 描述  ：将秒数据转换成公历的时间日期
 * 输入  ：当前的秒数据、时间数据结构；
 * 输出  ：无
 * **********************************************************************************/
void RTC_Second_To_Date_Time(u32 tim,Times * tm)
{
	register u32    i;
	register long   hms, day;

	day = tim / SECDAY;
	hms = tim % SECDAY;

	/* Hours, minutes, seconds are easy */
	tm->Hour = hms / 3600;
	tm->Min = (hms % 3600) / 60;
	tm->Sec = (hms % 3600) % 60;

	/* Number of years in days */ /*算出当前年份，起始的计数年份为1970年*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->Year = i;

	/* Number of months in days left */ /*计算当前的月份*/
	if (leapyear(tm->Year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->Month = i;

	/* Days are what is left over (+1) from all that. *//*计算当前日期*/
	tm->Day = day + 1;
	
}

/************************************************************************************************/
/* 函数名	: RTC_System_Clock_Calibration_State_Check										 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 检测系统启动后时钟是否校准过。				 									 	*/
/* 创建日期 	: 2015/6/26																			*/
/************************************************************************************************/
void RTC_System_Clock_Calibration_State_Check(void)
{

	/* 判断系统启动后时钟是否校准过。 */
	if(RTC_System_Clock_Calibration_State_Reg == FALSE)
	{
		/* 系统启动后时钟未校准 */
		/* 发送时钟校准信息。（向服务器） */
		RTC_System_Clock_Calibration_State_Reg = GSM_Send_Q(GSM_EVENT_Str_Q,System_Clock_Calibration_EVENT);
	}

}

/************************************************************************************************/
/* 函数名	: RTC_System_Clock_Calibration_State_Reset										 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 复位系统时钟校准状态寄存器				 									 	*/
/* 创建日期 	: 2015/6/26																			*/
/************************************************************************************************/
void RTC_System_Clock_Calibration_State_Reset(void)
{

	/* 复位系统时钟校准状态寄存器 */
	RTC_System_Clock_Calibration_State_Reg = FALSE;
	
}





/* **********************************************************************************
 * 函数名： Check_RTC_Time
 * 描述  ：判断BCD码是否合法 年：17~35;月：01~12；日：01~31；时：00~23;分：00~59;秒：00~59
 * 输入  ：无
 * 输出  ：BCD码合法返回true；否则返回false
 * **********************************************************************************/
static bool Check_RTC_Time(void)
{
	bool ret=false;
	
		/* 读取 RTC 的时间数据 */
	RTC_Read(RTC_Time.REGS,0x00,7);//先读取RTC中的数据
	
	
	 if(  ((RTC_Time.STREG.Years)>0x16)&&((RTC_Time.STREG.Years)<0x35)&&   
	      ((RTC_Time.STREG.Months)>=0x01)&&((RTC_Time.STREG.Months)<0x13)&& 
	      ((RTC_Time.STREG.Days)>=0x01)&&((RTC_Time.STREG.Days)<0x32)&&   
	      ((RTC_Time.STREG.Hours)<0x24)&&  
	      ((RTC_Time.STREG.Minutes)<0x60)&&  
	      ((RTC_Time.STREG.Seconds)<0x60)
	   )
	{
		ret=true;
	}
	
	

	return ret;
}

/*************RTC出错时的相关操作**************************************/

void RTC_Equipment_Offline(void)
{

    if(Check_RTC_Time())//如果RTC的值未出错
		{
		        Last_RTC_Time=RTC_Time;//记录上一时刻RTC未出错时的值
		}
		else
		{
	     	    RTC_Time=Last_RTC_Time;
			/* 由于 读出的是 BCD 码，需要转换成HEX码. */
	RTC_Offline_Cok_DR.Year 	= (BCD_To_HEX(RTC_Time.STREG.Years) + STARTYEAR);
	RTC_Offline_Cok_DR.Month = BCD_To_HEX(RTC_Time.STREG.Months);
	RTC_Offline_Cok_DR.Day	= BCD_To_HEX(RTC_Time.STREG.Days);
	RTC_Offline_Cok_DR.Hour	= BCD_To_HEX(RTC_Time.STREG.Hours);
	RTC_Offline_Cok_DR.Min	= BCD_To_HEX(RTC_Time.STREG.Minutes);
	RTC_Offline_Cok_DR.Sec 	= BCD_To_HEX(RTC_Time.STREG.Seconds);
	RTC_Offline_Cok_DR.Week 	= BCD_To_HEX(RTC_Time.STREG.Weekdays);
			
			
		
	/* 由于 读出的是 BCD 码，需要转换成HEX码. */
	if(RTC_Offline_Cok_DR.Year >= ENDYEAR)
	{
		/* RTC 年数据溢出，取百位以后的数据. */
		RTC_Time.STREG.Years	  = HEX_To_BCD(RTC_Offline_Cok_DR.Year%100);
		/* RTC 年数据溢出，设置溢出标志. */
		RTC_Time.STREG.YearCarry  = 1;
	}else
	{
		/* 年数据未溢出， */
		RTC_Time.STREG.Years	  = HEX_To_BCD(RTC_Offline_Cok_DR.Year - STARTYEAR);		
	}
	/* 月份 */
	RTC_Time.STREG.Months   = HEX_To_BCD(RTC_Offline_Cok_DR.Month);
	/* 日期 */
	RTC_Time.STREG.Days	  	= HEX_To_BCD(RTC_Offline_Cok_DR.Day);
	/* 时 */
	RTC_Time.STREG.Hours	= HEX_To_BCD(RTC_Offline_Cok_DR.Hour);
	/* 分 */
	RTC_Time.STREG.Minutes  = HEX_To_BCD(RTC_Offline_Cok_DR.Min);
	/* 秒 */
	RTC_Time.STREG.Seconds  = HEX_To_BCD(RTC_Offline_Cok_DR.Sec);
	/* 星期 */
	RTC_Time.STREG.Weekdays = BCD_To_HEX(RTC_Offline_Cok_DR.Week);
	
	
	
	
	
	
	
	
	/* 写入 RTC 的时间数据 */
	RTC_Write(RTC_Time.REGS,0x00,7);		
			
			
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year 	= RTC_Offline_Cok_DR.Year;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month 	= RTC_Offline_Cok_DR.Month;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day 	= RTC_Offline_Cok_DR.Day;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour 	= RTC_Offline_Cok_DR.Hour;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min 	= RTC_Offline_Cok_DR.Min;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec 	= RTC_Offline_Cok_DR.Sec;
		
		//为了解决RTC时钟芯片意外被写故障，这里先保存一次当前的实时时钟数据为\
		  当上电启动检测到RTC被异常后可以使用这个时间来做作实时时间。
		/* 修改 仪器 信息表 设置标志 */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_RTC,
					    sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC));
	
	
	
			
			
		/* 设置更新RTC时钟 向服务器对时*/
		RTC_System_Clock_Calibration_State_Reg = FALSE;
		
		
		}





}






/************************************************************************************************/
/* 函数名	: RTC_System_Clock_Calibration													 	*/
/* 输入  	: (void *p_arg) 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 校准系统时钟。								 									 	*/
/* 创建日期 	: 2015/6/26																			*/
/************************************************************************************************/
void RTC_System_Clock_Calibration(Times system_time)
{
	/* 系统时间 */
	u32 system_second_b;
	/* 打印更新时间 */
	u32 updata_time;


	/* 转换当前的系统时间，获得系统的秒中数量. */
	system_second_b = RTC_Date_Time_To_Second(&system_time);
	/* 设置更新时间 */
	updata_time = system_second_b%SECDAY;

	/* 判断更新到达时间. */
	if((updata_time == System_Clock_Time_Updata)||(Check_RTC_Time()==false))//凌晨1点或RTC读出的值不合法向服务器对时
	{
		/* 设置更新RTC时钟 */
		RTC_System_Clock_Calibration_State_Reg = FALSE;
	}
	

 RTC_Equipment_Offline();//对RTC出错时的操作zwc
	
	
	
	
}



/******************* (C) ZHOU *****END OF FILE****/

