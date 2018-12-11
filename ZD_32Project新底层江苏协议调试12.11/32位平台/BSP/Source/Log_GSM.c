/*************************************************************************************************/
/*************************************     32Î»Æ½Ì¨²âÊÔ     **************************************/
/* ´´½¨ÈÕÆÚ£º2014/11/20																			 */
/* ÐÞ¸ÄÈÕÆÚ£º2016/07/25																			 */
/*************************************************************************************************/
#include "interface.h"
#include "BSP.h"
#include "app.h"

#include "Ks0108.h"
#include "Log_GSM.h"


#include "Dev_RTC.h"//2018.12.04  ByZWC  Add
/***********¸ü¸Äµ×²ãÔö¼ÓµÄÍ·ÎÄ¼þ*************/
#include "GSM_app.h"
#include "Gsm_Interface.h"
#include "Gsm_Api.h"
/****************************/

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
	#include "Log_ConfigService.h"
	#include "ComProtocol_JS.h"//½­ËÕÊ¡ÃâÒß¹æ»®Êý¾ÝÖÐÐÄÐ­Òé
#elif (AppGPRSComProtocol == 0x01)
	#include "Log_ConfigService_Http.h"
#elif (AppGPRSComProtocol == 0x02)
	#include "ComProtocol_ZDE.h"
#endif
#endif

#if (AppGYForwardEnable == 0x01)
#include "ComProtocol_GY.h"
#include "App_Save.h"
#endif

#if (ComProtocol_WZ_Enable == 0x01)
#include "ComProtocol_WZ.h"
#include "App_Save.h"
#endif

#include "Log_Download.h"
#include "Log_HTTP.h"
#include "Log_IAP.h"
#include "Log_Save.h"
#include "Log_Sensor.h"
#include "Log_SMS.h"

#include "Dev_Flash.h"
#include "Dev_Fram.h"
#include "Dev_RTC.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern BOOL GSM_DEBUG_Flag;
/************************************************************************************************/
/*	ÕâÀïn±íÊ¾network£¬h±íÊ¾host£¬																	*/
/*	htons( ) htonl( )ÓÃÓÚ±¾µØ×Ö½ÚÏòÍøÂç×Ö½Ú×ª»»µÄ³¡ºÏ£¬												*/
/*	s±íÊ¾short£¬¼´¶Ô2×Ö½Ú²Ù×÷£¬																	*/
/*	l±íÊ¾long,  ¼´¶Ô4×Ö½Ú²Ù×÷¡£																	*/
/*	Í¬Ñùntohs( )ntohl( )ÓÃÓÚÍøÂç×Ö½ÚÏò±¾µØ¸ñÊ½×ª»»µÄ³¡ºÏ¡£ 											*/
/************************************************************************************************/
#define htons(A)     ((((u16)(A) & 0xff00) >> 8) | \
(((u16)(A) & 0x00ff) << 8))
#define htonl(A)     ((((u32)(A) & 0xff000000) >> 24) | \
(((u32)(A) & 0x00ff0000) >> 8) | \
(((u32)(A) & 0x0000ff00) << 8) | \
(((u32)(A) & 0x000000ff) << 24))
//#define ntohs htons
//#define ntohl htohl

/* ÉùÃ÷ÎÄ¼þ º¯Êý */

/* ½«ÒÇÆ÷µÄ´«¸ÐÆ÷Êý¾Ý×ª»»³ÉÍøÂçÐòÊý¾Ý */
static void GSM_Module_Config(INSTRU_GSM* gprs_inf);
//static void GPRS_Config_Server_Inf(INSTRU_GSM* gprs_inf);

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
static void GPRS_Data_To_Network(void);
#endif

static BOOL GPRS_Parsing_Network_Data(void);//½âÎö·þÎñÆ÷·µ»ØµÄÊý¾ÝÖ¸ÁîÊÇ·ñºÏ·¨
 void GPRS_Send_Network_Data(void);//Ïò·þÎñÆ÷·¢ËÍÎÂÊÒ¶ÈÊý¾Ý
//static void Remote_Upgrade_Program(void);//Ïò·þÎñÆ÷·¢ËÍÌØ¶¨µÄÖ¸Áî
#if ((GSM_Module_Type==0)||(GSM_Module_Type==3)) //SIM800C»òÕßÖÐÐËÄ£¿é
static void GSM_SMS_Send(INSTRU_GSM* gprs_inf);
#endif
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý */
static void GPRS_Send_System_Clock_Calibration_Data(void);
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))
/* ½âÎö·þÎñÆ÷·µ»ØµÄÊ±¼äÊý¾Ý£¬²¢ÅÐ¶Ï±¾´ÎGPRSÍ¨ÐÅ×´Ì¬¡£ */
static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer);
#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x02))
extern CreateHTTPParaStruct HTTPPara;
#endif
#endif

/* ¶¨Òå1KµÄÊý¾Ý ·¢ËÍ»º´æ */
#define GPRS_Send_Sensor_Data_Buffer_Size	(1536u)
/* GPRS ·¢ËÍÊý¾Ý»º´æ */
static u8 GPRS_Send_Sensor_Data_Buffer[GPRS_Send_Sensor_Data_Buffer_Size];
/* ¶¨Òå1500×Ö½ÚµÄÊý¾Ý ½ÓÊÕ»º´æ(ÕâÀïÊÇ¸ù¾Ýµ×²ãµÄ½ÓÊÕ¿Õ¼ä) */
#define GPRS_Rece_Sensor_Data_Buffer_Size	(1500u)
/* GPRS ½ÓÊÕÊý¾Ý»º´æ */
static u8 GPRS_Rece_Sensor_Data_Buffer[GPRS_Rece_Sensor_Data_Buffer_Size];


/* ÅäÖÃGSM ÏûÏ¢ */
static GSM_QUE GSM_QUE_Init_Module			                   = {Init_GSM_Module_EVENT,	((u32*)0X0),TRUE};
/* ¸üÐÂÐÅºÅÇ¿¶È ÏûÏ¢ */                                     
static GSM_QUE GSM_QUE_Update_Signal 		                   = {Update_SGL_EVENT,		  ((u32*)0X0),TRUE};
/* ¸üÐÂ¸½×Å×´Ì¬ ÏûÏ¢ */                                     
static GSM_QUE GSM_QUE_Update_Link 			                   = {Update_Link_EVENT,		  ((u32*)0X0),TRUE};
/* ÉÏ´«ÎÂÊª¶ÈÊý¾Ý ÏûÏ¢ */                                   
static GSM_QUE GSM_QUE_Update_Data 			                   = {Updata_Data_EVENT,		  ((u32*)0X0),TRUE};
/* ÏÂÔØ ³ÌÐò¸üÐÂ Êý¾Ý */                                   
static GSM_QUE GSM_QUE_Download_IAP_Data	                 = {IAP_EVENT,				    ((u32*)0X0),TRUE};
/* ·¢ËÍ¶ÌÐÅ Êý¾Ý */                                        
static GSM_QUE GSM_QUE_Send_SMS				                     = {Send_SMS_EVENT,			    ((u32*)0X0),TRUE};
/* ·¢ËÍÅäÖÃ Êý¾Ý */                                        
static GSM_QUE GSM_QUE_ConfigService		                   = {ConfigService_EVENT,		((u32*)0X0),TRUE};
/* ÏµÍ³Ê±ÖÓÐ£×¼ÐÅÏ¢ */
static GSM_QUE GSM_QUE_Send_System_Clock_Calibration	     = {System_Clock_Calibration_EVENT,	((u32*)0X0),TRUE};

#if (GSM_Module_Type==0x02) //ESP8266WifiÄ£¿é
/* ÉèÖÃWifiÃû×ÖºÍÃÜÂë*/
static GSM_QUE    Wifi_QUE_SetWifiNamePWD	                 = {SetWifiNamePWD_EVENT,	((u32*)0X0),TRUE};
/* ÉèÖÃWifiWlan²ÎÊý*/
static GSM_QUE    Wifi_QUE_SetWifiWLAN                     = {SetWifiWLAN_EVENT,	((u32*)0X0),TRUE};
/* ÉèÖÃWifiÄ£¿é»ñÈ¡IPµØÖ··½Ê½   STATIC»òÕßDHCPÄ£Ê½²ÎÊý*/
static GSM_QUE    Wifi_QUE_GET_IP_WAY                      = {SetWifiWLAN_GET_IP_WAY_EVENT,	((u32*)0X0),TRUE};
/*¸´Î»WifiÄ£¿é*/
static GSM_QUE    Wifi_QUE_Reset                           = {RestWifi_EVENT,	((u32*)0X0),TRUE};
/* WifiÄ£¿é»Ö¸´³ö³§ÉèÖÃ*/
static GSM_QUE    Wifi_QUE_Factory_Data_Reset              = {Factory_Data_Reset_Wifi_EVENT,	((u32*)0X0),TRUE};
#endif	



//GSMÏûÏ¢ ±êÊ¶Âë
#define	GPRS_COM_Identify			(0X7E)
//GSM ×ªÒå ±êºÅ
#define	GSM_Translator_Symbol_0X7E	GPRS_COM_Identify
#define	GSM_Translator_Symbol_0X02	0X02	
#define	GSM_Translator_Symbol_0X7D	0X7D
#define	GSM_Translator_Symbol_0X01	0X01
//GSMÏûÏ¢ ÏûÏ¢Í· ÏûÏ¢ID(0X0900:ÉÏÐÐÍ¨ÐÅ;0X8900:ÏÂÐÐÍ¨ÐÅ;0X8001:Í¨ÓÃÓ¦´ð)
#define	GPRS_COM_ID_Upload			   (0X0900)
#define	GPRS_COM_ID_Download		   (0X8900)
#define GPRS_COM_ID_ACK				     (0X8001)
//ÏûÏ¢Ìå³¤¶È£¨±ÈÈç:ÏûÏ¢ÀàÐÍ+ÏûÏ¢Êý¾ÝÀàÐÍ+ÒÇÆ÷µ±Ç°×´Ì¬+Ê±¼ä´Á+Í¨µÀÊý+2¸öÍ¨µÀÊý¾Ý=19Byte£©
#define GPRS_COM_Body_Len			     (0u)
//ÏûÏ¢ Á÷Ë®ºÅ
#define GPRS_COM_Serial_Num			   (0u)
//ÏûÏ¢ÌåÊôÐÔ Êý¾Ý¼ÓÃÜ·½Ê½(²»¼ÓÃÜ)
#define GPRS_COM_Enc_Mode			     (0u)
//ÏûÏ¢ÌåÊôÐÔ Êý¾Ý·Ö°ü±êÖ¾(²¿·Ö±í)
#define GPRS_COM_Sub_Flag			     (0u)
//ÏûÏ¢ÀàÐÍ:WIFI ¿¨Æ¬Ê½´«¸ÐÆ÷
#define GPRS_COM_Message_Type		   (0XF0)


#if (AppGPRSComProtocol == 0x00)//²¿±êÐ­Òé
//ÏûÏ¢ÀàÐÍ:32Î»Æ½Ì¨ÎÂÊª¶ÈÊý¾Ý
#define GPRS_32Platforms_Type		(0XFC)//GPRS_ZWC
#endif

#if (AppGPRSComProtocol == 0x03)//ºÓ±±ÊÓ´°Ð­Òé
//ÏûÏ¢ÀàÐÍ:32Î»Æ½Ì¨ÎÂÊª¶ÈÊý¾Ý
#define GPRS_32Platforms_Type		(0XFD)//GPRS_ZWC
#endif


//ÏûÏ¢Êý¾ÝÀàÐÍ:0:ÊµÊ±Êý¾Ý£»1:ÀúÊ·Êý¾Ý
#define GPRS_COM_Data_Type			(0x01)
//´«¸ÐÆ÷Í¨µÀÊýÁ¿ 
#define GPRS_COM_Sensor_Num			(Sensor_CH_Config_Num)
//Ó¦´ð ÏûÏ¢Ìå³¤¶È */
#define GPRS_COM_ACK_Att_Len		(5u)

// ÏµÍ³ÄÚÖÃµÄgprs·þÎñÆ÷µØÖ·(GSMÄ£¿éÖ§³Ö×î¶à3¸öGPRS²¢·¢Á¬½Ó)
#define GPRS_Server_CONN_Num_Max	(MAX_NUM_TCP_CONN)//µ«ÊÇÕâÀïÎªÊ²Ã´ÉèÖÃ³É4..wo²»ÊÇºÜÀí½â¡£
// ·þÎñÆ÷ ±àºÅ
#define	GPRS_Server1_NO					(0u)
#define	GPRS_Server2_NO					(1u)
#define	GPRS_Server3_NO					(2u)


/************************************************************************************************/
/* GPRS ÎÂÊª¶ÈÊý¾ÝÐÅÏ¢ Êý¾Ý¼Ä´æÆ÷ */
static GSM_GPRS_Send_Struct GPRS_Send_Reg;

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
/* GPRS ·¢ËÍÎÂÊª¶ÈÊý¾ÝÐÅÏ¢ Êý¾Ý¼Ä´æÆ÷¹Ì¶¨ÅäÖÃ */
static const GSM_GPRS_Send_Struct GPRS_Send_Reg_Fix =
{
	/* Í· ±êÊ¶Î» */ //(1 Byte)
	GPRS_COM_Identify,
	/* Í¨ÐÅ ÏûÏ¢Í· *///(16 Byte)
	/* ÏûÏ¢ ID(ÃüÁî) *///(2 Byte)
	GPRS_COM_ID_Upload/0X100,
	GPRS_COM_ID_Upload%0X100,
	/* ÏûÏ¢Ìå ÊôÐÔ *///(2 Byte)
	//ÏûÏ¢ÌåÊôÐÔ ±£Áô
	0X0,
	//ÏûÏ¢ÌåÊôÐÔ ·Ö°ü
	GPRS_COM_Sub_Flag,
	//ÏûÏ¢ÌåÊôÐÔ Êý¾Ý¼ÓÃÜ·½Ê½
	GPRS_COM_Enc_Mode,
	//ÏûÏ¢ÌåÊôÐÔ ÏûÏ¢Ìå³¤¶È
	GPRS_COM_Body_Len/0X100,
	//ÏûÏ¢ÌåÊôÐÔ ÏûÏ¢Ìå³¤¶È
	GPRS_COM_Body_Len%0X100,
	/* ÖÕ¶ËÐòÁÐºÅ(SNºÅ): */	//(6 Byte)
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* ÏûÏ¢ Á÷Ë®ºÅ *///(2 Byte)
	GPRS_COM_Serial_Num/0X100,GPRS_COM_Serial_Num%0X100,
	
	/* Í¨ÐÅ ÏûÏ¢Ìå */
	/* ÏûÏ¢ÀàÐÍ:WIFI ¿¨Æ¬Ê½´«¸ÐÆ÷(ÏÖÔÚÖ»ÓÃÕâÖÖÄ£Ê½) *///(1 Byte)
	GPRS_32Platforms_Type,
	/* ÏûÏ¢Êý¾ÝÀàÐÍ:0->ÊµÊ±Êý¾Ý£»1->ÀúÊ·Êý¾Ý¡£ *///(1 Byte)
	GPRS_COM_Data_Type,
	/* Êý¾ÝÌõÊý *///(1 Byte)
	0,
	/* Êý¾ÝÄÚÈÝ(Õâ¸öÖ÷ÒªÊÇÎªÁË¸øºóÃæÌí¼ÓÊý¾ÝµÄÖ¸Õë) *///(1 Byte)
	0,
	
};
/************************************************************************************************/
/* GPRS Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
static GSM_GPRS_Data_Content_Struct GPRS_Data_Content_Reg;


/* GPRS Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ ¹Ì¶¨ÅäÖÃ */
static const GSM_GPRS_Data_Content_Struct GPRS_Data_Content_Reg_Fix = 
{
	/* ´«¸ÐÆ÷(IDºÅ): */	//(4 Byte)
	0,
	/* Íâ½ÓµçÔ´×´Ì¬ */	//Ä¬ÈÏ0 ²»Õý³£
	0,
	/* Ô¤Áô1 */
	0,
	/* Ô¤Áô2 */
	0,
	/* µç³ØµçÁ¿ */		//Ä¬ÈÏ0 ²»Õý³£
	0,
	/* Ê±¼ä´Á */			//(6 Byte)
	//Äê//(1 Byte)
	0,
	//ÔÂ//(1 Byte)
	0,
	//ÈÕ//(1 Byte)
	0,
	//Ê±//(1 Byte)
	0,
	//·Ö//(1 Byte)
	0,
	//Ãë//(1 Byte)
	0,
	/* Í¨µÀÊý */
	0,
	/* ´«¸ÐÆ÷ÀàÐÍ */
	0,
	/* Êý¾Ý(Õâ¸öÖ÷ÒªÊÇÎªÁË¸øºóÃæÌí¼ÓÊý¾ÝµÄÖ¸Õë) */
	0.0,
};

#endif
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))
/* GPRS ½ÓÊÕÎÂÊª¶ÈÊý¾ÝÐÅÏ¢ Êý¾Ý¼Ä´æÆ÷ */
static GSM_GPRS_Rece_Struct GPRS_Rece_Reg;
#endif
#endif
/************************************************************************************************/
/* GPRS ·¢ËÍÎÂÊª¶ÈÊý¾ÝÐÅÏ¢ Êý¾Ý¼Ä´æÆ÷ */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_Sensor_Data_Reg = 
{
	/* GPRS ½ÓÊÕ ´«¸ÐÆ÷ Êý¾ÝÊýÁ¿ */
	0,
	/* GPRS ·¢ËÍ ´«¸ÐÆ÷ Êý¾ÝÊýÁ¿ */
	0,
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾ÝÍ¨ÐÅ×´Ì¬£¨FALSE:Ã»ÓÐ·¢ËÍ£©*/
	FALSE,
	/* GPRS Ö÷»úÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬£¨FALSE:Ã»ÓÐÊý¾Ý´¦Àí£© */
	FALSE,
	/* GPRS ÍøÂçÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬£¨FALSE:Ã»ÓÐÊý¾Ý´¦Àí£© */
	FALSE,
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾Ý½á¹û */
	TRUE,
	/* GPRS ½ÓÊÕ´«¸ÐÆ÷Êý¾Ý½á¹û */
	TRUE,
	/* GPRS ½âÎöÊý¾Ý×´Ì¬ */
	TRUE,	
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾Ý Ê§°Ü´ÎÊý */
	0,
};
/************************************************************************************************/
//static GSM_GPRS_SR_Struct GPRS_SR_Reg = //GSM_ZWC
 GSM_GPRS_SR_Struct GPRS_SR_Reg = 
{
	/* GPRS ÐÅºÅ Ç¿¶È */
	0,
	/* GPRS ÐÅºÅ Ç¿¶È ÏÔÊ¾(0-5:0¸ñ;6-8:1¸ñ;9-11:2¸ñ;12-14:3¸ñ;15-31:4¸ñ;) */
	0,
	/* GPRS Ä£¿é¹¤×÷×´Ì¬ */
	FALSE,
	/* GPRS ¸½×Å×´Ì¬ */
	FALSE,
	/* ÎÞGPRSÐÅºÅ */
	SIM_NoSignal,
};
/************************************************************************************************/
////GPRS·þÎñÆ÷ÐÅÏ¢
//static GPRS_Connection GPRS_COM_Reg[GPRS_Server_CONN_Num_Max];
/************************************************************************************************/
//GSM ¶ÌÐÅ ×´Ì¬¼Ä´æÆ÷
GSM_SMS_SR_Struct GSM_SMS_SR_Reg = 
{
	FALSE,
#if (SMS_Function_Enable == 0x01)
	
#if (SMS_User_Number == 0x00)
#error "ÎÂÊª¶È¿¨Æ¬Í¨µÀÅäÖÃ²»ÕýÈ·"
#endif	
	
#if (SMS_User_Number >= 0x01)
	TRUE,
#endif
	
#if (SMS_User_Number >= 0x02)
	TRUE,
#endif
	
#if (SMS_User_Number >= 0x03)
	TRUE,
#endif
	
#endif
};
/************************************************************************************************/
#if ((GSM_Module_Type==0)||(GSM_Module_Type==3)) //SIM800C»òÕßÖÐÐËÄ£¿é
//GSM ¶ÌÐÅ Êý¾Ý¼Ä´æÆ÷
static GSM_SMS_DR_Struct GSM_SMS_DR_Reg;
#endif
/************************************************************************************************/
/* GPRS Ó¦ÓÃ¼Ä´æÆ÷ */
GPRS_Struct GPRS;
/************************************************************************************************/

#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
/************************************************************************************************/
/* º¯ÊýÃû	: GPRS_Timestamp_Hex_To_BCD_Code													*/
/* ÊäÈë		: src:ÊäÈëHEXÂë;dest:Êä³öHEXÂë														*/
/* Êä³ö		: ÎÞ 																				*/
/* ×÷ÓÃ		: ½«HEXµÄÊ±¼äÈÕÆÚ×ª»»³ÉBCDÊ±¼äÈÕÆÚÐÎÊ½				 									*/
/* ´´½¨ÈÕÆÚ	: 2015/03/30		HEX2BCD															*/
/************************************************************************************************/
static void GPRS_Timestamp_Hex_To_BCD_Code(GSM_GPRS_Data_Content_Struct* dest,Save_Sensor_Data_Struct* src)
{
	/* »ñµÃÃëÊý¾Ý */
	dest->GPRS_DR_FIX.Timestamp_Sec 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS);
	/* »ñµÃ·ÖÊý¾Ý */                        
	dest->GPRS_DR_FIX.Timestamp_Min 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS);
	/* »ñµÃÊ±Êý¾Ý */                        
	dest->GPRS_DR_FIX.Timestamp_Hour 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS);
	/* »ñµÃÈÕÊý¾Ý */                        
	dest->GPRS_DR_FIX.Timestamp_Day 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS);
	/* »ñµÃÔÂÊý¾Ý */                        
	dest->GPRS_DR_FIX.Timestamp_Month 	=   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS);
	/* »ñµÃÄêÊý¾Ý *///ÕâÀïµÄÄê£¬×î´ó±£´æ63Äê£¬´Ó2000Äê¿ªÊ¼ËãÆð£¬³¬¹ý½«ÖØÐÂ´Ó2010¿ªÊ¼Ëã
	dest->GPRS_DR_FIX.Timestamp_Year 	  =   HEX_To_BCD(src->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS);
}
#endif

