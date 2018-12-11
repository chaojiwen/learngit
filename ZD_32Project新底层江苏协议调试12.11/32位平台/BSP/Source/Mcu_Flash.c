/************************************************************************************************/
/** @file              : Mcu_Flash.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��08��25��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--MCU�ڲ�FLASH�ײ������ļ�						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��08��25��     ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Mcu_Flash.h"
#include "Mcu_Config.h"
#include "Log_WatchDog.h"
#include "string.h"
#include "stdio.h"

#if (1)//�洢��غ궨��

/* �������ݻ���ַ */
//�������STM32F103VEƬ�ڵ�Flash����ô�洢�����ĵ�ַ�������2K������
#define ParaBaseAddre       	(BLCOK_SYS_PARAM_START)
/* �������ݴ洢������С *///���2K�����������
#define ParaCapacity      		(BLCOK_SYS_PARAM_SPACE)
/* �������ݵ�ҳ��С */
#define ParaPageSpace      		(FLASH_PAGE_SIZE)

//�������STM32F103VEƬ�ڵ�Flash����ô�洢�����ĵ�ַ�������2K������
#define UpgradeBaseAddre    	(BLOCK_CODESTORE_START)
/* �������ݴ洢������С *///���2K�����������
#define UpgradeCapacity      	(BLOCK_CODESTORE_SPACE)//250K
/* �������ݵ�ҳ��С */
#define UpgradePageSpace     	(FLASH_PAGE_SIZE)

#endif

#if (1)//�洢��ؽṹ����

/* �̼�����״̬��Ϣ���� */
typedef enum {
	
	/* flash���� */
	NandFlash = 0x00,
	//����ֻ��һ�����ͺ���������
	
}StorageType;

/* �洢����Ϣ�ṹ */
typedef struct {
	/* ���� */
	StorageType	Type;
	/* �洢�ռ� */
	uint32_t	Size;
	/* �洢���С */
	int 	BlockSize;
}StorageInfStruct;

/* �洢���ṹ */
typedef struct {
	/* �����洢�� */
	StorageInfStruct	Para;
	/* �̼����ݱ��ݴ洢�� */
	StorageInfStruct	Data;
}StorageStruct;

/* �洢�����ݼĴ��� */
static StorageStruct Storage = 
{
	/* �����洢�� */
	/* ���� */
	NandFlash,
	/* �洢�ռ� */
	ParaCapacity,
	/* �洢���С */
	ParaPageSpace,
	
	/* �̼����ݱ��ݴ洢�� */
	/* ���� */
	NandFlash,
	/* �洢�ռ� */
	UpgradeCapacity,
	/* �洢���С */
	UpgradePageSpace,
};

#endif

/************************************************************************************************/

