/************************************************************************************************/
/** @file              : MenuCutPowerSoundLightAlarmSet.c										*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015��09��25��															*/
/** @date changed      : 2015��09��25��															*/
/** @brief             : c file																	*/
/** @description       : GUI�˵�->�ϵ����ⱨ�����ý���											*/
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

/* �͵������ⱨ������ ���� �˵���Ŀ�� */
static u8 * const Menu_Cut_Pwr_Sud_Lgt_Aam_Set_CN[]=
{
	{Menu_ON_CN},
	{Menu_OFF_CN},		
};

/* �͵������ⱨ������ Ӣ�� �˵���Ŀ�� */
static u8 * const Menu_Cut_Pwr_Sud_Lgt_Aam_Set_EN[]=
{
	{Menu_ON_EN},
	{Menu_OFF_EN},		
};

/* �͵������ⱨ������ ��Ӣ�� �˵��� */
const struct Menu Menu_Cut_Pwr_Sud_Lgt_Aam_Set[] =
{
	MenuWhat(Menu_Cut_Pwr_Sud_Lgt_Aam_Set_CN),
	MenuWhat(Menu_Cut_Pwr_Sud_Lgt_Aam_Set_EN),
};


/* �͵��� "��" �������� ָ�� */
static BOOL *Cut_Pwr_Sud_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Sound_Alarm_Switch;
/* �͵��� "��" �������� ָ�� */
static BOOL *Cut_Pwr_Lgt_Sth_P = &Instru_Config_Inf_Tab.Inf_State.Instru_Power.Inf_State.ET_Light_Alarm_Switch;




/************************************************************************************************/
/* ������	: DispMenuCutPwrSudLgtAamSetInit													*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ��ʾ �͵������ⱨ�� ������Ϣ														*/
/* ��������	: 2015/09/21																		*/
/************************************************************************************************/
void DispMenuCutPwrSudLgtAamSetInit(void)
{
	/* �ֶ����ò��� */
    if((Flash == FlashMode_AutoInit)||(Flash == FlashMode_ManualInit)||(Flash == FlashMode_ReFlash))
	{
		/* �����Ļ��ʾ */
		GUI_Clear();
		/* �л�Bar��ʾ���� */
		BarMenu = (struct Menu *)&Menu_Cut_Pwr_Sud_Lgt_Aam_Set[*Language];
		ItemNum = (*BarMenu).TextNum;
		DispItem = (*BarMenu).Text;
		Item = ((*Cut_Pwr_Sud_Sth_P  == TRUE)&&(*Cut_Pwr_Lgt_Sth_P == TRUE)) ? (0u) : (1u);		
        Where = Item;
		/* ��ʾ��ǰ�����µ���Ŀ�͹����� */
		BarMenuInit();
		/* ��ʾ��ǰ��������� */
		GUI_Disp_Headline(*(Menu_Sud_Lgt_Aam[*Language].Text+ItemBackup[ItemBackup_i-1]));		
    }

}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSudLgtAamSetUp														*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ���� �͵������ⱨ�� ���ܿ���														*/
/* ��������	: 2015/09/21																		*/
/************************************************************************************************/
void DispMenuCutPwrSudLgtAamSetUp(void)
{

    switch(Item)
	{
        case 0:
		{
			/* �͵��� "��" ����:�� */
			if((*Cut_Pwr_Sud_Sth_P == FALSE) || (*Cut_Pwr_Lgt_Sth_P == FALSE))
			{
				/* ���ⱨ��״̬Ϊ:��,�������ⱨ��. */
				*Cut_Pwr_Sud_Sth_P = TRUE;
				*Cut_Pwr_Lgt_Sth_P = TRUE;

				/* �޸� ���� ��Ϣ�� ���ñ�־ */
				Write_Config_Tab((u8*)Cut_Pwr_Sud_Sth_P,sizeof(*Cut_Pwr_Sud_Sth_P));
				Write_Config_Tab((u8*)Cut_Pwr_Lgt_Sth_P,sizeof(*Cut_Pwr_Lgt_Sth_P));
				/* �˻ص���һ������ */
				POP();
			}else
			{
				/* ������ָ��ָ���ʼ������ */
				Jump2Menu(MenuCutPwrSudLgtAamSetIndex,FlashMode_NoAction);
			}
			break;
        }
		
        case 1:
		{
			/* �͵��� "��" ����:�� */
			if((*Cut_Pwr_Sud_Sth_P == TRUE) || (*Cut_Pwr_Lgt_Sth_P == TRUE))
			{
				/* ���ⱨ��״̬Ϊ:��,�ر����ⱨ��. */
				*Cut_Pwr_Sud_Sth_P = FALSE;
				*Cut_Pwr_Lgt_Sth_P = FALSE;
				
				/* �޸� ���� ��Ϣ�� ���ñ�־ */
				Write_Config_Tab((u8*)Cut_Pwr_Sud_Sth_P,sizeof(*Cut_Pwr_Sud_Sth_P));
				Write_Config_Tab((u8*)Cut_Pwr_Lgt_Sth_P,sizeof(*Cut_Pwr_Lgt_Sth_P));
				/* �˻ص���һ������ */
				POP();
			}else
			{
				/* ������ָ��ָ���ʼ������ */
				Jump2Menu(MenuCutPwrSudLgtAamSetIndex,FlashMode_NoAction);
			}
	        break;
        }


        default:
		{
			/* δ�������,���ý��档 */
			/* ������ָ��ָ���ʼ������ */
			Jump2Menu(MenuCutPwrSudLgtAamSetIndex,FlashMode_NoAction);
			break;
		}
    }


}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSudLgtAamSetDown													*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: �˳� �͵������ⱨ�� ����															*/
/* ��������	: 2015/09/22																		*/
/************************************************************************************************/
void DispMenuCutPwrSudLgtAamSetDown(void)
{
	/* �˻ص���һ������ */
	POP();
}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSudLgtAamSetLeft													*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ѡ�� �� �͵������ⱨ�� ����														*/
/* ��������	: 2015/09/22																			*/
/************************************************************************************************/
void DispMenuCutPwrSudLgtAamSetLeft(void)
{
    BarMenuLeft();
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuCutPwrSudLgtAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSudLgtAamSetRight														*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ѡ�� �ر� �͵������ⱨ�� ����																	*/
/* ��������	: 2015/09/22																			*/
/************************************************************************************************/
void DispMenuCutPwrSudLgtAamSetRight(void)
{
    BarMenuRight();
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuCutPwrSudLgtAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSudLgtAamSetOK															*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����ȷ����������ʱû��ʹ�ã�															*/
/* ��������	: 2015/09/22																			*/
/************************************************************************************************/
void DispMenuCutPwrSudLgtAamSetOK(void)
{
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuCutPwrSudLgtAamSetIndex,FlashMode_NoAction);
}

/************************************************************************************************/
/* ������	: DispMenuCutPwrSudLgtAamSetCancel															*/
/* ����		: ��																				*/
/* ���		: �� 																				*/
/* ����		: ����ȡ����������ʱû��ʹ�ã�															*/
/* ��������	: 2015/09/22																			*/
/************************************************************************************************/
void DispMenuCutPwrSudLgtAamSetCancel(void)
{
	/* ������ָ��ָ���Ӧ����ĳ�ʼ������ */
	Jump2Menu(MenuCutPwrSudLgtAamSetIndex,FlashMode_NoAction);
}  

/******************* (C) ZHOU *****END OF FILE****/

