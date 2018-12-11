/************************************************************************************************/
/** @file              : Log_Print.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月28日															*/
/** @brief             : c file																	*/
/** @description       : 打印逻辑																*/
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
//	<h>打印功能相关的宏设置
//	<e>打印功能项目选择
//  <o.0..2>配置打印方案.
//  <0=> [默认]方案.
//  <1=> [金卫信]方案.
//	<i>打印方案选择.
#define Print_Scheme		(0x00)
//  </e>

// </h>
//	<<< end of configuration section >>>

//#if (Print_Scheme == 1)
//	#if (Print_Sensor_CH_Num != 3)
//		#error	"打印通道数量不是3个，于金卫信打印方案不相符。"
//	#endif
//#endif

/************************************************************************************************/
/* 打印 命令头 */
#define Print_CMD_Head_0X1B				(0x1B)
#define Print_CMD_Head_0X1C				(0x1C)
#define Print_CMD_Head_0X1D				(0x1D)
#define Print_CMD_Head_0X1F				(0x1F)
//打印命令
/* 打印 设置字体点整大小 */
#define Print_CMD_Set_Font_Size			(0x38)
/* 打印 设置回车 */
#define Print_CMD_Set_CRLF				(0x0A)
/* 打印 设置行间距 */
#define Print_CMD_Set_Line_Space		(0x31)
/* 打印 设置字间距 */ 
#define Print_CMD_Set_Word_Space		(0x20)
/* 打印 设置空格或空行 */
#define Print_CMD_Set_Blank_Line_Null	(0x66)
/* 打印 设置右限 */
#define Print_CMD_Set_Right_Limit		(0x51)
/* 打印 设置左限 */
#define Print_CMD_Set_Left_Limit		(0x6C)
/* 打印 允许/禁止下划线 */
#define Print_CMD_Set_Underline_Config	(0x2D)
/* 打印 设置旋转字符 */
#define Print_CMD_Set_Rotate_Char		(0x49)
/* 打印 初始化打印机 */
#define Print_CMD_Init					(0x40)
/* 打印 	允许/禁止反白显示 */
#define Print_CMD_Set_Highlight_Char	(0x69)
/* 打印 	图像数据 */
#define Print_CMD_Set_Picture			(0x10)
/* 打印 	图像数据 */
#define Print_CMD_Set_BarCode			(0x6B)
/* 打印 设置字体翻倍/还原 */
#define Print_CMD_Set_Font_Double		(0x57)


/************************************************************************************************/
/* 打印字体设置 */
#define Print_Font_12_12				(0x01)
#define Print_Font_16_16				(0x02)
/* 打印字体旋转角度 */
#define Print_Font_Rotate_Angle_0		(0x00)
#define Print_Font_Rotate_Angle_90		(0x01)
#define Print_Font_Rotate_Angle_180		(0x02)
#define Print_Font_Rotate_Angle_270		(0x03)


/************************************************************************************************/
/* 打印图片参数 */
/* 打印图片的数据寄存器大小） */
#define Print_Picture_Data_Size			(288u)


/************************************************************************************************/
/* 测试公司名称 */
#define Company_Name_Text_Str	"杭州泽大仪器有限公司"



/************************************************************************************************/
/* 公司名称:%s */
#define Company_Name_Str		"%s\r\n"
/* 设备号码:%04X%05d; */
#define Device_Number_Str		"设备号码:%04X%05d\r\n"
/* 打印时间段:\r\n */
#define Print_Time_Period_Str	"打印时间段:\r\n"
/* 起始时间:%04d年%02d月%02d日,%02d时%02d分%02d秒\r\n */
#define Print_Start_Time_Str	"起始时间:%04d年%02d月%02d日,%02d时%02d分\r\n"
/* 结束时间:%04d年%02d月%02d日,%02d时%02d分%02d秒\r\n */
#define Print_End_Time_Str		"结束时间:%04d年%02d月%02d日,%02d时%02d分\r\n"
/* 传感器通道%d温度范围(%02d.%01d℃-%02d.%01d℃)\r\n */
#define Print_sensor_Temp_Range_Str	"通道%d温度范围(%c%d.%01d℃到%c%d.%01d℃)\r\n"
/* 传感器通道%d湿度范围(%02d.%01d%%RH-%02d.%01d%%RH)\r\n */
#define Print_Sensor_Humi_Range_Str	"通道%d湿度范围(%d.%01d%%RH到%d.%01d%%RH)\r\n"
/* 时间项目条 */
#define Print_Time_Bar_Str			"- 时间 -"
/* 温度传感器项目条 */
#define Print_Temp_Bar_Str			" 温度%d"
/* 湿度传感器项目条 */
#define Print_Humi_Bar_Str			" 湿度%d"


/* 数据记录如下: */
#define Print_Record_Data_Str		"数据记录如下:\r\n"
/* 数据:送货方_____________ */
#define Print_Delivery_Party_Str	"送 货 方:%s\r\n"
/* 数据:收货方_____________ */
#define Print_Receiver_Party_Str	"收 货 方:\r\n"
/* 数据:车牌号_____________ */
#define Print_Card_NO_Str			"车 牌 号:%s\r\n"

/* 金卫信打印方案 */
#if (Print_Scheme == 1)

/* 金卫信定制产品“环境温度” */
#define Print_EnvironmentTemp_Bar_Str	"环境温度"

/* 金卫信定制产品“单据编号:” */
#define Print_FBillNo_Str				"单据编号:                    "
/* 金卫信定制产品“运单号:” */
#define Print_TransportNo_Str			"运单号:%04X%05d-%X-%X\r\n"


/* 金卫信定制产品“出库时间:” */
#define Print_StartOffTime_Str			"出库时间:%04d年%02d月%02d日,%02d时%02d分\r\n"

/* 金卫信定制产品“到达时间:” */
#define Print_ArriveTime_Str			"到达时间:%04d年%02d月%02d日,%02d时%02d分\r\n"




/* 金卫信打印方案“出库温度” */
#define Print_StartOffTemp_Str			"出库温度:"

/* 金卫信打印方案“到达温度” */
#define Print_ArriveTemp_Str			"到达温度:"

/* 金卫信定制产品“环境温度” */
#define Print_StartOffEnvironmentTemp_Bar_Str	"出库环境温度:"

/* 金卫信定制产品“环境温度” */
#define Print_ArriveEnvironmentTemp_Bar_Str		"到达环境温度:"

#endif

/************************************************************************************************/
/* 打印数据体的输出格式 */
/* 打印日期格式 */
#define Print_Date_Str				"<<<<<<日期: %04d年%02d月%02d日>>>>>>\r\n"
/* 时间段格式 */
#define Print_Data_Body_Time_Str	"%02d:%02d:%02d"
/* 数据段格式 */
#define Print_Data_Body_Data_Str	" %c%d.%01d"
/* 卡片 无数据 格式 */
#define Print_Data_Body_Rece_Timeout_Data_Str	" 缺失 "
/* 传感器数据最大值 */
#define Print_Sensor_Data_Max_Value_Str			"最大值:"
/* 传感器数据最小值 */
#define Print_Sensor_Data_Min_Value_Str			"最小值:"


/************************************************************************************************/
/* 打印错误信息 */
/* 错误信息1:搜索起始时间大于记录的结束时间. */
#define Print_Search_Start_Time_Than_Record_End_Time	"搜索起始时间大于记录的结束时间.\r\n"
/* 错误信息2:搜索结束时间小于记录的起始时间. */
#define Print_Search_End_Time_Than_Record_Start_Time	"搜索结束时间小于记录的起始时间.\r\n"
/* 错误信息3:搜索起始时间大于搜索结束时间. */
#define Print_Search_Start_Time_Than_Search_End_Time	"搜索起始时间大于搜索结束时间.\r\n"
/* 错误信息4:搜索起始时间大于搜索结束时间. */
#define Print_Set_Time_Range_Null_Data_Str				"设置的时间范围内没有记录数据.\r\n"


/************************************************************************************************/
/* 打印起始和结束的分割符 */
#define Print_Indivisible_Symbol_Start_Str	"------------------------\r\n"
#define Print_Indivisible_Symbol_End_Str	"************************\r\n"
/************************************************************************************************/

/************************************************************************************************/
/* 打印起始和结束的卸货符 */
#define Print_StartUnload_Symbol_Start_Str	"  *****卸货开始->%d*****  \r\n"
#define Print_StoptUnload_Symbol_End_Str	  "  *****卸货结束->%d*****  \r\n"
/************************************************************************************************/

/* 打印传感器数据最大/小值 */
typedef struct 
{
#if (Print_Sensor_CH_Num >= 1)
	/* 打印通道1的最值 */
	/* 最大值 */
	float Ch1_Max;
	/* 最小值 */
	float Ch1_Min;	
#endif	
	
#if (Print_Sensor_CH_Num >= 2)	
	/* 打印通道2的最值 */
	/* 最大值 */
	float Ch2_Max;
	/* 最小值 */
	float Ch2_Min;	
#endif	
	
#if (Print_Sensor_CH_Num >= 3)
	/* 打印通道3的最值 */
	/* 最大值 */
	float Ch3_Max;
	/* 最小值 */
	float Ch3_Min;	
#endif

#if (Print_Sensor_CH_Num >= 4)
	/* 打印通道4的最值 */
	/* 最大值 */
	float Ch4_Max;
	/* 最小值 */
	float Ch4_Min;	
#endif

}PRINT_SENSOR_MAX_MIN_VALUE; 
/************************************************************************************************/
#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
/* 二维码LOGO */
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

/* 出库温度/达到温度结构 */
typedef struct
{
	/* 时间 */
	Times	Time;
	
#if (Print_Sensor_CH_Num >= 1)
	/* 通道1 */
	float Ch1_Value;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* 通道2 */
	float Ch2_Value;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* 通道3(环境值) */
	float Ch3_Value;
#endif

#if (Print_Sensor_CH_Num >= 4)
	/* 通道4 */
	float Ch4_Value;
#endif
}Print_JWX_OutInTemp_Struct;

/* 出库温度 */
static Print_JWX_OutInTemp_Struct StartOffTemp = 
{
	/* 年 */
	0,
	/* 月 */
	0,
	/* 日 */
	0,
	/* 时 */
	0,
	/* 分 */
	0,
	/* 秒 */
	0,
	/* 星期 */
	0,
	/* 时区 */
	0,
#if (Print_Sensor_CH_Num >= 1)
	/* 通道1 */
	0,
#endif	
	
#if (Print_Sensor_CH_Num >= 2)	
	/* 通道2 */
	0,
#endif	
	
#if (Print_Sensor_CH_Num >= 3)
	/* 通道3(环境值) */
	0,
#endif

#if (Print_Sensor_CH_Num >= 4)
	/* 通道4 */
	0,
#endif
};

/* 金卫信打印方案 */
#if (Print_Scheme == 1)

/* 到达温度 */
static Print_JWX_OutInTemp_Struct ArriveTemp = 
{
	0,
#if (Print_Sensor_CH_Num >= 1)
	/* 通道1 */
	0,
#endif	
	
#if (Print_Sensor_CH_Num >= 2)	
	/* 通道2 */
	0,
#endif	
	
#if (Print_Sensor_CH_Num >= 3)
	/* 通道3(环境值) */
	0,
#endif

#if (Print_Sensor_CH_Num >= 4)
	/* 通道4 */
	0,
#endif
};

#endif


/*************************************************************************************************/
static BOOL Print_Get_Time_Data(INSTRU_PRINT_FIX* search_time,u32 adder,u32 *search_time_dr);

/*************************************************************************************************/

/*************************************************************************************************/
/* 函数名	: Print_Index_Save_Sensor_Data										 	 			 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 记录当前保存数据的系统时间和Flash存储地址. 									 	 	 */
/* 创建日期  : 2015/7/1																			 */
/*************************************************************************************************/
void Print_Index_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* 时间寄存器 */
	Times time_b;
	/* 起始时间数据指针 */
	u8* start_time_p;
	/* 起始时间 */
	u32 start_time;
	/* 起始记录地址 */
	u32 fifo_start_record_adder;
	
	/* 获取当前系统时间 */
	time_b.Year 	= config_tab->Inf_State.Instru_RTC.Inf_State.Year;
	time_b.Month 	= config_tab->Inf_State.Instru_RTC.Inf_State.Month;
	time_b.Day 		= config_tab->Inf_State.Instru_RTC.Inf_State.Day;
	time_b.Hour 	= config_tab->Inf_State.Instru_RTC.Inf_State.Hour;
	time_b.Min 		= config_tab->Inf_State.Instru_RTC.Inf_State.Min;
	time_b.Sec 		= config_tab->Inf_State.Instru_RTC.Inf_State.Sec;
//	time_b.Week 	= config_tab->Inf_State.Instru_RTC.Inf_State.Week;
//	time_b.Zone 	= config_tab->Inf_State.Instru_RTC.Inf_State.Zone;
	
	/* 将获取当前系统时间 转换成秒 作为起始时间 */
	start_time = RTC_Date_Time_To_Second(&time_b);
	/* 小于芯片容量 */
	/* 判断数据是否刚开始记录 */
	if((fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time == 0)&&(fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_End_Time == 0)&&
	   (fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder == 0)&&(fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder == 0))			
	{
		/* 设置 打印起始时间 */
		fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time = start_time;			
		/* 设置 打印起始数据地址 */
		fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder = 0;
	}
	
	/* 设置 打印结束时间 */
	fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_End_Time = start_time;
	/* 设置 打印结束数据地址 */
	fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder = ((s32)fix_tab->Inf_State.Instru_Flash.Inf_State.Write_Pointer+Flash_Capacity_Size-Save_Sensor_Data_Len)%Flash_Capacity_Size;
	
	/* 根据扇区指针设置起始记录地址 */
	fifo_start_record_adder = (fix_tab->Inf_State.Instru_Flash.Inf_State.Sector_Pointer + 2*Flash_Sector_Size)%Flash_Capacity_Size;
	
	
	/* 判断起始数据地址是否在被删除的FLASH扇区内。 */
	if(((fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder) >= fifo_start_record_adder)&&
	   ((fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder) < (fifo_start_record_adder+Flash_Sector_Size))&&
	    (fifo_start_record_adder <= fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder))
	{
		/* 起始数据地址在被擦除的FLASH扇区里面。 */
		/* 判断打印的起始数据地址 小于 Flash扇区地址 */
		while(fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder < (fifo_start_record_adder+Flash_Sector_Size))
		{
			/* 打印的起始数据地址 小于 Flash扇区地址 */
			/* 设置 打印起始数据地址 */
			fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder += Save_Sensor_Data_Len;
		}
		/* 设置 打印起始数据地址 */
		fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder = fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder%Flash_Capacity_Size;
		/* 读取起始地址下的时间作为 起始时间 */
		start_time_p = Flash_Read_Sensor_Data((fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Adder%Flash_Capacity_Size),sizeof(DATETIME_Storage_Format));
		/* 获取起始时间 */
		time_b.Year 	= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)start_time_p)->DATETIME_Bit.SECOND_BITS;
		/* 设置 打印起始时间 */
		fix_tab->Inf_State.Instru_Print.Inf_State.Print_Record_Start_Time = RTC_Date_Time_To_Second(&time_b);
	}
	
	
	/* 保存打印配置表参数 */
	Write_Config_Tab(fix_tab->Inf_State.Instru_Print.Inf_State_Space,sizeof(INSTRU_PRINT_FIX));
	
}

/*************************************************************************************************/
/* 函数名	: Print_Set_Font														 	 		 */
/* 输入		: 无																				 */
/* 输出		: 无																			 	 */
/* 作用		: 设置字体点阵大小															 	 	 */
/* n=1 12*12 点阵汉字打印																	 	 */
/* n=2 16*16 点阵汉字打印																	 	 */
/* 创建日期  : 2015/7/7																			 */
/* 注意  	:修改字体前需要确保打印机已经停止打印，否则会影响正在打印的字体。							 */
/*************************************************************************************************/
static BOOL Print_Set_Font(u8 font_size)
{
	/* 字体设置缓存大小 */
#define	set_font_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_font_data_buffer[set_font_data_buffer_size];
	
	/* 判断 选择字体 数据 是否合法 */
	if((font_size != 1)&&(font_size != 2))
	{
		/* 其他参数无效 */
		return FALSE;
	}
	
	/* 设置参数 */
	/* 命令头 */
	set_font_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:设置字体大小 */
	set_font_data_buffer[1] = Print_CMD_Set_Font_Size;
	/* 设置参数 */
	set_font_data_buffer[2] = font_size;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_font_data_buffer,set_font_data_buffer_size);
	
	return TRUE;

