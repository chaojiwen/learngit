/**
  ******************************************************************************
  * @file              : Log_COM.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2015Äê3ÔÂ12ÈÕ 09:55
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 STMicroelectronics
  *
  *
  ******************************************************************************
  */
#include "System_Config.h"
  
#include "BSP.h" 
 
#include "Log_COM.h"

#include "Log_Download.h"
#include "Log_GSM.h"
#include "Log_Sensor.h"

#include "Dev_Fram.h"

#include "Mcu_Config.h"
#include "Mcu_Usart.h"

#include <string.h>
#include "app.h"
#include "Mcu_Beep.h"

#include "GSM_app.h"

#include "String_Utility.h"

#if (GSM_Module_Type==0x02) //ESP8266WifiÄ£¿é	
#include "Log_GSM_Config.h"
#endif

/************************GSMÄ£¿éĞÅºÅÇ¿¶È½á¹¹Ìå*****************ZWC********/
extern  GSM_GPRS_SR_Struct GPRS_SR_Reg;
extern void DispMenuDeskTopDown(void);
//static void Com_Config_SMSAamPoeNOSet (void);
/************************************************************************************************/
extern void Instru_Rte_Fty_Set(void);
extern void Parameter_Setting_Successful(void);

/* ´òÓ¡µ÷ÊÔ±êÖ¾ *///ZWC
extern BOOL GSM_DEBUG_Flag;// TRUE




/**********************************´®¿Ú¿ØÖÆÊı¾İ¼ÇÂ¼µÄÆô¶¯ºÍÍ£Ö¹*********************************/

extern void App_Save_Config(FunctionalState New_state);
/**********************************ÓÃÓÚ¶ÁÈ¡Êı¾İ¼ÇÂ¼µÄ×´Ì¬***************************************/
/* ÊµÊ±Êı¾İ´æ´¢¿ª¹Ø Êı¾İÖ¸Õë SaveDataZWC */
static FunctionalState *Save_Data_Switch_A = &Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Menu_Save_Data_Switch;

/********************************Í¨¹ı´®¿Úµç³ØÁ¬½Ó×´Ì¬ºÍµç³ØµçÁ¿************************************/
extern u8 Power_ET_Alarm_Check(INSTRU_POWER* power_t,INSTRU_POWER_FIX* power_f);

/*************************RTCÊ±ÖÓÉèÖÃ******************************************/
extern u8 HEX_To_BCD(u8 hex_data);    //HEX×ªÎªBCD×Ó³ÌĞò   
extern u8 BCD_To_HEX(u8 hex_data);    //BCD×ªÎªHEX×Ó³ÌĞò 

extern u8 HEX_Hi4(u8 Data);//È¡³öÒ»¸ö×Ö½ÚÊı¾İµÄ¸ß4Î»
extern u8 HEX_Lo4(u8 Data);//È¡³öÒ»¸ö×Ö½ÚÊı¾İµÄµÍ4Î»

#define System_Year_Start		(2000u)//´®¿ÚÉèÖÃÊ±¼ä£»Äê·İ¿ªÊ¼¿ªÊ¼Ê±¼ä



/* ´«¸ĞÆ÷Ğ£×¼Í¨ĞÅÊı¾İ¾İÖ¡ ½ÓÊÕ ¼Ä´æÆ÷ ÊıÁ¿ */
#define Com_Sensor_Cal_RX_Reg_Size			(COM_Sensor_Cal_EVENT_Q_Len)
/* ´«¸ĞÆ÷Ğ£×¼Í¨ĞÅÊı¾İ¾İÖ¡´æ´¢¼Ä´æÆ÷ */
COM_SENSOR_CAL_TYPEDEF	Com_Sensor_Cal_RX_Reg[Com_Sensor_Cal_RX_Reg_Size];

/************************************************************************************************/
//Ô­Ê¼°æ±¾µÄ´«¸ĞÆ÷½á¹¹
//ÓÉÓÚÅäÖÃ¹¤¾ßÔİÊ±Ã»ÓĞÈË½ÓÊÖ¸üĞÂ£¬Õâ¸öÎÊÌâÖ»ÄÜÏÈÕâÑù×öÒ»ÏÂ´¦Àí¡£
/* ´«¸ĞÆ÷ Í¨µÀÍ¨ĞÅÊı¾İ½á¹¹ */
typedef struct {
	/* ´«¸ĞÆ÷ ÀàĞÍ */						//	0:Ã»ÓĞÉè¶¨µÄ´«¸ĞÆ÷ÀàĞÍ	1:ÎÂ¶È´«¸ĞÆ÷		2:Êª¶È´«¸ĞÆ÷		3:µ¥ÎÂ¿¨Æ¬	4£ºÎÂÊª¿¨Æ¬	ÆäËû:ÎŞĞ§
	u32				Type						:8;
	/* ´«¸ĞÆ÷ ±¨¾¯¿ª¹Ø */						//	1:±¨¾¯¿ª			0:±¨¾¯¹Ø
	u32				SENSOR_Alarm_Switch			:1;	
	/* ´«¸ĞÆ÷ ÉùÒô±¨¾¯¿ª¹Ø */					//	1:±¨¾¯¿ª			0:±¨¾¯¹Ø
	u32				SENSOR_Sound_Alarm_Switch	:1;
	/* ´«¸ĞÆ÷ ¹â±¨¾¯¿ª¹Ø */					//	1:±¨¾¯¿ª			0:±¨¾¯¹Ø
	u32				SENSOR_Light_Alarm_Switch	:1;
	/* ´«¸ĞÆ÷ ¶ÌĞÅ±¨¾¯¿ª¹Ø */					//	1:±¨¾¯¿ª			0:±¨¾¯¹Ø
	u32				SENSOR_SMS_Alarm_Switch		:1;
	/* ´«¸ĞÆ÷ ¿ª¹Ø±£Áô */						//	00:Î´ÅäÖÃ£¨Ä¬ÈÏÅäÖÃ£©¡£(¸ÃÑ¡Ïî²»¿É¸ü¸Ä)
	u32				SENSOR_Alarm_Switch_Reserve	:4;	
	/* ´«¸ĞÆ÷ ±¨¾¯×´Ì¬ */						//	00:´«¸ĞÆ÷Î´±¨¾¯	01:´«¸ĞÆ÷±¨¾¯
	u32				SENSOR_Alarm_State			:2;
	/* ´«¸ĞÆ÷ ±¨¾¯×´Ì¬±£Áô */					//	00:Î´ÅäÖÃ£¨Ä¬ÈÏÅäÖÃ£©¡£(¸ÃÑ¡Ïî²»¿É¸ü¸Ä)
	u32 			SENSOR_Alarm_State_Reserve	:6;	
	/* ´«¸ĞÆ÷ ĞÅÏ¢ ±£Áô¿Õ¼ä */				//	00:Î´ÅäÖÃ£¨Ä¬ÈÏÅäÖÃ£©¡£(¸ÃÑ¡Ïî²»¿É¸ü¸Ä)
	u32				SENSOR_State_Inf_Reserve	:8;
	/* ¿¨Æ¬IDºÅ */							//	0:Î´ÅäÖÃ£¨Ä¬ÈÏÅäÖÃ£©
	u32				SENSOR_RF_Card_ID_NO;
	/* ´«¸ĞÆ÷ SNºÅ */						//	0:Î´ÅäÖÃ£¨Ä¬ÈÏÅäÖÃ£©
	u8				SENSOR_SN_NO[SENSOR_SN_NO_Size];
	/* ´«¸ĞÆ÷ ±¨¾¯ÉÏÏŞ */						//	00:Î´ÅäÖÃ£¨Ä¬ÈÏÅäÖÃ£©£¬³ö³§Ç°ĞèÒªÅäÖÃ¸ÃÏîÄÚÈİ¡£(¸ÃÑ¡Ïî ¿É¸ü¸Ä)
	float			SENSOR_Up_Limit;
	/* ´«¸ĞÆ÷ ±¨¾¯ÏÂÏŞ */						//	00:Î´ÅäÖÃ£¨Ä¬ÈÏÅäÖÃ£©£¬³ö³§Ç°ĞèÒªÅäÖÃ¸ÃÏîÄÚÈİ¡£(¸ÃÑ¡Ïî ¿É¸ü¸Ä)
	float			SENSOR_Low_Limit;
	
}INSTRU_SENSOR_Old;

/************************************************************************************************/

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Send_Q																*/
/**	¹¦ÄÜËµÃ÷: ·¢ËÍ´«¸ĞÆ÷Ğ£×¼Êı¾İÏûÏ¢																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void Com_Sensor_Cal_Send_Q(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	INT8U      	err;
	OS_Q_DATA	com_sensor_cal_q;

	/* »ñµÃ ´«¸ĞÆ÷Ğ£×¼¶ÓÁĞ ĞÅÏ¢ */
	err = OSQQuery (COM_Sensor_Cal_EVENT_Q,&com_sensor_cal_q);
	/* ÅĞ¶Ï²éÑ¯¶ÓÁĞ³É¹¦ */
	if(err == OS_ERR_NONE)
	{
		/* ÅĞ¶Ï ¶ÓÁĞÖĞÏûÏ¢ÊıÁ¿ÊÇ·ñ³¬³öÏŞÖÆ */
		if(com_sensor_cal_q.OSNMsgs < COM_Sensor_Cal_EVENT_Q_Len)
		{
			/* ¶ÓÁĞ Î´³¬ÏŞ */
			/* ¿½±´´«¸ĞÆ÷Ğ£×¼Êı¾İÊı¾İ */
			Com_Sensor_Cal_RX_Reg[com_sensor_cal_q.OSNMsgs] = *Cal_Inf;
			/* Ïò¶ÔÁĞÖĞ·¢ËÍÏûÏ¢ */
			err = OSQPost(COM_Sensor_Cal_EVENT_Q,&Com_Sensor_Cal_RX_Reg[com_sensor_cal_q.OSNMsgs]);
		}else
		{
			/* ¶ÓÁĞ ³¬ÏŞ */
			/* ²»·¢ËÍÏûÏ¢ */
			__nop();
		}	
	}
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Get_Check															*/
/**	¹¦ÄÜËµÃ÷: ¼ÆËã´«¸ĞÆ÷Í¨ĞÅÖ¡µÄĞ£Ñé																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
u8 Com_Sensor_Cal_Get_Check(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	u8 check = 0;
	u8 num;
	
	/* ¼ÆËã´«¸ĞÆ÷Í¨ĞÅÖ¡µÄĞ£Ñé */
	check += Cal_Inf->Head_Code;
	check += Cal_Inf->Instru_NO;
	check += Cal_Inf->Cmd_Code;
	check += Cal_Inf->Chanel_Num;
	check += Cal_Inf->Data_Len;
	
	for(num = 0;num < (Cal_Inf->Data_Len+1);num++)
	{
		check += Cal_Inf->Com_Data[num];
	}
	
	return check;
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Deleted_Buffer_Data													*/
/**	¹¦ÄÜËµÃ÷: Çå³ı Êı¾İ»º´æµÄÊı¾İ																	*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Deleted_Buffer_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* Çå³ı Êı¾İ»º´æµÄÊı¾İ */
	memset(Cal_Inf,0X00,sizeof(COM_SENSOR_CAL_TYPEDEF));
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Deleted_Buffer_Data													*/
/**	¹¦ÄÜËµÃ÷: Çå³ı Êı¾İ»º´æµÄÊı¾İ																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static BOOL Log_Com_Sensor_CH_Check(u8 ch)
{

	/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
	switch(ch)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			return TRUE;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			return TRUE;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			return TRUE;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			return TRUE;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			return TRUE;
		}
#endif	
		
		default:
		{
			/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */

			return FALSE;
		}
	}

}

#if (1)

#if (GSM_Module_Type==2) //ESP8266WifiÄ£¿é
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Read_Write_Wifi_Name	*/
/**	¹¦ÄÜËµÃ÷: ¶ÁºÍĞ´WifiÃû³Æ***************/
/**	ĞÎ    ²Î£º²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_Name(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/* Ğ´WifiÃû³Æ Ó¦´ğµÄÊı¾İ³¤¶È */
#define Com_Read_Write_Wifi_Name_Data_Len	(1u) //×î´ó1¸ö´«ÊäÊı¾İ
/* Ğ´WifiÃû³Æ Ó¦´ğĞ­Òé³¤¶È */
//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Read_Write_Wifi_Name_Len		       (Com_Read_Write_Wifi_Name_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* Ğ´WifiÃû³ÆµÄÊı¾İµØÖ· */
#define  Read_Write_Wifi_Name_Data_P				   	(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Name)
COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_Name;//½ÓÊÕ´®¿ÚÊı¾İ 
/*Cal_Inf->Chanel_Num=0x55´ú±í¶Á²Ù×÷    Read_Write_Wifi_Name.Chanel_Num=0xAA´ú±íĞ´²Ù×÷*/
	    if((Cal_Inf->Chanel_Num==0x55)&&(Cal_Inf->Data_Len<=Wifi_Name_Size))//¶Á
			{	
	        /* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
					Read_Write_Wifi_Name.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
					Read_Write_Wifi_Name.Instru_NO 	   = Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
					Read_Write_Wifi_Name.Cmd_Code	     = Com_Read_Write_Wifi_Name_Cmd_Code;
					Read_Write_Wifi_Name.Chanel_Num    = Cal_Inf->Chanel_Num;
					Read_Write_Wifi_Name.Data_Len	     = strlen(Read_Write_Wifi_Name_Data_P);
					/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
					memcpy(Read_Write_Wifi_Name.Com_Data,Read_Write_Wifi_Name_Data_P,Read_Write_Wifi_Name.Data_Len);
					/* ÉèÖÃ ³£Êı */
					*((u8*)&(Read_Write_Wifi_Name.Com_Data[Read_Write_Wifi_Name.Data_Len])) = Com_Sensor_Cal_Constant;
					/* ÉèÖÃ Ğ£Ñé */
					*((u8*)&(Read_Write_Wifi_Name.Com_Data[Read_Write_Wifi_Name.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Name);
					/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
					USART_printf(USART1,(u8*)&Read_Write_Wifi_Name,(Read_Write_Wifi_Name.Data_Len + Com_Sensor_Cal_Fix_Data_Len));		
			
			}
			else if((Cal_Inf->Chanel_Num==0xAA)&&(Cal_Inf->Data_Len<=Wifi_Name_Size))//Ğ´
			{
				/* Çå³ıÊı¾İÇøÄÚÈİ *//* Ğ´WifiÃû³Æ */
				memset(Read_Write_Wifi_Name_Data_P,'\0',Wifi_Name_Size);
				/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
				memcpy(Read_Write_Wifi_Name_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len > (Wifi_Name_Size - 1)) ? (Wifi_Name_Size - 1):Cal_Inf->Data_Len);
				Write_Config_Tab((u8*)Read_Write_Wifi_Name_Data_P,sizeof(Read_Write_Wifi_Name_Data_P));/* ±£´æWifiÃû³Æ */
				/* »Ø¸´ÉÏÎ»»úÖ¸Áî²ÎÊı×é×° */
				Read_Write_Wifi_Name.Head_Code       	= Com_Sensor_Cal_Rece_Head_Code;
				Read_Write_Wifi_Name.Instru_NO 	      = Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
				Read_Write_Wifi_Name.Cmd_Code		      = Com_Read_Write_Wifi_Name_Cmd_Code;
				Read_Write_Wifi_Name.Chanel_Num	      = Cal_Inf->Chanel_Num;
				Read_Write_Wifi_Name.Data_Len		      = Com_Read_Write_Wifi_Name_Data_Len;
				/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
				Read_Write_Wifi_Name.Com_Data[0]	    = Com_Sensor_Cal_Set_Para_Succeed;
				/* ÉèÖÃ ³£Êı */
				Read_Write_Wifi_Name.Com_Data[1]	    = Com_Sensor_Cal_Constant;
				/* ÉèÖÃ Ğ£Ñé */
				Read_Write_Wifi_Name.Com_Data[2] 	    = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Name);
				/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
				USART_printf(USART1,(u8*)&Read_Write_Wifi_Name,Com_Read_Write_Wifi_Name_Len);	
				
				
			
				
			}
			
   #undef Com_Read_Write_Wifi_Name_Data_Len
   #undef Com_Read_Write_Wifi_Name_Len
   #undef Write_Wifi_Name_Data_P				
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Read_Write_Wifi_WSKEY*/
/**	¹¦ÄÜËµÃ÷: ¶ÁºÍĞ´Wifi_WSKEY(¼ÓÃÜ·½Ê½ºÍÃÜÂë)***************/
/**	ĞÎ    ²Î£º²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_WSKEY(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/*Ğ´Wifi_WSKEY(¼ÓÃÜ·½Ê½ºÍÃÜÂë) Ó¦´ğµÄÊı¾İ³¤¶È */
#define Com_Read_Write_Wifi_WSKEY_Data_Len	  (1u) //×î´ó1¸ö´«ÊäÊı¾İ
/* Ğ´Wifi_WSKEY(¼ÓÃÜ·½Ê½ºÍÃÜÂë) Ó¦´ğĞ­Òé³¤¶È */
//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Read_Write_Wifi_WSKEY_Len		       (Com_Read_Write_Wifi_WSKEY_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* Ğ´Wifi_WSKEY(¼ÓÃÜ·½Ê½ºÍÃÜÂë)µÄÊı¾İµØÖ· */
#define  Read_Write_Wifi_WSKEY_Data_P				   (Instru_Fix_Inf_State_Tab.Inf_State.Instru_Wifi.Inf_State.Wifi_Password)	
COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_WSKEY;//½ÓÊÕ´®¿ÚÊı¾İ 
/*Cal_Inf->Chanel_Num=0x55´ú±í¶Á²Ù×÷    Cal_Inf->Chanel_Num=0xAA´ú±íĞ´²Ù×÷*/
	    if((Cal_Inf->Chanel_Num==0x55)&&(Cal_Inf->Data_Len<=Wifi_Passward_Size))//¶Á
			{				
	        /* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
					Read_Write_Wifi_WSKEY.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
					Read_Write_Wifi_WSKEY.Instru_NO 	   = Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
					Read_Write_Wifi_WSKEY.Cmd_Code	     = Com_Read_Write_Wifi_WSKEY_Cmd_Code;
					Read_Write_Wifi_WSKEY.Chanel_Num     = Cal_Inf->Chanel_Num;
					Read_Write_Wifi_WSKEY.Data_Len	     = strlen(&Read_Write_Wifi_WSKEY_Data_P[1])+2;
					Read_Write_Wifi_WSKEY.Com_Data[0]    =0;//Ä¬ÈÏÎŞ¼ÓÃÜ·½Ê½
					/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
					memcpy(&Read_Write_Wifi_WSKEY.Com_Data[1],Read_Write_Wifi_WSKEY_Data_P,Read_Write_Wifi_WSKEY.Data_Len);
				
					/* ÉèÖÃ ³£Êı */
					*((u8*)&(Read_Write_Wifi_WSKEY.Com_Data[Read_Write_Wifi_WSKEY.Data_Len])) = Com_Sensor_Cal_Constant;
					/* ÉèÖÃ Ğ£Ñé */
					*((u8*)&(Read_Write_Wifi_WSKEY.Com_Data[Read_Write_Wifi_WSKEY.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_WSKEY);
				
				
					/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
					USART_printf(USART1,(u8*)&Read_Write_Wifi_WSKEY,(Read_Write_Wifi_WSKEY.Data_Len + Com_Sensor_Cal_Fix_Data_Len));		
			
			}
			else if((Cal_Inf->Chanel_Num==0xAA)&&(Cal_Inf->Data_Len<=Wifi_Passward_Size))//Ğ´
			{
				/* Çå³ıÊı¾İÇøÄÚÈİ *//* Ğ´WifiÃû³Æ */
				memset(Read_Write_Wifi_WSKEY_Data_P,'\0',Wifi_Passward_Size);
				/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
				memcpy(Read_Write_Wifi_WSKEY_Data_P,&Cal_Inf->Com_Data[1],(Cal_Inf->Data_Len > (Wifi_Passward_Size - 1)) ? (Wifi_Passward_Size - 1):Cal_Inf->Data_Len-1);
				
				Write_Config_Tab((u8*)Read_Write_Wifi_WSKEY_Data_P,sizeof(Read_Write_Wifi_WSKEY_Data_P));/* ±£´æWifiÃÜÂë  */
				/* »Ø¸´ÉÏÎ»»úÖ¸Áî²ÎÊı×é×° */
				Read_Write_Wifi_WSKEY.Head_Code       	= Com_Sensor_Cal_Rece_Head_Code;
				Read_Write_Wifi_WSKEY.Instru_NO 	      = Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
				Read_Write_Wifi_WSKEY.Cmd_Code		      = Com_Read_Write_Wifi_WSKEY_Cmd_Code;
				Read_Write_Wifi_WSKEY.Chanel_Num	      = Cal_Inf->Chanel_Num;
				Read_Write_Wifi_WSKEY.Data_Len		      = Com_Read_Write_Wifi_WSKEY_Data_Len;
				/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
				Read_Write_Wifi_WSKEY.Com_Data[0]	      = Com_Sensor_Cal_Set_Para_Succeed;
				/* ÉèÖÃ ³£Êı */
				Read_Write_Wifi_WSKEY.Com_Data[1]	      = Com_Sensor_Cal_Constant;
				/* ÉèÖÃ Ğ£Ñé */
				Read_Write_Wifi_WSKEY.Com_Data[2] 	    = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_WSKEY);
//////////						OSTimeDly(100);//ÑÓÊ±100ms
				/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
				USART_printf(USART1,(u8*)&Read_Write_Wifi_WSKEY,Com_Read_Write_Wifi_WSKEY_Len);	
				
				
				
					GSM_Config_Reg.WifiNamePWD_Config_State=TRUE;//ÉÏÎ»»úÏÂ·¢ÁËÅäÖÃWifiÄ£¿éµÄÃû×Ö¼°ÃÜÂëÃüÁî
//////				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiNamePWD_EVENT);//·¢ËÍÉèÖÃWifiÄ£¿éÃû×ÖºÍÃÜÂëÏûÏ¢
				
							
			}
#undef Com_Read_Write_Wifi_Name_Data_Len
#undef Com_Read_Write_Wifi_Name_Len
#undef Write_Wifi_Name_Data_P	
			
}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Read_Write_Wifi_Wlan*/
/**	¹¦ÄÜËµÃ÷: ¶ÁºÍĞ´Wifi_WANN(LOCALIP:±¾µØIPµØÖ·¡¢  MASK(×ÓÍøÑÚÂë) ¡¢GATEWAY(Íø¹ØµØÖ·))***************/
/**	ĞÎ    ²Î£º²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_Wlan(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_Wlan;//½ÓÊÕ´®¿ÚÊı¾İ 
/*Cal_Inf->Chanel_Num=0x55´ú±í¶Á²Ù×÷    Cal_Inf->Chanel_Num=0xAA´ú±íĞ´²Ù×÷*/
	    if(Cal_Inf->Chanel_Num==0x55)//¶Á
			{	
	      	memset((u8*)&Read_Write_Wifi_Wlan,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,&Read_Write_Wifi_Wlan.Com_Data[0],3,".");//±¾µØIPµØÖ·  4¸ö×Ö½Ú  3¸ö  .
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,&Read_Write_Wifi_Wlan.Com_Data[4],3,".");//×ÓÍøÑÚÂë 
				  ConvertIP_StringToArray(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,&Read_Write_Wifi_Wlan.Com_Data[8],3,"."); //Íø¹ØIPµØÖ· 
				
				  Read_Write_Wifi_Wlan.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_Wlan.Instru_NO 	= Cal_Inf->Instru_NO;
	        Read_Write_Wifi_Wlan.Cmd_Code		= Com_Read_Write_Wifi_WALN_Cmd_Code;
	        Read_Write_Wifi_Wlan.Chanel_Num	= Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_Wlan.Data_Len		= 12u;//4¸ö×Ö½ÚLOCALIP+4¸ö×Ö½ÚMASK+4¸ö×Ö½ÚGATEWAY
				  Read_Write_Wifi_Wlan.Com_Data[Read_Write_Wifi_Wlan.Data_Len] = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_Wlan.Com_Data[Read_Write_Wifi_Wlan.Data_Len+1] = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Wlan);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_Wlan,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_Wlan.Data_Len));	
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//Ğ´
			{
				
				

		    sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Local_IP,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[0],
		                               Cal_Inf->Com_Data[1],
		                               Cal_Inf->Com_Data[2],
		                               Cal_Inf->Com_Data[3]);	//±¾µØIPµØÖ·
				
				
			  sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Subnet_Mask,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[4],
		                               Cal_Inf->Com_Data[5],
		                               Cal_Inf->Com_Data[6],
		                               Cal_Inf->Com_Data[7]);//×ÓÍøÑÚÂë 
				
			  sprintf(Instru_Wifi_Fix_Tab.Inf.Wifi_Gateway_IP,"%d.%d.%d.%d",
		                               Cal_Inf->Com_Data[8],
		                               Cal_Inf->Com_Data[9],
		                               Cal_Inf->Com_Data[10],
		                               Cal_Inf->Com_Data[11]);//Íø¹ØIPµØÖ·

				Read_Write_Wifi_Wlan.Head_Code 	   = Com_Sensor_Cal_Rece_Head_Code;
	      Read_Write_Wifi_Wlan.Instru_NO 	   = Cal_Inf->Instru_NO;
	      Read_Write_Wifi_Wlan.Cmd_Code	     = ComConfigDataNetworkWrite;
	      Read_Write_Wifi_Wlan.Chanel_Num    = Cal_Inf->Chanel_Num;
	      Read_Write_Wifi_Wlan.Data_Len	     = (1u);	
				Read_Write_Wifi_Wlan.Com_Data[0]	 = Com_Sensor_Cal_Set_Para_Succeed;
	      Read_Write_Wifi_Wlan.Com_Data[1]	 = Com_Sensor_Cal_Constant;
	      Read_Write_Wifi_Wlan.Com_Data[2]   = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_Wlan);
				
