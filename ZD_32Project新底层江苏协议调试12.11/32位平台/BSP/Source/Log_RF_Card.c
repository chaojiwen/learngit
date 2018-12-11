/**
  ******************************************************************************
  * @file              : Log_RF_Card.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015��4��12�� 09:55
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
***/
#include "System_Config.h"

#if (Wireless_RF_Card_Config_Enable == 0x01)

#include "Log_Sensor.h"
#include "Log_RF_Card.h"
#include "Log_Adc1.h"

#include "Mcu_DMA.h"
#include "Mcu_Usart.h"
#include "Mcu_led.h"

#include "app.h"


#define Radioactive_Location 1 //����Դ��λ




/************************************************************************************************/
/* ����RF��Ƭ���ͽṹ */
typedef enum {
			  Null_RF_Card 			= 0,	//û�����ù��Ŀ�Ƭ
			  Single_TEM_RF_Card 	= 1,	//���¿�Ƭ
			  TEM_HUM_RF_Card 		= 2,	//��ʪ��Ƭ
			 }Wireless_RF_Card_Type_Struct;
/************************************************************************************************/
/* ����RF��Ƭ�������ݼĴ��� ���ݴ�С */
#define Wireless_RF_Card_Rece_Data_Buffer_Size	(32u)
/* ����RF��Ƭ�������ݼĴ��� */
	  char Wireless_RF_Card_Rece_Data_Buffer[Wireless_RF_Card_Rece_Data_Buffer_Size];
/* ����RF��Ƭ�������ݼĴ��� ָ�루������Ҫ��Ϊ�˸�DMA����ַ�õģ� */
const char *Wireless_RF_Card_Data_Buffer_Pointer = Wireless_RF_Card_Rece_Data_Buffer;
/************************************************************************************************/
/* ����RF��Ƭ��������״̬�Ĵ����ṹ */
typedef enum {
			  NULL_Rece_Data 	= 0,	//�޽�������
			  HT_Rece_Data 		= 1,	//����ɽ�������
			  TC_Rece_Data 		= 2,	//��ɽ�������
			 }Wireless_RF_Card_Rece_Data_SR_Struct;
/* ����RF��Ƭ��������״̬�Ĵ��� */
static Wireless_RF_Card_Rece_Data_SR_Struct Wireless_RF_Rece_Data_SR;
/************************************************************************************************/
/* ����RF��Ƭ���ݼĴ�����С */
#define Wireless_RF_Card_Collection_Data_DR_Size	(32u)
/* ����RF��Ƭ���ݼĴ��� */
static float Wireless_RF_Card_Collection_Data_DR[Wireless_RF_Card_Collection_Data_DR_Size] = 
{
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	Wireless_RF_Card_Data_Rece_Timeout_Para,
	
};
/************************************************************************************************/		 
/* ����RF��Ƭ��ص������ݼĴ�����С */
#define Wireless_RF_Card_Power_DR_Size	(32u)
/* ����RF��Ƭ���ݼĴ��� */
static u8 	Wireless_RF_Power_Data_DR[Wireless_RF_Card_Power_DR_Size];
/************************************************************************************************/	 	 
/* ����RF��Ƭ���ݻ����С */
#define Wireless_RF_Card_Data_Buffer_Size	(16u)
/* ����RF��Ƭ����֡ͷ */
#define Wireless_RF_Card_Data_Frames_Head	(0X24)			 
/* ����RF��Ƭ���ݽṹ */
typedef union 
{
	struct			 
	{
		/* ���� ֡ͷ (����ǳ���:0X24) */
		u8  Data_Frames_Head;
		/* ��ĿID */	
		u8 	UID1;
		u8 	UID0;		
		/* ����RF��ƬID */
		u8 ADS_ID_3;
		u8 ADS_ID_2;
		u8 ADS_ID_1;
		u8 ADS_ID_0;
		/* ����״̬ */
		u8  Reserve;
		/* ����RF��Ƭ�ĵ�ص�ѹ.(Ĭ����ADCֵ,������Ҫ����:VCC = (BV/256)*3.3����ѹС�� 2.5VΪǷѹ��) */
		u8  Crad_Power;
		/* ʪ������ */
		u8 HUM0;
		u8 HUM1;
		/* �¶����� */
		u8 TMP0;
		u8 TMP1;		
		/* �������к� */
		u8  TX_Serial_Num;
		/* �����ź�ǿ��.(Ĭ����HEXֵ��������Ҫ����:�ź�ǿ�� = -((~RSSI)+1)) */
		u8  RSSI;
		/* У��.(��ǰ����������У���) */
		u8  Check;
	}Reg;
	u8 Data_Buffer[Wireless_RF_Card_Data_Buffer_Size];
}Wireless_RF_Card_Data_Struct;
/************************************************************************************************/
/* ��¼��Ƭ�������ݵ�ʱ������� */
/* ��¼��������Ļ����С�� */
#define Wireless_RF_Card_Data_Rece_Time_Buffer	(32u)
/* ��¼�������� */
static u16 Wireless_RF_Card_Data_Rece_Time[Wireless_RF_Card_Data_Rece_Time_Buffer];
/************************************************************************************************/	 	


/************************************************************************************************/
/* ������	: Wireless_RF_Card_Init_Congif_Delay										 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���ÿ�Ƭ����ģ�����ʱ����						 									*/
/* ��������	: 2015/04/16																		*/
/************************************************************************************************/
static void Wireless_RF_Card_Init_Congif_Delay(u32 cnt)
{
	while(cnt)
	{
		cnt--;
	}
}

/************************************************************************************************/
/* ������	: Wireless_RF_Card_GPIO_Config												 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: �������߿�Ƭģ��Ŀ�������						 									*/
/* ��������	: 2015/04/12																		*/
/************************************************************************************************/
static void Wireless_RF_Card_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RF_Card_Power_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= RF_Card_Power_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	/*
		PC13��PC14��PC15����ͨ����Դ���ؽ��й��磬�������Դ����ֻ�ܹ��������޵ĵ���(3mA)�������������
		����Ϊ�������ʱ���������ƣ���ͬһʱ��ֻ��һ����������Ϊ�������Ϊ�����ʱֻ�ܹ�����2MHzģʽ�£�
		�����������Ϊ30pF�����Ҳ�����Ϊ����Դ(������LED)��
	*/	
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Init(RF_Card_Power_PORT, &GPIO_InitStructure);
}


