/******************** (C)Copyright zjuee.cn ************************************
* File Name          : stm32glibc_stream.h
* Author             : Zhiyix(Wang)
* Version            : V1.0.0
* Create Date        : 2014-11-06
* Last Update        : 2014-11-06
* Description        : AFIO firmware library (for IAR C++).
********************************************************************************/
#ifndef __STM32GLIBC_STREAM_H
#define __STM32GLIBC_STREAM_H

#include "stm32glibc_fifo.h"

/* Configuration Mode enumeration --------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
namespace io
{
    //	输入流
    template<class T>
    class IStream
    {
    protected:
        //	从FIFO缓冲区中读字符，如果缓冲区是空的，那么就会一直等待
        __forceinline bool Read(T &t)
        {
            if(fifo.IsEmpty())
                return false;
            fifo >> t;
            return true;
        }
        __forceinline void Clean(void)
        {
            fifo.Clean();
        }
    public:
        __forceinline IStream( FIFO<T> &q):
            fifo(q)
        {}

        __forceinline IStream &operator >>(T &t)
        {
            Read(t);
            return *this;
        }
    protected:
        FIFO<T> &fifo;
    };
    /**
      * @brief  Description ""
      * @param  FUNC_SEND_T
      * @note   提供直接发送数据到输出设备的函数
      */
    template<class T, class FUNC_SEND_T>
    class OStream
    {
    protected:
        __forceinline void Send(const T &t)
        {
            FUNC_SEND_T::Send(t);
            return;
        }
        __forceinline void Write(const T &t)
        {
            fifo << t;
            if(fifo.IsFull())
            {
                Flush();
            }
            return;
        }
    public:
        __forceinline OStream( FIFO<T> &q):
            fifo(q)
        {}

        void Flush()
        {
            for(T tmp = NULL; !fifo.IsEmpty(); )
            {
                fifo >> tmp;
                Send(tmp);
            }
        }
    protected:
        FIFO<T> &fifo;
    };

    // 文本输出流
    template<class FUNC_SEND_T>
    class TextOutStream: public OStream<char, FUNC_SEND_T>
    {
    protected:
        __forceinline void Write(const char &t)
        {
            if(t == '\n')
            {
                OStream<char, FUNC_SEND_T>::Flush();
                OStream<char, FUNC_SEND_T>::Send(t);
            }
            else
                OStream<char, FUNC_SEND_T>::Write(t);
        }
    public:
        __forceinline TextOutStream( FIFO<char> &q):
            OStream<char, FUNC_SEND_T>(q)
        {}

        TextOutStream &operator <<(const char &t)
        {
            Write(t);
            return *this;
        }
        TextOutStream &operator <<(const char *str)
        {
            while(*str)
            {
                Write(*str);
                str++;
            }
            return *this;
        }
    };

    //	文本输入流
    // 因为每次请求输入之前都要清空输出流的缓冲区, 所以要持有一个相应的文件流的引用.
    template<class FUNC_SEND_T>
    class TextInStream:  public IStream<char>
    {
    protected:
        __forceinline bool Read(char &c)
        {
            return IStream<char>::Read(c);
        }
        __forceinline void Clean(void)
        {
            IStream<char>::Clean();
        }
    public:
        __forceinline TextInStream( FIFO<char> &q, TextOutStream<FUNC_SEND_T> &o):
            IStream<char>(q),
            output(o)
        {}
		// TextInStream Startup Initialization
        void Init(uint32_t period = 1000, FunctionalState e = ENABLE) //jump input.DeInit
        {
            FUNC_SEND_T::TimoutInit(period);
            FUNC_SEND_T::TimoutEnable(e);
            timout_tick = (0);
        }
		// TextInStream Close DeInitialization
        void DeInit(void) //jump input.DeInit
        {
            FUNC_SEND_T::TimoutEnable(DISABLE);
            timout_tick = (0);
        }
        TextInStream &operator >>(char &c)
        {
            bool bRead = false;
            output.Flush();		// 在请求输入之前把输出流清空(全部发送出去)
            do
            {
                bRead = Read(c);
            }
            while(!(bRead || IsTimout()));
            return *this;
        }
#define TXT_IN_STREAM_INIT		0x50
#define TXT_IN_STREAM_WORK		0x51
#define TXT_IN_STREAM_END		0x5F
        // 这个函数会吃掉输入流中前面的所有连续空格字符，以及下一个空格字符
        TextInStream &operator >>(char *str)
        {
			uint32_t fRecvTimout = 0;
			uint32_t fRecvSize   = 0;
            Clean();
            output.Flush();		// 在请求输入之前把输出流清空(全部发送出去)
			for (txtInStreamFSM = TXT_IN_STREAM_INIT; txtInStreamFSM != TXT_IN_STREAM_END;)
			{
				switch (txtInStreamFSM)
				{
				case TXT_IN_STREAM_INIT:
					if (Read(*str))
					{
						str ++;
						fRecvSize   ++;
						txtInStreamFSM = TXT_IN_STREAM_WORK;
						FUNC_SEND_T::Debug('@');
					}
					if (timout_dest < timout_tick)
					{
						*str = '\0';
						txtInStreamFSM = TXT_IN_STREAM_END;
						FUNC_SEND_T::Debug('!');
					}
					break;
				case TXT_IN_STREAM_WORK:
					if (Read(*str))
					{
						str ++;
						fRecvSize ++;
						fRecvTimout = 0;
					}
					if (fRecvTimout++ > FUNC_SEND_T::TMR_DLAY)	// 超时时间=TMR_DLAY^2(ms)
					{
						*str = '\0';
						txtInStreamFSM = TXT_IN_STREAM_END;
						FUNC_SEND_T::Debug('#');
					}
					if (fRecvSize >= txtInStreamSize)
					{
						*str = '\0';
						txtInStreamFSM = TXT_IN_STREAM_END;
						FUNC_SEND_T::Debug('$');
					}
					break;
				case TXT_IN_STREAM_END:
					return *this;
				}
				FUNC_SEND_T::TimeoutDelay();
			}
            return *this;
        }
    protected:
        TextOutStream<FUNC_SEND_T> &output;
    public:
        __IO uint32_t timout_tick;
        __IO uint32_t timout_dest;
        __IO uint32_t txtInStreamSize;
        __IO uint32_t txtInStreamFSM;
		// Initialization
        __forceinline void TimoutInit(uint32_t to, uint32_t size)
        {
			txtInStreamSize = size;
			timout_tick = 0x00;
            timout_dest = timout_tick + to * FUNC_SEND_T::TMR_RATE;
        }
		// Interrupt Service Function
        __forceinline void TimoutInc(uint32_t delta)
        {
            timout_tick += delta;
        }
		// true:A timeout occurred!
        __forceinline bool IsTimout(void)
        {
            return (timout_dest < timout_tick);
        }
    };
}
//!
//! @{
using namespace io;
//! @}

#endif /* __STM32GLIBC_STREAM_H */
/*******************************************************************************
**                                END OF FILE                                 **
*******************************************************************************/
