#ifndef __Log_Sensor_H
#define __Log_Sensor_H

#include "System_Config.h"
#include "stm32f10x.h"
#include "Mcu_Config.h"






/************************************************************************************************/

/* 计算 各种类型传感器的通道数量 */
#if (Sensor_Function_Config_Enable != 0x00)

/* 传感器通道1 */
#define Sensors_CH_1    (0u)
/* 传感器通道2 */
#define Sensors_CH_2    (1u)
/* 传感器通道3 */
#define Sensors_CH_3    (2u)
/* 传感器通道4 */
#define Sensors_CH_4    (3u)
/* 传感器通道5 */
#define Sensors_CH_5    (4u)
/* 传感器通道6 */
#define Sensors_CH_6    (5u)
/* 传感器通道7 */
#define Sensors_CH_7    (6u)
/* 传感器通道8 */
#define Sensors_CH_8    (7u)
/* 传感器通道9 */
#define Sensors_CH_9    (8u)
/* 传感器通道10 */
#define Sensors_CH_10    (9u)
/* 传感器通道11 */
#define Sensors_CH_11    (10u)
/* 传感器通道12 */
#define Sensors_CH_12    (11u)
/* 传感器通道13 */
#define Sensors_CH_13    (12u)
/* 传感器通道14 */
#define Sensors_CH_14    (13u)
/* 传感器通道15 */
#define Sensors_CH_15    (14u)
/* 传感器通道16 */
#define Sensors_CH_16    (15u)
/* 传感器通道17 */
#define Sensors_CH_17    (16u)
/* 传感器通道18 */
#define Sensors_CH_18    (17u)
/* 传感器通道19 */
#define Sensors_CH_19    (18u)
/* 传感器通道20 */
#define Sensors_CH_20    (19u)
/* 传感器通道21 */
#define Sensors_CH_21    (20u)
/* 传感器通道22 */
#define Sensors_CH_22    (21u)
/* 传感器通道23 */
#define Sensors_CH_23    (22u)
/* 传感器通道24 */
#define Sensors_CH_24    (23u)
/* 传感器通道25 */
#define Sensors_CH_25    (24u)
/* 传感器通道26 */
#define Sensors_CH_26    (25u)
/* 传感器通道27 */
#define Sensors_CH_27    (26u)
/* 传感器通道28 */
#define Sensors_CH_28    (27u)
/* 传感器通道29 */
#define Sensors_CH_29    (28u)
/* 传感器通道30 */
#define Sensors_CH_30    (29u)
/* 传感器通道31 */
#define Sensors_CH_31    (30u)
/* 传感器通道32 */
#define Sensors_CH_32    (31u)

/* 传感器通道数量0 */
#define CH_Num_0    	(0u)
/* 传感器通道数量1 */
#define CH_Num_1   		(1u)
/* 传感器通道数量2 */
#define CH_Num_2    	(2u)
/* 传感器通道数量3 */
#define CH_Num_3   		(3u)
/* 传感器通道数量4 */
#define CH_Num_4    	(4u)
/* 传感器通道数量5 */
#define CH_Num_5    	(5u)
/* 传感器通道数量6 */
#define CH_Num_6    	(6u)
/* 传感器通道数量7 */
#define CH_Num_7    	(7u)
/* 传感器通道数量8 */
#define CH_Num_8    	(8u)
/* 传感器通道数量9 */
#define CH_Num_9    	(9u)
/* 传感器通道数量10 */
#define CH_Num_10    	(10u)
/* 传感器通道数量11 */
#define CH_Num_11    	(11u)
/* 传感器通道数量12 */
#define CH_Num_12    	(12u)
/* 传感器通道数量13 */
#define CH_Num_13   	(13u)
/* 传感器通道数量14 */
#define CH_Num_14    	(14u)
/* 传感器通道数量15 */
#define CH_Num_15    	(15u)
/* 传感器通道数量16 */
#define CH_Num_16    	(16u)
/* 传感器通道数量17 */
#define CH_Num_17   	(17u)
/* 传感器通道数量18 */
#define CH_Num_18   	(18u)
/* 传感器通道数量19 */
#define CH_Num_19    	(19u)
/* 传感器通道数量20 */
#define CH_Num_20   	(20u)
/* 传感器通道数量21 */
#define CH_Num_21    	(21u)
/* 传感器通道数量22 */
#define CH_Num_22    	(22u)
/* 传感器通道数量23 */
#define CH_Num_23   	(23u)
/* 传感器通道数量24 */
#define CH_Num_24   	(24u)
/* 传感器通道数量25 */
#define CH_Num_25    	(25u)
/* 传感器通道数量26 */
#define CH_Num_26    	(26u)
/* 传感器通道数量27 */
#define CH_Num_27    	(27u)
/* 传感器通道数量28 */
#define CH_Num_28   	(28u)
/* 传感器通道数量29 */
#define CH_Num_29    	(29u)
/* 传感器通道数量30 */
#define CH_Num_30    	(30u)
/* 传感器通道数量31 */
#define CH_Num_31    	(31u)
/* 传感器通道数量32 */
#define CH_Num_32   	(32u)



/* 温度传感器通道数量 */
#define Sensor_Tmp_Num					(0x00)
/* 湿度传感器通道数量 */
#define Sensor_Hum_Num					(0x00)

/* 单温卡片通道数量 */
#define Sensor_RF_Single_Tmp_Num		(0x00)

/* 温湿卡片通道数量 */
#define Sensor_RF_Humiture_Num			(0x00)
/* 温湿卡片温度通道数量 */
#define Sensor_RF_Humiture_Tmp_Num		(0x00)
/* 温湿卡片湿度通道数量 */
#define Sensor_RF_Humiture_Hum_Num		(0x00)

/* 南京盛亿网关温度探头数量 */
#define Sensor_GW_NJSY_Tmp_Num			(0x00)
/* 南京盛亿网关湿度探头数量 */
#define Sensor_GW_NJSY_Hum_Num			(0x00)
/* 南京盛亿网关探头数量 */
#define Sensor_GW_NJSY_Num				(0x00)

/* 杭州创辉网关温度探头数量 */
#define Sensor_GW_HZZH_Tmp_Num			(0x00)
/* 杭州创辉网关湿度探头数量 */
#define Sensor_GW_HZZH_Hum_Num			(0x00)
/* 杭州创辉网关探头数量 */
#define Sensor_GW_HZZH_Num				(0x00)


