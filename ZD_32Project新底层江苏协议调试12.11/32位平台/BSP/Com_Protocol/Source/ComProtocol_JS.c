/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : ComProtocol_JS.c																*/
/**作用       : 根据江苏省免疫规划数据中心提供的通信协议格式将探头数据上传到云平台							*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1     ZWC         2018年10月19日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "ComProtocol_JS.h"
#if (AppGPRSComProtocol == 0x04)
#include <string.h>
#include "App_Save.h"
#include "Dev_Flash.h"
#include <stdio.h>
#include <stdlib.h>
#include "String_Utility.h"
#include "GSM_app.h"
/* 打印调试标志 */
extern BOOL GSM_DEBUG_Flag;// TRUE

/*******************************************************************************/
/********************************* 全局变量声明 *****************************/
ComProtocolZDEAppStruct    ComProtocolZDEApp; 	
/* 没有查到匹配的通道 */
#define Sensors_CH_Null (0xFF)
/* 读 Fram 数据的缓存指针 */
Save_Sensor_Data_Struct *read_sensor_data_p;

#define SEC_PER_8HOUR       (28800u)//8个小时的秒数


//CRC16_CCITT(1021)校验值表
const int crc16_ccitt_table[] = { 0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a,
            0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6, 0x9339, 0x8318, 0xb37b,
            0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de, 0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a, 0xb54b, 0x8528,
            0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719,
            0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e,
            0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf,
            0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41, 0xedae, 0xfd8f, 0xcdec,
            0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe, 0xdfdd,
            0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2,
            0x20e3, 0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3,
            0x32d2, 0x4235, 0x5214, 0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d, 0x34e2, 0x24c3, 0x14a0,
            0x0481, 0x7466, 0x6447, 0x5424, 0x4405, 0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3, 0x36f2, 0x0691,
            0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806,
            0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37,
            0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64,
            0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8, 0x6e17, 0x7e36, 0x4e55,
            0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0 };
//校验标准  CRC16_CCITT(1021)算法
 int docrc16(uint8_t *pSrcData,int length) {
        int crc_reg = 0x0000;
        int i=0;
        for (i = 0; i <length; i++) {
            crc_reg =  crc16_ccitt_table[(crc_reg ^ pSrcData[i]) & 0xFF] ^ (crc_reg >> 8);
        }
        return crc_reg;
    }
 
		
		
uint32_t mktimev(RTC_T tm)
{
	if(0 >= (int)(tm.Mon -= 2) )
	{	/* 1..12 -> 11,12,1..10 */
		tm.Mon += 12;		/* Puts Feb last since it has leap day */
		tm.Year -= 1;
	}

	//先换算闰年和非闰年得到days天数
	//再得出有多少个小时
	//再得出有多少个分钟
	//最后得出有多少个秒
	return ( ( ( (uint32_t)(tm.Year/4 - tm.Year/100 + tm.Year/400 + 367*tm.Mon/12 + tm.Day) + \
		         tm.Year * 365 - 719499 													  \
			   ) * 24 + tm.Hour 															  \
			 ) * 60 + tm.Min 	 															  \
		   ) * 60 + tm.Sec; 	 
}
	
		

//将电池电量转化为百分比
uint8_t   Power_BT_State(INSTRU_POWER* power_t)
{
	 uint8_t  Power_BT_Percentage=0;//电池电量百分比
	
	/* 判断当前电池电量状态 */
	if(power_t->Inf_State.BT_Quantity_State <10)
	{
		/* 电池电量不足 */
		
		 Power_BT_Percentage=0;//电池电量百分比
		
		
	}else if((power_t->Inf_State.BT_Quantity_State >=10)&&(power_t->Inf_State.BT_Quantity_State<30))
	{
		/* 电池电量正常 */
	
		 Power_BT_Percentage=40;//电池电量百分比
		
	}
	else if((power_t->Inf_State.BT_Quantity_State >=30)&&(power_t->Inf_State.BT_Quantity_State<70))
	{
		/* 电池电量正常 */
	
		 Power_BT_Percentage=80;//电池电量百分比
		
	}
	else if((power_t->Inf_State.BT_Quantity_State >=70)&&(power_t->Inf_State.BT_Quantity_State<100))
	{
		/* 电池电量正常 */
	
		 Power_BT_Percentage=100;//电池电量百分比
		
	}
	else
	{
	  /* 电池电量正常 */
		 Power_BT_Percentage=255;//电池电量采集异常
	}

  return	Power_BT_Percentage;//返回电池电量百分比
}		


