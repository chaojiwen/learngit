/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/22																			 */
/*************************************************************************************************/
#include "Log_Battery.h"
#include "Ks0108.h"
#include "Menu.H"


// ��ȡGPIO�ڵ�ƽֵӦʹ��STM32�⺯��������Ӧֱ�Ӷ�ȡ�Ĵ���(ĳЩIO�ڿ���ͨ��ֱ�Ӷ�ȡ�Ĵ�����ȷ��������Щ����)
#define Battery_Miss_Check()      GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)	

/* ��ص���%0ֵ����Ӧ��ADCֵ */
#define Battery_Level_0		(2080ul)
/* ��ص���%100ֵ����Ӧ��ADCֵ */
#define Battery_Level_100	(2638ul)

static void Battery_Charge_Display(void);


/*
*********************************************************************************************************
*	�� �� ��: Check_Battery_State
*	����˵��: z���ڼ���ص�ѹ
*	��    �Σ���
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��50MS���һ�Ρ�
*********************************************************************************************************
*/
void Power_BT_Quantity_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* ����ֵ */
	uint8_t powerValue;
	/* ��ص���״̬�ۼƼ����� */
	static u16 bt_quantity_state_cnt = 0;
	/* �״ε�Դ����־ */
	static BOOL powerFirstCheckFlag = FALSE;
	
	
	/* �жϵ�ǰǰ��ص��� */
	if(Battery_Level_0 >= power_f->Inf.BT_Quantity_Sampl_Value)
	{
		/* ��������0%������0% */
		powerValue = 0;
	}else if(Battery_Level_100 <= power_f->Inf.BT_Quantity_Sampl_Value)
	{
		/* ��������100%������100% */
		powerValue = 100;
	}else
	{
		/* ���㵱ǰ���� */
		powerValue = (((uint32_t)power_f->Inf.BT_Quantity_Sampl_Value-Battery_Level_0)*100)/(Battery_Level_100-Battery_Level_0);
	}
	
	/* �жϵ�ǰ�Ƿ�Ϊ��һ�ε�ص������ */
	if(powerFirstCheckFlag == FALSE)
	{
		/* ���õ�ǰ����Ϊ�����ĵ��� */
		power_t->Inf_State.BT_Quantity_State = powerValue;
		/* �����״ε�Դ����־ */
		powerFirstCheckFlag = TRUE;
		return;
	}
	
	/* �ж� ��ǰ�ĵ��� */
	if(power_t->Inf_State.BT_Quantity_State != powerValue)
	{
		/* ����  ��ص���״̬�ۼƼ����� */
		bt_quantity_state_cnt++;
		/* �ж� ��ص���״̬�ۼƼ����� ֵ �Ƿ�����趨ֵ */
		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
		{
			/* ��ص���״̬�ۼƼ�����ֵ�����趨ֵ */
			/* �����ص���״̬�ۼƼ����� */
			bt_quantity_state_cnt = 0;
			/* ���õ�ص���״̬ */
			power_t->Inf_State.BT_Quantity_State = powerValue;
		}
	}else 
	{
		/* �ж� ��ص���״̬�ۼƼ����� �Ƿ������ֵ */
		if(bt_quantity_state_cnt)
		{
			/* ��ص���״̬�ۼƼ�������ֵ */
			/* ��� ��ص���״̬�ۼƼ�����ֵ */
			bt_quantity_state_cnt = 0;
		}
	}

}

/*
*********************************************************************************************************
*	�� �� ��: Check_Battery_State
*	����˵��: z���ڼ���ص�ѹ
*	��    �Σ���
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��50MS���һ�Ρ�
*********************************************************************************************************
*/
//void Power_BT_Quantity_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
//{
/////* δ�� ��� *///....-3.0V
////#define Battery_Miss					((u16)1861)
///* ��ص���0��׼ *///....-3.5V			������GSMģ����͵Ĺ�����ѹΪ3.4+��������������Ϊ3.5V����֤GSMģ���ܹ�������������
//#define Battery_Quantity_Null_Base		((u16)2173)
///* ��ص���1��׼ *///3.5V-3.65V
//#define Battery_Quantity_One_Base		((u16)2265)
///* ��ص���2��׼ *///3.65V-3.8V
//#define Battery_Quantity_Two_Base		((u16)2358)
///* ��ص���3��׼ *///3.8V-3.95V
//#define Battery_Quantity_There_Base		((u16)2451)
///* ��ص���4��׼ *///3.95V-4.25V
//#define Battery_Quantity_Four_Base		((u16)2638)


