/******************** (C) COPYRIGHT 2009 www.armjishu.com ************************
* File Name          : date.h
* Author             : www.armjishu.com Team
* Version            : V1.0
* Date               : 12/1/2009
* Description        : 日期相关函数
*******************************************************************************/


#ifndef __date_H
#define __date_H

#include "stm32f10x.h"
#include "common.h"   

u32 mktimev(Times *tm);
void to_tm(uint32_t tim,Times * tm);
void Now_Time_Transition_Seconed_Counter(void);
void Compression_Times_Data(Times* Tm,DATETIME_Storage_Format* Compression_Data);

#endif