/************************************************************************************************/
/* ������	: RF_Card_Init														 	 			*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ����������ʪ�ȿ�Ƭ								 									*/
/* ��������	: 2015/04/12																		*/
/* �޸�����	: 2015/11/11																		*/
/************************************************************************************************/
void RF_Card_Init(void)
{
/* ģ��ͨ�ŵĲ�����:38400(Ĭ��ֵ) */
#define	RF_Card_Define_BaudRate		(38400u)


	/* ����ģ��Ŀ������� */
	Wireless_RF_Card_GPIO_Config();
	
	//0���� ģ���Դ
	Close_RF_Card_Power;
	/* ��֤ģ�鳹�׶ϵ磬20ms */
	Wireless_RF_Card_Init_Congif_Delay(0X20000);	

	//1���������ݽӷ�ʽ��ʹ��DMA�Ӵ��ڼĴ����������ݣ�
	DMA_Rece_USART2_Data_Config();

	//2������ģ���ͨ�Ŷ˿�
	USART2_Config(RF_Card_Define_BaudRate);
	/* ���� USART2 DMA ���� */
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	/* ʹ�ܴ��ڹ��� */
	USART_Cmd(USART2 , ENABLE );		


	//3���� ģ���Դ
	Open_RF_Card_Power;


#undef	RF_Card_Define_BaudRate	

}


/************************************************************************************************/
/* ������	: RF_Card_Reset_Rece_Module											 	 			*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��λ����ģ��									 									*/
/* ��������	: 2015/04/16																		*/
/************************************************************************************************/
static void RF_Card_Reset_Rece_Module(void)
{
	//0���� ģ���Դ
	Close_RF_Card_Power;	
	/* ��֤ģ�鳹�׶ϵ磬20ms */
	Wireless_RF_Card_Init_Congif_Delay(0X35000);	
	
	//1���������ݽӷ�ʽ��ʹ��DMA�Ӵ��ڼĴ����������ݣ�
	DMA_Rece_USART2_Data_Config();

	//2���� ģ���Դ	
	Open_RF_Card_Power;
}


/************************************************************************************************/
/* ������	: RF_Card_Request_Process_TC												 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������һ��RF��Ƭ������ɣ����������ݡ���DMA����������� �жϴ�������					*/
/* ��������	: 2015/04/15																		*/
/************************************************************************************************/
void RF_Card_Request_Process_TC(void)
{
	
	/* ���� DMA �������ݰ���ɱ�־ */
	Wireless_RF_Rece_Data_SR = TC_Rece_Data;
	
	/* ���� RF��Ƭ�� �ź��� */
	OSSemPost(RF_Card_QUE_Sem);

}

/************************************************************************************************/
/* ������	: RF_Card_Request_Process_HT												 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��������һ��RF��Ƭ������ɣ����������ݡ���DMA�������ݰ� �жϴ�������					*/
/* ��������	: 2015/04/15																		*/
/************************************************************************************************/
void RF_Card_Request_Process_HT(void)
{

	/* ���� DMA �������ݰ���ɱ�־ */
	Wireless_RF_Rece_Data_SR = HT_Rece_Data;
	
	/* ���� RF��Ƭ�� �ź��� */
	OSSemPost(RF_Card_QUE_Sem);

}


/************************************************************************************************/
/* ������	: RF_Card_Screen															 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ɸѡRF��Ƭ����									 									*/
/* ��������	: 2015/04/12																		*/
/************************************************************************************************/
void RF_Card_Screen(INSTRU_SENSOR *sensor_t)
{

	/* ����RF��Ƭ���ݼĴ���ָ�� */
	Wireless_RF_Card_Data_Struct *rf_card_data_reg;
	/* ������ */
	u8 num;
	/* ����У�� */
	u8 check;
	/* ����RF��Ƭ ID ָ�� */
	u32 rf_card_id;
	
	/* �¶����� */
	float	tempdata;
	/* ʪ������ */
	float	humidata;

	
	//1����ȡ ����RF��Ƭ�������ݼĴ��� ����
	if(Wireless_RF_Rece_Data_SR == HT_Rece_Data)
	{
		rf_card_data_reg = (Wireless_RF_Card_Data_Struct*)Wireless_RF_Card_Rece_Data_Buffer;
	}else if(Wireless_RF_Rece_Data_SR == TC_Rece_Data)
	{
		rf_card_data_reg = (Wireless_RF_Card_Data_Struct*)&Wireless_RF_Card_Rece_Data_Buffer[Wireless_RF_Card_Data_Buffer_Size];

	}else
	{
		__nop();
		return;
	}
	
	/* ��� ����RF��Ƭ ��������״̬ */
	Wireless_RF_Rece_Data_SR = NULL_Rece_Data;
	//2���������ݺϷ��Լ��:
	/* 
		a��֡ͷ;
		b��У���;
	*/
	if(rf_card_data_reg->Reg.Data_Frames_Head != Wireless_RF_Card_Data_Frames_Head)
	{
		/* 
			����ͷ֡���� 0X24 ������ǱȽ����ص����⣬�п��������ݴ�λ�Ľ���������ʱ������
			���򣬾��ղ������������ݡ��������ղ������ݣ�
		*/
		/* ��λģ�� */
		RF_Card_Reset_Rece_Module();
		return;	
	}
	
	
	for(num = 0,check = 0;num < (Wireless_RF_Card_Data_Buffer_Size-1) ;num++)
	{
		/* ���У�� */
		check += rf_card_data_reg->Data_Buffer[num];
	}
	
	if(check != rf_card_data_reg->Reg.Check)
	{
		/* У�� ���� */
		__nop();
		return;
	}
	/* ��ʱֻ��������У�飬�������Ҫ������������������� */
	
	//3��ɸѡIDƥ�������(��������ֻ�Կ�ƬID����ƥ�����)
	/* ��ȡ��ƬID */
	rf_card_id = ((((u32)rf_card_data_reg->Reg.ADS_ID_3) << 24) |
				  (((u32)rf_card_data_reg->Reg.ADS_ID_2) << 16) |
				  (((u32)rf_card_data_reg->Reg.ADS_ID_1) << 8)  |
				   ((u32)rf_card_data_reg->Reg.ADS_ID_0));
	
	/* �жϵ�ǰ��Ƭ�ǵ��»�����ʪ�� */// ֵ:1750���ڵ��¿�Ƭ������¶�125��
	if((rf_card_data_reg->Reg.HUM0 == 0x00)&&
	   (rf_card_data_reg->Reg.HUM1 == 0x00)&&
	   (((((s16)rf_card_data_reg->Reg.TMP0)<<8)+rf_card_data_reg->Reg.TMP1) < 1750))
	{
		/* ʪ������ȫ��Ϊ0˵����ǰ��ƬΪ���� */
		tempdata = (((float)((((s16)rf_card_data_reg->Reg.TMP0)<<8)+rf_card_data_reg->Reg.TMP1)-500.0)/10.0);
		humidata = Wireless_RF_Card_Data_Rece_Timeout_Para;
	}else
	{
		/* ��ǰ��Ƭ��ʪ��Ƭ */
		tempdata = (((float)((((s16)rf_card_data_reg->Reg.TMP0)<<8)+rf_card_data_reg->Reg.TMP1)*175.72)/65536.0-46.85);
		humidata = ((((float)((((s16)rf_card_data_reg->Reg.HUM0)<<8)+rf_card_data_reg->Reg.HUM1)*125.0)/65536.0)-6);
	}
	
/* �ж� �����������Ƿ����� */
#if (Wireless_RF_Card_Config_Enable == 0x01)

/* �ж��Ƿ�����ʪ�ȿ�Ƭ */
#if (0 == Sensor_RF_Humiture_Num)
	/* û�� */
	(void)humidata;
#endif
/* �ж� ͨ��1 */
#if	((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH1.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1] 	= tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_1] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_1] = 0;
	
	}