/************************************************************************************************/
/* º¯ÊýÃû	: GPRS_GET_Save_BSP_Sensor_Data														*/
/* ÊäÈë		: src:ÊäÈëHEXÂë;dest:Êä³öHEXÂë														*/
/* Êä³ö		: ÎÞ 																				*/
/* ×÷ÓÃ		: »ñÈ¡´æ´¢µÄ°åÔØ´«¸ÐÆ÷Êý¾Ý							 									*/
/* ´´½¨ÈÕÆÚ	: 2015/03/30																		*/
/************************************************************************************************/
#if ((AppGPRSComProtocolEnable == 0x01)&&((AppGPRSComProtocol == 0x00)))
#if ((Sensor_Tmp_Num+Sensor_Hum_Num+Sensor_PT1000_Num) != 0)
static void GPRS_GET_Save_BSP_Sensor_Data(GSM_GPRS_Data_Content_Struct* dest,Save_Sensor_Data_Struct* src)
{
	/* ´«¸ÐÆ÷Êý¾Ý´æ·ÅÊý¾ÝµÄ¿ªÊ¼Î»ÖÃ */
	u8 sensor_data_offset = 0;
	/* GPRSÊý¾Ý°ü´æ·ÅÊý¾ÝµÄÎ»ÖÃ */
	u8 gprs_data_content_offset = 0;
	/* ÎÂÊª¶ÈÊý¾Ý */
	float sensor_data;

	
/* ÅÐ¶Ï Í¨µÀ1 */
#if 	((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
		(Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH1_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH1_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* ÅÐ¶Ï Í¨µÀ2 */
#if 	((Sensor_CH2_Config_Type == Sensor_Tmp) ||\
(Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH2_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH2_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ3 */
#if 	((Sensor_CH3_Config_Type == Sensor_Tmp) ||\
(Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH3_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH3_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ4 */
#if 	((Sensor_CH4_Config_Type == Sensor_Tmp) ||\
(Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH4_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH4_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ5 */
#if 	((Sensor_CH5_Config_Type == Sensor_Tmp) ||\
(Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH5_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH5_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ6 */
#if 	((Sensor_CH6_Config_Type == Sensor_Tmp) ||\
(Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH6_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH6_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ7 */
#if 	((Sensor_CH7_Config_Type == Sensor_Tmp) ||\
(Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
(Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
		 (Sensor_CH7_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH7_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ8 */
#if 	((Sensor_CH8_Config_Type == Sensor_Tmp) ||\
(Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH8_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH8_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* ÅÐ¶Ï Í¨µÀ9 */
#if 	((Sensor_CH9_Config_Type == Sensor_Tmp) ||\
(Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH9_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH9_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* ÅÐ¶Ï Í¨µÀ10 */
#if 	((Sensor_CH10_Config_Type == Sensor_Tmp) ||\
(Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH10_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH10_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ11 */
#if 	((Sensor_CH11_Config_Type == Sensor_Tmp) ||\
(Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH11_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH11_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ12 */
#if 	((Sensor_CH12_Config_Type == Sensor_Tmp) ||\
(Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH12_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH12_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ13 */
#if 	((Sensor_CH13_Config_Type == Sensor_Tmp) ||\
(Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH13_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH13_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ14 */
#if 	((Sensor_CH14_Config_Type == Sensor_Tmp) ||\
(Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH14_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH14_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ15 */
#if 	((Sensor_CH15_Config_Type == Sensor_Tmp) ||\
(Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH15_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH15_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ16 */
#if 	((Sensor_CH16_Config_Type == Sensor_Tmp) ||\
(Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH16_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH16_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* ÅÐ¶Ï Í¨µÀ17 */
#if 	((Sensor_CH17_Config_Type == Sensor_Tmp) ||\
(Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH17_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH17_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* ÅÐ¶Ï Í¨µÀ18 */
#if 	((Sensor_CH18_Config_Type == Sensor_Tmp) ||\
(Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH18_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH18_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ19 */
#if 	((Sensor_CH19_Config_Type == Sensor_Tmp) ||\
(Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH19_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH19_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ20 */
#if 	((Sensor_CH20_Config_Type == Sensor_Tmp) ||\
(Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH20_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH20_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ21 */
#if 	((Sensor_CH21_Config_Type == Sensor_Tmp) ||\
(Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH21_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH21_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ22 */
#if 	((Sensor_CH22_Config_Type == Sensor_Tmp) ||\
(Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH22_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH22_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ23 */
#if 	((Sensor_CH23_Config_Type == Sensor_Tmp) ||\
(Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH23_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH23_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ24 */
#if 	((Sensor_CH24_Config_Type == Sensor_Tmp) ||\
(Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH24_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH24_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* ÅÐ¶Ï Í¨µÀ25 */
#if 	((Sensor_CH25_Config_Type == Sensor_Tmp) ||\
(Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH25_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH25_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif
	
/* ÅÐ¶Ï Í¨µÀ26 */
#if 	((Sensor_CH26_Config_Type == Sensor_Tmp) ||\
(Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH26_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH26_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

/* ÅÐ¶Ï Í¨µÀ27 */
#if 	((Sensor_CH27_Config_Type == Sensor_Tmp) ||\
(Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH27_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH27_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ28 */
#if 	((Sensor_CH28_Config_Type == Sensor_Tmp) ||\
(Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH28_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH28_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ29 */
#if 	((Sensor_CH29_Config_Type == Sensor_Tmp) ||\
(Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH29_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH29_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif

	
/* ÅÐ¶Ï Í¨µÀ30 */
#if 	((Sensor_CH30_Config_Type == Sensor_Tmp) ||\
(Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH30_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH30_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ31 */
#if 	((Sensor_CH31_Config_Type == Sensor_Tmp) ||\
(Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH31_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH31_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


/* ÅÐ¶Ï Í¨µÀ32 */
#if 	((Sensor_CH32_Config_Type == Sensor_Tmp) ||\
(Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
		 (Sensor_CH32_Config_Type == Sensor_Hum) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
		 (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
		 (Sensor_CH32_Config_Type == Sensor_PT1000))
		//d¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(src->GPRS_IR.Sensor_Data[sensor_data_offset++]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*(((u32*)(&dest->GPRS_DR_FIX.Data_P))+(gprs_data_content_offset++)) = htonl(*((u32*)(&sensor_data)));
		//e¡¢ÉèÖÃÍ¨µÀÊý
		dest->GPRS_DR_FIX.CH_Num++;
#elif 	((Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum) ||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp) ||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum))
		sensor_data_offset++;
#endif


}

#endif
#endif
	
/************************************************************************************************/
/* º¯ÊýÃû	: GPRS_GET_RF_Humiture_Hum_Data														*/
/* ÊäÈë		: src:ÊäÈëHEXÂë;dest:Êä³öHEXÂë														*/
/* Êä³ö		: ÎÞ 																				*/
/* ×÷ÓÃ		: ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ					 									*/
/* ´´½¨ÈÕÆÚ	: 2015/03/30																		*/
/************************************************************************************************/
#if ((Sensor_RF_Humiture_Num > 0)||\
	 (AppGPRSComProtocol == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
static u8 GPRS_GET_RF_Humiture_Hum_Data(u32 dest_sn,INSTRU_SENSOR *sensor_t)
{
/* Ã»ÓÐ²éµ½Æ¥ÅäµÄÍ¨µÀ */
#define Sensors_CH_Null (0xFF)
	
	
/* ÅÐ¶Ï Í¨µÀ1 */
#if		((Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH1.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_1;
		}
#endif
	
/* ÅÐ¶Ï Í¨µÀ2 */
#if		((Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH2.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_2;
		}
#endif
	
/* ÅÐ¶Ï Í¨µÀ3 */
#if		((Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH3.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_3;
		}
#endif	
	
/* ÅÐ¶Ï Í¨µÀ4 */
#if		((Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH4.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_4;
		}
#endif	
	
/* ÅÐ¶Ï Í¨µÀ5 */
#if		((Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH5.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_5;
		}
#endif

/* ÅÐ¶Ï Í¨µÀ6 */
#if		((Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH6.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_6;
		}
#endif

/* ÅÐ¶Ï Í¨µÀ7 */
#if		((Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH7.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_7;
		}
#endif

/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH8.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_8;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH9.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_9;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH10.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_10;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH11.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_11;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH12.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_12;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH13.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_13;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH14.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_14;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH15.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_15;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH16.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_16;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH17.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_17;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH18.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_18;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH19.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_19;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH20.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_20;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH21.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_21;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH22.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_22;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH23.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_23;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH24.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_24;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH25.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_25;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH26.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_26;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH27.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_27;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH28.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_28;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH29.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_29;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH30.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_30;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH31.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_31;
		}
#endif
		
/* ÅÐ¶Ï Í¨µÀ8 */
#if		((Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)||\
		 (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Hum))
		/* ÅÐ¶Ï Êª¶ÈÍ¨µÀSNºÅÓëÄ¿±êµÄSNºÅÊÇ·ñÏàÍ¬ */
		if(sensor_t->Inf_State.CH32.SENSOR_RF_Card_ID_NO == dest_sn)
		{
			/* ÓÚÄ¿±êµÄSNºÅÏàÍ¬£¬ËµÃ÷¸ÃÊª¶ÈÍ¨µÀÓëÄ¿±êÎªÍ¬Ò»¿¨Æ¬ */
			return Sensors_CH_32;
		}
#endif
	
		/* µ½×îºó£¬Ã»ÓÐÕÒµ½Æ¥ÅäµÄÍ¨µÀ */
		return Sensors_CH_Null;
#undef Sensors_CH_Null
}
#endif

/************************************************************************************************/
/* º¯ÊýÃû	£ºGPRS_Config_Server_Inf															*/
/* ÊäÈë		£ºGPRS·þÎñÆ÷ÐÅÏ¢																		*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£ºÅäÖÃGPRS·þÎñÆ÷ÓòÃû¡¢IPµØÖ·¡¢¶Ë¿Ú¡£ 									 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/10																		*/
/************************************************************************************************/
//static void GPRS_Config_Server_Inf(INSTRU_GSM* gprs_inf)
//{

//	/* »ñÈ¡ GPRS µÄ ·þÎñÆ÷ÐÅÏ¢ */
//	//»ñÈ¡ÓòÃû
//	sprintf(GPRS_COM_Reg[GPRS_Server1_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID1);
//	sprintf(GPRS_COM_Reg[GPRS_Server2_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID2);
//	sprintf(GPRS_COM_Reg[GPRS_Server3_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID3);
//	//»ñÈ¡IP
//	if((gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server1_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server1_NO].ip,"");
//	}
//	
//	if((gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server2_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server2_NO].ip,"");
//	}
//	
//	if((gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C != 0) ||
//	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D != 0))
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"%03d.%03d.%03d.%03d",
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C,
//		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D);	
//	}else
//	{
//		sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"");
//		//sprintf(GPRS_COM_Reg[GPRS_Server3_NO].ip,"115.28.105.232");//2017.11.06½«Éý¼¶·þÎñÆ÷µÄIPµØÖ·¹Ì¶¨
//	}
//	
//	//»ñÈ¡¶Ë¿Ú
//	sprintf(GPRS_COM_Reg[GPRS_Server1_NO].port,"%05d",gprs_inf->Inf_State.Port_ID1);	
//	sprintf(GPRS_COM_Reg[GPRS_Server2_NO].port,"%05d",gprs_inf->Inf_State.Port_ID2);
//	sprintf(GPRS_COM_Reg[GPRS_Server3_NO].port,"%05d",gprs_inf->Inf_State.Port_ID3);
// // sprintf(GPRS_COM_Reg[GPRS_Server3_NO].port,"%05d",12342);//2017.11.06½«Éý¼¶·þÎñÆ÷µÄ¶Ë¿Ú¹Ì¶¨
//	
//}

/************************************************************************************************/
/* º¯ÊýÃû	£ºGSM_Module_Config																	*/
/* ÊäÈë		£ºGPRS·þÎñÆ÷ÐÅÏ¢																		*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£ºÅäÖÃ GSM Ä£¿é					 									 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/10																		*/
/************************************************************************************************/
static void GSM_Module_Config(INSTRU_GSM* gprs_inf)
{
	
	/* Çå³ý ·¢ËÍÊ§°Ü´ÎÊý */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum = 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum = 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum = 0;
	/* ÅäÖÃ GPRS ·þÎñÆ÷ ÐÅÏ¢ */
//	GPRS_Config_Server_Inf(gprs_inf);
	
	
	GSMConfig_Server_Inf(gprs_inf);
	
	
	
	/* ÖØÐÂÅäÖÃ GSM Ä£¿é */
//	GPRS_SR_Reg.GPRS_Work_State = (BOOL)gsm_env_init(GPRS_COM_Reg,(_SCSStruct*)&GPRS_SR_Reg.SimState);
	GPRS_SR_Reg.GPRS_Work_State = (BOOL)Sim800c_env_init((_SCSStruct*)&GPRS_SR_Reg.SimState);	
	

	//ÕâÀïµÄÒ»Ð©¹Ø¼üµÄµÄ×´Ì¬ÐÅÏ¢ÒÔºóºóÓÃÅÂÄãÆÁÄ»ÏÔÊ¾³öÀ´ÌáÊ¾£¬ÕâÐ©´úÂë¿ÉÒÔÈ¡Ïûµô¡£***±êÖ¾
	/* ÅÐ¶Ï Ä£¿é ÅäÖÃ ½á¹û */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		
//		printf("\rGSMÄ£¿é³õÊ¼»¯ÅäÖÃ³É¹¦£¡£¡£¡\r\n");	
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* GSMÄ£¿é ÅäÖÃ ³É¹¦ */
//			printf("\rGSMÄ£¿éÅäÖÃ³É¹¦\r\n");						
//		}				
	}else
	{
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* GSMÄ£¿é ÅäÖÃ Ê§°Ü */
//			printf("\rGSMÄ£¿é³õÊ¼»¯ÅäÖÃÊ§°Ü£¡£¡£¡\r\n");
//		}	
	}
}

/************************************************************************************************/
/* º¯ÊýÃû	£GSMConfig_Server_Inf(INSTRU_GSM* gprs_inf)													*/
/* ÊäÈë		£ºGPRS·þÎñÆ÷ÐÅÏ¢																		*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£ºÅäÖÃGPRS·þÎñÆ÷ÓòÃû¡¢IPµØÖ·¡¢¶Ë¿Ú¡£ 									 		 		*/
/* ´´½¨ÈÕÆÚ	£º2018/08/30																		*/
/************************************************************************************************/
void GSMConfig_Server_Inf(INSTRU_GSM* gprs_inf)
{

	/* »ñÈ¡ GPRS µÄ ·þÎñÆ÷ÐÅÏ¢ */
	//»ñÈ¡ÓòÃû
	sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID1);
	sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID2);
	sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].dn,"%s",gprs_inf->Inf_State.Domain_Name_ID3);
	//»ñÈ¡IP
	if((gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID1.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].ip,"");
	}
	
	if((gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID2.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].ip,"");
	}
	
	if((gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C != 0) ||
	   (gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D != 0))
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"%d.%d.%d.%d",
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_A,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_B,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_C,
		gprs_inf->Inf_State.IP_Address_ID3.Serial_Number_Number.Address_D);	
	}else
	{
		sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"");
		//sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].ip,"115.28.105.232");//2017.11.06½«Éý¼¶·þÎñÆ÷µÄIPµØÖ·¹Ì¶¨
	}
	
	//»ñÈ¡¶Ë¿Ú
	sprintf(GSM_Tcp_Conn[GPRS_Server1_NO].port,"%d",gprs_inf->Inf_State.Port_ID1);	
	sprintf(GSM_Tcp_Conn[GPRS_Server2_NO].port,"%d",gprs_inf->Inf_State.Port_ID2);
	sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].port,"%d",gprs_inf->Inf_State.Port_ID3);
 // sprintf(GSM_Tcp_Conn[GPRS_Server3_NO].port,"%05d",12342);//2017.11.06½«Éý¼¶·þÎñÆ÷µÄ¶Ë¿Ú¹Ì¶¨
	
}
//´òÓ¡GSMÄ£¿é·¢ËÍºÍ½ÓÊÕµÄÊý¾ÝÐÅÏ¢
void PrintGsmDebug(char *wbuf, int wlen, char *rbuf, int *rlen,u8 Server_Id)
{
	      unsigned int i;
       	if(DEBUG_Flag ==TRUE)	
				{
				   switch(Server_Id)
					{
						 case 0:
						 {
							     if(wlen>0)//·¢ËÍ
							     {
							    	  printf("·¢ËÍ¸øÊý¾Ý·þÎñÆ÷µÄÊý¾Ý:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("·¢ËÍ¸øÊý¾Ý·þÎñÆ÷µÄÊý¾Ý³¤¶ÈÎª0£¡£¡£¡\n"); 
							     }
							     if(rlen>0)//½ÓÊÕ
							     {
							    	  printf("½ÓÊÕµ½Êý¾Ý·þÎñÆ÷µÄÊý¾Ý:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("½ÓÊÕµ½Êý¾Ý·þÎñÆ÷µÄÊý¾Ý³¤¶ÈÎª0£¡£¡£¡\n"); 
							     }	 
							     break;
						 }
						  case 1:
						 {
							     if(wlen>0)//·¢ËÍ
							     {
							    	  printf("·¢ËÍ¸øÅäÖÃ·þÎñÆ÷µÄÊý¾Ý:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("·¢ËÍ¸øÅäÖÃ·þÎñÆ÷µÄÊý¾Ý³¤¶ÈÎª0£¡£¡£¡\n"); 
							     }
							     if(rlen>0)//½ÓÊÕ
							     {
							    	  printf("½ÓÊÕµ½ÅäÖÃ·þÎñÆ÷µÄÊý¾Ý:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("½ÓÊÕµ½ÅäÖÃ·þÎñÆ÷µÄÊý¾Ý³¤¶ÈÎª0£¡£¡£¡\n"); 
							     }	 
							     break;
						 }
						 	case 2:
						 {
							     if(wlen>0)//·¢ËÍ
							     {
							    	  printf("·¢ËÍ¸øÉý¼¶·þÎñÆ÷µÄÊý¾Ý:\n");
							        for(i=0;i<wlen;i++)
							        {
							          printf("%02X ",wbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("·¢ËÍ¸øÉý¼¶·þÎñÆ÷µÄÊý¾Ý³¤¶ÈÎª0£¡£¡£¡\n"); 
							     }
							     if(rlen>0)//½ÓÊÕ
							     {
							    	  printf("½ÓÊÕµ½Éý¼¶·þÎñÆ÷µÄÊý¾Ý:\n");
							        for(i=0;i<*rlen;i++)
							        {
							          printf("%02X ",rbuf[i]);
							        }
							    	  printf("\n\n");
							     }
							     else
							     {
							        printf("½ÓÊÕµ½Éý¼¶·þÎñÆ÷µÄÊý¾Ý³¤¶ÈÎª0£¡£¡£¡\n"); 
							     }	 
							     break;
						 }
						 
					
					   default:
						break;
					
					}
					
				
				
				
				}					

}

/************************************************************************************************/
/* º¯ÊýÃû	£ºGSM_Function																		*/
/* ÊäÈë		£ºgsm_que_inf:µ±Ç°µÄÏûÏ¢¶ÓÁÐ															*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£ºÖ´ÐÐÏûÏ¢¶ÓÁÐ µ±ÖÐµÄ ÏûÏ¢¹¦ÄÜ			 								 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/01																		*/
/************************************************************************************************/
void GSM_Function(GSM_QUE*  gsm_que_inf,INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
//	/* ÅÐ¶Ï µ±Ç° µÄµç³ØµçÁ¿ */
//	if(MCU_Battery.Power_State > Power_Null)
//	{
		/* µ±Ç°µÄµç³ØµçÁ¿ÄÜ¹»Î¬³Ö GSMÄ£¿éÕý³£¹¤×÷¡££¨´óÓÚ3.5V£© */
		/* ¸ù¾Ý ÏûÏ¢¶ÓÁÐ µÄÃüÁîÀàÐÍ */
		switch(gsm_que_inf->EVENT_Cmd)
		{
			case Init_GSM_Module_EVENT:
			{
				
				/* GSM ³õÊ¼»¯ÅäÖÃ */
				GSM_Module_Config(&config_tab->Inf_State.Instru_GSM);
				break;
			}

			default :
			{
				/* ÓÉÓÚºóÃæµÄ ¹¦ÄÜ ¶¼ÊÇ»ùÓÚÄ£¿é³É¹¦ÅäÖÃºó²ÅÄÜÕý³£Ê¹ÓÃµÄ£¬Òò´ËÏÈ×öÅÐ¶Ï */
				if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
				{
					/* GSM Ä£¿éÅäÖÃ³É¹¦ */
					/* ¸ù¾Ý ÏûÏ¢¶ÓÁÐ µÄÃüÁîÀàÐÍ */
					switch(gsm_que_inf->EVENT_Cmd)
					{
						//	/* ¸üÐÂÐÅºÅÇ¿¶È */
						case Update_SGL_EVENT:
						{	
							/* »ñµÃµ±Ç° ÐÅºÅÇ¿¶È */
//						GPRS_SR_Reg.GPRS_Sign_Stre = gsm_get_signal();
						GPRS_SR_Reg.GPRS_Sign_Stre =Gsm_get_signal();
						break;
						}										
/*********************************************************************************/				
						//	/* ¸üÐÂ¸½×Å×´Ì¬ */
						case Update_Link_EVENT:
						{
//							/* »ñµÃµ±Ç°µÄ ¸½×Å×´Ì¬ */
//						GPRS_SR_Reg.GPRS_Link_State = (BOOL)gsm_get_gprs_state();
			
//////////						GPRS_SR_Reg.GPRS_Link_State =(BOOL)Gsm_get_gprs_state();
						break;
						}
/*********************************************************************************/						
						//	/* ÉÏ´«ÎÂÊª¶ÈÊý¾Ý */
						case Updata_Data_EVENT:
						{		
////						    	/* ·¢ËÍÊý¾Ý */
////					       GPRS_Send_Network_Data();
						break;
						}
/*********************************************************************************/		
						//	/* ÏÂÔØ ³ÌÐò¸üÐÂ Êý¾Ý */
						case IAP_EVENT:
						{	
							#if (AppGPRSComProtocolEnable == 0x01)
              #if (AppGPRSComProtocol == 0x00)	
							/* »ñÈ¡ ¸üÐÂ³ÌÐò Êý¾Ý */
							GPRS.IAP.State.Result = 
							(BOOL)Gsm_gprs_comm(
							((char*)GPRS.IAP.Para.TXDR),
							(GPRS.IAP.Para.TXLen),
							((char*)GPRS.IAP.Para.RXDR),
							((&GPRS.IAP.Para.RXLen)),
							 GSM_GPRS_Update
							);
							
              PrintGsmDebug(
							((char*)GPRS.IAP.Para.TXDR),
							(GPRS.IAP.Para.TXLen),
							((char*)GPRS.IAP.Para.RXDR),
							((&GPRS.IAP.Para.RXLen)),
							 GSM_GPRS_Update
							);	
							/* Êý¾Ý·¢ËÍÊ§°Ü */
							if(FALSE == GPRS.IAP.State.Result)
							{
								GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum++;	

								/* ÅÐ¶Ï·¢ËÍÊ§°Ü ´ÎÊý µ±Á¬½ÓÉý¼¶·þÎñÆ÷Ê§°Ü10ÒÔÉÏÊ±ÖØÆôµ¥Æ¬»ú */
								if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum >= 10)
								{
									   mcu_reset();//ÖØÆôÉè±¸  ¸´Î»µ¥Æ¬»úzwc
//									/* ÖØÐÂÅäÖÃGSMÄ£¿é */
//									GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
								}							
							}else
							{
									/* Çå³ý GPRS Êý¾Ý·¢ËÍÊ§°Ü´ÎÊý */
									GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum = 0;
							}
							/* ÏûÏ¢Ö´ÐÐÍê³É */
							gsm_que_inf->EVENT_Getarg = TRUE;
							/* ¹Ì¼þÉý¼¶ */
							IAP_UploadLog(&GPRS);
						   #endif
							 #endif
							break;	
						}
/*********************************************************************************/		
#if (GSM_Module_Type==0x00) //SIM800CÄ£¿é		
						/* ·¢ËÍ¶ÌÐÅ */
						case Send_SMS_EVENT:
						{

						 GSM_SMS_Send(&config_tab->Inf_State.Instru_GSM);
							
						   break;
						}
/*********************************************************************************/			
#endif				
						/* ÉÏ´« ÅäÖÃ Êý¾Ý */
						case ConfigService_EVENT:
						{
////////								/* ·¢ËÍÊý¾Ý */
////////								GPRS.ConfigService.State.Result = 
////////								(BOOL)Gsm_gprs_comm(
////////								(char*)GPRS.ConfigService.Para.TXDR,
////////								(GPRS.ConfigService.Para.TXLen),
////////								((char*)GPRS.ConfigService.Para.RXDR),
////////								((&GPRS.ConfigService.Para.RXLen)),
////////								  GSM_GPRS_Config
////////								);
								 #if (AppGPRSComProtocolEnable == 0x01)
                 #if (AppGPRSComProtocol == 0x00)	
								PrintGsmDebug(
								(char*)GPRS.ConfigService.Para.TXDR,
								(GPRS.ConfigService.Para.TXLen),
								((char*)GPRS.ConfigService.Para.RXDR),
								((&GPRS.ConfigService.Para.RXLen)),
								  GSM_GPRS_Config
								);
									#endif
							    #endif
							/* Êý¾Ý·¢ËÍÊ§°Ü */
							if(FALSE == GPRS.ConfigService.State.Result)
							{
								 GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum++;	
								/* ÅÐ¶Ï·¢ËÍÊ§°Ü ´ÎÊý */
								if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum >= 5)
								{
										 
										/* ÖØÐÂÅäÖÃGSMÄ£¿é */
									//	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
								}
							}else
							{
								/* Çå³ý GPRS Êý¾Ý·¢ËÍÊ§°Ü´ÎÊý */
								GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum = 0;
							}
							/* ÏûÏ¢Ö´ÐÐÍê³É */
							gsm_que_inf->EVENT_Getarg = TRUE;	
							ConfigService_RXLog(GPRS,&Instru_Config_Inf_Tab);
							break;
						}
/*************************************** ÏµÍ³Ê±ÖÓÐ£×¼ ******************************************/	
						case System_Clock_Calibration_EVENT:	
						{
							 /* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý */
					  	GPRS_Send_System_Clock_Calibration_Data();	
							break;							
						}
/*********************************************************************************/		
						default :
						/* Î´ÖªÀàÐÍµÄ ÏûÏ¢ */
						__nop();
						break;
					}
				}else
				{
					/* GSMÄ£¿éÅäÖÃ Ê§°Ü */
					/* Ä£¿é Í£Ö¹¹¤×÷ ÐèÒªÖØÐÂÅäÖÃ */
					/* ÉèÖÃ ÏûÏ¢Ö´ÐÐ½á¹û */
					gsm_que_inf->EVENT_Getarg = TRUE;
					switch(gsm_que_inf->EVENT_Cmd)
					{
						//	/* ¸üÐÂÐÅºÅÇ¿¶È */
						case Update_SGL_EVENT:
						__nop();
						break;
/*********************************************************************************/				
						//	/* ¸üÐÂ¸½×Å×´Ì¬ */
						case Update_Link_EVENT:
						__nop();
						break;
/*********************************************************************************/						
						//	/* ÉÏ´«ÎÂÊª¶ÈÊý¾Ý */
						case Updata_Data_EVENT:
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State			 				= FALSE;
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State 	= TRUE;//(µ±Ç°×´Ì¬²»ÐèÒªÉèÖÃ)
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result 						= FALSE;
						GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result 						= FALSE;
						break;
/*********************************************************************************/		
						/* ÏÂÔØ ³ÌÐò¸üÐÂ Êý¾Ý */
						case IAP_EVENT:
						{
							#if (AppGPRSComProtocolEnable == 0x01)
              #if (AppGPRSComProtocol == 0x00)
							/* GSMÄ£¿éÃ»ÓÐ¹¤×÷£¬Í£Ö¹¹Ì¼þÉý¼¶ */
							IAP_StopRun();
							#endif
							#endif
							break;
						}
/*********************************************************************************/		
						/* ·¢ËÍ¶ÌÐÅ */
						case Send_SMS_EVENT:
						/* Î´ÖªÀàÐÍµÄ ÏûÏ¢ */
/* ÅÐ¶ÏÓÃ»§ÊýÁ¿ */
#if SMS_User_Number == 0x01	
						/* GSMÄ£¿é¹¤×÷Òì³££¬²»·¢ËÍ¶ÌÐÅ¡£ */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
#elif SMS_User_Number == 0x02
						/* GSMÄ£¿é¹¤×÷Òì³££¬²»·¢ËÍ¶ÌÐÅ¡£ */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
						/* GSMÄ£¿é¹¤×÷Òì³££¬²»·¢ËÍ¶ÌÐÅ¡£ */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = FALSE;
#elif SMS_User_Number == 0x03
						/* GSMÄ£¿é¹¤×÷Òì³££¬²»·¢ËÍ¶ÌÐÅ¡£ */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = FALSE;
						/* GSMÄ£¿é¹¤×÷Òì³££¬²»·¢ËÍ¶ÌÐÅ¡£ */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = FALSE;
						/* GSMÄ£¿é¹¤×÷Òì³££¬²»·¢ËÍ¶ÌÐÅ¡£ */
						GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = FALSE; 
#endif
						/* ÉèÖÃ ¶ÌÐÅ·¢ËÍÍê³É×´Ì¬ */
						GSM_SMS_SR_Reg.GSM_SMS_Send_State = FALSE;
						break;
/*********************************************************************************/					
						/* ÉÏ´« ÅäÖÃ Êý¾Ý */
						case ConfigService_EVENT:
						{
//			 /* GSMÄ£¿éÃ»ÓÐ¹¤×÷£¬Í£Ö¹ÅäÖÃ */
							ConfigService_StopRun();
							break;
						}
/*************************************** ÏµÍ³Ê±ÖÓÐ£×¼ ******************************************/	
						case System_Clock_Calibration_EVENT:	
						{
							/* GSMÄ£¿éÎ´ÅäÖÃ£¬Òò´ËÕâ´ÎÊ±ÖÓÐ£×¼Ã»ÓÐ±»Ö´ÐÐ£¬»Ö¸´ÏµÍ³Ê±ÖÓÐ£×¼×´Ì¬¼Ä´æÆ÷ */
							RTC_System_Clock_Calibration_State_Reset();
							break;							
						}
/*********************************************************************************/		
						default :
						/* Î´ÖªÀàÐÍµÄ ÏûÏ¢ */
						__nop();
						break;
					}
				}
				break;
			}
		}
//	}else
//	{
//		/* ÐÞ¸Ä GSMÄ£¿é×´Ì¬:Ê§°Ü */
//		GPRS_SR_Reg.GPRS_Work_State = FALSE;
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			/* µç³ØµçÁ¿²»×ã£¬GSMÎÞ·¨Õý³£¹¤×÷¡£ */
//			printf("\rµç³ØµçÁ¿²»×ã£¬GSMÎÞ·¨Õý³£¹¤×÷¡£\r\n");				
//		}
//	}
	/* ÏûÏ¢Ö´ÐÐÍê³É */
	gsm_que_inf->EVENT_Getarg = TRUE;
}
/************************************************************************************************/
/* º¯ÊýÃû	£ºGPRS_Send_Network_Data															*/
/* ÊäÈë		£º×´Ì¬ÐÅÏ¢´òÓ¡±êÖ¾																	*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£ºGPRS ·¢ËÍ ÍøÂçÐòÊý¾Ý			 									 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/01																		*/
/************************************************************************************************/
/**
  * @brief  Description "GPRSÍ¨Ñ¶"
  * @param  wbuf		·¢ËÍ»º³åÇø
  * @param  wlen		·¢ËÍ»º³åÇø×Ö½ÚÊý´óÐ¡
  * @param  rbuf		½ÓÊÕ»º³åÇø
  * @param  rlen		½ÓÊÕ»º³åÇø×Ö½ÚÊý´óÐ¡
  * @retval bool		GPRSÍ¨Ñ¶ÊÇ·ñ³É¹
bool gsm_gprs_comm(char *wbuf, int wlen, char *rbuf, int* rlen)
  */
 void GPRS_Send_Network_Data(void)//ÒªÐÞ¸ÄµÄº¯ÊýZWC
{
/* GPRS ´«ÊäÍøÂçÐòÊý¾ÝÊ§°Ü´ÎÊýÏÞÖÆ */
#define GPRS_COM_Send_Network_Fail_Num_Limit	(3u)
	
	/* ÉèÖÃ GPRS½ÓÊÕ×î´óÊý¾Ý³¤¶È¡£ */
	GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len = GPRS_Rece_Sensor_Data_Buffer_Size;
/* ·¢ËÍÍøÂçÐòÊý¾Ý *//*****************Ïò·þÎñÆ÷·¢ËÍÊý¾Ý*********************************/
//	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = 
//	(BOOL)gsm_gprs_comm(
//	  (char*)GPRS_Send_Sensor_Data_Buffer,
//		(GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
//	  (char*)GPRS_Rece_Sensor_Data_Buffer,
//	  ((int*)(&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)));
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = 
	  (BOOL)Gsm_gprs_comm(
	                      (char*)GPRS_Send_Sensor_Data_Buffer,
		                    (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
	                      (char*)GPRS_Rece_Sensor_Data_Buffer,
	                      ((&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)),
									       GSM_GPRS_Data
								       );
	if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result)//´òÓ¡µ÷ÊÔÐÅÏ¢
	{
				PrintGsmDebug(
	                     (char*)GPRS_Send_Sensor_Data_Buffer,
		                   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len),
	                     (char*)GPRS_Rece_Sensor_Data_Buffer,
	                     ((&GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len)),
									     GSM_GPRS_Data
								     );	
	}		
					 
									 

/******************************END******************************************************/
	/* ÅÐ¶Ï Êý¾Ý·¢ËÍ½á¹û */
	if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == FALSE)
	{
		/* Êý¾Ý·¢ËÍÊ§°Ü */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum++;//Êý¾Ý·¢ËÍÊ§°Ü¼ÆÊý¼Ó1
		
		/* ÅÐ¶Ï·¢ËÍÊ§°Ü ´ÎÊý */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum >= GPRS_COM_Send_Network_Fail_Num_Limit)//Èç¹û·¢ËÍÊý¾ÝÊ§°Ü´ÎÊý´ïµ½3´ÎÔòÖØÐÂ¶ÔGSMÄ£¿é½øÐÐÅäÖÃ
		{
			/* ÖØÐÂÅäÖÃGSMÄ£¿é */
			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
		}
		/* 
			ÉèÖÃ ´«¸ÐÆ÷Êý¾Ý ÕýÔÚ·¢ËÍ ×´Ì¬:ÖØÐÂ·¢ËÍ´«¸ÐÆ÷µ±Ç°Êý¾Ý
			(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE) 
		*/;
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;//(µ±Ç°×´Ì¬²»ÐèÒªÉèÖÃ)
	}else//Êý¾Ý·¢ËÍ³É¹¦
	{
		 Log_printf("ÏòÊý¾Ý·þÎñÆ÷·¢ËÍ´«¸ÐÆ÷Êý¾Ý³É¹¦£¡£¡£¡\r\n");	
		
		/* ½âÎö ·þÎñÆ÷·µ»ØµÄÊý¾Ý£¬ÅÐ¶Ï±¾´ÎGPRSÍ¨ÐÅ×´Ì¬ */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_Network_Data();
		/* ÅÐ¶Ï ±¾´ÎGPRSÍ¨ÐÅ ½á¹û */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE)//Èç¹û±¾´ÎÊý¾ÝÍ¨ÐÅÕý³£
		{
#if (AppGYForwardEnable == 0x01)			
			/* ÅÐ¶Ï´´½¨¹úÒ©Êý¾Ý±êÖ¾ */
			if(TRUE == AppComProtocolGY.SR.CreateDataFlag)
			{
				/* Êý¾Ý´´½¨³É¹¦£¬¿ªÊ¼·¢ËÍÊý¾Ý. */
				/* ·¢ËÍÊý¾Ý×´Ì¬ */
				BOOL gSendState;
				/* ·¢ËÍÊý¾Ý³¤¶È */
				int gSendLen;
				/* ½ÓÊÕÊý¾Ý³¤¶È */
				int gReceLen = (128ul);


				/* ¼ÆËã·¢ËÍÊý¾Ý³¤¶È */
				gSendLen = strlen((char*)AppComProtocolGY.DR.TXDR);

				/* ÉèÖÃ¹úÒ©ÉÏ´«IP/¶Ë¿Ú */
				GSM_Config_Data_Network((void*)&AppComProtocolGY.CR.NetworkConnection);
				
				/* ·¢ËÍ¹úÒ©Êý¾Ý */
				gSendState = (BOOL)gsm_gprs_comm(
					(char*)AppComProtocolGY.DR.TXDR,
					(gSendLen),
					(char*)AppComProtocolGY.DR.RXDR,
					(&gReceLen));
					
				/* ÅÐ¶ÏÊý¾Ý·¢ËÍ×´Ì¬ */
				if(TRUE == gSendState)
				{
					/* Êý¾Ý·¢ËÍ³É¹¦ */
					__nop();
				}
				
				/* ÉèÖÃÄ¬ÈÏÉÏ´«IP/¶Ë¿Ú */
				GSM_Config_Data_Network((void*)&GSM_Tcp_Conn[0]);	
			}
#endif
			
#if (ComProtocol_WZ_Enable == 0x01)			
			/* ÅÐ¶Ï´´½¨ÍòÖÇÊý¾Ý±êÖ¾ */
			if(TRUE == ComProtocolWZDR.SR.CreateDataFlag)
			{
				/* Êý¾Ý´´½¨³É¹¦£¬¿ªÊ¼·¢ËÍÊý¾Ý. */
				/* ·¢ËÍÊý¾Ý×´Ì¬ */
				BOOL gSendState;
				/* ·¢ËÍÊý¾Ý³¤¶È */
				int gSendLen;
				/* ½ÓÊÕÊý¾Ý³¤¶È */
				int gReceLen = (128ul);


				/* ¼ÆËã·¢ËÍÊý¾Ý³¤¶È */
				gSendLen = strlen((char*)ComProtocolWZDR.DR.TXDR);

				/* ÉèÖÃ¹úÒ©ÉÏ´«IP/¶Ë¿Ú */
				GSM_Config_Data_Network((void*)&ComProtocolWZDR.CR.NetworkConnection);
				
				/* ·¢ËÍ¹úÒ©Êý¾Ý */
				gSendState = (BOOL)gsm_gprs_comm(
					(char*)ComProtocolWZDR.DR.TXDR,
					(gSendLen),
					(char*)ComProtocolWZDR.DR.RXDR,
					(&gReceLen));
					
				/* ÅÐ¶ÏÊý¾Ý·¢ËÍ×´Ì¬ */
				if(TRUE == gSendState)
				{
					/* Êý¾Ý·¢ËÍ³É¹¦ */
					__nop();
				}
				
				/* ÉèÖÃÄ¬ÈÏÉÏ´«IP/¶Ë¿Ú */
				GSM_Config_Data_Network((void*)&GPRS_COM_Reg[0]);	
			}
#endif	
			
			/* 
				ÉèÖÃ ´«¸ÐÆ÷Êý¾Ý ÕýÔÚ·¢ËÍ ×´Ì¬:·¢ËÍÐÂµÄ´«¸ÐÆ÷µ±Ç°Êý¾Ý
				(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE)
			*/
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = FALSE;
			
			/* ÅÐ¶Ï ·¢ËÍÊ§°Ü ´ÎÊý */
			if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum)
			{
				/* Çå³ý GPRS Êý¾Ý·¢ËÍÊ§°Ü´ÎÊý */
				GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum = 0;
			}
		}else
		{
			/* 
				ÉèÖÃ ´«¸ÐÆ÷Êý¾Ý ÕýÔÚ·¢ËÍ ×´Ì¬:ÖØÐÂ·¢ËÍ´«¸ÐÆ÷µ±Ç°Êý¾Ý
				(GPRS_COM_Send_State=FALSE,GPRS_COM_Network_Waiting_State=TRUE) 
			*/
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;//(µ±Ç°×´Ì¬²»ÐèÒªÉèÖÃ)
			/* Êý¾Ý½âÎöÊ§°Ü */
			//ÔÚ²¿±êÐ´Ò»ÏÂ²»´æÔÚÕâ¸öÇé¿ö
			//ÔÚHTTPÐ­Òé£¬ÔÆ·þÎñÆ÷Òì³£»áµ¼ÖÂ½ÓÊÕµ½"9999"(·þÎñÆ÷Òì³£Ö¸Áî)
			//´¦Àí°ì·¨£ºµ±·¢ÏÖÕâ¸öÊÂ¼þºóÑÓÊ±1·ÖÖÓ·¢ËÍÊý¾Ý£¬¼õÉÙÁ÷Á¿£¬¹ÀËãÔÚ40M/ÔÂ¡£
			/* ÉèÖÃ½âÎöÊ§°Ü±êÖ¾ */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState = FALSE;
		}
	}
	/* 
		ÉèÖÃ ´«¸ÐÆ÷Êý¾Ý ´«¸ÐÆ÷Êý¾Ý ÕýÔÚ·¢ËÍ ×´Ì¬:Êý¾Ý·¢ËÍÍê³É
	*/
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = FALSE;
#undef GPRS_COM_Send_Network_Fail_Num_Limit
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: GPRS_Get_Save_Sensor_Data
*	¹¦ÄÜËµÃ÷: »ñµÃ´æ´¢µÄ´«¸ÐÆ÷Êý¾Ý
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ Instru_Config_Inf_Tab  Instru_Fix_Inf_State_Tab
*********************************************************************************************************
*/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
static BOOL GPRS_Get_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
/* Ä¬ÈÏ:ÎÞ:0x00 */
#define GPRS_Sensor_Null				(0x00)		
/* ´«¸ÐÆ÷ÀàÐÍ:ÎÂÊª¶È´«¸ÐÆ÷:0x01 */
#define GPRS_Sensor_Board_On			(0x01)	
/* ´«¸ÐÆ÷ÀàÐÍ:µ¥ÎÂ¿¨Æ¬:0x02 */
#define GPRS_Sensor_RF_Tmp				(0x02)	
/* ´«¸ÐÆ÷ÀàÐÍ:ÎÂÊª¶È¿¨Æ¬:0x03 */	
#define GPRS_Sensor_RF_Hum				(0x03)
/* ·¢ËÍÊý¾Ý¼Ä´æÆ÷¿Õ¼ä´óÐ¡ */
#define GPRS_Send_Reg_Size				(sizeof(GSM_GPRS_Send_Struct))
	
#if (AppGPRSComProtocol == 0x03)//ÊÓ´°Ð­Òé
/* ·¢ËÍÊý¾Ý¼Ä´æÆ÷ Êý¾ÝÄÚÈÝµÄ¹Ì¶¨³¤¶È */
#define GPRS_Send_Data_Content_Fix_Len	(20u)//zwc
#endif

#if (AppGPRSComProtocol == 0x00)//²¿±êÐ­Òé
/* ·¢ËÍÊý¾Ý¼Ä´æÆ÷ Êý¾ÝÄÚÈÝµÄ¹Ì¶¨³¤¶È */
#define GPRS_Send_Data_Content_Fix_Len	(16u)//zwc
#endif
/* ·¢ËÍÊý¾Ý¼Ä´æÆ÷ ¹Ì¶¨³¤¶È */
#define GPRS_Send_Data_Fix_Len			(22u)
/* Ò»Ö¡Êý¾ÝÔÚ´æ´¢µÄ³¤¶È */
#if ((Sensor_Tmp_Num+Sensor_Hum_Num+Sensor_CarbonDioxide_Num+Sensor_DoorContact_Num+Sensor_Lon_Num+Sensor_Lat_Num+Sensor_Spd_Num+Sensor_PT1000_Num) > 0)
#define Sensors_Data_Store_Size		(GPRS_Send_Data_Content_Fix_Len+\
									(Sensor_Tmp_Num+Sensor_CarbonDioxide_Num+Sensor_DoorContact_Num+Sensor_Hum_Num+Sensor_Lon_Num+Sensor_Lat_Num+Sensor_Spd_Num+Sensor_PT1000_Num)*sizeof(float)+\
									(Sensor_RF_Single_Tmp_Num)*(GPRS_Send_Data_Content_Fix_Len+sizeof(float))+\
									(Sensor_RF_Humiture_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									(Sensor_GW_NJSY_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									(Sensor_GW_HZZH_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float)))
#else
#define Sensors_Data_Store_Size		((Sensor_RF_Single_Tmp_Num)*(GPRS_Send_Data_Content_Fix_Len+sizeof(float))+\
									 (Sensor_RF_Humiture_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									 (Sensor_GW_NJSY_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float))+\
									 (Sensor_GW_HZZH_Num)*(GPRS_Send_Data_Content_Fix_Len+2*sizeof(float)))
#endif			

#if (AppGYForwardEnable == 0x00)
/* µ¥´Î·¢ËÍÎÂÊª¶ÈÊý¾Ý×î´ó´«ÊäµÄÊý¾ÝÖ¡ÊýÁ¿ */
#define GPRS_Send_Data_Frames_Max_Num	((u8)((GPRS_Send_Reg_Size-GPRS_Send_Data_Fix_Len)/Sensors_Data_Store_Size))
#elif (AppGYForwardEnable == 0x01)
#define GPRS_Send_Data_Frames_Max_Num	(1u)
#endif
									 /* Ò»Ö¡Êý¾Ý°üº¬µÄÊý¾ÝÌõÊý(°üº¬¼¸¸öSNºÅÊý¾Ý) */
#define Sensors_One_Frames_Include_Data_Num		((((Sensor_Tmp_Num>0)||\
													  (Sensor_DoorContact_Num>0) ||\
													  (Sensor_CarbonDioxide_Num>0) ||\
												    (Sensor_Hum_Num>0)||\
												    (Sensor_PT1000_Num>0))?1:0)+\
													   Sensor_RF_Single_Tmp_Num+\
												     Sensor_RF_Humiture_Num+\
													   Sensor_GW_NJSY_Num+\
													   Sensor_GW_HZZH_Num)
/* Ã»ÓÐ²éµ½Æ¥ÅäµÄÍ¨µÀ */
#define Sensors_CH_Null (0xFF)
	/* ¶Á Fram Êý¾ÝµÄ»º´æÖ¸Õë */
	Save_Sensor_Data_Struct *read_sensor_data_p;
	/* ·¢ËÍÊý¾Ý¼Ä´æÆ÷ Êý¾ÝÄÚÈÝÖ¸Õë */
	u8 * gprs_data_content_p = &GPRS_Send_Reg.GPRS_IR_FIX.Data_P;
#if ((Wireless_RF_Card_Config_Enable == 0x01)||\
	 (Gateway_NJSY_Enable == 0x01)||\
	 (Gateway_HZZH_Enable == 0x01))
	/* ÎÂÊª¶ÈÊý¾Ý */
	float sensor_data;
	#if ((Sensor_RF_Humiture_Num > 0)||\
		 (Sensor_GW_NJSY_Num > 0)||\
		 (Sensor_GW_HZZH_Num > 0))
	/* ÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ±àºÅ */
	u8 rf_hum_no;					
	#endif							
#endif								
	/* µ±Ç°´«ÊäµÄÊý¾ÝÖ¡Êý */
	u8 data_frames_num;
	/* ¼ÆÊýÆ÷ */
	u16 num;

	/* ÅÐ¶Ï ÊÇ·ñÓÐÀúÊ·Êý¾Ý */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM < Save_Sensor_Data_Len)
	{
		/* Ã»ÓÐÀúÊ·Êý¾Ý */
		return FALSE;
	}
									 							
	//1¡¢¸´Î»·¢ËÍÊý¾Ý¼Ä´æÆ÷ÄÚÈÝ
	/* ÖØÐÂÉèÖÃ ¼Ä´æÆ÷ Êý¾Ý */
	GPRS_Send_Reg = GPRS_Send_Reg_Fix;
	/* ÉèÖÃ SNºÅ */
	memcpy(GPRS_Send_Reg.GPRS_IR_FIX.SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),ENV_SN_NO_SIZE);

	//2¡¢»ñÈ¡Êý¾ÝÄÚÈÝ							 
	for(data_frames_num = 0;
		 (fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)&&
		 (data_frames_num < GPRS_Send_Data_Frames_Max_Num);
		 data_frames_num++
	   )
	{
		/* »ñÈ¡µ±Ç°µÄ´«¸ÐÆ÷Í¨µÀÊý¾Ý */
		/* ¸ù¾Ýµ±Ç°Íâ²¿´æ´¢Æ÷£¨W25Q64£©µÄÊý¾Ý´æ´¢Ö¸ÕëÀ´·¢ËÍÊý¾Ý */
		if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)
		{
			/* FlashÓÐÀúÊ·Êý¾Ý£¬¾Í¶ÁÈ¡Êý¾Ý£¬ÏÈ·¢ÀúÊ·Êý¾Ý */
			read_sensor_data_p = (Save_Sensor_Data_Struct*)Flash_Read_Sensor_Data(
								  fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size,
								  Save_Sensor_Data_Len);

			//ÉèÖÃ Flash ¶ÁÖ¸Õë
			/* Ôø¼Ó ¶ÁÈ¡µÄÊý¾Ý³¤¶È */
			fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer 		  += Save_Sensor_Data_Len; 
			/* ´æ´¢Êý¾ÝÁ¿ ¼õ*/
			fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	-= Save_Sensor_Data_Len;
			
#if (AppGYForwardEnable == 0x01)
			/* ´´½¨¹úÒ©Ì½Í·Êý¾Ý */
			AppComProtocolGY.SR.CreateDataFlag = ComProtGY_CreateData(read_sensor_data_p);
#endif	
#if (ComProtocol_WZ_Enable == 0x01)
			/* ´´½¨¹úÒ©Ì½Í·Êý¾Ý */
			ComProtocolWZDR.SR.CreateDataFlag = ComProtWZ_CreateData(read_sensor_data_p);
#endif			
			
		
		}
		//I¡¢ËÑË÷´«¸ÐÆ÷Í¨µÀ£¨ÕâÀïÓÐÒ»µãÒªËµÃ÷£¬ÏÖÔÚ°åÔØµÄ´«¸ÐÆ÷×î¶àÖ»ÓÐ°Ë¸ö£¬Òò´ËÎÒÃÇËÑË÷µÄ·¶Î§Ò²¾ÍÔÚÇ°Ãæ8¸öÍ¨µÀÀï¡££©
#if ((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
		 (Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH1_Config_Type == Sensor_Hum) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
 	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH1_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH2_Config_Type == Sensor_Tmp) ||\
		 (Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH2_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH3_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH3_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH4_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH4_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH4_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH5_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH5_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH5_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH6_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH6_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH6_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH7_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
	   (Sensor_CH7_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH7_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH8_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH8_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH8_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH9_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH9_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH9_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH10_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH10_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH10_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH11_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH11_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH11_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH12_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH12_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH12_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH13_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH13_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH13_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH14_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH14_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH14_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH15_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH15_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH15_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH16_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH16_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH16_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH17_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH17_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH17_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH18_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH18_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH18_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH19_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH19_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH19_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH20_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH20_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH20_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH21_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH21_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH21_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH22_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH22_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH22_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH23_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH23_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH23_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH24_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH24_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH24_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH25_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH25_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH25_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH26_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH26_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH26_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH27_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH27_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH27_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH28_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH28_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH28_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH29_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH29_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH29_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH30_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH30_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH30_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH31_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH31_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH31_Config_Type == Sensor_PT1000) ||\
	   (Sensor_CH32_Config_Type == Sensor_Tmp) ||\
	   (Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH32_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH32_Config_Type == Sensor_PT1000))
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		/* ÒÇÆ÷ÓÐ´«¸ÐÆ÷ÅäÖÃ£¬ÏÈÉèÖÃÊý¾ÝÄÚÈÝ */
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,(u8*)(BLCOK_SYS_PARAM_START+ENV_SN_NO_START),sizeof(u32));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		//ÐÅºÅÇ¿¶È
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved1 = (u8)GPRS_SR_Reg.GPRS_Sign_Stre;
/******************************zwcÊÀ´°Ð­ÒéÐÞ¸Ä*************************************/
	     #if (AppGPRSComProtocol == 0x03)
		   {
				 //Ô¤Áô×Ö½Úzwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved2 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved3 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved4 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved5 = 0x00;//zwc
					GPRS_Data_Content_Reg.GPRS_DR_FIX.Reserved6 = 0x00;//zwc
	     }	
			 #endif
/*********************************************************************************/	
/* ÕâÀïµÄÒÇÆ÷×´Ì¬¶¼ÊÇÏàÍ¬µÄ£¬ÕâÑù¶¼¸ù¾ÝÍ¨µÀ1-8µÄË³Ðò¼ì²éÀ´È·¶¨Ê¹ÓÃÄÇ¸öÍ¨µÀµÄµçÔ´×´Ì¬ºÍµç³ØµçÁ¿Êý¾Ý */		
#if	((Sensor_CH1_Config_Type == Sensor_Tmp) ||\
     (Sensor_CH1_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH1_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH1_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH1_Config_Type == Sensor_PT1000))
		//µç³ØµçÁ¿¸ñÊý
			if(AppGPRSComProtocol == 0x03)//ºÓ±±ÊÓ´°Ð­Òé
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//ÆäËûÐ­Òé(Èç:²¿±êÐ­ÒéµÈ)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}				
#elif ((Sensor_CH2_Config_Type == Sensor_Tmp) ||\
(Sensor_CH2_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH2_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH2_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH2_Config_Type == Sensor_PT1000))
				//µç³ØµçÁ¿¸ñÊý
			if(AppGPRSComProtocol == 0x03)//ºÓ±±ÊÓ´°Ð­Òé
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//ÆäËûÐ­Òé(Èç:²¿±êÐ­ÒéµÈ)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}
		
		
		
#elif ((Sensor_CH3_Config_Type == Sensor_Tmp) ||\
(Sensor_CH3_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH3_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH3_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH3_Config_Type == Sensor_PT1000))
				//µç³ØµçÁ¿¸ñÊý
			if(AppGPRSComProtocol == 0x03)//ºÓ±±ÊÓ´°Ð­Òé
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//ÆäËûÐ­Òé(Èç:²¿±êÐ­ÒéµÈ)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4;
			   
			}	
