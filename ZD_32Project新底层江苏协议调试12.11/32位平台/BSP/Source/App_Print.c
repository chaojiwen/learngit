/************************************************************************************************/
/** @file              : App_Print.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��9��14��															*/
/** @brief             : c file																	*/
/** @description       : Ӧ�ô�ӡ�����ļ�														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "System_Config.h"


#if (Print_Function_Enable == 0x01)

#include "App_Print.h"

#include "Dev_Fram.h"

#include "Log_Time.h"

#include "Mcu_Config.h"



/************************************************************************************************/


/* ��ӡӦ�� ��ӡ��������ָ�� */
static INSTRU_PRINT_FIX *App_Print_Data_P = &Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print;

/************************************************************************************************/
/* ������	: App_Print_Date_Init													 	 		*/
/* ����  	: ��																				*/
/* ���		: ��. 																				*/
/* ����	 	: ��ʼ����ӡ����ʱ��																	*/
/* ��������	: 2015/11/26																		*/
/************************************************************************************************/
void App_Print_Date_Init(void)
{

/* һ��ʱ���������. */
#define Print_Updata_Interval	(60ul*60ul*24ul)
	
	/* ϵͳʱ�� */
	u32 system_second_b;
	Times system_time;
	/* ��ӡ����ʱ�� */
	u32 updata_time;

	
	/* ��ȡ RTC8025 ��ʱ������ */
	Read_System_Date_Time_Data((Times*)&system_time);

	/* ת����ǰ��ϵͳʱ�䣬���ϵͳ����������. */
	system_second_b = RTC_Date_Time_To_Second(&system_time);
	
	/* �жϴ�ӡ����ʱ���Ƿ���һ��ķ�Χ�ڡ� */
	if((App_Print_Data_P->Inf_State.Print_Start_Time > system_second_b)||(App_Print_Data_P->Inf_State.Print_End_Time < system_second_b))
	{
		/* ���ø���ʱ�� */
		updata_time = system_second_b%Print_Updata_Interval;
		
		/* ��ǰ�����Զ����´�ӡ"��ʼʱ��"��"����ʱ��"��ʱ��. */
		App_Print_Data_P->Inf_State.Print_Start_Time = system_second_b + Print_Time_Updata - updata_time;
		App_Print_Data_P->Inf_State.Print_End_Time   = App_Print_Data_P->Inf_State.Print_Start_Time + Print_Updata_Interval;
		
		/* �޸� ���� ��Ϣ�� */
		Write_Config_Tab((u8*)&App_Print_Data_P->Inf_State.Print_Start_Time,
						sizeof(App_Print_Data_P->Inf_State.Print_Start_Time));
		
		Write_Config_Tab((u8*)&App_Print_Data_P->Inf_State.Print_End_Time,
						sizeof(App_Print_Data_P->Inf_State.Print_End_Time));	
	}

#undef Print_Updata_Interval

}

/************************************************************************************************/
/* ������	: App_Print_Updata_Date													 	 		*/
/* ����  	: cur_time:��ǰʱ��(���ʱ�䵥λ:��).													*/
/* ���		: ��. 																				*/
/* ����	 	: �Զ����´�ӡ"��ʼʱ��"��"����ʱ��"(һ�����һ��).										*/
/* ��������	: 2015/09/15																		*/
/************************************************************************************************/
void App_Print_Updata_Date(Times system_time)
{

/* һ��ʱ���������. */
#define Print_Updata_Interval	(60ul*60ul*24ul)

	/* ϵͳʱ�� */
	u32 system_second_b;
	/* ��ӡ����ʱ�� */
	u32 updata_time;



	/* ת����ǰ��ϵͳʱ�䣬���ϵͳ����������. */
	system_second_b = RTC_Date_Time_To_Second(&system_time);
	/* ���ø���ʱ�� */
	updata_time = system_second_b%Print_Updata_Interval;
	
	/* �жϸ��µ���ʱ��. */
	if((updata_time == Print_Time_Updata)||
	  ((App_Print_Data_P->Inf_State.Print_Start_Time == 0)&&
	   (App_Print_Data_P->Inf_State.Print_End_Time   == 0)))
	{
		/* ��ǰ�����Զ����´�ӡ"��ʼʱ��"��"����ʱ��"��ʱ��. */
		App_Print_Data_P->Inf_State.Print_Start_Time = system_second_b + Print_Time_Updata - updata_time;
		App_Print_Data_P->Inf_State.Print_End_Time   = App_Print_Data_P->Inf_State.Print_Start_Time + Print_Updata_Interval;
		/* �޸� ���� ��Ϣ�� */
		Write_Config_Tab((u8*)&App_Print_Data_P->Inf_State.Print_Start_Time,
						sizeof(App_Print_Data_P->Inf_State.Print_Start_Time));
		Write_Config_Tab((u8*)&App_Print_Data_P->Inf_State.Print_End_Time,
						sizeof(App_Print_Data_P->Inf_State.Print_End_Time));


	}
	


#undef Print_Updata_Interval
}

#endif

/******************* (C) HZ Zeda Team *****END OF FILE****/

















