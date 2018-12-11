/************************************************************************************************/
/** @file              : App_GPS.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��8��11��															*/
/** @brief             : c file																	*/
/** @description       : GPS��λӦ���ļ�														*/
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


#include "App_GPS.h"





/************************************************************************************************/
/* GPS��λӦ�����ݼĴ��� */
INSTRU_GPS_POS_App_DR GPS_POS_App_DR = 
{
	/* γ�� */
	GPS_Data_Rece_Timeout_Value,
	/* ���� */
	GPS_Data_Rece_Timeout_Value,
	/* �ٶ� */
	GPS_Data_Rece_Timeout_Value,
};


/************************************************************************************************/
/* GPS��λӦ��״̬ */
INSTRU_GPS_POS_App_SR GPS_POS_App_SR =
{
	/* γ�����ݸ���״̬ */
	FALSE,
	/* �������ݸ���״̬ */
	FALSE,
	/* �ٶ����ݸ���״̬ */
	FALSE,

};


/************************************************************************************************/
/* ������	: GPS_App_Set_Rece_Timeout_Para														*/
/* ����		: ��			 																	*/
/* ���		: �� 																				*/
/* ����		: ����GPS���ճ�ʱӦ�ò���															*/
/* ��������	: 2015/8/12																			*/
/************************************************************************************************/
void GPS_App_Set_Rece_Timeout_Para(void)
{

	/* ���� γ�� ���� */
	GPS_POS_App_DR.Lat = GPS_Data_Rece_Timeout_Value;
	
	/* ���� ���� ���� */
	GPS_POS_App_DR.Lon = GPS_Data_Rece_Timeout_Value;
	
	/* ���� �ٶ� ���� */
	GPS_POS_App_DR.Spd = GPS_Data_Rece_Timeout_Value;

	/* ��� γ�����ݸ���״̬ */
	GPS_POS_App_SR.Lat_Updata_State = FALSE;
	
	/* ��� �������ݸ���״̬ */
	GPS_POS_App_SR.Lon_Updata_State = FALSE;

	/* ��� �ٶ����ݸ���״̬ */
	GPS_POS_App_SR.Spd_Updata_State = FALSE;


}


/************************************************************************************************/
/* ������	: GPS_App_Get_Data														 	 		*/
/* ����  	: ��			 																	*/
/* ���		: �� 																				*/
/* ����	 	: ��ȡGPSӦ�ò���Ҫ������						 									*/
/* ��������	: 2015/08/11																		*/
/************************************************************************************************/
void GPS_App_Get_Data(INSTRU_GPS_POS_Log_SR *state,INSTRU_GPS_POS *data)
{



	//����ֱ�ӻ�õ�����Ч�����õĻ��͵�����Ӧ�Ĵ���

	/* �жϵ�ǰ��GPS���� ���� ״̬�� */
	if(state->Ana_State == TRUE)
	{
		/* �����ɹ� */

		/* ���� γ�� ���� */
		GPS_POS_App_DR.Lat = data->Lat;
		
		/* ���� ���� ���� */
		GPS_POS_App_DR.Lon = data->Lon;
		
		/* ���� �ٶ� ���� */
		GPS_POS_App_DR.Spd = data->Spd;
		
//		/* ���� γ�� ���� */
//		GPS_POS_App_DR.Lat = -10;
//		
//		/* ���� ���� ���� */
//		GPS_POS_App_DR.Lon = -10;
//		
//		/* ���� �ٶ� ���� */
//		GPS_POS_App_DR.Spd = data->Spd;

		/* ����γ�����ݸ���״̬ */
		GPS_POS_App_SR.Lat_Updata_State = TRUE;
		
		/* ���þ������ݸ���״̬ */
		GPS_POS_App_SR.Lon_Updata_State = TRUE;
		
		/* �����ٶ����ݸ���״̬ */
		GPS_POS_App_SR.Spd_Updata_State = TRUE;
		
		/* ��0 GPS���ݽ��ճ�ʱ�Ĵ��� */
     	state->Rece_Timeout_Reg = 0;
	}else
	{
		/* ����ʧ�� */

		/* ���GPS��λ���� */
		GPS_Log_RESET_POS_Inf();
		
		/* �ж�GPSӦ������״̬ */
		if((GPS_POS_App_SR.Lat_Updata_State == TRUE)&&
		   (GPS_POS_App_SR.Lon_Updata_State == TRUE)&&
		   (GPS_POS_App_SR.Spd_Updata_State == TRUE))
		{
			/* ��ǰ����Ϊ�µ����ݣ��ȴ�Ӧ�ö�ȡ�� */
			__nop();
		}else
		{
			/* ��ǰ����Ϊ�ɵ����ݣ��ȴ����¡� */

			/* �ж����ݽ��ճ�ʱ */
			if(state->Rece_Timeout_Reg >= GPS_Data_Rece_Timeout)
			{
				/* ���ݽ��ճ�ʱ */
				/* ����GPS���ճ�ʱ�߼����� */
				GPS_Log_Set_Rece_Timeout_Para();
				/* ����GPS���ճ�ʱӦ�ò��� */
				GPS_App_Set_Rece_Timeout_Para();
				
			}else
			{
				/* ���ݽ���δ��ʱ */
				/* ʹ�õ�ǰδ���µ�������ΪӦ������ */
				__nop();
			}
		}
	}

}


/************************************************************************************************/
/* ������	: GPS_App_Send_Lat_Data												 	 			*/
/* ����  	: ��			 																	*/
/* ���		: ��������. 																		*/
/* ����	 	: �����������						 												*/
/* ��������	: 2015/08/19																		*/
/************************************************************************************************/
float GPS_App_Send_Lat_Data(void)
{
	/* ��� γ�����ݸ���״̬ */
	GPS_POS_App_SR.Lat_Updata_State = FALSE;

	return GPS_POS_App_DR.Lat;
}


/************************************************************************************************/
/* ������	: GPS_App_Send_Lon_Data												 	 			*/
/* ����  	: ��			 																	*/
/* ���		: γ������. 																		*/
/* ����	 	: ���γ������						 												*/
/* ��������	: 2015/08/19																		*/
/************************************************************************************************/
float GPS_App_Send_Lon_Data(void)
{
	

	/* ��� �������ݸ���״̬ */
	GPS_POS_App_SR.Lon_Updata_State = FALSE;

	return GPS_POS_App_DR.Lon;
}


/************************************************************************************************/
/* ������	: GPS_App_Send_Spd_Data												 	 			*/
/* ����  	: ��			 																	*/
/* ���		: �ٶ�����. 																		*/
/* ����	 	: ����ٶ�����						 												*/
/* ��������	: 2015/08/19																		*/
/************************************************************************************************/
float GPS_App_Send_Spd_Data(void)
{
	

	/* ��� �ٶ����ݸ���״̬ */
	GPS_POS_App_SR.Spd_Updata_State = FALSE;

	return GPS_POS_App_DR.Spd;
}





#endif


/******************* (C) ZHOU *****END OF FILE****/