#elif ((Sensor_CH4_Config_Type == Sensor_Tmp) ||\
(Sensor_CH4_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH4_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH4_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH4_Config_Type == Sensor_PT1000))
				//µç³ØµçÁ¿¸ñÊý
			if(AppGPRSComProtocol == 0x03)//ºÓ±±ÊÓ´°Ð­Òé
			{		if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 25;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 50;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 75;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 100;
			}	
      else//ÆäËûÐ­Òé(Èç:²¿±êÐ­ÒéµÈ)
			{
			    if(config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<10)
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 0;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=10)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<30))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 1;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=30)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<50))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 2;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=50)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<70))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 3;
						else if((config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State>=70)&&
								 (config_tab->Inf_State.Instru_Power.Inf_State.BT_Quantity_State<100))
							GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = 4; 
			}
			
			
#elif ((Sensor_CH5_Config_Type == Sensor_Tmp) ||\
(Sensor_CH5_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH5_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH5_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH5_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
#elif ((Sensor_CH6_Config_Type == Sensor_Tmp) ||\
(Sensor_CH6_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH6_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH6_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH6_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
#elif ((Sensor_CH7_Config_Type == Sensor_Tmp) ||\
			(Sensor_CH7_Config_Type == Sensor_CarbonDioxide) ||\
			(Sensor_CH7_Config_Type == Sensor_DoorContact) ||\
	   (Sensor_CH7_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH7_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH7_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
#elif ((Sensor_CH8_Config_Type == Sensor_Tmp) ||\
(Sensor_CH8_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH8_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH8_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH8_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;


#elif ((Sensor_CH9_Config_Type == Sensor_Tmp) ||\
(Sensor_CH9_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH9_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH9_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH9_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
#elif ((Sensor_CH10_Config_Type == Sensor_Tmp) ||\
(Sensor_CH10_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH10_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH10_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH10_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
#elif ((Sensor_CH11_Config_Type == Sensor_Tmp) ||\
(Sensor_CH11_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH11_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH11_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH11_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
#elif ((Sensor_CH12_Config_Type == Sensor_Tmp) ||\
(Sensor_CH12_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH12_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH12_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH12_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
#elif ((Sensor_CH13_Config_Type == Sensor_Tmp) ||\
(Sensor_CH13_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH13_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH13_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH13_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
#elif ((Sensor_CH14_Config_Type == Sensor_Tmp) ||\
(Sensor_CH14_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH14_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH14_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH14_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
#elif ((Sensor_CH15_Config_Type == Sensor_Tmp) ||\
(Sensor_CH15_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH15_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH15_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH15_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
#elif ((Sensor_CH16_Config_Type == Sensor_Tmp) ||\
(Sensor_CH16_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH16_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH16_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH16_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;

#elif ((Sensor_CH17_Config_Type == Sensor_Tmp) ||\
(Sensor_CH17_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH17_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH17_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH17_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
#elif ((Sensor_CH18_Config_Type == Sensor_Tmp) ||\
(Sensor_CH18_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH18_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH18_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH18_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
#elif ((Sensor_CH19_Config_Type == Sensor_Tmp) ||\
(Sensor_CH19_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH19_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH19_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH19_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
#elif ((Sensor_CH20_Config_Type == Sensor_Tmp) ||\
(Sensor_CH20_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH20_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH20_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH20_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
#elif ((Sensor_CH21_Config_Type == Sensor_Tmp) ||\
(Sensor_CH21_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH21_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH21_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH21_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
#elif ((Sensor_CH22_Config_Type == Sensor_Tmp) ||\
(Sensor_CH22_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH22_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH22_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH22_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
#elif ((Sensor_CH23_Config_Type == Sensor_Tmp) ||\
(Sensor_CH23_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH23_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH23_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH23_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
#elif ((Sensor_CH24_Config_Type == Sensor_Tmp) ||\
(Sensor_CH24_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH24_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH24_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH24_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
#elif ((Sensor_CH25_Config_Type == Sensor_Tmp) ||\
(Sensor_CH25_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH25_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH25_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH25_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
#elif ((Sensor_CH26_Config_Type == Sensor_Tmp) ||\
(Sensor_CH26_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH26_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH26_Config_Type == Sensor_GPS_POS_Spd))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
#elif ((Sensor_CH27_Config_Type == Sensor_Tmp) ||\
(Sensor_CH27_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH27_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH27_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH27_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
#elif ((Sensor_CH28_Config_Type == Sensor_Tmp) ||\
(Sensor_CH28_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH28_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH28_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH28_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
#elif ((Sensor_CH29_Config_Type == Sensor_Tmp) ||\
(Sensor_CH29_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH29_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH29_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH29_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
#elif ((Sensor_CH30_Config_Type == Sensor_Tmp) ||\
(Sensor_CH30_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH30_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH30_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH30_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
#elif ((Sensor_CH31_Config_Type == Sensor_Tmp) ||\
(Sensor_CH31_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH31_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH31_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH31_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
#elif ((Sensor_CH32_Config_Type == Sensor_Tmp) ||\
(Sensor_CH32_Config_Type == Sensor_CarbonDioxide) ||\
	   (Sensor_CH32_Config_Type == Sensor_Hum) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lon) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Lat) ||\
	   (Sensor_CH32_Config_Type == Sensor_GPS_POS_Spd) ||\
	   (Sensor_CH32_Config_Type == Sensor_PT1000))
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;

#endif

		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_Board_On;
		//d¡¢ÉèÖÃÊý¾Ý ºÍ ÉèÖÃÍ¨µÀÊý
		GPRS_GET_Save_BSP_Sensor_Data(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//e¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//f¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#endif
		//II¡¢ËÑË÷µ¥ÎÂ¿¨Æ¬Í¨µÀ£¨¿¨Æ¬Í¨µÀ¸ù¾ÝÉèÖÃµÄ×î´ó´«¸ÐÆ÷ÅäÖÃÊýÁ¿À´ËÑË÷£©
#if (Sensor_CH1_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO, config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH1_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH1_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ */
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///µçÁ¿Õý³£/µçÁ¿²»×ãÏÖÔÚÖ»ÓÐÕâÁ½ÖÖ×´Ì¬
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH1.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_1]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH1.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
#elif (Sensor_CH1_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#elif (Sensor_CH1_Config_Type == Sensor_GW_HZZH_Hum)
		__nop();	
#else
		__nop();
#endif		



#if (Sensor_CH2_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH2_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH2_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH2.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_2]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH2.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH2_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif


#if (Sensor_CH3_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH3_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH3_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH3.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_3]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH3.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH3_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif
		
		
#if (Sensor_CH4_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();	
#elif ((Sensor_CH4_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH4_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH4.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_4]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH4.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH4_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH5_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH5_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH5_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH5.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_5]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH5.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH5_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();	
#else
		__nop();
#endif	
		
		
#if (Sensor_CH6_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH6_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH6_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH6.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_6]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH6.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH6_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	

		
#if (Sensor_CH7_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH7_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH7_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH7.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_7]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH7.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH7_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH8_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH8_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH8_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH8.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_8]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH8.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH8_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif


#if (Sensor_CH9_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH9_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH9_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH9.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_9]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH9.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH9_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH10_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH10_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH10_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH10.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_10]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH10.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH10_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH11_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH11_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH11_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH11.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_11]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH11.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH11_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH12_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH12_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH12_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH12.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_12]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH12.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH12_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	

		
#if (Sensor_CH13_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH13_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH13_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH13.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_13]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH13.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH13_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH14_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH14_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH14_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH14.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_14]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH14.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH14_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH15_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH15_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH15_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH15.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_15]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH15.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH15_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH16_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH16_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH16_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH16.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_16]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH16.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH16_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH17_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH17_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH17_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH17.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_17]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH17.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH17_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH18_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH18_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH18_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH18.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_18]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH18.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH18_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif		


#if (Sensor_CH19_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH19_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH19_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH19.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_19]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH19.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH19_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


#if (Sensor_CH20_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH20_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH20_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH20.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_20]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH20.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH20_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH21_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH21_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH21_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH21.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_21]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH21.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH21_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


#if (Sensor_CH22_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH22_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH22_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH22.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_22]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH22.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH22_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH23_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH23_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH23_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH23.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_23]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH23.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH23_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif		


#if (Sensor_CH24_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH24_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH24_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH24.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_24]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH24.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH24_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH25_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH25_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH25_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH25.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_25]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH25.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH25_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH26_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH26_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH26_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH26.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_26]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH26.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH26_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH27_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH27_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH27_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH27.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_27]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH27.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH27_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH28_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH28_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH28_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH28.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_28]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH28.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH28_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH29_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH29_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH29_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH29.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_29]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH29.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH29_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	


#if (Sensor_CH30_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH30_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH30_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH30.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_30]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH30.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH30_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH31_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH31_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH31_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH31.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_31]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH31.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH31_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		

#if (Sensor_CH32_Config_Type == Sensor_RF_Card_Single_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³Éµ¥ÎÂ¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Tmp;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x01;
		//e¡¢ÉèÖÃÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		//f¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//g¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Tmp)
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = Save_Tmp_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = Save_Hum_To_Float(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_RF_Card_Humiture_Hum)
		__nop();
#elif ((Sensor_CH32_Config_Type == Sensor_GW_NJSY_Tmp)||\
	   (Sensor_CH32_Config_Type == Sensor_GW_HZZH_Tmp))
		//Í¨µÀÒ»±»ÅäÖÃ³ÉÎÂÊª¿¨Æ¬ÀàÐÍ
		/* ¸´Î»Êý¾ÝÄÚÈÝ¼Ä´æÆ÷ */
		GPRS_Data_Content_Reg = GPRS_Data_Content_Reg_Fix;
		//a¡¢ÉèÖÃ SNºÅ
		/* »ñÈ¡ SNºÅ */
		memcpy(GPRS_Data_Content_Reg.GPRS_DR_FIX.SN_NO,config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO,
			   sizeof(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_SN_NO));
		//b¡¢ÉèÖÃÒÇÆ÷×´Ì¬
		/* »ñÈ¡µçÔ´×´Ì¬ *///¿¨Æ¬Ã»ÓÐÎ´½ÓµçÔ´£¬Õâ¸ö×´Ì¬¹Ì¶¨ÎªÕý³££¨1£©
		GPRS_Data_Content_Reg.GPRS_DR_FIX.ET_State = (u8)config_tab->Inf_State.Instru_Power.Inf_State.ET_Link_State;
		/* »ñÈ¡µç³ØµçÁ¿±¨¾¯×´Ì¬ *///Õâ¸ö×´Ì¬ÐÅÏ¢ÔÝÊ±Ã»ÓÐ¶¨Òå£¬ÏÈÓÃÕý³£ÐÅÏ¢¹Ì¶¨ºóÃæÌí¼Ó¡£
		GPRS_Data_Content_Reg.GPRS_DR_FIX.BT_Charge_Alarm_State = (u8)fix_tab->Inf_State.Instru_Sensor.Inf.CH32.Sensor_Power_State;
		//b¡¢ÉèÖÃÊ±¼ä´Á
		/* »ñÈ¡ Ê±¼ä´Á */
		GPRS_Timestamp_Hex_To_BCD_Code(&GPRS_Data_Content_Reg,read_sensor_data_p);
		//c¡¢ÉèÖÃÍ¨µÀÀàÐÍ
		GPRS_Data_Content_Reg.GPRS_DR_FIX.Sensor_Type = GPRS_Sensor_RF_Hum;
		//d¡¢ÉèÖÃÍ¨µÀÊý
		GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num = 0x02;
		//e¡¢ÉèÖÃÎÂ¶ÈÊý¾Ý
		sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[Sensors_CH_32]);
		/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
		*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)) = htonl(*((u32*)(&sensor_data)));
		/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
		rf_hum_no = GPRS_GET_RF_Humiture_Hum_Data(config_tab->Inf_State.Instru_Sensor.Inf_State.CH32.SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
		/* ÅÐ¶ÏÎÂÊª¶È¿¨Æ¬ÊÇ·ñÓÐÆ¥ÅäµÄÊª¶ÈÍ¨µÀ */
		if(rf_hum_no != Sensors_CH_Null)
		{
			//f¡¢ÉèÖÃÊª¶ÈÊý¾Ý
			sensor_data = SaveGw_TimeoutParaCheck(read_sensor_data_p->GPRS_IR.Sensor_Data[rf_hum_no]);	
			/* ÕâÀï½øÐÐ´óÐ¡¶ËÊý¾Ý×ª»»£¬ËäÈ»½á¹ûÓÐµãÄÑ¿´¡£ */
			*((u32*)(&GPRS_Data_Content_Reg.GPRS_DR_FIX.Data_P)+1) = htonl(*((u32*)(&sensor_data)));			
		}
		//g¡¢½«ÉèÖÃµÄÊý¾ÝÄÚÈÝµ¼Èëµ½ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷ÖÐ
		memcpy(gprs_data_content_p,GPRS_Data_Content_Reg.GSM_GPRS_Data_Content_Buffer,
		      (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num));
		//h¡¢½«Ö¸ÕëÖ¸ÏòÊý¾ÝÄ©Î²
		gprs_data_content_p += (GPRS_Send_Data_Content_Fix_Len+sizeof(float)*GPRS_Data_Content_Reg.GPRS_DR_FIX.CH_Num);
		
#elif (Sensor_CH32_Config_Type == Sensor_GW_NJSY_Hum)
		__nop();
#else
		__nop();
#endif	
		


	}
	/* ¼ÆËãÏûÏ¢Ìå³¤¶È *///Êý¾ÝÖ¡Êý*Ò»×éÊý¾ÝµÄ³¤¶È+ÏûÏ¢ÀàÐÍ£¨1×Ö½Ú£©+ Êý¾ÝÀàÐÍ£¨1×Ö½Ú£©+ Êý¾ÝÖ¡Êý£¨1×Ö½Ú£©
	GPRS_Send_Reg.GPRS_IR_FIX.Attribute_HSB_Length = (((u16)Sensors_Data_Store_Size*data_frames_num+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num))/0X100);
	GPRS_Send_Reg.GPRS_IR_FIX.Attribute_LSB_Length = (((u16)Sensors_Data_Store_Size*data_frames_num+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Type)+
															sizeof(GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num))%0X100);
	/* ¼ÆËãÊý¾ÝÖ¡Êý */
	GPRS_Send_Reg.GPRS_IR_FIX.Data_Frame_Num = data_frames_num*Sensors_One_Frames_Include_Data_Num;

/**********************************¼ÆËãÐ£ÑéÂë**************************************************/															
	/* ¸ù¾Ý ÏûÏ¢Í·ºÍÏûÏ¢Ìå µÄÊý¾Ý »ñµÃ Ð£ÑéÂë */
	for(num = 1,*(gprs_data_content_p) = 0;num < ((u16)(gprs_data_content_p - GPRS_Send_Reg.GSM_GPRS_Send_Buffer)); num++)
	{
		/* ´Ó ÏûÏ¢Í· µ½ ÏûÏ¢Ìå ×îºóÒ»¸ö Êý¾Ý Òì»ò */
		*(gprs_data_content_p) ^= GPRS_Send_Reg.GSM_GPRS_Send_Buffer[num];
	}
/**************************************END*****************************************************/	

	/* Ö´ÐÐµ½ÕâÀï ËµÃ÷ ÉèÖÃ³É¹¦ */
	return TRUE;
	
#undef GPRS_Sensor_Null
#undef GPRS_Sensor_Board_On
#undef GPRS_Sensor_RF_Tmp
#undef GPRS_Sensor_RF_Hum
#undef GPRS_Send_Reg_Size
#undef GPRS_Send_Data_Content_Fix_Len
#undef GPRS_Send_Data_Fix_Len
#undef Sensors_Data_Store_Size	
#undef GPRS_Send_Data_Frames_Max_Num
#undef Sensors_One_Frames_Include_Data_Num	
#undef Sensors_CH_Null
}

#elif (AppGPRSComProtocol == 0x01)//HttpÐ­Òé´«ÊäÎÂÊª¶ÈÊý¾Ý
static BOOL GPRS_Get_Save_Sensor_Data(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	/* ¶ÁÊý¾ÝµÄ»º´æÖ¸Õë */
	Save_Sensor_Data_Struct            *sensordata_p;
	/* ÁÙÊ±Ê±¼ä»º´æ */
	Times                                       time;
	/* ¼ÆÊýÆ÷ */
	unsigned char                                num;
	/* Ì½Í·±àºÅ */
	unsigned char                           probeNum;
	/* Íø¹Ø±àºÅÖ¸Õë */
	SERIAL_NUMBER* gatewayIdNoP         = &config_tab->Inf_State.Instru_System.Inf.Serial_Number;
	/* ´«¸ÐÆ÷Í¨µÀÖ¸Õë */
	INSTRU_SENSOR_CHANNEL* sensorchP    = &config_tab->Inf_State.Instru_Sensor.Inf_State.CH1;


	/* Çå³ýÊý¾Ý½Ó¿Ú²ÎÊý */
	memset((char*)&HTTPPara.ApiPara,0x00,sizeof(HTTPPara.ApiPara));
	
	
	/* ÅÐ¶Ï ÊÇ·ñÓÐÀúÊ·Êý¾Ý */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM < Save_Sensor_Data_Len)
	{
		/* Ã»ÓÐÀúÊ·Êý¾Ý */
		return FALSE;
	}
	
	/* ÉèÖÃÍø¹Ø±àºÅ */
	sprintf(HTTPPara.ApiPara.GatewayIdNo,"%04X%05d",
	((((*(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[0])) & 0x00ff) << 8)),
	((((*(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0xff00) >> 8)|
	((( *(u16*)(&gatewayIdNoP->Serial_Number[2])) & 0x00ff) << 8)));
	
	
	/* »ñÈ¡µ±Ç°µÄ´«¸ÐÆ÷Í¨µÀÊý¾Ý */
	/* ¸ù¾Ýµ±Ç°Íâ²¿´æ´¢Æ÷£¨W25Q64£©µÄÊý¾Ý´æ´¢Ö¸ÕëÀ´·¢ËÍÊý¾Ý */
	if(fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM >= Save_Sensor_Data_Len)
	{
		/* FlashÓÐÀúÊ·Êý¾Ý£¬¾Í¶ÁÈ¡Êý¾Ý£¬ÏÈ·¢ÀúÊ·Êý¾Ý */
		sensordata_p = (Save_Sensor_Data_Struct*)Flash_Read_Sensor_Data(
						fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer%Flash_Capacity_Size,
						Save_Sensor_Data_Len);

		//ÉèÖÃ Flash ¶ÁÖ¸Õë
		/* Ôø¼Ó ¶ÁÈ¡µÄÊý¾Ý³¤¶È */
		fix_tab->Inf_State.Instru_Flash.Inf_State.Read_Pointer 		+= Save_Sensor_Data_Len; 
		/* ´æ´¢Êý¾ÝÁ¿ ¼õ*/
		fix_tab->Inf_State.Instru_Flash.Inf_State.Storage_Data_NUM 	-= Save_Sensor_Data_Len;
		
	}
	
	/* ÉèÖÃÉÏ´«Ê±¼ä´Á */
	time.Year 	= config_tab->Inf_State.Instru_RTC.Inf_State.Year;
	time.Month	= config_tab->Inf_State.Instru_RTC.Inf_State.Month;
	time.Day 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Day;
	time.Hour 	= config_tab->Inf_State.Instru_RTC.Inf_State.Hour;
	time.Min 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Min;
	time.Sec 	  = config_tab->Inf_State.Instru_RTC.Inf_State.Sec;
	/* ½«²ÎÊýµÄ²É¼¯Ê±¼ä×ª»»³ÉÃë£¬½«Õâ¸öÊý¾ÝÉèÖÃÎª²ÎÊýÇ©ÃûµÄÊ±¼äÊý¾Ý¡£ */
	HTTPPara.ApiPara.TimeStamp = RTC_Date_Time_To_Second(&time);
	
	/* Çå³ý²ÎÊýÇ©ÃûÊý¾Ý»º´æ¡£ */
	memset(HTTPPara.ApiPara.Sign,0x00,sizeof(HTTPPara.ApiPara.Sign));
	
	/* ÉèÖÃGPSÉè±¸×´Ì¬ */
	if((GPS_Config_Enable == 0x01)&&
	   ((Sensor_Lon_Num + Sensor_Lat_Num + Sensor_Spd_Num) > 0))
	{
		/* Ê¹ÓÃGPSÉè±¸ */
		/* Î´Ê¹ÓÃGPSÉè±¸ */
		HTTPPara.ApiPara.GPS_State = TRUE;
		/* ²éÑ¯¾­Î³¶ÈÊý¾Ý */
		for(num = 0;num < 32 ;num++)
		{
			/* ÅÐ¶ÏÍ¨µÀÀàÐÍ */
			if(Sensor_GPS_POS_Lon == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
			{
				/* ÉèÖÃ¾­¶ÈÊý¾Ý */
				HTTPPara.ApiPara.Lng = sensordata_p->GPRS_IR.Sensor_Data[num];
			}else if(Sensor_GPS_POS_Lat == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
			{
				/* ÉèÖÃÎ³¶ÈÊý¾Ý */
				HTTPPara.ApiPara.Lat = sensordata_p->GPRS_IR.Sensor_Data[num];
			}
		}
	}else
	{
		/* Î´Ê¹ÓÃGPSÉè±¸ */
		HTTPPara.ApiPara.GPS_State = FALSE;
		/* Çå³ý¾­¶ÈÊý¾Ý *///ÔÝ¶¨0£¬ºóÃæÉèÖÃ³ÉÎÞÏßºÅÊý¾Ý¡£
		HTTPPara.ApiPara.Lng = 0;
		/* Çå³ýÎ³¶ÈÊý¾Ý *///ÔÝ¶¨0£¬ºóÃæÉèÖÃ³ÉÎÞÏßºÅÊý¾Ý¡£
		HTTPPara.ApiPara.Lat = 0;
	}
	
	/* ÉèÖÃ²É¼¯Ê±¼ä */
	time.Year 	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.YEAR_BITS + 2000;
	time.Month	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.MONTH_BITS;
	time.Day 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.DAY_BITS;
	time.Hour 	= sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.HOUR_BITS;
	time.Min 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.MINUTE_BITS;
	time.Sec 	  = sensordata_p->GPRS_IR.Timestamp.DATETIME_Bit.SECOND_BITS;
	
	/* ÉèÖÃ²É¼¯Ê±¼ä */
	sprintf(HTTPPara.ApiPara.CollectTime,"%04d-%02d-%02d %02d:%02d:%02d",
			time.Year,time.Month,time.Day,time.Hour,time.Min,time.Sec);
	
	/* »ñÈ¡µ±Ç°Ì½Í·ÊýÁ¿ */
	HTTPPara.ApiPara.ProbeNum = Sensor_Tmp_Num + Sensor_Hum_Num +
								Sensor_RF_Single_Tmp_Num + 
								(Sensor_RF_Humiture_Tmp_Num +
								Sensor_RF_Humiture_Hum_Num)/2 +
								Sensor_PT1000_Num +
								Sensor_GW_NJSY_Num;
	
	
	/* °´Ë³ÐòÀ´ÉèÖÃÌ½Í·Êý¾Ý */
	for(num = 0,probeNum = 0;(probeNum < HTTPPara.ApiPara.ProbeNum)&&(num < 32);num++)
	{
		if(Sensor_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ÉèÖÃÌ½Í·±àºÅ */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* ÉèÖÃÌ½Í·ÀàÐÍ */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ÉèÖÃµ¥ÎÂÊý¾Ý */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_Hum == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ÉèÖÃÌ½Í·±àºÅ */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* ÉèÖÃÌ½Í·ÀàÐÍ */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ÉèÖÃµ¥ÎÂÊý¾Ý */
			HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_RF_Card_Single_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ÅÐ¶Ï¿¨Æ¬SNºÅ */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* ¿¨Æ¬SNºÅÎ´ÅäÖÃ¹ý£¬²»·¢ËÍµ±Ç°Í¨µÀÊý¾Ý. */
				/* ¼õµôÒ»¸öÍ¨µÀ. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ¼ÌÐø */
				continue;
			}
			
			/* ÉèÖÃÌ½Í·±àºÅ */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* ÉèÖÃÌ½Í·ÀàÐÍ */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ÉèÖÃµ¥ÎÂÊý¾Ý */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_RF_Card_Humiture_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* Êª¶È´«¸ÐÆ÷±àºÅ */
			unsigned char humiNO;
			
			/* ÅÐ¶Ï¿¨Æ¬SNºÅ */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* ¿¨Æ¬SNºÅÎ´ÅäÖÃ¹ý£¬²»·¢ËÍµ±Ç°Í¨µÀÊý¾Ý. */
				/* ¼õµôÒ»¸öÍ¨µÀ. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ¼ÌÐø */
				continue;
			}
			
			/* ÉèÖÃÌ½Í·±àºÅ */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* ÉèÖÃÌ½Í·ÀàÐÍ */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ÉèÖÃÎÂ¶ÈÊý¾Ý */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
			/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
			humiNO = GPRS_GET_RF_Humiture_Hum_Data(((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
			/* ÅÐ¶ÏÊÕË÷½á¹û */
			if(humiNO != 0xFF)
			{
				/* ÉèÖÃÎÂ¶ÈÊý¾Ý */
				HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[humiNO];		
			}else
			{
				/* ÉèÖÃÎÂ¶ÈÊý¾Ý */
				HTTPPara.ApiPara.Logs[probeNum].Humi = Wireless_RF_Card_Data_Rece_Timeout_Para;		
			}
			
			/* ÅÐ¶ÏÊÇ·ñ½ÓÊÕµ½Êý¾Ý */
			if((Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Wireless_RF_Card_Data_Rece_Timeout_Para == HTTPPara.ApiPara.Logs[probeNum].Temp))
			{
				/* ¿¨Æ¬SNºÅÎ´ÅäÖÃ¹ý£¬²»·¢ËÍµ±Ç°Í¨µÀÊý¾Ý. */
				/* ¼õµôÒ»¸öÍ¨µÀ. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ¼ÌÐø */
				continue;
			}
		}else if(Sensor_PT1000 == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* ÉèÖÃÌ½Í·±àºÅ */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%sCH%02d",
					HTTPPara.ApiPara.GatewayIdNo,(probeNum+1));
			/* ÉèÖÃÌ½Í·ÀàÐÍ */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ÉèÖÃµ¥ÎÂÊý¾Ý */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
		}else if(Sensor_GW_NJSY_Tmp == ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type)
		{
			/* Êª¶È´«¸ÐÆ÷±àºÅ */
			unsigned char humiNO;
			
			/* ÅÐ¶Ï¿¨Æ¬SNºÅ */
			if((((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[1] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2] == 0x00)&&
			   (((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[3] == 0x00))
			{
				/* ¿¨Æ¬SNºÅÎ´ÅäÖÃ¹ý£¬²»·¢ËÍµ±Ç°Í¨µÀÊý¾Ý. */
				/* ¼õµôÒ»¸öÍ¨µÀ. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ¼ÌÐø */
				continue;
			}
			
			/* ÉèÖÃÌ½Í·±àºÅ */
			sprintf(HTTPPara.ApiPara.Logs[probeNum].probeIdNo,"%04X%05d",
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[0])) & 0x00ff) << 8)),
			((((*(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0xff00) >> 8)|
			((( *(u16*)(&((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_SN_NO[2])) & 0x00ff) << 8)));
			/* ÉèÖÃÌ½Í·ÀàÐÍ */
			HTTPPara.ApiPara.Logs[probeNum].Type = ((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->Type;
			/* ÉèÖÃÎÂ¶ÈÊý¾Ý */
			HTTPPara.ApiPara.Logs[probeNum].Temp = sensordata_p->GPRS_IR.Sensor_Data[num];
			/* ²éÕÒ¶ÔÓ¦µÄÎÂÊª¶È¿¨Æ¬µÄÊª¶ÈÍ¨µÀ */
			humiNO = GPRS_GET_RF_Humiture_Hum_Data(((INSTRU_SENSOR_CHANNEL*)(sensorchP+num))->SENSOR_RF_Card_ID_NO,&config_tab->Inf_State.Instru_Sensor);
			/* ÅÐ¶ÏÊÕË÷½á¹û */
			if(humiNO != 0xFF)
			{
				/* ÉèÖÃÎÂ¶ÈÊý¾Ý */
				HTTPPara.ApiPara.Logs[probeNum].Humi = sensordata_p->GPRS_IR.Sensor_Data[humiNO];		
			}else
			{
				/* ÉèÖÃÎÂ¶ÈÊý¾Ý */
				HTTPPara.ApiPara.Logs[probeNum].Humi = Gateway_NJSY_TimeoutPara;		
			}
			
			/* ÅÐ¶ÏÊÇ·ñ½ÓÊÕµ½Êý¾Ý */
			if((Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp)&&
			   (Gateway_NJSY_TimeoutPara == HTTPPara.ApiPara.Logs[probeNum].Temp))
			{
				/* ¿¨Æ¬SNºÅÎ´ÅäÖÃ¹ý£¬²»·¢ËÍµ±Ç°Í¨µÀÊý¾Ý. */
				/* ¼õµôÒ»¸öÍ¨µÀ. */
				HTTPPara.ApiPara.ProbeNum--;
				/* ¼ÌÐø */
				continue;
			}
		}else
		{
			continue;
		}
		
		/* ÉèÖÃÍ¨µÀ±àºÅ */
		probeNum++;
		
	}
	/* ´´½¨HTTP·¢ËÍÊý¾Ý¡£ */
HTTP_Create_Data((char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
	/* Ã»ÓÐÀúÊ·Êý¾Ý */
	return TRUE;
}

#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x02)||(AppGPRSComProtocol == 0x04))
/*
*********************************************************************************************************
*	º¯ Êý Ãû		: GPRS_Data_To_Network
*	¹¦ÄÜËµÃ÷		: ½«ÒÇÆ÷µÄÊý¾Ý×°»»³ÉÍøÂçÊý¾Ý£¬µÈ´ý·¢ËÍ¡£ 
*	ÐÎ    ²Î		: ÎÞ
*	·µ »Ø Öµ		: ÎÞ
*********************************************************************************************************
*/
void GPRS_Data_To_Network(void)
{
	
/* ·¢ËÍÊý¾Ý¼Ä´æÆ÷ ÏûÏ¢ÌåÊý¾ÝÖ®Ç°µÄ ¹Ì¶¨Êý¾Ý³¤¶È *///ÓëÖ®Ç°µÄ²»Í¬
#define GPRS_Send_Data_Before_Fix_Len	((u32)(&GPRS_Send_Reg.GPRS_IR_FIX.Body_Type)-\
										 (u32)(&GPRS_Send_Reg.GPRS_IR_FIX.ID_Type_HSB))
/* Ð£ÑéÂëÊý¾ÝÎ»Êý */
#define GPRS_Send_Data_Check_Byte		(1u)
	
	/*  GPRS·¢ËÍÊý¾Ý³¤¶È */
	u16 gprs_send_sensor_data_len = 0;	
	

	//1¡¢×ªÒå Êý¾Ý
	/* ×ªÒå *//* »ñµÃGPRS·¢ËÍÊý¾Ý³¤¶È */
	gprs_send_sensor_data_len = GPRS_Host_To_Network_Translator_Data(&GPRS_Send_Sensor_Data_Buffer[1],
																	 &GPRS_Send_Reg.GSM_GPRS_Send_Buffer[1],
																	 (GPRS_Send_Data_Before_Fix_Len+
																((u16)GPRS_Send_Reg.GPRS_IR_FIX.Attribute_HSB_Length*256)+
																((u16)GPRS_Send_Reg.GPRS_IR_FIX.Attribute_LSB_Length)+GPRS_Send_Data_Check_Byte));
	
	//2¡¢Ôö¼Ó Í·ºÍÎ² ±êÊ¶
	/* ¼ÓÍ·±êÖ¾  */
	GPRS_Send_Sensor_Data_Buffer[0] = GPRS_COM_Identify;
	/* GPRS·¢ËÍÊý¾Ý³¤¶È  */
	gprs_send_sensor_data_len++;
	/* ¼ÓÎ²±êÖ¾ */
	GPRS_Send_Sensor_Data_Buffer[gprs_send_sensor_data_len] = GPRS_COM_Identify;
	/* GPRS·¢ËÍÊý¾Ý³¤¶È  */
	gprs_send_sensor_data_len++;
	/* ¼ÆËã ´«ÊäµÄÊý¾Ý ³¤¶È *///Í·±êÖ¾(1 Byte)Î²±êÖ¾(1 Byte)
	GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = gprs_send_sensor_data_len;
	
	
#undef GPRS_Send_Data_Before_Fix_Len
#undef GPRS_Send_Data_Check_Byte
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: GPRS_Host_To_Network_Translator_Data
*	¹¦ÄÜËµÃ÷: ½«Ö÷»úÐò×ª»»³ÉÍøÂçÐòÊý¾Ý
*	ÐÎ    ²Î£ºnetwork_endian   host_endian	T_num
*	·µ »Ø Öµ: ×ª»»ºóµÄÊý¾Ý»º´æ³¤¶È
*********************************************************************************************************
*/
u16 GPRS_Host_To_Network_Translator_Data(u8* network_endian,u8* host_endian,u16 T_num)
{
	u16 num;
	u16 b_sp;
	
	/* »º´æÖ¸ÕëÇå0 */
	b_sp = 0;

	for(num = 0;num < T_num; num++)
	{
		if(host_endian[num] == GSM_Translator_Symbol_0X7E)
		{
			/* µ±Ç°Êý¾Ý ÊÇ×ªÒë ·ûºÅ0X7E */
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X7D;
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X02;
		}else if(host_endian[num] == GSM_Translator_Symbol_0X7D)
		{
			/* µ±Ç°Êý¾Ý ÊÇ×ªÒë ·ûºÅ0X7D */
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X7D;
			*(network_endian+b_sp++) = GSM_Translator_Symbol_0X01;
		}else
		{
			/* µ±Ç°Êý¾Ý ·Ç×ªÒë ·ûºÅ */
			*(network_endian+b_sp++) = *(host_endian+num);
		}
	}
	return b_sp;
}

/************************************************************************************************/
/* º¯ÊýÃû	£ºGPRS_Network_To_Host_Translator_Data												*/
/* ÊäÈë		£ºnetwork_endian   host_endian	T_num												*/
/* Êä³ö		£º×ª»»ºóµÄÊý¾Ý»º´æ³¤¶È																	*/
/* ×÷ÓÃ		£º½«ÍøÂçÐò×ª»»³ÉÖ÷»úÐòÊý¾Ý			 									 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/01																		*/
/************************************************************************************************/
u16 GPRS_Network_To_Host_Translator_Data(u8* host_endian,u8* network_endian,u16 T_num)
{
	u16 num;
	u16 b_sp;
	
	/* »º´æÖ¸ÕëÇå0 */
	b_sp = 0;
	/* ¶ÁÈ¡Êý¾Ý */
	for(num = 0;num < T_num; num++)
	{
		/* ÅÐ¶Ï µ±Ç°µÄÊý¾Ý ÊÇ·ñÎª×ªÒå±àÂë. */
		if(network_endian[num] == GSM_Translator_Symbol_0X7D)
		{
			/* ÊÇ×ªÒå±àÂë */
			/* ÅÐ¶Ï ÊÇ2¸ö±àÂëÖÐµÄÄÇ¸ö? */
			if(network_endian[num+1] == GSM_Translator_Symbol_0X02)
			{
				/* ËµÃ÷ ¸Ã±àÂëÔ´ÂëÊÇ0X7E */
				host_endian[b_sp++] = GSM_Translator_Symbol_0X7E;
				/* Êý¾ÝÖ¸Õë Ö¸Ïò ÏÂÏÂ¸öÊý¾Ý */
				num++;
			}else if(network_endian[num+1] == GSM_Translator_Symbol_0X01)
			{
				/* ËµÃ÷ ¸Ã±àÂëÔ´ÂëÊÇ0X7D */
				host_endian[b_sp++] = GSM_Translator_Symbol_0X7D;
				/* Êý¾ÝÖ¸Õë Ö¸Ïò ÏÂÏÂ¸öÊý¾Ý */
				num++;
			}else
			{
				/* Î»ÖÃÀàÐÍµÄ±àÂë£¬ÕâÖÖÇé¿öÊÇ´íÎóµÄ×ªÒÆµ¼ÖÂµÄ¡£ */
				b_sp = 0;
				return b_sp;
			}
		}else
		{
			/* Õý³£Êý¾Ý£¬Ö±½Ó»ñÈ¡ */
			host_endian[b_sp++] = network_endian[num];
		}
	}
	return b_sp;
}
#endif
#endif
/*
*********************************************************************************************************
*	º¯ Êý Ãû: GSM_Send_Q
*	¹¦ÄÜËµÃ÷: ·¢ËÍ GSM ÏûÏ¢¶ÓÁÐ
*	ÐÎ    ²Î£ºpevent:ÏûÏ¢¶ÓÁÐµÄÖ¸Õë£¬GSM_CMD_TYPE:ÏûÏ¢¶ÓÁÐÀàÐÍ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
BOOL GSM_Send_Q(OS_EVENT* pevent,GSM_CMD_TYPE EVENT_Type)
{
	INT8U     	err;
	OS_Q_DATA	Q_data;
	GSM_QUE*	EVENT_p;	
	u16 		num;
	u32 		Q_p;

	
	/* ·¢ËÍµÄ ÊÂ¼þ ÀàÐÍ  */
	if(EVENT_Type == Init_GSM_Module_EVENT)
	{
		/* ÅäÖÃGSMÄ£¿é */
		EVENT_p = &GSM_QUE_Init_Module;
	}else if(EVENT_Type == IAP_EVENT)
	{
		/* ÏÂÔØ ³ÌÐò¸üÐÂ Êý¾Ý */	
		EVENT_p = &GSM_QUE_Download_IAP_Data;	
	}else if(EVENT_Type == Update_SGL_EVENT)
	{
		/* ¸üÐÂÐÅºÅÇ¿¶È */
		EVENT_p = &GSM_QUE_Update_Signal;	
	}else if(EVENT_Type == Update_Link_EVENT)
	{
		/* ¸üÐÂ¸½×Å×´Ì¬ */
		EVENT_p = &GSM_QUE_Update_Link;	
	}else if(EVENT_Type == Updata_Data_EVENT)
	{
		/* ÉÏ´«ÎÂÊª¶ÈÊý¾Ý */
		EVENT_p = &GSM_QUE_Update_Data;	
	}else if(EVENT_Type == Send_SMS_EVENT)
	{
		/* ·¢ËÍ ¶ÌÐÅ */		
		EVENT_p = &GSM_QUE_Send_SMS;	
	}else if(EVENT_Type == ConfigService_EVENT)
	{
		/* ÉÏ´« ÅäÖÃ Êý¾Ý */		
		EVENT_p = &GSM_QUE_ConfigService;		
	}
//	else if(EVENT_Type == Updata_Config_Back_EVENT)
//	{
//		/* ÉÏ´« ÅäÖÃ Êý¾Ý */		
//		EVENT_p = &GSM_QUE_Send_Config_Back;		
//	}
	else if(EVENT_Type == System_Clock_Calibration_EVENT)
	{
		/* ÉÏ´« ÅäÖÃ Êý¾Ý */		
		EVENT_p = &GSM_QUE_Send_System_Clock_Calibration;		
	}
#if (GSM_Module_Type==0x02) //ESP8266WifiÄ£¿é	
	else if(EVENT_Type == SetWifiNamePWD_EVENT)
	{
		/* ÉèÖÃWifiÃû×ÖºÍÃÜÂë */		
		EVENT_p = &Wifi_QUE_SetWifiNamePWD;		
	}
	else if(EVENT_Type == SetWifiWLAN_EVENT)
	{
		/* ÉèÖÃWifiWlan²ÎÊý */		
		EVENT_p = &Wifi_QUE_SetWifiWLAN;		
	}
	else if(EVENT_Type == SetWifiWLAN_GET_IP_WAY_EVENT)
	{
		/* ÉèÖÃWifiÄ£¿éIP»ñÈ¡·½Ê½²ÎÊý */		
		EVENT_p = &Wifi_QUE_GET_IP_WAY;		
	}
	else if(EVENT_Type == RestWifi_EVENT)
	{
		/*¸´Î»WifiÄ£¿é*/		
		EVENT_p = &Wifi_QUE_Reset;		
	}	
	else if(EVENT_Type == Factory_Data_Reset_Wifi_EVENT)
	{
		/* WifiÄ£¿é»Ö¸´³ö³§ÉèÖÃ */		
		EVENT_p = &Wifi_QUE_Factory_Data_Reset;		
	}		

#endif	
	
	/* ³ÌÐò±¨µÀÕâÀï ËµÃ÷³ÌÐòÖÐ²»´æÔÚ ÉÏ´«Êý¾ÝÏûÏ¢ */
	/* ÅÐ¶Ï ÉÏ´«Êý¾Ý Ö´ÐÐ½á¹û */
	if(EVENT_p->EVENT_Getarg == FALSE)
	{
		/* ÉÏ´«Êý¾ÝÏûÏ¢ ÕýÔÚ ±»Ö´ÐÐ£¬µÈ´ýÖ´ÐÐ½áÊø */
		return FALSE;
	}
	
	//ÓÉÓÚ ÄÚ´æ¿Õ¼äÓÐÏÞ ÕâÀïµÄÏûÏ¢Ö»ÓÐÒ»¸öÊý¾Ý¿Õ¼ä£¬Òò´Ë£¬ÔÚ·¢ËÍÏûÏ¢Ç°ÐèÒªÈ·¶¨ÉÏ´ÎµÄÏûÏ¢ÒÑ¾­±»Ö´ÐÐÁË¡£
	//·ñÔò£¬Í¬Ò»ÖÖÏûÏ¢½«Õ¼¾Ý2¸öÏûÏ¢¿Õ¼ä£¬µ¼ÖÂÆäËûÏûÏ¢ÎÞ·¨½øÈë¡£
	/* ¼ì²éÏûÏ¢¶ÓÁÐÖÐÊÇ·ñ´æÔÚ ¸üÐÂ ¸½×Å×´Ì¬ ÏûÏ¢¡£ */
	err = OSQQuery(pevent,&Q_data);
	
	/* ÅÐ¶Ï²éÑ¯½á¹û */
	if(err == OS_ERR_NONE)
	{
		/* ²éÑ¯³É¹¦ */
		
		/* ²éÕÒ ¶ÓÁÐÖÐÊÇ·ñÒÑ¾­´æÔÚ ¸½×Å×´Ì¬ ÏûÏ¢¡£ */
		for(num = 0,Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQOut);
			num < Q_data.OSNMsgs;
			num ++)
		{
			/* ÅÐ¶Ïµ¥Ç°µÄÏûÏ¢Ö¸ÕëÓë·¢ËÍÖ¸ÕëÊÇ·ñÏàÍ¬ */
			if((*(u32*)Q_p) == (u32)EVENT_p)
			{
				/* ÏàµÈ£¬¶ÓÁÐÖÐ´æÔÚ ¸½×Å×´Ì¬ ÏûÏ¢,ÄÇÃ´¾Í²»ÔÚ·¢ËÍ ¸½×Å×´Ì¬ ÏûÏ¢ */
				return FALSE;
			}
			/* ÉèÖÃÖ¸Õëµ½ÏÂÒ»¸öÎ»ÖÃ */
			Q_p += sizeof(GSM_QUE);
			/* ÅÐ¶ÏÖ¸ÕëÊÇ·ñÔ½½ç */
			if(Q_p == (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQEnd))
			{
				/* Ô½½ç¾Í½« Ö¸Õë Ö¸ÏòÍ·Î»ÖÃ */
				Q_p = (u32)(((OS_Q *)(pevent->OSEventPtr))->OSQStart);
			}
		}
	}else
	{
		/* ²éÑ¯Ê§°Ü */
		return FALSE;
	}
	
	/* ³ÌÐòµ½ÕâÀï¾ÍËµÃ÷ ¶ÓÁÐÖÐÃ»ÓÐ´æÔÚ ÐÅºÅÇ¿¶È ÏûÏ¢£¬
	   Òò´Ë£¬ÐèÒª·¢ËÍÒ»´Î ÐÅºÅÇ¿¶È¸üÐÂ ÏûÏ¢¡£ */
	/* Çå³þ ÏûÏ¢ Ö´ÐÐ½á¹û */
	EVENT_p->EVENT_Getarg = FALSE;
	/* ·¢ËÍ ÏûÏ¢ *///FIFO	
	err = OSQPost(pevent, EVENT_p);  
	/* ÅÐ¶Ï ÏûÏ¢ ·¢ËÍ ×´Ì¬ */
	if(err == OS_ERR_Q_FULL)
	{
		/* ÏûÏ¢¶ÓÁÐÒÑÂú,²»ÄÜÔÙ½ÓÊÕÐÂÏûÏ¢ */
		/* ÕâÖÖÇé¿ö ²»¿ÉÄÜ ³öÏÖ */
		return FALSE;
	}
	/* ÅÐ¶Ï ÏûÏ¢ ·¢ËÍ ³É¹¦ */
	return TRUE;
}

/************************************************************************************************/
/* º¯ÊýÃû	£ºGPRS_Save_Sensor_Data_Check														*/
/* ÊäÈë		£ºÎÞ																				*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£º¼ì²â ÊÇ·ñÓÐÐÂµÄ ´æ´¢µÄ ÎÂÊª¶È Êý¾Ý 									 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/01																		*/
/************************************************************************************************/
void GPRS_Save_Sensor_Data_Check(INSTRU_CONFIG_INF_TAB* config_tab,INSTRU_FIX_INF_TAB* fix_tab)
{
	
	/* ÅÐ¶Ïµ¥Ç°Ä£¿éµÄ¹¤×÷×´Ì¬ÊÇ·ñÕý³£ */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		/* Ä£¿é¹¤×÷Õý³£ */
		
#if (AppGPRSComProtocolEnable == 0x01)
#if (AppGPRSComProtocol == 0x02)
	
		/* ÅÐ¶Ï ÊÇ·ñÓÐ´«¸ÐÆ÷Êý¾ÝÔÚµÈ´ý·¢ËÍ */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)
		{
			/* ´´½¨Êý¾Ý */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
			ComProtocolZDE_CreateData(config_tab,fix_tab,
			GPRS_Send_Sensor_Data_Buffer,
			&GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len);
			/* ¿½±´Êý¾Ý */
//			memcpy((char*)GPRS_Send_Sensor_Data_Buffer,
//				   ComProtocolZDEApp.DR.TXDR,
//					TXLen);
			
			/* ÅÐ¶Ï */
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
			   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE) &&
			 (((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == TRUE) &&
			   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE))||
			   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0)))
			{
					/* ·¢ËÍÊý¾Ý */
				GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;		
			}
		}
		
#else		
#if (AppGPRSComProtocol == 0x04)//½­ËÕÊ¡ÃâÒß¹æ»®Êý¾ÝÖÐÐÄÐ­Òé
/* ÅÐ¶Ï ÊÇ·ñÓÐ´«¸ÐÆ÷Êý¾ÝÔÚµÈ´ý·¢ËÍ */
if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)&&(RTC_System_Clock_Calibration_State_Reg == TRUE)&&(ConfigService_App.SYNC_State == TRUE))
{
	/* ´´½¨Êý¾Ý */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
	ComProtocol_JS_CreateData(config_tab,fix_tab,
	GPRS_Send_Sensor_Data_Buffer,
	&GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len);
}
 #endif
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01))//²¿±êÐ­Òé»òÕßHTTPÐ­Òé
		//²¿±êÐ­Òé
		/* ÅÐ¶Ï ÊÇ·ñÓÐ´«¸ÐÆ÷Êý¾ÝÔÚµÈ´ý·¢ËÍ */
		if(GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == FALSE)
		{
			/* Ã»ÓÐ ´«¸ÐÆ÷Êý¾ÝÔÚµÈ´ý×ª»» */
			/* ¼ì²â ÊÇ·ñÓÐÐÂµÄ ´æ´¢µÄ ÎÂÊª¶È Êý¾Ý */
 			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = 
			GPRS_Get_Save_Sensor_Data(config_tab,fix_tab);
		}
#endif			
		/* ÅÐ¶Ï GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾ÝÍ¨ÐÅ×´Ì¬ */
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE :ËµÃ÷µ±Ç°Ã»ÓÐÊý¾ÝÉÏ´«
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE :ËµÃ÷ÓÐÊý¾ÝµÈ´ýÉÏ´«
		//GPRS_COM_Send_Result == TRUE && GPRS_COM_Rece_Result == TRUE :ËµÃ÷Ö®Ç°µÄÊý¾ÝÉÏ´«³É¹¦¡¢¿ÉÒÔ¿ªÊ¼ÉÏ´«ÏÂÒ»¸öÊý¾Ý
		//GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0 : ËµÃ÷·¢Ã»ÓÐÊý¾Ý·¢ËÍ»òÕß·¢ËÍµÄÊý¾Ý±»Çå³ý£¬¿ÉÒÔ¿ªÊ¼ÉÏ´«ÏÂÒ»¸öÊý¾Ý¡£
		if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
		   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE) &&
		 (((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result == TRUE) &&
		   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result == TRUE))||
		   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len == 0))
		)
		{
			/* Ã»ÓÐ ´«¸ÐÆ÷Êý¾Ý ÕýÔÚ·¢ËÍ */
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))
			/* ½«ÒÇÆ÷µÄÊý¾Ý×°»»³ÉÍøÂçÊý¾Ý£¬µÈ´ý·¢ËÍ. */
			GPRS_Data_To_Network();
#elif (AppGPRSComProtocol == 0x01)
			/* ¼ÆËã·¢ËÍÊý¾Ý³¤¶È */
			GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = strlen((char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
			/* Çå³ýÊý¾Ý»º´æ */
			memset(GPRS_Send_Sensor_Data_Buffer,0x00,GPRS_Send_Sensor_Data_Buffer_Size);
			/* ¿½±´»º´æ */
			strcpy((char*)GPRS_Send_Sensor_Data_Buffer,
				   (char*)GPRS_Send_Reg.GSM_GPRS_Send_Buffer);
#endif
#endif
			/* Çå³ý GPRS Ö÷»úÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬ */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = FALSE;
			/* ÉèÖÃ GPRS ÍøÂçÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬ */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = TRUE;
		}
#endif
#endif
	}
}

/************************************************************************************************/
/* º¯ÊýÃû	£ºGPRSSendData_ParseStateCheck														*/
/* ÊäÈë		£ºÎÞ																					*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£ºGPRS½ÓÊÕ½âÎöÊý¾Ý×´Ì¬¼ì²â													 		 */
/* ´´½¨ÈÕÆÚ	£º2016/09/09																		*/
/************************************************************************************************/
void GPRSSendData_ParseStateCheck(void)
{
	/* ¼ÆÊýÆ÷ */
	static uint32_t num = 0;
	
	/* ÅÐ¶Ï GPRS½ÓÊÕ½âÎöÊý¾Ý×´Ì¬¼ì²â */
	if(FALSE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState)
	{
		/* ½âÎöÊ§°Ü */
		/* ¼Ó1£¬ÑÓÊ± */
		num++;
		/* ÅÐ¶ÏÑÓÊ±Ê±¼ä */
		if(num >= AppGPRSUsageProtectTime)
		{
			/* ÑÓÊ±Ê±¼äµ½£¬Çå³ý×´Ì¬±êÖ¾ */
			GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState = TRUE;
			/* Çå³ý¼ÆÊý */
			num = 0;
		}
	}
}

/************************************************************************************************/
/* º¯ÊýÃû	£ºGPRS_Send_Sensor_Data																*/
/* ÊäÈë		£ºÎÞ																					*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£º·¢ËÍÐÂµÄ´«¸ÐÆ÷Êý¾Ý													 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/01																		*/
/************************************************************************************************/
void GPRS_Send_Sensor_Data(void)
{

	/* ÅÐ¶Ï GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾ÝÍ¨ÐÅ×´Ì¬ */
	/* ÅÐ¶Ï ´«¸ÐÆ÷Êý¾Ý ÕýÔÚ·¢ËÍ && ÓÐÍøÂçÐòÊý¾ÝµÈ´ý·¢ËÍ */
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE :ËµÃ÷µ±Ç°Ã»ÓÐÊý¾ÝÉÏ´«£¬¿ÉÒÔ¿ªÊ¼ÉÏ´«Êý¾Ý
		//GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State == TRUE :ËµÃ÷ÓÐÊý¾ÝµÈ´ýÉÏ´«
		//GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0 : ËµÃ÷ÓÐÊý¾Ý·¢ËÍ£¬¿ÉÒÔ¿ªÊ¼ÉÏ´«Êý¾Ý¡
	 //  IAP_App.RunState == FALSE;   IAP_App.RunState=FALSE±íÊ¾Ã»ÓÐÕýÔÚ½øÐÐÔ¶³ÌÉý¼¶;
// printf("1 ¡·¡·¡·¡·¡·¡·GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,
//	  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
	
	
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x00)
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State == TRUE) &&
	   (TRUE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0)&&
		  IAP_App.RunState == FALSE)
	{
		/* ·¢ËÍ ÉÏ´«Êý¾ÝÏûÏ¢ */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Updata_Data_EVENT);
		
//	printf("2 ¡·¡·¡·¡·¡·¡·GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n   GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
		
	}
	
		#endif
		#endif
		#if (AppGPRSComProtocolEnable == 0x01)
    #if (AppGPRSComProtocol == 0x01)
			if((GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State == FALSE) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State == TRUE) &&
	   (TRUE == GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState) &&
	   (GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len > 0))
	{
		/* ·¢ËÍ ÉÏ´«Êý¾ÝÏûÏ¢ */
		GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Updata_Data_EVENT);
		
//	printf("2 ¡·¡·¡·¡·¡·¡·GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State= %d\r\n   GGPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState= %d\r\n  GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len= %d\r\n ",
//	   GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State,GPRS_Send_Sensor_Data_Reg.GPRS_COM_ParseState,GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len     
//	);
		
	}
	
		#endif
		#endif

}


/************************************************************************************************/
/* º¯ÊýÃû	£ºGPRS_Parsing_Network_Data															*/
/* ÊäÈë		£ºÎÞ																					*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£º¸ù¾Ý·þÎñÆ÷·µ»ØµÄÊý¾ÝÐÅÏ¢£¬ÅÐ¶Ï´«¸ÐÆ÷Êý¾ÝÉÏ´«×´Ì¬£¬ÉèÖÃÏà¹Ø²ÎÊý¡£			 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/01																		*/
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00))//½âÎö²¿±êÐ­Òé·µ»ØÀ´µÄÊý¾Ý
static BOOL GPRS_Parsing_Network_Data(void)
{
/* ½ÓÊÕµÄÍøÂçÐòÊý¾Ý³¤¶È */
#define GPRS_Rece_Network_Data_Len		(20u)
/* Í¨ÓÃÓ¦´ðÐ­ÒéµÄ½á¹û */
/* Êý¾ÝÉÏ´«³É¹¦ */
#define GPRS_Rece_ACK_Result_Success	(0X00)
/* Êý¾ÝÉÏ´«Ê§°Ü */
#define GPRS_Rece_ACK_Result_Failure	(0X01)
/* Êý¾ÝÉÏ´«ÓÐÎó */
#define GPRS_Rece_ACK_Result_Error		(0X02)
/* Êý¾Ý²»Ö§³Ö */	
#define GPRS_Rece_ACK_ON_Format			(0X03)
/* ±¨¾¯´¦ÀíÈ·ÈÏ */	
#define GPRS_Rece_ACK_Alarm_Confirm		(0X04)	
	
	/* Ð£ÑéÂë */
	u8 check_code;
	/* ¼ÆÊýÆ÷ */
	u8 num;
	/* ·þÎñÆ÷·µ»ØµÄÊý¾Ý³¤¶È */
	u8 gprs_rece_data_len;
	
	//1¡¢ÄæÏò×ªÒåÊý¾Ý
	/* ÏÈ¶Ô ½ÓÊÕÊý¾Ý ½øÐÐÄæÏò×ªÒå */
	gprs_rece_data_len = GPRS_Network_To_Host_Translator_Data(GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,
															  GPRS_Rece_Sensor_Data_Buffer,
															  GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len);
	
	//2¡¢½âÎöÊý¾Ý
	/* ÅÐ¶ÏÊý¾Ý³¤¶È *//* ÏÖÔÚÓ¦´ðÏûÏ¢³¤¶ÈÊÇ¹Ì¶¨µÄ(20×Ö½Ú) */
	if(gprs_rece_data_len != GPRS_Rece_Network_Data_Len)
	{
		/* ½ÓÊÕµ½µÄÊý¾Ý³¤¶ÈÓëÉè¶¨µÄ²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* ÅÐ¶Ï±êÊ¶Î»Êý¾Ý */
	if((GPRS_Rece_Reg.GPRS_IR_FIX.Start_Iden != GPRS_COM_Identify)||
	   (GPRS_Rece_Reg.GPRS_IR_FIX.End_Iden   != GPRS_COM_Identify))
	{
		/* Í·±êÊ¶»òÎ²±êÊ¶ÓëÐ­Òé²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* ÅÐ¶ÏÏûÏ¢Í· ÏûÏ¢IDÊý¾Ý */
	if((GPRS_Rece_Reg.GPRS_IR_FIX.ACK_ID_Type_HSE != (GPRS_COM_ID_Upload/0X100))||	
	   (GPRS_Rece_Reg.GPRS_IR_FIX.ACK_ID_Type_LSE != (GPRS_COM_ID_Upload%0X100)))
	{
		/* ÏûÏ¢Í·ÏûÏ¢IDÓëÐ­Òé²»Ò»ÖÂ */		
		return FALSE;
	}
	
	/* ÅÐ¶ÏÏûÏ¢Ìå³¤¶È */	
	if((GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Length != (GPRS_COM_ACK_Att_Len/0X100))||	
	   (GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_LSB_Length != (GPRS_COM_ACK_Att_Len%0X100)))
	{
		/* ÏûÏ¢Ìå³¤¶ÈÓëÐ­Òé²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* ÅÐ¶ÏÏûÏ¢¼ÓÃÜ·½Ê½ */	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Enc_Mode != GPRS_COM_Enc_Mode)
	{
		/* ÏûÏ¢¼ÓÃÜ·½Ê½ÓëÉèÖÃ²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* ÅÐ¶ÏÏûÏ¢·Ö°ü±êÖ¾ */	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Attribute_HSB_Sub_Flag != GPRS_COM_Sub_Flag)
	{
		/* ÏûÏ¢·Ö°ü±êÖ¾ÓëÉèÖÃ²»Ò»ÖÂ */
		return FALSE;
	}	
	
//	/* ÅÐ¶ÏÏûÏ¢ÖÕ¶ËÐòÁÐºÅ */	
//	if((GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||	
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != )||
//	   (GPRS_Rece_Reg.GPRS_IR_FIX.SN_NO[] != ))	
//	{
//		/* ÏûÏ¢ÖÕ¶ËÐòÁÐºÅÓëÒÇÆ÷SNºÅ²»Ò»ÖÂ */
//		return FALSE;	
//	}
	
	//ÕâÀïµÄ ¡°Ó¦´ðÁ÷Ë®ºÅ¡± ºÍ ¡°Ó¦´ð ID¡±Ã»ÓÐÊ²Ã´ÊµÖÊÐÔµÄÒâÒåËùÒÔ¾Í²»½øÐÐÅÐ¶Ï
	
	/* ÅÐ¶ÏÏûÏ¢Ó¦´ð½á¹û */
	if(GPRS_Rece_Reg.GPRS_IR_FIX.ACK_Result != GPRS_Rece_ACK_Result_Success)
	{
		/* ÏûÏ¢Ó¦´ðÒì³£ */
		//¿ÉÒÔÉèÖÃ²é¿´Òì³£ÀàÐÍ
		//ÕâÀï¾ÍÓÃÒ»¸önopÀ´¶¨Î»£¬¾ßÌåÓÃ·ÂÕæÀ´²é¿´½á¹û£¬¼ÙÈçÐèÒª´òÓ¡»òÏÔÊ¾³öÀ´¾ÍµÄ×öÏàÓ¦µÄº¯Êý¹¦ÄÜ¡£
		__nop();
		return FALSE;
	}
	
	/* Çå³ý Ð£Ñé Êý¾Ý¼Ä´æÆ÷ */
	check_code = 0;
	/* ÅÐ¶ÏÊý¾ÝÐ£Ñé */
	/* ¼ÆËã ÄæÏò×ªÒåºóµÄÍøÂçÐòÊý¾ÝµÄÐ£Ñé  */
	for(num = 1;num < (GPRS_Rece_Network_Data_Len-2); num++)
	{
		/* ÕâÀïµÄÐ£Ñé·½Ê½:¶ÔÓÐÐ§µÄÊý¾Ý¶ÎÈ¡Òì»ò */
		check_code ^= GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer[num];
	}
	
	if(GPRS_Rece_Reg.GPRS_IR_FIX.Check_Code != check_code)
	{
		/* ÏûÏ¢Êý¾ÝÐ£ÑéÒì³£ */
		return FALSE;
	}
	
	/* ÍøÂçÐòÊý¾ÝÕý³££¬Êý¾Ý·¢ËÍ³É¹¦¡£ */
	return TRUE;
	
#undef GPRS_Rece_Network_Data_Len	
#undef GPRS_Rece_ACK_Result_Success
#undef GPRS_Rece_ACK_Result_Failure
#undef GPRS_Rece_ACK_Result_Error
#undef GPRS_Rece_ACK_ON_Format
#undef GPRS_Rece_ACK_Alarm_Confirm
}
#elif (AppGPRSComProtocol == 0x01)//½âÎöHTTPÐ­Òé·µ»ØÀ´µÄÊý¾Ý
static BOOL GPRS_Parsing_Network_Data(void)
{
	/* Êý¾ÝÖ¸Õë */
	char * datap;
		
	/* Çå³ýÊý¾Ý»º´æ */
	memset(GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,0x00,GPRS_Rece_Buffer_Size);
	/* ¿½±´½ÓÊÕÊý¾Ý */
	strcpy((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,(char*)GPRS_Rece_Sensor_Data_Buffer);
	
	
	/* ²éÑ¯HTTP´«Êä³É¹¦×Ö·û´® */
	datap = strstr((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,"HTTP/1.1 200 OK");
	/* ÅÐ¶ÏHTTP´«ÊäÊÇ·ñ³É¹¦ */
	if(NULL == datap)
	{
		/* ´«ÊäÊ§°Ü */
		return FALSE;
	}
	
	/* ²éÑ¯Êý¾ÝÆ½Ì¨³É¹¦½ÓÊÕ×Ö·û´® */
	datap = strstr((char*)GPRS_Rece_Reg.GSM_GPRS_Rece_Buffer,"\"code\":\"0000\"");
	/* ÅÐ¶ÏÊý¾ÝÆ½Ì¨ÊÇ·ñ³É¹¦½ÓÊÕÊý¾Ý */
	if(NULL == datap)
	{
		/* ´«ÊäÊ§°Ü */
		return FALSE;
	}

	return TRUE;
}
#elif (AppGPRSComProtocol == 0x02)//½âÎö¿ª·ÅÐÔÐ­Òé·µ»ØÀ´µÄÊý¾Ý
static BOOL GPRS_Parsing_Network_Data(void)
{
	
	/* Ã»ÓÐÀúÊ·Êý¾Ý */
	return ComProtZDE_ParseData(GPRS_Rece_Sensor_Data_Buffer,
		   &GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len);
}
#elif (AppGPRSComProtocol == 0x04)//½âÎö½­ËÕÊ¡ÃâÒß¹æ»®Êý¾ÝÖÐÐÄÐ­Òé·µ»ØÀ´µÄÊý¾Ý
static const u8 JS_DL_CMD_Buffer[6]={0x55,0x7A,0x04 ,0x85 ,0x00 ,0x00 };

static BOOL GPRS_Parsing_Network_Data(void)
{
	 if( GPRS_Rece_Sensor_Data_Buffer[0]!=JS_DL_CMD_Buffer[0]  || \
		   GPRS_Rece_Sensor_Data_Buffer[1]!=JS_DL_CMD_Buffer[1]  || \
	     GPRS_Rece_Sensor_Data_Buffer[2]!=JS_DL_CMD_Buffer[2]  || \
		   GPRS_Rece_Sensor_Data_Buffer[3]!=JS_DL_CMD_Buffer[3]  || \
	     GPRS_Rece_Sensor_Data_Buffer[4]!=JS_DL_CMD_Buffer[4]  || \
		   GPRS_Rece_Sensor_Data_Buffer[5]!=JS_DL_CMD_Buffer[5] )
{
	
			 Log_printf("·þÎñÆ÷½âÎöÉè±¸ÉÏ´«µÄÊý¾ÝÊ§°Ü£¡£¡£¡\n");

        return FALSE;//·þÎñÆ÷½âÎöÊý¾ÝÊ§°Ü

}
	 return TRUE;
}
#else
static BOOL GPRS_Parsing_Network_Data(void)
{
	__NOP();
	return TRUE;
}
#endif
#endif

/*
*********************************************************************************************************
*	º¯ Êý Ãû: GSM_Signal_Strength
*	¹¦ÄÜËµÃ÷: ¸ù¾ÝÏÖÔÚµÄÐÅºÅÇ¿¶È£¬Á¿»¯ÏÔÊ¾¡£
*	ÐÎ    ²Î£ºsignal_num :µ±Ç° µÄ GSM Ä£¿éµÄÐÅºÅÇ¿¶ÈÐÅÏ¢¡£
*			 max_signal :×î´óµÄÕý³£ÐÅºÅÇ¿¶È¡£
*			 sub		:ÐÅºÅÏ¸·Ö¡£
*	·µ »Ø Öµ: ÔÚ²Ëµ¥½çÃæÉÏÏÔÊ¾µÄÐÅºÅ¡£
*********************************************************************************************************
*/
u8 GSM_Signal_Strength(int* signal_num)
{
/* ÏÔÊ¾ÐÅºÅÇ¿¶ÈµÄÇø¼äÉèÖÃ */
//0-5:0¸ñ
#define GPRS_Signal_Strength_0		(5u)
//6-8:1¸ñ
#define GPRS_Signal_Strength_1		(8u)
//9-11:2¸ñ
#define GPRS_Signal_Strength_2		(11u)	
//12-14:3¸ñ
#define GPRS_Signal_Strength_3		(14u)		
//15-31:4¸ñ
#define GPRS_Signal_Strength_4		(15u)
/* GPRS ×î´óÐÅºÅÇ¿¶È */	
#define GPRS_Max_Signal				(31u)
	
	
	
	if((*signal_num > GPRS_Max_Signal)||(*signal_num <= 0))
	{
		/* ÐÅºÅ ¼ì²âÊ§°Ü */
		/* µ±Ç°ÐÅºÅÇ¿¶È£º0¸ñ */
		return 0;
	}

	/* ÅÐ¶Ïµ±Ç°ÐÅºÅÇ¿¶È */
	if(*signal_num <= GPRS_Signal_Strength_0)
	{
		/* µ±Ç°ÐÅºÅÇ¿¶È£º0¸ñ */
		return 0;
	}else if((*signal_num > GPRS_Signal_Strength_0)&&(*signal_num <= GPRS_Signal_Strength_1))
	{
		/* µ±Ç°ÐÅºÅÇ¿¶È£º1¸ñ */
		return 1;
	}else if((*signal_num > GPRS_Signal_Strength_1)&&(*signal_num <= GPRS_Signal_Strength_2))
	{
		/* µ±Ç°ÐÅºÅÇ¿¶È£º2¸ñ */
		return 2;
	}else if((*signal_num > GPRS_Signal_Strength_2)&&(*signal_num <= GPRS_Signal_Strength_3))
	{
		/* µ±Ç°ÐÅºÅÇ¿¶È£º3¸ñ */
		return 3;
	}else if((*signal_num > GPRS_Signal_Strength_3)&&(*signal_num <= GPRS_Max_Signal))
	{
		/* µ±Ç°ÐÅºÅÇ¿¶È£º4¸ñ */
		return 4;
	}else
	{
		/* µ±Ç°ÐÅºÅÇ¿¶È£º0¸ñ */
		return 0;
	}
	
#undef GPRS_Signal_Strength_0
#undef GPRS_Signal_Strength_1
#undef GPRS_Signal_Strength_2
#undef GPRS_Signal_Strength_3	
#undef GPRS_Signal_Strength_4
#undef GPRS_Max_Signal
}

/************************************************************************************************/
/* º¯ÊýÃû	: GSMGetSignalStrength										 	 					*/
/* ÊäÈë  	: ÎÞ			 																	*/
/* Êä³ö		: GSMÐÅºÅÇ¿¶È																		*/
/* ×÷ÓÃ	 	: ¶ÁÈ¡µ±Ç°GSMÐÅºÅÇ¿¶È					 											*/
/* ´´½¨ÈÕÆÚ	: 2017/02/23																		*/
/* Èí¼þ°æ±¾	: V0																				*/
/************************************************************************************************/
uint8_t GSMGetSignalStrength(void)
{
	return GPRS_SR_Reg.GPRS_Sign_Stre;
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: GSM_Signal_Check
*	¹¦ÄÜËµÃ÷: ¼ì²âµ±Ç° GSMÐÅºÅ:Èç¹ûÐÅºÅÕý³£¾ÍÏÔÊ¾ÐÅºÅÇ¿¶È£»²»Õý³£¾Í ÖØÐÂÅäÖÃ GSM Ä£¿é(·ÀÖ¹Ä£¿é½øÈëÒì³£µÄ¹¤×÷×´Ì¬)
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void GSM_Signal_Check(void)
{
/* GSM ÐÅºÅÒì³£ */
#define GPRS_Signal_Error		                          	(-1)
/* GPRS ×î´óÐÅºÅÇ¿¶È */	                              
#define GPRS_Max_Signal			                          	(31u)
	                                                    
	/* ÅÐ¶Ï GSMÄ£¿éÅäÖÃ×´Ì¬ */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
		/* Ä£¿éÅäÖÃ³É¹¦ */
		if((GPRS_SR_Reg.GPRS_Sign_Stre >= 0)&&(GPRS_SR_Reg.GPRS_Sign_Stre <= GPRS_Max_Signal))
		{
			/* GSM¹¤×÷Õý³£ */
			/* ÐÅºÅÇ¿¶È ÏÔÊ¾´¦Àí */
			GPRS_SR_Reg.GPRS_Sign_Stre_Disp = GSM_Signal_Strength(&GPRS_SR_Reg.GPRS_Sign_Stre);
		}else if(GPRS_SR_Reg.GPRS_Sign_Stre == GPRS_Signal_Error)//(-1£©
		{
//			/* GSM¹¤×÷²»Õý³£ */
			
			/* ÉèÖÃ ÏÔÊ¾µÄ ÐÅºÅÇ¿¶È:0 */
//			GSM_Tab.Signal_Display = 0;
//			/* ÐÞ¸Ä GSMÄ£¿é×´Ì¬:Ê§°Ü */
//			GSM_Tab.GSM_State = FALSE;
//			/* ÐèÒª ÖØÐÂ ÅäÖÃGSMÄ£¿é */
//			GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
//			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//			{
//				/* GSMÄ£¿éÐÅºÅÇ¿¶ÈÒì³£ */
//				printf("\rGSMÄ£¿éÐÅºÅÇ¿¶ÈÒì³£\r\n");
//			}
		}else
		{
			/* Î´Öª×´Ì¬ */
			/* ÉèÖÃ ÏÔÊ¾µÄ ÐÅºÅÇ¿¶È:0 */
//			GSM_Tab.Signal_Display = 0;
			
		}
//		/* ÅÐ¶Ïµ¥Ç°¸½×Å×´Ì¬ */
//		if(GPRS_SR_Reg.GPRS_Link_State == FALSE)
//		{
//			/* ¸½×Å Ê§°Ü */
//			/* Á¬½Ó Ê§°Ü Ê±¼ä */
//			if(GSM_GPRS_Link_Fail_Num >= 300)
//			{
//				/* Á¬½ÓÊ±¼ä³¬³ö ÉèÖÃÖµ ÖØÐÂ ÅäÖÃGSMÄ£¿é */
//				GSM_GPRS_Link_Fail_Num = 0;
//				/* ÉèÖÃ ÏÔÊ¾µÄ ÐÅºÅÇ¿¶È:0 */
//				GSM_Tab.Signal_Display = 0;
//				/* ÐÞ¸Ä GSMÄ£¿é×´Ì¬:Ê§°Ü */
//				GSM_Tab.GSM_State = FALSE;
//				/* ÐèÒª ÖØÐÂ ÅäÖÃGSMÄ£¿é */
//				GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/* GSMÄ£¿é ¸½×Å×´Ì¬ Òì³£ */
//					printf("\rGSMÄ£¿é¸½×Å×´Ì¬Òì³£\r\n");
//				}				
//			}else
//			{
//				/* GSMµÄGPRS¹¦ÄÜÁ¬½ÓÊ§°Ü */
//				GSM_GPRS_Link_Fail_Num++;
//			}
//		}else
//		{
//			/* ¸½×Å ³É¹¦ */
//			if(GSM_GPRS_Link_Fail_Num)
//			{
//				/* Çå³ý ¸½×Å Ê§°Ü Ê±¼ä */
//				GSM_GPRS_Link_Fail_Num = 0;
//			}
//		} 
	}else
	{
		/* Ä£¿é ¡°Î´ÅäÖÃ¡± »ò ¡°ÅäÖÃÊ§°Ü¡± */
		/* ÉèÖÃÐÅºÅÇ¿¶È Îª 0 */
		GPRS_SR_Reg.GPRS_Sign_Stre = 0;
		/* ÉèÖÃ ¸½×Å×´Ì¬ Îª Î´Á¬½Ó */
		GPRS_SR_Reg.GPRS_Link_State = FALSE;
		/* ÐèÒª ÖØÐÂ ÅäÖÃGSMÄ£¿é */
		GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Init_GSM_Module_EVENT);
	}
	

#undef GPRS_Signal_Error
#undef GPRS_Max_Signal
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: GSM_Link_Check
*	¹¦ÄÜËµÃ÷: GPRS ¸½×Å×´Ì¬ ¼ì²é 
*	ÐÎ    ²Î£ºÎÞ
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void GSM_Link_Check(void)
{

	/* ÅÐ¶Ï GSMÄ£¿éÅäÖÃ×´Ì¬ */
	if(GPRS_SR_Reg.GPRS_Work_State == TRUE)
	{
	
	
	}

}
#if (GSM_Module_Type==0x00) //SIM800CÄ£¿é	
/************************************************************************************************/
/* º¯ÊýÃû	£ºGSM_Send_SMS																		*/
/* ÊäÈë		£º¶ÌÐÅ·¢ËÍµÄÊÖ»úºÅÂë																	*/
/* Êä³ö		£ºÎÞ 																				*/
/* ×÷ÓÃ		£º·¢ËÍ¶ÌÐÅÏ¢						 									 		 		*/
/* ´´½¨ÈÕÆÚ	£º2015/04/10																		*/
/************************************************************************************************/
static void GSM_SMS_Send(INSTRU_GSM* gprs_inf)
{

/* ÅÐ¶ÏÓÃ»§ÊýÁ¿ */
#if SMS_User_Number == 0x01	         //¶ÌÐÅ±¨¾¯ºÅÂë¸öÊý
	/* ÅÐ¶Ï ¶ÌÐÅºÅÂë ÊÇ·ñÊÇÐèÒª·¢ËÍ±¨¾¯µÄºÅÂë */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))//ºÅÂëÎª¿ÕÊ±²»·¢ËÍ¶ÌÐÅ
	{
		/* Õâ¸öºÅÂë²»·¢ËÍ¶ÌÐÅ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* ·¢ËÍ ÓÃ»§1 µÄ¶ÌÐÅ */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//·¢ËÍºÅÂë1¶ÌÐÅº¯Êý
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//·¢ËÍºÅÂë1¶ÌÐÅº¯Êý
		 if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅ³É¹¦£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅÊ§°Ü£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
		
	}
#elif SMS_User_Number == 0x02        //¶ÌÐÅ±¨¾¯ºÅÂë¸öÊý
	/* ÅÐ¶Ï ¶ÌÐÅºÅÂë ÊÇ·ñÊÇÐèÒª·¢ËÍ±¨¾¯µÄºÅÂë */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))
	{
		/* Õâ¸öºÅÂë²»·¢ËÍ¶ÌÐÅ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* ·¢ËÍ ÓÃ»§1 µÄ¶ÌÐÅ */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);		
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//·¢ËÍºÅÂë1¶ÌÐÅº¯Êý
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅ³É¹¦£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅÊ§°Ü£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}
	
	/* ÅÐ¶Ï ¶ÌÐÅºÅÂë ÊÇ·ñÊÇÐèÒª·¢ËÍ±¨¾¯µÄºÅÂë */
	if((gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11 == 0))
	{
		/* Õâ¸öºÅÂë²»·¢ËÍ¶ÌÐÅ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = TRUE;
	}else
	{
		/* ·¢ËÍ ÓÃ»§2 µÄ¶ÌÐÅ */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11+'0';
////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//·¢ËÍºÅÂë2¶ÌÐÅº¯Êý
		 if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]){
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅ³É¹¦£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅÊ§°Ü£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}

