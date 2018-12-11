/************************************************************************************************/
/** @file              : Log_Print.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��6��28��															*/
/** @brief             : c file																	*/
/** @description       : ��ӡ�߼�																*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_Print.h"

#include "Mcu_Config.h"
#include "Mcu_Usart.h"

#include "Dev_RTC.h"
#include "Dev_Flash.h"
#include "Dev_Fram.h"

#include "Log_RF_Card.h"
#include "Log_Save.h"

#include "Menu.h"
#include <stdbool.h>
#include <ucos_ii.h>

#include <string.h>



/*============================ MACRO =========================================*/
//	<<< Use Configuration Wizard in Context Menu >>>
//	<h>��ӡ������صĺ�����
//	<e>��ӡ������Ŀѡ��
//  <o.0..2>���ô�ӡ����.
//  <0=> [Ĭ��]����.
//  <1=> [������]����.
//	<i>��ӡ����ѡ��.
#define Print_Scheme		(0x00)
//  </e>

// </h>
//	<<< end of configuration section >>>

//#if (Print_Scheme == 1)
//	#if (Print_Sensor_CH_Num != 3)
//		#error	"��ӡͨ����������3�����ڽ����Ŵ�ӡ�����������"
//	#endif
//#endif

/************************************************************************************************/
/* ��ӡ ����ͷ */
#define Print_CMD_Head_0X1B				(0x1B)
#define Print_CMD_Head_0X1C				(0x1C)
#define Print_CMD_Head_0X1D				(0x1D)
#define Print_CMD_Head_0X1F				(0x1F)
//��ӡ����
/* ��ӡ �������������С */
#define Print_CMD_Set_Font_Size			(0x38)
/* ��ӡ ���ûس� */
#define Print_CMD_Set_CRLF				(0x0A)
/* ��ӡ �����м�� */
#define Print_CMD_Set_Line_Space		(0x31)
/* ��ӡ �����ּ�� */ 
#define Print_CMD_Set_Word_Space		(0x20)
/* ��ӡ ���ÿո����� */
#define Print_CMD_Set_Blank_Line_Null	(0x66)
/* ��ӡ �������� */
#define Print_CMD_Set_Right_Limit		(0x51)
/* ��ӡ �������� */
#define Print_CMD_Set_Left_Limit		(0x6C)
/* ��ӡ ����/��ֹ�»��� */
#define Print_CMD_Set_Underline_Config	(0x2D)
/* ��ӡ ������ת�ַ� */
#define Print_CMD_Set_Rotate_Char		(0x49)
/* ��ӡ ��ʼ����ӡ�� */
#define Print_CMD_Init					(0x40)
/* ��ӡ 	����/��ֹ������ʾ */
#define Print_CMD_Set_Highlight_Char	(0x69)
/* ��ӡ 	ͼ������ */
#define Print_CMD_Set_Picture			(0x10)
/* ��ӡ 	ͼ������ */
#define Print_CMD_Set_BarCode			(0x6B)
/* ��ӡ �������巭��/��ԭ */
#define Print_CMD_Set_Font_Double		(0x57)


/************************************************************************************************/
/* ��ӡ�������� */
#define Print_Font_12_12				(0x01)
#define Print_Font_16_16				(0x02)
/* ��ӡ������ת�Ƕ� */
#define Print_Font_Rotate_Angle_0		(0x00)
#define Print_Font_Rotate_Angle_90		(0x01)
#define Print_Font_Rotate_Angle_180		(0x02)
#define Print_Font_Rotate_Angle_270		(0x03)


/************************************************************************************************/
/* ��ӡͼƬ���� */
/* ��ӡͼƬ�����ݼĴ�����С�� */
#define Print_Picture_Data_Size			(288u)


/************************************************************************************************/
/* ���Թ�˾���� */
#define Company_Name_Text_Str	"��������������޹�˾"



/************************************************************************************************/
/* ��˾����:%s */
#define Company_Name_Str		"%s\r\n"
/* �豸����:%04X%05d; */
#define Device_Number_Str		"�豸����:%04X%05d\r\n"
/* ��ӡʱ���:\r\n */
#define Print_Time_Period_Str	"��ӡʱ���:\r\n"
/* ��ʼʱ��:%04d��%02d��%02d��,%02dʱ%02d��%02d��\r\n */
#define Print_Start_Time_Str	"��ʼʱ��:%04d��%02d��%02d��,%02dʱ%02d��\r\n"
/* ����ʱ��:%04d��%02d��%02d��,%02dʱ%02d��%02d��\r\n */
#define Print_End_Time_Str		"����ʱ��:%04d��%02d��%02d��,%02dʱ%02d��\r\n"
/* ������ͨ��%d�¶ȷ�Χ(%02d.%01d��-%02d.%01d��)\r\n */
#define Print_sensor_Temp_Range_Str	"ͨ��%d�¶ȷ�Χ(%c%d.%01d�浽%c%d.%01d��)\r\n"
/* ������ͨ��%dʪ�ȷ�Χ(%02d.%01d%%RH-%02d.%01d%%RH)\r\n */
#define Print_Sensor_Humi_Range_Str	"ͨ��%dʪ�ȷ�Χ(%d.%01d%%RH��%d.%01d%%RH)\r\n"
/* ʱ����Ŀ�� */
#define Print_Time_Bar_Str			"- ʱ�� -"
/* �¶ȴ�������Ŀ�� */
#define Print_Temp_Bar_Str			" �¶�%d"
/* ʪ�ȴ�������Ŀ�� */
#define Print_Humi_Bar_Str			" ʪ��%d"


/* ���ݼ�¼����: */
#define Print_Record_Data_Str		"���ݼ�¼����:\r\n"
/* ����:�ͻ���_____________ */
#define Print_Delivery_Party_Str	"�� �� ��:%s\r\n"
/* ����:�ջ���_____________ */
#define Print_Receiver_Party_Str	"�� �� ��:\r\n"
/* ����:���ƺ�_____________ */
#define Print_Card_NO_Str			"�� �� ��:%s\r\n"

/* �����Ŵ�ӡ���� */
#if (Print_Scheme == 1)

/* �����Ŷ��Ʋ�Ʒ�������¶ȡ� */
#define Print_EnvironmentTemp_Bar_Str	"�����¶�"

/* �����Ŷ��Ʋ�Ʒ�����ݱ��:�� */
#define Print_FBillNo_Str				"���ݱ��:                    "
/* �����Ŷ��Ʋ�Ʒ���˵���:�� */
#define Print_TransportNo_Str			"�˵���:%04X%05d-%X-%X\r\n"


/* �����Ŷ��Ʋ�Ʒ������ʱ��:�� */
#define Print_StartOffTime_Str			"����ʱ��:%04d��%02d��%02d��,%02dʱ%02d��\r\n"

/* �����Ŷ��Ʋ�Ʒ������ʱ��:�� */
#define Print_ArriveTime_Str			"����ʱ��:%04d��%02d��%02d��,%02dʱ%02d��\r\n"




/* �����Ŵ�ӡ�����������¶ȡ� */
#define Print_StartOffTemp_Str			"�����¶�:"

/* �����Ŵ�ӡ�����������¶ȡ� */
#define Print_ArriveTemp_Str			"�����¶�:"

/* �����Ŷ��Ʋ�Ʒ�������¶ȡ� */
#define Print_StartOffEnvironmentTemp_Bar_Str	"���⻷���¶�:"

/* �����Ŷ��Ʋ�Ʒ�������¶ȡ� */
#define Print_ArriveEnvironmentTemp_Bar_Str		"���ﻷ���¶�:"

#endif

/************************************************************************************************/
/* ��ӡ������������ʽ */
/* ��ӡ���ڸ�ʽ */
#define Print_Date_Str				"<<<<<<����: %04d��%02d��%02d��>>>>>>\r\n"
/* ʱ��θ�ʽ */
#define Print_Data_Body_Time_Str	"%02d:%02d:%02d"
/* ���ݶθ�ʽ */
#define Print_Data_Body_Data_Str	" %c%d.%01d"
/* ��Ƭ ������ ��ʽ */
#define Print_Data_Body_Rece_Timeout_Data_Str	" ȱʧ "
/* �������������ֵ */
#define Print_Sensor_Data_Max_Value_Str			"���ֵ:"
/* ������������Сֵ */
#define Print_Sensor_Data_Min_Value_Str			"��Сֵ:"


/************************************************************************************************/
/* ��ӡ������Ϣ */
/* ������Ϣ1:������ʼʱ����ڼ�¼�Ľ���ʱ��. */
#define Print_Search_Start_Time_Than_Record_End_Time	"������ʼʱ����ڼ�¼�Ľ���ʱ��.\r\n"
/* ������Ϣ2:��������ʱ��С�ڼ�¼����ʼʱ��. */
#define Print_Search_End_Time_Than_Record_Start_Time	"��������ʱ��С�ڼ�¼����ʼʱ��.\r\n"
/* ������Ϣ3:������ʼʱ�������������ʱ��. */
#define Print_Search_Start_Time_Than_Search_End_Time	"������ʼʱ�������������ʱ��.\r\n"
/* ������Ϣ4:������ʼʱ�������������ʱ��. */
#define Print_Set_Time_Range_Null_Data_Str				"���õ�ʱ�䷶Χ��û�м�¼����.\r\n"


/************************************************************************************************/
/* ��ӡ��ʼ�ͽ����ķָ�� */
#define Print_Indivisible_Symbol_Start_Str	"------------------------\r\n"
#define Print_Indivisible_Symbol_End_Str	"************************\r\n"
/************************************************************************************************/

/************************************************************************************************/
/* ��ӡ��ʼ�ͽ�����ж���� */
#define Print_StartUnload_Symbol_Start_Str	"  *****ж����ʼ->%d*****  \r\n"
#define Print_StoptUnload_Symbol_End_Str	  "  *****ж������->%d*****  \r\n"
/************************************************************************************************/

/* ��ӡ�������������/Сֵ */
typedef struct 
{
#if (Print_Sensor_CH_Num >= 1)
	/* ��ӡͨ��1����ֵ */
	/* ���ֵ */
	float Ch1_Max;
	/* ��Сֵ */
	float Ch1_Min;	
#endif	
	
#if (Print_Sensor_CH_Num >= 2)	
	/* ��ӡͨ��2����ֵ */
	/* ���ֵ */
	float Ch2_Max;
	/* ��Сֵ */
	float Ch2_Min;	
#endif	
	
#if (Print_Sensor_CH_Num >= 3)
	/* ��ӡͨ��3����ֵ */
	/* ���ֵ */
	float Ch3_Max;
	/* ��Сֵ */
	float Ch3_Min;	
#endif

#if (Print_Sensor_CH_Num >= 4)
	/* ��ӡͨ��4����ֵ */
	/* ���ֵ */
	float Ch4_Max;
	/* ��Сֵ */
	float Ch4_Min;	
#endif

}PRINT_SENSOR_MAX_MIN_VALUE; 
/************************************************************************************************/
#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
/* ��ά��LOGO */
static const char Print_LOGO[Print_Picture_Data_Size] = 
{
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0xF7,0xE4,0x23,0x9F,0xC0,0x04,0x16,0xD2,0x0C,0x90,0x40,
	0x05,0xD0,0xC5,0xA0,0xD7,0x40,0x05,0xD4,0xC0,0x53,0xD7,0x40,0x05,0xD7,0x26,0x2C,
	0xD7,0x40,0x04,0x16,0xBB,0xC3,0xD0,0x40,0x07,0xF5,0x55,0x55,0x5F,0xC0,0x00,0x01,
	0x3F,0x0C,0x00,0x00,0x00,0x90,0x67,0x87,0x8E,0xC0,0x01,0x03,0x0C,0xFA,0x70,0xC0,
	0x06,0xB7,0x4A,0x44,0x5C,0x40,0x06,0x2E,0x1C,0x1E,0x84,0x40,0x06,0xBF,0x62,0x56,
	0xD9,0x40,0x05,0x02,0xB6,0x86,0x9A,0xC0,0x05,0x9E,0x40,0x0F,0x3D,0x40,0x04,0x01,
	0x40,0x0F,0x4C,0x80,0x03,0xD5,0x00,0x04,0xB3,0x00,0x06,0x61,0x20,0x87,0x77,0x40,
	0x05,0x53,0x41,0x01,0x73,0xC0,0x03,0x86,0xE0,0x04,0x1D,0x40,0x06,0xD8,0xA1,0xC1,
	0xDC,0x80,0x02,0x2E,0xE0,0x04,0x31,0x40,0x06,0xB0,0x00,0x06,0x7F,0x40,0x04,0xE0,
	0xAD,0x42,0x44,0x80,0x00,0x94,0x8A,0x1C,0xBC,0x00,0x03,0x4A,0xFD,0x3D,0x7A,0x40,
	0x05,0x35,0x73,0xD5,0x67,0x40,0x03,0x2C,0x54,0x87,0x38,0x40,0x07,0x13,0xC4,0x58,
	0x7F,0x80,0x00,0x06,0x67,0x3D,0xC4,0xC0,0x07,0xF3,0x22,0x84,0x55,0xC0,0x04,0x12,
	0xC1,0x3D,0x45,0x00,0x05,0xD2,0xAE,0x8E,0x7E,0x80,0x05,0xD5,0x62,0x2A,0x6D,0xC0,
	0x05,0xD1,0x64,0x82,0x85,0x40,0x04,0x13,0x97,0x72,0xC2,0x00,0x07,0xF3,0xEA,0x96,
	0x7C,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};
#endif

/* �����¶�/�ﵽ�¶Ƚṹ */
typedef struct
{
	/* ʱ�� */
	Times	Time;
	
#if (Print_Sensor_CH_Num >= 1)
	/* ͨ��1 */
	float Ch1_Value;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* ͨ��2 */
	float Ch2_Value;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ͨ��3(����ֵ) */
	float Ch3_Value;
#endif

#if (Print_Sensor_CH_Num >= 4)
	/* ͨ��4 */
	float Ch4_Value;
#endif
}Print_JWX_OutInTemp_Struct;

/* �����¶� */
static Print_JWX_OutInTemp_Struct StartOffTemp = 
{
	/* �� */
	0,
	/* �� */
	0,
	/* �� */
	0,
	/* ʱ */
	0,
	/* �� */
	0,
	/* �� */
	0,
	/* ���� */
	0,
	/* ʱ�� */
	0,
#if (Print_Sensor_CH_Num >= 1)
	/* ͨ��1 */
	0,
#endif	
	
#if (Print_Sensor_CH_Num >= 2)	
	/* ͨ��2 */
	0,
#endif	
	
#if (Print_Sensor_CH_Num >= 3)
	/* ͨ��3(����ֵ) */
	0,
#endif

#if (Print_Sensor_CH_Num >= 4)
	/* ͨ��4 */
	0,
#endif
};

/* �����Ŵ�ӡ���� */
#if (Print_Scheme == 1)

/* �����¶� */
static Print_JWX_OutInTemp_Struct ArriveTemp = 
{
	0,
#if (Print_Sensor_CH_Num >= 1)
	/* ͨ��1 */
	0,
#endif	
	
#if (Print_Sensor_CH_Num >= 2)	
	/* ͨ��2 */
	0,
#endif	
	
#if (Print_Sensor_CH_Num >= 3)
	/* ͨ��3(����ֵ) */
	0,
#endif

#if (Print_Sensor_CH_Num >= 4)
	/* ͨ��4 */
	0,
#endif
};

#endif


/*************************************************************************************************/
static BOOL Print_Get_Time_Data(INSTRU_PRINT_FIX* search_time,u32 adder,u32 *search_time_dr);

/*************************************************************************************************/

/*************************************************************************************************/
/* ������	: Print_Index_Save_Sensor_Data										 	 			 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: ��¼��ǰ�������ݵ�ϵͳʱ���Flash�洢��ַ. 									 	 	 */
/* ��������  : 2015/7/1																			 */
/*************************************************************************************************/
void Print_Index_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* ʱ��Ĵ��� */
	Times time_b;
	/* ��ʼʱ������ָ�� */
	u8* start_time_p;
	/* ��ʼʱ�� */
	u32 start_time;
	/* ��ʼ��¼��ַ */
	u32 fifo_start_record_adder;
	
	/* ��ȡ��ǰϵͳʱ�� */
	time_b.Year 	= config_tab->Inf_State.Instru_RTC.Inf_State.Year;
	time_b.Month 	= config_tab->Inf_State.Instru_RTC.Inf_State.Month;
	time_b.Day 		= config_tab->Inf_State.Instru_RTC.Inf_State.Day;
	time_b.Hour 	= config_tab->Inf_State.Instru_RTC.Inf_State.Hour;
	time_b.Min 		= config_tab->Inf_State.Instru_RTC.Inf_State.Min;
	time_b.Sec 		= config_tab->Inf_State.Instru_RTC.Inf_State.Sec;
//	time_b.Week 	= config_tab->Inf_State.Instru_RTC.Inf_State.Week;
//	time_b.Zone 	= config_tab->Inf_State.Instru_RTC.Inf_State.Zone;
	
	/* ����ȡ��ǰϵͳʱ�� ת������ ��Ϊ��ʼʱ�� */
	start_time = RTC_Date_Time_To_Second(&time_b);
	/* С��оƬ���� */
	/* �ж������Ƿ�տ�ʼ��¼ */
	if((fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time == 0)&&(fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_End_Time == 0)&&
	   (fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder == 0)&&(fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder == 0))			
	{
		/* ���� ��ӡ��ʼʱ�� */
		fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time = start_time;			
		/* ���� ��ӡ��ʼ���ݵ�ַ */
		fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder = 0;
	}
	
	/* ���� ��ӡ����ʱ�� */
	fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_End_Time = start_time;
	/* ���� ��ӡ�������ݵ�ַ */
	fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder = ((s32)fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer+Flash_Capacity_Size-Save_Sensor_Data_Len)%Flash_Capacity_Size;
	
	/* ��������ָ��������ʼ��¼��ַ */
	fifo_start_record_adder = (fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer + 2*Flash_Sector_Size)%Flash_Capacity_Size;
	
	
	/* �ж���ʼ���ݵ�ַ�Ƿ��ڱ�ɾ����FLASH�����ڡ� */
	if(((fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder) >= fifo_start_record_adder)&&
	   ((fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder) < (fifo_start_record_adder+Flash_Sector_Size))&&
	    (fifo_start_record_adder <= fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder))
	{
		/* ��ʼ���ݵ�ַ�ڱ�������FLASH�������档 */
		/* �жϴ�ӡ����ʼ���ݵ�ַ С�� Flash������ַ */
		while(fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder < (fifo_start_record_adder+Flash_Sector_Size))
		{
			/* ��ӡ����ʼ���ݵ�ַ С�� Flash������ַ */
			/* ���� ��ӡ��ʼ���ݵ�ַ */
			fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder += Save_Sensor_Data_Len;
		}
		/* ���� ��ӡ��ʼ���ݵ�ַ */
		fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder = fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder%Flash_Capacity_Size;
		/* ��ȡ��ʼ��ַ�µ�ʱ����Ϊ ��ʼʱ�� */
		start_time_p = Flash_Read_Sensor_Data((fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder%Flash_Capacity_Size),sizeof(DATETIME_Storage_Format));
		/* ��ȡ��ʼʱ�� */
		time_b.Year 	= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.SECOND_BITS;
		/* ���� ��ӡ��ʼʱ�� */
		fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time = RTC_Date_Time_To_Second(&time_b);
	}
	
	
	/* �����ӡ���ñ���� */
	Write_Config_Tab(fix_tab->Inf_State.Instru_Print.Inf_State_Space,sizeof(INSTRU_PRINT_FIX));
	
}

