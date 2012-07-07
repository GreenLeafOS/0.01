#ifndef LOCK_H_
#define LOCK_H_
/*===============================================================
 *                     	  Lock.h
 *                    原子操作锁相关宏 定义
 ===============================================================*/

/* 保存以前的中断状态 */
#define Kernel_Store_Flags(eflags)	\
	unsigned int eflags;		\
	asm volatile( 				\
		"pushf\n\t"				\
		"pop %0\n\t"			\
		: "+m"(eflags)			\
		:						\
		: "memory"				\
        );

/* 关中断 */
#define Kernel_Lock() 			\
		asm volatile("cli");

/* 开中断 */
#define Kernel_Unlock()			\
		asm volatile("sti");

/* 恢复以前的中断状态 */
#define Kernel_Recovery_Flags(eflags) \
	asm volatile( 				\
		"pushl %0\n\t"			\
		"popf\n\t"				\
		: 						\
		: "m"(eflags)			\
		: "memory"				\
        );

//#endif