#elif SMS_User_Number == 0x03      //¶ÌÐÅ±¨¾¯ºÅÂë¸öÊý
	/* ÅÐ¶Ï ¶ÌÐÅºÅÂë ÊÇ·ñÊÇÐèÒª·¢ËÍ±¨¾¯µÄºÅÂë */
	if((gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11 == 0))
	{
		/* Õâ¸öºÅÂë²»·¢ËÍ¶ÌÐÅ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = TRUE;
	}else
	{
		/* ·¢ËÍ ÓÃ»§1 µÄ¶ÌÐÅ */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO1.Serial_Number_Number.Number_11+'0';
//		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//·¢ËÍ¶ÌÐÅ±¨¾¯ºÅÂëº¯Êý
	  GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//·¢ËÍºÅÂë1¶ÌÐÅº¯Êý	
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[0]){
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅ³É¹¦£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅÊ§°Ü£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
		
	}                                                                         //¶ÌÐÅ±¨¾¯ºÅÂë        //¶ÌÐÅ±¨¾¯ÄÚÈÝ
	
	/* ÅÐ¶Ï ¶ÌÐÅºÅÂë ÊÇ·ñÊÇÐèÒª·¢ËÍ±¨¾¯µÄºÅÂë */
	if((gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11 == 0))
	{
		/* Õâ¸öºÅÂë²»·¢ËÍ¶ÌÐÅ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = TRUE;
	}else
	{
		/* ·¢ËÍ ÓÃ»§2 µÄ¶ÌÐÅ */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO2.Serial_Number_Number.Number_11+'0';
////////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);//·¢ËÍ¶ÌÐÅ±¨¾¯ºÅÂëº¯Êý
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//·¢ËÍºÅÂë2¶ÌÐÅº¯Êý
			if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[1]){
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅ³É¹¦£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }else{
		  Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅÊ§°Ü£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		 }
	}                                                                                     //¶ÌÐÅ±¨¾¯ºÅÂë         //¶ÌÐÅ±¨¾¯ÄÚÈÝ
	
	/* ÅÐ¶Ï ¶ÌÐÅºÅÂë ÊÇ·ñÊÇÐèÒª·¢ËÍ±¨¾¯µÄºÅÂë */
	if((gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_1 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_2 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_3 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_4 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_5 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_6 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_7 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_8 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_9 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_10 == 0)&&
		(gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_11 == 0))
	{
		/* Õâ¸öºÅÂë²»·¢ËÍ¶ÌÐÅ */
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = TRUE;
	}else
	{
		/* ·¢ËÍ ÓÃ»§3 µÄ¶ÌÐÅ */
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[0] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_1+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[1] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_2+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[2] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_3+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[3] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_4+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[4] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_5+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[5] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_6+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[6] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_7+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[7] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_8+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[8] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_9+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[9] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_10+'0';
		GSM_SMS_DR_Reg.GSM_SMS_User_NO[10] = gprs_inf->Inf_State.Alarm_SMS_NO3.Serial_Number_Number.Number_11+'0';
//////		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2] = (BOOL)gsm_send_sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);//·¢ËÍ¶ÌÐÅ±¨¾¯ºÅÂëº¯Êý
		GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2]=(BOOL)gsm_handle.sim800c.pGSM_Send_Sms(GSM_SMS_DR_Reg.GSM_SMS_User_NO,SMS_UTF8);	//·¢ËÍºÅÂë3¶ÌÐÅº¯Êý
	                                                                                   //¶ÌÐÅ±¨¾¯ºÅÂë              //¶ÌÐÅ±¨¾¯ÄÚÈÝ
		if(GSM_SMS_SR_Reg.GSM_SMS_Send_User_Result[2]){
		    Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅ³É¹¦£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		   }else{
		    Log_printf("Ïò%sÓÃ»§·¢ËÍ±¨¾¯¶ÌÐÅÊ§°Ü£¡£¡£¡\n",GSM_SMS_DR_Reg.GSM_SMS_User_NO);	
		   }
	
	}                                                                                
	
	

