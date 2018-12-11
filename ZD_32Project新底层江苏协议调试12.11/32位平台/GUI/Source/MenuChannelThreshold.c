/************************************************************************************************/
/** @file              : MenuChaTsd.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年6月9日															*/
/** @date changed      : 2015年9月16日(文件名从MenuSensor改成MenuChaTsd)							*/
/** @brief             : c file																	*/
/** @description       : GUI菜单->通道阈值界面												*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Ks0108.H"
#include "Menu.H"

#include "System_Config.h"
#include "Log_Sensor.h"


/* 通道阈值 中文列表 */
u8* Cha_Tsd_CN_Tab[] =
{
#if	(Sensor_CH1_Config_Type == Sensor_Tmp)
{"通道-1(温度)"},
#elif (Sensor_CH1_Config_Type == Sensor_CarbonDioxide)
{"通道-1(二氧化碳)"},
#elif (Sensor_CH1_Config_Type == Sensor_Hum)
{"通道-1(湿度)"},
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-1(卡片温)"},
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-1(卡片温)"},	
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-1(卡片湿)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon)
{"通道-1(经度)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)
{"通道-1(纬度)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)
{"通道-1(速度)"},
#elif (Sensor_CH1_Config_Type == Sensor_PT1000)
{"通道-1(PT1000)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-1(GWSY-温)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-1(GWSY-湿)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-1(GWCH-温)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-1(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH2_Config_Type == Sensor_Tmp)
{"通道-2(温度)"},
#elif (Sensor_CH2_Config_Type == Sensor_CarbonDioxide)
{"通道-2(二氧化碳)"},
#elif (Sensor_CH2_Config_Type == Sensor_Hum)
{"通道-2(湿度)"},
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-2(卡片温)"},
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-2(卡片温)"},	
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-2(卡片湿)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon)
{"通道-2(经度)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat)
{"通道-2(纬度)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd)
{"通道-2(速度)"},
#elif (Sensor_CH2_Config_Type == Sensor_PT1000)
{"通道-2(PT1000)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-2(GWSY-温)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-2(GWSY-湿)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-2(GWCH-温)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-2(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH3_Config_Type == Sensor_Tmp)
{"通道-3(温度)"},
#elif (Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
{"通道-3(二氧化碳)"},
#elif (Sensor_CH3_Config_Type == Sensor_Hum)
{"通道-3(湿度)"},
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-3(卡片温)"},
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-3(卡片温)"},	
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-3(卡片湿)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon)
{"通道-3(经度)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat)
{"通道-3(纬度)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd)
{"通道-3(速度)"},
#elif (Sensor_CH3_Config_Type == Sensor_PT1000)
{"通道-3(PT1000)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-3(GWSY-温)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-3(GWSY-湿)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-3(GWCH-温)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-3(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH4_Config_Type == Sensor_Tmp)
{"通道-4(温度)"},
#elif (Sensor_CH4_Config_Type == Sensor_CarbonDioxide)
{"通道-4(二氧化碳)"},
#elif (Sensor_CH4_Config_Type == Sensor_Hum)
{"通道-4(湿度)"},
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-4(卡片温)"},
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-4(卡片温)"},	
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-4(卡片湿)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon)
{"通道-4(经度)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat)
{"通道-4(纬度)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd)
{"通道-4(速度)"},
#elif (Sensor_CH4_Config_Type == Sensor_PT1000)
{"通道-4(PT1000)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-4(GWSY-温)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-4(GWSY-湿)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-4(GWCH-温)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-4(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH5_Config_Type == Sensor_Tmp)
{"通道-5(温度)"},
#elif (Sensor_CH5_Config_Type == Sensor_CarbonDioxide)
{"通道-5(二氧化碳)"},
#elif (Sensor_CH5_Config_Type == Sensor_Hum)
{"通道-5(湿度)"},
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-5(卡片温)"},
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-5(卡片温)"},	
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-5(卡片湿)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon)
{"通道-5(经度)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat)
{"通道-5(纬度)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd)
{"通道-5(速度)"},
#elif (Sensor_CH5_Config_Type == Sensor_PT1000)
{"通道-5(PT1000)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-5(GWSY-温)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-5(GWSY-湿)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-5(GWCH-温)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-5(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH6_Config_Type == Sensor_Tmp)
{"通道-6(温度)"},
#elif (Sensor_CH6_Config_Type == Sensor_CarbonDioxide)
{"通道-6(二氧化碳)"},
#elif (Sensor_CH6_Config_Type == Sensor_Hum)
{"通道-6(湿度)"},
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-6(卡片温)"},
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-6(卡片温)"},	
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-6(卡片湿)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon)
{"通道-6(经度)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat)
{"通道-6(纬度)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd)
{"通道-6(速度)"},
#elif (Sensor_CH6_Config_Type == Sensor_PT1000)
{"通道-6(PT1000)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-6(GWSY-温)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-6(GWSY-湿)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-6(GWCH-温)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-6(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH7_Config_Type == Sensor_Tmp)
{"通道-7(温度)"},
#elif (Sensor_CH7_Config_Type == Sensor_CarbonDioxide)
{"通道-7(二氧化碳)"},
#elif (Sensor_CH7_Config_Type == Sensor_DoorContact)
{"通道-7(门磁开关)"},
#elif (Sensor_CH7_Config_Type == Sensor_Hum)
{"通道-7(湿度)"},
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-7(卡片温)"},
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-7(卡片温)"},	
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-7(卡片湿)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon)
{"通道-7(经度)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat)
{"通道-7(纬度)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd)
{"通道-7(速度)"},
#elif (Sensor_CH7_Config_Type == Sensor_PT1000)
{"通道-7(PT1000)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-7(GWSY-温)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-7(GWSY-湿)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-7(GWCH-温)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-7(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH8_Config_Type == Sensor_Tmp)
{"通道-8(温度)"},
#elif (Sensor_CH8_Config_Type == Sensor_CarbonDioxide)
{"通道-8(二氧化碳)"},
#elif (Sensor_CH8_Config_Type == Sensor_Hum)
{"通道-8(湿度)"},
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-8(卡片温)"},
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-8(卡片温)"},	
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-8(卡片湿)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon)
{"通道-8(经度)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat)
{"通道-8(纬度)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd)
{"通道-8(速度)"},
#elif (Sensor_CH8_Config_Type == Sensor_PT1000)
{"通道-8(PT1000)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-8(GWSY-温度)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-8(GWSY-湿度)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-8(GWCH-温)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-8(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH9_Config_Type == Sensor_Tmp)
{"通道-9(温度)"},
#elif (Sensor_CH9_Config_Type == Sensor_CarbonDioxide)
{"通道-9(二氧化碳)"},
#elif (Sensor_CH9_Config_Type == Sensor_Hum)
{"通道-9(湿度)"},
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-9(卡片温)"},
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-9(卡片温)"},	
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-9(卡片湿)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon)
{"通道-9(经度)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat)
{"通道-9(纬度)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd)
{"通道-9(速度)"},
#elif (Sensor_CH9_Config_Type == Sensor_PT1000)
{"通道-9(PT1000)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-9(GWSY-温)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-9(GWSY-湿)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-9(GWCH-温)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-9(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH10_Config_Type == Sensor_Tmp)
{"通道-10(温度)"},
#elif (Sensor_CH10_Config_Type == Sensor_CarbonDioxide)
{"通道-10(二氧化碳)"},
#elif (Sensor_CH10_Config_Type == Sensor_Hum)
{"通道-10(湿度)"},
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-10(卡片温)"},
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-10(卡片温)"},	
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-10(卡片湿)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon)
{"通道-10(经度)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat)
{"通道-10(纬度)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd)
{"通道-10(速度)"},
#elif (Sensor_CH10_Config_Type == Sensor_PT1000)
{"通道-10(PT1000)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-10(GWSY-温)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-10(GWSY-湿)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-10(GWCH-温)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-10(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH11_Config_Type == Sensor_Tmp)
{"通道-11(温度)"},
#elif (Sensor_CH11_Config_Type == Sensor_CarbonDioxide)
{"通道-11(二氧化碳)"},
#elif (Sensor_CH11_Config_Type == Sensor_Hum)
{"通道-11(湿度)"},
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-11(卡片温)"},
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-11(卡片温)"},	
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-11(卡片湿)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon)
{"通道-11(经度)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat)
{"通道-11(纬度)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd)
{"通道-11(速度)"},
#elif (Sensor_CH11_Config_Type == Sensor_PT1000)
{"通道-11(PT1000)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-11(GWSY-温)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-11(GWSY-湿)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-11(GWCH-温)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-11(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH12_Config_Type == Sensor_Tmp)
{"通道-12(温度)"},
#elif (Sensor_CH12_Config_Type == Sensor_CarbonDioxide)
{"通道-12(二氧化碳)"},
#elif (Sensor_CH12_Config_Type == Sensor_Hum)
{"通道-12(湿度)"},
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-12(卡片温)"},
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-12(卡片温)"},	
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-12(卡片湿)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon)
{"通道-12(经度)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat)
{"通道-12(纬度)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd)
{"通道-12(速度)"},
#elif (Sensor_CH12_Config_Type == Sensor_PT1000)
{"通道-12(PT1000)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-12(GWSY-温)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-12(GWSY-湿)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-12(GWCH-温)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-12(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH13_Config_Type == Sensor_Tmp)
{"通道-13(温度)"},
#elif (Sensor_CH13_Config_Type == Sensor_CarbonDioxide)
{"通道-13(二氧化碳)"},
#elif (Sensor_CH13_Config_Type == Sensor_Hum)
{"通道-13(湿度)"},
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-13(卡片温)"},
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-13(卡片温)"},	
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-13(卡片湿)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon)
{"通道-13(经度)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat)
{"通道-13(纬度)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd)
{"通道-13(速度)"},
#elif (Sensor_CH13_Config_Type == Sensor_PT1000)
{"通道-13(PT1000)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-13(GWSY-温)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-13(GWSY-湿)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-13(GWCH-温)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-13(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH14_Config_Type == Sensor_Tmp)
{"通道-14(温度)"},
#elif (Sensor_CH14_Config_Type == Sensor_CarbonDioxide)
{"通道-14(二氧化碳)"},
#elif (Sensor_CH14_Config_Type == Sensor_Hum)
{"通道-14(湿度)"},
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-14(卡片温)"},
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-14(卡片温)"},	
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-14(卡片湿)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon)
{"通道-14(经度)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat)
{"通道-14(纬度)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd)
{"通道-14(速度)"},
#elif (Sensor_CH14_Config_Type == Sensor_PT1000)
{"通道-14(PT1000)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-14(GWSY-温)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-14(GWSY-湿)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-14(GWCH-温)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-14(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH15_Config_Type == Sensor_Tmp)
{"通道-15(温度)"},
#elif (Sensor_CH15_Config_Type == Sensor_CarbonDioxide)
{"通道-15(二氧化碳)"},
#elif (Sensor_CH15_Config_Type == Sensor_Hum)
{"通道-15(湿度)"},
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-15(卡片温)"},
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-15(卡片温)"},	
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-15(卡片湿)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon)
{"通道-15(经度)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat)
{"通道-15(纬度)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd)
{"通道-15(速度)"},
#elif (Sensor_CH15_Config_Type == Sensor_PT1000)
{"通道-15(PT1000)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-15(GWSY-温)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-15(GWSY-湿)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-15(GWCH-温)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-15(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH16_Config_Type == Sensor_Tmp)
{"通道-16(温度)"},
#elif (Sensor_CH16_Config_Type == Sensor_CarbonDioxide)
{"通道-16(二氧化碳)"},
#elif (Sensor_CH16_Config_Type == Sensor_Hum)
{"通道-16(湿度)"},
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-16(卡片温)"},
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-16(卡片温)"},	
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-16(卡片湿)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon)
{"通道-16(经度)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat)
{"通道-16(纬度)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd)
{"通道-16(速度)"},
#elif (Sensor_CH16_Config_Type == Sensor_PT1000)
{"通道-16(PT1000)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-16(GWSY-温)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-16(GWSY-湿)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-16(GWCH-温)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-16(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH17_Config_Type == Sensor_Tmp)
{"通道-17(温度)"},
#elif (Sensor_CH17_Config_Type == Sensor_CarbonDioxide)
{"通道-17(二氧化碳)"},
#elif (Sensor_CH17_Config_Type == Sensor_Hum)
{"通道-17(湿度)"},
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-17(卡片温)"},
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-17(卡片温)"},	
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-17(卡片湿)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon)
{"通道-17(经度)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat)
{"通道-17(纬度)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd)
{"通道-17(速度)"},
#elif (Sensor_CH17_Config_Type == Sensor_PT1000)
{"通道-17(PT1000)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-17(GWSY-温)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-17(GWSY-湿)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-17(GWCH-温)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-17(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH18_Config_Type == Sensor_Tmp)
{"通道-18(温度)"},
#elif (Sensor_CH18_Config_Type == Sensor_CarbonDioxide)
{"通道-18(二氧化碳)"},
#elif (Sensor_CH18_Config_Type == Sensor_Hum)
{"通道-18(湿度)"},
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-18(卡片温)"},
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-18(卡片温)"},	
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-18(卡片湿)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon)
{"通道-18(经度)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat)
{"通道-18(纬度)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd)
{"通道-18(速度)"},
#elif (Sensor_CH18_Config_Type == Sensor_PT1000)
{"通道-18(PT1000)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-18(GWSY-温)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-18(GWSY-湿)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-18(GWCH-温)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-18(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH19_Config_Type == Sensor_Tmp)
{"通道-19(温度)"},
#elif (Sensor_CH19_Config_Type == Sensor_CarbonDioxide)
{"通道-19(二氧化碳)"},
#elif (Sensor_CH19_Config_Type == Sensor_Hum)
{"通道-19(湿度)"},
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-19(卡片温)"},
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-19(卡片温)"},	
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-19(卡片湿)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon)
{"通道-19(经度)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat)
{"通道-19(纬度)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd)
{"通道-19(速度)"},
#elif (Sensor_CH19_Config_Type == Sensor_PT1000)
{"通道-19(PT1000)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-19(GWSY-温)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-19(GWSY-湿)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-19(GWCH-温)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-19(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH20_Config_Type == Sensor_Tmp)
{"通道-20(温度)"},
#elif (Sensor_CH20_Config_Type == Sensor_CarbonDioxide)
{"通道-20(二氧化碳)"},
#elif (Sensor_CH20_Config_Type == Sensor_Hum)
{"通道-20(湿度)"},
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-20(卡片温)"},
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-20(卡片温)"},	
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-20(卡片湿)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon)
{"通道-20(经度)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat)
{"通道-20(纬度)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd)
{"通道-20(速度)"},
#elif (Sensor_CH20_Config_Type == Sensor_PT1000)
{"通道-20(PT1000)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-20(GWSY-温)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-20(GWSY-湿)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-20(GWCH-温)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-20(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH21_Config_Type == Sensor_Tmp)
{"通道-21(温度)"},
#elif (Sensor_CH21_Config_Type == Sensor_CarbonDioxide)
{"通道-21(二氧化碳)"},
#elif (Sensor_CH21_Config_Type == Sensor_Hum)
{"通道-21(湿度)"},
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-21(卡片温)"},
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-21(卡片温)"},	
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-21(卡片湿)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon)
{"通道-21(经度)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat)
{"通道-21(纬度)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd)
{"通道-21(速度)"},
#elif (Sensor_CH21_Config_Type == Sensor_PT1000)
{"通道-21(PT1000)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-21(GWSY-温)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-21(GWSY-湿)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-21(GWCH-温)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-21(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH22_Config_Type == Sensor_Tmp)
{"通道-22(温度)"},
#elif (Sensor_CH22_Config_Type == Sensor_CarbonDioxide)
{"通道-22(二氧化碳)"},
#elif (Sensor_CH22_Config_Type == Sensor_Hum)
{"通道-22(湿度)"},
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-22(卡片温)"},
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-22(卡片温)"},	
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-22(卡片湿)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon)
{"通道-22(经度)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat)
{"通道-22(纬度)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd)
{"通道-22(速度)"},
#elif (Sensor_CH22_Config_Type == Sensor_PT1000)
{"通道-22(PT1000)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-22(GWSY-温)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-22(GWSY-湿)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-22(GWCH-温)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-22(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH23_Config_Type == Sensor_Tmp)
{"通道-23(温度)"},
#elif (Sensor_CH23_Config_Type == Sensor_CarbonDioxide)
{"通道-23(二氧化碳)"},
#elif (Sensor_CH23_Config_Type == Sensor_Hum)
{"通道-23(湿度)"},
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-23(卡片温)"},
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-23(卡片温)"},	
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-23(卡片湿)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon)
{"通道-23(经度)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat)
{"通道-23(纬度)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd)
{"通道-23(速度)"},
#elif (Sensor_CH23_Config_Type == Sensor_PT1000)
{"通道-23(PT1000)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-23(GWSY-温)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-23(GWSY-湿)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-23(GWCH-温)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-23(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH24_Config_Type == Sensor_Tmp)
{"通道-24(温度)"},
#elif (Sensor_CH24_Config_Type == Sensor_CarbonDioxide)
{"通道-24(二氧化碳)"},
#elif (Sensor_CH24_Config_Type == Sensor_Hum)
{"通道-24(湿度)"},
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-24(卡片温)"},
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-24(卡片温)"},	
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-24(卡片湿)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon)
{"通道-24(经度)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat)
{"通道-24(纬度)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd)
{"通道-24(速度)"},
#elif (Sensor_CH24_Config_Type == Sensor_PT1000)
{"通道-24(PT1000)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-24(GWSY-温)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-24(GWSY-湿)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-24(GWCH-温)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-24(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH25_Config_Type == Sensor_Tmp)
{"通道-25(温度)"},
#elif (Sensor_CH25_Config_Type == Sensor_CarbonDioxide)
{"通道-25(二氧化碳)"},
#elif (Sensor_CH25_Config_Type == Sensor_Hum)
{"通道-25(湿度)"},
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-25(卡片温)"},
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-25(卡片温)"},	
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-25(卡片湿)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon)
{"通道-25(经度)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat)
{"通道-25(纬度)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd)
{"通道-25(速度)"},
#elif (Sensor_CH25_Config_Type == Sensor_PT1000)
{"通道-25(PT1000)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-25(GWSY-温)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-25(GWSY-湿)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-25(GWCH-温)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-25(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH26_Config_Type == Sensor_Tmp)
{"通道-26(温度)"},
#elif (Sensor_CH26_Config_Type == Sensor_CarbonDioxide)
{"通道-26(二氧化碳)"},
#elif (Sensor_CH26_Config_Type == Sensor_Hum)
{"通道-26(湿度)"},
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-26(卡片温)"},
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-26(卡片温)"},	
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-26(卡片湿)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon)
{"通道-26(经度)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat)
{"通道-26(纬度)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd)
{"通道-26(速度)"},
#elif (Sensor_CH26_Config_Type == Sensor_PT1000)
{"通道-26(PT1000)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-26(GWSY-温)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-26(GWSY-湿)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-26(GWCH-温)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-26(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH27_Config_Type == Sensor_Tmp)
{"通道-27(温度)"},
#elif (Sensor_CH27_Config_Type == Sensor_CarbonDioxide)
{"通道-27(二氧化碳)"},
#elif (Sensor_CH27_Config_Type == Sensor_Hum)
{"通道-27(湿度)"},
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-27(卡片温)"},
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-27(卡片温)"},	
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-27(卡片湿)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon)
{"通道-27(经度)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat)
{"通道-27(纬度)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd)
{"通道-27(速度)"},
#elif (Sensor_CH27_Config_Type == Sensor_PT1000)
{"通道-27(PT1000)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-27(GWSY-温)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-27(GWSY-湿)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-27(GWCH-温)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-27(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH28_Config_Type == Sensor_Tmp)
{"通道-28(温度)"},
#elif (Sensor_CH28_Config_Type == Sensor_CarbonDioxide)
{"通道-28(二氧化碳)"},
#elif (Sensor_CH28_Config_Type == Sensor_Hum)
{"通道-28(湿度)"},
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-28(卡片温)"},
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-28(卡片温)"},	
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-28(卡片湿)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon)
{"通道-28(经度)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat)
{"通道-28(纬度)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd)
{"通道-28(速度)"},
#elif (Sensor_CH28_Config_Type == Sensor_PT1000)
{"通道-28(PT1000)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-28(GWSY-温)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-28(GWSY-湿)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-28(GWCH-温)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-28(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH29_Config_Type == Sensor_Tmp)
{"通道-29(温度)"},
#elif (Sensor_CH29_Config_Type == Sensor_CarbonDioxide)
{"通道-29(二氧化碳)"},
#elif (Sensor_CH29_Config_Type == Sensor_Hum)
{"通道-29(湿度)"},
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-29(卡片温)"},
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-29(卡片温)"},	
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-29(卡片湿)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon)
{"通道-29(经度)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat)
{"通道-29(纬度)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd)
{"通道-29(速度)"},
#elif (Sensor_CH29_Config_Type == Sensor_PT1000)
{"通道-29(PT1000)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-29(GWSY-温)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-29(GWSY-湿)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-29(GWCH-温)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-29(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH30_Config_Type == Sensor_Tmp)
{"通道-30(温度)"},
#elif (Sensor_CH30_Config_Type == Sensor_CarbonDioxide)
{"通道-30(二氧化碳)"},
#elif (Sensor_CH30_Config_Type == Sensor_Hum)
{"通道-30(湿度)"},
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-30(卡片温)"},
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-30(卡片温)"},	
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-30(卡片湿)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon)
{"通道-30(经度)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat)
{"通道-30(纬度)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd)
{"通道-30(速度)"},
#elif (Sensor_CH30_Config_Type == Sensor_PT1000)
{"通道-30(PT1000)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-30(GWSY-温)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-30(GWSY-湿)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-30(GWCH-温)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-30(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH31_Config_Type == Sensor_Tmp)
{"通道-31(温度)"},
#elif (Sensor_CH31_Config_Type == Sensor_CarbonDioxide)
{"通道-31(二氧化碳)"},
#elif (Sensor_CH31_Config_Type == Sensor_Hum)
{"通道-31(湿度)"},
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-31(卡片温)"},
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-31(卡片温)"},	
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-31(卡片湿)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon)
{"通道-31(经度)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat)
{"通道-31(纬度)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd)
{"通道-31(速度)"},
#elif (Sensor_CH31_Config_Type == Sensor_PT1000)
{"通道-31(PT1000)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-31(GWSY-温)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-31(GWSY-湿)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-31(GWCH-温)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-31(GWCH-湿)"},
#else
{"无"},
#endif

#if	(Sensor_CH32_Config_Type == Sensor_Tmp)
{"通道-32(温度)"},
#elif (Sensor_CH32_Config_Type == Sensor_CarbonDioxide)
{"通道-32(二氧化碳)"},
#elif (Sensor_CH32_Config_Type == Sensor_Hum)
{"通道-32(湿度)"},
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
{"通道-32(卡片温)"},
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"通道-32(卡片温)"},	
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"通道-32(卡片湿)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon)
{"通道-32(经度)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat)
{"通道-32(纬度)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd)
{"通道-32(速度)"},
#elif (Sensor_CH32_Config_Type == Sensor_PT1000)
{"通道-32(PT1000)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)
{"通道-32(GWSY-温)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
{"通道-32(GWSY-湿)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)
{"通道-32(GWCH-温)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum)
{"通道-32(GWCH-湿)"},
#else
{"无"},
#endif

};

/* 通道阈值 英文列表 */
u8* Cha_Tsd_EN_Tab[] =
{
#if	(Sensor_CH1_Config_Type == Sensor_Tmp)
{"CH-1(Tmp)"},
#elif (Sensor_CH1_Config_Type == Sensor_CarbonDioxide)
{"CH-1(CO2)"},
#elif (Sensor_CH1_Config_Type == Sensor_Hum)
{"CH-1(Hum)"},
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-1(C-Tmp)"},
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-1(C-Tmp)"},	
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-1(C-Hum)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon)
{"CH-1(Lon)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)
{"CH-1(Lat)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)
{"CH-1(Spd)"},
#elif (Sensor_CH1_Config_Type == Sensor_PT1000)
{"CH-1(PT1000)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-1(GWSY-Tmp)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-1(GWSY-Hum)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-1(GWCH-Tmp)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-1(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH2_Config_Type == Sensor_Tmp)
{"CH-2(Tmp)"},
#elif (Sensor_CH2_Config_Type == Sensor_CarbonDioxide)
{"CH-2(CO2)"},
#elif (Sensor_CH2_Config_Type == Sensor_Hum)
{"CH-2(Hum)"},
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-2(C-Tmp)"},
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-2(C-Tmp)"},	
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-2(C-Hum)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon)
{"CH-2(Lon)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat)
{"CH-2(Lat)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd)
{"CH-2(Spd)"},
#elif (Sensor_CH2_Config_Type == Sensor_PT1000)
{"CH-2(PT1000)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-2(GWSY-Tmp)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-2(GWSY-Hum)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-2(GWCH-Tmp)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-2(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH3_Config_Type == Sensor_Tmp)
{"CH-3(Tmp)"},
#elif (Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
{"CH-3(CO2)"},
#elif (Sensor_CH3_Config_Type == Sensor_Hum)
{"CH-3(Hum)"},
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-3(C-Tmp)"},
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-3(C-Tmp)"},	
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-3(C-Hum)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon)
{"CH-3(Lon)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat)
{"CH-3(Lat)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd)
{"CH-3(Spd)"},
#elif (Sensor_CH3_Config_Type == Sensor_PT1000)
{"CH-3(PT1000)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-3(GWSY-Tmp)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-3(GWSY-Hum)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-3(GWCH-Tmp)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-3(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH4_Config_Type == Sensor_Tmp)
{"CH-4(Tmp)"},
#elif (Sensor_CH4_Config_Type == Sensor_CarbonDioxide)
{"CH-4(CO2)"},
#elif (Sensor_CH4_Config_Type == Sensor_Hum)
{"CH-4(Hum)"},
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-4(C-Tmp)"},
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-4(C-Tmp)"},	
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-4(C-Hum)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon)
{"CH-4(Lon)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat)
{"CH-4(Lat)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd)
{"CH-4(Spd)"},
#elif (Sensor_CH4_Config_Type == Sensor_PT1000)
{"CH-4(PT1000)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-4(GWSY-Tmp)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-4(GWSY-Hum)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-4(GWCH-Tmp)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-4(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH5_Config_Type == Sensor_Tmp)
{"CH-5(Tmp)"},
#elif (Sensor_CH5_Config_Type == Sensor_CarbonDioxide)
{"CH-5(CO2)"},
#elif (Sensor_CH5_Config_Type == Sensor_Hum)
{"CH-5(Hum)"},
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-5(C-Tmp)"},
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-5(C-Tmp)"},	
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-5(C-Hum)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon)
{"CH-5(Lon)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat)
{"CH-5(Lat)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd)
{"CH-5(Spd)"},
#elif (Sensor_CH5_Config_Type == Sensor_PT1000)
{"CH-5(PT1000)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-5(GWSY-Tmp)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-5(GWSY-Hum)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-5(GWCH-Tmp)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-5(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH6_Config_Type == Sensor_Tmp)
{"CH-6(Tmp)"},
#elif (Sensor_CH6_Config_Type == Sensor_CarbonDioxide)
{"CH-6(CO2)"},
#elif (Sensor_CH6_Config_Type == Sensor_Hum)
{"CH-6(Hum)"},
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-6(C-Tmp)"},
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-6(C-Tmp)"},	
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-6(C-Hum)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon)
{"CH-6(Lon)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat)
{"CH-6(Lat)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd)
{"CH-6(Spd)"},
#elif (Sensor_CH6_Config_Type == Sensor_PT1000)
{"CH-6(PT1000)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-6(GWSY-Tmp)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-6(GWSY-Hum)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-6(GWCH-Tmp)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-6(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH7_Config_Type == Sensor_Tmp)
{"CH-7(Tmp)"},
#elif (Sensor_CH7_Config_Type == Sensor_CarbonDioxide)
{"CH-7(CO2)"},
#elif (Sensor_CH7_Config_Type == Sensor_DoorContact)
{"CH-7(CD)"},
#elif (Sensor_CH7_Config_Type == Sensor_Hum)
{"CH-7(Hum)"},
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-7(C-Tmp)"},
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-7(C-Tmp)"},	
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-7(C-Hum)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon)
{"CH-7(Lon)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat)
{"CH-7(Lat)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd)
{"CH-7(Spd)"},
#elif (Sensor_CH7_Config_Type == Sensor_PT1000)
{"CH-7(PT1000)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-7(GWSY-Tmp)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-7(GWSY-Hum)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-7(GWCH-Tmp)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-7(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH8_Config_Type == Sensor_Tmp)
{"CH-8(Tmp)"},
#elif (Sensor_CH8_Config_Type == Sensor_CarbonDioxide)
{"CH-8(CO2)"},
#elif (Sensor_CH8_Config_Type == Sensor_Hum)
{"CH-8(Hum)"},
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-8(C-Tmp)"},
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-8(C-Tmp)"},	
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-8(C-Hum)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon)
{"CH-8(Lon)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat)
{"CH-8(Lat)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd)
{"CH-8(Spd)"},
#elif (Sensor_CH8_Config_Type == Sensor_PT1000)
{"CH-8(PT1000)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-8(GWSY-Tmp)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-8(GWSY-Hum)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-8(GWCH-Tmp)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-8(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH9_Config_Type == Sensor_Tmp)
{"CH-9(Tmp)"},
#elif (Sensor_CH9_Config_Type == Sensor_CarbonDioxide)
{"CH-9(CO2)"},
#elif (Sensor_CH9_Config_Type == Sensor_Hum)
{"CH-9(Hum)"},
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-9(C-Tmp)"},
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-9(C-Tmp)"},	
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-9(C-Hum)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon)
{"CH-9(Lon)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat)
{"CH-9(Lat)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd)
{"CH-9(Spd)"},
#elif (Sensor_CH9_Config_Type == Sensor_PT1000)
{"CH-9(PT1000)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-9(GWSY-Tmp)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-9(GWSY-Hum)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-9(GWCH-Tmp)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-9(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH10_Config_Type == Sensor_Tmp)
{"CH-10(Tmp)"},
#elif (Sensor_CH10_Config_Type == Sensor_CarbonDioxide)
{"CH-10(CO2)"},
#elif (Sensor_CH10_Config_Type == Sensor_Hum)
{"CH-10(Hum)"},
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-10(C-Tmp)"},
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-10(C-Tmp)"},	
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-10(C-Hum)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon)
{"CH-10(Lon)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat)
{"CH-10(Lat)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd)
{"CH-10(Spd)"},
#elif (Sensor_CH10_Config_Type == Sensor_PT1000)
{"CH-10(PT1000)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-10(GWSY-Tmp)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-10(GWSY-Hum)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-10(GWCH-Tmp)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-10(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH11_Config_Type == Sensor_Tmp)
{"CH-11(Tmp)"},
#elif (Sensor_CH11_Config_Type == Sensor_CarbonDioxide)
{"CH-11(CO2)"},
#elif (Sensor_CH11_Config_Type == Sensor_Hum)
{"CH-11(Hum)"},
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-11(C-Tmp)"},
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-11(C-Tmp)"},	
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-11(C-Hum)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon)
{"CH-11(Lon)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat)
{"CH-11(Lat)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd)
{"CH-11(Spd)"},
#elif (Sensor_CH11_Config_Type == Sensor_PT1000)
{"CH-11(PT1000)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-11(GWSY-Tmp)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-11(GWSY-Hum)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-11(GWCH-Tmp)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-11(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH12_Config_Type == Sensor_Tmp)
{"CH-12(Tmp)"},
#elif (Sensor_CH12_Config_Type == Sensor_CarbonDioxide)
{"CH-12(CO2)"},
#elif (Sensor_CH12_Config_Type == Sensor_Hum)
{"CH-12(Hum)"},
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-12(C-Tmp)"},
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-12(C-Tmp)"},	
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-12(C-Hum)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon)
{"CH-12(Lon)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat)
{"CH-12(Lat)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd)
{"CH-12(Spd)"},
#elif (Sensor_CH12_Config_Type == Sensor_PT1000)
{"CH-12(PT1000)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-12(GWSY-Tmp)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-12(GWSY-Hum)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-12(GWCH-Tmp)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-12(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH13_Config_Type == Sensor_Tmp)
{"CH-13(Tmp)"},
#elif (Sensor_CH13_Config_Type == Sensor_CarbonDioxide)
{"CH-13(CO2)"},
#elif (Sensor_CH13_Config_Type == Sensor_Hum)
{"CH-13(Hum)"},
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-13(C-Tmp)"},
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-13(C-Tmp)"},	
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-13(C-Hum)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon)
{"CH-13(Lon)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat)
{"CH-13(Lat)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd)
{"CH-13(Spd)"},
#elif (Sensor_CH13_Config_Type == Sensor_PT1000)
{"CH-13(PT1000)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-13(GWSY-Tmp)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-13(GWSY-Hum)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-13(GWCH-Tmp)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-13(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH14_Config_Type == Sensor_Tmp)
{"CH-14(Tmp)"},
#elif (Sensor_CH14_Config_Type == Sensor_CarbonDioxide)
{"CH-14(CO2)"},
#elif (Sensor_CH14_Config_Type == Sensor_Hum)
{"CH-14(Hum)"},
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-14(C-Tmp)"},
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-14(C-Tmp)"},	
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-14(C-Hum)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon)
{"CH-14(Lon)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat)
{"CH-14(Lat)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd)
{"CH-14(Spd)"},
#elif (Sensor_CH14_Config_Type == Sensor_PT1000)
{"CH-14(PT1000)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-14(GWSY-Tmp)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-14(GWSY-Hum)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-14(GWCH-Tmp)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-14(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH15_Config_Type == Sensor_Tmp)
{"CH-15(Tmp)"},
#elif (Sensor_CH15_Config_Type == Sensor_CarbonDioxide)
{"CH-15(CO2)"},
#elif (Sensor_CH15_Config_Type == Sensor_Hum)
{"CH-15(Hum)"},
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-15(C-Tmp)"},
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-15(C-Tmp)"},	
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-15(C-Hum)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon)
{"CH-15(Lon)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat)
{"CH-15(Lat)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd)
{"CH-15(Spd)"},
#elif (Sensor_CH15_Config_Type == Sensor_PT1000)
{"CH-15(PT1000)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-15(GWSY-Tmp)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-15(GWSY-Hum)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-15(GWCH-Tmp)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-15(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH16_Config_Type == Sensor_Tmp)
{"CH-16(Tmp)"},
#elif (Sensor_CH16_Config_Type == Sensor_CarbonDioxide)
{"CH-16(CO2)"},
#elif (Sensor_CH16_Config_Type == Sensor_Hum)
{"CH-16(Hum)"},
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-16(C-Tmp)"},
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-16(C-Tmp)"},	
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-16(C-Hum)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon)
{"CH-16(Lon)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat)
{"CH-16(Lat)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd)
{"CH-16(Spd)"},
#elif (Sensor_CH16_Config_Type == Sensor_PT1000)
{"CH-16(PT1000)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-16(GWSY-Tmp)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-16(GWSY-Hum)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-16(GWCH-Tmp)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-16(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH17_Config_Type == Sensor_Tmp)
{"CH-17(Tmp)"},
#elif (Sensor_CH17_Config_Type == Sensor_CarbonDioxide)
{"CH-17(CO2)"},
#elif (Sensor_CH17_Config_Type == Sensor_Hum)
{"CH-17(Hum)"},
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-17(C-Tmp)"},
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-17(C-Tmp)"},	
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-17(C-Hum)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon)
{"CH-17(Lon)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat)
{"CH-17(Lat)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd)
{"CH-17(Spd)"},
#elif (Sensor_CH17_Config_Type == Sensor_PT1000)
{"CH-17(PT1000)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-17(GWSY-Tmp)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-17(GWSY-Hum)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-17(GWCH-Tmp)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-17(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH18_Config_Type == Sensor_Tmp)
{"CH-18(Tmp)"},
#elif (Sensor_CH18_Config_Type == Sensor_CarbonDioxide)
{"CH-18(CO2)"},
#elif (Sensor_CH18_Config_Type == Sensor_Hum)
{"CH-18(Hum)"},
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-18(C-Tmp)"},
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-18(C-Tmp)"},	
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-18(C-Hum)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon)
{"CH-18(Lon)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat)
{"CH-18(Lat)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd)
{"CH-18(Spd)"},
#elif (Sensor_CH18_Config_Type == Sensor_PT1000)
{"CH-18(PT1000)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-18(GWSY-Tmp)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-18(GWSY-Hum)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-18(GWCH-Tmp)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-18(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH19_Config_Type == Sensor_Tmp)
{"CH-19(Tmp)"},
#elif (Sensor_CH19_Config_Type == Sensor_CarbonDioxide)
{"CH-19(CO2)"},
#elif (Sensor_CH19_Config_Type == Sensor_Hum)
{"CH-19(Hum)"},
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-19(C-Tmp)"},
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-19(C-Tmp)"},	
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-19(C-Hum)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon)
{"CH-19(Lon)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat)
{"CH-19(Lat)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd)
{"CH-19(Spd)"},
#elif (Sensor_CH19_Config_Type == Sensor_PT1000)
{"CH-19(PT1000)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-19(GWSY-Tmp)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-19(GWSY-Hum)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-19(GWCH-Tmp)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-19(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH20_Config_Type == Sensor_Tmp)
{"CH-20(Tmp)"},
#elif (Sensor_CH20_Config_Type == Sensor_CarbonDioxide)
{"CH-20(CO2)"},
#elif (Sensor_CH20_Config_Type == Sensor_Hum)
{"CH-20(Hum)"},
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-20(C-Tmp)"},
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-20(C-Tmp)"},	
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-20(C-Hum)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon)
{"CH-20(Lon)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat)
{"CH-20(Lat)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd)
{"CH-20(Spd)"},
#elif (Sensor_CH20_Config_Type == Sensor_PT1000)
{"CH-20(PT1000)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-20(GWSY-Tmp)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-20(GWSY-Hum)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-20(GWCH-Tmp)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-20(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH21_Config_Type == Sensor_Tmp)
{"CH-21(Tmp)"},
#elif (Sensor_CH21_Config_Type == Sensor_CarbonDioxide)
{"CH-21(CO2)"},
#elif (Sensor_CH21_Config_Type == Sensor_Hum)
{"CH-21(Hum)"},
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-21(C-Tmp)"},
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-21(C-Tmp)"},	
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-21(C-Hum)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon)
{"CH-21(Lon)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat)
{"CH-21(Lat)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd)
{"CH-21(Spd)"},
#elif (Sensor_CH21_Config_Type == Sensor_PT1000)
{"CH-21(PT1000)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-21(GWSY-Tmp)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-21(GWSY-Hum)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-21(GWCH-Tmp)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-21(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH22_Config_Type == Sensor_Tmp)
{"CH-22(Tmp)"},
#elif (Sensor_CH22_Config_Type == Sensor_CarbonDioxide)
{"CH-22(CO2)"},
#elif (Sensor_CH22_Config_Type == Sensor_Hum)
{"CH-22(Hum)"},
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-22(C-Tmp)"},
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-22(C-Tmp)"},	
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-22(C-Hum)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon)
{"CH-22(Lon)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat)
{"CH-22(Lat)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd)
{"CH-22(Spd)"},
#elif (Sensor_CH22_Config_Type == Sensor_PT1000)
{"CH-22(PT1000)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-22(GWSY-Tmp)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-22(GWSY-Hum)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-22(GWCH-Tmp)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-22(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH23_Config_Type == Sensor_Tmp)
{"CH-23(Tmp)"},
#elif (Sensor_CH23_Config_Type == Sensor_CarbonDioxide)
{"CH-23(CO2)"},
#elif (Sensor_CH23_Config_Type == Sensor_Hum)
{"CH-23(Hum)"},
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-23(C-Tmp)"},
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-23(C-Tmp)"},	
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-23(C-Hum)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon)
{"CH-23(Lon)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat)
{"CH-23(Lat)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd)
{"CH-23(Spd)"},
#elif (Sensor_CH23_Config_Type == Sensor_PT1000)
{"CH-23(PT1000)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-23(GWSY-Tmp)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-23(GWSY-Hum)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-23(GWCH-Tmp)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-23(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH24_Config_Type == Sensor_Tmp)
{"CH-24(Tmp)"},
#elif (Sensor_CH24_Config_Type == Sensor_CarbonDioxide)
{"CH-24(CO2)"},
#elif (Sensor_CH24_Config_Type == Sensor_Hum)
{"CH-24(Hum)"},
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-24(C-Tmp)"},
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-24(C-Tmp)"},	
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-24(C-Hum)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon)
{"CH-24(Lon)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat)
{"CH-24(Lat)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd)
{"CH-24(Spd)"},
#elif (Sensor_CH24_Config_Type == Sensor_PT1000)
{"CH-24(PT1000)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-24(GWSY-Tmp)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-24(GWSY-Hum)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-24(GWCH-Tmp)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-24(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH25_Config_Type == Sensor_Tmp)
{"CH-25(Tmp)"},
#elif (Sensor_CH25_Config_Type == Sensor_CarbonDioxide)
{"CH-25(CO2)"},
#elif (Sensor_CH25_Config_Type == Sensor_Hum)
{"CH-25(Hum)"},
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-25(C-Tmp)"},
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-25(C-Tmp)"},	
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-25(C-Hum)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon)
{"CH-25(Lon)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat)
{"CH-25(Lat)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd)
{"CH-25(Spd)"},
#elif (Sensor_CH25_Config_Type == Sensor_PT1000)
{"CH-25(PT1000)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-25(GWSY-Tmp)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-25(GWSY-Hum)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-25(GWCH-Tmp)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-25(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH26_Config_Type == Sensor_Tmp)
{"CH-26(Tmp)"},
#elif (Sensor_CH26_Config_Type == Sensor_CarbonDioxide)
{"CH-26(CO2)"},
#elif (Sensor_CH26_Config_Type == Sensor_Hum)
{"CH-26(Hum)"},
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-26(C-Tmp)"},
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-26(C-Tmp)"},	
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-26(C-Hum)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon)
{"CH-26(Lon)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat)
{"CH-26(Lat)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd)
{"CH-26(Spd)"},
#elif (Sensor_CH26_Config_Type == Sensor_PT1000)
{"CH-26(PT1000)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-26(GWSY-Tmp)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-26(GWSY-Hum)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-26(GWCH-Tmp)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-26(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH27_Config_Type == Sensor_Tmp)
{"CH-27(Tmp)"},
#elif (Sensor_CH27_Config_Type == Sensor_CarbonDioxide)
{"CH-27(CO2)"},
#elif (Sensor_CH27_Config_Type == Sensor_Hum)
{"CH-27(Hum)"},
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-27(C-Tmp)"},
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-27(C-Tmp)"},	
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-27(C-Hum)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon)
{"CH-27(Lon)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat)
{"CH-27(Lat)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd)
{"CH-27(Spd)"},
#elif (Sensor_CH27_Config_Type == Sensor_PT1000)
{"CH-27(PT1000)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-27(GWSY-Tmp)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-27(GWSY-Hum)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-27(GWCH-Tmp)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-27(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH28_Config_Type == Sensor_Tmp)
{"CH-28(Tmp)"},
#elif (Sensor_CH28_Config_Type == Sensor_CarbonDioxide)
{"CH-28(CO2)"},
#elif (Sensor_CH28_Config_Type == Sensor_Hum)
{"CH-28(Hum)"},
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-28(C-Tmp)"},
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-28(C-Tmp)"},	
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-28(C-Hum)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon)
{"CH-28(Lon)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat)
{"CH-28(Lat)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd)
{"CH-28(Spd)"},
#elif (Sensor_CH28_Config_Type == Sensor_PT1000)
{"CH-28(PT1000)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-28(GWSY-Tmp)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-28(GWSY-Hum)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-28(GWCH-Tmp)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-28(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH29_Config_Type == Sensor_Tmp)
{"CH-29(Tmp)"},
#elif (Sensor_CH29_Config_Type == Sensor_CarbonDioxide)
{"CH-29(CO2)"},
#elif (Sensor_CH29_Config_Type == Sensor_Hum)
{"CH-29(Hum)"},
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-29(C-Tmp)"},
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-29(C-Tmp)"},	
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-29(C-Hum)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon)
{"CH-29(Lon)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat)
{"CH-29(Lat)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd)
{"CH-29(Spd)"},
#elif (Sensor_CH29_Config_Type == Sensor_PT1000)
{"CH-29(PT1000)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-29(GWSY-Tmp)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-29(GWSY-Hum)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-29(GWCH-Tmp)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-29(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH30_Config_Type == Sensor_Tmp)
{"CH-30(Tmp)"},
#elif (Sensor_CH1_Config_Type == Sensor_CarbonDioxide)
{"CH-30(CO2)"},
#elif (Sensor_CH30_Config_Type == Sensor_Hum)
{"CH-30(Hum)"},
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-30(C-Tmp)"},
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-30(C-Tmp)"},	
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-30(C-Hum)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon)
{"CH-30(Lon)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat)
{"CH-30(Lat)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd)
{"CH-30(Spd)"},
#elif (Sensor_CH30_Config_Type == Sensor_PT1000)
{"CH-30(PT1000)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-30(GWSY-Tmp)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-30(GWSY-Hum)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-30(GWCH-Tmp)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-30(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH31_Config_Type == Sensor_Tmp)
{"CH-31(Tmp)"},
#elif (Sensor_CH1_Config_Type == Sensor_CarbonDioxide)
{"CH-31(CO2)"},
#elif (Sensor_CH31_Config_Type == Sensor_Hum)
{"CH-31(Hum)"},
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-31(C-Tmp)"},
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-31(C-Tmp)"},	
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-31(C-Hum)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon)
{"CH-31(Lon)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat)
{"CH-31(Lat)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd)
{"CH-31(Spd)"},
#elif (Sensor_CH31_Config_Type == Sensor_PT1000)
{"CH-31(PT1000)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-31(GWSY-Tmp)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-31(GWSY-Hum)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-31(GWCH-Tmp)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-31(GWCH-Hum)"},
#else
{"NULL"},
#endif

#if	(Sensor_CH32_Config_Type == Sensor_Tmp)
{"CH-32(Tmp)"},
#elif (Sensor_CH1_Config_Type == Sensor_CarbonDioxide)
{"CH-32(CO2)"},
#elif (Sensor_CH32_Config_Type == Sensor_Hum)
{"CH-32(Hum)"},
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
{"CH-32(C-Tmp)"},
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"CH-32(C-Tmp)"},	
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"CH-32(C-Hum)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon)
{"CH-32(Lon)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat)
{"CH-32(Lat)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd)
{"CH-32(Spd)"},
#elif (Sensor_CH32_Config_Type == Sensor_PT1000)
{"CH-32(PT1000)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)
{"CH-32(GWSY-Tmp)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
{"CH-32(GWSY-Hum)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)
{"CH-32(GWCH-Tmp)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum)
{"CH-32(GWCH-Hum)"},
#else
{"NULL"},
#endif
	
};

/* 通道阈值界面（无传感器时） 中文 菜单栏 */
u8* const Cha_NULL_CN_Tab = "无";
/* 通道阈值界面（无传感器时） 英文 菜单栏 */
u8* const Cha_NULL_EN_Tab = "NULL";

/* 通道阈值界面 中文 菜单项目栏 */
u8 * Menu_Cha_Tsd_CN[Sensor_CH_Config_Num] = {0};
/* 通道阈值界面 英文 菜单项目栏 */
u8 * Menu_Cha_Tsd_EN[Sensor_CH_Config_Num] = {0};

/* 通道阈值界面 中英文 菜单栏 */
const struct Menu Menu_Cha_Tsd[] =
{
	MenuWhat(Menu_Cha_Tsd_CN),
	MenuWhat(Menu_Cha_Tsd_EN),
};

/* 选择的通道类型指针 */
#define Menu_Cha_Type_P(Cha) 	(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->Type)


/************************************************************************************************/
/* 函数名	: Menu_Cha_Type_Sec															*/
/* 输入		: 传感器通道类型																		*/
/* 输出		: 无 																				*/
/* 作用		: 根据传感器类型选择进入对应的类型界面。													*/
/* 创建日期	: 2015/09/16																			*/
/************************************************************************************************/
static void Menu_Cha_Type_Sec(u8 cha_type)
{

	/* 判断传感器通道类型 *///现在在菜单中传感器功能主要分成两类：1、温度类型；2、湿度类型。
	switch(cha_type)
	{
		case Sensor_Tmp:
		{
			/* 进入 温度类型 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
    }
		case Sensor_CarbonDioxide:
		{
			/* 进入 温度类型 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
    }
		case Sensor_Hum:
		{
			/* 进入 湿度类型 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
			break;
        }
		
		case Sensor_RF_Card_Single_Tmp:
		{
			/* 进入 温度类型 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
		case Sensor_RF_Card_Humiture_Tmp:
		{
			/* 进入 温度类型 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
		case Sensor_RF_Card_Humiture_Hum:
		{
			/* 进入 湿度类型 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);	
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
		
		/* 经度 类型 */
		case Sensor_GPS_POS_Lon:
		{
			/* 经度类型 没有上下限设置界面. */
			Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
            break;
        }
		
		/* 纬度 类型 */
		case Sensor_GPS_POS_Lat:
		{
			/* 纬度类型 没有上下限设置界面. */
			Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
            break;
        }
		
		/* 速度 类型 */
		case Sensor_GPS_POS_Spd:
		{
			/* 速度类型 没有上下限设置界面. */
			Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
            break;
        }
		
		/* 	PT1000 类型 */
		case Sensor_PT1000:
		{
			/* 进入 温度类型 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
		case Sensor_GW_NJSY_Tmp:
		{
			/* 进入 上下限选择 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
		case Sensor_GW_NJSY_Hum:
		{
			/* 进入 上下限选择 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
		case Sensor_GW_HZZH_Tmp:
		{
			/* 进入 上下限选择 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
		case Sensor_GW_HZZH_Hum:
		{
			/* 进入 上下限选择 界面 */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* 进入到下一级界面 */
			PUSH();
            break;
        }
		
		
        default:
		{
			/* 未知类型，不进入界面。 */
			Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
            break;
        }
	}

}

/************************************************************************************************/
/* 函数名	: DispMenuChaTsdInit																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 显示通道阈值信息界面																	*/
/* 创建日期	: 2015/9/16																			*/
/************************************************************************************************/
void DispMenuChaTsdInit(void)
{
	
	
	/* 手动设置参数 */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{	
		/* 清除屏幕显示 */
		GUI_Clear();
		/* 设置界面显示内容和语言 */
		BarMenu = (struct Menu *)&Menu_Cha_Tsd[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		if(Flash != FlashMode_ReFlash)
		{
			/* 设置项目光标 */
			Item = 0;
			Where = 0;			
		}
		/* 显示当前界面下的项目和工具条 */
		BarMenuInit();
		/* 显示当前界面的名称 */
		GUI_Disp_Headline(*(Menu_Alarm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));
	}
	
}

/************************************************************************************************/
/* 函数名	: DispMenuChaTsdUp																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 进入 "选择" 通道界面																*/
/* 创建日期	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdUp(void)
{
	/* 根据传感器类型选择界面 */
	Menu_Cha_Type_Sec(Menu_Cha_Type_P(Item));
}

/************************************************************************************************/
/* 函数名	: DispMenuChaTsdDown																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 退出 "选择" 通道界面															*/
/* 创建日期	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdDown(void)
{
	/* 退回到上一级界面 */
	POP();
}

/************************************************************************************************/
/* 函数名	: DispMenuChaTsdLeft																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向上选择通道项目																*/
/* 创建日期	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdLeft(void)
{
    BarMenuLeft();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuChaTsdRight																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 向下选择通道项目																*/
/* 创建日期	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdRight(void)
{
    BarMenuRight();
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuChaTsdOK																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按确定按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdOK(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* 函数名	: DispMenuChaTsdCancel																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 长按取消按键（暂时没有使用）															*/
/* 创建日期	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdCancel(void)
{
	/* 将函数指针指向对应界面的初始化函数 */
	Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
} 

/******************* (C) ZHOU *****END OF FILE****/