#elif	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH1.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡʪ������ */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_1] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_1] = 0;
	}
#endif	
	
/* �ж� ͨ��2 */
#if	((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH2.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_2] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_2] = 0;	
	}
#elif	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH2.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_2] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_2] = 0;	
	}
#endif
	
/* �ж� ͨ��3 */
#if	((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH3.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_3] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_3] = 0;
	}
#elif	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH3.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_3] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_3] = 0;
	}
#endif			
	
/* �ж� ͨ��4 */
#if	((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH4.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_4] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_4] = 0;
	}
#elif	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH4.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_4] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_4] = 0;
	}	
#endif			
	
/* �ж� ͨ��5 */
#if	((Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH5.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_5] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_5] = 0;
	}
#elif	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH5.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_5] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_5] = 0;
	}	
#endif			
	
/* �ж� ͨ��6 */
#if	((Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH6.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_6] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_6] = 0;
	}
#elif	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH6.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_6] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_6] = 0;
	}	
#endif			
	
/* �ж� ͨ��7 */
#if	((Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH7.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_7] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_7] = 0;
	}
#elif	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH7.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_7] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_7] = 0;
	}	
#endif			
	
/* �ж� ͨ��8 */
#if	((Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH8.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_8] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_8] = 0;
	}
#elif	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH8.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_8] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_8] = 0;
	}	
#endif			
	
	/* �ж� ͨ��9 */
#if	((Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH9.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_9] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_9] = 0;
	}
#elif	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH9.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_9] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_9] = 0;
	}	
#endif	
	
	/* �ж� ͨ��10 */
#if	((Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH10.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_10] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_10] = 0;
	}
#elif	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH10.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_10] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_10] = 0;
	}	
#endif	
	
	/* �ж� ͨ��11 */
#if	((Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH11.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_11] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_11] = 0;
	}
#elif	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH11.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_11] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_11] = 0;
	}	
#endif	
	
	/* �ж� ͨ��12 */
#if	((Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH12.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_12] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_12] = 0;
	}
#elif	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH12.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_12] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_12] = 0;
	}	
#endif	
	
	/* �ж� ͨ��13 */
#if	((Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH13.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_13] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_13] = 0;
	}
#elif	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH13.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_13] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_13] = 0;
	}	
#endif	
	
	/* �ж� ͨ��14 */
#if	((Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH14.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_14] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_14] = 0;
	}
#elif	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH14.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_14] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_14] = 0;
	}	
#endif	
	
	/* �ж� ͨ��15 */
#if	((Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH15.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_15] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_15] = 0;
	}
#elif	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH15.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_15] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_15] = 0;
	}	
#endif	
	
	/* �ж� ͨ��16 */
#if	((Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH16.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_16] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_16] = 0;
	}
#elif	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH16.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_16] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_16] = 0;
	}	
#endif	
	
	/* �ж� ͨ��17 */
#if	((Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH17.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_17] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_17] = 0;
	}
#elif	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH17.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_17] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_17] = 0;
	}	
#endif	
	
	/* �ж� ͨ��18 */
#if	((Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH18.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_18] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_18] = 0;
	}
#elif	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH18.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_18] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_18] = 0;
	}	
#endif	
	
	/* �ж� ͨ��19 */
#if	((Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH19.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_19] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_19] = 0;
	}
#elif	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH19.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_19] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_19] = 0;
	}	
#endif	
	
	/* �ж� ͨ��20 */
#if	((Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH20.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_20] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_20] = 0;
	}
#elif	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH20.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_20] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_20] = 0;
	}	
#endif	
	
	/* �ж� ͨ��21 */
#if	((Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH21.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_21] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_21] = 0;
	}
#elif	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH21.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_21] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_21] = 0;
	}	
#endif	
	
	/* �ж� ͨ��22 */
#if	((Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH22.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_22] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_22] = 0;
	}
#elif	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH22.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_22] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_22] = 0;
	}	
#endif	
	
	/* �ж� ͨ��23 */
#if	((Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH23.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_23] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_23] = 0;
	}
#elif	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH23.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_23] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_23] = 0;
	}	
#endif	
	
	/* �ж� ͨ��24 */
#if	((Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH24.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_24] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_24] = 0;
	}
#elif	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH24.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_24] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_24] = 0;
	}	
#endif	
	
	/* �ж� ͨ��25 */
#if	((Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH25.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_25] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_25] = 0;
	}
#elif	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH25.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_25] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_25] = 0;
	}	
#endif	
	
	
	/* �ж� ͨ��26 */