////////							OSTimeDly(100);//ÑÓÊ±100ms
	      USART_printf(USART1,(u8*)&Read_Write_Wifi_Wlan,(Com_Sensor_Cal_Fix_Data_Len+1));	
				
				
				GSM_Config_Reg.WifiWLAN_Config_State=TRUE;//ÉÏÎ»»úÏÂ·¢ÁËÅäÖÃWifiÄ£¿éµÄWLAN²ÎÊıÃüÁî
				

				  
				GSM_Config_Reg.GSM_Config_State=TRUE;//¿ªÊ¼ÅäÖÃWifiÄ£¿é²ÎÊı
				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);//ÏÂ·¢³õÊ¼»¯WifiÄ£¿éÏûÏ¢
	
				
				
				
////////				
////////				//			/* WifiÉèÖÃWifiWALN²ÎÊı*/
////////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiWLAN_EVENT);
				
				
			}		
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Wifi_NetP_Read														*/
/**	¹¦ÄÜËµÃ÷: ¶ÁWifiÊı¾İ·şÎñÆ÷µÄÅäÖÃ²ÎÊı																*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Wifi_NetP_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
//	/* ÍøÂçÅäÖÃÖ¸Õë */
//	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;

	/* ÉèÖÃ¾Ö²¿±äÁ¿³õÊ¼Öµ */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* ÅäÖÃÊı¾İ·şÎñÆ÷²ÎÊı */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= Com_Read_Write_Wifi_NETP_Cmd_Code;
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 6u;


   ackDataNetworkRead.Com_Data[0]=(0xFF00&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1)>>8;
   ackDataNetworkRead.Com_Data[1]=(0xFF&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1);
	 ackDataNetworkRead.Com_Data[2]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A;
   ackDataNetworkRead.Com_Data[3]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B;
	 ackDataNetworkRead.Com_Data[4]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C;
   ackDataNetworkRead.Com_Data[5]=Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D;
	 

		   
	/* ÉèÖÃ ³£Êı */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* ÉèÖÃ Ğ£Ñé */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Wifi_NetP_Write														*/
/**	¹¦ÄÜËµÃ÷: Ğ´Êı¾İ·şÎñÆ÷µÄÅäÖÃ²ÎÊı																*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Wifi_NetP_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	  /* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	  COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	  /* Êı¾İºÏ·¨ĞÔ±êÖ¾ */
	  BOOL data_vldy = TRUE;

////////	
////////		/* »ñÈ¡ÍøÂçÊı¾İ */
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A = Cal_Inf->Com_Data[2];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B = Cal_Inf->Com_Data[3];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C = Cal_Inf->Com_Data[4];
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D = Cal_Inf->Com_Data[5];
////////	
////////		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1 = (Cal_Inf->Com_Data[0]<<8)|(Cal_Inf->Com_Data[1]&0xff);
	
	
	
////		/* ÉèÖÃÇå³ıÓòÃû¼Ä´æÆ÷ */
////		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,'\0',32);


////		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦IPµÄÊı¾İ */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address));
////		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦¶Ë¿ÚµÄÊı¾İ */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1));
////		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦ÓòÃûµÄÊı¾İ */
////		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
////						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));	

	
	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	  = Com_Read_Write_Wifi_NETP_Cmd_Code;
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	  = (1u);
	
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ÉèÖÃ ³£Êı */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Read_Write_Wifi_NetP*/
/**	¹¦ÄÜËµÃ÷: ¶ÁºÍĞ´WifiµÄNETPÏà¹Ø²ÎÊıIPµØÖ·ºÍ¶Ë¿Ú***************/
/**	ĞÎ    ²Î£º²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_NetP(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{ 
/*Cal_Inf->Chanel_Num=0x55´ú±í¶Á²Ù×÷    Cal_Inf->Chanel_Num=0xAA´ú±íĞ´²Ù×÷*/
	         if(Cal_Inf->Chanel_Num==0x55)//¶Á
			     {	
						 
						 Com_Config_Wifi_NetP_Read( Cal_Inf);

			     }
			    else if(Cal_Inf->Chanel_Num==0xAA)//Ğ´
			    {
	           Com_Config_Wifi_NetP_Write( Cal_Inf); 
					 }						

}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Read_Write_Wifi_DHCP*/
/**	¹¦ÄÜËµÃ÷: ¶ÁºÍĞ´WifiÄ£¿é»ñÈ¡IPµØÖ··½Ê½***************/
/**	ĞÎ    ²Î£º²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_DHCP(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_DHCP;//½ÓÊÕ´®¿ÚÊı¾İ 
/*Cal_Inf->Chanel_Num=0x55´ú±í¶Á²Ù×÷    Cal_Inf->Chanel_Num=0xAA´ú±íĞ´²Ù×÷*/
	    if(Cal_Inf->Chanel_Num==0x55)//¶Á
			{	
	      	memset((u8*)&Read_Write_Wifi_DHCP,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  Read_Write_Wifi_DHCP.Head_Code 	  = Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_DHCP.Instru_NO 	  = Cal_Inf->Instru_NO;
	        Read_Write_Wifi_DHCP.Cmd_Code		  = Com_Read_Write_Wifi_DHCP_Cmd_Code;
	        Read_Write_Wifi_DHCP.Chanel_Num	  = Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_DHCP.Data_Len		  = 1u;//1¸ö×Ö½Ú      
				  Read_Write_Wifi_DHCP.Com_Data[0]  = Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way  ;//0x00´ú±í»ñÈ¡IPµØÖ·µÄ·½Ê½ÎªSTATIC   0x01´ú±í»ñÈ¡IPµØÖ·µÄ·½Ê½ÎªDHCP 
				  Read_Write_Wifi_DHCP.Com_Data[Read_Write_Wifi_DHCP.Data_Len] = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_DHCP.Com_Data[Read_Write_Wifi_DHCP.Data_Len+1] = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_DHCP);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_DHCP,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_DHCP.Data_Len));	
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//Ğ´
			{
				 Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way = Cal_Inf->Com_Data[0];//0x00´ú±í»ñÈ¡IPµØÖ·µÄ·½Ê½ÎªSTATIC   0x01´ú±í»ñÈ¡IPµØÖ·µÄ·½Ê½ÎªDHCP 
				 Read_Write_Wifi_DHCP.Head_Code 	       = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Write_Wifi_DHCP.Instru_NO 	       = Cal_Inf->Instru_NO;
	       Read_Write_Wifi_DHCP.Cmd_Code	         = Com_Read_Write_Wifi_DHCP_Cmd_Code;
	       Read_Write_Wifi_DHCP.Chanel_Num         = Cal_Inf->Chanel_Num;
	       Read_Write_Wifi_DHCP.Data_Len	         = (1u);	
				 Read_Write_Wifi_DHCP.Com_Data[0]	       = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Write_Wifi_DHCP.Com_Data[1]	       = Com_Sensor_Cal_Constant;
	       Read_Write_Wifi_DHCP.Com_Data[2]        = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_DHCP);
				
////////////							OSTimeDly(100);//ÑÓÊ±100ms
	       USART_printf(USART1,(u8*)&Read_Write_Wifi_DHCP,(Com_Sensor_Cal_Fix_Data_Len+1));	
				
				
				GSM_Config_Reg.WifiWLAN_GET_IP_WAY_Config_State=TRUE;//ÉÏÎ»»úÏÂ·¢ÁËÅäÖÃWifiÄ£¿éµÄ»ñÈ¡IPµØÖ··½Ê½ÃüÁî
				
				if((GSM_Config_Reg.WifiWLAN_Config_State==FALSE)&&(Instru_Wifi_Fix_Tab.Inf.Wifi_IP_Get_Way==0x01))
				{
				     GSM_Config_Reg.GSM_Config_State=TRUE;//¿ªÊ¼ÅäÖÃWifiÄ£¿é²ÎÊı
				   GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);//ÏÂ·¢³õÊ¼»¯WifiÄ£¿éÏûÏ¢
				}
				
				
				
				
////////				 /* WifiÄ£¿é»ñÈ¡IPµØÖ··½Ê½²ÎÊı*/
////////			   GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,SetWifiWLAN_GET_IP_WAY_EVENT);

			}		
}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Read_Write_Wifi_MAC*/
/**	¹¦ÄÜËµÃ÷: ¶ÁºÍĞ´WifiÄ£¿éµÄMACµØÖ·***************/
/**	ĞÎ    ²Î£º²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Read_Write_Wifi_MAC(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
   /* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
   COM_SENSOR_CAL_TYPEDEF                          Read_Write_Wifi_MAC;//½ÓÊÕ´®¿ÚÊı¾İ 
/*Cal_Inf->Chanel_Num=0x55´ú±í¶Á²Ù×÷    Cal_Inf->Chanel_Num=0xAA´ú±íĞ´²Ù×÷*/
	    if(Cal_Inf->Chanel_Num==0x55)//¶Á
			{	
	      	memset((u8*)&Read_Write_Wifi_MAC,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF)); 
				  Read_Write_Wifi_MAC.Head_Code 	  = Com_Sensor_Cal_Rece_Head_Code;
	        Read_Write_Wifi_MAC.Instru_NO 	  = Cal_Inf->Instru_NO;
	        Read_Write_Wifi_MAC.Cmd_Code		  = Com_Read_Write_Wifi_MAC_Cmd_Code;
	        Read_Write_Wifi_MAC.Chanel_Num	  = Cal_Inf->Chanel_Num;
	        Read_Write_Wifi_MAC.Data_Len		  = strlen(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address);//MACµØÖ·³¤¶È×Ö½Ú 
				  memcpy(Read_Write_Wifi_MAC.Com_Data,Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address, Read_Write_Wifi_MAC.Data_Len);
				  Read_Write_Wifi_MAC.Com_Data[Read_Write_Wifi_MAC.Data_Len]    = Com_Sensor_Cal_Constant;
	        Read_Write_Wifi_MAC.Com_Data[Read_Write_Wifi_MAC.Data_Len+1]  = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_MAC);
	        USART_printf(USART1,(u8*)&Read_Write_Wifi_MAC,(Com_Sensor_Cal_Fix_Data_Len+Read_Write_Wifi_MAC.Data_Len));

				
			}
			else if(Cal_Inf->Chanel_Num==0xAA)//Ğ´
			{
				
////////				 memcpy(Instru_Wifi_Fix_Tab.Inf.Wifi_Mac_Address,Cal_Inf->Com_Data,Cal_Inf->Data_Len);

				 Read_Write_Wifi_MAC.Head_Code 	       = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Write_Wifi_MAC.Instru_NO 	       = Cal_Inf->Instru_NO;
	       Read_Write_Wifi_MAC.Cmd_Code	         = Com_Read_Write_Wifi_MAC_Cmd_Code;
	       Read_Write_Wifi_MAC.Chanel_Num        = Cal_Inf->Chanel_Num;
	       Read_Write_Wifi_MAC.Data_Len	         = (1u);	
				 Read_Write_Wifi_MAC.Com_Data[0]	     = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Write_Wifi_MAC.Com_Data[1]	     = Com_Sensor_Cal_Constant;
	       Read_Write_Wifi_MAC.Com_Data[2]       = Com_Sensor_Cal_Get_Check(&Read_Write_Wifi_MAC);
	       USART_printf(USART1,(u8*)&Read_Write_Wifi_MAC,(Com_Sensor_Cal_Fix_Data_Len+1));	

			}		
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Rest_Wifi*/
/**	¹¦ÄÜËµÃ÷:¸´Î»»òÕß»Ö¸´³ö³§ÉèÖÃWifiÄ£¿é***************/
/**	ĞÎ    ²Î£º²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Rest_Wifi(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
   /* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
   COM_SENSOR_CAL_TYPEDEF                          Read_Rest_Wifi;//½ÓÊÕ´®¿ÚÊı¾İ 
/*Cal_Inf->Chanel_Num=0x55´ú±í¶Á²Ù×÷    Cal_Inf->Chanel_Num=0xAA´ú±íĞ´²Ù×÷*/
      if(Cal_Inf->Chanel_Num==0xAA)//Ğ´
			{
				  if(Cal_Inf->Com_Data[0]==1)
					{//Íê³ÉWIFI²ÎÊıĞŞ¸Ä£¬ÍË³öÅäÖÃÄ£Ê½£¬¸üĞÂÅäÖÃºó½øÈë×Ô¶¯ÉÏ´«Ä£Ê½¡£

					}
					else if(Cal_Inf->Com_Data[0]==2)
					{
									OSTimeDly(100);//ÑÓÊ±100ms
						//Ç¿ÖÆ¸´Î»WIFIÄ£¿é
					  /* ¸´Î»WifiÄ£¿é*/
             GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,RestWifi_EVENT);
					}
					else if(Cal_Inf->Com_Data[0]==3)
					{
									OSTimeDly(100);//ÑÓÊ±100ms
						//Ç¿ÖÆ»Ö¸´³ö³§ÉèÖÃ
		   	    /* »Ö¸´³ö³§ÉèÖÃWifiÄ£¿é*/
      		 	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Factory_Data_Reset_Wifi_EVENT);
					}
					
				 Read_Rest_Wifi.Head_Code 	           = Com_Sensor_Cal_Rece_Head_Code;
	       Read_Rest_Wifi.Instru_NO 	           = Cal_Inf->Instru_NO;
	       Read_Rest_Wifi.Cmd_Code	         = Com_Set_Rest_Wifi_Cmd_Code;
	       Read_Rest_Wifi.Chanel_Num        = Cal_Inf->Chanel_Num;
	       Read_Rest_Wifi.Data_Len	         = (1u);	
				 Read_Rest_Wifi.Com_Data[0]	     = Com_Sensor_Cal_Set_Para_Succeed;
	       Read_Rest_Wifi.Com_Data[1]	     = Com_Sensor_Cal_Constant;
	       Read_Rest_Wifi.Com_Data[2]       = Com_Sensor_Cal_Get_Check(&Read_Rest_Wifi);
	       USART_printf(USART1,(u8*)&Read_Rest_Wifi,(Com_Sensor_Cal_Fix_Data_Len+1));		
			}		
}
#endif
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Read_Real_Data														*/
/**	¹¦ÄÜËµÃ÷: »ñÈ¡´«¸ĞÆ÷ÊµÊ±Êı¾İ																	*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Real_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Real_Data_Len	 	(11u)//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+4(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Sensor_Cal_Read_data_buffer_Size	(sizeof(float))//µ¥¾«¶È¸¡µãĞÍÊı¾İ³¤¶È
/* ×î¶àÅäÖÃµÄ´«¸ĞÆ÷ÊıÁ¿ */
	COM_SENSOR_CAL_TYPEDEF read_real_data_ack;
	float com_data_buffer;

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	read_real_data_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;	
	read_real_data_ack.Instru_NO 	= Cal_Inf->Instru_NO;
	read_real_data_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Real_Data;
	read_real_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_real_data_ack.Data_Len		= Com_Sensor_Cal_Read_data_buffer_Size;
	
	
	/* ÅĞ¶Ï»ñÈ¡µÄÊÇ µÚ¼¸Í¨µÀ µÄ´«¸ĞÆ÷Êı¾İÇúÏß */
	/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data;
			break;
		}
#endif	
		
		default:
		{
			/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */
			__nop();
			return;
		}
	}

	memcpy(read_real_data_ack.Com_Data,(u8*)(&com_data_buffer),Com_Sensor_Cal_Read_data_buffer_Size);
	/* ÉèÖÃ ³£Êı */
	read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] = Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size + 1] 	= Com_Sensor_Cal_Get_Check(&read_real_data_ack);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&read_real_data_ack,Com_Sensor_Cal_Read_Real_Data_Len);
#undef Com_Sensor_Cal_Read_Real_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
}

