#include "interface.h"
#include "BSP.h"
#include "app.h"

#include "Ks0108.h"
#include "Log_GSM.h"

/* GPRS 发送数据缓存 */
static u8 GPRS_Send_Sensor_Data_Buffer1[1500];


/* GPRS 接收数据缓存 */
static u8 GPRS_Rece_Sensor_Data_Buffer1[1500];

/* GPRS 发送温湿度数据信息 数据寄存器 */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_Sensor_Data_Reg = 
{
	/* GPRS 接收 传感器 数据数量 */
	0,
	/* GPRS 发送 传感器 数据数量 */
	0,
	/* GPRS 发送传感器数据通信状态（FALSE:没有发送）*/
	FALSE,
	/* GPRS 主机序传感器数据等待处理状态（FALSE:没有数据处理） */
	FALSE,
	/* GPRS 网络序传感器数据等待处理状态（FALSE:没有数据处理） */
	FALSE,
	/* GPRS 发送传感器数据结果 */
	TRUE,
	/* GPRS 接收传感器数据结果 */
	TRUE,
	/* GPRS 解析数据状态 */
	TRUE,	
	/* GPRS 发送传感器数据 失败次数 */
	0,
};


/*
***向服务器发送远程升级指令



*/
void Remote_Upgrade_Program(void)
{
    gsm_gprs_comm(
	  (char*)GPRS_Send_Sensor_Data_Buffer1,
		(1500),
	  (char*)GPRS_Rece_Sensor_Data_Buffer1,
	  ((int*)(&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)));
		
}
