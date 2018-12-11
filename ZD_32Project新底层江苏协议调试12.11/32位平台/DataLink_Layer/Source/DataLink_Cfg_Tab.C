/************************************************************************************************/
/** @file              : DataLink_Cfg_Tab.c														*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年11月06日															*/
/** @date changed      : 2015年11月06日															*/
/** @brief             : c file																	*/
/** @description       : 配置表(数据链路层)														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/

#include "DataLink_Cfg_Tab.h" 
#include "Dev_AI2C.h"

#include "Mcu_Config.h"



/* 配置表信息存储在存储器当中的物理地址 */
#define DataLink_CfgTab_Inf_Sre_Add			(0X0000)	//从0K的地方开始存
/* 配置表信息存储在存储器当中的空间大小 */
#define DataLink_CfgTab_Inf_Sre_Space_size	(0X400)		//1K的最大空间

/* 配置表信息存储在存储器当中的物理地址 */
#define DataLink_CfgTab_Para_Sre_Add		(0X1000)	//从0K的地方开始存
/* 配置表信息存储在存储器当中的空间大小 */
#define DataLink_CfgTab_Para_Sre_Space_size	(0X1000)	//4K的最大空间



/************************************************************************************************/
/* 函数名	: DataLink_Cfg_Tab_Inf_Read															*/
/* 输入		: Data_Src_Add:		数据目标(物理)地址												*/
/*			: Data_Offset_Add:	数据偏移(相对)地址												*/
/*			: Data_Size:		数据大小														*/
/* 输出		: 无 																				*/
/* 作用		: 读取 存储在外部存储器中的配置表 信息 数据											*/
/* 创建日期	: 2015/11/06																		*/
/************************************************************************************************/
void DataLink_Cfg_Tab_Inf_Read(char *Data_Src_Add,int Data_Offset_Add,int Data_Size)
{
	/* 写存储地址大小判断 */
	if((Data_Offset_Add < 0)&&(Data_Offset_Add > (DataLink_CfgTab_Inf_Sre_Add+DataLink_CfgTab_Inf_Sre_Space_size))&&
	  ((Data_Offset_Add + Data_Size) > DataLink_CfgTab_Inf_Sre_Space_size))
	{
		/* 写地址超出范围内。 */
		return;
	}
	/* I2C 读 数据  */
	Dev_AI2C_Read(Data_Src_Add,DataLink_CfgTab_Inf_Sre_Add+Data_Offset_Add,Data_Size);
}

/************************************************************************************************/
/* 函数名	: DataLink_Cfg_Tab_Inf_Write														*/
/* 输入		: Data_Src_Add:		数据目标(物理)地址												*/
/*			: Data_Offset_Add:	数据偏移(相对)地址												*/
/*			: Data_Size:		数据大小														*/
/* 输出		: 无 																				*/
/* 作用		: 写入 存储在外部存储器中的配置表 信息 数据											*/
/* 创建日期	: 2015/11/06																		*/
/************************************************************************************************/
void DataLink_Cfg_Tab_Inf_Write(char *Data_Src_Add,int Data_Offset_Add,int Data_Size)
{

	/* 写存储地址大小判断 */
	if((Data_Offset_Add < 0)&&(Data_Offset_Add > (DataLink_CfgTab_Inf_Sre_Add+DataLink_CfgTab_Inf_Sre_Space_size))&&
	  ((Data_Offset_Add + Data_Size) > DataLink_CfgTab_Inf_Sre_Space_size))
	{
		/* 写地址超出范围内。 */
		return;
	}

	/* I2C 读 数据 */
	Dev_AI2C_Write(Data_Src_Add,DataLink_CfgTab_Inf_Sre_Add+Data_Offset_Add,Data_Size);


}


/************************************************************************************************/
/* 函数名	: DataLink_Cfg_Tab_Para_Read														*/
/* 输入		: Data_Src_Add:		数据目标(物理)地址												*/
/*			: Data_Offset_Add:	数据偏移(相对)地址												*/
/*			: Data_Size:		数据大小															*/
/* 输出		: 无 																				*/
/* 作用		: 读取 存储在外部存储器中的配置表 参数 数据												*/
/* 创建日期	: 2015/11/06																		*/
/************************************************************************************************/
void DataLink_Cfg_Tab_Para_Read(char *Data_Src_Add,int Data_Offset_Add,int Data_Size)
{
	
	/* 写存储地址大小判断 */
	if((Data_Offset_Add < 0)&&(Data_Offset_Add > (DataLink_CfgTab_Para_Sre_Add+DataLink_CfgTab_Para_Sre_Space_size))&&
	  ((Data_Offset_Add + Data_Size) > DataLink_CfgTab_Para_Sre_Space_size))
	{
		/* 写地址超出范围内。 */
		return;
	}


	/* I2C 读 数据  */
	Dev_AI2C_Read(Data_Src_Add,DataLink_CfgTab_Para_Sre_Add+Data_Offset_Add,Data_Size);


}


/************************************************************************************************/
/* 函数名	: DataLink_Cfg_Tab_Para_Write														*/
/* 输入		: Data_Src_Add:		数据目标(物理)地址												*/
/*			: Data_Offset_Add:	数据偏移(相对)地址												*/
/*			: Data_Size:		数据大小														*/
/* 输出		: 无 																				*/
/* 作用		: 写入 存储在外部存储器中的配置表 参数 数据											*/
/* 创建日期	: 2015/11/06																		*/
/************************************************************************************************/
void DataLink_Cfg_Tab_Para_Write(char *Data_Src_Add,int Data_Offset_Add,int Data_Size)
{

	/* 写存储地址大小判断 */
	if((Data_Offset_Add < 0)&&(Data_Offset_Add > (DataLink_CfgTab_Para_Sre_Add+DataLink_CfgTab_Para_Sre_Space_size))&&
	  ((Data_Offset_Add + Data_Size) > DataLink_CfgTab_Para_Sre_Space_size))
	{
		/* 写地址超出范围内。 */
		return;
	}

	/* I2C 读 数据 */
	Dev_AI2C_Write(Data_Src_Add,DataLink_CfgTab_Para_Sre_Add+Data_Offset_Add,Data_Size);


}