/* 经度通道数量 */
#define Sensor_Lon_Num					(0x00)
/* 纬度通道数量 */
#define Sensor_Lat_Num					(0x00)
/* 速度通道数量 */
#define Sensor_Spd_Num					(0x00)

/* 速度通道数量 */
#define Sensor_PT1000_Num				(0x00)

/* 二氧化碳通道数量 */
#define Sensor_CarbonDioxide_Num	(0x00)

/* 门磁开关通道数量 */
#define Sensor_DoorContact_Num	(0x00)


/* 计算 温度传感器通道数量 */
#if (Sensor_CH1_Config_Type == Sensor_Tmp)
	#if	(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_CarbonDioxide)	
	#if	(Sensor_CarbonDioxide_Num == CH_Num_0)
		#undef 	Sensor_CarbonDioxide_Num
		#define Sensor_CarbonDioxide_Num				(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_Hum)	
	#if	(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if	(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if	(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if	(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#endif	
#elif	(Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#endif
#elif	(Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#endif		

#endif

#if (Sensor_CH2_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_CarbonDioxide)	
	#if		(Sensor_CarbonDioxide_Num == CH_Num_0)
		#undef 	Sensor_CarbonDioxide_Num
		#define Sensor_CarbonDioxide_Num				(CH_Num_1)
	#elif	(Sensor_CarbonDioxide_Num == CH_Num_1)
		#undef 	Sensor_CarbonDioxide_Num
		#define Sensor_CarbonDioxide_Num				(CH_Num_2)
	#endif	
#elif	(Sensor_CH2_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#endif
#elif	(Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#endif	
	
#endif

#if (Sensor_CH3_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_CarbonDioxide)	
	#if		(Sensor_CarbonDioxide_Num == CH_Num_0)
		#undef 	Sensor_CarbonDioxide_Num
		#define Sensor_CarbonDioxide_Num				(CH_Num_1)
	#elif	(Sensor_CarbonDioxide_Num == CH_Num_1)
		#undef 	Sensor_CarbonDioxide_Num
		#define Sensor_CarbonDioxide_Num				(CH_Num_2)
	#elif	(Sensor_CarbonDioxide_Num == CH_Num_2)
		#undef 	Sensor_CarbonDioxide_Num
		#define Sensor_CarbonDioxide_Num				(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#endif
#elif	(Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#endif	

#endif


#if (Sensor_CH4_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)		
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)		
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#endif
#elif	(Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#endif	

#endif


#if (Sensor_CH5_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)			
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)		
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)		
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)			
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)		
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#endif
#elif	(Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#endif	
	
#endif

#if (Sensor_CH6_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)				
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)			
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)			
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)		
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)		
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)

	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#endif
#elif	(Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#endif	

#endif


#if (Sensor_CH7_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)		
	#endif

#elif	(Sensor_CH7_Config_Type == Sensor_DoorContact)	
	#if		(Sensor_DoorContact_Num == CH_Num_0)
		#undef 	Sensor_DoorContact_Num
		#define Sensor_DoorContact_Num				(CH_Num_1)		
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)			
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)			
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)			
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)		
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)	
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#endif
#elif	(Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#endif	

#endif


#if (Sensor_CH8_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)			
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)		
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)		
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)		
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)	
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)	
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)	
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#endif
#elif	(Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#endif	

#endif

#if (Sensor_CH9_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)		
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)		
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)			
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)			
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)		
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)	
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)	
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)		
	#endif
#elif	(Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#endif

#endif

#if (Sensor_CH10_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)		
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)			
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)			
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)			
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)			
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)	
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)		
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)	
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#endif
#elif	(Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#endif

#endif

#if (Sensor_CH11_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_11)			
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_11)			
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_11)		
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)	
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)			
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#endif
#elif	(Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#endif

#endif

#if (Sensor_CH12_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)	
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)	
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)	
	#elif	(Sensor_Hum_Num == CH_Num_11)	
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)		
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_11)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_12)			
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_11)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_12)				
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_11)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_12)		
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)		
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)		
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#endif
#elif	(Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#endif

#endif

#if (Sensor_CH13_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)		
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)	
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)	
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_11)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_12)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_13)	
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_11)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_12)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_13)		
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_12)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_13)			
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)		
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)	
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)	
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)			
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#endif
#elif	(Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)		
	#endif

#endif

#if (Sensor_CH14_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)	
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)	
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_14)
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_14)
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_14)
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)	
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)		
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#endif
#elif	(Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#endif

#endif

#if (Sensor_CH15_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)		
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)		
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#endif
#elif	(Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#endif

#endif

#if (Sensor_CH16_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)		
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)	
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)	
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)	
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)		
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)		
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)		
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)	
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#endif
#elif	(Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#endif

#endif

#if (Sensor_CH17_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)	
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)		
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)		
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)		
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)		
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)				
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#endif
#elif	(Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#endif

#endif

#if (Sensor_CH18_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)	
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)			
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)		
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)		
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#endif
#elif	(Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#endif

#endif

#if (Sensor_CH19_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)	
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)			
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)		
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 	
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)		
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)				
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)			
	#endif		
#elif	(Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#endif
#elif	(Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#endif

#endif

#if (Sensor_CH20_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 	
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#endif	
#elif	(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)			
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#endif	
#elif	(Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#endif
#elif	(Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#endif

#endif

#if (Sensor_CH21_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)		
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)	
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)			
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)		
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)		
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21) 	
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)		
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)		
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)	
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#endif		
#elif	(Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#endif
#elif	(Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#endif

#endif

#if (Sensor_CH22_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)		
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)		
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)			
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)		
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num				(CH_Num_22)	
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)				
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)			
	#endif	
#elif	(Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#endif
#elif	(Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#endif

#endif

#if (Sensor_CH23_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)			
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)		
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)	
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)		
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)	
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)		
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)		
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)				
	#endif	
#elif	(Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)		
	#endif
#elif	(Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#endif

#endif

#if (Sensor_CH24_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)		
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)		
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)			
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)		
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)		
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)			
	#endif	
#elif	(Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)		
	#endif
#elif	(Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#endif

#endif

#if (Sensor_CH25_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_Tmp_Num == CH_Num_24)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)	
	#elif	(Sensor_Hum_Num == CH_Num_24)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)			
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#elif	(Sensor_Lon_Num == CH_Num_24)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)	
	#elif	(Sensor_Lat_Num == CH_Num_24)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)	
	#elif	(Sensor_Spd_Num == CH_Num_24)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_25)		
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#elif	(Sensor_PT1000_Num == CH_Num_24)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_25)	
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_25)				
	#endif	
