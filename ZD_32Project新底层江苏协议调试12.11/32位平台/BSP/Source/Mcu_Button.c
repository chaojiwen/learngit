/*
*********************************************************************************************************
*	                                  
*	ģ������ : ��������ģ��	���ο�armfly�����壩	 
*	�ļ����� : Mcu_Button.c
*	��    �� : V1.0
*	˵    �� : ʵ�ְ����ļ�⣬��������˲����ƣ����Լ�������¼���
*			  (1) ��������
*			  (2) ��������
*			  (3) ������
*			  (4) ����ʱ�Զ�����
*			  (5) ��ϼ�
*
*	�޸ļ�¼ :
*	    �汾��    ����          ����                 ˵��
*		v1.0    2011-12-07   WildFire Team  ST�̼���汾 V3.5.0�汾��
*
*	Copyright (C), 2012-2013, WildFire Team
*
*********************************************************************************************************
*/
#include "Mcu_Button.h"
#include "stm32f10x.h"
#include <stdio.h>

/* ���ڲ�ѯ����ֵʱ���浱ǰ�ź������ݵ����ݽṹ */
OS_MBOX_DATA KEY_Mbox_DATA;

static BUTTON_T s_BtnSure;		   	/* ȷ�� �� */
static BUTTON_T s_BtnForward;	   	/* ��ǰ �� */
static BUTTON_T s_BtnBackward;   	/* ��� �� */
static BUTTON_T s_BtnCancel;   		/* ȡ�� �� */

static KEY_FIFO_T s_Key;	  	   /* ����FIFO����,�ṹ�� */

static void bsp_InitButtonVar(void);
// static void bsp_InitButtonHard(void);
static void bsp_DetectButton(BUTTON_T *_pBtn);