#else
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Read_Real_Data														*/
/**	¹¦ÄÜËµÃ÷: »ñÈ¡´«¸ĞÆ÷ÊµÊ±Êı¾İ																	*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Real_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Real_Data_Len	 	(9u)//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+2(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Sensor_Cal_Read_data_buffer_Size	(2u)//Êı¾İ³¤¶È£º2×Ö½Ú
#define Com_Sensor_Cal_Data_Ratio_K				(10u)//Êı¾İ·Å´ó±¶Êı
#define Com_Sensor_Cal_Temp_Data_Offset_Value	(2731u)//ÎÂ¶ÈÖµÊı¾İµÄÆ«ÒÆÖµ


	/* ×î¶àÅäÖÃµÄ´«¸ĞÆ÷ÊıÁ¿ */
		COM_SENSOR_CAL_TYPEDEF read_real_data_ack;
		u16 com_data_buffer;
	
		/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
		read_real_data_ack.Head_Code	= Com_Sensor_Cal_Rece_Head_Code;	
		read_real_data_ack.Instru_NO	= Cal_Inf->Instru_NO;
		read_real_data_ack.Cmd_Code 	= Com_Sensor_Cal_Cmd_Code_Read_Real_Data;
		read_real_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
		read_real_data_ack.Data_Len 	= Com_Sensor_Cal_Read_data_buffer_Size;
		
		
		/* ÅĞ¶Ï»ñÈ¡µÄÊÇ µÚ¼¸Í¨µÀ µÄ´«¸ĞÆ÷Êı¾İÇúÏß */
		/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
		switch(Cal_Inf->Chanel_Num)
		{
#if	(Sensor_CH1_Config_Type != Sensor_Null)
			case Sensors_CH_1:
			{
#if	((Sensor_CH1_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}	
			
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
			case Sensors_CH_2:
			{
#if	((Sensor_CH2_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
			case Sensors_CH_3:
			{
#if	((Sensor_CH3_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
			case Sensors_CH_4:
			{
#if	((Sensor_CH4_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif
				break;
			}
#endif	
			
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
			case Sensors_CH_5:
			{
#if	((Sensor_CH5_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH5_Config_Type == Sensor_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
			case Sensors_CH_6:
			{
#if	((Sensor_CH6_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH6_Config_Type == Sensor_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
			case Sensors_CH_7:
			{
#if	((Sensor_CH7_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH7_Config_Type == Sensor_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
			case Sensors_CH_8:
			{
#if	((Sensor_CH8_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH8_Config_Type == Sensor_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
			case Sensors_CH_9:
			{
#if	((Sensor_CH9_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH9_Config_Type == Sensor_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}				
#endif	
			
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
			case Sensors_CH_10:
			{
#if	((Sensor_CH10_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH10_Config_Type == Sensor_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
			case Sensors_CH_11:
			{
#if	((Sensor_CH11_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH11_Config_Type == Sensor_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
			case Sensors_CH_12:
			{
#if	((Sensor_CH12_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH12_Config_Type == Sensor_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
			case Sensors_CH_13:
			{
#if	((Sensor_CH13_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH13_Config_Type == Sensor_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
			case Sensors_CH_14:
		{
#if	((Sensor_CH14_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH14_Config_Type == Sensor_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
			case Sensors_CH_15:
			{
#if	((Sensor_CH15_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH15_Config_Type == Sensor_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
			case Sensors_CH_16:
			{
#if	((Sensor_CH16_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH16_Config_Type == Sensor_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
			case Sensors_CH_17:
			{
#if	((Sensor_CH17_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH17_Config_Type == Sensor_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
			case Sensors_CH_18:
			{
#if	((Sensor_CH18_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH18_Config_Type == Sensor_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
			case Sensors_CH_19:
			{
#if	((Sensor_CH19_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH19_Config_Type == Sensor_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
			case Sensors_CH_20:
			{
#if	((Sensor_CH20_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH20_Config_Type == Sensor_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
			case Sensors_CH_21:
			{
#if	((Sensor_CH21_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH21_Config_Type == Sensor_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
			case Sensors_CH_22:
			{
#if	((Sensor_CH22_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH22_Config_Type == Sensor_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
			case Sensors_CH_23:
			{
#if	((Sensor_CH23_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH23_Config_Type == Sensor_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
			case Sensors_CH_24:
			{
#if	((Sensor_CH24_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH24_Config_Type == Sensor_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
			case Sensors_CH_25:
			{
#if	((Sensor_CH25_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH25_Config_Type == Sensor_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
			case Sensors_CH_26:
			{
#if	((Sensor_CH26_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH26_Config_Type == Sensor_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
			case Sensors_CH_27:
			{
#if	((Sensor_CH27_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH27_Config_Type == Sensor_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
			case Sensors_CH_28:
			{
#if	((Sensor_CH28_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH28_Config_Type == Sensor_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}		
#endif	
			
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
			case Sensors_CH_29:
			{
#if	((Sensor_CH29_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH29_Config_Type == Sensor_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
			case Sensors_CH_30:
			{
#if	((Sensor_CH30_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH30_Config_Type == Sensor_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif		
			
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
			case Sensors_CH_31:
			{
#if	((Sensor_CH31_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH31_Config_Type == Sensor_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
			case Sensors_CH_32:
			{
#if	((Sensor_CH32_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH32_Config_Type == Sensor_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
				com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Real_Time_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
				break;
			}
#endif	
			
			default:
			{
				/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */
				__nop();
				return;
			}
		}
#endif
		memcpy(read_real_data_ack.Com_Data,(u8*)(&com_data_buffer),Com_Sensor_Cal_Read_data_buffer_Size);
		/* ÉèÖÃ ³£Êı */
		read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] = Com_Sensor_Cal_Constant;
		/* ÉèÖÃ Ğ£Ñé */
		read_real_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size + 1]	= Com_Sensor_Cal_Get_Check(&read_real_data_ack);
		/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
		USART_printf(USART1,(u8*)&read_real_data_ack,Com_Sensor_Cal_Read_Real_Data_Len);

		
#undef Com_Sensor_Cal_Read_Real_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
#undef Com_Sensor_Cal_Data_Ratio_K
#undef Com_Sensor_Cal_Temp_Data_Offset_Value
}
#endif


#if (1)
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Read_Orig_Data														*/
/**	¹¦ÄÜËµÃ÷: »ñÈ¡´«¸ĞÆ÷Ô­Ê¼Êı¾İ																	*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Orig_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Orig_Data_Len		(11u)//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+2(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Sensor_Cal_Read_data_buffer_Size	(sizeof(float))//µ¥¾«¶È¸¡µãĞÍÊı¾İ³¤¶È
	COM_SENSOR_CAL_TYPEDEF read_Orig_data_ack;
	float com_data_buffer;

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	read_Orig_data_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_Orig_data_ack.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	read_Orig_data_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Orig_Data;
	read_Orig_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_Orig_data_ack.Data_Len		= Com_Sensor_Cal_Read_data_buffer_Size;
	
	
	/* ÅĞ¶Ï»ñÈ¡µÄÊÇ µÚ¼¸Í¨µÀ µÄ´«¸ĞÆ÷Êı¾İÇúÏß */
	/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data;
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			com_data_buffer			= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data;
			break;
		}
#endif	
		
		default:
		{
			/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */
			__nop();
			return;
		}
	}
	
	memcpy(read_Orig_data_ack.Com_Data,(u8*)&com_data_buffer,Com_Sensor_Cal_Read_data_buffer_Size);
	/* ÉèÖÃ ³£Êı */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size] 	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size+1] = Com_Sensor_Cal_Get_Check(&read_Orig_data_ack);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&read_Orig_data_ack,Com_Sensor_Cal_Read_Orig_Data_Len);
#undef Com_Sensor_Cal_Read_Orig_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
}
#else
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Read_Orig_Data														*/
/**	¹¦ÄÜËµÃ÷: »ñÈ¡´«¸ĞÆ÷Ô­Ê¼Êı¾İ																	*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Orig_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
#define Com_Sensor_Cal_Read_Orig_Data_Len		(9u)//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+2(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Sensor_Cal_Read_data_buffer_Size	(2u)//Êı¾İ³¤¶È£º2×Ö½Ú
#define Com_Sensor_Cal_Data_Ratio_K				(10u)//Êı¾İ·Å´ó±¶Êı
#define Com_Sensor_Cal_Temp_Data_Offset_Value	(2731u)//ÎÂ¶ÈÖµÊı¾İµÄÆ«ÒÆÖµ


	COM_SENSOR_CAL_TYPEDEF read_Orig_data_ack;
	u16 com_data_buffer;

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	read_Orig_data_ack.Head_Code	= Com_Sensor_Cal_Rece_Head_Code;
	read_Orig_data_ack.Instru_NO	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	read_Orig_data_ack.Cmd_Code 	= Com_Sensor_Cal_Cmd_Code_Read_Orig_Data;
	read_Orig_data_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_Orig_data_ack.Data_Len 	= Com_Sensor_Cal_Read_data_buffer_Size;
	
			
			/* ÅĞ¶Ï»ñÈ¡µÄÊÇ µÚ¼¸Í¨µÀ µÄ´«¸ĞÆ÷Êı¾İÇúÏß */
			/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
			switch(Cal_Inf->Chanel_Num)
			{
#if	(Sensor_CH1_Config_Type != Sensor_Null)
				case Sensors_CH_1:
				{
#if	((Sensor_CH1_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH1_Config_Type == Sensor_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH1.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
				case Sensors_CH_2:
				{
#if	((Sensor_CH2_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH2_Config_Type == Sensor_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH2.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
				case Sensors_CH_3:
				{
#if	((Sensor_CH3_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH3_Config_Type == Sensor_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH3.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
				case Sensors_CH_4:
				{
#if	((Sensor_CH4_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH4_Config_Type == Sensor_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH4.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif
					break;
				}
#endif	
				
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
				case Sensors_CH_5:
				{
#if	((Sensor_CH5_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH5_Config_Type == Sensor_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH5.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
				case Sensors_CH_6:
				{
#if	((Sensor_CH6_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH6_Config_Type == Sensor_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH6.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
				case Sensors_CH_7:
				{
#if	((Sensor_CH7_Config_Type == Sensor_Tmp)||\
					(Sensor_CH7_Config_Type == Sensor_DoorContact) || \
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH7_Config_Type == Sensor_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH7.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
				case Sensors_CH_8:
				{
#if	((Sensor_CH8_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH8_Config_Type == Sensor_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH8.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
				case Sensors_CH_9:
				{
#if	((Sensor_CH9_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH9_Config_Type == Sensor_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH9.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}				
#endif	
				
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
				case Sensors_CH_10:
				{
#if	((Sensor_CH10_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH10_Config_Type == Sensor_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH10.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
				case Sensors_CH_11:
				{
#if	((Sensor_CH11_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH11_Config_Type == Sensor_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH11.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
				case Sensors_CH_12:
				{
#if	((Sensor_CH12_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH12_Config_Type == Sensor_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH12.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
				case Sensors_CH_13:
				{
#if	((Sensor_CH13_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH13_Config_Type == Sensor_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH13.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
				case Sensors_CH_14:
			{
#if	((Sensor_CH14_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH14_Config_Type == Sensor_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH14.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
				case Sensors_CH_15:
				{
#if	((Sensor_CH15_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH15_Config_Type == Sensor_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH15.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
				case Sensors_CH_16:
				{
#if	((Sensor_CH16_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH16_Config_Type == Sensor_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH16.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
				case Sensors_CH_17:
				{
#if	((Sensor_CH17_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH17_Config_Type == Sensor_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH17.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
				case Sensors_CH_18:
				{
#if	((Sensor_CH18_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH18_Config_Type == Sensor_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH18.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
				case Sensors_CH_19:
				{
#if	((Sensor_CH19_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH19_Config_Type == Sensor_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH19.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
				case Sensors_CH_20:
				{
#if	((Sensor_CH20_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH20_Config_Type == Sensor_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH20.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
				case Sensors_CH_21:
				{
#if	((Sensor_CH21_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH21_Config_Type == Sensor_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH21.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
				case Sensors_CH_22:
				{
#if	((Sensor_CH22_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH22_Config_Type == Sensor_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH22.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
				case Sensors_CH_23:
				{
#if	((Sensor_CH23_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH23_Config_Type == Sensor_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH23.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
				case Sensors_CH_24:
				{
#if	((Sensor_CH24_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH24_Config_Type == Sensor_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH24.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
				case Sensors_CH_25:
				{
#if	((Sensor_CH25_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH25_Config_Type == Sensor_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH25.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
				case Sensors_CH_26:
				{
#if	((Sensor_CH26_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH26_Config_Type == Sensor_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH26.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
				case Sensors_CH_27:
				{
#if	((Sensor_CH27_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH27_Config_Type == Sensor_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH27.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
				case Sensors_CH_28:
				{
#if	((Sensor_CH28_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH28_Config_Type == Sensor_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH28.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}		
#endif	
				
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
				case Sensors_CH_29:
				{
#if	((Sensor_CH29_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH29_Config_Type == Sensor_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH29.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
				case Sensors_CH_30:
				{
#if	((Sensor_CH30_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH30_Config_Type == Sensor_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH30.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif		
				
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
				case Sensors_CH_31:
				{
#if	((Sensor_CH31_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH31_Config_Type == Sensor_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH31.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
				case Sensors_CH_32:
				{
#if	((Sensor_CH32_Config_Type == Sensor_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)||\
	 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K + Com_Sensor_Cal_Temp_Data_Offset_Value;
#elif ((Sensor_CH32_Config_Type == Sensor_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
					com_data_buffer 		= Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf.CH32.Sensor_Sampl_Data*Com_Sensor_Cal_Data_Ratio_K;
#endif		
					break;
				}
#endif	
				
				default:
				{
					/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */
					__nop();
					return;
				}
			}

	
	memcpy(read_Orig_data_ack.Com_Data,(u8*)&com_data_buffer,Com_Sensor_Cal_Read_data_buffer_Size);
	/* ÉèÖÃ ³£Êı */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	read_Orig_data_ack.Com_Data[Com_Sensor_Cal_Read_data_buffer_Size+1] = Com_Sensor_Cal_Get_Check(&read_Orig_data_ack);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&read_Orig_data_ack,Com_Sensor_Cal_Read_Orig_Data_Len);
#undef Com_Sensor_Cal_Read_Orig_Data_Len
#undef Com_Sensor_Cal_Read_data_buffer_Size
#undef Com_Sensor_Cal_Data_Ratio_K
#undef Com_Sensor_Cal_Temp_Data_Offset_Value
}
#endif


/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Write_Curve_Data													*/
/**	¹¦ÄÜËµÃ÷: ĞŞ¸Ä´«¸ĞÆ÷¶ÔÓ¦Í¨µÀÇúÏßÊı¾İ															*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Write_Curve_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* ´«¸ĞÆ÷Ğ£×¼ Ğ´ÇúÏß Êı¾İÖ¡³¤¶È */
#define Com_Sensor_Cal_Write_Curve_Data_Len			(1u)//×î´ó103¸ö´«ÊäÊı¾İ
/* ´«¸ĞÆ÷Ğ£×¼ Ğ´ÇúÏß Êı¾İÖ¡³¤¶È *///Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Sensor_Cal_Write_Curve_Packet_Len		(Com_Sensor_Cal_Write_Curve_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//×î´ó103¸ö´«ÊäÊı¾İ	
	
	/* ´«¸ĞÆ÷ÕæÊµÖµÊı¾İÖ¸Õë */
	u16* sensor_act_curve_data_p;
	/* ´«¸ĞÆ÷ÕæÊµÖµÊı¾İÖ¸Õë */
	u16* sensor_cal_curve_data_p;
	/* ·¢ËÍ ´«¸ĞÆ÷Ğ´ÇúÏß Êı¾İÖ¡ ¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF write_curve_ack;
	
	
	/* ÅĞ¶Ï»ñÈ¡µÄÊÇ µÚ¼¸Í¨µÀ µÄ´«¸ĞÆ÷Êı¾İÇúÏß */
	/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value;
			break;
		}
#endif	
		
		default:
		{
			/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */
			__nop();
			return;
		}
	}
	

	/* ¸üĞÂ¶ÔÓ¦´«¸ĞÆ÷Í¨µÀµÄĞ£×¼Êı¾İ */
	memcpy((u8*)sensor_cal_curve_data_p,Cal_Inf->Com_Data,Com_Sensor_Cal_Curve_Data_Len);
	memcpy((u8*)sensor_act_curve_data_p,&Cal_Inf->Com_Data[Com_Sensor_Cal_Curve_Data_Len],Com_Sensor_Cal_Curve_Data_Len);

	/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
	Write_Config_Tab((u8*)sensor_act_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	
	/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
	Write_Config_Tab((u8*)sensor_cal_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	
	/* ÉèÖÃ ÏìÓ¦Êı¾İÖ¡ ²ÎÊı */
	write_curve_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_curve_ack.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	write_curve_ack.Cmd_Code	= Com_Sensor_Cal_Cmd_Code_Write_Curve_Data;
	write_curve_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_curve_ack.Data_Len	= Com_Sensor_Cal_Write_Curve_Data_Len;
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	write_curve_ack.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ÉèÖÃ ³£Êı */
	write_curve_ack.Com_Data[1]	= Com_Sensor_Cal_Constant;	
	/* ÉèÖÃ Ğ£Ñé */
	write_curve_ack.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_curve_ack);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&write_curve_ack,Com_Sensor_Cal_Write_Curve_Packet_Len);
	
/* ´«¸ĞÆ÷Ğ£×¼ Ğ´ÇúÏß ÏàÓ¦Êı¾İ³¤¶È *///Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+96(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#undef Com_Sensor_Cal_Write_Curve_Data_Len
/* ´«¸ĞÆ÷Ğ£×¼ Ğ´ÇúÏß Êı¾İÖ¡³¤¶È */
#undef Com_Sensor_Cal_Write_Curve_Packet_Len
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Sensor_Cal_Read_Curve_Data														*/
/**	¹¦ÄÜËµÃ÷: ¶Á´«¸ĞÆ÷¶ÔÓ¦Í¨µÀÇúÏßÊı¾İ																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Sensor_Cal_Read_Curve_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ´«¸ĞÆ÷ÕæÊµÖµÊı¾İÖ¸Õë */
	u16* sensor_act_curve_data_p;
	/* ´«¸ĞÆ÷ÕæÊµÖµÊı¾İÖ¸Õë */
	u16* sensor_cal_curve_data_p;
	/* ·¢ËÍ ´«¸ĞÆ÷¶ÁÇúÏß Êı¾İÖ¡ ¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF read_curve_ack;
	
/* ´«¸ĞÆ÷Ğ£×¼ ¶ÁÇúÏß ÏàÓ¦Êı¾İ³¤¶È *///Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+32(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Sensor_Cal_Read_Curve_Data_Len		(Com_Sensor_Cal_Data_Buffer_Size)
/* ´«¸ĞÆ÷Ğ£×¼ ¶ÁÇúÏß Êı¾İÖ¡³¤¶È */
#define Com_Sensor_Cal_Read_Curve_Packet_Len	(Com_Sensor_Cal_Read_Curve_Data_Len+Com_Sensor_Cal_Fix_Data_Len)


	/* ÅĞ¶Ï»ñÈ¡µÄÊÇ µÚ¼¸Í¨µÀ µÄ´«¸ĞÆ÷Êı¾İÇúÏß */
	/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
	switch(Cal_Inf->Chanel_Num)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH1_Calibration_Value;
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH2_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH3_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH4_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH5_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH6_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH7_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH8_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH9_Calibration_Value;
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH10_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH11_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH12_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH13_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH14_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH15_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH16_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH17_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH18_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH19_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH20_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH21_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH22_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH23_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH24_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH25_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH26_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH27_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH28_Calibration_Value;
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH29_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH30_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH31_Calibration_Value;
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1ÕæÊµÖµÊı¾İÇúÏß */
			sensor_act_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Actual_Value;
			/* »ñÈ¡´«¸ĞÆ÷Í¨µÀ1Ğ£×¼ÖµÊı¾İÇúÏß */
			sensor_cal_curve_data_p = Instru_Config_Inf_Tab.Inf_State.Instru_Data_Tab.Inf_State.CH32_Calibration_Value;
			break;
		}
#endif	
		
		default:
		{
			/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */
			__nop();
			return;
		}
	}
	
	/* ÉèÖÃ ÏìÓ¦Êı¾İÖ¡ ²ÎÊı */
	read_curve_ack.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_curve_ack.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	read_curve_ack.Cmd_Code	 	= Com_Sensor_Cal_Cmd_Code_Read_Curve_Data;
	read_curve_ack.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_curve_ack.Data_Len		= Com_Sensor_Cal_Read_Curve_Data_Len;

	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	/* ÕâÀïĞ£×¼Öµ·ÅÇ°Ãæ£¨16×Ö½Ú£©£¬ÕæÊµÖµ·ÅºóÃæ£¨16×Ö½Ú£© */
	memcpy(read_curve_ack.Com_Data,(u8*)sensor_cal_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	memcpy(&read_curve_ack.Com_Data[Com_Sensor_Cal_Curve_Data_Len],(u8*)sensor_act_curve_data_p,Com_Sensor_Cal_Curve_Data_Len);
	/* ÉèÖÃ ³£Êı */
	read_curve_ack.Com_Data[read_curve_ack.Data_Len]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	read_curve_ack.Com_Data[read_curve_ack.Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_curve_ack);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ *///Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+16(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
	USART_printf(USART1,(u8*)&read_curve_ack,Com_Sensor_Cal_Read_Curve_Packet_Len);
	

/* ´«¸ĞÆ÷Ğ£×¼ ¶ÁÇúÏß ÏàÓ¦Êı¾İ³¤¶È *///Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+96(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#undef Com_Sensor_Cal_Read_Curve_Data_Len
/* ´«¸ĞÆ÷Ğ£×¼ ¶ÁÇúÏß Êı¾İÖ¡³¤¶È */
#undef Com_Sensor_Cal_Read_Curve_Packet_Len
}


/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_SN_Write																*/
/**	¹¦ÄÜËµÃ÷: Ğ´SNºÅ																				*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Config_SN_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* Ğ´SNºÅ Ó¦´ğµÄÊı¾İ³¤¶È */
#define Com_Write_SN_NO_Data_Len	(1u) //×î´ó1¸ö´«ÊäÊı¾İ
/* Ğ´SNºÅ Ó¦´ğĞ­Òé³¤¶È */
#define Com_Write_SN_NO_Len			(Com_Write_SN_NO_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
	/* Ğ´SNºÅ Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF write_sn_no;
	

	/* ÉèÖÃSNºÅ */
	Flash_Write_SN_NO(Cal_Inf->Com_Data);
	
	memcpy(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,Cal_Inf->Com_Data,sizeof(SERIAL_NUMBER));
				/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
 	Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,sizeof(SERIAL_NUMBER));
	
	

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	write_sn_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_sn_no.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	write_sn_no.Cmd_Code	= Com_Config_SN_Cmd_Code_Write;
	write_sn_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_sn_no.Data_Len	= Com_Write_SN_NO_Data_Len;
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	write_sn_no.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ÉèÖÃ ³£Êı */
	write_sn_no.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	write_sn_no.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_sn_no);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&write_sn_no,Com_Write_SN_NO_Len);
	
	
#undef Com_Write_SN_NO_Data_Len	
#undef Com_Read_SN_NO_Len
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_SN_Read																	*/
/**	¹¦ÄÜËµÃ÷: ¶ÁSNºÅ																				*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void static Com_Config_SN_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* ¶Á SN ºÅÓ¦´ğĞ­Òé³¤¶È */
#define Com_Read_SN_NO_Len	(ENV_SN_NO_SIZE+Com_Sensor_Cal_Fix_Data_Len)//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+6(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
	/* ¶Á SN ºÅÓ¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF read_sn_no;
	

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	read_sn_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_sn_no.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	read_sn_no.Cmd_Code	 	= Com_Config_SN_Cmd_Code_Read;
	read_sn_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_sn_no.Data_Len		= ENV_SN_NO_SIZE;
////	/* ¶ÁÈ¡SNºÅ */
////	Flash_Read_SN_NO(read_sn_no.Com_Data);
	
	memcpy(read_sn_no.Com_Data,&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number,sizeof(SERIAL_NUMBER));
	
	
	
	/* ÉèÖÃ ³£Êı */
	read_sn_no.Com_Data[ENV_SN_NO_SIZE] 		= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	read_sn_no.Com_Data[ENV_SN_NO_SIZE + 1] 	= Com_Sensor_Cal_Get_Check(&read_sn_no);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&read_sn_no,Com_Read_SN_NO_Len);
#undef Com_Read_SN_NO_Len
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Sensor_Write															*/
/**	¹¦ÄÜËµÃ÷: Ğ´´«¸ĞÆ÷ÅäÖÃ																			*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Sensor_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* Ğ´´«¸ĞÆ÷ÅäÖÃ Êı¾İ³¤¶È */
#define Com_Write_Sensor_Data_Len		(1u) //×î´ó1¸ö´«ÊäÊı¾İ
/* Ğ´´«¸ĞÆ÷ÅäÖÃ Êı¾İÖ¡³¤¶È */
#define Com_Write_Sensor_Len			(Com_Write_Sensor_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
/* Êı³¤¶È */
#define Write_Sensor_Data_Len			(sizeof(INSTRU_SENSOR_Old))
/* Êı³¤¶È */
#define Write_Sensor_Para_Data_Len		(sizeof(INSTRU_SENSOR_CHANNEL_FIX))
/* ´«¸ĞÆ÷Í¨µÀÖ¸Õë */
#define Sensor_Cha_P(cha)				(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[(cha-1)*sizeof(INSTRU_SENSOR_CHANNEL)])
	
	/* Ğ´´«¸ĞÆ÷ÅäÖÃ¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF write_sensor_config;
	/*  */
	INSTRU_SENSOR_CHANNEL_FIX	*sensor_para_p;
	/* ´«¸ĞÆ÷Í¨ĞÅÊı¾İ¼Ä´æÆ÷. */
	INSTRU_SENSOR_Old			sensor_data;


	/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
	switch(Cal_Inf->Chanel_Num-1)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			break;
		}
#endif	
		
		default:
		{
			/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */
			__nop();
			return;
		}
	}
	
	/* Çå³ı»º´æÊı¾İ */
	memset((u8*)&sensor_data,0x00,sizeof(INSTRU_SENSOR_Old));

	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	memcpy((u8*)&sensor_data,Cal_Inf->Com_Data,Write_Sensor_Data_Len);
	
	/* ½«»ñµÃµÄ´«¸ĞÆ÷Êı¾İ×ªÒÆµ½ÅäÖÃ±íÖĞ */
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_RF_Card_ID_NO = sensor_data.SENSOR_RF_Card_ID_NO;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[0] 	= sensor_data.SENSOR_SN_NO[0];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[1] 	= sensor_data.SENSOR_SN_NO[1];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[2] 	= sensor_data.SENSOR_SN_NO[2];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[3] 	= sensor_data.SENSOR_SN_NO[3];
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Up_Limit 	= sensor_data.SENSOR_Up_Limit;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Low_Limit 	= sensor_data.SENSOR_Low_Limit;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->Type 				= sensor_data.Type;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_Switch 		= (sensor_data.SENSOR_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Sound_Alarm_Switch = (sensor_data.SENSOR_Sound_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Light_Alarm_Switch = (sensor_data.SENSOR_Light_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SMS_Alarm_Switch 	= (sensor_data.SENSOR_SMS_Alarm_Switch == 1) ? TRUE : FALSE;
	((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_State =  0;
			
	/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
	Write_Config_Tab((u8*)Sensor_Cha_P(Cal_Inf->Chanel_Num),sizeof(INSTRU_SENSOR_CHANNEL));

	/* ÉèÖÃ´«¸ĞÆ÷Ïà¹ØµÄ²ÎÊıÖ¸Õë */
	sensor_para_p = (INSTRU_SENSOR_CHANNEL_FIX*)&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[(Cal_Inf->Chanel_Num-1)*Write_Sensor_Para_Data_Len];

	/* Çå³ı²ÎÊıÖ¸Õë */
	sensor_para_p->Sensor_Up_Alarm_Cnt = 0;	
	sensor_para_p->Sensor_Low_Alarm_Cnt = 0;
	sensor_para_p->Sensor_Suspend_Alarm_Flag = FALSE;

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	write_sensor_config.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_sensor_config.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	write_sensor_config.Cmd_Code	= Com_Config_Sensor_Cmd_Code_Write;
	write_sensor_config.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_sensor_config.Data_Len	= 1;
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	write_sensor_config.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ÉèÖÃ ³£Êı */
	write_sensor_config.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	write_sensor_config.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_sensor_config);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&write_sensor_config,Com_Write_Sensor_Len);
	
	
#undef Com_Write_Sensor_Data_Len
#undef Com_Write_Sensor_Len
#undef Write_Sensor_Data_Len
#undef Sensor_Cha_P
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Sensor_Read																*/
/**	¹¦ÄÜËµÃ÷: ¶Á´«¸ĞÆ÷ÅäÖÃ																			*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Sensor_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* ´«¸ĞÆ÷ÅäÖÃ Êı¾İ³¤¶È */
#define Com_Read_Sensor_Data_Len	(sizeof(INSTRU_SENSOR_Old))
/* ´«¸ĞÆ÷ÅäÖÃ Êı¾İÖ¡³¤¶È */
#define Com_Read_Sensor_Len			  (Com_Read_Sensor_Data_Len+Com_Sensor_Cal_Fix_Data_Len)//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
/* ´«¸ĞÆ÷Í¨µÀÖ¸Õë */
#define Sensor_Cha_P(cha)			    (&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[(cha-1)*sizeof(INSTRU_SENSOR_CHANNEL)])

	
	/* Ğ´´«¸ĞÆ÷ÅäÖÃ¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF read_sensor_config;
	/* ´«¸ĞÆ÷Í¨ĞÅÊı¾İ¼Ä´æÆ÷. */
	INSTRU_SENSOR_Old			sensor_data;


	/* »ñÈ¡¶ÔÓ¦Í¨µÀµÄÕæÊµÖµºÍĞ£×¼Öµ */
	switch(Cal_Inf->Chanel_Num-1)
	{
#if	(Sensor_CH1_Config_Type != Sensor_Null)	
		case Sensors_CH_1:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH2_Config_Type != Sensor_Null)	
		case Sensors_CH_2:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH3_Config_Type != Sensor_Null)	
		case Sensors_CH_3:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH4_Config_Type != Sensor_Null)	
		case Sensors_CH_4:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH5_Config_Type != Sensor_Null)	
		case Sensors_CH_5:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH6_Config_Type != Sensor_Null)	
		case Sensors_CH_6:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH7_Config_Type != Sensor_Null)	
		case Sensors_CH_7:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH8_Config_Type != Sensor_Null)	
		case Sensors_CH_8:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH9_Config_Type != Sensor_Null)	
		case Sensors_CH_9:
		{
			break;
		}				
#endif	
		
#if	(Sensor_CH10_Config_Type != Sensor_Null)	
		case Sensors_CH_10:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH11_Config_Type != Sensor_Null)	
		case Sensors_CH_11:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH12_Config_Type != Sensor_Null)	
		case Sensors_CH_12:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH13_Config_Type != Sensor_Null)	
		case Sensors_CH_13:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH14_Config_Type != Sensor_Null)	
		case Sensors_CH_14:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH15_Config_Type != Sensor_Null)	
		case Sensors_CH_15:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH16_Config_Type != Sensor_Null)	
		case Sensors_CH_16:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH17_Config_Type != Sensor_Null)	
		case Sensors_CH_17:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH18_Config_Type != Sensor_Null)	
		case Sensors_CH_18:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH19_Config_Type != Sensor_Null)	
		case Sensors_CH_19:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH20_Config_Type != Sensor_Null)	
		case Sensors_CH_20:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH21_Config_Type != Sensor_Null)	
		case Sensors_CH_21:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH22_Config_Type != Sensor_Null)	
		case Sensors_CH_22:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH23_Config_Type != Sensor_Null)	
		case Sensors_CH_23:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH24_Config_Type != Sensor_Null)	
		case Sensors_CH_24:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH25_Config_Type != Sensor_Null)	
		case Sensors_CH_25:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH26_Config_Type != Sensor_Null)	
		case Sensors_CH_26:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH27_Config_Type != Sensor_Null)	
		case Sensors_CH_27:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH28_Config_Type != Sensor_Null)	
		case Sensors_CH_28:
		{
			break;
		}		
#endif	
		
#if	(Sensor_CH29_Config_Type != Sensor_Null)	
		case Sensors_CH_29:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH30_Config_Type != Sensor_Null)	
		case Sensors_CH_30:
		{
			break;
		}
#endif		
		
#if	(Sensor_CH31_Config_Type != Sensor_Null)	
		case Sensors_CH_31:
		{
			break;
		}
#endif	
		
#if	(Sensor_CH32_Config_Type != Sensor_Null)	
		case Sensors_CH_32:
		{
			break;
		}
#endif	
		
		default:
		{
			/* »ñÈ¡µÄÍ¨µÀÄ¿±ê²»´æÔÚ */
			__nop();
			return;
		}
	}
	
	/* Çå³ı»º´æÊı¾İ */
	memset((u8*)&sensor_data,0x00,sizeof(INSTRU_SENSOR_Old));
	
	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	read_sensor_config.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_sensor_config.Instru_NO 	= Cal_Inf->Instru_NO;
	read_sensor_config.Cmd_Code	 	= Com_Config_Sensor_Cmd_Code_Read;
	read_sensor_config.Chanel_Num	= Cal_Inf->Chanel_Num;
	read_sensor_config.Data_Len		= Com_Read_Sensor_Data_Len;
	
	/* ½«»ñµÃµÄ´«¸ĞÆ÷Êı¾İ×ªÒÆµ½ÅäÖÃ±íÖĞ */	
	sensor_data.SENSOR_RF_Card_ID_NO 	= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_RF_Card_ID_NO;
	sensor_data.SENSOR_SN_NO[0] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[0];
	sensor_data.SENSOR_SN_NO[1] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[1];
	sensor_data.SENSOR_SN_NO[2] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[2];
	sensor_data.SENSOR_SN_NO[3] 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SN_NO[3];
	sensor_data.SENSOR_Up_Limit 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Up_Limit;
	sensor_data.SENSOR_Low_Limit 		= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Low_Limit;
	sensor_data.Type 					= ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->Type;
	sensor_data.SENSOR_Alarm_Switch 	= (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Sound_Alarm_Switch = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Sound_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Light_Alarm_Switch = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Light_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_SMS_Alarm_Switch   = (((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_SMS_Alarm_Switch == TRUE) ? 1 : 0;
	sensor_data.SENSOR_Alarm_State 		  = ((INSTRU_SENSOR_CHANNEL *)Sensor_Cha_P(Cal_Inf->Chanel_Num))->SENSOR_Alarm_State;
	
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	memcpy(read_sensor_config.Com_Data,(u8 *)&sensor_data,Com_Read_Sensor_Data_Len);
	/* ÉèÖÃ ³£Êı */
	read_sensor_config.Com_Data[Com_Read_Sensor_Data_Len]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	read_sensor_config.Com_Data[Com_Read_Sensor_Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_sensor_config);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&read_sensor_config,Com_Read_Sensor_Len);

#undef Com_Read_Sensor_Data_Len
#undef Com_Read_Sensor_Len
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Card_NO_Write															*/
/**	¹¦ÄÜËµÃ÷: Ğ´´òÓ¡³µÅÆºÅ																		*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF											*/
/**	·µ »Ø Öµ: ÎŞ																				*/
/************************************************************************************************/
static void Com_Config_Card_NO_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* Ğ´´òÓ¡³µÅÆºÅ Ó¦´ğµÄÊı¾İ³¤¶È */
#define Com_Write_Card_NO_Data_Len	(1u) //×î´ó1¸ö´«ÊäÊı¾İ
/* Ğ´´òÓ¡³µÅÆºÅ Ó¦´ğĞ­Òé³¤¶È */
//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Write_Card_NO_Len		(Com_Write_Card_NO_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* ´òÓ¡³µÅÆºÅµÄÊı¾İµØÖ· */
#define Card_NO_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO)

	/* Ğ´´òÓ¡³µÅÆºÅ Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF write_card_no;
	

	/* ÉèÖÃ´òÓ¡³µÅÆºÅ */
	/* Çå³ıÊı¾İÇøÄÚÈİ */
	memset(Card_NO_Data_P,'\0',Print_Card_NO_Size);
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	memcpy(Card_NO_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len >(9u))? (9u):Cal_Inf->Data_Len);
	/* ±£´æ ´òÓ¡³µÅÆºÅ ²ÎÊıÅäÖÃ */
	Write_Config_Tab((u8*)Card_NO_Data_P,sizeof(Card_NO_Data_P));

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	write_card_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_card_no.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	write_card_no.Cmd_Code		= Com_Config_Card_NO_Cmd_Code_Write;
	write_card_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_card_no.Data_Len		= Com_Write_Card_NO_Data_Len;
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	write_card_no.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ÉèÖÃ ³£Êı */
	write_card_no.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	write_card_no.Com_Data[2] 	= Com_Sensor_Cal_Get_Check(&write_card_no);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&write_card_no,Com_Write_Card_NO_Len);
	
	
#undef Com_Write_Card_NO_Data_Len	
#undef Com_Write_Card_NO_Len
#undef Card_NO_Data_P
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Card_NO_Read																	*/
/**	¹¦ÄÜËµÃ÷: ¶ÁSNºÅ																				*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Card_NO_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* ´òÓ¡³µÅÆºÅµÄÊı¾İµØÖ· */
#define Card_NO_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Card_NO)
	/* ¶Á´òÓ¡³µÅÆºÅÓ¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF read_card_no;
	

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	read_card_no.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_card_no.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	read_card_no.Cmd_Code	= Com_Config_Card_NO_Cmd_Code_Read;
	read_card_no.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_card_no.Data_Len	= strlen(Card_NO_Data_P);
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	memcpy(read_card_no.Com_Data,Card_NO_Data_P,read_card_no.Data_Len);
 
	/* ÉèÖÃ ³£Êı */
	*((u8*)&(read_card_no.Com_Data[read_card_no.Data_Len])) = Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	*((u8*)&(read_card_no.Com_Data[read_card_no.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&read_card_no);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&read_card_no,(read_card_no.Data_Len + Com_Sensor_Cal_Fix_Data_Len));
	

#undef Card_NO_Data_P
}


/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Company_Name_Read														*/
/**	¹¦ÄÜËµÃ÷: ¶ÁÅäÖÃ ´òÓ¡¹«Ë¾Ãû³Æ																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF											*/
/**	·µ »Ø Öµ: ÎŞ																				*/
/************************************************************************************************/
static void Com_Config_Company_Name_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* ´òÓ¡ ¹«Ë¾Ãû³Æ µÄÊı¾İµØÖ· */
#define Card_Cpy_Name_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name)
	/* ¶Á´òÓ¡³µÅÆºÅÓ¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF read_cpy_name;
	

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	read_cpy_name.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_cpy_name.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	read_cpy_name.Cmd_Code		= Com_Config_Company_Name_Cmd_Code_Read;
	read_cpy_name.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	read_cpy_name.Data_Len		= strlen(Card_Cpy_Name_P);
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	memcpy(read_cpy_name.Com_Data,Card_Cpy_Name_P,read_cpy_name.Data_Len);
	
	/* ÉèÖÃ ³£Êı */
	*((u8*)&(read_cpy_name.Com_Data[read_cpy_name.Data_Len])) = Com_Sensor_Cal_Constant;
	
	/* ÉèÖÃ Ğ£Ñé */
	*((u8*)&(read_cpy_name.Com_Data[read_cpy_name.Data_Len])+1) = Com_Sensor_Cal_Get_Check(&read_cpy_name);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&read_cpy_name,read_cpy_name.Data_Len + Com_Sensor_Cal_Fix_Data_Len);
	

#undef Card_Cpy_Name_P
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Company_Name_Write														*/
/**	¹¦ÄÜËµÃ÷: Ğ´ÅäÖÃ ´òÓ¡¹«Ë¾Ãû³Æ																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF											*/
/**	·µ »Ø Öµ: ÎŞ																				*/
/************************************************************************************************/
static void Com_Config_Company_Name_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
/* Ğ´´ò¹«Ë¾Ãû³Æ Ó¦´ğµÄÊı¾İ³¤¶È */
#define Com_Write_Cpy_Name_Data_Len	(1u) //×î´ó1¸ö´«ÊäÊı¾İ
/* Ğ´´ò¹«Ë¾Ãû³Æ Ó¦´ğĞ­Òé³¤¶È */
//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Write_Cpy_Name_Len		(Com_Write_Cpy_Name_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* ´òÓ¡¹«Ë¾Ãû³ÆµÄÊı¾İµØÖ· */
#define Cpy_Name_Data_P				(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Company_Name)

	
	/* Ğ´´òÓ¡¹«Ë¾Ãû³Æ Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF write_cpy_name;
	

	/* ÉèÖÃ´òÓ¡¹«Ë¾Ãû³Æ */
	/* Çå³ıÊı¾İÇøÄÚÈİ */
	memset(Cpy_Name_Data_P,'\0',Print_Company_Name_Size);
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	memcpy(Cpy_Name_Data_P,Cal_Inf->Com_Data,(Cal_Inf->Data_Len > (Print_Company_Name_Size - 1)) ? (Print_Company_Name_Size - 1):Cal_Inf->Data_Len);
	/* ±£´æ¹«Ë¾Ãû³ÆÅäÖÃ */
	Write_Config_Tab((u8*)Cpy_Name_Data_P,sizeof(Cpy_Name_Data_P));

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	write_cpy_name.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_cpy_name.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	write_cpy_name.Cmd_Code		= Com_Config_Company_Name_Cmd_Code_Write;
	write_cpy_name.Chanel_Num	= Com_Sensor_Cal_Read_Real_Data_CH_NO;
	write_cpy_name.Data_Len		= Com_Write_Cpy_Name_Data_Len;
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	write_cpy_name.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Succeed;
	/* ÉèÖÃ ³£Êı */
	write_cpy_name.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	write_cpy_name.Com_Data[2] 	= Com_Sensor_Cal_Get_Check(&write_cpy_name);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&write_cpy_name,Com_Write_Cpy_Name_Len);
	
	
#undef Com_Write_Cpy_Name_Data_Len
#undef Com_Write_Cpy_Name_Len
#undef Cpy_Name_Data_P
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Fad_Data_Write															*/
/**	¹¦ÄÜËµÃ÷: Ğ´´«¸ĞÆ÷Í¨µÀ×÷±×ÉÏÏÂÏŞÖµ																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Í¨µÀÊı¾İ×÷±×ÏûÏ¢Ìå£ºCOM_SENSOR_CAL_TYPEDEF										*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Fad_Data_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* ×ö×÷±×Í¨µÀÉÏÏŞÊı¾İÖ¸Õë */
#define Fad_Up_Lmt_Data_P(ch)			(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Up_Limit))
/* ×ö×÷±×Í¨µÀÏÂÏŞÊı¾İÖ¸Õë */
#define Fad_Low_Lmt_Data_P(ch)			(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Low_Limit))
/* Êı¾İ³¤¶È */
#define Com_Write_Fad_Data_Len			(1u)
/* Ó¦´ğĞ­Òé³¤¶È */
//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Write_Fad_Data_Func_Len		(Com_Write_Fad_Data_Len+Com_Sensor_Cal_Fix_Data_Len)
/* ÏûÏ¢ÌåÊı¾İ³¤¶È */
#define Com_Inf_Data_Len				(8u)
/* ´«¸ĞÆ÷Í¨µÀÉÏÏŞ²ÎÊı */
#define Sen_Up_Lmt_Data(ch)				(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Up_Limit)
/* ´«¸ĞÆ÷Í¨µÀÉÏÏŞ²ÎÊı */
#define Sen_Low_Lmt_Data(ch)			(((INSTRU_SENSOR_CHANNEL*)(&Instru_Config_Inf_Tab.Inf_State.Instru_Sensor.Inf_State_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL)]))->SENSOR_Low_Limit)


	/* ´«¸ĞÆ÷Í¨µÀÊı¾İ×÷±× Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF write_fad_data_func;

	/* Êı¾İºÏ·¨ĞÔ±êÖ¾ */
	BOOL data_vldy = TRUE;
	/* ×÷±×ÉÏÏÂÏŞÊı¾İ²ÎÊı */
	float* fad_up_lmt = (float*)Cal_Inf->Com_Data;
	float* fad_low_lmt = (float*)(&Cal_Inf->Com_Data[sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)))]);

	/* ÅĞ¶ÏÉèÖÃµÄÊı¾İÊÇ·ñºÏ·¨ */
	/* ÅĞ¶ÏÍ¨µÀÊÇ·ñºÏ·¨ */
	/* »ñÈ¡Í¨µÀ²ÎÊı¼ì²â */
	data_vldy = Log_Com_Sensor_CH_Check((Cal_Inf->Chanel_Num-1));
	
	/* ÅĞ¶ÏÊÇ·ñºÏ·¨ */
	if((data_vldy == TRUE)&&(Cal_Inf->Data_Len != Com_Inf_Data_Len))
	{
		/* Êı¾İ³¤¶È²»ºÏ·¨ */
		data_vldy = FALSE;
	}
	
	/* ÅĞ¶Ï²ÎÊıºÏ·¨ĞÔ */
	if(((data_vldy == TRUE)&&(*fad_up_lmt < *fad_low_lmt))||
	   (*fad_up_lmt > 1000)||(*fad_low_lmt < -1000))
	{
		/* ²ÎÊı²»ºÏ·¨ */
		data_vldy = FALSE;
		
	}else
	{
		/* Êı¾İ³¤¶ÈºÏ·¨ */
		/* ×÷±×ÉÏÏŞ´óÓÚÏÂÏŞ */
		if((*fad_low_lmt > Sen_Up_Lmt_Data((Cal_Inf->Chanel_Num-1)))||(*fad_up_lmt < Sen_Low_Lmt_Data((Cal_Inf->Chanel_Num-1))))
		{
			/* ×÷±×ÉÏÏÂÏŞ²»ÔÚ´«¸ĞÆ÷ÉÏÏÂÏŞ·¶Î§ÄÚ */
			/* ²ÎÊı²»ºÏ·¨ */
			data_vldy = FALSE;
		}
	}
		
	/* ÅĞ¶ÏÊı¾İºÏ·¨ĞÔ±êÖ¾ */
	if(data_vldy == TRUE)
	{
		/* ²ÎÊıºÏ·¨ */
		/* ÉèÖÃ×÷±×´«¸ĞÆ÷ÉÏÏÂÏŞ²ÎÊı */
		/* ÉèÖÃÉÏÏŞ	*/
		memcpy(Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),Cal_Inf->Com_Data,sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
		/* ÉèÖÃÏÂÏŞ */
		memcpy(Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),&Cal_Inf->Com_Data[sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)))],sizeof(*Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));

		/* ±£´æ²ÎÊı */
		/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
		Write_Config_Tab((u8*)Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),sizeof(*Fad_Up_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
		
		/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
		Write_Config_Tab((u8*)Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1)),sizeof(*Fad_Low_Lmt_Data_P((Cal_Inf->Chanel_Num-1))));
	}


	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	write_fad_data_func.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	write_fad_data_func.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	write_fad_data_func.Cmd_Code	= Com_Config_Fad_Data_Cmd_Code_Write;
	write_fad_data_func.Chanel_Num	= Cal_Inf->Chanel_Num;
	write_fad_data_func.Data_Len	= Com_Write_Fad_Data_Len;
	
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	write_fad_data_func.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ÉèÖÃ ³£Êı */
	write_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	write_fad_data_func.Com_Data[2] = Com_Sensor_Cal_Get_Check(&write_fad_data_func);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&write_fad_data_func,Com_Write_Fad_Data_Func_Len);
	

#undef Fad_Up_Lmt_Data_P
#undef Fad_Low_Lmt_Data_P
#undef Com_Write_Fad_Data_Len
#undef Com_Write_Fad_Data_Func_Len
#undef Com_Inf_Data_Len
#undef Sen_Up_Lmt_Data
#undef Sen_Low_Lmt_Data
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Fad_Data_Read															*/
/**	¹¦ÄÜËµÃ÷: ¶Á´«¸ĞÆ÷Í¨µÀ×÷±×ÉÏÏÂÏŞÖµ																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Í¨µÀÊı¾İ×÷±×ÏûÏ¢Ìå£ºCOM_SENSOR_CAL_TYPEDEF										*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Fad_Data_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

/* ×ö×÷±×Í¨µÀÊı¾İÖ¸Õë */
#define Fad_Data_P(ch)			((char*)(&(((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[ch*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Fraud_Up_Limit)))

/* Êı¾İ³¤¶È */
#define Com_Read_Fad_Data_Len	(8u)

/* ÏûÏ¢ÌåÊı¾İ³¤¶È */
#define Com_Inf_Data_Len		(0u)

/* Ó¦´ğĞ­Òé³¤¶È */
//Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+8(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
#define Com_Read_Fad_Len		(Com_Read_Fad_Data_Len+Com_Sensor_Cal_Fix_Data_Len)


	/* ´«¸ĞÆ÷Í¨µÀÊı¾İ×÷±× Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF 		read_fad_data_func;

	/* Êı¾İºÏ·¨ĞÔ±êÖ¾ */
	BOOL data_vldy = TRUE;

	/* ÅĞ¶ÏÉèÖÃµÄÊı¾İÊÇ·ñºÏ·¨ */
	/* ÅĞ¶ÏÍ¨µÀÊÇ·ñºÏ·¨ */
	/* »ñÈ¡Í¨µÀ²ÎÊı¼ì²â */
	data_vldy = Log_Com_Sensor_CH_Check((Cal_Inf->Chanel_Num-1));

	/* ÅĞ¶ÏÊÇ·ñºÏ·¨ */
	if((data_vldy == TRUE)&&(Cal_Inf->Data_Len != Com_Inf_Data_Len))
	{
		/* Êı¾İ³¤¶È²»ºÏ·¨ */
		data_vldy = FALSE;
	}

	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	read_fad_data_func.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	read_fad_data_func.Instru_NO 	= Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	read_fad_data_func.Cmd_Code		= Com_Config_Fad_Data_Cmd_Code_Read;
	read_fad_data_func.Chanel_Num	= Cal_Inf->Chanel_Num;
	
	if(data_vldy == TRUE)
	{
		/* ÉèÖÃÊı¾İ³¤¶È */
		read_fad_data_func.Data_Len		= Com_Read_Fad_Data_Len;

		/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
		memcpy(read_fad_data_func.Com_Data,Fad_Data_P((Cal_Inf->Chanel_Num-1)),Com_Read_Fad_Data_Len);
		
		/* ÉèÖÃ ³£Êı */
		read_fad_data_func.Com_Data[Com_Read_Fad_Data_Len]		= Com_Sensor_Cal_Constant;
		
		/* ÉèÖÃ Ğ£Ñé */
		read_fad_data_func.Com_Data[Com_Read_Fad_Data_Len+1]	= Com_Sensor_Cal_Get_Check(&read_fad_data_func);	
	}else
	{
		/* ÉèÖÃÊı¾İ³¤¶È */
		read_fad_data_func.Data_Len		= 1;
		
		/* ÉèÖÃ ³£Êı */
		read_fad_data_func.Com_Data[0]	= Com_Sensor_Cal_Set_Para_Fail;
		
		/* ÉèÖÃ ³£Êı */
		read_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Constant;
		
		/* ÉèÖÃ Ğ£Ñé */
		read_fad_data_func.Com_Data[1]	= Com_Sensor_Cal_Get_Check(&read_fad_data_func);
	}

	
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&read_fad_data_func,Com_Read_Fad_Len);


#undef Fad_Data_P
#undef Com_Read_Fad_Data_Len
#undef Com_Inf_Data_Len
#undef Com_Read_Fad_Len
}


/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_DataNetwork_Read														*/
/**	¹¦ÄÜËµÃ÷: ¶ÁÊı¾İ·şÎñÆ÷µÄÅäÖÃ²ÎÊı																*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_DataNetwork_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* ÍøÂçÅäÖÃÖ¸Õë */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* ÓòÃû×Ö·û´®³¤¶È */
	u32	domainNameLen;
	
	
	/* ÉèÖÃ¾Ö²¿±äÁ¿³õÊ¼Öµ */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* ¼ÆËãÓòÃû×Ö·û´®³¤¶È */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1);
	/* ÅĞ¶ÏÓòÃûµÄÊı¾İ³¤¶È */
	if(domainNameLen > 31u)
	{
		/* ´óÓÚ31£¬ÔòÉèÖÃÎª×î´ó31 */
		domainNameLen = 31u;
	}
	
	/* ÅäÖÃÊı¾İ·şÎñÆ÷²ÎÊı */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigDataNetworkRead;
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* ÉèÖÃ·¢ËÍÊı¾İÊı¾İ */
	/* ÉèÖÃIP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address;
	/* ÉèÖÃ¶Ë¿Ú */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1;
	/* ÉèÖÃÓòÃû */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
		   domainNameLen);
		   
	/* ÉèÖÃ ³£Êı */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* ÉèÖÃ Ğ£Ñé */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}



