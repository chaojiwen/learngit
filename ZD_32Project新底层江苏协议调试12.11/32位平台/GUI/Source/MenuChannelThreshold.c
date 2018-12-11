/************************************************************************************************/
/** @file              : MenuChaTsd.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��6��9��															*/
/** @date changed      : 2015��9��16��(�ļ�����MenuSensor�ĳ�MenuChaTsd)							*/
/** @brief             : c file																	*/
/** @description       : GUI�˵�->ͨ����ֵ����												*/
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


/* ͨ����ֵ �����б� */
u8* Cha_Tsd_CN_Tab[] =
{
#if	(Sensor_CH1_Config_Type == Sensor_Tmp)
{"ͨ��-1(�¶�)"},
#elif (Sensor_CH1_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-1(������̼)"},
#elif (Sensor_CH1_Config_Type == Sensor_Hum)
{"ͨ��-1(ʪ��)"},
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-1(��Ƭ��)"},
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-1(��Ƭ��)"},	
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-1(��Ƭʪ)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-1(����)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-1(γ��)"},
#elif (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-1(�ٶ�)"},
#elif (Sensor_CH1_Config_Type == Sensor_PT1000)
{"ͨ��-1(PT1000)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-1(GWSY-��)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-1(GWSY-ʪ)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-1(GWCH-��)"},
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-1(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH2_Config_Type == Sensor_Tmp)
{"ͨ��-2(�¶�)"},
#elif (Sensor_CH2_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-2(������̼)"},
#elif (Sensor_CH2_Config_Type == Sensor_Hum)
{"ͨ��-2(ʪ��)"},
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-2(��Ƭ��)"},
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-2(��Ƭ��)"},	
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-2(��Ƭʪ)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-2(����)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-2(γ��)"},
#elif (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-2(�ٶ�)"},
#elif (Sensor_CH2_Config_Type == Sensor_PT1000)
{"ͨ��-2(PT1000)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-2(GWSY-��)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-2(GWSY-ʪ)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-2(GWCH-��)"},
#elif (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-2(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH3_Config_Type == Sensor_Tmp)
{"ͨ��-3(�¶�)"},
#elif (Sensor_CH3_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-3(������̼)"},
#elif (Sensor_CH3_Config_Type == Sensor_Hum)
{"ͨ��-3(ʪ��)"},
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-3(��Ƭ��)"},
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-3(��Ƭ��)"},	
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-3(��Ƭʪ)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-3(����)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-3(γ��)"},
#elif (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-3(�ٶ�)"},
#elif (Sensor_CH3_Config_Type == Sensor_PT1000)
{"ͨ��-3(PT1000)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-3(GWSY-��)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-3(GWSY-ʪ)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-3(GWCH-��)"},
#elif (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-3(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH4_Config_Type == Sensor_Tmp)
{"ͨ��-4(�¶�)"},
#elif (Sensor_CH4_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-4(������̼)"},
#elif (Sensor_CH4_Config_Type == Sensor_Hum)
{"ͨ��-4(ʪ��)"},
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-4(��Ƭ��)"},
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-4(��Ƭ��)"},	
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-4(��Ƭʪ)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-4(����)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-4(γ��)"},
#elif (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-4(�ٶ�)"},
#elif (Sensor_CH4_Config_Type == Sensor_PT1000)
{"ͨ��-4(PT1000)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-4(GWSY-��)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-4(GWSY-ʪ)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-4(GWCH-��)"},
#elif (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-4(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH5_Config_Type == Sensor_Tmp)
{"ͨ��-5(�¶�)"},
#elif (Sensor_CH5_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-5(������̼)"},
#elif (Sensor_CH5_Config_Type == Sensor_Hum)
{"ͨ��-5(ʪ��)"},
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-5(��Ƭ��)"},
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-5(��Ƭ��)"},	
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-5(��Ƭʪ)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-5(����)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-5(γ��)"},
#elif (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-5(�ٶ�)"},
#elif (Sensor_CH5_Config_Type == Sensor_PT1000)
{"ͨ��-5(PT1000)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-5(GWSY-��)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-5(GWSY-ʪ)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-5(GWCH-��)"},
#elif (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-5(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH6_Config_Type == Sensor_Tmp)
{"ͨ��-6(�¶�)"},
#elif (Sensor_CH6_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-6(������̼)"},
#elif (Sensor_CH6_Config_Type == Sensor_Hum)
{"ͨ��-6(ʪ��)"},
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-6(��Ƭ��)"},
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-6(��Ƭ��)"},	
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-6(��Ƭʪ)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-6(����)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-6(γ��)"},
#elif (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-6(�ٶ�)"},
#elif (Sensor_CH6_Config_Type == Sensor_PT1000)
{"ͨ��-6(PT1000)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-6(GWSY-��)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-6(GWSY-ʪ)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-6(GWCH-��)"},
#elif (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-6(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH7_Config_Type == Sensor_Tmp)
{"ͨ��-7(�¶�)"},
#elif (Sensor_CH7_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-7(������̼)"},
#elif (Sensor_CH7_Config_Type == Sensor_DoorContact)
{"ͨ��-7(�Ŵſ���)"},
#elif (Sensor_CH7_Config_Type == Sensor_Hum)
{"ͨ��-7(ʪ��)"},
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-7(��Ƭ��)"},
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-7(��Ƭ��)"},	
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-7(��Ƭʪ)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-7(����)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-7(γ��)"},
#elif (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-7(�ٶ�)"},
#elif (Sensor_CH7_Config_Type == Sensor_PT1000)
{"ͨ��-7(PT1000)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-7(GWSY-��)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-7(GWSY-ʪ)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-7(GWCH-��)"},
#elif (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-7(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH8_Config_Type == Sensor_Tmp)
{"ͨ��-8(�¶�)"},
#elif (Sensor_CH8_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-8(������̼)"},
#elif (Sensor_CH8_Config_Type == Sensor_Hum)
{"ͨ��-8(ʪ��)"},
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-8(��Ƭ��)"},
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-8(��Ƭ��)"},	
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-8(��Ƭʪ)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-8(����)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-8(γ��)"},
#elif (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-8(�ٶ�)"},
#elif (Sensor_CH8_Config_Type == Sensor_PT1000)
{"ͨ��-8(PT1000)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-8(GWSY-�¶�)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-8(GWSY-ʪ��)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-8(GWCH-��)"},
#elif (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-8(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH9_Config_Type == Sensor_Tmp)
{"ͨ��-9(�¶�)"},
#elif (Sensor_CH9_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-9(������̼)"},
#elif (Sensor_CH9_Config_Type == Sensor_Hum)
{"ͨ��-9(ʪ��)"},
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-9(��Ƭ��)"},
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-9(��Ƭ��)"},	
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-9(��Ƭʪ)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-9(����)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-9(γ��)"},
#elif (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-9(�ٶ�)"},
#elif (Sensor_CH9_Config_Type == Sensor_PT1000)
{"ͨ��-9(PT1000)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-9(GWSY-��)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-9(GWSY-ʪ)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-9(GWCH-��)"},
#elif (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-9(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH10_Config_Type == Sensor_Tmp)
{"ͨ��-10(�¶�)"},
#elif (Sensor_CH10_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-10(������̼)"},
#elif (Sensor_CH10_Config_Type == Sensor_Hum)
{"ͨ��-10(ʪ��)"},
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-10(��Ƭ��)"},
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-10(��Ƭ��)"},	
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-10(��Ƭʪ)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-10(����)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-10(γ��)"},
#elif (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-10(�ٶ�)"},
#elif (Sensor_CH10_Config_Type == Sensor_PT1000)
{"ͨ��-10(PT1000)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-10(GWSY-��)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-10(GWSY-ʪ)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-10(GWCH-��)"},
#elif (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-10(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH11_Config_Type == Sensor_Tmp)
{"ͨ��-11(�¶�)"},
#elif (Sensor_CH11_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-11(������̼)"},
#elif (Sensor_CH11_Config_Type == Sensor_Hum)
{"ͨ��-11(ʪ��)"},
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-11(��Ƭ��)"},
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-11(��Ƭ��)"},	
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-11(��Ƭʪ)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-11(����)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-11(γ��)"},
#elif (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-11(�ٶ�)"},
#elif (Sensor_CH11_Config_Type == Sensor_PT1000)
{"ͨ��-11(PT1000)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-11(GWSY-��)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-11(GWSY-ʪ)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-11(GWCH-��)"},
#elif (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-11(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH12_Config_Type == Sensor_Tmp)
{"ͨ��-12(�¶�)"},
#elif (Sensor_CH12_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-12(������̼)"},
#elif (Sensor_CH12_Config_Type == Sensor_Hum)
{"ͨ��-12(ʪ��)"},
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-12(��Ƭ��)"},
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-12(��Ƭ��)"},	
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-12(��Ƭʪ)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-12(����)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-12(γ��)"},
#elif (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-12(�ٶ�)"},
#elif (Sensor_CH12_Config_Type == Sensor_PT1000)
{"ͨ��-12(PT1000)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-12(GWSY-��)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-12(GWSY-ʪ)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-12(GWCH-��)"},
#elif (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-12(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH13_Config_Type == Sensor_Tmp)
{"ͨ��-13(�¶�)"},
#elif (Sensor_CH13_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-13(������̼)"},
#elif (Sensor_CH13_Config_Type == Sensor_Hum)
{"ͨ��-13(ʪ��)"},
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-13(��Ƭ��)"},
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-13(��Ƭ��)"},	
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-13(��Ƭʪ)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-13(����)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-13(γ��)"},
#elif (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-13(�ٶ�)"},
#elif (Sensor_CH13_Config_Type == Sensor_PT1000)
{"ͨ��-13(PT1000)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-13(GWSY-��)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-13(GWSY-ʪ)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-13(GWCH-��)"},
#elif (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-13(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH14_Config_Type == Sensor_Tmp)
{"ͨ��-14(�¶�)"},
#elif (Sensor_CH14_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-14(������̼)"},
#elif (Sensor_CH14_Config_Type == Sensor_Hum)
{"ͨ��-14(ʪ��)"},
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-14(��Ƭ��)"},
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-14(��Ƭ��)"},	
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-14(��Ƭʪ)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-14(����)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-14(γ��)"},
#elif (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-14(�ٶ�)"},
#elif (Sensor_CH14_Config_Type == Sensor_PT1000)
{"ͨ��-14(PT1000)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-14(GWSY-��)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-14(GWSY-ʪ)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-14(GWCH-��)"},
#elif (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-14(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH15_Config_Type == Sensor_Tmp)
{"ͨ��-15(�¶�)"},
#elif (Sensor_CH15_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-15(������̼)"},
#elif (Sensor_CH15_Config_Type == Sensor_Hum)
{"ͨ��-15(ʪ��)"},
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-15(��Ƭ��)"},
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-15(��Ƭ��)"},	
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-15(��Ƭʪ)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-15(����)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-15(γ��)"},
#elif (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-15(�ٶ�)"},
#elif (Sensor_CH15_Config_Type == Sensor_PT1000)
{"ͨ��-15(PT1000)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-15(GWSY-��)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-15(GWSY-ʪ)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-15(GWCH-��)"},
#elif (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-15(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH16_Config_Type == Sensor_Tmp)
{"ͨ��-16(�¶�)"},
#elif (Sensor_CH16_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-16(������̼)"},
#elif (Sensor_CH16_Config_Type == Sensor_Hum)
{"ͨ��-16(ʪ��)"},
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-16(��Ƭ��)"},
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-16(��Ƭ��)"},	
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-16(��Ƭʪ)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-16(����)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-16(γ��)"},
#elif (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-16(�ٶ�)"},
#elif (Sensor_CH16_Config_Type == Sensor_PT1000)
{"ͨ��-16(PT1000)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-16(GWSY-��)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-16(GWSY-ʪ)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-16(GWCH-��)"},
#elif (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-16(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH17_Config_Type == Sensor_Tmp)
{"ͨ��-17(�¶�)"},
#elif (Sensor_CH17_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-17(������̼)"},
#elif (Sensor_CH17_Config_Type == Sensor_Hum)
{"ͨ��-17(ʪ��)"},
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-17(��Ƭ��)"},
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-17(��Ƭ��)"},	
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-17(��Ƭʪ)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-17(����)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-17(γ��)"},
#elif (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-17(�ٶ�)"},
#elif (Sensor_CH17_Config_Type == Sensor_PT1000)
{"ͨ��-17(PT1000)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-17(GWSY-��)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-17(GWSY-ʪ)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-17(GWCH-��)"},
#elif (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-17(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH18_Config_Type == Sensor_Tmp)
{"ͨ��-18(�¶�)"},
#elif (Sensor_CH18_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-18(������̼)"},
#elif (Sensor_CH18_Config_Type == Sensor_Hum)
{"ͨ��-18(ʪ��)"},
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-18(��Ƭ��)"},
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-18(��Ƭ��)"},	
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-18(��Ƭʪ)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-18(����)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-18(γ��)"},
#elif (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-18(�ٶ�)"},
#elif (Sensor_CH18_Config_Type == Sensor_PT1000)
{"ͨ��-18(PT1000)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-18(GWSY-��)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-18(GWSY-ʪ)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-18(GWCH-��)"},
#elif (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-18(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH19_Config_Type == Sensor_Tmp)
{"ͨ��-19(�¶�)"},
#elif (Sensor_CH19_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-19(������̼)"},
#elif (Sensor_CH19_Config_Type == Sensor_Hum)
{"ͨ��-19(ʪ��)"},
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-19(��Ƭ��)"},
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-19(��Ƭ��)"},	
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-19(��Ƭʪ)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-19(����)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-19(γ��)"},
#elif (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-19(�ٶ�)"},
#elif (Sensor_CH19_Config_Type == Sensor_PT1000)
{"ͨ��-19(PT1000)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-19(GWSY-��)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-19(GWSY-ʪ)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-19(GWCH-��)"},
#elif (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-19(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH20_Config_Type == Sensor_Tmp)
{"ͨ��-20(�¶�)"},
#elif (Sensor_CH20_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-20(������̼)"},
#elif (Sensor_CH20_Config_Type == Sensor_Hum)
{"ͨ��-20(ʪ��)"},
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-20(��Ƭ��)"},
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-20(��Ƭ��)"},	
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-20(��Ƭʪ)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-20(����)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-20(γ��)"},
#elif (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-20(�ٶ�)"},
#elif (Sensor_CH20_Config_Type == Sensor_PT1000)
{"ͨ��-20(PT1000)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-20(GWSY-��)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-20(GWSY-ʪ)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-20(GWCH-��)"},
#elif (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-20(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH21_Config_Type == Sensor_Tmp)
{"ͨ��-21(�¶�)"},
#elif (Sensor_CH21_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-21(������̼)"},
#elif (Sensor_CH21_Config_Type == Sensor_Hum)
{"ͨ��-21(ʪ��)"},
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-21(��Ƭ��)"},
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-21(��Ƭ��)"},	
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-21(��Ƭʪ)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-21(����)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-21(γ��)"},
#elif (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-21(�ٶ�)"},
#elif (Sensor_CH21_Config_Type == Sensor_PT1000)
{"ͨ��-21(PT1000)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-21(GWSY-��)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-21(GWSY-ʪ)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-21(GWCH-��)"},
#elif (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-21(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH22_Config_Type == Sensor_Tmp)
{"ͨ��-22(�¶�)"},
#elif (Sensor_CH22_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-22(������̼)"},
#elif (Sensor_CH22_Config_Type == Sensor_Hum)
{"ͨ��-22(ʪ��)"},
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-22(��Ƭ��)"},
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-22(��Ƭ��)"},	
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-22(��Ƭʪ)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-22(����)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-22(γ��)"},
#elif (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-22(�ٶ�)"},
#elif (Sensor_CH22_Config_Type == Sensor_PT1000)
{"ͨ��-22(PT1000)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-22(GWSY-��)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-22(GWSY-ʪ)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-22(GWCH-��)"},
#elif (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-22(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH23_Config_Type == Sensor_Tmp)
{"ͨ��-23(�¶�)"},
#elif (Sensor_CH23_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-23(������̼)"},
#elif (Sensor_CH23_Config_Type == Sensor_Hum)
{"ͨ��-23(ʪ��)"},
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-23(��Ƭ��)"},
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-23(��Ƭ��)"},	
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-23(��Ƭʪ)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-23(����)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-23(γ��)"},
#elif (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-23(�ٶ�)"},
#elif (Sensor_CH23_Config_Type == Sensor_PT1000)
{"ͨ��-23(PT1000)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-23(GWSY-��)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-23(GWSY-ʪ)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-23(GWCH-��)"},
#elif (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-23(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH24_Config_Type == Sensor_Tmp)
{"ͨ��-24(�¶�)"},
#elif (Sensor_CH24_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-24(������̼)"},
#elif (Sensor_CH24_Config_Type == Sensor_Hum)
{"ͨ��-24(ʪ��)"},
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-24(��Ƭ��)"},
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-24(��Ƭ��)"},	
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-24(��Ƭʪ)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-24(����)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-24(γ��)"},
#elif (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-24(�ٶ�)"},
#elif (Sensor_CH24_Config_Type == Sensor_PT1000)
{"ͨ��-24(PT1000)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-24(GWSY-��)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-24(GWSY-ʪ)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-24(GWCH-��)"},
#elif (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-24(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH25_Config_Type == Sensor_Tmp)
{"ͨ��-25(�¶�)"},
#elif (Sensor_CH25_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-25(������̼)"},
#elif (Sensor_CH25_Config_Type == Sensor_Hum)
{"ͨ��-25(ʪ��)"},
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-25(��Ƭ��)"},
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-25(��Ƭ��)"},	
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-25(��Ƭʪ)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-25(����)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-25(γ��)"},
#elif (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-25(�ٶ�)"},
#elif (Sensor_CH25_Config_Type == Sensor_PT1000)
{"ͨ��-25(PT1000)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-25(GWSY-��)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-25(GWSY-ʪ)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-25(GWCH-��)"},
#elif (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-25(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH26_Config_Type == Sensor_Tmp)
{"ͨ��-26(�¶�)"},
#elif (Sensor_CH26_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-26(������̼)"},
#elif (Sensor_CH26_Config_Type == Sensor_Hum)
{"ͨ��-26(ʪ��)"},
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-26(��Ƭ��)"},
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-26(��Ƭ��)"},	
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-26(��Ƭʪ)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-26(����)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-26(γ��)"},
#elif (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-26(�ٶ�)"},
#elif (Sensor_CH26_Config_Type == Sensor_PT1000)
{"ͨ��-26(PT1000)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-26(GWSY-��)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-26(GWSY-ʪ)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-26(GWCH-��)"},
#elif (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-26(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH27_Config_Type == Sensor_Tmp)
{"ͨ��-27(�¶�)"},
#elif (Sensor_CH27_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-27(������̼)"},
#elif (Sensor_CH27_Config_Type == Sensor_Hum)
{"ͨ��-27(ʪ��)"},
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-27(��Ƭ��)"},
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-27(��Ƭ��)"},	
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-27(��Ƭʪ)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-27(����)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-27(γ��)"},
#elif (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-27(�ٶ�)"},
#elif (Sensor_CH27_Config_Type == Sensor_PT1000)
{"ͨ��-27(PT1000)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-27(GWSY-��)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-27(GWSY-ʪ)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-27(GWCH-��)"},
#elif (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-27(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH28_Config_Type == Sensor_Tmp)
{"ͨ��-28(�¶�)"},
#elif (Sensor_CH28_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-28(������̼)"},
#elif (Sensor_CH28_Config_Type == Sensor_Hum)
{"ͨ��-28(ʪ��)"},
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-28(��Ƭ��)"},
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-28(��Ƭ��)"},	
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-28(��Ƭʪ)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-28(����)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-28(γ��)"},
#elif (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-28(�ٶ�)"},
#elif (Sensor_CH28_Config_Type == Sensor_PT1000)
{"ͨ��-28(PT1000)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-28(GWSY-��)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-28(GWSY-ʪ)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-28(GWCH-��)"},
#elif (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-28(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH29_Config_Type == Sensor_Tmp)
{"ͨ��-29(�¶�)"},
#elif (Sensor_CH29_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-29(������̼)"},
#elif (Sensor_CH29_Config_Type == Sensor_Hum)
{"ͨ��-29(ʪ��)"},
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-29(��Ƭ��)"},
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-29(��Ƭ��)"},	
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-29(��Ƭʪ)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-29(����)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-29(γ��)"},
#elif (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-29(�ٶ�)"},
#elif (Sensor_CH29_Config_Type == Sensor_PT1000)
{"ͨ��-29(PT1000)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-29(GWSY-��)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-29(GWSY-ʪ)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-29(GWCH-��)"},
#elif (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-29(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH30_Config_Type == Sensor_Tmp)
{"ͨ��-30(�¶�)"},
#elif (Sensor_CH30_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-30(������̼)"},
#elif (Sensor_CH30_Config_Type == Sensor_Hum)
{"ͨ��-30(ʪ��)"},
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-30(��Ƭ��)"},
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-30(��Ƭ��)"},	
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-30(��Ƭʪ)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-30(����)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-30(γ��)"},
#elif (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-30(�ٶ�)"},
#elif (Sensor_CH30_Config_Type == Sensor_PT1000)
{"ͨ��-30(PT1000)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-30(GWSY-��)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-30(GWSY-ʪ)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-30(GWCH-��)"},
#elif (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-30(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH31_Config_Type == Sensor_Tmp)
{"ͨ��-31(�¶�)"},
#elif (Sensor_CH31_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-31(������̼)"},
#elif (Sensor_CH31_Config_Type == Sensor_Hum)
{"ͨ��-31(ʪ��)"},
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-31(��Ƭ��)"},
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-31(��Ƭ��)"},	
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-31(��Ƭʪ)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-31(����)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-31(γ��)"},
#elif (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-31(�ٶ�)"},
#elif (Sensor_CH31_Config_Type == Sensor_PT1000)
{"ͨ��-31(PT1000)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-31(GWSY-��)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-31(GWSY-ʪ)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-31(GWCH-��)"},
#elif (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-31(GWCH-ʪ)"},
#else
{"��"},
#endif

#if	(Sensor_CH32_Config_Type == Sensor_Tmp)
{"ͨ��-32(�¶�)"},
#elif (Sensor_CH32_Config_Type == Sensor_CarbonDioxide)
{"ͨ��-32(������̼)"},
#elif (Sensor_CH32_Config_Type == Sensor_Hum)
{"ͨ��-32(ʪ��)"},
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
{"ͨ��-32(��Ƭ��)"},
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
{"ͨ��-32(��Ƭ��)"},	
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
{"ͨ��-32(��Ƭʪ)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon)
{"ͨ��-32(����)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat)
{"ͨ��-32(γ��)"},
#elif (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd)
{"ͨ��-32(�ٶ�)"},
#elif (Sensor_CH32_Config_Type == Sensor_PT1000)
{"ͨ��-32(PT1000)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)
{"ͨ��-32(GWSY-��)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
{"ͨ��-32(GWSY-ʪ)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp)
{"ͨ��-32(GWCH-��)"},
#elif (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum)
{"ͨ��-32(GWCH-ʪ)"},
#else
{"��"},
#endif

};

/* ͨ����ֵ Ӣ���б� */
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

/* ͨ����ֵ���棨�޴�����ʱ�� ���� �˵��� */
u8* const Cha_NULL_CN_Tab = "��";
/* ͨ����ֵ���棨�޴�����ʱ�� Ӣ�� �˵��� */
u8* const Cha_NULL_EN_Tab = "NULL";

/* ͨ����ֵ���� ���� �˵���Ŀ�� */
u8 * Menu_Cha_Tsd_CN[Sensor_CH_Config_Num] = {0};
/* ͨ����ֵ���� Ӣ�� �˵���Ŀ�� */
u8 * Menu_Cha_Tsd_EN[Sensor_CH_Config_Num] = {0};

/* ͨ����ֵ���� ��Ӣ�� �˵��� */
const struct Menu Menu_Cha_Tsd[] =
{
	MenuWhat(Menu_Cha_Tsd_CN),
	MenuWhat(Menu_Cha_Tsd_EN),
};

/* ѡ���ͨ������ָ�� */
#define Menu_Cha_Type_P(Cha) 	(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[Cha*sizeof(INSTRU_SENSOR_CHANNEL)]))->Type)


/************************************************************************************************/
/* ������	: Menu_Cha_Type_Sec															*/
/* ����		: ������ͨ������																		*/
/* ���		: �� 																				*/
/* ����		: ���ݴ���������ѡ������Ӧ�����ͽ��档													*/
/* ��������	: 2015/09/16																			*/
/************************************************************************************************/
static void Menu_Cha_Type_Sec(u8 cha_type)
{

	/* �жϴ�����ͨ������ *///�����ڲ˵��д�����������Ҫ�ֳ����ࣺ1���¶����ͣ�2��ʪ�����͡�
	switch(cha_type)
	{
		case Sensor_Tmp:
		{
			/* ���� �¶����� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
    }
		case Sensor_CarbonDioxide:
		{
			/* ���� �¶����� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
    }
		case Sensor_Hum:
		{
			/* ���� ʪ������ ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
			break;
        }
		
		case Sensor_RF_Card_Single_Tmp:
		{
			/* ���� �¶����� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
        }
		
		case Sensor_RF_Card_Humiture_Tmp:
		{
			/* ���� �¶����� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
        }
		
		case Sensor_RF_Card_Humiture_Hum:
		{
			/* ���� ʪ������ ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);	
			/* ���뵽��һ������ */
			PUSH();
            break;
        }
		
		
		/* ���� ���� */
		case Sensor_GPS_POS_Lon:
		{
			/* �������� û�����������ý���. */
			Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
            break;
        }
		
		/* γ�� ���� */
		case Sensor_GPS_POS_Lat:
		{
			/* γ������ û�����������ý���. */
			Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
            break;
        }
		
		/* �ٶ� ���� */
		case Sensor_GPS_POS_Spd:
		{
			/* �ٶ����� û�����������ý���. */
			Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
            break;
        }
		
		/* 	PT1000 ���� */
		case Sensor_PT1000:
		{
			/* ���� �¶����� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
        }
		
		case Sensor_GW_NJSY_Tmp:
		{
			/* ���� ������ѡ�� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
        }
		
		case Sensor_GW_NJSY_Hum:
		{
			/* ���� ������ѡ�� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
        }
		
		case Sensor_GW_HZZH_Tmp:
		{
			/* ���� ������ѡ�� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
        }
		
		case Sensor_GW_HZZH_Hum:
		{
			/* ���� ������ѡ�� ���� */
			Jump2Menu(MenuSenUpLowlitSecIndex,FlashMode_ManualInit);
			/* ���뵽��һ������ */
			PUSH();
            break;
        }
		
		
        default:
		{
			/* δ֪���ͣ���������档 */
			Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
            break;
        }
	}

}

/************************************************************************************************/
/* ������	: DispMenuChaTsdInit																*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ��ʾͨ����ֵ��Ϣ����																	*/
/* ��������	: 2015/9/16																			*/
/************************************************************************************************/
void DispMenuChaTsdInit(void)
{
	
	
	/* �ֶ����ò��� */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{	
		/* �����Ļ��ʾ */
		GUI_Clear();
		/* ���ý�����ʾ���ݺ����� */
		BarMenu = (struct Menu *)&Menu_Cha_Tsd[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		if(Flash != FlashMode_ReFlash)
		{
			/* ������Ŀ��� */
			Item = 0;
			Where = 0;			
		}
		/* ��ʾ��ǰ�����µ���Ŀ�͹����� */
		BarMenuInit();
		/* ��ʾ��ǰ��������� */
		GUI_Disp_Headline(*(Menu_Alarm_Set[*Language].Text+ItemBackup[ItemBackup_i-1]));
	}
	
}

/************************************************************************************************/
/* ������	: DispMenuChaTsdUp																	*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ���� "ѡ��" ͨ������																*/
/* ��������	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdUp(void)
{
	/* ���ݴ���������ѡ����� */
	Menu_Cha_Type_Sec(Menu_Cha_Type_P(Item));
}

/************************************************************************************************/
/* ������	: DispMenuChaTsdDown																*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: �˳� "ѡ��" ͨ������															*/
/* ��������	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdDown(void)
{
	/* �˻ص���һ������ */
	POP();
}

/************************************************************************************************/
/* ������	: DispMenuChaTsdLeft																*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����ѡ��ͨ����Ŀ																*/
/* ��������	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdLeft(void)
{
    BarMenuLeft();
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuChaTsdRight																*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����ѡ��ͨ����Ŀ																*/
/* ��������	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdRight(void)
{
    BarMenuRight();
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuChaTsdOK																	*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����ȷ����������ʱû��ʹ�ã�															*/
/* ��������	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdOK(void)
{
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuChaTsdCancel																*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����ȡ����������ʱû��ʹ�ã�															*/
/* ��������	: 2015/09/16																			*/
/************************************************************************************************/
void DispMenuChaTsdCancel(void)
{
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuChaTsdIndex,FlashMode_NoAction);
} 

/******************* (C) ZHOU *****END OF FILE****/


