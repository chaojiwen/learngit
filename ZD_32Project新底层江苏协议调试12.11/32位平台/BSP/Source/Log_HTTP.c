/************************************************************************************************/
/** @file              : Log_HTTP.c																*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年06月03日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--变送器--逻辑层--HTTP 应用文件								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : Log_HTTP.c																		*/
/**作用       : 将温湿度数据数转换成HTTP格式字符串。												*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年06月03日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Log_HTTP.h"

/************************************************************************************************/
#include <string.h>
#include <stdio.h>


/************************************************************************************************/
#include "BSP.h"

#include "Log_MD5.h"
#include "Log_Time.h"

#include "Mcu_Config.h"

#include "System_Config.h"

/* HTTP参数 */
CreateHTTPParaStruct HTTPPara;


/************************************************************************************************/
/* 函数名	：HTTPSet_Dody																				*/
/* 输入		：destbuf:数据缓存指针																				*/
/* 输出		：主体信息长度 																				*/
/* 作用		：创建主体信息					 									 		 		*/
/* 创建日期	：2015/06/21																		*/
/* 创建日期	：2015/06/21																		*/
/************************************************************************************************/
static u32 HTTPSet_Dody(char * destbuf)
{
	/* 缓存 */
	char buf[256];
	/* 探头编号 */
	unsigned char probeNum;
	/* 计数器 */
	unsigned char num;

	
	
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置Body开始标签 */
	sprintf(buf,HTTPBody_Start);
	/* 拷贝缓存数据 */
	strcpy(destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置网关编号 */
	sprintf(buf,	HTTPBody_ProbeIdNo,HTTPPara.ApiPara.GatewayIdNo);
	/* 拷贝缓存数据 */
	strcat(destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置时间戳 */
	sprintf(buf,HTTPBody_Timestamp,HTTPPara.ApiPara.TimeStamp);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	
	/* 设置参数签名 */
	sprintf(buf,"gatewayIdNo=%stimestamp=%dapiKey=%s",
			HTTPPara.ApiPara.GatewayIdNo,
			HTTPPara.ApiPara.TimeStamp,
			HTTP_USER_KEY);
	/* 进行MD5运算 */
	App_MD5(HTTPPara.ApiPara.Sign,buf);
	/* 清除参数签名缓存 */
	memset(buf,	 0x00, sizeof(buf));
	/* 保存参数签名 */
	sprintf(buf,HTTPBody_Sign,HTTPPara.ApiPara.Sign);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	
	/* 设置GPS参数 */
	/* 判断GPS设备状态 */
	if((HTTPPara.ApiPara.GPS_State == TRUE)&&
	   (HTTPPara.ApiPara.Lng != GPS_Data_Rece_Timeout_Value)&&
	   (HTTPPara.ApiPara.Lat != GPS_Data_Rece_Timeout_Value))
	{
		/* 数据(UCOS-II使用sprintf存在字节对齐问题) */
		char lngdataStr[16];
		char latdataStr[16];
		
		
		/* 清除缓存 */
		memset(lngdataStr,0x00, sizeof(lngdataStr));
		memset(latdataStr,0x00, sizeof(latdataStr));
		
		/* 将数据转换成字符串 */
		float_to_string(HTTPPara.ApiPara.Lng,lngdataStr);
		/* 存在GPS设备，设置经度参数 */
		sprintf(buf,HTTPBody_Lng,lngdataStr);
		/* 拷贝缓存数据 */
		strcat (destbuf,buf);
		/* 清除缓存 */
		memset(buf,0x00, sizeof(buf));
		
		/* 将数据转换成字符串 */
		float_to_string(HTTPPara.ApiPara.Lat,latdataStr);
		/* 存在GPS设备，设置纬度参数 */
		sprintf(buf,HTTPBody_Lat,latdataStr);
		/* 拷贝缓存数据 */
		strcat (destbuf,buf);
		/* 清除缓存 */
		memset(buf,0x00, sizeof(buf));
	}
	
	/* 设置采集时间 */
	sprintf(buf,HTTPBody_CollectTime,HTTPPara.ApiPara.CollectTime);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置logs开始标签 */
	sprintf(buf,HTTPBody_Logs_Start);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置Logs参数 */
	for(num = 0,probeNum = 0;(probeNum < HTTPPara.ApiPara.ProbeNum)&&(num < 32) ;num++)
	{
		/* 判断当前探头类型 */
		if(Sensor_Tmp == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* 数据(UCOS-II使用sprintf存在字节对齐问题) */
			char tempdataStr[16];
			
			
			/* 清除缓存 */
			memset(tempdataStr,0x00, sizeof(tempdataStr));
			/* 将温度数据转换成字符串 */
			float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
			/* 设置logs */
			sprintf(buf,HTTPBody_Logs_Temp,
			HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
			tempdataStr);

		}else if(Sensor_RF_Card_Single_Tmp == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* 数据(UCOS-II使用sprintf存在字节对齐问题) */
			char tempdataStr[16];
				
			/* 清除缓存 */
			memset(tempdataStr,0x00, sizeof(tempdataStr));
			/* 判断是否接收到数据 */
			if(Wireless_RF_Card_Data_Rece_Timeout_Para != HTTPPara.ApiPara.Logs[probeNum].Temp)
			{
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* 设置logs */
				sprintf(buf,HTTPBody_Logs_Temp,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr);
			}else
			{
				/* 未接收到数据，只发送探头编号，不发送数据。 */
				/* 设置logs */
				continue;
			}
		}else if(Sensor_Hum == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* 数据(UCOS-II使用sprintf存在字节对齐问题) */
			char humidataStr[16];
			
			
			/* 清除缓存 */
			memset(humidataStr,0x00, sizeof(humidataStr));
			/* 将温度数据转换成字符串 */
			float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
			/* 设置logs */
			sprintf(buf,HTTPBody_Logs_Humi,
			HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
			humidataStr);
		}else if(Sensor_RF_Card_Humiture_Tmp == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* 数据(UCOS-II使用sprintf存在字节对齐问题) */
			char tempdataStr[16];
			char humidataStr[16];
			
			
			/* 清除缓存 */
			memset(tempdataStr,0x00, sizeof(tempdataStr));
			memset(humidataStr,0x00, sizeof(humidataStr));
			/* 判断是否接收到数据 */
			if((Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* 未接收到数据，只发送探头编号，不发送数据。 */
				/* 设置logs */
				continue;
//				sprintf(buf,HTTPBody_Logs_Null,HTTPPara.ApiPara.Logs[probeNum].probeIdNo);
			}else if((Wireless_RF_Card_Data_Rece_Timeout_Para != HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* 只有温度数据 */
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* 设置logs */
				sprintf(buf,HTTPBody_Logs_Temp,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr);
			}else if((Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para != HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* 只有湿度度数据 */
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
				/* 设置logs */
				sprintf(buf,HTTPBody_Logs_Humi,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				humidataStr);
			}else
			{
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
				/* 设置logs */
				sprintf(buf,HTTPBody_Logs_TempHumi,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr,
				humidataStr);
			}
			
		}else if(Sensor_GW_NJSY_Tmp == HTTPPara.ApiPara.Logs[num].Type)
		{
			/* 数据(UCOS-II使用sprintf存在字节对齐问题) */
			char tempdataStr[16];
			char humidataStr[16];
			
			
			/* 清除缓存 */
			memset(tempdataStr,0x00, sizeof(tempdataStr));
			memset(humidataStr,0x00, sizeof(humidataStr));
			/* 判断是否接收到数据 */
			if((Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* 未接收到数据，只发送探头编号，不发送数据。 */
				/* 设置logs */
				continue;
//				sprintf(buf,HTTPBody_Logs_Null,HTTPPara.ApiPara.Logs[probeNum].probeIdNo);
			}else if((Gateway_NJSY_TimeoutPara != HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* 只有温度数据 */
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* 设置logs */
				sprintf(buf,HTTPBody_Logs_Temp,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr);
			}else if((Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara != HTTPPara.ApiPara.Logs[probeNum].Humi))
			{
				/* 只有湿度度数据 */
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
				/* 设置logs */
				sprintf(buf,HTTPBody_Logs_Humi,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				humidataStr);
			}else
			{
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Temp,tempdataStr);
				/* 将温度数据转换成字符串 */
				float_to_string(HTTPPara.ApiPara.Logs[probeNum].Humi,humidataStr);
				/* 设置logs */
				sprintf(buf,HTTPBody_Logs_TempHumi,
				HTTPPara.ApiPara.Logs[probeNum].probeIdNo,
				tempdataStr,
				humidataStr);
			}
		}else
		{
			continue;
		}
		/* 拷贝缓存数据 */
		strcat (destbuf,buf);
		/* 清除缓存 */
		memset(buf,0x00, sizeof(buf));
		
		/* 设置探头编号 */
		probeNum++;
		/* 设置Json连续符号 */
		if(probeNum < HTTPPara.ApiPara.ProbeNum)
		{
			/* 拷贝缓存数据 */
			strcat (destbuf,",");
		}
	}
	
	/* 设置logs结束标签 */
	sprintf(buf,HTTPBody_Logs_End);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置Body结束标签 */
	sprintf(buf,HTTPBody_End);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	return strlen(destbuf);
}

/************************************************************************************************/
/* 函数名	：HTTPSetState_Dody																				*/
/* 输入		：destbuf:数据缓存指针																				*/
/* 输出		：主体信息长度 																				*/
/* 作用		：创建主体信息					 									 		 		*/
/* 创建日期	：2015/09/20																		*/
/* 创建日期	：2015/09/20																		*/
/************************************************************************************************/
static u32 HTTPSetState_Dody(char * destbuf)
{
	/* 缓存 */
	char buf[256];
//	/* 探头编号 */
//	unsigned char probeNum;
//	/* 计数器 */
//	unsigned char num;


	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置Body开始标签 */
	sprintf(buf,HTTPBody_Start);
	/* 拷贝缓存数据 */
	strcpy(destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置网关编号 */
	sprintf(buf,	HTTPBody_ProbeIdNo,HTTPPara.ApiState.GatewayIdNo);
	/* 拷贝缓存数据 */
	strcat(destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置时间戳 */
	sprintf(buf,HTTPBody_Timestamp,HTTPPara.ApiState.TimeStamp);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置参数签名 */
	sprintf(buf,"gatewayIdNo=%stimestamp=%dapiKey=%s",
			HTTPPara.ApiState.GatewayIdNo,
			HTTPPara.ApiState.TimeStamp,
			HTTP_USER_KEY);
	/* 进行MD5运算 */
	App_MD5(HTTPPara.ApiState.Sign,buf);
	/* 清除参数签名缓存 */
	memset(buf,	 0x00, sizeof(buf));
	/* 保存参数签名 */
	sprintf(buf,HTTPBody_Sign,HTTPPara.ApiState.Sign);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置"电源状态"参数 */
	sprintf(buf,HTTPBody_PowerState,(HTTPPara.ApiState.PowerState==1)?1:2);
	/* 拷贝缓存数据 */
	strcat(destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置"网关电量"参数 */
	sprintf(buf,HTTPBody_BatteryLevel,HTTPPara.ApiState.BatteryLevel);
	/* 拷贝缓存数据 */
	strcat(destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	/* 设置Body结束标签 */
	sprintf(buf,HTTPBody_End);
	/* 拷贝缓存数据 */
	strcat (destbuf,buf);
	/* 清除缓存 */
	memset(buf,0x00, sizeof(buf));
	
	return strlen(destbuf);
}


/************************************************************************************************/
/* 函数名	：HTTPSet_Header																	*/
/* 输入		：destbuf:数据缓存指针																*/
/* 输出		：主体信息长度 																		*/
/* 作用		：创建主体信息					 									 		 		*/
/* 创建日期	：2015/06/23																		*/
/* 创建日期	：2015/06/23																		*/
/************************************************************************************************/
static void HTTPSet_Header(char * destbuf,u32 ContentLen)
{	
	/* 缓存 */
	char contentStr[256];
	
	/* 清除缓存 */
	memset(contentStr,0x00, sizeof(contentStr));
	
	/* 设置Body长度信息 */
	sprintf(contentStr,HTTPHeader_ContentLength,ContentLen);

	/* 设置首部字段内容 */
	sprintf(destbuf,"%s%s%s%s%s",
			HTTPHeader_Host,
			contentStr,
			HTTPHeader_ContentType,
			HTTPHeader_Connection,
			HTTPHeader_End);
}

/************************************************************************************************/
/* 函数名	：HTTP_Create_Data																	*/
/* 输入		：无																					*/
/* 输出		：无 																				*/
/* 作用		：创建HTTP数据					 									 		 		*/
/* 创建日期	：2015/06/03																		*/
/* 创建日期	：2015/06/03																		*/
/************************************************************************************************/
void HTTP_Create_Data(char * destbuf)
{
	/* 起始行 */
	char requestLine[128];
	/* 首部字段 */
	char headers[128];
	/* 主体 */
	char bodyJson[1280];
	/* 主体信息长度 */
	u32  bodyLen = 0;
	
	
	/* 清除缓存 */
	memset(requestLine,0x00, sizeof(requestLine));
	memset(headers,0x00, sizeof(headers));
	memset(bodyJson,0x00, sizeof(bodyJson));
	
	/* 设置起始行内容 */
	sprintf(requestLine,"%s %s %s\r\n",
			HTTPRequestLine_Method,
			HTTPRequestLine_Path,
			HTTPRequestLine_Verison);
	
	/* 设置主体内容 */
	bodyLen = HTTPSet_Dody(bodyJson);
	
	/* 设置首部字段内容 */
	HTTPSet_Header(headers,bodyLen);
	
	/* HTTP 数据 */
	sprintf(destbuf,"%s%s%s",
			requestLine,
			headers,
			bodyJson);
}


/************************************************************************************************/
/* 函数名	：HTTP_Create_State																	*/
/* 输入		：destbuf:数据缓存																					*/
/* 输出		：无 																				*/
/* 作用		：创建HTTP数据					 									 		 		*/
/* 创建日期	：2015/09/20																		*/
/* 创建日期	：2015/09/20																		*/
/************************************************************************************************/
void HTTP_Create_State(char * destbuf)
{
	/* 起始行 */
	char requestLine[128];
	/* 首部字段 */
	char headers[128];
	/* 主体 */
	char bodyJson[512];
	/* 主体信息长度 */
	u32  bodyLen = 0;
	
	
	/* 清除缓存 */
	memset(requestLine,0x00, sizeof(requestLine));
	memset(headers,0x00, sizeof(headers));
	memset(bodyJson,0x00, sizeof(bodyJson));
	
	/* 设置起始行内容 */
	sprintf(requestLine,"%s %s %s\r\n",
			HTTPRequestLine_Method,
			HTTPRequestLine_Path_State,
			HTTPRequestLine_Verison);
			
	/* 设置主体内容 */
	bodyLen = HTTPSetState_Dody(bodyJson);
	
	/* 设置首部字段内容 */
	HTTPSet_Header(headers,bodyLen);
	
	/* HTTP 数据 */
	sprintf(destbuf,"%s%s%s",
			requestLine,
			headers,
			bodyJson);

}


/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
