/************************************************************************************************/
/** @file              : App_GPS.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年8月11日															*/
/** @brief             : c file																	*/
/** @description       : GPS定位应用文件														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "System_Config.h"

#if (GPS_Config_Enable == 0x01)


#include "App_GPS.h"





/************************************************************************************************/
/* GPS定位应用数据寄存器 */
INSTRU_GPS_POS_App_DR GPS_POS_App_DR = 
{
	/* 纬度 */
	GPS_Data_Rece_Timeout_Value,
	/* 经度 */
	GPS_Data_Rece_Timeout_Value,
	/* 速度 */
	GPS_Data_Rece_Timeout_Value,
};


/************************************************************************************************/
/* GPS定位应用状态 */
INSTRU_GPS_POS_App_SR GPS_POS_App_SR =
{
	/* 纬度数据更新状态 */
	FALSE,
	/* 经度数据更新状态 */
	FALSE,
	/* 速度数据更新状态 */
	FALSE,

};


/************************************************************************************************/
/* 函数名	: GPS_App_Set_Rece_Timeout_Para														*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: 设置GPS接收超时应用参数															*/
/* 创建日期	: 2015/8/12																			*/
/************************************************************************************************/
void GPS_App_Set_Rece_Timeout_Para(void)
{

	/* 设置 纬度 数据 */
	GPS_POS_App_DR.Lat = GPS_Data_Rece_Timeout_Value;
	
	/* 设置 精度 数据 */
	GPS_POS_App_DR.Lon = GPS_Data_Rece_Timeout_Value;
	
	/* 设置 速度 数据 */
	GPS_POS_App_DR.Spd = GPS_Data_Rece_Timeout_Value;

	/* 清除 纬度数据更新状态 */
	GPS_POS_App_SR.Lat_Updata_State = FALSE;
	
	/* 清除 经度数据更新状态 */
	GPS_POS_App_SR.Lon_Updata_State = FALSE;

	/* 清除 速度数据更新状态 */
	GPS_POS_App_SR.Spd_Updata_State = FALSE;


}


/************************************************************************************************/
/* 函数名	: GPS_App_Get_Data														 	 		*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用	 	: 获取GPS应用层需要的数据						 									*/
/* 创建日期	: 2015/08/11																		*/
/************************************************************************************************/
void GPS_App_Get_Data(INSTRU_GPS_POS_Log_SR *state,INSTRU_GPS_POS *data)
{



	//假如直接获得的数据效果不好的话就得做相应的处理。

	/* 判断当前的GPS数据 解析 状态。 */
	if(state->Ana_State == TRUE)
	{
		/* 解析成功 */

		/* 设置 纬度 数据 */
		GPS_POS_App_DR.Lat = data->Lat;
		
		/* 设置 经度 数据 */
		GPS_POS_App_DR.Lon = data->Lon;
		
		/* 设置 速度 数据 */
		GPS_POS_App_DR.Spd = data->Spd;
		
//		/* 设置 纬度 数据 */
//		GPS_POS_App_DR.Lat = -10;
//		
//		/* 设置 经度 数据 */
//		GPS_POS_App_DR.Lon = -10;
//		
//		/* 设置 速度 数据 */
//		GPS_POS_App_DR.Spd = data->Spd;

		/* 设置纬度数据更新状态 */
		GPS_POS_App_SR.Lat_Updata_State = TRUE;
		
		/* 设置经度数据更新状态 */
		GPS_POS_App_SR.Lon_Updata_State = TRUE;
		
		/* 设置速度数据更新状态 */
		GPS_POS_App_SR.Spd_Updata_State = TRUE;
		
		/* 清0 GPS数据接收超时寄存器 */
     	state->Rece_Timeout_Reg = 0;
	}else
	{
		/* 解析失败 */

		/* 清除GPS定位参数 */
		GPS_Log_RESET_POS_Inf();
		
		/* 判断GPS应用数据状态 */
		if((GPS_POS_App_SR.Lat_Updata_State == TRUE)&&
		   (GPS_POS_App_SR.Lon_Updata_State == TRUE)&&
		   (GPS_POS_App_SR.Spd_Updata_State == TRUE))
		{
			/* 当前数据为新的数据，等待应用读取。 */
			__nop();
		}else
		{
			/* 当前数据为旧的数据，等待更新。 */

			/* 判断数据接收超时 */
			if(state->Rece_Timeout_Reg >= GPS_Data_Rece_Timeout)
			{
				/* 数据接收超时 */
				/* 设置GPS接收超时逻辑参数 */
				GPS_Log_Set_Rece_Timeout_Para();
				/* 设置GPS接收超时应用参数 */
				GPS_App_Set_Rece_Timeout_Para();
				
			}else
			{
				/* 数据接收未超时 */
				/* 使用当前未更新的数据作为应用数据 */
				__nop();
			}
		}
	}

}


/************************************************************************************************/
/* 函数名	: GPS_App_Send_Lat_Data												 	 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 经度数据. 																		*/
/* 作用	 	: 输出经度数据						 												*/
/* 创建日期	: 2015/08/19																		*/
/************************************************************************************************/
float GPS_App_Send_Lat_Data(void)
{
	/* 清除 纬度数据更新状态 */
	GPS_POS_App_SR.Lat_Updata_State = FALSE;

	return GPS_POS_App_DR.Lat;
}


/************************************************************************************************/
/* 函数名	: GPS_App_Send_Lon_Data												 	 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 纬度数据. 																		*/
/* 作用	 	: 输出纬度数据						 												*/
/* 创建日期	: 2015/08/19																		*/
/************************************************************************************************/
float GPS_App_Send_Lon_Data(void)
{
	

	/* 清除 经度数据更新状态 */
	GPS_POS_App_SR.Lon_Updata_State = FALSE;

	return GPS_POS_App_DR.Lon;
}


/************************************************************************************************/
/* 函数名	: GPS_App_Send_Spd_Data												 	 			*/
/* 输入  	: 无			 																	*/
/* 输出		: 速度数据. 																		*/
/* 作用	 	: 输出速度数据						 												*/
/* 创建日期	: 2015/08/19																		*/
/************************************************************************************************/
float GPS_App_Send_Spd_Data(void)
{
	

	/* 清除 速度数据更新状态 */
	GPS_POS_App_SR.Spd_Updata_State = FALSE;

	return GPS_POS_App_DR.Spd;
}





#endif


/******************* (C) ZHOU *****END OF FILE****/