//将GSM模块信号强度转化为百分比
uint8_t   GSM_SINGLE_State(void)
{
	 uint8_t  GSM_SINGLE_Percentage=0;//GSM模块信号百分比
	/* 判断GSM模块信号强度状态 */
	if(GPRS_SR_Reg.GPRS_Sign_Stre<10)
	{
		/* 电池电量不足 */
		
		 GSM_SINGLE_Percentage=0;//GSM模块信号百分比
		
		
	}else if((GPRS_SR_Reg.GPRS_Sign_Stre >=10)&&(GPRS_SR_Reg.GPRS_Sign_Stre<12))
	{
		/* 电池电量正常 */
	
		 GSM_SINGLE_Percentage=40;//GSM模块信号百分比
		
	}
	else if((GPRS_SR_Reg.GPRS_Sign_Stre>=12)&&(GPRS_SR_Reg.GPRS_Sign_Stre<15))
	{
		/* 电池电量正常 */
	
		 GSM_SINGLE_Percentage=80;//GSM模块信号百分比
		
	}
	else if((GPRS_SR_Reg.GPRS_Sign_Stre>=15)&&(GPRS_SR_Reg.GPRS_Sign_Stre<32))
	{
		/* 电池电量正常 */
	
		 GSM_SINGLE_Percentage=100;//GSM模块信号百分比
		
	}
	else
	{
	  /* 电池电量正常 */
		 GSM_SINGLE_Percentage=255;//电池电量采集异常
	}

  return	GSM_SINGLE_Percentage;//返回电池电量百分比
}

void   fun1(char *SN)	
{

      ComProtocolJS_MH1* COMReg1 = (ComProtocolJS_MH1*)&ComProtocolZDEApp.DR.TXDR;
	    COMReg1->JS_SYNC_Flag    = JS_UL_SYNC_Identify;//设置同步标志参数 
	    COMReg1->JS_STX_Flag     = JS_UL_STX_Identify ;//设置起始标志参数
	    COMReg1->JS_DATA_BODY_Len=SN_LEN_Bytes+strlen(SN)+Time_Stamp_Bytes+SENSOR_CH_NUM_LEN_Bytes+Sensor_CH_Num*Single_Sensor_Bytes+COM_END_Bytes; //数据体总长度 
	    COMReg1->JS_SN_DATA_Len=strlen(SN); //SN号数据长度 
	    memcpy(COMReg1->JS_SN_DATA,SN,strlen(SN)) ;
}

