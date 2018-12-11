/************************************************************************************************/
/** @file              : Mcu_GPS.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��7��30��															*/
/** @brief             : c file																	*/
/** @description       : 32Ϊƽ̨Ӧ���ļ�														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "System_Config.h"


#if (GPS_Config_Enable == 0x01)

#include "app.h"

#include "Mcu_GPS.h"
#include "Mcu_Usart.h"

#include "string.h"
//#include "stdlib.h"




/************************************************************************************************/
#define GPS_COM_Preemption_Priority		(2u)		/* ��ռ���ȼ� 2 */
#define GPS_COM_Sub_Priority			(1u)		/* �����ȼ� 1 */

/************************************************************************************************/

/* GPS �������ݻ����С */
#define GPS_Buffer_Size   			(256u)
/* GPS �������ݻ��� */
static unsigned char 				GPS_Buffer[GPS_Buffer_Size];

/* �� GPS �������ݻ��� */
#define GPS_RX_RD(n)				(GPS_Buffer[(n)&(GPS_Buffer_Size-1)])
/* д GPS �������ݻ��� */
#define GPS_RX_WR(n, v)				(GPS_Buffer[(n)&(GPS_Buffer_Size-1)] = v)

/* �� GPS �������ݻ���ָ�� */
static volatile unsigned int 		GPS_Rx_Rd_p = 0;
/* д GPS �������ݻ���ָ�� */
static volatile unsigned int 		GPS_Rx_Wr_p = 0; 


/************************************************************************************************/
/* �����ַ��� */
/* �����ַ��� "$GPRMC" ��� */
#define GPS_Spe_Str_GPRMC_NO		(8u)
/* �����ַ��� "$GPGSA" ��� */
#define GPS_Spe_Str_GPGSA_NO		(9u)


/************************************************************************************************/
/* ��������������� */
#define MAX_Spe_Char_Index			(8u)
/* ��������ַ������ */
#define MAX_Spe_Str_NO				(8u)

/* ������������ */
static unsigned char 	Spe_Char_Index = 0u;
/* �����ַ������ */
static unsigned char 	Spe_Str_NO = 0u;

/* GPS���������ַ�������� */
static const unsigned char 	GPS_Spe_Str_Tab[MAX_Spe_Str_NO][MAX_Spe_Char_Index]=
{
//   $ A C G M P R S
	{1,0,0,0,0,0,0,0},	// state 0
	{0,0,0,2,0,0,0,0},	// state 1
	{0,0,0,0,0,3,0,0},	// state 2
	{0,0,0,6,0,0,4,0},	// state 3
	{0,0,0,0,5,0,0,0},	// state 4
	{0,0,8,0,0,0,0,0},	// state 5
	{0,0,0,0,0,0,0,7},	// state 6
	{0,9,0,0,0,0,0,0},	// state 7
};

/* ����'A' - 'S'֮���ַ��� �����ַ�������� ��������;			*/
/* (�������ַ���Ӧ0��������ζ����gps_ptn_map���е�������Ϊ0). 	*/
/* �Ӷ��������뵱ǰ������ַ����ٵõ�����GPS_Spe_Str_Tab���е������š� */
static const unsigned char GPS_Spe_Char_Index_Tab[19] = {1,0,2,0,0,0,3,0,0,0,0,0,4,0,0,5,0,6,7};


/************************************************************************************************/
/* GPRMC�����ֽ���״̬ */
static BOOL	GPSRMC_Spe_Str_Rec_State 	= FALSE;
/* GPGSA�����ֽ���״̬ */
static BOOL	GPSGSA_Spe_Str_Rec_State 	= FALSE;
/* GPRMC ��Ϣ ����״̬ */
BOOL		GPSRMC_Data_Rec_State 		= FALSE;
/* GPGSA ��Ϣ ����״̬ */
BOOL		GPSGSA_Data_Rec_State 		= FALSE;
/* GPS ��λ״̬ */
static BOOL GPS_Pos_State 				= FALSE;


/************************************************************************************************/
/* ��ǰ����GPRMC��Ϣ�������� */
static int 	GPRMC_Rec_Data_Ctr		= 0u;
/* ��ǰ����GPGSA��Ϣ�������� */
static int 	GPGSA_Rec_Data_Ctr 		= 0u;
/* GPRMC��Ϣ"��λ״̬"�ֶ���Ϣ������ */
static unsigned char GPRMC_Pos_State_Inf_Ctr = 0u;