/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_DataNetwork_Read														*/
/**	¹¦ÄÜËµÃ÷: ¶ÁÅäÖÃ·şÎñÆ÷µÄÅäÖÃ²ÎÊı																*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Network_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* ÍøÂçÅäÖÃÖ¸Õë */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* ÓòÃû×Ö·û´®³¤¶È */
	u32	domainNameLen;
	
	
	/* ÉèÖÃ¾Ö²¿±äÁ¿³õÊ¼Öµ */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* ¼ÆËãÓòÃû×Ö·û´®³¤¶È */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2);
	/* ÅĞ¶ÏÓòÃûµÄÊı¾İ³¤¶È */
	if(domainNameLen > 31u)
	{
		/* ´óÓÚ31£¬ÔòÉèÖÃÎª×î´ó31 */
		domainNameLen = 31u;
	}
	
	/* ÅäÖÃÊı¾İ·şÎñÆ÷²ÎÊı */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigNetworkRead;//¶ÁÅäÖÃ·şÎñÆ÷²Ù×÷Âë
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* ÉèÖÃ·¢ËÍÊı¾İÊı¾İ */
	/* ÉèÖÃIP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address;
	/* ÉèÖÃ¶Ë¿Ú */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2;
	/* ÉèÖÃÓòÃû */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
		   domainNameLen);
		   
	/* ÉèÖÃ ³£Êı */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* ÉèÖÃ Ğ£Ñé */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Upgrade_Network_Read											*/