#endif
	
	/* ÉèÖÃ ¶ÌÐÅ·¢ËÍÍê³É×´Ì¬ */
	GSM_SMS_SR_Reg.GSM_SMS_Send_State = FALSE;
}

#endif


/*
*********************************************************************************************************
*	º¯ Êý Ãû: 
*	¹¦ÄÜËµÃ÷: ½ÓÊÕÊý¾Ý»º´æ£¬ÅÐ¶ÏÅäÖÃ±íÊÇ·ñÓÐÐÞ¸Ä£¬ÓÐÐÞ¸ÄµÄ»°£¬¸ÄÅäÖÃ±íÀïµÄ¶ÔÓ¦Êý¾Ý
*	´«µÝ²ÎÊý£º*buf£º½ÓÊÕÊý×é»º´æ£¬·Ç¿Õ(ÅäÖÃÐÅÏ¢)
					  len£ºÊý×é»º´æ ³¤¶È
						* Config_Content£ºÅäÖÃ±í ÐÅÏ¢ ½á¹¹Ìå
						Packet_num£ºÊý×é»º´æÀï °üº¬¼¸°üÊý¾Ý!
						* tab£º¸ù¾Ý»º´æÐÞ¸ÄµÄÄÚÈÝ£¬ÐÞ¸ÄÄÚ´æÖÐ ÒÇÆ÷ÅäÖÃ±í
*	·µ »Ø Öµ: ÎÞ __nop(); 
*********************************************************************************************************
*/
void RECBUF_config_change_Deal(char *buf,int len,CONFIG_TAB_CHANGE (*Config_Content)[],u16 Packet_num,INSTRU_CONFIG_INF_TAB* TAB)
{
	u32 data_Length=0;
	u16 j=0,i=0;
	
	//ÀïÃæÓÃµ½µÄÊý×é Òª¿¼ÂÇÏÂÒç³ö±£»¤µÄÎÊÌâ£¡
	for(i=0;i<Packet_num;i++)//Êý×éÖÐ ÓÐ¼¸°üÊý¾Ý(Ð¡°üÊý¾Ý) ´¦Àí¼¸´Î
	{
		/* ¸´ÖÆ ²ÎÊý */
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type = buf[0+data_Length];
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder = buf[2+data_Length]+buf[1+data_Length]*256;
		((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len = buf[4+data_Length]+buf[3+data_Length]*256;
//		memcpy(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR_REG,&buf[0+data_Length],Config_Tab_Chane_Size); 
		data_Length=data_Length+Config_Tab_Chane_Size;//ÀàÐÍ µØÖ· Êý¾Ý³¤¶È 5×Ö½Ú	
		
		if(0x01==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//ÏµÍ³ÐÅÏ¢
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_System.Inf_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}
			data_Length=data_Length+j;
		}
		else if(0x02==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//µçÔ´×´Ì¬ÐÅÏ¢
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Power.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}	  
			data_Length=data_Length+j;			
		}
		else if(0x03==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//ÏµÍ³Ê±ÖÓÐÅÏ¢
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_RTC.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x04==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//GSMÄ£¿éÐÅÏ¢
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_GSM.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}			
			data_Length=data_Length+j;			
		}
		else if(0x05==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//´«¸ÐÆ÷ÐÅÏ¢
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Sensor.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) = 
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x06==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//²Ëµ¥¿ØÖÆÐÅÏ¢,¼´±¨¾¯×Ü¿ª¹ØÐÅÏ¢µØÖ·
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Menu.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
			}				
			data_Length=data_Length+j;
		}
		else if(0x07==((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Type)//´«¸ÐÆ÷Ð£×¼Êý¾Ý±íÐÅÏ¢
		{
			for(j=0;j<(((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Data_Len);j++)
			{
				(*((u8*)(&TAB->Inf_State.Instru_Data_Tab.Inf_State_Space[0] + ((CONFIG_TAB_CHANGE*)Config_Content+i)->CR.Change_Adder + j))) =
				buf[data_Length+j];
      }				
			data_Length=data_Length+j;
		}
		
	}
  
}

/************************************************************************************************/
/* º¯ÊýÃû	: GPRS_Disp_Sign_Stre															 	*/
/* ÊäÈë		: ÎÞ 																				*/
/* Êä³ö		: µ±Ç°GPRSÐÅºÅÇ¿¶È(0-5:0¸ñ;6-8:1¸ñ;9-11:2¸ñ;12-14:3¸ñ;15-31:4¸ñ;)						*/
/* ×÷ÓÃ		: ¸üÐÂÈÎÎñµ±ÖÐµÄ¸÷ÖÖ×´Ì¬ÐÅÏ¢						 									*/
/* ´´½¨ÈÕÆÚ	: 2015/04/07																		*/
/************************************************************************************************/
GSM_GPRS_SR_Struct GPRS_Disp_Sign_Stre(void)
{
	return GPRS_SR_Reg;
}


#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
/************************************************************************************************/
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾ÝÏûÏ¢Í·ÏûÏ¢ID */
#define GPRS_Send_ID_System_Clock_Calibration			(0x0FF0)
/* GPRS½ÓÊÕÊ±ÖÓÐ£×¼Êý¾ÝÏûÏ¢Í·ÏûÏ¢ID */
#define GPRS_Rece_ID_System_Clock_Calibration			(0x8FF0)
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾ÝÏûÐ£Ñé */
#define GPRS_Send_ID_System_Clock_Calibration_Check		(0xFF)
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý¼Ä´æÆ÷´óÐ¡ */
#define GPRS_Send_System_Clock_Calibration_Buffer_Size	(15u)

/************************************************************************************************/
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý¼Ä´æÆ÷ */
static const u8 GPRS_Send_System_Clock_Calibration_Buffer[GPRS_Send_System_Clock_Calibration_Buffer_Size] = 
{
	/* Í· ±êÊ¶Î» */ 				//(1 Byte)
	GPRS_COM_Identify,
	/* Í¨ÐÅ ÏûÏ¢Í· */			//
	/* ÏûÏ¢ ID(ÃüÁî) */			//(2 Byte)
	GPRS_Send_ID_System_Clock_Calibration/0X100,
	GPRS_Send_ID_System_Clock_Calibration%0X100,
	/* ÏûÏ¢Ìå ÊôÐÔ */			//(2 Byte)
	0x00,0x00,
	/* ÖÕ¶ËÐòÁÐºÅ(SNºÅ): */		//(6 Byte)
	0X00,0X00,0X00,0X00,0X00,0X00,
	/* ÏûÏ¢ Á÷Ë®ºÅ */			//(2 Byte)
	GPRS_COM_Serial_Num,GPRS_COM_Serial_Num,
	/* ÏûÏ¢Ð£Ñé */				//(1 Byte)
	GPRS_Send_ID_System_Clock_Calibration_Check,
	/* Î² ±êÊ¶Î» */				//(1 Byte)
	GPRS_COM_Identify,
};
/************************************************************************************************/
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý»º´æ */
static u8 GPRS_Rece_System_Clock_Calibration_Buffer[GPRS_Rece_System_Clock_Calibration_Buffer_Size];
/************************************************************************************************/
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý¿ØÖÆ¼Ä´æÆ÷ */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_System_Clock_Calibration_CR_Reg = 
{
	/* GPRS ½ÓÊÕ ´«¸ÐÆ÷ Êý¾ÝÊýÁ¿ */
	GPRS_Rece_System_Clock_Calibration_Buffer_Size,
	/* GPRS ·¢ËÍ ´«¸ÐÆ÷ Êý¾ÝÊýÁ¿ */
	GPRS_Send_System_Clock_Calibration_Buffer_Size,
	
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾ÝÍ¨ÐÅ×´Ì¬£¨TURE:ÕýÔÚ·¢ËÍ£¬FALSE:Ã»ÓÐ·¢ËÍ£© */
	FALSE,
	/* GPRS Ö÷»úÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬£¨TURE:µÈ´ý´¦Àí£¬FALSE:Ã»ÓÐÊý¾Ý´¦Àí£© */
	FALSE,
	/* GPRS ÍøÂçÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬£¨TURE:µÈ´ý´¦Àí£¬FALSE:Ã»ÓÐÊý¾Ý´¦Àí£© */
	FALSE,
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾Ý½á¹û */
	TRUE,
	/* GPRS ½ÓÊÕ´«¸ÐÆ÷Êý¾Ý½á¹û */
	TRUE,	
	/* GPRS ½âÎöÊý¾Ý×´Ì¬   */	
	TRUE,
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾Ý Ê§°Ü´ÎÊý */
	0,
};
/************************************************************************************************/
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý¼Ä´æÆ÷ */
static GSM_GPRS_RECE_CLOCK_CALI_Struct GPRS_Rece_System_Clock_Calibration_DR;
#elif (AppGPRSComProtocol == 0x01)
/* GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý¿ØÖÆ¼Ä´æÆ÷ */
static GSM_GPRS_Send_Sensor_Data_Struct GPRS_Send_System_Clock_Calibration_CR_Reg = 
{
	/* GPRS ½ÓÊÕ ´«¸ÐÆ÷ Êý¾ÝÊýÁ¿ */
	0,
	/* GPRS ·¢ËÍ ´«¸ÐÆ÷ Êý¾ÝÊýÁ¿ */
	0,
	
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾ÝÍ¨ÐÅ×´Ì¬£¨TURE:ÕýÔÚ·¢ËÍ£¬FALSE:Ã»ÓÐ·¢ËÍ£© */
	FALSE,
	/* GPRS Ö÷»úÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬£¨TURE:µÈ´ý´¦Àí£¬FALSE:Ã»ÓÐÊý¾Ý´¦Àí£© */
	FALSE,
	/* GPRS ÍøÂçÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬£¨TURE:µÈ´ý´¦Àí£¬FALSE:Ã»ÓÐÊý¾Ý´¦Àí£© */
	FALSE,
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾Ý½á¹û */
	TRUE,
	/* GPRS ½ÓÊÕ´«¸ÐÆ÷Êý¾Ý½á¹û */
	TRUE,
	/* GPRS ½âÎöÊý¾Ý×´Ì¬   */	
	TRUE,
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾Ý Ê§°Ü´ÎÊý */
	0,
};
#endif
#endif
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x01)||(AppGPRSComProtocol == 0x04))
/* ÏµÍ³Ê±¼ä¸üÐÂÊý¾Ý¼Ä´æÆ÷ */
static Times System_Clock_Updata_DR;
#endif
#endif