/* GPRMC��Ϣ���������ȡ�γ�ȡ��ٶȡ����εȣ�ʵ����󳤶�ԼΪ71�ֽ� */
char		GPRMC_MSG_Buf[GPRMC_MSG_Buf_Size+1];
/* GPGSA��Ϣ����������DOP��Ϣ����λģʽ�����ڶ�λ�����Ǳ��(���12��)�ȣ�ʵ����󳤶�Ϊ64�ֽ� */
char		GPGSA_MSG_Buf[GPGSA_MSG_Buf_Size+1];


/************************************************************************************************/



/************************************************************************************************/
/* ������	: GPS_NVIC_Configure																*/
/* ����		: �� 																				*/
/* ���		: �� 																				*/
/* ����		: ����GPSͨ��(Usart2)ȫ���ж����ȼ�													*/
/* ��������	: 2015/7/30																			*/
/************************************************************************************************/
static void GPS_NVIC_Configure(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;


	/* ʹ��USART2�ж� */
	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;					//�����ⲿ�ж�Դ��USART2�жϣ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= GPS_COM_Preemption_Priority;	//������ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= GPS_COM_Sub_Priority;			//���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//ʹ��ͨ��
	NVIC_Init(&NVIC_InitStructure);  								  						//��Ĵ�����д������
}


/************************************************************************************************/
/* ������	: GPS_Mcu_Para_Init																	*/
/* ����		: �� 																				*/
/* ���		: �� 																				*/
/* ����		: ����GPSͨ�Ŷ˿ڣ�Usart2��															*/
/* ��������	: 2015/7/30																			*/
/************************************************************************************************/
static void GPS_Mcu_Para_Init(void)
{
	/* ��ǰ����GPRMC��Ϣ�������� */
	GPRMC_Rec_Data_Ctr		= 0u;
	/* ��ǰ����GPGSA��Ϣ�������� */
	GPGSA_Rec_Data_Ctr		= 0u;
	/* GPRMC��Ϣ"��λ״̬"�ֶ���Ϣ������ */
	GPRMC_Pos_State_Inf_Ctr = 0u;

	/* ��ʼ��GPRMC��Ϣ��GPGSA��Ϣ������ر��� */
	strcpy(GPRMC_MSG_Buf, "");
	strcpy(GPGSA_MSG_Buf, "");

	/* GPRMC�����ֽ���״̬ */
	GPSRMC_Spe_Str_Rec_State 	= FALSE;
	/* GPGSA�����ֽ���״̬ */
	GPSGSA_Spe_Str_Rec_State 	= FALSE;
	/* GPRMC ��Ϣ ����״̬ */
	GPSRMC_Data_Rec_State 		= FALSE;
	/* GPGSA ��Ϣ ����״̬ */
	GPSGSA_Data_Rec_State 		= FALSE;
	/* GPS ��λ״̬ */
	GPS_Pos_State 				= FALSE;

}


/************************************************************************************************/
/* ������	: GPS_Com_Configure																	*/
/* ����		: �� 																				*/
/* ���		: �� 																				*/
/* ����		: ����GPSͨ�Ŷ˿ڣ�Usart2��															*/
/* ��������	: 2015/7/30																			*/
/************************************************************************************************/
static void GPS_Com_Configure(void)
{
/* ģ��ͨ�ŵĲ�����:9600(Ĭ��ֵ) */
#define	GPS_Define_BaudRate		(9600u)



#if (Usart_2_Function_Config_Enable == 0x01)		
	/* ����ģ���ͨ�Ŷ˿� */
	USART2_Config(GPS_Define_BaudRate);
	/* ���ô���2�����ж� */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	/* ʹ�ܴ���2���� */
	USART_Cmd(USART2 , ENABLE );
#endif



#undef	GPS_Define_BaudRate	

}

