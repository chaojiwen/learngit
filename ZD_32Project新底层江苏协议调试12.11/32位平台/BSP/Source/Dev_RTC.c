/************************************************************************************************/
/** @file              : Dev_RTC.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��6��25��															*/
/** @brief             : c file																	*/
/** @description       : RTC �����ļ�														*/
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
/* ˵����																			  */
/* 1��RTC����ܱ���100������������ݡ�����2000��1��1�յ�2099��12��31�գ�					  */
/* 2��																				  */
/* 3��																				  */
/* ********************************************************************************** */



/* ******************************* �·ݶ�Ӧ�������� ************************************ */
u16 month_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
/* ***************************** �������빫��ʱ�任�� ********************************** */
/* ��׼ʱ����� */
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
#define	days_in_year(a) 	(leapyear(a) ? 366 : 365)  //�ж�a % 4 == 0������1��0����Ӧ���366��365
#define	days_in_month(a) 	(month_days[(a) - 1])

/* ***************************** RTC �ڲ��Ĵ��� ********************************** */
#define RTC_Sec_Reg_Adder			((int)0X00000000)		//��Ĵ���
#define RTC_Min_Reg_Adder			((int)0X00000001)		//�ּĴ���
#define RTC_Hour_Reg_Adder			((int)0X00000002)		//ʱ�Ĵ���
#define RTC_Week_Reg_Adder			((int)0X00000003)		//���ڼĴ���
#define RTC_Day_Reg_Adder			((int)0X00000004)		//���ڼĴ���
#define RTC_Mon_Reg_Adder			((int)0X00000005)		//�·ݼĴ���
#define RTC_Year_Reg_Adder			((int)0X00000006)		//��ݼĴ���
	
#define RTC_Digit_Offset_Reg_Adder	((int)0X00000007)		//ʱ�ӵ����Ĵ���
	
#define RTC_Alarm_W_Min_Reg_Adder	((int)0X00000008)		//����W �� �Ĵ���
#define RTC_Alarm_W_Hour_Reg_Adder	((int)0X00000009)		//����W ʱ �Ĵ���
#define RTC_Alarm_W_Week_Reg_Adder	((int)0X0000000A)		//����W ���� �Ĵ���
	
#define RTC_Alarm_D_Min_Reg_Adder	((int)0X0000000B)		//����D �� �Ĵ���
#define RTC_Alarm_D_Hour_Reg_Adder	((int)0X0000000C)		//����D ʱ �Ĵ���
	
#define RTC_Reserved				((int)0X0000000D)		//����

#define RTC_Control_1_Reg_Adder		((int)0X0000000E)		//���ƼĴ��� 1 �Ĵ���
#define RTC_Control_2_Reg_Adder		((int)0X0000000F)		//���ƼĴ��� 2 �Ĵ���
	
/* ���� RTC ���ݽṹ */
typedef RTC8025_Typedef RTC_TYPEDEF;
/* ���� RTC ���ƽṹ */
typedef RTC8025_Ctrl_Typedef RTC_CTRL_TYPEDEF;

/* RTC �ļ��ڲ��ı���. */
static RTC_TYPEDEF	RTC_Time;	//zwc	
static RTC_TYPEDEF	Last_RTC_Time;//��һʱ��RTC��ֵ
static Times  RTC_Offline_Cok_DR;
extern void Write_Config_Tab(u8* buffer_p,u16 count);//д�������ñ���Ϣ



static RTC_CTRL_TYPEDEF RTC_Ctrl;	

/* ϵͳʱ��У׼״̬�Ĵ���(FALSE:δУ׼;TRUE:��У׼) */
////////static BOOL RTC_System_Clock_Calibration_State_Reg = FALSE;

BOOL RTC_System_Clock_Calibration_State_Reg = FALSE;//2018.12.4  By  ZWC  
/************************************************************************************************/
/* ������	: RTC_Write																		 	*/
/* ����  	: *pdata:����ָ��;addr:������ַ;	count:��������;										*/
/* ���		: ִ�н����																			*/
/* ����	 	: дRTC������								 									 	*/
/* �������� 	: 2015/8/27																			*/
/************************************************************************************************/
static BOOL RTC_Write(char *pdata, int addr, int count)
{
	/* �������. */
	BOOL res = FALSE;
	
	/* ������������ */   
	OSSchedLock();
	res = (BOOL)RTC8025_Write(pdata, addr, count);
	/* ������������ */
	OSSchedUnlock();
	
	return res;
}