#undef	set_font_data_buffer_size
}

/*************************************************************************************************/
/* 函数名	: Print_Set_CRLF														 	 		 */
/* 输入		: 无																				 */
/* 输出		: 无																			 	 */
/* 作用		: 打印换行																 	 	 	*/
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_CRLF(void)
{

//	/* 输出回车换行 */
//	printf("\r\n");
	
	/* 字体设置缓存大小 */
#define	set_CRLF_data_buffer_size	(1u)
	/* 传输数据缓存 */
	char set_CRLF_data_buffer;
	
	/* 设置参数 */
	/* 命令:换行 */
	set_CRLF_data_buffer = Print_CMD_Set_CRLF;

	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)&set_CRLF_data_buffer,set_CRLF_data_buffer_size);
	
#undef	set_CRLF_data_buffer_size
}

/*************************************************************************************************/
/* 函数名	: Print_Set_Line_Space													 	 		 */
/* 输入		: 无																				 */
/* 输出		: 无																			 	 */
/* 作用		: 设置行间距																	 	 	 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Line_Space(u8 line_space)
{
	/* 字体设置缓存大小 */
#define	set_line_space_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_line_space_data_buffer[set_line_space_data_buffer_size];
	
	
	/* 设置参数 */
	/* 命令头 */
	set_line_space_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:行间距 */
	set_line_space_data_buffer[1] = Print_CMD_Set_Line_Space;
	/* 设置:间距数 */
	set_line_space_data_buffer[2] = line_space;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_line_space_data_buffer,set_line_space_data_buffer_size);
	
#undef	set_line_space_data_buffer_size

}

/*************************************************************************************************/
/* 函数名	: Print_Set_Word_Space													 	 		 */
/* 输入		: 无																				 */
/* 输出		: 无																			 	 */
/* 作用		: 设置字间距																	 	 	 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Word_Space(u8 word_space)
{
	/* 字体设置缓存大小 */
#define	set_word_space_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_word_space_data_buffer[set_word_space_data_buffer_size];
	
	
	/* 设置参数 */
	/* 命令头 */
	set_word_space_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:字间距 */
	set_word_space_data_buffer[1] = Print_CMD_Set_Word_Space;
	/* 设置:间距数 */
	set_word_space_data_buffer[2] = word_space;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_word_space_data_buffer,set_word_space_data_buffer_size);
	
#undef	set_word_space_data_buffer_size

}

#if (0)
/*************************************************************************************************/
/* 函数名	: Print_Set_Blank_line_Null												 	 		 */
/* 输入		: blank_line_state=0:设置空格；blank_line_state =1:设置空行。num:空格或空行的数量。		 */
/* 输出		: 无																			 	 */
/* 作用		: 打印空格或空行(这个空行功能好像没用？) 												 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Blank_Line_Null(u8 blank_line_state,u8 num)
{
	/* 字体设置缓存大小 */
#define	set_blank_line_null_data_buffer_size	(4u)
	/* 传输数据缓存 */
	char set_blank_line_null_data_buffer[set_blank_line_null_data_buffer_size];
	
	/* 判断 设置空行或空格状态 */
	if(blank_line_state != 0)
	{
		/* 非0 ，就是1，为设置空行。 */
		blank_line_state = 1;
	}
	
	/* 设置参数 */
	/* 命令头 */
	set_blank_line_null_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:空格或空行 */
	set_blank_line_null_data_buffer[1] = Print_CMD_Set_Blank_Line_Null;
	/* 设置:空格或空行 */
	set_blank_line_null_data_buffer[2] = blank_line_state;
	/* 设置:打印数量 */
	set_blank_line_null_data_buffer[3] = num;	
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_blank_line_null_data_buffer,set_blank_line_null_data_buffer_size);
	
#undef	set_blank_line_null_data_buffer_size
}
#endif

/*************************************************************************************************/
/* 函数名	: Print_Set_Right_Limit													 	 		 */
/* 输入		: offset:右限偏移量																	 */
/* 输出		: 无																			 	 */
/* 作用		: 设置右限 																	 	 	 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Right_Limit(u8 offset)
{
	/* 字体设置缓存大小 */
#define	set_right_limit_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_right_limit_data_buffer[set_right_limit_data_buffer_size];
	
	
	/* 设置参数 */
	/* 命令头 */
	set_right_limit_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:右限 */
	set_right_limit_data_buffer[1] = Print_CMD_Set_Right_Limit;
	/* 设置:偏移量 */
	set_right_limit_data_buffer[2] = offset;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_right_limit_data_buffer,set_right_limit_data_buffer_size);
	
#undef	set_right_limit_data_buffer_size
}

/*************************************************************************************************/
/* 函数名	: Print_Set_Left_Limit													 	 		 */
/* 输入		: offset:左限偏移量																	 */
/* 输出		: 无																			 	 */
/* 作用		: 设置左限 																	 	 	 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Set_Left_Limit(u8 offset)
{
	/* 字体设置缓存大小 */
#define	set_left_limit_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_left_limit_data_buffer[set_left_limit_data_buffer_size];
	
	
	/* 设置参数 */
	/* 命令头 */
	set_left_limit_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:左限 */
	set_left_limit_data_buffer[1] = Print_CMD_Set_Left_Limit;
	/* 设置:偏移量 */
	set_left_limit_data_buffer[2] = offset;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_left_limit_data_buffer,set_left_limit_data_buffer_size);
	
#undef	set_left_limit_data_buffer_size
}

/*************************************************************************************************/
/* 函数名	: Print_Set_Underline_Config													 	 */
/* 输入		: NewState=ENABLE;配置下划线；NewState=DISABLE;关闭下划线；							 */
/* 输出		: 无																			 	 */
/* 作用		: 打印允许/禁止下划线.														 	 	 */
/* 创建日期  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Set_Underline_Config(FunctionalState NewState)
{
	/* 字体设置缓存大小 */
#define	set_underline_config_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_underline_config_data_buffer[set_underline_config_data_buffer_size];
	
	
	/* 设置参数 */
	/* 命令头 */
	set_underline_config_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:允许/禁止下划线 */
	set_underline_config_data_buffer[1] = Print_CMD_Set_Underline_Config;
	/* 设置:下划线功能状态 */
	set_underline_config_data_buffer[2] = NewState;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_underline_config_data_buffer,set_underline_config_data_buffer_size);
	
#undef	set_underline_config_data_buffer_size

}

/*************************************************************************************************/
/* 函数名	: Print_Set_Underline_Config													 	 */
/* 输入		: Angle:字符旋转角度																	 */
/* 输出		: TRUE:设置成功；FALSE：设置失败。													 	 */
/* Angle=0 0 度 																		 	 	 */
/* Angle=1 90 度																		 	 	 */
/* Angle=2 180 度																		 	 	 */
/* Angle=3 270 度																		 	 	 */
/* 作用		: 设置字符旋转.																 	 	 */
/* 创建日期  : 2015/7/8																			 */
/*************************************************************************************************/
static BOOL Print_Set_Rotate_Char(u8 Angle)
{
	/* 字体设置缓存大小 */
#define	set_rotate_char_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_rotate_char_data_buffer[set_rotate_char_data_buffer_size];
	
	/* 判断角度参数合法性 */
	if(Angle > 3)
	{
		/* 角度参数非法，设置失败 */
		return FALSE;
	}
	
	/* 设置参数 */
	/* 命令头 */
	set_rotate_char_data_buffer[0] = Print_CMD_Head_0X1C;
	/* 命令:旋转字符 */
	set_rotate_char_data_buffer[1] = Print_CMD_Set_Rotate_Char;
	/* 设置:旋转角度 */
	set_rotate_char_data_buffer[2] = Angle;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_rotate_char_data_buffer,set_rotate_char_data_buffer_size);
	
	return FALSE;
	
#undef	set_rotate_char_data_buffer_size

}

/*************************************************************************************************/
/* 函数名	: Print_Separator_Line															 	 */
/* 输入		: 无																				 */
/* 输出		: 无													 	 						 */
/* 作用		: 打印分隔符（不连续的数据）													 	 	 */
/* 创建日期  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Separator_Line(void)
{
	
	/* 数据不连续，打印分割符号。 */
	/* 回车 */
	Print_Set_CRLF();
	/* 回车 */
	Print_Set_CRLF();
	/* 回车 */
	Print_Set_CRLF();
	/* 回车 */
	Print_Set_CRLF();
	/* 回车 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(1500);
}

/*************************************************************************************************/
/* 函数名	: Print_Init																	 	 */
/* 输入		: 无																				 */
/* 输出		: 无																			 	 */
/* 作用		: 初始化打印机																 	 	 */
/* 创建日期  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Init(void)
{
	/* 字体设置缓存大小 */
#define	print_init_buffer_size	(2u)
	/* 传输数据缓存 */
	char print_init_buffer[print_init_buffer_size];
	
	/* 设置参数 */
	/* 命令头 */
	print_init_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:初始化打印机 */
	print_init_buffer[1] = Print_CMD_Init;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)print_init_buffer,print_init_buffer_size);
	
	/* 数据发送完成，等待打印机设置命令。 */
	OSTimeDly(1);
#undef	print_init_buffer_size

}

/*************************************************************************************************/
/* 函数名	: Print_Set_Highlight_Char														 	 */
/* 输入		: NewState=ENABLE:使能反白显示；NewState=DISABLE:关闭反白显示。							 */
/* 输出		: 无																			 	 */
/* 作用		: 允许/禁止反白打印															 	 	 */
/* 创建日期  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Set_Highlight_Char(FunctionalState NewState)
{
	/* 字体设置缓存大小 */
#define	set_highlight_char_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_highlight_char_data_buffer[set_highlight_char_data_buffer_size];
	
	
	/* 设置参数 */
	/* 命令头 */
	set_highlight_char_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:允许/禁止反白打印 */
	set_highlight_char_data_buffer[1] = Print_CMD_Set_Highlight_Char;
	/* 设置:反白打印状态 */
	set_highlight_char_data_buffer[2] = NewState;
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_highlight_char_data_buffer,set_highlight_char_data_buffer_size);
	
#undef	set_highlight_char_data_buffer_size

}

#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
/*************************************************************************************************/
/* 函数名	: Print_Set_Picture																 	 */
/* 输入		: Len:图像数据长度；*Data:图像数据指针。												 */
/* 输出		: TRUE:设置成功；FALSE：设置失败。													 	 */
/* 作用		: 打印图像数据																 	 	 */
/* 创建日期  : 2015/7/8																			 */
/*************************************************************************************************/
static BOOL Print_Set_Picture(u16 Len,char *Data)
{
	/* 字体设置缓存大小 */
#define	set_picture_data_buffer_size	(4u)
	/* 传输数据缓存 */
	char set_picture_data_buffer[set_picture_data_buffer_size];
	
	/* 判断图像数据长度 是否超标 */
	if(Len > 48)
	{
		/* 图像数据长度超标 */
		return FALSE;
	}
	
	/* 设置参数 */
	/* 命令头 */
	set_picture_data_buffer[0] = Print_CMD_Head_0X1F;
	/* 命令:图像数据 */
	set_picture_data_buffer[1] = Print_CMD_Set_Picture;
	/* 设置低位数据长度 */
	set_picture_data_buffer[2] = Len%0xFF;	
	/* 设置高位数据长度 */
	set_picture_data_buffer[3] = Len/0xFF;
	
	/* 发送命令 */
	USART_printf(USART1,(uint8_t*)set_picture_data_buffer,set_picture_data_buffer_size);
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)Data,Len);	
	
	return TRUE;
	
#undef	set_picture_data_buffer_size

}
#endif

#if 0
/*************************************************************************************************/
/* 函数名	: Print_Set_Barcode																 	 */
/* 输入		: Mode=0:EAN-8;Mode=1:EAN-13;*Data:条形码数据指针。									 */
/* 输出		: 无																			 	 */
/* Mode=0 EAN-8																		 	 	 	 */
/* Mode=1 EAN-13																		 	 	 */
/* 作用		: 打印条形码																	 	 	 */
/* 创建日期  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Set_BarCode(u8 Mode,char *Data)
{
	/* 字体设置缓存大小 */
#define	set_barcode_data_buffer_size	(4u)
	/* 传输数据缓存 */
	char set_barcode_data_buffer[set_barcode_data_buffer_size];
	
	
	/* 判断 打印条形码模式 */
	if(Mode != 0)
	{
		Mode = 1;
	}
	
	/* 设置参数 */
	/* 命令头 */
	set_barcode_data_buffer[0] = Print_CMD_Head_0X1D;
	/* 命令:打印条形码 */
	set_barcode_data_buffer[1] = Print_CMD_Set_BarCode;
	
	/* 根据当前模式 设置打印的条形码类型 */
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

	/* 发送命令 */
	USART1_printf((uint8_t*)set_barcode_data_buffer,set_barcode_data_buffer_size);
	
	/* 发送数据 */
	USART1_printf((uint8_t*)Data,set_barcode_data_buffer[3]);	
	
#undef	set_barcode_data_buffer_size

}

#endif

/*************************************************************************************************/
/* 函数名	: Print_Set_Font_Double															 	 */
/* 输入		: State=RESET:正常字体；State=SET:字体大小翻倍。										 */
/* 输出		: 无																			 	 */
/* 作用		: 字体翻倍/还原																 	 	 */
/* 创建日期  : 2015/7/8																			 */
/* 注意  	:修改字体前需要确保打印机已经停止打印，否则会影响正在打印的字体。							 */
/*************************************************************************************************/
static void Print_Set_Font_Double(FlagStatus State)
{
	/* 字体设置缓存大小 */
#define	set_font_double_data_buffer_size	(3u)
	/* 传输数据缓存 */
	char set_font_double_data_buffer[set_font_double_data_buffer_size];
	
	/* 设置参数 */
	/* 命令头 */
	set_font_double_data_buffer[0] = Print_CMD_Head_0X1B;
	/* 命令:字体翻倍/还原 */
	set_font_double_data_buffer[1] = Print_CMD_Set_Font_Double;
	/* 设置:字体翻倍状态 */
	set_font_double_data_buffer[2] = State;
	
	
	/* 发送数据 */
	USART_printf(USART1,(uint8_t*)set_font_double_data_buffer,set_font_double_data_buffer_size);
	
#undef	set_font_double_data_buffer_size

}

/*************************************************************************************************/
/* 函数名	: Print_Set_Para													 	 			 */
/* 输入		: 无																				 */
/* 输出		: 无																			 	 */
/* 作用		: 打印数据标题																 	 	 */
/* 创建日期  : 2015/7/8																			 */
/*************************************************************************************************/
static void Print_Set_Para(void)
{


	/* 1、初始化打印机 */
	Print_Init();
	
	/* 2、设置字体点阵大小 */
	/* 字体大小不翻倍 */
	Print_Set_Font_Double(RESET);
	/* 设置为16*16大小的字体 */
	Print_Set_Font(Print_Font_16_16);
	
	/* 3、设置行间距 */
	Print_Set_Line_Space(8);
	
	/* 4、设置字间距 */
	Print_Set_Word_Space(0);
	
	/* 5、设置右限 */
	Print_Set_Right_Limit(0);	
	
	/* 6、设置左限 */
	Print_Set_Left_Limit(0);	
	
	/* 7、设置允许/禁止下划线. */
	Print_Set_Underline_Config(DISABLE);
		
	/* 8、设置字符旋转角度. */
	Print_Set_Rotate_Char(Print_Font_Rotate_Angle_0);	

	/* 9、设置允许/禁止反白打印. */
	Print_Set_Highlight_Char(DISABLE);		
	
	/* 10、设置打印回车换行. */
	Print_Set_CRLF();
	/* 等待打印结束 */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* 等待打印结束 */
	OSTimeDly(120);
	
}

