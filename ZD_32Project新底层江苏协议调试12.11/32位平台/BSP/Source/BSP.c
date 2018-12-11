/************************************************************************************************/
/** @file              : BSP.c																		*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																	*/
/** @date              : 2015��6��17��														*/
/** @brief             : c file																	*/
/** @description       : �弶����																*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "BSP.h"
#include "Mcu_ADC.h"
#include "Mcu_Fram.h"
#include "Mcu_led.h"
#include "Mcu_Power.h"
#include "Mcu_RTC.h"
#include "Mcu_SPI.h"
#include "Mcu_Usart.h"
#include "Dev_RTC.h"
#include "Log_Adc1.h"
#include "Log_Alarm.h"
#include "Log_ConfigService.h"
#include "Log_Download.h"
#include "Log_INF.h"
#include "Log_RF_Card.h"
#include "App_Save.h"
#include "Log_Sensor.h"
#include "Log_Time.h"
#include "Mcu_Beep.h"
#include "Mcu_button.h"
#include "Ks0108.h"
#include "Menu.h"
#include "Psl_AI2C.h"
#include "Sve_Cfg_Tab.h"
#include "Sve_Fix_Cfg_Tab.h"
#include <string.h>
/************************************************************************************************/
//����sprintf��Keil+Ucos-IIʹ��%f�����ֽڶ������⣬�����Ҫ�ֽ�floatͨ����������ת���ַ�����ʽ��
//�ٽ���������ͨ��sprintf���ϳɡ�
static int n_tu(int number, int count)
{
	int result=1;
	
	
	while(count-- > 0)
	result *=number;  

	return result;
}

/***Convert float to string***/
void float_to_string(float f, char r[])
{
	long long int length, length2, i, number, position, sign;
	float number2;
	
	sign=-1;   // -1 == positive number
	if (f <0)
	{
		sign='-';
		f *= -1;    
	}   


	number2=f;  
	number=f;
	length=0;  // size of decimal part
	length2=0; //  size of tenth


	/* calculate length2 decimal part */
	while( (number2 - (float)number) != 0.0 && !((number2 - (float)number) < 0.0) )
	{
		number2= f * (n_tu(10.0,length2+1));
		number=number2; 

		length2++;
	}

	/* calculate length integer part*/
	for(length=(f >= 1) ? 0 : 1; f >= 1; length++) 
		f /= 10;

	if(length2 != 0)
	{
		position=length;
		length=length+1+length2;
	}else
	{
		/* û��С�������������һ�������ڵ�ֵ��ȥ��С���� */
		position = length+2;
	}
	
	 number=number2;
	 if(sign=='-')
	 {
		 length++;
		 position++;
	 }

	 for(i=length; i >= 0 ; i--)
	 {
		if(i== (length))
		  r[i]='\0';
		else if(i==(position))
		  r[i]='.';
		else if(sign=='-' && i==0)
		  r[i]='-';
		else  
		{
		  r[i]= (number % 10)+'0';
		  number /=10;
		}
	}
}

/************************************************************************************************/
/* ������	: BSP_Init																			*/
/* ����  	: ��																				*/
/* ���		: �� 																				*/
/* ����	 	: �弶����(�漰һЩ ����� ��ص�����)													*/
/* �������� 	: 2014/09/11																		*/
/* �޸����� 	: 2015/11/11																		*/
/************************************************************************************************/
void BSP_Init(void)
{    

	/* LED ��ʼ�� */
	LED_GPIO_Init();

	/* USART1 ��ʼ�� */
	USART1_Init();
	
	/* ADC1 ��ʼ�� */
	ADC1_Init();
	
#if (Wireless_RF_Card_Config_Enable == 0x01)
	/* ����RF��Ƭ ��ʼ�� */
	RF_Card_Init();
#endif
	/* SPI1 ��ʼ�� */ //��ʵ SPI1 ������ ����FLASH �еģ����Ӧ��д��SPI1������㵱�� 
	Mcu_SPI1_Init();

	#ifndef Hardware_Version_V131
	/* ��ʼ��Fram WP д����GPIO */
	McuFramInit();
	#endif
	/* ��ʼ���洢̽ͷ���ݹ��ܽӿ� */
	AppSave_InitAPI();
	/* ��ʼ��ͨ�ſ���ʹ������OE */
	McuRTCInit();
	/* ��ʼ����ס(���I2C����IOģ���) */
	Psl_AI2C_Bus_Unlock();
	/* ���� OLED ��ʼ��  */	//�����������д�ǲ�����ģ�������ʱ��ɸ��� 
	GUI_Init();
	/* Beep ��ʼ�� */
	Beep_Init();
	/* ��Դ��� ��ʼ�� */
	Power_Check_Init();
	/* ���� ��ʼ�� */
	Button_Init();
}