/************************************************************************************************/
/* ������	: RTC_Read																		 	*/
/* ����  	: *pdata:����ָ��;addr:������ַ;	count:��������;										*/
/* ���		: ִ�н����																			*/
/* ����	 	: ��RTC������								 									 	*/
/* �������� 	: 2015/8/27																			*/
/************************************************************************************************/
BOOL RTC_Read(char *pdata, int addr, int count)
{

	/* �������. */
	BOOL res = FALSE;
	
	/* ������������ */   
	OSSchedLock();
	res = (BOOL)RTC8025_Read(pdata, addr, count);
	/* ������������ */
	OSSchedUnlock();
	
	return res;
}

/************************************************************************************************/
/* ������	: RTC_Reset																		 	*/
/* ����  	: need_reset:��λ����;																*/
/* ���		: ִ�н����																			*/
/* ����	 	: ����need_reset�ͼ��RTC״̬�Ĵ��� ��λRTC��	 									 	*/
/* �������� 	: 2015/8/27																			*/
/************************************************************************************************/
BOOL RTC_Reset(BOOL need_reset)
{

	/* �������. */
	BOOL res = FALSE;
	
	/* ������������ */
	OSSchedLock();
	res = (BOOL)RTC8025_Reset(need_reset);
	/* ������������ */
	OSSchedUnlock();
	
	return res;
}

/* **********************************************************************************
 * ��������BCD_To_HEX
 * ����  ���� BCD�� ת�� �� HEX��
 * ����  ��bcd_data��BCD������
 * ���  ��HEX������
 * **********************************************************************************/
u8 BCD_To_HEX(u8 bcd_data)   
{   
    u8 temp;   
    temp=(bcd_data/16*10 + bcd_data%16);   
    return temp;   
}   

/* **********************************************************************************
 * ��������HEX_To_BCD
 * ����  ���� HEX�� ת�� �� BCD��
 * ����  ��hex_data: HEX������
 * ���  ��BCD������
 * **********************************************************************************/
u8 HEX_To_BCD(u8 hex_data)    //HEXתΪBCD�ӳ���      
{   
    u8 temp;   
    temp=(hex_data/10*16 + hex_data%10);   
    return temp;   
}
/**********ȡ��һ���ֽ����ݵĸ�4λ****************/
u8 HEX_Hi4(u8 Data)
{
	u8 DataHi4=0;
  DataHi4 = (Data & 0xf0) >> 4;
   return DataHi4;
}
/**********ȡ��һ���ֽ����ݵĵ�4λ****************/
u8 HEX_Lo4(u8 Data)
{
	u8 DataLo4=0;
  DataLo4 = Data & 0x0f;
	return  DataLo4;
}






/* **********************************************************************************
 * ��������RTC_Init
 * ����  ������ ��Ҫ �� RTC8025 ���и�λ����
 * ����  ��reset_state: �ֶ�/�Զ���λ״̬��־
 * ���  ����
 * **********************************************************************************/
BOOL RTC_Init(BOOL reset_state)
{   
	BOOL state;
	
	state = RTC_Reset(reset_state);
	
	/* ���ݶ� RTC ��λ��� */
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
 * ��������RTC_FOE_GPIO_Config
 * ����  ������ RTC 32.768Khzʱ�� ���
 * ����  ����
 * ���  ����
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
	/* ���� RTC ��FOE����˵���� �͵�ƽ�򸡿�״̬ʱ�������32.768Khz����. */	
	GPIO_ResetBits(GPIOC,GPIO_Pin_10);// turn off all led  
}

/* **********************************************************************************
 * ��������RTC_WakeUp_PA0_Config
 * ����  ������ RTC ���Ź����ܣ�PA0 ��Ϊ �ⲿ�ж����ţ����1s����һ�Σ�
 * ����  ����
 * ���  ����
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
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //���ش����ж�
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); 
}

/* **********************************************************************************
 * ��������RTC_Mcu_Out_Config
 * ����  ��Mcu ���յ�RTC�Ĵ����źţ�����źŴ������һ�������źţ�
		  ���ⲿ�Ŀ��Ź�оƬMAX823���� ι��������
 * ����  ����
 * ���  ����
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
 * ��������Read_System_Date_Time_Data
 * ����  ���� RTC �ж�ȡ ��ǰʱ�� ����
 * ����  ��time��������ݵ� ָ��
 * ���  ����
 * **********************************************************************************/
