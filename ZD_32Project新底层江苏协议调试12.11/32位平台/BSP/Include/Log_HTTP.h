#ifndef __Log_HTTP_H
#define __Log_HTTP_H

#include "stm32f10x.h"

#include "Log_HTTP.h"


/************************************************************************************************/
//Define
	/* ��ʼ�� */
	/* ��ʽ */
	#define HTTPRequestLine_Method    	"POST"
	/* ·��Դ */
	#define HTTPRequestLine_Path		"/upload.do"
	/* ·��Դ */
	#define HTTPRequestLine_Path_State	"/statusUpload.do"
	/* HTTP�汾 */
	#define HTTPRequestLine_Verison		"HTTP/1.1"
	
	
	/* �ײ��ֶ� */
	/* ����ͷ�� */
	#define HTTPHeader_Host				"Host: coldchain.jwx.com.cn\r\n"
	/* �û���Կ */
    #define HTTPHeader_UserKey 			"nop\r\n"
	/* ��Ӧ���峤�� */
	#define HTTPHeader_ContentLength	"Content-Length:%d\r\n" 
	/* MIME ���� */
	#define HTTPHeader_ContentType		"Content-Type: application/x-www-form-urlencoded\r\n"
	/* ���Ӵ��� */
	#define HTTPHeader_Connection		"Connection: Close\r\n"
	/* �ײ�������־ */
	#define HTTPHeader_End				"\r\n"
	
//	/* �����û� */
//    #define HTTP_Headers_Agent	"User-Agent: ZJUEE"


	/* �����Ա */
	/* Body��ǩ */
	#define HTTPBody_Start			"{"
	#define HTTPBody_End			"}"
	/* ���ر�� */
	#define HTTPBody_ProbeIdNo		"\"gatewayIdNo\":\"%s\","	
	/* ʱ��� */
	#define HTTPBody_Timestamp		"\"timestamp\":\"%d\","
	/* ����ǩ�� */
	#define HTTPBody_Sign			"\"sign\":\"%s\","
	/* ���Ȳ��� */
	#define HTTPBody_Lng			"\"lng\":\"%s\","
	/* ���Ȳ��� */
	#define HTTPBody_Lat			"\"lat\":\"%s\","
	/* �ɼ�ʱ�� */
	#define HTTPBody_CollectTime	"\"collectTime\":\"%s\","
	/* logs��ǩ */
	#define HTTPBody_Logs_Start		"\"logs\":["
	#define HTTPBody_Logs_End		"]"
	/* Logs����-������ */
	#define HTTPBody_Logs_Null		"{\"probeIdNo\":\"%s\"}"
	/* Logs����-���� */
	#define HTTPBody_Logs_Temp		"{\"probeIdNo\":\"%s\",\"temperature\":%s}"
	/* Logs����-��ʪ */ 
	#define HTTPBody_Logs_Humi		"{\"probeIdNo\":\"%s\",\"humidity\":%s}"
	/* Logs����-��ʪ */ 
	#define HTTPBody_Logs_TempHumi	"{\"probeIdNo\":\"%s\",\"temperature\":%s,\"humidity\":%s}"
	
	
	/* ApiKeyֵ(���ֵ���ɽ������ṩ) */
	#define HTTP_USER_KEY			"ipXObFhhnhSEEeGW2koaAvG4x4Tmywwd"	
	
	
	//״̬
	/* ���ص�Դ״̬ */
	#define HTTPBody_PowerState		"\"powerStatus\":\"%d\","
	/* ���ص���״̬ */
	#define HTTPBody_BatteryLevel	"\"batteryLevel\":\"%d\""

/************************************************************************************************/

/* HTTP POST ��������ݽӿ�Logs */
typedef struct {

	/* ̽ͷ��� */
	char	probeIdNo[16];
	/* ̽ͷ���� */
	unsigned char	Type;
	/* �¶����� */
	float	Temp;
	/* ʪ������ */
	float	Humi;
}HTTPBodyLogs;

/* HTTP�ӿڲ����ṹ */
typedef struct {
	/* ����GatewayIdNo�� */
	char GatewayIdNo[12];
	/* ����ǩ��ʱ�� */
	int	TimeStamp;
	/* ����ǩ�� */
	char Sign[36];

	/* GPS�豸״̬ */
	BOOL GPS_State;
	/* ����ֵ */
	float Lng;
	/* γ��ֵ */
	float Lat;

	/* ���ݲɼ�ʱ��,��ʽ:yyyy-MM-dd%20HH:mm:ss������2016-05-30 11:48:00 */
	char CollectTime[24];
	
	/* ̽ͷ���� */
	unsigned char ProbeNum;
	/* logs������Ϣ */
	HTTPBodyLogs  Logs[32];
	
}HTTPApiParaStruct;

/* HTTP״̬�ӿڽṹ */
typedef struct {
	/* ����GatewayIdNo�� */
	char GatewayIdNo[12];
	/* ����ǩ��ʱ�� */
	int	TimeStamp;
	/* ����ǩ�� */
	char Sign[36];

	/* ���ص�Դ״̬ */
	int	PowerState;
	/* ���ص�ص��� */
	int	BatteryLevel;
	
}HTTPApiStateStruct;

/* ����HTTP�������ݽṹ */
typedef struct {
	/* HTTP�ӿڲ����ṹ */
	HTTPApiParaStruct ApiPara;
	HTTPApiStateStruct ApiState;
	
}CreateHTTPParaStruct;






/************************************************************************************************/
//extern 



/************************************************************************************************/
/* �������� */
void HTTP_Create_Data(char * databuf);
void HTTP_Create_State(char * databuf);



/************************************************************************************************/
#endif /* Log_GSM_H */