//	/* ��ص���״̬�ۼƼ����� */
//	static u16 bt_quantity_state_cnt = 0;
////	/* �鿴��ǰ״̬ */
////	if((MCU_Power.Link_State == On)&&\
////	   ((Battery*)(b_Battery->Sensor))->State != Power_Full)
////	{  
////		((Battery*)(b_Battery->Sensor))->CNT++;
////		if(((Battery*)(b_Battery->Sensor))->CNT > ((Battery*)(b_Battery->Sensor))->Filter_Time)
////		{
////			((Battery*)(b_Battery->Sensor))->CNT = 0;
////			((Battery*)(b_Battery->Sensor))->State = Power_Full;
////		}	
////	}else 
//	/* �ж� ��ǰ�ĵ��� */
//	if((power_f->Inf.BT_Quantity_Sampl_Value >= Battery_Quantity_There_Base)&&
//	   (power_t->Inf_State.BT_Quantity_State != Power_Four))
//	{	
//		/* ��ǰ������4���(����)״̬ */
//		/* ����  ��ص���״̬�ۼƼ����� */
//		bt_quantity_state_cnt++;
//		/* �ж� ��ص���״̬�ۼƼ����� ֵ �Ƿ�����趨ֵ */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* ��ص���״̬�ۼƼ�����ֵ�����趨ֵ */
//			/* �����ص���״̬�ۼƼ����� */
//			bt_quantity_state_cnt = 0;
//			/* ���õ�ص���״̬ */
//			power_t->Inf_State.BT_Quantity_State = Power_Four;
//		}
//	}else if((power_f->Inf.BT_Quantity_Sampl_Value <  Battery_Quantity_There_Base)&&
//			 (power_f->Inf.BT_Quantity_Sampl_Value >= Battery_Quantity_Two_Base)&&
//			 (power_t->Inf_State.BT_Quantity_State != Power_There))
//	{	
//		/* ��ǰ������3���״̬ */
//		/* ����  ��ص���״̬�ۼƼ����� */
//		bt_quantity_state_cnt++;
//		/* �ж� ��ص���״̬�ۼƼ����� ֵ �Ƿ�����趨ֵ */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* ��ص���״̬�ۼƼ�����ֵ�����趨ֵ */
//			/* �����ص���״̬�ۼƼ����� */
//			bt_quantity_state_cnt = 0;
//			/* ���õ�ص���״̬ */
//			power_t->Inf_State.BT_Quantity_State = Power_There;
//		}
//	}else if((power_f->Inf.BT_Quantity_Sampl_Value  <  Battery_Quantity_Two_Base)&&\
//			 (power_f->Inf.BT_Quantity_Sampl_Value  >= Battery_Quantity_One_Base)&&\
//			 (power_t->Inf_State.BT_Quantity_State  != Power_Two))
//	{	
//		/* ��ǰ������3���״̬ */
//		/* ����  ��ص���״̬�ۼƼ����� */
//		bt_quantity_state_cnt++;
//		/* �ж� ��ص���״̬�ۼƼ����� ֵ �Ƿ�����趨ֵ */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* ��ص���״̬�ۼƼ�����ֵ�����趨ֵ */
//			/* �����ص���״̬�ۼƼ����� */
//			bt_quantity_state_cnt = 0;
//			/* ���õ�ص���״̬ */
//			power_t->Inf_State.BT_Quantity_State= Power_Two;
//		}
//	}else if((power_f->Inf.BT_Quantity_Sampl_Value < Battery_Quantity_One_Base)&&\
//			 (power_f->Inf.BT_Quantity_Sampl_Value >= Battery_Quantity_Null_Base)&&\
//			 (power_t->Inf_State.BT_Quantity_State != Power_One))
//	{	
//		/* ��ǰ������3���״̬ */
//		/* ����  ��ص���״̬�ۼƼ����� */
//		bt_quantity_state_cnt++;
//		/* �ж� ��ص���״̬�ۼƼ����� ֵ �Ƿ�����趨ֵ */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* ��ص���״̬�ۼƼ�����ֵ�����趨ֵ */
//			/* �����ص���״̬�ۼƼ����� */
//			bt_quantity_state_cnt = 0;
//			/* ���õ�ص���״̬ */
//			power_t->Inf_State.BT_Quantity_State = Power_One;
//		}
//	}else if((power_f->Inf.BT_Quantity_Sampl_Value < Battery_Quantity_Null_Base)&&\
//			 (power_t->Inf_State.BT_Quantity_State != Power_Null))
//	{	
//		/* ��ǰ������3���״̬ */
//		/* ����  ��ص���״̬�ۼƼ����� */
//		bt_quantity_state_cnt++;
//		/* �ж� ��ص���״̬�ۼƼ����� ֵ �Ƿ�����趨ֵ */
//		if(bt_quantity_state_cnt > power_f->Inf.BT_Status_Update_Time)
//		{
//			/* ��ص���״̬�ۼƼ�����ֵ�����趨ֵ */
//			/* �����ص���״̬�ۼƼ����� */
//			bt_quantity_state_cnt = 0;
//			/* ���õ�ص���״̬ */
//			power_t->Inf_State.BT_Quantity_State = Power_Null;
//		}
//	}else 
//	{
//		/* �ж� ��ص���״̬�ۼƼ����� �Ƿ������ֵ */
//		if(bt_quantity_state_cnt)
//		{
//			/* ��ص���״̬�ۼƼ�������ֵ */
//			/* ��� ��ص���״̬�ۼƼ�����ֵ */
//			bt_quantity_state_cnt = 0;
//		}
//	}
//	
//#undef Battery_Quantity_Null_Base
//#undef Battery_Quantity_One_Base
//#undef Battery_Quantity_Two_Base
//#undef Battery_Quantity_There_Base
//#undef Battery_Quantity_Four_Base
//}

