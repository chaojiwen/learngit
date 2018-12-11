#ifndef _GSM_MG2639_H   
#define _GSM_MG2639_H	
#include "Gsm_Api.h"
#if (GSM_Module_Type==3)//����MG2639ģ�� 

#include <stdbool.h>
#include "GSM_USART.h"
#include "stm32f10x.h"

#define   ON  1  //��
#define   OFF 0  //��

//GSMģ���Դ���ƽ�
#define   GSM_MG2639_PWRCTRL_PORT          GPIOB
#define   GSM_MG2639_PWRCTRL_CLK           RCC_APB2Periph_GPIOB
#define   GSM_MG2639_PWRCTRL_PIN           GPIO_Pin_1 
//GSMģ�鿪�����ƽ�
#define   GSM_MG2639_ONOFF_PORT            GPIOB
#define   GSM_MG2639_ONOFF_CLK             RCC_APB2Periph_GPIOB
#define   GSM_MG2639_ONOFF_PIN             GPIO_Pin_2 
 
/*GSMģ���Դ���ƽ�������ƺ���*/		
#define   GSM_MG2639_Power(a)		if (a)	\
					GPIO_SetBits(GSM_MG2639_PWRCTRL_PORT,GSM_MG2639_PWRCTRL_PIN); \
					else		\
					GPIO_ResetBits(GSM_MG2639_PWRCTRL_PORT,GSM_MG2639_PWRCTRL_PIN) 
					
					
/*GSMģ�鿪�����ƽ�������ƺ���*/		
#define   GSM_MG2639_OnOff(a)		if (a)	\
					GPIO_SetBits(GSM_MG2639_ONOFF_PORT,GSM_MG2639_ONOFF_PIN); \
					else		\
					GPIO_ResetBits(GSM_MG2639_ONOFF_PORT,GSM_MG2639_ONOFF_PIN) 

// ģ����Ӧ��ָͨ��ȴ���ʱ�䵥λΪ:10ms
#define   Ordinary_AT_WaitTime_n10ms					50	
					
// ģ����Ӧ��ָͨ��ȴ���ʱ�䵥λΪ:10ms
#define   Mg2639_Open_GPRSdatalink_n10ms			  500						

//ָ��ȴ���ʱ�䵥λΪ:10ms �򿪷���������
#define   Open_Server_AT_WaitTime_n10ms		    6000

//ָ��ȴ���ʱ�䵥λΪ:10ms �رշ���������
#define   Close_Server_AT_WaitTime_n10ms	    500	
//ָ��ȴ���ʱ�䵥λΪ:10ms ���ͷ���������
#define   SendData_Server_AT_WaitTime_n10ms		6000	

//ָ��ȴ���ʱ�䵥λΪ:10ms ���Ͷ���
#define   Send_Sms_AT_WaitTime_n10ms	        100					
		
void     GSM_MG2639_Io_Init(void);//GSMģ�����IO�ڳ�ʼ��					
void     GSM_MG2639_Power_Up(void);//��GSMģ���Դ					
void     GSM_MG2639_Power_Down(void);//�ر�GSMģ���Դ
bool     GSM_MG2639_Power_Rest(void);//����GSMģ��
u8       GSM_MG2639_send_data_Status(char *wbuf, int wlen,char *ack,u16 waittime,u8 *UsartBuf);//��������Ƿ��ͳɹ�
u8       GSM_MG2639_send_data(uint8_t ServerID,char *wbuf, int wlen,u16 waittime,uint8_t *UartBuf);//�������ݺ���
bool     GSM_MG2639_AskCSQ(uint8_t *csq, uint8_t *UartBuf);//��ȡģ���ź�ǿ��
u8       GSM_MG2639_Open_Connect_Srever(uint8_t ServerID,char *ServerName,char *ServerPort,u16 waittime,uint8_t *UartBuf);//��TCP����������
bool     GSM_MG2639_Close_Connect_Srever(uint8_t *UartBuf);//�رշ���������
bool     GSM_MG2639_check_cmd(char *str,u8 *UsartBuf);//���Gsmģ�鷵�ص����������Ƿ���ȷ
bool     GSM_MG2639_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf);//��Gsmģ�鷢��ָ��������,���ڹ涨��ʱ���ڼ���Ƿ���Ҫ���ص�Ŀ������
u8       GSM_MG2639_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf);//��������;//��������
bool     GSM_MG2639_Check_SIm_Card(uint8_t *UartBuf);//���SIM�Ƿ���λ
bool     GSM_MG2639_CheckNetwork(uint8_t *UartBuf);//���SIM�Ƿ��Ѿ�ע��������·
bool     GSM_MG2639_CheckGPRSActive(uint8_t *UartBuf);//���SIM�Ƿ��Ѿ�����������·
bool     GSM_MG2639_Echo(bool flag,u8 *UsartBuf);//�Ƿ�����GSM���� flag:true������� flag:false��������
bool     GSM_MG2639_Send_AT(u8 *UsartBuf);//��ģ�鷢��"AT"
bool     GSM_MG2639_Send_Sms(char *num,char *smstext,uint8_t *UartBuf);//���Ͷ�������
bool     GSM_MG2639_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf);//���Ͳ�Ĭ�ϴ��س�����ATָ��
int	     GSM_MG2639_Adaptive_BaudRate(u8 *UartBuf);  /* ����Ӧ������ */
bool	   GSM_MG2639_SetBaudRate(int br,u8 *UartBuf);//����ģ�鲨����;//����ģ�鲨����
bool     GSM_MG2639_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf);//��ģ�鷢��ATָ��
#endif
#endif  /*_GSM_MG2639_H*/
