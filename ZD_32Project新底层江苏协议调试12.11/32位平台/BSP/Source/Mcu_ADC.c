/******************** (C) zhou ***************************
 * 文件名  ：
 * 描述    ：       
 * 实验平台：32 位平台（测试）
 *
 * 库版本  ：ST3.5.0
 *
 * 作者    ：zhou 
**********************************************************************************/
#include "Mcu_ADC.h"

#include "Mcu_DMA.h"

#include "Log_Adc1.h"
#include "Log_Sensor.h"




static void ADC1_Mode_config(void);
static void ADC1_GPIO_Config(void);

/************************************************************************************************/
/* 函数名	: ADC1_Mode_Config															 	 	*/
/* 输入		: 无 																		 		*/
/* 输出		: 无 																				*/
/* 作用		: 配置ADC1的工作模式为MDA模式									 						*/
/* 创建日期	: 2015/04/10																		*/
/************************************************************************************************/
static void ADC1_Mode_config(void)
{
/* ADC采样通道数量 这里暂时这样 */
#define ADC1_CH_NUM		(9u)


	
	ADC_InitTypeDef ADC_InitStructure;

	DMA_Rece_ADC1_Data_Config();
	/* 配置ADC1 */
	ADC_InitStructure.ADC_Mode 					= ADC_Mode_Independent;				//使用单ADC模式
	ADC_InitStructure.ADC_ScanConvMode 			= ENABLE;							//使能扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode 	= DISABLE;							//使能连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv 		= ADC_ExternalTrigConv_None;		//不设置外部触发事件
	ADC_InitStructure.ADC_DataAlign 			= ADC_DataAlign_Right;				//设置ADC1的数据对齐方式为 右对齐
	ADC_InitStructure.ADC_NbrOfChannel 			= ADC1_CH_NUM;						/* 这个什么意思不清楚 */
	ADC_Init(ADC1,&ADC_InitStructure);												//将数据写入寄存器
	/* 配置ADC1的通道规则 *///设置ADC1的通道11转换时间为239.5周期
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14,5,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,6,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,7,ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,8,ADC_SampleTime_239Cycles5);
	//BT->
	ADC_RegularChannelConfig(ADC1,ADC_Channel_8,9,ADC_SampleTime_239Cycles5);

	/* 配置高、低模拟看门狗阈值 */
	ADC_AnalogWatchdogThresholdsConfig(ADC1, 0x0FFF, 0X0000);
	/* 通道14配置为单模拟看门狗保护通道 */
	ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_8);
	/* 在一个常规频道上启用模拟看门狗 */
	ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);
	/* 使能模拟看门狗中断 */
	ADC_ITConfig(ADC1, ADC_IT_AWD, ENABLE);
	
	/* 使能ADC的DMA功能 */		  
	ADC_DMACmd(ADC1,ENABLE);																				
	/* 使能ADC1 */
	ADC_Cmd(ADC1,ENABLE);
	/* 复位ADC1的重置校准寄存器 */
	ADC_ResetCalibration(ADC1);
	/* 检测被选中的ADC1重置校准寄存器复位状态完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	/* 开始ADC1的校准 */
	ADC_StartCalibration(ADC1);
	/* 检测被选中的ADC1重置校准寄存器就绪状态完成 */
	while(ADC_GetCalibrationStatus(ADC1));
	/* 开始ADC1转换 */
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);	
	 		
#undef ADC1_CH_NUM
}

/************************************************************************************************/
/* 函数名	: ADC1_GPIO_Config															 	 	*/
/* 输入		: 无 																		 		*/
/* 输出		: 无 																				*/
/* 作用		: 使能ADC1和DMA1的时钟，初始化PC.01							 						*/
/* 创建日期	: 2015/04/10																		*/
/************************************************************************************************/
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能DMA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	/* 使能GPIOA和ADC1的时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
						   RCC_APB2Periph_GPIOB |
						   RCC_APB2Periph_GPIOC |
						   RCC_APB2Periph_GPIOE |
						   RCC_APB2Periph_ADC1,ENABLE);

	/* 设置IO的工作方式 *///CH0\CH1\CH2\CH3\CH4\CH5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
								  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//IO为模拟输入
	GPIO_Init(GPIOC,&GPIO_InitStructure);//将数据写入寄存器
	
	/* 设置IO的工作方式 *///CH6\CH7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;//IO为模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure);//将数据写入寄存器
	
	/* 设置IO的工作方式 *///CH8->BT
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//IO为模拟输入
	GPIO_Init(GPIOB,&GPIO_InitStructure);//将数据写入寄存器
	
	/* 设置 传感器 的电源控制 引脚 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	Sensor_Power_Close;
	GPIO_SetBits(GPIOE, GPIO_Pin_15);
	
#if (Sensor_PT1000_Num > 0x00)
	/* 设置 传感器 的电源控制 引脚 */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	Sensor_Power_Close;
	GPIO_SetBits(GPIOE, GPIO_Pin_12);
#endif
	
}

/************************************************************************************************/
/* 函数名	: ADC1_Init																 	 	*/
/* 输入		: 无 																		 		*/
/* 输出		: 无 																				*/
/* 作用		: 配置ADC1							 												*/
/* 创建日期	: 2015/04/10																		*/
/* 修改日期	: 2015/11/11																		*/
/************************************************************************************************/
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_config();
	
	/* 仪器电源电压检测:开 */
	Itu_Pwr_Spe_Open;
	/* 传感器信号检测  :开 */
	Sen_Pwr_Spe_Open;
	
#if (Sensor_PT1000_Num > 0x00)
	/* PT1000传感器信号检测  :开 */
	PT1000_Pwr_Spe_Open;
#endif
}


/******************* (C) ZHOU *****END OF FILE****/
