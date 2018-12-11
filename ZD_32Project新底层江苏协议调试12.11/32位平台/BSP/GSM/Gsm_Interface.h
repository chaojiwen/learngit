#ifndef GSM_INTERFACE_H
#define GSM_INTERFACE_H



// GPRS ID
// 系统内置的gprs服务器地址(GSM模块支持最多3个GPRS并发连接)
#define GSM_MAX_NUM_TCP_CONN					4

// ip地址的最大长度，如"128.128.128.128"
#define GSM_MAX_LEN_GPRS_IP						20
// 域名的最大长度，如"www.google.com"
#define GSM_MAX_LEN_GPRS_DN						32
// 端口的最大长度，如"8080"
#define GSM_MAX_LEN_GPRS_PORT					8

// 系统内置的gprs服务器地址(GSM模块支持最多6个GPRS并发连接)//此时模块必须处于多链路模式
//#define MAX_NUM_TCP_CONN					    6

//各个服务器序号
#define	GSM_GPRS_Data							    0  //数据服务器
#define	GSM_GPRS_Config							  1  //配置服务器
#define	GSM_GPRS_Update							  2  //升级服务器
//......后续如果有其他的服务器可以再次添加                                      



typedef struct
{
	char	dn[GSM_MAX_LEN_GPRS_DN + 1];			// Domain Name
	char	ip[GSM_MAX_LEN_GPRS_IP + 1];			// IP Address
	char	port[GSM_MAX_LEN_GPRS_PORT + 1];		// Port
}GSM_GPRS_Connection;


extern GSM_GPRS_Connection    GSM_Tcp_Conn[GSM_MAX_NUM_TCP_CONN];// 设备要访问的tcp服务器参数数组





#endif /*GSM_INTERFACE_H*/