/************************************************************************************************/
/* ������	: Serial_Number_Init																*/
/* ����		: ��������SN��																		*/
/* ���		: �� 																				*/
/* ����		: ���� ������SN��																		*/
/* ��������	: 2015/06/01																		*/
/* ��������	: 2015/11/12																		*/
/************************************************************************************************/
void Serial_Number_Init(SERIAL_NUMBER *serial_number)
{
	
	/* �ж� ������� �Ƿ�һ�� */
	if((serial_number->Serial_Number[0] != (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+0)))||
	   (serial_number->Serial_Number[1] != (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+1)))||
	   (serial_number->Serial_Number[2] != (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)))||
	   (serial_number->Serial_Number[3] != (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+3))))
	{
		/* �жϵ�ǰ����SN�������. */
		if(((*(u32*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START))) == 0XFFFFFFFF)
		{
			/* MCU���ڲ�FLASH�洢�ռ��е�SNû�����ù� */
			
			/* �жϵ�ǰ������ ��������д���� ���� ���״���д���� */
			if((serial_number->Serial_Number[0] == 0X00) &&
			   (serial_number->Serial_Number[1] == 0X00) &&
			   (serial_number->Serial_Number[2] == 0X00) &&
			   (serial_number->Serial_Number[3] == 0X00))
			{
				/* �жϵ�ǰ������ ���״���д����,��Ҫ���ù��� ����SN��. */
				__nop();
			}else
			{
				/* �жϵ�ǰ������ ��������д����,���Ը������ñ��������SN�Զ�����. */
				Flash_Write_SN_NO((unsigned char*)serial_number->Serial_Number);
				
			}
			
		}else
		{
//////			/* MCU���ڲ�FLASH�洢�ռ��е�SN�������ñ��е�SN�Ų�һ�£���ʹ��MCU���ڲ�FLASH�洢�ռ��е�SN��. */
//////			serial_number->Serial_Number[0] = (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+0));
//////			serial_number->Serial_Number[1] = (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+1));
//////			serial_number->Serial_Number[2] = (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2));
//////			serial_number->Serial_Number[3] = (*(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+3));
			Flash_Write_SN_NO((unsigned char*)serial_number->Serial_Number);
			/* �޸� ���� ��Ϣ�� ���ñ�־ */
			Write_Config_Tab((u8*)serial_number,sizeof(SERIAL_NUMBER));
		}		
		
	}else
	{
		/* ����SN�������ñ��е�SNһ��. */
		__nop();
	}
	
	
	/* ������������ַ��� */
	sprintf((char*)Serial_Number,"%04X%05d",
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0xff00) >> 8)|
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START)) & 0x00ff) << 8)),
	((((*(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0xff00) >> 8)|
	((( *(u16*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START+2)) & 0x00ff) << 8)));
	
	
	
	
	
}

/************************************************************************************************/
/* ������	: Ste_Vsn_NO_Init																	*/
/* ����		: ����汾��																			*/
/* ���		: �� 																				*/
/* ����		: ���� ���SN��																		*/
/* ��������	: 2015/11/12																		*/
/* ��������	: 2015/11/12																		*/
/************************************************************************************************/
static void Ste_Vsn_NO_Init(VERSION *vsn_no)
{
	/* �ж�����汾���Ƿ�һ�� */
	if((vsn_no->Serial_Number_Code.Code_A != Software_Version_NO_Major)||
	   (vsn_no->Serial_Number_Code.Code_B != Software_Version_NO_Child)||
	   (vsn_no->Serial_Number_Code.Code_C != Software_Version_NO_Stage))
	{
		/* ���ݲ�һ�£���ʹ������Ĭ�ϵ�����汾��Ϣ */
		vsn_no->Serial_Number_Code.Code_A = Software_Version_NO_Major;
		vsn_no->Serial_Number_Code.Code_B = Software_Version_NO_Child;
		vsn_no->Serial_Number_Code.Code_C = Software_Version_NO_Stage;
		
		/* �޸� ���� ��Ϣ�� ���ñ�־ */
		Write_Config_Tab((u8*)&vsn_no,sizeof(*vsn_no));
	}

	/* ������������汾�ַ��� */
	sprintf((char*)Software_Version_CN,"����汾:%d.%d.%d",
	Software_Version_NO_Major,
	Software_Version_NO_Child,
	Software_Version_NO_Stage);
	
	/* ����Ӣ������汾�ַ��� */
	sprintf((char*)Software_Version_EN,"SV:%d.%d.%d",
	Software_Version_NO_Major,
	Software_Version_NO_Child,
	Software_Version_NO_Stage);
	
}

