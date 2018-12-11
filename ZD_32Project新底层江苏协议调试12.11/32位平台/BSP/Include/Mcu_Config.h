#ifndef __Mcu_Config_H
#define __Mcu_Config_H

#include "stm32f10x.h"



/*************************************************************************************************
									�û��������
**************************************************************************************************/
/* ����汾���: ���汾.�Ӱ汾.�׶ΰ汾 */
/* ���汾��� */
#define Software_Version_NO_Major		   (2u)
/* �Ӱ汾��� */
#define Software_Version_NO_Child		   (1u)
/* �׶ΰ汾��� */
#define Software_Version_NO_Stage		   (5u)

#ifndef Hardware_Version_V131
/* Ӳ���汾���: ���汾.�Ӱ汾.�׶ΰ汾 */
/* ���汾��� */
#define Hardware_Version_NO_Major		   (2u)
/* �Ӱ汾��� */
#define Hardware_Version_NO_Child		   (0u)
/* �׶ΰ汾��� */
#define Hardware_Version_NO_Stage		   (1u)
#else
/* Ӳ���汾���: ���汾.�Ӱ汾.�׶ΰ汾 */
/* ���汾��� */
#define Hardware_Version_NO_Major		   (1u)
/* �Ӱ汾��� */
#define Hardware_Version_NO_Child		   (3u)
/* �׶ΰ汾��� */
#define Hardware_Version_NO_Stage		   (1u)
#endif


/* ����������ݻ����С */
#define Serial_Number_Menu_Buffer_Size 	(12u)
/* �汾��Ϣ�����ݻ����С */
#define Version_Inf_Buffer_Size 				(18u)

/* ��ʪ�� ������ �����ݱ��С */
#define INSTRU_SENSOR_Data_Tab_Size			(8u)

/* ��ӡ���ƺŵ����ݳ��� */
#define Print_Card_NO_Size							(12u)
/* ��ӡ��˾���� */
#define Print_Company_Name_Size					(32u)
/* ��¼ж����ʼ��ַ�����С */
#define Print_Start_upload_Adder_Size   (100u)
/* ��¼ж��������ַ�����С */
#define Print_Stop_upload_Adder_Size    (100u)

//�������ý��ձ�Ĵ�С
#define Instru_Config_Tab_Size	        (7u)


/* IAP ������ز��� */

#define FLASH_ACCESS_WIDTH				      (2)				// Flash��д���(2�ֽڣ�ע��Nand Flash��������д)
// STM32 Flash��ҳ��С(����Flashʱ����ҳΪ��λ)
#define FLASH_PAGE_SIZE					        (2048)			//2K	
// ϵͳĬ�ϵ���ʼROM��RAM��ַ
#define BASE_OF_ROM						          (0x08000000)
#define BASE_OF_RAM						          (0x20000000)

#define SIZE_OF_ROM						          (0x80000)	 	// 512K bytes
#define SIZE_OF_RAM						          (0x10000)		// 64K bytes

////////#define STARTUP_FROM_BOOTLOADER     (0x00)

#ifdef STARTUP_FROM_BOOTLOADER				// Ӧ�ó�����������������bootloader����
	// | 250K App1 | 250K app2| 10K BOOT | 2k env |

	// APP�������� 250K
	#define BLOCK_APPLICATION_START			(BASE_OF_ROM)
	#define BLOCK_APPLICATION_NUM			    (125)									// 125ҳ
	#define BLCOK_APPLICATION_SPACE			(FLASH_PAGE_SIZE*BLOCK_APPLICATION_NUM)	// 250K�ռ�
	
	// ��APP����洢���� 250K
	#define BLOCK_CODESTORE_START			(BLOCK_APPLICATION_START+BLCOK_APPLICATION_SPACE)
	#define BLOCK_CODESTORE_NUM				    (125)									// 125ҳ
	#define BLOCK_CODESTORE_SPACE			(FLASH_PAGE_SIZE*BLOCK_CODESTORE_NUM)	// 250K�ռ�
	
	// Bootloader��������
	#define BLOCK_BOOTLOADER_START			(BLOCK_CODESTORE_START+BLOCK_CODESTORE_SPACE)
	#define BLOCK_BOOTLOADER_NUM			    (5)										// 5ҳ
	#define BLOCK_BOOTLOADER_SPACE			(FLASH_PAGE_SIZE*BLOCK_BOOTLOADER_NUM)	// 10K�ռ�

	// ϵͳ��������	
	#define BLCOK_SYS_PARAM_START			(BLOCK_BOOTLOADER_START+BLOCK_BOOTLOADER_SPACE)
	#define BLCOK_SYS_PARAM_NUM				    (1)										// 1ҳ
	#define BLCOK_SYS_PARAM_SPACE			(FLASH_PAGE_SIZE*BLCOK_SYS_PARAM_NUM)	// 2K�ռ�

#else										// Ӧ�ó�����bootloader����������
	// | 10K boot  | 2k env | 250K app1 | 250K app2|

	// Bootloader�������� 10K
	#define BLOCK_BOOTLOADER_START			(BASE_OF_ROM)
	#define BLOCK_BOOTLOADER_NUM			    (5)										// 5ҳ
	#define BLOCK_BOOTLOADER_SPACE			(FLASH_PAGE_SIZE*BLOCK_BOOTLOADER_NUM)	// 10K�ռ�
	
	// ϵͳ��������	2K
	#define BLCOK_SYS_PARAM_START			(BLOCK_BOOTLOADER_START+BLOCK_BOOTLOADER_SPACE)
	#define BLCOK_SYS_PARAM_NUM				    (1)										// 1ҳ
	#define BLCOK_SYS_PARAM_SPACE			(FLASH_PAGE_SIZE*BLCOK_SYS_PARAM_NUM)	// 2K�ռ�

	// APP�������� 250K
	#define BLOCK_APPLICATION_START			(BLCOK_SYS_PARAM_START+BLCOK_SYS_PARAM_SPACE)
	#define BLOCK_APPLICATION_NUM			    (125)									// 125ҳ
	#define BLCOK_APPLICATION_SPACE			(FLASH_PAGE_SIZE*BLOCK_APPLICATION_NUM)	// 250K�ռ�

	// ��APP����洢���� 250K
	#define BLOCK_CODESTORE_START			(BLOCK_APPLICATION_START+BLCOK_APPLICATION_SPACE)
	#define BLOCK_CODESTORE_NUM				    (125)									// 125ҳ
	#define BLOCK_CODESTORE_SPACE			(FLASH_PAGE_SIZE*BLOCK_CODESTORE_NUM)	// 250K�ռ�

#endif

// flash����������;����





// Ӧ�ó����Ƿ񱻸��µı�־(0x00: δ���£�0x01: �Ѹ���)
#define ENV_APP_UPDATED_START			      (0u)		// �Ƿ����°�������صı�־�ڻ�����������������ʼλ��
#define ENV_APP_UPDATED_SIZE			      (2u)		// �Ƿ����°�������صı�־�Ĵ洢����

/******************************************** ϵͳ�汾�� *******************************************/
// ϵͳSN��
#define ENV_SN_NO_START					(ENV_APP_UPDATED_START+ENV_APP_UPDATED_SIZE)
#define ENV_SN_NO_SIZE					        (6u)


//// ϵͳ����汾��
//#define ENV_VERSION_SOFTWARE_START			(ENV_ERR_DETECT_MOTION_START+ENV_ERR_DETECT_MOTION_SIZE)
//#define ENV_VERSION_SOFTWARE_SIZE			16		
//// ϵͳӲ���汾��			
//#define ENV_VERSION_HARDWARE_START			(ENV_VERSION_SOFTWARE_START+ENV_VERSION_SOFTWARE_SIZE)		
//#define ENV_VERSION_HARDWARE_SIZE			16	 	

