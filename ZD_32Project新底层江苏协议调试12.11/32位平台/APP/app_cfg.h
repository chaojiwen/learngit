#ifndef  APP_CFG_H    
#define  APP_CFG_H   
   
/*******************�����������ȼ�*******************/
#define Startup_Task_PRIO      (4u)   						//��ʼ		�������ȼ�

#define OS_TASK_TMR_PRIO       (Startup_Task_PRIO+1)  		//�����ʱ��     ���ȼ�

#define APP_TASK_RFCard_PRIO   (OS_TASK_TMR_PRIO+1)   		//RF��Ƭ		�������ȼ�

#define APP_TASK_Save_PRIO     (APP_TASK_RFCard_PRIO+1)   	//��Ϣ����	�������ȼ�

#define APP_TASK_Dis_PRIO      (APP_TASK_Save_PRIO+1)   	//��ʾ		�������ȼ�

#define APP_TASK_Key_PRIO      (APP_TASK_Dis_PRIO+1)   		//����		�������ȼ�

#define APP_TASK_Time_PRIO     (APP_TASK_Key_PRIO+1)   		//ʱ��		�������ȼ�

#define APP_TASK_GPS_PRIO      (APP_TASK_Time_PRIO+1)   	//GPS		�������ȼ�

#define APP_TASK_State_PRIO    (APP_TASK_GPS_PRIO+1)   	    //״̬����	�������ȼ�

#define APP_TASK_Gateway_PRIO  (APP_TASK_State_PRIO+1)   	//����		�������ȼ�

#define APP_TASK_GatewayHZZH_PRIO  (APP_TASK_Gateway_PRIO+1)   	//���ݴ��������������ȼ�

#define APP_TASK_Upg_PRIO      (APP_TASK_GatewayHZZH_PRIO+1)   	//��������	�������ȼ�

#define APP_TASK_COM_PRIO      (APP_TASK_Upg_PRIO+1)   		//ͨ��		�������ȼ�

#define APP_TASK_Beep_PRIO     (APP_TASK_COM_PRIO+1)   		//Beep		�������ȼ�

#define APP_TASK_Light_PRIO    (APP_TASK_Beep_PRIO+1)   	//Light		�������ȼ�

#define APP_TASK_ADC_PRIO      (APP_TASK_Light_PRIO+1)	  	//ADC����	�������ȼ�

#define APP_TASK_Print_PRIO    (APP_TASK_ADC_PRIO+1)	  	//��ӡ		�������ȼ�

#define APP_TASK_Easy_Print_PRIO (APP_TASK_Print_PRIO+1)	//һ����ӡ	�������ȼ�

#define APP_TASK_GSM_PRIO       (APP_TASK_Easy_Print_PRIO+1) //GPRS		�������ȼ�

#define APP_TASK_GSM_Config_PRIO       (APP_TASK_GSM_PRIO+1) //GSM	����	�������ȼ�



/************����ջ��С����λΪ OS_STK ��************/   
#define STARTUP_TASK_STK_SIZE   	80			//��ʼ		�����ջ��С

#define APP_TASK_RFCard_STK_SIZE   	80			//RF��Ƭ		�����ջ��С

#define APP_TASK_Save_STK_SIZE  	80			//��Ϣ����	�����ջ��С

#define APP_TASK_ADC_STK_SIZE  		256			//ADC����	�����ջ��С

#define APP_TASK_Dis_STK_SIZE  		256			//��ʾ		�����ջ��С

#define APP_TASK_Key_STK_SIZE  		80			//����		�����ջ��С

#define APP_TASK_Time_STK_SIZE  	80			//ʱ��		�����ջ��С

#define APP_TASK_GPS_STK_SIZE  		256			//GPS		�����ջ��С

#define APP_TASK_State_STK_SIZE 	(768u)		//״̬����	�����ջ��С
 
#define APP_TASK_Upg_STK_SIZE   	80			//��������	�����ջ��С

#define APP_TASK_COM_STK_SIZE  		512			//ͨ��		�����ջ��С

#define APP_TASK_Beep_STK_SIZE  	80			//������		�����ջ��С

#define APP_TASK_Light_STK_SIZE 	80			//����ʾ		�����ջ��С

#define APP_TASK_Print_STK_SIZE 	128			//��ӡ		�����ջ��С

#define APP_TASK_Easy_Print_STK_SIZE (128u)		//һ����ӡ	�����ջ��С


#define GSM_Config_STK_SIZE (128u)		//GSMģ����������(��Ҫ��Wifiģ�������)



#define APP_TASK_GSM_STK_SIZE   	(2048u)		//GPRS		�����ջ��С



#endif 
