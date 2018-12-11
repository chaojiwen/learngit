/************************************************************************************************/
/** @file              : Mcu_Flash.c															*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年08月25日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--MCU内部FLASH底层驱动文件						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年08月25日     初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#include "Mcu_Flash.h"
#include "Mcu_Config.h"
#include "Log_WatchDog.h"
#include "string.h"
#include "stdio.h"

#if (1)//存储相关宏定义

/* 参数数据基地址 */
//如果是用STM32F103VE片内的Flash，那么存储参数的地址就是最后2K数据区
#define ParaBaseAddre       	(BLCOK_SYS_PARAM_START)
/* 参数数据存储容量大小 *///最后2K用来保存参数
#define ParaCapacity      		(BLCOK_SYS_PARAM_SPACE)
/* 参数数据的页大小 */
#define ParaPageSpace      		(FLASH_PAGE_SIZE)

//如果是用STM32F103VE片内的Flash，那么存储参数的地址就是最后2K数据区
#define UpgradeBaseAddre    	(BLOCK_CODESTORE_START)
/* 参数数据存储容量大小 *///最后2K用来保存参数
#define UpgradeCapacity      	(BLOCK_CODESTORE_SPACE)//250K
/* 参数数据的页大小 */
#define UpgradePageSpace     	(FLASH_PAGE_SIZE)

#endif

#if (1)//存储相关结构变量

/* 固件升级状态消息类型 */
typedef enum {
	
	/* flash类型 */
	NandFlash = 0x00,
	//现在只有一种类型后面可以添加
	
}StorageType;

/* 存储器信息结构 */
typedef struct {
	/* 类型 */
	StorageType	Type;
	/* 存储空间 */
	uint32_t	Size;
	/* 存储块大小 */
	int 	BlockSize;
}StorageInfStruct;

/* 存储器结构 */
typedef struct {
	/* 参数存储器 */
	StorageInfStruct	Para;
	/* 固件数据备份存储器 */
	StorageInfStruct	Data;
}StorageStruct;

/* 存储器数据寄存器 */
static StorageStruct Storage = 
{
	/* 参数存储器 */
	/* 类型 */
	NandFlash,
	/* 存储空间 */
	ParaCapacity,
	/* 存储块大小 */
	ParaPageSpace,
	
	/* 固件数据备份存储器 */
	/* 类型 */
	NandFlash,
	/* 存储空间 */
	UpgradeCapacity,
	/* 存储块大小 */
	UpgradePageSpace,
};

#endif

/************************************************************************************************/

/************************************************************************************************/
#if (1)//存储驱动函数
/************************************************************************************************/
/* 函数名	: Write_Para																		*/
/* 输入  	: adder:写地址;data:数据;Len:写数据长度; 											*/
/* 输出		: 写参数状态 																		*/
/* 作用	 	: 写入向存储器写入参数																*/
/* 创建日期 	: 2016/03/05																		*/
/************************************************************************************************/
BOOL Write_Para(uint32_t adder,uint8_t * data,uint32_t Len)
{
	/* 计数器 */
    uint32_t num0;
	
	/* 判断写各个参数是否合法 */
	/* 1、判断地址是否在参数容量空间内 */
	if(adder >= Storage.Para.Size)
	{
		/* 写参数地址超出了参数容量空间 */
		return (FALSE);
	}
	/* 2、判断写的数据是否全部都在参数容量空间内 */
	else if((adder+Len) >= Storage.Para.Size)
	{
		/* 写参数数据超出了参数容量空间 */
		return (FALSE);
	}
	
	/* 3、存储数据 */
	/* 判断当前的存储器类型 */
	if(Storage.Para.Type == NandFlash)
	{
		/* 存储器为NandFlash */
		/* 先读取当前页的全部数据，由于当前参数存储空间就一页，
		   因此，每次都需要读取该也的全部数据，
		注意:这样有可能导致Flash寿命问题! */
		
		/* 开辟数据缓临时存储 */
		uint8_t paraBuf[ParaPageSpace];
		/* NandFlash页擦除操作状态 */
		FLASH_Status eraseState;
		/* 当前页地址 */
		uint32_t currentPage = ParaBaseAddre+adder;
		/* 下一页地址 */
		uint32_t nextPage = ParaBaseAddre+adder+ParaPageSpace;
		
		/* 赋值全部参数到缓存中 */
		memcpy(paraBuf,(uint8_t*)(ParaBaseAddre),ParaPageSpace);
		
		/* 将数据先写入到缓存空间 */
		memcpy(paraBuf,data,Len);
		
		/* 解锁Flash */
		FLASH_Unlock();
		/* 清除Flash状态标志 */
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		
		/* 擦除当前页数据 */
		eraseState = FLASH_ErasePage(currentPage);
		/* 判断擦除操作状态 */
		if(eraseState != FLASH_COMPLETE)
		{
			/* 擦除失败，查看失败原因 */
			__nop();
			return (FALSE);
		}
		
		for (num0 = 0; num0 < ParaPageSpace; num0 += 4)
		{
			/* 存储器为NandFlash */
			/* 向NandFlash中写入数据 */
			FLASH_ProgramWord(currentPage+num0,*((uint32_t*)(paraBuf+num0)));
			
			/* 判断数据属否跨页写 */
			if((currentPage+num0) > nextPage)
			{
				//这里不会产生写溢出问题，假如产生写溢出，则会泽过程2中退出函数
				/* 擦除下一页数据 */
				eraseState = FLASH_ErasePage(nextPage);
				/* 判断擦除操作状态 */
				if(eraseState != FLASH_COMPLETE)
				{
					/* 擦除失败，查看失败原因 */
					__nop();
					return (FALSE);
				}else//这里由于存储参数存储容量只有一页，所以不会到这里执行代码，\
					   但是考虑底层代码的标志性就先写好这个功能。
				{
					/* 擦除成功，设置下一页为当前页 */
					currentPage = nextPage;
					nextPage += ParaPageSpace;
				}
			}
		}
		/* 上锁Flash */
		FLASH_Lock();
	}

	/* 操作成功 */
    return (TRUE);
}