/* ���� ϵͳ ��Ϣ �ռ��С */
#define Instru_System_Inf_Space_Size						  (20u)
/* ���� ��Դ ״̬ ��Ϣ �ռ��С */
#define Instru_Power_Inf_State_Space_Size					(12u)
/* ���� ʵʱʱ��RTC ��Ϣ �ռ��С */
#define Instru_RTC_Inf_State_Space_Size						(12u)
/* ���� GSMģ�� ״̬��Ϣ �ռ��С */
#define Instru_GSM_Inf_State_Space_Size						(160u)
/* ���� ������ ״̬��Ϣ �ռ��С */
#define Instru_Sensor_Inf_State_Space_Size				(768u)
/* ���� �˵� ״̬��Ϣ �ռ��С */
#define Instru_Menu_Inf_State_Space_Size					(48u)
/* ���� У׼�� ��Ϣ �ռ��С */
#define Instru_Cal_Tab_Space_Size							    (1024u)
/* ���� ���ñ� �ռ��С(2k�ֽ�) */
#define Instru_Inf_State_Space_Size							  (0X0800)
/* ���� ������ ״̬��Ϣ ʣ��ռ��С */
#define Instru_Inf_State_Surplus_Space_Size	(Instru_Inf_State_Space_Size		-\
											(Instru_System_Inf_Space_Size		+\
											 Instru_Power_Inf_State_Space_Size 	+\
											 Instru_RTC_Inf_State_Space_Size	+\
											 Instru_GSM_Inf_State_Space_Size	+\
											 Instru_Sensor_Inf_State_Space_Size	+\
											 Instru_Menu_Inf_State_Space_Size	+\
											 Instru_Cal_Tab_Space_Size))
									


/*Wifiģ���������*/
#define Wifi_Name_Size					                  (32u)
#define Wifi_Passward_Size					              (32u)
    /*Wifiģ��Mac��ַ��С*/
#define Wifi_Mac_Address_Size                      (32u)
    /*Wifiģ�鱾��IP��С*/
#define Wifi_Local_IP_Size					              (32u)
		/*Wifiģ�����������С*/
#define Wifi_Subnet_Mask_Size					            (32u)
    /*Wifiģ������IP��ַ��С*/
#define Wifi_Gateway_IP_Size					            (32u)
 /* ���� Wifi�̶���Ϣ �ռ��С */                                                 
#define Instru_Wifi_Inf_Space_Size                (100u)





/* ���� ϵͳ�̶���Ϣ �ռ��С */
#define Instru_System_Fix_Inf_Space_Size					 (16u)
/* ���� ��Դ�̶���Ϣ �ռ��С */                    
#define Instru_Power_Fix_Inf_Space_Size						 (16u)
/* ���� �������̶���Ϣ �ռ��С */                  
#define Instru_Sensor_Fix_Inf_Space_Size					 (768u)
/* ���� Fram�̶���Ϣ �ռ��С */                    
#define Instru_Fram_Fix_Inf_Space_Size						 (32u)
/* ���� ���ع̶���Ϣ �ռ��С */                    
#define Instru_Gateway_Fix_Inf_Space_Size					 (16u)
/* ���� Flash�̶���Ϣ �ռ��С */                   
#define Instru_Flash_Fix_Inf_Space_Size						 (48u)
/* ���� ͨ��1�ڹ̶���Ϣ �ռ��С */                 
#define Instru_Com1_Fix_Inf_Space_Size						 (48u)
/* ���� ��ӡ��Ϣ �ռ��С */                        
#define Instru_Print_Fix_Inf_Space_Size						 (888u)
/* ���� Wifi��Ϣ �ռ��С */  
#define Instru_Wifi_Fix_Inf_Space_Size					   (68u)
                                                   
                                                   
/* ���� �̶���Ϣ �ռ��С */				                 
#define Instru_Fix_Inf_Tab_Space_Size						   (0X076C)//������Wifi�������Ƽ�����
/* ���� �̶���Ϣ ʣ��ռ��С */
#define Instru_Fix_Inf_Tab_Surplus_Space_Size	(Instru_Fix_Inf_Tab_Space_Size		-\
												(Instru_System_Fix_Inf_Space_Size	+\
												 Instru_Power_Fix_Inf_Space_Size	+\
												 Instru_Sensor_Fix_Inf_Space_Size	+\
												 Instru_Fram_Fix_Inf_Space_Size		+\
												 Instru_Gateway_Fix_Inf_Space_Size	+\
												 Instru_Flash_Fix_Inf_Space_Size	+\
												 Instru_Com1_Fix_Inf_Space_Size		+\
												 Instru_Print_Fix_Inf_Space_Size))
									
/* ���� ���к� �ռ��С */ 
#define Serial_Number_Buffer_Size			             (8u)
/* ���� �汾�� �ռ��С */
#define Version_Buffer_Size					               (4u)
/* ������SN�ſռ��С */
#define SENSOR_SN_NO_Size					                 (4u)	
/* �ֻ����� �ռ��С */
#define Mobile_No_Buffer_Size				               (8u)
/* ���� ������ ʶ��� �ռ��С */
#define SENSOR_ID_Buffer_Size				               (4u)
/* ����ʱ��� �ռ��С */
#define Workaday_Data_Buffer_Size			             (6u)
/* ���ñ� ��Ϣ �ռ��С */
#define Config_Tab_Chane_Size				               (5u)



/* ���� ��Ϣ ���ñ� ��FRAM�еĴ洢���� */
/* ���� ��Ϣ���ñ�洢��FRAM�� ��ʼ��ַ */
#define Instru_Inf_Tab_Mem_Star 			             (0X1000)
/* ���� ��Ϣ���ñ�洢��FRAM�� �ռ��С�����Ӧ�ø����ñ�Ĵ�Сһ�£������ڿ���FRAM�ռ䲻�㣬�ȵ�������һ��С�ĵط��� */
#define Instru_Inf_Tab_Mem_Size 			             (0X0800)
/* ���� ��Ϣ���ñ�洢��FRAM�� β��ַ */
#define Instru_Inf_Tab_Mem_End 				    (Instru_Inf_Tab_Mem_Star+Instru_Inf_Tab_Mem_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� ϵͳ��Ϣ��ַ */
#define Instru_Inf_Tab_Mem_System_adder		(Instru_Inf_Tab_Mem_Star)
/* ���� ��Ϣ���ñ�洢��FRAM�� ��Դ״̬��Ϣ��ַ */
#define Instru_Inf_Tab_Mem_Power_adder		(Instru_Inf_Tab_Mem_System_adder+Instru_System_Inf_Space_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� ʵʱʱ����Ϣ��ַ */
#define Instru_Inf_Tab_Mem_RTC_adder		  (Instru_Inf_Tab_Mem_Power_adder+Instru_Power_Inf_State_Space_Size)	
/* ���� ��Ϣ���ñ�洢��FRAM�� GSMģ����Ϣ��ַ */
#define Instru_Inf_Tab_Mem_GSM_adder		  (Instru_Inf_Tab_Mem_RTC_adder+Instru_RTC_Inf_State_Space_Size)	
/* ���� ��Ϣ���ñ�洢��FRAM�� ��������Ϣ��ַ */
#define Instru_Inf_Tab_Mem_Sensor_adder		(Instru_Inf_Tab_Mem_GSM_adder+Instru_GSM_Inf_State_Space_Size)	
/* ���� ��Ϣ���ñ�洢��FRAM�� �����ܿ�����Ϣ��ַ */
#define Instru_Inf_Tab_Mem_Menu_adder 		(Instru_Inf_Tab_Mem_Sensor_adder+Instru_Sensor_Inf_State_Space_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� У׼����Ϣ��ַ */
#define Instru_Inf_Tab_Mem_Cal_adder		  (Instru_Inf_Tab_Mem_Menu_adder+Instru_Menu_Inf_State_Space_Size)