#elif	(Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_25)		
	#endif
#elif	(Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_25)		
	#endif

#endif

#if (Sensor_CH26_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_Tmp_Num == CH_Num_24)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_Tmp_Num == CH_Num_25)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_26)		
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)	
	#elif	(Sensor_Hum_Num == CH_Num_24)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_25)
	#elif	(Sensor_Hum_Num == CH_Num_25)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_26)	
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)		
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_26)	
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_26)	
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_26)	
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#elif	(Sensor_Lon_Num == CH_Num_24)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_25)	
	#elif	(Sensor_Lon_Num == CH_Num_25)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_26)		
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)	
	#elif	(Sensor_Lat_Num == CH_Num_24)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_25)	
	#elif	(Sensor_Lat_Num == CH_Num_25)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_26)		
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)	
	#elif	(Sensor_Spd_Num == CH_Num_24)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_25)
	#elif	(Sensor_Spd_Num == CH_Num_25)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_26)	
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#elif	(Sensor_PT1000_Num == CH_Num_24)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_25)	
	#elif	(Sensor_PT1000_Num == CH_Num_25)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_26)	
	#endif	
#elif	(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_26)	
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_26)	
	#endif	
#elif	(Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_25)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_26)	
	#endif
#elif	(Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_26)	
	#endif

#endif

#if (Sensor_CH27_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)		
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_Tmp_Num == CH_Num_24)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_Tmp_Num == CH_Num_25)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_26)	
	#elif	(Sensor_Tmp_Num == CH_Num_26)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_27)			
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)	
	#elif	(Sensor_Hum_Num == CH_Num_24)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_25)
	#elif	(Sensor_Hum_Num == CH_Num_25)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_26)	
	#elif	(Sensor_Hum_Num == CH_Num_26)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_27)		
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_26)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_27)		
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_26)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_27)	
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_26)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_27)	
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#elif	(Sensor_Lon_Num == CH_Num_24)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_25)	
	#elif	(Sensor_Lon_Num == CH_Num_25)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_26)
	#elif	(Sensor_Lon_Num == CH_Num_26)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_27)			
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)	
	#elif	(Sensor_Lat_Num == CH_Num_24)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_25)	
	#elif	(Sensor_Lat_Num == CH_Num_25)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_26)
	#elif	(Sensor_Lat_Num == CH_Num_26)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_27)	
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)	
	#elif	(Sensor_Spd_Num == CH_Num_24)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_25)
	#elif	(Sensor_Spd_Num == CH_Num_25)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_26)	
	#elif	(Sensor_Spd_Num == CH_Num_26)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_27)	
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#elif	(Sensor_PT1000_Num == CH_Num_24)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_25)	
	#elif	(Sensor_PT1000_Num == CH_Num_25)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_26)	
	#elif	(Sensor_PT1000_Num == CH_Num_26)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_27)	
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_27)	
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_27)			
	#endif	
#elif	(Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_25)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_27)	
	#endif
#elif	(Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_27)	
	#endif

#endif

#if (Sensor_CH28_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_Tmp_Num == CH_Num_24)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_Tmp_Num == CH_Num_25)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_26)	
	#elif	(Sensor_Tmp_Num == CH_Num_26)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_27)	
	#elif	(Sensor_Tmp_Num == CH_Num_27)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_28)		
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)	
	#elif	(Sensor_Hum_Num == CH_Num_24)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_25)
	#elif	(Sensor_Hum_Num == CH_Num_25)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_26)	
	#elif	(Sensor_Hum_Num == CH_Num_26)
		#undef 	Sensor_Hum_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_27)
	#elif	(Sensor_Hum_Num == CH_Num_27)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num	(CH_Num_28)	
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_26)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_28)	
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_26)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_28)	
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_26)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_28)	
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#elif	(Sensor_Lon_Num == CH_Num_24)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_25)	
	#elif	(Sensor_Lon_Num == CH_Num_25)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_26)
	#elif	(Sensor_Lon_Num == CH_Num_26)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_27)		
	#elif	(Sensor_Lon_Num == CH_Num_27)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_28)			
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)	
	#elif	(Sensor_Lat_Num == CH_Num_24)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_25)	
	#elif	(Sensor_Lat_Num == CH_Num_25)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_26)
	#elif	(Sensor_Lat_Num == CH_Num_26)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_27)
	#elif	(Sensor_Lat_Num == CH_Num_27)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_28)	
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)	
	#elif	(Sensor_Spd_Num == CH_Num_24)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_25)
	#elif	(Sensor_Spd_Num == CH_Num_25)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_26)	
	#elif	(Sensor_Spd_Num == CH_Num_26)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_27)
	#elif	(Sensor_Spd_Num == CH_Num_27)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_28)	
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#elif	(Sensor_PT1000_Num == CH_Num_24)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_25)	
	#elif	(Sensor_PT1000_Num == CH_Num_25)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_26)	
	#elif	(Sensor_PT1000_Num == CH_Num_26)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_27)	
	#elif	(Sensor_PT1000_Num == CH_Num_27)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_28)	
	#endif		
#elif	(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_28)	
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_27)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_28)		
	#endif	
#elif	(Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_25)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_28)		
	#endif
#elif	(Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_28)		
	#endif

#endif