#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))
/************************************************************************************************/
/* º¯ÊýÃû	: GPRS_Parsing_System_Clock_Calibration_Data										*/
/* ÊäÈë		: parsing_data_buffer:½ÓÊÕµ½·þÎñÆ÷µÄÊ±ÖÓÊý¾Ý»º´æ¡£									*/
/* Êä³ö		: TRUR:³É¹¦½ÓÊÕ·þÎñÆ÷Ê±ÖÓÊý¾Ý£»FALSE:	·þÎñÆ÷·µ»ØÊý¾ÝÊ±ÖÓÊý¾ÝÒì³£						*/
/* ×÷ÓÃ		: ½âÎö·þÎñÆ÷·µ»ØµÄÊ±¼äÊý¾Ý£¬²¢ÅÐ¶Ï±¾´ÎGPRSÍ¨ÐÅ×´Ì¬¡£							 		*/
/* ´´½¨ÈÕÆÚ	: 2015/06/26																		*/
/************************************************************************************************/
static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer)
{
/* ½ÓÊÕµÄÊý¾Ý³¤¶È */
#define GPRS_Rece_Data_Len				(GPRS_Rece_System_Clock_Calibration_Buffer_Size)
/* ½ÓÊÕµÄÏûÏ¢ÌåÊý¾Ý³¤¶È */
#define GPRS_Rece_Attribute_Length_Len	(8u)
	
	/* Ð£ÑéÂë */
	u8 check_code;
	/* ¼ÆÊýÆ÷ */
	u8 num;
	/* ·þÎñÆ÷·µ»ØµÄÊý¾Ý³¤¶È */
	u8 gprs_rece_data_len;
	
#if	(System_Clock_Enable == 0x01)
	/* ÏµÍ³¸üÐÂÊ±¼äµÄÃë¼Ä´æÆ÷ */
	u32 system_clock_updata_second_b;
#endif
	
	//1¡¢ÄæÏò×ªÒåÊý¾Ý
	/* ÏÈ¶Ô ½ÓÊÕÊý¾Ý ½øÐÐÄæÏò×ªÒå */
	gprs_rece_data_len = GPRS_Network_To_Host_Translator_Data(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_Buffer,
															  parsing_data_buffer,
															  GPRS_Rece_Data_Len);
	
	//2¡¢½âÎöÊý¾Ý
	/* ÅÐ¶ÏÊý¾Ý³¤¶È *//* ÏÖÔÚÓ¦´ðÏûÏ¢³¤¶ÈÊÇ¹Ì¶¨µÄ(20×Ö½Ú) */
	if(gprs_rece_data_len != GPRS_Rece_Data_Len)
	{
		/* ½ÓÊÕµ½µÄÊý¾Ý³¤¶ÈÓëÉè¶¨µÄ²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* ÅÐ¶Ï±êÊ¶Î»Êý¾Ý */
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Start_Iden != GPRS_COM_Identify)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.End_Iden   != GPRS_COM_Identify))
	{
		/* Í·±êÊ¶»òÎ²±êÊ¶ÓëÐ­Òé²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* ÅÐ¶ÏÏûÏ¢ID */
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.ID_Type_HSB != (GPRS_Rece_ID_System_Clock_Calibration/0X100))||	
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.ID_Type_LSB != (GPRS_Rece_ID_System_Clock_Calibration%0X100)))
	{
		/* ½ÓÊÕµÄÏûÏ¢Í·ÓëÐ­Òé²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* ÅÐ¶ÏÏûÏ¢Ìå³¤¶È */	
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Attribute_HSB_Length != (GPRS_Rece_Attribute_Length_Len/0X100))||	
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Attribute_LSB_Length != (GPRS_Rece_Attribute_Length_Len%0X100)))
	{
		/* ÏûÏ¢Ìå³¤¶ÈÓëÐ­Òé²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* ÅÐ¶ÏÏûÏ¢ÖÕ¶ËÐòÁÐºÅ */	
	if((GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[0] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[1] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[2] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[3] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[4] != 0)||
	   (GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.SN_NO[5] != 0))
	{
		/* ÏûÏ¢ÖÕ¶ËÐòÁÐºÅÓëÒÇÆ÷SNºÅ²»Ò»ÖÂ */
		return FALSE;
	}
	
	/* Çå³ý Ð£Ñé Êý¾Ý¼Ä´æÆ÷ */
	check_code = 0;
	/* ÅÐ¶ÏÊý¾ÝÐ£Ñé */
	/* ¼ÆËã ÄæÏò×ªÒåºóµÄÍøÂçÐòÊý¾ÝµÄÐ£Ñé  */
	for(num = 1;num < (GPRS_Rece_Data_Len-2); num++)
	{
		/* ÕâÀïµÄÐ£Ñé·½Ê½:¶ÔÓÐÐ§µÄÊý¾Ý¶ÎÈ¡Òì»ò */
		check_code ^= GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_Buffer[num];
	}
	
	if(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Check_Code != check_code)
	{
		/* ÏûÏ¢Êý¾ÝÐ£ÑéÒì³£ */
		return FALSE;
	}
	
	/* Êý¾ÝÈ«²¿Õý³£»ñÈ¡·þÎñÆ÷Ê±¼äÊý¾Ý */
	System_Clock_Updata_DR.Year		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Year) + 2000;
	System_Clock_Updata_DR.Month 	= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Month);
	System_Clock_Updata_DR.Day 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Day);
	System_Clock_Updata_DR.Hour 	= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Hour);
	System_Clock_Updata_DR.Min 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Min);
	System_Clock_Updata_DR.Sec 		= BCD_To_HEX(GPRS_Rece_System_Clock_Calibration_DR.GSM_GPRS_Rece_IR.Server_Sec);
	

	/* ÉèÖÃÏµÍ³Ê±¼ä³¬Ç°Á¿ */
	/* ½«µ±Ç°µÄÊ±¼äÊý¾Ý×ª»»³ÉÃëÊý¾Ý¡£ */
	system_clock_updata_second_b = RTC_Date_Time_To_Second(&System_Clock_Updata_DR);
	//ÏÖÔÚÓÉÓÚ·þÎñÆ÷Èí¼þ´æÔÚÊý¾Ý½ÓÊÕ³¬Ê±±£»¤¹¦ÄÜ£¬»áµ¼ÖÂÐí¶àÕæÊµÊý¾ÝÎÞ·¨´æ´¢µ½Æ½Ì¨ÉÏÃæ£¬Òò´ËÔÚÓë·þÎñÆ÷Æ½Ì¨½øÐÐÊ±¼äÐ£×¼Ê±½«Ê±¼ä³¬Ç°·þÎñÆ÷Æ½Ì¨,
	//ÕâÑù¾Í¿ÉÒÔ³¬Ç°·þÎñÆ÷±£»¤Êý¾ÝÇ°½«Êý¾ÝÉÏ´«µ½ÔÆÆ½Ì¨ÉÏ£¬ÕâÑù¿ÉÒÔ¼õÉÙ´óÁ¿µÄ±£»¤Êý¾Ý¡£