/* ���� �̶���Ϣ�� ��FRAM�еĴ洢���� */
/* ���� �̶���Ϣ�� �洢��FRAM�� ��ʼ��ַ */
#define Instru_Fix_Inf_Tab_Mem_Star 			    (0X1800)
/* ���� �̶���Ϣ�� �洢��FRAM�� �ռ��С */     
#define Instru_Fix_Inf_Tab_Mem_Size 			    (0X076C)//������Wifi�������Ƽ�����
/* ���� �̶���Ϣ�� �洢��FRAM�� β��ַ */      
#define Instru_Fix_Inf_Tab_Mem_End 				    (Instru_Fix_Inf_Tab_Mem_Star+Instru_Fix_Inf_Tab_Mem_Size)
/* ���� �̶���Ϣ�� �洢��FRAM�� ϵͳ��Ϣ��ַ */
#define Instru_Fix_Inf_Tab_Mem_System_adder		(Instru_Fix_Inf_Tab_Mem_Star)
/* ���� �̶���Ϣ�� �洢��FRAM�� ��Դ״̬��Ϣ��ַ */			
#define Instru_Fix_Inf_Tab_Mem_Power_adder		(Instru_Fix_Inf_Tab_Mem_System_adder+		  Instru_System_Fix_Inf_Space_Size)
/* ���� �̶���Ϣ�� �洢��FRAM�� ��������Ϣ��ַ */                                         
#define Instru_Fix_Inf_Tab_Mem_Sensor_adder		(Instru_Fix_Inf_Tab_Mem_Power_adder+		  Instru_Power_Fix_Inf_Space_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� �洢���ݿ�FRAM��Ϣ��ַ */                                  
#define Instru_Fix_Inf_Tab_Mem_Fram_adder		  (Instru_Fix_Inf_Tab_Mem_Sensor_adder+		  Instru_Sensor_Fix_Inf_Space_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� �洢���ݿ�FRAM��Ϣ��ַ */                                  
#define Instru_Fix_Inf_Tab_Mem_Gateway_adder	(Instru_Fix_Inf_Tab_Mem_Fram_adder+			  Instru_Fram_Fix_Inf_Space_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� �洢���ݿ�Flash��Ϣ��ַ */
#define Instru_Fix_Inf_Tab_Mem_Flash_adder		(Instru_Fix_Inf_Tab_Mem_Gateway_adder+		Instru_Gateway_Fix_Inf_Space_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� �洢���ݿ�Com1��Ϣ��ַ */
#define Instru_Fix_Inf_Tab_Mem_Com1_adder		  (Instru_Fix_Inf_Tab_Mem_Flash_adder+		  Instru_Flash_Fix_Inf_Space_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� �洢���ݿ��ӡ��Ϣ��ַ */
#define Instru_Fix_Inf_Tab_Mem_Print_adder		(Instru_Fix_Inf_Tab_Mem_Com1_adder+			  Instru_Com1_Fix_Inf_Space_Size)
/* ���� ��Ϣ���ñ�洢��FRAM�� �洢���ݿ�Wifiģ�������������ơ����뼰���ܷ�ʽ��ַ */
#define Instru_Fix_Inf_Tab_Mem_Wifi_adder		  (Instru_Fix_Inf_Tab_Mem_Print_adder+			Instru_Wifi_Fix_Inf_Space_Size)




/*************************************************************************************************/
/************************ ע��: ���²������������޸ģ��������ϵͳ�����쳣 ***********************/
/*************************************************************************************************/
/* ������ ���к� ���ݽṹ */
typedef union 
{
	struct 
	{	
		/* ��1�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_1			:4;
		/* ��2�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_2			:4;
		/* ��3�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_3			:4;
		/* ��4�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_4			:4;
		/* ��5�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_5			:4;
		/* ��6�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_6			:4;
		/* ��7�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_7			:4;
		/* ��8�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_8			:4;
		/* ��9�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_9			:4;
		/* ��10�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_10			:4;
		/* ��11�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_11			:4;
		/* ��12�� SN ���� */	//	�������к��������á�����ѡ��ɸ��ģ�
		u32 Code_12			:4;
	}Serial_Number_Code;
	u8 Serial_Number[Serial_Number_Buffer_Size];
}SERIAL_NUMBER;

/* ������ �汾�� ���ݽṹ */
typedef union 
{
	struct 
	{
		/* �汾�� ��λ *///	�������к��������á�����ѡ��ɸ��ģ�
		u8 Code_C;
		/* �汾�� ��λ *///	�������к��������á�����ѡ��ɸ��ģ�
		u8 Code_B;
		/* �汾�� ��λ *///	�������к��������á�����ѡ��ɸ��ģ�
		u8 Code_A;
	}Serial_Number_Code;
	u8 Serial_Number[Version_Buffer_Size];
}VERSION;


/* ϵͳ ��Ϣ */
typedef union 
{
	struct 
	{
		/* ���к� */										//	0 0 0 0 0 0 0 0 0 0 0 0	:δ���ù������кţ�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�
		SERIAL_NUMBER 	Serial_Number;					//	SNֻ�������ù��������޸ã�ƽ̨����޷��޸ġ�
//		/* ���к� ���� */
//		u32 			Serial_Number_Reserve		:16;//	00:Ĭ������
		/* ******************************************* */
		/* ����汾 */									//	00(��) 00���У� 00���ͣ����汾��ֻ�������ù��������޸ã�ƽ̨����޷��޸ġ�
														//	����汾�����г�������ģ���˲���Ҫ���á�
		VERSION 		Software_Version;
//		/* ����汾 ���� */
//		u32 			Software_Version_Reserve	:8; //	00:Ĭ������
		/* ******************************************* */
		/* Ӳ���汾 */									//	00(��) 00���У� 00���ͣ� :δ���ù��İ汾�ţ�����ǰ��Ҫ���ø������ݡ�
														//	�����޸ĵ���Ҫ�� ���� ������
		VERSION 		Hardware_Version;
//		/* Ӳ���汾 ���� */
//		u32 			Hardware_Version_Reserve	:8; //  00:Ĭ������
		/* ******************************************* */
		/* Զ������ ������ */								//	00:δ���ã�Ĭ�����ã�	01:Զ���������������ѣ�	02:Զ������(��������)
		u8 				UpdateFlag;
		/* �������� */									//	00:Ĭ������
		u8				Instru_Type;
	}Inf;
	u8 Inf_Space[Instru_System_Inf_Space_Size];
}INSTRU_SYSTEM;