#if (Sensor_CH29_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_Tmp_Num == CH_Num_24)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_Tmp_Num == CH_Num_25)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_Tmp_Num == CH_Num_26)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_Tmp_Num == CH_Num_27)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_Tmp_Num == CH_Num_28)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_29)
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)
	#elif	(Sensor_Hum_Num == CH_Num_24)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_25)
	#elif	(Sensor_Hum_Num == CH_Num_25)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_26)
	#elif	(Sensor_Hum_Num == CH_Num_26)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_27)
	#elif	(Sensor_Hum_Num == CH_Num_27)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_28)
	#elif	(Sensor_Hum_Num == CH_Num_28)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_29)
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_28)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_29)
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_28)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_29)
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_26)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_28)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_28)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_29)
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#elif	(Sensor_Lon_Num == CH_Num_24)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_25)	
	#elif	(Sensor_Lon_Num == CH_Num_25)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_26)
	#elif	(Sensor_Lon_Num == CH_Num_26)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_27)		
	#elif	(Sensor_Lon_Num == CH_Num_27)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_28)	
	#elif	(Sensor_Lon_Num == CH_Num_28)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_29)		
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)	
	#elif	(Sensor_Lat_Num == CH_Num_24)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_25)	
	#elif	(Sensor_Lat_Num == CH_Num_25)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_26)
	#elif	(Sensor_Lat_Num == CH_Num_26)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_27)
	#elif	(Sensor_Lat_Num == CH_Num_27)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_28)	
	#elif	(Sensor_Lat_Num == CH_Num_28)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_29)			
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)	
	#elif	(Sensor_Spd_Num == CH_Num_24)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_25)
	#elif	(Sensor_Spd_Num == CH_Num_25)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_26)	
	#elif	(Sensor_Spd_Num == CH_Num_26)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_27)
	#elif	(Sensor_Spd_Num == CH_Num_27)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_28)	
	#elif	(Sensor_Spd_Num == CH_Num_28)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_29)		
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#elif	(Sensor_PT1000_Num == CH_Num_24)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_25)	
	#elif	(Sensor_PT1000_Num == CH_Num_25)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_26)	
	#elif	(Sensor_PT1000_Num == CH_Num_26)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_27)	
	#elif	(Sensor_PT1000_Num == CH_Num_27)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_28)	
	#elif	(Sensor_PT1000_Num == CH_Num_28)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_29)	
	#endif	
#elif	(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_28)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_28)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_29)		
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_27)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_28)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_28)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_29)			
	#endif	
#elif	(Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_25)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_28)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_28)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_29)		
	#endif
#elif	(Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_28)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_28)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_29)	
	#endif

#endif

#if (Sensor_CH30_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_Tmp_Num == CH_Num_24)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_Tmp_Num == CH_Num_25)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_Tmp_Num == CH_Num_26)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_Tmp_Num == CH_Num_27)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_Tmp_Num == CH_Num_28)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_Tmp_Num == CH_Num_29)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_30)			
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)
	#elif	(Sensor_Hum_Num == CH_Num_24)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_25)
	#elif	(Sensor_Hum_Num == CH_Num_25)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_26)
	#elif	(Sensor_Hum_Num == CH_Num_26)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_27)
	#elif	(Sensor_Hum_Num == CH_Num_27)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_28)
	#elif	(Sensor_Hum_Num == CH_Num_28)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_29)
	#elif	(Sensor_Hum_Num == CH_Num_29)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_30)	
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_28)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_29)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_30)	
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_28)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_29)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_30)	
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_26)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_28)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_28)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_29)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_29)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_30)	
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#elif	(Sensor_Lon_Num == CH_Num_24)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_25)	
	#elif	(Sensor_Lon_Num == CH_Num_25)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_26)
	#elif	(Sensor_Lon_Num == CH_Num_26)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_27)		
	#elif	(Sensor_Lon_Num == CH_Num_27)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_28)	
	#elif	(Sensor_Lon_Num == CH_Num_28)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_29)
	#elif	(Sensor_Lon_Num == CH_Num_29)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_30)	
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)	
	#elif	(Sensor_Lat_Num == CH_Num_24)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_25)	
	#elif	(Sensor_Lat_Num == CH_Num_25)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_26)
	#elif	(Sensor_Lat_Num == CH_Num_26)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_27)
	#elif	(Sensor_Lat_Num == CH_Num_27)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_28)	
	#elif	(Sensor_Lat_Num == CH_Num_28)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_29)	
	#elif	(Sensor_Lat_Num == CH_Num_29)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_30)		
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)	
	#elif	(Sensor_Spd_Num == CH_Num_24)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_25)
	#elif	(Sensor_Spd_Num == CH_Num_25)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_26)	
	#elif	(Sensor_Spd_Num == CH_Num_26)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_27)
	#elif	(Sensor_Spd_Num == CH_Num_27)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_28)	
	#elif	(Sensor_Spd_Num == CH_Num_28)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_29)	
	#elif	(Sensor_Spd_Num == CH_Num_29)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_30)		
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#elif	(Sensor_PT1000_Num == CH_Num_24)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_25)	
	#elif	(Sensor_PT1000_Num == CH_Num_25)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_26)	
	#elif	(Sensor_PT1000_Num == CH_Num_26)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_27)	
	#elif	(Sensor_PT1000_Num == CH_Num_27)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_28)	
	#elif	(Sensor_PT1000_Num == CH_Num_28)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_29)	
	#elif	(Sensor_PT1000_Num == CH_Num_29)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_30)
	#endif		
#elif	(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_28)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_28)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_29)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_29)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_30)	
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_27)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_28)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_28)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_29)			
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_29)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_30)			
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_25)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_28)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_28)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_29)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_29)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_30)		
	#endif
#elif	(Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_28)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_28)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_29)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_29)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_30)		
	#endif

#endif

