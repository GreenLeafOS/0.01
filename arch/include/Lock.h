#ifndef LOCK_H_
#define LOCK_H_
/*===============================================================
 *                     	  Lock.h
 *                    ԭ�Ӳ�������غ� ����
 ===============================================================*/

/* ������ǰ���ж�״̬ */
#define Kernel_Store_Flags(eflags)	\
	unsigned int eflags;		\
	asm volatile( 				\
		"pushf\n\t"				\
		"pop %0\n\t"			\
		: "+m"(eflags)			\
		:						\
		: "memory"				\
        );

/* ���ж� */
#define Kernel_Lock() 			\
		asm volatile("cli");

/* ���ж� */
#define Kernel_Unlock()			\
		asm volatile("sti");

/* �ָ���ǰ���ж�״̬ */
#define Kernel_Recovery_Flags(eflags) \
	asm volatile( 				\
		"pushl %0\n\t"			\
		"popf\n\t"				\
		: 						\
		: "m"(eflags)			\
		: "memory"				\
        );

//#endif