/*
	Ұ�𿪷��尴�����߷��䣺
	KEY1��     : PC6  (�͵�ƽ��ʾ����)
	KEY2��     : PC7  (�͵�ƽ��ʾ����)
	KEY3��     : PC8  (�͵�ƽ��ʾ����)
	KEY4��     : PC9  (�͵�ƽ��ʾ����)	
	Sure  	-----> KEY1
	Forward	-----> KEY2
	Backward-----> KEY3
	Cancel	-----> KEY4
	���庯���жϰ����Ƿ��£�����ֵ1 ��ʾ���£�0��ʾδ����
*/												
static uint8_t IsKeyDownSure(void) 		{if (GPIO_ReadInputDataBit(Btn1_PORT,Btn1_PIN) == Bit_SET) return 1; return 0;}
static uint8_t IsKeyDownForward(void) 	{if (GPIO_ReadInputDataBit(Btn2_PORT,Btn2_PIN) == Bit_SET) return 1; return 0;}
static uint8_t IsKeyDownBackward(void)	{if (GPIO_ReadInputDataBit(Btn3_PORT,Btn3_PIN) == Bit_SET) return 1; return 0;}	
static uint8_t IsKeyDownCancel(void)	{if (GPIO_ReadInputDataBit(Btn4_PORT,Btn4_PIN) == Bit_SET) return 1; return 0;}
/*
*********************************************************************************************************
*	�� �� ��: Button_Init
*	����˵��: ��ʼ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Button_Init(void)
{
	bsp_InitButtonVar();		/* ��ʼ���������� */
 	bsp_InitButtonHard();		/* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    �Σ�_KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE)
	{
		s_Key.Write = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Get_Key_State
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t Get_Key_State(void)
{
	uint8_t ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];

		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyState
*	����˵��: ��ȡ������״̬
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t bsp_KeyState(uint8_t _ucKeyID)
{
	uint8_t ucState = 0;

	switch (_ucKeyID)
	{
		case KID_Sure:
			ucState = s_BtnSure.State;
			break;

		case KID_Forward:
			ucState = s_BtnForward.State;
			break;
		
		case KID_Backward:
			ucState = s_BtnBackward.State;
			break;
		
		case KID_Cancel:
			ucState = s_BtnCancel.State;
			break;				
		
	}

	return ucState;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitButtonVar
*	����˵��: ��ʼ������Ӳ��
*	��    �Σ�strName : 
*			  strDate : 
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitButtonHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOE ,
						   ENABLE);
	/* K2/K1/K4/K3 */
	GPIO_InitStructure.GPIO_Pin = Btn1_PIN | Btn2_PIN |
								  Btn3_PIN | Btn4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;    	
	GPIO_Init(GPIOC, &GPIO_InitStructure);   
	
	/* KEY_Power */
	GPIO_InitStructure.GPIO_Pin 	= KEY_Power_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;       
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;	
	GPIO_Init(KEY_Power_PORT, &GPIO_InitStructure);	
	GPIO_SetBits(KEY_Power_PORT, KEY_Power_PIN );// turn off all led  
}
		

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitButtonVar
*	����˵��: ��ʼ����������
*	��    �Σ�strName : ���������ַ���
*			  strDate : ���̷�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitButtonVar(void)
{
	/* �԰���FIFO��дָ������ */
	s_Key.Read = 0;
	s_Key.Write = 0;

	/* ��ʼ��Sure����������֧�ְ��¡����� */
	s_BtnSure.IsKeyDownFunc = IsKeyDownSure;		/* �жϰ������µĺ��� */
	s_BtnSure.FilterTime = BUTTON_FILTER_TIME;		/* �����˲�ʱ�� */
 	s_BtnSure.LongTime = BUTTON_LONG_TIME;			/* ����ʱ�� */
	s_BtnSure.Count = s_BtnSure.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnSure.State = 0;							/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnSure.KeyCodeDown = KEY_DOWN_Sure;			/* �������µļ�ֵ���� */
	s_BtnSure.KeyCodeUp = KEY_UP_Sure;				/* ��������ļ�ֵ���� */
 	s_BtnSure.KeyCodeLong = KEY_LONG_Sure;			/* �������������µļ�ֵ���� */	
	s_BtnSure.RepeatSpeed = 0;						/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnSure.RepeatCount = 0;						/* ���������� */		

	/* ��ʼ��Forward����������֧�ְ��¡����𡢳��� */
	s_BtnForward.IsKeyDownFunc = IsKeyDownForward;		/* �жϰ������µĺ��� */
	s_BtnForward.FilterTime = BUTTON_FILTER_TIME;		/* �����˲�ʱ�� */
	s_BtnForward.LongTime = BUTTON_LONG_TIME;			/* ����ʱ��, 0��ʾ����� */
	s_BtnForward.Count = s_BtnForward.FilterTime / 2;	/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnForward.State = 0;								/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnForward.KeyCodeDown = KEY_DOWN_Forward;		/* �������µļ�ֵ���� */
	s_BtnForward.KeyCodeUp = KEY_UP_Forward;			/* ��������ļ�ֵ���� */
	s_BtnForward.KeyCodeLong = KEY_LONG_Forward;		/* �������������µļ�ֵ���� */
	s_BtnForward.RepeatSpeed = 0;						/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnForward.RepeatCount = 0;						/* ���������� , Ӧ������������������*/		

	/* ��ʼ��Backward��������֧�ְ��¡����𡢳��� */
	s_BtnBackward.IsKeyDownFunc = IsKeyDownBackward;		/* �жϰ������µĺ��� */
	s_BtnBackward.FilterTime = BUTTON_FILTER_TIME;			/* �����˲�ʱ�� */
	s_BtnBackward.LongTime = BUTTON_LONG_TIME;				/* ����ʱ�� */
	s_BtnBackward.Count = s_BtnBackward.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnBackward.State = 0;								/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnBackward.KeyCodeDown = KEY_DOWN_Backward;			/* �������µļ�ֵ���� */
	s_BtnBackward.KeyCodeUp = KEY_UP_Backward;				/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnBackward.KeyCodeLong = KEY_LONG_Backward;			/* �������������µļ�ֵ���룬0��ʾ����� */
	s_BtnBackward.RepeatSpeed = 0;							/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnBackward.RepeatCount = 0;							/* ���������� */
	
	/* ��ʼ��Cancel��������֧�ְ��¡����� */
	s_BtnCancel.IsKeyDownFunc = IsKeyDownCancel;			/* �жϰ������µĺ��� */
	s_BtnCancel.FilterTime = BUTTON_FILTER_TIME;			/* �����˲�ʱ�� */
	s_BtnCancel.LongTime = BUTTON_LONG_TIME;				/* ����ʱ�� */
	s_BtnCancel.Count = s_BtnCancel.FilterTime / 2;			/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnCancel.State = 0;									/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnCancel.KeyCodeDown = KEY_DOWN_Cancel;				/* �������µļ�ֵ���� */
	s_BtnCancel.KeyCodeUp = KEY_UP_Cancel;					/* ��������ļ�ֵ���룬0��ʾ����� */
	s_BtnCancel.KeyCodeLong = KEY_LONG_Cancel;				/* �������������µļ�ֵ���룬0��ʾ����� */
	s_BtnCancel.RepeatSpeed = 0;							/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnCancel.RepeatCount = 0;							/* ���������� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectButton
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    �Σ������ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectButton(BUTTON_T *_pBtn)
{
	/* ���û�г�ʼ�������������򱨴�
	if (_pBtn->IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), _pBtn->IsKeyDownFunc undefine");
	}
	*/

	if (_pBtn->IsKeyDownFunc())
	{
		if (_pBtn->Count < _pBtn->FilterTime)
		{
			_pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count < 2 * _pBtn->FilterTime)
		{
			_pBtn->Count++;
		}
		else
		{
			if (_pBtn->State == 0)
			{
				_pBtn->State = 1;

//				/* ���Ͱ�ť���µ���Ϣ */
//				if (_pBtn->KeyCodeDown > 0)
//				{
//					/* ��ֵ���밴��FIFO */
//					bsp_PutKey(_pBtn->KeyCodeDown);
//				}
			}

			if (_pBtn->LongTime > 0)
			{
				if (_pBtn->LongCount < _pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++_pBtn->LongCount == _pBtn->LongTime)
					{
						/* ��ֵ���밴��FIFO */
						bsp_PutKey(_pBtn->KeyCodeLong);						
					}
				}
				else
				{
					if (_pBtn->RepeatSpeed > 0)
					{
						if (++_pBtn->RepeatCount >= _pBtn->RepeatSpeed)
						{
							_pBtn->RepeatCount = 0;
							/* ��������ÿ��10ms����1������ */
							bsp_PutKey(_pBtn->KeyCodeDown);														
						}
					}
				}
			}
		}
	}
	else
	{
		if(_pBtn->Count != 0)
		{
			if (_pBtn->State == 1)
			{
				_pBtn->State = 0;

				if (_pBtn->LongTime > 0)
				{
					/* �ж� ���� ������ ֵ */
					if (_pBtn->LongCount < _pBtn->LongTime)
					{
						/* ���Ͱ�ť���µ���Ϣ */
						if (_pBtn->KeyCodeDown > 0)
						{
							/* ��ֵ���밴��FIFO */
							bsp_PutKey(_pBtn->KeyCodeDown);
						}		
					}else
					{

						__nop();
					}
				}else
				{
					/* ���Ͱ�ť���µ���Ϣ */
					if (_pBtn->KeyCodeDown > 0)
					{
						/* ��ֵ���밴��FIFO */
						bsp_PutKey(_pBtn->KeyCodeDown);
					}
				}

//				/* ���Ͱ�ť�������Ϣ */
//				if (_pBtn->KeyCodeUp > 0)
//				{
//					/* ��ֵ���밴��FIFO */
//					bsp_PutKey(_pBtn->KeyCodeUp);			
//				}
			}
			_pBtn->LongCount = 0;
			_pBtn->RepeatCount = 0;
			_pBtn->Count =0;
		}
	}
}