/**	¹¦ÄÜËµÃ÷: ¶ÁÉı¼¶·şÎñÆ÷µÄÅäÖÃ²ÎÊı																*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Upgrade_Network_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	/* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkRead;
	/* ÍøÂçÅäÖÃÖ¸Õë */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)ackDataNetworkRead.Com_Data;
	/* ÓòÃû×Ö·û´®³¤¶È */
	u32	domainNameLen;
	
	
	/* ÉèÖÃ¾Ö²¿±äÁ¿³õÊ¼Öµ */
	memset((u8*)&ackDataNetworkRead,0x00,sizeof(COM_SENSOR_CAL_TYPEDEF));
	
	/* ¼ÆËãÓòÃû×Ö·û´®³¤¶È */
	domainNameLen = strlen((char*)Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3);
	/* ÅĞ¶ÏÓòÃûµÄÊı¾İ³¤¶È */
	if(domainNameLen > 31u)
	{
		/* ´óÓÚ31£¬ÔòÉèÖÃÎª×î´ó31 */
		domainNameLen = 31u;
	}
	
	/* ÅäÖÃÊı¾İ·şÎñÆ÷²ÎÊı */
	ackDataNetworkRead.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkRead.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkRead.Cmd_Code		= ComConfigUpgradeNetworkRead;//¶ÁÉı¼¶·şÎñÆ÷²Ù×÷Âë
	ackDataNetworkRead.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkRead.Data_Len		= 38u;

	/* ÉèÖÃ·¢ËÍÊı¾İÊı¾İ */
	/* ÉèÖÃIP */
	networkDatap->IP.Server_Address = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address;
	/* ÉèÖÃ¶Ë¿Ú */
	networkDatap->Port = Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3;
	/* ÉèÖÃÓòÃû */
	memcpy(networkDatap->DomainName,
		   Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
		   domainNameLen);
		   
	/* ÉèÖÃ ³£Êı */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len] = Com_Sensor_Cal_Constant;
	
	/* ÉèÖÃ Ğ£Ñé */
	ackDataNetworkRead.Com_Data[ackDataNetworkRead.Data_Len+1] = Com_Sensor_Cal_Get_Check(&ackDataNetworkRead);

	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&ackDataNetworkRead,(Com_Sensor_Cal_Fix_Data_Len+ackDataNetworkRead.Data_Len));

}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_DataNetwork_Write														*/
/**	¹¦ÄÜËµÃ÷: Ğ´Êı¾İ·şÎñÆ÷µÄÅäÖÃ²ÎÊı																*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_DataNetwork_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ÍøÂçÅäÖÃÖ¸Õë */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* Êı¾İºÏ·¨ĞÔ±êÖ¾ */
	BOOL data_vldy = TRUE;
	/* ÓòÃû×Ö·û´®³¤¶È */
	u32	domainNameLen;
	
	/* ºÏ·¨ĞÔ¼ì²â */
	if(data_vldy == TRUE)
	{
		/* ¼ì²âÓòÃûÊı¾İ³¤¶È */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* ÅĞ¶ÏÓàÃû³¤¶ÈÊÇ·ñºÏÀí */
		if(domainNameLen > 31u)
		{
			/* ÓòÃûÊı¾İ³¤¶È²»ºÏÀí */
			data_vldy = FALSE;
		}
	}
	
	
	/* ÅĞ¶ÏÊı¾İºÏ·¨ĞÔ±êÖ¾ */
	if(data_vldy == TRUE)
	{
		/* »ñÈ¡ÍøÂçÊı¾İ */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1 = networkDatap->Port;
		/* ÉèÖÃÇå³ıÓòÃû¼Ä´æÆ÷ */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,networkDatap->DomainName,domainNameLen);

		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦IPµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID1.Server_Address));
		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦¶Ë¿ÚµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID1));
		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦ÓòÃûµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID1));	
	}
	
	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigDataNetworkWrite;
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ÉèÖÃ ³£Êı */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}



/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_DataNetwork_Write														*/
/**	¹¦ÄÜËµÃ÷: Ğ´ÅäÖÃ·şÎñÆ÷µÄÅäÖÃ²ÎÊı																*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Network_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ÍøÂçÅäÖÃÖ¸Õë */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* Êı¾İºÏ·¨ĞÔ±êÖ¾ */
	BOOL data_vldy = TRUE;
	/* ÓòÃû×Ö·û´®³¤¶È */
	u32	domainNameLen;
	
	/* ºÏ·¨ĞÔ¼ì²â */
	if(data_vldy == TRUE)
	{
		/* ¼ì²âÓòÃûÊı¾İ³¤¶È */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* ÅĞ¶ÏÓàÃû³¤¶ÈÊÇ·ñºÏÀí */
		if(domainNameLen > 31u)
		{
			/* ÓòÃûÊı¾İ³¤¶È²»ºÏÀí */
			data_vldy = FALSE;
		}
	}
	
	
	/* ÅĞ¶ÏÊı¾İºÏ·¨ĞÔ±êÖ¾ */
	if(data_vldy == TRUE)
	{
		/* »ñÈ¡ÍøÂçÊı¾İ */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2 = networkDatap->Port;
		/* ÉèÖÃÇå³ıÓòÃû¼Ä´æÆ÷ */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,networkDatap->DomainName,domainNameLen);

		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦IPµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID2.Server_Address));
		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦¶Ë¿ÚµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID2));
		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦ÓòÃûµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID2));	
	}
	
	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigNetworkWrite;//ÅäÖÃ·şÎñÆ÷²Ù×÷Âë
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ÉèÖÃ ³£Êı */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}