/* ��Դ ״̬ ��Ϣ*/
typedef union 
{
	struct 
	{
		/* ��ӵ�Դ ״̬ */			//	TRUE:������ӣ�Ĭ�����ã�	FALSE:���δ����
		BOOL ET_Link_State;
		/* ��ӵ�Դ ���������� */		//	TRUE:��������Ĭ�����ã�	FALSE:������
		BOOL ET_Sound_Alarm_Switch;
		/* ��ӵ�Դ �ⱨ������ */		//	TRUE:��������Ĭ�����ã�	FALSE:������
		BOOL ET_Light_Alarm_Switch;
		/* ��ӵ�Դ ���ű������� */	//	TRUE:������				FALSE:�����أ�Ĭ�����ã�
		BOOL ET_SMS_Alarm_Switch;
		/* ��ӵ�Դ ����״̬ */		//	TRUE:�ѱ���				FALSE:δ������Ĭ�����ã�
		BOOL ET_Alarm_State;


		/* ��� ���״̬ */			//	TRUE:������ڳ��		FALSE:���δ��磨Ĭ�����ã�
		BOOL BT_Charge_State;
		/* ��� ����״̬ */			//	0:�����0���(û�磬GSM�޷�����)
									//	1:�����1���	
									//	2:�����2���	
									//	3:�����3���	
									//	4:�����4��磨Ĭ�����ã�	
									//	5:״̬����		
									//	6:���δ��(��ʱû���������)
		u8	 BT_Quantity_State;
		/* ��� ���������� */		//	TRUE:������				FALSE:�����أ�Ĭ�����ã�
		BOOL BT_Sound_Alarm_Switch;
		/* ��� �ⱨ������ */		//	TRUE:������				FALSE:�����أ�Ĭ�����ã�
		BOOL BT_Light_Alarm_Switch;
		/* ��� ���ű������� */		//	TRUE:������				FALSE:�����أ�Ĭ�����ã�
		BOOL BT_SMS_Alarm_Switch;
		/* ��� ����״̬ */			//	TRUE:�ѱ���				FALSE:δ������Ĭ�����ã�
		BOOL BT_Alarm_State;

	}Inf_State;
	u8 Inf_State_Space[Instru_Power_Inf_State_Space_Size];
}INSTRU_POWER; 

/* ʵʱʱ�� ��Ϣ */
typedef union
{
	struct 
	{
		/* ʵʱʱ�� �� ���� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ���BCD���ʽ��		
		u8 Sec					;  
		/* ʵʱʱ�� �� ���� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ���BCD���ʽ��
		u8 Min					; 	
		/* ʵʱʱ�� ʱ ���� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ���BCD���ʽ��
		u8 Hour					;  	
		/* ʵʱʱ�� ���� ���� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ���BCD���ʽ��
		u8 Week					;  
		/* ʵʱʱ�� ���� ���� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ���BCD���ʽ��
		u8 Day					;  
		/* ʵʱʱ�� �·� ���� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ���BCD���ʽ��
		u8 Month				;  	
		/* ʵʱʱ�� ��� ���� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ���BCD���ʽ��
		u16 Year				;
		/* ʵʱʱ�� ʱ�� ���� */	//	08:Ĭ�����ã���8����������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)��BCD���ʽ��
		int	Zone				;

	}Inf_State;
	u8 Inf_State_Space[Instru_RTC_Inf_State_Space_Size];
}INSTRU_RTC;


/* ������ ��ַ */
typedef union 
{
	struct 
	{	/* ��ʽ:A.B.C.D(255.255.255.255) */
		/* ��ַ A */			//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u8 Address_A;
		/* ��ַ B */			//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u8 Address_B;
		/* ��ַ C */			//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u8 Address_C;
		/* ��ַ D */			//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u8 Address_D;
	}Serial_Number_Number;
	u32 Server_Address;
}SERVER_IP_ADDRESS;

/* �ֻ� ���� �ṹ */
typedef union 
{
	struct 
	{	
		/* ��1�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_1			:4;
		/* ��2�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_2			:4;
		/* ��3�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_3			:4;
		/* ��4�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_4			:4;
		/* ��5�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_5			:4;
		/* ��6�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_6			:4;
		/* ��7�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_7			:4;
		/* ��8�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_8			:4;
		/* ��9�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_9			:4;
		/* ��10�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_10			:4;
		/* ��11�� SN ���� */	//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32 Number_11			:4;
	}Serial_Number_Number;
	u8 Mobile_No[Mobile_No_Buffer_Size];
}MOBILE_NO;

/* GSMģ�� ״̬��Ϣ */
typedef union
{
	struct
	{
		/* GSMģ�� ��Ӧ������1 ���� */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		u8  Domain_Name_ID1[32];
		/* GSMģ�� ��Ӧ������1 IP��ַ */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		SERVER_IP_ADDRESS  IP_Address_ID1	;
		/* GSMģ�� ��Ӧ������1 �˿� */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		u16 Port_ID1						;
		/* GSMģ�� ������1���� */			//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		u16 GSM_Inf_ID1_Reserve				;
		
		/* GSMģ�� ��Ӧ������2 ���� */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		u8 	Domain_Name_ID2[32];
		/* GSMģ�� ��Ӧ������2 IP��ַ */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		SERVER_IP_ADDRESS 	IP_Address_ID2;
		/* GSMģ�� ��Ӧ������2 �˿� */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		u16 Port_ID2						;
		/* GSMģ�� ���� */				//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		u16 GSM_Inf_ID2_Reserve				;
		
		/* GSMģ�� ��Ӧ������3 ���� */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		u8 	Domain_Name_ID3[32];
		/* GSMģ�� ��Ӧ������3 IP��ַ */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		SERVER_IP_ADDRESS 	IP_Address_ID3	;
		/* GSMģ�� ��Ӧ������3 �˿� */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		u16 Port_ID3						;
		/* GSMģ�� ���� */				//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		u16 GSM_Inf_ID3_Reserve				;

		/* GSMģ�� �������ź���_1 */		//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		MOBILE_NO 	Alarm_SMS_NO1;
		/* GSMģ�� �������ź���_2 */		//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		MOBILE_NO	Alarm_SMS_NO2;
		/* GSMģ�� �������ź���_3 */		//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		MOBILE_NO 	Alarm_SMS_NO3;
		/* GSMģ�� �������� ����4 */		//	00:δ���ã�Ĭ�����ã���(��ѡ�� �ɸ��ģ�Ԥ��)
		MOBILE_NO 	Alarm_SMS_NO_Reserve4;
		/* GSMģ�� �������� ����5 */		//	00:δ���ã�Ĭ�����ã���(��ѡ�� �ɸ��ģ�Ԥ��)
		MOBILE_NO 	Alarm_SMS_NO_Reserve5;
		

	}Inf_State;
	u8 Inf_State_Space[Instru_GSM_Inf_State_Space_Size];
}INSTRU_GSM;

    			  		  
/* ���� ������ ͨ����Ϣ�ṹ */
typedef struct {
	/* ��ƬID�� */							//	0:δ����(Ĭ������)
	u32				SENSOR_RF_Card_ID_NO;
	/* ������ SN�� */						//	0:δ����(Ĭ������)
	u8				SENSOR_SN_NO[SENSOR_SN_NO_Size];
	/* ������ �������� */						//	00:δ����(Ĭ������)������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
	float			SENSOR_Up_Limit;
	/* ������ �������� */						//	00:δ����(Ĭ������)������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
	float			SENSOR_Low_Limit;
	/* ������ ���� */						
	u8		Type;							//	0:û���趨�Ĵ���������	
											//	1:�¶ȴ�����		
											//	2:ʪ�ȴ�����		
											//	3:���¿�Ƭ	
											//	4:��ʪ��Ƭ->�¶�
											//	5:��ʪ��Ƭ->ʪ��	
											//	6:����	
											//	7:γ��
											//	8:�ٶ�
											//	����:��Ч
	
	/* ������ �������� */						//	TRUE:������(Ĭ������)		FALSE:������
	BOOL	SENSOR_Alarm_Switch;
	/* ������ ������������ */					//	TRUE:������				FALSE:������(Ĭ������)
	BOOL	SENSOR_Sound_Alarm_Switch;
	/* ������ �ⱨ������ */					//	TRUE:������				FALSE:������(Ĭ������)
	BOOL	SENSOR_Light_Alarm_Switch;
	/* ������ ���ű������� */					//	TRUE:������				FALSE:������(Ĭ������)
	BOOL	SENSOR_SMS_Alarm_Switch;
	/* ������ ����״̬ */						//	0x0:������δ����(Ĭ������)	0x1:���ޱ��� 0x02:���ޱ���			
	u8		SENSOR_Alarm_State;
	
}INSTRU_SENSOR_CHANNEL;