BOOL   fun2(AppSaveFrameStruct* dataFrame,char *SN)	
{
//	    time_t  t=0;
	
	
////      struct tm stm;
		   RTC_T    t;
	
	
   	  ComProtocolJS_MH2* COMReg2 = (ComProtocolJS_MH2*)&ComProtocolZDEApp.DR.TXDR[COMReg1ExceptSN_Len+strlen(SN)];
	
	
	if((dataFrame->Timestamp.Year<=0)\
		|| (dataFrame->Timestamp.Month<=0)\
		|| (dataFrame->Timestamp.Day<=0))
	{
		return FALSE;
	}
	



		       /* 探头信息 */
  	       /* 设置采集时间  */
	         Log_printf("数据采集时间:%d年%d月%d日 %d时%d分%d秒\n",dataFrame->Timestamp.Year,
	                                                        dataFrame->Timestamp.Month,
	                                                        dataFrame->Timestamp.Day,
	                                                        dataFrame->Timestamp.Hour,
	                                                        dataFrame->Timestamp.Min,
	                                                        dataFrame->Timestamp.Sec);
	                                                        
	          Log_printf("通道1数据:%d  通道2数据:%d\n",(int)(dataFrame->Sensor[0]*100),(int)(dataFrame->Sensor[1]*100));
				 

	
	
/***************时间戳**************/
////////      stm.tm_year =dataFrame->Timestamp.Year; //年
////////      stm.tm_mon  =dataFrame->Timestamp.Month;//月
////////      stm.tm_mday =dataFrame->Timestamp.Day;//日
////////      stm.tm_hour =dataFrame->Timestamp.Day;//时
////////      stm.tm_min  =dataFrame->Timestamp.Min;//分
////////      stm.tm_sec  =dataFrame->Timestamp.Sec;//秒
////////      stm.tm_year-=1900; /*年份值减去1900，得到tm结构中保存的年份序数*/ 
////////      stm.tm_mon-=1;    /*月份值减去1，得到tm结构中保存的月份序数*/
////////			stm.tm_mday-=1;    /*日份值减去1，得到tm结构中保存的日份序数*/
////////      t=mktime(&stm);  /* 若用户输入的日期时间有误，则函数返回值为-1*/
////////	    t=(t-SEC_PER_16HOUR); //这里计算出来的时间是北京时间要转化为零时区的时间需要减去16小时时间/
				   t.Year=dataFrame->Timestamp.Year; //年
			     t.Mon=dataFrame->Timestamp.Month; //月
			     t.Day=dataFrame->Timestamp.Day;   //日
			     t.Hour=dataFrame->Timestamp.Hour; //时
			     t.Min=dataFrame->Timestamp.Min;   //分
				   t.Sec=dataFrame->Timestamp.Sec;   //秒

/******************************************/

				    Log_printf("数据采集时间十进制数:%d\n",(mktimev(t)-28800));
				 
	    COMReg2->JS_SN_Time_Stamp=(mktimev(t)-28800);//设置时间戳   //这里计算出来的时间是北京时间要转化为零时区的时间需要减去8小时时间/
	    
	    COMReg2->JS_SENSOR_CH_Num=Sensor_CH_Config_Num;//传感器通道数量
			
			
		return TRUE;	
}
void   fun3(AppSaveFrameStruct* dataFrame,char *SN)
{
	    int i=0;
      ComProtocolJS_MH3* COMReg3 = (ComProtocolJS_MH3*)&ComProtocolZDEApp.DR.TXDR[COMReg1ExceptSN_Len+strlen(SN)+COMReg2_Len];
    

	   //拷贝各个通道传感器数据 
     for(i=0;i<Sensor_CH_Config_Num;i++)
	   {
	     	COMReg3->JS_SENSOR_Data[i]=(int)(dataFrame->Sensor[i]*100);	//将浮点型温湿度数据转换为整型数据
	   }
}	