/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Upgrade_Network_Write												*/
/**	¹¦ÄÜËµÃ÷: Ğ´Éı¼¶·şÎñÆ÷µÄÅäÖÃ²ÎÊı																*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Upgrade_Network_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{
	/* ÍøÂçÅäÖÃÖ¸Õë */
	NetworkConfigStruct* networkDatap = (NetworkConfigStruct*)Cal_Inf->Com_Data;
	/* Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF ackDataNetworkWrite;
	/* Êı¾İºÏ·¨ĞÔ±êÖ¾ */
	BOOL data_vldy = TRUE;
	/* ÓòÃû×Ö·û´®³¤¶È */
	u32	domainNameLen;
	
	/* ºÏ·¨ĞÔ¼ì²â */
	if(data_vldy == TRUE)
	{
		/* ¼ì²âÓòÃûÊı¾İ³¤¶È */
		domainNameLen = strlen((char*)networkDatap->DomainName);
		
		/* ÅĞ¶ÏÓàÃû³¤¶ÈÊÇ·ñºÏÀí */
		if(domainNameLen > 31u)
		{
			/* ÓòÃûÊı¾İ³¤¶È²»ºÏÀí */
			data_vldy = FALSE;
		}
	}
	
	
	/* ÅĞ¶ÏÊı¾İºÏ·¨ĞÔ±êÖ¾ */
	if(data_vldy == TRUE)
	{
		/* »ñÈ¡ÍøÂçÊı¾İ */
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3 = networkDatap->IP;
		Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3 = networkDatap->Port;
		/* ÉèÖÃÇå³ıÓòÃû¼Ä´æÆ÷ */
		memset(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,'\0',32);
		memcpy(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,networkDatap->DomainName,domainNameLen);

		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦IPµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.IP_Address_ID3.Server_Address));
		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦¶Ë¿ÚµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Port_ID3));
		/* ĞŞ¸ÄÒÇÆ÷ÅäÖÃ±íÖĞ¶ÔÓ¦ÓòÃûµÄÊı¾İ */
		Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3,
						sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Domain_Name_ID3));	
	}
	
	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	ackDataNetworkWrite.Head_Code 	= Com_Sensor_Cal_Rece_Head_Code;
	ackDataNetworkWrite.Instru_NO 	= Cal_Inf->Instru_NO;
	ackDataNetworkWrite.Cmd_Code	= ComConfigUpgradeNetworkWrite;//Ğ´Éı¼¶·şÎñÆ÷²Ù×÷Âë
	ackDataNetworkWrite.Chanel_Num	= Cal_Inf->Chanel_Num;
	ackDataNetworkWrite.Data_Len	= (1u);
	
	/* ½«Êı¾İ¿½±´µ½·¢ËÍ¼Ä´æÆ÷ÖĞ */
	ackDataNetworkWrite.Com_Data[0]	= ((data_vldy == TRUE) ? Com_Sensor_Cal_Set_Para_Succeed : Com_Sensor_Cal_Set_Para_Fail);
	/* ÉèÖÃ ³£Êı */
	ackDataNetworkWrite.Com_Data[1]	= Com_Sensor_Cal_Constant;
	/* ÉèÖÃ Ğ£Ñé */
	ackDataNetworkWrite.Com_Data[2] = Com_Sensor_Cal_Get_Check(&ackDataNetworkWrite);
	/* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
	USART_printf(USART1,(u8*)&ackDataNetworkWrite,(Com_Sensor_Cal_Fix_Data_Len+1));
}












































/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_SMSAamPoeNOSet												*/
/**	¹¦ÄÜËµÃ÷: ÉèÖÃ¶ÌĞÅ±¨¾¯ºÅÂë															*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
///************************************************************************************************/
//static void Com_Config_SMSAamPoeNOSet (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
/* ¶ÌĞÅ±¨¾¯ºÅÂëÊı¾İÖ¸Õë. */
#define SMS_Aam_Poe_NO_P_1(num) (((MOBILE_NO*)(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM.Inf_State.Alarm_SMS_NO1))+num)
static void Com_Config_SMSAamPoeNOSet (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	/* Ğ´±¨¾¯ÊÖ»úºÅ Ó¦´ğµÄÊı¾İ³¤¶È */
  #define Com_Write_SMSAamPoeNOSet_Data_Len	(1u) //×î´ó1¸ö´«ÊäÊı¾İ
	
	#define Com_Write_SMSAamPoeNOSet_Len	(Com_Write_SMSAamPoeNOSet_Data_Len+Com_Config_SMSAamPoeNO_Fix_Data_Len) //Êı¾İ³¤¶È£º[1(Í·Âë)+1(Éè±¸µØÖ·)+1(²Ù×÷Âë)+1(Í¨µÀºÅ)]+1(Êı¾İ³¤¶È)+1(Êı¾İ)+1(³£Êı)+1(Ğ£ÑéºÍ)
	
	
	   MOBILE_NO *sms_no_p;//ÊÖ»ú ºÅÂë ½á¹¹Ìå
		/* Ğ´±¨¾¯ÊÖ»úºÅ Ó¦´ğĞ­Òé¼Ä´æÆ÷ */
	COM_SENSOR_CAL_TYPEDEF      write_SMSAamPoeNOSet;//½ÓÊÕ´®¿ÚÊı¾İ
/**********************´¦ÀíÉÏÎ»»úÖ¸Áî*****************************************/
	sms_no_p = SMS_Aam_Poe_NO_P_1(Cal_Inf->Chanel_Num-1);//ÉèÖÃÊÖ»úºÅÂëÎ»ÖÃµØÖ·£¬²ÎÊı0:´ú±íºÅÂëÒ»,²ÎÊı1:´ú±íºÅÂë ¶ş,²ÎÊı2:´ú±íºÅÂëÈı;
	if(Cal_Inf->Data_Len==Check_SMSAamPhone_Len)
	{/* ½«ÉèÖÃµÄ¶ÌĞÅºÅÂëĞ´ÈëÊı¾İ¼Ä´æÆ÷ */
		sms_no_p->Serial_Number_Number.Number_1  = (Cal_Inf->Com_Data[0]-0x30);
		sms_no_p->Serial_Number_Number.Number_2  = (Cal_Inf->Com_Data[1]-0x30);
		sms_no_p->Serial_Number_Number.Number_3  = (Cal_Inf->Com_Data[2]-0x30);
		sms_no_p->Serial_Number_Number.Number_4  = (Cal_Inf->Com_Data[3]-0x30);
		sms_no_p->Serial_Number_Number.Number_5  = (Cal_Inf->Com_Data[4]-0x30);
		sms_no_p->Serial_Number_Number.Number_6  = (Cal_Inf->Com_Data[5]-0x30);
		sms_no_p->Serial_Number_Number.Number_7  = (Cal_Inf->Com_Data[6]-0x30);
		sms_no_p->Serial_Number_Number.Number_8  = (Cal_Inf->Com_Data[7]-0x30);
		sms_no_p->Serial_Number_Number.Number_9  = (Cal_Inf->Com_Data[8]-0x30);
		sms_no_p->Serial_Number_Number.Number_10 = (Cal_Inf->Com_Data[9]-0x30);
		sms_no_p->Serial_Number_Number.Number_11 = (Cal_Inf->Com_Data[10]-0x30);
		/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
		Write_Config_Tab((u8*)sms_no_p,sizeof(MOBILE_NO));
/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
   /* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
    write_SMSAamPoeNOSet.Head_Code =Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
    write_SMSAamPoeNOSet.Instru_NO =Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
    write_SMSAamPoeNOSet.Cmd_Code  =Com_Config_SMSAamPoeNO_Cmd_Code_Write;//Ğ´±¨¾¯ÊÖ»úºÅÂë²Ù×÷Âë
		write_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//¶ÔÓ¦µÄºÅÂëĞòºÅ
		write_SMSAamPoeNOSet.Data_Len  =0x01;
		write_SMSAamPoeNOSet.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/
		write_SMSAamPoeNOSet.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	  write_SMSAamPoeNOSet.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_SMSAamPoeNOSet);/* ÉèÖÃ Ğ£Ñé */
   /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
  	USART_printf(USART1,(u8*)&write_SMSAamPoeNOSet,Com_Write_SMSAamPoeNOSet_Len);
	}
	else
	{
		
	/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
   /* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
    write_SMSAamPoeNOSet.Head_Code =Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
    write_SMSAamPoeNOSet.Instru_NO =Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
    write_SMSAamPoeNOSet.Cmd_Code  =Com_Config_SMSAamPoeNO_Cmd_Code_Write;//Ğ´±¨¾¯ÊÖ»úºÅÂë²Ù×÷Âë
		write_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//¶ÔÓ¦µÄºÅÂëĞòºÅ
		write_SMSAamPoeNOSet.Data_Len  =0x01;
		write_SMSAamPoeNOSet.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*²Ù×÷Ê§°Ü±êÖ¾*/
		write_SMSAamPoeNOSet.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	  write_SMSAamPoeNOSet.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_SMSAamPoeNOSet);/* ÉèÖÃ Ğ£Ñé */
   /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
  	USART_printf(USART1,(u8*)&write_SMSAamPoeNOSet,Com_Write_SMSAamPoeNOSet_Len);
	
	}

}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_SMSAamPoeNORead												*/
/**	¹¦ÄÜËµÃ÷:¶ÁÈ¡¶ÌĞÅ±¨¾¯ºÅÂë															*/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
///************************************************************************************************/
static void Com_Config_SMSAamPoeNORead (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
       MOBILE_NO *sms_no_p;//ÊÖ»ú ºÅÂë ½á¹¹Ìå 
       COM_SENSOR_CAL_TYPEDEF      read_SMSAamPoeNOSet;//·¢ËÍ´®¿ÚÊı¾İ(°üº¬ÊÖ»úºÅĞÅÏ¢)
/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
     /* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
sms_no_p = SMS_Aam_Poe_NO_P_1(Cal_Inf->Chanel_Num-1);//¶¨Î»µ½Òª·¢ËÍÊÖ»úºÅÂëÎ»ÖÃµØÖ·£¬²ÎÊı0:´ú±íºÅÂëÒ»,²ÎÊı1:´ú±íºÅÂë ¶ş,²ÎÊı2:´ú±íºÅÂëÈı;	

	    read_SMSAamPoeNOSet.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	    read_SMSAamPoeNOSet.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	    read_SMSAamPoeNOSet.Cmd_Code =Com_Config_SMSAamPoeNO_Cmd_Code_Read;//¶Á±¨¾¯ÊÖ»úºÅÂë²Ù×÷Âë
			read_SMSAamPoeNOSet.Chanel_Num=Cal_Inf->Chanel_Num;//¶ÔÓ¦µÄºÅÂëĞòºÅ
	    read_SMSAamPoeNOSet.Data_Len=Check_SMSAamPhone_Len_Read;//ÊÖ»úºÅÂë³¤¶È;Ä¬ÈÏÎª11Î»
	                                     
	
      read_SMSAamPoeNOSet.Com_Data[0]=(sms_no_p->Serial_Number_Number.Number_1+0x30);
	    read_SMSAamPoeNOSet.Com_Data[1]=(sms_no_p->Serial_Number_Number.Number_2+0x30);
      read_SMSAamPoeNOSet.Com_Data[2]=(sms_no_p->Serial_Number_Number.Number_3+0x30);
	    read_SMSAamPoeNOSet.Com_Data[3]=(sms_no_p->Serial_Number_Number.Number_4+0x30);
	    read_SMSAamPoeNOSet.Com_Data[4]=(sms_no_p->Serial_Number_Number.Number_5+0x30);
	    read_SMSAamPoeNOSet.Com_Data[5]=(sms_no_p->Serial_Number_Number.Number_6+0x30);
      read_SMSAamPoeNOSet.Com_Data[6]=(sms_no_p->Serial_Number_Number.Number_7+0x30);
	    read_SMSAamPoeNOSet.Com_Data[7]=(sms_no_p->Serial_Number_Number.Number_8+0x30);
	    read_SMSAamPoeNOSet.Com_Data[8]=(sms_no_p->Serial_Number_Number.Number_9+0x30);
      read_SMSAamPoeNOSet.Com_Data[9]=(sms_no_p->Serial_Number_Number.Number_10+0x30);
	    read_SMSAamPoeNOSet.Com_Data[10]=(sms_no_p->Serial_Number_Number.Number_11+0x30);
	    read_SMSAamPoeNOSet.Com_Data[11]=0;
	    read_SMSAamPoeNOSet.Com_Data[12]=0;
	    read_SMSAamPoeNOSet.Com_Data[13]=0;
	    read_SMSAamPoeNOSet.Com_Data[14]=0;
			read_SMSAamPoeNOSet.Com_Data[15]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */;
			read_SMSAamPoeNOSet.Com_Data[16]=Com_Sensor_Cal_Get_Check(&read_SMSAamPoeNOSet);/* ÉèÖÃ Ğ£Ñé */;

	  /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
  	USART_printf(USART1,(u8*)&read_SMSAamPoeNOSet,22);
	                                               
}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Time_Interval_Set											*/
/**	¹¦ÄÜËµÃ÷:ÉèÖÃÊ±¼ä¼ä¸ô£¨ÉÏ´«Ê±¼ä¼ä¸ô¡¢ÀúÊ·¼ÇÂ¼Ê±¼ä¼ä¸ô¡¢´òÓ¡Ê±¼ä¼ä¸ô¡¢ ±¨¾¯¼ÇÂ¼¼ä¸ô£©****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
/* ·Ç³¬±ê×´Ì¬´òÓ¡Ê±¼ä¼ä¸ôÊı¾İÖ¸Õë */
#define Pit_Time_Irl_Data_P_1  (&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Print.Inf_State.Print_Nrl_Itl)
/* " Õı³£´æ´¢¼ä¸ô"Êı¾İÖ¸Õë */
#define Menu_Nml_Save_Irl_Data_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Normal_Save_Data_Interval)
/* "±¨¾¯´æ´¢¼ä¸ô"Êı¾İÖ¸Õë */
#define Menu_Aam_Save_Irl_Data_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Alarm_Save_Data_Interval)
static void Com_Config_Time_Interval_Set (COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
		COM_SENSOR_CAL_TYPEDEF      write_Time_Interval_Set;//½ÓÊÕ´®¿ÚÊı¾İ
     /* ÉèÖÃµÄÊ±¼ä²ÎÊı */
     u32 normalsavetime;//´òÓ¡¼ä¸ô
	 //  u32 uploadtime;//ÉÏ´«¼ä¸ô¼ä¸ô
	   u32 alarm_intervaltime;//±¨¾¯¼ÇÂ¼¼ä¸ô
	   u32 normalsavetHHH;//ÀúÊ·¼ÇÂ¼¼ä¸ô
    /* »ñÈ¡ÉèÖÃµÄÊ±¼äÖµ */
    normalsavetime =((Cal_Inf->Com_Data[5]<<8)+Cal_Inf->Com_Data[4])*60;//´òÓ¡¼ä¸ô
	//  uploadtime     =((Cal_Inf->Com_Data[1]<<8)+Cal_Inf->Com_Data[0])*60;//ÉÏ´«¼ä¸ô
    alarm_intervaltime =((Cal_Inf->Com_Data[7]<<8)+Cal_Inf->Com_Data[6])*60;//±¨¾¯¼ÇÂ¼¼ä¸ô
  	normalsavetHHH=((Cal_Inf->Com_Data[3]<<8)+Cal_Inf->Com_Data[2])*60;//ÀúÊ·¼ÇÂ¼¼ä¸ô
	
    	/* ÅĞ¶ÏÉèÖÃµÄÊ±¼äÖµÊÇ·ñÔÚ¿ÉÒÔÉèÖÃµÄ·¶Î§ÄÚ */
	if( ((normalsavetime == 0)||(normalsavetime > Com_Config_Time_Interval_Set_Max_Second_Num))||
		  ((normalsavetHHH == 0)||(normalsavetHHH > Com_Config_Time_Interval_Set_Max_Second_Num))||
	    ((alarm_intervaltime == 0)||(alarm_intervaltime > Com_Config_Time_Interval_Set_Max_Second_Num))
	)//ÉèÖÃÊ±¼ä¼ä¸ôÊ§°Ü
	
	{
		/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	write_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
  write_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	write_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Write;//Ğ´±¨ Ê±¼ä¼ä¸ô²Ù×÷Âë	
	write_Time_Interval_Set.Chanel_Num=0xFF;	
	write_Time_Interval_Set.Data_Len=0X00;
	write_Time_Interval_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*²Ù×÷Ê§°Ü±êÖ¾*/
	write_Time_Interval_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	write_Time_Interval_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_Time_Interval_Set);/* ÉèÖÃ Ğ£Ñé */	
	 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
  USART_printf(USART1,(u8*)&write_Time_Interval_Set,8);
	//	return;
	}
	else//ÉèÖÃÊ±¼ä¼ä¸ô³É¹¦
	{
/***********ÉèÖÃ´òÓ¡Ê±¼ä¼ä¸ô***************************************/		
		//Ê±¼äµÄÊäÈë·½Ê½Ó¦¸Ã¸ü¼ÑÈËĞÔ»¯µã±È½ÏºÃ¡£
	*Pit_Time_Irl_Data_P_1	= normalsavetime;	
	/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
	 Write_Config_Tab((u8*)Pit_Time_Irl_Data_P_1,sizeof(*Pit_Time_Irl_Data_P_1));
/******************************************************************/
///***********ÉèÖÃÉÏ´«Ê±¼ä¼ä¸ô***************************************/	
//			//Ê±¼äµÄÊäÈë·½Ê½Ó¦¸Ã¸ü¼ÑÈËĞÔ»¯µã±È½ÏºÃ¡£
//		*Menu_Nml_Save_Irl_Data_A	= uploadtime;
/***********ÉèÖÃÀúÊ·¼ÇÂ¼Ê±¼ä¼ä¸ô***************************************/	
			//Ê±¼äµÄÊäÈë·½Ê½Ó¦¸Ã¸ü¼ÑÈËĞÔ»¯µã±È½ÏºÃ¡£
		*Menu_Nml_Save_Irl_Data_A	= normalsavetHHH;		
		/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
		Write_Config_Tab((u8*)Menu_Nml_Save_Irl_Data_A,sizeof(*Menu_Nml_Save_Irl_Data_A));
/*******************************************************************/
/*************ÉèÖÃ±¨¾¯¼ÇÂ¼¼ä¸ô************************************/
		/* ÉèÖÃ±¨¾¯×´Ì¬µÄ±¨¾¯Ê±¼ä¼ä¸ô */
		*Menu_Aam_Save_Irl_Data_A	= alarm_intervaltime;
		
		/* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
		Write_Config_Tab((u8*)Menu_Aam_Save_Irl_Data_A,sizeof(*Menu_Aam_Save_Irl_Data_A));
			
/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	write_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
  write_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	write_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Write;//Ğ´±¨ Ê±¼ä¼ä¸ô²Ù×÷Âë	
	write_Time_Interval_Set.Chanel_Num=0xFF;	
	write_Time_Interval_Set.Data_Len=0x01;
	write_Time_Interval_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	write_Time_Interval_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	write_Time_Interval_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&write_Time_Interval_Set);/* ÉèÖÃ Ğ£Ñé */	
	 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
  USART_printf(USART1,(u8*)&write_Time_Interval_Set,8);	
	}

}



/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Time_Interval_Read											*/
/**	¹¦ÄÜËµÃ÷:¶ÁÈ¡Ê±¼ä¼ä¸ô£¨ÉÏ´«Ê±¼ä¼ä¸ô¡¢ÀúÊ·¼ÇÂ¼Ê±¼ä¼ä¸ô¡¢´òÓ¡Ê±¼ä¼ä¸ô¡¢ ±¨¾¯¼ÇÂ¼¼ä¸ô£©****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
//u32 mmn=0;
static void Com_Config_Time_Interval_Read	(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
     COM_SENSOR_CAL_TYPEDEF      read_Time_Interval_Set;//½ÓÊÕ´®¿ÚÊı¾İ
	
	       /* ÉèÖÃµÄÊ±¼ä²ÎÊı */
     u32 normalsavetime;//´òÓ¡¼ä¸ô
	 //  u32 uploadtime;//ÉÏ´«¼ä¸ô¼ä¸ô
	   u32 alarm_intervaltime;//±¨¾¯¼ÇÂ¼¼ä¸ô
	   u32 normalsavetHHH;//ÀúÊ·¼ÇÂ¼¼ä¸ô
	
	   normalsavetHHH=(*Menu_Nml_Save_Irl_Data_A)/60;//ÀúÊ·¼ÇÂ¼¼ä¸ô
	   alarm_intervaltime=(*Pit_Time_Irl_Data_P_1)/60;//±¨¾¯¼ÇÂ¼¼ä¸ô
	   normalsavetime=(*Menu_Aam_Save_Irl_Data_A)/60;//´òÓ¡¼ä¸ô
	
	
	   read_Time_Interval_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Time_Interval_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Time_Interval_Set.Cmd_Code =Com_Config_Time_Interval_Cmd_Code_Read;//¶Á Ê±¼ä¼ä¸ô ²Ù×÷Âë
	   read_Time_Interval_Set.Chanel_Num=0xFF;
	   read_Time_Interval_Set.Data_Len=0X08;
	   read_Time_Interval_Set.Com_Data[0]=0x78;
	   read_Time_Interval_Set.Com_Data[1]=0x00;
	   read_Time_Interval_Set.Com_Data[2]=(normalsavetHHH&0xFF);/**/
	   read_Time_Interval_Set.Com_Data[3]=(normalsavetHHH>>8);
	   read_Time_Interval_Set.Com_Data[4]=(alarm_intervaltime&0xFF);/**/ 
	   read_Time_Interval_Set.Com_Data[5]=(alarm_intervaltime>>8);
	   read_Time_Interval_Set.Com_Data[6]=(normalsavetime&0xFF);/**/;
	   read_Time_Interval_Set.Com_Data[7]=(normalsavetime>>8);;
	   read_Time_Interval_Set.Com_Data[8]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Time_Interval_Set.Com_Data[9]=Com_Sensor_Cal_Get_Check(&read_Time_Interval_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Time_Interval_Set,15);	