/*************************************************************************************************/
/* ������	: Print_Set_Font														 	 		 */
/* ����		: ��																				 */
/* ���		: ��																			 	 */
/* ����		: ������������С															 	 	 */
/* n=1 12*12 �����ִ�ӡ																	 	 */
/* n=2 16*16 �����ִ�ӡ																	 	 */
/* ��������  : 2015/7/7																			 */
/* ע��  	:�޸�����ǰ��Ҫȷ����ӡ���Ѿ�ֹͣ��ӡ�������Ӱ�����ڴ�ӡ�����塣							 */
/*************************************************************************************************/
static BOOL Print_Set_Font(u8 font_size)
{
	/* �������û����С */
#define	set_font_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_font_data_buffer[set_font_data_buffer_size];
	
	/* �ж� ѡ������ ���� �Ƿ�Ϸ� */
	if((font_size != 1)&&(font_size != 2))
	{
		/* ����������Ч */
		return FALSE;
	}
	
	/* ���ò��� */
	/* ����ͷ */
	set_font_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:���������С */
	set_font_data_buffer[1] = Print_CMD_Set_Font_Size;
	/* ���ò��� */
	set_font_data_buffer[2] = font_size;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_font_data_buffer,set_font_data_buffer_size);
	
	return TRUE;

#undef	set_font_data_buffer_size
}

/*************************************************************************************************/
/* ������	: Print_Set_CRLF														 	 		 */
/* ����		: ��																				 */
/* ���		: ��																			 	 */
/* ����		: ��ӡ����																 	 	 	*/
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_CRLF(void)
{

//	/* ����س����� */
//	printf("\r\n");
	
	/* �������û����С */
#define	set_CRLF_data_buffer_size	(1u)
	/* �������ݻ��� */
	char set_CRLF_data_buffer;
	
	/* ���ò��� */
	/* ����:���� */
	set_CRLF_data_buffer = Print_CMD_Set_CRLF;

	/* �������� */
	USART_printf(USART1,(uint8_t*)&set_CRLF_data_buffer,set_CRLF_data_buffer_size);
	
#undef	set_CRLF_data_buffer_size
}

/*************************************************************************************************/
/* ������	: Print_Set_Line_Space													 	 		 */
/* ����		: ��																				 */
/* ���		: ��																			 	 */
/* ����		: �����м��																	 	 	 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Line_Space(u8 line_space)
{
	/* �������û����С */
#define	set_line_space_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_line_space_data_buffer[set_line_space_data_buffer_size];
	
	
	/* ���ò��� */
	/* ����ͷ */
	set_line_space_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:�м�� */
	set_line_space_data_buffer[1] = Print_CMD_Set_Line_Space;
	/* ����:����� */
	set_line_space_data_buffer[2] = line_space;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_line_space_data_buffer,set_line_space_data_buffer_size);
	
#undef	set_line_space_data_buffer_size

}

/*************************************************************************************************/
/* ������	: Print_Set_Word_Space													 	 		 */
/* ����		: ��																				 */
/* ���		: ��																			 	 */
/* ����		: �����ּ��																	 	 	 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Word_Space(u8 word_space)
{
	/* �������û����С */
#define	set_word_space_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_word_space_data_buffer[set_word_space_data_buffer_size];
	
	
	/* ���ò��� */
	/* ����ͷ */
	set_word_space_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:�ּ�� */
	set_word_space_data_buffer[1] = Print_CMD_Set_Word_Space;
	/* ����:����� */
	set_word_space_data_buffer[2] = word_space;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_word_space_data_buffer,set_word_space_data_buffer_size);
	
#undef	set_word_space_data_buffer_size

}

#if (0)
/*************************************************************************************************/
/* ������	: Print_Set_Blank_line_Null												 	 		 */
/* ����		: blank_line_state=0:���ÿո�blank_line_state =1:���ÿ��С�num:�ո����е�������		 */
/* ���		: ��																			 	 */
/* ����		: ��ӡ�ո�����(������й��ܺ���û�ã�) 												 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Blank_Line_Null(u8 blank_line_state,u8 num)
{
	/* �������û����С */
#define	set_blank_line_null_data_buffer_size	(4u)
	/* �������ݻ��� */
	char set_blank_line_null_data_buffer[set_blank_line_null_data_buffer_size];
	
	/* �ж� ���ÿ��л�ո�״̬ */
	if(blank_line_state != 0)
	{
		/* ��0 ������1��Ϊ���ÿ��С� */
		blank_line_state = 1;
	}
	
	/* ���ò��� */
	/* ����ͷ */
	set_blank_line_null_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:�ո����� */
	set_blank_line_null_data_buffer[1] = Print_CMD_Set_Blank_Line_Null;
	/* ����:�ո����� */
	set_blank_line_null_data_buffer[2] = blank_line_state;
	/* ����:��ӡ���� */
	set_blank_line_null_data_buffer[3] = num;	
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_blank_line_null_data_buffer,set_blank_line_null_data_buffer_size);
	
#undef	set_blank_line_null_data_buffer_size
}
#endif

/*************************************************************************************************/
/* ������	: Print_Set_Right_Limit													 	 		 */
/* ����		: offset:����ƫ����																	 */
/* ���		: ��																			 	 */
/* ����		: �������� 																	 	 	 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Right_Limit(u8 offset)
{
	/* �������û����С */
#define	set_right_limit_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_right_limit_data_buffer[set_right_limit_data_buffer_size];
	
	
	/* ���ò��� */
	/* ����ͷ */
	set_right_limit_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:���� */
	set_right_limit_data_buffer[1] = Print_CMD_Set_Right_Limit;
	/* ����:ƫ���� */
	set_right_limit_data_buffer[2] = offset;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_right_limit_data_buffer,set_right_limit_data_buffer_size);
	
#undef	set_right_limit_data_buffer_size
}

/*************************************************************************************************/
/* ������	: Print_Set_Left_Limit													 	 		 */
/* ����		: offset:����ƫ����																	 */
/* ���		: ��																			 	 */
/* ����		: �������� 																	 	 	 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Left_Limit(u8 offset)
{
	/* �������û����С */
#define	set_left_limit_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_left_limit_data_buffer[set_left_limit_data_buffer_size];
	
	
	/* ���ò��� */
	/* ����ͷ */
	set_left_limit_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:���� */
	set_left_limit_data_buffer[1] = Print_CMD_Set_Left_Limit;
	/* ����:ƫ���� */
	set_left_limit_data_buffer[2] = offset;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_left_limit_data_buffer,set_left_limit_data_buffer_size);
	
#undef	set_left_limit_data_buffer_size
}

/*************************************************************************************************/
/* ������	: Print_Set_Underline_Config													 	 */
/* ����		: NewState=ENABLE;�����»��ߣ�NewState=DISABLE;�ر��»��ߣ�							 */
/* ���		: ��																			 	 */
/* ����		: ��ӡ����/��ֹ�»���.														 	 	 */
/* ��������  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Set_Underline_Config(FunctionalState NewState)
{
	/* �������û����С */
#define	set_underline_config_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_underline_config_data_buffer[set_underline_config_data_buffer_size];
	
	
	/* ���ò��� */
	/* ����ͷ */
	set_underline_config_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:����/��ֹ�»��� */
	set_underline_config_data_buffer[1] = Print_CMD_Set_Underline_Config;
	/* ����:�»��߹���״̬ */
	set_underline_config_data_buffer[2] = NewState;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_underline_config_data_buffer,set_underline_config_data_buffer_size);
	
#undef	set_underline_config_data_buffer_size

}

/*************************************************************************************************/
/* ������	: Print_Set_Underline_Config													 	 */
/* ����		: Angle:�ַ���ת�Ƕ�																	 */
/* ���		: TRUE:���óɹ���FALSE������ʧ�ܡ�													 	 */
/* Angle=0 0 �� 																		 	 	 */
/* Angle=1 90 ��																		 	 	 */
/* Angle=2 180 ��																		 	 	 */
/* Angle=3 270 ��																		 	 	 */
/* ����		: �����ַ���ת.																 	 	 */
/* ��������  : 2015/7/8																			 */
/*************************************************************************************************/
static BOOL Print_Set_Rotate_Char(u8 Angle)
{
	/* �������û����С */
#define	set_rotate_char_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_rotate_char_data_buffer[set_rotate_char_data_buffer_size];
	
	/* �жϽǶȲ����Ϸ��� */
	if(Angle > 3)
	{
		/* �ǶȲ����Ƿ�������ʧ�� */
		return FALSE;
	}
	
	/* ���ò��� */
	/* ����ͷ */
	set_rotate_char_data_buffer[0] = Print_CMD_Head_0X1C;
	/* ����:��ת�ַ� */
	set_rotate_char_data_buffer[1] = Print_CMD_Set_Rotate_Char;
	/* ����:��ת�Ƕ� */
	set_rotate_char_data_buffer[2] = Angle;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_rotate_char_data_buffer,set_rotate_char_data_buffer_size);
	
	return FALSE;
	
#undef	set_rotate_char_data_buffer_size

}

/*************************************************************************************************/
/* ������	: Print_Separator_Line															 	 */
/* ����		: ��																				 */
/* ���		: ��													 	 						 */
/* ����		: ��ӡ�ָ����������������ݣ�													 	 	 */
/* ��������  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Separator_Line(void)
{
	
	/* ���ݲ���������ӡ�ָ���š� */
	/* �س� */
	Print_Set_CRLF();
	/* �س� */
	Print_Set_CRLF();
	/* �س� */
	Print_Set_CRLF();
	/* �س� */
	Print_Set_CRLF();
	/* �س� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(1500);
}

/*************************************************************************************************/
/* ������	: Print_Init																	 	 */
/* ����		: ��																				 */
/* ���		: ��																			 	 */
/* ����		: ��ʼ����ӡ��																 	 	 */
/* ��������  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Init(void)
{
	/* �������û����С */
#define	print_init_buffer_size	(2u)
	/* �������ݻ��� */
	char print_init_buffer[print_init_buffer_size];
	
	/* ���ò��� */
	/* ����ͷ */
	print_init_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:��ʼ����ӡ�� */
	print_init_buffer[1] = Print_CMD_Init;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)print_init_buffer,print_init_buffer_size);
	
	/* ���ݷ�����ɣ��ȴ���ӡ��������� */
	OSTimeDly(1);
#undef	print_init_buffer_size

}

/*************************************************************************************************/
/* ������	: Print_Set_Highlight_Char														 	 */
/* ����		: NewState=ENABLE:ʹ�ܷ�����ʾ��NewState=DISABLE:�رշ�����ʾ��							 */
/* ���		: ��																			 	 */
/* ����		: ����/��ֹ���״�ӡ															 	 	 */
/* ��������  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Set_Highlight_Char(FunctionalState NewState)
{
	/* �������û����С */
#define	set_highlight_char_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_highlight_char_data_buffer[set_highlight_char_data_buffer_size];
	
	
	/* ���ò��� */
	/* ����ͷ */
	set_highlight_char_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:����/��ֹ���״�ӡ */
	set_highlight_char_data_buffer[1] = Print_CMD_Set_Highlight_Char;
	/* ����:���״�ӡ״̬ */
	set_highlight_char_data_buffer[2] = NewState;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_highlight_char_data_buffer,set_highlight_char_data_buffer_size);
	
#undef	set_highlight_char_data_buffer_size

}

#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
/*************************************************************************************************/
/* ������	: Print_Set_Picture																 	 */
/* ����		: Len:ͼ�����ݳ��ȣ�*Data:ͼ������ָ�롣												 */
/* ���		: TRUE:���óɹ���FALSE������ʧ�ܡ�													 	 */
/* ����		: ��ӡͼ������																 	 	 */
/* ��������  : 2015/7/8																			 */
/*************************************************************************************************/
static BOOL Print_Set_Picture(u16 Len,char *Data)
{
	/* �������û����С */
#define	set_picture_data_buffer_size	(4u)
	/* �������ݻ��� */
	char set_picture_data_buffer[set_picture_data_buffer_size];
	
	/* �ж�ͼ�����ݳ��� �Ƿ񳬱� */
	if(Len > 48)
	{
		/* ͼ�����ݳ��ȳ��� */
		return FALSE;
	}
	
	/* ���ò��� */
	/* ����ͷ */
	set_picture_data_buffer[0] = Print_CMD_Head_0X1F;
	/* ����:ͼ������ */
	set_picture_data_buffer[1] = Print_CMD_Set_Picture;
	/* ���õ�λ���ݳ��� */
	set_picture_data_buffer[2] = Len%0xFF;	
	/* ���ø�λ���ݳ��� */
	set_picture_data_buffer[3] = Len/0xFF;
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_picture_data_buffer,set_picture_data_buffer_size);
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)Data,Len);	
	
	return TRUE;
	
#undef	set_picture_data_buffer_size

}
#endif

#if 0
/*************************************************************************************************/
/* ������	: Print_Set_Barcode																 	 */
/* ����		: Mode=0:EAN-8;Mode=1:EAN-13;*Data:����������ָ�롣									 */
/* ���		: ��																			 	 */
/* Mode=0 EAN-8																		 	 	 	 */
/* Mode=1 EAN-13																		 	 	 */
/* ����		: ��ӡ������																	 	 	 */
/* ��������  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Set_BarCode(u8 Mode,char *Data)
{
	/* �������û����С */
#define	set_barcode_data_buffer_size	(4u)
	/* �������ݻ��� */
	char set_barcode_data_buffer[set_barcode_data_buffer_size];
	
	
	/* �ж� ��ӡ������ģʽ */
	if(Mode != 0)
	{
		Mode = 1;
	}
	
	/* ���ò��� */
	/* ����ͷ */
	set_barcode_data_buffer[0] = Print_CMD_Head_0X1D;
	/* ����:��ӡ������ */
	set_barcode_data_buffer[1] = Print_CMD_Set_BarCode;
	
	/* ���ݵ�ǰģʽ ���ô�ӡ������������ */
	if(Mode == 0)
	{
		/* EAN-8 */
		set_barcode_data_buffer[2] = 0X02;	
		set_barcode_data_buffer[3] = 0X08;	
	}else
	{
		/* EAN-13 */
		set_barcode_data_buffer[2] = 0X03;	
		set_barcode_data_buffer[3] = 0X0D;	
	}

	/* �������� */
	USART1_printf((uint8_t*)set_barcode_data_buffer,set_barcode_data_buffer_size);
	
	/* �������� */
	USART1_printf((uint8_t*)Data,set_barcode_data_buffer[3]);	
	
#undef	set_barcode_data_buffer_size

}

#endif

/*************************************************************************************************/
/* ������	: Print_Set_Font_Double															 	 */
/* ����		: State=RESET:�������壻State=SET:�����С������										 */
/* ���		: ��																			 	 */
/* ����		: ���巭��/��ԭ																 	 	 */
/* ��������  : 2015/7/8																			 */
/* ע��  	:�޸�����ǰ��Ҫȷ����ӡ���Ѿ�ֹͣ��ӡ�������Ӱ�����ڴ�ӡ�����塣							 */
/*************************************************************************************************/
static void Print_Set_Font_Double(FlagStatus State)
{
	/* �������û����С */
#define	set_font_double_data_buffer_size	(3u)
	/* �������ݻ��� */
	char set_font_double_data_buffer[set_font_double_data_buffer_size];
	
	/* ���ò��� */
	/* ����ͷ */
	set_font_double_data_buffer[0] = Print_CMD_Head_0X1B;
	/* ����:���巭��/��ԭ */
	set_font_double_data_buffer[1] = Print_CMD_Set_Font_Double;
	/* ����:���巭��״̬ */
	set_font_double_data_buffer[2] = State;
	
	
	/* �������� */
	USART_printf(USART1,(uint8_t*)set_font_double_data_buffer,set_font_double_data_buffer_size);
	
#undef	set_font_double_data_buffer_size

}

/*************************************************************************************************/
/* ������	: Print_Set_Para													 	 			 */
/* ����		: ��																				 */
/* ���		: ��																			 	 */
/* ����		: ��ӡ���ݱ���																 	 	 */
/* ��������  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Set_Para(void)
{


	/* 1����ʼ����ӡ�� */
	Print_Init();
	
	/* 2��������������С */
	/* �����С������ */
	Print_Set_Font_Double(RESET);
	/* ����Ϊ16*16��С������ */
	Print_Set_Font(Print_Font_16_16);
	
	/* 3�������м�� */
	Print_Set_Line_Space(8);
	
	/* 4�������ּ�� */
	Print_Set_Word_Space(0);
	
	/* 5���������� */
	Print_Set_Right_Limit(0);	
	
	/* 6���������� */
	Print_Set_Left_Limit(0);	
	
	/* 7����������/��ֹ�»���. */
	Print_Set_Underline_Config(DISABLE);
		
	/* 8�������ַ���ת�Ƕ�. */
	Print_Set_Rotate_Char(Print_Font_Rotate_Angle_0);	

	/* 9����������/��ֹ���״�ӡ. */
	Print_Set_Highlight_Char(DISABLE);		
	
	/* 10�����ô�ӡ�س�����. */
	Print_Set_CRLF();
	/* �ȴ���ӡ���� */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* �ȴ���ӡ���� */
	OSTimeDly(120);
	
}

/*************************************************************************************************/
/* ������	: Print_Indivisible_Symbol											 	 			 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: ��ӡ�ָ��																 	 	 	 */
/* ��������  : 2015/7/13																			 */
/*************************************************************************************************/
static void Print_Indivisible_Symbol(void)
{
	
	
	/* �����С���� */
	Print_Set_Font_Double(SET);
	/* ����Ϊ12*12��С������ */
	Print_Set_Font(Print_Font_12_12);
	/* ���÷ָ��� */
	printf(Print_Indivisible_Symbol_Start_Str);
	OSTimeDly(200);
	printf(Print_Indivisible_Symbol_End_Str);
	OSTimeDly(200);
	
}