#if	(System_Clock_Enable == 0x01)
	/* ÉèÖÃÏµÍ³³¬Ç°Ê±¼ä */
	system_clock_updata_second_b += System_Time_Lead_Num;
#endif
	/* ½«ÃëÊ±¼ä×ª»»³ÉÊµ¼ùÊ±¼ä. */
	RTC_Second_To_Date_Time(system_clock_updata_second_b,&System_Clock_Updata_DR);

	
	/* ÍøÂçÐòÊý¾ÝÕý³££¬Êý¾Ý·¢ËÍ³É¹¦¡£ */
	return TRUE;
	
/* ½ÓÊÕµÄÊý¾Ý³¤¶È */
#undef GPRS_Rece_Data_Len
/* ½ÓÊÕµÄÏûÏ¢ÌåÊý¾Ý³¤¶È */	
#undef GPRS_Rece_Attribute_Length_Len
}

#elif (AppGPRSComProtocol == 0x01)
/* httpÊ±¼äÐ£×¼½á¹¹ */
typedef struct 
{
	/* ¹Ì¶¨×Ö·û´®£¨Date: Tue, 07 Jun 2016 01:16:00 GMT£©  */
	char Date[4];
	/* ¹Ì¶¨×Ö·û´®¡°: ¡± */
	char Sign_1[2];
	/* ÐÇÆÚ */
	char Week[3];
	/* ¹Ì¶¨×Ö·û´®¡°, ¡± */
	char Sign_2[2];
	/* ÈÕÆÚ */
	char Day[2];
	/* ¹Ì¶¨×Ö·û´®¡° ¡± */
	char Sign_3;
	/* ÔÂ·Ý */
	char Month[3];
	/* ¹Ì¶¨×Ö·û´®¡° ¡± */
	char Sign_4;
	/* Äê·Ý */
	char Year[4];
	/* ¹Ì¶¨×Ö·û´®¡° ¡± */
	char Sign_5;
	/* Ê± */
	char Hour[2];
	/* ¹Ì¶¨×Ö·û´®¡°:¡± */
	char Sign_6;
	/* ·Ö */
	char Min[2];
	/* ¹Ì¶¨×Ö·û´®¡°:¡± */
	char Sign_7;
	/* Ãë */
	char Sec[2];
	/* ¹Ì¶¨×Ö·û´®¡°:¡± */
	char Sign_8;
	/* ¸ñÁÖÍþÖÎÊ±¼ä¡°GMT¡± */
	char Sign_9[3];
}HTTPClockCalibrationStruct;



