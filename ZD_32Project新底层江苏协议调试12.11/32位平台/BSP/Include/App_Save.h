#ifndef __App_Save_H
#define	__App_Save_H

#include "stm32f10x.h"

#include "Log_Time.h" 
#include "Log_Save.h"
#include "Mcu_Config.h"







/********************************* 存储探头数据帧结构 *****************************/
typedef struct
{
	/* 时间戳 */
	Times Timestamp;
	
	/* 传感器数据 */
	float Sensor[Sensor_CH_Config_Num];
}AppSaveFrameStruct;

/********************************* 读探头存储状态 *********************************/
typedef enum {Save_ReadFail = (0u), Save_ReadSuccess = (1u)}Save_ReadState;

/********************************* 网关应用接口结构 *******************************/
typedef struct
{	
	
	uint32_t		(*ReadSaveNum)			(void);
	Save_ReadState 	(*ReadSaveDataFrame)	(AppSaveFrameStruct* dataFrame);
}AppSaveAPIStruct;





/************************************************************************************************/
/********************************* 探头数据存储应用接口寄存器声明 *********************************/
extern AppSaveAPIStruct AppSaveAPI;


/********************************* 函数声明 *********************************/
void AppSave_InitAPI(void);
#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
void App_Save_Config(FunctionalState New_state);
void App_Save_Print_Start_upload_Adder(void);//记录卸货开始数据记录地址
void App_Save_Print_Stop_upload_Adder(void);//记录卸货结束数据记录地址
#endif





#endif /* __App_GPS_H */