void Read_System_Date_Time_Data(Times* time)
{
	/* ��ȡ RTC ��ʱ������ */
	RTC_Read(RTC_Time.REGS,0x00,7);
	/* ���� �������� BCD �룬��Ҫת����HEX��. */
	time->Year 	= (BCD_To_HEX(RTC_Time.STREG.Years) + STARTYEAR);
	time->Month = BCD_To_HEX(RTC_Time.STREG.Months);
	time->Day	= BCD_To_HEX(RTC_Time.STREG.Days);
	time->Hour	= BCD_To_HEX(RTC_Time.STREG.Hours);
	time->Min	= BCD_To_HEX(RTC_Time.STREG.Minutes);
	time->Sec 	= BCD_To_HEX(RTC_Time.STREG.Seconds);
	time->Week 	= BCD_To_HEX(RTC_Time.STREG.Weekdays);

}

/* **********************************************************************************
 * ��������RTC_Alarm_D_Config
 * ����  ������ ���� D �Ĺ̶������жϹ���
 * ����  ��reg��������ݵ� ָ��
 * ���  ����
 * **********************************************************************************/
void RTC_Alarm_D_Config(u8 reg)
{
	/* д�� RTC ��ʱ������ */
	RTC_Write((char*)&reg,RTC_Control_1_Reg_Adder,1);
}

/* **********************************************************************************
 * ��������Write_System_Date_Time_Data
 * ����  ���� RTC ��д�� ��ǰʱ�� ����
 * ����  ��time��������ݵ� ָ��
 * ���  ����
 * **********************************************************************************/
void Write_System_Date_Time_Data(Times* time)
{
	/* ���� �������� BCD �룬��Ҫת����HEX��. */
	if(time->Year >= ENDYEAR)
	{
		/* RTC �����������ȡ��λ�Ժ������. */
		RTC_Time.STREG.Years	  = HEX_To_BCD(time->Year%100);
		/* RTC ��������������������־. */
		RTC_Time.STREG.YearCarry  = 1;
	}else
	{
		/* ������δ����� */
		RTC_Time.STREG.Years	  = HEX_To_BCD(time->Year - STARTYEAR);		
	}
	/* �·� */
	RTC_Time.STREG.Months   = HEX_To_BCD(time->Month);
	/* ���� */
	RTC_Time.STREG.Days	  	= HEX_To_BCD(time->Day);
	/* ʱ */
	RTC_Time.STREG.Hours	= HEX_To_BCD(time->Hour);
	/* �� */
	RTC_Time.STREG.Minutes  = HEX_To_BCD(time->Min);
	/* �� */
	RTC_Time.STREG.Seconds  = HEX_To_BCD(time->Sec);
	/* ���� */
	RTC_Time.STREG.Weekdays = BCD_To_HEX(time->Week);
	
	/* д�� RTC ��ʱ������ */
	RTC_Write(RTC_Time.REGS,0x00,7);

}
/*****************************************************/
void Write_System_Set(Times* time)
{
	/* ���� �������� BCD �룬��Ҫת����HEX��. */
	if(time->Year >= ENDYEAR)
	{
		/* RTC �����������ȡ��λ�Ժ������. */
		RTC_Time.STREG.Years	  = HEX_To_BCD(time->Year%100);
		/* RTC ��������������������־. */
		RTC_Time.STREG.YearCarry  = 1;
	}else
	{
		/* ������δ����� */
		RTC_Time.STREG.Years	  = (time->Year - STARTYEAR);		
	}
	/* �·� */
	RTC_Time.STREG.Months   =(time->Month);
	/* ���� */
	RTC_Time.STREG.Days	  	= (time->Day);
	/* ʱ */
	RTC_Time.STREG.Hours	= (time->Hour);
	/* �� */
	RTC_Time.STREG.Minutes  =(time->Min);
	/* �� */
	RTC_Time.STREG.Seconds  = (time->Sec);
	/* ���� */
	RTC_Time.STREG.Weekdays = (time->Week);
	
	/* д�� RTC ��ʱ������ */
	RTC_Write(RTC_Time.REGS,0x00,7);

}