#if (Sensor_CH31_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_Tmp_Num == CH_Num_24)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_Tmp_Num == CH_Num_25)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_Tmp_Num == CH_Num_26)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_Tmp_Num == CH_Num_27)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_Tmp_Num == CH_Num_28)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_Tmp_Num == CH_Num_29)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_30)	
	#elif	(Sensor_Tmp_Num == CH_Num_30)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_31)	
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)
	#elif	(Sensor_Hum_Num == CH_Num_24)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_25)
	#elif	(Sensor_Hum_Num == CH_Num_25)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_26)
	#elif	(Sensor_Hum_Num == CH_Num_26)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_27)
	#elif	(Sensor_Hum_Num == CH_Num_27)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_28)
	#elif	(Sensor_Hum_Num == CH_Num_28)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_29)
	#elif	(Sensor_Hum_Num == CH_Num_29)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_30)	
	#elif	(Sensor_Hum_Num == CH_Num_30)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_31)
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)			
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_28)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_29)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_30)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_30)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_31)
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_28)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_29)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_30)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_30)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_31)
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_26)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_28)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_28)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_29)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_29)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_30)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_30)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_31)
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#elif	(Sensor_Lon_Num == CH_Num_24)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_25)	
	#elif	(Sensor_Lon_Num == CH_Num_25)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_26)
	#elif	(Sensor_Lon_Num == CH_Num_26)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_27)		
	#elif	(Sensor_Lon_Num == CH_Num_27)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_28)	
	#elif	(Sensor_Lon_Num == CH_Num_28)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_29)
	#elif	(Sensor_Lon_Num == CH_Num_29)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_30)	
	#elif	(Sensor_Lon_Num == CH_Num_30)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_31)		
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)	
	#elif	(Sensor_Lat_Num == CH_Num_24)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_25)	
	#elif	(Sensor_Lat_Num == CH_Num_25)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_26)
	#elif	(Sensor_Lat_Num == CH_Num_26)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_27)
	#elif	(Sensor_Lat_Num == CH_Num_27)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_28)	
	#elif	(Sensor_Lat_Num == CH_Num_28)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_29)	
	#elif	(Sensor_Lat_Num == CH_Num_29)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_30)	
	#elif	(Sensor_Lat_Num == CH_Num_30)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_31)		
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)	
	#elif	(Sensor_Spd_Num == CH_Num_24)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_25)
	#elif	(Sensor_Spd_Num == CH_Num_25)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_26)	
	#elif	(Sensor_Spd_Num == CH_Num_26)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_27)
	#elif	(Sensor_Spd_Num == CH_Num_27)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_28)	
	#elif	(Sensor_Spd_Num == CH_Num_28)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_29)	
	#elif	(Sensor_Spd_Num == CH_Num_29)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_30)
	#elif	(Sensor_Spd_Num == CH_Num_30)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_31)	
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#elif	(Sensor_PT1000_Num == CH_Num_24)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_25)	
	#elif	(Sensor_PT1000_Num == CH_Num_25)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_26)	
	#elif	(Sensor_PT1000_Num == CH_Num_26)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_27)	
	#elif	(Sensor_PT1000_Num == CH_Num_27)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_28)	
	#elif	(Sensor_PT1000_Num == CH_Num_28)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_29)	
	#elif	(Sensor_PT1000_Num == CH_Num_29)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_30)
	#elif	(Sensor_PT1000_Num == CH_Num_30)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_31)	
	#endif	
#elif	(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_28)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_28)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_29)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_29)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_30)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_30)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num	(CH_Num_31) 
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_27)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_28)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_28)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_29)			
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_29)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_30)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_30)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_31)			
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_25)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_28)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_28)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_29)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_29)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_30)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_30)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_31)	
	#endif
#elif	(Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_28)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_28)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_29)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_29)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_30)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_30)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_31)	
	#endif

#endif

