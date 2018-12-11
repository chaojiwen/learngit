/**
  ******************************************************************************
  * @file              : mx_main.c
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 27/09/2014 17:41:56
  * @brief             : c file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2014 STMicroelectronics
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef INTERFACE_MX_MAIN_C
#define INTERFACE_MX_MAIN_C

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
//! \name include
//! @{
#include "..\inc\mx_main.h"
#include "app_flash.h"
//! @}

/*============================ TYPES =========================================*/


/*============================ LOCAL VARIABLES ===============================*/


/*============================ GLOBAL VARIABLES ==============================*/

/* USER CODE BEGIN 0 */
void LED_TEST(bool);
void COM_TEST(bool);
void GSM_TEST(bool);
/* USER CODE END 0 */

int main(void)
{
/* USER CODE BEGIN 1 */
	LED_TEST(false);
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
	COM_TEST(false);
/* USER CODE END 2 */

	//! 检查系统是否首次运行，若是，则将默认运行参数初始化到Flash中
	APP_FLASH[0].Init();
	//!
	if(APP_FLASH[0].Check1stRun())
	{
		printf("Device is in first running.\r\n");		
		
		APP_FLASH[0].StructInit();

		printf("Evironment variables initialized into Flash.\r\n");
	}
	else
	{	
		if(APP_FLASH[0].CheckNormalStartup())
		{
			printf("Device is in non-first running.\r\n");
			
			printf("Device is shutdown manually last time.\r\n");
		}
		else
		{
			printf("Device is in non-first running.\r\n");
			
			printf("Device is not shutdown manually last time.\r\n");
		}
	}

	// 不管系统是否首次启动，每次启动时都会从flash中恢复环境变量参数	
	APP_FLASH[0].Recover();
	
	printf("Environment variables recoveried from Flash.\r\n");	
	
/* USER CODE BEGIN 2 */
	GSM_TEST(true);
/* USER CODE END 2 */
	/* Infinite loop */
	while (1)
	{
		
	}
  
/* USER CODE BEGIN 3 */

/* USER CODE END 3 */ 
}


#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    
    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/*============================ EXTERN FUNCTIONS ==============================*/
#ifdef __cplusplus
}
#endif

#endif /* INTERFACE_MX_MAIN_C */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

