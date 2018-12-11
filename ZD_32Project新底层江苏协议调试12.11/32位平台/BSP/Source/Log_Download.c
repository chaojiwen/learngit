/*************************************************************************************************/
/*************************************     32位平台测试     ***************************************/
/* 创建日期：2014/12/06																			 */
/*************************************************************************************************/
#include "interface.h"
#include "app.h"

#include <string.h>



#include "Log_Download.h"
#include "Log_GSM.h"

//温度传感器的数据
typedef struct {
	/* 消息体 长度 */
	u8 H_Data :4;
	/* 数据加密方式 */
	u8 L_Data :4;       
}Upgrade_Data_Decryption_Structure;

/************************************* 文件 宏定义 ************************************************/



/************************************* 文件函数 声明 **********************************************/
static void Program_Inf_Tab_set(unsigned int bkp_start, unsigned char* bkp_ptr, unsigned int bkp_len);
static void flash_bytestream_read(unsigned int flash_start, unsigned char* data_ptr, unsigned int data_len);
static void flash_bytestream_write(unsigned int flash_start, unsigned char* data_ptr, unsigned int data_len);
static void Upgrade_Get_Data(void);
static void Upgrade_Decryption_Data(char* data,u16 no);
//static u16 crc_cal_by_byte(char* ptr,u32 len);

/************************************* 文件变量 **************************************************/
///* 程序升级 数据缓存 */
//char IAP_Data_Buffer[IAP_PageSize];
///* 程序升级 数据结构 */
IAP_SR_Structure Mcu_IAP_Flash_Data;

/* 存放 程序升级 的状态 信息缓存 */
unsigned char Program_Upgrade_SR_Inf_Buffer[BLCOK_SYS_PARAM_SPACE];

