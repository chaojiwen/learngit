#ifndef __Log_HTTP_H
#define __Log_HTTP_H

#include "stm32f10x.h"

#include "Log_HTTP.h"


/************************************************************************************************/
//Define
	/* 起始行 */
	/* 方式 */
	#define HTTPRequestLine_Method    	"POST"
	/* 路径源 */
	#define HTTPRequestLine_Path		"/upload.do"
	/* 路径源 */
	#define HTTPRequestLine_Path_State	"/statusUpload.do"
	/* HTTP版本 */
	#define HTTPRequestLine_Verison		"HTTP/1.1"
	
	
	/* 首部字段 */
	/* 主机头域 */
	#define HTTPHeader_Host				"Host: coldchain.jwx.com.cn\r\n"
	/* 用户密钥 */
    #define HTTPHeader_UserKey 			"nop\r\n"
	/* 响应主体长度 */
	#define HTTPHeader_ContentLength	"Content-Length:%d\r\n" 
	/* MIME 类型 */
	#define HTTPHeader_ContentType		"Content-Type: application/x-www-form-urlencoded\r\n"
	/* 连接处理 */
	#define HTTPHeader_Connection		"Connection: Close\r\n"
	/* 首部结束标志 */
	#define HTTPHeader_End				"\r\n"
	
//	/* 代理用户 */
//    #define HTTP_Headers_Agent	"User-Agent: ZJUEE"


	/* 主体成员 */
	/* Body标签 */
	#define HTTPBody_Start			"{"
	#define HTTPBody_End			"}"
	/* 网关编号 */
	#define HTTPBody_ProbeIdNo		"\"gatewayIdNo\":\"%s\","	
	/* 时间戳 */
	#define HTTPBody_Timestamp		"\"timestamp\":\"%d\","
	/* 数字签名 */
	#define HTTPBody_Sign			"\"sign\":\"%s\","
	/* 经度参数 */
	#define HTTPBody_Lng			"\"lng\":\"%s\","
	/* 经度参数 */
	#define HTTPBody_Lat			"\"lat\":\"%s\","
	/* 采集时间 */
	#define HTTPBody_CollectTime	"\"collectTime\":\"%s\","
	/* logs标签 */
	#define HTTPBody_Logs_Start		"\"logs\":["
	#define HTTPBody_Logs_End		"]"
	/* Logs参数-无数据 */
	#define HTTPBody_Logs_Null		"{\"probeIdNo\":\"%s\"}"
	/* Logs参数-单温 */
	#define HTTPBody_Logs_Temp		"{\"probeIdNo\":\"%s\",\"temperature\":%s}"
	/* Logs参数-单湿 */ 
	#define HTTPBody_Logs_Humi		"{\"probeIdNo\":\"%s\",\"humidity\":%s}"
	/* Logs参数-温湿 */ 
	#define HTTPBody_Logs_TempHumi	"{\"probeIdNo\":\"%s\",\"temperature\":%s,\"humidity\":%s}"
	
	
	/* ApiKey值(这个值是由金卫信提供) */
	#define HTTP_USER_KEY			"ipXObFhhnhSEEeGW2koaAvG4x4Tmywwd"	
	
	
	//状态
	/* 网关电源状态 */
	#define HTTPBody_PowerState		"\"powerStatus\":\"%d\","
	/* 网关电量状态 */
	#define HTTPBody_BatteryLevel	"\"batteryLevel\":\"%d\""

/************************************************************************************************/

/* HTTP POST 主体参数据接口Logs */
typedef struct {

	/* 探头编号 */
	char	probeIdNo[16];
	/* 探头类型 */
	unsigned char	Type;
	/* 温度数据 */
	float	Temp;
	/* 湿度数据 */
	float	Humi;
}HTTPBodyLogs;

/* HTTP接口参数结构 */
typedef struct {
	/* 仪器GatewayIdNo号 */
	char GatewayIdNo[12];
	/* 参数签名时间 */
	int	TimeStamp;
	/* 参数签名 */
	char Sign[36];

	/* GPS设备状态 */
	BOOL GPS_State;
	/* 经度值 */
	float Lng;
	/* 纬度值 */
	float Lat;

	/* 数据采集时间,格式:yyyy-MM-dd%20HH:mm:ss，比如2016-05-30 11:48:00 */
	char CollectTime[24];
	
	/* 探头数量 */
	unsigned char ProbeNum;
	/* logs数据信息 */
	HTTPBodyLogs  Logs[32];
	
}HTTPApiParaStruct;

/* HTTP状态接口结构 */
typedef struct {
	/* 仪器GatewayIdNo号 */
	char GatewayIdNo[12];
	/* 参数签名时间 */
	int	TimeStamp;
	/* 参数签名 */
	char Sign[36];

	/* 网关电源状态 */
	int	PowerState;
	/* 网关电池电量 */
	int	BatteryLevel;
	
}HTTPApiStateStruct;

/* 创建HTTP参数数据结构 */
typedef struct {
	/* HTTP接口参数结构 */
	HTTPApiParaStruct ApiPara;
	HTTPApiStateStruct ApiState;
	
}CreateHTTPParaStruct;






/************************************************************************************************/
//extern 



/************************************************************************************************/
/* 函数声明 */
void HTTP_Create_Data(char * databuf);
void HTTP_Create_State(char * databuf);



/************************************************************************************************/
#endif /* Log_GSM_H */

