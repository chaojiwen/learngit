#ifndef _GSM_H   
#define _GSM_H	

#include <stdbool.h>
#include "GSM_USART.h"
#include "stm32f10x.h"

#define   ON  1  //��
#define   OFF 0  //��

//GSMģ���Դ���ƽ�
#define   GSM_PWRCTRL_PORT          GPIOB
#define   GSM_PWRCTRL_CLK           RCC_APB2Periph_GPIOB
#define   GSM_PWRCTRL_PIN           GPIO_Pin_1 
//GSMģ�鿪�����ƽ�
#define   GSM_ONOFF_PORT            GPIOB
#define   GSM_ONOFF_CLK             RCC_APB2Periph_GPIOB
#define   GSM_ONOFF_PIN             GPIO_Pin_2 
 
/*GSMģ���Դ���ƽ�������ƺ���*/		
#define   GSM_Power(a)		if (a)	\
					GPIO_SetBits(GSM_PWRCTRL_PORT,GSM_PWRCTRL_PIN); \
					else		\
					GPIO_ResetBits(GSM_PWRCTRL_PORT,GSM_PWRCTRL_PIN) 
					
					
/*GSMģ�鿪�����ƽ�������ƺ���*/		
#define   GSM_OnOff(a)		if (a)	\
					GPIO_SetBits(GSM_ONOFF_PORT,GSM_ONOFF_PIN); \
					else		\
					GPIO_ResetBits(GSM_ONOFF_PORT,GSM_ONOFF_PIN) 

// ģ����Ӧ��ָͨ��ȴ���ʱ�䵥λΪ:10ms
#define   Ordinary_AT_WaitTime_n10ms					50	

//ָ��ȴ���ʱ�䵥λΪ:10ms �򿪷���������
#define   Open_Server_AT_WaitTime_n10ms		    6000

//ָ��ȴ���ʱ�䵥λΪ:10ms �رշ���������
#define   Close_Server_AT_WaitTime_n10ms	    500	
//ָ��ȴ���ʱ�䵥λΪ:10ms ���ͷ���������
#define   SendData_Server_AT_WaitTime_n10ms		6000	

//ָ��ȴ���ʱ�䵥λΪ:10ms ���Ͷ���
#define   Send_Sms_AT_WaitTime_n10ms	        100					

void     Gsm_Io_Init(void);//GSMģ�����IO�ڳ�ʼ��					
void     Gsm_Power_Up(void);//��GSMģ���Դ					
void     Gsm_Power_Down(void);//�ر�GSMģ���Դ
bool     Gsm_Power_Rest(void);//����GSMģ��
u8       Gsm_send_data_Status(char *wbuf, int wlen,char *ack,u16 waittime,u8 *UsartBuf);//��������Ƿ��ͳ�?
u8       Gsm_send_data(char *wbuf, int wlen,u16 waittime,uint8_t *UartBuf);//�������ݺ���
bool     Gsm_AskCSQ(uint8_t *csq, uint8_t *UartBuf);//��ȡģ���ź�ǿ��
u8       Gsm_Open_Connect_Srever(char *ServerName,char *ServerPort,u16 waittime,uint8_t *UartBuf);//��TCP����������
bool     Gsm_Close_Connect_Srever(uint8_t *UartBuf);//�رշ���������
bool     Gsm_check_cmd(char *str,u8 *UsartBuf);//���Gsmģ�鷵�ص����������Ƿ���ȷ
bool     Gsm_send_cmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf);//��Gsmģ�鷢��ָ��������,���ڹ涨��ʱ���ڼ���Ƿ���Ҫ���ص�Ŀ������
u8       Gsm_Rev_data(char *rbuf, int *rlen,u16 waittime,uint8_t *UartBuf);//��������;//��������
bool     Gsm_Check_SIm_Card(uint8_t *UartBuf);//���SIM�Ƿ���λ
bool     Gsm_CheckNetwork(uint8_t *UartBuf);//���SIM�Ƿ��Ѿ�ע��������·
bool     Gsm_CheckGPRSActive(uint8_t *UartBuf);//���SIM�Ƿ��Ѿ�����������·
bool     Gsm_Echo(bool flag,u8 *UsartBuf);//�Ƿ�����GSM���� flag:true������� flag:false��������
bool     Gsm_Send_AT(u8 *UsartBuf);//��ģ�鷢��"AT"
bool     Gsm_Send_Sms(char *num,char *smstext,uint8_t *UartBuf);//���Ͷ�������
bool     Gsm_send_no_entercmd(char *cmd,char *ack,u16 waittime,u8 *UsartBuf);//���Ͳ�Ĭ�ϴ��س�����ATָ��
#endif  /*_GSM_H_*/


