/*************************************************************************************************/
/* 函数名	: Print_Indivisible_Symbol											 	 			 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 打印分割符																 	 	 	 */
/* 创建日期  : 2015/7/13																			 */
/*************************************************************************************************/
static void Print_Indivisible_Symbol(void)
{
	
	
	/* 字体大小翻倍 */
	Print_Set_Font_Double(SET);
	/* 设置为12*12大小的字体 */
	Print_Set_Font(Print_Font_12_12);
	/* 设置分隔符 */
	printf(Print_Indivisible_Symbol_Start_Str);
	OSTimeDly(200);
	printf(Print_Indivisible_Symbol_End_Str);
	OSTimeDly(200);
	
}

/*************************************************************************************************/
/* 函数名	: Print_Search_End_Time														 */
/* 输入		: 无																		 		 */
/* 输出		: 无		 																		 */
/* 作用		: 搜索结束时间及参数															 	 	 */
/* 创建日期  : 2016/09/10																		 */
/*************************************************************************************************/
static void Print_Search_End_Time(INSTRU_SENSOR* sensor_inf,Print_JWX_OutInTemp_Struct * Arrive_p,int start_adder,int end_adder)
{
	/* 读数据指针 */
	u8* data_p;
	/* 时间寄存器 */
	Times time_b;
	/* 秒时间寄存器 */
	u32 second_time_b;
	
	
#if (Print_Sensor_CH_Num >= 1)
	/* 通道1数据 */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* 通道2数据 */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* 通道3数据 */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* 通道4数据 */
	float ch_4_data;
#endif
	
	
	
	
	while(start_adder <= end_adder)
	{
		
		/* 读取起始地址数据 */
		data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		
			/* 获取 时间和数据 */
	#if (Print_Sensor_CH_Num >= 1)
		/* 获取通道1数据 */
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
		/* 获取通道2数据 */
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
		/* 获取通道3数据 */
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
		/* 获取通道4数据 */
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

		/* 获取时间数据 */
		time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
			
		/* 将时间转换成秒 */
		second_time_b = RTC_Date_Time_To_Second(&time_b);

		/* 判断当前数据状态 */
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
				/* 数据正常 */
				/* 判断当前的时间点是否为正常记录时间点 */
				if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl == 0)
				{
					/* 当前点设置为出库时间。 */
					#if (Print_Sensor_CH_Num >= 1)
							/* 数据1 */
							Arrive_p->Ch1_Value = ch_1_data;
					#endif
					#if (Print_Sensor_CH_Num >= 2)
							/* 数据2 */
							Arrive_p->Ch2_Value = ch_2_data;
					#endif
					#if (Print_Sensor_CH_Num >= 3)
							/* 数据3 */
							Arrive_p->Ch3_Value = ch_3_data;
					#endif
					#if (Print_Sensor_CH_Num >= 4)
							/* 数据4 */
							Arrive_p->Ch4_Value = ch_4_data;
					#endif
							/* 设置出库时间 */
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
				/* 数据超标 */ 
				/* 判断当前的时间点是否为报警记录时间点 */
				if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl == 0)
				{
					/* 当前点设置为出库时间。 */
					#if (Print_Sensor_CH_Num >= 1)
							/* 数据1 */
							Arrive_p->Ch1_Value = ch_1_data;
					#endif
					#if (Print_Sensor_CH_Num >= 2)
							/* 数据2 */
							Arrive_p->Ch2_Value = ch_2_data;
					#endif
					#if (Print_Sensor_CH_Num >= 3)
							/* 数据3 */
							Arrive_p->Ch3_Value = ch_3_data;
					#endif
					#if (Print_Sensor_CH_Num >= 4)
							/* 数据4 */
							Arrive_p->Ch4_Value = ch_4_data;
					#endif
							/* 设置出库时间 */
							Arrive_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
							Arrive_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
							Arrive_p->Time.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
							Arrive_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
							Arrive_p->Time.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
							Arrive_p->Time.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
					return;
				}
			}
		/* 设置下一个数据地址 */
		end_adder -= Save_Sensor_Data_Len;
	}
}

/*************************************************************************************************/
/* 函数名	: Print_Data_Title													 	 			 */
/* 输入		: sensor_inf:传感器通道信息；print_inf:打印信息										 */
/* 输出		: 无																			 	 */
/* 作用		: 打印数据标题																 	 	 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static void Print_Data_Title(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf,int start_adder,int end_adder)
{ 
	/* 时间数据缓存 */
	Times time_dr;
	
	
	/* 打印分割符 */
	Print_Indivisible_Symbol();
	
//	/* 公司名称:****** */	
//	printf(Company_Name_Str,Company_Name_Text_Str);
//	/* 等待标题打印结束 */
//	OSTimeDly(200);
	
	/* 字体大小不翻倍 */
	Print_Set_Font_Double(RESET);
	/* 设置为16*16大小的字体 */
	Print_Set_Font(Print_Font_16_16);
	/* 设备号码:****** */
	printf(Device_Number_Str,
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
/* 金卫信打印方案 */
#if (Print_Scheme == 1)
	/* 打印运单号:7DC311001-1F9314C0-1F946640 */
	printf(Print_TransportNo_Str,
		   ((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
		   ((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
		   ((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
		   ((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)),
		   print_inf->Inf_State.Print_Start_Time,
		   print_inf->Inf_State.Print_End_Time);
#endif
	
	/* 打印时间段:****** */
	printf(Print_Time_Period_Str);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
	/* 根据打印起始时间，设置时间数据寄存器 */
	RTC_Second_To_Date_Time(print_inf->Inf_State.Print_Start_Time,&time_dr);
	/* 起始时间:****** */
	printf(Print_Start_Time_Str,
	time_dr.Year,
	time_dr.Month,
	time_dr.Day,
	time_dr.Hour,
	time_dr.Min);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
	/* 判断是否有数据 */ 
	if((start_adder == 0)&&(end_adder == 0))
	{
		/* 根据打印结束时间，设置时间数据寄存器 */
		RTC_Second_To_Date_Time(print_inf->Inf_State.Print_Record_End_Time,&time_dr);
	}else
	{
		//华东医药提粗的需求：假如打印结束时间大于记录结束时间时，“打印结束时间”用记录结束时间；\
							 当打印结束时间小于记录结束时间时，“打印结束时间”用打印结束时间。
		Print_Search_End_Time(sensor_inf,&StartOffTemp,start_adder,end_adder);
		
		time_dr = StartOffTemp.Time;
	}
	
	/* 结束时间:****** */
	printf(Print_End_Time_Str,
	time_dr.Year,
	time_dr.Month,
	time_dr.Day,
	time_dr.Hour,
	time_dr.Min);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
	/* 打印传感器通道1的数据范围 */
#if (Print_Sensor_CH_Num >= 0x01)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
	/* 温度类型传感器 */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_1+1),
	(	  (sensor_inf->Inf_State.CH1.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH1.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH1.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH1.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* 湿度类型传感器 */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_1+1),
	((s16) sensor_inf->Inf_State.CH1.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Low_Limit*10)%10),	
	((s16) sensor_inf->Inf_State.CH1.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#endif
#endif

	/* 打印传感器通道2的数据范围 */
#if (Print_Sensor_CH_Num >= 0x02)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
	/* 温度类型传感器 */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_2+1),
	(	  (sensor_inf->Inf_State.CH2.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH2.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH2.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH2.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* 湿度类型传感器 */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_2+1),
	((s16) sensor_inf->Inf_State.CH2.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Low_Limit*10)%10),	
	((s16) sensor_inf->Inf_State.CH2.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#endif
#endif

	/* 打印传感器通道3的数据范围 */
#if (Print_Sensor_CH_Num >= 0x03)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
	/* 温度类型传感器 */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_3+1),
	(	  (sensor_inf->Inf_State.CH3.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH3.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH3.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH3.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* 湿度类型传感器 */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_3+1),
	((s16) sensor_inf->Inf_State.CH3.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Low_Limit*10)%10),
	((s16) sensor_inf->Inf_State.CH3.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#endif
#endif

	/* 打印传感器通道4的数据范围 */
#if (Print_Sensor_CH_Num >= 0x04)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
	/* 温度类型传感器 */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_4+1),
	(	  (sensor_inf->Inf_State.CH4.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH4.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH4.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH4.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* 湿度类型传感器 */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_4+1),
	((s16) sensor_inf->Inf_State.CH4.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Low_Limit*10)%10),
	((s16) sensor_inf->Inf_State.CH4.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#endif
#endif

	/* 数据记录如下:****** */
	printf(Print_Record_Data_Str);
	/* 等待标题打印结束 */
	OSTimeDly(200);

}

/*************************************************************************************************/
/* 函数名	: Print_Get_Time_Data												 	 			 */
/* 输入		: search_time,adder,search_time_dr													 */
/* 输出		: TRUE:获取时间成功，FALSE:获取时间失败。											 	 */
/* 作用		: 根据地址获取存储在FLASH中的传感器数据时间戳。									 	 	 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static BOOL Print_Get_Time_Data(INSTRU_PRINT_FIX* search_time,u32 adder,u32 *search_time_dr)
{
	/* 时间数据指针 */
	u8 * time_p;
	/* 时间数据缓存 */
	Times time_dr;
	
	/* 获取 时间数据 */
	time_p = Flash_Read_Sensor_Data(adder%Flash_Capacity_Size,sizeof(DATETIME_Storage_Format));

	/* 转换时间格式 */
	time_dr.Year 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.YEAR_BITS + 2000;
	time_dr.Month 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.MONTH_BITS;
	time_dr.Day 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.DAY_BITS;
	time_dr.Hour 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.HOUR_BITS;
	time_dr.Min 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.MINUTE_BITS;
	time_dr.Sec 	= ((DATETIME_Storage_Format*)time_p)->DATETIME_Bit.SECOND_BITS;
	
	/* 将时间数据转换成秒数据 */
	*search_time_dr = RTC_Date_Time_To_Second(&time_dr);
	
	/* 判断时间数据是否正常 */
	if(*search_time_dr < search_time->Inf_State.Print_Record_Start_Time)
	{
		/* 数据小于记录起始时间 */
		return FALSE;
	}else if(*search_time_dr > search_time->Inf_State.Print_Record_End_Time)
	{
		/* 数据大于记录结束时间 */
		return FALSE;
	}else
	{
		/* 数据在记录起始时间和记录结束时间之间。 */
		return TRUE;
	}

}

/*************************************************************************************************/
/* 函数名	: Print_Search_Start_Adder													 	 	 */
/* 输入		: search_time,search_adder															 */
/* 输出		: TRUE:获取地址成功，FALSE:获取地址失败。											 	 */
/* 作用		: 根据时间索打印数据在FLASH中的地址位置											 	 	 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static 	BOOL Print_Search_Start_Adder(INSTRU_PRINT_FIX* search_time,u32 *start_adder)
{
	/* 搜索 开始 位置 */
	u32 search_start_adder;
	/* 搜索 结束 位置 */
	u32 search_end_adder;
	/* 搜索 中间地址 位置 */
	u32 search_centre_adder;
	/* 搜索 范围 */
	u32 search_range_max = Flash_Capacity_Size/Save_Sensor_Data_Len;
	/* 搜索的时间数据 */
	u32 search_time_dr;
	/* 搜索的时间结果 */
	BOOL search_time_result;
	/* 计数器 */
	u8 num;
	

	/* 判断起始时间在那个区间范围 */
	if(search_time->Inf_State.Print_Start_Time <= search_time->Inf_State.Print_Record_Start_Time)
	{
		/* 搜索的时间小于等于起始记录时间，用起始记录地址作为搜索的起始地址。 */
		*start_adder = search_time->Inf_State.Print_Record_Start_Adder;
		/* 成功 */
		return TRUE;
	}else if((search_time->Inf_State.Print_Start_Time <= search_time->Inf_State.Print_Record_End_Time)&&
			 (search_time->Inf_State.Print_Start_Time >= search_time->Inf_State.Print_Record_Start_Time))
	{
		/* 搜索的时间小于等于结束记录时间、大于等于起始记录时间。 */
		/* 设置搜索开始位置 */
		search_start_adder = search_time->Inf_State.Print_Record_Start_Adder;
		/* 设置搜索结束位置 */
		if(search_time->Inf_State.Print_Record_End_Adder < search_time->Inf_State.Print_Record_Start_Adder)
		{
			/* 结束记录地址数据 小于 起始记录地址数据 */
			search_end_adder = search_time->Inf_State.Print_Record_End_Adder + Flash_Capacity_Size;
		}else
		{
			/* 结束记录地址数据 大于等于 起始记录地址数据 */
			search_end_adder = search_time->Inf_State.Print_Record_End_Adder;	
		}
		/* 判断起始地址参数和结束地址参数的合法性 */
		if(search_end_adder < search_start_adder)
		{
			/* 系统异常 */
			return FALSE;
		}
		
		/* 开始搜索起始地址 */
		for(num = 0 ; num < search_range_max ; num ++)
		{
			
			/* 判断搜索的起始地址和结束地址是否小于等于一个数据长度 */
			if((search_end_adder-search_start_adder) > Save_Sensor_Data_Len)
			{
				/* 获取 中间点 地址 */				
				search_centre_adder = ((search_end_adder - search_start_adder)/2/Save_Sensor_Data_Len)*Save_Sensor_Data_Len + search_start_adder;
				/* 获取 中间点 时间数据 */
				search_time_result = Print_Get_Time_Data(search_time,search_centre_adder,&search_time_dr);
				
				/* 判断搜索时间是否正常 */
				if(search_time_result == FALSE)
				{
					/* 不正常,搜索失败 */
					return FALSE;
				}
				
				/* 搜索中间点的时间数据 与 查询的时间是否相等 */
				if(search_time_dr > search_time->Inf_State.Print_Start_Time)
				{
					/* 搜索的时间大于起始时间 *///这样写是为了去掉有可能的小数部分。
					search_end_adder = search_centre_adder;
				}
				else if(search_time_dr < search_time->Inf_State.Print_Start_Time)
				{
					/* 搜索的时间大于起始时间 *///这样写是为了去掉有可能的小数部分。
					search_start_adder = search_centre_adder;
				}
				else
				{
					/* 成功获得起始地址 */
					*start_adder = search_centre_adder;
					/* 成功 */
					return TRUE;
				}
			
			}else
			{
				/* 起始地址和结束地址相邻，没有更加合适的起始地址了。选择结束地址作为搜索起始地址 */
				*start_adder = search_end_adder;			
				/* 成功 */
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*************************************************************************************************/
/* 函数名	: Print_Search_End_Adder													 	 	 */
/* 输入		: search_time,search_adder															 */
/* 输出		: TRUE:获取地址成功，FALSE:获取地址失败。											 	 */
/* 作用		: 根据时间索打印数据在FLASH中的地址位置											 	 	 */
/* 创建日期  : 2015/7/7																			 */
/*************************************************************************************************/
static 	BOOL Print_Search_End_Adder(INSTRU_PRINT_FIX* search_time,u32 *end_adder)
{
	/* 搜索 开始 位置 */
	u32 search_start_adder;
	/* 搜索 结束 位置 */
	u32 search_end_adder;
	/* 搜索 中间地址 位置 */
	u32 search_centre_adder;
	/* 搜索 范围 */
	u32 search_range_max = Flash_Capacity_Size/Save_Sensor_Data_Len;
	/* 搜索的时间数据 */
	u32 search_time_dr;
	/* 搜索的时间结果 */
	BOOL search_time_result;
	/* 计数器 */
	u8 num;
	

	/* 判断起始时间在那个区间范围 */
	if(search_time->Inf_State.Print_End_Time >= search_time->Inf_State.Print_Record_End_Time)
	{
		/* 设置搜索结束位置 */
		if(search_time->Inf_State.Print_Record_End_Adder < search_time->Inf_State.Print_Record_Start_Adder)
		{
			/* 结束记录地址数据 小于 起始记录地址数据 */
			*end_adder = search_time->Inf_State.Print_Record_End_Adder + Flash_Capacity_Size;		
		}else
		{
			/* 搜索的时间小于等于起始记录时间，用起始记录地址作为搜索的起始地址。 */
			*end_adder = search_time->Inf_State.Print_Record_End_Adder;		
		}

		/* 成功 */
		return TRUE;
	}else if((search_time->Inf_State.Print_End_Time <= search_time->Inf_State.Print_Record_End_Time)&&
			 (search_time->Inf_State.Print_End_Time >= search_time->Inf_State.Print_Record_Start_Time))
	{
		/* 搜索的时间小于等于结束记录时间、大于等于起始记录时间。 */
		/* 设置搜索开始位置 */
		search_start_adder = search_time->Inf_State.Print_Record_Start_Adder;
		/* 设置搜索结束位置 */
		if(search_time->Inf_State.Print_Record_End_Adder < search_time->Inf_State.Print_Record_Start_Adder)
		{
			/* 结束记录地址数据 小于 起始记录地址数据 */
			search_end_adder = search_time->Inf_State.Print_Record_End_Adder + Flash_Capacity_Size;
		}else
		{
			/* 结束记录地址数据 大于等于 起始记录地址数据 */
			search_end_adder = search_time->Inf_State.Print_Record_End_Adder;	
		}
		/* 判断起始地址参数和结束地址参数的合法性 */
		if(search_end_adder < search_start_adder)
		{
			/* 系统异常 */
			return FALSE;
		}
		
		/* 开始搜索起始地址 */
		for(num = 0 ; num < search_range_max ; num ++)
		{
			/* 判断搜索的起始地址和结束地址是否小于等于一个数据长度 */
			if((search_end_adder-search_start_adder) > Save_Sensor_Data_Len)
			{

				/* 获取 中间点 地址 */				
				search_centre_adder = ((search_end_adder - search_start_adder)/2/Save_Sensor_Data_Len)*Save_Sensor_Data_Len + search_start_adder;
				
				/* 获取 中间点 时间数据 */
				search_time_result = Print_Get_Time_Data(search_time,search_centre_adder,&search_time_dr);
				
				/* 判断搜索时间是否正常 */
				if(search_time_result == FALSE)
				{
					/* 不正常,搜索失败 */
					return FALSE;
				}
				
				/* 搜索中间点的时间数据 与 查询的时间是否相等 */
				if(search_time_dr > search_time->Inf_State.Print_End_Time)
				{
					/* 搜索的时间大于起始时间 *///这样写是为了去掉有可能的小数部分。
					search_end_adder = search_centre_adder;
				}
				else if(search_time_dr < search_time->Inf_State.Print_End_Time)
				{
					/* 搜索的时间大于起始时间 *///这样写是为了去掉有可能的小数部分。
					search_start_adder = search_centre_adder;
				}
				else
				{
					/* 成功获得起始地址 */
					*end_adder = search_centre_adder;
					/* 成功 */
					return TRUE;
				}
			
			}else
			{
				/* 起始地址和结束地址相邻，没有更加合适的起始地址了。选择结起始址作为搜索起始地址 */
				*end_adder = search_start_adder;
				/* 成功 */
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*************************************************************************************************/
/* 函数名	: Print_Date														 	 			 */
/* 输入		: date_p:日期数据指针。														 		 */
/* 输出		: 无 																				 */
/* 作用		: 打印日期																	 	 	 */
/* 创建日期  : 2015/7/13																			 */
/*************************************************************************************************/	
static void	Print_Date(u8* date_p)
{

	/* 打印日期 */
	printf(Print_Date_Str,
	((DATETIME_Storage_Format*)date_p)->DATETIME_Bit.YEAR_BITS+2000,
	((DATETIME_Storage_Format*)date_p)->DATETIME_Bit.MONTH_BITS,
	((DATETIME_Storage_Format*)date_p)->DATETIME_Bit.DAY_BITS);
	/* 等待标题打印结束 */
	OSTimeDly(200);

}

/*************************************************************************************************/
/* 函数名	: Print_Data_Body													 	 			 */
/* 输入		: sensor_inf:传感器通道信息,start_adder:数据起始地址;end_adder:数据拘束地址。			 */
/* 输出		: 无 																				 */
/* 作用		: 输出数据体内容（传感器数据）													 	 	 */
/* 创建日期  : 2015/7/9																			 */
/*************************************************************************************************/
static void Print_Data_Body(INSTRU_SENSOR* sensor_inf,PRINT_SENSOR_MAX_MIN_VALUE* print_sensor_max_min_value,u32 start_adder,u32 end_adder)
{
	
	/* 一天时间的秒钟数，这里用于判断打印的日期是否超过一天 */
#define Day_Second_Num			(86400uL)
	/* 时间寄存器 */
	Times time_b;
	/* 秒时间寄存器 */
	u32 second_time_b;
	/* 前一次打印时间 */
	u32 beforeprinttime = 0;
	/* 读数据指针 */
	u8* data_p;
	u8  Temp=0;
	/* 首次打印日期标志 */
	BOOL print_date_flag = TRUE;
	/* 记录上次打印日期数据 */
	static u32 print_date_dr = 0;
	
#if (Print_Sensor_CH_Num >= 1)
	/* 通道1数据 */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* 通道2数据 */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* 通道3数据 */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* 通道4数据 */
	float ch_4_data;
#endif
	
	
	/* 设置时间的项目条 */
	printf(Print_Time_Bar_Str);
#if (Print_Sensor_CH_Num >= 1)
	/* 设置传感器1项目条 */
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
	/* 设置传感器2项目条 */
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
	/* 设置传感器3项目条 */
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
	/* 设置传感器4项目条 */
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
	/* 回车 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
  
	
	/* 循环读取传感器数据 */
	while(start_adder <= end_adder)
	{
		/* 读取起始地址数据 */
		data_p = Flash_Read_Sensor_Data(start_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		/* 设置下一个数据地址 */
		start_adder += Save_Sensor_Data_Len;
		/* 获取 时间和数据 */
#if (Print_Sensor_CH_Num >= 1)
	/* 获取通道1数据 */
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
	/* 获取通道2数据 */
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
	/* 获取通道3数据 */
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
	/* 获取通道4数据 */
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
		
		/* 获取时间数据 */
		time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
		/* 将时间转换成秒 */
		second_time_b = RTC_Date_Time_To_Second(&time_b);
		
		/* 判断通道状态 */
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
			/* 通道正常状态 */
			/* 判断 报警时间是否为 报警时间的倍数 */
			if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl != 0)
			{
				/* 判断时间间隔是否小于报警时间间隔 */
				if((second_time_b - beforeprinttime)<Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
				{
					
				/*********************打印是否卸货字符串**************************/
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
					
					
					/* 不打印这类数据 */
					/* 从头开始执行 */
					continue;
				}
			}
		}else
		{
			/* 通道报警状态 */
			/* 判断 报警时间是否为 正常时间的倍数 */
			if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl != 0)
			{
				/* 判断时间间隔是否小于报警时间间隔 */
				if((second_time_b - beforeprinttime)<Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
				{
					
					/*********************打印是否卸货字符串**************************/
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
					
					/* 不打印这类数据 */
					/* 从头开始执行 */
					continue;
				}
			}
		}
		
		/* 打印读取的日期数据 */
		if((print_date_flag == TRUE)||(second_time_b / Day_Second_Num) != print_date_dr)
		{
			/* 只有2中条件下需要打印日期：1、打印第一条数据;2、打印时间间隔1天以上 */
			/* 判断 打印日期标志 */
			if(print_date_flag == TRUE)
			{
				/* 清除日期标志  */
				print_date_flag = FALSE;
				/* 设置最值寄存器数据 */
#if (Print_Sensor_CH_Num >= 1)
				/* 设置最值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
				/* 设置最值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
#endif	
	
#if (Print_Sensor_CH_Num >= 2)	
				/* 设置最值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
				/* 设置最值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
#endif	
	
#if (Print_Sensor_CH_Num >= 3)
				/* 设置最值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
				/* 设置最值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
#endif

#if (Print_Sensor_CH_Num >= 4)
				/* 设置最值 */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
				/* 设置最值 */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
#endif
			}
			/* 判断打印日期超过一天 */
			if((second_time_b / Day_Second_Num) != print_date_dr)
			{
				/* 设置新的打印日期 */
				print_date_dr = (second_time_b / Day_Second_Num);
			}
			/* 打印日期 */
			Print_Date(data_p);
		}
		
	/*********************打印是否卸货字符串**************************/
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
		
		
		/* 打印读取的时间数据 */
		printf(Print_Data_Body_Time_Str,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS);

		
#if (Print_Sensor_CH_Num == 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
		/* 判断数据是否超标 */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),	
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}		else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
			/* 判断数据是否超标 */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器1数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* 判断数据是否超标 */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器2数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}		else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_Hum))
		/* 判断数据是否超标 */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);
		}
		
		/* 打印读取的传感器1数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* 判断数据是否超标 */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器2数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
		
		/* 判断数据是否超标 */
		if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器3数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_3_data >= 0.0) ? ' ':'-'),
		((u16) ch_3_data),
		((u16)(ch_3_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch3_Max = ch_3_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch3_Min = ch_3_data;
		}
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_3_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_Hum))
		/* 判断数据是否超标 */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);
		}
		/* 打印读取的传感器1数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* 判断数据是否超标 */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器2数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
		
		/* 判断数据是否超标 */
		if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器3数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_3_data >= 0.0) ? ' ':'-'),
		((u16) ch_3_data),
		((u16)(ch_3_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch3_Max = ch_3_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch3_Min = ch_3_data;
		}
		/* 判断数据是否超标 */
		if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器4数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_4_data >= 0.0) ? ' ':'-'),
		((u16) ch_4_data),
		((u16)(ch_4_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch4_Max = ch_4_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch4_Min = ch_4_data;
		}
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{	
			/* 判断数据是否超标 */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_4_data >= 0.0) ? ' ':'-'),
			((u16) ch_4_data),
			((u16)(ch_4_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
			}
		}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_3_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{	
			/* 判断数据是否超标 */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_4_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_4_data >= 0.0) ? ' ':'-'),
			((u16) ch_4_data),
			((u16)(ch_4_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
			}
		}
	#endif
