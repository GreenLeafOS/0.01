#ifndef _ATOMIC_H
#define _ATOMIC_H
/*===============================================================
 *                      Atomic.h
 *                     原子数据类 定义
 *                     								绿叶-柳暗花明
 ===============================================================*/
#include "Type.h"

class Atomic
{
public:

	int Read();				/* 读 */
	void Write(int Value);	/* 写 */

	void Add(int Value);		/* 加 */
	void Sub(int Value);		/* 减 */

	/* 自加或自减运算 */
	void Inc();		/* 加1 */
	void Dec();		/* 减1 */

	/* 加或减后，返回新值 */
	int AddRet(int Value);
	int SubRet(int Value);
	int IncRet();
	int DecRet();

	/* 位图操作 */

	void SetBit(u8 Bit);
	void ClearBit(u8 Bit);
	bool TestBit(u8 Bit);
private:
	volatile int value;
};

/************************************************************************/
/*					       读写操作
/*                       Read Write
/************************************************************************/
inline int Atomic::Read()
{
	return value;
}

inline void Atomic::Write(int Value)
{
	this->value = Value;
	return;
}

/************************************************************************/
/*					       加减操作
/*                         Add Sub
/************************************************************************/
inline void Atomic::Add(int Value)
{
	asm volatile("addl %1,%0"
	             : "+m" (this->value)
	             : "r" (Value));
	return;
}

inline void Atomic::Sub(int Value)
{
	asm volatile("subl %1,%0"
	             : "+m" (this->value)
	             : "r" (Value));
	return;
}
/************************************************************************/
/*					      自加或自减运算
/*                         Inc Dec
/************************************************************************/
inline void Atomic::Inc()
{
	asm volatile("incl %0"
				: "+m" (this->value));
	return;
}

inline void Atomic::Dec()
{
	asm volatile("decl %0"
				: "+m" (this->value));
	return;
}
/************************************************************************/
/*					      加减后返回新值
/*                        IncTest DecTest
/************************************************************************/
inline int Atomic::AddRet(int Value)
{
	Add(Value);
	return value;
}

inline int Atomic::SubRet(int Value)
{
	Sub(Value);
	return value;
}

inline int Atomic::IncRet()
{
	Inc();
	return value;
}

inline int Atomic::DecRet()
{
	Dec();
	return value;
}
/************************************************************************/
/*					       位图操作
/*					SetBit ClearBit TestBit
/************************************************************************/
inline void Atomic::SetBit(u8 Bit)
{
	int mask = 1 << Bit;
	asm volatile("orl %0,%1"
				:
				: "r" (mask), "m" (value)
				: "memory");
	return;

}

inline void Atomic::ClearBit(u8 Bit)
{
	int mask = ~(1 << Bit);
	asm volatile("andl %0,%1"
				:
				: "r" (mask), "m" (value)
				: "memory");
	return;

}

inline bool Atomic::TestBit(u8 Bit)
{
	int mask = 1 << Bit;
	asm volatile("andl %0,%1"
				:
				: "r" (value),"m" (mask)
				: "memory");
	return mask;

}

#endif