/*************************************************************************************************/
/* ������  ��Compression_Times_Data(ʱ������)												 		 */
/* ����    ��(void *p_arg) 																		 */
/* ���	   : �� 																				 */
/* ����	   ������ǰ��ʱ������ת����ѹ����ʽ��ʱ�����ݡ�		 										 */
/* �������ڣ�2014/10/09																			 */
/*************************************************************************************************/
void Compression_Times_Data(Times* Tm,INSTRU_CONFIG_INF_TAB* tab)
{
	/* �洢���ݵ�ʱ��� */
	DATETIME_Storage_Format save_data_timestamp;

	/* ��������� */
	save_data_timestamp.DATETIME_Bit.SECOND_BITS = Tm->Sec;
	/* ��÷����� */
	save_data_timestamp.DATETIME_Bit.MINUTE_BITS = Tm->Min;
	/* ���ʱ���� */
	save_data_timestamp.DATETIME_Bit.HOUR_BITS   = Tm->Hour;
	/* ��������� */
	save_data_timestamp.DATETIME_Bit.DAY_BITS    = Tm->Day;
	/* ��������� */
	save_data_timestamp.DATETIME_Bit.MONTH_BITS  = Tm->Month;
	/* ��������� *///������꣬��󱣴�63�꣬��2010�꿪ʼ���𣬳��������´�2010��ʼ��
	save_data_timestamp.DATETIME_Bit.YEAR_BITS   = Tm->Year - STARTOFTIME;	
	
	tab->Inf_State.Instru_RTC.Inf_State.Year	= Tm->Year;
	tab->Inf_State.Instru_RTC.Inf_State.Month	= Tm->Month;
	tab->Inf_State.Instru_RTC.Inf_State.Day		= Tm->Day;
	tab->Inf_State.Instru_RTC.Inf_State.Hour	= Tm->Hour;
	tab->Inf_State.Instru_RTC.Inf_State.Min		= Tm->Min;
	tab->Inf_State.Instru_RTC.Inf_State.Sec		= Tm->Sec;
	
	/* ��ȡ �洢 ��ʱ��� */
	Save_Get_Timestamp(&save_data_timestamp);
	
}

/* **********************************************************************************
 * ��������RTC_WakeUp_Func
 * ����  ��RTC �жϹ���
 * ����  ����
 * ���  ����
 * **********************************************************************************/
void RTC_WakeUp_Func(void)
{


}

/* **********************************************************************************
 * ��������RTC_Read_Control_Reg
 * ����  ����ȡ�������Ĵ�������
 * ����  ����
 * ���  ����
 * **********************************************************************************/
void RTC_Read_Control_Reg(void)
{
	
	RTC_Read((char*)&RTC_Ctrl.Control, 0x0E, 2);
	
}

/* **********************************************************************************
 * ��������Date_Time_To_Second
 * ����  ��������ʱ������ת����������
 * ����  ��ʱ���������ݽṹ
 * ���  ��������
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
 * ��������Second_To_Date_Time
 * ����  ����������ת���ɹ�����ʱ������
 * ����  ����ǰ�������ݡ�ʱ�����ݽṹ��
 * ���  ����
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
	
}

/************************************************************************************************/
/* ������	: RTC_System_Clock_Calibration_State_Check										 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���ϵͳ������ʱ���Ƿ�У׼����				 									 	*/
/* �������� 	: 2015/6/26																			*/
/************************************************************************************************/
void RTC_System_Clock_Calibration_State_Check(void)
{

	/* �ж�ϵͳ������ʱ���Ƿ�У׼���� */
	if(RTC_System_Clock_Calibration_State_Reg == FALSE)
	{
		/* ϵͳ������ʱ��δУ׼ */
		/* ����ʱ��У׼��Ϣ������������� */
		RTC_System_Clock_Calibration_State_Reg = GSM_Send_Q(GSM_EVENT_Str_Q,System_Clock_Calibration_EVENT);
	}

}

/************************************************************************************************/
/* ������	: RTC_System_Clock_Calibration_State_Reset										 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��λϵͳʱ��У׼״̬�Ĵ���				 									 	*/
/* �������� 	: 2015/6/26																			*/
/************************************************************************************************/
void RTC_System_Clock_Calibration_State_Reset(void)
{

	/* ��λϵͳʱ��У׼״̬�Ĵ��� */
	RTC_System_Clock_Calibration_State_Reg = FALSE;
	
}





/* **********************************************************************************
 * �������� Check_RTC_Time
 * ����  ���ж�BCD���Ƿ�Ϸ� �꣺17~35;�£�01~12���գ�01~31��ʱ��00~23;�֣�00~59;�룺00~59
 * ����  ����
 * ���  ��BCD��Ϸ�����true�����򷵻�false
 * **********************************************************************************/
