/************************************************************************************************/
/** @file              : ComProtocol_SetPlateNo.h												*/
/** @author            : ZJUEE Team																*/
/** @version           : V1.0.0.0																*/
/** @date              : 2017年02月14日															*/
/** @brief             : c file																	*/
/** @description       : 杭州泽大仪器--变送器--通信协议--设置车牌号码 应用文件						*/
/************************************************************************************************/
/** @attention		   :																		*/
/**																								*/
/** COPYRIGHT 2015 STMicroelectronics															*/
/**																								*/
/**文件名     : ComProtocol_SetPlateNo.h															*/
/**作用       : 将GB2313格式的车牌号码，转换为UTF8格式											*/
/**原理       : 																					*/
/************************************************************************************************/
/**版本     作者            日期            说明													*/
/**V0.1    ZRDang      2017年02月14日   	 初始版本												*/
/************************************************************************************************/
/************************************************************************************************/
#ifndef __ComProtocol_SetPlateNo_h
#define	__ComProtocol_SetPlateNo_h

#include "stm32f10x.h"




/********************************* 函数声明 *********************************/
void ComProtocol_SetPlateNo(uint8_t* cbuf,char* plateNoString);




#endif
/******************* (C) HZ Zeda Team *******************/

/*******************   (END OF FILE   *******************/