//     mmn=*Pit_Time_Irl_Data_P_1;//»ñÈ¡´òÓ¡Ê±¼ä¼ä¸ôÊı¾İ



}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Alarm_time_delay_Write										*/
/**	¹¦ÄÜËµÃ÷:Ğ´±¨¾¯ÑÓÊ±¼ä¸ô****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
/* Í¨µÀ±¨¾¯ÑÓÊ±Êı¾İÖ¸Õë. */
//u32 bbk=0;
#define Cha_Aam_Dly_Data_A(cha)		(&((INSTRU_SENSOR_CHANNEL_FIX*)(&Instru_Fix_Inf_State_Tab.Inf_State.Instru_Sensor.Inf_Space[cha*sizeof(INSTRU_SENSOR_CHANNEL_FIX)]))->Sensor_Alarm_Delay)
static void Com_Config_Alarm_time_delay_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{

     	/* ÉèÖÃµÄÊ±¼ä²ÎÊı */
	     u32 alarmsavetime;
	  	/* ¼ÆÊı±äÁ¿ */
	     u8 num;
	   COM_SENSOR_CAL_TYPEDEF    Alarm_time_delay_Set;//½ÓÊÕ´®¿ÚÊı¾İ
	
	   alarmsavetime =(Cal_Inf->Com_Data[1]<<8)+Cal_Inf->Com_Data[0];//½«´®¿Ú½ÓÊÕµÄÁ½¸öÊı¾İºÏ³ÉÒ»¸öÕûĞÎÊı¾İ¼´Îª±¨¾¯ÑÓÊ±¼ä¸ôÊ±¼ä
//	   bbk=alarmsavetime;
	   if((alarmsavetime==0)||(alarmsavetime>Com_Config_Alarm_time_delay_SetMaxSecond))//³¬¹ı×î´óµÄ±¨¾¯Ê±¼ä¼ä¸ô3600Ãë£¨1¸öĞ¡Ê±£
		 {
		 
			 	/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
	     /* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	    Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
      Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	    Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Write;//Ğ´±¨¾¯ÑÓÊ±¼ä¸ô²Ù×÷Âë	
	    Alarm_time_delay_Set.Chanel_Num=0xFF;	
	    Alarm_time_delay_Set.Data_Len=0X02;
	    Alarm_time_delay_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*²Ù×÷Ê§°Ü±êÖ¾*/
	    Alarm_time_delay_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	    Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Alarm_time_delay_Set);/* ÉèÖÃ Ğ£Ñé */	
	     /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
      USART_printf(USART1,(u8*)&Alarm_time_delay_Set,8);
			
			 
		 }
		 else//Ö´ĞĞÉèÖÃ±¨¾¯ÑÓÊ±¼ä¸ô²Ù×÷
		 {
		 
		       /* ÉèÖÃ ËùÓĞÍ¨µÀµÄ ±¨¾¯ÑÓÊ±²ÎÊı */
		    for(num = 0; num < Sensor_CH_Config_Num; num++)
		  {
			  /* ÉèÖÃÕı³£×´Ì¬µÄ±¨¾¯Ê±¼ä¼ä¸ô */
			   *Cha_Aam_Dly_Data_A(num) = alarmsavetime;
			  /* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
		   	Write_Config_Tab((u8*)Cha_Aam_Dly_Data_A(num),sizeof(*Cha_Aam_Dly_Data_A(num)));
				
			}
				
			/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
	/* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
  Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Write;//Ğ´±¨¾¯ÑÓÊ±¼ä¸ô²Ù×÷Âë	
	Alarm_time_delay_Set.Chanel_Num=0xFF;	
	Alarm_time_delay_Set.Data_Len=0x02;
	Alarm_time_delay_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	Alarm_time_delay_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Alarm_time_delay_Set);/* ÉèÖÃ Ğ£Ñé */			
	 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
  USART_printf(USART1,(u8*)&Alarm_time_delay_Set,8);						
	}
}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Alarm_time_delay_Read									*/
/**	¹¦ÄÜËµÃ÷:¶Á±¨¾¯ÑÓÊ±¼ä¸ô****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Alarm_time_delay_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	
   COM_SENSOR_CAL_TYPEDEF      read_Alarm_time_delay_Set;//½ÓÊÕ´®¿ÚÊı¾İ


	   read_Alarm_time_delay_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Alarm_time_delay_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Alarm_time_delay_Set.Cmd_Code =Com_Config_Alarm_time_delay_Cmd_Code_Read;//¶Á ÑÓÊ±±¨¾¯¼ä¸ô ²Ù×÷Âë
	   read_Alarm_time_delay_Set.Chanel_Num=0xFF;
	   read_Alarm_time_delay_Set.Data_Len=0X02;
	   read_Alarm_time_delay_Set.Com_Data[1]=((*Cha_Aam_Dly_Data_A(Sensors_CH_1))>>8);         
	   read_Alarm_time_delay_Set.Com_Data[0]=((*Cha_Aam_Dly_Data_A(Sensors_CH_1))&0xFF); 
	   read_Alarm_time_delay_Set.Com_Data[2]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Alarm_time_delay_Set.Com_Data[3]=Com_Sensor_Cal_Get_Check(&read_Alarm_time_delay_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Alarm_time_delay_Set,9);	
}

/* ±¨¾¯×Ü¿ª¹Ø "Éù" ×´Ì¬Ö¸Õë */
#define Sud_Aam_Mtr_Sth_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Sound_Alarm_Master_Switch)
/* ±¨¾¯×Ü¿ª¹Ø "¹â" ×´Ì¬Ö¸Õë */
#define Lgt_Aam_Mtr_Sth_A	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.Light_Alarm_Master_Switch)
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Sound_lightSwitch_Read									*/
/**	¹¦ÄÜËµÃ÷:¶ÁÈ¡Éù¹â±¨¾¯×Ü¿ª¹Ø×´Ì¬****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Sound_lightSwitch_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Sound_lightSwitch_Set;//½ÓÊÕ´®¿ÚÊı¾İ
	    
	
	   read_Sound_lightSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Sound_lightSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Sound_lightSwitch_Set.Cmd_Code =Com_ConfigTotal_audible_light_alarm_Cmd_Code_Read;//¶Á Éù¹â±¨¾¯×Ü¿ª¹Ø ²Ù×÷Âë
	   read_Sound_lightSwitch_Set.Chanel_Num=0xFF;
	   read_Sound_lightSwitch_Set.Data_Len=0X01;      
	   read_Sound_lightSwitch_Set.Com_Data[0]=((*Sud_Aam_Mtr_Sth_A)&&(*Lgt_Aam_Mtr_Sth_A)); 
	   read_Sound_lightSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Sound_lightSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Sound_lightSwitch_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Sound_lightSwitch_Set,8);	
	
     




}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Sound_lightSwitch_Write									*/
/**	¹¦ÄÜËµÃ÷:Ğ´Éù¹â±¨¾¯×Ü¿ª¹Ø×´Ì¬****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Sound_lightSwitch_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	      COM_SENSOR_CAL_TYPEDEF      read_Sound_lightSwitch_Set;//½ÓÊÕ´®¿ÚÊı¾İ
	
          if((Cal_Inf->Com_Data[0]==Switch_ON)||(Cal_Inf->Com_Data[0]==Switch_OFF))
					{
					   	/* Éù¹â±¨¾¯×Ü¿ª¹Ø×´Ì¬Îª:¹Ø,¿ªÆô Í¨µÀ Éù¹â±¨¾¯. */
				        *Sud_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				        *Lgt_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				      /* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
				    Write_Config_Tab((u8*)Sud_Aam_Mtr_Sth_A,sizeof(*Sud_Aam_Mtr_Sth_A));
				    Write_Config_Tab((u8*)Lgt_Aam_Mtr_Sth_A,sizeof(*Lgt_Aam_Mtr_Sth_A));
					
	/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
	     /* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	    read_Sound_lightSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
      read_Sound_lightSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	    read_Sound_lightSwitch_Set.Cmd_Code =Com_ConfigTotal_audible_light_alarm_Cmd_Code_Write;//Ğ´Éù¹â±¨¾¯×Ü¿ª¹Ø²Ù×÷Âë	
	    read_Sound_lightSwitch_Set.Chanel_Num=0xFF;	
	    read_Sound_lightSwitch_Set.Data_Len=0x01;
	    read_Sound_lightSwitch_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	    read_Sound_lightSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	    read_Sound_lightSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Sound_lightSwitch_Set);/* ÉèÖÃ Ğ£Ñé */			
	    /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Sound_lightSwitch_Set,8);
								
					}





}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_SMSalarmSwitch_Read									*/
/**	¹¦ÄÜËµÃ÷:¶ÁÈ¡¶ÌĞÅ±¨¾¯×Ü¿ª¹Ø×´Ì¬****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
/* "¶ÌĞÅ±¨¾¯"×Ü¿ª¹Ø×´Ì¬Ö¸Õë */
#define SMS_Aam_Mtr_Sth_A 	(&Instru_Config_Inf_Tab.Inf_State.Instru_Menu.Inf_State.SMS_Alarm_Master_Switch)
static void Com_Config_SMSalarmSwitch_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_SMSalarmSwitch_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_SMSalarmSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_SMSalarmSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_SMSalarmSwitch_Set.Cmd_Code =Com_Config_SMS_alarm_total_switch_Cmd_Code_Read;//¶Á ¶ÌĞÅ±¨¾¯×Ü¿ª¹Ø ²Ù×÷Âë
	   read_SMSalarmSwitch_Set.Chanel_Num=0xFF;
	   read_SMSalarmSwitch_Set.Data_Len=0X01;      
	   read_SMSalarmSwitch_Set.Com_Data[0]=(*SMS_Aam_Mtr_Sth_A); 
	   read_SMSalarmSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_SMSalarmSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_SMSalarmSwitch_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_SMSalarmSwitch_Set,8);	
}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_SMSalarmSwitch_Write									*/
/**	¹¦ÄÜËµÃ÷:Ğ´¶ÌĞÅ±¨¾¯×Ü¿ª¹Ø×´Ì¬****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_SMSalarmSwitch_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
     COM_SENSOR_CAL_TYPEDEF      Write_SMSalarmSwitch_Set;//½ÓÊÕ´®¿ÚÊı¾İ

     if((Cal_Inf->Com_Data[0]==Switch_ON)||(Cal_Inf->Com_Data[0]==Switch_OFF))
					{
					   	/* ¶ÌĞÅ±¨¾¯×Ü¿ª¹Ø×´Ì¬Îª:¹Ø,¿ªÆô*/
				        *SMS_Aam_Mtr_Sth_A =(BOOL)(Cal_Inf->Com_Data[0]);
				    /* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
				     Write_Config_Tab((u8*)SMS_Aam_Mtr_Sth_A,sizeof(*SMS_Aam_Mtr_Sth_A));
	/***********************ÏìÓ¦ÉÏÎ»»úÖ¸Áî****************************************/
	     /* ÅäÖÃ ÊµÊ±Êı¾İ ²ÎÊı */
	    Write_SMSalarmSwitch_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
      Write_SMSalarmSwitch_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	    Write_SMSalarmSwitch_Set.Cmd_Code =Com_Config_SMS_alarm_total_switch_Cmd_Code_Write;//Ğ´¶ÌĞÅ±¨¾¯×Ü¿ª¹Ø²Ù×÷Âë	
	    Write_SMSalarmSwitch_Set.Chanel_Num=0xFF;	
	    Write_SMSalarmSwitch_Set.Data_Len=0x01;
	    Write_SMSalarmSwitch_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	    Write_SMSalarmSwitch_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	    Write_SMSalarmSwitch_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Write_SMSalarmSwitch_Set);/* ÉèÖÃ Ğ£Ñé */			
	    /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&Write_SMSalarmSwitch_Set,8);
								
					}

}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_SystemTime_Write									*/
/**	¹¦ÄÜËµÃ÷:Í¨¹ıÉÏÎ»»úÉèÖÃÏµÍ³Ê±¼ä****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
/* ÏµÍ³Ê±ÖÓÊı¾İ¼Ä´æÆ÷ */
static Times System_Cok_DR;
extern BOOL RTC_Init(BOOL reset_state);
extern void Write_System_Set(Times* time);
static void Com_Config_SystemTime_Write(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
	  COM_SENSOR_CAL_TYPEDEF      read_GSM_SystemTime_Set;//½ÓÊÕ´®¿ÚÊı¾İ 
   // COM_SENSOR_CAL_TYPEDEF      Write_SystemTime_Set;//½ÓÊÕ´®¿ÚÊı¾İ

          if((Cal_Inf->Chanel_Num==0xBB)&&
						 ((Cal_Inf->Com_Data[0]>=0x17)&&(Cal_Inf->Com_Data[0]<=0x34))&&
					   ((Cal_Inf->Com_Data[1]>=0x01)&&(Cal_Inf->Com_Data[1]<=0x12))&&
					   ((Cal_Inf->Com_Data[2]>=0x01)&&(Cal_Inf->Com_Data[2]<=0x31))&&
					   ((Cal_Inf->Com_Data[3]>0x00)&&(Cal_Inf->Com_Data[3]<=0x23))&&
					   ((Cal_Inf->Com_Data[4]>0x00)&&(Cal_Inf->Com_Data[4]<=0x59))&&
					   ((Cal_Inf->Com_Data[5]>0x00)&&(Cal_Inf->Com_Data[2]<=0x59))
					
					)
					{    
           
					     System_Cok_DR.Year= System_Year_Start+(Cal_Inf->Com_Data[0]);//ÉèÖÃÄê
					     System_Cok_DR.Month=(Cal_Inf->Com_Data[1]);//ÉèÖÃÔÂ
						   System_Cok_DR.Day=(Cal_Inf->Com_Data[2]);//ÉèÖÃÈÕ
						   System_Cok_DR.Hour=(Cal_Inf->Com_Data[3]);//ÉèÖÃÊ±
						   System_Cok_DR.Min=(Cal_Inf->Com_Data[4]);//ÉèÖÃ·Ö
						   System_Cok_DR.Sec=(Cal_Inf->Com_Data[5]);//ÉèÖÃÃë
						
						
						   /* ¸´Î» RTC8025 */
		           RTC_Init(TRUE);	
						   /* ÉèÖÃ Ê±¼ä */ 
	        	 Write_System_Set(&System_Cok_DR);

					  Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year 	=System_Cok_DR.Year;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month 	= System_Cok_DR.Month;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day 	= System_Cok_DR.Day;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour 	= System_Cok_DR.Hour;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min 	= System_Cok_DR.Min;
		        Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec 	= System_Cok_DR.Sec;
		
	       	//ÎªÁË½â¾öRTCÊ±ÖÓĞ¾Æ¬ÒâÍâ±»Ğ´¹ÊÕÏ£¬ÕâÀïÏÈ±£´æÒ»´Îµ±Ç°µÄÊµÊ±Ê±ÖÓÊı¾İÎª\
		     µ±ÉÏµçÆô¶¯¼ì²âµ½RTC±»Òì³£ºó¿ÉÒÔÊ¹ÓÃÕâ¸öÊ±¼äÀ´×ö×÷ÊµÊ±Ê±¼ä¡£
		     /* ĞŞ¸Ä ÒÇÆ÷ ĞÅÏ¢±í ÉèÖÃ±êÖ¾ */
		       Write_Config_Tab((u8*)&Instru_Config_Inf_Tab.Inf_State.Instru_RTC,
					    sizeof(Instru_Config_Inf_Tab.Inf_State.Instru_RTC));




					}
			else if(Cal_Inf->Chanel_Num==0xCC)
			   {
				 
		 read_GSM_SystemTime_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_GSM_SystemTime_Set.Instru_NO=0x01;//ÒÇÆ÷±àºÅ
	   read_GSM_SystemTime_Set.Cmd_Code =Com_Config_SystemTime_Cmd_Code_Read_Write;//¶ÁÏµÍ³Ê±¼ä²Ù×÷Âë
	   read_GSM_SystemTime_Set.Chanel_Num=0xFF;
	   read_GSM_SystemTime_Set.Data_Len=0X06;      
	   read_GSM_SystemTime_Set.Com_Data[0]=HEX_To_BCD((Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year-0X30));//Äê
		 read_GSM_SystemTime_Set.Com_Data[1]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month);//ÔÂ
	   read_GSM_SystemTime_Set.Com_Data[2]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day);//ÈÕ
		 read_GSM_SystemTime_Set.Com_Data[3]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour);//Ê±
	   read_GSM_SystemTime_Set.Com_Data[4]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min);//·Ö
		 read_GSM_SystemTime_Set.Com_Data[5]=HEX_To_BCD(Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec);//Ãë
	   read_GSM_SystemTime_Set.Com_Data[6]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_GSM_SystemTime_Set.Com_Data[7]=Com_Sensor_Cal_Get_Check(&read_GSM_SystemTime_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_GSM_SystemTime_Set,13);	
					 
					 
				 }				
					
					

}



/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_GSM_Signal_Strength_Read									*/
/**	¹¦ÄÜËµÃ÷:¶ÁÈ¡¶ÌĞÅGSMÄ£¿éĞÅºÅÇ¿¶È****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_GSM_Signal_Strength_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_GSM_Signal_Strength_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_GSM_Signal_Strength_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_GSM_Signal_Strength_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_GSM_Signal_Strength_Set.Cmd_Code =Com_Config_GSM_Signal_Strength_Cmd_Code_Read;//¶Á GSMÄ£¿éĞÅºÅÇ¿¶È²Ù×÷Âë
	   read_GSM_Signal_Strength_Set.Chanel_Num=0xFF;
	   read_GSM_Signal_Strength_Set.Data_Len=0X01;      
	   read_GSM_Signal_Strength_Set.Com_Data[0]=(u8)GPRS_SR_Reg.GPRS_Sign_Stre;//GSMÄ£¿éĞÅºÅÇ¿¶ÈÖµ
	   read_GSM_Signal_Strength_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_GSM_Signal_Strength_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_GSM_Signal_Strength_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_GSM_Signal_Strength_Set,8);	
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Cancel_Sound_Light_Alarm		*/
/**	¹¦ÄÜËµÃ÷:ÏòÉÏÎ»»ú·´À¡È¡ÏûÉù¹â±¨¾¯³É¹¦****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Cancel_Sound_Light_Alarm(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Cancel_Sound_Light_Alarm_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_Cancel_Sound_Light_Alarm_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Cancel_Sound_Light_Alarm_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Cancel_Sound_Light_Alarm_Set.Cmd_Code =Cancel_Sound_Light_Alarm_Cmd_Code;//È¡ÏûÉù¹â±¨¾¯
	   read_Cancel_Sound_Light_Alarm_Set.Chanel_Num=0xFF;
	   read_Cancel_Sound_Light_Alarm_Set.Data_Len=0X01;      
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Cancel_Sound_Light_Alarm_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Cancel_Sound_Light_Alarm_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Cancel_Sound_Light_Alarm_Set,8);	
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Cancel_Sound_Light_Alarm		*/
/**	¹¦ÄÜËµÃ÷:ÏòÉÏÎ»»ú·´À¡È¡ÏûÉù¹â±¨¾¯³É¹¦****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Clear_Historical_Data(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Clear_Historical_Data_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_Clear_Historical_Data_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Clear_Historical_Data_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Clear_Historical_Data_Set.Cmd_Code =Clear_Historical_Data_Cmd_Code;//Çå³ıÀúÊ·Êı¾İ²Ù×÷Âë
	   read_Clear_Historical_Data_Set.Chanel_Num=0xFF;
	   read_Clear_Historical_Data_Set.Data_Len=0X01;      
	   read_Clear_Historical_Data_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	   read_Clear_Historical_Data_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Clear_Historical_Data_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Clear_Historical_Data_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Clear_Historical_Data_Set,8);	
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Open_GSM_Debug_Information	*/
/**	¹¦ÄÜËµÃ÷:ÏòÉÏÎ»»ú·´À¡´ò¿ªGSMÄ£¿é´®¿Úµ÷ÊÔĞÅÏ¢³É¹¦****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Open_GSM_Debug_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Open_GSM_Debug_Information_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_Com_Config_Open_GSM_Debug_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Com_Config_Open_GSM_Debug_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Com_Config_Open_GSM_Debug_Information_Set.Cmd_Code =Open_GSM_Debug_Information_Cmd_Code;//´ò¿ªGSMÄ£¿éµ÷ÊÔĞÅÏ¢
	   read_Com_Config_Open_GSM_Debug_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_Open_GSM_Debug_Information_Set.Data_Len=0X01;      
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Com_Config_Open_GSM_Debug_Information_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Open_GSM_Debug_Information_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Com_Config_Open_GSM_Debug_Information_Set,8);	
}


/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Close_GSM_Debug_Information	*/
/**	¹¦ÄÜËµÃ÷:ÏòÉÏÎ»»ú·´À¡¹Ø±ÕGSMÄ£¿é´®¿Úµ÷ÊÔĞÅÏ¢³É¹¦****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Close_GSM_Debug_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Close_GSM_Debug_Information_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_Com_Config_Close_GSM_Debug_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Com_Config_Close_GSM_Debug_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Com_Config_Close_GSM_Debug_Information_Set.Cmd_Code =Open_GSM_Debug_Information_Cmd_Code;//´ò¿ªGSMÄ£¿éµ÷ÊÔĞÅÏ¢
	   read_Com_Config_Close_GSM_Debug_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_Close_GSM_Debug_Information_Set.Data_Len=0X01;      
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Com_Config_Close_GSM_Debug_Information_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Close_GSM_Debug_Information_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Com_Config_Close_GSM_Debug_Information_Set,8);	
}


/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_PowerLink_And_Battery_Voltage_Information	*/
/**	¹¦ÄÜËµÃ÷:ÏòÉÏÎ»»ú·´À¡¹Ø±ÕGSMÄ£¿é´®¿Úµ÷ÊÔĞÅÏ¢³É¹¦****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static INSTRU_POWER* Battery=&Instru_Config_Inf_Tab.Inf_State.Instru_Power;//µçÔ´Ïà¹ØĞÅÏ¢½á¹¹ÌåÖ¸Õë
static void Com_Config_PowerLink_And_Battery_Voltage_Information(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Cmd_Code =Check_PowerLink_And_Battery_Voltage_Cmd_Code;//·µ»Ø32Î»Ö÷°åÍâ½Óµç¼°µç³ØµçÑ¹×´Ì¬²Ù×÷Âë
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Chanel_Num=0xFF;
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Data_Len=0X02;      
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[0]=Battery->Inf_State.ET_Link_State;//Ö÷°åÍâ½Óµç×´Ì¬
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[1]=Battery->Inf_State.BT_Quantity_State;//32Î»Ö÷°åµç³ØµçÁ¿                                                                                    
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[2]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set.Com_Data[3]=Com_Sensor_Cal_Get_Check(&read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Com_Config_PowerLink_And_Battery_Voltage_Information_Set,9);	
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Open_Data_Save_Record	*/
/**	¹¦ÄÜËµÃ÷:ÏòÉÏÎ»»ú·´À¡´ò¿ªÊı¾İ¼ÇÂ¼³É¹¦****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Open_Data_Save_Record(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Open_Data_Save_Record_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_Com_Config_Open_Data_Save_Record_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Com_Config_Open_Data_Save_Record_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Com_Config_Open_Data_Save_Record_Set.Cmd_Code =Open_DataSave_Cmd_Code;//´ò¿ªÊı¾İ¼ÇÂ¼¹¦ÄÜ
	   read_Com_Config_Open_Data_Save_Record_Set.Chanel_Num=0xFF;
	   read_Com_Config_Open_Data_Save_Record_Set.Data_Len=0X01;      
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Com_Config_Open_Data_Save_Record_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Open_Data_Save_Record_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Com_Config_Open_Data_Save_Record_Set,8);	
}
/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Open_Data_Save_Record	*/
/**	¹¦ÄÜËµÃ÷:ÏòÉÏÎ»»ú·´À¡¹Ø±ÕÊı¾İ¼ÇÂ¼³É¹¦****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Close_Data_Save_Record(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Close_Data_Save_Record_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_Com_Config_Close_Data_Save_Record_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Com_Config_Close_Data_Save_Record_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Com_Config_Close_Data_Save_Record_Set.Cmd_Code =Open_DataSave_Cmd_Code;//´ò¿ªÊı¾İ¼ÇÂ¼¹¦ÄÜ
	   read_Com_Config_Close_Data_Save_Record_Set.Chanel_Num=0xFF;
	   read_Com_Config_Close_Data_Save_Record_Set.Data_Len=0X01;      
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*²Ù×÷³É¹¦±êÖ¾*/	
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Com_Config_Close_Data_Save_Record_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Close_Data_Save_Record_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Com_Config_Close_Data_Save_Record_Set,8);	
}


/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Open_Data_Save_Record	*/
/**	¹¦ÄÜËµÃ÷:ÏòÉÏÎ»»ú·´À¡¹Ø±ÕÊı¾İ¼ÇÂ¼³É¹¦****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_DataSave_Status_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_DataSave_Status_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

     read_Com_Config_DataSave_Status_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   read_Com_Config_DataSave_Status_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   read_Com_Config_DataSave_Status_Set.Cmd_Code =Check_DataSave_Status_Cmd_Code;//²éÑ¯Êı¾İ¼ÇÂ¼×´Ì¬²Ù×÷Âë
	   read_Com_Config_DataSave_Status_Set.Chanel_Num=0xFF;
	   read_Com_Config_DataSave_Status_Set.Data_Len=0X01;      
	   read_Com_Config_DataSave_Status_Set.Com_Data[0]=*Save_Data_Switch_A;/*Êı¾İ¼ÇÂ¼×´Ì¬£»0£ºÎ´Æô¶¯£»1£ºÆô¶¯ÁËÊı¾İ¼ÇÂ¼*/	
	   read_Com_Config_DataSave_Status_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   read_Com_Config_DataSave_Status_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&read_Com_Config_DataSave_Status_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&read_Com_Config_DataSave_Status_Set,8);	
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Software_Version_NO_Major_Read	*/
/**	¹¦ÄÜËµÃ÷:¶ÁÈ¡32Î»Æ½Ì¨µÄÈí¼ş°æ±¾ºÍÓ²¼ş°æ±¾****************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Software_Version_NO_Major_Read(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF      read_Com_Config_Software_Version_NO_Major_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

	
	   if(Cal_Inf->Chanel_Num==0xAA)
		 {
				 read_Com_Config_Software_Version_NO_Major_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
				 read_Com_Config_Software_Version_NO_Major_Set.Instru_NO=0x01;//ÒÇÆ÷±àºÅ
				 read_Com_Config_Software_Version_NO_Major_Set.Cmd_Code =Com_Config_Software_Version_NO_Major_Cmd_Code_Read;//²éÑ¯32Î»Æ½Ì¨Èí¼ş°æ±¾ºÍÓ²¼ş°æ±¾
				 read_Com_Config_Software_Version_NO_Major_Set.Chanel_Num=0xAA;
				 read_Com_Config_Software_Version_NO_Major_Set.Data_Len=0X08;      
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[4]=(Software_Version_NO_Major+0x30);/*Èí¼ş Ö÷°æ±¾±àºÅ */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[5]=0X2E;/* .µÄASCIIÂë*/
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[6]=(Software_Version_NO_Child+0x30);/*Èí¼ş ×Ó°æ±¾±àºÅ */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[7]=(Software_Version_NO_Stage+0x30);/*Èí¼ş ½×¶Î°æ±¾±àºÅ */
			
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[0]=(Hardware_Version_NO_Major+0x30);/*Ó²¼ş Ö÷°æ±¾±àºÅ */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[1]=0X2E;/* .µÄASCIIÂë*/
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[2]=(Hardware_Version_NO_Child+0x30);/*Ó²¼ş ×Ó°æ±¾±àºÅ */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[3]=(Hardware_Version_NO_Stage+0x30);/*Ó²¼ş ½×¶Î°æ±¾±àºÅ */
			
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[8]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
				 read_Com_Config_Software_Version_NO_Major_Set.Com_Data[9]=Com_Sensor_Cal_Get_Check(&read_Com_Config_Software_Version_NO_Major_Set);/* ÉèÖÃ Ğ£Ñé */	
				 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
				 USART_printf(USART1,(u8*)&read_Com_Config_Software_Version_NO_Major_Set,15);	
		 }
}

/************************************************************************************************/
/**	º¯ Êı Ãû: Com_Config_Factory_Setting	*/
/**	¹¦ÄÜËµÃ÷:»Ö¸´³ö³§ÉèÖÃ***************/
/**	ĞÎ    ²Î£º·şÎñÆ÷µÄÅäÖÃ²ÎÊıÏûÏ¢Ö¸Õë£ºCal_Inf													*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
static void Com_Config_Factory_Setting(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)//ZWC
{
       COM_SENSOR_CAL_TYPEDEF         Factory_Setting_Set;//½ÓÊÕ´®¿ÚÊı¾İ 

	    if(Cal_Inf->Com_Data[0]==0x01)
			{	
		 Factory_Setting_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   Factory_Setting_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   Factory_Setting_Set.Cmd_Code =Com_Config_System_Factory_Setting;//»Ö¸´³ö³§ÉèÖÃ²Ù×÷Âë
	   Factory_Setting_Set.Chanel_Num=0xFF;
	   Factory_Setting_Set.Data_Len=0X01;      
	   Factory_Setting_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Succeed;/*Ö´ĞĞ»Ö¸´³ö³§ÉèÖÃÈÎÎñ³É¹¦*/	
	   Factory_Setting_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   Factory_Setting_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Factory_Setting_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&Factory_Setting_Set,8);	
			/* »Ö¸´³ö³§ÉèÖÃ */
		 Instru_Rte_Fty_Set();
			}
			else
			{
		 Factory_Setting_Set.Head_Code=Com_Sensor_Cal_Rece_Head_Code;//Í·Âë
	   Factory_Setting_Set.Instru_NO=Cal_Inf->Instru_NO;//ÒÇÆ÷±àºÅ
	   Factory_Setting_Set.Cmd_Code =Com_Config_System_Factory_Setting;//»Ö¸´³ö³§ÉèÖÃ²Ù×÷Âë
	   Factory_Setting_Set.Chanel_Num=0xFF;
	   Factory_Setting_Set.Data_Len=0X01;      
	   Factory_Setting_Set.Com_Data[0]=Com_Sensor_Cal_Set_Para_Fail;/*Ö´ĞĞ»Ö¸´³ö³§ÉèÖÃÈÎÎñÊ§°Ü*/			
	   Factory_Setting_Set.Com_Data[1]=Com_Sensor_Cal_Constant;/* ÉèÖÃ ³£Êı */
	   Factory_Setting_Set.Com_Data[2]=Com_Sensor_Cal_Get_Check(&Factory_Setting_Set);/* ÉèÖÃ Ğ£Ñé */	
		 /* ÏìÓ¦ ÊµÊ±Êı¾İ Êı¾İĞÅÏ¢ */
     USART_printf(USART1,(u8*)&Factory_Setting_Set,8);
			}
}