/*************************************************************************************************/
/* ������	: Print_Search_End_Time														 */
/* ����		: ��																		 		 */
/* ���		: ��		 																		 */
/* ����		: ��������ʱ�估����															 	 	 */
/* ��������  : 2016/09/10																		 */
/*************************************************************************************************/
static void Print_Search_End_Time(INSTRU_SENSOR* sensor_inf,Print_JWX_OutInTemp_Struct * Arrive_p,int start_adder,int end_adder)
{
	/* ������ָ�� */
	u8* data_p;
	/* ʱ��Ĵ��� */
	Times time_b;
	/* ��ʱ��Ĵ��� */
	u32 second_time_b;
	
	
#if (Print_Sensor_CH_Num >= 1)
	/* ͨ��1���� */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* ͨ��2���� */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ͨ��3���� */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* ͨ��4���� */
	float ch_4_data;
#endif
	
	
	
	
	while(start_adder <= end_adder)
	{
		
		/* ��ȡ��ʼ��ַ���� */
		data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		
			/* ��ȡ ʱ������� */
	#if (Print_Sensor_CH_Num >= 1)
		/* ��ȡͨ��1���� */
		#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
			ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
		#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
			   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
			ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
		#endif	
	#endif
			
	#if (Print_Sensor_CH_Num >= 2)
		/* ��ȡͨ��2���� */
		#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
			ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
		#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
			   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
			ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
		#endif		
	#endif
			
	#if (Print_Sensor_CH_Num >= 3)
		/* ��ȡͨ��3���� */
		#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
			ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
		#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
			   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
			ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
		#endif		
	#endif

	#if (Print_Sensor_CH_Num == 4)
		/* ��ȡͨ��4���� */
		#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
			ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
		#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
			   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
			ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
		#endif		
	#endif

		/* ��ȡʱ������ */
		time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
			
		/* ��ʱ��ת������ */
		second_time_b = RTC_Date_Time_To_Second(&time_b);

		/* �жϵ�ǰ����״̬ */
		if(
	#if (Print_Sensor_CH_Num >= 1)
			((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
		   &&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
		   ||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
	#endif
	#if (Print_Sensor_CH_Num >= 2)
		   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
			&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
			||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
	#endif
	#if (Print_Sensor_CH_Num >= 3)
		   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
			&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
			||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
	#endif
	#if (Print_Sensor_CH_Num >= 4)
		   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
			&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
			||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
	#endif		
			)
			{
				/* �������� */
				/* �жϵ�ǰ��ʱ����Ƿ�Ϊ������¼ʱ��� */
				if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl == 0)
				{
					/* ��ǰ������Ϊ����ʱ�䡣 */
					#if (Print_Sensor_CH_Num >= 1)
							/* ����1 */
							Arrive_p->Ch1_Value = ch_1_data;
					#endif
					#if (Print_Sensor_CH_Num >= 2)
							/* ����2 */
							Arrive_p->Ch2_Value = ch_2_data;
					#endif
					#if (Print_Sensor_CH_Num >= 3)
							/* ����3 */
							Arrive_p->Ch3_Value = ch_3_data;
					#endif
					#if (Print_Sensor_CH_Num >= 4)
							/* ����4 */
							Arrive_p->Ch4_Value = ch_4_data;
					#endif
							/* ���ó���ʱ�� */
							Arrive_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
							Arrive_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
							Arrive_p->Time.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
							Arrive_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
							Arrive_p->Time.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
							Arrive_p->Time.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
					return;
				}
			}else
			{
				/* ���ݳ��� */ 
				/* �жϵ�ǰ��ʱ����Ƿ�Ϊ������¼ʱ��� */
				if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl == 0)
				{
					/* ��ǰ������Ϊ����ʱ�䡣 */
					#if (Print_Sensor_CH_Num >= 1)
							/* ����1 */
							Arrive_p->Ch1_Value = ch_1_data;
					#endif
					#if (Print_Sensor_CH_Num >= 2)
							/* ����2 */
							Arrive_p->Ch2_Value = ch_2_data;
					#endif
					#if (Print_Sensor_CH_Num >= 3)
							/* ����3 */
							Arrive_p->Ch3_Value = ch_3_data;
					#endif
					#if (Print_Sensor_CH_Num >= 4)
							/* ����4 */
							Arrive_p->Ch4_Value = ch_4_data;
					#endif
							/* ���ó���ʱ�� */
							Arrive_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
							Arrive_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
							Arrive_p->Time.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
							Arrive_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
							Arrive_p->Time.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
							Arrive_p->Time.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
					return;
				}
			}
		/* ������һ�����ݵ�ַ */
		end_adder -= Save_Sensor_Data_Len;
	}
}

/*************************************************************************************************/
/* ������	: Print_Data_Title													 	 			 */
/* ����		: sensor_inf:������ͨ����Ϣ��print_inf:��ӡ��Ϣ										 */
/* ���		: ��																			 	 */
/* ����		: ��ӡ���ݱ���																 	 	 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Data_Title(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf,int start_adder,int end_adder)
{ 
	/* ʱ�����ݻ��� */
	Times time_dr;
	
	
	/* ��ӡ�ָ�� */
	Print_Indivisible_Symbol();
	