/* CRC 字节余式表 */
unsigned int crc_ta_8[256]={ 
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/************************************************************************************************/
/**	函 数 名: crc_cal_by_byte																	*/
/**	功能说明: 按字节计算CRC,对指定的数据段 进行数据校验.												*/
/**	形    参：unsigned char* ptr:指向发送缓冲区的首字节\unsigned char len:要发送的总字节数			*/
/**	返 回 值: unsigned int																		*/
/**	注释	   : 适用于程序空间较大且CRC计算速度要求较高的计算机或微控制系统,算法概要说明：计算本字节后的CRC.	*/
/**			 码等于上一字节余式CRC码的低8位左移8位后，再加上上一字节CRC 右移8 位（也既取高8 位）和本字节	*/
/**			 之和后所求得的CRC码.																	*/
/************************************************************************************************/
static u16 crc_cal_by_byte(char* ptr,u32 len)
{
    u16 crc = 0;

    while(len-- != 0)
    {
        unsigned int high = (unsigned int)(crc/256); //取CRC高8位
        crc <<= 8;
        crc ^= crc_ta_8[high^*ptr];
        ptr++;
    }

    return crc;
}

/************************************************************************************************/
/**	函 数 名: Remote_Upgrade																		*/
/**	功能说明: 远程升级																			*/
/**	形    参：无																					*/
/**	返 回 值: 程序下载 进度																		*/
/************************************************************************************************/
BOOL Remote_Upgrade(void)
{
	u16 updated_state;
	u32 num;
	IAP_Data_Inf_Structure* data_inf;
	

//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		/* 阶段1:获取 更新程序 信息（数据块 总数） */
//		printf("\r阶段1:获取更新程序信息.(数据块 总数)\r\n");
//	}
	
	
	
	/* 开始请求数据包. */
	Upgrade_Get_Data();
	/* 判断 数据 获取 情况 */
	if(Mcu_IAP_Flash_Data.RX_COM_State == FALSE)
	{
//		if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//		{
//			printf("\r阶段1:获取更新程序信息失败.\r\n");	
//		}
		return FALSE;
	}	
	
	/* 从数据块中 解析 更新程序 信息(现在只有：1、数据块总数) 并保存 */
	/* 将消息指针，指向数据缓存 */
	data_inf = ((IAP_Data_Inf_Structure*)Mcu_IAP_Flash_Data.Data_Buffer);
	/* 获取 信息 */
	Mcu_IAP_Flash_Data.Data_Block_Total = data_inf->Block_Total;
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		/* 解析数据成功，并打印 相关 信息 */
//		printf("\r阶段1:数据成功. 更新程序的数据块总数:%d\r\n",Mcu_IAP_Flash_Data.Data_Block_Total);	
//	}
	
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		printf("\r阶段2:删除备份区域数据.\r\n");	
//	}
	/* Flash 解锁 */
	FLASH_Unlock();
	/* 阶段2:擦除 下载程序区域 数据 *///擦除整片FLASH 时间太长了，导致看门狗生效
	for(num = 0; num < BLOCK_CODESTORE_SPACE; num += FLASH_PAGE_SIZE)
	{
		FLASH_ErasePage(BLOCK_CODESTORE_START+num);	
	}
	/* Flash 上锁 */		
	FLASH_Lock();
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		printf("\r阶段2:备份区域数据删除完成.\r\n");	
//	}
	/* 下载最新程序前清除Flash中的程序更新标志 */
	updated_state = 0;
	Program_Inf_Tab_set(ENV_APP_UPDATED_START, (u8*)&updated_state ,ENV_APP_UPDATED_SIZE);
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		printf("\r阶段2:删除程序更新标志.\r\n");
//		
//		printf("\r阶段3:获取和固化更新程序数据.\r\n");		
//	}
	/* 阶段3:获取和固化 更新程序 数据（写入FLASH） */
	for((Mcu_IAP_Flash_Data.Data_Block_No = 1);
		(Mcu_IAP_Flash_Data.Data_Block_No < (Mcu_IAP_Flash_Data.Data_Block_Total+1));
		(Mcu_IAP_Flash_Data.Data_Block_No++))
	
	{	
		/* 开始获取 更新程序 数据包 */
		/* 开始请求数据包. */
		Upgrade_Get_Data();
		/* 判断 数据 获取 情况 */
		if(Mcu_IAP_Flash_Data.RX_COM_State == TRUE)
		{
//			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//			{
//				/* 成功接收 数据包 */
//				printf("\r阶段3: 更新程序数据接收成功，数据包:%d.\r\n",Mcu_IAP_Flash_Data.Data_Block_No);	
//				/* 数据获取 成功 */
//				/* 对数据块 进行 校验 获得校验结果 */
//				printf("\r阶段3: 数据包进行校验.\r\n");
//			}
			/* 将1K数据进行校验. */
			Mcu_IAP_Flash_Data.Data_Block_Check = crc_cal_by_byte((Mcu_IAP_Flash_Data.Data_Buffer+IAP_Page_Inf_Size),
																  (data_inf->Block_Len));
			/* 判断 校验的数据 与 服务器提供的数据 是否相同. */
			if(Mcu_IAP_Flash_Data.Data_Block_Check == (data_inf->Block_Check))
			{
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/* 数据校验正常 */
//					printf("\r阶段3: 数据包校验正常.\r\n");
//				}
				/* 对数据块的数据是进行 解密 */
				Upgrade_Decryption_Data((Mcu_IAP_Flash_Data.Data_Buffer+IAP_Page_Inf_Size),data_inf->Block_Len);	
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					printf("\r阶段3: 数据包解码完成.\r\n");
//					/* 固化程序 */
//					printf("\r阶段3: 开始固化数据包代码.\r\n");
//				}
				
				/* 给调度器上锁 */
				OSSchedLock();
				/* Flash 解锁 */
				FLASH_Unlock();
				//这里数据写入 成功/失败 都没有判断。。。可以吗？？？
				for(num = 0; num < data_inf->Block_Len; num += 4)
				{
					FLASH_ProgramWord((Mcu_IAP_Flash_Data.Write_Pointer+num),
									 (*(int*)(Mcu_IAP_Flash_Data.Data_Buffer+IAP_Page_Inf_Size+num)));
				}
				/* Flash 上锁 */		
				FLASH_Lock();
				/* 给调度器解锁 */ 
				OSSchedUnlock();
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/* 数据固化 完成 */
//					printf("\r阶段3: 固化数据包代码完成.\r\n");
//				}
				/* 设置 下一次的 写地址指针 */
				Mcu_IAP_Flash_Data.Write_Pointer += IAP_Page_Data_Size;
			}else
			{
//				if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//				{
//					/*  校验错误，重新获取数据 */
//					printf("\r阶段3: 数据块进行校验错误.\r\n");
//				}
				/* 重新获取 当前 数据块 */
				Mcu_IAP_Flash_Data.Data_Block_No--;
			}
		}else
		{
//			if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//			{
//				printf("\r阶段3:获取更新程序数据失败，重新获取当前数据块.\r\n");				
//			}
		
			/* 重新获取 当前 数据块 */
			Mcu_IAP_Flash_Data.Data_Block_No--;
		}		
	}
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		/* 阶段3: 所有数据包下载、固化完成. */
//		printf("\r阶段3: 所有数据包下载、固化完成.\r\n");		
//	}

	/* 阶段3:下载最新程序前清除Flash中的程序更新标志 */ 
	updated_state = 1;
	Program_Inf_Tab_set(ENV_APP_UPDATED_START, (u8*)&updated_state, ENV_APP_UPDATED_SIZE);
////	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
////	{
////		printf("\r阶段3:设置程序更新标志.\r\n");
////		
////		printf("\r阶段4:重新启动仪器.\r\n");		
////	}

	/* 阶段4:重新启动仪器 */
//	mcu_reset();
	__set_FAULTMASK(1);//关闭总中断
    NVIC_SystemReset();//请求单片机重启
	
	
	
	
	/* 程序 下载 成功. */
	return TRUE;
}

/************************************************************************************************/
/**	函 数 名: Upgrade_Decryption_Data															*/
/**	功能说明: 对 获取 的数据块 进行 解密															*/
/**	形    参：data 数据缓存指针，no解密数据数量														*/
/**	返 回 值: 数据获取：TURE成功；FALSE失败														*/
/************************************************************************************************/
static void Upgrade_Decryption_Data(char* data,u16 no)
{
	u16 num;
	Upgrade_Data_Decryption_Structure* data_buffer;
	
	for(num = 0; num < no; num++)
	{
		/* 获取 加密数据 */
		data_buffer = ((Upgrade_Data_Decryption_Structure*)(data + num));
		/* 解密数据 */
		*(data+num) = ((data_buffer->H_Data<<4)+ data_buffer->L_Data);
	}
}

/************************************************************************************************/
/**	函 数 名: Upgrade_Get_Data																	*/
/**	功能说明: 获取 GPRS 升级程序的数据																*/
/**	形    参：无																					*/
/**	返 回 值: 数据获取：TURE成功；FALSE失败														*/
/************************************************************************************************/
static void Upgrade_Get_Data(void)
{
	INT8U err;
	
	/* 开始获取 更新程序 数据包 */
//	printf("\r请求数据包:%d.\r\n",Mcu_IAP_Flash_Data.Data_Block_No);
//	/* 发送 更新程序 事件标志 */	
//	OSFlagPost(					//向标志发信号
//	Str_GPRS_Upg_Get_Data_Flag, //发送标志的指针
//	(OS_FLAGS)0XFF, 			//选择要发送的信号 给第1位发信号 0010  同样把2强制转化为OS_FLAGS型的数据
//								//因为信号为OS_FLAGS型的
//	OS_FLAG_SET,				//信号有效的选项 信号置1  OS_FLAG_SET为置1  OS_FLAG_CLR为置0
//	&err);						//标志信号
	/* GSM 信号强度 */
////	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,Download_IAP_Data_EVENT);//zwc修改
	GSM_Send_Q((OS_EVENT*)GSM_EVENT_Str_Q,IAP_EVENT);//zwc修改
	
	
	
	
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		/* 等待数据 */
//		printf("\r等待数据包:%d.\r\n",Mcu_IAP_Flash_Data.Data_Block_No);		
//	}

	OSFlagPend(								//请求标志信号
	Str_GPRS_Upg_RXC_Data_Flag,				//请求标志信号指针
	(OS_FLAGS)0XFF,							//过滤器 请求第0和第1位信号 0011 这里是把数据3强制转化为OS_FLAGS类型的数据，
											//因为过滤器和标志信号中的信号都是OS_FLAGS类型的数据
											//OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,   
											//信号全是1表示信号有效 参数OS_FLAG_CONSUME表示当
											//任务等待的事件发生后，清除相应的事件标志位
	OS_FLAG_WAIT_SET_ALL+OS_FLAG_CONSUME,	//信号全是1表示信号有效  没有加参数OS_FLAG_CONSUME，所以不会清除标志位
	0,										//等待时限，0表示无限等待
	&err									//错误信息
	);
//	if(Instru_Fix_Inf_State_Tab.Inf_State.Instru_Menu.Inf.MENU_Com1_Debug_EN)
//	{
//		printf("\r收到数据包:%d.\r\n",Mcu_IAP_Flash_Data.Data_Block_No);		
//	}
}

/************************************************************************************************/
/**	函 数 名: Program_Inf_Tab_set																*/
/**	功能说明: 设置指定的整数型备份参数。															*/
/**	形    参：bkp_start、val																		*/
/**	返 回 值: 无																					*/
/************************************************************************************************/
static void Program_Inf_Tab_set(unsigned int bkp_start, unsigned char* bkp_ptr, unsigned int bkp_len)
{
	/* 计数器 */
	
	/* 清除 数据缓存 */
	memset(Program_Upgrade_SR_Inf_Buffer,0X00,BLCOK_SYS_PARAM_SPACE);

	/* 将Flash中的全部系统变量一次性读出到RAM */ 
	flash_bytestream_read(BLCOK_SYS_PARAM_START, Program_Upgrade_SR_Inf_Buffer, BLCOK_SYS_PARAM_SPACE);
	
	/* 在对应的 信息位 写入数据 */
	/* 写程序 更新 标志 */
	memcpy((u8*)(Program_Upgrade_SR_Inf_Buffer+bkp_start),bkp_ptr,bkp_len);
	
	/* 给调度器上锁 */   
	OSSchedLock();
	/* Flash 解锁 */
	FLASH_Unlock();
	/* 擦除当前页(系统变量存储在Flash中一个完整页里，从页的起始地址开始存储) */ 
	FLASH_ErasePage(BLCOK_SYS_PARAM_START);

	/* 将RAM中更新的数据整体写回Flash */ 
	flash_bytestream_write(BLCOK_SYS_PARAM_START, Program_Upgrade_SR_Inf_Buffer, BLCOK_SYS_PARAM_SPACE);	
	/* Flash 上锁 */		
	FLASH_Lock();
	/* 给调度器解锁 */ 
	OSSchedUnlock();
}

/************************************************************************************************/
/**	函 数 名: flash_bytestream_read																*/
/**	功能说明: 从指定Flash起始地址处读取指定长度的字节流数据到指定的缓存中。							*/
/**	形    参：flash_start data_ptr data_len：待读出字节流的长度									*/
/**	返 回 值: 无																					*/
/************************************************************************************************/		
static void flash_bytestream_read(unsigned int flash_start, unsigned char* data_ptr, unsigned int data_len)
{	
	int				i;
	unsigned int 	data_len2 = 0;
	unsigned short	halfword = 0x0000;
	
	// 检查待读出的字节流长度是否为奇数
	data_len2 = data_len&1?data_len-1:data_len;

	// 读取最大偶数个字节
	for(i = 0; i < data_len2; i += 2)
	{
		halfword = *(unsigned short*)(flash_start+i);

		// 对读出的双字节数据做字节倒序处理
		*data_ptr++ = halfword&0xFF;
		*data_ptr++ = (halfword>>8)&0xFF;
	}

	// 如果待读出的字节流长度为奇数，则读取最后一个字节
	if(data_len2 < data_len)
	{	
		halfword = *(unsigned short*)(flash_start+i);
		
		*data_ptr  = halfword&0xFF;
	}
}

/************************************************************************************************/
/**	函 数 名: flash_bytestream_write																*/
/**	功能说明: 将指定长度的字节流数据写入到指定的Flash起始地址处										*/
/**	形    参：flash_start data_ptr data_len：待读出字节流的长度									*/
/**	返 回 值: 无																					*/
/************************************************************************************************/	
static void flash_bytestream_write(unsigned int flash_start, unsigned char* data_ptr, unsigned int data_len)
{
	int 			i;
	unsigned int	data_len2 = 0;	

	// 检查记录的长度是否为奇数
	data_len2 = data_len&1?(data_len-1):data_len;

	// 写入最大偶数个字节
	for(i = 0; i < data_len2; i += 2)
	{
		// 字节流数据类似大端存储格式，但是ARM内核采用小端格式读写数据，因此写入字节流数据时可采用双字节写入，
		// 但是读出字节流时除了要以双字节读出，还要对双字节数据做字节倒序处理。
		FLASH_ProgramHalfWord(flash_start+i, *((unsigned short*)data_ptr));	

		data_ptr += 2;
	}

	// 若记录长度为奇数，末尾字节补0x00(读出时舍去)后写入
	if(data_len2 < data_len)
	{
		FLASH_ProgramHalfWord(flash_start+i, (*(unsigned char*)data_ptr<<8) | 0x00);
	}
}





/************************************************************************************************/
/**	函 数 名: Flash_Write_SN_NO																	*/
/**	功能说明: 设置SN号																			*/
/**	形    参：待写的sn号指针																		*/
/**	返 回 值: 无																					*/
/************************************************************************************************/	
void Flash_Write_SN_NO(unsigned char* sn_ptr)
{
	/* 设置 SN 号 */ 
	Program_Inf_Tab_set(ENV_SN_NO_START,sn_ptr,ENV_SN_NO_SIZE);
}

/************************************************************************************************/
/**	函 数 名: Flash_Read_SN_NO																	*/
/**	功能说明: 读SN号																				*/
/**	形    参：待读的sn号指针																		*/
/**	返 回 值: 无																					*/
/************************************************************************************************/	
void Flash_Read_SN_NO(unsigned char* sn_ptr)
{
	/* 读 SN 号 */
	flash_bytestream_read(BLCOK_SYS_PARAM_START+ENV_SN_NO_START,sn_ptr,ENV_SN_NO_SIZE);
}

/************************************************************************************************/
/**	函 数 名: Flash_Write_Pgm_Ude_State															*/
/**	功能说明: 设置程序更新状态																	*/
/**	形    参：待写的sn号指针																		*/
/**	返 回 值: 无																					*/
/************************************************************************************************/	
void Flash_Write_Pgm_Ude_State(unsigned char* state_ptr)
{
	/* 设置 SN 号 */ 
	Program_Inf_Tab_set(ENV_APP_UPDATED_START,state_ptr,ENV_APP_UPDATED_SIZE);
}

/************************************************************************************************/
/**	函 数 名: Flash_Read_Pgm_Ude_State															*/
/**	功能说明: 读程序更新状态																		*/
/**	形    参：待读的sn号指针																		*/
/**	返 回 值: 无																					*/
/************************************************************************************************/	
void Flash_Read_Pgm_Ude_State(unsigned char* sn_ptr)
{
	/* 读 SN 号 */
	flash_bytestream_read(BLCOK_SYS_PARAM_START+ENV_APP_UPDATED_START,sn_ptr,ENV_APP_UPDATED_SIZE);
}




/******************* (C) ZHOU *****END OF FILE****/


