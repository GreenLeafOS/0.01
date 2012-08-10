#ifndef _ATOMIC_H
#define _ATOMIC_H
/*===============================================================
 *                      Atomic.h
 *                     ԭ�������� ����
 *                     								��Ҷ-��������
 ===============================================================*/
#include <lib/include/type.h>

typedef struct atomic_t
{
	volatile int value;
}Atomic;

/* �� */
static inline int atomic_read(Atomic *data)
{
	return data->value;
}

/* д */
static inline void atomic_write(Atomic *data,int value)
{
	data->value = value;
	return;
}

/* �� */
static inline void atomic_add(Atomic *data,int value)
{
	__asm volatile("addl %1,%0"
		             : "+m" (data->value)
		             : "r" (value));
}

/* �� */
static inline void atomic_Sub(Atomic *data,int value)
{
	__asm volatile("subl %1,%0"
			             : "+m" (data->value)
			             : "r" (value));
}


/* �Լӻ��Լ����� */
static inline void atomic_inc(Atomic *data)
{
	asm volatile("incl %0"
					: "+m" (data->value));
}
static inline void atomic_dec(Atomic *data)
{
	asm volatile("decl %0"
					: "+m" (data->value));
}


/* �ӻ���󣬷�����ֵ */
static inline int atomic_add_ret(Atomic *data,int value)
{
	atomic_add(data,value);
	return data->value;
}

static inline int atomic_sub_ret(Atomic *data,int value)
{
	atomic_sub(data,value);
	return data->value;
}

static inline int atomic_inc_ret(Atomic *data)
{
	atomic_inc(data);
	return data->value;
}
static inline int atomic_dec_ret(Atomic *data)
{
	atomic_dec(data);
	return data->value;
}


/* λͼ���� */
static inline void atomic_set_bit(Atomic *data,u8 bit)
{
	int mask = 1 << bit;
	asm volatile("orl %0,%1"
				:
				: "r" (mask), "m" (data->value)
				: "memory");
}

static inline void atomic_clear_bit(Atomic *data,u8 bit)
{
	int mask = ~(1 << bit);
	asm volatile("andl %0,%1"
				:
				: "r" (mask), "m" (data->value)
				: "memory");
}

static inline bool atomic_test_bit(Atomic *data,u8 bit)
{
	int mask = 1 << bit;
	asm volatile("andl %0,%1"
				:
				: "r" (data->value),"m" (mask)
				: "memory");
}

#endif
