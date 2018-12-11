/************************************************************************************************/
/** @file              : Log_Print.h															*/
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

#ifndef __Log_Print_H
#define __Log_Print_H

#include "System_Config.h"

#include "stm32f10x.h"

#include "Log_Sensor.h"

#include "Mcu_Config.h"




//判断是否使能打印功能
#if (Print_Function_Enable == 0x01)
//使能打印功能

	/* 传感器通道数量最大为4个 */
	#define Print_Sensor_CH_Num_Max 	(0x04)

	#if (Print_Sensor_CH_Num > Print_Sensor_CH_Num_Max)
	#error	"设置的传感器数量大于4个传感器通道。"
	#endif

	#if (Print_Sensor_CH_Num > Sensor_CH_Config_Num)
	#error	"设置的传感器数量大于传感器通道配置数量。"
	#endif


	//判断设置的打印的通道数量 是否大于1
	#if (Print_Sensor_CH_Num >= 0x01)
		//判断 通道一 的类型是否可打印的传感器类型
		#if ((Sensor_CH1_Config_Type != Sensor_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_Hum) &&\
			 (Sensor_CH1_Config_Type != Sensor_RF_Card_Single_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_RF_Card_Humiture_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_RF_Card_Humiture_Hum) &&\
			 (Sensor_CH1_Config_Type != Sensor_PT1000) &&\
			 (Sensor_CH1_Config_Type != Sensor_GW_NJSY_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_GW_NJSY_Hum) &&\
			 (Sensor_CH1_Config_Type != Sensor_GW_HZZH_Tmp) &&\
			 (Sensor_CH1_Config_Type != Sensor_GW_HZZH_Hum))
			 
		#error "传感器通道<1>配置设定为不可打印的传感器类型。"
		#endif
	#endif

	//判断设置的打印的通道数量 是否大于2
	#if (Print_Sensor_CH_Num >= 0x02)
		//判断 通道二 的类型是否可打印的传感器类型
		#if ((Sensor_CH2_Config_Type != Sensor_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_Hum) &&\
			 (Sensor_CH2_Config_Type != Sensor_RF_Card_Single_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_RF_Card_Humiture_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_RF_Card_Humiture_Hum) &&\
			 (Sensor_CH2_Config_Type != Sensor_PT1000) &&\
			 (Sensor_CH2_Config_Type != Sensor_GW_NJSY_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_GW_NJSY_Hum) &&\
			 (Sensor_CH2_Config_Type != Sensor_GW_HZZH_Tmp) &&\
			 (Sensor_CH2_Config_Type != Sensor_GW_HZZH_Hum))
		#error "传感器通道<2>配置设定为不可打印的传感器类型。"
		#endif
	#endif

	//判断设置的打印的通道数量 是否大于3
	#if (Print_Sensor_CH_Num >= 0x03)
		//判断 通道三 的类型是否可打印的传感器类型
		#if ((Sensor_CH3_Config_Type != Sensor_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_Hum) &&\
			 (Sensor_CH3_Config_Type != Sensor_RF_Card_Single_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_RF_Card_Humiture_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_RF_Card_Humiture_Hum) &&\
			 (Sensor_CH3_Config_Type != Sensor_PT1000) &&\
			 (Sensor_CH3_Config_Type != Sensor_GW_NJSY_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_GW_NJSY_Hum) &&\
			 (Sensor_CH3_Config_Type != Sensor_GW_HZZH_Tmp) &&\
			 (Sensor_CH3_Config_Type != Sensor_GW_HZZH_Hum))
		#error "传感器通道<3>配置设定为不可打印的传感器类型。"
		#endif
	#endif

	//判断设置的打印的通道数量 是否大于4
	#if (Print_Sensor_CH_Num == 0x04)
		//判断 通道四 的类型是否可打印的传感器类型
		#if ((Sensor_CH4_Config_Type != Sensor_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_Hum) &&\
			 (Sensor_CH4_Config_Type != Sensor_RF_Card_Single_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_RF_Card_Humiture_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_RF_Card_Humiture_Hum) &&\
			 (Sensor_CH4_Config_Type != Sensor_PT1000) &&\
			 (Sensor_CH4_Config_Type != Sensor_GW_NJSY_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_GW_NJSY_Hum) &&\
			 (Sensor_CH4_Config_Type != Sensor_GW_HZZH_Tmp) &&\
			 (Sensor_CH4_Config_Type != Sensor_GW_HZZH_Hum))
		#error "传感器通道<4>配置设定为不可打印的传感器类型。"
		#endif
	#endif

#endif


/************************************************************************************************/


/************************************************************************************************/
/* 函数声明 */
void Print_Index_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
/* 打印数据输出 */
void Print_Data_Output(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf);
/* 快捷打印数据输出 */
void Easy_Print_Process_Flow(INSTRU_SENSOR* sensor_inf,INSTRU_PRINT_FIX* print_inf);
/* 快捷打印状态 */
BOOL Easy_Print_Get_State(void);
/* 快捷打印剩余时间 */
u32 Easy_Print_Get_Out_Time(void);


#endif  
/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/ 