/* ������ ״̬��Ϣ */
typedef union 
{
	struct
	{
		/* ������ ͨ��1 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH1;
		/* ������ ͨ��2 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH2;
		/* ������ ͨ��3 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH3;
		/* ������ ͨ��4 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH4;
		/* ������ ͨ��5 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH5;
		/* ������ ͨ��6 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH6;
		/* ������ ͨ��7 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH7;
		/* ������ ͨ��8*/			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH8;
		/* ������ ͨ��9 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH9;
		/* ������ ͨ��10 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH10;
		/* ������ ͨ��11 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH11;
		/* ������ ͨ��12 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH12;
		/* ������ ͨ��13 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH13;
		/* ������ ͨ��14 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH14;
		/* ������ ͨ��15 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH15;
		/* ������ ͨ��16 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH16;
		/* ������ ͨ��17 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH17;
		/* ������ ͨ��18 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH18;
		/* ������ ͨ��19 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH19;
		/* ������ ͨ��20 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH20;
		/* ������ ͨ��21 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH21;
		/* ������ ͨ��22 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH22;
		/* ������ ͨ��23 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH23;
		/* ������ ͨ��24 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH24;
		/* ������ ͨ��25 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH25;
		/* ������ ͨ��26 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH26;
		/* ������ ͨ��27 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH27;
		/* ������ ͨ��28 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH28;
		/* ������ ͨ��29 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH29;
		/* ������ ͨ��30 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH30;
		/* ������ ͨ��31 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH31;
		/* ������ ͨ��32 */			//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ�� �ɸ���)
		INSTRU_SENSOR_CHANNEL		CH32;

	}Inf_State;
	u8 Inf_State_Space[Instru_Sensor_Inf_State_Space_Size];
}INSTRU_SENSOR;

/* ����ʱ��� ���ݽṹ */
typedef union 
{
	struct 
	{	
		/* ����ʱ��� ģʽ���� */			//  00:�أ�Ĭ�����ã�01:�ϰ�ʱ���	02:���ϰ�ʱ���	03:�Զ���ʱ��� 04:ȫ��ʱ���
		u32  Alarm_Mode			:8;
		/* ��һ ���� ���� */				//	00:�����أ�Ĭ�����ã�01:���������Զ���ʱ��ģʽ��������
		u32	 Alarm_Week_1		:1;
		/* �ܶ� ���� ���� */				//	00:�����أ�Ĭ�����ã�01:���������Զ���ʱ��ģʽ��������
		u32	 Alarm_Week_2		:1;
		/* ���� ���� ���� */				//	00:�����أ�Ĭ�����ã�01:���������Զ���ʱ��ģʽ��������
		u32	 Alarm_Week_3		:1;
		/* ���� ���� ���� */				//	00:�����أ�Ĭ�����ã�01:���������Զ���ʱ��ģʽ��������
		u32	 Alarm_Week_4		:1;
		/* ���� ���� ���� */				//	00:�����أ�Ĭ�����ã�01:���������Զ���ʱ��ģʽ��������
		u32	 Alarm_Week_5		:1;
		/* ���� ���� ���� */				//	00:�����أ�Ĭ�����ã�01:���������Զ���ʱ��ģʽ��������
		u32	 Alarm_Week_6		:1;
		/* ���� ���� ���� */				//	00:�����أ�Ĭ�����ã�01:���������Զ���ʱ��ģʽ��������
		u32	 Alarm_Week_7		:1;
		/* �� ���� ���� */				//	00:δ���ã�Ĭ�����ã���(��ѡ��ɸ���)
		u32	 Alarm_Reserve		:1;
		/* ����ʱ��� �ϰ�ʱ�� ʱ */		//	08:δ���ã�Ĭ�����ã�(��ѡ��ɸ���)���Զ���ʱ��ģʽ��������
		u32  OnWork_Time_Hour	:8;			
		/* ����ʱ��� �ϰ�ʱ�� �� */		//	00:δ���ã�Ĭ�����ã�(��ѡ��ɸ���)���Զ���ʱ��ģʽ��������
		u32  OnWork_Time_Min	:8;		
		/* ����ʱ��� �°�ʱ�� ʱ */		//	17:δ���ã�Ĭ�����ã�(��ѡ��ɸ���)���Զ���ʱ��ģʽ��������
		u32  OffWork_Time_Hour	:8;			
		/* ����ʱ��� �°�ʱ�� �� */		//	00:δ���ã�Ĭ�����ã�(��ѡ��ɸ���)���Զ���ʱ��ģʽ��������
		u32  OffWork_Time_Min	:8;
	}Inf;
	u8 Workaday_Data[Workaday_Data_Buffer_Size];
}WORKADAY;

/* �˵����� ��Ϣ*/
typedef union 
{
	struct 
	{
		/* �˵����� ���������ܿ��� */		//	FALSE:�� ���� ������Ĭ�����ã���	TRUE:�� ���� ����
		BOOL Sound_Alarm_Master_Switch	;
		/* �˵����� ��  �����ܿ��� */		//	FALSE:�� �� ������Ĭ�����ã���		TRUE:�� �� ����
		BOOL Light_Alarm_Master_Switch	;
		/* �˵����� ���ű����ܿ��� */		//	FALSE:�� ���� ������Ĭ�����ã���	TRUE:�� ���� ����
		BOOL SMS_Alarm_Master_Switch	;
		/* �˵����� �������� ������ʾ */	//	FALSE:�أ�Ĭ�����ã���			TRUE:�� 
		BOOL Alarm_Normal_SMS			;
		/* ���� ������ ��ʱ ʱ�� */		//	Ĭ������:300s ��Χ:0-4294967295����λ���룩
		u32 Sound_Alarm_Delay_Time		;
		/* ��   ������ ��ʱ ʱ�� */		//	Ĭ������:300s ��Χ:0-4294967295����λ���룩
		u32 Light_Alarm_Delay_Time		;
		/* ���� ������ ��ʱ ʱ�� */		//	Ĭ������:300s ��Χ:0-4294967295����λ���룩
		u32 SMS_Alarm_Delay_Time		;
		/* �ɼ����ݼ�� */				//	Ĭ������:1s ��Χ:0-4294967295����λ���룩
		u32 Collect_Data_Interval		;
		/* �������ݼ�¼��� */			//	Ĭ������:120s ��Χ:0-4294967295����λ���룩
		u32 Alarm_Save_Data_Interval	;
		/* �������ݼ�¼��� */			//	Ĭ������:300s ��Χ:0-4294967295����λ���룩
		u32 Normal_Save_Data_Interval	;
		/* �ϴ����ݼ�� */				//	Ĭ������:60s ��Χ:0-4294967295����λ���룩
		u32 Upload_Data_Interval		;
		/* �����ظ�����ʱ���� */			//	Ĭ������:0s ��Χ:0-4294967295����λ���룩
		u32 SMS_Repeat_Alarm_Interval	;
		/* ʵʱ���ݴ洢���� */			//	FALSE:�� �洢���ܣ�	TRUE:�� ��Ĭ�����ã�.
		FunctionalState Menu_Save_Data_Switch;
	}Inf_State;
	u8 Inf_State_Space[Instru_Menu_Inf_State_Space_Size];
}INSTRU_MENU;

