/************************************************************************************************/
/** @file              : Mcu_GPS.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年7月30日															*/
/** @brief             : c file																	*/
/** @description       : 32为平台应用文件														*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/ 
#include "System_Config.h"


#if (GPS_Config_Enable == 0x01)

#include "app.h"

#include "Mcu_GPS.h"
#include "Mcu_Usart.h"

#include "string.h"
//#include "stdlib.h"




/************************************************************************************************/
#define GPS_COM_Preemption_Priority		(2u)		/* 抢占优先级 2 */
#define GPS_COM_Sub_Priority			(1u)		/* 子优先级 1 */

/************************************************************************************************/

/* GPS 接收数据缓存大小 */
#define GPS_Buffer_Size   			(256u)
/* GPS 接收数据缓存 */
static unsigned char 				GPS_Buffer[GPS_Buffer_Size];

/* 读 GPS 接收数据缓存 */
#define GPS_RX_RD(n)				(GPS_Buffer[(n)&(GPS_Buffer_Size-1)])
/* 写 GPS 接收数据缓存 */
#define GPS_RX_WR(n, v)				(GPS_Buffer[(n)&(GPS_Buffer_Size-1)] = v)

/* 读 GPS 接收数据缓存指针 */
static volatile unsigned int 		GPS_Rx_Rd_p = 0;
/* 写 GPS 接收数据缓存指针 */
static volatile unsigned int 		GPS_Rx_Wr_p = 0; 


/************************************************************************************************/
/* 特征字符串 */
/* 特征字符串 "$GPRMC" 编号 */
#define GPS_Spe_Str_GPRMC_NO		(8u)
/* 特征字符串 "$GPGSA" 编号 */
#define GPS_Spe_Str_GPGSA_NO		(9u)


/************************************************************************************************/
/* 最大特征字索引号 */
#define MAX_Spe_Char_Index			(8u)
/* 最大特征字符串编号 */
#define MAX_Spe_Str_NO				(8u)

/* 特征字索引号 */
static unsigned char 	Spe_Char_Index = 0u;
/* 特征字符串编号 */
static unsigned char 	Spe_Str_NO = 0u;

/* GPS串口特征字符串编码表 */
static const unsigned char 	GPS_Spe_Str_Tab[MAX_Spe_Str_NO][MAX_Spe_Char_Index]=
{
//   $ A C G M P R S
	{1,0,0,0,0,0,0,0},	// state 0
	{0,0,0,2,0,0,0,0},	// state 1
	{0,0,0,0,0,3,0,0},	// state 2
	{0,0,0,6,0,0,4,0},	// state 3
	{0,0,0,0,5,0,0,0},	// state 4
	{0,0,8,0,0,0,0,0},	// state 5
	{0,0,0,0,0,0,0,7},	// state 6
	{0,9,0,0,0,0,0,0},	// state 7
};

/* 建立'A' - 'S'之间字符在 特征字符串编码表 的索引号;			*/
/* (非特征字符对应0，但不意味其在gps_ptn_map表中的索引号为0). 	*/
/* 从而便于输入当前带检测字符快速得到其在GPS_Spe_Str_Tab表中的索引号。 */
static const unsigned char GPS_Spe_Char_Index_Tab[19] = {1,0,2,0,0,0,3,0,0,0,0,0,4,0,0,5,0,6,7};


/************************************************************************************************/
/* GPRMC特征字接收状态 */
static BOOL	GPSRMC_Spe_Str_Rec_State 	= FALSE;
/* GPGSA特征字接收状态 */
static BOOL	GPSGSA_Spe_Str_Rec_State 	= FALSE;
/* GPRMC 消息 接收状态 */
BOOL		GPSRMC_Data_Rec_State 		= FALSE;
/* GPGSA 消息 接收状态 */
BOOL		GPSGSA_Data_Rec_State 		= FALSE;
/* GPS 定位状态 */
static BOOL GPS_Pos_State 				= FALSE;


/************************************************************************************************/
/* 当前接收GPRMC消息数据数量 */
static int 	GPRMC_Rec_Data_Ctr		= 0u;
/* 当前接收GPGSA消息数据数量 */
static int 	GPGSA_Rec_Data_Ctr 		= 0u;
/* GPRMC消息"定位状态"字段信息计数器 */
static unsigned char GPRMC_Pos_State_Inf_Ctr = 0u;