/************************************************************************************************/
/* 函数名	: Read_Para																			*/
/* 输入  	: adder:写地址;data:数据;Len:写数据长度; 											*/
/* 输出		: 读参数状态 																		*/
/* 作用	 	: 读存储器参数																		*/
/* 创建日期 	: 2016/03/06																		*/
/************************************************************************************************/
BOOL Read_Para(uint32_t adder,uint8_t * data,uint32_t Len)
{

	/* 判断写各个参数是否合法 */
	/* 1、判断地址是否在参数容量空间内 */
	if(adder >= Storage.Para.Size)
	{
		/* 写参数地址超出了参数容量空间 */
		return (FALSE);
	}
	/* 2、判断写的数据是否全部都在参数容量空间内 */
	else if((adder+Len) >= Storage.Para.Size)
	{
		/* 写参数数据超出了参数容量空间 */
		return (FALSE);
	}

	
	/* 判断当前的存储器类型 */
	if(Storage.Para.Type == NandFlash)
	{
		/* 拷贝参数到缓存中 */
		memcpy(data,(uint8_t*)(ParaBaseAddre+adder),Len);
	}
	
	/* 操作成功 */
    return (TRUE);
}

/************************************************************************************************/
/* 函数名	: Write_Data																		*/
/* 输入  	: adder:写地址;data:数据;Len:写数据长度; 											*/
/* 输出		: 写固件升级备份数据状态 															*/
/* 作用	 	: 储器固件升级份数据																	*/
/* 创建日期 	: 2016/03/06																		*/
/************************************************************************************************/
BOOL Write_Data(uint32_t adder,uint8_t * data,uint32_t Len)
{
	/* 计数器 */
    uint32_t num0;
	
	/* 判断写各个参数是否合法 */
	/* 1、判断地址是否在参数容量空间内 */
	if(adder >= Storage.Data.Size)
	{
		/* 写参数地址超出了参数容量空间 */
		return (FALSE);
	}
	/* 2、判断写的数据是否全部都在参数容量空间内 */
	else if((adder+Len) >= Storage.Data.Size)
	{
		/* 写参数数据超出了参数容量空间 */
		return (FALSE);
	}
	
	/* 3、存储数据 */
	/* 判断当前的存储器类型 */
	if(Storage.Data.Type == NandFlash)
	{
		/* 存储器为NandFlash */
		/* 每次都需要读取该也的全部数据，注意:这样有可能导致Flash寿命问题! */
		/* 当前页地址 */
		uint32_t currentPage = UpgradeBaseAddre+adder;

		
		/* 解锁Flash */
		FLASH_Unlock();
		/* 清除Flash状态标志 */
		FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

		for (num0 = 0; num0 < Len; num0 += 4)
		{
			/* 存储器为NandFlash */
			/* 向NandFlash中写入数据 */
			FLASH_ProgramWord(currentPage+num0,*((uint32_t*)(data+num0)));
			
			 MAX823_WDI_Signal();//喂狗
			
		}
		
		/* 上锁Flash */
		FLASH_Lock();
	}

	/* 操作成功 */
    return (TRUE);
}

/************************************************************************************************/
/* 函数名	: Erase_Data																		*/
/* 输入  	: 无		 																		*/
/* 输出		: 写固件升级备份数据状态 															*/
/* 作用	 	: 储器固件升级份数据																	*/
/* 创建日期 	: 2016/03/06																		*/
/************************************************************************************************/
void Erase_Data(void)
{
	/* 当前页地址 */
	uint32_t currentPage = UpgradeBaseAddre;
	
	/* 解锁Flash */
	FLASH_Unlock();
	/* 清除Flash状态标志 */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	/* 擦除备份数据区 */
	for (; currentPage < (UpgradeBaseAddre+UpgradeCapacity); currentPage += UpgradePageSpace)
	{
		/* 擦除当前页数据 */
		FLASH_ErasePage(currentPage);
		
		MAX823_WDI_Signal();//喂狗
		
	}

	/* 上锁Flash */
	FLASH_Lock();
}

/************************************************************************************************/
/* 函数名	: Erase_Para																		*/
/* 输入  	: 无 																				*/
/* 输出		: 无 																				*/
/* 作用	 	: 擦除备份参数																		*/
/* 创建日期 	: 2016/03/08																		*/
/************************************************************************************************/
void Erase_Para(void)
{
	/* 当前页地址 */
	uint32_t currentPage = UpgradeBaseAddre;
	
	/* 解锁Flash */
	FLASH_Unlock();
	/* 清除Flash状态标志 */
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	/* 擦除备份数据区 */
	for (; currentPage < (UpgradeBaseAddre+UpgradeCapacity); currentPage += UpgradePageSpace)
	{
		/* 擦除当前页数据 */
		FLASH_ErasePage(currentPage);
		MAX823_WDI_Signal();
		
	}

	/* 上锁Flash */
	FLASH_Lock();
}

#endif

/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
