/******************** (C) COPYRIGHT 2009 www.armjishu.com ***********************
 * 文件名  ：date.c
 * 描述    ：无         
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------
 *          |                 |
 *          |      无         |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：www.armjishu.com Team
 * modify  ：fire  QQ: 313303034 
 * 博客    ：firestm32.blog.chinaunix.net
**********************************************************************************/  
#include "date.h"




#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)  //判断a % 4 == 0，返回1或0，对应输出366或365
#define	days_in_month(a) 	(month_days[(a) - 1])

static int month_days[12] = {	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };



u32 mktimev(Times *tm)
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
			tm->Year*365 - 734109
	    )*24 + tm->Hour /* now have hours 719499 */
	  )*60 + tm->Min /* now have minutes */
	)*60 + tm->Sec; /* finally seconds */
}


void to_tm(u32 tim,Times * tm)
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

	/*
	 * Determine the day of week
	 */
// 	GregorianDay(tm);
	
}

//比较两个字符串指定长度的内容是否相等
//参数:s1,s2要比较的两个字符串;len,比较长度
//返回值:1,相等;0,不相等
static unsigned char str_cmpx(u8*s1,u8*s2,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
	return 1;	   
}

/* **********************************************************************************

 * 函数名：Now_Time_Transition_Seconed_Counter
 * 描述  ：将当前编译的系统时间转换成RTC的秒数量
 *         RTC 计数寄存器中。
 * 输入  ：无
 * 输出  ：无
 * 调用  ：初始化RTC时钟

 * **********************************************************************************/
void Now_Time_Transition_Seconed_Counter(void)
{
	Times 			XX_time;	//定义时间结构体
	unsigned char 	XX_temp[3];	//临时存放月份字符串数据
	unsigned char 	XX_i;
	
	/* 获取月份字符串 */
	for(XX_i=0;XX_i<3;XX_i++)XX_temp[XX_i]=COMPILED_DATE[XX_i];
	
	/* 比较字符串中的数据与12个月份当中的那个相同，并获得当前的月份。 */
	for(XX_i=0;XX_i<12;XX_i++)
	{
		if(str_cmpx((u8*)Month_Tab[XX_i],XX_temp,3))
		{
			break;
		}
	}
	
	/* 获得月份 */
	XX_time.Month = XX_i+1;
	
	/* 获得时期 */
	if(COMPILED_DATE[4]==' ')
	{
		XX_time.Day = COMPILED_DATE[5]-'0'; 	
	}else
	{
		XX_time.Day = 10*(COMPILED_DATE[4]-'0') + COMPILED_DATE[5]-'0';  
	}
	
	/* 获得年份 */
	XX_time.Year = 1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')\
				   +10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';
	
	/* 获得小时 */
	XX_time.Hour = 10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';
	/* 获得分钟 */
	XX_time.Min  = 10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';
	/* 获得秒钟 */
	XX_time.Sec  = 10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';
		
	//将数据转换成秒数据
	Seconds_Counter = mktimev(&XX_time);
}

/*************************************************************************************************/
/* 函数名  ：Compression_Times_Data(时间任务)												 	 */
/* 输入    ：(void *p_arg) 																		 */
/* 输出	   : 无 																				 */
/* 作用	   ：将当前的时间数据转换成压缩格式的时间数据。		 									 */
/* 创建日期：2014/10/09																			 */
/*************************************************************************************************/
void Compression_Times_Data(Times* Tm,DATETIME_Storage_Format* Compression_Data)
{
   /* 获得秒数据 */
   Compression_Data->DATETIME_Bit.SECOND_BITS = Tm->Sec;
   /* 获得分数据 */
   Compression_Data->DATETIME_Bit.MINUTE_BITS = Tm->Min;
   /* 获得时数据 */
   Compression_Data->DATETIME_Bit.HOUR_BITS   = Tm->Hour;

   /* 获得日数据 */
   Compression_Data->DATETIME_Bit.DAY_BITS    = Tm->Day;
   /* 获得月数据 */
   Compression_Data->DATETIME_Bit.MONTH_BITS  = Tm->Month;
   /* 获得年数据 */
   Compression_Data->DATETIME_Bit.YEAR_BITS   = Tm->Year - STARTOFTIME;

}





/******************** (C) COPYRIGHT 2009 www.armjishu.com ***********************/

