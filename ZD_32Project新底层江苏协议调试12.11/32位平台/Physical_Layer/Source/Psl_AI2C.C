/************************************************************************************************/
/** @file              : Psl_AI2C.c																*/
/** @author            : HZ Zeda Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2015年11月06日															*/
/** @date changed      : 2015年11月06日															*/
/** @brief             : c file																	*/
/** @description       : 配置表(物理层)															*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**																								*/
/************************************************************************************************/
/************************************************************************************************/

#include "Psl_AI2C.h"
#include "Dev_AI2C.h"
/* 定义模拟I2C总线引脚 */
#define Psl_I2C_SDA_PORT         (GPIOB)
#define Psl_I2C_SDA_CLK          (RCC_APB2Periph_GPIOB)
#define Psl_I2C_SDA_PIN          (GPIO_Pin_9)
#define Psl_I2C_SCL_PORT         (GPIOB)
#define Psl_I2C_SCL_CLK          (RCC_APB2Periph_GPIOB)
#define Psl_I2C_SCL_PIN          (GPIO_Pin_8)
/************************************************************************************************/
/* 函数名	: Psl_AI2C_Bus_Delay										 	 					*/
/* 输入  	: 延时数量（注意这个数量是在主时钟为72M的时钟下面延时）									*/
/* 输出		: 无 																				*/
/* 作用	 	: 文件延时函数										 								*/
/* 创建日期	: 2015/04/16																		*/
/************************************************************************************************/
static void Psl_AI2C_Bus_Delay(u32 cnt)
{
	while(cnt)
	{
		cnt--;
	}
}
/************************************************************************************************/
/* 函数名	: Psl_AI2C_Bus_Init														 	 		*/
/* 输入  	: 无			 																	*/
/* 输出		: 无 																				*/
/* 作用		: I2C总线初始化											 							*/
/* 创建日期 	: 2015/10/09																		*/
/* 修改日期 	: 2015/11/11																		*/
/************************************************************************************************/
static void Psl_AI2C_Bus_Init(void)
{
	u8 num;
	/* 循环检测I2C总线的SDA状态:(SDA=0:总线冲突;SDA=1:总线空闲正常) */
	while(GPIO_ReadInputDataBit(Psl_I2C_SDA_PORT,Psl_I2C_SDA_PIN)==Bit_RESET)
	{
		/* 检测到总线冲突,这时需要模拟9个SCL时钟脉冲，来解除锁死I2C总线状态. */
		for(num = 0; num < 9 ;num++)
		{
			GPIO_ResetBits(Psl_I2C_SCL_PORT,Psl_I2C_SCL_PIN);
			Psl_AI2C_Bus_Delay(0xFF);
			GPIO_SetBits(Psl_I2C_SCL_PORT,Psl_I2C_SCL_PIN);
			Psl_AI2C_Bus_Delay(0xFF);
		}
		Psl_AI2C_Bus_Delay(0xFFFF);
	}

}

/************************************************************************************************/
/* 函数名	: Psl_AI2C_Init																		*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 配置模拟I2C																		*/
/* 创建日期	: 2015/11/06																		*/
/************************************************************************************************/
void Psl_AI2C_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/* 初始化端口 */
	RCC_APB2PeriphClockCmd(Psl_I2C_SDA_CLK|Psl_I2C_SCL_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin		= Psl_I2C_SCL_PIN;
	GPIO_Init(Psl_I2C_SCL_PORT, &GPIO_InitStructure);
	GPIO_SetBits(Psl_I2C_SCL_PORT,Psl_I2C_SCL_PIN);
	GPIO_InitStructure.GPIO_Pin		= Psl_I2C_SDA_PIN;
	GPIO_Init(Psl_I2C_SDA_PORT, &GPIO_InitStructure);
	GPIO_SetBits(Psl_I2C_SDA_PORT,Psl_I2C_SDA_PIN);
	/* 初始化 I2C总线 */
	Psl_AI2C_Bus_Init();
	/* 创建 模拟I2C 信号量 */
	Dev_AI2C_Sem = OSSemCreate(1);
}

/************************************************************************************************/
/* 函数名	: Psl_AI2C_Bus_Unlock																*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 配置模拟I2C_锁I2C总线																*/
/* 创建日期	: 2016/04/26																		*/
/************************************************************************************************/
void Psl_AI2C_Bus_Unlock(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/* 初始化端口 */
	RCC_APB2PeriphClockCmd(Psl_I2C_SDA_CLK|Psl_I2C_SCL_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin		= Psl_I2C_SCL_PIN;
	GPIO_Init(Psl_I2C_SCL_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(Psl_I2C_SCL_PORT,Psl_I2C_SCL_PIN);
	GPIO_InitStructure.GPIO_Pin		= Psl_I2C_SDA_PIN;
	GPIO_Init(Psl_I2C_SDA_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(Psl_I2C_SDA_PORT,Psl_I2C_SDA_PIN);
}


/************************************************************************************************/
/* 函数名	: Psl_AI2C_DeInit																	*/
/* 输入		: 无																				*/
/* 输出		: 无 																				*/
/* 作用		: 删除模拟I2C功能																		*/
/* 创建日期	: 2015/11/06																		*/
/************************************************************************************************/
void Psl_AI2C_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	INT8U      err;
	/* 初始化端口 */
	RCC_APB2PeriphClockCmd(Psl_I2C_SDA_CLK|Psl_I2C_SCL_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Pin		= Psl_I2C_SCL_PIN;
	GPIO_Init(Psl_I2C_SCL_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin		= Psl_I2C_SDA_PIN;
	GPIO_Init(Psl_I2C_SDA_PORT, &GPIO_InitStructure);
	/* 删除模拟I2C信号量 */
	OSSemDel(Dev_AI2C_Sem,OS_DEL_NO_PEND,&err);
}





























