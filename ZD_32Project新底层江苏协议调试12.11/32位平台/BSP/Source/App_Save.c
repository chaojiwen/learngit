/************************************************************************************************/
/** @file              : App_Save.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年8月27日															*/
/** @brief             : c file																	*/
/** @description       : 应用数据存储文件														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "System_Config.h"

#include "App_Save.h"

#include "Dev_Fram.h"
#include "Dev_Flash.h"
#include "Dev_RTC.h"

#include "ucos_ii.h"
#include <string.h>
#include "app.h"

#include "stdlib.h"



/********************************* 探头数据存储应用接口寄存器定义 *********************************/

/********************************* 探头数据存储应用接口寄存器定义 *********************************/
AppSaveAPIStruct AppSaveAPI;

/************************************************************************************************/

#if (Menu_Save_Data_Func == 0x01)
/* 实时数据存储开关 数据指针 */
static FunctionalState *Save_Data_Switch_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch;
/* 实时数据存储开关 数据大小 */
#define Save_Data_Switch_size (sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch))
	
/* 卸货开始 数据指针 */
#define Save_Data_Start_Upload_P    (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_upload_Adder)
/* 卸货开始 数据大小 */
#define Save_Data_Start_Upload_size (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_upload_Adder))
/* 卸货结束 数据指针 */
#define Save_Data_Stop_Upload_P     (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[48])
/* 卸货结束 数据大小 */
#define Save_Data_Stop_Upload_size  (sizeof(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[48]))

#endif


/************************************************************************************************/
/* 函数名	: AppSave_ReadNum													 	 			*/
/* 输入  	: 无.																				*/
/* 输出		: 无. 																				*/
/* 作用	 	: 读探头数据存储数量																	*/
/* 创建日期	: 2016/12/22																		*/
/************************************************************************************************/
static uint32_t AppSave_ReadNum(void)
{
	/* 返回探头数据存储数量 */
	return Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM;
}


/************************************************************************************************/
/* 函数名	: AppSave_ReadSaveDataFrame													 	 	*/
/* 输入  	: dataFrame:传感器数据帧指针.														*/
/* 输出		: 读存储数据结果 																	*/
/* 作用	 	: 读存储数据.																		*/
/* 创建日期	: 2016/12/22																		*/
/************************************************************************************************/
Save_ReadState AppSave_ReadSaveDataFrame(AppSaveFrameStruct* dataFrame)
{
	/* 读传感器数据指针 */
	Save_Sensor_Data_Struct* sReadDatap;
	
	
	/* 判断探头存储数据数量 */
	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM < Save_Sensor_Data_Len)
	{
		/* 没有探头数据 */
		__nop();
		/* 读探头数据失败 */
		return Save_ReadFail;
	}
	
	/* 有存储数据，读取数据。 */
	sReadDatap = (Save_Sensor_Data_Struct*)Flash_Read_Sensor_Data(
				 (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size),
				 Save_Sensor_Data_Len);

	//设置 Flash 读指针
	/* 曾加 读取的数据长度 */
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Read_Pointer 		+= Save_Sensor_Data_Len; 
	/* 存储数据量 减*/
	Instru_Fix_Inf_State_Tab.Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	-= Save_Sensor_Data_Len;
	
	/* 设置采集时间 */
	dataFrame->Timestamp.Year 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS + 2000;
	dataFrame->Timestamp.Month	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS;
	dataFrame->Timestamp.Day 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS;
	dataFrame->Timestamp.Hour 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS;
	dataFrame->Timestamp.Min 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS;
	dataFrame->Timestamp.Sec 	= sReadDatap->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS;
	/* 复制传感器数据 */
	memcpy(dataFrame->Sensor,sReadDatap->GPRS_IR.Sensor_Data,sizeof(dataFrame->Sensor));
	
	/* 读探头数据成功 */
	return Save_ReadSuccess;
}

/************************************************************************************************/
/* 函数名	: AppSave_InitAPI										 	 						*/
/* 输入  	: 无																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 初始化探头存储接口功能					 											*/
/* 创建日期	: 2016/12/16																		*/
/* 软件版本	: V0																				*/
/************************************************************************************************/
void AppSave_InitAPI(void)
{

	/* 读探头数据存储数量。 */
	AppSaveAPI.ReadSaveNum = AppSave_ReadNum;

	/* 读探头数据存储数量。 */
	AppSaveAPI.ReadSaveDataFrame = AppSave_ReadSaveDataFrame;

}