/************************************************************************************************/
#if (1)//�洢��������
/************************************************************************************************/
/* ������	: Write_Para																		*/
/* ����  	: adder:д��ַ;data:����;Len:д���ݳ���; 											*/
/* ���		: д����״̬ 																		*/
/* ����	 	: д����洢��д�����																*/
/* �������� 	: 2016/03/05																		*/
/************************************************************************************************/
BOOL Write_Para(uint32_t adder,uint8_t * data,uint32_t Len)
{
	/* ������ */
    uint32_t num0;
	
	/* �ж�д���������Ƿ�Ϸ� */
	/* 1���жϵ�ַ�Ƿ��ڲ��������ռ��� */
	if(adder >= Storage.Para.Size)
	{
		/* д������ַ�����˲��������ռ� */
		return (FALSE);
	}
	/* 2���ж�д�������Ƿ�ȫ�����ڲ��������ռ��� */
	else if((adder+Len) >= Storage.Para.Size)
	{
		/* д�������ݳ����˲��������ռ� */
		return (FALSE);
	}
	
	/* 3���洢���� */
	/* �жϵ�ǰ�Ĵ洢������ */
	if(Storage.Para.Type == NandFlash)
	{
		/* �洢��ΪNandFlash */
		/* �ȶ�ȡ��ǰҳ��ȫ�����ݣ����ڵ�ǰ�����洢�ռ��һҳ��
		   ��ˣ�ÿ�ζ���Ҫ��ȡ��Ҳ��ȫ�����ݣ�
		ע��:�����п��ܵ���Flash��������! */
		
		/* �������ݻ���ʱ�洢 */
		uint8_t paraBuf[ParaPageSpace];
		/* NandFlashҳ��������״̬ */
		FLASH_Status eraseState;
		/* ��ǰҳ��ַ */
		uint32_t currentPage = ParaBaseAddre+adder;
		/* ��һҳ��ַ */
		uint32_t nextPage = ParaBaseAddre+adder+ParaPageSpace;
		
		/* ��ֵȫ�������������� */
		memcpy(paraBuf,(uint8_t*)(ParaBaseAddre),ParaPageSpace);
		
		/* ��������д�뵽����ռ� */
		memcpy(paraBuf,data,Len);
		
		/* ����Flash */
		FLASH_Unlock();
		/* ���Flash״̬��־ */
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		
		/* ������ǰҳ���� */
		eraseState = FLASH_ErasePage(currentPage);
		/* �жϲ�������״̬ */
		if(eraseState != FLASH_COMPLETE)
		{
			/* ����ʧ�ܣ��鿴ʧ��ԭ�� */
			__nop();
			return (FALSE);
		}
		
		for (num0 = 0; num0 < ParaPageSpace; num0 += 4)
		{
			/* �洢��ΪNandFlash */
			/* ��NandFlash��д������ */
			FLASH_ProgramWord(currentPage+num0,*((uint32_t*)(paraBuf+num0)));
			
			/* �ж����������ҳд */
			if((currentPage+num0) > nextPage)
			{
				//���ﲻ�����д������⣬�������д�������������2���˳�����
				/* ������һҳ���� */
				eraseState = FLASH_ErasePage(nextPage);
				/* �жϲ�������״̬ */
				if(eraseState != FLASH_COMPLETE)
				{
					/* ����ʧ�ܣ��鿴ʧ��ԭ�� */
					__nop();
					return (FALSE);
				}else//�������ڴ洢�����洢����ֻ��һҳ�����Բ��ᵽ����ִ�д��룬\
					   ���ǿ��ǵײ����ı�־�Ծ���д��������ܡ�
				{
					/* �����ɹ���������һҳΪ��ǰҳ */
					currentPage = nextPage;
					nextPage += ParaPageSpace;
				}
			}
		}
		/* ����Flash */
		FLASH_Lock();
	}

	/* �����ɹ� */
    return (TRUE);
}

/************************************************************************************************/
/* ������	: Read_Para																			*/
/* ����  	: adder:д��ַ;data:����;Len:д���ݳ���; 											*/
/* ���		: ������״̬ 																		*/
/* ����	 	: ���洢������																		*/
/* �������� 	: 2016/03/06																		*/
/************************************************************************************************/
BOOL Read_Para(uint32_t adder,uint8_t * data,uint32_t Len)
{

	/* �ж�д���������Ƿ�Ϸ� */
	/* 1���жϵ�ַ�Ƿ��ڲ��������ռ��� */
	if(adder >= Storage.Para.Size)
	{
		/* д������ַ�����˲��������ռ� */
		return (FALSE);
	}
	/* 2���ж�д�������Ƿ�ȫ�����ڲ��������ռ��� */
	else if((adder+Len) >= Storage.Para.Size)
	{
		/* д�������ݳ����˲��������ռ� */
		return (FALSE);
	}

	
	/* �жϵ�ǰ�Ĵ洢������ */
	if(Storage.Para.Type == NandFlash)
	{
		/* ���������������� */
		memcpy(data,(uint8_t*)(ParaBaseAddre+adder),Len);
	}
	
	/* �����ɹ� */
    return (TRUE);
}

