/************************************************************************************************/
/** @file              : C_Json.C																*/
/** @author            : Mr.ZRDang Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016��09��17��															*/
/** @brief             : c file																	*/
/** @description       : �����������--32λ������--������--Json�ļ�								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**�ļ���     : C_Json.C																			*/
/**����       : Json��������																		*/
/**ԭ��       : 																					*/
/************************************************************************************************/
/**�汾     ����            ����            ˵��													*/
/**V0.1    ZRDang      2016��09��17��   	 ��ʼ�汾												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef C_Json__h
#define C_Json__h

/* C JSON Types: */
#define cJSON_False 		(0ul)	//�߼�ֵ"��"
#define cJSON_True 			(1ul)	//�߼�ֵ"��"
#define cJSON_NULL 			(2ul)	//"null"	
#define cJSON_Number 		(3ul)	//����
#define cJSON_String 		(4ul)	//�ַ���
#define cJSON_Array 		(5ul)	//����"[]"
#define cJSON_Object 		(6ul)	//����{}
#define cJSON_IsReference 	(256ul)	//��

/* C_JSON ��Ŀ�ṹ */
typedef struct JSON {
	//next��prev������Json���ݽṹ�ĵ�������
	struct 	C_JSON *next;		/* ��һ��Json����ָ�롣(Alternatively, use GetArraySize/GetArrayItem/GetObjectItem) */
	struct 	C_JSON *prev;		/* ��һ��Json����ָ�롣 */
	struct 	C_JSON *child;		/* ������߶������� */
	int 	type;				/* Json���� */

	char 	*valuestring;		/* Json�ַ�������ָ�� */
	int 	valueint;			/* Json�������� */
	float 	valuefloat;			/* Json���������� */

	char 	*string;			/* ������The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} JSON;








/******************* (C) HZ Zeda Team *******************/
/********************* END OF FILE **********************/