#if	((Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH26.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_26] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_26] = 0;
	}
#elif	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH26.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_26] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_26] = 0;
	}	
#endif	
	
	/* �ж� ͨ��27 */
#if	((Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH27.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_27] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_27] = 0;
	}
#elif	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH27.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_27] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_27] = 0;
	}	
#endif	
	
	/* �ж� ͨ��28 */
#if	((Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH28.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_28] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_28] = 0;
	}
#elif	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH28.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_28] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_28] = 0;
	}	
#endif	
	
	/* �ж� ͨ��29 */
#if	((Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH29.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_29] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_29] = 0;
	}
#elif	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH29.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_29] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_29] = 0;
	}	
#endif	
	
	/* �ж� ͨ��30 */
#if	((Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH30.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_30] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_30] = 0;
	}
#elif	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH30.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_30] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_30] = 0;
	}	
#endif	
	
	/* �ж� ͨ��31 */
#if	((Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH31.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_31] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_31] = 0;
	}
#elif	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH31.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_31] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_31] = 0;
	}	
#endif	
	
	/* �ж� ͨ��32 */
#if	((Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp))
	/* �жϵ�ǰ�Ŀ�Ƭ�Ƿ�Ϊ���õĿ�Ƭ */
	if(rf_card_id == sensor_t->Inf_State.CH32.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32] = tempdata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_32] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_32] = 0;
	}
#elif	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum
	if(rf_card_id == sensor_t->Inf_State.CH32.SENSOR_RF_Card_ID_NO)
	{
		/* ��ȡ�������� */
		Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32] 	= humidata;
		/* ��ȡ��ص������� */
		Wireless_RF_Power_Data_DR[Sensors_CH_32] = rf_card_data_reg->Reg.Crad_Power;
		/* �����Ƭ��ʱʱ�� */
		Wireless_RF_Card_Data_Rece_Time[Sensors_CH_32] = 0;
	}	
#endif	
	
	
#endif		
	
return;
}


/************************************************************************************************/
/* ������	: RF_Card_Trans_Sensor_Data														 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���ɼ�������ʪ������ת��ָ���ļĴ�����			 										*/
/* ��������	: 2015/04/16																		*/
/* �޸�����	: 2015/08/18																		*/
/************************************************************************************************/
void RF_Card_Trans_Sensor_Data(float *data_buffer)
{

/* �ж� ͨ��1 */
#if		Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_1) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_1) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_1) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1];		
#endif	
	
/* �ж� ͨ��2 */
#if		Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_2) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_2) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_2) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2];	
#endif	
	
/* �ж� ͨ��3 */
#if		Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_3) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_3) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_3) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3];		
#endif	
	
/* �ж� ͨ��4 */
#if		Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_4) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_4) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_4) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4];	
#endif	
	
/* �ж� ͨ��5 */
#if		Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_5) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_5) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_5) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5];	
#endif	
	
/* �ж� ͨ��6 */
#if		Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_6) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_6) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_6) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6];		
#endif	
	
/* �ж� ͨ��7 */
#if		Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_7) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_7) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_7) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7];		
#endif	
	
/* �ж� ͨ��8 */
#if		Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_8) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_8) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_8) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8];
#endif	

/* �ж� ͨ��9 */
#if		Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_9) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9];
#elif 	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_9) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9];
#elif 	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_9) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9];
#endif	

/* �ж� ͨ��10 */
#if		Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_10) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10];
#elif 	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_10) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10];
#elif 	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_10) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10];
#endif	

/* �ж� ͨ��11 */
#if		Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_11) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11];
#elif 	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_11) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11];
#elif 	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_11) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11];
#endif	

/* �ж� ͨ��12 */
#if		Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_12) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12];
#elif 	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_12) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12];
#elif 	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_12) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12];
#endif	

/* �ж� ͨ��13 */
#if		Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_13) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13];
#elif 	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_13) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13];
#elif 	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_13) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13];
#endif	

/* �ж� ͨ��14 */
#if		Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_14) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14];
#elif 	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_14) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14];
#elif 	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_14) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14];
#endif	

/* �ж� ͨ��15 */
#if		Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_15) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15];
#elif 	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_15) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15];
#elif 	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_15) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15];
#endif	

/* �ж� ͨ��16 */
#if		Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_16) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16];
#elif 	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_16) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16];
#elif 	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_16) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16];
#endif	

/* �ж� ͨ��17 */
#if		Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_17) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17];
#elif 	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_17) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17];
#elif 	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_17) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17];
#endif	

/* �ж� ͨ��18 */
#if		Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_18) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18];
#elif 	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_18) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18];
#elif 	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_18) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18];
#endif	

/* �ж� ͨ��19 */
#if		Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_19) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19];
#elif 	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_19) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19];
#elif 	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_19) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19];
#endif	

/* �ж� ͨ��20 */
#if		Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_20) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20];
#elif 	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_20) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20];
#elif 	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_20) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20];
#endif	

/* �ж� ͨ��21 */
#if		Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_21) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21];
#elif 	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_21) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21];
#elif 	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_21) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21];
#endif	

/* �ж� ͨ��22 */
#if		Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_22) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22];
#elif 	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_22) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22];
#elif 	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_22) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22];
#endif	

/* �ж� ͨ��23 */
#if		Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_23) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23];
#elif 	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_23) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23];
#elif 	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_23) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23];
#endif	

/* �ж� ͨ��24 */
#if		Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_24) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24];
#elif 	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_24) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24];
#elif 	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_24) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24];
#endif	

/* �ж� ͨ��25 */
#if		Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_25) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25];
#elif 	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_25) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25];
#elif 	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_25) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25];
#endif	

/* �ж� ͨ��26 */
#if		Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_26) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26];
#elif 	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_26) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26];
#elif 	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_26) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26];
#endif	

/* �ж� ͨ��27 */
#if		Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_27) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27];
#elif 	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_27) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27];
#elif 	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_27) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27];
#endif	

/* �ж� ͨ��28 */
#if		Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_28) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28];
#elif 	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_28) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28];
#elif 	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_28) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28];
#endif	

/* �ж� ͨ��29 */
#if		Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_29) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29];
#elif 	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_29) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29];
#elif 	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_29) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29];
#endif	

/* �ж� ͨ��30 */
#if		Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_30) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30];
#elif 	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_30) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30];
#elif 	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_30) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30];
#endif	

/* �ж� ͨ��31 */
#if		Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_31) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31];
#elif 	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_31) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31];
#elif 	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_31) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31];
#endif	

/* �ж� ͨ��32 */
#if		Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp
		*(data_buffer + Sensors_CH_32) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32];
