/*************************************************************************************************/
/*************************************     32位平台测试     **************************************/
/* 创建日期：2014/09/15																			 */
/*************************************************************************************************/
#include "Dev_Fram.h"
#include "Mcu_Fram.h"
#include "interface.h"

#include  "ucos_ii.h"
#include <string.h>








/*************************************************************************************************/
/* 函数名：Write_Config_Tab																		 */
/* 描述  ：写 仪器 配置信息表																		 */
/* 输入  ：buffer:数据存储指针；count:写取数据数量													 */
/* 返回  ：无																					 */
/* 注意  ：这里使用的是模拟I2C																		 */
/*************************************************************************************************/
void Write_Config_Tab(u8* buffer_p,u16 count)
{
/* 配置表信息 */
/* 仪器配置表开始地址 */
#define Instru_Config_Tab_Start_Adder	((u32)&Instru_Config_Inf_Tab.Inf_State_Space[0])
/* 仪器配置表结束地址 */
#define Instru_Config_Tab_End_Adder		((u32)&Instru_Config_Inf_Tab.Inf_State_Space[Instru_Inf_State_Space_Size])	
/* 仪器配置表开始地址 */
#define Instru_Fix_Tab_Start_Adder		((u32)&Instru_Fix_Inf_State_Tab.Inf_State_Space[0])
/* 仪器配置表结束地址 */
#define Instru_Fix_Tab_End_Adder		((u32)&Instru_Fix_Inf_State_Tab.Inf_State_Space[Instru_Fix_Inf_Tab_Space_Size])		

	
	/* 写数据地址 */
	u32 w_adder;
	
	/* 根据数据缓存指针判断本次操作是否执行 */
	if(((u32)buffer_p 				>= Instru_Config_Tab_Start_Adder)&&
	  (((u32)buffer_p+count) 		<= Instru_Config_Tab_End_Adder))
	{
		/* 设置写数据地址(配置表地址) */
		w_adder = (((u32)buffer_p)-Instru_Config_Tab_Start_Adder)+Instru_Inf_Tab_Mem_Star;
	}else if(((u32)buffer_p 		>= Instru_Fix_Tab_Start_Adder)&&
			(((u32)buffer_p+count) 	<= Instru_Fix_Tab_End_Adder))
	{
		/* 设置写数据地址(配置表地址) */
		w_adder = (((u32)buffer_p)-Instru_Fix_Tab_Start_Adder)+Instru_Fix_Inf_Tab_Mem_Star;
	}
	else
	{
		/* 写的数据部分或全部数据不再可写的地址范围内。 */
		return;
	}
	
	/* 使能写保护 */
	FramWP_GPIO_Enable();
	/* 给调度器上锁 */
	OSSchedLock();
	/* 将 指定数据缓存的 数据 写到数据配置表的FRAM中 */
	AI2C_Write((char*)buffer_p,w_adder,count);
	/* 给调度器解锁 */
	OSSchedUnlock();
	/* 关闭写保护 */
	FramWP_GPIO_DISABLE();
	
#undef Instru_Config_Tab_Start_Adder
#undef Instru_Config_Tab_End_Adder
#undef Instru_Fix_Tab_Start_Adder
#undef Instru_Fix_Tab_End_Adder	
}

/*************************************************************************************************/
/* 函数名：Read_Config_Tab																		 */
/* 描述  ：读 仪器 配置信息表																		 */
/* 输入  ：buffer:数据存储指针；count:写取数据数量													 */
/* 返回  ：无																					 */
/* 注意  ：这里使用的是模拟I2C																		 */
/*************************************************************************************************/
void Read_Config_Tab(u8* buffer_p,u16 count)
{
/* 配置表信息 */
/* 仪器配置表开始地址 */
#define Instru_Config_Tab_Start_Adder	((u32)&Instru_Config_Inf_Tab.Inf_State_Space[0])
/* 仪器配置表结束地址 */
#define Instru_Config_Tab_End_Adder		((u32)&Instru_Config_Inf_Tab.Inf_State_Space[Instru_Inf_State_Space_Size])	
/* 仪器配置表开始地址 */
#define Instru_Fix_Tab_Start_Adder		((u32)&Instru_Fix_Inf_State_Tab.Inf_State_Space[0])
/* 仪器配置表结束地址 */
#define Instru_Fix_Tab_End_Adder		((u32)&Instru_Fix_Inf_State_Tab.Inf_State_Space[Instru_Fix_Inf_Tab_Space_Size])			

	
	/* 写数据地址 */
	u32 r_adder;

	
	/* 根据数据缓存指针判断本次操作是否执行 */
	if(((u32)buffer_p 				>= Instru_Config_Tab_Start_Adder)&&
	  (((u32)buffer_p+count) 		<= Instru_Config_Tab_End_Adder))
	{
		/* 设置写数据地址(配置表地址) */
		r_adder = (((u32)buffer_p)-Instru_Config_Tab_Start_Adder)+Instru_Inf_Tab_Mem_Star;
	}else if(((u32)buffer_p 		>= Instru_Fix_Tab_Start_Adder)&&
			(((u32)buffer_p+count) 	<= Instru_Fix_Tab_End_Adder))
	{
		/* 设置写数据地址(配置表地址) */
		r_adder = (((u32)buffer_p)-Instru_Fix_Tab_Start_Adder)+Instru_Fix_Inf_Tab_Mem_Star;
	}
	else
	{
		/* 写的数据部分或全部数据不再可写的地址范围内。 */
		return;
	}
	
	/* 给调度器上锁 */
	OSSchedLock();
	/* 将 指定数据缓存的 数据 写到数据配置表的FRAM中 */
	AI2C_Read((char*)buffer_p,r_adder,count);
	/* 给调度器解锁 */
	OSSchedUnlock();
	
#undef Instru_Config_Tab_Start_Adder
#undef Instru_Config_Tab_End_Adder
#undef Instru_Fix_Tab_Start_Adder
#undef Instru_Fix_Tab_End_Adder	
}





/*************************END OF FILE*************************************/
