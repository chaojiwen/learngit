#ifndef __Dev_RTC_H
#define __Dev_RTC_H

#include "stm32f10x.h"

#include "Mcu_Config.h"

#include "Log_Time.h"




//#define RTC8025_FOE_PORT         GPIOC
//#define RTC8025_FOE_CLK          RCC_APB2Periph_GPIOC
//#define RTC8025_FOE_PIN          GPIO_Pin_10

#define MAX823_WDI_PORT         GPIOE
#define MAX823_WDI_CLK          RCC_APB2Periph_GPIOE
#define MAX823_WDI_PIN          GPIO_Pin_2

/* ���Ź�оƬMAX823 WDI �������� */
#define MAX823_WDI_High  	{GPIO_SetBits(MAX823_WDI_PORT,MAX823_WDI_PIN);}
#define MAX823_WDI_Low 		{GPIO_ResetBits(MAX823_WDI_PORT,MAX823_WDI_PIN);}


/* ϵͳʱ��У׼״̬�Ĵ���(FALSE:δУ׼;TRUE:��У׼) */
extern BOOL RTC_System_Clock_Calibration_State_Reg;//2018.12.04  By  ZWC

/* �·ݶ�Ӧ�������� */
extern u16 month_days[12];	

u8 BCD_To_HEX(u8 bcd_data);    //BCDתΪHEX�ӳ���   
u8 HEX_To_BCD(u8 hex_data);

void RTC_WakeUp_PA0_Config(void);
BOOL RTC_Init(BOOL reset_state);
void RTC_Second_To_Date_Time(u32 tim,Times * tm);
u32  RTC_Date_Time_To_Second(Times *tm);

void MAX823_WDI_GPIO_Config(void);
void Read_System_Date_Time_Data(Times* time);
void Write_System_Date_Time_Data(Times* time);
void Compression_Times_Data(Times* Tm,INSTRU_CONFIG_INF_TAB* tab);

/* ���ϵͳ������ʱ���Ƿ�У׼���� */
void RTC_System_Clock_Calibration_State_Check(void);
/* У׼ϵͳʱ�ӡ� */
void RTC_System_Clock_Calibration(Times system_time);
/* ��λϵͳʱ��У׼״̬�Ĵ��� */
void RTC_System_Clock_Calibration_State_Reset(void);
#endif


/******************** (C) COPYRIGHT 2009 www.armjishu.com ***********************/



