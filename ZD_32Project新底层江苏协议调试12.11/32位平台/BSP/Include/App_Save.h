#ifndef __App_Save_H
#define	__App_Save_H

#include "stm32f10x.h"

#include "Log_Time.h" 
#include "Log_Save.h"
#include "Mcu_Config.h"







/********************************* �洢̽ͷ����֡�ṹ *****************************/
typedef struct
{
	/* ʱ��� */
	Times Timestamp;
	
	/* ���������� */
	float Sensor[Sensor_CH_Config_Num];
}AppSaveFrameStruct;

/********************************* ��̽ͷ�洢״̬ *********************************/
typedef enum {Save_ReadFail = (0u), Save_ReadSuccess = (1u)}Save_ReadState;

/********************************* ����Ӧ�ýӿڽṹ *******************************/
typedef struct
{	
	
	uint32_t		(*ReadSaveNum)			(void);
	Save_ReadState 	(*ReadSaveDataFrame)	(AppSaveFrameStruct* dataFrame);
}AppSaveAPIStruct;





/************************************************************************************************/
/********************************* ̽ͷ���ݴ洢Ӧ�ýӿڼĴ������� *********************************/
extern AppSaveAPIStruct AppSaveAPI;


/********************************* �������� *********************************/
void AppSave_InitAPI(void);
#if ((Menu_Save_Data_Func == 0x01)&&(Save_Function_Config_Enable == 0x01))
void App_Save_Config(FunctionalState New_state);
void App_Save_Print_Start_upload_Adder(void);//��¼ж����ʼ���ݼ�¼��ַ
void App_Save_Print_Stop_upload_Adder(void);//��¼ж���������ݼ�¼��ַ
#endif





#endif /* __App_GPS_H */


