/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/09/15																			 */
/*************************************************************************************************/
#include "Dev_Fram.h"
#include "Mcu_Fram.h"
#include "interface.h"

#include  "ucos_ii.h"
#include <string.h>








/*************************************************************************************************/
/* ��������Write_Config_Tab																		 */
/* ����  ��д ���� ������Ϣ��																		 */
/* ����  ��buffer:���ݴ洢ָ�룻count:дȡ��������													 */
/* ����  ����																					 */
/* ע��  ������ʹ�õ���ģ��I2C																		 */
/*************************************************************************************************/
void Write_Config_Tab(u8* buffer_p,u16 count)
{
/* ���ñ���Ϣ */
/* �������ñ�ʼ��ַ */
#define Instru_Config_Tab_Start_Adder	((u32)&Instru_Config_Inf_Tab.Inf_State_Space[0])
/* �������ñ������ַ */
#define Instru_Config_Tab_End_Adder		((u32)&Instru_Config_Inf_Tab.Inf_State_Space[Instru_Inf_State_Space_Size])	
/* �������ñ�ʼ��ַ */
#define Instru_Fix_Tab_Start_Adder		((u32)&Instru_Fix_Inf_State_Tab.Inf_State_Space[0])
/* �������ñ������ַ */
#define Instru_Fix_Tab_End_Adder		((u32)&Instru_Fix_Inf_State_Tab.Inf_State_Space[Instru_Fix_Inf_Tab_Space_Size])		

	
	/* д���ݵ�ַ */
	u32 w_adder;
	
	/* �������ݻ���ָ���жϱ��β����Ƿ�ִ�� */
	if(((u32)buffer_p 				>= Instru_Config_Tab_Start_Adder)&&
	  (((u32)buffer_p+count) 		<= Instru_Config_Tab_End_Adder))
	{
		/* ����д���ݵ�ַ(���ñ��ַ) */
		w_adder = (((u32)buffer_p)-Instru_Config_Tab_Start_Adder)+Instru_Inf_Tab_Mem_Star;
	}else if(((u32)buffer_p 		>= Instru_Fix_Tab_Start_Adder)&&
			(((u32)buffer_p+count) 	<= Instru_Fix_Tab_End_Adder))
	{
		/* ����д���ݵ�ַ(���ñ��ַ) */
		w_adder = (((u32)buffer_p)-Instru_Fix_Tab_Start_Adder)+Instru_Fix_Inf_Tab_Mem_Star;
	}
	else
	{
		/* д�����ݲ��ֻ�ȫ�����ݲ��ٿ�д�ĵ�ַ��Χ�ڡ� */
		return;
	}
	
	/* ʹ��д���� */
	FramWP_GPIO_Enable();
	/* ������������ */
	OSSchedLock();
	/* �� ָ�����ݻ���� ���� д���������ñ��FRAM�� */
	AI2C_Write((char*)buffer_p,w_adder,count);
	/* ������������ */
	OSSchedUnlock();
	/* �ر�д���� */
	FramWP_GPIO_DISABLE();
	
#undef Instru_Config_Tab_Start_Adder
#undef Instru_Config_Tab_End_Adder
#undef Instru_Fix_Tab_Start_Adder
#undef Instru_Fix_Tab_End_Adder	
}

/*************************************************************************************************/
/* ��������Read_Config_Tab																		 */
/* ����  ���� ���� ������Ϣ��																		 */
/* ����  ��buffer:���ݴ洢ָ�룻count:дȡ��������													 */
/* ����  ����																					 */
/* ע��  ������ʹ�õ���ģ��I2C																		 */
/*************************************************************************************************/
void Read_Config_Tab(u8* buffer_p,u16 count)
{
/* ���ñ���Ϣ */
/* �������ñ�ʼ��ַ */
#define Instru_Config_Tab_Start_Adder	((u32)&Instru_Config_Inf_Tab.Inf_State_Space[0])
/* �������ñ������ַ */
#define Instru_Config_Tab_End_Adder		((u32)&Instru_Config_Inf_Tab.Inf_State_Space[Instru_Inf_State_Space_Size])	
/* �������ñ�ʼ��ַ */
#define Instru_Fix_Tab_Start_Adder		((u32)&Instru_Fix_Inf_State_Tab.Inf_State_Space[0])
/* �������ñ������ַ */
#define Instru_Fix_Tab_End_Adder		((u32)&Instru_Fix_Inf_State_Tab.Inf_State_Space[Instru_Fix_Inf_Tab_Space_Size])			

	
	/* д���ݵ�ַ */
	u32 r_adder;

	
	/* �������ݻ���ָ���жϱ��β����Ƿ�ִ�� */
	if(((u32)buffer_p 				>= Instru_Config_Tab_Start_Adder)&&
	  (((u32)buffer_p+count) 		<= Instru_Config_Tab_End_Adder))
	{
		/* ����д���ݵ�ַ(���ñ��ַ) */
		r_adder = (((u32)buffer_p)-Instru_Config_Tab_Start_Adder)+Instru_Inf_Tab_Mem_Star;
	}else if(((u32)buffer_p 		>= Instru_Fix_Tab_Start_Adder)&&
			(((u32)buffer_p+count) 	<= Instru_Fix_Tab_End_Adder))
	{
		/* ����д���ݵ�ַ(���ñ��ַ) */
		r_adder = (((u32)buffer_p)-Instru_Fix_Tab_Start_Adder)+Instru_Fix_Inf_Tab_Mem_Star;
	}
	else
	{
		/* д�����ݲ��ֻ�ȫ�����ݲ��ٿ�д�ĵ�ַ��Χ�ڡ� */
		return;
	}
	
	/* ������������ */
	OSSchedLock();
	/* �� ָ�����ݻ���� ���� д���������ñ��FRAM�� */
	AI2C_Read((char*)buffer_p,r_adder,count);
	/* ������������ */
	OSSchedUnlock();
	
#undef Instru_Config_Tab_Start_Adder
#undef Instru_Config_Tab_End_Adder
#undef Instru_Fix_Tab_Start_Adder
#undef Instru_Fix_Tab_End_Adder	
}





/*************************END OF FILE*************************************/
