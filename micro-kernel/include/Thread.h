#ifndef THREAD_H_
#define THREAD_H_
/*===============================================================
 *                      Thread.h
 *                     	内核线程 定义
 *                     								绿叶-柳暗花明
 ===============================================================*/
#include "DataStruct.h"


/*===========线程上下文寄存器结构=============*/
typedef struct stack_frame {
	/* 单个压入 */
	u32	gs;
	u32	fs;
	u32	es;
	u32	ds;
	/* pushal压入*/
	u32	edi;
	u32	esi;
	u32	ebp;
	u32	kernel_esp;
	u32	ebx;
	u32	edx;
	u32	ecx;
	u32	eax;
	/* 某某压入 */
	u32	retaddr;
	/* 系统压入 */
	u32	eip;
	u32	cs;
	u32	eflags;
	u32	esp;
	u32	ss;
}StackFrame;
#define REGFRAME_HIGH 	&Thread::run_thread->thread_info.reg.eax
#define REGFRAME_LOW 	&Thread::run_thread->thread_info.reg.gs

/*==============线程描述符================*/
struct kernel_thread
{
	u16 id;			/* 线程ID */
	u16 state;		/* 状态信息 */
	u32 rec_des;	/* 占有资源描述符 */
	StackFrame reg;	/* 寄存器值 */
};
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_READY 2
#define THREAD_STATE_SLEEPING 4


/*==============线程栈==================*/
typedef union thread_union
{
	struct kernel_thread thread_info;
	u32 stack[1024];		/* 4k的栈 */
}KernelThread;
#define NR_THREAD 32


/*==============Thread命名空间==================*/
namespace Thread
{
/*************************线程状态控制函数**********************************/
	/* 创建线程
	 * 分配一个线程描述符
	 * 初始化线程描述符
	 * 并在就绪线程链表中添加一个项
	 * 函数地址和参数压入线程的栈
	 */
	Result Create(ThreadFun fun,point params);

	/* 睡眠线程
	 * 如果处于运行态,保存上下文,（通过调用Save()函数）
	 * 把线程的状态设为睡眠态
	 * 从就绪或运行队列中移除
	 * 加入睡眠队列 */
	Result Sleep(u16 id);

	/* 唤醒线程
	 * 把线程的状态设为就绪态
	 * 从睡眠队列中移除
	 * 加入就绪队列 */
	Result Wake(u16 id);

	/* 杀死线程
	 * 把线程从任何一个队列中移除
	 * 释放线程占有的任何资源(发送广播消息) */
	Result Kill(u16 id);


/***************************仅针对于run_thread***************************/

	/* 保存run_thread的通用寄存器和部分段寄存器到线程栈 */
	inline void Save();

	/* 恢复run_thread的上下文，并把控制权交给线程 */
	inline void Restart();

	/* 调度算法,选择一个线程作为run_thread */
	void Schedule();

/*******************************数据区***********************************/
	typedef ShareLinkList<KernelThread*,NR_THREAD> ThreadList;

	extern u32 kernel_esp;
	extern KernelThread* run_thread;
	extern ThreadList ready_thread;
	extern ThreadList sleep_thread;
};

/************************************************************************/
/*                      保存寄存器函数
/*                         Save
/************************************************************************/
inline void Thread::Save()
{
	/* 保存原esp的值到kernel_esp变量中 */
	asm volatile("movl %%esp,%0"
				:"+m"(Thread::kernel_esp)
				);

	/* esp指向 线程栈中保存通用寄存器部分的最高地址 */
	asm volatile("movl %0,%%esp"
				:
				:"m"(RUNTHREAD_REG_HIGH)
				);

	/*保存通用寄存器和一些段寄存器*/
	asm volatile("cld 		\n"
				"pushal		\n"
				"push %ds	\n"
				"push %es	\n"
				"push %fs	\n"
				"push %gs	\n"
				);

	/* 恢复esp */
	asm volatile("movl %0,%%esp"
				:
				: "m"(Thread::kernel_esp)
				);
	return;
}
/************************************************************************/
/*                      恢复寄存器函数
/*                        Restart
/************************************************************************/
inline void Thread::Restart()
{
	/* 保存原esp的值到kernel_esp变量中 */
	asm volatile("movl %%esp,%0"
				:"+m"(Thread::kernel_esp)
				);

	/* esp指向 线程栈中保存通用寄存器部分的最低地址 */
	asm volatile("movl %0,%%esp"
				:
				:"m"(REGFRAME_LOW)
				);

	/*恢复通用寄存器和一些段寄存器*/
	asm volatile("cld 			\n"
				"pop %gs		\n"
				"pop %fs		\n"
				"pop %es		\n"
				"pop %ds		\n"
				"popal			\n"
				"addl $4,%esp	\n"
				"iretd"
				);
}
#endif /* KERNEL_H_ */
