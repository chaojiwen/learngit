/************************************************************************************************/
/** @file              : C_Json.C																*/
/** @author            : Mr.ZRDang Team															*/
/** @version           : V1.0.0.0																*/
/** @date              : 2016年09月17日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--32位变送器--驱动层--Json文件								*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : C_Json.C																			*/
/**作用       : Json数据驱动																		*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2016年09月17日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef C_Json__h
#define C_Json__h

/* C JSON Types: */
#define cJSON_False 		(0ul)	//逻辑值"假"
#define cJSON_True 			(1ul)	//逻辑值"真"
#define cJSON_NULL 			(2ul)	//"null"	
#define cJSON_Number 		(3ul)	//整型
#define cJSON_String 		(4ul)	//字符串
#define cJSON_Array 		(5ul)	//数组"[]"
#define cJSON_Object 		(6ul)	//对象{}
#define cJSON_IsReference 	(256ul)	//？

/* C_JSON 项目结构 */
typedef struct JSON {
	//next和prev构成有Json数据结构的单向链表
	struct 	C_JSON *next;		/* 下一个Json类型指针。(Alternatively, use GetArraySize/GetArrayItem/GetObjectItem) */
	struct 	C_JSON *prev;		/* 上一个Json类型指针。 */
	struct 	C_JSON *child;		/* 数组或者对象的子项。 */
	int 	type;				/* Json类型 */

	char 	*valuestring;		/* Json字符串类型指针 */
	int 	valueint;			/* Json整型数据 */
	float 	valuefloat;			/* Json浮点型数据 */

	char 	*string;			/* ？？？The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} JSON;








/******************* (C) HZ Zeda Team *******************/
/********************* END OF FILE **********************/