void   fun4(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,char *SN)
{
	
   	 ComProtocolJS_MH4* COMReg4 = (ComProtocolJS_MH4*)&ComProtocolZDEApp.DR.TXDR[COMReg1ExceptSN_Len+strlen(SN)+COMReg2_Len+Sensor_CH_Config_Num*sizeof(uint16_t)];
	   COMReg4->JS_RUN_STATE_Flag =JS_YUN_STATE_Identify;//运行状态标志位 
	    if(config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State==TRUE)
			{
			       COMReg4->JS_RUN_State=0x00;//仪器供上了外接电   (仪器运行状态正常)
			}
			else
			{
			      COMReg4->JS_RUN_State=0x01;//仪器未供上外接电   (仪器运行状态故障)
			}
	   COMReg4->JS_POWER_STATE_Flag=JS_POWER_STATE_Identify;//电池电量标志位 
	   COMReg4->JS_POWER_State= Power_BT_State(&Instru_Config_Inf_Tab.Inf_State.Instru_Power);//电池电量值  0%--40%--80%--100%
	   COMReg4->JS_SIGNLE_STATE_Flag=JS_SIGNLE_STATE_Identify;//信号强度标志位 
	   COMReg4->JS_SIGNLE_State= GSM_SINGLE_State();//GSM模块信号强度值 
	   COMReg4->JS_DATA_CRC_CheckCode=docrc16(ComProtocolZDEApp.DR.TXDR,COMReg1ExceptSN_Len+strlen(SN)+COMReg2_Len+Sensor_CH_Config_Num*sizeof(uint16_t)+(COM_END_Bytes-sizeof(uint16_t)));
}
/************************************************************************************************/
/* 函数名	: ComProtocol_JS_CreateData										 	 				*/
/* 输入  	: 无			 																	*/
/* 输出		: 无																				*/
/* 作用	 	: 根据江苏省免疫规划数据中心通信协议，创建数据。					 							*/
/* 创建日期	: 2018/10/23																		*/
/* 软件版本	: V1.0																				*/
/************************************************************************************************/
//BOOL ComProtocol_JS_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab,
//							                  uint8_t* destBuf,int* dataLen)
////BOOL ComProtocol_JS_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
BOOL ComProtocol_JS_CreateData(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab, uint8_t* destBuf,int* dataLen)
{
//////	     int i=0;
	    uint16_t  Instrument_Serial_Number=0;//仪器序号
//config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[]  前2个字节表示年份和月 第3和第4个字节为序号(数据类型为整型)
/******************SN号********************/
	   char SN[50]={0};
		 	/* 应用存储数据 */
	   AppSaveFrameStruct cSaveFrameReg;
		 	/* 存储数据状态 */
	   Save_ReadState SaveDataState;
		  
		 HexToStr((u8*)SN,config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number,2);
	
		 Instrument_Serial_Number=(config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[2]<<8)+ \
		                           config_tab->Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[3];//获取仪器序列号
 
		 sprintf(&SN[4],"%05d",Instrument_Serial_Number);//格式化输出仪器SN号
		 
	    /* 读当当前存储数据状态 */
	   SaveDataState = AppSaveAPI.ReadSaveDataFrame(&cSaveFrameReg);
	   /* 判断读读存储数据状态 */
	   if(Save_ReadFail == SaveDataState)
	   {
			 
			  *dataLen=0;
//////////			 if(GSM_DEBUG_Flag ==TRUE)
//////////			 {
//////////			 			 printf("没有历史数据\n");
//////////			 }

	   	/* 没有存储数据 */
	   	return FALSE;
	   }
		 
	   memset(ComProtocolZDEApp.DR.TXDR,0x00,sizeof(ComProtocolZDEApp.DR.TXDR));
/***********************单条数据*******************************/	

	     fun1(SN);
			 fun2(&cSaveFrameReg,SN);
			 fun3(&cSaveFrameReg,SN);
			 fun4(config_tab,fix_tab,SN);
		 
		 
	  //数据长度 
	   ComProtocolZDEApp.DR.TXLen=COMReg1ExceptSN_Len+strlen(SN)+COMReg2_Len+Sensor_CH_Config_Num*sizeof(uint16_t)+COM_END_Bytes; 
/*********************************************************************/	
   //拷贝组装好要发送的的数据
     memcpy(destBuf,ComProtocolZDEApp.DR.TXDR, ComProtocolZDEApp.DR.TXLen);

     *dataLen=ComProtocolZDEApp.DR.TXLen;
		 
////		 printf("要发送数据的内容:\n"); 
////    for(i=0;i<ComProtocolZDEApp.DR.TXLen;i++)
////    {
////    	printf("%02X ",ComProtocolZDEApp.DR.TXDR[i]);	
////	  }


	   	return TRUE;		
  } 

#endif



/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/