#elif 	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(data_buffer + Sensors_CH_32) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32];
#elif 	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(data_buffer + Sensors_CH_32) 		= Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32];
#endif	

}

/************************************************************************************************/
/* ������	: RF_Card_Trans_Power_Data													 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���ɼ����Ŀ�Ƭ������ת��ָ���ļĴ�����			 										*/
/* ��������	: 2015/04/16																		*/
/************************************************************************************************/
void RF_Card_Trans_Power_Data(u8 *power_data_buffer)
{
	
/* �ж� ͨ��1 */
#if		Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_1) 		= Wireless_RF_Power_Data_DR[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_1) 		= Wireless_RF_Power_Data_DR[Sensors_CH_1];
#elif 	Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_1) 		= Wireless_RF_Power_Data_DR[Sensors_CH_1];		
#endif	
	
/* �ж� ͨ��2 */
#if		Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_2) 		= Wireless_RF_Power_Data_DR[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_2) 		= Wireless_RF_Power_Data_DR[Sensors_CH_2];
#elif 	Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_2) 		= Wireless_RF_Power_Data_DR[Sensors_CH_2];	
#endif	
	
/* �ж� ͨ��3 */
#if		Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_3) 		= Wireless_RF_Power_Data_DR[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_3) 		= Wireless_RF_Power_Data_DR[Sensors_CH_3];
#elif 	Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_3) 		= Wireless_RF_Power_Data_DR[Sensors_CH_3];		
#endif	
	
/* �ж� ͨ��4 */
#if		Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_4) 		= Wireless_RF_Power_Data_DR[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_4) 		= Wireless_RF_Power_Data_DR[Sensors_CH_4];
#elif 	Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_4) 		= Wireless_RF_Power_Data_DR[Sensors_CH_4];	
#endif	
	
/* �ж� ͨ��5 */
#if		Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_5) 		= Wireless_RF_Power_Data_DR[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_5) 		= Wireless_RF_Power_Data_DR[Sensors_CH_5];
#elif 	Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_5) 		= Wireless_RF_Power_Data_DR[Sensors_CH_5];	
#endif	
	
/* �ж� ͨ��6 */
#if		Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_6) 		= Wireless_RF_Power_Data_DR[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_6) 		= Wireless_RF_Power_Data_DR[Sensors_CH_6];
#elif 	Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_6) 		= Wireless_RF_Power_Data_DR[Sensors_CH_6];		
#endif	
	
/* �ж� ͨ��7 */
#if		Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_7) 		= Wireless_RF_Power_Data_DR[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_7) 		= Wireless_RF_Power_Data_DR[Sensors_CH_7];
#elif 	Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_7) 		= Wireless_RF_Power_Data_DR[Sensors_CH_7];		
#endif	
	
/* �ж� ͨ��8 */
#if		Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_8) 		= Wireless_RF_Power_Data_DR[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_8) 		= Wireless_RF_Power_Data_DR[Sensors_CH_8];
#elif 	Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_8) 		= Wireless_RF_Power_Data_DR[Sensors_CH_8];
#endif	

/* �ж� ͨ��9 */
#if		Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_9) 		= Wireless_RF_Power_Data_DR[Sensors_CH_9];
#elif 	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_9) 		= Wireless_RF_Power_Data_DR[Sensors_CH_9];
#elif 	Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_9) 		= Wireless_RF_Power_Data_DR[Sensors_CH_9];
#endif	

/* �ж� ͨ��10 */
#if		Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_10) 		= Wireless_RF_Power_Data_DR[Sensors_CH_10];
#elif 	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_10) 		= Wireless_RF_Power_Data_DR[Sensors_CH_10];
#elif 	Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_10) 		= Wireless_RF_Power_Data_DR[Sensors_CH_10];
#endif	

/* �ж� ͨ��11 */
#if		Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_11) 		= Wireless_RF_Power_Data_DR[Sensors_CH_11];
#elif 	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_11) 		= Wireless_RF_Power_Data_DR[Sensors_CH_11];
#elif 	Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_11) 		= Wireless_RF_Power_Data_DR[Sensors_CH_11];
#endif	

/* �ж� ͨ��12 */
#if		Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_12) 		= Wireless_RF_Power_Data_DR[Sensors_CH_12];
#elif 	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_12) 		= Wireless_RF_Power_Data_DR[Sensors_CH_12];
#elif 	Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_12) 		= Wireless_RF_Power_Data_DR[Sensors_CH_12];
#endif	

/* �ж� ͨ��13 */
#if		Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_13) 		= Wireless_RF_Power_Data_DR[Sensors_CH_13];
#elif 	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_13) 		= Wireless_RF_Power_Data_DR[Sensors_CH_13];
#elif 	Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_13) 		= Wireless_RF_Power_Data_DR[Sensors_CH_13];
#endif	

/* �ж� ͨ��14 */
#if		Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_14) 		= Wireless_RF_Power_Data_DR[Sensors_CH_14];
#elif 	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_14) 		= Wireless_RF_Power_Data_DR[Sensors_CH_14];
#elif 	Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_14) 		= Wireless_RF_Power_Data_DR[Sensors_CH_14];
#endif	

/* �ж� ͨ��15 */
#if		Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_15) 		= Wireless_RF_Power_Data_DR[Sensors_CH_15];
#elif 	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_15) 		= Wireless_RF_Power_Data_DR[Sensors_CH_15];
#elif 	Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_15) 		= Wireless_RF_Power_Data_DR[Sensors_CH_15];
#endif	

/* �ж� ͨ��16 */
#if		Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_16) 		= Wireless_RF_Power_Data_DR[Sensors_CH_16];
#elif 	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_16) 		= Wireless_RF_Power_Data_DR[Sensors_CH_16];
#elif 	Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_16) 		= Wireless_RF_Power_Data_DR[Sensors_CH_16];
#endif	

/* �ж� ͨ��17 */
#if		Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_17) 		= Wireless_RF_Power_Data_DR[Sensors_CH_17];
#elif 	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_17) 		= Wireless_RF_Power_Data_DR[Sensors_CH_17];
#elif 	Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_17) 		= Wireless_RF_Power_Data_DR[Sensors_CH_17];
#endif	

/* �ж� ͨ��18 */
#if		Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_18) 		= Wireless_RF_Power_Data_DR[Sensors_CH_18];
#elif 	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_18) 		= Wireless_RF_Power_Data_DR[Sensors_CH_18];
#elif 	Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_18) 		= Wireless_RF_Power_Data_DR[Sensors_CH_18];
#endif	

/* �ж� ͨ��19 */
#if		Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_19) 		= Wireless_RF_Power_Data_DR[Sensors_CH_19];
#elif 	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_19) 		= Wireless_RF_Power_Data_DR[Sensors_CH_19];
#elif 	Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_19) 		= Wireless_RF_Power_Data_DR[Sensors_CH_19];
#endif	

/* �ж� ͨ��20 */
#if		Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_20) 		= Wireless_RF_Power_Data_DR[Sensors_CH_20];
#elif 	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_20) 		= Wireless_RF_Power_Data_DR[Sensors_CH_20];
#elif 	Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_20) 		= Wireless_RF_Power_Data_DR[Sensors_CH_20];
#endif	

/* �ж� ͨ��21 */
#if		Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_21) 		= Wireless_RF_Power_Data_DR[Sensors_CH_21];
#elif 	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_21) 		= Wireless_RF_Power_Data_DR[Sensors_CH_21];
#elif 	Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_21) 		= Wireless_RF_Power_Data_DR[Sensors_CH_21];
#endif	

/* �ж� ͨ��22 */
#if		Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_22) 		= Wireless_RF_Power_Data_DR[Sensors_CH_22];
#elif 	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_22) 		= Wireless_RF_Power_Data_DR[Sensors_CH_22];
#elif 	Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_22) 		= Wireless_RF_Power_Data_DR[Sensors_CH_22];
#endif	

/* �ж� ͨ��23 */
#if		Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_23) 		= Wireless_RF_Power_Data_DR[Sensors_CH_23];
#elif 	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_23) 		= Wireless_RF_Power_Data_DR[Sensors_CH_23];
#elif 	Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_23) 		= Wireless_RF_Power_Data_DR[Sensors_CH_23];
#endif	

/* �ж� ͨ��24 */
#if		Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_24) 		= Wireless_RF_Power_Data_DR[Sensors_CH_24];
#elif 	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_24) 		= Wireless_RF_Power_Data_DR[Sensors_CH_24];
#elif 	Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_24) 		= Wireless_RF_Power_Data_DR[Sensors_CH_24];
#endif	

/* �ж� ͨ��25 */
#if		Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_25) 		= Wireless_RF_Power_Data_DR[Sensors_CH_25];
#elif 	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_25) 		= Wireless_RF_Power_Data_DR[Sensors_CH_25];
#elif 	Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_25) 		= Wireless_RF_Power_Data_DR[Sensors_CH_25];
#endif	

/* �ж� ͨ��26 */
#if		Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_26) 		= Wireless_RF_Power_Data_DR[Sensors_CH_26];
#elif 	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_26) 		= Wireless_RF_Power_Data_DR[Sensors_CH_26];
#elif 	Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_26) 		= Wireless_RF_Power_Data_DR[Sensors_CH_26];
#endif	

/* �ж� ͨ��27 */
#if		Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_27) 		= Wireless_RF_Power_Data_DR[Sensors_CH_27];
#elif 	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_27) 		= Wireless_RF_Power_Data_DR[Sensors_CH_27];
#elif 	Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_27) 		= Wireless_RF_Power_Data_DR[Sensors_CH_27];
#endif	

/* �ж� ͨ��28 */
#if		Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_28) 		= Wireless_RF_Power_Data_DR[Sensors_CH_28];
#elif 	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_28) 		= Wireless_RF_Power_Data_DR[Sensors_CH_28];
#elif 	Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_28) 		= Wireless_RF_Power_Data_DR[Sensors_CH_28];
#endif	

/* �ж� ͨ��29 */
#if		Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_29) 		= Wireless_RF_Power_Data_DR[Sensors_CH_29];
#elif 	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_29) 		= Wireless_RF_Power_Data_DR[Sensors_CH_29];
#elif 	Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_29) 		= Wireless_RF_Power_Data_DR[Sensors_CH_29];
#endif	

/* �ж� ͨ��30 */
#if		Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_30) 		= Wireless_RF_Power_Data_DR[Sensors_CH_30];
#elif 	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_30) 		= Wireless_RF_Power_Data_DR[Sensors_CH_30];
#elif 	Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_30) 		= Wireless_RF_Power_Data_DR[Sensors_CH_30];
#endif	

/* �ж� ͨ��31 */
#if		Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_31) 		= Wireless_RF_Power_Data_DR[Sensors_CH_31];
#elif 	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_31) 		= Wireless_RF_Power_Data_DR[Sensors_CH_31];
#elif 	Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_31) 		= Wireless_RF_Power_Data_DR[Sensors_CH_31];
#endif	

/* �ж� ͨ��32 */
#if		Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp
		*(power_data_buffer + Sensors_CH_32) 		= Wireless_RF_Power_Data_DR[Sensors_CH_32];
#elif 	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp
		*(power_data_buffer + Sensors_CH_32) 		= Wireless_RF_Power_Data_DR[Sensors_CH_32];
#elif 	Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum
		*(power_data_buffer + Sensors_CH_32) 		= Wireless_RF_Power_Data_DR[Sensors_CH_32];
#endif	

}