#endif
		
		/* 数据未超标,设置禁止下划线. */
		Print_Set_Underline_Config(DISABLE);
		/* 换行 */
		Print_Set_CRLF();
		/* 等待标题打印结束 */
		OSTimeDly(120);
		/* 将当前时间设置为过去的打印时间 */
		beforeprinttime = second_time_b;
	}
	
#undef Day_Second_Num
}

#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
/*************************************************************************************************/
/* 函数名	: Print_Picture													 	 			 	 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 打印一张图片（大小:6*6字节数据）														 */
/* 创建日期  : 2015/7/10																			 */
/*************************************************************************************************/
static void Print_Picture(char *picture_data_p)
{
/* 打印图片的X轴像素大小 */
#define Print_Picture_Xpos_Size			(384u)
/* 打印图片的数据缓存大小 */
#define Print_Picture_Buffer_Size		(Print_Picture_Xpos_Size/8u)
/* 一个压缩字节有8个像素点数据 */		
#define Picture_Compression_Ratio		(8u)	
/* 保存一行图片数据需要6字节的空间 */
#define Picture_Data_Line_Size			(Print_Picture_Buffer_Size/Picture_Compression_Ratio)	
/* 一张图片需要发送48次像素行 */
#define Print_Picture_Data_Line_Num		(Print_Picture_Data_Size/Picture_Data_Line_Size)
	
	/* 图片数据缓存 */
	char picture_data_buffer[Print_Picture_Buffer_Size];
	/* 计数器 */
	u8 num1,num2,num3,num4;

	
	/* 发送图片数据 */
	for(num4 = 0;num4 < Print_Picture_Data_Line_Num; num4++)
	{
		/* 按像素行发送数据 */
		for(num3 = 0;num3 < Picture_Data_Line_Size; num3++)
		{
			/* 将原始的压缩数据解压到数据缓存中 */
			for(num2 = 0;num2 < Picture_Compression_Ratio; num2++)
			{
				/* 提取数据 */
				if(((*(picture_data_p+num3+num4*Picture_Data_Line_Size) << num2)&0x80) == 0x80)
				{
					picture_data_buffer[num2+num3*Picture_Compression_Ratio] = 0xFF;
				}else
				{
					picture_data_buffer[num2+num3*Picture_Compression_Ratio] = 0x00;
				}
				
			}	
		}
		
		/* 每一行像素行发送8次 */
		for(num1 = 0;num1 < Picture_Compression_Ratio; num1++)
		{
			/* 打印一行图片数据 */
			Print_Set_Picture(Print_Picture_Buffer_Size,picture_data_buffer);
		}
	}

/* 打印图片的X轴像素大小 */
#undef Print_Picture_Xpos_Size
/* 打印图片的数据缓存大小 */
#undef Print_Picture_Buffer_Size
/* 一个压缩字节有8个像素点数据 */		
#undef Picture_Compression_Ratio	
/* 保存一行图片数据需要6字节的空间 */
#undef Picture_Data_Line_Size
/* 一张图片需要发送48次像素行 */
#undef Print_Picture_Data_Line_Num
}
#endif