/*������ ���ݱ� �ṹ */
typedef union 
{
	struct
	{
		/* ͨ��1 ��ʵֵ */
		u16 CH1_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��2 ��ʵֵ */
		u16 CH2_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��3 ��ʵֵ */
		u16 CH3_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��4 ��ʵֵ */
		u16 CH4_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��5 ��ʵֵ */
		u16 CH5_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��6 ��ʵֵ */
		u16 CH6_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��7 ��ʵֵ */
		u16 CH7_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��8 ��ʵֵ */
		u16 CH8_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��9 ��ʵֵ */
		u16 CH9_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��10 ��ʵֵ */
		u16 CH10_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��11 ��ʵֵ */
		u16 CH11_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��12 ��ʵֵ */
		u16 CH12_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��13 ��ʵֵ */
		u16 CH13_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��14 ��ʵֵ */
		u16 CH14_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��15 ��ʵֵ */
		u16 CH15_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��16 ��ʵֵ */
		u16 CH16_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��17 ��ʵֵ */
		u16 CH17_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��18 ��ʵֵ */
		u16 CH18_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��19 ��ʵֵ */
		u16 CH19_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��20 ��ʵֵ */
		u16 CH20_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��21 ��ʵֵ */
		u16 CH21_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��22 ��ʵֵ */
		u16 CH22_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��23 ��ʵֵ */
		u16 CH23_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��24 ��ʵֵ */
		u16 CH24_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��25 ��ʵֵ */
		u16 CH25_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��26 ��ʵֵ */
		u16 CH26_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��27 ��ʵֵ */
		u16 CH27_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��28 ��ʵֵ */
		u16 CH28_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��29 ��ʵֵ */
		u16 CH29_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��30 ��ʵֵ */
		u16 CH30_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��31 ��ʵֵ */
		u16 CH31_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��32 ��ʵֵ */
		u16 CH32_Actual_Value[INSTRU_SENSOR_Data_Tab_Size];


		/* ͨ��1 У׼ֵ */
		u16 CH1_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��2 У׼ֵ */
		u16 CH2_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��3 У׼ֵ */
		u16 CH3_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��4 У׼ֵ */
		u16 CH4_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��5 У׼ֵ */
		u16 CH5_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��6 У׼ֵ */
		u16 CH6_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��7 У׼ֵ */
		u16 CH7_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��8 У׼ֵ */
		u16 CH8_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��9 У׼ֵ */
		u16 CH9_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��10 У׼ֵ */
		u16 CH10_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��11 У׼ֵ */
		u16 CH11_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��12 У׼ֵ */
		u16 CH12_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��13 У׼ֵ */
		u16 CH13_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��14 У׼ֵ */
		u16 CH14_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��15 У׼ֵ */
		u16 CH15_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��16 У׼ֵ */
		u16 CH16_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��17 У׼ֵ */
		u16 CH17_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��18 У׼ֵ */
		u16 CH18_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��19 У׼ֵ */
		u16 CH19_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��20 У׼ֵ */
		u16 CH20_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��21 У׼ֵ */
		u16 CH21_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��22 У׼ֵ */
		u16 CH22_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��23 У׼ֵ */
		u16 CH23_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��24 У׼ֵ */
		u16 CH24_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��25 У׼ֵ */
		u16 CH25_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��26 У׼ֵ */
		u16 CH26_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��27 У׼ֵ */
		u16 CH27_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��28 У׼ֵ */
		u16 CH28_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��29 У׼ֵ */
		u16 CH29_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��30 У׼ֵ */
		u16 CH30_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��31 У׼ֵ */
		u16 CH31_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];
		/* ͨ��32 У׼ֵ */
		u16 CH32_Calibration_Value[INSTRU_SENSOR_Data_Tab_Size];

		
	}Inf_State;
	u8 Inf_State_Space[Instru_Cal_Tab_Space_Size];
}INSTRU_DATA_TAB;

/* ���� ��״̬��Ϣ�� */
typedef union 
{
	struct
	{
		/* ϵͳ ״̬��Ϣ  */
		INSTRU_SYSTEM	Instru_System;
		
		/* �� ϵͳϵ�� �������� ѡ�����ѡ���Ӧ���͵����ݽṹ�����ú���ĵ�ַ���ݡ� */
		/* ��Դ ״̬��Ϣ */
		INSTRU_POWER	Instru_Power;
		/* ʵʱʱ�� ��Ϣ */
		INSTRU_RTC		Instru_RTC;
		/* GSM ״̬ ��Ϣ */
		INSTRU_GSM		Instru_GSM;
		/* ������ ״̬ ��Ϣ */
		INSTRU_SENSOR	Instru_Sensor;
		/* �˵����� ״̬��Ϣ */
		INSTRU_MENU		Instru_Menu;
		/* ������ͨ�� ���ݱ� */
		INSTRU_DATA_TAB	Instru_Data_Tab;
		
//		/* ���� ��Ϣ �����ռ� */
//		u8 Inf_State_Reserve[Instru_Inf_State_Surplus_Space_Size-4];//�����ռ�//����ļ�1����Ϊ���滹��һ�� ���ñ�־
		
		/* �������ñ� ���� ��־ */			//	00:δ���ù���Ĭ�����ã���01:�Ѿ��������
		u8 Data_Update_Flag;				//	00:δ���ù���Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ�
	}Inf_State;/* ��Ӧģ���״̬��Ϣ */
	u8 Inf_State_Space[Instru_Inf_State_Space_Size];
}INSTRU_CONFIG_INF_TAB;/* ����������Ϣ�� */

/* ϵͳ �̶���Ϣ */
typedef union 
{
	struct 
	{
		/* ϵͳ ���� */								//	00:���ģ�Ĭ�����ã���	01:Ӣ��		����:��Ч
		u8	 System_Language						;
	}Inf_State;	
	u8 Inf_Space[Instru_System_Fix_Inf_Space_Size];
}INSTRU_SYSTEM_FIX;

/* ��Դ �̶���Ϣ */
typedef union
{
	struct
	{
		/* ��ӵ�Դ ״̬�ı����ʱ�� *///	00:δ���ù���Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ�
		u8 ET_Status_Update_Time;
		/* ��� ״̬�ı����ʱ�� */	//	00:δ���ù���Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ�
		u8 BT_Status_Update_Time;
		/* ��ش�������ADC����ֵ */	//	00:δ���ù���Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		u16	BT_Quantity_Sampl_Value;
	}Inf;
	u8 Inf_Space[Instru_Power_Fix_Inf_Space_Size];
}INSTRU_POWER_FIX;


/* ���� ������  �̶���Ϣ ͨ�����ݽṹ */
typedef struct {
	/* ������ ��������ֵ */			//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
	float Sensor_Sampl_Data;
	/* ������ ʵʱ���ݣ�У׼ֵ�� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ�
	float Sensor_Real_Time_Data;
	/* ������ ��������ʱ������Ĵ��� *///	00:δ���ù��ĵ�ַ��Ĭ�����ã������������к��������á�����ѡ�� �ɸ��ģ�
	u16 Sensor_Up_Alarm_Cnt;
	/* ������ ��������ʱ������Ĵ��� *///	00:δ���ù��ĵ�ַ��Ĭ�����ã������������к��������á�����ѡ�� �ɸ��ģ�
	u16 Sensor_Low_Alarm_Cnt;
	/* ������ ����ȡ����־ */			//	00:δ���ã�Ĭ�����ã���01:ȡ���������������к��������á�����ѡ�� �ɸ��ģ�
	BOOL	Sensor_Suspend_Alarm_Flag;
	/* ������ ����״̬ */				//	00: ��ص����ͱ�����Ĭ�����ã���01:��ص���������
	u8	Sensor_Power_State;
	/* ������ ������ʱʱ�� */			//	00:δ���ã�Ĭ�����ã�
	u16	Sensor_Alarm_Delay;
	/* ������ �������� ���� */
	float Sensor_Fraud_Up_Limit;	
	/* ������ �������� ���� */
	float Sensor_Fraud_Low_Limit;
	
}INSTRU_SENSOR_CHANNEL_FIX;

