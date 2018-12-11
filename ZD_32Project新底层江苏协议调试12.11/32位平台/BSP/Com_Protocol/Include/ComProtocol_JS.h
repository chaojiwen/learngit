/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : ComProtocol_JS.h																*/
/**作用       : 根据江苏省免疫规划数据中心提供的通信协议格式将探头数据上传到云平台							*/
/**原理       : 																					*/
/************************************************************************************************/
/************************************************************************************************/ 
#ifndef __ComProtocol_JS_h
#define	__ComProtocol_JS_h
#include "Log_GSM.h"
#if (AppGPRSComProtocol == 0x04)
#include "App_Save.h"

#define ComProtocolZDE_TXSize                    (512u)
#define ComProtocolZDE_RXSize                    (512u)

#define  COM_END_Bytes                           (8u)//协议末尾占8个字节 =运行状态及标识(2)+电量状态及标识(2)+信号状态及标识(2)+校验码(2) 
#define  SENSOR_CH_NUM_LEN_Bytes                 (1u)//通道数长度值占1个字节 
#define  SN_LEN_Bytes                            (1u)//SN长度值占1个字节 
#define  Single_Sensor_Bytes                     (2u)//每个传感器通道数据长度
#define  Time_Stamp_Bytes                        (4u)//时间戳数据长度
#define  SN_DATA_MAX_Len	                       (50u)//SN号数据最大长度(数据类型为uint8_t)
#define  SENSOR_DATA_MAX_Len	                   (32u)//数据体最大长度  (32个通道，每个通道的数据类型为uint16_t)
                                                 
#define  COMReg1ExceptSN_Len                     (4u)//除SN号COMReg1的长度 
#define  COMReg2_Len                             (5u)//COMReg2_Len的长度 

/*************************************************************************************/
#define JS_UL_SYNC_Identify		                   (0xAA)//设备发送数据同步标识位值
#define JS_DL_SYNC_Identify		                   (0x55)//设备接收数据同步标识位值
                                                 
#define JS_UL_STX_Identify		                   (0x75)//设备发送数据起始标识位值
#define JS_DL_STX_Identify		                   (0x7A)//设备接收数据起始标识位值
                                                   
#define JS_YUN_STATE_Identify		                 (0xFD)//设备运行状态标识位值
                                                 
#define JS_POWER_STATE_Identify		               (0xFC)//设备电池电量标识位值
                                                 
#define JS_SIGNLE_STATE_Identify		             (0xFB)//设备GSM信号强度标识位值
                                                 
#define  Sensor_CH_Num                           (2u)//传感器通道数量 


typedef struct
{	
	/* 数据寄存器 */
	struct
	{
		/* “泽大通用协议”数据寄存器 */
		uint8_t TXDR[ComProtocolZDE_TXSize];
		uint8_t RXDR[ComProtocolZDE_RXSize];
		/* 发送数据长度 */
		uint16_t TXLen;
	}DR;	
}
ComProtocolZDEAppStruct;
//消息头数据结构1
  typedef struct
  {
  /* 同步数据标志 */
    uint8_t   JS_SYNC_Flag;
  /* 起始数据标志 */
    uint8_t   JS_STX_Flag;
  /*总的数据体长度=SN号长度(4个字节)+时间戳长度(4个字节)+CH_Num*Single_Sensor_Bytes    CH_Num表示温湿度通道个数*/
    uint8_t   JS_DATA_BODY_Len;
  /*SN号数据体内容长度 */
  	uint8_t   JS_SN_DATA_Len;
  /*SN号数据体内容 */
    uint8_t   JS_SN_DATA[SN_DATA_MAX_Len];
  } ComProtocolJS_MH1; 


//消息头数据结构2
  typedef struct
  {
  	/*时间戳数据体内容 */
    uint32_t   JS_SN_Time_Stamp;	
   /*传感器通道个数 */
    uint8_t   JS_SENSOR_CH_Num;	
  }ComProtocolJS_MH2; //5个字节 
 
 //消息头数据结构3
  typedef struct
  {
   /*传感器数据体内容 */
    uint16_t  JS_SENSOR_Data[32];
  }ComProtocolJS_MH3;  
  
//消息头数据结构4
  typedef struct
  {	
  /*设备运行状态标志位 */
    uint8_t   JS_RUN_STATE_Flag;//0xFD
  	/*设备运行状态 */
    uint8_t   JS_RUN_State;//第0位代表供电状态 0：正常 1故障 	  第1~7位为预留状态位
  	/*设备电池电量标志位 */
    uint8_t   JS_POWER_STATE_Flag;//0xFC
  	/*设备电池电量百分比 */
    uint8_t   JS_POWER_State;//数值范围：0%~100%
  	/*设备GSM模块信号强度标志位 */
    uint8_t   JS_SIGNLE_STATE_Flag;//0xFB
  	/*设备GSM模块信号强度百分比 */
    uint8_t   JS_SIGNLE_State;//数值范围：0%~100%
    /* 数据CRC校验值*/
    unsigned short    JS_DATA_CRC_CheckCode;	
  } ComProtocolJS_MH4;//共 8个字节 

typedef struct
{
	uint16_t Year;
	int8_t Mon;
	int8_t Day;	
	int8_t Week;	
	int8_t Hour;		
	int8_t Min;				
	int8_t Sec;					
}RTC_T;



BOOL ComProtocol_JS_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab, uint8_t* destBuf,int* dataLen);

int docrc16(uint8_t *pSrcData,int length);//校验标准  CRC16_CCITT(1021)算法

#endif

#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