/*************************************************************************************************/
/* 函数名	: Print_Data_Tail													 	 			 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 输出数据尾内容																 	 	 */
/* 创建日期  : 2015/7/10																			 */
/*************************************************************************************************/
static void Print_Data_Tail(PRINT_SENSOR_MAX_MIN_VALUE* print_sensor_max_min_value)
{

	/* 1、空2行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	
	/* 2、打印数据最大/小值. */
	/* 打印数据最大值 */
	printf(Print_Sensor_Data_Max_Value_Str);
	/* 打印读取的传感器数据 */
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* 打印传感器通道1最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch1_Max),
	((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch1_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Max),
		((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch1_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Max),
		((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
	/* 打印传感器通道2最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch2_Max),
	((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch2_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Max),
		((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch2_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Max),
		((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
	/* 打印传感器通道3最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch3_Max),
	((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch3_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Max),
		((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}	
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch3_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Max),
		((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||(Sensor_CH4_Config_Type == Sensor_Hum))
	/* 打印传感器通道4最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch4_Max),
	((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch4_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Max),
		((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch4_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Max),
		((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	/* 回车 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
	/* 打印数据最小值. */
	printf(Print_Sensor_Data_Min_Value_Str);
	/* 打印读取的传感器数据 */
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* 打印传感器通道1最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch1_Min),
	((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch1_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Min),
		((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch1_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Min),
		((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
	/* 打印传感器通道2最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch2_Min),
	((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch2_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Min),
		((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch2_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Min),
		((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_Hum))
	/* 打印传感器通道3最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch3_Min),
	((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch3_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Min),
		((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}	
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch3_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Min),
		((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_Hum))
	/* 打印传感器通道4最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch4_Min),
	((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch4_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Min),
		((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch4_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Min),
		((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);
	}
	#endif
#endif
	/* 回车 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(200);	
	
	/* 空1行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);

	
#if (Print_Scheme == 0)
/* 默认方案 */
	
	/* 3、打印送货方数据. */
	/* 设置允许下划线. */
	Print_Set_Underline_Config(ENABLE);
	/* 打印送货方数据. */
	printf(Print_Delivery_Party_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	/* 打印收货方数据. */
	printf(Print_Receiver_Party_Str);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	
	
	/* 4、打印 车牌号. */	
	printf(Print_Card_NO_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
#elif (Print_Scheme == 1)
///* 金卫信方案 */

//	/* 3、打印 出库温度. */	
//	/* 设置允许下划线. */
//	Print_Set_Underline_Config(ENABLE);
//	
//	/* 打印 出库时间 */
//	//
//	printf(Print_StartOffTime_Str,
//		   StartOffTemp.Time.Year,
//		   StartOffTemp.Time.Month,
//		   StartOffTemp.Time.Day,
//		   StartOffTemp.Time.Hour,
//		   StartOffTemp.Time.Min);
//	/* 等待打印结束 */
//	OSTimeDly(120);
//	/* 打印 出库温度. */
//	printf(Print_StartOffTemp_Str);
//	
//#if (Print_Sensor_CH_Num >= 1)
//	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||(Sensor_CH1_Config_Type == Sensor_Hum))
//	/* 打印传感器通道1最大值 */
//	printf(Print_Data_Body_Data_Str,
//	(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
//	((u16) StartOffTemp.Ch1_Value),
//	((u16)(StartOffTemp.Ch1_Value*10)%10));
//	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* 判断 传感器数据是否为 超时接收数据 */
//	if(StartOffTemp.Ch1_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
//		((u16) StartOffTemp.Ch1_Value),
//		((u16)(StartOffTemp.Ch1_Value*10)%10));
//	}else
//	{
//		/* 超时接收数据 打印缺失 */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}
//	#endif
//#endif
//	
//#if (Print_Sensor_CH_Num >= 2)
//	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
//	/* 打印传感器通道2最大值 */
//	printf(Print_Data_Body_Data_Str,
//	(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
//	((u16) StartOffTemp.Ch2_Value),
//	((u16)(StartOffTemp.Ch2_Value*10)%10));
//	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* 判断 传感器数据是否为 超时接收数据 */
//	if(StartOffTemp.Ch2_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
//		((u16) StartOffTemp.Ch2_Value),
//		((u16)(StartOffTemp.Ch2_Value*10)%10));
//	}else
//	{
//		/* 超时接收数据 打印缺失 */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}
//	#endif
//	/* 空1行 */
//	Print_Set_CRLF();
//	/* 等待标题打印结束 */
//	OSTimeDly(120);
//#endif
//	
//#if (Print_Sensor_CH_Num >= 3)
//	/* 打印 环境温度. */
//	printf(Print_StartOffEnvironmentTemp_Bar_Str);
//	
//	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
//	/* 打印传感器通道3最大值 */
//	printf(Print_Data_Body_Data_Str,
//	(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
//	((u16) StartOffTemp.Ch3_Value),
//	((u16)(StartOffTemp.Ch3_Value*10)%10));
//	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* 判断 传感器数据是否为 超时接收数据 */
//	if(StartOffTemp.Ch3_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
//		((u16) StartOffTemp.Ch3_Value),
//		((u16)(StartOffTemp.Ch3_Value*10)%10));
//	}else
//	{
//		/* 超时接收数据 打印缺失 */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}	
//	#endif
//#endif
//	/* 等待标题打印结束 */
//	OSTimeDly(200);
//	/* 空1行 */
//	Print_Set_CRLF();
//	/* 等待标题打印结束 */
//	OSTimeDly(120);
//	Print_Set_CRLF();
//	/* 等待标题打印结束 */
//	OSTimeDly(120);
//	
//	/* 4、打印 到达温度. */
//	
//	/* 打印 到达时间 */
//	//
//	printf(Print_ArriveTime_Str,
//		   ArriveTemp.Time.Year,
//		   ArriveTemp.Time.Month,
//		   ArriveTemp.Time.Day,
//		   ArriveTemp.Time.Hour,
//		   ArriveTemp.Time.Min);
//	/* 等待打印结束 */
//	OSTimeDly(120);
//	/* 打印 到达温度. */
//	printf(Print_ArriveTemp_Str);
//	
//#if (Print_Sensor_CH_Num >= 1)
//	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||(Sensor_CH1_Config_Type == Sensor_Hum))
//	/* 打印传感器通道1最大值 */
//	printf(Print_Data_Body_Data_Str,
//	(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
//	((u16) ArriveTemp.Ch1_Value),
//	((u16)(ArriveTemp.Ch1_Value*10)%10));
//	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* 判断 传感器数据是否为 超时接收数据 */
//	if(ArriveTemp.Ch1_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
//		((u16) ArriveTemp.Ch1_Value),
//		((u16)(ArriveTemp.Ch1_Value*10)%10));
//	}else
//	{
//		/* 超时接收数据 打印缺失 */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}
//	#endif
//#endif
//	
//#if (Print_Sensor_CH_Num >= 2)
//	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
//	/* 打印传感器通道2最大值 */
//	printf(Print_Data_Body_Data_Str,
//	(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
//	((u16) ArriveTemp.Ch2_Value),
//	((u16)(ArriveTemp.Ch2_Value*10)%10));
//	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* 判断 传感器数据是否为 超时接收数据 */
//	if(ArriveTemp.Ch2_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
//		((u16) ArriveTemp.Ch2_Value),
//		((u16)(ArriveTemp.Ch2_Value*10)%10));
//	}else
//	{
//		/* 超时接收数据 打印缺失 */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}
//	#endif
//	/* 空1行 */
//	Print_Set_CRLF();
//	/* 等待标题打印结束 */
//	OSTimeDly(120);
//#endif
//	
//#if (Print_Sensor_CH_Num >= 3)
//	/* 打印 环境温度. */
//	printf(Print_ArriveEnvironmentTemp_Bar_Str);
//	
//	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
//	/* 打印传感器通道3最大值 */
//	printf(Print_Data_Body_Data_Str,
//	(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
//	((u16) ArriveTemp.Ch3_Value),
//	((u16)(ArriveTemp.Ch3_Value*10)%10));
//	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
//	/* 判断 传感器数据是否为 超时接收数据 */
//	if(ArriveTemp.Ch3_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
//	{
//		printf(Print_Data_Body_Data_Str,
//		(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
//		((u16) ArriveTemp.Ch3_Value),
//		((u16)(ArriveTemp.Ch3_Value*10)%10));
//	}else
//	{
//		/* 超时接收数据 打印缺失 */
//		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
//	}	
//	#endif
//#endif
//	/* 等待标题打印结束 */
//	OSTimeDly(200);
//	/* 空1行 */
//	Print_Set_CRLF();
//	/* 等待标题打印结束 */
//	OSTimeDly(120);
//	Print_Set_CRLF();
//	/* 等待标题打印结束 */
//	OSTimeDly(120);
	
//	/* 5、打印 单据编号. */	
//	printf(Print_FBillNo_Str);
//	/* 等待标题打印结束 */
//	OSTimeDly(200);
//	/* 空1行. */	
//	Print_Set_CRLF();
//	/* 等待标题打印结束 */
//	OSTimeDly(120);

	//现在有GPS信息配置信息来判断是否为车载设备
#if (GPS_Config_Enable == 0x01)
	/* 4、打印 车牌号. */	
	printf(Print_Card_NO_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
#endif

#endif

#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
	/* 5、打印公司LOGO二维码. */
	Print_Picture((char*)Print_LOGO);
#endif

	/* 6、设置禁止下划线. */
	Print_Set_Underline_Config(DISABLE);
	/* 打印分割符 */
	Print_Indivisible_Symbol();
	
}


/*************************************************************************************************/
/* 函数名	: Print_Data_Output													 	 			 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 输出打印数据																 	 	 */
/* 创建日期  : 2015/7/6																			 */
/*************************************************************************************************/
void Print_Data_Output(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf)
{
	/* 打印的起始地址 */
	u32 print_start_adder = 0;
	/* 打印的结束地址 */
	u32 print_end_adder = 0;
	/* 打印传感器通道的最值 */
	PRINT_SENSOR_MAX_MIN_VALUE print_sensor_max_min_value;
	/* 查询结果 */
	BOOL search_result;
	
	
	/* 1、设置的打印时间是否在记录的时间范围内。 */
	if((print_inf->Inf_State.Print_Start_Time 		<= print_inf->Inf_State.Print_Record_End_Time)&&
	   (print_inf->Inf_State.Print_End_Time 		>= print_inf->Inf_State.Print_Record_Start_Time)&&
	   (print_inf->Inf_State.Print_Start_Time 		<= print_inf->Inf_State.Print_End_Time)&&
	   (print_inf->Inf_State.Print_Record_Start_Time<= print_inf->Inf_State.Print_Record_End_Time))
	{
		/* 在设置的时间范围内有记录的传感器数据 */
		
		//1、
		/* 设置打印参数 */
		Print_Set_Para();
		
		//2、
		/* 搜索打印数据的起始位置. */
		search_result = Print_Search_Start_Adder(print_inf,&print_start_adder);
		/* 判断搜索起始数据地址是否成功 */
		if(search_result == FALSE)
		{
			/* 起始数据地址搜索失败 */
			/* 打印分割符 */
			Print_Indivisible_Symbol();
			/* 字体大小不翻倍 */
			Print_Set_Font_Double(RESET);
			/* 设置为16*16大小的字体 */
			Print_Set_Font(Print_Font_16_16);
			/* 错误信息1:搜索起始时间大于记录的结束时间。 */
			printf(Print_Search_Start_Time_Than_Record_End_Time);
			OSTimeDly(200);
			/* 打印分割符 */
			Print_Indivisible_Symbol();
			return;
		}
		//3、
		/* 搜索打印数据的结束位置. */
		search_result = Print_Search_End_Adder(print_inf,&print_end_adder);
		/* 判断搜索结束数据地址是否成功 */
		if(search_result == FALSE)
		{
			/* 结束数据地址搜索失败 */
			/* 打印分割符 */
			Print_Indivisible_Symbol();
			/* 字体大小不翻倍 */
			Print_Set_Font_Double(RESET);
			/* 设置为16*16大小的字体 */
			Print_Set_Font(Print_Font_16_16);
			/* 错误信息2:搜索结束时间小于记录的起始时间。 */
			printf(Print_Search_End_Time_Than_Record_Start_Time);
			OSTimeDly(200);
			/* 打印分割符 */
			Print_Indivisible_Symbol();
			return;
		}
		
		//4、
		/* 判断搜索的起始地址和结束地址是否合法（起始地址 小于等于 结束地址） */
		if(print_start_adder > print_end_adder)
		{
			/* 起始地址 大于 结束地址 */
			/* 打印分割符 */
			Print_Indivisible_Symbol();
			/* 字体大小不翻倍 */
			Print_Set_Font_Double(RESET);
			/* 设置为16*16大小的字体 */
			Print_Set_Font(Print_Font_16_16);
			/* 错误信息2:搜索结束时间小于记录的起始时间。 */
			printf(Print_Search_Start_Time_Than_Search_End_Time);
			OSTimeDly(200);
			/* 打印分割符 */
			Print_Indivisible_Symbol();
			return;
		}
		
		/* 数据地址搜索成功 */
		//5、
		/* 输出数据头内容 */
		Print_Data_Title(sensor_inf,print_inf,print_start_adder,print_end_adder);
		
		//6、
		/* 输出数据体内容（传感器数据） */
		Print_Data_Body(sensor_inf,&print_sensor_max_min_value,print_start_adder,print_end_adder);
		
		//7、
		/* 输出数据尾内容 */
		Print_Data_Tail(&print_sensor_max_min_value);
		
	}else
	{		
		/* 设置打印参数 */
		Print_Set_Para();
		/* 打印分割符 */
		Print_Indivisible_Symbol();
		/* 字体大小不翻倍 */
		Print_Set_Font_Double(RESET);
		/* 设置为16*16大小的字体 */
		Print_Set_Font(Print_Font_16_16);
		/* 错误信息1:搜索起始时间大于记录的结束时间。 */
		printf(Print_Set_Time_Range_Null_Data_Str);
		OSTimeDly(200);
		/* 打印分割符 */
		Print_Indivisible_Symbol();
	}

}
/************************************************************************************************/

/************************************************************************************************/
#if (1)/* 一键打印函数 */


/* 开始一键打印的按键时间长度 */
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


/* 连续打印时间寄存器 */
static u32 EasyPrintContinuousReg = 0;

/* 打印数据状态 */
static FlagStatus EasyPrintDataState = RESET;
#if (Print_Scheme == 1)
/*************************************************************************************************/
/* 函数名	: Easy_Print_Search_Arrive_Time														 */
/* 输入		: 无																		 		 */
/* 输出		: 无		 																		 */
/* 作用		: 搜索到达时间及参数															 	 	 */
/* 创建日期  : 2016/05/10																		 */
/*************************************************************************************************/
static void Easy_Print_Search_Arrive_Time(INSTRU_SENSOR* sensor_inf,Print_JWX_OutInTemp_Struct * Arrive_p,int start_adder,int end_adder)
{
	/* 读数据指针 */
	u8* data_p;
	
	
#if (Print_Sensor_CH_Num >= 1)
	/* 通道1数据 */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* 通道2数据 */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* 通道3数据 */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* 通道4数据 */
	float ch_4_data;
#endif
	
		//这里应该判断打印的数据长度，但是由于现在打印数据结构中没有这个参数，
		//暂时先用打印起始地址和打印结束地址进行判断（这样会有一个漏洞存在）。
		/* 判断打印起始地址和打印结束地址 */
		if(end_adder < 0)
		{
			/* 无数据打印，0作物抵达时间。 */
			return;
		}
		
		/* 读取起始地址数据 */
		data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		
			/* 获取 时间和数据 */
	#if (Print_Sensor_CH_Num >= 1)
		/* 获取通道1数据 */
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
		/* 获取通道2数据 */
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
		/* 获取通道3数据 */
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
		/* 获取通道4数据 */
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

	/* 判断当前数据状态 */

	/* 当前点设置为出库时间。 */
	#if (Print_Sensor_CH_Num >= 1)
			/* 数据1 */
			Arrive_p->Ch1_Value = ch_1_data;
	#endif
	#if (Print_Sensor_CH_Num >= 2)
			/* 数据2 */
			Arrive_p->Ch2_Value = ch_2_data;
	#endif
	#if (Print_Sensor_CH_Num >= 3)
			/* 数据3 */
			Arrive_p->Ch3_Value = ch_3_data;
	#endif
	#if (Print_Sensor_CH_Num >= 4)
			/* 数据4 */
			Arrive_p->Ch4_Value = ch_4_data;
	#endif
	/* 设置出库时间 */
	Arrive_p->Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
	Arrive_p->Time.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
	Arrive_p->Time.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
	Arrive_p->Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
	Arrive_p->Time.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
	Arrive_p->Time.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
}

/*************************************************************************************************/
/* 函数名	: Easy_Print_Search_StartOff_Time													 */
/* 输入		: 无																		 		 */
/* 输出		: 无		 																		 */
/* 作用		: 搜索出库时间及参数															 	 	 */
/* 创建日期  : 2016/05/10																		 */
/*************************************************************************************************/
static void Easy_Print_Search_StartOff_Time(INSTRU_SENSOR* sensor_inf,Print_JWX_OutInTemp_Struct * StartOff_p,int start_adder,int end_adder)
{
	/* 读数据指针 */
	u8* data_p;
	/* 时间寄存器 */
	Times time_b;
	/* 秒时间寄存器 */
	u32 second_time_b;
	/* 前一条数据的时间 */
	u32 before_time;
	/* 原始结束地址 */
	u32 o_EndAdder = end_adder;
	/* 出库参数寄存器 */
	Print_JWX_OutInTemp_Struct startoff;
	/* 数据状态 */
	BOOL dataState;
	
	
#if (Print_Sensor_CH_Num >= 1)
	/* 通道1数据 */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* 通道2数据 */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* 通道3数据 */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* 通道4数据 */
	float ch_4_data;
#endif

	/* 判断几率数据指针状态 */
	if(start_adder >= end_adder)
	{
		/* 无存储数据 */
		return;
	}

	/* 读取起始地址数据 */
	data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);

	/* 获取时间数据 */
	time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
	time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
	time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
	time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
	time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
	time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;

	/* 设置出库时间 */
	
	startoff.Time.Year 	= time_b.Year;
	startoff.Time.Month = time_b.Month;
	startoff.Time.Day 	= time_b.Day;
	startoff.Time.Hour 	= time_b.Hour;
	startoff.Time.Min 	= time_b.Min;
	startoff.Time.Sec 	= time_b.Sec;
	
	/* 设置数据 */
#if (Print_Sensor_CH_Num >= 1)
	/* 获取通道1数据 */
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
	/* 获取通道2数据 */
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
	/* 获取通道3数据 */
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
	/* 获取通道4数据 */
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

	/* 设置当前数据这样 */
	/* 判断数据状态 */
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
		/* 数据正常 */
		dataState = TRUE;
	}else
	{
		/* 报警 */
		dataState = FALSE;
	}
		

	/* 设置前一条数据的时间为第一条数据时间 */
	before_time = RTC_Date_Time_To_Second(&time_b);
	
	while(start_adder <= end_adder)
	{
		/* 读取起始地址数据 */
		data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		/* 获取 时间和数据 */
#if (Print_Sensor_CH_Num >= 1)
	/* 获取通道1数据 */
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
	/* 获取通道2数据 */
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
	/* 获取通道3数据 */
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
	/* 获取通道4数据 */
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

		/* 获取时间数据 */
		time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
		
		/* 将时间转换成秒 */
		second_time_b = RTC_Date_Time_To_Second(&time_b);

/* 判断通道状态 */
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
			/* 数据正常 */
			/* 前一组数据状态 */
			if(dataState == TRUE)
			{
				/* 前一组数据状态:正常 */
				/* 判断数据是否连续 */
				if((before_time > second_time_b)&&
				   (before_time - second_time_b > Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl))
					{
						/* 数据不连续 */
						/* 将前一组当数据设置为出库时间/温度。 */
						*StartOff_p = startoff;
						return;
					}
			}else
			{
				/* 前一组数据状态:报警 */
				/* 判断数据是否连续 */
				if(before_time > second_time_b)
				{
					/* 判断正常/报警记录时间大小 */
					if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl > 
					   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
					{
						/* 正常记录时间长 */
						/* 判断数据是否连续 */
						if((before_time - second_time_b) > 
						   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
						{
							/* 数据不连续 */
							/* 当前点不是报警等间隔时间点，向前收索出库时间点。 */
							while(end_adder < o_EndAdder)
							{
								/* 设置前一个数据地址 */
								end_adder += Save_Sensor_Data_Len;					
								/* 读取起始地址数据 */
								data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
								
				/* 获取 时间和数据 */
		#if (Print_Sensor_CH_Num >= 1)
			/* 获取通道1数据 */
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
			/* 获取通道2数据 */
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
			/* 获取通道3数据 */
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
			/* 获取通道4数据 */
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

		/* 判断通道状态 */
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
									/* 获取时间数据 */
									time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
									time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
									time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
									time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
									time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
									time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
									
									/* 将时间转换成秒 */
									second_time_b = RTC_Date_Time_To_Second(&time_b);
									/* 判断 判断记录的数据是否连续 */
									if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl == 0)
									{
		#if (Print_Sensor_CH_Num >= 1)
										/* 数据1 */
										StartOff_p->Ch1_Value = ch_1_data;
		#endif
		#if (Print_Sensor_CH_Num >= 2)
										/* 数据2 */
										StartOff_p->Ch2_Value = ch_2_data;
		#endif
		#if (Print_Sensor_CH_Num >= 3)
										/* 数据3 */
										StartOff_p->Ch3_Value = ch_3_data;
		#endif
		#if (Print_Sensor_CH_Num >= 4)
										/* 数据4 */
										StartOff_p->Ch4_Value = ch_4_data;
		#endif
										/* 设置出库时间 */
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
						/* 报警记录时间长 */
						if((before_time - second_time_b) > 
						    Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
						{
							/* 数据不连续 */
							/* 当前点不是报警等间隔时间点，向前收索出库时间点。 */
							while(end_adder < o_EndAdder)
							{
								/* 设置前一个数据地址 */
								end_adder += Save_Sensor_Data_Len;					
								/* 读取起始地址数据 */
								data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
								
				/* 获取 时间和数据 */
		#if (Print_Sensor_CH_Num >= 1)
			/* 获取通道1数据 */
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
			/* 获取通道2数据 */
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
			/* 获取通道3数据 */
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
			/* 获取通道4数据 */
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

		/* 判断通道状态 */
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
									/* 获取时间数据 */
									time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
									time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
									time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
									time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
									time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
									time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
									
									/* 将时间转换成秒 */
									second_time_b = RTC_Date_Time_To_Second(&time_b);
									/* 判断 判断记录的数据是否连续 */
									if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl == 0)
									{
		#if (Print_Sensor_CH_Num >= 1)
										/* 数据1 */
										StartOff_p->Ch1_Value = ch_1_data;
		#endif
		#if (Print_Sensor_CH_Num >= 2)
										/* 数据2 */
										StartOff_p->Ch2_Value = ch_2_data;
		#endif
		#if (Print_Sensor_CH_Num >= 3)
										/* 数据3 */
										StartOff_p->Ch3_Value = ch_3_data;
		#endif
		#if (Print_Sensor_CH_Num >= 4)
										/* 数据4 */
										StartOff_p->Ch4_Value = ch_4_data;
		#endif
										/* 设置出库时间 */
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
			/* 数据超标 */
			/* 前一组数据状态 */
			if(dataState == TRUE)
			{
				/* 前一组数据状态:正常 */
				/* 判断数据是否连续 */
				if(before_time > second_time_b)
				{
					/* 判断正常/报警记录时间大小 */
					if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl > 
					   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
					{
						/* 正常记录时间长 */
						/* 判断数据是否连续 */
						if((before_time - second_time_b) > 
						   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
						{
							/* 数据不连续 */
							/* 当前点不是报警等间隔时间点，向前收索出库时间点。 */
							while(end_adder < o_EndAdder)
							{
								/* 设置前一个数据地址 */
								end_adder += Save_Sensor_Data_Len;					
								/* 读取起始地址数据 */
								data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
								
/* 获取 时间和数据 */
#if (Print_Sensor_CH_Num >= 1)
	/* 获取通道1数据 */
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
	/* 获取通道2数据 */
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
	/* 获取通道3数据 */
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
	/* 获取通道4数据 */
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

		/* 判断通道状态 */
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
									/* 获取时间数据 */
									time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
									time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
									time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
									time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
									time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
									time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
									
									/* 将时间转换成秒 */
									second_time_b = RTC_Date_Time_To_Second(&time_b);
									/* 判断 判断记录的数据是否连续 */
									if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl == 0)
									{
		#if (Print_Sensor_CH_Num >= 1)
										/* 数据1 */
										StartOff_p->Ch1_Value = ch_1_data;
		#endif
		#if (Print_Sensor_CH_Num >= 2)
										/* 数据2 */
										StartOff_p->Ch2_Value = ch_2_data;
		#endif
		#if (Print_Sensor_CH_Num >= 3)
										/* 数据3 */
										StartOff_p->Ch3_Value = ch_3_data;
		#endif
		#if (Print_Sensor_CH_Num >= 4)
										/* 数据4 */
										StartOff_p->Ch4_Value = ch_4_data;
		#endif
										/* 设置出库时间 */
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
						/* 报警记录时间长 */
						if((before_time - second_time_b) > 
						    Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
						{
							/* 数据不连续 */
							/* 当前点不是报警等间隔时间点，向前收索出库时间点。 */
							while(end_adder < o_EndAdder)
							{
								/* 设置前一个数据地址 */
								end_adder += Save_Sensor_Data_Len;					
								/* 读取起始地址数据 */
								data_p = Flash_Read_Sensor_Data(end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
								
/* 获取 时间和数据 */
#if (Print_Sensor_CH_Num >= 1)
	/* 获取通道1数据 */
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
	/* 获取通道2数据 */
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
	/* 获取通道3数据 */
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
	/* 获取通道4数据 */
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

		/* 判断通道状态 */
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
									/* 获取时间数据 */
									time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
									time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
									time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
									time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
									time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
									time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
									
									/* 将时间转换成秒 */
									second_time_b = RTC_Date_Time_To_Second(&time_b);
									/* 判断 判断记录的数据是否连续 */
									if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl == 0)
									{
		#if (Print_Sensor_CH_Num >= 1)
										/* 数据1 */
										StartOff_p->Ch1_Value = ch_1_data;
		#endif
		#if (Print_Sensor_CH_Num >= 2)
										/* 数据2 */
										StartOff_p->Ch2_Value = ch_2_data;
		#endif
		#if (Print_Sensor_CH_Num >= 3)
										/* 数据3 */
										StartOff_p->Ch3_Value = ch_3_data;
		#endif
		#if (Print_Sensor_CH_Num >= 4)
										/* 数据4 */
										StartOff_p->Ch4_Value = ch_4_data;
		#endif
										/* 设置出库时间 */
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
				/* 前一组数据状态:报警 */
				/* 判断数据是否连续 */
				if((before_time > second_time_b)&&
				   (before_time - second_time_b > Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl))
				{
					/* 数据不连续 */
					/* 将前一组当数据设置为出库时间/温度。 */
					*StartOff_p = startoff;
					return;
				}
			}
		}
			
/* 当前点设置为出库时间。 */
#if (Print_Sensor_CH_Num >= 1)
		/* 数据1 */
		startoff.Ch1_Value = ch_1_data;
#endif
#if (Print_Sensor_CH_Num >= 2)
		/* 数据2 */
		startoff.Ch2_Value = ch_2_data;
#endif
#if (Print_Sensor_CH_Num >= 3)
		/* 数据3 */
		startoff.Ch3_Value = ch_3_data;
#endif
#if (Print_Sensor_CH_Num >= 4)
		/* 数据4 */
		startoff.Ch4_Value = ch_4_data;
#endif
		/* 设置当前数据这样 */
		/* 判断数据状态 */
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
			/* 数据正常 */
			dataState = TRUE;
		}else
		{
			/* 报警 */
			dataState = FALSE;
		}
		
		/* 设置出库时间 */
		startoff.Time.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		startoff.Time.Month = ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		startoff.Time.Day 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		startoff.Time.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		startoff.Time.Min 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		startoff.Time.Sec 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
		
		/* 设置单前时间为前一条数据时间 */
		before_time = second_time_b;
		
		/* 设置下一个数据地址 */
		end_adder -= Save_Sensor_Data_Len;	

	}
	*StartOff_p = startoff;
	return;
}

#endif
/*************************************************************************************************/
/* 函数名	: Easy_Print_Data_Title														 	 	 */
/* 输入		: sensor_inf:传感器通道信息；print_inf:打印信息								 		 */
/* 输出		: 无 																				 */
/* 作用		: 一键打印标题															 	 		 */
/* 创建日期  : 2016/04/08																		 */
/*************************************************************************************************/
static void Easy_Print_Data_Title(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf)
{
/* 金卫信打印方案 */
#if (Print_Scheme == 1)
	/* 出发时间 */
	uint32_t startofftemp = 0;
	/* 抵达时间 */
	uint32_t arrivetemp = 0;
	/* 临时时间寄存器 */
	Times 	 temp_;
#endif	
	
	/* 打印分割符 */
	Print_Indivisible_Symbol();
	
	/* 字体大小不翻倍 */
	Print_Set_Font_Double(RESET);
	/* 设置为16*16大小的字体 */
	Print_Set_Font(Print_Font_16_16);
	/* 设备号码:****** */
	printf(Device_Number_Str,
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
/* 金卫信打印方案 */
#if (Print_Scheme == 1)
	/* 计算出库时间和抵达时间 */
	temp_ = StartOffTemp.Time;
	startofftemp 	= RTC_Date_Time_To_Second(&temp_);
	temp_ = ArriveTemp.Time;
	arrivetemp 		= RTC_Date_Time_To_Second(&temp_);
	/* 打印运单号:7DC311001-1F9314C0-1F946640 */
	printf(Print_TransportNo_Str,
		   ((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8) |
		   ((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
		   ((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8) |
		   ((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)),
		   startofftemp,
		   arrivetemp);
#endif	
	
	/* 打印传感器通道1的数据范围 */
#if (Print_Sensor_CH_Num >= 0x01)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
	/* 温度类型传感器 */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_1+1),
	(	  (sensor_inf->Inf_State.CH1.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH1.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Low_Limit*10)%10),
	(	  (sensor_inf->Inf_State.CH1.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH1.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* 湿度类型传感器 */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_1+1),
	((s16) sensor_inf->Inf_State.CH1.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Low_Limit*10)%10),	
	((s16) sensor_inf->Inf_State.CH1.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH1.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#endif
#endif

	/* 打印传感器通道2的数据范围 */
#if (Print_Sensor_CH_Num >= 0x02)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
	/* 温度类型传感器 */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_2+1),
	(	  (sensor_inf->Inf_State.CH2.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH2.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH2.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH2.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* 湿度类型传感器 */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_2+1),
	((s16) sensor_inf->Inf_State.CH2.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Low_Limit*10)%10),	
	((s16) sensor_inf->Inf_State.CH2.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH2.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#endif
#endif

	/* 打印传感器通道3的数据范围 */
#if (Print_Sensor_CH_Num >= 0x03)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
	/* 温度类型传感器 */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_3+1),
	(	  (sensor_inf->Inf_State.CH3.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH3.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH3.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH3.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* 湿度类型传感器 */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_3+1),
	((s16) sensor_inf->Inf_State.CH3.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Low_Limit*10)%10),
	((s16) sensor_inf->Inf_State.CH3.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH3.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#endif
#endif

	/* 打印传感器通道4的数据范围 */
#if (Print_Sensor_CH_Num >= 0x04)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
	/* 温度类型传感器 */
	printf(Print_sensor_Temp_Range_Str,
	(Sensors_CH_4+1),
	(	  (sensor_inf->Inf_State.CH4.SENSOR_Low_Limit >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH4.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Low_Limit*10)%10),	
	(	  (sensor_inf->Inf_State.CH4.SENSOR_Up_Limit  >= 0.0) ? ' ':'-'),
	((u16) sensor_inf->Inf_State.CH4.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* 湿度类型传感器 */
	printf(Print_Sensor_Humi_Range_Str,
	(Sensors_CH_4+1),
	((s16) sensor_inf->Inf_State.CH4.SENSOR_Low_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Low_Limit*10)%10),
	((s16) sensor_inf->Inf_State.CH4.SENSOR_Up_Limit),
	((u16)(sensor_inf->Inf_State.CH4.SENSOR_Up_Limit*10)%10));
	/* 等待标题打印结束 */
	OSTimeDly(200);
	#endif
#endif

	/* 数据记录如下:****** */
	printf(Print_Record_Data_Str);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
	/* 设置时间的项目条 */
	printf(Print_Time_Bar_Str);
#if (Print_Sensor_CH_Num >= 1)
	/* 设置传感器1项目条 */
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
	/* 设置传感器2项目条 */
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
	/* 设置传感器3项目条 */
	#if (Print_Scheme == 0)
	/* 默认方案 */
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
	/* 金卫信方案 */
	printf(" ");
	/* 金卫信方案 */
	printf(Print_EnvironmentTemp_Bar_Str);
	#endif	
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* 设置传感器4项目条 */
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
	/* 回车 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
}



///************************************************************************************************
//*函数名     : IsOrNotStartPrintUoloadBody															
//*输入参数  	: adder1代表要对比的参数   								
//*输出参数	  : true代表输出卸货开始字符串 	 false代表不输出卸货字符串 																				
//*作用	 	    : xxx																
//*创建日期	  : 2018/05/28																		
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
//*函数名     : IsOrNotStopPrintUoloadBody															
//*输入参数  	: adder1代表要对比的参数   								
//*输出参数	  : true代表输出卸货结束字符串 	 false代表不输出卸货结束字符串 		 																				
//*作用	 	    : xxx																
//*创建日期	  : 2018/05/28																		
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
/* 函数名	: Easy_Print_Data_Body														 	 	 */
/* 输入		: sensor_inf:传感器通道信息；print_inf:打印信息								 		 */
/* 输出		: 无 																				 */
/* 作用		: 一键打印标题															 	 		 */
/* 创建日期  : 2016/04/08																		 */
/*************************************************************************************************/
static void Easy_Print_Data_Body(INSTRU_SENSOR* sensor_inf,PRINT_SENSOR_MAX_MIN_VALUE* print_sensor_max_min_value,int* start_adder,int* end_adder,BOOL * printDateFlag)
{
/* 一天时间的秒钟数，这里用于判断打印的日期是否超过一天 */
#define Day_Second_Num			(86400uL)
	/* 读数据指针 */
	u8* data_p;
	u8  Temp=0;
	/* 时间寄存器 */
	Times time_b;
	/* 秒时间寄存器 */
	u32 second_time_b;
	/* 记录上次打印日期数据 */
	static u32 print_date_dr = 0;
	/* 打印日期标志 */
	static BOOL PrintDateFlag = FALSE;
	
#if (Print_Sensor_CH_Num >= 1)
	/* 通道1数据 */
	float ch_1_data;
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	/* 通道2数据 */
	float ch_2_data;
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* 通道3数据 */
	float ch_3_data;
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	/* 通道4数据 */
	float ch_4_data;
#endif
	
	
	
	/* 判断传感器数据 */
	if(*start_adder <= *end_adder)
	{
		/* 读取起始地址数据 */
		data_p = Flash_Read_Sensor_Data(*end_adder%Flash_Capacity_Size,Save_Sensor_Data_Len);
		/* 设置下一个数据地址 */
		*end_adder -= Save_Sensor_Data_Len;
		/* 获取 时间和数据 */
#if (Print_Sensor_CH_Num >= 1)
	/* 获取通道1数据 */
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
	/* 获取通道2数据 */
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
	/* 获取通道3数据 */
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
	/* 获取通道4数据 */
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
		
		/* 获取时间数据 */
		time_b.Year 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.YEAR_BITS + 2000;
		time_b.Month 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MONTH_BITS;
		time_b.Day 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.DAY_BITS;
		time_b.Hour 	= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS;
		time_b.Min 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS;
		time_b.Sec 		= ((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS;
		/* 将时间转换成秒 */
		second_time_b = RTC_Date_Time_To_Second(&time_b);
		
		/* 判断通道状态 */
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
			/* 通道正常状态 */
			/* 判断 报警时间是否为 报警时间的倍数 */
			if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl != 0)
			{
				/* 判断时间间隔是否小于报警时间间隔 */
				if((EasyPrintContinuousReg - second_time_b)<Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
				{
					
					/*********************打印是否卸货字符串**************************/
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
					
					
					/* 不打印这类数据 */
					/* 从头开始执行 */
					return;
				}
			}
		}else
		{
			/* 通道报警状态 */
			/* 判断 报警时间是否为 正常时间的倍数 */
			if(second_time_b%Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl != 0)
			{
				/* 判断时间间隔是否小于报警时间间隔 */
				if((EasyPrintContinuousReg - second_time_b)<Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
				{
					
				 /*********************打印是否卸货字符串**************************/
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
					/* 不打印这类数据 */
					/* 从头开始执行 */
					return;
				}
			}
		}
		/* 打印读取的日期数据 */
		if((*printDateFlag == TRUE)||((second_time_b / Day_Second_Num) != print_date_dr))
		{
			/* 只有2中条件下需要打印日期：1、打印第一条数据;2、打印时间间隔1天以上 */
			/* 判断 打印日期标志 */
			if(*printDateFlag == TRUE)
			{
				/* 清除日期标志  */
				*printDateFlag = FALSE;
				/* 设置最值寄存器数据 */
#if (Print_Sensor_CH_Num >= 1)
				/* 设置最值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
				/* 设置最值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;

#endif
	
#if (Print_Sensor_CH_Num >= 2)	
				/* 设置最值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
				/* 设置最值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;

#endif	
#if (Print_Sensor_CH_Num >= 3)
				/* 设置最值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
				/* 设置最值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;

#endif
#if (Print_Sensor_CH_Num >= 4)
				/* 设置最值 */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
				/* 设置最值 */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;

#endif
				/* 设置新的打印日期 */
				print_date_dr = (second_time_b / Day_Second_Num);
				
				/* 设置连续打印 */
				EasyPrintContinuousReg = second_time_b;
				
				/* 设置打印日期标志 */
				PrintDateFlag = TRUE;
				
			}else
			{
					/*********************打印是否卸货字符串**************************/
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
				
				/* 数据不连续，打印分割符号。 */
				Print_Separator_Line();
				
				/* 设置打印日期标志 */
				PrintDateFlag = TRUE;
				
//				/* 设置下一个数据地址 */
//				*end_adder += Save_Sensor_Data_Len;
				
				/* 设置新的打印日期 */
				print_date_dr = (second_time_b / Day_Second_Num);
				
				return;
			}
		}
		
		/* 判断打印日期标志 */
		if(PrintDateFlag == TRUE)
		{
			/* 打印日期 */
			Print_Date(data_p);
			/* 清除标志 */
			PrintDateFlag = FALSE;
		}
		
		/* 判断通道状态 */
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
			/* 数据正常 */
			/* 判断打印数据状态 */
			if(EasyPrintDataState == RESET)
			{
				/* 前一组数据状态:正常 */
				/* 判断数据是否连续 */
				if((EasyPrintContinuousReg > second_time_b)&&
				   (EasyPrintContinuousReg - second_time_b > Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl))
					{
						/* 数据不连续，打印分割符号。 */
						Print_Separator_Line();
						
						/* 设置当前数据时间为一段连续数据的开始数据 */
						EasyPrintContinuousReg = second_time_b;
						
					/*********************打印是否卸货字符串**************************/
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
						
						
//						/* 设置下一个数据地址 */
//						*end_adder += Save_Sensor_Data_Len;
						
						/* 设置数据状态 */
						EasyPrintDataState = RESET;
						return;
					}
			}else
			{
				/* 前一组数据状态:报警 */
				/* 判断数据是否连续 */
				if(EasyPrintContinuousReg > second_time_b)
				{
					/* 判断正常/报警记录时间大小 */
					if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl > 
					   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
					{
						/* 正常记录时间长 */
						/* 判断数据是否连续 */
						if((EasyPrintContinuousReg - second_time_b) > 
						   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
						{
							/* 数据不连续，打印分割符号。 */
							Print_Separator_Line();
							
							/* 设置当前数据时间为一段连续数据的开始数据 */
							EasyPrintContinuousReg = second_time_b;
							
					/*********************打印是否卸货字符串**************************/
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
							
//							/* 设置下一个数据地址 */
//							*end_adder += Save_Sensor_Data_Len;
							
							/* 设置数据状态 */
							EasyPrintDataState = RESET;
							return;		
						}
					}else
					{
						/* 报警记录时间长 */
						if((EasyPrintContinuousReg - second_time_b) > 
						    Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
						{
							/* 数据不连续，打印分割符号。 */
							Print_Separator_Line();
							
							/* 设置当前数据时间为一段连续数据的开始数据 */
							EasyPrintContinuousReg = second_time_b;
					/*********************打印是否卸货字符串**************************/
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
							
//							/* 设置下一个数据地址 */
//							*end_adder += Save_Sensor_Data_Len;
							
							/* 设置数据状态 */
							EasyPrintDataState = RESET;
							return;
						}
					}
				}
			}
		}else
		{
			/* 数据超标 */
			/* 判断打印数据状态 */
			if(EasyPrintDataState == SET)
			{
				/* 前一组数据状态:正常 */
				/* 判断数据是否连续 */
				if((EasyPrintContinuousReg > second_time_b)&&
				   (EasyPrintContinuousReg - second_time_b > Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl))
					{
						/* 数据不连续，打印分割符号。 */
						Print_Separator_Line();
						
						/* 设置当前数据时间为一段连续数据的开始数据 */
						EasyPrintContinuousReg = second_time_b;
					/*********************打印是否卸货字符串**************************/
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
						/* 设置下一个数据地址 */
//						*end_adder += Save_Sensor_Data_Len;
						
						/* 设置数据状态 */
						EasyPrintDataState = SET;
						return;
					}
			}else
			{
				/* 前一组数据状态:报警 */
				/* 判断数据是否连续 */
				if(EasyPrintContinuousReg > second_time_b)
				{
					/* 判断正常/报警记录时间大小 */
					if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl > 
					   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
					{
						/* 正常记录时间长 */
						/* 判断数据是否连续 */
						if((EasyPrintContinuousReg - second_time_b) > 
						   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
						{
							/* 数据不连续，打印分割符号。 */
							Print_Separator_Line();
							
							/* 设置当前数据时间为一段连续数据的开始数据 */
							EasyPrintContinuousReg = second_time_b;
					/*********************打印是否卸货字符串**************************/
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
//							/* 设置下一个数据地址 */
//							*end_adder += Save_Sensor_Data_Len;
							
							/* 设置数据状态 */
							EasyPrintDataState = SET;
							return;		
						}
					}else
					{
						/* 报警记录时间长 */
						if((EasyPrintContinuousReg - second_time_b) > 
						    Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Out_Itl)
						{
							/* 数据不连续，打印分割符号。 */
							Print_Separator_Line();
							
							/* 设置当前数据时间为一段连续数据的开始数据 */
							EasyPrintContinuousReg = second_time_b;
					/*********************打印是否卸货字符串**************************/
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
							
//							/* 设置下一个数据地址 */
//							*end_adder += Save_Sensor_Data_Len;
							
							/* 设置数据状态 */
							EasyPrintDataState = SET;
							return;
						}
					}
				}
			}
		}
		
					/*********************打印是否卸货字符串**************************/
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
		
		
		/* 设置当前数据时间为一段连续数据的开始数据 */
		EasyPrintContinuousReg = second_time_b;
		
		/* 打印读取的时间数据 */
		printf(Print_Data_Body_Time_Str,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.HOUR_BITS,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.MINUTE_BITS,
		((DATETIME_Storage_Format*)data_p)->DATETIME_Bit.SECOND_BITS);
		
#if (Print_Sensor_CH_Num == 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
		/* 判断数据是否超标 */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),	
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}

	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
			/* 判断数据是否超标 */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器1数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* 判断数据是否超标 */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器2数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		
	#endif
#elif (Print_Sensor_CH_Num == 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_Hum))
		/* 判断数据是否超标 */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);
		}
		
		/* 打印读取的传感器1数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}

		/* 判断数据是否超标 */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器2数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
		/* 判断数据是否超标 */
		if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器3数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_3_data >= 0.0) ? ' ':'-'),
		((u16) ch_3_data),
		((u16)(ch_3_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch3_Max = ch_3_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch3_Min = ch_3_data;
		}
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_3_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
	#endif
#elif (Print_Sensor_CH_Num == 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_Hum))
		/* 判断数据是否超标 */
		if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);
		}
		/* 打印读取的传感器1数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_1_data >= 0.0) ? ' ':'-'),
		((u16) ch_1_data),
		((u16)(ch_1_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch1_Max = ch_1_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch1_Min = ch_1_data;
		}
		
		/* 判断数据是否超标 */
		if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器2数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_2_data >= 0.0) ? ' ':'-'),
		((u16) ch_2_data),
		((u16)(ch_2_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch2_Max = ch_2_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch2_Min = ch_2_data;
		}
		/* 判断数据是否超标 */
		if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器3数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_3_data >= 0.0) ? ' ':'-'),
		((u16) ch_3_data),
		((u16)(ch_3_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch3_Max = ch_3_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch3_Min = ch_3_data;
		}
		/* 判断数据是否超标 */
		if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
		}else
		{
			/* 数据未超标,设置允许下划线. */
			Print_Set_Underline_Config(ENABLE);		
		}
		/* 打印读取的传感器4数据 */
		printf(Print_Data_Body_Data_Str,
		((ch_4_data >= 0.0) ? ' ':'-'),
		((u16) ch_4_data),
		((u16)(ch_4_data*10)%10));
		
		/* 判断传感器通道的最值 */
		if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
		{
			/* 更新最大值 */
			print_sensor_max_min_value->Ch4_Max = ch_4_data;
		}
		
		/* 判断传感器通道的最值 */
		if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
		{
			/* 更新最小值 */
			print_sensor_max_min_value->Ch4_Min = ch_4_data;
		}
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_3_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{	
			/* 判断数据是否超标 */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_4_data == Wireless_RF_Card_Data_Rece_Timeout_Value)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_4_data >= 0.0) ? ' ':'-'),
			((u16) ch_4_data),
			((u16)(ch_4_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
			}
		}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))	
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_1_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_1_data <= sensor_inf->Inf_State.CH1.SENSOR_Up_Limit)&&(ch_1_data >= sensor_inf->Inf_State.CH1.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_1_data >= 0.0) ? ' ':'-'),
			((u16) ch_1_data),
			((u16)(ch_1_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_1_data > print_sensor_max_min_value->Ch1_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch1_Max = ch_1_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_1_data < print_sensor_max_min_value->Ch1_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch1_Min = ch_1_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_2_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_2_data <= sensor_inf->Inf_State.CH2.SENSOR_Up_Limit)&&(ch_2_data >= sensor_inf->Inf_State.CH2.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_2_data >= 0.0) ? ' ':'-'),
			((u16) ch_2_data),
			((u16)(ch_2_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_2_data > print_sensor_max_min_value->Ch2_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch2_Max = ch_2_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_2_data < print_sensor_max_min_value->Ch2_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch2_Min = ch_2_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_3_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{	
			/* 判断数据是否超标 */
			if((ch_3_data <= sensor_inf->Inf_State.CH3.SENSOR_Up_Limit)&&(ch_3_data >= sensor_inf->Inf_State.CH3.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_3_data >= 0.0) ? ' ':'-'),
			((u16) ch_3_data),
			((u16)(ch_3_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_3_data > print_sensor_max_min_value->Ch3_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch3_Max = ch_3_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_3_data < print_sensor_max_min_value->Ch3_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch3_Min = ch_3_data;
			}
		}
		/* 判断 传感器数据是否为 超时接收数据 */
		if(ch_4_data == Gateway_NJSY_TimeoutPara)
		{
			/* 数据未超标,设置禁止下划线. */
			Print_Set_Underline_Config(DISABLE);
			/* 为超时接收数据，打印无数据。 */
			printf(Print_Data_Body_Rece_Timeout_Data_Str);
		}else
		{
			/* 判断数据是否超标 */
			if((ch_4_data <= sensor_inf->Inf_State.CH4.SENSOR_Up_Limit)&&(ch_4_data >= sensor_inf->Inf_State.CH4.SENSOR_Low_Limit))
			{
				/* 数据未超标,设置禁止下划线. */
				Print_Set_Underline_Config(DISABLE);
			}else
			{
				/* 数据未超标,设置允许下划线. */
				Print_Set_Underline_Config(ENABLE);		
			}
			/* 正常数据 */
			printf(Print_Data_Body_Data_Str,
			((ch_4_data >= 0.0) ? ' ':'-'),
			((u16) ch_4_data),
			((u16)(ch_4_data*10)%10));
			
			/* 判断传感器通道的最值 */
			if(ch_4_data > print_sensor_max_min_value->Ch4_Max)
			{
				/* 更新最大值 */
				print_sensor_max_min_value->Ch4_Max = ch_4_data;
			}
			
			/* 判断传感器通道的最值 */
			if(ch_4_data < print_sensor_max_min_value->Ch4_Min)
			{
				/* 更新最小值 */
				print_sensor_max_min_value->Ch4_Min = ch_4_data;
			}
		}
		
	#endif
#endif
		
		/* 数据未超标,设置禁止下划线. */
		Print_Set_Underline_Config(DISABLE);
		/* 换行 */
		Print_Set_CRLF();
		/* 等待标题打印结束 */
		OSTimeDly(120);
		
	}
	
	
#undef Day_Second_Num
}

/*************************************************************************************************/
/* 函数名	: Easy_Print_Data_Tail													 	 		 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 输出数据尾内容																 	 	 */
/* 创建日期  : 2016/10/18																			 */
/*************************************************************************************************/
static void Easy_Print_Data_Tail(PRINT_SENSOR_MAX_MIN_VALUE* print_sensor_max_min_value)
{

	/* 1、空2行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	
	/* 2、打印数据最大/小值. */
	/* 打印数据最大值 */
	printf(Print_Sensor_Data_Max_Value_Str);
	/* 打印读取的传感器数据 */
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* 打印传感器通道1最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch1_Max),
	((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))			   
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch1_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Max),
		((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch1_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Max),
		((u16)(print_sensor_max_min_value->Ch1_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
	/* 打印传感器通道2最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch2_Max),
	((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	       (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch2_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Max),
		((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch2_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Max),
		((u16)(print_sensor_max_min_value->Ch2_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
	/* 打印传感器通道3最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch3_Max),
	((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch3_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Max),
		((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch3_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Max),
		((u16)(print_sensor_max_min_value->Ch3_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||(Sensor_CH4_Config_Type == Sensor_Hum))
	/* 打印传感器通道4最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch4_Max),
	((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch4_Max != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Max),
		((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch4_Max != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Max >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Max),
		((u16)(print_sensor_max_min_value->Ch4_Max*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	/* 回车 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(200);
	
	/* 打印数据最小值. */
	printf(Print_Sensor_Data_Min_Value_Str);
	/* 打印读取的传感器数据 */
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* 打印传感器通道1最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch1_Min),
	((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch1_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Min),
		((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch1_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch1_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch1_Min),
		((u16)(print_sensor_max_min_value->Ch1_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH2_Config_Type == Sensor_Hum))
	/* 打印传感器通道2最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch2_Min),
	((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch2_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Min),
		((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch2_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch2_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch2_Min),
		((u16)(print_sensor_max_min_value->Ch2_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH3_Config_Type == Sensor_Hum))
	/* 打印传感器通道3最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch3_Min),
	((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch3_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Min),
		((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}	
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch3_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch3_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch3_Min),
		((u16)(print_sensor_max_min_value->Ch3_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 4)
	#if ((Sensor_CH4_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH4_Config_Type == Sensor_Hum))
	/* 打印传感器通道4最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
	((u16) print_sensor_max_min_value->Ch4_Min),
	((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	#elif ((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	       (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch4_Min != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Min),
		((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(print_sensor_max_min_value->Ch4_Min != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (print_sensor_max_min_value->Ch4_Min >= 0.0) ? ' ':'-'),
		((u16) print_sensor_max_min_value->Ch4_Min),
		((u16)(print_sensor_max_min_value->Ch4_Min*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	/* 回车 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(200);	
	
	/* 空1行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);

	
#if (Print_Scheme == 0)
/* 默认方案 */
	
	/* 3、打印送货方数据. */
	/* 设置允许下划线. */
	Print_Set_Underline_Config(ENABLE);
	/* 打印送货方数据. */
	printf(Print_Delivery_Party_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	/* 打印收货方数据. */
	printf(Print_Receiver_Party_Str);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	
	/* 4、打印 车牌号. */	
	printf(Print_Card_NO_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
#elif (Print_Scheme == 1)
/* 金卫信方案 */

	/* 3、打印 出库温度. */	
	/* 设置允许下划线. */
	Print_Set_Underline_Config(ENABLE);
	
	/* 打印 出库时间 */
	//
	printf(Print_StartOffTime_Str,
		   StartOffTemp.Time.Year,
		   StartOffTemp.Time.Month,
		   StartOffTemp.Time.Day,
		   StartOffTemp.Time.Hour,
		   StartOffTemp.Time.Min);
	/* 等待打印结束 */
	OSTimeDly(120);
	/* 打印 出库温度. */
	printf(Print_StartOffTemp_Str);
	
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* 打印传感器通道1最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
	((u16) StartOffTemp.Ch1_Value),
	((u16)(StartOffTemp.Ch1_Value*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(StartOffTemp.Ch1_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch1_Value),
		((u16)(StartOffTemp.Ch1_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(StartOffTemp.Ch1_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch1_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch1_Value),
		((u16)(StartOffTemp.Ch1_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
	/* 打印传感器通道2最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
	((u16) StartOffTemp.Ch2_Value),
	((u16)(StartOffTemp.Ch2_Value*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(StartOffTemp.Ch2_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch2_Value),
		((u16)(StartOffTemp.Ch2_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(StartOffTemp.Ch2_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch2_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch2_Value),
		((u16)(StartOffTemp.Ch2_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
	/* 空1行 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* 打印 环境温度. */
	printf(Print_StartOffEnvironmentTemp_Bar_Str);
	
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
	/* 打印传感器通道3最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
	((u16) StartOffTemp.Ch3_Value),
	((u16)(StartOffTemp.Ch3_Value*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(StartOffTemp.Ch3_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch3_Value),
		((u16)(StartOffTemp.Ch3_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(StartOffTemp.Ch3_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (StartOffTemp.Ch3_Value >= 0.0) ? ' ':'-'),
		((u16) StartOffTemp.Ch3_Value),
		((u16)(StartOffTemp.Ch3_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);
	}
	#endif
#endif
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	
	/* 4、打印 到达温度. */
	
	/* 打印 到达时间 */
	//
	printf(Print_ArriveTime_Str,
		   ArriveTemp.Time.Year,
		   ArriveTemp.Time.Month,
		   ArriveTemp.Time.Day,
		   ArriveTemp.Time.Hour,
		   ArriveTemp.Time.Min);
	/* 等待打印结束 */
	OSTimeDly(120);
	/* 打印 到达温度. */
	printf(Print_ArriveTemp_Str);
	
#if (Print_Sensor_CH_Num >= 1)
	#if ((Sensor_CH1_Config_Type == Sensor_Tmp)||\
		 (Sensor_CH1_Config_Type == Sensor_Hum))
	/* 打印传感器通道1最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
	((u16) ArriveTemp.Ch1_Value),
	((u16)(ArriveTemp.Ch1_Value*10)%10));
	#elif ((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(ArriveTemp.Ch1_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch1_Value),
		((u16)(ArriveTemp.Ch1_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(ArriveTemp.Ch1_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch1_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch1_Value),
		((u16)(ArriveTemp.Ch1_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
#endif
	
#if (Print_Sensor_CH_Num >= 2)
	#if ((Sensor_CH2_Config_Type == Sensor_Tmp)||(Sensor_CH2_Config_Type == Sensor_Hum))
	/* 打印传感器通道2最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
	((u16) ArriveTemp.Ch2_Value),
	((u16)(ArriveTemp.Ch2_Value*10)%10));
	#elif ((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(ArriveTemp.Ch2_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch2_Value),
		((u16)(ArriveTemp.Ch2_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(ArriveTemp.Ch2_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch2_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch2_Value),
		((u16)(ArriveTemp.Ch2_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#endif
	/* 空1行 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
#endif
	
#if (Print_Sensor_CH_Num >= 3)
	/* 打印 环境温度. */
	printf(Print_ArriveEnvironmentTemp_Bar_Str);
	
	#if ((Sensor_CH3_Config_Type == Sensor_Tmp)||(Sensor_CH3_Config_Type == Sensor_Hum))
	/* 打印传感器通道3最大值 */
	printf(Print_Data_Body_Data_Str,
	(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
	((u16) ArriveTemp.Ch3_Value),
	((u16)(ArriveTemp.Ch3_Value*10)%10));
	#elif ((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(ArriveTemp.Ch3_Value != Wireless_RF_Card_Data_Rece_Timeout_Value)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch3_Value),
		((u16)(ArriveTemp.Ch3_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}
	#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
		   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))	
	/* 判断 传感器数据是否为 超时接收数据 */
	if(ArriveTemp.Ch3_Value != Gateway_NJSY_TimeoutPara)
	{
		printf(Print_Data_Body_Data_Str,
		(     (ArriveTemp.Ch3_Value >= 0.0) ? ' ':'-'),
		((u16) ArriveTemp.Ch3_Value),
		((u16)(ArriveTemp.Ch3_Value*10)%10));
	}else
	{
		/* 超时接收数据 打印缺失 */
		printf(Print_Data_Body_Rece_Timeout_Data_Str);	
	}	
	#endif
#endif
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行 */
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
	
//	/* 5、打印 单据编号. */	
//	printf(Print_FBillNo_Str);
//	/* 等待标题打印结束 */
//	OSTimeDly(200);
//	/* 空1行. */	
//	Print_Set_CRLF();
//	/* 等待标题打印结束 */
//	OSTimeDly(120);
	
	//现在有GPS信息配置信息来判断是否为车载设备
#if (GPS_Config_Enable == 0x01)
	/* 4、打印 车牌号. */	
	printf(Print_Card_NO_Str,Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO);
	/* 等待标题打印结束 */
	OSTimeDly(200);
	/* 空1行. */	
	Print_Set_CRLF();
	/* 等待标题打印结束 */
	OSTimeDly(120);
#endif
#endif

#if ((Print_ZJUEE_LOGO == 0x01) && (Print_Function_Enable == 0x01))
	/* 5、打印公司LOGO二维码. */
	Print_Picture((char*)Print_LOGO);
#endif

	/* 6、设置禁止下划线. */
	Print_Set_Underline_Config(DISABLE);
	/* 打印分割符 */
	Print_Indivisible_Symbol();
	
}

/* 一键打印状态标志 */
static BOOL EasyPrintState = FALSE;
/* 一键打印超时基准时间 */
static u32 BaseOutTimeEasyPrint = 0;

/*************************************************************************************************/
/* 函数名	: Easy_Print_Get_Out_Time													 	 	 */
/* 输入		: 无																		 		 */
/* 输出		: 快捷打印状态 																		 */
/* 作用		: 输出快捷打印状态															 	 	 */
/* 创建日期  : 2016/04/21																		 */
/*************************************************************************************************/
BOOL Easy_Print_Get_State(void)
{
	return EasyPrintState;
}

/*************************************************************************************************/
/* 函数名	: Easy_Print_Get_Out_Time													 	 	 */
/* 输入		: 无																		 		 */
/* 输出		: 打印剩余时间 																		 */
/* 作用		: 输出打印剩余时间																 	 */
/* 创建日期  : 2016/04/21																		 */
/*************************************************************************************************/
u32 Easy_Print_Get_Out_Time(void)
{
	return (EasyPrintOutTime - (OSTimeGet() - BaseOutTimeEasyPrint))/1000;
}

/*************************************************************************************************/
/* 函数名	: Easy_Print_Process_Flow													 	 	 */
/* 输入		: 无																		 		 */
/* 输出		: 无 																				 */
/* 作用		: 一键打印流程																 	 	 */
/* 创建日期  : 2016/04/08																		 */
/*************************************************************************************************/
void Easy_Print_Process_Flow(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf)
{

	/* 一键开始打印基准时间 */
	static u32 BaseTimeStartEasyPrint = 0;
	/* 一键停止打印基准时间 */
	static u32 BaseTimeStopEasyPrint = 0;
	/* 打印的起始地址 */
	static int print_start_adder = 0;
	/* 打印的结束地址 */
	static int print_end_adder = 0;
	/* 首次打印日期标志 */
	static BOOL printDateFlag = TRUE;
	
	/* 打印传感器通道的最值 */
	static PRINT_SENSOR_MAX_MIN_VALUE print_sensor_max_min_value = 
	{
#if (Print_Sensor_CH_Num >= 1)
	/* 打印通道1的最值 */
	/* 最大值 */
	0,
	/* 最小值 */
	0,
#endif	
#if (Print_Sensor_CH_Num >= 2)	
	/* 打印通道2的最值 */
	/* 最大值 */
	0,
	/* 最小值 */
	0,
#endif	
#if (Print_Sensor_CH_Num >= 3)
	/* 打印通道3的最值 */
	/* 最大值 */
	0,
	/* 最小值 */
	0,
#endif
#if (Print_Sensor_CH_Num >= 4)
	/* 打印通道4的最值 */
	/* 最大值 */
	0,
	/* 最小值 */
	0,	
#endif
	};
	
	
	/* 判断一键打印状态标志 */
	if(EasyPrintState == TRUE)
	{
		/* 判断‘>’键是否长按大于4S，停止一键打印 */
		if(IsKeyStopEasyPrint() == Bit_SET)
		{
			/* 判断按下时间长度 */
			if((OSTimeGet() - BaseTimeStopEasyPrint) >= StopEasyPrintTime)
			{

				//打印结束内容
				/* 输出数据尾内容 */
				Easy_Print_Data_Tail(&print_sensor_max_min_value);
				
				/* 设置打印日期标志 */
				printDateFlag = TRUE;
				
				/* 记录当前的时间为开始一键打印时间 */ 
				BaseTimeStartEasyPrint = OSTimeGet();
				
				/* 刷新显示 */
				Flash = FlashMode_ManualInit;	
				/* 复位一键打印标志 */
				EasyPrintState = FALSE;
				/* 复位打印数据状态 */
				EasyPrintDataState = RESET;
				
				/* 复位打印通道的最值 */
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
			/* 记录当前的时间为停止一键打印时间 */
			BaseTimeStopEasyPrint = OSTimeGet();
		}
		
		/* 判断‘<’键是否按下 */
		if(IsKeyStartEasyPrint() == Bit_SET)
		{
			//打印一条数据
			Easy_Print_Data_Body(sensor_inf,&print_sensor_max_min_value,&print_start_adder,&print_end_adder,&printDateFlag);
		}
		
		/* 判断退出超时时间 */
		if((IsKeyStopEasyPrint() == Bit_RESET)&&(IsKeyStartEasyPrint() == Bit_RESET))
		{
			/* 记录当前的时间为开始一键打印时间 */ 
			if(OSTimeGet() - BaseOutTimeEasyPrint >= EasyPrintOutTime)
			{
				/* 复位一键打印标志 */
				EasyPrintState = FALSE;
				//打印结束内容
				/* 输出数据尾内容 */
				Easy_Print_Data_Tail(&print_sensor_max_min_value);
				
				/* 设置打印日期标志 */
				printDateFlag = TRUE;
				
				/* 记录当前的时间为开始一键打印时间 */ 
				BaseTimeStartEasyPrint = OSTimeGet();
				
				/* 刷新显示 */
				Flash = FlashMode_ManualInit;	
				/* 复位一键打印标志 */
				EasyPrintState = FALSE;
				/* 复位打印数据状态 */
				EasyPrintDataState = RESET;
				
				/* 复位打印通道的最值 */
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
			/* 记录当前的时间为开始一键打印时间 */
			BaseOutTimeEasyPrint = OSTimeGet();
		}

	}else
	{
		/* 一键打印功能未开始 */
		/* 判断‘<’键是否长按大于4S，开始一键打印 */
		if(IsKeyStartEasyPrint() == Bit_SET)
		{
			/* 判断按下时间长度 */
			if((OSTimeGet() - BaseTimeStartEasyPrint) >= StartEasyPrintTime)
			{
				
				//1、
				/* 设置打印参数 */
				Print_Set_Para();
				//2、
				/* 搜索打印数据的起始位置. */
				print_start_adder = print_inf->Inf_State.Print_Record_Start_Adder;
				//3、
				/* 搜索打印数据的结束位置. */
				print_end_adder = print_inf->Inf_State.Print_Record_End_Adder;
				//4、
				/* 判断搜索的起始地址和结束地址是否合法（起始地址 小于等于 结束地址） */
				if(print_start_adder > print_end_adder)
				{
					/* 起始地址 大于 结束地址 */
					/* 设置打印结束地址 */
					print_end_adder += Flash_Capacity_Size;
				}
				//5、
				/* 判断是否无数据打印 */
				if(print_end_adder == 0)
				{
					/* 没有打印数据 */
					print_end_adder = -1;
				}
				
				/* 记录当前的时间为停止一键打印时间 */ 
				BaseTimeStopEasyPrint = BaseOutTimeEasyPrint;
				/* 刷新显示 */
				Flash = FlashMode_ManualInit;
				
				/* 设置一键打印标志 */
				EasyPrintState = TRUE;
				/* 记录当前的时间为开始一键打印时间 */
				BaseOutTimeEasyPrint = OSTimeGet();
				
#if (Print_Scheme == 1)
				
				/* 清除出库时间寄存器和到达时间寄存器数据 */
				memset(&StartOffTemp,0x00,sizeof(StartOffTemp));
				memset(&ArriveTemp,0x00,sizeof(ArriveTemp));
				/* 搜索出库时间 */
				Easy_Print_Search_StartOff_Time(sensor_inf,&StartOffTemp,print_start_adder,print_end_adder);
				/* 搜索到达时间 */
				Easy_Print_Search_Arrive_Time(sensor_inf,&ArriveTemp,print_start_adder,print_end_adder);
#endif	
				//打印起始内容
				Easy_Print_Data_Title(sensor_inf,print_inf);
			}
		}else
		{
			/* 记录当前的时间为开始一键打印时间 */
			BaseTimeStartEasyPrint = OSTimeGet();
		}
	}
}



#endif

/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/
