/*************************************************************************************************/
/*************************************     32λƽ̨����     **************************************/
/* �������ڣ�2014/10/26																			 */
/*************************************************************************************************/
#include "Mcu_Power.h" 
/************************************************************************************************/
/* ������	: Power_Check_Init																 	*/
/* ����		: �� 																				*/
/* ���		: �� 																				*/
/* ����		: �����ⲿ��Դ����õ���I/O�� 														 	*/
/* ��������	: 2014/10/08																		*/
/* �޸�����	: 2014/11/11																		*/
/************************************************************************************************/
void Power_Check_Init(void)
{			   
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);
	/* �����͵�ؼ�� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;       
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	/* ��ص������ ���� �������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE****/