/************************************************************************************************/
/**	º¯ Êı Ãû: COM_Sensor_Cal_Deal																*/
/**	¹¦ÄÜËµÃ÷: ´¦Àí ´«¸ĞÆ÷Ğ£×¼Êı¾İ																	*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void COM_Sensor_Cal_Deal(COM_SENSOR_CAL_TYPEDEF* Cal_Inf)
{

	
	/* ¸ù¾İ ²Ù×÷Âë ×ö¶ÔÓ¦µÄÊÂÇé */
	switch(Cal_Inf->Cmd_Code)
	{
#if (GSM_Module_Type==2) //ESP8266WifiÄ£¿é		
	/*************WifiÄ£¿éÏà¹ØÉèÖÃ**********************/
		case Com_Read_Write_Wifi_Name_Cmd_Code:
		{
		  /* ¶ÁºÍĞ´WifiÃû³Æ */
		 Com_Config_Read_Write_Wifi_Name(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_WSKEY_Cmd_Code:
		{
	   /*¶ÁºÍĞ´WifiµÄWSKEYÏà¹Ø²ÎÊı(¼ÓÃÜ·½Ê½ºÍÃÜÂë)*/
     Com_Config_Read_Write_Wifi_WSKEY(Cal_Inf);
			break;
		}
	 case Com_Read_Write_Wifi_WALN_Cmd_Code:
		{
	  /*¶ÁºÍĞ´WifiµÄWANN²Ù×÷ÂëÏà¹Ø²ÎÊı(±¾µØIPµØÖ·¡¢×ÓÍøÑÚÂë¡¢Íø¹ØIPµØÖ· )*/
     Com_Config_Read_Write_Wifi_Wlan(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_NETP_Cmd_Code:
		{
   /* ¶ÁºÍĞ´WifiµÄNETP²Ù×÷ÂëÏà¹Ø²ÎÊı(ÒªÁ¬½ÓµÄÊı¾İ·şÎñÆ÷µÄIPµØÖ·¼°¶Ë¿ÚºÅ)*/
     Com_Config_Read_Write_Wifi_NetP(Cal_Inf);
			break;
		}	
		
		case Com_Read_Write_Wifi_DHCP_Cmd_Code:
		{
   /* ¶ÁºÍĞ´Wifi»ñÈ¡IPµØÖ·µÄ·½Ê½*/
     Com_Config_Read_Write_Wifi_DHCP(Cal_Inf);
			break;
		}
		case Com_Read_Write_Wifi_MAC_Cmd_Code:
		{
      /* ¶ÁºÍĞ´WifiÄ£¿éµÄMACµØÖ·*/
     Com_Config_Read_Write_Wifi_MAC(Cal_Inf);
			break;
		}	
		case Com_Set_Rest_Wifi_Cmd_Code:
 		{
		  /*¸´Î»»òÕß»Ö¸´³ö³§ÉèÖÃ*/
		 Com_Config_Rest_Wifi(Cal_Inf);
 		 break;
 		}			
#endif
/**************************************************/		
		case Com_Sensor_Cal_Cmd_Code_Read_Real_Data:
		{
			/* ¶ÁÊµÊ±Êı¾İ */
			Com_Sensor_Cal_Read_Real_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Read_Orig_Data:
		{
			/* ¶ÁÔ­Ê¼Öµ */
			Com_Sensor_Cal_Read_Orig_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Write_Curve_Data:
		{
			/* Ğ´ÇúÏß */
			Com_Sensor_Cal_Write_Curve_Data(Cal_Inf);
			break;
		}
		case Com_Sensor_Cal_Cmd_Code_Read_Curve_Data:
		{
			/* ¶ÁÇúÏß */
			Com_Sensor_Cal_Read_Curve_Data(Cal_Inf);
			break;
		}
		case Com_Config_SN_Cmd_Code_Write:
		{
			
			/* Ğ´ÒÇÆ÷ºÅ£¨SNºÅ£© */
			Com_Config_SN_Write(Cal_Inf);
			/* ÅäÖÃÒÇÆ÷SNºÅ */
			Serial_Number_Init(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number);
			/* ¸´Î»ÎÂÊª¶ÈÊı¾İ¼Ä´æÆ÷ */
			GPRS_Reset_Sensor_Data_Reg();
			break;
		}
		case Com_Config_SN_Cmd_Code_Read:
		{
			/* ¶ÁÒÇÆ÷ºÅ£¨SNºÅ£© */
			Com_Config_SN_Read(Cal_Inf);
			break;
		}
		case Com_Config_Sensor_Cmd_Code_Write:
		{
			/* Ğ´´«¸ĞÆ÷ÅäÖÃ */
			Com_Config_Sensor_Write(Cal_Inf);
			
#if (Wireless_RF_Card_Config_Enable == 0x01)
			/* ¸´Î»ÎÂÊª¶ÈÊı¾İ¼Ä´æÆ÷ */
			GPRS_Reset_Sensor_Data_Reg();
#endif
			break;
		}
		case Com_Config_Sensor_Cmd_Code_Read:
		{
			/* ¶Á´«¸ĞÆ÷ÅäÖÃ */
			Com_Config_Sensor_Read(Cal_Inf);
			break;
		}
		
 		/* ¶Á´òÓ¡³µÅÆºÅ */
 		case Com_Config_Card_NO_Cmd_Code_Read:
 		{
 			Com_Config_Card_NO_Read(Cal_Inf);
 			break;
 		}
		
 		/* Ğ´´òÓ¡³µÅÆºÅ */
 		case Com_Config_Card_NO_Cmd_Code_Write:
 		{
 			Com_Config_Card_NO_Write(Cal_Inf);
 			break;
 		}
		
		 /* ¶Á ´òÓ¡¹«Ë¾Ãû³Æ */
 		case Com_Config_Company_Name_Cmd_Code_Read:
 		{
 			Com_Config_Company_Name_Read(Cal_Inf);
 			break;
 		}
		
 		/* Ğ´ ´òÓ¡¹«Ë¾Ãû³Æ */
 		case Com_Config_Company_Name_Cmd_Code_Write:
 		{
 			Com_Config_Company_Name_Write(Cal_Inf);
 			break;
 		}
		
		/* Ğ´ Êı¾İ×ö¼Ù¹¦ÄÜ */
 		case Com_Config_Fad_Data_Cmd_Code_Write:
 		{
 			Com_Config_Fad_Data_Write(Cal_Inf);
 			break;
 		}
		
		/* ¶Á Êı¾İ×ö¼Ù¹¦ÄÜ */
 		case Com_Config_Fad_Data_Cmd_Code_Read:
 		{
 			Com_Config_Fad_Data_Read(Cal_Inf);
 			break;
 		}
		
		/* Ğ´Êı¾İ·şÎñÆ÷ÓòÃû¡¢IP¡¢¶Ë¿Ú */
 		case ComConfigDataNetworkWrite:
 		{	
 			Com_Config_DataNetwork_Write(Cal_Inf);
			
////			/* ÖØĞÂÅäÖÃGSMÄ£¿é */
////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//ÉèÖÃ·şÎñÆ÷²ÎÊı
 			break;
 		}	
		/* ¶ÁÊı¾İ·şÎñÆ÷ÓòÃû¡¢IP¡¢¶Ë¿Ú */
 		case ComConfigDataNetworkRead:
 		{
 			Com_Config_DataNetwork_Read(Cal_Inf);
 			break;
 		}
			/* Ğ´ÅäÖÃ·şÎñÆ÷ÓòÃû¡¢IP¡¢¶Ë¿Ú */
 		case ComConfigNetworkWrite:
 		{	
 			Com_Config_Network_Write(Cal_Inf);
			
//////			/* ÖØĞÂÅäÖÃGSMÄ£¿é */
//////			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//ÉèÖÃ·şÎñÆ÷²ÎÊı
 			break;
 		}	
		/* ¶ÁÅäÖÃ·şÎñÆ÷ÓòÃû¡¢IP¡¢¶Ë¿Ú */
 		case ComConfigNetworkRead:
 		{
 			 Com_Config_Network_Read(Cal_Inf);
 			break;
 		}
					/* Ğ´Éı¼¶·şÎñÆ÷ÓòÃû¡¢IP¡¢¶Ë¿Ú */
 		case ComConfigUpgradeNetworkWrite:
 		{	
 			Com_Config_Upgrade_Network_Write(Cal_Inf);
			
//			/* ÖØĞÂÅäÖÃGSMÄ£¿é */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
			GSMConfig_Server_Inf(&Instru_Config_Inf_Tab.Inf_State.Instru_GSM);//ÉèÖÃ·şÎñÆ÷²ÎÊı
 			break;
 		}	
		/* ¶ÁÉı¼¶·şÎñÆ÷ÓòÃû¡¢IP¡¢¶Ë¿Ú */
 		case ComConfigUpgradeNetworkRead:
 		{
 			Com_Config_Upgrade_Network_Read(Cal_Inf);
 			break;
 		}
			/* ¶Á¶ÌÏ¢±¨¾¯ºÅÂë*/
 		case Com_Config_SMSAamPoeNO_Cmd_Code_Read:
 		{
 			Com_Config_SMSAamPoeNORead(Cal_Inf);
 			break;
 		}
			/* Ğ´¶ÌÏ¢±¨¾¯ºÅÂë*/
 		case Com_Config_SMSAamPoeNO_Cmd_Code_Write:
 		{
 			Com_Config_SMSAamPoeNOSet(Cal_Inf);
 			break;
 		}
		 /*¶ÁÊ±¼ä¼ä¸ô*/
		case Com_Config_Time_Interval_Cmd_Code_Read:
 		{
 	  	Com_Config_Time_Interval_Read	(Cal_Inf);
 			break;
 		}
/*ÉèÖÃÊ±¼ä¼ä¸ô£¨ÉÏ´«Ê±¼ä¼ä¸ô¡¢ÀúÊ·¼ÇÂ¼Ê±¼ä¼ä¸ô¡¢´òÓ¡Ê±¼ä¼ä¸ô¡¢ ±¨¾¯¼ÇÂ¼¼ä¸ô£©*/
		/**************Ğ´Ê±¼ä¼ä¸ô***************************/
		case Com_Config_Time_Interval_Cmd_Code_Write:
 		{
 			 Com_Config_Time_Interval_Set(Cal_Inf);
 			break;
 		}
/***********************ÉèÖÃ±¨¾¯ÑÓÊ± (ÒÔÃëÎªµ¥Î») *********************************/	

		 /************¶Á±¨¾¯ÑÓÊ±¼ä¸ô****************************/
		case Com_Config_Alarm_time_delay_Cmd_Code_Read  :
 		{
			
 			Com_Config_Alarm_time_delay_Read(Cal_Inf);
 			break;
 		}	
			
	 /************Ğ´±¨¾¯ÑÓÊ±¼ä¸ô****************************/
		case Com_Config_Alarm_time_delay_Cmd_Code_Write :
 		{
 			Com_Config_Alarm_time_delay_Write(Cal_Inf);
 			break;
 		}

  /************¶ÁÉù¹â±¨¾¯×Ü¿ª¹Ø×´Ì¬*************************/
		case Com_ConfigTotal_audible_light_alarm_Cmd_Code_Read :
 		{
			
			
 			Com_Config_Sound_lightSwitch_Read(Cal_Inf);
 			break;
 		}			
 /************Ğ´Éù¹â±¨¾¯×Ü¿ª¹Ø×´Ì¬*************************/
		case Com_ConfigTotal_audible_light_alarm_Cmd_Code_Write :
 		{
 			Com_Config_Sound_lightSwitch_Write(Cal_Inf);
 			break;
 		}	
/************¶Á¶ÌĞÅ±¨¾¯×Ü¿ª¹Ø×´Ì¬*************************/
		case Com_Config_SMS_alarm_total_switch_Cmd_Code_Read :
 		{
 			Com_Config_SMSalarmSwitch_Read(Cal_Inf);
 			break;
 		}				
/************Ğ´¶ÌĞÅ±¨¾¯×Ü¿ª¹Ø×´Ì¬*************************/
		case Com_Config_SMS_alarm_total_switch_Cmd_Code_Write :
 		{
 			Com_Config_SMSalarmSwitch_Write(Cal_Inf);
 			break;
 		}	

/************¶ÁÈ¡GSMÄ£¿éĞÅºÅÇ¿¶È*************************/
		case Com_Config_GSM_Signal_Strength_Cmd_Code_Read :
 		{

			Com_Config_GSM_Signal_Strength_Read(Cal_Inf);
 			break;
 		}	 
/************È¡ÏûÉù¹â±¨¾¯×÷ÓÃÓÚ°´·µ»Ø¼üÈ¡ÏûÉù¹â±¨¾¯µÄ×÷ÓÃÏàÍ¬*************************/
		case Cancel_Sound_Light_Alarm_Cmd_Code:
 		{
			
			DispMenuDeskTopDown();//È¡ÏûÉù¹â±¨¾¯
			Beep(Beep_OFF);
			Com_Config_Cancel_Sound_Light_Alarm(Cal_Inf);//ÏòÉÏÎ»»ú·µ»ØÈ¡ÏûÉù¹â±¨¾¯³É¹¦
			
 			break;
 		}	
/************Çå³ıÀúÊ·Êı¾İ0X52************************/
		case Clear_Historical_Data_Cmd_Code:
 		{

			Mcu_Cer_Htc_Data();//Çå³şÀúÊ·Êı¾İº¯Êı
		  Com_Config_Clear_Historical_Data(Cal_Inf);//ÏòÉÏÎ»»ú·µ»ØÇå³ıÀúÊ·Êı¾İ³É¹¦
 			break;
 		}	

/************´ò¿ªGSMÄ£¿é´®¿Úµ÷ÊÔĞÅÏ¢************************/
		case Open_GSM_Debug_Information_Cmd_Code:
 		{
       
			   if(Cal_Inf->Com_Data[0]==1)
				 {
				 	/* ´ò¿ªGSMµ÷ÊÔĞÅÏ¢Êä³ö */
//					GSM_DEBUG_Flag = TRUE;
					 
					DEBUG_Flag = TRUE;
			 Com_Config_Open_GSM_Debug_Information(Cal_Inf);//ÏòÉÏÎ»»ú·µ»Ø´ò¿ªGSMÄ£¿é´®¿Úµ÷ÊÔĞÅÏ¢³É¹¦ 
				 }
				 else if(Cal_Inf->Com_Data[0]==0)
				 {
				 	/* ¹Ø±ÕGSMµ÷ÊÔĞÅÏ¢Êä³ö */
//					GSM_DEBUG_Flag = FALSE;
						DEBUG_Flag = FALSE; 
			 Com_Config_Close_GSM_Debug_Information(Cal_Inf);//ÏòÉÏÎ»»ú·µ»Ø¹Ø±ÕGSMÄ£¿é´®¿Úµ÷ÊÔĞÅÏ¢³É¹¦
				 
				 }
 			break;
 		}
/************Êä³ö32Ö÷°åÍâ½Óµç×´Ì¬ºÍµç³ØµçÑ¹ÖµĞÅÏ¢************************/
		case Check_PowerLink_And_Battery_Voltage_Cmd_Code:
 		{
       
		Com_Config_PowerLink_And_Battery_Voltage_Information(Cal_Inf);//ÏòÉÏÎ»»ú·µ»Ø32Î»Ö÷°åÍâ½Óµç×´Ì¬¼°µç³ØµçÑ¹
 			break;
 		}		
/************¿ªÆôÊı¾İ¼ÇÂ¼************************/
		case Open_DataSave_Cmd_Code:
 		{
			  if(Cal_Inf->Com_Data[0]==1)
				{	
       	/* Æô¶¯ Êı¾İ´æ´¢¹¦ÄÜ. */
			App_Save_Config(ENABLE);
		  Com_Config_Open_Data_Save_Record(Cal_Inf);//ÏòÉÏÎ»»ú·µ»Ø´ò¿ªÊı¾İ¼ÇÂ¼³É¹¦
				}
				else if(Cal_Inf->Com_Data[0]==0)
				{
				
				    /* ¹Ø±Õ Êı¾İ´æ´¢¹¦ÄÜ. */
			App_Save_Config(DISABLE);
			
		Com_Config_Close_Data_Save_Record(Cal_Inf);//ÏòÉÏÎ»»ú·µ»Ø¹Ø±ÕÊı¾İ¼ÇÂ¼³É¹¦	
				
				}
				
 			break;
 		}			
///************Í£Ö¹Êı¾İ¼ÇÂ¼************************/
//		case Close_DataSave_Cmd_Code:
// 		{
//    
// 			break;
// 		}	
/************²éÑ¯Êı¾İ¼ÇÂ¼µÄ×´Ì¬************************/
		case Check_DataSave_Status_Cmd_Code:
 		{
			
	   Com_Config_DataSave_Status_Read(Cal_Inf);//ÏòÉÏÎ»»ú·µ»ØÊı¾İ¼ÇÂ¼×´Ì¬			
			 
 			break;
 		}

	case Com_Config_SystemTime_Cmd_Code_Read_Write://ÉÏÎ»»úÈí¼şÉèÖÃ ºÍ¶ÁÈ¡ÏµÍ³Ê±¼ä
 		{
      Com_Config_SystemTime_Write(Cal_Inf);//¶ÁĞ´ÏµÍ³Ê±¼ä
 			break;
 		}	
 case Com_Config_Software_Version_NO_Major_Cmd_Code_Read://ÉÏÎ»»úÈí¼ş32Î»Ö÷°åÈí¼ş°æ±¾ºÍÓ²¼ş°æ±¾
 		{

			Com_Config_Software_Version_NO_Major_Read(Cal_Inf);//¶ÁÈ¡32Î»Æ½Ì¨µÄÈí¼ş°æ±¾ºÍÓ²¼ş°æ±¾
		 
 			break;
 		}	
 case Com_Config_System_Factory_Setting://»Ö¸´³ö³§ÉèÖÃÖ¸Áî
 		{
			
		 Com_Config_Factory_Setting(Cal_Inf);//»Ö¸´³ö³§ÉèÖÃÖ¸Áî
 			break;
 		}
 case  0x4F://´®¿Ú²âÊÔ¹¦ÄÜÂë
 {
	 
////	 /* µ¥ÎÂ¿¨Æ¬´«¸ĞÆ÷ */
////#define SMS_Sensor_String	
//// char SMS_GBK[50];
////	 
////	 /* ÉèÖÃµçÔ´±¨¾¯ĞÅÏ¢ */
////	sprintf(SMS_GBK,SMS_Sensor_String);
	 
//////////      if(gsm_handle.sim800c.pGSM_Send_Sms("18738738517","µ¥ÎÂ¿¨Æ¬´«¸ĞÆ÷123!!!"))
//////////			{
//////////			   printf("·¢ËÍ¶ÌĞÅÄÚÈİ³É¹¦£¡£¡£¡\n");
//////////			
//////////			}else{
//////////			
//////////			printf("·¢ËÍ¶ÌĞÅÄÚÈİÊ§°Ü£¡£¡£¡\n");
//////////			}
    break;
 }

		
		default:
		{
			/* Î´Éè¶¨¹¦ÄÜ ²»×÷´¦Àí */
			__nop();
			break;
		}

	}	
	/* ÊÂ¼şÖ´ĞĞÍê³É Çå³ı Êı¾İ»º´æµÄÊı¾İ */
	Com_Sensor_Cal_Deleted_Buffer_Data(Cal_Inf);
}

/************************************************************************************************/
/**	º¯ Êı Ãû: COM_Sensor_Cal_Timeout_Check														*/
/**	¹¦ÄÜËµÃ÷: ´«¸ĞÆ÷Ğ£×¼ Í¨ĞÅ³¬Ê±¼ì²â																*/
/**	ĞÎ    ²Î£º´«¸ĞÆ÷Ğ£×¼Êı¾İ£ºCOM_SENSOR_CAL_TYPEDEF												*/
/**	·µ »Ø Öµ: ÎŞ																					*/
/************************************************************************************************/
void COM_Sensor_Cal_Timeout_Check(INSTRU_FIX_INF_TAB* fix_tab,u16 check_time)
{
	
	/* ÅĞ¶Ïµ±Ç°ÊÇ·ñ´¦ÓÚ½ÓÊÕÊı¾İÖ¡×´Ì¬ */
	if(fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout > 0)
	{
		/* µ±Ç°´¦ÓÚ ´«¸ĞÆ÷Ğ£×¼Êı¾İÖ¡½ÓÊÕ×´Ì¬ */
		fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout -= check_time;
		/* ÅĞ¶Ï Êı¾İ½ÓÊÕÊÇ·ñ³¬Ê± */
		if(fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout <= 0)
		{
			/* Êı¾İ½ÓÊÕ³¬Ê±£¬½áÊøÊı¾İ½ÓÊÕ */
			/* Çå³ı½ÓÊÕ¾İÊı¾İÁ¿ */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Data_Size = 0;
			/* Çå³ıÊı¾İÖ¡³¤¶È */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Packet_Len = 0;
			/* Çå³ı³¬Ê±Ê±¼ä */
			fix_tab->Inf_State.Instru_Com1.Inf_State.Rece_Com_Timeout = 0;
		}else
		{
			/* Êı¾İ½ÓÊÕÎ´³¬Ê±£¬¼ÌĞøÊı¾İ½ÓÊÕ */
			__nop();
		}
	}else
	{
		/* µ±Ç°Ã»ÓĞ½ÓÊÕÊı¾İ */
		__nop();
	}
}

/******************* (C) COPYRIGHT 2015 HZ Zeda Team ****END OF FILE****/