#if (Sensor_CH32_Config_Type == Sensor_Tmp)
	#if		(Sensor_Tmp_Num == CH_Num_0)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_1)
	#elif	(Sensor_Tmp_Num == CH_Num_1)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_2)
	#elif	(Sensor_Tmp_Num == CH_Num_2)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_3)
	#elif	(Sensor_Tmp_Num == CH_Num_3)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_4)	
	#elif	(Sensor_Tmp_Num == CH_Num_4)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_5)		
	#elif	(Sensor_Tmp_Num == CH_Num_5)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_6)
	#elif	(Sensor_Tmp_Num == CH_Num_6)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_7)	
	#elif	(Sensor_Tmp_Num == CH_Num_7)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_8)
	#elif	(Sensor_Tmp_Num == CH_Num_8)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_9)
	#elif	(Sensor_Tmp_Num == CH_Num_9)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_10)
	#elif	(Sensor_Tmp_Num == CH_Num_10)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_Tmp_Num == CH_Num_11)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_Tmp_Num == CH_Num_12)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_Tmp_Num == CH_Num_13)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_Tmp_Num == CH_Num_14)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_Tmp_Num == CH_Num_15)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_Tmp_Num == CH_Num_16)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_Tmp_Num == CH_Num_17)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_Tmp_Num == CH_Num_18)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_Tmp_Num == CH_Num_19)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_20)	
	#elif	(Sensor_Tmp_Num == CH_Num_20)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_Tmp_Num == CH_Num_21)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_Tmp_Num == CH_Num_22)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_23)
	#elif	(Sensor_Tmp_Num == CH_Num_23)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_Tmp_Num == CH_Num_24)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_Tmp_Num == CH_Num_25)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_Tmp_Num == CH_Num_26)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_Tmp_Num == CH_Num_27)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_Tmp_Num == CH_Num_28)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_Tmp_Num == CH_Num_29)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_30)
	#elif	(Sensor_Tmp_Num == CH_Num_30)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_31)
	#elif	(Sensor_Tmp_Num == CH_Num_31)
		#undef 	Sensor_Tmp_Num
		#define Sensor_Tmp_Num				(CH_Num_32)
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_Hum)	
	#if		(Sensor_Hum_Num == CH_Num_0)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_1)
	#elif	(Sensor_Hum_Num == CH_Num_1)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_2)
	#elif	(Sensor_Hum_Num == CH_Num_2)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_3)
	#elif	(Sensor_Hum_Num == CH_Num_3)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_4)
	#elif	(Sensor_Hum_Num == CH_Num_4)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_5)	
	#elif	(Sensor_Hum_Num == CH_Num_5)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_6)		
	#elif	(Sensor_Hum_Num == CH_Num_6)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_7)	
	#elif	(Sensor_Hum_Num == CH_Num_7)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_8)
	#elif	(Sensor_Hum_Num == CH_Num_8)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_9)	
	#elif	(Sensor_Hum_Num == CH_Num_9)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_10)	
	#elif	(Sensor_Hum_Num == CH_Num_10)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_11)
	#elif	(Sensor_Hum_Num == CH_Num_11)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_12)
	#elif	(Sensor_Hum_Num == CH_Num_12)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_13)
	#elif	(Sensor_Hum_Num == CH_Num_13)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_14)
	#elif	(Sensor_Hum_Num == CH_Num_14)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_15)
	#elif	(Sensor_Hum_Num == CH_Num_15)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_16)
	#elif	(Sensor_Hum_Num == CH_Num_16)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_Hum_Num == CH_Num_17)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_Hum_Num == CH_Num_18)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_19)
	#elif	(Sensor_Hum_Num == CH_Num_19)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_20)	
	#elif	(Sensor_Hum_Num == CH_Num_20)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_21)
	#elif	(Sensor_Hum_Num == CH_Num_21)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_Hum_Num == CH_Num_22)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_Hum_Num == CH_Num_23)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_24)
	#elif	(Sensor_Hum_Num == CH_Num_24)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_25)
	#elif	(Sensor_Hum_Num == CH_Num_25)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_26)
	#elif	(Sensor_Hum_Num == CH_Num_26)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_27)
	#elif	(Sensor_Hum_Num == CH_Num_27)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_28)
	#elif	(Sensor_Hum_Num == CH_Num_28)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_29)
	#elif	(Sensor_Hum_Num == CH_Num_29)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_30)
	#elif	(Sensor_Hum_Num == CH_Num_30)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_31)
	#elif	(Sensor_Hum_Num == CH_Num_31)
		#undef 	Sensor_Hum_Num
		#define Sensor_Hum_Num				(CH_Num_32)
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)	
	#if		(Sensor_RF_Single_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_3)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_4)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_4)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_5)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_5)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_6)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_7)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_8)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_8)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_9)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num	(CH_Num_10)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_23)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_28)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_29)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_30)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_30)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_31)
	#elif	(Sensor_RF_Single_Tmp_Num == CH_Num_31)
		#undef 	Sensor_RF_Single_Tmp_Num
		#define Sensor_RF_Single_Tmp_Num				(CH_Num_32)
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
	#if		(Sensor_RF_Humiture_Tmp_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_3)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_4)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_4)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_5)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_6)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_6)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_7)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_8)		
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_8)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_9)		
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num	(CH_Num_10)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_24)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_26)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_28)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_28)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_29)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_29)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_30)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_30)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_31)
	#elif	(Sensor_RF_Humiture_Tmp_Num == CH_Num_31)
		#undef 	Sensor_RF_Humiture_Tmp_Num
		#define Sensor_RF_Humiture_Tmp_Num				(CH_Num_32)
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
	#if		(Sensor_RF_Humiture_Hum_Num == CH_Num_0)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_1)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_1)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_2)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_2)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_3)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_3)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_4)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_4)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_5)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_5)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_6)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_6)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_7)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_7)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_8)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_8)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_9)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_9)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num	(CH_Num_10)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_10)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_11)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_11)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_12)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_12)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_13)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_13)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_14)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_14)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_15)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_15)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_16)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_16)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_17)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_17)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_18)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_18)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_19)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_19)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_20)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_20)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_21)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_21)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_22)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_22)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_23)	
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_23)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_24)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_24)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_25)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_25)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_26)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_26)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_27)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_27)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_28)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_28)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_29)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_29)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_30)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_30)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_31)
	#elif	(Sensor_RF_Humiture_Hum_Num == CH_Num_31)
		#undef 	Sensor_RF_Humiture_Hum_Num
		#define Sensor_RF_Humiture_Hum_Num				(CH_Num_32)
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon)
	#if	(Sensor_Lon_Num == CH_Num_0)
		#undef 	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_1)
	#elif	(Sensor_Lon_Num == CH_Num_1)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_2)
	#elif	(Sensor_Lon_Num == CH_Num_2)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_3)
	#elif	(Sensor_Lon_Num == CH_Num_3)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_4)
	#elif	(Sensor_Lon_Num == CH_Num_4)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_5)
	#elif	(Sensor_Lon_Num == CH_Num_5)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_6)
	#elif	(Sensor_Lon_Num == CH_Num_6)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_7)
	#elif	(Sensor_Lon_Num == CH_Num_7)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_8)
	#elif	(Sensor_Lon_Num == CH_Num_8)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_9)
	#elif	(Sensor_Lon_Num == CH_Num_9)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_10)
	#elif	(Sensor_Lon_Num == CH_Num_10)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_11)
	#elif	(Sensor_Lon_Num == CH_Num_11)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_12)	
	#elif	(Sensor_Lon_Num == CH_Num_12)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_13)
	#elif	(Sensor_Lon_Num == CH_Num_13)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_14)	
	#elif	(Sensor_Lon_Num == CH_Num_14)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_15)	
	#elif	(Sensor_Lon_Num == CH_Num_15)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_16)	
	#elif	(Sensor_Lon_Num == CH_Num_16)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_17)	
	#elif	(Sensor_Lon_Num == CH_Num_17)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_18)		
	#elif	(Sensor_Lon_Num == CH_Num_18)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_19) 
	#elif	(Sensor_Lon_Num == CH_Num_19)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_20) 
	#elif	(Sensor_Lon_Num == CH_Num_20)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_21)
	#elif	(Sensor_Lon_Num == CH_Num_21)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_22)	
	#elif	(Sensor_Lon_Num == CH_Num_22)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_23)
	#elif	(Sensor_Lon_Num == CH_Num_23)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_24)	
	#elif	(Sensor_Lon_Num == CH_Num_24)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_25)	
	#elif	(Sensor_Lon_Num == CH_Num_25)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_26)
	#elif	(Sensor_Lon_Num == CH_Num_26)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_27)		
	#elif	(Sensor_Lon_Num == CH_Num_27)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_28)	
	#elif	(Sensor_Lon_Num == CH_Num_28)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_29)
	#elif	(Sensor_Lon_Num == CH_Num_29)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_30)	
	#elif	(Sensor_Lon_Num == CH_Num_30)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_31)
	#elif	(Sensor_Lon_Num == CH_Num_31)
		#undef	Sensor_Lon_Num
		#define Sensor_Lon_Num				(CH_Num_32)	
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat)
	#if	(Sensor_Lat_Num == CH_Num_0)
		#undef 	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_1)
	#elif	(Sensor_Lat_Num == CH_Num_1)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_2)
	#elif	(Sensor_Lat_Num == CH_Num_2)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_3)
	#elif	(Sensor_Lat_Num == CH_Num_3)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_4)
	#elif	(Sensor_Lat_Num == CH_Num_4)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_5)
	#elif	(Sensor_Lat_Num == CH_Num_5)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_6)
	#elif	(Sensor_Lat_Num == CH_Num_6)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_7)
	#elif	(Sensor_Lat_Num == CH_Num_7)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_8)
	#elif	(Sensor_Lat_Num == CH_Num_8)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_9)	
	#elif	(Sensor_Lat_Num == CH_Num_9)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_10)
	#elif	(Sensor_Lat_Num == CH_Num_10)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_11)	
	#elif	(Sensor_Lat_Num == CH_Num_11)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_12)
	#elif	(Sensor_Lat_Num == CH_Num_12)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_13)
	#elif	(Sensor_Lat_Num == CH_Num_13)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_14)	
	#elif	(Sensor_Lat_Num == CH_Num_14)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_15)	
	#elif	(Sensor_Lat_Num == CH_Num_15)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_16)	
	#elif	(Sensor_Lat_Num == CH_Num_16)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_17)	
	#elif	(Sensor_Lat_Num == CH_Num_17)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_18)
	#elif	(Sensor_Lat_Num == CH_Num_18)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_19)	
	#elif	(Sensor_Lat_Num == CH_Num_19)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_20)		
	#elif	(Sensor_Lat_Num == CH_Num_20)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_21)	
	#elif	(Sensor_Lat_Num == CH_Num_21)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_22)	
	#elif	(Sensor_Lat_Num == CH_Num_22)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_23)		
	#elif	(Sensor_Lat_Num == CH_Num_23)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_24)	
	#elif	(Sensor_Lat_Num == CH_Num_24)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_25)	
	#elif	(Sensor_Lat_Num == CH_Num_25)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_26)
	#elif	(Sensor_Lat_Num == CH_Num_26)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_27)
	#elif	(Sensor_Lat_Num == CH_Num_27)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_28)	
	#elif	(Sensor_Lat_Num == CH_Num_28)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_29)	
	#elif	(Sensor_Lat_Num == CH_Num_29)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_30)	
	#elif	(Sensor_Lat_Num == CH_Num_30)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_31)	
	#elif	(Sensor_Lat_Num == CH_Num_31)
		#undef	Sensor_Lat_Num
		#define Sensor_Lat_Num				(CH_Num_32)		
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd)
	#if	(Sensor_Spd_Num == CH_Num_0)
		#undef 	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_1)
	#elif	(Sensor_Spd_Num == CH_Num_1)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_2)
	#elif	(Sensor_Spd_Num == CH_Num_2)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_3)
	#elif	(Sensor_Spd_Num == CH_Num_3)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_4)
	#elif	(Sensor_Spd_Num == CH_Num_4)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_5)
	#elif	(Sensor_Spd_Num == CH_Num_5)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_6)
	#elif	(Sensor_Spd_Num == CH_Num_6)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_7)
	#elif	(Sensor_Spd_Num == CH_Num_7)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_8)
	#elif	(Sensor_Spd_Num == CH_Num_8)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_9)
	#elif	(Sensor_Spd_Num == CH_Num_9)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_10)
	#elif	(Sensor_Spd_Num == CH_Num_10)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_11)	
	#elif	(Sensor_Spd_Num == CH_Num_11)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_12)
	#elif	(Sensor_Spd_Num == CH_Num_12)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_13)
	#elif	(Sensor_Spd_Num == CH_Num_13)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_14)
	#elif	(Sensor_Spd_Num == CH_Num_14)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_15)	
	#elif	(Sensor_Spd_Num == CH_Num_15)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_16)	
	#elif	(Sensor_Spd_Num == CH_Num_16)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_17)	
	#elif	(Sensor_Spd_Num == CH_Num_17)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_18)	
	#elif	(Sensor_Spd_Num == CH_Num_18)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_19)
	#elif	(Sensor_Spd_Num == CH_Num_19)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_20)	
	#elif	(Sensor_Spd_Num == CH_Num_20)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_21)
	#elif	(Sensor_Spd_Num == CH_Num_21)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_22)	
	#elif	(Sensor_Spd_Num == CH_Num_22)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_23)	
	#elif	(Sensor_Spd_Num == CH_Num_23)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_24)	
	#elif	(Sensor_Spd_Num == CH_Num_24)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_25)
	#elif	(Sensor_Spd_Num == CH_Num_25)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_26)	
	#elif	(Sensor_Spd_Num == CH_Num_26)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_27)
	#elif	(Sensor_Spd_Num == CH_Num_27)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_28)	
	#elif	(Sensor_Spd_Num == CH_Num_28)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_29)	
	#elif	(Sensor_Spd_Num == CH_Num_29)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_30)
	#elif	(Sensor_Spd_Num == CH_Num_30)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_31)	
	#elif	(Sensor_Spd_Num == CH_Num_31)
		#undef	Sensor_Spd_Num
		#define Sensor_Spd_Num				(CH_Num_32)		
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_PT1000)
	#if	(Sensor_PT1000_Num == CH_Num_0)
		#undef 	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_1)
	#elif	(Sensor_PT1000_Num == CH_Num_1)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_2)
	#elif	(Sensor_PT1000_Num == CH_Num_2)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_3)
	#elif	(Sensor_PT1000_Num == CH_Num_3)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_4)
	#elif	(Sensor_PT1000_Num == CH_Num_4)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_5)
	#elif	(Sensor_PT1000_Num == CH_Num_5)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_6)
	#elif	(Sensor_PT1000_Num == CH_Num_6)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_7)	
	#elif	(Sensor_PT1000_Num == CH_Num_7)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_8)	
	#elif	(Sensor_PT1000_Num == CH_Num_8)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_9)	
	#elif	(Sensor_PT1000_Num == CH_Num_9)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_10)	
	#elif	(Sensor_PT1000_Num == CH_Num_10)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_11)	
	#elif	(Sensor_PT1000_Num == CH_Num_11)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_12)	
	#elif	(Sensor_PT1000_Num == CH_Num_12)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_13)	
	#elif	(Sensor_PT1000_Num == CH_Num_13)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_14)	
	#elif	(Sensor_PT1000_Num == CH_Num_14)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_15)	
	#elif	(Sensor_PT1000_Num == CH_Num_15)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_16)
	#elif	(Sensor_PT1000_Num == CH_Num_16)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_17)		
	#elif	(Sensor_PT1000_Num == CH_Num_17)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_18)		
	#elif	(Sensor_PT1000_Num == CH_Num_18)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_19)		
	#elif	(Sensor_PT1000_Num == CH_Num_19)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_20)	
	#elif	(Sensor_PT1000_Num == CH_Num_20)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_21)	
	#elif	(Sensor_PT1000_Num == CH_Num_21)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_22)	
	#elif	(Sensor_PT1000_Num == CH_Num_22)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_23)	
	#elif	(Sensor_PT1000_Num == CH_Num_23)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_24)		
	#elif	(Sensor_PT1000_Num == CH_Num_24)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_25)	
	#elif	(Sensor_PT1000_Num == CH_Num_25)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_26)	
	#elif	(Sensor_PT1000_Num == CH_Num_26)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_27)	
	#elif	(Sensor_PT1000_Num == CH_Num_27)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_28)	
	#elif	(Sensor_PT1000_Num == CH_Num_28)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_29)	
	#elif	(Sensor_PT1000_Num == CH_Num_29)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_30)
	#elif	(Sensor_PT1000_Num == CH_Num_30)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_31)	
	#elif	(Sensor_PT1000_Num == CH_Num_31)
		#undef	Sensor_PT1000_Num
		#define Sensor_PT1000_Num			(CH_Num_32)	
	#endif	
	