/* ������ �̶���Ϣ */
typedef union
{
	struct
	{
		/* ͨ�� 1 */
		INSTRU_SENSOR_CHANNEL_FIX		CH1;
		/* ͨ�� 2 */
		INSTRU_SENSOR_CHANNEL_FIX		CH2;
		/* ͨ�� 3 */
		INSTRU_SENSOR_CHANNEL_FIX		CH3;
		/* ͨ�� 4 */
		INSTRU_SENSOR_CHANNEL_FIX		CH4;
		/* ͨ�� 5 */
		INSTRU_SENSOR_CHANNEL_FIX		CH5;
		/* ͨ�� 6 */
		INSTRU_SENSOR_CHANNEL_FIX		CH6;
		/* ͨ�� 7 */
		INSTRU_SENSOR_CHANNEL_FIX		CH7;
		/* ͨ�� 8 */
		INSTRU_SENSOR_CHANNEL_FIX		CH8;
		/* ͨ�� 9 */
		INSTRU_SENSOR_CHANNEL_FIX		CH9;
		/* ͨ�� 10 */
		INSTRU_SENSOR_CHANNEL_FIX		CH10;
		/* ͨ�� 11 */
		INSTRU_SENSOR_CHANNEL_FIX		CH11;
		/* ͨ�� 12 */
		INSTRU_SENSOR_CHANNEL_FIX		CH12;
		/* ͨ�� 13 */
		INSTRU_SENSOR_CHANNEL_FIX		CH13;
		/* ͨ�� 14 */
		INSTRU_SENSOR_CHANNEL_FIX		CH14;
		/* ͨ�� 15 */
		INSTRU_SENSOR_CHANNEL_FIX		CH15;
		/* ͨ�� 16 */
		INSTRU_SENSOR_CHANNEL_FIX		CH16;
		/* ͨ�� 17 */
		INSTRU_SENSOR_CHANNEL_FIX		CH17;
		/* ͨ�� 18 */
		INSTRU_SENSOR_CHANNEL_FIX		CH18;
		/* ͨ�� 19 */
		INSTRU_SENSOR_CHANNEL_FIX		CH19;
		/* ͨ�� 20 */
		INSTRU_SENSOR_CHANNEL_FIX		CH20;
		/* ͨ�� 21 */
		INSTRU_SENSOR_CHANNEL_FIX		CH21;
		/* ͨ�� 22 */
		INSTRU_SENSOR_CHANNEL_FIX		CH22;
		/* ͨ�� 23 */
		INSTRU_SENSOR_CHANNEL_FIX		CH23;
		/* ͨ�� 24 */
		INSTRU_SENSOR_CHANNEL_FIX		CH24;
		/* ͨ�� 25 */
		INSTRU_SENSOR_CHANNEL_FIX		CH25;
		/* ͨ�� 26 */
		INSTRU_SENSOR_CHANNEL_FIX		CH26;
		/* ͨ�� 27 */
		INSTRU_SENSOR_CHANNEL_FIX		CH27;
		/* ͨ�� 28 */
		INSTRU_SENSOR_CHANNEL_FIX		CH28;
		/* ͨ�� 29 */
		INSTRU_SENSOR_CHANNEL_FIX		CH29;
		/* ͨ�� 30 */
		INSTRU_SENSOR_CHANNEL_FIX		CH30;
		/* ͨ�� 31 */
		INSTRU_SENSOR_CHANNEL_FIX		CH31;
		/* ͨ�� 32 */
		INSTRU_SENSOR_CHANNEL_FIX		CH32;

	}Inf;
	u8 Inf_Space[Instru_Sensor_Fix_Inf_Space_Size];
}INSTRU_SENSOR_FIX;

/* �洢���ݿ�FRAM ״̬��Ϣ */
typedef union 
{
	struct 
	{
		/* �洢���ݿ�FRAM ����״̬ */
		u8 Chip_Work_State; 			//	00:оƬ�쳣��Ĭ�����ã� 01:оƬ���� 	�������к��������á�����ѡ��ɸ��ģ�
	}Inf_State;
	u8 Inf_State_Space[Instru_Fram_Fix_Inf_Space_Size];
}INSTRU_FRAM_FIX;

/* ���ش洢��Ϣ */
typedef union 
{
	struct 
	{
		/* ���ص�ַ */
		uint8_t Adder;
		/* �ŵ�(����) */
		uint8_t Channel;
		/* ���ݶ�ʧʱ�� */
		uint8_t ActiveTime;
		/* ����ģʽ������/���ԣ� */
		uint8_t Mode;
		/* ͨ�Ų����� */
		uint32_t Baudrate;
		/* ���ع���״̬ */
		uint8_t WorkState;
	}Inf_State;
	u8 Inf_State_Space[Instru_Gateway_Fix_Inf_Space_Size];
}INSTRU_Gateway_FIX;

/* �洢���ݿ�Flash ��Ϣ״̬ */
typedef union 
{
	struct 
	{
		/* �洢���ݿ�Flash оƬ���� */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ��ɸ���)
		u32 Chip_Capacity;
		/* �洢���ݿ�Flash ���� ҳ ��С */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ��ɸ���)
		u32 Chip_Page_Size;
		/* �洢���ݿ�Flash ���� ҳ ���� */	//	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ��ɸ���)
		u32 Chip_Page_Num;
		/* �洢���ݿ�Flash ���� ���� ��С *///	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ��ɸ���)
		u32 Sector_Size;
		/* �洢���ݿ�Flash ���� ���� ָ�� *///	00:δ���ã�Ĭ�����ã�������ǰ��Ҫ���ø������ݡ�(��ѡ��ɸ���)
		u32 Sector_Pointer;	
		/* �洢���ݿ�Flash ���ݴ洢���� */	//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		u32 Storage_Data_NUM;
		/* �洢���ݿ�Flash дָ�� */		//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		s32	Write_Pointer;	
		/* �洢���ݿ�Flash ��ָ�� */		//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		s32	Read_Pointer;	

		/* �洢���ݿ�Flash ����״̬ */
		ErrorStatus Chip_Work_State; //	00:оƬ�쳣��Ĭ�����ã� 01:оƬ���� 	�������к��������á�����ѡ��ɸ��ģ�
	}Inf_State;
	u8 Inf_State_Space[Instru_Flash_Fix_Inf_Space_Size];
}INSTRU_FLASH_FIX;

/* ����1ͨ�� ״̬��Ϣ */
typedef union 
{
	struct 
	{
		/* ������������ �Ĵ��� */			//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		u8 	Rece_Com_Data_Size;
		/* ����֡���� �Ĵ��� */			//	00:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		u8	Rece_Com_Packet_Len;
		/* ���ݽ��� ��ʱ���� �Ĵ��� */		//	300:δ���ù��ĵ�ַ��Ĭ�����ã����������к��������á�����ѡ��ɸ��ģ�
		signed short int Rece_Com_Timeout;
	}Inf_State;
	u8 Inf_State_Space[Instru_Com1_Fix_Inf_Space_Size];
}INSTRU_COM1_FIX;