/************************************************************************************************/
/* ������	: Write_Data																		*/
/* ����  	: adder:д��ַ;data:����;Len:д���ݳ���; 											*/
/* ���		: д�̼�������������״̬ 															*/
/* ����	 	: �����̼�����������																	*/
/* �������� 	: 2016/03/06																		*/
/************************************************************************************************/
BOOL Write_Data(uint32_t adder,uint8_t * data,uint32_t Len)
{
	/* ������ */
    uint32_t num0;
	
	/* �ж�д���������Ƿ�Ϸ� */
	/* 1���жϵ�ַ�Ƿ��ڲ��������ռ��� */
	if(adder >= Storage.Data.Size)
	{
		/* д������ַ�����˲��������ռ� */
		return (FALSE);
	}
	/* 2���ж�д�������Ƿ�ȫ�����ڲ��������ռ��� */
	else if((adder+Len) >= Storage.Data.Size)
	{
		/* д�������ݳ����˲��������ռ� */
		return (FALSE);
	}
	
	/* 3���洢���� */
	/* �жϵ�ǰ�Ĵ洢������ */
	if(Storage.Data.Type == NandFlash)
	{
		/* �洢��ΪNandFlash */
		/* ÿ�ζ���Ҫ��ȡ��Ҳ��ȫ�����ݣ�ע��:�����п��ܵ���Flash��������! */
		/* ��ǰҳ��ַ */
		uint32_t currentPage = UpgradeBaseAddre+adder;

		
		/* ����Flash */
		FLASH_Unlock();
		/* ���Flash״̬��־ */
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

		for (num0 = 0; num0 < Len; num0 += 4)
		{
			/* �洢��ΪNandFlash */
			/* ��NandFlash��д������ */
			FLASH_ProgramWord(currentPage+num0,*((uint32_t*)(data+num0)));
			
			 MAX823_WDI_Signal();//ι��
			
		}
		
		/* ����Flash */
		FLASH_Lock();
	}

	/* �����ɹ� */
    return (TRUE);
}

/************************************************************************************************/
/* ������	: Erase_Data																		*/
/* ����  	: ��		 																		*/
/* ���		: д�̼�������������״̬ 															*/
/* ����	 	: �����̼�����������																	*/
/* �������� 	: 2016/03/06																		*/
/************************************************************************************************/
void Erase_Data(void)
{
	/* ��ǰҳ��ַ */
	uint32_t currentPage = UpgradeBaseAddre;
	
	/* ����Flash */
	FLASH_Unlock();
	/* ���Flash״̬��־ */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	/* �������������� */
	for (; currentPage < (UpgradeBaseAddre+UpgradeCapacity); currentPage += UpgradePageSpace)
	{
		/* ������ǰҳ���� */
		FLASH_ErasePage(currentPage);
		
		MAX823_WDI_Signal();//ι��
		
	}

	/* ����Flash */
	FLASH_Lock();
}

/************************************************************************************************/
/* ������	: Erase_Para																		*/
/* ����  	: �� 																				*/
/* ���		: �� 																				*/
/* ����	 	: �������ݲ���																		*/
/* �������� 	: 2016/03/08																		*/
/************************************************************************************************/
void Erase_Para(void)
{
	/* ��ǰҳ��ַ */
	uint32_t currentPage = UpgradeBaseAddre;
	
	/* ����Flash */
	FLASH_Unlock();
	/* ���Flash״̬��־ */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	/* �������������� */
	for (; currentPage < (UpgradeBaseAddre+UpgradeCapacity); currentPage += UpgradePageSpace)
	{
		/* ������ǰҳ���� */
		FLASH_ErasePage(currentPage);
		MAX823_WDI_Signal();
		
	}

	/* ����Flash */
	FLASH_Lock();
}

#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
