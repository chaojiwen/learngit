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

/* Json数据区指针 */
static OS_MEM *JsonDataPtr;
/* Json数据区 */
static JSON Json_Buf[100];


/************************************************************************************************/
/* 函数名	: JSON_ParseWithOpts																*/
/* 输入  	: string:Json对象数据字符串， 																					*/
/* 输出		: 无 																				*/
/* 作用	 	: 解析一个对象，创建一个root(根)，并填入信息。											*/
/* 创建日期 	: 2016/09/17																		*/
/************************************************************************************************/
JSON *Json_ParseWithOpts(const char *string,const char **return_parse_end,int require_null_terminated)
{
	const char *end=0;
	cJSON *c=cJSON_New_Item();
	ep=0;
	if (!c) return 0;       /* memory fail */

	end=parse_value(c,skip(value));
	if (!end)	{cJSON_Delete(c);return 0;}	/* parse failure. ep is set. */

	/* if we require null-terminated JSON without appended garbage, skip and then check for a null terminator */
	if (require_null_terminated) {end=skip(end);if (*end) {cJSON_Delete(c);ep=end;return 0;}}
	if (return_parse_end) *return_parse_end=end;
	return c;


}

/************************************************************************************************/
/* 函数名	: cJSON_Parse																		*/
/* 输入  	: string:Json数据字符串 																*/
/* 输出		: 无 																				*/
/* 作用	 	: 解析Json数据																		*/
/* 创建日期 	: 2016/09/17																		*/
/************************************************************************************************/
JSON *Json_Parse(const char *string)
{
	/* 解析Json */
	return cJSON_ParseWithOpts(string,0,0);
}

/************************************************************************************************/
/* 函数名	: Json_Init																			*/
/* 输入  	: string:Json数据字符串 																*/
/* 输出		: 无 																				*/
/* 作用	 	: 解析Json数据																		*/
/* 创建日期 	: 2016/09/17																		*/
/************************************************************************************************/
void *Json_Init(void)
{
	
	/* 创建内存数据区 */
	JsonDataPtr = OSMemCreate(Partition, 100, 32, &err);
	
	
}

/******************* (C) HZ Zeda Team *******************/
/********************* END OF FILE **********************/