#elif	(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)
	#if	(Sensor_GW_NJSY_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_14)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_20)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_21)		
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_22)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_28)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_28)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_29)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_29)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num		(CH_Num_30)	
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_30)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num	(CH_Num_31) 
	#elif	(Sensor_GW_NJSY_Tmp_Num == CH_Num_31)
		#undef	Sensor_GW_NJSY_Tmp_Num
		#define Sensor_GW_NJSY_Tmp_Num	(CH_Num_32) 		
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
	#if	(Sensor_GW_NJSY_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_9)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_11)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_22)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_27)		
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_28)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_28)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_29)			
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_29)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_30)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_30)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_31)	
	#elif	(Sensor_GW_NJSY_Hum_Num == CH_Num_31)
		#undef	Sensor_GW_NJSY_Hum_Num
		#define Sensor_GW_NJSY_Hum_Num		(CH_Num_32)			
	#endif
	
#elif	(Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)
	#if	(Sensor_GW_HZZH_Tmp_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_9)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_13)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_19)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_22)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_24)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_25)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_28)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_28)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_29)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_29)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_30)		
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_30)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_31)	
	#elif	(Sensor_GW_HZZH_Tmp_Num == CH_Num_31)
		#undef	Sensor_GW_HZZH_Tmp_Num
		#define Sensor_GW_HZZH_Tmp_Num		(CH_Num_32)		
	#endif