//	/* ��˾����:****** */	
//	printf(Company_Name_Str,Company_Name_Text_Str);
//	/* �ȴ������ӡ���� */
//	OSTimeDly(200);
	
	/* �����С������ */
	Print_Set_Font_Double(RESET);
	/* ����Ϊ16*16��С������ */
	Print_Set_Font(Print_Font_16_16);
	/* �豸����:****** */
	printf(Device_Number_Str,
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
/* �����Ŵ�ӡ���� */
#if (Print_Scheme == 1)
	/* ��ӡ�˵���:7DC311001-1F9314C0-1F946640 */
	printf(Print_TransportNo_Str,
		   ((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
		   ((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
		   ((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
		   ((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)),
		   print_inf->Inf_State.Print_Start_Time,
		   print_inf->Inf_State.Print_End_Time);
#endif
	
	/* ��ӡʱ���:****** */
	printf(Print_Time_Period_Str);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
	/* ���ݴ�ӡ��ʼʱ�䣬����ʱ�����ݼĴ��� */
	RTC_Second_To_Date_Time(print_inf->Inf_State.Print_Start_Time,&time_dr);
	/* ��ʼʱ��:****** */
	printf(Print_Start_Time_Str,
	time_dr.Year,
	time_dr.Month,
	time_dr.Day,
	time_dr.Hour,
	time_dr.Min);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
	/* �ж��Ƿ������� */ 
	if((start_adder == 0)&&(end_adder == 0))
	{
		/* ���ݴ�ӡ����ʱ�䣬����ʱ�����ݼĴ��� */
		RTC_Second_To_Date_Time(print_inf->Inf_State.Print_Record_End_Time,&time_dr);
	}else
	{
		//����ҽҩ��ֵ����󣺼����ӡ����ʱ����ڼ�¼����ʱ��ʱ������ӡ����ʱ�䡱�ü�¼����ʱ�䣻\
							 ����ӡ����ʱ��С�ڼ�¼����ʱ��ʱ������ӡ����ʱ�䡱�ô�ӡ����ʱ�䡣
		Print_Search_End_Time(sensor_inf,&StartOffTemp,start_adder,end_adder);
		
		time_dr = StartOffTemp.Time;
	}
	
	/* ����ʱ��:****** */
	printf(Print_End_Time_Str,
	time_dr.Year,
	time_dr.Month,
	time_dr.Day,
	time_dr.Hour,
	time_dr.Min);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
	/* ��ӡ������ͨ��1�����ݷ�Χ */
#if (Print_Sensor_CH_Num >= 0x01)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
	/* �¶����ʹ����� */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_1+1),
	(	  (sensor_inf->Inf_State.CH1.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH1.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH1.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH1.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* ʪ�����ʹ����� */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_1+1),
	((s16) sensor_inf->Inf_State.CH1.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Low_Limit*10)%10),	
	((s16) sensor_inf->Inf_State.CH1.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#endif
#endif

	/* ��ӡ������ͨ��2�����ݷ�Χ */
#if (Print_Sensor_CH_Num >= 0x02)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
	/* �¶����ʹ����� */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_2+1),
	(	  (sensor_inf->Inf_State.CH2.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH2.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH2.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH2.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* ʪ�����ʹ����� */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_2+1),
	((s16) sensor_inf->Inf_State.CH2.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Low_Limit*10)%10),	
	((s16) sensor_inf->Inf_State.CH2.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#endif
#endif

	/* ��ӡ������ͨ��3�����ݷ�Χ */
#if (Print_Sensor_CH_Num >= 0x03)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
	/* �¶����ʹ����� */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_3+1),
	(	  (sensor_inf->Inf_State.CH3.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH3.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH3.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH3.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* ʪ�����ʹ����� */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_3+1),
	((s16) sensor_inf->Inf_State.CH3.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Low_Limit*10)%10),
	((s16) sensor_inf->Inf_State.CH3.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#endif
#endif

	/* ��ӡ������ͨ��4�����ݷ�Χ */
#if (Print_Sensor_CH_Num >= 0x04)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
	/* �¶����ʹ����� */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_4+1),
	(	  (sensor_inf->Inf_State.CH4.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH4.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH4.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH4.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* ʪ�����ʹ����� */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_4+1),
	((s16) sensor_inf->Inf_State.CH4.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Low_Limit*10)%10),
	((s16) sensor_inf->Inf_State.CH4.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#endif
#endif

	/* ���ݼ�¼����:****** */
	printf(Print_Record_Data_Str);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);

}

/*************************************************************************************************/
/* ������	: Print_Get_Time_Data												 	 			 */
/* ����		: search_time,adder,search_time_dr													 */
/* ���		: TRUE:��ȡʱ��ɹ���FALSE:��ȡʱ��ʧ�ܡ�											 	 */
/* ����		: ���ݵ�ַ��ȡ�洢��FLASH�еĴ���������ʱ�����									 	 	 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static BOOL Print_Get_Time_Data(INSTRU_PRINT_FIX* search_time,u32 adder,u32 *search_time_dr)
{
	/* ʱ������ָ�� */
	u8 * time_p;
	/* ʱ�����ݻ��� */
	Times time_dr;
	
	/* ��ȡ ʱ������ */
	time_p = Flash_Read_Sensor_Data(adder%Flash_Capacity_Size,sizeof(DATETIME_Storage_Format));

	/* ת��ʱ���ʽ */
	time_dr.Year 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.YEAR_BITS + 2000;
	time_dr.Month 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.MONTH_BITS;
	time_dr.Day 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.DAY_BITS;
	time_dr.Hour 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.HOUR_BITS;
	time_dr.Min 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.MINUTE_BITS;
	time_dr.Sec 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.SECOND_BITS;
	
	/* ��ʱ������ת���������� */
	*search_time_dr = RTC_Date_Time_To_Second(&time_dr);
	
	/* �ж�ʱ�������Ƿ����� */
	if(*search_time_dr < search_time->Inf_State.Print_Record_Start_Time)
	{
		/* ����С�ڼ�¼��ʼʱ�� */
		return FALSE;
	}else if(*search_time_dr > search_time->Inf_State.Print_Record_End_Time)
	{
		/* ���ݴ��ڼ�¼����ʱ�� */
		return FALSE;
	}else
	{
		/* �����ڼ�¼��ʼʱ��ͼ�¼����ʱ��֮�䡣 */
		return TRUE;
	}

}

/*************************************************************************************************/
/* ������	: Print_Search_Start_Adder													 	 	 */
/* ����		: search_time,search_adder															 */
/* ���		: TRUE:��ȡ��ַ�ɹ���FALSE:��ȡ��ַʧ�ܡ�											 	 */
/* ����		: ����ʱ������ӡ������FLASH�еĵ�ַλ��											 	 	 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static 	BOOL Print_Search_Start_Adder(INSTRU_PRINT_FIX* search_time,u32 *start_adder)
{
	/* ���� ��ʼ λ�� */
	u32 search_start_adder;
	/* ���� ���� λ�� */
	u32 search_end_adder;
	/* ���� �м��ַ λ�� */
	u32 search_centre_adder;
	/* ���� ��Χ */
	u32 search_range_max = Flash_Capacity_Size/Save_Sensor_Data_Len;
	/* ������ʱ������ */
	u32 search_time_dr;
	/* ������ʱ���� */
	BOOL search_time_result;
	/* ������ */
	u8 num;
	

	/* �ж���ʼʱ�����Ǹ����䷶Χ */
	if(search_time->Inf_State.Print_Start_Time <= search_time->Inf_State.Print_Record_Start_Time)
	{
		/* ������ʱ��С�ڵ�����ʼ��¼ʱ�䣬����ʼ��¼��ַ��Ϊ��������ʼ��ַ�� */
		*start_adder = search_time->Inf_State.Print_Record_Start_Adder;
		/* �ɹ� */
		return TRUE;
	}else if((search_time->Inf_State.Print_Start_Time <= search_time->Inf_State.Print_Record_End_Time)&&
			 (search_time->Inf_State.Print_Start_Time >= search_time->Inf_State.Print_Record_Start_Time))
	{
		/* ������ʱ��С�ڵ��ڽ�����¼ʱ�䡢���ڵ�����ʼ��¼ʱ�䡣 */
		/* ����������ʼλ�� */
		search_start_adder = search_time->Inf_State.Print_Record_Start_Adder;
		/* ������������λ�� */
		if(search_time->Inf_State.Print_Record_End_Adder < search_time->Inf_State.Print_Record_Start_Adder)
		{
			/* ������¼��ַ���� С�� ��ʼ��¼��ַ���� */
			search_end_adder = search_time->Inf_State.Print_Record_End_Adder + Flash_Capacity_Size;
		}else
		{
			/* ������¼��ַ���� ���ڵ��� ��ʼ��¼��ַ���� */
			search_end_adder = search_time->Inf_State.Print_Record_End_Adder;	
		}
		/* �ж���ʼ��ַ�����ͽ�����ַ�����ĺϷ��� */
		if(search_end_adder < search_start_adder)
		{
			/* ϵͳ�쳣 */
			return FALSE;
		}
		
		/* ��ʼ������ʼ��ַ */
		for(num = 0 ; num < search_range_max ; num ++)
		{
			
			/* �ж���������ʼ��ַ�ͽ�����ַ�Ƿ�С�ڵ���һ�����ݳ��� */
			if((search_end_adder-search_start_adder) > Save_Sensor_Data_Len)
			{
				/* ��ȡ �м�� ��ַ */				
				search_centre_adder = ((search_end_adder - search_start_adder)/2/Save_Sensor_Data_Len)*Save_Sensor_Data_Len + search_start_adder;
				/* ��ȡ �м�� ʱ������ */
				search_time_result = Print_Get_Time_Data(search_time,search_centre_adder,&search_time_dr);
				
				/* �ж�����ʱ���Ƿ����� */
				if(search_time_result == FALSE)
				{
					/* ������,����ʧ�� */
					return FALSE;
				}
				
				/* �����м���ʱ������ �� ��ѯ��ʱ���Ƿ���� */
				if(search_time_dr > search_time->Inf_State.Print_Start_Time)
				{
					/* ������ʱ�������ʼʱ�� *///����д��Ϊ��ȥ���п��ܵ�С�����֡�
					search_end_adder = search_centre_adder;
				}
				else if(search_time_dr < search_time->Inf_State.Print_Start_Time)
				{
					/* ������ʱ�������ʼʱ�� *///����д��Ϊ��ȥ���п��ܵ�С�����֡�
					search_start_adder = search_centre_adder;
				}
				else
				{
					/* �ɹ������ʼ��ַ */
					*start_adder = search_centre_adder;
					/* �ɹ� */
					return TRUE;
				}
			
			}else
			{
				/* ��ʼ��ַ�ͽ�����ַ���ڣ�û�и��Ӻ��ʵ���ʼ��ַ�ˡ�ѡ�������ַ��Ϊ������ʼ��ַ */
				*start_adder = search_end_adder;			
				/* �ɹ� */
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*************************************************************************************************/
/* ������	: Print_Search_End_Adder													 	 	 */
/* ����		: search_time,search_adder															 */
/* ���		: TRUE:��ȡ��ַ�ɹ���FALSE:��ȡ��ַʧ�ܡ�											 	 */
/* ����		: ����ʱ������ӡ������FLASH�еĵ�ַλ��											 	 	 */
/* ��������  : 2015/7/7																			 */
/*************************************************************************************************/
static 	BOOL Print_Search_End_Adder(INSTRU_PRINT_FIX* search_time,u32 *end_adder)
{
	/* ���� ��ʼ λ�� */
	u32 search_start_adder;
	/* ���� ���� λ�� */
	u32 search_end_adder;
	/* ���� �м��ַ λ�� */
	u32 search_centre_adder;
	/* ���� ��Χ */
	u32 search_range_max = Flash_Capacity_Size/Save_Sensor_Data_Len;
	/* ������ʱ������ */
	u32 search_time_dr;
	/* ������ʱ���� */
	BOOL search_time_result;
	/* ������ */
	u8 num;
	

	/* �ж���ʼʱ�����Ǹ����䷶Χ */
	if(search_time->Inf_State.Print_End_Time >= search_time->Inf_State.Print_Record_End_Time)
	{
		/* ������������λ�� */
		if(search_time->Inf_State.Print_Record_End_Adder < search_time->Inf_State.Print_Record_Start_Adder)
		{
			/* ������¼��ַ���� С�� ��ʼ��¼��ַ���� */
			*end_adder = search_time->Inf_State.Print_Record_End_Adder + Flash_Capacity_Size;		
		}else
		{
			/* ������ʱ��С�ڵ�����ʼ��¼ʱ�䣬����ʼ��¼��ַ��Ϊ��������ʼ��ַ�� */
			*end_adder = search_time->Inf_State.Print_Record_End_Adder;		
		}

		/* �ɹ� */
		return TRUE;
	}else if((search_time->Inf_State.Print_End_Time <= search_time->Inf_State.Print_Record_End_Time)&&
			 (search_time->Inf_State.Print_End_Time >= search_time->Inf_State.Print_Record_Start_Time))
	{
		/* ������ʱ��С�ڵ��ڽ�����¼ʱ�䡢���ڵ�����ʼ��¼ʱ�䡣 */
		/* ����������ʼλ�� */
		search_start_adder = search_time->Inf_State.Print_Record_Start_Adder;
		/* ������������λ�� */
		if(search_time->Inf_State.Print_Record_End_Adder < search_time->Inf_State.Print_Record_Start_Adder)
		{
			/* ������¼��ַ���� С�� ��ʼ��¼��ַ���� */
			search_end_adder = search_time->Inf_State.Print_Record_End_Adder + Flash_Capacity_Size;
		}else
		{
			/* ������¼��ַ���� ���ڵ��� ��ʼ��¼��ַ���� */
			search_end_adder = search_time->Inf_State.Print_Record_End_Adder;	
		}
		/* �ж���ʼ��ַ�����ͽ�����ַ�����ĺϷ��� */
		if(search_end_adder < search_start_adder)
		{
			/* ϵͳ�쳣 */
			return FALSE;
		}
		
		/* ��ʼ������ʼ��ַ */
		for(num = 0 ; num < search_range_max ; num ++)
		{
			/* �ж���������ʼ��ַ�ͽ�����ַ�Ƿ�С�ڵ���һ�����ݳ��� */
			if((search_end_adder-search_start_adder) > Save_Sensor_Data_Len)
			{

				/* ��ȡ �м�� ��ַ */				
				search_centre_adder = ((search_end_adder - search_start_adder)/2/Save_Sensor_Data_Len)*Save_Sensor_Data_Len + search_start_adder;
				
				/* ��ȡ �м�� ʱ������ */
				search_time_result = Print_Get_Time_Data(search_time,search_centre_adder,&search_time_dr);
				
				/* �ж�����ʱ���Ƿ����� */
				if(search_time_result == FALSE)
				{
					/* ������,����ʧ�� */
					return FALSE;
				}
				
				/* �����м���ʱ������ �� ��ѯ��ʱ���Ƿ���� */
				if(search_time_dr > search_time->Inf_State.Print_End_Time)
				{
					/* ������ʱ�������ʼʱ�� *///����д��Ϊ��ȥ���п��ܵ�С�����֡�
					search_end_adder = search_centre_adder;
				}
				else if(search_time_dr < search_time->Inf_State.Print_End_Time)
				{
					/* ������ʱ�������ʼʱ�� *///����д��Ϊ��ȥ���п��ܵ�С�����֡�
					search_start_adder = search_centre_adder;
				}
				else
				{
					/* �ɹ������ʼ��ַ */
					*end_adder = search_centre_adder;
					/* �ɹ� */
					return TRUE;
				}
			
			}else
			{
				/* ��ʼ��ַ�ͽ�����ַ���ڣ�û�и��Ӻ��ʵ���ʼ��ַ�ˡ�ѡ�����ʼַ��Ϊ������ʼ��ַ */
				*end_adder = search_start_adder;
				/* �ɹ� */
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*************************************************************************************************/
/* ������	: Print_Date														 	 			 */
/* ����		: date_p:��������ָ�롣														 		 */
/* ���		: �� 																				 */
/* ����		: ��ӡ����																	 	 	 */
/* ��������  : 2015/7/13																			 */
/*************************************************************************************************/	
static void	Print_Date(u8* date_p)
{

	/* ��ӡ���� */
	printf(Print_Date_Str,
	((DATETIME_Storage_Format*)date_p)->DATETIME_Bit.YEAR_BITS+2000,
	((DATETIME_Storage_Format*)date_p)->DATETIME_Bit.MONTH_BITS,
	((DATETIME_Storage_Format*)date_p)->DATETIME_Bit.DAY_BITS);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);

}

/*************************************************************************************************/
/* ������	: Print_Data_Body													 	 			 */
/* ����		: sensor_inf:������ͨ����Ϣ,start_adder:������ʼ��ַ;end_adder:���ݾ�����ַ��			 */
/* ���		: �� 																				 */
/* ����		: ������������ݣ����������ݣ�													 	 	 */
/* ��������  : 2015/7/9																			 */
/*************************************************************************************************/
static void Print_Data_Body(INSTRU_SENSOR* sensor_inf,PRINT_SENSOR_MAX_MIN_VALUE* print_sensor_max_min_value,u32 start_adder,u32 end_adder)
{
	
	/* һ��ʱ��������������������жϴ�ӡ�������Ƿ񳬹�һ�� */
#define Day_Second_Num			(86400uL)
	/* ʱ��Ĵ��� */
	Times time_b;
	/* ��ʱ��Ĵ��� */
	u32 second_time_b;
	/* ǰһ�δ�ӡʱ�� */
	u32 beforeprinttime = 0;
	/* ������ָ�� */
	u8* data_p;
	u8  Temp=0;
	/* �״δ�ӡ���ڱ�־ */
	BOOL print_date_flag = TRUE;
	/* ��¼�ϴδ�ӡ�������� */
	static u32 print_date_dr = 0;
	
#if (Print_Sensor_CH_Num >= 1)
	/* ͨ��1���� */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* ͨ��2���� */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ͨ��3���� */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* ͨ��4���� */
	float ch_4_data;
#endif
	
	
	/* ����ʱ�����Ŀ�� */
	printf(Print_Time_Bar_Str);
#if (Print_Sensor_CH_Num >= 1)
	/* ���ô�����1��Ŀ�� */
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
	printf(Print_Temp_Bar_Str,1);
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	printf(Print_Humi_Bar_Str,1);
	#endif	
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* ���ô�����2��Ŀ�� */
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
	printf(Print_Temp_Bar_Str,2);
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	printf(Print_Humi_Bar_Str,2);
	#endif	
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ���ô�����3��Ŀ�� */
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
	printf(Print_Temp_Bar_Str,3);
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	printf(Print_Humi_Bar_Str,3);
	#endif	
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* ���ô�����4��Ŀ�� */
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
	printf(Print_Temp_Bar_Str,4);
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	printf(Print_Humi_Bar_Str,4);
	#endif	
#endif
	/* �س� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
  
	
	/* ѭ����ȡ���������� */
	while(start_adder <= end_adder)
	{
		/* ��ȡ��ʼ��ַ���� */
		data_p = Flash_Read_Sensor_Data(start_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		/* ������һ�����ݵ�ַ */
		start_adder += Save_Sensor_Data_Len;
		/* ��ȡ ʱ������� */
#if (Print_Sensor_CH_Num >= 1)
	/* ��ȡͨ��1���� */
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#endif	
#endif
		
#if (Print_Sensor_CH_Num >= 2)
	/* ��ȡͨ��2���� */
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#endif		
#endif
		
#if (Print_Sensor_CH_Num >= 3)
	/* ��ȡͨ��3���� */
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#endif		
#endif

#if (Print_Sensor_CH_Num == 4)
	/* ��ȡͨ��4���� */
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
		ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#endif		
#endif
		
		/* ��ȡʱ������ */
		time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
		/* ��ʱ��ת������ */
		second_time_b = RTC_Date_Time_To_Second(&time_b);
		
		/* �ж�ͨ��״̬ */
		if(
#if (Print_Sensor_CH_Num >= 1)
			((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
		   &&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
		   ||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 2)
		   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
			&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
			||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 3)
		   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
			&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
			||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 4)
		   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
			&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
			||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif		
			)
		{
			/* ͨ������״̬ */
			/* �ж� ����ʱ���Ƿ�Ϊ ����ʱ��ı��� */
			if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl != 0)
			{
				/* �ж�ʱ�����Ƿ�С�ڱ���ʱ���� */
				if((second_time_b - beforeprinttime)<Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
				{
					
				/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((start_adder-Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((start_adder-Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/	
					
					
					/* ����ӡ�������� */
					/* ��ͷ��ʼִ�� */
					continue;
				}
			}
		}else
		{
			/* ͨ������״̬ */
			/* �ж� ����ʱ���Ƿ�Ϊ ����ʱ��ı��� */
			if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl != 0)
			{
				/* �ж�ʱ�����Ƿ�С�ڱ���ʱ���� */
				if((second_time_b - beforeprinttime)<Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
				{
					
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((start_adder-Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((start_adder-Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/		
					
					/* ����ӡ�������� */
					/* ��ͷ��ʼִ�� */
					continue;
				}
			}
		}
		
		/* ��ӡ��ȡ���������� */
		if((print_date_flag == TRUE)||(second_time_b / Day_Second_Num) != print_date_dr)
		{
			/* ֻ��2����������Ҫ��ӡ���ڣ�1����ӡ��һ������;2����ӡʱ����1������ */
			/* �ж� ��ӡ���ڱ�־ */
			if(print_date_flag == TRUE)
			{
				/* ������ڱ�־  */
				print_date_flag = FALSE;
				/* ������ֵ�Ĵ������� */
#if (Print_Sensor_CH_Num >= 1)
				/* ������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
				/* ������ֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
#endif	
	
#if (Print_Sensor_CH_Num >= 2)	
				/* ������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
				/* ������ֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
#endif	
	
#if (Print_Sensor_CH_Num >= 3)
				/* ������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
				/* ������ֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
#endif

#if (Print_Sensor_CH_Num >= 4)
				/* ������ֵ */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
				/* ������ֵ */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
#endif
			}
			/* �жϴ�ӡ���ڳ���һ�� */
			if((second_time_b / Day_Second_Num) != print_date_dr)
			{
				/* �����µĴ�ӡ���� */
				print_date_dr = (second_time_b / Day_Second_Num);
			}
			/* ��ӡ���� */
			Print_Date(data_p);
		}
		
	/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((start_adder-Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((start_adder-Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
	/*****************************************************************/			
		
		
		/* ��ӡ��ȡ��ʱ������ */
		printf(Print_Data_Body_Time_Str,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS);

		
#if (Print_Sensor_CH_Num == 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
		/* �ж������Ƿ񳬱� */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ��������� */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),	
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}		else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
			/* �ж������Ƿ񳬱� */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����1���� */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* �ж������Ƿ񳬱� */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����2���� */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}		else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_Hum))
		/* �ж������Ƿ񳬱� */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);
		}
		
		/* ��ӡ��ȡ�Ĵ�����1���� */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* �ж������Ƿ񳬱� */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����2���� */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
		
		/* �ж������Ƿ񳬱� */
		if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����3���� */
		printf(Print_Data_Body_Data_Str,
		((ch_3_data >= 0.0) ? ' ':'-'),
		((u16) ch_3_data),
		((u16)(ch_3_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch3_Max = ch_3_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch3_Min = ch_3_data;
		}
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_3_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_Hum))
		/* �ж������Ƿ񳬱� */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);
		}
		/* ��ӡ��ȡ�Ĵ�����1���� */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* �ж������Ƿ񳬱� */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����2���� */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
		
		/* �ж������Ƿ񳬱� */
		if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����3���� */
		printf(Print_Data_Body_Data_Str,
		((ch_3_data >= 0.0) ? ' ':'-'),
		((u16) ch_3_data),
		((u16)(ch_3_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch3_Max = ch_3_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch3_Min = ch_3_data;
		}
		/* �ж������Ƿ񳬱� */
		if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����4���� */
		printf(Print_Data_Body_Data_Str,
		((ch_4_data >= 0.0) ? ' ':'-'),
		((u16) ch_4_data),
		((u16)(ch_4_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch4_Max = ch_4_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch4_Min = ch_4_data;
		}
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{	
			/* �ж������Ƿ񳬱� */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_4_data >= 0.0) ? ' ':'-'),
			((u16) ch_4_data),
			((u16)(ch_4_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
			}
		}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_3_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{	
			/* �ж������Ƿ񳬱� */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_4_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_4_data >= 0.0) ? ' ':'-'),
			((u16) ch_4_data),
			((u16)(ch_4_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
			}
		}
	#endif
#endif
		
		/* ����δ����,���ý�ֹ�»���. */
		Print_Set_Underline_Config(DISABLE);
		/* ���� */
		Print_Set_CRLF();
		/* �ȴ������ӡ���� */
		OSTimeDly(120);
		/* ����ǰʱ������Ϊ��ȥ�Ĵ�ӡʱ�� */
		beforeprinttime = second_time_b;
	}
	
#undef Day_Second_Num
}

#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
/*************************************************************************************************/
/* ������	: Print_Picture													 	 			 	 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: ��ӡһ��ͼƬ����С:6*6�ֽ����ݣ�														 */
/* ��������  : 2015/7/10																			 */
/*************************************************************************************************/
static void Print_Picture(char *picture_data_p)
{
/* ��ӡͼƬ��X�����ش�С */
#define Print_Picture_Xpos_Size			(384u)
/* ��ӡͼƬ�����ݻ����С */
#define Print_Picture_Buffer_Size		(Print_Picture_Xpos_Size/8u)
/* һ��ѹ���ֽ���8�����ص����� */		
#define Picture_Compression_Ratio		(8u)	
/* ����һ��ͼƬ������Ҫ6�ֽڵĿռ� */
#define Picture_Data_Line_Size			(Print_Picture_Buffer_Size/Picture_Compression_Ratio)	
/* һ��ͼƬ��Ҫ����48�������� */
#define Print_Picture_Data_Line_Num		(Print_Picture_Data_Size/Picture_Data_Line_Size)
	
	/* ͼƬ���ݻ��� */
	char picture_data_buffer[Print_Picture_Buffer_Size];
	/* ������ */
	u8 num1,num2,num3,num4;

	
	/* ����ͼƬ���� */
	for(num4 = 0;num4 < Print_Picture_Data_Line_Num; num4++)
	{
		/* �������з������� */
		for(num3 = 0;num3 < Picture_Data_Line_Size; num3++)
		{
			/* ��ԭʼ��ѹ�����ݽ�ѹ�����ݻ����� */
			for(num2 = 0;num2 < Picture_Compression_Ratio; num2++)
			{
				/* ��ȡ���� */
				if(((*(picture_data_p+num3+num4*Picture_Data_Line_Size) << num2)&0x80) == 0x80)
				{
					picture_data_buffer[num2+num3*Picture_Compression_Ratio] = 0xFF;
				}else
				{
					picture_data_buffer[num2+num3*Picture_Compression_Ratio] = 0x00;
				}
				
			}	
		}
		
		/* ÿһ�������з���8�� */
		for(num1 = 0;num1 < Picture_Compression_Ratio; num1++)
		{
			/* ��ӡһ��ͼƬ���� */
			Print_Set_Picture(Print_Picture_Buffer_Size,picture_data_buffer);
		}
	}

/* ��ӡͼƬ��X�����ش�С */
#undef Print_Picture_Xpos_Size
/* ��ӡͼƬ�����ݻ����С */
#undef Print_Picture_Buffer_Size
/* һ��ѹ���ֽ���8�����ص����� */		
#undef Picture_Compression_Ratio	
/* ����һ��ͼƬ������Ҫ6�ֽڵĿռ� */
#undef Picture_Data_Line_Size
/* һ��ͼƬ��Ҫ����48�������� */
#undef Print_Picture_Data_Line_Num
}
#endif

/*************************************************************************************************/
/* ������	: Print_Data_Tail													 	 			 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: �������β����																 	 	 */
/* ��������  : 2015/7/10																			 */
/*************************************************************************************************/
static void Print_Data_Tail(PRINT_SENSOR_MAX_MIN_VALUE* print_sensor_max_min_value)
{

	/* 1����2��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	
	/* 2����ӡ�������/Сֵ. */
	/* ��ӡ�������ֵ */
	printf(Print_Sensor_Data_Max_Value_Str);
	/* ��ӡ��ȡ�Ĵ��������� */
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��1���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch1_Max),
	((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch1_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Max),
		((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch1_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Max),
		((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��2���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch2_Max),
	((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch2_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Max),
		((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch2_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Max),
		((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��3���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch3_Max),
	((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch3_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Max),
		((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}	
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch3_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Max),
		((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||(Sensor_CH4_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��4���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch4_Max),
	((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch4_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Max),
		((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch4_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Max),
		((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	/* �س� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
	/* ��ӡ������Сֵ. */
	printf(Print_Sensor_Data_Min_Value_Str);
	/* ��ӡ��ȡ�Ĵ��������� */
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��1���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch1_Min),
	((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch1_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Min),
		((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch1_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Min),
		((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��2���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch2_Min),
	((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch2_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Min),
		((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch2_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Min),
		((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��3���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch3_Min),
	((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch3_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Min),
		((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}	
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch3_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Min),
		((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��4���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch4_Min),
	((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch4_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Min),
		((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch4_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Min),
		((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);
	}
	#endif
#endif
	/* �س� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(200);	
	
	/* ��1��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);

	
#if (Print_Scheme == 0)
/* Ĭ�Ϸ��� */
	
	/* 3����ӡ�ͻ�������. */
	/* ���������»���. */
	Print_Set_Underline_Config(ENABLE);
	/* ��ӡ�ͻ�������. */
	printf(Print_Delivery_Party_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1�� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	/* ��ӡ�ջ�������. */
	printf(Print_Receiver_Party_Str);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	
	
	/* 4����ӡ ���ƺ�. */	
	printf(Print_Card_NO_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
#elif (Print_Scheme == 1)
///* �����ŷ��� */

//	/* 3����ӡ �����¶�. */	
//	/* ���������»���. */
//	Print_Set_Underline_Config(ENABLE);
//	
//	/* ��ӡ ����ʱ�� */
//	//
//	printf(Print_StartOffTime_Str,
//		   StartOffTemp.Time.Year,
//		   StartOffTemp.Time.Month,
//		   StartOffTemp.Time.Day,
//		   StartOffTemp.Time.Hour,
//		   StartOffTemp.Time.Min);
//	/* �ȴ���ӡ���� */
//	OSTimeDly(120);
//	/* ��ӡ �����¶�. */
//	printf(Print_StartOffTemp_Str);
//	
//#if (Print_Sensor_CH_Num >= 1)
//	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||(Sensor_CH1_Config_Type == Sensor_Hum))
//	/* ��ӡ������ͨ��1���ֵ */
//	printf(Print_Data_Body_Data_Str,
//	(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
//	((u16) StartOffTemp.Ch1_Value),
//	((u16)(StartOffTemp.Ch1_Value*10)%10));
//	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
//	if(StartOffTemp.Ch1_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
//		((u16) StartOffTemp.Ch1_Value),
//		((u16)(StartOffTemp.Ch1_Value*10)%10));
//	}else
//	{
//		/* ��ʱ�������� ��ӡȱʧ */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}
//	#endif
//#endif
//	
//#if (Print_Sensor_CH_Num >= 2)
//	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
//	/* ��ӡ������ͨ��2���ֵ */
//	printf(Print_Data_Body_Data_Str,
//	(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
//	((u16) StartOffTemp.Ch2_Value),
//	((u16)(StartOffTemp.Ch2_Value*10)%10));
//	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
//	if(StartOffTemp.Ch2_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
//		((u16) StartOffTemp.Ch2_Value),
//		((u16)(StartOffTemp.Ch2_Value*10)%10));
//	}else
//	{
//		/* ��ʱ�������� ��ӡȱʧ */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}
//	#endif
//	/* ��1�� */
//	Print_Set_CRLF();
//	/* �ȴ������ӡ���� */
//	OSTimeDly(120);
//#endif
//	
//#if (Print_Sensor_CH_Num >= 3)
//	/* ��ӡ �����¶�. */
//	printf(Print_StartOffEnvironmentTemp_Bar_Str);
//	
//	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
//	/* ��ӡ������ͨ��3���ֵ */
//	printf(Print_Data_Body_Data_Str,
//	(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
//	((u16) StartOffTemp.Ch3_Value),
//	((u16)(StartOffTemp.Ch3_Value*10)%10));
//	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
//	if(StartOffTemp.Ch3_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
//		((u16) StartOffTemp.Ch3_Value),
//		((u16)(StartOffTemp.Ch3_Value*10)%10));
//	}else
//	{
//		/* ��ʱ�������� ��ӡȱʧ */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}	
//	#endif
//#endif
//	/* �ȴ������ӡ���� */
//	OSTimeDly(200);
//	/* ��1�� */
//	Print_Set_CRLF();
//	/* �ȴ������ӡ���� */
//	OSTimeDly(120);
//	Print_Set_CRLF();
//	/* �ȴ������ӡ���� */
//	OSTimeDly(120);
//	
//	/* 4����ӡ �����¶�. */
//	
//	/* ��ӡ ����ʱ�� */
//	//
//	printf(Print_ArriveTime_Str,
//		   ArriveTemp.Time.Year,
//		   ArriveTemp.Time.Month,
//		   ArriveTemp.Time.Day,
//		   ArriveTemp.Time.Hour,
//		   ArriveTemp.Time.Min);
//	/* �ȴ���ӡ���� */
//	OSTimeDly(120);
//	/* ��ӡ �����¶�. */
//	printf(Print_ArriveTemp_Str);
//	
//#if (Print_Sensor_CH_Num >= 1)
//	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||(Sensor_CH1_Config_Type == Sensor_Hum))
//	/* ��ӡ������ͨ��1���ֵ */
//	printf(Print_Data_Body_Data_Str,
//	(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
//	((u16) ArriveTemp.Ch1_Value),
//	((u16)(ArriveTemp.Ch1_Value*10)%10));
//	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
//	if(ArriveTemp.Ch1_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
//		((u16) ArriveTemp.Ch1_Value),
//		((u16)(ArriveTemp.Ch1_Value*10)%10));
//	}else
//	{
//		/* ��ʱ�������� ��ӡȱʧ */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}
//	#endif
//#endif
//	
//#if (Print_Sensor_CH_Num >= 2)
//	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
//	/* ��ӡ������ͨ��2���ֵ */
//	printf(Print_Data_Body_Data_Str,
//	(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
//	((u16) ArriveTemp.Ch2_Value),
//	((u16)(ArriveTemp.Ch2_Value*10)%10));
//	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
//	if(ArriveTemp.Ch2_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
//		((u16) ArriveTemp.Ch2_Value),
//		((u16)(ArriveTemp.Ch2_Value*10)%10));
//	}else
//	{
//		/* ��ʱ�������� ��ӡȱʧ */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}
//	#endif
//	/* ��1�� */
//	Print_Set_CRLF();
//	/* �ȴ������ӡ���� */
//	OSTimeDly(120);
//#endif
//	
//#if (Print_Sensor_CH_Num >= 3)
//	/* ��ӡ �����¶�. */
//	printf(Print_ArriveEnvironmentTemp_Bar_Str);
//	
//	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
//	/* ��ӡ������ͨ��3���ֵ */
//	printf(Print_Data_Body_Data_Str,
//	(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
//	((u16) ArriveTemp.Ch3_Value),
//	((u16)(ArriveTemp.Ch3_Value*10)%10));
//	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
//	if(ArriveTemp.Ch3_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
//		((u16) ArriveTemp.Ch3_Value),
//		((u16)(ArriveTemp.Ch3_Value*10)%10));
//	}else
//	{
//		/* ��ʱ�������� ��ӡȱʧ */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}	
//	#endif
//#endif
//	/* �ȴ������ӡ���� */
//	OSTimeDly(200);
//	/* ��1�� */
//	Print_Set_CRLF();
//	/* �ȴ������ӡ���� */
//	OSTimeDly(120);
//	Print_Set_CRLF();
//	/* �ȴ������ӡ���� */
//	OSTimeDly(120);
	
//	/* 5����ӡ ���ݱ��. */	
//	printf(Print_FBillNo_Str);
//	/* �ȴ������ӡ���� */
//	OSTimeDly(200);
//	/* ��1��. */	
//	Print_Set_CRLF();
//	/* �ȴ������ӡ���� */
//	OSTimeDly(120);

	//������GPS��Ϣ������Ϣ���ж��Ƿ�Ϊ�����豸
#if (GPS_Config_Enable == 0x01)
	/* 4����ӡ ���ƺ�. */	
	printf(Print_Card_NO_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
#endif

#endif

#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
	/* 5����ӡ��˾LOGO��ά��. */
	Print_Picture((char*)Print_LOGO);
#endif

	/* 6�����ý�ֹ�»���. */
	Print_Set_Underline_Config(DISABLE);
	/* ��ӡ�ָ�� */
	Print_Indivisible_Symbol();
	
}


/*************************************************************************************************/
/* ������	: Print_Data_Output													 	 			 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: �����ӡ����																 	 	 */
/* ��������  : 2015/7/6																			 */
/*************************************************************************************************/
void Print_Data_Output(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf)
{
	/* ��ӡ����ʼ��ַ */
	u32 print_start_adder = 0;
	/* ��ӡ�Ľ�����ַ */
	u32 print_end_adder = 0;
	/* ��ӡ������ͨ������ֵ */
	PRINT_SENSOR_MAX_MIN_VALUE print_sensor_max_min_value;
	/* ��ѯ��� */
	BOOL search_result;
	
	
	/* 1�����õĴ�ӡʱ���Ƿ��ڼ�¼��ʱ�䷶Χ�ڡ� */
	if((print_inf->Inf_State.Print_Start_Time 		<= print_inf->Inf_State.Print_Record_End_Time)&&
	   (print_inf->Inf_State.Print_End_Time 		>= print_inf->Inf_State.Print_Record_Start_Time)&&
	   (print_inf->Inf_State.Print_Start_Time 		<= print_inf->Inf_State.Print_End_Time)&&
	   (print_inf->Inf_State.Print_Record_Start_Time<= print_inf->Inf_State.Print_Record_End_Time))
	{
		/* �����õ�ʱ�䷶Χ���м�¼�Ĵ��������� */
		
		//1��
		/* ���ô�ӡ���� */
		Print_Set_Para();
		
		//2��
		/* ������ӡ���ݵ���ʼλ��. */
		search_result = Print_Search_Start_Adder(print_inf,&print_start_adder);
		/* �ж�������ʼ���ݵ�ַ�Ƿ�ɹ� */
		if(search_result == FALSE)
		{
			/* ��ʼ���ݵ�ַ����ʧ�� */
			/* ��ӡ�ָ�� */
			Print_Indivisible_Symbol();
			/* �����С������ */
			Print_Set_Font_Double(RESET);
			/* ����Ϊ16*16��С������ */
			Print_Set_Font(Print_Font_16_16);
			/* ������Ϣ1:������ʼʱ����ڼ�¼�Ľ���ʱ�䡣 */
			printf(Print_Search_Start_Time_Than_Record_End_Time);
			OSTimeDly(200);
			/* ��ӡ�ָ�� */
			Print_Indivisible_Symbol();
			return;
		}
		//3��
		/* ������ӡ���ݵĽ���λ��. */
		search_result = Print_Search_End_Adder(print_inf,&print_end_adder);
		/* �ж������������ݵ�ַ�Ƿ�ɹ� */
		if(search_result == FALSE)
		{
			/* �������ݵ�ַ����ʧ�� */
			/* ��ӡ�ָ�� */
			Print_Indivisible_Symbol();
			/* �����С������ */
			Print_Set_Font_Double(RESET);
			/* ����Ϊ16*16��С������ */
			Print_Set_Font(Print_Font_16_16);
			/* ������Ϣ2:��������ʱ��С�ڼ�¼����ʼʱ�䡣 */
			printf(Print_Search_End_Time_Than_Record_Start_Time);
			OSTimeDly(200);
			/* ��ӡ�ָ�� */
			Print_Indivisible_Symbol();
			return;
		}
		
		//4��
		/* �ж���������ʼ��ַ�ͽ�����ַ�Ƿ�Ϸ�����ʼ��ַ С�ڵ��� ������ַ�� */
		if(print_start_adder > print_end_adder)
		{
			/* ��ʼ��ַ ���� ������ַ */
			/* ��ӡ�ָ�� */
			Print_Indivisible_Symbol();
			/* �����С������ */
			Print_Set_Font_Double(RESET);
			/* ����Ϊ16*16��С������ */
			Print_Set_Font(Print_Font_16_16);
			/* ������Ϣ2:��������ʱ��С�ڼ�¼����ʼʱ�䡣 */
			printf(Print_Search_Start_Time_Than_Search_End_Time);
			OSTimeDly(200);
			/* ��ӡ�ָ�� */
			Print_Indivisible_Symbol();
			return;
		}
		
		/* ���ݵ�ַ�����ɹ� */
		//5��
		/* �������ͷ���� */
		Print_Data_Title(sensor_inf,print_inf,print_start_adder,print_end_adder);
		
		//6��
		/* ������������ݣ����������ݣ� */
		Print_Data_Body(sensor_inf,&print_sensor_max_min_value,print_start_adder,print_end_adder);
		
		//7��
		/* �������β���� */
		Print_Data_Tail(&print_sensor_max_min_value);
		
	}else
	{		
		/* ���ô�ӡ���� */
		Print_Set_Para();
		/* ��ӡ�ָ�� */
		Print_Indivisible_Symbol();
		/* �����С������ */
		Print_Set_Font_Double(RESET);
		/* ����Ϊ16*16��С������ */
		Print_Set_Font(Print_Font_16_16);
		/* ������Ϣ1:������ʼʱ����ڼ�¼�Ľ���ʱ�䡣 */
		printf(Print_Set_Time_Range_Null_Data_Str);
		OSTimeDly(200);
		/* ��ӡ�ָ�� */
		Print_Indivisible_Symbol();
	}

}
/************************************************************************************************/

/************************************************************************************************/
#if (1)/* һ����ӡ���� */


/* ��ʼһ����ӡ�İ���ʱ�䳤�� */
#define StartEasyPrintTime		(4000u)		//4S
#define StopEasyPrintTime		(4000u)		//4S
#define EasyPrintOutTime		(20000u)	//20S


#ifdef Hardware_Version_V131

#define StartKeyPORT           GPIOC
#define StartKeyCLK            RCC_APB2Periph_GPIOC
#define StartKeyPIN            GPIO_Pin_10

#define StopKeyPORT            GPIOC
#define StopKeyCLK             RCC_APB2Periph_GPIOC 
#define StopKeyPIN             GPIO_Pin_11

#else

#define StartKeyPORT           GPIOC
#define StartKeyCLK            RCC_APB2Periph_GPIOC
#define StartKeyPIN            GPIO_Pin_7

#define StopKeyPORT            GPIOC
#define StopKeyCLK             RCC_APB2Periph_GPIOC 
#define StopKeyPIN             GPIO_Pin_8

#endif

static uint8_t IsKeyStartEasyPrint(void) 	{if (GPIO_ReadInputDataBit(StartKeyPORT,StartKeyPIN) == Bit_SET) return 1; return 0;}
static uint8_t IsKeyStopEasyPrint (void) 	{if (GPIO_ReadInputDataBit(StopKeyPORT,StopKeyPIN) == Bit_SET) return 1; return 0;}


/* ������ӡʱ��Ĵ��� */
static u32 EasyPrintContinuousReg = 0;

/* ��ӡ����״̬ */
static FlagStatus EasyPrintDataState = RESET;
#if (Print_Scheme == 1)
/*************************************************************************************************/
/* ������	: Easy_Print_Search_Arrive_Time														 */
/* ����		: ��																		 		 */
/* ���		: ��		 																		 */
/* ����		: ��������ʱ�估����															 	 	 */
/* ��������  : 2016/05/10																		 */
/*************************************************************************************************/
static void Easy_Print_Search_Arrive_Time(INSTRU_SENSOR* sensor_inf,Print_JWX_OutInTemp_Struct * Arrive_p,int start_adder,int end_adder)
{
	/* ������ָ�� */
	u8* data_p;
	
	
#if (Print_Sensor_CH_Num >= 1)
	/* ͨ��1���� */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* ͨ��2���� */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ͨ��3���� */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* ͨ��4���� */
	float ch_4_data;
#endif
	
		//����Ӧ���жϴ�ӡ�����ݳ��ȣ������������ڴ�ӡ���ݽṹ��û�����������
		//��ʱ���ô�ӡ��ʼ��ַ�ʹ�ӡ������ַ�����жϣ���������һ��©�����ڣ���
		/* �жϴ�ӡ��ʼ��ַ�ʹ�ӡ������ַ */
		if(end_adder < 0)
		{
			/* �����ݴ�ӡ��0����ִ�ʱ�䡣 */
			return;
		}
		
		/* ��ȡ��ʼ��ַ���� */
		data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		
			/* ��ȡ ʱ������� */
	#if (Print_Sensor_CH_Num >= 1)
		/* ��ȡͨ��1���� */
		#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
			ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
		#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
			   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
			ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
		#endif	
	#endif
			
	#if (Print_Sensor_CH_Num >= 2)
		/* ��ȡͨ��2���� */
		#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
			ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
		#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
			   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
			ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
		#endif		
	#endif
			
	#if (Print_Sensor_CH_Num >= 3)
		/* ��ȡͨ��3���� */
		#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
			ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
		#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
			   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
			ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
		#endif		
	#endif

	#if (Print_Sensor_CH_Num == 4)
		/* ��ȡͨ��4���� */
		#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
			ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
		#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
			   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
			ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
		#endif		
	#endif

	/* �жϵ�ǰ����״̬ */

	/* ��ǰ������Ϊ����ʱ�䡣 */
	#if (Print_Sensor_CH_Num >= 1)
			/* ����1 */
			Arrive_p->Ch1_Value = ch_1_data;
	#endif
	#if (Print_Sensor_CH_Num >= 2)
			/* ����2 */
			Arrive_p->Ch2_Value = ch_2_data;
	#endif
	#if (Print_Sensor_CH_Num >= 3)
			/* ����3 */
			Arrive_p->Ch3_Value = ch_3_data;
	#endif
	#if (Print_Sensor_CH_Num >= 4)
			/* ����4 */
			Arrive_p->Ch4_Value = ch_4_data;
	#endif
	/* ���ó���ʱ�� */
	Arrive_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
	Arrive_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
	Arrive_p->Time.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
	Arrive_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
	Arrive_p->Time.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
	Arrive_p->Time.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
}

/*************************************************************************************************/
/* ������	: Easy_Print_Search_StartOff_Time													 */
/* ����		: ��																		 		 */
/* ���		: ��		 																		 */
/* ����		: ��������ʱ�估����															 	 	 */
/* ��������  : 2016/05/10																		 */
/*************************************************************************************************/
static void Easy_Print_Search_StartOff_Time(INSTRU_SENSOR* sensor_inf,Print_JWX_OutInTemp_Struct * StartOff_p,int start_adder,int end_adder)
{
	/* ������ָ�� */
	u8* data_p;
	/* ʱ��Ĵ��� */
	Times time_b;
	/* ��ʱ��Ĵ��� */
	u32 second_time_b;
	/* ǰһ�����ݵ�ʱ�� */
	u32 before_time;
	/* ԭʼ������ַ */
	u32 o_EndAdder = end_adder;
	/* ��������Ĵ��� */
	Print_JWX_OutInTemp_Struct startoff;
	/* ����״̬ */
	BOOL dataState;
	
	
#if (Print_Sensor_CH_Num >= 1)
	/* ͨ��1���� */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* ͨ��2���� */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ͨ��3���� */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* ͨ��4���� */
	float ch_4_data;
#endif

	/* �жϼ�������ָ��״̬ */
	if(start_adder >= end_adder)
	{
		/* �޴洢���� */
		return;
	}

	/* ��ȡ��ʼ��ַ���� */
	data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);

	/* ��ȡʱ������ */
	time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
	time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
	time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
	time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
	time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
	time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;

	/* ���ó���ʱ�� */
	
	startoff.Time.Year 	= time_b.Year;
	startoff.Time.Month = time_b.Month;
	startoff.Time.Day 	= time_b.Day;
	startoff.Time.Hour 	= time_b.Hour;
	startoff.Time.Min 	= time_b.Min;
	startoff.Time.Sec 	= time_b.Sec;
	
	/* �������� */
#if (Print_Sensor_CH_Num >= 1)
	/* ��ȡͨ��1���� */
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
		startoff.Ch1_Value = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		startoff.Ch1_Value = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#endif	
#endif
		
#if (Print_Sensor_CH_Num >= 2)
	/* ��ȡͨ��2���� */
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
		startoff.Ch2_Value = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		startoff.Ch2_Value = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#endif		
#endif
		
#if (Print_Sensor_CH_Num >= 3)
	/* ��ȡͨ��3���� */
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
		startoff.Ch3_Value = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		startoff.Ch3_Value = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#endif		
#endif

#if (Print_Sensor_CH_Num == 4)
	/* ��ȡͨ��4���� */
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
		startoff.Ch4_Value = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
		startoff.Ch4_Value = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#endif		
#endif

	/* ���õ�ǰ�������� */
	/* �ж�����״̬ */
	if(
#if (Print_Sensor_CH_Num >= 1)
		((startoff.Ch1_Value <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
		&&(startoff.Ch1_Value >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
		||(startoff.Ch1_Value == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 2)
		&&((startoff.Ch2_Value <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
		&&(startoff.Ch2_Value >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
		||(startoff.Ch2_Value == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 3)
		&&((startoff.Ch3_Value <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
		&&(startoff.Ch3_Value >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
		||(startoff.Ch3_Value == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 4)
		&&((startoff.Ch4_Value <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
		&&(startoff.Ch4_Value >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
		||(startoff.Ch4_Value == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif		
	)
	{
		/* �������� */
		dataState = TRUE;
	}else
	{
		/* ���� */
		dataState = FALSE;
	}
		

	/* ����ǰһ�����ݵ�ʱ��Ϊ��һ������ʱ�� */
	before_time = RTC_Date_Time_To_Second(&time_b);
	
	while(start_adder <= end_adder)
	{
		/* ��ȡ��ʼ��ַ���� */
		data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		/* ��ȡ ʱ������� */
#if (Print_Sensor_CH_Num >= 1)
	/* ��ȡͨ��1���� */
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#endif	
#endif
		
#if (Print_Sensor_CH_Num >= 2)
	/* ��ȡͨ��2���� */
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#endif		
#endif
		
#if (Print_Sensor_CH_Num >= 3)
	/* ��ȡͨ��3���� */
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#endif		
#endif

#if (Print_Sensor_CH_Num == 4)
	/* ��ȡͨ��4���� */
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
		ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#endif		
#endif

		/* ��ȡʱ������ */
		time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
		
		/* ��ʱ��ת������ */
		second_time_b = RTC_Date_Time_To_Second(&time_b);

/* �ж�ͨ��״̬ */
		if(
#if (Print_Sensor_CH_Num >= 1)
			((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
		   &&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
		   ||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 2)
		   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
			&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
			||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 3)
		   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
			&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
			||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 4)
		   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
			&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
			||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif		
			)
		{
			/* �������� */
			/* ǰһ������״̬ */
			if(dataState == TRUE)
			{
				/* ǰһ������״̬:���� */
				/* �ж������Ƿ����� */
				if((before_time > second_time_b)&&
				   (before_time - second_time_b > Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl))
					{
						/* ���ݲ����� */
						/* ��ǰһ�鵱��������Ϊ����ʱ��/�¶ȡ� */
						*StartOff_p = startoff;
						return;
					}
			}else
			{
				/* ǰһ������״̬:���� */
				/* �ж������Ƿ����� */
				if(before_time > second_time_b)
				{
					/* �ж�����/������¼ʱ���С */
					if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl > 
					   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
					{
						/* ������¼ʱ�䳤 */
						/* �ж������Ƿ����� */
						if((before_time - second_time_b) > 
						   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
						{
							/* ���ݲ����� */
							/* ��ǰ�㲻�Ǳ����ȼ��ʱ��㣬��ǰ��������ʱ��㡣 */
							while(end_adder < o_EndAdder)
							{
								/* ����ǰһ�����ݵ�ַ */
								end_adder += Save_Sensor_Data_Len;					
								/* ��ȡ��ʼ��ַ���� */
								data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
								
				/* ��ȡ ʱ������� */
		#if (Print_Sensor_CH_Num >= 1)
			/* ��ȡͨ��1���� */
			#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
				 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
				 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
				 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
			#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
				   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
				   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
								ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
			#endif	
		#endif
				
		#if (Print_Sensor_CH_Num >= 2)
			/* ��ȡͨ��2���� */
			#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
				 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
				 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
				 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
			#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
				   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
				   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
								ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
			#endif		
		#endif
				
		#if (Print_Sensor_CH_Num >= 3)
			/* ��ȡͨ��3���� */
			#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
				 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
				 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
				 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
			#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
				   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
				   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
								ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
			#endif		
		#endif

		#if (Print_Sensor_CH_Num == 4)
			/* ��ȡͨ��4���� */
			#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
				 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
				 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
				 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
			#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
				   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
				   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
								ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
			#endif		
		#endif

		/* �ж�ͨ��״̬ */
								if(
		#if (Print_Sensor_CH_Num >= 1)
									((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
									&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
									||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 2)
								   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
									&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
									||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 3)
								   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
									&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
									||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 4)
								   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
									&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
									||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif		
									)
								{
									__nop();
								}else
								{
									/* ��ȡʱ������ */
									time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
									time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
									time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
									time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
									time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
									time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
									
									/* ��ʱ��ת������ */
									second_time_b = RTC_Date_Time_To_Second(&time_b);
									/* �ж� �жϼ�¼�������Ƿ����� */
									if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl == 0)
									{
		#if (Print_Sensor_CH_Num >= 1)
										/* ����1 */
										StartOff_p->Ch1_Value = ch_1_data;
		#endif
		#if (Print_Sensor_CH_Num >= 2)
										/* ����2 */
										StartOff_p->Ch2_Value = ch_2_data;
		#endif
		#if (Print_Sensor_CH_Num >= 3)
										/* ����3 */
										StartOff_p->Ch3_Value = ch_3_data;
		#endif
		#if (Print_Sensor_CH_Num >= 4)
										/* ����4 */
										StartOff_p->Ch4_Value = ch_4_data;
		#endif
										/* ���ó���ʱ�� */
										StartOff_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
										StartOff_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
										StartOff_p->Time.Day 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
										StartOff_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
										StartOff_p->Time.Min 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
										StartOff_p->Time.Sec 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
										return;			
									}		
								}
							}
							return;		
						}
					
					}else
					{
						/* ������¼ʱ�䳤 */
						if((before_time - second_time_b) > 
						    Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
						{
							/* ���ݲ����� */
							/* ��ǰ�㲻�Ǳ����ȼ��ʱ��㣬��ǰ��������ʱ��㡣 */
							while(end_adder < o_EndAdder)
							{
								/* ����ǰһ�����ݵ�ַ */
								end_adder += Save_Sensor_Data_Len;					
								/* ��ȡ��ʼ��ַ���� */
								data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
								
				/* ��ȡ ʱ������� */
		#if (Print_Sensor_CH_Num >= 1)
			/* ��ȡͨ��1���� */
			#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
				 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
				 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
				 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
			#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
				   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
				   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
								ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
			#endif	
		#endif
				
		#if (Print_Sensor_CH_Num >= 2)
			/* ��ȡͨ��2���� */
			#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
				 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
				 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
				 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
			#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
				   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
				   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
								ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
			#endif		
		#endif
				
		#if (Print_Sensor_CH_Num >= 3)
			/* ��ȡͨ��3���� */
			#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
				 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
				 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
				 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
			#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
				   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
				   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
								ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
			#endif		
		#endif

		#if (Print_Sensor_CH_Num == 4)
			/* ��ȡͨ��4���� */
			#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
				 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
				 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
				 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
			#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
				   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
				   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
								ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
			#endif		
		#endif

		/* �ж�ͨ��״̬ */
								if(
		#if (Print_Sensor_CH_Num >= 1)
									((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
									&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
									||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 2)
								   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
									&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
									||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 3)
								   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
									&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
									||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 4)
								   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
									&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
									||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif		
									)
								{
									__nop();
								}else
								{
									/* ��ȡʱ������ */
									time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
									time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
									time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
									time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
									time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
									time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
									
									/* ��ʱ��ת������ */
									second_time_b = RTC_Date_Time_To_Second(&time_b);
									/* �ж� �жϼ�¼�������Ƿ����� */
									if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl == 0)
									{
		#if (Print_Sensor_CH_Num >= 1)
										/* ����1 */
										StartOff_p->Ch1_Value = ch_1_data;
		#endif
		#if (Print_Sensor_CH_Num >= 2)
										/* ����2 */
										StartOff_p->Ch2_Value = ch_2_data;
		#endif
		#if (Print_Sensor_CH_Num >= 3)
										/* ����3 */
										StartOff_p->Ch3_Value = ch_3_data;
		#endif
		#if (Print_Sensor_CH_Num >= 4)
										/* ����4 */
										StartOff_p->Ch4_Value = ch_4_data;
		#endif
										/* ���ó���ʱ�� */
										StartOff_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
										StartOff_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
										StartOff_p->Time.Day 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
										StartOff_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
										StartOff_p->Time.Min 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
										StartOff_p->Time.Sec 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
										return;			
									}		
								}
							}
							return;	
						}
						
					}
					
				}
			}
		}else
		{
			/* ���ݳ��� */
			/* ǰһ������״̬ */
			if(dataState == TRUE)
			{
				/* ǰһ������״̬:���� */
				/* �ж������Ƿ����� */
				if(before_time > second_time_b)
				{
					/* �ж�����/������¼ʱ���С */
					if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl > 
					   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
					{
						/* ������¼ʱ�䳤 */
						/* �ж������Ƿ����� */
						if((before_time - second_time_b) > 
						   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
						{
							/* ���ݲ����� */
							/* ��ǰ�㲻�Ǳ����ȼ��ʱ��㣬��ǰ��������ʱ��㡣 */
							while(end_adder < o_EndAdder)
							{
								/* ����ǰһ�����ݵ�ַ */
								end_adder += Save_Sensor_Data_Len;					
								/* ��ȡ��ʼ��ַ���� */
								data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
								
/* ��ȡ ʱ������� */
#if (Print_Sensor_CH_Num >= 1)
	/* ��ȡͨ��1���� */
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
								ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#endif	
#endif
				
#if (Print_Sensor_CH_Num >= 2)
	/* ��ȡͨ��2���� */
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
								ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#endif		
#endif
				
#if (Print_Sensor_CH_Num >= 3)
	/* ��ȡͨ��3���� */
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
								ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#endif		
#endif

#if (Print_Sensor_CH_Num == 4)
	/* ��ȡͨ��4���� */
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
								ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#endif		
#endif

		/* �ж�ͨ��״̬ */
								if(
		#if (Print_Sensor_CH_Num >= 1)
									((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
									&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
									||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 2)
								   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
									&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
									||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 3)
								   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
									&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
									||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 4)
								   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
									&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
									||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif		
									)
								{
									/* ��ȡʱ������ */
									time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
									time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
									time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
									time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
									time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
									time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
									
									/* ��ʱ��ת������ */
									second_time_b = RTC_Date_Time_To_Second(&time_b);
									/* �ж� �жϼ�¼�������Ƿ����� */
									if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl == 0)
									{
		#if (Print_Sensor_CH_Num >= 1)
										/* ����1 */
										StartOff_p->Ch1_Value = ch_1_data;
		#endif
		#if (Print_Sensor_CH_Num >= 2)
										/* ����2 */
										StartOff_p->Ch2_Value = ch_2_data;
		#endif
		#if (Print_Sensor_CH_Num >= 3)
										/* ����3 */
										StartOff_p->Ch3_Value = ch_3_data;
		#endif
		#if (Print_Sensor_CH_Num >= 4)
										/* ����4 */
										StartOff_p->Ch4_Value = ch_4_data;
		#endif
										/* ���ó���ʱ�� */
										StartOff_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
										StartOff_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
										StartOff_p->Time.Day 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
										StartOff_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
										StartOff_p->Time.Min 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
										StartOff_p->Time.Sec 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
										return;			
									}
								}else
								{
									__nop();
								}
							}
							return;
						}
					}else
					{
						/* ������¼ʱ�䳤 */
						if((before_time - second_time_b) > 
						    Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
						{
							/* ���ݲ����� */
							/* ��ǰ�㲻�Ǳ����ȼ��ʱ��㣬��ǰ��������ʱ��㡣 */
							while(end_adder < o_EndAdder)
							{
								/* ����ǰһ�����ݵ�ַ */
								end_adder += Save_Sensor_Data_Len;					
								/* ��ȡ��ʼ��ַ���� */
								data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
								
/* ��ȡ ʱ������� */
#if (Print_Sensor_CH_Num >= 1)
	/* ��ȡͨ��1���� */
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
								ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#endif	
#endif
				
#if (Print_Sensor_CH_Num >= 2)
	/* ��ȡͨ��2���� */
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
								ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#endif		
#endif
				
#if (Print_Sensor_CH_Num >= 3)
	/* ��ȡͨ��3���� */
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
								ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#endif		
#endif

#if (Print_Sensor_CH_Num == 4)
	/* ��ȡͨ��4���� */
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
								ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
								ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#endif		
#endif

		/* �ж�ͨ��״̬ */
								if(
		#if (Print_Sensor_CH_Num >= 1)
									((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
									&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
									||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 2)
								   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
									&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
									||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 3)
								   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
									&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
									||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif
		#if (Print_Sensor_CH_Num >= 4)
								   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
									&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
									||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
		#endif		
									)
								{
									/* ��ȡʱ������ */
									time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
									time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
									time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
									time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
									time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
									time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
									
									/* ��ʱ��ת������ */
									second_time_b = RTC_Date_Time_To_Second(&time_b);
									/* �ж� �жϼ�¼�������Ƿ����� */
									if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl == 0)
									{
		#if (Print_Sensor_CH_Num >= 1)
										/* ����1 */
										StartOff_p->Ch1_Value = ch_1_data;
		#endif
		#if (Print_Sensor_CH_Num >= 2)
										/* ����2 */
										StartOff_p->Ch2_Value = ch_2_data;
		#endif
		#if (Print_Sensor_CH_Num >= 3)
										/* ����3 */
										StartOff_p->Ch3_Value = ch_3_data;
		#endif
		#if (Print_Sensor_CH_Num >= 4)
										/* ����4 */
										StartOff_p->Ch4_Value = ch_4_data;
		#endif
										/* ���ó���ʱ�� */
										StartOff_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
										StartOff_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
										StartOff_p->Time.Day 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
										StartOff_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
										StartOff_p->Time.Min 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
										StartOff_p->Time.Sec 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
										return;			
									}
								}else
								{
									__nop();
								}
							}
							return;
						}
					}
				}
			}else
			{
				/* ǰһ������״̬:���� */
				/* �ж������Ƿ����� */
				if((before_time > second_time_b)&&
				   (before_time - second_time_b > Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl))
				{
					/* ���ݲ����� */
					/* ��ǰһ�鵱��������Ϊ����ʱ��/�¶ȡ� */
					*StartOff_p = startoff;
					return;
				}
			}
		}
			
/* ��ǰ������Ϊ����ʱ�䡣 */
#if (Print_Sensor_CH_Num >= 1)
		/* ����1 */
		startoff.Ch1_Value = ch_1_data;
#endif
#if (Print_Sensor_CH_Num >= 2)
		/* ����2 */
		startoff.Ch2_Value = ch_2_data;
#endif
#if (Print_Sensor_CH_Num >= 3)
		/* ����3 */
		startoff.Ch3_Value = ch_3_data;
#endif
#if (Print_Sensor_CH_Num >= 4)
		/* ����4 */
		startoff.Ch4_Value = ch_4_data;
#endif
		/* ���õ�ǰ�������� */
		/* �ж�����״̬ */
		if(
#if (Print_Sensor_CH_Num >= 1)
			((startoff.Ch1_Value <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
			&&(startoff.Ch1_Value >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
			||(startoff.Ch1_Value == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 2)
			&&((startoff.Ch2_Value <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
			&&(startoff.Ch2_Value >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
			||(startoff.Ch2_Value == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 3)
			&&((startoff.Ch3_Value <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
			&&(startoff.Ch3_Value >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
			||(startoff.Ch3_Value == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 4)
			&&((startoff.Ch4_Value <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
			&&(startoff.Ch4_Value >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
			||(startoff.Ch4_Value == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif		
		)
		{
			/* �������� */
			dataState = TRUE;
		}else
		{
			/* ���� */
			dataState = FALSE;
		}
		
		/* ���ó���ʱ�� */
		startoff.Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		startoff.Time.Month = ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		startoff.Time.Day 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		startoff.Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		startoff.Time.Min 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		startoff.Time.Sec 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
		
		/* ���õ�ǰʱ��Ϊǰһ������ʱ�� */
		before_time = second_time_b;
		
		/* ������һ�����ݵ�ַ */
		end_adder -= Save_Sensor_Data_Len;	

	}
	*StartOff_p = startoff;
	return;
}

#endif
/*************************************************************************************************/
/* ������	: Easy_Print_Data_Title														 	 	 */
/* ����		: sensor_inf:������ͨ����Ϣ��print_inf:��ӡ��Ϣ								 		 */
/* ���		: �� 																				 */
/* ����		: һ����ӡ����															 	 		 */
/* ��������  : 2016/04/08																		 */
/*************************************************************************************************/
static void Easy_Print_Data_Title(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf)
{
/* �����Ŵ�ӡ���� */
#if (Print_Scheme == 1)
	/* ����ʱ�� */
	uint32_t startofftemp = 0;
	/* �ִ�ʱ�� */
	uint32_t arrivetemp = 0;
	/* ��ʱʱ��Ĵ��� */
	Times 	 temp_;
#endif	
	
	/* ��ӡ�ָ�� */
	Print_Indivisible_Symbol();
	
	/* �����С������ */
	Print_Set_Font_Double(RESET);
	/* ����Ϊ16*16��С������ */
	Print_Set_Font(Print_Font_16_16);
	/* �豸����:****** */
	printf(Device_Number_Str,
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
/* �����Ŵ�ӡ���� */
#if (Print_Scheme == 1)
	/* �������ʱ��͵ִ�ʱ�� */
	temp_ = StartOffTemp.Time;
	startofftemp 	= RTC_Date_Time_To_Second(&temp_);
	temp_ = ArriveTemp.Time;
	arrivetemp 		= RTC_Date_Time_To_Second(&temp_);
	/* ��ӡ�˵���:7DC311001-1F9314C0-1F946640 */
	printf(Print_TransportNo_Str,
		   ((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
		   ((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
		   ((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
		   ((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)),
		   startofftemp,
		   arrivetemp);
#endif	
	
	/* ��ӡ������ͨ��1�����ݷ�Χ */
#if (Print_Sensor_CH_Num >= 0x01)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
	/* �¶����ʹ����� */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_1+1),
	(	  (sensor_inf->Inf_State.CH1.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH1.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Low_Limit*10)%10),
	(	  (sensor_inf->Inf_State.CH1.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH1.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* ʪ�����ʹ����� */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_1+1),
	((s16) sensor_inf->Inf_State.CH1.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Low_Limit*10)%10),	
	((s16) sensor_inf->Inf_State.CH1.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#endif
#endif

	/* ��ӡ������ͨ��2�����ݷ�Χ */
#if (Print_Sensor_CH_Num >= 0x02)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
	/* �¶����ʹ����� */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_2+1),
	(	  (sensor_inf->Inf_State.CH2.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH2.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH2.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH2.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* ʪ�����ʹ����� */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_2+1),
	((s16) sensor_inf->Inf_State.CH2.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Low_Limit*10)%10),	
	((s16) sensor_inf->Inf_State.CH2.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#endif
#endif

	/* ��ӡ������ͨ��3�����ݷ�Χ */
#if (Print_Sensor_CH_Num >= 0x03)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
	/* �¶����ʹ����� */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_3+1),
	(	  (sensor_inf->Inf_State.CH3.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH3.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH3.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH3.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* ʪ�����ʹ����� */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_3+1),
	((s16) sensor_inf->Inf_State.CH3.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Low_Limit*10)%10),
	((s16) sensor_inf->Inf_State.CH3.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#endif
#endif

	/* ��ӡ������ͨ��4�����ݷ�Χ */
#if (Print_Sensor_CH_Num >= 0x04)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
	/* �¶����ʹ����� */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_4+1),
	(	  (sensor_inf->Inf_State.CH4.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH4.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH4.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH4.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* ʪ�����ʹ����� */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_4+1),
	((s16) sensor_inf->Inf_State.CH4.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Low_Limit*10)%10),
	((s16) sensor_inf->Inf_State.CH4.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Up_Limit*10)%10));
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	#endif
#endif

	/* ���ݼ�¼����:****** */
	printf(Print_Record_Data_Str);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
	/* ����ʱ�����Ŀ�� */
	printf(Print_Time_Bar_Str);
#if (Print_Sensor_CH_Num >= 1)
	/* ���ô�����1��Ŀ�� */
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
	printf(Print_Temp_Bar_Str,1);
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	printf(Print_Humi_Bar_Str,1);
	#endif	
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* ���ô�����2��Ŀ�� */
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
	printf(Print_Temp_Bar_Str,2);
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	printf(Print_Humi_Bar_Str,2);
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ���ô�����3��Ŀ�� */
	#if (Print_Scheme == 0)
	/* Ĭ�Ϸ��� */
		#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
		printf(Print_Temp_Bar_Str,3);
		#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
			   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
			   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		printf(Print_Humi_Bar_Str,3);
		#endif
	#elif (Print_Scheme == 1)
	/* �����ŷ��� */
	printf(" ");
	/* �����ŷ��� */
	printf(Print_EnvironmentTemp_Bar_Str);
	#endif	
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* ���ô�����4��Ŀ�� */
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
	printf(Print_Temp_Bar_Str,4);
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	printf(Print_Humi_Bar_Str,4);
	#endif	
#endif
	/* �س� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
}



///************************************************************************************************
//*������     : IsOrNotStartPrintUoloadBody															
//*�������  	: adder1����Ҫ�ԱȵĲ���   								
//*�������	  : true�������ж����ʼ�ַ��� 	 false�������ж���ַ��� 																				
//*����	 	    : xxx																
//*��������	  : 2018/05/28																		
//************************************************************************************************/
//static _Bool  IsOrNotStartPrintUoloadBody(int adder1)
//{
//   u8 i=0;
//	for(i=0;i<sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder);i++)
//	{
//	  if(adder1==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[i])
//		{
//		
//		  break;
//			return  true;
//		  
//		}

//	}	
//return  false;
//}
///************************************************************************************************
//*������     : IsOrNotStopPrintUoloadBody															
//*�������  	: adder1����Ҫ�ԱȵĲ���   								
//*�������	  : true�������ж�������ַ��� 	 false�������ж�������ַ��� 		 																				
//*����	 	    : xxx																
//*��������	  : 2018/05/28																		
//************************************************************************************************/
//static _Bool  IsOrNotStopPrintUoloadBody(int adder1)
//{
//   u8 i=0;
//	for(i=0;i<sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder);i++)
//	{
//	  if(adder1==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[i])
//		{
//		
//		  break;
//			return  true;
//		  
//		}

//	}	
//return  false;
//}
/*************************************************************************************************/
/* ������	: Easy_Print_Data_Body														 	 	 */
/* ����		: sensor_inf:������ͨ����Ϣ��print_inf:��ӡ��Ϣ								 		 */
/* ���		: �� 																				 */
/* ����		: һ����ӡ����															 	 		 */
/* ��������  : 2016/04/08																		 */
/*************************************************************************************************/
static void Easy_Print_Data_Body(INSTRU_SENSOR* sensor_inf,PRINT_SENSOR_MAX_MIN_VALUE* print_sensor_max_min_value,int* start_adder,int* end_adder,BOOL * printDateFlag)
{
/* һ��ʱ��������������������жϴ�ӡ�������Ƿ񳬹�һ�� */
#define Day_Second_Num			(86400uL)
	/* ������ָ�� */
	u8* data_p;
	u8  Temp=0;
	/* ʱ��Ĵ��� */
	Times time_b;
	/* ��ʱ��Ĵ��� */
	u32 second_time_b;
	/* ��¼�ϴδ�ӡ�������� */
	static u32 print_date_dr = 0;
	/* ��ӡ���ڱ�־ */
	static BOOL PrintDateFlag = FALSE;
	
#if (Print_Sensor_CH_Num >= 1)
	/* ͨ��1���� */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* ͨ��2���� */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ͨ��3���� */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* ͨ��4���� */
	float ch_4_data;
#endif
	
	
	
	/* �жϴ��������� */
	if(*start_adder <= *end_adder)
	{
		/* ��ȡ��ʼ��ַ���� */
		data_p = Flash_Read_Sensor_Data(*end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		/* ������һ�����ݵ�ַ */
		*end_adder -= Save_Sensor_Data_Len;
		/* ��ȡ ʱ������� */
#if (Print_Sensor_CH_Num >= 1)
	/* ��ȡͨ��1���� */
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_1_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		ch_1_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_1*sizeof(float))));
	#endif	
#endif
		
#if (Print_Sensor_CH_Num >= 2)
	/* ��ȡͨ��2���� */
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))	
		ch_2_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		ch_2_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_2*sizeof(float))));
	#endif		
#endif
		
#if (Print_Sensor_CH_Num >= 3)
	/* ��ȡͨ��3���� */
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_3_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		ch_3_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_3*sizeof(float))));
	#endif		
#endif

#if (Print_Sensor_CH_Num == 4)
	/* ��ȡͨ��4���� */
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
		ch_4_data = Save_Tmp_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
		ch_4_data = Save_Hum_To_Float(*((float*)(data_p+sizeof(DATETIME_Storage_Format) + Sensors_CH_4*sizeof(float))));
	#endif		
#endif
		
		/* ��ȡʱ������ */
		time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
		/* ��ʱ��ת������ */
		second_time_b = RTC_Date_Time_To_Second(&time_b);
		
		/* �ж�ͨ��״̬ */
		if(
#if (Print_Sensor_CH_Num >= 1)
			((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
		   &&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
		   ||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 2)
		   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
			&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
			||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 3)
		   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
			&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
			||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 4)
		   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
			&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
			||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif		
			)
		{
			/* ͨ������״̬ */
			/* �ж� ����ʱ���Ƿ�Ϊ ����ʱ��ı��� */
			if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl != 0)
			{
				/* �ж�ʱ�����Ƿ�С�ڱ���ʱ���� */
				if((EasyPrintContinuousReg - second_time_b)<Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
				{
					
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/	
					
					
					/* ����ӡ�������� */
					/* ��ͷ��ʼִ�� */
					return;
				}
			}
		}else
		{
			/* ͨ������״̬ */
			/* �ж� ����ʱ���Ƿ�Ϊ ����ʱ��ı��� */
			if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl != 0)
			{
				/* �ж�ʱ�����Ƿ�С�ڱ���ʱ���� */
				if((EasyPrintContinuousReg - second_time_b)<Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
				{
					
				 /*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/		
					/* ����ӡ�������� */
					/* ��ͷ��ʼִ�� */
					return;
				}
			}
		}
		/* ��ӡ��ȡ���������� */
		if((*printDateFlag == TRUE)||((second_time_b / Day_Second_Num) != print_date_dr))
		{
			/* ֻ��2����������Ҫ��ӡ���ڣ�1����ӡ��һ������;2����ӡʱ����1������ */
			/* �ж� ��ӡ���ڱ�־ */
			if(*printDateFlag == TRUE)
			{
				/* ������ڱ�־  */
				*printDateFlag = FALSE;
				/* ������ֵ�Ĵ������� */
#if (Print_Sensor_CH_Num >= 1)
				/* ������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
				/* ������ֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;

#endif
	
#if (Print_Sensor_CH_Num >= 2)	
				/* ������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
				/* ������ֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;

#endif	
#if (Print_Sensor_CH_Num >= 3)
				/* ������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
				/* ������ֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;

#endif
#if (Print_Sensor_CH_Num >= 4)
				/* ������ֵ */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
				/* ������ֵ */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;

#endif
				/* �����µĴ�ӡ���� */
				print_date_dr = (second_time_b / Day_Second_Num);
				
				/* ����������ӡ */
				EasyPrintContinuousReg = second_time_b;
				
				/* ���ô�ӡ���ڱ�־ */
				PrintDateFlag = TRUE;
				
			}else
			{
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/	
				
				/* ���ݲ���������ӡ�ָ���š� */
				Print_Separator_Line();
				
				/* ���ô�ӡ���ڱ�־ */
				PrintDateFlag = TRUE;
				
//				/* ������һ�����ݵ�ַ */
//				*end_adder += Save_Sensor_Data_Len;
				
				/* �����µĴ�ӡ���� */
				print_date_dr = (second_time_b / Day_Second_Num);
				
				return;
			}
		}
		
		/* �жϴ�ӡ���ڱ�־ */
		if(PrintDateFlag == TRUE)
		{
			/* ��ӡ���� */
			Print_Date(data_p);
			/* �����־ */
			PrintDateFlag = FALSE;
		}
		
		/* �ж�ͨ��״̬ */
		if(
#if (Print_Sensor_CH_Num >= 1)
			((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)
		   &&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit)
		   ||(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 2)
		   &&((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)
			&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit)
			||(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 3)
		   &&((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)
			&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit)
			||(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif
#if (Print_Sensor_CH_Num >= 4)
		   &&((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)
			&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit)
			||(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value))
#endif		
			)
		{
			/* �������� */
			/* �жϴ�ӡ����״̬ */
			if(EasyPrintDataState == RESET)
			{
				/* ǰһ������״̬:���� */
				/* �ж������Ƿ����� */
				if((EasyPrintContinuousReg > second_time_b)&&
				   (EasyPrintContinuousReg - second_time_b > Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl))
					{
						/* ���ݲ���������ӡ�ָ���š� */
						Print_Separator_Line();
						
						/* ���õ�ǰ����ʱ��Ϊһ���������ݵĿ�ʼ���� */
						EasyPrintContinuousReg = second_time_b;
						
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/	
						
						
//						/* ������һ�����ݵ�ַ */
//						*end_adder += Save_Sensor_Data_Len;
						
						/* ��������״̬ */
						EasyPrintDataState = RESET;
						return;
					}
			}else
			{
				/* ǰһ������״̬:���� */
				/* �ж������Ƿ����� */
				if(EasyPrintContinuousReg > second_time_b)
				{
					/* �ж�����/������¼ʱ���С */
					if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl > 
					   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
					{
						/* ������¼ʱ�䳤 */
						/* �ж������Ƿ����� */
						if((EasyPrintContinuousReg - second_time_b) > 
						   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
						{
							/* ���ݲ���������ӡ�ָ���š� */
							Print_Separator_Line();
							
							/* ���õ�ǰ����ʱ��Ϊһ���������ݵĿ�ʼ���� */
							EasyPrintContinuousReg = second_time_b;
							
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/	
							
//							/* ������һ�����ݵ�ַ */
//							*end_adder += Save_Sensor_Data_Len;
							
							/* ��������״̬ */
							EasyPrintDataState = RESET;
							return;		
						}
					}else
					{
						/* ������¼ʱ�䳤 */
						if((EasyPrintContinuousReg - second_time_b) > 
						    Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
						{
							/* ���ݲ���������ӡ�ָ���š� */
							Print_Separator_Line();
							
							/* ���õ�ǰ����ʱ��Ϊһ���������ݵĿ�ʼ���� */
							EasyPrintContinuousReg = second_time_b;
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/		
							
//							/* ������һ�����ݵ�ַ */
//							*end_adder += Save_Sensor_Data_Len;
							
							/* ��������״̬ */
							EasyPrintDataState = RESET;
							return;
						}
					}
				}
			}
		}else
		{
			/* ���ݳ��� */
			/* �жϴ�ӡ����״̬ */
			if(EasyPrintDataState == SET)
			{
				/* ǰһ������״̬:���� */
				/* �ж������Ƿ����� */
				if((EasyPrintContinuousReg > second_time_b)&&
				   (EasyPrintContinuousReg - second_time_b > Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl))
					{
						/* ���ݲ���������ӡ�ָ���š� */
						Print_Separator_Line();
						
						/* ���õ�ǰ����ʱ��Ϊһ���������ݵĿ�ʼ���� */
						EasyPrintContinuousReg = second_time_b;
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/	
						/* ������һ�����ݵ�ַ */
//						*end_adder += Save_Sensor_Data_Len;
						
						/* ��������״̬ */
						EasyPrintDataState = SET;
						return;
					}
			}else
			{
				/* ǰһ������״̬:���� */
				/* �ж������Ƿ����� */
				if(EasyPrintContinuousReg > second_time_b)
				{
					/* �ж�����/������¼ʱ���С */
					if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl > 
					   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
					{
						/* ������¼ʱ�䳤 */
						/* �ж������Ƿ����� */
						if((EasyPrintContinuousReg - second_time_b) > 
						   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
						{
							/* ���ݲ���������ӡ�ָ���š� */
							Print_Separator_Line();
							
							/* ���õ�ǰ����ʱ��Ϊһ���������ݵĿ�ʼ���� */
							EasyPrintContinuousReg = second_time_b;
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/			
//							/* ������һ�����ݵ�ַ */
//							*end_adder += Save_Sensor_Data_Len;
							
							/* ��������״̬ */
							EasyPrintDataState = SET;
							return;		
						}
					}else
					{
						/* ������¼ʱ�䳤 */
						if((EasyPrintContinuousReg - second_time_b) > 
						    Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
						{
							/* ���ݲ���������ӡ�ָ���š� */
							Print_Separator_Line();
							
							/* ���õ�ǰ����ʱ��Ϊһ���������ݵĿ�ʼ���� */
							EasyPrintContinuousReg = second_time_b;
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/			
							
//							/* ������һ�����ݵ�ַ */
//							*end_adder += Save_Sensor_Data_Len;
							
							/* ��������״̬ */
							EasyPrintDataState = SET;
							return;
						}
					}
				}
			}
		}
		
					/*********************��ӡ�Ƿ�ж���ַ���**************************/
					for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp])
						 {
						     printf(Print_StartUnload_Symbol_Start_Str,Temp+1);
						 }
					
					}
					
				 for(Temp=0;Temp<100;Temp++)
					{
					   if((*end_adder+Save_Sensor_Data_Len)==Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp])
						 {
						     printf(Print_StoptUnload_Symbol_End_Str,Temp+1);
						 }
					
					}
					
				/*****************************************************************/	
		
		
		/* ���õ�ǰ����ʱ��Ϊһ���������ݵĿ�ʼ���� */
		EasyPrintContinuousReg = second_time_b;
		
		/* ��ӡ��ȡ��ʱ������ */
		printf(Print_Data_Body_Time_Str,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS);
		
#if (Print_Sensor_CH_Num == 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
		/* �ж������Ƿ񳬱� */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ��������� */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),	
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}

	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
			/* �ж������Ƿ񳬱� */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����1���� */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* �ж������Ƿ񳬱� */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����2���� */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		
	#endif
#elif (Print_Sensor_CH_Num == 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_Hum))
		/* �ж������Ƿ񳬱� */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);
		}
		
		/* ��ӡ��ȡ�Ĵ�����1���� */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}

		/* �ж������Ƿ񳬱� */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����2���� */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
		/* �ж������Ƿ񳬱� */
		if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����3���� */
		printf(Print_Data_Body_Data_Str,
		((ch_3_data >= 0.0) ? ' ':'-'),
		((u16) ch_3_data),
		((u16)(ch_3_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch3_Max = ch_3_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch3_Min = ch_3_data;
		}
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_3_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_Hum))
		/* �ж������Ƿ񳬱� */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);
		}
		/* ��ӡ��ȡ�Ĵ�����1���� */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* �ж������Ƿ񳬱� */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����2���� */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
		/* �ж������Ƿ񳬱� */
		if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����3���� */
		printf(Print_Data_Body_Data_Str,
		((ch_3_data >= 0.0) ? ' ':'-'),
		((u16) ch_3_data),
		((u16)(ch_3_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch3_Max = ch_3_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch3_Min = ch_3_data;
		}
		/* �ж������Ƿ񳬱� */
		if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* ����δ����,���������»���. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* ��ӡ��ȡ�Ĵ�����4���� */
		printf(Print_Data_Body_Data_Str,
		((ch_4_data >= 0.0) ? ' ':'-'),
		((u16) ch_4_data),
		((u16)(ch_4_data*10)%10));
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
		{
			/* �������ֵ */
			print_sensor_max_min_value->Ch4_Max = ch_4_data;
		}
		
		/* �жϴ�����ͨ������ֵ */
		if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
		{
			/* ������Сֵ */
			print_sensor_max_min_value->Ch4_Min = ch_4_data;
		}
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{	
			/* �ж������Ƿ񳬱� */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_4_data >= 0.0) ? ' ':'-'),
			((u16) ch_4_data),
			((u16)(ch_4_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
			}
		}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))	
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_3_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{	
			/* �ж������Ƿ񳬱� */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
		/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
		if(ch_4_data == Gateway_NJSY_TimeoutPara)
		{
			/* ����δ����,���ý�ֹ�»���. */
			Print_Set_Underline_Config(DISABLE);
			/* Ϊ��ʱ�������ݣ���ӡ�����ݡ� */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* �ж������Ƿ񳬱� */
			if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
			{
				/* ����δ����,���ý�ֹ�»���. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* ����δ����,���������»���. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* �������� */
			printf(Print_Data_Body_Data_Str,
			((ch_4_data >= 0.0) ? ' ':'-'),
			((u16) ch_4_data),
			((u16)(ch_4_data*10)%10));
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
			{
				/* �������ֵ */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
			}
			
			/* �жϴ�����ͨ������ֵ */
			if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
			{
				/* ������Сֵ */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
			}
		}
		
	#endif
#endif
		
		/* ����δ����,���ý�ֹ�»���. */
		Print_Set_Underline_Config(DISABLE);
		/* ���� */
		Print_Set_CRLF();
		/* �ȴ������ӡ���� */
		OSTimeDly(120);
		
	}
	
	
#undef Day_Second_Num
}

/*************************************************************************************************/
/* ������	: Easy_Print_Data_Tail													 	 		 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: �������β����																 	 	 */
/* ��������  : 2016/10/18																			 */
/*************************************************************************************************/
static void Easy_Print_Data_Tail(PRINT_SENSOR_MAX_MIN_VALUE* print_sensor_max_min_value)
{

	/* 1����2��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	
	/* 2����ӡ�������/Сֵ. */
	/* ��ӡ�������ֵ */
	printf(Print_Sensor_Data_Max_Value_Str);
	/* ��ӡ��ȡ�Ĵ��������� */
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��1���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch1_Max),
	((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))			   
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch1_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Max),
		((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch1_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Max),
		((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��2���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch2_Max),
	((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	       (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch2_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Max),
		((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch2_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Max),
		((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��3���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch3_Max),
	((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch3_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Max),
		((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch3_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Max),
		((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||(Sensor_CH4_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��4���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch4_Max),
	((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch4_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Max),
		((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch4_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Max),
		((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	/* �س� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	
	/* ��ӡ������Сֵ. */
	printf(Print_Sensor_Data_Min_Value_Str);
	/* ��ӡ��ȡ�Ĵ��������� */
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��1���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch1_Min),
	((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch1_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Min),
		((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch1_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Min),
		((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��2���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch2_Min),
	((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch2_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Min),
		((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch2_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Min),
		((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��3���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch3_Min),
	((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch3_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Min),
		((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}	
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch3_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Min),
		((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��4���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch4_Min),
	((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	       (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch4_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Min),
		((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(print_sensor_max_min_value->Ch4_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Min),
		((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	/* �س� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(200);	
	
	/* ��1��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);

	
#if (Print_Scheme == 0)
/* Ĭ�Ϸ��� */
	
	/* 3����ӡ�ͻ�������. */
	/* ���������»���. */
	Print_Set_Underline_Config(ENABLE);
	/* ��ӡ�ͻ�������. */
	printf(Print_Delivery_Party_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1�� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	/* ��ӡ�ջ�������. */
	printf(Print_Receiver_Party_Str);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	
	/* 4����ӡ ���ƺ�. */	
	printf(Print_Card_NO_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
#elif (Print_Scheme == 1)
/* �����ŷ��� */

	/* 3����ӡ �����¶�. */	
	/* ���������»���. */
	Print_Set_Underline_Config(ENABLE);
	
	/* ��ӡ ����ʱ�� */
	//
	printf(Print_StartOffTime_Str,
		   StartOffTemp.Time.Year,
		   StartOffTemp.Time.Month,
		   StartOffTemp.Time.Day,
		   StartOffTemp.Time.Hour,
		   StartOffTemp.Time.Min);
	/* �ȴ���ӡ���� */
	OSTimeDly(120);
	/* ��ӡ �����¶�. */
	printf(Print_StartOffTemp_Str);
	
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��1���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
	((u16) StartOffTemp.Ch1_Value),
	((u16)(StartOffTemp.Ch1_Value*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(StartOffTemp.Ch1_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch1_Value),
		((u16)(StartOffTemp.Ch1_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(StartOffTemp.Ch1_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch1_Value),
		((u16)(StartOffTemp.Ch1_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��2���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
	((u16) StartOffTemp.Ch2_Value),
	((u16)(StartOffTemp.Ch2_Value*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(StartOffTemp.Ch2_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch2_Value),
		((u16)(StartOffTemp.Ch2_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(StartOffTemp.Ch2_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch2_Value),
		((u16)(StartOffTemp.Ch2_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
	/* ��1�� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ��ӡ �����¶�. */
	printf(Print_StartOffEnvironmentTemp_Bar_Str);
	
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��3���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
	((u16) StartOffTemp.Ch3_Value),
	((u16)(StartOffTemp.Ch3_Value*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(StartOffTemp.Ch3_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch3_Value),
		((u16)(StartOffTemp.Ch3_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(StartOffTemp.Ch3_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch3_Value),
		((u16)(StartOffTemp.Ch3_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);
	}
	#endif
#endif
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1�� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	
	/* 4����ӡ �����¶�. */
	
	/* ��ӡ ����ʱ�� */
	//
	printf(Print_ArriveTime_Str,
		   ArriveTemp.Time.Year,
		   ArriveTemp.Time.Month,
		   ArriveTemp.Time.Day,
		   ArriveTemp.Time.Hour,
		   ArriveTemp.Time.Min);
	/* �ȴ���ӡ���� */
	OSTimeDly(120);
	/* ��ӡ �����¶�. */
	printf(Print_ArriveTemp_Str);
	
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��1���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
	((u16) ArriveTemp.Ch1_Value),
	((u16)(ArriveTemp.Ch1_Value*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(ArriveTemp.Ch1_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch1_Value),
		((u16)(ArriveTemp.Ch1_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(ArriveTemp.Ch1_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch1_Value),
		((u16)(ArriveTemp.Ch1_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��2���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
	((u16) ArriveTemp.Ch2_Value),
	((u16)(ArriveTemp.Ch2_Value*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(ArriveTemp.Ch2_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch2_Value),
		((u16)(ArriveTemp.Ch2_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(ArriveTemp.Ch2_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch2_Value),
		((u16)(ArriveTemp.Ch2_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
	/* ��1�� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* ��ӡ �����¶�. */
	printf(Print_ArriveEnvironmentTemp_Bar_Str);
	
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
	/* ��ӡ������ͨ��3���ֵ */
	printf(Print_Data_Body_Data_Str,
	(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
	((u16) ArriveTemp.Ch3_Value),
	((u16)(ArriveTemp.Ch3_Value*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(ArriveTemp.Ch3_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch3_Value),
		((u16)(ArriveTemp.Ch3_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
	/* �ж� �����������Ƿ�Ϊ ��ʱ�������� */
	if(ArriveTemp.Ch3_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch3_Value),
		((u16)(ArriveTemp.Ch3_Value*10)%10));
	}else
	{
		/* ��ʱ�������� ��ӡȱʧ */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}	
	#endif
#endif
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1�� */
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
	
//	/* 5����ӡ ���ݱ��. */	
//	printf(Print_FBillNo_Str);
//	/* �ȴ������ӡ���� */
//	OSTimeDly(200);
//	/* ��1��. */	
//	Print_Set_CRLF();
//	/* �ȴ������ӡ���� */
//	OSTimeDly(120);
	
	//������GPS��Ϣ������Ϣ���ж��Ƿ�Ϊ�����豸
#if (GPS_Config_Enable == 0x01)
	/* 4����ӡ ���ƺ�. */	
	printf(Print_Card_NO_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO);
	/* �ȴ������ӡ���� */
	OSTimeDly(200);
	/* ��1��. */	
	Print_Set_CRLF();
	/* �ȴ������ӡ���� */
	OSTimeDly(120);
#endif
#endif

#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
	/* 5����ӡ��˾LOGO��ά��. */
	Print_Picture((char*)Print_LOGO);
#endif

	/* 6�����ý�ֹ�»���. */
	Print_Set_Underline_Config(DISABLE);
	/* ��ӡ�ָ�� */
	Print_Indivisible_Symbol();
	
}

/* һ����ӡ״̬��־ */
static BOOL EasyPrintState = FALSE;
/* һ����ӡ��ʱ��׼ʱ�� */
static u32 BaseOutTimeEasyPrint = 0;

/*************************************************************************************************/
/* ������	: Easy_Print_Get_Out_Time													 	 	 */
/* ����		: ��																		 		 */
/* ���		: ��ݴ�ӡ״̬ 																		 */
/* ����		: �����ݴ�ӡ״̬															 	 	 */
/* ��������  : 2016/04/21																		 */
/*************************************************************************************************/
BOOL Easy_Print_Get_State(void)
{
	return EasyPrintState;
}

/*************************************************************************************************/
/* ������	: Easy_Print_Get_Out_Time													 	 	 */
/* ����		: ��																		 		 */
/* ���		: ��ӡʣ��ʱ�� 																		 */
/* ����		: �����ӡʣ��ʱ��																 	 */
/* ��������  : 2016/04/21																		 */
/*************************************************************************************************/
u32 Easy_Print_Get_Out_Time(void)
{
	return (EasyPrintOutTime - (OSTimeGet() - BaseOutTimeEasyPrint))/1000;
}

/*************************************************************************************************/
/* ������	: Easy_Print_Process_Flow													 	 	 */
/* ����		: ��																		 		 */
/* ���		: �� 																				 */
/* ����		: һ����ӡ����																 	 	 */
/* ��������  : 2016/04/08																		 */
/*************************************************************************************************/
void Easy_Print_Process_Flow(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf)
{

	/* һ����ʼ��ӡ��׼ʱ�� */
	static u32 BaseTimeStartEasyPrint = 0;
	/* һ��ֹͣ��ӡ��׼ʱ�� */
	static u32 BaseTimeStopEasyPrint = 0;
	/* ��ӡ����ʼ��ַ */
	static int print_start_adder = 0;
	/* ��ӡ�Ľ�����ַ */
	static int print_end_adder = 0;
	/* �״δ�ӡ���ڱ�־ */
	static BOOL printDateFlag = TRUE;
	
	/* ��ӡ������ͨ������ֵ */
	static PRINT_SENSOR_MAX_MIN_VALUE print_sensor_max_min_value = 
	{
#if (Print_Sensor_CH_Num >= 1)
	/* ��ӡͨ��1����ֵ */
	/* ���ֵ */
	0,
	/* ��Сֵ */
	0,
#endif	
#if (Print_Sensor_CH_Num >= 2)	
	/* ��ӡͨ��2����ֵ */
	/* ���ֵ */
	0,
	/* ��Сֵ */
	0,
#endif	
#if (Print_Sensor_CH_Num >= 3)
	/* ��ӡͨ��3����ֵ */
	/* ���ֵ */
	0,
	/* ��Сֵ */
	0,
#endif
#if (Print_Sensor_CH_Num >= 4)
	/* ��ӡͨ��4����ֵ */
	/* ���ֵ */
	0,
	/* ��Сֵ */
	0,	
#endif
	};
	
	
	/* �ж�һ����ӡ״̬��־ */
	if(EasyPrintState == TRUE)
	{
		/* �жϡ�>�����Ƿ񳤰�����4S��ֹͣһ����ӡ */
		if(IsKeyStopEasyPrint() == Bit_SET)
		{
			/* �жϰ���ʱ�䳤�� */
			if((OSTimeGet() - BaseTimeStopEasyPrint) >= StopEasyPrintTime)
			{

				//��ӡ��������
				/* �������β���� */
				Easy_Print_Data_Tail(&print_sensor_max_min_value);
				
				/* ���ô�ӡ���ڱ�־ */
				printDateFlag = TRUE;
				
				/* ��¼��ǰ��ʱ��Ϊ��ʼһ����ӡʱ�� */ 
				BaseTimeStartEasyPrint = OSTimeGet();
				
				/* ˢ����ʾ */
				Flash = FlashMode_ManualInit;	
				/* ��λһ����ӡ��־ */
				EasyPrintState = FALSE;
				/* ��λ��ӡ����״̬ */
				EasyPrintDataState = RESET;
				
				/* ��λ��ӡͨ������ֵ */
#if (Print_Sensor_CH_Num >= 1)
				print_sensor_max_min_value.Ch1_Max = 0;
				print_sensor_max_min_value.Ch1_Min = 0;
#if (Print_Scheme == 1)
				StartOffTemp.Ch1_Value = 0;
				ArriveTemp.Ch1_Value = 0;
#endif	
#endif	
#if (Print_Sensor_CH_Num >= 2)	
				print_sensor_max_min_value.Ch2_Max = 0;
				print_sensor_max_min_value.Ch2_Min = 0;
#if (Print_Scheme == 1)
				StartOffTemp.Ch2_Value = 0;
				ArriveTemp.Ch2_Value = 0;
#endif
#endif
#if (Print_Sensor_CH_Num >= 3)
				print_sensor_max_min_value.Ch3_Max = 0;
				print_sensor_max_min_value.Ch3_Min = 0;
#if (Print_Scheme == 1)
				StartOffTemp.Ch3_Value = 0;
				ArriveTemp.Ch3_Value = 0;
#endif
#endif
#if (Print_Sensor_CH_Num >= 4)
				print_sensor_max_min_value.Ch4_Max = 0;
				print_sensor_max_min_value.Ch4_Min = 0;
#if (Print_Scheme == 1)				
				StartOffTemp.Ch4_Value = 0;
				ArriveTemp.Ch4_Value = 0;
#endif
#endif

#if (Print_Scheme == 1)
				StartOffTemp.Time.Year 	= 0;
				StartOffTemp.Time.Month = 0;
				StartOffTemp.Time.Day	= 0;
				StartOffTemp.Time.Hour	= 0;
				StartOffTemp.Time.Min	= 0;
				StartOffTemp.Time.Sec	= 0;

				ArriveTemp.Time.Year 	= 0;
				ArriveTemp.Time.Month 	= 0;
				ArriveTemp.Time.Day		= 0;
				ArriveTemp.Time.Hour	= 0;
				ArriveTemp.Time.Min		= 0;
				ArriveTemp.Time.Sec		= 0;
#endif	
			}
		
		}else
		{
			/* ��¼��ǰ��ʱ��Ϊֹͣһ����ӡʱ�� */
			BaseTimeStopEasyPrint = OSTimeGet();
		}
		
		/* �жϡ�<�����Ƿ��� */
		if(IsKeyStartEasyPrint() == Bit_SET)
		{
			//��ӡһ������
			Easy_Print_Data_Body(sensor_inf,&print_sensor_max_min_value,&print_start_adder,&print_end_adder,&printDateFlag);
		}
		
		/* �ж��˳���ʱʱ�� */
		if((IsKeyStopEasyPrint() == Bit_RESET)&&(IsKeyStartEasyPrint() == Bit_RESET))
		{
			/* ��¼��ǰ��ʱ��Ϊ��ʼһ����ӡʱ�� */ 
			if(OSTimeGet() - BaseOutTimeEasyPrint >= EasyPrintOutTime)
			{
				/* ��λһ����ӡ��־ */
				EasyPrintState = FALSE;
				//��ӡ��������
				/* �������β���� */
				Easy_Print_Data_Tail(&print_sensor_max_min_value);
				
				/* ���ô�ӡ���ڱ�־ */
				printDateFlag = TRUE;
				
				/* ��¼��ǰ��ʱ��Ϊ��ʼһ����ӡʱ�� */ 
				BaseTimeStartEasyPrint = OSTimeGet();
				
				/* ˢ����ʾ */
				Flash = FlashMode_ManualInit;	
				/* ��λһ����ӡ��־ */
				EasyPrintState = FALSE;
				/* ��λ��ӡ����״̬ */
				EasyPrintDataState = RESET;
				
				/* ��λ��ӡͨ������ֵ */
#if (Print_Sensor_CH_Num >= 1)
				print_sensor_max_min_value.Ch1_Max = 0;
				print_sensor_max_min_value.Ch1_Min = 0;
#endif	
#if (Print_Sensor_CH_Num >= 2)	
				print_sensor_max_min_value.Ch2_Max = 0;
				print_sensor_max_min_value.Ch2_Min = 0;
#endif	
#if (Print_Sensor_CH_Num >= 3)
				print_sensor_max_min_value.Ch3_Max = 0;
				print_sensor_max_min_value.Ch3_Min = 0;
#endif
#if (Print_Sensor_CH_Num >= 4)
				print_sensor_max_min_value.Ch4_Max = 0;
				print_sensor_max_min_value.Ch4_Min = 0;
#endif

#if (Print_Scheme == 1)
				StartOffTemp.Time.Year 	= 0;
				StartOffTemp.Time.Month = 0;
				StartOffTemp.Time.Day	= 0;
				StartOffTemp.Time.Hour	= 0;
				StartOffTemp.Time.Min	= 0;
				StartOffTemp.Time.Sec	= 0;

				ArriveTemp.Time.Year 	= 0;
				ArriveTemp.Time.Month 	= 0;
				ArriveTemp.Time.Day		= 0;
				ArriveTemp.Time.Hour	= 0;
				ArriveTemp.Time.Min		= 0;
				ArriveTemp.Time.Sec		= 0;
#endif	
			}
		}else
		{
			/* ��¼��ǰ��ʱ��Ϊ��ʼһ����ӡʱ�� */
			BaseOutTimeEasyPrint = OSTimeGet();
		}

	}else
	{
		/* һ����ӡ����δ��ʼ */
		/* �жϡ�<�����Ƿ񳤰�����4S����ʼһ����ӡ */
		if(IsKeyStartEasyPrint() == Bit_SET)
		{
			/* �жϰ���ʱ�䳤�� */
			if((OSTimeGet() - BaseTimeStartEasyPrint) >= StartEasyPrintTime)
			{
				
				//1��
				/* ���ô�ӡ���� */
				Print_Set_Para();
				//2��
				/* ������ӡ���ݵ���ʼλ��. */
				print_start_adder = print_inf->Inf_State.Print_Record_Start_Adder;
				//3��
				/* ������ӡ���ݵĽ���λ��. */
				print_end_adder = print_inf->Inf_State.Print_Record_End_Adder;
				//4��
				/* �ж���������ʼ��ַ�ͽ�����ַ�Ƿ�Ϸ�����ʼ��ַ С�ڵ��� ������ַ�� */
				if(print_start_adder > print_end_adder)
				{
					/* ��ʼ��ַ ���� ������ַ */
					/* ���ô�ӡ������ַ */
					print_end_adder += Flash_Capacity_Size;
				}
				//5��
				/* �ж��Ƿ������ݴ�ӡ */
				if(print_end_adder == 0)
				{
					/* û�д�ӡ���� */
					print_end_adder = -1;
				}
				
				/* ��¼��ǰ��ʱ��Ϊֹͣһ����ӡʱ�� */ 
				BaseTimeStopEasyPrint = BaseOutTimeEasyPrint;
				/* ˢ����ʾ */
				Flash = FlashMode_ManualInit;
				
				/* ����һ����ӡ��־ */
				EasyPrintState = TRUE;
				/* ��¼��ǰ��ʱ��Ϊ��ʼһ����ӡʱ�� */
				BaseOutTimeEasyPrint = OSTimeGet();
				
#if (Print_Scheme == 1)
				
				/* �������ʱ��Ĵ����͵���ʱ��Ĵ������� */
				memset(&StartOffTemp,0x00,sizeof(StartOffTemp));
				memset(&ArriveTemp,0x00,sizeof(ArriveTemp));
				/* ��������ʱ�� */
				Easy_Print_Search_StartOff_Time(sensor_inf,&StartOffTemp,print_start_adder,print_end_adder);
				/* ��������ʱ�� */
				Easy_Print_Search_Arrive_Time(sensor_inf,&ArriveTemp,print_start_adder,print_end_adder);
#endif	
				//��ӡ��ʼ����
				Easy_Print_Data_Title(sensor_inf,print_inf);
			}
		}else
		{
			/* ��¼��ǰ��ʱ��Ϊ��ʼһ����ӡʱ�� */
			BaseTimeStartEasyPrint = OSTimeGet();
		}
	}
}



#endif

/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/