/* GPRMC消息，包含经度、纬度、速度、海拔等，实际最大长度约为71字节 */
char		GPRMC_MSG_Buf[GPRMC_MSG_Buf_Size+1];
/* GPGSA消息，包含三种DOP信息、定位模式、用于定位的卫星编号(最多12颗)等，实际最大长度为64字节 */
char		GPGSA_MSG_Buf[GPGSA_MSG_Buf_Size+1];


/************************************************************************************************/



/************************************************************************************************/
/* 函数名	: GPS_NVIC_Configure																*/
/* 输入		: 无 																				*/
/* 输出		: 无 																				*/
/* 作用		: 设置GPS通信(Usart2)全局中断优先级													*/
/* 创建日期	: 2015/7/30																			*/
/************************************************************************************************/
static void GPS_NVIC_Configure(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;


	/* 使能USART2中断 */
	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;					//配置外部中断源（USART2中断）
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= GPS_COM_Preemption_Priority;	//设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= GPS_COM_Sub_Priority;			//设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;					  	//使能通道
	NVIC_Init(&NVIC_InitStructure);  								  						//向寄存器中写如数据
}


/************************************************************************************************/
/* 函数名	: GPS_Mcu_Para_Init																	*/
/* 输入		: 无 																				*/
/* 输出		: 无 																				*/
/* 作用		: 配置GPS通信端口（Usart2）															*/
/* 创建日期	: 2015/7/30																			*/
/************************************************************************************************/
static void GPS_Mcu_Para_Init(void)
{
	/* 当前接收GPRMC消息数据数量 */
	GPRMC_Rec_Data_Ctr		= 0u;
	/* 当前接收GPGSA消息数据数量 */
	GPGSA_Rec_Data_Ctr		= 0u;
	/* GPRMC消息"定位状态"字段信息计数器 */
	GPRMC_Pos_State_Inf_Ctr = 0u;

	/* 初始化GPRMC消息和GPGSA消息接收相关变量 */
	strcpy(GPRMC_MSG_Buf, "");
	strcpy(GPGSA_MSG_Buf, "");

	/* GPRMC特征字接收状态 */
	GPSRMC_Spe_Str_Rec_State 	= FALSE;
	/* GPGSA特征字接收状态 */
	GPSGSA_Spe_Str_Rec_State 	= FALSE;
	/* GPRMC 消息 接收状态 */
	GPSRMC_Data_Rec_State 		= FALSE;
	/* GPGSA 消息 接收状态 */
	GPSGSA_Data_Rec_State 		= FALSE;
	/* GPS 定位状态 */
	GPS_Pos_State 				= FALSE;

}


/************************************************************************************************/
/* 函数名	: GPS_Com_Configure																	*/
/* 输入		: 无 																				*/
/* 输出		: 无 																				*/
/* 作用		: 配置GPS通信端口（Usart2）															*/
/* 创建日期	: 2015/7/30																			*/
/************************************************************************************************/
static void GPS_Com_Configure(void)
{
/* 模块通信的波特率:9600(默认值) */
#define	GPS_Define_BaudRate		(9600u)



#if (Usart_2_Function_Config_Enable == 0x01)		
	/* 配置模块的通信端口 */
	USART2_Config(GPS_Define_BaudRate);
	/* 配置串口2接收中断 */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	/* 使能串口2功能 */
	USART_Cmd(USART2 , ENABLE );
#endif



#undef	GPS_Define_BaudRate	

}