static BOOL GPRS_Parsing_System_Clock_Calibration_Data(u8 *parsing_data_buffer)
{
	/* Êý¾ÝÖ¸Õë */
	char * datap;
	/* ¼ÆÊýÆ÷ */
	u8 num;
	char   *stop_at;
	
	/* ÐÇÆÚ */
	char* weekData[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun",};
	/* ÔÂ·Ý */
	char* monthData[] = {"Jan" ,"Feb" ,"Mar" ,"Apr" ,"May" ,"Jun",
						 "Jul" ,"Aug" ,"Sep" ,"Oct" ,"Nov" ,"Dec",};
	
	/* ÈÕÆÚÊ±¼ä½á¹¹ */
	HTTPClockCalibrationStruct * timestamp;
						 

	/* ÏµÍ³¸üÐÂÊ±¼äµÄÃë¼Ä´æÆ÷ */
	u32 system_clock_updata_second_b;
					 
	
	/* ²éÑ¯HTTP´«Êä³É¹¦×Ö·û´® */
	datap = strstr((char *)parsing_data_buffer,"HTTP/1.1 200 OK");
	/* ÅÐ¶ÏHTTP´«ÊäÊÇ·ñ³É¹¦ */
	if(NULL == datap)
	{
		/* ´«ÊäÊ§°Ü */
		return FALSE;
	}
	
	/* ²éÑ¯Êý¾ÝÆ½Ì¨³É¹¦½ÓÊÕ×Ö·û´® */
	datap = strstr((char *)parsing_data_buffer,"\"code\":\"0000\"");
	/* ÅÐ¶ÏÊý¾ÝÆ½Ì¨ÊÇ·ñ³É¹¦½ÓÊÕÊý¾Ý */
	if(NULL == datap)
	{
		/* ´«ÊäÊ§°Ü */
		return FALSE;
	}
	
	{
		/* ²éÑ¯Êý¾ÝÆ½Ì¨³É¹¦½ÓÊÕ×Ö·û´® */
		datap = strstr((char *)parsing_data_buffer,"Date: ");
		/* ÅÐ¶ÏÊý¾ÝÆ½Ì¨ÊÇ·ñ³É¹¦½ÓÊÕÊý¾Ý */
		if(NULL == datap)
		{
			/* ´«ÊäÊ§°Ü */
			return FALSE;
		}
		
		/* ÉèÖÃÊ±¼ä´Á */
		timestamp = (HTTPClockCalibrationStruct *)datap;
		
		/* ÉèÖÃÐÇÆÚ */
		/* Êý¾ÝÈ«²¿Õý³£»ñÈ¡·þÎñÆ÷Ê±¼äÊý¾Ý */
		for(num = 0;num < 7;num++)
		{
			/* ²éÑ¯Êý¾ÝÆ½Ì¨³É¹¦½ÓÊÕ×Ö·û´® */
			datap = strstr((char *)parsing_data_buffer,weekData[num]);
			/* ÅÐ¶ÏÊý¾ÝÆ½Ì¨ÊÇ·ñ³É¹¦½ÓÊÕÊý¾Ý */
			if(NULL != datap)
			{
				/* ÉèÖÃÐÇÆÚ */
				System_Clock_Updata_DR.Week = num+1;
				break;
			}
		}
		
		/* ÅÐ¶ÏÐÇÆÚÊýÊÇ·ñ´óÓÚµÈÓÚ7 */
		if(num >= 7)
		{
			/* ´«ÊäÊ§°Ü */
			return FALSE;
		}
		
		/* ÉèÖÃÔÂ·Ý */
		/* Êý¾ÝÈ«²¿Õý³£»ñÈ¡·þÎñÆ÷Ê±¼äÊý¾Ý */
		for(num = 0;num < 12;num++)
		{
			/* ²éÑ¯Êý¾ÝÆ½Ì¨³É¹¦½ÓÊÕ×Ö·û´® */
			datap = strstr((char *)parsing_data_buffer,monthData[num]);
			/* ÅÐ¶ÏÊý¾ÝÆ½Ì¨ÊÇ·ñ³É¹¦½ÓÊÕÊý¾Ý */
			if(NULL != datap)
			{
				/* ÉèÖÃÐÇÆÚ */
				System_Clock_Updata_DR.Month = num+1;
				break;
			}
		}
		
		/* ÅÐ¶ÏÐÇÆÚÊýÊÇ·ñ´óÓÚµÈÓÚ7 */
		if(num >= 12)
		{
			/* ´«ÊäÊ§°Ü */
			return FALSE;
		}
		
		/* ÉèÖÃÈÕÆÚ */
		stop_at = &timestamp->Sign_3;

		System_Clock_Updata_DR.Day = strtoul(timestamp->Day,&stop_at,10);
		
		/* ÉèÖÃÄê·Ý */
		stop_at = &timestamp->Sign_5;

		System_Clock_Updata_DR.Year = strtoul(timestamp->Year,&stop_at,10);
		
		/* ÉèÖÃ"Ê±" */
		stop_at = &timestamp->Sign_6;

		System_Clock_Updata_DR.Hour = strtoul(timestamp->Hour,&stop_at,10);
		
		/* ÉèÖÃ"·Ö" */
		stop_at = &timestamp->Sign_7;	

		System_Clock_Updata_DR.Min 	= strtoul(timestamp->Min,&stop_at,10);
		
		/* ÉèÖÃ"Ãë" */
		stop_at = &timestamp->Sign_8;

		System_Clock_Updata_DR.Sec 	= strtoul(timestamp->Sec,&stop_at,10);
	}
	/* Æ½Ì¨½ÓÊÕÊý¾ÝÕý³££¬Ê±¼ä½âÎö */
	

	/* ½«µ±Ç°µÄÊ±¼äÊý¾Ý×ª»»³ÉÃëÊý¾Ý¡£ */
	system_clock_updata_second_b = RTC_Date_Time_To_Second(&System_Clock_Updata_DR);
	//ÏÖÔÚÓÉÓÚ·þÎñÆ÷Èí¼þ´æÔÚÊý¾Ý½ÓÊÕ³¬Ê±±£»¤¹¦ÄÜ£¬»áµ¼ÖÂÐí¶àÕæÊµÊý¾ÝÎÞ·¨´æ´¢µ½Æ½Ì¨ÉÏÃæ£¬Òò´ËÔÚÓë·þÎñÆ÷Æ½Ì¨½øÐÐÊ±¼äÐ£×¼Ê±½«Ê±¼ä³¬Ç°·þÎñÆ÷Æ½Ì¨,
	//ÕâÑù¾Í¿ÉÒÔ³¬Ç°·þÎñÆ÷±£»¤Êý¾ÝÇ°½«Êý¾ÝÉÏ´«µ½ÔÆÆ½Ì¨ÉÏ£¬ÕâÑù¿ÉÒÔ¼õÉÙ´óÁ¿µÄ±£»¤Êý¾Ý¡£
#if	(System_Clock_Enable == 0x01)
	/* ÉèÖÃÏµÍ³Ê±¼ä³¬Ç°Á¿ */
	system_clock_updata_second_b += System_Time_Lead_Num;
#endif
	/* ÓÉÓÚHTTPÓÃµÄÊ±¼äÊÇ¸ñÁÖÍþÖÎÊ±¼äÍí±±¾©Ê±¼ä8¸öÐ¡Ê±£¬Òò´ËÕâÀïÒª²¹ÉÏ8¸öÐ¡Ê± */
	system_clock_updata_second_b += 8*60*60;
	/* ½«ÃëÊ±¼ä×ª»»³ÉÊµ¼ùÊ±¼ä. */
	RTC_Second_To_Date_Time(system_clock_updata_second_b,&System_Clock_Updata_DR);
	
	/* ÍøÂçÐòÊý¾ÝÕý³££¬Êý¾Ý·¢ËÍ³É¹¦¡£ */
	return TRUE;
	
}
#endif
#endif

/************************************************************************************************/
/* º¯ÊýÃû	: GPRS_Send_System_Clock_Calibration_Data											*/
/* ÊäÈë		: ÎÞ 																				*/
/* Êä³ö		: ÎÞ																				*/
/* ×÷ÓÃ		: GPRS·¢ËÍÊ±ÖÓÐ£×¼Êý¾Ý							 									*/
/* ´´½¨ÈÕÆÚ	: 2015/06/26																		*/
/************************************************************************************************/
#if (AppGPRSComProtocolEnable == 0x01)
#if ((AppGPRSComProtocol == 0x00)||(AppGPRSComProtocol == 0x04))//²¿±êÐ­ÒéºÍ
static  void GPRS_Send_System_Clock_Calibration_Data(void)//zwc
{

	//1¡¢·¢ËÍÊý¾Ý
	/* ÉèÖÃ½ÓÊÕ·¢ËÍÊý¾Ý³¤¶È¡£ */
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len = GPRS_Send_System_Clock_Calibration_Buffer_Size;//½ÓÊÕÊý¾Ý³¤¶È
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len = GPRS_Rece_System_Clock_Calibration_Buffer_Size;//·¢ËÍÊý¾Ý³¤¶È

#if	(AppGPRSComProtocol == 0x00)
	/* ·¢ËÍÊ±ÖÓÐ£×¼ÐÅÏ¢   µ±ÊÇ²¿±êÐ­ÒéÊ±Ê±ÖÓÐ£×¼µÄ·þÎñÆ÷ÎªÊý¾Ý·þÎñÆ÷*/
		/* ·¢ËÍÊ±ÖÓÐ£×¼ÐÅÏ¢ */
  GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
   (BOOL)Gsm_gprs_comm(
                       (char*)GPRS_Send_System_Clock_Calibration_Buffer,
  	                   (GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len),
                       (char*)GPRS_Rece_System_Clock_Calibration_Buffer,
                       ((&GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len)),
  								      GSM_GPRS_Data
  				             );
#endif	
#if	(AppGPRSComProtocol == 0x04)
	/* ·¢ËÍÊ±ÖÓÐ£×¼ÐÅÏ¢   µ±ÊÇ½­ËÕÊ¡ÃâÒß¹æ»®Êý¾ÝÖÐÐÄÐ­ÒéÊ±Ê±ÖÓÐ£×¼µÄ·þÎñÆ÷ÎªÅäÖÃ·þÎñÆ÷*/
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
	(BOOL)Gsm_gprs_comm(
	   (char*)GPRS_Send_System_Clock_Calibration_Buffer,
		 GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Send_Sensor_Data_Len,
	   (char*)GPRS_Rece_System_Clock_Calibration_Buffer,
	   ((int*)(&GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_Rece_Sensor_Data_Len)),
		 GSM_GPRS_Config);
#endif	
	//2¡¢½ÓÊÕÊý¾Ý	  
	/* ÅÐ¶Ï Êý¾Ý·¢ËÍ½á¹û */
	if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result == FALSE)
	{
		  /* »Ö¸´ÏµÍ³Ê±ÖÓÐ£×¼×´Ì¬¼Ä´æÆ÷,ÖØÐÂ»ñÈ¡Ê±ÖÓÊý¾Ý */
		  RTC_System_Clock_Calibration_State_Reset();
	}else
	{
		Log_printf("ÏòÊý¾Ý·þÎñÆ÷»ñÈ¡Ê±¼äÐÅÏ¢³É¹¦£¡£¡£¡\r\n");	
		/* Êý¾Ý·¢ËÍ³É¹¦ */
		/* ½âÎö·þÎñÆ÷·µ»ØµÄÊ±¼äÊý¾Ý£¬ÅÐ¶Ï±¾´ÎGPRSÍ¨ÐÅ×´Ì¬¡£ */
		GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_System_Clock_Calibration_Data(GPRS_Rece_System_Clock_Calibration_Buffer);
		/* ÅÐ¶Ï ±¾´ÎGPRSÍ¨ÐÅ ½á¹û */
		if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result == TRUE)
		{
			/* ·þÎñÆ÷½ÓÊÕÊý¾Ý³É¹¦ */
			/* ¸üÐÂRTCÊ±ÖÓ */
			/* ¸´Î» RTC */
			RTC_Init(TRUE);
			/* ÉèÖÃ Ê±¼ä */
			Write_System_Date_Time_Data(&System_Clock_Updata_DR);

		}else
		{
			/* »Ö¸´ÏµÍ³Ê±ÖÓÐ£×¼×´Ì¬¼Ä´æÆ÷,ÖØÐÂ»ñÈ¡Ê±ÖÓÊý¾Ý */
			RTC_System_Clock_Calibration_State_Reset();
		}
	}
	
}
#elif (AppGPRSComProtocol == 0x01)
static void GPRS_Send_System_Clock_Calibration_Data(void)
{
	/* ·¢ËÍÊ±¼äÐ£×¼Êý¾Ý»º´æ */
	char clockCalibrationSendBuf[512];
	/* ½ÓÊÕÊ±¼äÐ£×¼Êý¾Ý»º´æ */
	char clockCalibrationReceBuf[512];
	/* ·¢ËÍÊý¾Ý³¤¶È */
	int sendLen;
	/* ½ÓÊÕÊý¾Ý³¤¶È */
	int receLen;
	/* ÁÙÊ±Ê±¼ä¼Ä´æÆ÷ */
	Times time;
	
	
	/* Çå³ýÊý¾Ý»º´æ */
	memset(clockCalibrationSendBuf,0x00,512);
	memset(clockCalibrationReceBuf,0x00,512);
	memset(((char*)(&time)),0x00,sizeof(Times));
	/* ÉèÖÃÊý¾Ý */

	/* Çå³ýÊý¾Ý½Ó¿Ú²ÎÊý */
	memset((char*)&HTTPPara,0x00,sizeof(HTTPPara));

	/* ÉèÖÃÍø¹Ø±àºÅ */
	sprintf(HTTPPara.ApiPara.GatewayIdNo,"%04X%05d",
	((((*(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[0])) & 0xff00) >> 8)|
	((( *(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[0])) & 0x00ff) << 8)),
	((((*(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[2])) & 0xff00) >> 8)|
	((( *(u16*)(&Instru_Config_Inf_Tab.Inf_State.Instru_System.Inf.Serial_Number.Serial_Number[2])) & 0x00ff) << 8)));
	/* ½«²ÎÊýµÄ²É¼¯Ê±¼ä×ª»»³ÉÃë£¬½«Õâ¸öÊý¾ÝÉèÖÃÎª²ÎÊýÇ©ÃûµÄÊ±¼äÊý¾Ý¡£ */
	HTTPPara.ApiPara.TimeStamp = 0;
	
	/* Çå³ý²ÎÊýÇ©ÃûÊý¾Ý»º´æ¡£ */
	memset(HTTPPara.ApiPara.Sign,0x00,sizeof(HTTPPara.ApiPara.Sign));
	
	/* Î´Ê¹ÓÃGPSÉè±¸ */
	HTTPPara.ApiPara.GPS_State = FALSE;
	/* Çå³ý¾­¶ÈÊý¾Ý *///ÔÝ¶¨0£¬ºóÃæÉèÖÃ³ÉÎÞÏßºÅÊý¾Ý¡£
	HTTPPara.ApiPara.Lng = 0;
	/* Çå³ýÎ³¶ÈÊý¾Ý *///ÔÝ¶¨0£¬ºóÃæÉèÖÃ³ÉÎÞÏßºÅÊý¾Ý¡£
	HTTPPara.ApiPara.Lat = 0;
	
	/* ÉèÖÃ²É¼¯Ê±¼ä */
	time.Year 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Year;
	time.Month	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Month;
	time.Day 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Day;
	time.Hour 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Hour;
	time.Min 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Min;
	time.Sec 	= Instru_Config_Inf_Tab.Inf_State.Instru_RTC.Inf_State.Sec;
	
	/* ÉèÖÃ²É¼¯Ê±¼ä */
	sprintf(HTTPPara.ApiPara.CollectTime,"%04d-%02d-%02d %02d:%02d:%02d",
			time.Year,time.Month,time.Day,time.Hour,time.Min,time.Sec);
	
	/* »ñÈ¡µ±Ç°Ì½Í·ÊýÁ¿ */
	HTTPPara.ApiPara.ProbeNum = 0;

	/* »ñÈ¡Ê±¼äÐ£×¼Êý¾Ý */
	HTTP_Create_Data(clockCalibrationSendBuf);
	
	/* ¼ÆËã·¢ËÍÊý¾Ý³¤¶È */
	sendLen = strlen(clockCalibrationSendBuf);
	/* ÉèÖÃ½ÓÊÕÊý¾Ý³¤¶È */
	receLen = 512;
	
	/* ·¢ËÍÊ±ÖÓÐ£×¼ÐÅÏ¢ */
	GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result = 
	(BOOL)gsm_gprs_comm(
	  (char*)(clockCalibrationSendBuf),
			 (sendLen),
	  (char*)(clockCalibrationReceBuf),
	  ((int*)(&receLen)));

	//2¡¢½ÓÊÕÊý¾Ý	  
	/* ÅÐ¶Ï Êý¾Ý·¢ËÍ½á¹û */
	if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Send_Result == FALSE)
	{
		/* »Ö¸´ÏµÍ³Ê±ÖÓÐ£×¼×´Ì¬¼Ä´æÆ÷,ÖØÐÂ»ñÈ¡Ê±ÖÓÊý¾Ý */
		RTC_System_Clock_Calibration_State_Reset();
	}else
	{
		/* Êý¾Ý·¢ËÍ³É¹¦ */
		/* ½âÎö·þÎñÆ÷·µ»ØµÄÊ±¼äÊý¾Ý£¬ÅÐ¶Ï±¾´ÎGPRSÍ¨ÐÅ×´Ì¬¡£ */
		GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result = GPRS_Parsing_System_Clock_Calibration_Data((u8*)clockCalibrationReceBuf);
		/* ÅÐ¶Ï ±¾´ÎGPRSÍ¨ÐÅ ½á¹û */
		if(GPRS_Send_System_Clock_Calibration_CR_Reg.GPRS_COM_Rece_Result == TRUE)
		{
			/* ·þÎñÆ÷½ÓÊÕÊý¾Ý³É¹¦ */
			/* ¸üÐÂRTCÊ±ÖÓ */
			/* ¸´Î» RTC */
			RTC_Init(TRUE);
			/* ÉèÖÃ Ê±¼ä */
			Write_System_Date_Time_Data(&System_Clock_Updata_DR);
			
		}else
		{
			
			/* »Ö¸´ÏµÍ³Ê±ÖÓÐ£×¼×´Ì¬¼Ä´æÆ÷,ÖØÐÂ»ñÈ¡Ê±ÖÓÊý¾Ý */
			RTC_System_Clock_Calibration_State_Reset();
		}
	}
}
#else
static void GPRS_Send_System_Clock_Calibration_Data(void)
{
	__nop();
}

#endif
#endif
/************************************************************************************************/
/* º¯ÊýÃû	: GPRS_Reset_Sensor_Data_Reg														*/
/* ÊäÈë		: ÎÞ 																				*/
/* Êä³ö		: ÎÞ																				*/
/* ×÷ÓÃ		: ¸´Î»ÎÂÊª¶ÈÊý¾Ý¼Ä´æÆ÷								 									*/
/* ´´½¨ÈÕÆÚ	: 2015/06/26																		*/
/************************************************************************************************/
void GPRS_Reset_Sensor_Data_Reg(void)
{
	
	
	/* GPRS ½ÓÊÕ ´«¸ÐÆ÷ Êý¾ÝÊýÁ¿ */	
	GPRS_Send_Sensor_Data_Reg.GPRS_Rece_Sensor_Data_Len = 0;
	/* GPRS ·¢ËÍ ´«¸ÐÆ÷ Êý¾ÝÊýÁ¿ */
	GPRS_Send_Sensor_Data_Reg.GPRS_Send_Sensor_Data_Len = 0;
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾ÝÍ¨ÐÅ×´Ì¬£¨TURE:ÕýÔÚ·¢ËÍ£¬FALSE:Ã»ÓÐ·¢ËÍ£© */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_State = FALSE;
	/* GPRS Ö÷»úÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬£¨TURE:µÈ´ý´¦Àí£¬FALSE:Ã»ÓÐÊý¾Ý´¦Àí£© */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Host_Waiting_State = FALSE;
	/* GPRS ÍøÂçÐò´«¸ÐÆ÷Êý¾ÝµÈ´ý´¦Àí×´Ì¬£¨TURE:µÈ´ý´¦Àí£¬FALSE:Ã»ÓÐÊý¾Ý´¦Àí£© */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Network_Waiting_State = FALSE;
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾Ý½á¹û */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Send_Result = TRUE;
	/* GPRS ½ÓÊÕ´«¸ÐÆ÷Êý¾Ý½á¹û */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_Rece_Result = TRUE;
	/* GPRS ·¢ËÍ´«¸ÐÆ÷Êý¾Ý Ê§°Ü´ÎÊý */
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_DataFailNum	= 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_ConfigServiceFailNum	= 0;
	GPRS_Send_Sensor_Data_Reg.GPRS_COM_IAPFailNum	= 0;
	
}

/************************************************************************************************/
/* º¯ÊýÃû	: GSM_Config_GPRS_IP																*/
/* ÊäÈë		: string:IP×Ö·û´®																	*/
/* Êä³ö		: ÎÞ 																				*/
/* ×÷ÓÃ		: ÉèÖÃÊý¾Ý·þÎñÆ÷ÍøÂçIP¡£																*/
/* ´´½¨ÈÕÆÚ	: 2016/02/22																		*/
/* ÐÞ¸ÄÈÕÆÚ	: 2016/02/22																		*/
/************************************************************************************************/
BOOL GSM_Config_Network(char * networkstring,NetworkConfigStruct* network)
{

	/* ipÖµ */
	uint16_t ip[4] ={0,0,0,0};
	/* ¼ÆÊýÆ÷ */
	uint32_t num1,num2;
	/* µ±Ç°×Ö·ûÖµ */
	int8_t asciiValue;
	/* ¼ÆËã×Ö·û´®³¤¶È */
	int len = strlen((char*)networkstring);
	
	
	/* Çå³ýÊý¾ÝÄÚÈÝ */
	memset((int8_t*)network,0x00,sizeof(NetworkConfigStruct));
	

	/* ×Ö·û´®³¤¶È´óÓÚ×î´ó³¤¶È */
	if(len > sizeof(network->DomainName))
	{
		/* ´óÓÚÓòÃûÉè¼Æ×î´ó³¤¶È£¬²»ºÏÀí */
		return FALSE;
	}
	
	/* ÅÐ¶Ïµ±Ç°×Ö·û´®²ÎÊýÊÇ·ñºÏ·¨ */
	for(num1 = 0,num2 = 0; num1 < len; num1++)
	{
		/* ÅÐ¶Ïµ±Ç°×Ö·û´® */
		if(networkstring[num1] == '.')
		{
			/*·Ö¸î·û*/
			/* ÉèÖÃ×Ö·ûÎ»ÖÃ */
			num2 ++;
		}else
		{
			/* »ñÈ¡ASCIIÖµ */
			asciiValue = networkstring[num1] - '0';
			/* ÅÐ¶ÏASCIIµÄÖµÊÇ·ñÔÚ0-9·¶Î§ÄÚ */
            if(asciiValue > 9 || asciiValue < 0)
			{
				/* ´æÔÚ·ÇÊý×Ö²ÎÊý£¬ËµÃ÷ÊÇÓòÃû¡£ */
				memcpy(network->DomainName,networkstring,len);
				return FALSE;
			}else
			{
				/* ¼ÆËã */
				ip[num2] = ip[num2] * 10 + asciiValue;
				/* ÅÐ¶ÏIPÖµÊÇ·ñºÏÀí */
				if(ip[num2] > 255)
				{
					/* ·ÇIP¸ñÊ½Êý¾Ý */
					memcpy(network->DomainName,networkstring,len);
					return FALSE;
				}
			}
		}
	}
	
	network->IP.Serial_Number_Number.Address_A = ip[0];
	network->IP.Serial_Number_Number.Address_B = ip[1];
	network->IP.Serial_Number_Number.Address_C = ip[2];
	network->IP.Serial_Number_Number.Address_D = ip[3];

	/* IPÉèÖÃ³É¹¦ */
	return TRUE;
}

/******************* (C) ZHOU *****END OF FILE****/
