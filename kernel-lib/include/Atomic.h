#ifndef _ATOMIC_H
#define _ATOMIC_H
/*===============================================================
 *                      Atomic.h
 *                     ԭ�������� ����
 *                     								��Ҷ-��������
 ===============================================================*/
#include "Type.h"

class Atomic
{
public:

	inline int Read();				/* �� */
	inline void Write(int Value);	/* д */

	inline void Add(int Value);		/* �� */
	inline void Sub(int Value);		/* �� */

	/* �Լӻ��Լ����� */
	inline void Inc();		/* ��1 */
	inline void Dec();		/* ��1 */

	/* �ӻ���󣬷�����ֵ */
	inline int AddRet(int Value);
	inline int SubRet(int Value);
	inline int IncRet();
	inline int DecRet();

	/* λͼ���� */

	inline void SetBit(u8 Bit);
	inline void ClearBit(u8 Bit);
	inline bool TestBit(u8 Bit);
private:
	volatile int value;
};
/************************************************************************/
/*					       ��д����
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
/*					       �Ӽ�����
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
/*					      �Լӻ��Լ�����
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
/*					      �Ӽ��󷵻���ֵ
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
/*					       λͼ����
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