#elif	(Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum)
	#if	(Sensor_GW_HZZH_Hum_Num == CH_Num_0)
		#undef 	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_1)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_1)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_2)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_2)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_3)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_3)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_4)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_4)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_5)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_5)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_6)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_6)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_7)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_7)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_8)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_8)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_9)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_9)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_10)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_10)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_11)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_11)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_12)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_12)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_13)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_13)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_14)
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_14)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_15)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_15)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_16)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_16)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_17)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_17)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_18)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_18)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_19)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_19)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_20)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_20)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_21)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_21)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_22)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_22)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_23)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_23)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_24)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_24)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_25)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_25)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_26)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_26)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_27)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_27)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_28)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_28)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_29)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_29)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_30)		
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_30)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_31)	
	#elif	(Sensor_GW_HZZH_Hum_Num == CH_Num_31)
		#undef	Sensor_GW_HZZH_Hum_Num
		#define Sensor_GW_HZZH_Hum_Num		(CH_Num_32)	
	#endif

#endif

#if	(Sensor_RF_Humiture_Tmp_Num != Sensor_RF_Humiture_Hum_Num)
	#error "温湿度卡片通道配置不正确"
#else
	#undef	Sensor_RF_Humiture_Num
	#define	Sensor_RF_Humiture_Num	(Sensor_RF_Humiture_Tmp_Num)
#endif

#if	(Sensor_GW_NJSY_Tmp_Num != Sensor_GW_NJSY_Hum_Num)
	#error "南京盛亿网关探头配置不正确"
#else
	#undef	Sensor_GW_NJSY_Num
	#define	Sensor_GW_NJSY_Num	(Sensor_GW_NJSY_Tmp_Num)
#endif

#if	(Sensor_GW_HZZH_Tmp_Num != Sensor_GW_HZZH_Hum_Num)
	#error "杭州创辉网关探头配置不正确"
#else
	#undef	Sensor_GW_HZZH_Num
	#define	Sensor_GW_HZZH_Num	(Sensor_GW_HZZH_Tmp_Num)
#endif

#define Sensor_CH_Config_Num		(Sensor_Tmp_Num+\
										Sensor_CarbonDioxide_Num+\
										Sensor_DoorContact_Num+\
									 Sensor_Hum_Num+\
									 Sensor_RF_Single_Tmp_Num+\
									 Sensor_RF_Humiture_Tmp_Num+\
									 Sensor_RF_Humiture_Hum_Num+\
									 Sensor_Lon_Num+\
									 Sensor_Lat_Num+\
									 Sensor_Spd_Num+\
									 Sensor_PT1000_Num+\
									 Sensor_GW_NJSY_Tmp_Num+\
									 Sensor_GW_NJSY_Hum_Num+\
									 Sensor_GW_HZZH_Tmp_Num+\
									 Sensor_GW_HZZH_Hum_Num)

/* 探头数量 */
#define SystemProbeNum				((((Sensor_Tmp_Num+\
										Sensor_CarbonDioxide_Num+\
										Sensor_DoorContact_Num+\
									 Sensor_Hum_Num+\
									 Sensor_RF_Single_Tmp_Num+\
									 Sensor_PT1000_Num)>0)?1:0)+\
									 Sensor_RF_Humiture_Num+\
									 Sensor_GW_NJSY_Num+\
									 Sensor_GW_HZZH_Num)
									 
/* 集成在网关上的探头数量 */
#define IntProbeNum					(Sensor_Tmp_Num+\
										Sensor_CarbonDioxide_Num+\
									 Sensor_Hum_Num+\
									 Sensor_PT1000_Num)

/* 探头数量 */
#define WirelessProbeNum			(Sensor_RF_Single_Tmp_Num+\
									 Sensor_RF_Humiture_Num+\
									 Sensor_GW_NJSY_Num+\
									 Sensor_GW_HZZH_Num)


#endif


void Config_TP_CH_Low_Limit	(u16 low_limit);
void Config_HD_CH_Up_Limit	(u16 up_limit);
void Config_HD_CH_Low_Limit	(u16 low_limit);


void DispSensorNumConfig	(u8 sensor_num);

void Temp_Sensor_Failure	(void);
void Humi_Sensor_Failure	(void);

void Sensor_Updata_Data				(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
void Sensor_State_Check_ALL			(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab);
u8 	 Sensor_Alarm_Check				(INSTRU_SENSOR_CHANNEL* sensor_t,INSTRU_SENSOR_CHANNEL_FIX* sensor_f);

void Log_Sensor_Pgm_Ude_CH_Cfg(void);


#endif  
	 