/* ��ӡ�������� */
typedef union
{
	struct
	{
		/* ��ӡ��ʼʱ�� */
		u32 Print_Start_Time;
		/* ��ӡ����ʱ�� */
		u32 Print_End_Time;
		/* ��¼�Ĵ�ӡ��ʼʱ�� */
		u32 Print_Record_Start_Time;
		/* ��¼�Ĵ�ӡ����ʱ�� */
		u32 Print_Record_End_Time;
		/* ��¼�Ĵ�ӡ��ʼ���ݵ�ַ������Flash�ĵ�ַ�� */
		s32 Print_Record_Start_Adder;
		/* ��¼�Ĵ�ӡ�������ݵ�ַ������Flash�ĵ�ַ�� */
		s32 Print_Record_End_Adder;
		/* ��ӡ�ǳ�������ʱ����(Ĭ��:5����) */
		u32 Print_Nrl_Itl;
		/* ��ӡ��������ʱ����(Ĭ��:2����) */
		u32 Print_Out_Itl;
		/* ��ӡ���ƺ� */
		char Print_Card_NO[Print_Card_NO_Size];
		/* ��ӡ��˾���� */
		char Print_Company_Name[Print_Company_Name_Size];
		/* ��¼��ж����ʼ���ݵ�ַ���飨����Flash�ĵ�ַ�� */
		s32 Print_Start_Upload_Adder[Print_Start_upload_Adder_Size];
/* ��¼��ж���������ݵ�ַ���飨����Flash�ĵ�ַ�� */
		s32 Print_Stop_Upload_Adder[Print_Stop_upload_Adder_Size];	
		/*ж����ʼ��ַ������ ��Χ�ǣ�0~99*/
		u32 Print_Start_Upload_Counter;		
		/*ж��������ַ������ ��Χ�ǣ�0~99*/
		u32 Print_Stop_Upload_Counter;
		/*��ʾж����ʼ����ͼ���־λ   0������ʾ   1������ʾ*/
		u32 Print_Start_Upload_Pic_Flag;
	}Inf_State;
	u8 Inf_State_Space[Instru_Print_Fix_Inf_Space_Size];
}INSTRU_PRINT_FIX;


/* Wifiģ����ز�����Ϣ*/
typedef union
{
	struct
	{
		/* Ҫ����Wifi������ */
		char Wifi_Name[Wifi_Name_Size];
	  /* Ҫ����Wifi������ */
		char Wifi_Password[Wifi_Passward_Size];	
	  /* Ҫ����Wifi�ļ��ܷ�ʽ */
		u32   Wifi_WSKEY; 
	}Inf_State;
  u8 Inf_State_Space[Instru_Wifi_Fix_Inf_Space_Size];
}INSTRU_WIFI_FIX;

/* Wifiģ��Wlan������IP��ַȡ��ʽ������״̬����Ϣ*/
typedef union
{
	struct
	{
		/*Wifiģ�鹤��״̬*/
		u8   Wifi_Work_State;//0:ģ���ʼ�����ɹ�   1:ģ���ʼ���ɹ�
		/*Wifiģ��������ܷ�ʽ*/
		u8   Wifi_Encryption_Way;//0:Disable  1:OPENWEP  2:SHAREDWEP  3:WPA-PSK TKIP  4:WPA-PSK AES  5:WPA2-PSK TKIP  6:WPA2-PSK AES 
		/*Wifiģ��IP��ַ��ȡ��ʽ*/
		u8   Wifi_IP_Get_Way;//0:Static(��̬��ȡ)   1:DHCP(��̬��ȡ)
		/*Wifiģ�鱣���ֽ�*/
		u8   Wifi_IP_Res;
		    /*Wifiģ��Mac��ַ*/
		char Wifi_Mac_Address[Wifi_Mac_Address_Size];
    /*Wifiģ�鱾��IP��ַ*/
		char Wifi_Local_IP[Wifi_Local_IP_Size];
		    /*Wifiģ����������*/
		char Wifi_Subnet_Mask[Wifi_Subnet_Mask_Size];
		    /*Wifiģ������IP��ַ*/
		char Wifi_Gateway_IP[Wifi_Gateway_IP_Size];
	}Inf;
  u8   Inf_State_Space[Instru_Wifi_Inf_Space_Size];
}INSTRU_WIFI;





/* ���� ��״̬��Ϣ�� */
typedef union
{
	struct
	{
		/* ϵͳ �̶���Ϣ  */
		INSTRU_SYSTEM_FIX	Instru_System;
		/* ��Դ ״̬��Ϣ */
		INSTRU_POWER_FIX	Instru_Power;
		/* ������ ״̬ ��Ϣ */
		INSTRU_SENSOR_FIX	Instru_Sensor;
		/* �ⲿ�洢�� FRAM ��Ϣ */
		INSTRU_FRAM_FIX		Instru_Fram;
		/* �ⲿ�洢�� ���� ��Ϣ */
		INSTRU_Gateway_FIX	Instru_Gateway;
		/* �ⲿ�洢�� Flash ��Ϣ */
		INSTRU_FLASH_FIX	Instru_Flash;
		/* ����1ͨ�� ״̬��Ϣ */
		INSTRU_COM1_FIX		Instru_Com1;
		/* ��ӡ ��Ϣ */
		INSTRU_PRINT_FIX	Instru_Print;
		/* �������ñ� ���� ��־ */			  //	00:δ���ù���Ĭ�����ã���01:�Ѿ��������
		INSTRU_WIFI_FIX    Instru_Wifi; //Wifi���ӵ�wlan�������ơ����뼰���ܷ�ʽ
		
		u8                Data_Update_Flag;				    //	00:δ���ù���Ĭ�����ã����������к��������á�����ѡ�� �ɸ��ģ�
	}Inf_State;/* ��Ӧģ���״̬��Ϣ */
	u8 Inf_State_Space[Instru_Fix_Inf_Tab_Space_Size];
}INSTRU_FIX_INF_TAB;/* �����̶���Ϣ�� */
















extern   INSTRU_FIX_INF_TAB              Instru_Fix_Inf_State_Tab;	
extern   const INSTRU_FIX_INF_TAB        Default_Instru_Fix_Inf_State_Tab;	
        
extern   INSTRU_CONFIG_INF_TAB           Instru_Config_Inf_Tab;	
extern   const INSTRU_CONFIG_INF_TAB     Default_Instru_Config_Inf_State_Tab;	
        
extern   INSTRU_WIFI_FIX                 Instru_Wifi_NamwPWD;//Wifi���ּ�����
                                         
extern   INSTRU_WIFI                     Instru_Wifi_Fix_Tab;//Wifi����״̬��Wlan�Ȳ���




//ϵͳ
/* ������� */
extern u8 Serial_Number[Serial_Number_Menu_Buffer_Size];
/* ����汾 ���� */
extern u8 Software_Version_CN[Version_Inf_Buffer_Size];
/* Ӳ���汾 ���� */
extern u8 Hardware_Version_CN[Version_Inf_Buffer_Size];
/* ����汾 Ӣ�� */
extern u8 Software_Version_EN[Version_Inf_Buffer_Size];
/* Ӳ���汾 Ӣ�� */
extern u8 Hardware_Version_EN[Version_Inf_Buffer_Size];








/************************************************************************************************/
void Mcu_Pgm_Ude_State_Check		(void);
void Mcu_Cer_Htc_Data				(void);


#endif