/*
*********************************************************************************************************
*	�� �� ��: Power_BT_Charge_State_Check
*	����˵��: ����ϵͳ����
*	��    �Σ���
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��50MS���һ�Ρ�
*********************************************************************************************************
*/
void Power_BT_Charge_State_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
#define BT_Link_On		(Bit_RESET)
#define BT_Link_Off		(Bit_SET)	
	/* ������Ӷ˿�״̬ */
	u8 battery_pin_state;
	/* ���״̬�ۼƼ����� */
	static u16 bt_charge_cnt = 0;	
	
	/* ��ȡGPIO�ڵ�״̬ */
	battery_pin_state = Battery_Miss_Check();

	/* �ж� ��ǰ ��س��״̬ */
	if(power_t->Inf_State.BT_Charge_State == TRUE)
	{
		/* ��ǰ���ڳ��״̬ */
		/* �ж� ״̬ �Ƿ�ı䣨����磩 */
		if(battery_pin_state == BT_Link_On)
		{
			/* �˿ڼ�⵽������ */
			/* ���� ���״̬�ۼƼ����� */
			bt_charge_cnt++;
			if(bt_charge_cnt > power_f->Inf.BT_Status_Update_Time)
			{
				/* ������� */
				bt_charge_cnt = 0;	
				/* ����״̬ */
				power_t->Inf_State.BT_Charge_State = FALSE;
			}
		}else
		{
			/* ������� */
			/* ���� ��Դ���˿ڿ��� ��������źţ������Ҫ�������� */
			if(bt_charge_cnt)
			{
				/* ���� ���״̬�ۼƼ����� ��ֵ */
				bt_charge_cnt--;
			}	
		}
	}else
	{
		/* ��������δ���״̬ */
		/* �ж� ״̬ �Ƿ�ı䣨��磩 */
		if(battery_pin_state == BT_Link_Off)
		{
			/* �˿ڼ�⵽����ź� */
			/* ���� ���״̬�ۼƼ����� */
			bt_charge_cnt++;
			if(bt_charge_cnt > power_f->Inf.BT_Status_Update_Time)
			{
				/* ������� */
				bt_charge_cnt = 0;	
				/* ����״̬ */
				power_t->Inf_State.BT_Charge_State = TRUE;
			}
		}else
		{
			/* ���� ��Դ���˿ڿ��� ��������źţ������Ҫ�������� */
			if(bt_charge_cnt)
			{
				/* ���� ���״̬�ۼƼ����� ��ֵ */
				bt_charge_cnt--;
			}	
		}
	} 	
}


