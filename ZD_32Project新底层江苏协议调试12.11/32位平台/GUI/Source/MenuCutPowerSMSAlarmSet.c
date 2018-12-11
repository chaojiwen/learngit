/************************************************************************************************/
/** @file              : MenuCutPowerSMSAlarmSet.c												*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��09��29��															*/
/** @date changed      : 2015��09��29��															*/
/** @brief             : c file																	*/
/** @description       : GUI�˵�->�ϵ���ű�����������											*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/
#include "Ks0108.H"
#include "Menu.H"

/* �ϵ���ű������� ���� �˵���Ŀ�� */
static u8 * const Menu_Cut_Pwr_SMS_Aam_Set_CN[]=
{
	{Menu_ON_CN},
	{Menu_OFF_CN},		
};

/* �ϵ���ű������� Ӣ�� �˵���Ŀ�� */
static u8 * const Menu_Cut_Pwr_SMS_Aam_Set_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},		
};

/* �ϵ���ű������� ��Ӣ�� �˵��� */
const struct Menu Menu_Cut_Pwr_SMS_Aam_Set[] =
{
	MenuWhat(Menu_Cut_Pwr_SMS_Aam_Set_CN),
	MenuWhat(Menu_Cut_Pwr_SMS_Aam_Set_EN),
};


/* �ϵ籨������ָ�� */
static BOOL *Cut_Pwr_SMS_Aam_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_SMS_Alarm_Switch;


/************************************************************************************************/
/* ������	: DispMenuCutPwrSMSAamSetInit													*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ��ʾ �ϵ���ű��� ������Ϣ														*/
/* ��������	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetInit(void)
{
	/* �ֶ����ò��� */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* �����Ļ��ʾ */
		GUI_Clear();
		/* �л�Bar��ʾ���� */
		BarMenu = (struct Menu *)&Menu_Cut_Pwr_SMS_Aam_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		Item = (*Cut_Pwr_SMS_Aam_Sth_P  == TRUE) ? (0u) : (1u);		
        Where = Item;
		/* ��ʾ��ǰ�����µ���Ŀ�͹����� */
		BarMenuInit();
		/* ��ʾ��ǰ��������� */
		GUI_Disp_Headline(*(Menu_SMS_Aam[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSMSAamSetUp														*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ���� �ϵ���ű��� ���ܿ���														*/
/* ��������	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetUp(void)
{

    switch(Item)
	{
        case 0:
		{
			/* �ж϶��Ŷϵ籨��״̬. */
			if(*Cut_Pwr_SMS_Aam_Sth_P == FALSE)
			{
				/* ���ű���״̬Ϊ:��,�������ⱨ��. */
				*Cut_Pwr_SMS_Aam_Sth_P = TRUE;

				/* �޸� ���� ��Ϣ�� ���ñ�־ */
				Write_Config_Tab((u8*)Cut_Pwr_SMS_Aam_Sth_P,sizeof(*Cut_Pwr_SMS_Aam_Sth_P));
				
				/* �˻ص���һ������ */
				POP();
			}else
			{
				/* ������ָ��ָ���ʼ������ */
				Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
			}
			break;
        }
		
        case 1:
		{
			/* �ж϶��Ŷϵ籨��״̬. */
			if(*Cut_Pwr_SMS_Aam_Sth_P == TRUE)
			{
				/* ���ű���״̬Ϊ:��,�ر����ⱨ��. */
				*Cut_Pwr_SMS_Aam_Sth_P = FALSE;
				
				/* �޸� ���� ��Ϣ�� ���ñ�־ */
				Write_Config_Tab((u8*)Cut_Pwr_SMS_Aam_Sth_P,sizeof(*Cut_Pwr_SMS_Aam_Sth_P));
				
				/* �˻ص���һ������ */
				POP();
			}else
			{
				/* ������ָ��ָ���ʼ������ */
				Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
			}
	        break;
        }


        default:
		{
			/* δ�������,���ý��档 */
			/* ������ָ��ָ���ʼ������ */
			Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
			break;
		}
    }


}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSMSAamSetDown													*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: �˳� �ϵ���ű��� ����															*/
/* ��������	: 2015/09/29																	*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetDown(void)
{
	/* �˻ص���һ������ */
	POP();
}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSMSAamSetLeft													*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ѡ�� "��" �ϵ���ű��� ����														*/
/* ��������	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetLeft(void)
{
    BarMenuLeft();
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSMSAamSetRight														*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ѡ�� "��" �ϵ���ű��� ����																	*/
/* ��������	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetRight(void)
{
    BarMenuRight();
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSMSAamSetOK															*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����ȷ����������ʱû��ʹ�ã�															*/
/* ��������	: 2015/09/29																		*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetOK(void)
{
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSMSAamSetCancel															*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����ȡ����������ʱû��ʹ�ã�															*/
/* ��������	: 2015/09/29																			*/
/************************************************************************************************/
void DispMenuCutPwrSMSAamSetCancel(void)
{
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuCutPwrSMSAamSetIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/


