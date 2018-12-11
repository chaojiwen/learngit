/**
  ******************************************************************************
  * @file              : template.h
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
#ifndef TEMPLATE_H
#define TEMPLATE_H

/*============================ INCLUDES ======================================*/
//! @name include
//! @{
//! @}

/*============================ MACROS ========================================*/

/*============================ TYPES =========================================*/
/*============================ INTERFACE =====================================*/

/*============================ PROTOTYPES ====================================*/

/*============================ EXTERN FUNCTIONS ==============================*/
/**
  * @brief  Description "得到一个变量bits里的连续的几个位的最低位"
  * @exampl u16 a = BByte(00111000); 那么GetLSB(a)会返回 BByte(00001000)
  * @retval None
  */
template<class B>
__forceinline B GetLSB(B bits)
{
    return (B)(((bits << 1) | bits) ^ (bits << 1));
}


/**
  * @brief  Description "得到一个变量某几位位被置位后的值, 这个函数不会改变变量本身的值"
  * @note  	测试通过了, 用模板的方式会更好一点. 但是需要编译器设置optimize for time, 否则会生成一个函数
  * @note  	造成代码和时间上的浪费. (说代码浪费是因为传递参数的过程比计算的过程更常...)
  * @note 	1. 如果flag是寄存器, 必须是可读的
  */
template<class Flag_Type, class W>
__forceinline Flag_Type SetBBit(Flag_Type flag, W whichBit)
{
    return ((flag) | ((Flag_Type)whichBit));
}


/**
  * @brief  Description "清零位, flag变量中which_bit标明的位被清0"
  * @note   1. flag必须是可以正常读写的
  */
template<class Flag_Type, class W>
__forceinline Flag_Type ClearBBit(Flag_Type flag, W whichBit)
{
    return ((flag) & (~((Flag_Type)whichBit)));
}


/**
  * @brief  Description "检查flag中的某些是否被置位, 如果那些位都为0, 则返回0, 否则返回其它值"
  * @note   1. flag必须是可以读的
  */
template<class Flag_Type, class W>
__forceinline Flag_Type CheckBBit(Flag_Type flag, W whichBit)
{
    return 	( (flag) & ((Flag_Type)whichBit) );
}


/**
  * @brief  Description "这个函数用来得到一个变量的某个或某几个位被赋值后的值"
  * @param  flag	要赋值的变量
  * @param  which_bit 标明flag的哪几个位要被赋值
  * @param  val		表示要赋给flag的那几个位的值
  * @note	1. 对于宏, 其参数不要用++i之类的表达式! 也不要直接使用调用函数得到的返回值!
  * @note	2. 如果type是寄存器, 那么它必须是可读的
  * @note	3. 效率上, 如果参数(除flag外)都是常量, 编译器会自动优化, 如果参数是变量, 就算手动写代码也是这个样子的
  * @examp 	u8 a = 0xf0;
  *			AssignBit(u8, a, BIT4 | BIT0, BIT0);
  *			执行完后，a == 0xe1;
  */
template<class Flag_Type, class W, class V>
__forceinline Flag_Type AssignBBit(Flag_Type flag, W whichBit, V val)
{
    return ((((Flag_Type)whichBit) & ((Flag_Type)val)) | ((flag) & (~((Flag_Type)whichBit))));
}


/**
  * @brief  Description "得到变量的一个位段被赋值后的值, 这个函数本身不会改变变量的值"
  * @param  var		变量
  * @param  LSB		该位段的最低位
  * @param  len		位段的长度，最大值为8
  * @param  val		位段的值
  * @note	如果var是一个寄存器, 那么它必须是正常可读的
  */
template<class Flag_Type, class LSB, class L, class V>
__forceinline Flag_Type AssignBField(Flag_Type flag, LSB lsb, L len, V val)
{
    return AssignBBit(flag, ((1 << (len)) - 1) * ((Flag_Type)lsb), (Flag_Type)val * ((Flag_Type)lsb));
}


/**
  * @brief  Description "得到一个变量的一个 bit field 被设置为指定的值后的变量值, 这个函数本身不会改变变量的值"
  * @brief  Description "得到变量的连续几个位被赋值后的值, 这个函数不会改变传入的变量的值"
  * @param  flag	变量
  * @param  bits	连续的几个位
  * @param  val		bits表示的几个位要被赋的值
  * @note	val 不需要预先移位, 这个函数和assignb的使用不同
  * @examp 	u16 a = 0;	a = assignbs(a, BitFromTo(11, 4), 0x5a);
  *			那么执行完后，a = 0x05a0;
  */
template<class Flag_Type, class BITS, class V>
__forceinline Flag_Type AssignBField(Flag_Type flag, BITS bits, V val)
{
    return AssignBBit(flag, bits, (((Flag_Type)val) * ((Flag_Type)( GetLSB(bits) ))));
}

#define	assignf		AssignBField			// assign bit field


/**
  * @brief  Description "读一个变量的一个 bit field 的值"
  * @param  None
  * @retval None
  */
template<class Flag_Type, class BITS>
__forceinline Flag_Type ReadField(Flag_Type flag, BITS bits)
{
    return (Flag_Type)((flag & bits) / (Flag_Type)GetLSB(bits));
}

#define readf		ReadField


/**
  * @brief  Description "如果变量letter是一个小写字母, 使用这个函数可以得到相应的大家字母(不会改变变量的值)"
  * @param  None
  * @retval None
  */
__forceinline uint8_t getupper(uint8_t letter)
{
    return 	((((letter) >= 'a') && ((letter) <= 'z')) ? ((letter) - 'a' + 'A') : (letter) );
}


/*******************************************************************************
2012.3.7
除了getupper未测试，其它均测试通过，测试代码：仿真结果和预期一致！

加这个函数的目的是不让编译器把我测试的几个变量优化掉。
void fun(u32 a, u32 b, u32 c, u32 d, u32 e)
{

}
int main(void)
{
	u32 a, b, c, d = 0xffffffff;
	a = assignb(a, Bit(4), 0xff);
	b = assignbs(b, Bit(4), 4, 0xf);
	c = setb(c, Bit(0));
	d = clearb(d, c);
	fun(a, b, c, d, 0);
｝

2012.3.10
	打算增加一个功能函数：toggleb，利用异或的方式实现。
*******************************************************************************/

#endif /* TEMPLATE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