/*
*********************************************************************************************************
*	�� �� ��: Battery_Power_Display
*	����˵��: ��ص�����ʾ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Battery_Power_Display(void)
{
	/* �ж� ��ӵ� �Ƿ����� */
	if(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Link_State == TRUE)
	{
		/* ��Դ���� */
		/* �ж� ��ص����Ƿ����� */
		if(Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Charge_State == FALSE)
		{
			/* ��ص���δ�� */
			/* ������ڳ�� */
			Battery_Charge_Display();
		}else
		{
			/* ��ص������� */
			/* ����Ѿ������� */
			GUI_Draw_Bar_BMP(Battery_Power_Index[Power_Full],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}
	}else
	{
		/* ���δ�ӣ���ʾ��ص�ǰ����. *///����δ����磬ֻ�нӵ��ϵͳ���п��ܹ�����
		if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[0],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
				 (Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[1],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
				 (Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[2],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
				 (Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[3],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else if((Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
				 (Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[4],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}else
		{
			GUI_Draw_Bar_BMP(Battery_Power_Index[4],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
		}
		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Battery_Charge_Display
*	����˵��: ���״̬��ʾ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Battery_Charge_Display(void)
{
	static u8 num = 0;
	
	GUI_Draw_Bar_BMP(Battery_Power_Index[num++],Battery_Power_BMP_PosX,Battery_Power_BMP_PosY);
	if(num > Power_Four)
	{
		num = Power_Null;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Power_BT_SET_Alarm_State
*	����˵��: ��ص����ı���״̬����
*	��    �Σ�power_t: ��ص�������״̬;power_f:��ʱû��ʹ�ã�
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��50MS���һ�Ρ�
*********************************************************************************************************
*/
void Power_BT_SET_Alarm_State(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	/* �жϵ�ǰ��ص���״̬ */
	if(power_t->Inf_State.BT_Quantity_State <= 10)
	{
		/* ��ص������� */
		/* ���� ��ص������� */
		power_t->Inf_State.BT_Alarm_State =	TRUE; 
	}else if((power_t->Inf_State.BT_Quantity_State > 10)&&(power_t->Inf_State.BT_Quantity_State<=100))
	{
		/* ��ص������� */
		/* ���� ������ */
		power_t->Inf_State.BT_Alarm_State =	FALSE; 
	}else
	{
		/* δ����״̬ */
		__nop();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Power_BT_Alarm_Check
*	����˵��: ��ص�������״̬���
*	��    �Σ�power_t: ��ص�������״̬;power_f:��ʱû��ʹ�ã�
*	�� �� ֵ: ��
*	˵��	������ϵͳ�м��50MS���һ�Ρ�
*********************************************************************************************************
*/
u8 Power_BT_Alarm_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f)
{
	
	/* ͨ������״̬ */
	u8 ch_alarm_sr = 0;
	
	/* �ж� ��ص����͵ı���״̬ */
	if(power_t->Inf_State.BT_Alarm_State  == TRUE)
	{
		/* ��ǰ�������Ϊ����״̬ */
		/* ���ݵ�ǰ��Ϣ���ã����ñ������ݡ� */
		ch_alarm_sr |= ((power_t->Inf_State.BT_Sound_Alarm_Switch == TRUE) ? 0X1:0X0);
		ch_alarm_sr |= ((power_t->Inf_State.BT_Light_Alarm_Switch == TRUE) ? 0X2:0X0);
		ch_alarm_sr |= ((power_t->Inf_State.BT_SMS_Alarm_Switch   == TRUE) ? 0X4:0X0);

	}
	
	return ch_alarm_sr;
}

/************************************************************************************************/
/* ������	: Battery_Para_Init															 	 	*/
/* ����  	: *bt_state: ���״ָ̬�롣															*/
/* ���		: �� 																				*/
/* ����	 	: ��ʼ�����õ��״̬����							 									*/
/* �������� 	: 2015/08/13																		*/
/************************************************************************************************/
void Battery_Para_Init(INSTRU_POWER *bt_state)
{
	
	
	/* ���õ�س��״̬��û�ڳ�� */
	bt_state->Inf_State.BT_Charge_State = FALSE;
	/* ���ó�ʼ����״̬������ */
	bt_state->Inf_State.BT_Quantity_State = 100;
	/* ���ñ���״̬��Ϊ���� */
	bt_state->Inf_State.BT_Alarm_State = FALSE;
}

///************************************************************************************************/
///* ������	: Power_Record_Function_Check												 	 	*/
///* ����  	: pwr_vle:��Դ��ѹ�ɼ�ֵ.																*/
///* ���		: TRUE:����״̬��FALSE:ֹͣ״̬.														*/
///* ����	 	: ����Դ��ѹ,����ѹ����3.5Vʱ������ֹͣ��¼���ܡ�	 									*/
///* �������� 	: 2015/10/10																		*/
///************************************************************************************************/
//BOOL Power_Record_Function_Check(u16 pwr_vle)
//{
///* ��ص���0��׼ */		
//#define Battery_Quantity_Null_Base		((u16)2173)		//3.5V	
//	
//	
//	/* �жϵ�Դ��ѹֵ */
//	if(pwr_vle < Battery_Quantity_Null_Base)
//	{
//		/* ��Դ��ѹֵС��3.5V������Ӧ��ֹͣ��¼����. */
//		return FALSE;
//	}
//	
//	return TRUE;
//	
//}



/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