/************************************************************************************************/
/* ������	: RF_Card_Get_Power_State													 	 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ���ݸ����ĵ������ݼ������ǰ��Ƭ����״̬��BOOL��FLASHǷѹ��TRUE����������			 		*/
/* ע��	 	: ���ڵĵ��º���ʪ�ȿ�Ƭ���㿨Ƭ�������㷨��ͬ����ʱ���������������������㷨�ǲ�ͬ��	 		*/
/* �����㷨	: VCC= (BV/256)*3.3����ѹС�� 2.5VΪǷѹ.	 											*/
/* ��������	: 2015/05/21																		*/
/************************************************************************************************/
BOOL RF_Card_Get_Power_State(u8 power_data)
{
/* ��Ƭ�Ļ�׼��ѹ */
#define  RF_Card_VREF	(3.30)
/* ��Ƭ�ķ�ѹ���� */
#define  RF_Card_B		(256u)
/* ��Ƭ����״̬��ֵ */
#define  RF_Card_Power_State_HT	(2.50)
	
	/* �������״̬ */
	return((((float)power_data*RF_Card_VREF)/RF_Card_B)>RF_Card_Power_State_HT)?TRUE:FALSE;
	
#undef  RF_Card_VREF
#undef  RF_Card_B
#undef	RF_Card_Power_State_HT
}