/************************************************************************************************/
/* 函数名	: GPS_Rec_Data_Inf_Flow																*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: GPS接收信息流数据																	*/
/* 创建日期	: 2015/7/30																			*/
/************************************************************************************************/
void GPS_Rec_Data_Inf_Flow(char data)
{


	/* 将当前接收到的数据写入GPS接收缓冲 */
	GPS_RX_WR(GPS_Rx_Wr_p++, data);

	/************************ 检测GPS消息头 ********************************/		
	/* 取得当前输入字符在gps_ptn_map表中的索引号(若为非特征字符，则无索引号) */
	if(data == '$')
	{
		/* 清0 特征字索引号 */
		Spe_Char_Index = 0;
		/* 清0 特征字符串编号 */
		Spe_Str_NO = 0;
		/* 根据特征字符串编码表 "" 确定特征字符串编码. */
		Spe_Str_NO = GPS_Spe_Str_Tab[Spe_Str_NO][Spe_Char_Index];
	}
	else if(data >= 'A' && data <= 'S')/* 仅检测'A' - 'S'之间的字符 */
	{
		/* 检查获得的索引号是否有意义(索引号为0的则无意义，即非特征字符) */
		Spe_Char_Index = GPS_Spe_Char_Index_Tab[data - 'A'];

		/* 判断特征字符索引号是否我特征字索引 */
		if(Spe_Char_Index > 0)
		{	
			/* 单前数据为特征字索引 */
			/* 从特征字编码表中查询对应的特征字符串编号 */
			Spe_Str_NO = GPS_Spe_Str_Tab[Spe_Str_NO][Spe_Char_Index];
		}
		else
		{
			/* 清0 特征字编码表中查询对应的特征字符串编号. */ 
			Spe_Str_NO = 0;								
		}
	}
	else
	{
		/* 清0 特征字编码表中查询对应的特征字符串编号. */ 
		Spe_Str_NO = 0; 	
	}

	/* 检查特征字符串编号 */
	if(Spe_Str_NO > (MAX_Spe_Str_NO - 1))
	{
		/* 判断当前的GPS的命令类型 */
		switch(Spe_Str_NO)
		{
			case GPS_Spe_Str_GPRMC_NO:
			{
				/* $GPRMC检测到(每组NMEA消息中都会有一条RMC消息、RMC消息头被检测到的频率是1Hz) */
				/* 仅在上一条接收到的GPRMC消息被处理完后才接收新的GPRMC消息,以实现接收和处理的同步 */
				if( GPSRMC_Spe_Str_Rec_State == FALSE)
				{
					/* RMC命令接收完成 */
					GPSRMC_Spe_Str_Rec_State = TRUE;
					/* 清0 RMC数据接收数量 */
					GPRMC_Rec_Data_Ctr 	= 0;
					/* 清0 RMC ','符号接收数量 */
					GPRMC_Pos_State_Inf_Ctr	= 0;
				}
				break;
			}

			case GPS_Spe_Str_GPGSA_NO:
			{
				/* $GPGSA检测到(每组NMEA消息中都会有一条GSA消息、GSA消息头被检测到的频率是1Hz) */
				/* 仅在上一条接收到的gsa消息被处理完后才接收新的gsa消息，以实现接收和处理的同步 */
				if(GPSGSA_Data_Rec_State == FALSE)
				{
					/* GSA命令接收完成 */
					GPSGSA_Spe_Str_Rec_State = TRUE; 	
					/* 清0 GSA数据接收数量 */
					GPGSA_Rec_Data_Ctr 	= 0;
				}
				break;				
			}	

			
			default:
			{
				break;				
			}
		}
		/* 清0 特征字编码表中查询对应的特征字符串编号. */ 
		Spe_Str_NO = 0; 							// 状态机进入终态的话即刻复位
	}

	/************************ 提取并部分解析RMC/GSA消息 ********************************/
	
	/* 检测到GPRMC特征字符串消息头后准备接收rmc消息(从$GPRMC的C字符开始接收、直至换行符号) */
	if(GPSRMC_Spe_Str_Rec_State == TRUE)
	{	
		/* 仅在上一条rmc消息被处理后才开始接收新的rmc消息 */
		/* GPRMC消息是否已经被接收处理 */
		if(GPSRMC_Data_Rec_State == FALSE)
		{
			/* RMC消息已经被接收处理 */
			/* 保存新的数据 */
			GPRMC_MSG_Buf[GPRMC_Rec_Data_Ctr++] = data;
			/* 检测到换行符或提取的字符数超过gps_msg_rmc数组的最大长度，则停止提取 */
			if(((GPS_RX_RD(GPS_Rx_Wr_p-2) == '\r') && (GPS_RX_RD(GPS_Rx_Wr_p-1) == '\n')) || (GPRMC_Rec_Data_Ctr >= GPRMC_MSG_Buf_Size))
			{
				/* 添加字符串结尾符号(在换行符之后) */
				GPRMC_MSG_Buf[GPRMC_Rec_Data_Ctr++] = '\0';

				/* 提取完一条RMC消息后，将RMC检测标志复位，以便重新提取下一条RMC消息 */
				GPSRMC_Spe_Str_Rec_State = FALSE;
				/* 清除 GPRMC消息数据数量 */
				GPRMC_Rec_Data_Ctr 	= 0;

				/* 如果当前GPS已经定位，则设置GPRMC消息提取完成标志(之前提取的GPRMC消息由于是在GPS未定位时提取的，因此统统忽略) */
				if(GPS_Pos_State == TRUE)
				{
					/* 接收完一条GPRMC消息后，解析此消息需要时间，因此可能造成上次接收完但仍在处理的GPRMC消息被新的接收过程覆盖，这里需要同步 */
					GPSRMC_Data_Rec_State = TRUE;
				}
				/* 复位GPRMC定位标志 */
				GPS_Pos_State = FALSE;
				/* 复位GPRMC消息"定位状态"字段信息计数器。 */
				GPRMC_Pos_State_Inf_Ctr = 0;
			}
		}	

		/* 检测到GPRMC消息头后，紧接着检测定位状态 */
		if(data == ',')
		{
			GPRMC_Pos_State_Inf_Ctr++;

			/* 检测到GPRMC消息头后的第三个逗号。 */
			if(GPRMC_Pos_State_Inf_Ctr == 3)
			{	
				/* 提取此逗号之前的一个字符(即定位状态标志: A - 已定位，V - 未定位) */
				if(GPS_RX_RD(GPS_Rx_Wr_p-2) == 'A')
				{			
					/* 设置GPRMC定位标志 */
					GPS_Pos_State = TRUE;
				}
				else
				{
					/* 未设置GPRMC定位标志 */
					GPS_Pos_State = FALSE;
				}
				// else: exception...
			}
		}	
	}

	/* 不管当前是否定位， 在检测到GPGSA消息头后都逐个提取此消息(从GPGSA的末尾字符A开始提取) */
	if(GPSGSA_Spe_Str_Rec_State == TRUE)
	{			
		/* 从GPGSA消息头之后的第一个逗号(包含逗号本身)开始提取GPGSA消息 */
		if(GPSGSA_Data_Rec_State == FALSE)
		{
			/* GPGSA消息已经被接收处理 */
			/* 保存新的GPGSA消息数据 */
			GPGSA_MSG_Buf[GPGSA_Rec_Data_Ctr++] = data;
			/* 检测到换行符或提取的字符数超过gps_msg_gsa数组的最大长度，则停止提取 */
			if((GPS_RX_RD(GPS_Rx_Wr_p-2) == '\r' && GPS_RX_RD(GPS_Rx_Wr_p-1) == '\n') || (GPGSA_Rec_Data_Ctr >= GPGSA_MSG_Buf_Size))
			{
				/* 添加字符串结尾符号(在换行符之后) */
				GPGSA_MSG_Buf[GPGSA_Rec_Data_Ctr++] = '\0';	

				/* 提取完一条GPGSA消息后，将GPGSA检测标志复位，以便重新提取下一条GPGSA消息 */
				GPSGSA_Spe_Str_Rec_State = FALSE;
				GPGSA_Rec_Data_Ctr = 0;
				
				/* 如果当前GPRMC消息已经被提取完，则设置GPGSA消息提取完成标志(在一组NMEA消息中GPRMC消息最靠前) */
				if(GPSRMC_Data_Rec_State == TRUE)
				{
					/* 设置GPGSA消息接收完成标志 */
					GPSGSA_Data_Rec_State = TRUE;
					/* 发送GPS消息邮箱 */
					OSMboxPost(GPS_Box,(char*)GPGSA_MSG_Buf);
				}
			}	
		}
	}


}


/************************************************************************************************/
/* 函数名	: GPS_Mcu_Init																		*/
/* 输入		: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: GPS初始化MCU																		*/
/* 创建日期	: 2015/7/30																			*/
/************************************************************************************************/
void GPS_Mcu_Init(void)
{

	/* 初始化配置GPS Mcu参数 */
	GPS_Mcu_Para_Init();
	
	/* 设置GPS通信(Usart2)全局中断优先级 */
	GPS_NVIC_Configure();

	/* 配置GPS通信端口（Usart2） */
	GPS_Com_Configure();
	
	

}


#endif


/******************* (C) ZHOU *****END OF FILE****/


