/******************** (C) COPYRIGHT 2009 www.armjishu.com ***********************
 * �ļ���  ��date.c
 * ����    ����         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |      ��         |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��www.armjishu.com Team
 * modify  ��fire  QQ: 313303034 
 * ����    ��firestm32.blog.chinaunix.net
**********************************************************************************/  
#include "date.h"




#define SECYR			(SECDAY * 365)
#define	leapyear(year)		((year) % 4 == 0)
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)  //�ж�a % 4 == 0������1��0����Ӧ���366��365
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

	/* Number of years in days */ /*�����ǰ��ݣ���ʼ�ļ������Ϊ1970��*/
	for (i = STARTOFTIME; day >= days_in_year(i); i++) {
		day -= days_in_year(i);
	}
	tm->Year = i;

	/* Number of months in days left */ /*���㵱ǰ���·�*/
	if (leapyear(tm->Year)) {
		days_in_month(FEBRUARY) = 29;
	}
	for (i = 1; day >= days_in_month(i); i++) {
		day -= days_in_month(i);
	}
	days_in_month(FEBRUARY) = 28;
	tm->Month = i;

	/* Days are what is left over (+1) from all that. *//*���㵱ǰ����*/
	tm->Day = day + 1;

	/*
	 * Determine the day of week
	 */
// 	GregorianDay(tm);
	
}

//�Ƚ������ַ���ָ�����ȵ������Ƿ����
//����:s1,s2Ҫ�Ƚϵ������ַ���;len,�Ƚϳ���
//����ֵ:1,���;0,�����
static unsigned char str_cmpx(u8*s1,u8*s2,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
	return 1;	   
}

/* **********************************************************************************

 * ��������Now_Time_Transition_Seconed_Counter
 * ����  ������ǰ�����ϵͳʱ��ת����RTC��������
 *         RTC �����Ĵ����С�
 * ����  ����
 * ���  ����
 * ����  ����ʼ��RTCʱ��

 * **********************************************************************************/
void Now_Time_Transition_Seconed_Counter(void)
{
	Times 			XX_time;	//����ʱ��ṹ��
	unsigned char 	XX_temp[3];	//��ʱ����·��ַ�������
	unsigned char 	XX_i;
	
	/* ��ȡ�·��ַ��� */
	for(XX_i=0;XX_i<3;XX_i++)XX_temp[XX_i]=COMPILED_DATE[XX_i];
	
	/* �Ƚ��ַ����е�������12���·ݵ��е��Ǹ���ͬ������õ�ǰ���·ݡ� */
	for(XX_i=0;XX_i<12;XX_i++)
	{
		if(str_cmpx((u8*)Month_Tab[XX_i],XX_temp,3))
		{
			break;
		}
	}
	
	/* ����·� */
	XX_time.Month = XX_i+1;
	
	/* ���ʱ�� */
	if(COMPILED_DATE[4]==' ')
	{
		XX_time.Day = COMPILED_DATE[5]-'0'; 	
	}else
	{
		XX_time.Day = 10*(COMPILED_DATE[4]-'0') + COMPILED_DATE[5]-'0';  
	}
	
	/* ������ */
	XX_time.Year = 1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')\
				   +10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';
	
	/* ���Сʱ */
	XX_time.Hour = 10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';
	/* ��÷��� */
	XX_time.Min  = 10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';
	/* ������� */
	XX_time.Sec  = 10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';
		
	//������ת����������
	Seconds_Counter = mktimev(&XX_time);
}

/*************************************************************************************************/
/* ������  ��Compression_Times_Data(ʱ������)												 	 */
/* ����    ��(void *p_arg) 																		 */
/* ���	   : �� 																				 */
/* ����	   ������ǰ��ʱ������ת����ѹ����ʽ��ʱ�����ݡ�		 									 */
/* �������ڣ�2014/10/09																			 */
/*************************************************************************************************/
void Compression_Times_Data(Times* Tm,DATETIME_Storage_Format* Compression_Data)
{
   /* ��������� */
   Compression_Data->DATETIME_Bit.SECOND_BITS = Tm->Sec;
   /* ��÷����� */
   Compression_Data->DATETIME_Bit.MINUTE_BITS = Tm->Min;
   /* ���ʱ���� */
   Compression_Data->DATETIME_Bit.HOUR_BITS   = Tm->Hour;

   /* ��������� */
   Compression_Data->DATETIME_Bit.DAY_BITS    = Tm->Day;
   /* ��������� */
   Compression_Data->DATETIME_Bit.MONTH_BITS  = Tm->Month;
   /* ��������� */
   Compression_Data->DATETIME_Bit.YEAR_BITS   = Tm->Year - STARTOFTIME;

}





/******************** (C) COPYRIGHT 2009 www.armjishu.com ***********************/

