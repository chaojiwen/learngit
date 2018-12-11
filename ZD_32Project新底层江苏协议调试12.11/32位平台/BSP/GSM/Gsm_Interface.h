#ifndef GSM_INTERFACE_H
#define GSM_INTERFACE_H



// GPRS ID
// ϵͳ���õ�gprs��������ַ(GSMģ��֧�����3��GPRS��������)
#define GSM_MAX_NUM_TCP_CONN					4

// ip��ַ����󳤶ȣ���"128.128.128.128"
#define GSM_MAX_LEN_GPRS_IP						20
// ��������󳤶ȣ���"www.google.com"
#define GSM_MAX_LEN_GPRS_DN						32
// �˿ڵ���󳤶ȣ���"8080"
#define GSM_MAX_LEN_GPRS_PORT					8

// ϵͳ���õ�gprs��������ַ(GSMģ��֧�����6��GPRS��������)//��ʱģ����봦�ڶ���·ģʽ
//#define MAX_NUM_TCP_CONN					    6

//�������������
#define	GSM_GPRS_Data							    0  //���ݷ�����
#define	GSM_GPRS_Config							  1  //���÷�����
#define	GSM_GPRS_Update							  2  //����������
//......��������������ķ����������ٴ����                                      



typedef struct
{
	char	dn[GSM_MAX_LEN_GPRS_DN + 1];			// Domain Name
	char	ip[GSM_MAX_LEN_GPRS_IP + 1];			// IP Address
	char	port[GSM_MAX_LEN_GPRS_PORT + 1];		// Port
}GSM_GPRS_Connection;


extern GSM_GPRS_Connection    GSM_Tcp_Conn[GSM_MAX_NUM_TCP_CONN];// �豸Ҫ���ʵ�tcp��������������





#endif /*GSM_INTERFACE_H*/
