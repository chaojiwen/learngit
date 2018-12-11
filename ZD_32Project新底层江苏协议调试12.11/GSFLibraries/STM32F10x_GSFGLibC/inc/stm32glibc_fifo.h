/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_fifo.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_FIFO_H
#define __STM32GLIBC_FIFO_H

#include "stm32glibc_type.h"

/* Configuration Mode enumeration --------------------------------------------*/
#define BUFFER_READ(n)			(pBuffer[(n)&(nMaxSize-1)])
#define BUFFER_WRITE(n, v)		(pBuffer[(n)&(nMaxSize-1)] = v)
#define __ATOM_ACCESS(__CODE)	SAFE_ATOM_CODE(__CODE)
/* Exported constants --------------------------------------------------------*/
namespace io
{
    //	FIFO队列
    template<class T>
    class FIFO
    {
    public:
        // 队列的constructor, 对象一旦构造, 大小就不能再改变了
        FIFO(T *buf, unsigned int max):
            pBuffer(buf), nRdIndex(0), nWrIndex(0), nMaxSize(max), nCurSize(0)
        {
			int nIndex = 0;
			for (nIndex = 31; nIndex >=0; nIndex --)
			{
				if ((max & (0x1ul << nIndex)) != 0)
				{
					if (max == (0x1ul << nIndex))
					{
						return;
					}
				}
			}
			while (1);
        }
        void __forceinline Clean()
        {
            nRdIndex = nWrIndex;
			nCurSize = 0;
        }
        unsigned int __forceinline GetCurSize()
        {
            return nCurSize;
        }
        unsigned int __forceinline GetMaxSize()
        {
            return nMaxSize;
        }
        __forceinline bool IsFull()
        {
			bool bResult = false;
			__ATOM_ACCESS(
				if ((nRdIndex == nWrIndex) && (0 != nCurSize))
					bResult = true;
			)
			return bResult; 
        }
        __forceinline bool IsEmpty()
        {
			bool bResult = false;
			__ATOM_ACCESS(
				if ((nRdIndex == nWrIndex) && (0 == nCurSize))
					bResult = true;
			)
			return bResult; 
        }
        //	如果队列满了，后面任何写入的操作都会被忽略
        __forceinline FIFO &operator << (T c)
        {
			__ATOM_ACCESS(
				if(!IsFull())
				{
					nCurSize++;
					BUFFER_WRITE(nWrIndex++, c);
				}
			)
            return *this;
        }
        //	如果队列空了，后面任何读的操作都会被忽略
        __forceinline FIFO &operator >> (T &c)
        {
			__ATOM_ACCESS(
				if(!IsEmpty())
				{
					nCurSize--;
					c = BUFFER_READ(nRdIndex++);
				}
			)
            return *this;
        }
    protected:
        __IO T *pBuffer;
        __IO unsigned int nRdIndex;
        __IO unsigned int nWrIndex;
        __IO unsigned int nMaxSize;
        __IO unsigned int nCurSize;
    };
}
namespace io2
{
    //	FIFO队列
    template<class T>
    class FIFO
    {
    public:
        // 队列的constructor, 对象一旦构造, 大小就不能再改变了
        FIFO(T *buf, unsigned int max):
            pBuffer(buf), nRdIndex(0), nWrIndex(0), nMaxSize(max), nCurSize(0)
        {
			int nIndex = 0;
			for (nIndex = 31; nIndex >=0; nIndex --)
			{
				if ((max & (0x1ul << nIndex)) != 0)
				{
					if (max == (0x1ul << nIndex))
					{
						return;
					}
				}
			}
			while (1);
        }
        void __forceinline Clean()
        {
            nRdIndex = nWrIndex;
			nCurSize = 0;
        }
        unsigned int __forceinline GetCurSize()
        {
            return nCurSize;
        }
        unsigned int __forceinline GetMaxSize()
        {
            return nMaxSize;
        }
        __forceinline bool IsFull()
        {
			bool bResult = false;
			__ATOM_ACCESS(
				if (nCurSize == nMaxSize)
					bResult = true;
			)
			return bResult; 
        }
        __forceinline bool IsEmpty()
        {
			bool bResult = false;
			__ATOM_ACCESS(
				if (nCurSize == 0)
					bResult = true;
			)
			return bResult; 
        }
        //	如果队列满了，后面任何写入的操作都会被忽略
        __forceinline FIFO &operator << (T c)
        {
			__ATOM_ACCESS(
				if(!IsFull())
				{
					nCurSize++;
					BUFFER_WRITE(nWrIndex++, c);
				}
			)
            return *this;
        }
        //	如果队列空了，后面任何读的操作都会被忽略
        __forceinline FIFO &operator >> (T &c)
        {
			__ATOM_ACCESS(
				if(!IsEmpty())
				{
					nCurSize--;
					c = BUFFER_READ(nRdIndex++);
				}
			)
            return *this;
        }
    protected:
        __IO T *pBuffer;
        __IO unsigned int nRdIndex;
        __IO unsigned int nWrIndex;
        __IO unsigned int nMaxSize;
        __IO unsigned int nCurSize;
    };
}
//!
//! @{
using namespace io;
//! @}

#endif /* __STM32GLIBC_FIFO_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