///* **********************************************************************************
//
// * ��������Circulate_Data_Value_Deal
// * ����  ����ѭ����ֵ�Ĵ���
// * ����  ��data_p -> ���ݵ�ַָ��
//		   up_limit -> ��������
//		   down_limit -> ��������
//		   offset -> ����ƫ����
// * ���  ����
//
// * **********************************************************************************/
//void Circulate_Data_Value_Deal(u32* data_p,u32 up_limit,u32 down_limit,s32 offset)
//{
//	/* �ж�ƫ��������ֵ */
//	if(offset > 0)
//	{
//		//��ֵ
//		/* �ж��Ƿ���	*/
//		if((((s32)(*data_p)) + offset) <= up_limit)
//		{
//			/* û�г��� */
//			(*data_p) = (((s32)(*data_p))+ offset);
//		}
//	}else if(offset < 0)
//	{
//		//��ֵ
//		/* �ж��Ƿ��� */
//		if((((s32)(*data_p)) + offset) >= down_limit)
//		{
//			/* û�г��� */
//			(*data_p) = (((s32)(*data_p)) + offset);
//		}
//		
//	}else
//	{
//		//����0
//		/* Ԥ���ţ���������. */
//	}
//}

/************************************************************************************************/
/* ������	: Key_State_Check_All																*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ������а�����������״̬�����뱻�����Եĵ��á�											*/
/* ��������	: 2015/6/15																			*/
/************************************************************************************************/ 
void Key_State_Check_All(void)
{
	bsp_DetectButton(&s_BtnSure);		/* ȷ�� �� */
	bsp_DetectButton(&s_BtnForward);	/* ��ǰ �� */
	bsp_DetectButton(&s_BtnBackward);	/* ��� �� */
	bsp_DetectButton(&s_BtnCancel);		/* ȡ�� �� */		
}