static bool Check_RTC_Time(void)
{
	bool ret=false;
	
		/* ��ȡ RTC ��ʱ������ */
	RTC_Read(RTC_Time.REGS,0x00,7);//�ȶ�ȡRTC�е�����
	
	
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

/*************RTC����ʱ����ز���**************************************/

void RTC_Equipment_Offline(void)
{

    if(Check_RTC_Time())//���RTC��ֵδ����
		{
		        Last_RTC_Time=RTC_Time;//��¼��һʱ��RTCδ����ʱ��ֵ
		}
		else
		{
	     	    RTC_Time=Last_RTC_Time;
			/* ���� �������� BCD �룬��Ҫת����HEX��. */
	RTC_Offline_Cok_DR.Year 	= (BCD_To_HEX(RTC_Time.STREG.Years) + STARTYEAR);
	RTC_Offline_Cok_DR.Month = BCD_To_HEX(RTC_Time.STREG.Months);
	RTC_Offline_Cok_DR.Day	= BCD_To_HEX(RTC_Time.STREG.Days);
	RTC_Offline_Cok_DR.Hour	= BCD_To_HEX(RTC_Time.STREG.Hours);
	RTC_Offline_Cok_DR.Min	= BCD_To_HEX(RTC_Time.STREG.Minutes);
	RTC_Offline_Cok_DR.Sec 	= BCD_To_HEX(RTC_Time.STREG.Seconds);
	RTC_Offline_Cok_DR.Week 	= BCD_To_HEX(RTC_Time.STREG.Weekdays);
			
			
		
	/* ���� �������� BCD �룬��Ҫת����HEX��. */
	if(RTC_Offline_Cok_DR.Year >= ENDYEAR)
	{
		/* RTC �����������ȡ��λ�Ժ������. */
		RTC_Time.STREG.Years	  = HEX_To_BCD(RTC_Offline_Cok_DR.Year%100);
		/* RTC ��������������������־. */
		RTC_Time.STREG.YearCarry  = 1;
	}else
	{
		/* ������δ����� */
		RTC_Time.STREG.Years	  = HEX_To_BCD(RTC_Offline_Cok_DR.Year - STARTYEAR);		
	}
	/* �·� */
	RTC_Time.STREG.Months   = HEX_To_BCD(RTC_Offline_Cok_DR.Month);
	/* ���� */
	RTC_Time.STREG.Days	  	= HEX_To_BCD(RTC_Offline_Cok_DR.Day);
	/* ʱ */
	RTC_Time.STREG.Hours	= HEX_To_BCD(RTC_Offline_Cok_DR.Hour);
	/* �� */
	RTC_Time.STREG.Minutes  = HEX_To_BCD(RTC_Offline_Cok_DR.Min);
	/* �� */
	RTC_Time.STREG.Seconds  = HEX_To_BCD(RTC_Offline_Cok_DR.Sec);
	/* ���� */
	RTC_Time.STREG.Weekdays = BCD_To_HEX(RTC_Offline_Cok_DR.Week);
	
	
	
	
	
	
	
	
	/* д�� RTC ��ʱ������ */
	RTC_Write(RTC_Time.REGS,0x00,7);		
			
			
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year 	= RTC_Offline_Cok_DR.Year;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month 	= RTC_Offline_Cok_DR.Month;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day 	= RTC_Offline_Cok_DR.Day;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour 	= RTC_Offline_Cok_DR.Hour;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min 	= RTC_Offline_Cok_DR.Min;
		Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec 	= RTC_Offline_Cok_DR.Sec;
		
		//Ϊ�˽��RTCʱ��оƬ���ⱻд���ϣ������ȱ���һ�ε�ǰ��ʵʱʱ������Ϊ\
		  ���ϵ�������⵽RTC���쳣�����ʹ�����ʱ��������ʵʱʱ�䡣
		/* �޸� ���� ��Ϣ�� ���ñ�־ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_RTC,
					    sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC));
	
	
	
			
			
		/* ���ø���RTCʱ�� ���������ʱ*/
		RTC_System_Clock_Calibration_State_Reg = FALSE;
		
		
		}





}






/************************************************************************************************/
/* ������	: RTC_System_Clock_Calibration													 	*/
/* ����  	: (void *p_arg) 																	*/
/* ���		: �� 																				*/
/* ����	 	: У׼ϵͳʱ�ӡ�								 									 	*/
/* �������� 	: 2015/6/26																			*/
/************************************************************************************************/
void RTC_System_Clock_Calibration(Times system_time)
{
	/* ϵͳʱ�� */
	u32 system_second_b;
	/* ��ӡ����ʱ�� */
	u32 updata_time;


	/* ת����ǰ��ϵͳʱ�䣬���ϵͳ����������. */
	system_second_b = RTC_Date_Time_To_Second(&system_time);
	/* ���ø���ʱ�� */
	updata_time = system_second_b%SECDAY;

	/* �жϸ��µ���ʱ��. */
	if((updata_time == System_Clock_Time_Updata)||(Check_RTC_Time()==false))//�賿1���RTC������ֵ���Ϸ����������ʱ
	{
		/* ���ø���RTCʱ�� */
		RTC_System_Clock_Calibration_State_Reg = FALSE;
	}
	

 RTC_Equipment_Offline();//��RTC����ʱ�Ĳ���zwc
	
	
	
	
}



/******************* (C) ZHOU *****END OF FILE****/