#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
/************************************************************************************************/
/* 函数名	: App_Save_Write														 	 		*/
/* 输入  	: New_state:新的存储状态(TRUE:启动数据存储功能;FALSE:关闭数据存储功能).				*/
/* 输出		: 无. 																				*/
/* 作用	 	: 写存储数据.																		*/
/* 创建日期	: 2015/08/27																		*/
/************************************************************************************************/
void App_Save_Config(FunctionalState New_state)
{

	/* 判断 实时数据存储开关 与 设置的状态是否一致 */
	if(*Save_Data_Switch_P != New_state)
	{
		/* 不一致,修改实时数据存储开关状态. */
		*Save_Data_Switch_P = New_state;
		/* 将新的实时数据存储状态保存起来. */
		Write_Config_Tab((u8*)Save_Data_Switch_P,Save_Data_Switch_size);
	}else
	{
		/* 设置的状态与实时数据存储开关相同,无需更改状态. */
	}


}

/************************************************************************************************
*函数名     : App_Save_Print_Start_upload_Adder															
*输入参数  	: 无 									
*输出参数	  : 无 																				
*作用	 	    : 存储卸货开始时数据记录的地址以便于根据这个地址来确定卸货开始的地址															
*创建日期	  : 2018/05/23																		
************************************************************************************************/
void App_Save_Print_Start_upload_Adder(void)
{
	
   u32  Temp_Print_Start_Upload_Counter=0;//获取当前卸货开始点记录位置
	
	 Temp_Print_Start_Upload_Counter=Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter;
	
	
			 if(((Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter-  \
		     Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter)==1) || \
			 ( (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter > \
			   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter))
		   ) 
			  //使卸货开始点与卸货结束点成对出现   如不相同则结束设置操作
		  {
		     return ;
		  }
	
	  if(Temp_Print_Start_Upload_Counter>99)//当卸货开始点超过了100组,则将信息村粗点重头开始记录  实现循环存储
		{
		   Temp_Print_Start_Upload_Counter=0;
		}
		
	
  *(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp_Print_Start_Upload_Counter])=   \
	  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder;
		
	/* 存储卸货点地址方便后期打印卸货标志做铺垫 */
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp_Print_Start_Upload_Counter]), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp_Print_Start_Upload_Counter])));
	
	 Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter++;//每触发一次开始卸货动作 计数值加一
	/* 存储卸货开始点组数 以便后期打印多个卸货点*/
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter)));
	
	/*将卸货开始标志位置为1 根据此标志是否显示卸货开始图标 1：显示  0：不显示 */
	  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag=1;
	
	
	
	/* 存储卸货开始点组数 以便后期打印多个卸货点*/
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag)));
	
	
	
//	 printf("卸货开始  地址:%d\n", *(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Adder[Temp_Print_Start_Upload_Counter]));
}
/************************************************************************************************
*函数名     : App_Save_Print_Start_upload_Adder															
*输入参数  	: 无 									
*输出参数	  : 无 																				
*作用	 	    : 存储卸货结束时数据记录的地址以便于根据这个地址来确定卸货开始的地址															
*创建日期	  : 2018/05/23																		
************************************************************************************************/
void App_Save_Print_Stop_upload_Adder(void)
{

  u32  Temp_Print_Stop_Upload_Counter=0;//获取当前卸货结束点记录位置
	
	 Temp_Print_Stop_Upload_Counter=Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter;
	
			 if(((Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter-  \
		     Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter)==1) || \
			 ( (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter > \
			   Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Counter))
		   ) 
			  //使卸货开始点与卸货结束点成对出现   如不相同则结束设置操作
		  {
		     return ;
		  }
			
			
		if(Temp_Print_Stop_Upload_Counter>99)//当卸货结束点超过了100组,则将信息村粗点重头开始记录  实现循环存储
		{
		   Temp_Print_Stop_Upload_Counter=0;
		}
  *(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp_Print_Stop_Upload_Counter])=   \
	  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Record_End_Adder;
		
	/* 存储卸货结束点地址方便后期打印卸货标志做铺垫 */
	Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp_Print_Stop_Upload_Counter]), \
	sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp_Print_Stop_Upload_Counter])));
	
	 Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter++;//每触发一次结束卸货动作 计数值加一
	/* 存储卸货结束点组数 以便后期打印多个卸货点*/
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Counter)));
	
	/*将卸货开始标志位置为0 根据此标志是否显示卸货开始图标 1：显示  0：不显示  */
	  Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag=0;
	
	
	/* 存储显示卸货开始图标标志位信息*/
		Write_Config_Tab((u8*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag), \
	   sizeof(*(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Start_Upload_Pic_Flag)));
	


//	printf("卸货结束   地址:%d\n", *(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Stop_Upload_Adder[Temp_Print_Stop_Upload_Counter]));
}


#endif


/******************* (C) ZHOU *****END OF FILE****/

