/************************************************************************************************/
/* ������	: RF_Card_Rece_Data_State_Check													 	*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ������ÿ�Ƭͨ�������ݽ����Ƿ񳬳��趨ʱ�䣬���糬����������ΪERR��				 		*/
/* ��������	: 2015/05/25																		*/
/************************************************************************************************/
void RF_Card_Rece_Data_State_Check(void)
{
/* �ж� �����������Ƿ����� */
#if (Wireless_RF_Card_Config_Enable == 0x01)

	/* �ж� ͨ��1 */
	#if		(Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��1 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_1] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_1] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_1]++;
		}
	#endif		

	/* �ж� ͨ��2 */
	#if		(Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��2 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_2] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_2] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_2]++;
		}
	#endif	

	/* �ж� ͨ��3 */
	#if		(Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��3 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_3] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_3] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_3]++;
		}
	#endif	

	/* �ж� ͨ��4 */
	#if		(Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��4 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_4] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_4] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_4]++;
		}
	#endif			

	/* �ж� ͨ��5 */
	#if		(Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��5 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_5] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_5] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_5]++;
		}
	#endif		

	/* �ж� ͨ��6 */
	#if		(Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��6 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_6] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_6] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_6]++;
		}
	#endif	

	/* �ж� ͨ��7 */
	#if		(Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��7 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_7] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_7] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_7]++;
		}
	#endif	

	/* �ж� ͨ��8 */
	#if		(Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��8 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_8] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_8] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_8]++;
		}
	#endif		

	/* �ж� ͨ��9 */
	#if		(Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��9 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_9] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_9] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_9]++;
		}
	#endif		

	/* �ж� ͨ��10 */
	#if		(Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��10 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_10] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_10] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_10]++;
		}
	#endif	

	/* �ж� ͨ��11 */
	#if		(Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��11 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_11] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_11] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_11]++;
		}
	#endif	

	/* �ж� ͨ��12 */
	#if		(Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��12 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_12] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_12] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_12]++;
		}
	#endif		

	/* �ж� ͨ��13 */
	#if		(Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��13 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_13] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_13] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_13]++;
		}
	#endif		

	/* �ж� ͨ��14 */
	#if		(Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��14 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_14] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_14] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_14]++;
		}
	#endif	

	/* �ж� ͨ��15 */
	#if		(Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��15 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_15] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_15] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_15]++;
		}
	#endif	

	/* �ж� ͨ��16 */
	#if		(Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��16 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_16] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_16] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_16]++;
		}
	#endif		
		
	/* �ж� ͨ��17 */
	#if		(Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��17 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_17] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_17] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_17]++;
		}
	#endif		

	/* �ж� ͨ��18 */
	#if		(Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��18 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_18] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_18] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_18]++;
		}
	#endif	

	/* �ж� ͨ��19 */
	#if		(Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��19 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_19] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_19] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_19]++;
		}
	#endif	

	/* �ж� ͨ��20 */
	#if		(Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��20 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_20] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_20] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_20]++;
		}
	#endif			

	/* �ж� ͨ��21 */
	#if		(Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��21 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_21] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_21] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_21]++;
		}
	#endif		

	/* �ж� ͨ��22 */
	#if		(Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��22 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_22] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_22] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_22]++;
		}
	#endif	

	/* �ж� ͨ��23 */
	#if		(Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��23 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_23] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_23] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_23]++;
		}
	#endif	

	/* �ж� ͨ��24 */
	#if		(Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��24 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_24] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_24] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_24]++;
		}
	#endif		

	/* �ж� ͨ��25 */
	#if		(Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��25 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_25] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_25] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_25]++;
		}
	#endif		

	/* �ж� ͨ��26 */
	#if		(Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��26 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_26] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_26] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_26]++;
		}
	#endif	

	/* �ж� ͨ��27 */
	#if		(Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��27 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_27] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_27] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_27]++;
		}
	#endif	

	/* �ж� ͨ��28 */
	#if		(Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��28 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_28] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_28] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_28]++;
		}
	#endif		

	/* �ж� ͨ��29 */
	#if		(Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��29 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_29] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_29] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_29]++;
		}
	#endif		

	/* �ж� ͨ��30 */
	#if		(Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��30 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_30] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_30] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_30]++;
		}
	#endif	

	/* �ж� ͨ��31 */
	#if		(Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��31 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_31] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_31] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_31]++;
		}
	#endif	

	/* �ж� ͨ��32 */
	#if		(Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
			(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
			(Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
		/* �ж� ͨ��32 �Ƿ�ʱ  */
		if(Wireless_RF_Card_Data_Rece_Time[Sensors_CH_32] >= Wireless_RF_Card_Data_Rece_Timeout_Time)
		{
			/* ���տ�Ƭ�����Գ�ʱ�����ö�Ӧͨ��������Ϊ��ʱ���� */
			Wireless_RF_Card_Collection_Data_DR[Sensors_CH_32] = Wireless_RF_Card_Data_Rece_Timeout_Para;
		}else
		{
			/* ���տ�Ƭ����δ��ʱ��ʱ������ۼ� */
			Wireless_RF_Card_Data_Rece_Time[Sensors_CH_32]++;
		}
	#endif			
		

	
#endif
}

/*************************************************************************************************/
/* ������	:RF_Card_Get_Single_Tmp_Card_Data													 */
/* ����		:���¿�Ƭ���¶�����ֵ																	 */
/* ���		:���¿�Ƭ���¶Ȳ���ֵ																	 */
/* ����		:�����¿�Ƭ���¶�����ת����ʵ�ʲ���ֵ												 	 */
/* ��������	:2015/08/14																			 */
/*************************************************************************************************/
float RF_Card_Get_Single_Tmp_Card_Data(float tmp_data)
{
/* ���¿�Ƭ�¶Ȳ�����Χ�����ֵ��80�棩��������Ҫ��ĵ����ݷ�ΧҪ��Ķࣨ1000�棩�� */
#define Wireless_RF_Card_Single_Tmp_Data_MAX_Para	(10500u)
/* ��Ƭ���ݳ����趨��Χʱ�ϴ���������������ֵ */
#define Wireless_RF_Card_Single_Tmp_Data_MAX_Value	(1000.0)



	/* �жϵ��¿�Ƭ�¶�����ֵ */
	if(tmp_data == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* ����ֵ�������ݳ�ʱֵ������ʵ���¶�ֵΪһ���ض��Ĳ������ṩ��λ�����ʶ�� */
		__nop();
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	//��������Ĳ���ֻ��һ�� ��ʱ���ݣ����ܾ���˵��:��ָ����һ��ʱ����û���յ���Ӧ��Ƭ�����ݣ���
	/* �ж������Ƿ������������̷�Χ�� */
	else if(tmp_data < Wireless_RF_Card_Single_Tmp_Data_MAX_Para)
	{
		/* ���¶����̷�Χ�� */
		__nop();
		return (((float)tmp_data-500.0)/10.0);
	}else
	{
		/* �������ݣ����¿�Ƭ�����쳣(���ݴ��ڵ������ֵ) */
		__nop();
		return Wireless_RF_Card_Single_Tmp_Data_MAX_Value;
	}
	
#undef Wireless_RF_Card_Single_Tmp_Data_MAX_Para
#undef Wireless_RF_Card_Single_Tmp_Data_MAX_Value
}

/*************************************************************************************************/
/* ������	:RF_Card_Get_Humiture_Tmp_Card_Data												 	 */
/* ����		:��ʪ��Ƭ���¶�����ֵ																	 */
/* ���		:��ʪ��Ƭ���¶Ȳ���ֵ																	 */
/* ����		:����ʪ��Ƭ���¶�����ת����ʵ�ʲ���ֵ												 	 */
/* ��������	:2015/05/25																			 */
/*************************************************************************************************/
float  RF_Card_Get_Humiture_Tmp_Card_Data(float tmp_data)
{
/* 
	��ʪ��Ƭ�¶Ȳ�����Χ�����ֵ��80�棩��������Ҫ��ĵ����ݷ�ΧҪ��Ķࣨ1000�棩,������������2�ֽڵ�tmp_data�������Ϊ65535��
	�ܹ��������¶ȷ�ΧҲ��ֻ�ܴﵽ128.8�档	
*/
	/* ���¿�Ƭ�¶Ȳ�����Χ�����ֵ��80�棩��������Ҫ��ĵ����ݷ�ΧҪ��Ķࣨ1000�棩�� */
#define Wireless_RF_Card_Humiture_Tmp_Data_MAX_Para		(65535u)
	/* ��Ƭ���ݳ����趨��Χʱ�ϴ���������������ֵ */
#define Wireless_RF_Card_Humiture_Tmp_Data_MAX_Value	(1000.0)



	/* �жϵ��¿�Ƭ�¶�����ֵ	 */
	if(tmp_data == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* ����ֵ�������ݳ�ʱֵ������ʵ���¶�ֵΪһ���ض��Ĳ������ṩ��λ�����ʶ�� */
		__nop();
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	//��������Ĳ���ֻ��һ�� ��ʱ���ݣ����ܾ���˵��:��ָ����һ��ʱ����û���յ���Ӧ��Ƭ�����ݣ���
	/* �ж������Ƿ������������̷�Χ�� */
	else if(tmp_data < Wireless_RF_Card_Humiture_Tmp_Data_MAX_Para)
	{
		/* ���¶����̷�Χ�� */
		__nop();
		return (((float)tmp_data*175.72)/65536.0-46.85);
	}else
	{
		/* �������ݣ����¿�Ƭ�����쳣(���ݴ��ڵ������ֵ) */
		__nop();
		return Wireless_RF_Card_Humiture_Tmp_Data_MAX_Value;
	}
	
#undef Wireless_RF_Card_Humiture_Tmp_Data_MAX_Para
#undef Wireless_RF_Card_Humiture_Tmp_Data_MAX_Value

}

/*************************************************************************************************/
/* ������	:RF_Card_Get_Humiture_Hum_Card_Data												 	 */
/* ����		:��ʪ��Ƭ���¶�����ֵ																	 */
/* ���		:��ʪ��Ƭ���¶Ȳ���ֵ																	 */
/* ����		:����ʪ��Ƭ��ʪ������ת����ʵ�ʲ���ֵ												 	 */
/* ��������	:2015/05/25																			 */
/*************************************************************************************************/
float  RF_Card_Get_Humiture_Hum_Card_Data(float hum_data)
{
/* ��ʪ��Ƭ�¶Ȳ�����Χ�����ֵ��100%�� */
#define Wireless_RF_Card_Humiture_Hum_Data_MAX	(55575u)
/* ʪ�ȵ����ֵ 100% */
#define Hum_Data_Value_MAX	(100.0)

	/* �жϵ�ʪ��Ƭʪ������ֵ	 */
	if(hum_data == Wireless_RF_Card_Data_Rece_Timeout_Para)
	{
		/* ����ֵ�������ݳ�ʱֵ������ʵ���¶�ֵΪһ���ض��Ĳ������ṩ��λ�����ʶ�� */
		__nop();
		return Wireless_RF_Card_Data_Rece_Timeout_Para;
	}
	//��������Ĳ���ֻ��һ�� ��ʱ���ݣ����ܾ���˵��:��ָ����һ��ʱ����û���յ���Ӧ��Ƭ�����ݣ���
	/* �ж������Ƿ������������̷�Χ�� */
	else if(hum_data <= Wireless_RF_Card_Humiture_Hum_Data_MAX)
	{
		/* ���¶����̷�Χ�� */
		__nop();
		return ((((float)hum_data*125.0)/65536.0)-6);
	}else
	{
		/* �������ݣ���ʪ��Ƭ�����쳣(���ݴ��ڵ������ֵ) */
		__nop();
		return Hum_Data_Value_MAX;
	}
	
	
#undef Wireless_RF_Card_Humiture_Hum_Data_MAX
#undef Hum_Data_Value_MAX
}

#endif


/******************* (C) HZ Zeda Team *****END OF FILE****/