/************************************************************************************************/
/* ������	: Hde_Vsn_NO_Init																	*/
/* ����		: Ӳ���汾��																			*/
/* ���		: �� 																				*/
/* ����		: ���� Ӳ��SN��																		*/
/* ��������	: 2015/11/12																		*/
/* ��������	: 2015/11/12																		*/
/************************************************************************************************/
static void Hde_Vsn_NO_Init(VERSION *vsn_no)
{

	/* ��������Ӳ���汾�ַ��� */
	sprintf((char*)Hardware_Version_CN,"Ӳ���汾:%d.%d.%d",
	vsn_no->Serial_Number_Code.Code_A,
	vsn_no->Serial_Number_Code.Code_B,
	vsn_no->Serial_Number_Code.Code_C);

	/* ����Ӣ��Ӳ���汾�ַ��� */
	sprintf((char*)Hardware_Version_EN,"HV:%d.%d.%d",
	vsn_no->Serial_Number_Code.Code_A,
	vsn_no->Serial_Number_Code.Code_B,
	vsn_no->Serial_Number_Code.Code_C);

}

/************************************************************************************************/
/* ������	: System_Serial_Init																*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ϵͳ���кų�ʼ��																	*/
/* ��������	: 2015/11/17																		*/
/* ��������	: 2015/11/17																		*/
/************************************************************************************************/
void System_Serial_Init(void)
{

	/* ����SN�� ��ʼ�� */
	Serial_Number_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number);
	
	/* ��������汾�� ��ʼ�� */
	Ste_Vsn_NO_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Software_Version);
	
	/* ����Ӳ���汾�� ��ʼ�� */
	Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number_Code.Code_A=Hardware_Version_NO_Major;
	Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number_Code.Code_B=Hardware_Version_NO_Child;
	Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version.Serial_Number_Code.Code_C=Hardware_Version_NO_Stage;
	Hde_Vsn_NO_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Hardware_Version);

}

/************************************************************************************************/
/* ������	: System_Para_Init																	*/
/* ����  	: ��																				*/
/* ���		: �� 																				*/
/* ����	 	: ����ϵͳ����																		*/
/* �������� 	: 2014/11/11																		*/
/************************************************************************************************/
void System_Para_Init(void)
{
	/* ���ñ� ��ʼ�� */
	Sve_Cfg_Tab_Init();
	/* �̶����ñ� ��ʼ�� */
	Sve_Fix_Cfg_Tab_Init();
	/* ϵͳ���к� ��ʼ�� */
	System_Serial_Init();
	/********************* IAP �������� ���ݴ洢 ��ز���(�ڲ�flash) ***************************************/
	//	Mcu_IAP_Flash_Data.Data_Buffer		= IAP_Data_Buffer;		//IAP���ݻ���
	//	Mcu_IAP_Flash_Data.Write_Pointer	= BLOCK_CODESTORE_START;//IAPдָ��
	//	Mcu_IAP_Flash_Data.Data_Buffer_Size = IAP_PageSize;			//�������ݻ�������С
	//	Mcu_IAP_Flash_Data.Read_Pointer		= BLOCK_CODESTORE_START;//IAP��ָ��
	//	Mcu_IAP_Flash_Data.Space_Size		= BLOCK_CODESTORE_SPACE;//оƬ������250K�ֽ�
	//	Mcu_IAP_Flash_Data.Data_Block_Total	= 1;					//���ݿ�����	
	//	Mcu_IAP_Flash_Data.Data_Block_No	= 1;					//���ݿ���
	//	Mcu_IAP_Flash_Data.Data_Block_Check	= 0;					//����У��
	//	Mcu_IAP_Flash_Data.RX_COM_State		= FALSE;				//���ݶ�ȡ״̬
	/* �������˵���Ŀ ��ʼ�� */
	DispSensorNumConfig(Sensor_CH_Config_Num);
	/* ���ݿ� ��ʼ�� */
	Mcu_Pgm_Ude_State_Check();	
#if ((AppGPRSComProtocolEnable == 0x01)&& \
	   ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04)))
	/* ��ʼ�����÷��������� */
	ConfigService_InitPara(&Instru_Config_Inf_Tab);
#endif
	/* ��λϵͳʱ������� */
	OSTimeSet(0);
}
/******************* (C) ZHOU *****END OF FILE****/
 
