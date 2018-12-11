/************************************************************************************************/
/** @file              : Dev_GPS.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��8��7��															*/
/** @brief             : c file																	*/
/** @description       : GPS��λ�����ļ�														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "Dev_GPS.h"


#include "mcu_usart.h"




/************************************************************************************************/
/* ����GPS��Դ�������ŵ� �˿ڡ�ʱ��Դ������ */
#define GPS_Power_PORT       			(GPIOC)
#define GPS_Power_CLK        			(RCC_APB2Periph_GPIOC)
#define GPS_Power_PIN        			(GPIO_Pin_15)

#ifdef Hardware_Version_V131
/* GPSģ���Դ:�� */
#define Open_GPS_Power					{GPIO_ResetBits(GPS_Power_PORT,GPS_Power_PIN);}
/* GPSģ���Դ:�� */
#define Close_GPS_Power					{GPIO_SetBits(GPS_Power_PORT,GPS_Power_PIN);}
#else
/* GPSģ���Դ:�� */
#define Open_GPS_Power					{GPIO_SetBits(GPS_Power_PORT,GPS_Power_PIN);}
/* GPSģ���Դ:�� */
#define Close_GPS_Power					{GPIO_ResetBits(GPS_Power_PORT,GPS_Power_PIN);}
#endif



/************************************************************************************************/
/* GPRMC�������ݳ��� */
#define GPS_CMD_RMC_Data_Len			(16u)
/* GPGSA�������ݳ��� */
#define GPS_CMD_GSA_Data_Len			(16u)
/* GPGGA�������ݳ��� */
#define GPS_CMD_GGA_Data_Len			(16u)
/* GPGLL�������ݳ��� */
#define GPS_CMD_GLL_Data_Len			(16u)
/* GPGSV�������ݳ��� */
#define GPS_CMD_GSV_Data_Len			(16u)
/* GPVTG�������ݳ��� */
#define GPS_CMD_VTG_Data_Len			(16u)
/* ���������ݳ��� */
#define GPS_CMD_Cold_Start_Data_Len		(12u)


/* �ر�RMC��Ϣ��� */
const unsigned char 	GPS_CMD_TurnOff_RMC[GPS_CMD_RMC_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x04, 0x40};
/* �ر�GSA��Ϣ��� */
const unsigned char 	GPS_CMD_TurnOff_GSA[GPS_CMD_GSA_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x32};
/* �ر�GGA��Ϣ��� */
const unsigned char 	GPS_CMD_TurnOff_GGA[GPS_CMD_GGA_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x24};
/* �ر�GLL��Ϣ��� */
const unsigned char 	GPS_CMD_TurnOff_GLL[GPS_CMD_GLL_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x2B};
/* �ر�GSV��Ϣ��� */
const unsigned char 	GPS_CMD_TurnOff_GSV[GPS_CMD_GSV_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x39};
/* �ر�VTG��Ϣ��� */
const unsigned char 	GPS_CMD_TurnOff_VTG[GPS_CMD_VTG_Data_Len] = {0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0xF0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x05, 0x47};

/* ������ */
const unsigned char		GPS_CMD_ColdStart[GPS_CMD_Cold_Start_Data_Len] = {0xB5, 0x62, 0x06, 0x04, 0x04, 0x00, 0xFF, 0xFF, 0x02, 0x00, 0x0E, 0x61};


/************************************************************************************************/





/************************************************************************************************/
/* ������	: GPS_Power_Control_Pin_Config												 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: GPS��Դ�����������á�							 									*/
/* ��������	: 2015/08/07																		*/
/************************************************************************************************/
static void GPS_Power_Control_Pin_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;


	/* ���� GPS ��Դ�������� ��ʱ�� */
	RCC_APB2PeriphClockCmd(GPS_Power_CLK,ENABLE);
	/* ���� GPS ��Դ�������ŵĳ�ʼ״̬(��)  */
	GPIO_SetBits(GPS_Power_PORT,GPS_Power_PIN);
	
	/* ���� ��Դ�������� */
	GPIO_InitStructure.GPIO_Pin 	= GPS_Power_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	/*
		PC15����ͨ����Դ���ؽ��й��磬�������Դ����ֻ�ܹ��������޵ĵ���(3mA)��
		���������������Ϊ�������ʱ���������ƣ���ͬһʱ��ֻ��һ����������Ϊ�����
		��Ϊ�����ʱֻ�ܹ�����2MHzģʽ�£������������Ϊ30pF�����Ҳ�����Ϊ����Դ(������LED)��
	*/	
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Init(GPS_Power_PORT, &GPIO_InitStructure);
}

/************************************************************************************************/
/* ������	: GPS_CMD_Config															 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ѡ��GPSģ�������Ϣ���ݡ�						 									*/
/* ��������	: 2015/08/07																		*/
/************************************************************************************************/
void GPS_CMD_Config(void)
{
	
#if (GPS_CMD_RMC_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_RMC,GPS_CMD_RMC_Data_Len);
#endif
	
#if (GPS_CMD_GSA_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_GSA,GPS_CMD_GSA_Data_Len);
#endif
	
#if (GPS_CMD_GGA_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_GGA,GPS_CMD_GGA_Data_Len);
#endif
	
#if (GPS_CMD_GSV_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_GSV,GPS_CMD_GSV_Data_Len);
#endif
	
#if (GPS_CMD_GLL_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_GLL,GPS_CMD_GLL_Data_Len);
#endif
	
#if (GPS_CMD_VTG_ON_OFF == 0x00)
	USART_printf(USART2,(uint8_t*)GPS_CMD_TurnOff_VTG,GPS_CMD_VTG_Data_Len);
#endif

}


/************************************************************************************************/
/* ������	: GPS_Dev_Init																		*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ��ʼ��GPS_Dev																		*/
/* ��������	: 2015/08/07																		*/
/************************************************************************************************/
void GPS_Dev_Init(void)
{	
	/* �ϵ���ʼ��GPSʱ��GPSǿ�жϵ��Ա�ȷ���ϵ�ʱGPS������ȷ�ĵ�Դ����״̬. */
	/* GPS��Դ�����������á� */
	GPS_Power_Control_Pin_Config();
	
	/* ��GPSģ���Դ */
	Close_GPS_Power;

	__NOP();

	/* GPSģ���Դ:�� */
	Open_GPS_Power;

}