/************************************************************************************************/
/* ������	: GPS_Rec_Data_Inf_Flow																*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: GPS������Ϣ������																	*/
/* ��������	: 2015/7/30																			*/
/************************************************************************************************/
void GPS_Rec_Data_Inf_Flow(char data)
{


	/* ����ǰ���յ�������д��GPS���ջ��� */
	GPS_RX_WR(GPS_Rx_Wr_p++, data);

	/************************ ���GPS��Ϣͷ ********************************/		
	/* ȡ�õ�ǰ�����ַ���gps_ptn_map���е�������(��Ϊ�������ַ�������������) */
	if(data == '$')
	{
		/* ��0 ������������ */
		Spe_Char_Index = 0;
		/* ��0 �����ַ������ */
		Spe_Str_NO = 0;
		/* ���������ַ�������� "" ȷ�������ַ�������. */
		Spe_Str_NO = GPS_Spe_Str_Tab[Spe_Str_NO][Spe_Char_Index];
	}
	else if(data >= 'A' && data <= 'S')/* �����'A' - 'S'֮����ַ� */
	{
		/* ����õ��������Ƿ�������(������Ϊ0���������壬���������ַ�) */
		Spe_Char_Index = GPS_Spe_Char_Index_Tab[data - 'A'];

		/* �ж������ַ��������Ƿ������������� */
		if(Spe_Char_Index > 0)
		{	
			/* ��ǰ����Ϊ���������� */
			/* �������ֱ�����в�ѯ��Ӧ�������ַ������ */
			Spe_Str_NO = GPS_Spe_Str_Tab[Spe_Str_NO][Spe_Char_Index];
		}
		else
		{
			/* ��0 �����ֱ�����в�ѯ��Ӧ�������ַ������. */ 
			Spe_Str_NO = 0;								
		}
	}
	else
	{
		/* ��0 �����ֱ�����в�ѯ��Ӧ�������ַ������. */ 
		Spe_Str_NO = 0; 	
	}

	/* ��������ַ������ */
	if(Spe_Str_NO > (MAX_Spe_Str_NO - 1))
	{
		/* �жϵ�ǰ��GPS���������� */
		switch(Spe_Str_NO)
		{
			case GPS_Spe_Str_GPRMC_NO:
			{
				/* $GPRMC��⵽(ÿ��NMEA��Ϣ�ж�����һ��RMC��Ϣ��RMC��Ϣͷ����⵽��Ƶ����1Hz) */
				/* ������һ�����յ���GPRMC��Ϣ���������Ž����µ�GPRMC��Ϣ,��ʵ�ֽ��պʹ����ͬ�� */
				if( GPSRMC_Spe_Str_Rec_State == FALSE)
				{
					/* RMC���������� */
					GPSRMC_Spe_Str_Rec_State = TRUE;
					/* ��0 RMC���ݽ������� */
					GPRMC_Rec_Data_Ctr 	= 0;
					/* ��0 RMC ','���Ž������� */
					GPRMC_Pos_State_Inf_Ctr	= 0;
				}
				break;
			}

			case GPS_Spe_Str_GPGSA_NO:
			{
				/* $GPGSA��⵽(ÿ��NMEA��Ϣ�ж�����һ��GSA��Ϣ��GSA��Ϣͷ����⵽��Ƶ����1Hz) */
				/* ������һ�����յ���gsa��Ϣ���������Ž����µ�gsa��Ϣ����ʵ�ֽ��պʹ����ͬ�� */
				if(GPSGSA_Data_Rec_State == FALSE)
				{
					/* GSA���������� */
					GPSGSA_Spe_Str_Rec_State = TRUE; 	
					/* ��0 GSA���ݽ������� */
					GPGSA_Rec_Data_Ctr 	= 0;
				}
				break;				
			}	

			
			default:
			{
				break;				
			}
		}
		/* ��0 �����ֱ�����в�ѯ��Ӧ�������ַ������. */ 
		Spe_Str_NO = 0; 							// ״̬��������̬�Ļ����̸�λ
	}

	/************************ ��ȡ�����ֽ���RMC/GSA��Ϣ ********************************/
	
	/* ��⵽GPRMC�����ַ�����Ϣͷ��׼������rmc��Ϣ(��$GPRMC��C�ַ���ʼ���ա�ֱ�����з���) */
	if(GPSRMC_Spe_Str_Rec_State == TRUE)
	{	
		/* ������һ��rmc��Ϣ�������ſ�ʼ�����µ�rmc��Ϣ */
		/* GPRMC��Ϣ�Ƿ��Ѿ������մ��� */
		if(GPSRMC_Data_Rec_State == FALSE)
		{
			/* RMC��Ϣ�Ѿ������մ��� */
			/* �����µ����� */
			GPRMC_MSG_Buf[GPRMC_Rec_Data_Ctr++] = data;
			/* ��⵽���з�����ȡ���ַ�������gps_msg_rmc�������󳤶ȣ���ֹͣ��ȡ */
			if(((GPS_RX_RD(GPS_Rx_Wr_p-2) == '\r') && (GPS_RX_RD(GPS_Rx_Wr_p-1) == '\n')) || (GPRMC_Rec_Data_Ctr >= GPRMC_MSG_Buf_Size))
			{
				/* ����ַ�����β����(�ڻ��з�֮��) */
				GPRMC_MSG_Buf[GPRMC_Rec_Data_Ctr++] = '\0';

				/* ��ȡ��һ��RMC��Ϣ�󣬽�RMC����־��λ���Ա�������ȡ��һ��RMC��Ϣ */
				GPSRMC_Spe_Str_Rec_State = FALSE;
				/* ��� GPRMC��Ϣ�������� */
				GPRMC_Rec_Data_Ctr 	= 0;

				/* �����ǰGPS�Ѿ���λ��������GPRMC��Ϣ��ȡ��ɱ�־(֮ǰ��ȡ��GPRMC��Ϣ��������GPSδ��λʱ��ȡ�ģ����ͳͳ����) */
				if(GPS_Pos_State == TRUE)
				{
					/* ������һ��GPRMC��Ϣ�󣬽�������Ϣ��Ҫʱ�䣬��˿�������ϴν����굫���ڴ����GPRMC��Ϣ���µĽ��չ��̸��ǣ�������Ҫͬ�� */
					GPSRMC_Data_Rec_State = TRUE;
				}
				/* ��λGPRMC��λ��־ */
				GPS_Pos_State = FALSE;
				/* ��λGPRMC��Ϣ"��λ״̬"�ֶ���Ϣ�������� */
				GPRMC_Pos_State_Inf_Ctr = 0;
			}
		}	

		/* ��⵽GPRMC��Ϣͷ�󣬽����ż�ⶨλ״̬ */
		if(data == ',')
		{
			GPRMC_Pos_State_Inf_Ctr++;

			/* ��⵽GPRMC��Ϣͷ��ĵ��������š� */
			if(GPRMC_Pos_State_Inf_Ctr == 3)
			{	
				/* ��ȡ�˶���֮ǰ��һ���ַ�(����λ״̬��־: A - �Ѷ�λ��V - δ��λ) */
				if(GPS_RX_RD(GPS_Rx_Wr_p-2) == 'A')
				{			
					/* ����GPRMC��λ��־ */
					GPS_Pos_State = TRUE;
				}
				else
				{
					/* δ����GPRMC��λ��־ */
					GPS_Pos_State = FALSE;
				}
				// else: exception...
			}
		}	
	}

	/* ���ܵ�ǰ�Ƿ�λ�� �ڼ�⵽GPGSA��Ϣͷ�������ȡ����Ϣ(��GPGSA��ĩβ�ַ�A��ʼ��ȡ) */
	if(GPSGSA_Spe_Str_Rec_State == TRUE)
	{			
		/* ��GPGSA��Ϣͷ֮��ĵ�һ������(�������ű���)��ʼ��ȡGPGSA��Ϣ */
		if(GPSGSA_Data_Rec_State == FALSE)
		{
			/* GPGSA��Ϣ�Ѿ������մ��� */
			/* �����µ�GPGSA��Ϣ���� */
			GPGSA_MSG_Buf[GPGSA_Rec_Data_Ctr++] = data;
			/* ��⵽���з�����ȡ���ַ�������gps_msg_gsa�������󳤶ȣ���ֹͣ��ȡ */
			if((GPS_RX_RD(GPS_Rx_Wr_p-2) == '\r' && GPS_RX_RD(GPS_Rx_Wr_p-1) == '\n') || (GPGSA_Rec_Data_Ctr >= GPGSA_MSG_Buf_Size))
			{
				/* ����ַ�����β����(�ڻ��з�֮��) */
				GPGSA_MSG_Buf[GPGSA_Rec_Data_Ctr++] = '\0';	

				/* ��ȡ��һ��GPGSA��Ϣ�󣬽�GPGSA����־��λ���Ա�������ȡ��һ��GPGSA��Ϣ */
				GPSGSA_Spe_Str_Rec_State = FALSE;
				GPGSA_Rec_Data_Ctr = 0;
				
				/* �����ǰGPRMC��Ϣ�Ѿ�����ȡ�꣬������GPGSA��Ϣ��ȡ��ɱ�־(��һ��NMEA��Ϣ��GPRMC��Ϣ�ǰ) */
				if(GPSRMC_Data_Rec_State == TRUE)
				{
					/* ����GPGSA��Ϣ������ɱ�־ */
					GPSGSA_Data_Rec_State = TRUE;
					/* ����GPS��Ϣ���� */
					OSMboxPost(GPS_Box,(char*)GPGSA_MSG_Buf);
				}
			}	
		}
	}


}


/************************************************************************************************/
/* ������	: GPS_Mcu_Init																		*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: GPS��ʼ��MCU																		*/
/* ��������	: 2015/7/30																			*/
/************************************************************************************************/
void GPS_Mcu_Init(void)
{

	/* ��ʼ������GPS Mcu���� */
	GPS_Mcu_Para_Init();
	
	/* ����GPSͨ��(Usart2)ȫ���ж����ȼ� */
	GPS_NVIC_Configure();

	/* ����GPSͨ�Ŷ˿ڣ�Usart2�� */
	GPS_Com_Configure();
	
	

}


#endif


/******************* (C) ZHOU *****END OF FILE****/


