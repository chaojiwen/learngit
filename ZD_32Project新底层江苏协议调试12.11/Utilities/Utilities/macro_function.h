/**
  ******************************************************************************
  * @file              : macro_function.h
  * @author            : HZ Zeda Team
  * @version           : V1.0.0.0
  * @date              : 2014-11-21
  * @brief             : h file
  * @description       : Main program body
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2014 STMicroelectronics
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MACRO_FUNCTION
#define MACRO_FUNCTION


/*============================ INCLUDES ======================================*/
//! @name include
//! @{
#include <stdio.h>
//! @}

/*============================ MACROS ========================================*/
#undef  BIT
#define BIT(x)					((uint32_t)0x01L<<(x))

#undef  Bit
#define Bit(x)					((uint32_t)0x01L<<(x))

/**
  * @brief  Description "得到一个指定范围的BIT被置位的值"
  * @param  None
  * @retval BitFromTo(3, 0)	会得到 0x0f
  * @note 	1. 这个宏是为提高可读性才写的, 对于常数参数是表达式的, 编译器会自动优化.
  */
#define BitFromTo(h, l)															\
		((h > l)? ((BIT(h) - BIT(l) + BIT(h))): (BIT(l) - BIT(h) + BIT(l)) )
#define BitF2	BitFromTo
#define BitFT	BitFromTo


/**
  * @brief  Description "实现写8位二进制数的宏"
  * @param  b	以二进制格式写的长度, 不可以超过一个字节(8位)
  * @retval 如BByte(01011010) == 0x5a
  */
#define BByte(b)	(						\
	(((( 0x##b##L )			) & 1 )<<0)	|	\
	(((( 0x##b##L )	>>4		) & 1 )<<1)	|	\
	(((( 0x##b##L )	>>8		) & 1 )<<2)	|	\
	(((( 0x##b##L )	>>12	) & 1 )<<3)	|	\
	(((( 0x##b##L )	>>16	) & 1 )<<4)	|	\
	(((( 0x##b##L )	>>20	) & 1 )<<5)	|	\
	(((( 0x##b##L )	>>24	) & 1 )<<6)	|	\
	(((( 0x##b##L )	>>28	) & 1 )<<7)		\
	)

/**
  * @brief  Description "把一个二进制是8位的数的每一位按指定的倍数展开"
  * @param  var		变量
  * @param  n		指定的倍数, 注意展开后不可以超过32位, 所以n的合法的值应该是 1 ~ 4 (包括1和4)之间
  * @retval None
  */
#define ExpandU8Bit(num, n)	(													\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(0)))) << (0*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(1)))) << (1*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(2)))) << (2*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(3)))) << (3*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(4)))) << (4*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(5)))) << (5*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(6)))) << (6*(n-1)))		|	\
			( ((((uint32_t)(num)) & ((uint32_t)BIT(7)))) << (7*(n-1)))		)



/**
  * @brief  Description "实现写16位二进制的宏"
  * @param  h	高8位
  * @param  l	低8位
  * @retval BWord(01011010, 10100101) == 0x5aa5
  */
#define BWord(h,l)				( (((uint16_t)(BByte(h)))<<8) + BByte(l))

/*============================ TYPES =========================================*/
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERN FUNCTIONS ==============================*/

/**
  * @brief  Description "这个宏用来得到一个变量的某个或某几个位被赋值后的值"
  * @param  type		参数flag的类型
  * @param  flag		要赋值的变量
  * @param  which_bit	标明flag的哪几个位要被赋值
  * @param  val			表示要赋给flag的那几个位的值
  * @note   1. 对于宏, 其参数不要用++i之类的表达式! 也不要直接使用调用函数得到的返回值!
  * @note   2. 如果type是寄存器, 那么它必须是可读的
  * @note   3. 效率上, 如果参数(除flag外)都是常量, 编译器会自动优化, 如果参数是变量, 就算手动写代码也是这个样子的
  * @note   4. (which_bit) 括号是为了保证 AssignBit(flag, BIT0 | BIT1, 0)之类的用法不会出错
  * @examp 	u8 a = 0xf0;
  *			AssignBit(u8, a, BIT4 | BIT0, BIT0);
  *			执行完后，a == 0xe1;
  */
#define AssignBit(type ,flag , which_bit, val)									\
		((((type)which_bit)&((type)val)) | ((flag)&(~((type)which_bit))))
#define AssignFlag	AssignBit


/**
  * @brief  Description "得到一个变量某几位位被置位后的值"
  * @param  None
  * @note 	1. 如果flag是寄存器, 必须是可读的
  */
#define SetBit(type, flag, which_bit)			((flag) | ((type)which_bit))
#define SetB	SetBit


/**
  * @brief  Description "清零位, flag变量中which_bit标明的位被清0"
  * @param  None
  * @note 	1. flag必须是可以正常读写的
  */
#define ClearBit(type, flag, which_bit)			((flag) & (~((type)which_bit)))
#define ClearB	ClearBit


/**
  * @brief  Description "检查flag中的某一位是否被置位, 如果被置位, "
  * @brief			    "则宏的值为非0(不一定是1), 如果没有被置位, 则宏的值为0"
  * @param  None
  * @note 	1. flag必须是可以读的
  */
#define CheckBit(type, flag, which_bit)			( (flag) & ((type)which_bit) )


/**
  * @brief  Description "得到变量的一个位段被赋值后的值, 这个宏本身不会改变变量的值"
  * @param  type	变量var的类型
  * @param  var		变量
  * @param  LSB		该位段的最低位
  * @param  len		位段的长度，最大值为8
  * @param  val		位段的值
  * @note 	如果var是一个寄存器, 那么它必须是正常可读的
  */
#define AssignBitSection(type, var, LSB, length, value)							\
		AssignBit(type, var, ((1<<(length)) - 1)*LSB, (value)*LSB)
#define AssignBS	AssignBitSection


#endif /* MACRO_FUNCTION */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


